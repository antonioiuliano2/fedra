#ifndef ROOT_EdbLayer
#define ROOT_EdbLayer

#include "TNamed.h"
#include "TMath.h"
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
  bool  IsInside(float x, float y, float z);

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

//______________________________________________________________________________
class EdbScanCond : public TNamed {
  //
  //  Keep the accuracy parameters for 1 pattern/layer
  // 
 private:

  // grain:
  Float_t eSigmaXgr;
  Float_t eSigmaYgr;
  Float_t eSigmaZgr;

  // segment:
  Float_t  eDegrad;    // angular degradation of parameters: S = S0*(1 + eDegrad*Ang)

  Float_t  eSigmaX0;   // [microns]  Parameters at 0 angle
  Float_t  eSigmaY0;   // [microns]  SigmaX = S0*(1+eDegrad*Ax)
  Float_t  eSigmaZ0;   // z - uncertancy
  Float_t  eSigmaTX0;  // ["rad"]
  Float_t  eSigmaTY0;  // ["rad"]

  // puls height parametrizations:

  Float_t ePuls0[2];   // signal/all is parametrised as linear in range Pmin,Pmax
  Float_t ePuls04[2];  // - at angle .4 rad

  Float_t eBinX,eBinY;        // bins [normalized to Sigma()]
  Float_t eBinTX,eBinTY;      //

  Float_t eChi2Max;           //
  Float_t eChi2PMax;          //

  Float_t eOffX, eOffY;       // maximal offsets in x and y - the accuracy of pattern 
                              // itself in respect to the upper level RS

  Float_t eRadX0;             // radiation length for ECC media [microns]

 public:
  EdbScanCond();
  virtual ~EdbScanCond(){}

  void SetDefault();

  void SetRadX0( float x0 ) {eRadX0=x0;}
  float RadX0() const {return eRadX0;}

  void SetSigmaGR(  float sx, float sy, float sz ) 
    { eSigmaXgr=sx;  eSigmaYgr=sy;  eSigmaZgr=sz; }
  void SetSigma0(  float x, float y, float tx, float ty ) 
    { eSigmaX0=x;  eSigmaY0=y;  eSigmaTX0=tx;  eSigmaTY0=ty;  }
  void SetBins(float bx, float by, float btx, float bty) 
    { eBinX=bx; eBinY=by; eBinTX=btx; eBinTY=bty; }

  void SetOffset( float x, float y ) {eOffX=x; eOffY=y;}
  float OffX() const {return eOffX;}
  float OffY() const {return eOffY;}
  void SetDegrad( float d ) {eDegrad=d;}
  void SetSigmaZ0( float z ) {eSigmaZ0=z;}

  void SetPulsRamp0(  float p1, float p2 )  {ePuls0[0]=p1; ePuls0[1]=p2;}
  void SetPulsRamp04(  float p1, float p2 ) {ePuls04[0]=p1; ePuls04[1]=p2;}
  float BinX()  const {return eBinX;}
  float BinY()  const {return eBinY;}
  float BinTX() const {return eBinTX;}
  float BinTY() const {return eBinTY;}

  void  SetChi2Max(float chi2)  {eChi2Max=chi2;}
  void  SetChi2PMax(float chi2) {eChi2PMax=chi2;}
  float Chi2Max()    const {return eChi2Max;}
  float Chi2PMax()   const {return eChi2PMax;}

  float StepX(float dz)   const;
  float StepY(float dz)   const;
  float StepTX(float tx)  const { return BinTX()*SigmaTX(tx); }
  float StepTY(float ty)  const { return BinTY()*SigmaTY(ty); }

  float SigmaXgr()  const { return eSigmaXgr; }
  float SigmaYgr()  const { return eSigmaYgr; }
  float SigmaZgr()  const { return eSigmaZgr; }

  float SigmaX(float ax)  const { return eSigmaX0*(1. + TMath::Abs(ax)*eDegrad); }
  float SigmaY(float ay)  const { return eSigmaY0*(1. + TMath::Abs(ay)*eDegrad); }
  float SigmaZ(float ax, float ay)  const 
    { return eSigmaZ0; } // TODO
  float SigmaTX(float ax) const { return eSigmaTX0*(1. + TMath::Abs(ax)*eDegrad); }
  float SigmaTY(float ay) const { return eSigmaTY0*(1. + TMath::Abs(ay)*eDegrad); }

  float Ramp(float x, float x1, float x2) const;

  float ProbSeg( float tx, float ty, float puls) const;
  float ProbSeg( float t, float puls) const; 

  void Print() const;

  ClassDef(EdbScanCond,1)  // Scanning Conditions Parameters
};

#endif /* ROOT_EdbLayer */
