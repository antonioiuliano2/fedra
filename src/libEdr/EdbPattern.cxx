//-- Author :  Valeri Tioukov   19.05.2002
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPattern                                                           //
//                                                                      //
// Segments pattern                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TIndexCell.h"
#include "EdbAffine.h"
#include "EdbPattern.h"
#include "EdbPhys.h"
#include "EdbVertex.h"
#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"


ClassImp(EdbSegP)
ClassImp(EdbSegmentsBox)
ClassImp(EdbTrackP)
ClassImp(EdbPattern)
ClassImp(EdbPatternsVolume)

using namespace MATRIX;

//______________________________________________________________________________
EdbSegP::EdbSegP() 
{
  ePID=0;
  eID=0;
  eVid[0]=0;
  eVid[1]=0;
  eFlag=0;
  eTrack=-1;
  eProb=0;
  eW=0;
  eVolume=0;
  eDZ=0;
  eP=-999.;
  eChi2=0;
  eCOV=0;
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

  double Phi = -(TMath::ATan2((double)TY(),(double)TX()));
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
  TMatrixD c(5,5);
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
      SetAid(s.Aid(0),s.Aid(1));
      SetProb(s.Prob());
      SetVolume(s.Volume());
      SetDZ(s.DZ());
      SetDZem(s.DZem());
      SetP(s.P());
      SetChi2(s.Chi2());
      SetTrack(s.Track());
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
void EdbSegP::PropagateTo( float z ) 
{
  float dz = z-Z();

  eX  = X() + TX()*dz;
  eY  = Y() + TY()*dz;
  eZ  = z;
  (*eCOV)(0,0) = SX() + STX()*dz*dz;
  (*eCOV)(1,1) = SY() + STY()*dz*dz;
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
  printf("EdbSegP: %d  PID = %d  Vid = %d  %d \t Aid = %d %d\n", 
	 ID(),PID(),eVid[0],eVid[1],eAid[0],eAid[1] );
  printf("x,y,tx,ty,w,flag = %f %f %f  %f %f  %f  %d\n", 
	 X(),Y(),Z(),TX(),TY(),W(),Flag() );
  printf("P= %f \t Prob = %f \t Chi2 = %f \t Track = %d \n", 
	 P(),Prob(),Chi2(), Track() );
	
  if(eCOV) eCOV->Print();
} 

///______________________________________________________________________________
Bool_t EdbSegP::IsEqual(const TObject *obj) const
{
  const EdbSegP *s=(EdbSegP*)obj;
  if(s->ID()  != ID())  return false;
  if(s->PID() != PID()) return false;
  if(s->Z()   != Z())   return false;
  return true;
}

///______________________________________________________________________________
int EdbSegP::Compare( const TObject *obj ) const
{
  const EdbSegP *s=(EdbSegP*)obj;
  double f1=0, f2=0;

  f1 = Z();
  f2 = s->Z();

  if (f1>f2)
    return 1;
  else if (f1<f2)
    return -1;
  else
    return 0;
}

//______________________________________________________________________________
//______________________________________________________________________________
EdbSegmentsBox::EdbSegmentsBox(int nseg)
{
  if(nseg>0) eSegments = new TClonesArray("EdbSegP",nseg);
  else       eSegments = new TClonesArray("EdbSegP");
  Set0();
}
 
//______________________________________________________________________________
EdbSegmentsBox::EdbSegmentsBox(float x0, float y0, float z0, int nseg)
{
  if(nseg>0) eSegments = new TClonesArray("EdbSegP",nseg);
  else       eSegments = new TClonesArray("EdbSegP");
  eX=x0;  eY=y0;  eZ=z0;
  eDZkeep=0;
}
 
//______________________________________________________________________________
EdbSegmentsBox::~EdbSegmentsBox( )
{
  if(eSegments) delete eSegments;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::Set0()
{
  eX=0;  eY=0;  eZ=0;
  eDZkeep=0;
}
 
//______________________________________________________________________________
EdbSegP *EdbSegmentsBox::AddSegment(int id, float x, float y, float tx, float ty, 
			    float w, int flag)
{
  return new((*eSegments)[N()])  EdbSegP( id,x,y,tx,ty,w,flag );
}
 
//______________________________________________________________________________
EdbSegP *EdbSegmentsBox::AddSegment( int i, EdbSegP &s )
{
  return new((*eSegments)[i])  EdbSegP( s );
}
 
//______________________________________________________________________________
EdbSegP *EdbSegmentsBox::AddSegment( EdbSegP &s )
{
  return new((*eSegments)[N()])  EdbSegP( s );
}
 
//______________________________________________________________________________
EdbSegP *EdbSegmentsBox::AddSegment( EdbSegP &s1, EdbSegP &s2 )
{
  EdbSegP *s = new((*eSegments)[N()])  EdbSegP( s1 );
  s->MergeTo(s2);
  return s;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::Reset()
{
  Set0();
  if(eSegments) eSegments->Clear();
}
 
//______________________________________________________________________________
float EdbSegmentsBox::DiffAff(EdbAffine2D *aff)
{
  EdbSegmentsBox p,p1;
  p.AddSegment(0,Xmin(),Ymin(),0.,0.);
  p.AddSegment(0,Xmax(),Ymin(),0.,0.);
  p.AddSegment(0,Xmin(),Ymax(),0.,0.);
  p.AddSegment(0,Xmax(),Ymax(),0.,0.);

  p1.AddSegment(0,Xmin(),Ymin(),0.,0.);
  p1.AddSegment(0,Xmax(),Ymin(),0.,0.);
  p1.AddSegment(0,Xmin(),Ymax(),0.,0.);
  p1.AddSegment(0,Xmax(),Ymax(),0.,0.);

  p1.Transform(aff);
  return p.Diff(p1);
}
 
//______________________________________________________________________________
float EdbSegmentsBox::Diff(EdbSegmentsBox &p)
{
  // return the mean difference beteween pattern elements
  int nseg = TMath::Min( N(), p.N() );

  if(nseg<1) return 0;

  EdbSegP *s1=0, *s2=0;

  float dx=0, dy=0;
  double sdx=0;
  for(int i=0; i<nseg; i++ ) {
    s1 =   GetSegment(i);
    s2 = p.GetSegment(i);
    dx = s2->X() - s1->X();
    dy = s2->Y() - s1->Y();
    sdx += TMath::Sqrt( dx*dx + dy*dy);
  }
  return sdx/nseg;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::SetSegmentsZ()
{
  int nseg = N();
  for(int i=0; i<nseg; i++ )    {
    GetSegment(i)->SetZ( Z() );
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::ProjectTo(const float dz)
{
  eZ += dz;  eDZkeep += dz;
 
  EdbSegP *p;
  int nseg = N();
  for(int i=0; i<nseg; i++ ) {
    p = GetSegment(i);
    p->SetX( p->X() + p->TX()*dz );
    p->SetY( p->Y() + p->TY()*dz );
    p->SetZ( Z() );
  }
}
 
//______________________________________________________________________________
int EdbSegmentsBox::CalculateXY( EdbSegmentsBox *pat, EdbAffine2D *aff )
{
  int n=N();
  if( n>pat->N() )  n=pat->N();
  if(n<2) return 0;

  aff->Calculate(this,pat);
  return 1;
}

//______________________________________________________________________________
int EdbSegmentsBox::CalculateAXAY( EdbSegmentsBox *pat, EdbAffine2D *aff )
{
  int n=N();
  if( n>pat->N() )  n=pat->N();
  if(n<2) return 0;

  float *ax1 = new float[n];
  float *ay1 = new float[n];
  float *ax2 = new float[n];
  float *ay2 = new float[n];

  EdbSegP *p1,*p2;
  for(int i=0; i<n; i++ ) {
    p1 = GetSegment(i);
    p2 = pat->GetSegment(i);
    ax1[i] = p1->TX();
    ay1[i] = p1->TY();
    ax2[i] = p2->TX();
    ay2[i] = p2->TY();
  }
  aff->Calculate(n,ax1,ay1,ax2,ay2);

  delete ax1;
  delete ay1;
  delete ax2;
  delete ay2;

  return 1;
}

//______________________________________________________________________________
void EdbSegmentsBox::TransformA( const EdbAffine2D *aff )
{
  EdbSegP *p;
  float tx,ty;

  int nseg = N();
  for(int i=0; i<nseg; i++ ) {
    p = GetSegment(i);

    tx = aff->A11()*p->TX() + aff->A12()*p->TY() + aff->B1();
    ty = aff->A21()*p->TX() + aff->A22()*p->TY() + aff->B2();
    p->SetTX(tx);
    p->SetTY(ty);
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::TransformShr( float shr )
{
  EdbSegP *p;
  float tx,ty;

  int nseg = N();
  for(int i=0; i<nseg; i++ ) {
    p = GetSegment(i);

    tx = p->TX()/shr;
    ty = p->TY()/shr;
    p->SetTX(tx);
    p->SetTY(ty);
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::TransformARot( const EdbAffine2D *aff )
{
  // apply to the angles only rotation members of transformation

  EdbSegP *p;
  float tx,ty;

  int nseg = N();
  for(int i=0; i<nseg; i++ ) {
    p = GetSegment(i);

    tx = aff->A11()*p->TX() + aff->A12()*p->TY();
    ty = aff->A21()*p->TX() + aff->A22()*p->TY();
    p->SetTX(tx);
    p->SetTY(ty);
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::Print(Option_t *opt) const
{
  int nseg=GetN();
  printf("EdbSegmentsBox: %d segments\n", nseg );
  for(int i=0; i<nseg; i++) GetSegment(i)->Print();
} 

//______________________________________________________________________________
//______________________________________________________________________________
EdbTrackP::EdbTrackP(int nseg)
{
  eS=0;
  eSF=0;
  eM=0;
  eDE=0;
  eVertexS = 0;
  eVertexE = 0;
  if(nseg>0) eS  = new TSortedList();
  if(nseg>0) { eSF = new TSortedList();    eSF->SetOwner(); }
}
 
//______________________________________________________________________________
EdbTrackP::~EdbTrackP()
{
  if(eS)    { eS->Clear();  delete eS;  eS=0;  }
  if(eSF)   { eSF->Clear(); delete eSF; eSF=0; }
}

//______________________________________________________________________________
void EdbTrackP::Copy(const EdbTrackP &tr)
{
  // do the physical copy of segments
  Reset();
  ((EdbSegP*)(this))->Copy( *((EdbSegP*)(&tr)) );

  SetID(tr.ID());
  SetM(tr.M());
  SetNpl(tr.Npl());  
  SetN0(tr.N0());
  SetDE(tr.DE());
  SetVertexS(tr.VertexS());
  SetVertexE(tr.VertexE());

  int nseg=tr.N();
  for(int i=0; i<nseg; i++)
    AddSegment(new EdbSegP(*tr.GetSegment(i)));
  for(int i=0; i<nseg; i++)
    AddSegmentF(new EdbSegP(*tr.GetSegmentF(i)));
  eS->SetOwner();
  eSF->SetOwner();
}

//______________________________________________________________________________
float EdbTrackP::Wgrains() const
{
  float w=0.;
  int nseg=N();
  for(int i=0; i<nseg; i++)    w+=GetSegment(i)->W();
  return w;
}

//______________________________________________________________________________
void EdbTrackP::AddTrack(const EdbTrackP &tr)
{
  int nseg=tr.N();
  int nsegf=tr.NF();
  for(int i=0; i<nseg; i++)
    AddSegment(tr.GetSegment(i));
  for(int i=0; i<nsegf; i++)
    AddSegmentF(new EdbSegP(*(tr.GetSegmentF(i))));
}

//______________________________________________________________________________
void EdbTrackP::FitTrack()
{
  // track fit by averaging of segments parameters

  int nseg=N();
  float x=0,y=0,z=0,tx=0,ty=0,w=0;
  EdbSegP *seg=0;
  for(int i=0; i<nseg; i++) {
    seg = GetSegment(i);
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
  Set(ID(),x,y,tx,ty,w,Flag());
  SetZ(z);
}

/*
//______________________________________________________________________________
int  EdbTrackP::FitTrackKF( bool zmax)
{
  // if (zmax==true)  track parameters are calculated at segment with max Z
  // if (zmax==false) track parameters are calculated at segment with min Z
  //
  // Note: SetP() for track must be setted before!
  // Note: SetErrorP() for track should be setted - necessary for vertex fit with momenta
  //
  // on the output: Chi2: the full chi-square (not divided by NDF); NDF = (nseg-1)*4
  //                Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept events with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good event

  float dPb;
  double teta0sq;
  double dz;
  int step;
  int istart, iend;

  int nseg=N();
  if(nseg<2)   return -1;
 
  //TODO: elaborate case of 1 segment

  EdbSegP *seg0=0;
  EdbSegP *seg=0;

  if(  GetSegment(N()-1)->Z() <  GetSegment(0)->Z() )
  {
    if (zmax)
    {
	step=-1;
	istart=nseg-1;
	iend=0;
    }
    else
    {
	step=1;
	istart=0;
	iend=nseg-1;
    }
  }
  else
  {
    if (!zmax)
    {
	step=-1;
	istart=nseg-1;
	iend=0;
    }
    else
    {
	step=1;
	istart=0;
	iend=nseg-1;
    }

  }

  seg0 = GetSegment(istart);
  VtVector par( (double)(seg0->X()), 
		(double)(seg0->Y()),  
		(double)(seg0->TX()), 
		(double)(seg0->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (seg0->COV())(k,l);

  Double_t chi2=0.; 

  int i=istart; 
  while( (i+=step) != iend+step ) {

    seg = GetSegment(i);

    VtSymMatrix dms(4);   // multiple scattering matrix
    dms.clear();

    dz = seg->Z()-seg0->Z();                        //?
    dPb = dz*TMath::Sqrt(1.+par(2)*par(2)+par(3)*par(3)); // thickness of the Pb+emulsion cell in microns
    teta0sq = EdbPhysics::ThetaPb2( P(), M(), dPb );

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

    VtSymMatrix covpred(4);         // covariation matrix for prediction
    covpred = pred*(cov*pred.T())+dms;

    VtSymMatrix dmeas(4);           // original covariation  matrix for seg
    for(int k=0; k<4; k++) 
      for(int l=0; l<4; l++) dmeas(k,l) = (seg->COV())(k,l);

    covpred = covpred.dsinv();
    dmeas   = dmeas.dsinv();
    cov = covpred + dmeas;
    cov = cov.dsinv();

    VtVector meas( (double)(seg->X()), 
		   (double)(seg->Y()),  
		   (double)(seg->TX()), 
		   (double)(seg->TY()) );

    par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

    chi2 += (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

    seg0 = seg;
  }

  Set(ID(),(float)par(0),(float)par(1),(float)par(2),(float)par(3),1.);
  SetZ(GetSegment(iend)->Z());
  SetCOV( cov.array(), 4 );
  SetChi2((float)chi2);
  SetProb( (float)TMath::Prob(chi2,(nseg-1)*4));
  SetW( (float)nseg );

  if(zmax) {
    eSegZmax.Copy( *((EdbSegP*)this) );
  } else {
    eSegZmin.Copy( *((EdbSegP*)this) );
  }

  return 0;
}

*/
//______________________________________________________________________________
int  EdbTrackP::FitTrackKFS( bool zmax, float X0)
{
  // if (zmax==true)  track parameters are calculated at segment with max Z
  // if (zmax==false) track parameters are calculated at segment with min Z
  // TODO: track parameters??
  //
  // Note: SetP() for track must be setted before!
  // Note: SetErrorP() for track should be setted - necessary for vertex fit with momenta
  //
  // on the output: Chi2: the full chi-square (not divided by NDF); NDF = nseg*4
  //                Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept events with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good event


  if(!N()) return 0;
  if(NF()) {
    eSF->Delete();
  }

  //printf("%d segments to fit\n",N());

  float dPb = 0.;
  double teta0sq;
  double dz, ptx, pty;
  int step;
  int istart, iend;

  VtVector *par[60], *parpred[60], *pars[60], *meas[60];
  VtSqMatrix *pred[60];
  VtSymMatrix *cov[60], *covpred[60], *covpredinv[60], *covs[60], *dmeas[60];
 
  int i=0;
  int nseg=N();
  if(nseg<2)    return -1;
  if(nseg>59)   return -1;

  EdbSegP *seg0=0;
  EdbSegP *seg=0;

  if(  GetSegment(N()-1)->Z() <  GetSegment(0)->Z() )
  {
    if (zmax)
    {
	step=-1;
	istart=nseg-1;
	iend=0;
    }
    else
    {
	step=1;
	istart=0;
	iend=nseg-1;
    }
  }
  else
  {
    if (!zmax)
    {
	step=-1;
	istart=nseg-1;
	iend=0;
    }
    else
    {
	step=1;
	istart=0;
	iend=nseg-1;
    }

  }

  seg0 = GetSegment(istart);

  par[istart] = new VtVector(  (double)(seg0->X()), 
			       (double)(seg0->Y()),  
			       (double)(seg0->TX()), 
			       (double)(seg0->TY()) );
  meas[istart] = new VtVector(*par[istart]);
  pred[iend] = new VtSqMatrix(4);
//  (*pred[istart]).clear();
//  (*pred[istart])(0,0) = 1.;
//  (*pred[istart])(1,1) = 1.;
//  (*pred[istart])(2,2) = 1.;
//  (*pred[istart])(3,3) = 1.;
  cov[istart] = new VtSymMatrix(4);             // covariance matrix for seg0
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) (*cov[istart])(k,l) = (seg0->COV())(k,l);
  dmeas[istart] = new VtSymMatrix(*cov[istart]);             // covariance matrix for seg0

  Double_t chi2=0.; 

  i=istart; 
  while( (i+=step) != iend+step ) {

    seg = GetSegment(i);
 
    VtSymMatrix dms(4);   // multiple scattering matrix
    dms.clear();

    dz = seg->Z()-seg0->Z();
    ptx = (*par[i-step])(2);                        //?
    pty = (*par[i-step])(3);                        //?
    dPb = dz*TMath::Sqrt(1.+ptx*ptx+pty*pty); // thickness of the Pb+emulsion cell in microns
    //teta0sq = EdbPhysics::ThetaPb2( P(), M(), dPb );
    teta0sq = EdbPhysics::ThetaMS2( P(), M(), dPb, X0 );

    dms(0,0) = teta0sq*dz*dz/3.;
    dms(1,1) = dms(0,0);
    dms(2,2) = teta0sq;
    dms(3,3) = dms(2,2);
    dms(2,0) = teta0sq*dz/2.;
    dms(3,1) = dms(2,0);
    dms(0,2) = dms(2,0);
    dms(1,3) = dms(2,0);

    pred[i-step] = new VtSqMatrix(4);        //propagation matrix for track parameters (x,y,tx,ty)
    pred[i-step]->clear();

    (*pred[i-step])(0,0) = 1.;
    (*pred[i-step])(1,1) = 1.;
    (*pred[i-step])(2,2) = 1.;
    (*pred[i-step])(3,3) = 1.;
    (*pred[i-step])(0,2) = dz;
    (*pred[i-step])(1,3) = dz;

    parpred[i] = new VtVector(4);            // prediction from seg0 to seg
    *parpred[i] = (*pred[i-step])*(*par[i-step]);

    covpred[i] = new VtSymMatrix(4);         // covariation matrix for prediction
    *covpred[i] = (*pred[i-step])*((*cov[i-step])*((*pred[i-step]).T()))+dms;

    dmeas[i] = new VtSymMatrix(4);           // original covariation  matrix for seg
    for(int k=0; k<4; k++) 
      for(int l=0; l<4; l++) (*dmeas[i])(k,l) = (seg->COV())(k,l);

    covpredinv[i] = new VtSymMatrix(4);
    (*covpredinv[i]) = (*covpred[i]).dsinv();
    VtSymMatrix dmeasinv(4);
    dmeasinv  = (*dmeas[i]).dsinv();
    cov[i] = new VtSymMatrix(4);
    (*cov[i]) = (*covpredinv[i]) + dmeasinv;
    (*cov[i]) = (*cov[i]).dsinv();

    meas[i] = new VtVector( (double)(seg->X()), 
			    (double)(seg->Y()),  
			    (double)(seg->TX()), 
			    (double)(seg->TY()) );

    par[i] = new VtVector(4);
    (*par[i]) = (*cov[i])*((*covpredinv[i])*(*parpred[i]) + dmeasinv*(*meas[i]));   // new parameters for seg

    chi2 += ((*par[i])-(*parpred[i]))*((*covpredinv[i])*((*par[i])-(*parpred[i]))) + 
	    ((*par[i])-(*meas[i]))*(dmeasinv*((*par[i])-(*meas[i])));

//    VtSymMatrix dresid(4);
//    dresid = (*dmeas[i]) - (*cov[i]);
//    dresid = dresid.dsinv();
//
//    chi2 += ((*par[i])-(*meas[i]))*(dresid*((*par[i])-(*meas[i])));

    seg0 = seg;
  }

  Set(ID(),(float)(*par[iend])(0),(float)(*par[iend])(1),
	   (float)(*par[iend])(2),(float)(*par[iend])(3),1.,Flag());
  SetZ(GetSegment(iend)->Z());
  SetPID(GetSegment(iend)->PID());
  SetCOV( (*cov[iend]).array(), 4 );

  //SetChi2((float)chi2);
  //SetProb( (float)TMath::Prob(chi2,nseg*4));

// Smoothing

  pars[iend] = new VtVector(*par[iend]);
  covs[iend] = new VtSymMatrix(*cov[iend]);
  VtSymMatrix dresid(4);
  dresid = (*dmeas[iend]) - (*covs[iend]);
  dresid = dresid.dsinv();
  chi2 = ((*pars[iend])-(*meas[iend]))*(dresid*((*pars[iend])-(*meas[iend])));

  EdbSegP segf;
  segf.Set(ID(),(float)(*pars[iend])(0),(float)(*pars[iend])(1),
	   (float)(*pars[iend])(2),(float)(*pars[iend])(3),1.,Flag());
  segf.SetZ(GetSegment(iend)->Z());
  segf.SetCOV( (*covs[iend]).array(), 4 );
  segf.SetChi2((float)chi2);
  segf.SetProb( (float)TMath::Prob(chi2,4));
  segf.SetW( (float)nseg );
  segf.SetP( P() );
  segf.SetPID( GetSegment(iend)->PID() );

  AddSegmentF(new EdbSegP(segf));

  i=iend; 
  double chi2p=0;
  double DE=0.;
  EdbPhysics::DeAveragePbFastSet(P(), M());
   
  while( (i-=step) != istart-step ) {
	VtSqMatrix BackTr(4);
	BackTr = (*cov[i])*(((*pred[i]).T())*(*covpredinv[i+step]));
	pars[i] = new VtVector(4);
	covs[i] = new VtSymMatrix(4);
	(*pars[i]) = (*par[i]) + BackTr*((*pars[i+step])-(*parpred[i+step]));
	(*covs[i]) = (*cov[i]) + BackTr*(((*covs[i+step])-(*covpred[i+step]))*BackTr.T());
	dresid = (*dmeas[i]) - (*covs[i]);
	dresid = dresid.dsinv();
	chi2p = ((*pars[i])-(*meas[i]))*(dresid*((*pars[i])-(*meas[i])));
	chi2 += chi2p;

	segf.Set(ID(),(float)(*pars[i])(0),(float)(*pars[i])(1),
		 (float)(*pars[i])(2),(float)(*pars[i])(3),1.,Flag());
	segf.SetZ(GetSegment(i)->Z());
	segf.SetCOV( (*covs[i]).array(), 4 );
	segf.SetChi2((float)chi2p);
	segf.SetProb( (float)TMath::Prob(chi2p,4));
	segf.SetW( (float)nseg );
	segf.SetP( P() );
	segf.SetPID( GetSegment(i)->PID() );
	AddSegmentF(new EdbSegP(segf));
	dz = TMath::Abs(GetSegment(i)->Z() - GetSegment(i+step)->Z()); 
        dPb = dz*TMath::Sqrt(1.+(*pars[i])(2)*(*pars[i])(2)+(*pars[i])(3)*(*pars[i])(3)); // thickness of the Pb+emulsion cell in microns
	DE += EdbPhysics::DeAveragePbFast(P(),M(),dPb);
  }
  SetChi2((float)chi2);
  SetProb((float)TMath::Prob(chi2,nseg*4));
  SetW( (float)nseg );
  SetDE( (float)DE );

// Delete matrixes and vectors

  delete par[istart];
  par[istart] = 0;
  delete cov[istart];
  cov[istart] = 0;
  delete meas[istart];
  meas[istart] = 0;
  delete dmeas[istart];
  dmeas[istart] = 0;
  delete pred[istart];
  pred[istart] = 0;
  delete pars[istart];
  pars[istart] = 0;
  delete covs[istart];
  covs[istart] = 0;
  i=istart; 
  while( (i+=step) != iend+step ) {
    delete pred[i];
    pred[i] = 0;
    delete parpred[i];
    parpred[i] = 0;
    delete covpred[i];
    covpred[i] = 0;
    delete covpredinv[i];
    covpredinv[i] = 0;
    delete par[i];
    par[i] = 0;
    delete cov[i];
    cov[i] = 0;
    delete meas[i];
    meas[i] = 0;
    delete dmeas[i];
    dmeas[i] = 0;
    delete pars[i];
    pars[i] = 0;
    delete covs[i];
    covs[i] = 0;
  }
  return 0;
}

///______________________________________________________________________________
int EdbTrackP::MakeSelector( EdbSegP &ss, bool followZ )
{
  if(N()<2)           return 0;
  ss.SetCOV( GetSegment(0)->COV() );             // TODO ?
  const EdbSegP *tr;
  if( followZ ) tr = TrackZmax();
  else  tr = TrackZmin();
  ss.SetTX(tr->TX());
  ss.SetTY(tr->TY());
  ss.SetX(tr->X());
  ss.SetY(tr->Y());
  ss.SetZ(tr->Z());
  ss.SetPID(tr->PID());

  if( tr->PID() > GetSegment(0)->PID() )     return 1;
  if( tr->PID() > GetSegment(N()-1)->PID() ) return 1;
  return -1;
}

//______________________________________________________________________________
float EdbTrackP::CHI2()
{
  double dtx=0,dty=0,chi2=0;
  EdbSegP *seg=0;
  int    nseg=N();
  for(int i=0; i<nseg; i++) {
    seg = GetSegment(i);
    dtx = seg->TX()-TX();
    dty = seg->TY()-TY();
    chi2 += TMath::Sqrt( dtx*dtx/seg->STX() + 
			 dty*dty/seg->STY() );
  }
  chi2  /= nseg;
  return chi2;
}

//______________________________________________________________________________
float EdbTrackP::CHI2F()
{
  double dtx=0,dty=0,chi2=0;
  EdbSegP *s=0, *sf=0;
  int    nseg=N();
  for(int i=0; i<nseg; i++) {
    s  = GetSegment(i);
    sf = GetSegmentF(i);
    dtx = s->TX() - sf->TX();
    dty = s->TY() - sf->TY();
    chi2 += TMath::Sqrt( dtx*dtx/s->STX() + 
			 dty*dty/s->STY() );
  }
  chi2  /= nseg;
  return chi2;
}

//______________________________________________________________________________
void EdbTrackP::Print() 
{
  int nseg=0, nsegf=0;
  nseg = N();
  nsegf = NF();

  printf("EdbTrackP with %d segments and %d fitted segments\n", nseg, nsegf );
  printf("particle mass = %f\n", M() );
  ((EdbSegP*)this)->Print(); 

  if(nseg) 
    for(int i=0; i<nseg; i++)
      ((EdbSegP*)(eS->At(i)))->Print(); 

//    if(nsegf) 
//      for(int i=0; i<nsegf; i++) 
//        ((EdbSegP*)(eSF->At(i)))->Print();
}

//______________________________________________________________________________
//______________________________________________________________________________
EdbPattern::EdbPattern()
{
  eCell     = new TIndexCell();
  Set0();
}

//______________________________________________________________________________
EdbPattern::EdbPattern(float x0, float y0, float z0, int n) : EdbSegmentsBox(x0,y0,z0,n) 
{
  eCell     = new TIndexCell();
  Set0();
}
 
//______________________________________________________________________________
EdbPattern::~EdbPattern()
{
  if(eCell)     delete eCell;
}

//______________________________________________________________________________
void EdbPattern::Set0()
{
  eID = 0;
}

//______________________________________________________________________________
void EdbPattern::FillCell( float stepx, float stepy, float steptx, float stepty )
{
  // fill cells with fixed size at z=zPat

  TIndexCell *cell = Cell();
  if(cell) cell->Drop();
  SetStep(stepx,stepy,steptx,stepty);

  float x,y,tx,ty,dz;
  Long_t  val[5];  // x,y,ax,ay,i
  EdbSegP *p;
  int npat = N();
  for(int i=0; i<npat; i++ ) {
    p = GetSegment(i);
    dz = Z() - p->Z();
    tx = p->TX();
    ty = p->TY();
    x  = p->X() + tx*dz;
    y  = p->Y() + ty*dz;
    val[0]= (Long_t)( x / stepx  );
    val[1]= (Long_t)( y / stepy  );
    val[2]= (Long_t)( tx/ steptx );
    val[3]= (Long_t)( ty/ stepty );
    val[4]= (Long_t)(i);
    cell->Add(5,val);
  }
  cell->Sort();

}

//______________________________________________________________________________
int EdbPattern::FindCompliments(EdbSegP &s, TObjArray &arr, float nsigx, float nsigt)
{
  // return the array of segments compatible with the
  // prediction segment s with the accuracy of nsig (in sigmas)

  float dz = Z()-s.Z();
  float x = s.X() + s.TX()*dz;
  float y = s.Y() + s.TY()*dz;
  float sx = TMath::Sqrt( s.SX() + s.STX()*dz*dz );
  float sy = TMath::Sqrt( s.SY() + s.STY()*dz*dz );

  long vcent[4] = { (long)(x/StepX()),
		    (long)(y/StepY()),
		    (long)(s.TX()/StepTX()),
		    (long)(s.TY()/StepTY())  };
  long vdiff[4] = { (long)(sx*nsigx/StepX()+1),
		    (long)(sy*nsigx/StepY()+1),
		    (long)(TMath::Sqrt(s.STX())*nsigt/StepTX()+1),
		    (long)(TMath::Sqrt(s.STY())*nsigt/StepTY()+1) };

  sy*=sy;
  sx*=sx;

  float dx,dy,dtx,dty;

  int nseg=0;
  EdbSegP *seg=0;


  //printf("find compliments in pattern %d\n",ID());
  //s.Print();
  long vmin[4],vmax[4];
  for(int i=0; i<4; i++) {
    vmin[i] = vcent[i]-vdiff[i];
    vmax[i] = vcent[i]+vdiff[i];
    //printf("%f +- %f \t",(float)vcent[i],(float)vdiff[i]);
  }
  //printf("\n");

  ///TODO: move this cycle into iterator

  TIndexCell *c1=0,*c2=0,*c3=0,*c4=0;
  for(vcent[0]=vmin[0]; vcent[0]<=vmax[0]; vcent[0]++) {
    c1 = eCell->Find(vcent[0]);
    if(!c1) continue;
    for(vcent[1]=vmin[1]; vcent[1]<=vmax[1]; vcent[1]++) {
      c2 = c1->Find(vcent[1]);
      if(!c2) continue;
      for(vcent[2]=vmin[2]; vcent[2]<=vmax[2]; vcent[2]++) {
	c3 = c2->Find(vcent[2]);
	if(!c3) continue;
	for(vcent[3]=vmin[3]; vcent[3]<=vmax[3]; vcent[3]++) {
	  c4 = c3->Find(vcent[3]);
	  if(!c4) continue;

	  for(int i=0; i<c4->N(); i++) {
	    seg = GetSegment(c4->At(i)->Value());


	    dtx=s.TX()-seg->TX();
	    if( dtx*dtx > s.STX()*nsigt*nsigt )    continue;
	    dty=s.TY()-seg->TY();
	    if( dty*dty > s.STY()*nsigt*nsigt )    continue;
	    dz = seg->Z()-s.Z();
	    dx=s.X()+s.TX()*dz-seg->X();
	    if( dx*dx > sx*nsigx*nsigx )           continue;
	    dy=s.Y()+s.TY()*dz-seg->Y();
	    if( dy*dy > sy*nsigx*nsigx )           continue;	    
	    
	    //if(!s.IsCompatible(*seg,nsigx,nsigt)) continue;
	    arr.Add(seg);
	    nseg++;
	  }

	}
      }
    }
  }


//    TIndexCellIterV itr( eCell, 4, vcent, vdiff );
//    const TIndexCell *c=0;
//    while( (c=itr.Next()) ) 
//      for(int i=0; i<c->N(); i++) {
//        seg = GetSegment(c->At(i)->Value());
//        if(!s.IsCompatible(*seg,nsigx,nsigt)) continue;
//        arr.Add(seg);
//        nseg++;
//      }

  return nseg;
}

//______________________________________________________________________________
void EdbPattern::SetSegmentsPID()
{
  int nseg = N();
  for(int i=0; i<nseg; i++) {
    GetSegment(i)->SetPID(PID());
  }
}

//______________________________________________________________________________
EdbPattern *EdbPattern::ExtractSubPattern(float min[5], float max[5])
{
  //
  // return the copy of selected segments
  //
  EdbPattern *pat = new EdbPattern( X(), Y(), Z() );
  EdbSegP *s;
  int nseg = N();

  for(int i=0; i<nseg; i++) {
    s = GetSegment(i);

    if(s->X()  < min[0])   continue;
    if(s->Y()  < min[1])   continue;
    if(s->TX() < min[2])   continue;
    if(s->TY() < min[3])   continue;
    if(s->W()  < min[4])   continue;

    if(s->X()  > max[0])   continue;
    if(s->Y()  > max[1])   continue;
    if(s->TX() > max[2])   continue;
    if(s->TY() > max[3])   continue;
    if(s->W()  > max[4])   continue;

    pat->AddSegment(*s);
  }

  return pat;
}

//______________________________________________________________________________
void EdbPattern::Reset()
{
  ((EdbSegmentsBox *)this)->Reset();
  Set0();
  if(eCell)     eCell->Drop();
}
 
////////////////////////////////////////////////////////////////////////////////
//
//   EdbPatternsVolume
//
////////////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
EdbPatternsVolume::EdbPatternsVolume()
{
  ePatterns   = new TObjArray();
  eTracksCell = 0;
  eTracks     = new TObjArray();
  eSV  = 0;
  Set0();
}

//______________________________________________________________________________
EdbPatternsVolume::EdbPatternsVolume(EdbPatternsVolume &pvol)
{
  ePatterns   = new TObjArray();
  eTracksCell = 0;
  eTracks     = new TObjArray();
  eSV  = 0;
  Set0();

  pvol.PassProperties(*this);
  int npat,nseg;
  npat = Npatterns();
  for(int j=0; j<npat; j++) {
    nseg = GetPattern(j)->N();
    for(int i=0; i<nseg; i++ ) {
      pvol.GetPattern(j)->AddSegment( *(GetPattern(j)->GetSegment(i)) );
    }
  }
}

//______________________________________________________________________________
EdbPatternsVolume::~EdbPatternsVolume()
{
  if(ePatterns) {
    ePatterns->Delete();
    delete ePatterns;
  }
}

//______________________________________________________________________________
void EdbPatternsVolume::Set0()
{

}
 
//______________________________________________________________________________
int EdbPatternsVolume::DropCouples()
{
  int count=0;
  int npat=Npatterns();
  for(int i=0; i<npat; i++ )
    count += GetPattern(i)->Cell()->DropCouples(4);
  if(count) printf("%d couples are dropped in volume cells\n",count);
  return count;
}
 
//______________________________________________________________________________
void EdbPatternsVolume::SetPatternsID()
{
  int npat = Npatterns();
  for(int i=0; i<npat; i++ )
    GetPattern(i)->SetID(i);
}
 
//______________________________________________________________________________
void EdbPatternsVolume::Transform( const EdbAffine2D *aff )
{
  int npat = Npatterns();
  for(int i=0; i<npat; i++ )  {
    GetPattern(i)->Transform(aff);
    GetPattern(i)->TransformARot(aff);
  }
}
 
//______________________________________________________________________________
void EdbPatternsVolume::Centralize()
{
  // find geometrical center (XY) of all patterns and set it as the center of 
  // coordinates  to simplify transformations
  // To be used before any operations on patterns

  float xc=0;
  float yc=0;
  int npat = Npatterns();
  for(int i=0; i<npat; i++ ) {
    xc += GetPattern(i)->Xmax() + GetPattern(i)->Xmin();
    yc += GetPattern(i)->Ymax() + GetPattern(i)->Ymin();
  }
  xc = xc/Npatterns()/2;
  yc = yc/Npatterns()/2;

  Centralize(xc,yc);
}

//______________________________________________________________________________
void EdbPatternsVolume::Centralize( float xc, float yc )
{
  eX = xc;  eY=yc;
  Shift(-xc,-yc);
  float npat = Npatterns();
  for(int i=0; i<npat; i++ ) 
    GetPattern(i)->SetKeep(1,0,0,1,0,0);
}

//______________________________________________________________________________
void EdbPatternsVolume::PrintAff() const
{
  EdbAffine2D a;
  int npat = Npatterns();
  for(int i=0; i<npat; i++ ) {
    GetPattern(i)->GetKeep(a);
    printf(" %d ",i); a.Print();
  }
}

//______________________________________________________________________________
void EdbPatternsVolume::PrintStat( Option_t *opt) const
{
  int npat = Npatterns();
  printf("\nVolume statistics for %d patterns\n",npat);

  float dx,dy;
  EdbPattern *pat=0;
  printf("pat# \t segments \t dX \t\tdY \t meanDist \n");
  int i;
  for(i=0; i<npat; i++ ) {
    pat = GetPattern(i);
    dx = pat->Xmax() - pat->Xmin();
    dy = pat->Ymax()- pat->Ymin();
    printf(" %d\t %d\t %10.2f \t %10.2f \t %10.4f \n", 
	   i, pat->GetN(),dx,dy, TMath::Sqrt(dx*dy/pat->GetN()) );
  }

  npat=Npatterns();
  for(i=0; i<npat; i++ ) {
    pat = GetPattern(i);
    pat->Cell()->PrintStat();
  }
}
 
//______________________________________________________________________________
void EdbPatternsVolume::PrintStat(EdbPattern &pat) const
{
} 

//______________________________________________________________________________
void EdbPatternsVolume::Print() const
{
  int npat = Npatterns();
  printf("\nEdbPatternsVolume with %d patterns\n",npat);
  EdbPattern *pat=0;
  for(int i=0; i<npat; i++ ) {
    pat = GetPattern(i);
    printf(" x:y:z =  %f %f %f \t n= %d \n", 
	   pat->X(),pat->Y(),pat->Z(),pat->N());
  }
} 

//______________________________________________________________________________
void EdbPatternsVolume::AddPattern( EdbPattern *pat )
{
  ePatterns->Add(pat);
}

//______________________________________________________________________________
EdbPattern *EdbPatternsVolume::GetPattern( int id ) const
{
  if(Npatterns()>id) return (EdbPattern*)ePatterns->At(id);
  else return 0;
}


//______________________________________________________________________________
void EdbPatternsVolume::PassProperties( EdbPatternsVolume &pvol )
{
  pvol.SetXYZ(eX,eY,eZ);
  EdbAffine2D a;
  EdbPattern *p=0;

  int npat = Npatterns();
  for(int i=0; i<npat; i++ ) {
    p = GetPattern(i);
    p->GetKeep(a);
    EdbPattern *psel = new EdbPattern( p->X(),p->Y(),p->Z() );
    psel->SetKeep( a.A11(), a.A12(), a.A21(), a.A22(), a.B1(),a.B2() );
    pvol.AddPattern(psel);
  }
  pvol.SetPatternsID();
}


//______________________________________________________________________________
void EdbPatternsVolume::Shift( float x, float y )
{
  EdbAffine2D aff;
  aff.ShiftX(x);
  aff.ShiftY(y);

  int npat = Npatterns();
  for(int i=0; i<npat; i++)
    GetPattern(i)->Transform(&aff);
}

//______________________________________________________________________________
void EdbPatternsVolume::DropCell()
{
  int npat = Npatterns();
  for(int i=0; i<npat; i++ ) GetPattern(i)->Cell()->Drop();
}
