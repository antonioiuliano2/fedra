//-- Author :  Valeri Tioukov   13.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVirtual                                                           //
//                                                                      //
// Virtual algorithms                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TH2
#include "TH2.h"
#endif
 
#ifndef ROOT_TGraph
#include "TGraph.h"
#endif
 
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif
 
#ifndef ROOT_EdbVirtual
#include "EdbVirtual.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif

ClassImp(EdbPoint)
ClassImp(EdbPoint2D)
ClassImp(EdbPoint3D)
ClassImp(EdbPointsBox2D)
ClassImp(EdbPointsBox3D)

ClassImp(EdbAngle2D)

ClassImp(EdbTrack2D)

//_____________________________________________________________________________
void EdbTrack2D::Print( Option_t *opt ) const
{
  printf("EdbTrack2D: %f %f %f %f \n",X(), Y(), TX(),TY() );
}

//_____________________________________________________________________________
void EdbTrack2D::Transform( EdbAffine2D *a )
{
  EdbPoint2D::Transform(a);
  EdbAngle2D::Transform(a);
}

//_____________________________________________________________________________
void EdbTrack2D::Substruct( EdbTrack2D *t )
{
  EdbPoint2D::Substruct( (EdbPoint*)t );
  EdbAngle2D::Substruct( (EdbAngle2D*)t );
}

//_____________________________________________________________________________
void EdbAngle2D::Print( Option_t *opt ) const
{
  printf("EdbAngle2D: %f %f \n",TX(),TY() );
}

//_____________________________________________________________________________
void EdbAngle2D::Transform( EdbAffine2D *a )
{
  Float_t tx =  a->A11()*TX() + a->A12()*TY();
  Float_t ty =  a->A21()*TX() + a->A22()*TY();
  SetTX(tx);
  SetTY(ty);
}

//_____________________________________________________________________________
void EdbAngle2D::Substruct( EdbAngle2D *p )
{
  Float_t tx =  TX() - p->TX();
  Float_t ty =  TY() - p->TY();
  SetTX(tx);
  SetTY(ty);
}

//_____________________________________________________________________________
void EdbPoint3D::Print( Option_t *opt ) const
{
  printf("EdbPoint3D: %f %f %f\n",X(),Y(),Z() );
}

//_____________________________________________________________________________
void EdbPoint3D::Transform( EdbAffine3D *a )
{
  Float_t x =  a->A11()*X() + a->A12()*Y() + a->A13()*Z() + a->B1();
  Float_t y =  a->A21()*X() + a->A22()*Y() + a->A23()*Z() + a->B2();
  Float_t z =  a->A31()*X() + a->A32()*Y() + a->A33()*Z() + a->B3();
  SetX(x);
  SetY(y);
  SetZ(z);
}

//_____________________________________________________________________________
void EdbPoint3D::Substruct( EdbPoint *p )
{
  Float_t x =  X() - p->X();
  Float_t y =  Y() - p->Y();
  Float_t z =  Z() - p->Z();
  SetX(x);
  SetY(y);
  SetY(z);
}

//_____________________________________________________________________________
void EdbPoint2D::Print( Option_t *opt ) const
{
  printf("EdbPoint2D: %f %f \n",X(),Y() );
}

//_____________________________________________________________________________
void EdbPoint2D::Transform( EdbAffine2D *a )
{
  Float_t x =  a->A11()*X() + a->A12()*Y() + a->B1();
  Float_t y =  a->A21()*X() + a->A22()*Y() + a->B2();
  SetX(x);
  SetY(y);
}

//_____________________________________________________________________________
void EdbPoint2D::Substruct( EdbPoint *p )
{
  Float_t x =  X() - p->X();
  Float_t y =  Y() - p->Y();
  SetX(x);
  SetY(y);
}

//_____________________________________________________________________________
EdbPointsBox2D::EdbPointsBox2D()
{
  eKeep = new EdbAffine2D();
}

//_____________________________________________________________________________
EdbPointsBox2D::~EdbPointsBox2D()
{
  if(eKeep) delete eKeep;
}

