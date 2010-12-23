#ifndef ROOT_EdbPlateTracking
#define ROOT_EdbPlateTracking

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPlateTracking                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbRun.h"
#include "EdbPattern.h"
#include "EdbLayer.h"
#include "EdbRunAccess.h"
#include "EdbBrick.h"

//class EdbPlateP;

//______________________________________________________________________________
class EdbPlateTracking : public TObject {

 public:

  // --------- input data --------------------------- 

  EdbSegP    ePred;              // prediction to be found in this plate in Brick reference system
  EdbPattern eSide1;             // side 1 microtracks in plate reference system 
  EdbPattern eSide2;             // side 2 microtracks in plate reference system 
  EdbPlateP  ePlate;             // plate geometry and correction parameters to be applied to prediction

  // --------- output: main result --------------------------- 

  EdbSegP eS, eS1, eS2;          // found segments
  EdbSegP eNext;                 // next prediction
  Int_t   eStatus;               // -1-nothing, 0-bt, 1-mt1, 2-mt2

  // --------- processing parameters (can be default) and itermeadiate results ------------------------

  EdbScanCond eCondMT;   // conditions for microtracks
  EdbScanCond eCondBT;   // conditions for basetracks

  /*   Float_t eDeltaRview;           // (400) */
  /*   Float_t eDeltaTheta;           // (0.15) slope acceptance */
  /*   Float_t ePreliminaryPulsMinMT; // (6)   \_ preliminary cuts to microtracks candidates for */
  Float_t ePreliminaryChi2MaxMT; // (1.6) /  microtracks and basetracks selection

  EdbPattern eS1pre;             // the result of the selection of microtracks ordered by chi square
  EdbPattern eS2pre;

  Float_t eDeltaR;               // (20) 

  EdbPattern eSpre;              // the result of the selection of basetracks ordered by chi square

  Float_t ePulsMinBT;            // (18)
  Float_t ePulsMinDegradMT;      // (0)
  Float_t eChi2MaxBT;            // (1.5) maximum chi2 accepted between prediction and basetrack candidates

  EdbPattern eScnd;              // basetracks candidates passed all cuts

  Float_t ePulsMinMT;            // (10) mimimal number of grains accepted to select microtracks
  Float_t ePulsMinDegradBT;      // (0)
  Float_t eChi2MaxMT;            // (1.6) maximum chi2 accepted between prediction and microtrack candidates

  EdbPattern eS1cnd;             // microtrack candidates passed all cuts
  EdbPattern eS2cnd;

  Float_t eDegradPos;            // SigmaX  = SigmaX(0)  + degradPos   * mth
  Float_t eDegradSlope;          // SigmaTX = SigmaTX(0) + degradSlope * bth

  Int_t   eIdp[4];               // to read from sbt 
  Int_t   eIdf[4];

  bool    ePredictionScan;       // if true use GetPatternDataForPrediction( spred.ID(), side, pat ); in FindCandidates (default is false)

 public:

  EdbPlateTracking()
    { 
      Set0(); 
    };

  EdbPlateTracking(EdbPattern &S1,  EdbPattern &S2, EdbSegP &prediction, EdbPlateP &plate)
    { 
      
      for(int i=0;i<S1.N();i++)
	eSide1.AddSegment(*(S1.GetSegment(i)));
      for(int i=0;i<S2.N();i++)
	eSide2.AddSegment(*(S2.GetSegment(i)));
      
      ePred.Copy(prediction);
      ePlate.Copy(plate);
      Set0(); 
    };

  EdbPlateTracking(EdbPattern &S1,  EdbPattern &S2, EdbSegP &prediction)
    { 
      
      for(int i=0;i<S1.N();i++)
	eSide1.AddSegment(*(S1.GetSegment(i)));
      for(int i=0;i<S2.N();i++)
	eSide2.AddSegment(*(S2.GetSegment(i)));
      
      ePred.Copy(prediction);
      Set0(); 
    };

  virtual ~EdbPlateTracking();

  void Set0();
  void SetCondMT(EdbScanCond &cond) { eCondMT = cond; }
  void SetCondBT(EdbScanCond &cond) { eCondBT = cond; }

  void SetPred(const EdbSegP &pred);

  int UpdateFlag(int flag, int status);
  static int GetBTHoles(int flag) { return(flag/10000);     }
  static int GetMTHoles(int flag) { return((flag/100)%100); }
  int ExtrapolateCond(EdbScanCond &inputcond, int flag, EdbScanCond &outputcond );

  int FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr );

  int FindCandidates( EdbSegP &spred, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 );
  int FindCandidateMT( EdbPattern &fnds1, EdbPattern &fnds2, EdbSegP &fnd );
  //  int FindCandidateBT(EdbPattern &fndbt, EdbSegP &fnd );
  int FindBestCandidate(EdbPattern &fndbt, EdbSegP &fnd,EdbPattern &cnd, float wmin, float wmindegrad, float chi2max);
  int FindPrediction( EdbSegP &spred, EdbSegP &fndbt, EdbSegP &fnds1, EdbSegP &fnds2, EdbSegP &snewpred );

  //void TransformToPlateRS( EdbPlateP &plate);
  //void TransformFromPlateRS( EdbPlateP &plate);
  void TransformFromPlateRS();

  int FindTrack(EdbTrackP &pred,EdbTrackP &found, EdbPlateP &plate);

  void Print();

  //void Transform(const EdbAffine2D &aff);
  
  static void   CloseSBtree(TTree *tree);
  static TTree *InitSBtree(const char *file_name="sbt.root", const char *mode="RECREATE");
  bool   UpdateSBtree( TTree &tsbt, int idp[4], int idf[4]);
  bool   GetSBtreeEntry( int entry, TTree &tsbt);
    // int stat, 
    //	       EdbSegP &ps, EdbSegP &fndbt, EdbSegP &fnds1, EdbSegP &fnds2, EdbSegP &nextpred, 
    //	       EdbPattern *scnd=0, EdbPattern *s1cnd=0, EdbPattern *s2cnd=0 );

  ClassDef(EdbPlateTracking,1)  // track following in one emulsion plate
};

#endif /* ROOT_EdbPlateTracking */

