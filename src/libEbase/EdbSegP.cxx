//-- Author :  Valeri Tioukov   19.05.2002
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegP                                                              //
//                                                                      //
// Segment                                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "EdbSegP.h"

ClassImp(EdbSegP)

using namespace MATRIX;
using namespace TMath;

//______________________________________________________________________________
EdbSegP:: EdbSegP(int id, float x, float y, float tx, float ty, float w, int flag)
{
  eEMULDigitArray =0;
  Set0();
  Set(id,x,y,tx,ty,w,flag);
}

//______________________________________________________________________________
void EdbSegP::Set0() 
{
  ePID=0;
  eID=0;
  eVid[0]=eVid[1]=0;
  eAid[0]=eAid[1]=0;
  eFlag=0;
  eTrack=-1;
  eX=eY=eZ=eTX=eTY=eSZ=0;
  eProb=0;
  eW=0;
  eVolume=0;
  eDZ=0;
  eP=-999.;
  eChi2=0;
  eCOV=0;
  eMCTrack=-999;
  eMCEvt=-999;
}

//______________________________________________________________________________
void EdbSegP::SetErrors( float sx2, float sy2, float sz2, float stx2, float sty2, float sp2 )
{ 
  // setting the diagonal elements of covariance matrix

  if(!eCOV) eCOV = new TMatrixD(5,5);
  else eCOV->Zero();
  (*eCOV)(0,0) = (double)sx2; 
  (*eCOV)(1,1) = (double)sy2; 
  (*eCOV)(2,2) = (double)stx2;
  (*eCOV)(3,3) = (double)sty2;
  (*eCOV)(4,4) = (double)sp2;
  eSZ = sz2;
}

//______________________________________________________________________________
void EdbSegP::SetErrorsCOV( float sx2, float sy2, float sz2, float stx2, float sty2, float sp2 )
{ 
  // calculation of the non-diagonal covariance matrix considering the input 
  // sigma being in the track plane (Y - is transversal axis)

  SetErrors( sx2, sy2, sz2, stx2, sty2, sp2 );

  //double Phi = -(TMath::ATan2((double)TY(),(double)TX()));
  double Phi = (TMath::ATan2((double)TY(),(double)TX()));
  TMatrixD t(5,5);
  TMatrixD tt(5,5);
  t(0,0) =  TMath::Cos(Phi);
  t(0,1) = -(TMath::Sin(Phi));
  t(1,0) =  TMath::Sin(Phi);
  t(1,1) =  TMath::Cos(Phi);
  t(2,2) =  TMath::Cos(Phi);
  t(2,3) = -(TMath::Sin(Phi));
  t(3,2) =  TMath::Sin(Phi);
  t(3,3) =  TMath::Cos(Phi);
  t(4,4) =  1.;
  tt(0,0) =  t(0,0);
  tt(1,0) =  t(0,1);
  tt(0,1) =  t(1,0);
  tt(1,1) =  t(1,1);
  tt(2,2) =  t(2,2);
  tt(3,2) =  t(2,3);
  tt(2,3) =  t(3,2);
  tt(3,3) =  t(3,3);
  tt(4,4) =  t(4,4);
  (*eCOV) = t*((*eCOV)*tt);
}

///______________________________________________________________________________
void EdbSegP::Copy(const EdbSegP &s)
{
  SetPID(s.PID());
  Set(s.ID(),s.X(),s.Y(),s.TX(),s.TY(),s.W(),s.Flag());
  SetZ(s.Z());
  SetCOV(s.COV());
  SetSZ(s.SZ());
  SetVid(s.Vid(0),s.Vid(1));
  SetAid(s.Aid(0),s.Aid(1),s.Side());
  SetProb(s.Prob());
  SetVolume(s.Volume());
  SetDZ(s.DZ());
  SetDZem(s.DZem());
  SetP(s.P());
  SetChi2(s.Chi2());
  SetTrack(s.Track());
  SetMC( s.MCEvt(), s.MCTrack() );
  SetScanID(s.ScanID());
  if(s.eEMULDigitArray) {
    eEMULDigitArray = new TRefArray(*s.eEMULDigitArray);
    printf("copy: %d %d\n",s.eEMULDigitArray->GetEntries(),eEMULDigitArray->GetEntries());
  }
}

