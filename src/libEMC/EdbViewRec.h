#ifndef ROOT_EdbViewRec
#define ROOT_EdbViewRec
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewRec - Generation of one microscope view                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TArrayF.h"
#include "TArrayI.h"
#include "TArrayC.h"

#include "EdbView.h"
#include "EdbViewDef.h"

//______________________________________________________________________________
class EdbViewRec : public EdbViewDef {
private:

  Float_t eSx;      // bin along x
  Float_t eSy;      // bin along y
  Int_t   eNx;      // "raw"    - divisions along x
  Int_t   eNy;      // "column" - divisions along y
  Int_t   eNcell;   // eNx*eNy

  Int_t   eNtheta;  // divisions by theta
  TArrayF eR;       // R(it) limits definition
  TArrayF eSphi;    // phiStep(it)
  TArrayI eNphi;    // nphi(it)
  Int_t   eNphiTot; // Sum of all nphi(it)
  TArrayF eTmask;   // 
  TArrayF ePmask;   // 

  Long_t  eNtot;    // eNphiTot*eNx*eNy

  TArrayC ePhist;   // "Phase histogram"

public:
  EdbViewRec();
  EdbViewRec(EdbViewDef &vd): EdbViewDef(vd) {SetDefRec();}
  ~EdbViewRec(){}

  void  SetDefRec();

  int bm(EdbView &v);

  //  int It(

  ClassDef(EdbViewRec,1)  // Generation of one microscope view
};

#endif /* ROOT_EdbViewRec */
