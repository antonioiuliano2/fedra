#ifndef ROOT_EdbLayer1
#define ROOT_EdbLayer1

#include "EdbAffine.h"

//______________________________________________________________________________
class EdbLayer : public TObject {

 private:
  Int_t   eID;                // emulsion layer id (11,12 21,22, ...)
  Float_t eZ;                 // the z-coord where X and Y are calculated
  Float_t eZmin,eZmax;        // begin and the end of layer

  Float_t eX;                 // the center (0) of the layer's data
  Float_t eY;                 //
  Float_t eDX;                // acceptance in x:y
  Float_t eDY;                //
  Float_t eTX;                // direction
  Float_t eTY;                //

  Float_t eShr;               // shrinkage along axis z

  EdbAffine2D eAffXY;         // coordinate (XY) affine transformation

  EdbAffine2D eAffTXTY;       // tangents affine transformation

 public:
  EdbLayer();
  virtual ~EdbLayer(){}

  int   ID()   const {return eID;}
  float X()    const {return eX;}
  float Y()    const {return eY;}
  float Z()    const {return eZ;}
  float TX()   const {return eTX;}
  float TY()   const {return eTY;}
  float Zmin() const {return eZmin;}
  float Zmax() const {return eZmax;}
  float DX()   const {return eDX;}
  float DY()   const {return eDY;}
  float Xmin() const {return X()-DX();}
  float Xmax() const {return X()+DX();}
  float Ymin() const {return Y()-DY();}
  float Ymax() const {return Y()+DY();}

  float Shr()  const {return eShr;}

  void SetXY(float x, float y )    { eX=x; eY=y; }
  void SetDXDY(float dx, float dy) { eDX=dx; eDY=dy; }
  void SetTXTY(float tx, float ty )    { eTX=tx; eTY=ty; }
  void SetShrinkage(float shr) {eShr=shr;}
  void SetZlayer(float z,float zmin,float zmax) { eZ=z; eZmin=zmin; eZmax=zmax; }
  void SetAffXY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffXY.Set(a11,a12,a21,a22,b1,b2);}
  void SetAffTXTY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffTXTY.Set(a11,a12,a21,a22,b1,b2);}

  EdbAffine2D *GetAffineXY()   {return &eAffXY;}
  EdbAffine2D *GetAffineTXTY() {return &eAffTXTY;}

  void Print();

  ClassDef(EdbLayer,1)  // shrinked layer
};

#endif /* ROOT_EdbLayer */
