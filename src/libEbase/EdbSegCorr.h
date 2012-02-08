#ifndef ROOT_EdbSegCorr
#define ROOT_EdbSegCorr

#include "TArrayF.h"
#include "EdbSegP.h"

class EdbSegCorr : public TObject
{
 private:
  TArrayF  eV;           // variables: dx,dy,dz, dtx,dty, shr, phy
                         //             0  1  2    3   4    5    6
 public:
  Bool_t   eApplyCorr;   // do correction

 public:
  EdbSegCorr() { eApplyCorr=true; ResetCorr(); }
  virtual ~EdbSegCorr(){}

  void   ResetCorr() { eV.Set(10); eV[5]=1.; }

  void   SetV(int i, float x) {eV[i]=x;}
  void   AddV(int i, float x) {eV[i]+=x;}
  float  V(int i) { return eV[i]; }

  float X(EdbSegP &s)     { if(!eApplyCorr) return s.eX; if(TMath::Abs(eV[6])<0.00001) return Xpos(s); else return Xrot(s); }
  float Y(EdbSegP &s)     { if(!eApplyCorr) return s.eY; if(TMath::Abs(eV[6])<0.00001) return Ypos(s); else return Yrot(s); }

  float Xpos(EdbSegP &s)  { return eApplyCorr? eV[0] + s.eX + TX(s)*eV[2] : s.eX; }
  float Ypos(EdbSegP &s)  { return eApplyCorr? eV[1] + s.eY + TY(s)*eV[2] : s.eY; }

  float Xrot(EdbSegP &s)  { return  Xpos(s)*TMath::Cos(eV[6])-Ypos(s)*TMath::Sin(eV[6]); }
  float Yrot(EdbSegP &s)  { return  Xpos(s)*TMath::Sin(eV[6])+Ypos(s)*TMath::Cos(eV[6]); }

  float TX(EdbSegP &s)    { return eApplyCorr? s.eTX/eV[5]+eV[3] : s.eTX; }
  float TY(EdbSegP &s)    { return eApplyCorr? s.eTY/eV[5]+eV[4] : s.eTY; }
  float T(EdbSegP &s)     { return TMath::Sqrt( TX(s)*TX(s) + TY(s)*TY(s) ); }
  
  void  ApplyCorrections( EdbSegP &s );

  void Print();

  ClassDef(EdbSegCorr,1)  // segment corrections as separated variables
};

#endif /* EdbSegCorr */