///______________________________________________________________________________
void EdbSegP::LinkMT(const EdbSegP* s1,const EdbSegP* s2, EdbSegP* s)
{
  /// Segments fit by Andrey Aleksandrov (Jul-2003)

  Double_t dz = s2->Z() - s1->Z();
  Double_t dz2 = dz*dz;
 
  Double_t q1,q2,w1,w2;
  Double_t d1,d2,dxx11,dxx22;
  Double_t dtt01,dtt02,dtx01,dtx02;
  Double_t dxx01,dxx02,dxt01,dxt02;
  Double_t xm1,xm2,sx0,sy0,stx0,sty0;
 
  Double_t q;

  if(dz==0.0) {
    s->SetZ(s1->Z());
    s->SetID(s1->ID());
      
    q1 = s1->SX();
    q2 = s2->SX();
    w1 = s1->STX();
    w2 = s2->STX();
    
    sx0 = q1*q2/(q1+q2);
    q = (s1->X()/q1+s2->X()/q2)*sx0;
    s->SetX(q);
    stx0 = w1*w2/(w1+w2);
    q = (s1->TX()/w1+s2->TX()/w2)*stx0;
    s->SetTX(q);
 
    q1 = s1->SY();
    q2 = s2->SY();
    w1 = s1->STY();
    w2 = s2->STY();
 
    sy0 = q1*q2/(q1+q2);
    q = (s1->Y()/q1+s2->Y()/q2)*sy0;
    s->SetY(q);
    sty0 = w1*w2/(w1+w2);
    q = (s1->TY()/w1+s2->TY()/w2)*sty0;
    s->SetTY(q);
 
    s->SetErrors(sx0,sy0,0.0,stx0,sty0);
    s->SetW( s1->W()+s2->W() );
    return;
  }

  q = 0.5*(s1->Z()+s2->Z());
  Double_t dzr = 1.0/dz;
 
  s->SetZ(q);
  s->SetID(s1->ID());
 
  q1 = s1->SX();
  q2 = s2->SX();
  w1 = s1->STX();
  w2 = s2->STX();
 
  q = dz2*w2+q2;
  d1 = 1.0/(q+q1);
  xm1 = (q*s1->X()+(s2->X()-dz*s2->TX())*q1)*d1;
 
  q = dz2*w1+q1;
  d2 = 1.0/(q+q2);
  xm2 = (q*s2->X()+(s1->X()+dz*s1->TX())*q2)*d2;


  dtt01 = q2*d2;
  dtt02 = q1*d1;
  dxx11 = 1.0-dtt02;
  dxx22 = 1.0-dtt01;
  dxx01 = 0.5*(dxx11+dtt01);
  dxx02 = 0.5*(dxx22+dtt02);
  dxt01 = 0.5*dz*dtt01;
  dxt02 = -0.5*dz*dtt02;
  dtx01 = dzr*(dtt01-dxx11);
  dtx02 = dzr*(dxx22-dtt02);
 
  q = (xm1+xm2)*0.5;
  s->SetX(q);
  q = (xm2-xm1)*dzr;
  s->SetTX(q);
  sx0  = dxx01*dxx01*q1+dxx02*dxx02*q2+dxt01*dxt01*w1+dxt02*dxt02*w2;
  stx0 = dtx01*dtx01*q1+dtx02*dtx02*q2+dtt01*dtt01*w1+dtt02*dtt02*w2;
 
  q1 = s1->SY();
  q2 = s2->SY();
  w1 = s1->STY();
  w2 = s2->STY();
 
  q = dz2*w2+q2;
  d1 = 1.0/(q+q1);
  xm1 = (q*s1->Y()+(s2->Y()-dz*s2->TY())*q1)*d1;
 
  q = dz2*w1+q1;
  d2 = 1.0/(q+q2);
  xm2 = (q*s2->Y()+(s1->Y()+dz*s1->TY())*q2)*d2;

  dtt01 = q2*d2;
  dtt02 = q1*d1;
  dxx11 = 1.0-dtt02;
  dxx22 = 1.0-dtt01;
  dxx01 = 0.5*(dxx11+dtt01);
  dxx02 = 0.5*(dxx22+dtt02);
  dxt01 = 0.5*dz*dtt01;
  dxt02 = -0.5*dz*dtt02;
  dtx01 = dzr*(dtt01-dxx11);
  dtx02 = dzr*(dxx22-dtt02);
 
  q = (xm1+xm2)*0.5;
  s->SetY(q);
  q = (xm2-xm1)*dzr;
  s->SetTY(q);
  sy0  = dxx01*dxx01*q1+dxx02*dxx02*q2+dxt01*dxt01*w1+dxt02*dxt02*w2;
  sty0 = dtx01*dtx01*q1+dtx02*dtx02*q2+dtt01*dtt01*w1+dtt02*dtt02*w2;
 
  s->SetErrors(sx0,sy0,0.0,stx0,sty0);
  s->SetW( s1->W()+s2->W() );
  s->SetDZ(dz);
}

