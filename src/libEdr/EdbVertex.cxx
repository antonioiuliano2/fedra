//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertex                                                            //
//                                                                      //
// Class for vertex reconstruction                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream.h>

#include "TFile.h"
#include "TRandom.h"
#include "TPolyLine.h"

#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "smatrix/Functions.hh"
#include "smatrix/SVector.hh"

#include "EdbVertex.h"
#include "EdbPhys.h"

ClassImp(EdbVertex);
ClassImp(EdbVertexRec);

using namespace MATRIX;
using namespace VERTEX;

//______________________________________________________________________________
EdbVertex::EdbVertex()
{
  eEdbTracks = 0;
}

//________________________________________________________________________
EdbVertex::~EdbVertex()
{
  if(eEdbTracks) delete eEdbTracks;
  eEdbTracks=0;
}

//________________________________________________________________________
//________________________________________________________________________
EdbVertexRec::EdbVertexRec()
{
  eEdbTracks = new TObjArray(8000);
}

//----------------------------------------------------------------------------
TTree *EdbVertexRec::init_tracks_tree(const char *file_name, EdbTrackP *track)
{
  TTree *tracks=0;
  /*
  char *tree_name="tracks";
  TFile *f = new TFile(file_name);

  if (f)  tracks = (TTree*)f->Get(tree_name);
  if(!tracks) return 0;
  f->cd();

  EdbSegP *tr = (EdbSegP*)track;
  TClonesArray *segments  = track->S()->GetSegments();
  TClonesArray *segmentsf = track->SF()->GetSegments();

  tracks->SetBranchAddress("t.",&tr);
  tracks->SetBranchAddress("s" ,&segments);
  tracks->SetBranchAddress("sf",&segmentsf);
  */
  return tracks;
}

//________________________________________________________________________
double EdbVertexRec::Tdistance(const Track& t1, const Track& t2) {
  //
  // geometrical distance between 2 track lines in the space XYZ
  //
  const double a  = t1.tx();
  const double b  = t1.ty();
  const double c  = 1.;
  const double a1 = t2.tx();
  const double b1 = t2.ty();
  const double c1 = 1.;

  const double det = square(a*b1 - b*a1) + square(b*c1 - c*b1) + square(c*a1 - a*c1);
  const SVector<double,3> dx = t2.xvec() - t1.xvec();
  // are tracks parallel?
  if(det==0) return mag(cross(dx,t1.evec()));

  const double det2 = dx[0]*(b*c1 - c*b1) + dx[1]*(c*a1 - a*c1) + dx[2]*(a*b1 - b*a1);
  
  return fabs(det2/sqrt(det));
}

//________________________________________________________________________
double EdbVertexRec::Tdistance(const EdbSegP &s1, const EdbSegP &s2) {
  Track t1;
  Track t2;
  if(!Edb2Vt(s1,t1))  return 1.E20;
  if(!Edb2Vt(s2,t2))  return 1.E20;
  return Tdistance(t1,t2);
}

//________________________________________________________________________
int EdbVertexRec::BuildTracksArr(const char *file_name, int nsegMin )
{
  // read tracks from the "linked_tracks" tree and fill TObjArray

  EdbTrackP *track  = new EdbTrackP(8);
  TTree     *tracks = init_tracks_tree(file_name, track);

  EdbTrackP *track1=0;
  int ngood = 0;

  int nentr = (int)(tracks->GetEntries());

  for(int i=0; i<nentr; i++) {

    tracks->GetEntry(i);
    track1 = new EdbTrackP(*track);
    if (track1->N() < nsegMin ) continue;
    eEdbTracks->Add( (TObject *)track1 );

    ngood++;
  }

  eEdbTracks->Expand(ngood);
  return ngood;
}

//________________________________________________________________________
EdbTrackP *EdbVertexRec::GetEdbTrack(const int index)
{
  if (!eEdbTracks) return 0;
  if (index < 0)   return 0;
  if (index > eEdbTracks->GetLast()) return 0;
  return     (EdbTrackP *)eEdbTracks->At(index);
}

