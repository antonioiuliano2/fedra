//-- Author :  Valeri Tioukov  8.10.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TF1.h"
#include "TArrayF.h"
#include "TGraphErrors.h"

#include "EdbPhys.h"
#include "EdbAffine.h"
#include "EdbLayer.h"
#include "EdbTrackFitter.h"

#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "vt++/VtDistance.hh"
#include "smatrix/Functions.hh"
#include "smatrix/SVector.hh"

ClassImp(EdbTrackFitter);

using namespace TMath;
using namespace MATRIX;
using namespace VERTEX;

//______________________________________________________________________________
EdbTrackFitter::EdbTrackFitter()
{
  SetDefaultBrick();
}

//______________________________________________________________________________
void EdbTrackFitter::SetDefaultBrick()
{
  eX0            = EdbPhysics::kX0_Pb();
  eTPb           = 1000./1300.;
  ePcut          = 0.050;
  eM             = 0.13957;
  eDE_correction = false;

  eNsegMax=160;
}

//______________________________________________________________________________
void EdbTrackFitter::Print()
{
  printf("EdbTrackFitter seetings:\n");
  printf("eNsegMax = %d\n",eNsegMax);
  printf("eX0      = %f\n",eX0);
  printf("eM       = %f\n",eM);
  printf("\n");
}

//________________________________________________________________________
float EdbTrackFitter::Chi2Seg( EdbSegP *tr, EdbSegP *s)
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
float EdbTrackFitter::Chi2SegM( EdbSegP s1, EdbSegP s2, EdbSegP &s, EdbScanCond &cond1, EdbScanCond &cond2)
{
  // full estimation of chi2 without covariance matrix - the result seems to be identical to Chi2Seg 
  // VT: 19-Sep-2007
  //
  // Input: 2 segments passed by value because them will be modified during calculations
  // Return value - the result of the fit - passed by value
  // TODO: remove in this function the dependency of the COV

  // 1) calcualte the mean direction vector for the 2-seg group

  float dz = s2.Z() - s1.Z(); 
  float tbx=0, tby=0, wbx=0, wby=0;
  if(Abs(dz) > 0.1 ) {
    tbx = (s2.X() - s1.X())/(s2.Z() - s1.Z());
    tby = (s2.Y() - s1.Y())/(s2.Z() - s1.Z());
    wbx = Sqrt(s1.SX() + s2.SX())/dz;                  
    wby = Sqrt(s1.SY() + s2.SY())/dz;
  }
  float w1x = 1./s1.STX();
  float w1y = 1./s1.STY();
  float w2x = 1./s2.STX();
  float w2y = 1./s2.STY();

  float TX = (s1.TX()*w1x + s2.TX()*w2x + tbx*wbx)/(w1x+w2x+wbx);
  float TY = (s1.TY()*w1y + s2.TY()*w2y + tby*wby)/(w1y+w2y+wby);

  // 2) calcualte the COG of the 2-seg group

  w1x = 1./s1.SX();  w1y = 1./s1.SY();
  w2x = 1./s2.SX();  w2y = 1./s2.SY();
  float Z = (s1.Z()*(w1x+w1y) + s2.Z()*(w2x+w2y))/(w1x+w1y+w2x+w2y);
  float X = (s1.X()*(w1x+w1y) + s2.X()*(w2x+w2y))/(w1x+w1y+w2x+w2y);
  float Y = (s1.Y()*(w1x+w1y) + s2.Y()*(w2x+w2y))/(w1x+w1y+w2x+w2y);
  //printf("COG: x %f  y %f z %f\n",X,Y,Z);

  s.SetX(X);
  s.SetY(Y);
  s.SetTX(TX);
  s.SetTY(TY);
  s.SetW(s1.W()+s2.W());
  s.SetZ(Z);

  float PHI = ATan2(TY,TX);   // angle of the 2-seg group plane
  float T = Sqrt(TX*TX+TY*TY);

  EdbAffine2D aff;
  aff.Rotate(PHI);
  //aff.ShiftX(X);
  //aff.ShiftY(Y);
  aff.Invert();
  s1.Transform(&aff);
  s2.Transform(&aff);
  s.Transform(&aff);

  float stx1   = cond1.SigmaTX(T), sty1   = cond1.SigmaTY(0);
  float stx2   = cond2.SigmaTX(T), sty2   = cond2.SigmaTY(0);
  w1x = 1./(stx1*stx1);  w1y = 1./(sty1*sty1);
  w2x = 1./(stx2*stx2);  w2y = 1./(sty2*sty2);

  //printf("w1x %f w2x %f w1y %f w2y %f\n",w1x,w2x,w1y,w2y);
  float chi2t = TMath::Sqrt( ( (s1.TX()-T)*(s1.TX()-T)*w1x +
			       s1.TY()*s1.TY()        *w1y +
			       (s2.TX()-T)*(s2.TX()-T)*w2x +
			       s2.TY()*s2.TY()        *w2y )/4.
			     );
  //printf("angular component of chi2 = %f\n",chi2t);

  float sx1   = cond1.SigmaX(T), sy1   = cond1.SigmaY(0);
  float sx2   = cond2.SigmaX(T), sy2   = cond2.SigmaY(0);

  float dx1 = s1.X()-(s.X()+(s1.Z()-s.Z())*s.TX());  
  float dy1 = s1.Y()-(s.Y()+(s1.Z()-s.Z())*s.TY());  
  float dx2 = s2.X()-(s.X()+(s2.Z()-s.Z())*s.TX());  
  float dy2 = s2.Y()-(s.Y()+(s2.Z()-s.Z())*s.TY());  

  float chi2pos = Sqrt(dx1*dx1/sx1/sx1+dy1*dy1/sy1/sy1+dx2*dx2/sx2/sx2+dy2*dy2/sy2/sy2)/2.;
  //printf("position component of chi2 = %f\n",chi2pos);
  s.SetChi2( Sqrt(chi2t*chi2t+chi2pos*chi2pos));
  aff.Invert();
  s.Transform(&aff);
  //s.Print();

  return s.Chi2();
}