//______________________________________________________________________________
void EdbSegP::PropagateToCOV( float z ) 
{
  float dz = z-Z();
  eX  = X() + TX()*dz;
  eY  = Y() + TY()*dz;
  eZ  = z;

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();
  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;
  
  VtSymMatrix cov(4);             // covariance matrix for seg0
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (COV())(k,l);
  
  VtSymMatrix covpred(4);         // covariation matrix for prediction
  covpred = pred*(cov*(pred.T()));
  
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) (COV())(k,l) = covpred(k,l);
}

//______________________________________________________________________________
void EdbSegP::PropagateTo( float z ) 
{
  float dz = z-Z();

  eX  = X() + TX()*dz;
  eY  = Y() + TY()*dz;
  eZ  = z;
  if(eCOV) {
    (*eCOV)(0,0) = SX() + STX()*dz*dz;
    (*eCOV)(1,1) = SY() + STY()*dz*dz;
  }
}

//______________________________________________________________________________
bool EdbSegP::IsCompatible( EdbSegP &s, float nsigx, float nsigt ) const
{
  // return true if segments are closer then nsig sigma in all coordinates
  // assumed that z is the same
  float dtx=TX()-s.TX();
  if( dtx*dtx > STX()*nsigt*nsigt )    return false;
  float dty=TY()-s.TY();
  if( dty*dty > STY()*nsigt*nsigt )    return false;
  float dz=s.Z()-Z();
  float dx=X()+TX()*dz-s.X();
  if( dx*dx > SX()*nsigx*nsigx )       return false;
  float dy=Y()+TY()*dz-s.Y();
  if( dy*dy > SY()*nsigx*nsigx )       return false;
  return true;
}

//______________________________________________________________________________
float EdbSegP::ProbLink( EdbSegP &s1, EdbSegP &s2 )
{
  // return probability of the correct link in case Up/Down - specifics is 
  // that the position errors are neglected)

  double dz = s2.Z() - s1.Z();

  double tx = (s2.X() - s1.X()) / dz;
  double ty = (s2.Y() - s1.Y()) / dz;

  double dtx1 = (s1.TX()-tx)*(s1.TX()-tx)/s1.STX();
  double dty1 = (s1.TY()-ty)*(s1.TY()-ty)/s1.STY();
  double dtx2 = (s2.TX()-tx)*(s2.TX()-tx)/s2.STX();
  double dty2 = (s2.TY()-ty)*(s2.TY()-ty)/s2.STY();

  double chi2 = TMath::Sqrt(dtx1 + dty1 + dtx2 + dty2);
  double p3   = TMath::Prob(chi2*chi2,4);

  return s1.Prob()*s2.Prob()*p3;
}

