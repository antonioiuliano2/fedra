#ifndef ROOT_EdbRunTracking
#define ROOT_EdbRunTracking

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunTracking                                                       //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbRun.h"
#include "EdbPattern.h"
#include "EdbLayer.h"
#include "EdbRunAccess.h"

//______________________________________________________________________________
class EdbRunTracking : public EdbRunAccess {

 public:

  EdbScanCond eCondMT;   // conditions for microtracks
  EdbScanCond eCondBT;   // conditions for basetracks

  Float_t eDeltaRview;           // (400)
  Float_t eDeltaTheta;           // (0.15) slope acceptance
  Float_t ePreliminaryPulsMinMT; // (6)   \_ preliminary cuts to microtracks candidates for
  Float_t ePreliminaryChi2MaxMT; // (1.6) /  microtracks and basetracks selection

  EdbPattern eS1pre;             // the result of the preliminary selection of microtracks
  EdbPattern eS2pre;

  Float_t eDeltaR;               // (20) 

  EdbPattern eSpre;              // preliminary selected basetracks

  Float_t ePulsMinBT;            // (18)
  Float_t eChi2MaxBT;            // (1.5) maximum chi2 accepted between prediction and basetrack candidates

  EdbPattern eScnd;              // basetracks candidates passed all cuts

  Float_t ePulsMinMT;            // (10) mimimal number of grains accepted to select microtracks
  Float_t eChi2MaxMT;            // (1.6) maximum chi2 accepted between prediction and microtrack candidates

  EdbPattern eS1cnd;             // microtrack candidates passed all cuts
  EdbPattern eS2cnd;

  EdbSegP ePred;                 // prediction
  EdbSegP eS, eS1, eS2;          // found segments
  EdbSegP eNext;                 // next prediction
  Int_t   eStatus;               // -1-nothing, 0-bt, 1-mt1, 2-mt2

  Float_t eDegradPos;            // SigmaX  = SigmaX(0)  + degradPos   * mth
  Float_t eDegradSlope;          // SigmaTX = SigmaTX(0) + degradSlope * bth

 public:

  EdbRunTracking() : EdbRunAccess() { Set0(); };
  EdbRunTracking(EdbRun *run) : EdbRunAccess(run) { Set0(); };
  EdbRunTracking(const char *fname) : EdbRunAccess(fname) { Set0(); };

  virtual ~EdbRunTracking();

  void Set0();
  void SetCondMT(EdbScanCond &cond) { eCondMT = cond; }
  void SetCondBT(EdbScanCond &cond) { eCondBT = cond; }

  void SetPred(const EdbSegP &pred);

  int UpdateFlag(int flag, int status);
  int GetBTHoles(int flag);
  int GetMTHoles(int flag);
  int ExtrapolateCond(EdbScanCond &inputcond, int flag, EdbScanCond &outputcond );

  int FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr );

  int FindCandidates( EdbSegP &spred, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 );
  int FindCandidateMT( EdbPattern &fnds1, EdbPattern &fnds2, EdbSegP &fnd );
  //  int FindCandidateBT(EdbPattern &fndbt, EdbSegP &fnd );
  int FindBestCandidate(EdbPattern &fndbt, EdbSegP &fnd,EdbPattern &cnd, float wmin, float chi2max);
  int FindPrediction( EdbSegP &spred, EdbSegP &fndbt, EdbSegP &fnds1, EdbSegP &fnds2, EdbSegP &snewpred );
  void Print();

  //void Transform(const EdbAffine2D &aff);
  
  static void   CloseSBtree(TTree *tree);
  static TTree *InitSBtree(const char *file_name="sbt.root", const char *mode="RECREATE");
  bool   UpdateSBtree( TTree &tsbt, int idp[4], int idf[4]);
    // int stat, 
    //	       EdbSegP &ps, EdbSegP &fndbt, EdbSegP &fnds1, EdbSegP &fnds2, EdbSegP &nextpred, 
    //	       EdbPattern *scnd=0, EdbPattern *s1cnd=0, EdbPattern *s2cnd=0 );

  ClassDef(EdbRunTracking,1)  // helper class for access to the run data
};

#endif /* ROOT_EdbRunTracking */