//______________________________________________________________________________
float EdbTrackFitter::MaxChi2Seg(EdbTrackP &t)
{
  // return the maximal seg-to-seg chi2 along the track
  float chimax=0,chi=0;
  if(t.N()<2) return chimax;
  for(int i=0; i<t.N()-1; i++) {
    EdbSegP s(*t.GetSegment(i));
    chi =  Chi2Seg(&s,t.GetSegment(i+1));
    chimax = chi > chimax ? chi : chimax;
  }
  return chimax;
}

//______________________________________________________________________________
float EdbTrackFitter::MeanChi2Seg(EdbTrackP &t)
{
  float meanchi=0;
  if(t.N()<2) return meanchi;
  for(int i=0; i<t.N()-1; i++) {
    EdbSegP s(*t.GetSegment(i));
    meanchi +=  Chi2Seg(&s,t.GetSegment(i+1));
  }
  return meanchi /= (t.N()-1);
}


//______________________________________________________________________________
float EdbTrackFitter::MaxKink(EdbTrackP &t)
{
  // return the maximal seg-to-seg kink theta angle
  float kink=0,theta=0;
  if(t.N()<2) return kink;
  for(int i=0; i<t.N()-1; i++) {
    theta =  Theta(*t.GetSegment(i),*t.GetSegment(i+1));
    kink = theta > kink ? theta : kink;
  }
  return kink;
}

//______________________________________________________________________________
float EdbTrackFitter::MeanKink(EdbTrackP &t)
{
  float meankink=0;
  if(t.N()<2) return 0;
  for(int i=0; i<t.N()-1; i++) 
    meankink +=  Theta(*t.GetSegment(i),*t.GetSegment(i+1));
  return meankink /= (t.N()-1);
}

//______________________________________________________________________________
float EdbTrackFitter::Theta( EdbSegP &s, EdbSegP &s1 )
{
  return Sqrt( (s.TX()-s1.TX())*(s.TX()-s1.TX()) + (s.TY()-s1.TY())*(s.TY()-s1.TY()) );
}

//______________________________________________________________________________
bool EdbTrackFitter::SplitTrack( EdbTrackP &t, EdbTrackP &t1, int isplit )
{
  // split track t in 2 at the point isplit - will be the first segment of of t1
  if(t.N()<isplit) return false;
  for(int i=t.N()-1; i>=isplit; i--) {
    t1.AddSegment(   t.GetSegment(i) );
    t.RemoveSegment( t.GetSegment(i) );
  }
  t.SetCounters();
  //t1.FitTrackKFS(true, X0, 0);
  t1.SetCounters();
  t1.SetM(t.M());
  t1.SetP(t.P());
  //t1.FitTrackKFS(true, X0, 0);

  return true;
}

//______________________________________________________________________________
int EdbTrackFitter::SplitTrackByKink( EdbTrackP *t, TObjArray &tracks, float maxkink )
{
  // split track t in several tracks accourding to maxkink
  // return total number of tracks after splitting; 
  // after splitting all new tracks added to the array "tracks"

  if(t->N()<1) return 0;
  if(t->N()<2) return 1;
  int nsplit=1;
  for(int i=t->N()-1; i>0; i--) 
    if( Theta(*t->GetSegment(i),*t->GetSegment(i-1)) >= maxkink ) {
      EdbTrackP *t1 = new EdbTrackP();
      SplitTrack(*t,*t1, i);
      tracks.Add(t1);
      nsplit++;
    }
  return nsplit;;
}

