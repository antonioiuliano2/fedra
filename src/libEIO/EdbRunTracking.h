#ifndef ROOT_EdbRunTracking
#define ROOT_EdbRunTracking

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunTracking                                                       //
//                                                                      //
// ??????????????????                                                   //
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

  Float_t eDeltaTheta;           // (0.15) slope acceptance
  Float_t eDeltaR;               // (20) 

  Float_t ePreliminaryPulsMinMT; // (6)   \_ preliminary cuts to microtracks candidates for
  Float_t ePreliminaryChi2MaxMT; // (1.6) /  microtracks and basetracks selection

  Float_t ePulsMinMT;            // (10) mimimal number of grains accepted to select microtracks
  Float_t eChi2MaxMT;            // (1.6) maximum chi2 accepted between prediction and microtrack candidates

  Float_t ePulsMinBT;            // not used
  Float_t eChi2MaxBT;            // (1.5) maximum chi2 accepted between prediction and basetrack candidates

  EdbRunTracking() : EdbRunAccess() { Set0(); };
  EdbRunTracking(EdbRun *run) : EdbRunAccess(run) { Set0(); };
  EdbRunTracking(const char *fname) : EdbRunAccess(fname) { Set0(); };

  virtual ~EdbRunTracking();

  void Set0();
  void SetCondMT(EdbScanCond &cond) { eCondMT = cond; }
  void SetCondBT(EdbScanCond &cond) { eCondBT = cond; }

  int UpdateFlag(int flag, int status);
  int GetHoles(int flag);

  int FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr );

  int FindCandidates( EdbSegP &spred, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 );
  int FindCandidateMT( EdbPattern &fnds1, EdbPattern &fnds2, EdbSegP &fnd );
  int FindCandidateBT(EdbPattern &fndbt, EdbSegP &fnd );
  int FindPrediction( EdbSegP &spred, EdbSegP &fnd, EdbSegP &snewpred );

  ClassDef(EdbRunTracking,1)  // helper class for access to the run data
};

#endif /* ROOT_EdbRunTracking */