//  Track& EdbVertexRec::GetVtTrack(const int index, Track& t)
//  {
//      if (!pVtTracks) return t;
//      if (index < 0) return t;
//      if (index > pVtTracks->GetLast()) return t;
//      t=(*(Track *)pVtTracks->At(index));
//  //    cout << "Track #" << index << ", Momentum is " << t.p() << endl;
//      return t;
//  }

//________________________________________________________________________
bool EdbVertexRec::Edb2Vt(const EdbTrackP& tr, Track& t)
{
  return Edb2Vt( *((EdbSegP*)&tr), t);
}

//________________________________________________________________________
bool EdbVertexRec::Edb2Vt(const EdbSegP& tr, Track& t)
{
    double x1 = (double)tr.X();
    double y1 = (double)tr.Y();
    double z1 = (double)tr.Z();
    double tx1 = (double)tr.TX();
    double ty1 = (double)tr.TY();
    double p1 = (double)tr.P();

    CMatrix cov1;             // covariance matrix for seg0
    for(int k=0; k<5; k++) 
      for(int l=0; l<5; l++) cov1(k,l) = (tr.COV())(k,l);

    t.set(x1,  y1,  z1,  tx1,  ty1, p1, cov1);
    t.propagate(0.);
    return true;
}

//________________________________________________________________________
double EdbVertexRec::ProbeSeg(const EdbTrackP *tr1, EdbTrackP *tr2, const float X0)
{
  return ProbeSeg( (EdbSegP*)tr1, (EdbSegP*)tr2, X0, tr1->M() );
}

//________________________________________________________________________
double EdbVertexRec::ProbeSeg(const EdbTrackP *tr, EdbSegP *s,
				   const float X0)
{
  return ProbeSeg( (EdbSegP*)tr, s, X0, tr->M() );
}