//______________________________________________________________________________
float EdbTrackFitter::PMS_KF(EdbTrackP &t, float p0, float probbest)
{
  // select track momentum in a way to have the given chi2-probablity calculated by KF

  if(t.N()<2) return 0;
  float prob=0;
  float pu=100., pl=0., p=p0;
  int nstep = 0;
  while( Abs(prob-probbest)>0.001 ) {
    nstep++;
    t.SetP(p);
    t.FitTrackKFS(true);
    prob = t.Prob();
    if(prob<probbest) pu=p;
    else pl=p;
    if(nstep>30) break;
    p = (pu+pl)/2.;
  }
  if(nstep>20) printf("Warning in EdbTrackFitter::PMS_KF: nstep=%d     nseg=%d  p=%f  prob=%f\n",nstep,t.N(),p,prob);
  return t.P();
}

//______________________________________________________________________________
int EdbTrackFitter::FitTrackLine(EdbTrackP &tr)
{
  // track fit by averaging of segments parameters
  
  int nseg=tr.N();
  float x=0,y=0,z=0,tx=0,ty=0,w=0;
  EdbSegP *seg=0;
  for(int i=0; i<nseg; i++) {
    seg = tr.GetSegment(i);
    x  += seg->X();
    y  += seg->Y();
    z  += seg->Z();
    tx += seg->TX();
    ty += seg->TY();
    w  += seg->W();
  }
  x  /= nseg;
  y  /= nseg;
  z  /= nseg;
  tx /= nseg;
  ty /= nseg;
  tr.Set(tr.ID(),x,y,tx,ty,w,tr.Flag());
  tr.SetZ(z);
  return nseg;
}

//________________________________________________________________________________________
float EdbTrackFitter::PMS_Mag(EdbTrackP &tr, float detheta)
{
  // Momentum resolution by Multiple scattering (Annecy implementation Sep-2006)
  // 
  // Input: tr      - track
  //        detheta - average basetrack Theta angle resolution. Must be evaluated before.
  //        x0 - the radiation length of the media [mm]. The deafault is lead.

  // TODO: take into account the angular dependance of detheta

  // The constant term im the scattering formula is not 13.6 but 14.64, which 
  // is the right reevaluated number, due to a calculation with the moliere 
  // distribution. 13.6 is an approximation. See Geant3 or 4 references for more explanations.

  float x0 = eX0/1000.;                 //rad length in [mm]
  detheta = (detheta*Sqrt(2.)/1000.);
  detheta *= detheta;
  float k = 14.64*14.64/x0;                              // approx 38.3
  TF1 *f1=new TF1("f1",Form("sqrt(%f*x/([0])**2+%f)",k,detheta),0,14);  // 0-14 the fitting range (cells)
  f1->SetParameter(0,4000.);                             // strating value for momentum in MeV

  EdbSegP *s;
  int nseg = tr.N();
  if(nseg<2)   return 0;
  int npl = tr.Npl();

  TArrayF theta2(eNsegMax),dtheta(eNsegMax),theta(eNsegMax);
  TArrayF thick(eNsegMax);
  TArrayF dx(eNsegMax);
  TArrayI sPID(eNsegMax);
  TArrayF setx(eNsegMax), sety(eNsegMax);
  double   P=0, dP=0;
  double   slopecorx=0, slopecory=0;
  int      lastci=0;
  
  for(int ci=0;ci<nseg;ci++){
    s=tr.GetSegment(ci);
    if(s!=NULL){
      sPID[ci]=s->PID(); 
      lastci=sPID[ci];
      setx[sPID[ci]]=s->TX();
      sety[sPID[ci]]=s->TY();
      slopecorx=slopecorx+s->TX();
      slopecory=slopecory+s->TY();    
    }
  }
  slopecorx=slopecorx/nseg;
  slopecory=slopecory/nseg;
  double cor=Sqrt(slopecorx*slopecorx+slopecory*slopecory);
  double Zeff=(cor*cor/Sqrt(1+cor*cor));

  for(int plate=0;plate<npl;plate++){         //??? 
    //setx[plate]=setx[plate+lastci];
    //sety[plate]=sety[plate+lastci];
  }
  
  int Ncell=0;
  for (int m=0;m<npl-1;m++) {
    theta2[m]=0;dtheta[m]=0;theta[m]=0;
    Ncell++;
    int dim=0;
    int nshift=Ncell-1;
    if(nshift>npl-Ncell-1){ nshift=npl-Ncell-1; }
    
    for(int j=0;j<nshift+1;j++){	     	
      double x = (npl-(j+1))/Ncell;
      int nmes = (int)x;
      int nhole=0;
      for (int i=0;i<nmes;i++){		   
	if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0)&&(sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0))
	  {
	    theta2[m] = 
	      theta2[m] + 
	      Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 ) +
	      Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
	    dim++;
	  }
	else nhole++; 	  	 
      }
     }//end loop on shifts            
    
    if (dim!=0){
      theta[m]=sqrt(theta2[m]/(2*dim));
      double errstat=theta[m]/Sqrt(2*dim);
      dtheta[m]=errstat;
    }
    else{ P=0.5; break;}
   
    thick[m]=Ncell*(Zeff+1);
    dx[m]=0.25;                // not important
    
  } //end loop on Ncell 

  TGraphErrors *ef1=new TGraphErrors(npl-1,thick.GetArray(),theta.GetArray(),dx.GetArray(),dtheta.GetArray());
  ef1->Fit("f1","MQ");
  P=f1->GetParameter(0);
  dP=f1->GetParError(0);
  P=Abs(P/1000.);

  delete ef1;
  delete f1;
  return (float)P;
}

