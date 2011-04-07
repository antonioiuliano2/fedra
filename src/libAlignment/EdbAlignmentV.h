#ifndef ROOT_EdbAlignmentV
#define ROOT_EdbAlignmentV

#include "EdbBrick.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbCell1.h"
#include "EdbPatCell2.h"

//-------------------------------------------------------------------------------------------------
class EdbAlignmentV : public TObject
{
 public:

  Float_t    eDVsame[4];    // (dx,dy,dtx,dty) condition for the coinsidence

  EdbCell2   ePC[2];        // 2-d position cells with patterns segments
  Float_t    eXmarg,eYmarg; // margins for the cell definition

  TObjArray  eS[2];         // "parallel" arrays with the selected combinations of segments

  Bool_t     eUseAffCorr;   // if "true" - use eCorrL for corrections
  EdbSegCorr eCorr[2];      // corrections for side 1 and 2 (v[7]) - the result of the alignment
  EdbLayer   eCorrL[2];     // corrections in form of affine transformations - the final output

  EdbH1      eH[2][7];      // conditions for the selection procedure for each var of each side
                            // variables: dx,dy,dz,  dtx,dty, shr, phi
                            //             0  1  2    3   4    5    6 

  EdbH2      eHxy;          // position 2d histo to be used in OptimiseVar2

  TH1I      *eDoubletsRate; // can be filled in FillCombinations()
  
  TFile     *eOutputFile;

 public:
  EdbAlignmentV();
  virtual ~EdbAlignmentV();

  void    InitOutputFile(const char *file="report_al.root", const char *option="RECREATE");
  void    CloseOutputFile();
  void    OptimiseVar1(int side, int ivar,  EdbH2 *hdxy=0, EdbH2 *hdtxy=0);
  void    OptimiseVar2(int side1, int ivar1, int side2, int ivar2, EdbH2 &h12, EdbH2 *hdxy=0, EdbH2 *hdtxty=0);
  Int_t   Ncoins( float dvlim[4], EdbH2 *hdxy=0, EdbH2 *hdtxty=0, TObjArray *sel1=0, TObjArray *sel2=0 );
  Bool_t  ValidCoinsidence(EdbSegP &s1, EdbSegP &s2, float dvlim[4], float dvfound[4] );
  Int_t   FindDiff(TObjArray &arr1, TObjArray &arr2, float dvlim[4], float dvfound[4] );
  Int_t   FindCorrDiff(float dvsame[4], int side=0, int nlim=10);
  float   FineCorrZ(TObjArray &sel1, TObjArray &sel2);
  float   FineCorrPhi(TObjArray &sel1, TObjArray &sel2);
  void    CorrToCoG(int side, EdbPattern &p);
  void    CorrToCoG(int side, TObjArray &p);

  void    HDistance(EdbPattern &p1, EdbPattern &p2, float dxMax, float dyMax);
  void    InitPatCellN(   EdbCell2 &cell, EdbPattern &pat, int nx,     int ny);
  void    InitPatCellBin( int side, EdbPattern &pat, float binx, float biny);
  void    FillCell(int side, EdbPattern &pat);                   // limits should be already defined
  void    FillCell(int side, TObjArray  &pat);                   // limits should be already defined
  void    FillGuessCell(EdbPattern &p1, EdbPattern &p2, float binOK=1., float offsetMax=2000.);
  void    FillGuessCell(TObjArray &p1, TObjArray &p2, float binOK=1., float offsetMax=2000.);
  
  void    FillThetaHist(int side, EdbH2 &htxy);

  int     FillCombinations();
  int     FillCombinations(float dv[4], float dxMax, float dyMax, bool doFill);

  int     DoubletsFilterOut(bool checkview);
  int     SelectIsolated();
  int     SelectBestCouple();
  float   CoupleQuality( EdbSegP &s1, EdbSegP &s2 );

  Bool_t SideOK(int side);
  void   InitHx(    int n, float min, float max) { eH[0][0].InitH1(n, min, max); }
  void   InitHy(    int n, float min, float max) { eH[0][1].InitH1(n, min, max); }
  void   InitHz(    int n, float min, float max) { eH[0][2].InitH1(n, min, max); }
  void   InitHphi(  int n, float min, float max) { eH[0][6].InitH1(n, min, max); }
  void   InitHshr0( int n, float min, float max) { eH[0][5].InitH1(n, min, max); }
  void   InitHshr1( int n, float min, float max) { eH[1][5].InitH1(n, min, max); }

  static Int_t    CheckEqualArr(TObjArray &arr1, TObjArray &arr2);
  Int_t           CalculateAffXY(TObjArray &arr1, TObjArray &arr2, EdbAffine2D &aff);
  Int_t           CalculateAffTXTY(TObjArray &arr1, TObjArray &arr2, EdbAffine2D &aff);
  void            Corr2Aff(EdbLayer &layer);
  void            Corr2Aff(EdbSegCorr &corr, EdbLayer &layer);
  
  float          X(  int side, EdbSegP &s) { return eUseAffCorr? eCorrL[side].X(s)  : eCorr[side].X(s);  }
  float          Y(  int side, EdbSegP &s) { return eUseAffCorr? eCorrL[side].Y(s)  : eCorr[side].Y(s);  }
  float          TX( int side, EdbSegP &s) { return eUseAffCorr? eCorrL[side].TX(s) : eCorr[side].TX(s); }
  float          TY( int side, EdbSegP &s) { return eUseAffCorr? eCorrL[side].TY(s) : eCorr[side].TY(s); }
  char   *StrDVsame() const {return Form("%7.2f %7.2f %8.5f %8.5f",eDVsame[0],eDVsame[1],eDVsame[2],eDVsame[3]); }
  
  float           Xmin(int side,EdbPattern &p);
  float           Xmax(int side,EdbPattern &p);
  float           Ymin(int side,EdbPattern &p);
  float           Ymax(int side,EdbPattern &p);
  float           Xmin(int side,TObjArray &p);
  float           Xmax(int side,TObjArray &p);
  float           Ymin(int side,TObjArray &p);
  float           Ymax(int side,TObjArray &p);
  void      ApplyLimitsOffset( float &xmin1, float &xmax1, float &xmin2, float &xmax2, float offsetMax);
  void      DefineGuessCell( float xmin1, float xmax1, float ymin1, float ymax1, 
					float xmin2, float xmax2, float ymin2, float ymax2, int np1, int np2, float binOK);
  ClassDef(EdbAlignmentV,1)  // universal basic alignment class
};
#endif /* ROOT_EdbAlignmentV */