//________________________________________________________________________
double EdbVertexRec::ProbeSeg(const EdbSegP *tr, EdbSegP *s,
			      const float X0, const float ma)
{
  // Return value:        Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept couples with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good couple
  //
  // The mass and momentum of the tr are used for multiple scattering estimation

  float  ds;        // distance in the media for MS estimation
  double teta0sq;
  double dz;

  VtVector par( (double)(tr->X()), 
		(double)(tr->Y()),  
		(double)(tr->TX()), 
		(double)(tr->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (tr->COV())(k,l);

  Double_t chi2=0.; 


  VtSymMatrix dms(4);   // multiple scattering matrix (depends on P,m)
  dms.clear();

  dz = s->Z()-tr->Z();
  ds = dz*TMath::Sqrt(1.+par(2)*par(2)+par(3)*par(3)); // thickness of media in microns
  teta0sq = EdbPhysics::ThetaMS2( tr->P(), ma, ds, X0 );

  dms(0,0) = teta0sq*dz*dz/3.;
  dms(1,1) = dms(0,0);
  dms(2,2) = teta0sq;
  dms(3,3) = dms(2,2);
  dms(2,0) = teta0sq*dz/2.;
  dms(3,1) = dms(2,0);
  dms(0,2) = dms(2,0);
  dms(1,3) = dms(2,0);

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();

  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;

  VtVector parpred(4);            // prediction from seg0 to seg
  parpred = pred*par;
  
  VtSymMatrix covpred(4);         // covariance matrix for prediction
  covpred = pred*(cov*pred.T())+dms;

  VtSymMatrix dmeas(4);           // original covariance  matrix for seg2
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) dmeas(k,l) = (s->COV())(k,l);
  
  covpred = covpred.dsinv();
  dmeas   = dmeas.dsinv();
  cov = covpred + dmeas;
  cov = cov.dsinv();
  
  VtVector meas( (double)(s->X()), 
		 (double)(s->Y()),  
		 (double)(s->TX()), 
		 (double)(s->TY()) );

  par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

  chi2 = (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

  return TMath::Prob(chi2,4);
}

//________________________________________________________________________
bool EdbVertexRec::AttachSeg( EdbTrackP& tr, EdbSegP *s,
			      const float X0, const float ProbMin, float &prob )
{
  // Return value:        Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept couples with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good couple
  //
  // The mass and momentum of the tr are used for multiple scattering estimation

  float  ds;        // distance in the media for MS estimation
  double teta0sq;
  double dz;

  VtVector par( (double)(tr.X()), 
		(double)(tr.Y()),  
		(double)(tr.TX()), 
		(double)(tr.TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (tr.COV())(k,l);

  Double_t chi2=0.; 


  VtSymMatrix dms(4);   // multiple scattering matrix (depends on P,m)
  dms.clear();

  dz = s->Z()-tr.Z();
  ds = dz*TMath::Sqrt(1.+par(2)*par(2)+par(3)*par(3)); // thickness of media in microns
  teta0sq = EdbPhysics::ThetaMS2( tr.P(), tr.M(), ds, X0 );

  dms(0,0) = teta0sq*dz*dz/3.;
  dms(1,1) = dms(0,0);
  dms(2,2) = teta0sq;
  dms(3,3) = dms(2,2);
  dms(2,0) = teta0sq*dz/2.;
  dms(3,1) = dms(2,0);
  dms(0,2) = dms(2,0);
  dms(1,3) = dms(2,0);

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();

  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;

  VtVector parpred(4);            // prediction from seg0 to seg
  parpred = pred*par;
  
  VtSymMatrix covpred(4);         // covariance matrix for prediction
  covpred = pred*(cov*pred.T())+dms;

  VtSymMatrix dmeas(4);           // original covariance  matrix for seg2
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) dmeas(k,l) = (s->COV())(k,l);
  
  covpred = covpred.dsinv();
  dmeas   = dmeas.dsinv();
  cov = covpred + dmeas;
  cov = cov.dsinv();
  
  VtVector meas( (double)(s->X()), 
		 (double)(s->Y()),  
		 (double)(s->TX()), 
		 (double)(s->TY()) );

  par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

  chi2 = (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

  prob = (float)TMath::Prob(chi2,4);

  if (prob >= ProbMin)
  {
    tr.Set(tr.ID(),(float)par(0),(float)par(1),(float)par(2),(float)par(3),tr.W(),tr.Flag());
    tr.SetCOV( cov.array(), 4 );
    tr.SetChi2((float)chi2);
    tr.SetProb(prob);
    tr.SetZ(s->Z());
    tr.SetW(tr.N()+1);

    tr.AddSegment(s);

    return true;
  }
  return false;
}

//________________________________________________________________________
float EdbVertexRec::Chi2Seg( EdbSegP *tr, EdbSegP *s)
{
  // Return value:        Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept couples with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good couple
  //
  // The mass and momentum of the tr are used for multiple scattering estimation

  double dz;
  float prob;

  VtVector par( (double)(tr->X()), 
		(double)(tr->Y()),  
		(double)(tr->TX()), 
		(double)(tr->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (tr->COV())(k,l);

  Double_t chi2=0.; 


  dz = s->Z()-tr->Z();

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();

  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;

  VtVector parpred(4);            // prediction from seg0 to seg
  parpred = pred*par;
  
  VtSymMatrix covpred(4);         // covariance matrix for prediction
  covpred = pred*(cov*pred.T());

  VtSymMatrix dmeas(4);           // original covariance  matrix for seg2
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) dmeas(k,l) = (s->COV())(k,l);
  
  covpred = covpred.dsinv();
  dmeas   = dmeas.dsinv();
  cov = covpred + dmeas;
  cov = cov.dsinv();
  
  VtVector meas( (double)(s->X()), 
		 (double)(s->Y()),  
		 (double)(s->TX()), 
		 (double)(s->TY()) );

  par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

  chi2 = (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

  prob = (float)TMath::Prob(chi2,4);

  tr->Set(tr->ID(),(float)par(0),(float)par(1),(float)par(2),(float)par(3),tr->W(),tr->Flag());
  tr->SetCOV( cov.array(), 4 );
  tr->SetChi2((float)chi2);
  tr->SetProb(prob);
  tr->SetZ(s->Z());
  tr->SetW(tr->W()+s->W());

  return TMath::Sqrt(chi2/4.);
}

//______________________________________________________________________________
void  EdbVertexRec::TrackMC( EdbPatternsVolume &pv, float zlim[2],
			     float lim[4], float sigma[4], 
			     EdbTrackP &tr)
{
  // Segments generation for single MC track with taking into account MS and 
  //  XY errors correllation

  // Input parameters: gen  - the volume to be filled by segments (tracks)
  //                   lim  - XY limits
  //                   tr   - segments generated according to track parameters
  //
  // Output:  pv - fill with segments
  //          tr - add generated segments to the tr

  Float_t x0  =tr.X();
  Float_t y0  =tr.Y();
  Float_t z0  =tr.Z();
  Float_t tx0 =tr.TX();
  Float_t ty0 =tr.TY();
  Float_t p0  =tr.P();
  Float_t m   =tr.M();
  Float_t sx=sigma[0], sy=sigma[1], stx=sigma[2], sty=sigma[3];
  Float_t x,y,z,tx,ty, xs, ys, txs, tys;
  Float_t dz, dzm, cost, r1, r2, teta0, zold;
  Float_t dx = 0., dy = 0., dtx = 0., dty = 0.;
  Float_t dxs = 0., dys = 0., dtxs = 0., dtys = 0.;
  Double_t Phi,CPhi,SPhi;

  EdbPattern *pat = 0;
  EdbSegP    *seg = new EdbSegP();

  if ( x0 < lim[0] ) return; 
  if ( y0 < lim[1] ) return; 
  if ( x0 > lim[2] ) return; 
  if ( y0 > lim[3] ) return;
  x = x0;
  y = y0;
  tx = tx0;
  ty = ty0;
  zold = z0;

  for (int k=0; k<pv.Npatterns(); k++ ) {

    pat = pv.GetPattern(k);
    z = pat->Z();

    if ( z < zlim[0] ) { continue; }
    if ( z > zlim[1] ) { continue; }
    if ( z < zold    ) { continue; }

    dz = z - zold;
    cost = TMath::Sqrt((double)1.+(double)tx*(double)tx+(double)ty*(double)ty);
    if (cost <= 0.001) break;
    dzm = dz*cost;
    if (k)
      {
	teta0 = EdbPhysics::ThetaMS2( p0, m, dzm, EdbPhysics::kX0_Cell);
	teta0 = TMath::Sqrt(teta0);
	r1 = gRandom->Gaus();
	r2 = gRandom->Gaus();
	dx = (0.5*r1+0.866025*r2)*dzm*teta0/1.73205;
	dtx = r2*teta0;
	r1 = gRandom->Gaus();
	r2 = gRandom->Gaus();
	dy = (0.5*r1+0.866025*r2)*dzm*teta0/1.73205;
	dty = r2*teta0;
      }
    else
      {
	dx = 0.;
	dy = 0.;
	dtx = 0.;
	dty = 0.;
      }
    x = x + tx*dz + dx;
    y = y + ty*dz + dy;
    tx = tx + dtx;
    ty = ty + dty;
    zold = z;
    if ( x < lim[0] ) break; 
    if ( y < lim[1] ) break; 
    if ( x > lim[2] ) break; 
    if ( y > lim[3] ) break;
    dxs = sx*gRandom->Gaus();
    dys = sy*gRandom->Gaus();
    dtxs = stx*gRandom->Gaus();
    dtys = sty*gRandom->Gaus();
    Phi = -TMath::ATan2(ty,tx);          // azimuthal angle of the track
    CPhi = TMath::Cos(Phi);
    SPhi = TMath::Sin(Phi);
    xs = x + dxs*CPhi - dys*SPhi;        // smearing of track parameters with corellation 
    ys = y + dxs*SPhi + dys*CPhi;        // along the track projection to XY plane 
    txs = tx + dtxs*CPhi - dtys*SPhi;    // (regression line)
    tys = ty + dtys*SPhi + dtys*CPhi;
    seg->Set(k, xs, ys, txs, tys,25,0);
    seg->SetP(p0);
    seg->SetZ(z);
    seg->SetDZ(300.);
    seg->SetPID(k);
    seg->SetFlag(tr.Flag());
    seg->SetErrorsCOV(sx*sx, sy*sy, 0., stx*stx, sty*sty, 0.);

    tr.AddSegment(     pat->AddSegment(*seg) );
  }
}

//______________________________________________________________________________
float  EdbVertexRec::P_MS( EdbTrackP &tr,  float X0, bool draw)
{
  // momentum estimation by multiple scattering

  int    nms[60];
  double xms[60];
  double yms[60];
  double sxms[60];
  double syms[60];
  for(int i=0; i<60; i++) {
    nms[i]=0;
    xms[i]=0;
    yms[i]=0;
    sxms[i]=0;
    syms[i]=0;
  }

  EdbSegP *s1=0,*s2=0;

  double dx,dy,dz,ds;
  double dtx,dty,dts;

  int nseg=tr.N();
  for(int ist=1; ist<nseg; ist++) {     // step size

    int ii=0;
    while(ist*(ii+1)<nseg) {            // steps

      //for(int i1=0; i1<ist; i1++) {     // for each step and each segment
      for(int i1=0; i1<1; i1++) {       // for each step just once
	int istart = ii*ist+i1;
	int iend   = istart+ist;
	if(iend >= nseg-1)            break;
	s1 = tr.GetSegment(istart);
	s2 = tr.GetSegment(iend);

	dx = s2->X()-s1->X();
	dy = s2->Y()-s1->Y();
	dz = s2->Z()-s1->Z();
	ds = (TMath::Sqrt(dx*dx+dy*dy+dz*dz));

	dtx = s2->TX()-s1->TX();
	dty = s2->TY()-s1->TY();
	dts = TMath::Sqrt(dtx*dtx+dty*dty);

	xms[ist-1] += TMath::Sqrt(ds);
	yms[ist-1] += dts;
	nms[ist-1]++;
      }
      ii++;
    }
  }

  double K=0,W=0,w=0;
  int ist=0;
  //while( nms[ist]>ist ) {
  while( nms[ist]>0 ) {
    xms[ist] /=nms[ist];
    yms[ist] /=nms[ist];
    w = TMath::Sqrt((double)nms[ist]);
    float expected = TMath::Sqrt(2.*EdbPhysics::ThetaPb2( tr.P(),tr.M(), xms[ist]*xms[ist]));
    syms[ist] = expected/w;
    //syms[ist] = yms[ist]/w;
    //printf("%d %d \t %f \t %f\n",ist,nms[ist],xms[ist],yms[ist] );
    K += w*yms[ist]/xms[ist];
    W += w;
    ist++;
  }
  K /= W;
  printf("K = %f\n",K);

  if(draw) {
    TGraphErrors *gr = new TGraphErrors(ist,xms,yms,sxms,syms);
    gr->Draw("ALP");
    gr->Fit("pol1");

    double *x = new double[ist];
    double *y = new double[ist];

    for(int i=0; i<ist; i++) {
      x[i] = xms[i];
      y[i] = TMath::Sqrt(2.*EdbPhysics::ThetaPb2( tr.P(),tr.M(), x[i]*x[i]));
    }
    TPolyLine *line = new TPolyLine(ist,x,y);
    line->SetFillColor(38);
    line->SetLineColor(2);
    line->SetLineWidth(2);
    line->Draw("f");
    line->Draw();

    float Kteor=(y[ist]-y[0])/(x[ist]-x[0]);
    printf("Kteor = %f  k = %f\n",Kteor,Kteor*tr.P());
  }


  //TODO: tune coefficients !

  double p=0;
  double k = 1.4/X0;

  //double m2 = tr.M()*tr.M();
  //p = (k+TMath::Sqrt(k*k+4.*K*k*m2))/K;

  p = k/K;
  return p;
}
//______________________________________________________________________________