//________________________________________________________________________________________
float EdbTrackFitter::P_MS(EdbTrackP &tr)
{
  // momentum estimation by multiple scattering (first version)

  int	 stepmax = 1;
  int    nms = 0;
  double tms = 0.;
  int ist = 0;

  float m;  // the mass of the particle
  eM<0? m = tr.M(): m=eM;

  EdbSegP *s1=0,*s2=0;

  double dx,dy,dz,ds;
  double dtx,dty,dts,fact,ax1,ax2,ay1,ay2,dax1,dax2,day1,day2;

  int nseg = tr.N(), i1 = 0, i2 = 0;

  for (ist=1; ist<=stepmax; ist++) {     // step size

      for (i1=0; i1<(nseg-ist); i1++) {       // for each step just once

	i2 = i1+ist;

	s1 = tr.GetSegment(i1);
	s2 = tr.GetSegment(i2);
	
	dx = s2->X()-s1->X();
	dy = s2->Y()-s1->Y();
	dz = s2->Z()-s1->Z();
	ds = Sqrt(dx*dx+dy*dy+dz*dz);
	
	ax1 = ATan(s1->TX());
	ax2 = ATan(s2->TX());
	ay1 = ATan(s1->TY());
	ay2 = ATan(s2->TY());
	dax1 = s1->STX();
	dax2 = s2->STX();
	day1 = s1->STY();
	day2 = s2->STY();
	dtx = (ax2-ax1);
	dty = (ay2-ay1);
	dts = dtx*dtx+dty*dty;
	fact = 1.+0.038*Log(ds/eX0);
	dts = (dts-dax1-dax2-day1-day2)/ds/fact/fact;
//	if (dts < 0.) dts = 0.;
	tms += dts;
	nms++;
      }
  }

  if(tms<=0) { 
	printf("P_MS: BAD estimation for track %d: tms=%g  nms=%d\n",tr.ID(),tms,nms);
	return 10;   // with correct parameters setting this problem is usually happend for hard tracks >=10 GeV
  }
  double pbeta = 0., pbeta2 = 0.;
  pbeta = Sqrt((double)nms/tms/eX0)*0.01923;
  pbeta2 = pbeta*pbeta;
  double p = 0.5*(pbeta2 + Sqrt(pbeta2*pbeta2 + 4.*pbeta2*m*m));
  if (p <= 0.)
    p = 0.;
  else
    p = Sqrt(p);
  
  if (eDE_correction)
  {
    double dtot = 0., eTPb = 1000./1300., e = 0., tkin = 0.;
    s1 = tr.GetSegment(0);
    s2 = tr.GetSegment(nseg-1);

    dx = s2->X()-s1->X();
    dy = s2->Y()-s1->Y();
    dz = s2->Z()-s1->Z();
    
    dtot = Sqrt(dx*dx+dy*dy+dz*dz)*eTPb;

    double DE = EdbPhysics::DeAveragePb(p, m, dtot);
    tkin = Sqrt(p*p + m*m) - m;

    if (tkin < DE)
    {
	tkin = 0.5*DE;
	e = tkin + m;
	p = Sqrt(e*e - m*m);
	DE = EdbPhysics::DeAveragePb(p, m, dtot);
    }
    tkin = tkin + 0.5*DE;
    e = tkin + m;
    p = Sqrt(e*e - m*m);
  }
  
  return (float)p;
}
