//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertex                                                            //
//                                                                      //
// Class for vertex reconstruction                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TRandom.h"

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
  eV = 0;
  eX = 0.;
  eY = 0.;
  eZ = 0.;
  eNtr = 0;
  eFlag = 0;
}

//______________________________________________________________________________
EdbVertex::EdbVertex(EdbVertex &v) : eEdbTracks(v.GetEdbTracks()), eZpos(v.eZpos)
{
  if(v.V()) eV = new VERTEX::Vertex(*(v.V()));
  else eV=0;
  eX = v.eX;
  eY = v.eY;
  eZ = v.eZ;
  eNtr = v.N();
  eFlag = v.Flag();
}

//________________________________________________________________________
EdbVertex::~EdbVertex()
{
  if(eV) { eV->clear(); delete eV; eV=0; }
}

//________________________________________________________________________
void EdbVertex::Clear()
{
  eEdbTracks.Clear();
  eZpos.Set(0);
  if(eV) { eV->clear(); delete eV; eV=0; }
  eX = 0.;
  eY = 0.;
  eZ = 0.;
  eNtr = 0;
  eFlag = 0;
}

//________________________________________________________________________
int EdbVertex::MakeV( bool usemom )
{
  if(eV) { eV->clear(); delete eV; eV=0; }
  eV = new Vertex();
  eV->use_kalman(true);               //TODO: define as parameter
  eV->use_momentum(usemom);
  EdbTrackP *tr=0;
  EdbSegP *seg=0;
  for(int i=0; i<eNtr; i++) {
    tr = (EdbTrackP*)(eEdbTracks.At(i));
    if (tr->NF() <= 0) return 0;
    if   (eZpos.At(i)) seg = (EdbSegP *)(tr->TrackZmin());
    else  	       seg = (EdbSegP *)(tr->TrackZmax());
    Track *t=new Track();
    if( Edb2Vt( *seg, *t ) )
    {
	t->rm((double)(tr->M()));
	eV->push_back(*t);
    }
    else { delete t; t=0;}
  }
  return eV->ntracks();
}
//________________________________________________________________________
void EdbVertex::SetTracksVertex( )
{
  EdbTrackP *tr=0;
  for(int i=0; i<eNtr; i++) {
    tr = (EdbTrackP*)(eEdbTracks.At(i));
    if ( !tr )
    {
	printf("EdbVertex::SetTracksVertex - null pointer for track %d!\n", i);
	printf("EdbVertex::SetTracksVertex - number of tracks %d!\n", eNtr);
	return; 
    }
    if   (eZpos.At(i)) tr->SetVertexS(this);
    else	       tr->SetVertexE(this);
  }
  return;
}
//________________________________________________________________________
bool EdbVertex::AddTrack(EdbTrackP *track, int zpos, float ProbMin )
{
    if (!track) return false;
    bool result = true;
    EdbSegP *seg = 0;
    if   (zpos)
    {
	seg = (EdbSegP *)(track->TrackZmin());
    }
    else
    {
	seg = (EdbSegP *)(track->TrackZmax());
    }
    if (eV)
    {
	if (track->NF() <= 0) return false;
	Track *t=new Track();
	if( Edb2Vt( *seg, *t ) )
	{
	    if (eV->valid())
	    {
		t->rm((double)(track->M()));
		eV->push_back(*t);
//		eV->use_kalman(true);
//		eV->use_momentum(true);
		if (!(eV->findVertexVt()))
		{
		    eV->remove_last();
		    delete t;
		    t=0;
		    result = false;
		    printf("EdbVertex::AddTrack - vertex not found!");
		}
		else if (!(eV->valid()))
		{
		    eV->remove_last();
		    delete t;
		    t=0;
		    result = false;
		    printf("EdbVertex::AddTrack - vertex not valid!");
		}
		else if (eV->prob() < ProbMin )
		{
		    eV->remove_last();
		    delete t;
		    t=0;
		    result = false;
		}
		else
		{
		    eEdbTracks.Add(track);
		    eZpos.Set(eNtr+1);
		    eZpos.AddAt(zpos, eNtr);
		    eNtr++;
		    if   (zpos)
		    {
			track->SetVertexS(this);
		    }
		    else
		    {
			track->SetVertexE(this);
		    }
		}
	    }
	}
	else
	{ 
	    delete t;
	    t=0;
	    result = false;
	    printf("EdbVertex::AddTrack - conversion to VT failed!");
	}
    }
    else
    {
	eEdbTracks.Add(track);
	eZpos.Set(eNtr+1);
	eZpos.AddAt(zpos, eNtr);
	eNtr++;
	if (track->NF() <= 0) return result;
	if   (zpos)
	{
	    track->SetVertexS(this);
	}
	else
	{
	    track->SetVertexE(this);
	}
	eX = ((eNtr-1)*eX + seg->X())/eNtr;
	eY = ((eNtr-1)*eY + seg->Y())/eNtr;
	eZ = ((eNtr-1)*eZ + seg->Z())/eNtr;
    }
    return result;
}
//________________________________________________________________________
bool EdbVertex::EstimateVertexMath( float& xv, float& yv, float& zv, float& d )
{
  int nt = eNtr; 
  if(!nt) return false;

  double tx_sum  = 0.;
  double x_sum   = 0.;
  double xw_sum  = 0.;
  double xtx_sum = 0.;
  double tx2_sum = 0.;
  double ty_sum  = 0.;
  double y_sum   = 0.;
  double yw_sum  = 0.;
  double yty_sum = 0.;
  double ty2_sum = 0.;
  const EdbTrackP   *tr = 0;
  const EdbSegP   *seg = 0;

  double x,y,tx,ty,xweight,yweight,xweight2,yweight2;

  // fill cumulants
  for( int i = 0; i < nt; i++ ) {
    
    tr = GetTrack(i);

    if (eZpos.At(i)) seg = tr->TrackZmin();
    else	     seg = tr->TrackZmax();

    x        = seg->X();
    y        = seg->Y();
    tx       = seg->TX();
    ty       = seg->TY();
    xweight  = 1./(seg->COV())(0,0);
    yweight  = 1./(seg->COV())(1,1);
    xweight2 = xweight*xweight;
    yweight2 = yweight*yweight;

    tx_sum  += tx * xweight;
    x_sum   += x  * xweight;
    xw_sum  += xweight;
    xtx_sum += x  * tx * xweight2;
    tx2_sum += tx * tx * xweight2;

    ty_sum  += ty * yweight;
    y_sum   += y  * yweight;
    yw_sum  += yweight;
    yty_sum += y  * ty * yweight2;
    ty2_sum += ty * ty * yweight2;

  } // for track

  double det = -tx2_sum - ty2_sum + tx_sum*tx_sum/xw_sum + ty_sum*ty_sum/yw_sum;

  if(det == 0.) {
    return false;
  }

  zv = ( xtx_sum + yty_sum - tx_sum*x_sum/xw_sum - ty_sum*y_sum/yw_sum ) / det;
  xv = ( x_sum + tx_sum * zv ) / xw_sum;
  yv = ( y_sum + ty_sum * zv ) / yw_sum;


  float zTolerance=300.;

  for( int i = 0; i < nt; i++ ) {
      tr = GetTrack(i);
      if (eZpos.At(i)) seg = tr->TrackZmin();
      else	       seg = tr->TrackZmax();
      if( zv > (seg->Z() + zTolerance) )  return false;
      if( zv < (seg->Z() - zTolerance) )  return false;
  }

  double drx;
  double dry;
  double drz;
  double drt;
  double drms = 0.;

  for( int i = 0; i < nt; i++ ) {

    tr = GetTrack(i);

    if (eZpos.At(i)) seg = tr->TrackZmin();
    else	     seg = tr->TrackZmax();
    
    drx = seg->X() - xv;
    dry = seg->Y() - yv;
    drz = seg->Z() - zv;
    drt = (drx*seg->TX() + dry*seg->TY() + drz);
    drms += drx*drx + dry*dry + drz*drz -
      (drt*drt)/(1.+seg->TX()*seg->TX()+seg->TY()*seg->TY());
  }

  d = TMath::Sqrt(drms/nt);

  return true;
}

