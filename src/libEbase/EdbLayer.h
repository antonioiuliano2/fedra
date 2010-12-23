#ifndef ROOT_EdbLayer
#define ROOT_EdbLayer

#include "EdbAffine.h"
#include "EdbSegP.h"

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

  // corrections to be applied for the segments of this layer:

  Float_t eShr;               // shrinkage along axis z
  EdbAffine2D eAffXY;         // coordinate (XY) affine transformation
  EdbAffine2D eAffTXTY;       // tangents affine transformation
  Float_t eZcorr;             // z-correction 

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
  float DZ()   const {return eZmax-eZmin;}
  float Xmin() const {return X()-DX();}
  float Xmax() const {return X()+DX();}
  float Ymin() const {return Y()-DY();}
  float Ymax() const {return Y()+DY();}
  float Shr()  const {return eShr;}
  float Zcorr()  const {return eZcorr;}
  bool  IsInside(float x, float y, float z);
  bool  IsInside(float x, float y);

  void SetID(int id )               {eID=id;}
  void SetXY(float x, float y )     {eX=x; eY=y;}
  void SetDXDY(float dx, float dy)  {eDX=dx; eDY=dy;}
  void SetTXTY(float tx, float ty ) {eTX=tx; eTY=ty;}
  void SetShrinkage(float shr)      {eShr=shr;}
  void SetZ(float z) {eZ=z;}
  void SetZlayer(float z,float zmin,float zmax) { eZ=z;eZmin=zmin;eZmax=zmax; }
  void SetAffXY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffXY.Set(a11,a12,a21,a22,b1,b2);}
  void SetAffTXTY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffTXTY.Set(a11,a12,a21,a22,b1,b2);}
  void SetZcorr(float zcorr) {eZcorr = zcorr;}
  void ShiftZ(float dz);
  void ApplyCorrections(float shr, float zcorr, EdbAffine2D &affxy, EdbAffine2D &afftxty);
  void Copy(EdbLayer &l);
  void CopyCorr(EdbLayer &l);
  void ResetCorr();

  EdbAffine2D *GetAffineXY()   {return &eAffXY;}
  EdbAffine2D *GetAffineTXTY() {return &eAffTXTY;}

  float TXp(EdbSegP &s) { return (eAffXY.A11()*s.eTX + eAffXY.A12()*s.eTY) / eShr; } // apply shrinkage and rotation 
  float TYp(EdbSegP &s) { return (eAffXY.A21()*s.eTX + eAffXY.A22()*s.eTY) / eShr; } // of the main transform: eAffXY
  float TX(EdbSegP &s)  { return eAffTXTY.A11()*TXp(s) + eAffTXTY.A12()*TYp(s) + eAffTXTY.B1(); }
  float TY(EdbSegP &s)  { return eAffTXTY.A21()*TXp(s) + eAffTXTY.A22()*TYp(s) + eAffTXTY.B2(); }

  float Xp(EdbSegP &s)  { return eAffXY.A11()*s.eX + eAffXY.A12()*s.eY + eAffXY.B1(); } // only position transform
  float Yp(EdbSegP &s)  { return eAffXY.A21()*s.eX + eAffXY.A22()*s.eY + eAffXY.B2(); }
  float X(EdbSegP &s)   { return Xp(s) + TX(s)*eZcorr; } // apply propagation
  float Y(EdbSegP &s)   { return Yp(s) + TY(s)*eZcorr; }

  void Print();

  ClassDef(EdbLayer,2)  // shrinked layer
};

//______________________________________________________________________________
class EdbSegmentCut : public TObject {

 private:
  Int_t    eXI;         // 0-exclusive; 1-inclusive cut
  Float_t  eMin[5];     // min  x:y:tx:ty:puls
  Float_t  eMax[5];     // max  x:y:tx:ty:puls

 public:
  EdbSegmentCut() {}
  EdbSegmentCut( int xi, float var[10] );
  virtual ~EdbSegmentCut() {}

  void SetXI(int xi)           {eXI=xi;}
  void SetMin(  float min[5] ) { for(int i=0;i<5;i++) eMin[i]=min[i]; }
  void SetMax(  float max[5] ) { for(int i=0;i<5;i++) eMax[i]=max[i]; }
  int   XI() const {return eXI;}
  float Min(int i) const {return eMin[i];}
  float Max(int i) const {return eMax[i];}
  int  PassCut( float var[5] );
  int  PassCutX( float var[5] );
  int  PassCutI( float var[5] );
  void Print();
  const char *CutLine(char *str, int i=0, int j=0) const;

  ClassDef(EdbSegmentCut,1)  // segment cut
};

#endif /* ROOT_EdbLayer */
