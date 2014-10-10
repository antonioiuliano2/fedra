//-- Author :  Valeri Tioukov   13.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegment                                                           //
//                                                                      //
// Implementation of Segment class                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TClass.h>
#include "EdbSegment.h"
#include "EdbAffine.h"
 
ClassImp(EdbSeg3D)
ClassImp(EdbSegment)
ClassImp(EdbTrack)
 
//______________________________________________________________________________
void EdbSeg3D::Set(float x,  float y,  float z,  float tx,  float ty, float dz)
{
  eX0 =  x;
  eY0 =  y;
  eZ0 =  z;
  eTx = tx;
  eTy = ty;
  eDz = dz;
}

//______________________________________________________________________________
void EdbSeg3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbSeg3D.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbSeg3D::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      TObject::Streamer(R__b);
      R__b >> eX0;
      R__b >> eY0;
      R__b >> eZ0;
      R__b >> eTx;
      R__b >> eTy;
      R__b >> eDz;
      R__b.CheckByteCount(R__s, R__c, EdbSeg3D::IsA());
      //====end of old versions
   } else {
     EdbSeg3D::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbSeg3D::Transform( const EdbAffine2D *a )
{
  float tx,ty,x0,y0;
  tx =  a->A11()*eTx + a->A12()*eTy;       // rotate angles
  ty =  a->A21()*eTx + a->A22()*eTy;

  x0 =  a->A11()*eX0 + a->A12()*eY0 + a->B1();
  y0 =  a->A21()*eX0 + a->A22()*eY0 + a->B2();

  eTx=tx;  eTy=ty;  eX0=x0;  eY0=y0;
}

//______________________________________________________________________________
EdbSegment::EdbSegment()
{
  eSide      = -1;
  eElements  =  0;
}

//______________________________________________________________________________
EdbSegment::EdbSegment( float x, float y, float z, float tx, float ty, 
			float dz, int side, int puls, int id ):
  EdbSeg3D( x, y, z, tx, ty, dz), eSide(side), ePuls(puls), eID(id)
{
  eElements  =  0;
}

//______________________________________________________________________________
EdbSegment::~EdbSegment()
{
  if(eElements)  {eElements->Clear(); SafeDelete(eElements);}
}

//______________________________________________________________________________
void EdbSegment::Set(float x,  float y,  float z,  float tx,  float ty, 
		     float dz, int side, int puls, int id)
{
  EdbSeg3D::Set(x,y,z,tx,ty,dz);
  eSide = side;
  ePuls = puls;
  eID   = id;
}

//______________________________________________________________________________
void EdbSegment::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbSegment.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 2) {
	EdbSegment::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      EdbSeg3D::Streamer(R__b);
      R__b >> eSide;
      R__b >> ePuls;
      R__b >> eID;
      R__b >> eSigmaX;
      R__b >> eSigmaY;
      R__b.CheckByteCount(R__s, R__c, EdbSegment::IsA());
      //====end of old versions
   } else {
     EdbSegment::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbSegment::Copy(EdbSegment &s)
{
  //copy contents of s into this
  Set( s.GetX0(),s.GetY0(),s.GetZ0(),s.GetTx(),s.GetTy(),s.GetDz(), s.GetSide(),s.GetPuls(), s.GetID() );
  SetSigma(s.GetSigmaX(),s.GetSigmaY());
  if(eElements) eElements->Clear();
  int n = s.GetNelements();
  for(int i=0; i<n; i++) AddElement( s.GetElements()->UncheckedAt(i) );
}

//______________________________________________________________________________
void EdbSegment::Print( Option_t *opt ) const
{
  printf("EdbSegment: %f %f %f \t %f %f %f \t %d %d %d \t %d\n",
         GetX0(), GetY0(), GetZ0(), 
	 GetTx(), GetTy(), GetDz(), 
	 GetSide(), GetPuls(), GetID(),
	 GetNelements());
}

//______________________________________________________________________________
void EdbSegment::AddElement( TObject *element )
{
  if(!eElements) eElements = new TObjArray();
  eElements->Add(element);
}


//______________________________________________________________________________
EdbTrack::EdbTrack()
{
  eElements  =  0;
}

//______________________________________________________________________________
EdbTrack::EdbTrack( float x, float y, float z, float tx, float ty, 
			float dz, int id ):
  EdbSeg3D( x, y, z, tx, ty, dz), eID(id)
{
  eElements  =  0;
}

//______________________________________________________________________________
EdbTrack::~EdbTrack()
{
  if(eElements)  {eElements->Clear(); SafeDelete(eElements);}
}

//______________________________________________________________________________
void EdbTrack::Set(float x,  float y,  float z,  float tx,  float ty, 
		     float dz, int id)
{
  EdbSeg3D::Set(x,y,z,tx,ty,dz);
  eID   = id;
}

//______________________________________________________________________________
void EdbTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbTrack.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 2) {
	EdbTrack::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      EdbSeg3D::Streamer(R__b);
      R__b >> eID;
      R__b.CheckByteCount(R__s, R__c, EdbTrack::IsA());
      //====end of old versions
   } else {
     EdbTrack::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbTrack::Print( Option_t *opt ) const
{
  printf("EdbTrack: %f %f %f \t %f %f %f \t %d %d\n",
         GetX0(), GetY0(), GetZ0(), 
	 GetTx(), GetTy(), GetDz(), 
	 GetID(), GetNelements());
}

//______________________________________________________________________________
void EdbTrack::AddElement( TObject *element )
{
  if(!eElements) eElements = new TObjArray();
  eElements->Add(element);
}