//________________________________________________________________________
bool EdbVertex::Edb2Vt(const EdbTrackP& tr, Track& t)
{
  return Edb2Vt( *((EdbSegP*)&tr), t);
}

//________________________________________________________________________
bool EdbVertex::Edb2Vt(const EdbSegP& tr, Track& t)
{
    double x1 = (double)tr.X() - eX;
    double y1 = (double)tr.Y() - eY;
    double z1 = (double)tr.Z() - eZ;
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

  const EdbSegP *snear=0;
  if( TMath::Abs( s->Z() - tr.TrackZmin()->Z() ) < 
      TMath::Abs( s->Z() - tr.TrackZmax()->Z() ) )  snear = tr.TrackZmin();
  else snear = tr.TrackZmax();

  VtVector par( (double)(snear->X()), 
		(double)(snear->Y()),  
		(double)(snear->TX()), 
		(double)(snear->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (snear->COV())(k,l);

  Double_t chi2=0.; 


  VtSymMatrix dms(4);   // multiple scattering matrix (depends on P,m)
  dms.clear();

  dz = s->Z()-snear->Z();
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
    EdbSegP *sfnew = new EdbSegP( tr.ID(),
				  (float)par(0),(float)par(1),(float)par(2),(float)par(3),
				  tr.W()+1.,tr.Flag() );

    sfnew->SetCOV( cov.array(), 4 );
    sfnew->SetChi2((float)chi2);
    sfnew->SetProb(prob);
    sfnew->SetZ(s->Z());

    tr.AddSegment(s);
    tr.AddSegmentF(sfnew);

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