//______________________________________________________________________________
void EdbSegP::MergeTo( EdbSegP &s ) 
{
  // create linked segment at Z of s2 
  // TODO - navesti nauku covariantnuiu 
  
  PropagateTo( s.Z() );

  float wx1,wx2, wy1,wy2;
  float wtx1,wtx2, wty1,wty2;

  wx1 = 1/SX();
  wx2 = 1/s.SX();
  wy1 = 1/SY();
  wy2 = 1/s.SY();
  wtx1 = 1/STX();
  wtx2 = 1/s.STX();
  wty1 = 1/STY();
  wty2 = 1/s.STY();

  eX = (X()*wx1 + s.X()*wx2)/(wx1+wx2);
  eY = (Y()*wy1 + s.Y()*wy2)/(wy1+wy2);
  (*eCOV)(0,0)=  1./(wx1+wx2);
  (*eCOV)(1,1)=  1./(wy1+wy2);

  eTX = (TX()*wtx1 + s.TX()*wtx2)/(wtx1+wtx2);
  eTY = (TY()*wty1 + s.TY()*wty2)/(wty1+wty2);
  (*eCOV)(2,2)= 1./(wtx1+wtx2);
  (*eCOV)(3,3)= 1./(wty1+wty2);

  eZ = s.Z();
  eSZ = TMath::Sqrt(( SZ() + s.SZ())/2);

  eW = W()+s.W();

  eID   = s.ID();
  ePID  = s.PID();
  eFlag = s.Flag();
}

///______________________________________________________________________________
void EdbSegP::Print(Option_t *opt) const
{
  printf("EdbSegP: ID= %d  PID = %d  Vid = %d  %d \t Aid = %d %d  Flag = %d\n", 
	 ID(),PID(),eVid[0],eVid[1],eAid[0],eAid[1], Flag() );
  printf("x,y,z,dz,tx,ty,w,flag = %f %f %f   %f    %f %f\n", 
	 X(),Y(),Z(),DZ(),TX(),TY() );
  printf("W = %f    P= %f \t Prob = %f \t Chi2 = %f \t Track = %d \n", 
	 W(),P(),Prob(),Chi2(), Track() );
	
  if(eCOV) eCOV->Print();
}

///______________________________________________________________________________
void EdbSegP::PrintNice() const
{
  printf( "EdbSegP: ID= %8d  PID = %8d, x= %14.3f, y= %14.3f, z= %14.3f, tx= %7.4f, ty= %7.4f, w= %7.4f, chi2= %7.4f Flag= %6d P= %6.1f MCEvt= %6d \n", ID(),PID(),X(),Y(),Z(),TX(),TY(),W(),Chi2(), Flag(), P(), MCEvt() );
  return;
}

///______________________________________________________________________________
Bool_t EdbSegP::IsEqual(const TObject *obj) const
{
  const EdbSegP *s=(EdbSegP*)obj;
  if(s->ID()  != ID())               return false;
  if(s->PID() != PID())              return false;
  if(s->Vid(0) != Vid(0))            return false;
  if(s->Vid(1) != Vid(1))            return false;
  if(s->Aid(0) != Aid(0))            return false;
  if(s->Aid(1) != Aid(1))            return false;
  if(s->Side() != Side())            return false;
  if( Abs(s->Z()- Z())  >0.000001 )  return false;
  if( Abs(s->X()- X())  >0.000001 )  return false;
  if( Abs(s->Y()- Y())  >0.000001 )  return false;
  if( Abs(s->TX()- TX())>0.000001 )  return false;
  if( Abs(s->TY()- TY())>0.000001 )  return false;
  if( Abs(s->W()- W())  >0.000001 )  return false;
  if(s->Plate() != Plate())          return false;
  return true;
}

///______________________________________________________________________________
int EdbSegP::Compare( const TObject *obj ) const
{
  const EdbSegP *s=(EdbSegP*)obj;
  double f1=0, f2=0;

  if     ( Abs(s->Z()- Z())  >0.000001 )   {  f1 = Z();   f2 = s->Z();  }  // usual case for tracks
  else if( Abs(s->X()- X())  >0.000001 )   {  f1 = X();   f2 = s->X();  }
  else if( Abs(s->Y()- Y())  >0.000001 )   {  f1 = Y();   f2 = s->Y();  }
  else if( Abs(s->TX()- TX())>0.000001 )   {  f1 = TX();  f2 = s->TX(); }
  else if( Abs(s->TY()- TY())>0.000001 )   {  f1 = TY();  f2 = s->TY(); }
  else if( Abs(s->W()- W())  >0.000001 )   {  f1 = W();   f2 = W();     }

  if      (f1>f2)    return  1;
  else if (f1<f2)    return -1;
  else               return  0;
}
