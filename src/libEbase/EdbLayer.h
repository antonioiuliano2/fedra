#ifndef ROOT_EdbLayer
#define ROOT_EdbLayer

#include "EdbAffine.h"
#include "EdbCell2.h"
#include "EdbSegP.h"
#include "EdbSegCorr.h"

class EdbLayer;

//-------------------------------------------------------------------------------------------------
class EdbCorrectionMap : public EdbCell2
{
 private:
  
 public:
  EdbCorrectionMap(){}
  EdbCorrectionMap(const EdbCorrectionMap &map) : EdbCell2(map){}
  virtual ~EdbCorrectionMap();

  void      Init( EdbCell2 &c );
  void      Init( int nx, float minx, float maxx, int ny, float miny, float maxy );
  
  EdbLayer *GetLayer(float x, float y) { return (EdbLayer *)(GetObject(x, y, 0)); }
  EdbLayer *GetLayer(int i) { return (EdbLayer *)(GetObject(i, 0)); }
  void      CorrectSeg( EdbSegP &s );
  void      ApplyCorrections(EdbCorrectionMap &map);
  void      PrintDZ();
  
  //EdbSegP    *CorrLoc(int j);
  EdbSegCorr CorrLoc(int j);
  EdbSegCorr CorrLoc(float x, float y);

  ClassDef(EdbCorrectionMap,1)  // to keep and apply correction map
};

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

  Int_t   eNcp;               // number of coincidences used for corrections calculation
  
  EdbCorrectionMap  eMap;       // local corrections (if any)

 public:
  EdbLayer();
  virtual ~EdbLayer(){}

  void Copy(EdbLayer &l);
  void CopyCorr(EdbLayer &l);
  EdbCorrectionMap    &Map() { return eMap; }
  int   ID()   const {return eID;}
  int   Ncp()  const {return eNcp;}
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
  void SetNcp(int n )               {eNcp=n;}
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
  
  void SubstructCorrections(EdbLayer &la);
  void ApplyCorrections(EdbLayer &la);
  void ApplyCorrectionsLocal(EdbCorrectionMap &map) { eMap.ApplyCorrections(map); }
  void ApplyCorrections(float shr, float zcorr, EdbAffine2D &affxy, EdbAffine2D &afftxty);
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
  
  
  void CorrectSeg( EdbSegP &s );
  void CorrectSegLocal( EdbSegP &s );

  void Print();

  ClassDef(EdbLayer,5)  // shrinked layer
};

#endif /* ROOT_EdbLayer */
