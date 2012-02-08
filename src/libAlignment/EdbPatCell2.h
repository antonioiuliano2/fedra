#ifndef ROOT_EdbPatCell2
#define ROOT_EdbPatCell2

#include "EdbLayer.h"
#include "EdbPattern.h"
#include "EdbCell2.h"

//-------------------------------------------------------------------------------------------------
class EdbPatCell2 : public EdbCell2
{
public:
  float eDXlim,  eDYlim;       // acceptance limits for the combinations selection
  float eDTXlim, eDTYlim;      // 

  float eXmarg, eYmarg;         // margins for the cell definition

  bool  eApplyCorr;
  float eDX, eDY, eDZ;   // corrections to be applied if eApplyCorr==true
  float eShr;            // corrections to be applied if eApplyCorr==true
  float eDTX, eDTY;      // corrections to be applied if eApplyCorr==true
  float ePhi;            // rotation angle (using dx,dy one can set-up the center of rotation)

  TH1I *eDoubletsRate;          // to be filled in FillCombinations()

public:
  EdbPatCell2();
  virtual ~EdbPatCell2(){ResetCorr();}

  void  ResetCorr();
  void  PrintCorr();
  void  InitPat(EdbPattern &pat, int nx, int ny);   // define limits to fit the pattern
  void  InitPatBin(EdbPattern &pat, float binx, float biny);   // define limits to fit the pattern

  int   FillCell(EdbPattern &pat);                   // limits should be already defined
  int   FillCell(TObjArray  &pat);                   // limits should be already defined

  int   FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arrC1, TObjArray &arrC2, bool doFill=1);


  float Xs(EdbSegP &s)  { if(!eApplyCorr) return s.eX; if(ePhi<0.00001) return XsPos(s); else return XsRot(s); }
  float Ys(EdbSegP &s)  { if(!eApplyCorr) return s.eY; if(ePhi<0.00001) return YsPos(s); else return YsRot(s); }

  float XsPos(EdbSegP &s)  { return eApplyCorr? eDX + s.eX + TXs(s)*eDZ : s.eX; }
  float YsPos(EdbSegP &s)  { return eApplyCorr? eDY + s.eY + TYs(s)*eDZ : s.eY; }

  float XsRot(EdbSegP &s)  { return  XsPos(s)*TMath::Cos(ePhi)-YsPos(s)*TMath::Sin(ePhi); }
  float YsRot(EdbSegP &s)  { return  XsPos(s)*TMath::Sin(ePhi)+YsPos(s)*TMath::Cos(ePhi); }

  float TXs(EdbSegP &s) { return eApplyCorr? s.eTX/eShr+eDTX : s.eTX; }
  float TYs(EdbSegP &s) { return eApplyCorr? s.eTY/eShr+eDTY : s.eTY; }

  void ApplyCorrections(EdbPattern &pat);

  void ApplyCorrections(EdbLayer &layer);
  void AcceptCorrections(EdbLayer &layer);

  EdbH2 *FillSelectedH2();

  ClassDef(EdbPatCell2,1)  // helper class to analyse the single pattern
};

#endif /* ROOT_EdbPatCell2 */
