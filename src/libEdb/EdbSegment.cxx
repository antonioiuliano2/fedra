//-- Author :  Valeri Tioukov   13.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegment                                                           //
//                                                                      //
// Implementation of Segment class                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_EdbSegment
#include "EdbSegment.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif
 
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
	if(eElements)  {delete eElements; eElements=0;}
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
  if(eElements)  eElements->Delete();
}

//______________________________________________________________________________
void EdbTrack::Set(float x,  float y,  float z,  float tx,  float ty, 
		     float dz, int id)
{
  EdbSeg3D::Set(x,y,z,tx,ty,dz);
  eID   = id;
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
