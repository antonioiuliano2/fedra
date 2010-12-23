#ifndef ROOT_EdbScanCond
#define ROOT_EdbScanCond

#include "TNamed.h"
#include "TMatrixD.h"
#include "TMath.h"

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
  Int_t   eChi2Mode;          // mode of coupling chi2 calculation (default is 0)

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

  void  SetChi2Mode(int mode)  {eChi2Mode=mode;}
  int   Chi2Mode()       const {return eChi2Mode;}

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

  void FillErrorsCov( float tx,float ty, TMatrixD &cov );

  void Print() const;

  ClassDef(EdbScanCond,1)  // Scanning Conditions Parameters
};

#endif /* ROOT_EdbScanCond */