//_____________________________________________________________________________
void EdbPointsBox2D::SetKeep(float a11,float a12,float a21,
			     float a22,float b1,float b2)
    {eKeep->Set(a11,a12,a21,a22,b1,b2);}

//_____________________________________________________________________________
void EdbPointsBox2D::GetKeep( EdbAffine2D &a )
{
  a.Set( eKeep->A11(),eKeep->A12(),eKeep->A21(),eKeep->A22(),
	 eKeep->B1(),eKeep->B2() );
}

//_____________________________________________________________________________
void EdbPointsBox2D::Transform( EdbAffine2D *a )
{
  for( int i=0; i<N(); i++ )  At(i)->Transform(a);
  eKeep->Transform(a);
}

//_____________________________________________________________________________
void EdbPointsBox2D::Rotate( float angle )
{
  EdbAffine2D aff(1,0,0,1,0,0);
  aff.Rotate(angle);
  Transform(&aff);
}

//_____________________________________________________________________________
void EdbPointsBox2D::Retransform()
{
  eKeep->Invert();
  Transform(eKeep);
  eKeep->Reset();
}

//_____________________________________________________________________________
void EdbPointsBox2D::Substruct( EdbPointsBox2D *b )
{
  for( int i=0; i<N(); i++ )  At(i)->Substruct( b->At(i) );
}

//_____________________________________________________________________________
Float_t EdbPointsBox2D::Xmin()
{
  if( N()<1 )                 return 0;
  Float_t a = At(0)->X();
  for( int i=0; i<N(); i++ )  a = TMath::Min(a,At(i)->X());
  return a;
}

//_____________________________________________________________________________
Float_t EdbPointsBox2D::Xmax()
{
  if( N()<1 )                 return 0;
  Float_t a = At(0)->X();
  for( int i=0; i<N(); i++ )  a = TMath::Max(a,At(i)->X());
  return a;
}

//_____________________________________________________________________________
Float_t EdbPointsBox2D::Ymin()
{
  if( N()<1 )                 return 0;
  Float_t a = At(0)->Y();
  for( int i=0; i<N(); i++ )  a = TMath::Min(a,At(i)->Y());
  return a;
}

//_____________________________________________________________________________
Float_t EdbPointsBox2D::Ymax()
{
  if( N()<1 )                 return 0;
  Float_t a = At(0)->Y();
  for( int i=0; i<N(); i++ )  a = TMath::Max(a,At(i)->Y());
  return a;
}

//_____________________________________________________________________________
TH1F *EdbPointsBox2D::Xhist()
{
  TH1F *hist = new TH1F("xhist","X coordinate of the box",100, Xmin(),Xmax() );
  for( int i=0; i<N(); i++ )  hist->Fill( At(i)->X() );
  return hist;
}

//_____________________________________________________________________________
TH1F *EdbPointsBox2D::Yhist()
{
  TH1F *hist = new TH1F("yhist","Y coordinate of the box",100, Ymin(),Ymax() );
  for( int i=0; i<N(); i++ )  hist->Fill( At(i)->Y() );
  return hist;
}

//_____________________________________________________________________________
TH2F *EdbPointsBox2D::XYhist()
{
  TH2F *hist = new TH2F("xyhist","X coordinate of the box",
			100, Xmin(), Xmax(),
			100, Ymin(), Ymax() );
  for( int i=0; i<N(); i++ )  hist->Fill( At(i)->X(),  At(i)->Y() );
  return hist;
}

//_____________________________________________________________________________
void EdbPointsBox2D::DrawPoints( int   style, int   col, float size )
{
  TGraph  *mark = new TGraph( N() );

  for( int i=0; i<N(); i++ )  
    mark->SetPoint( i, At(i)->X(),  At(i)->Y() );

  printf("EdbPointsBox2D::Draw: %d points\n", N() );

  mark->SetMarkerStyle(style);
  mark->SetMarkerColor(col);
  mark->SetMarkerSize(size);
  mark->SetTitle("Points box");
  mark->Draw("AP");
}

//_____________________________________________________________________________
void EdbPointsBox2D::Print( Option_t *opt ) const
{
  printf("EdbPointsBox2D: %d elements \n", N() );
}

