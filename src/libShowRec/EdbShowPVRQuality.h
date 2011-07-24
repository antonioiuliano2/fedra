#ifndef ROOT_EdbShowPVRQuality
#define ROOT_EdbShowPVRQuality
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TF1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TCanvas.h"
#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"
#include "EdbShowerP.h"
using namespace std;

//______________________________________________________________________________
//___
//___			Declaration of EdbShowPVRQuality Class:
//______________________________________________________________________________


//______________________________________________________________________________

class EdbShowPVRQuality : public TObject {

private:

    // The source and target EdbPVRec objects:
    EdbPVRec* eAli_orig;
    EdbPVRec* eAli_modified;
    Bool_t    eIsSource;
    Bool_t    eIsTarget;
    Int_t     eHistGeometry;
    Int_t     eAli_maxNpatterns;

    // Variables related for calculation Issues
    Int_t		eCutMethod;
    Float_t		eBTDensityLevel;
    Bool_t		eCutMethodIsDone[2];
    Bool_t		eBTDensityLevelCalcMethodMC;
    Int_t		eBTDensityLevelCalcMethodMCConfirmationNumber;

    Float_t		ePatternBTDensity_orig[114];
    Float_t		ePatternBTDensity_modified[114];

    // Histograms for calculations
    TH2F*			eHistChi2W;
    TH2F*			eHistYX;
    Int_t			NbinsX,NbinsY;

    Float_t		minX,maxX;
    Float_t		minY,maxY;
    // TProfile: BT dens/mm2 versus PID()	source histogram
    TProfile* 		eProfileBTdens_vs_PID_source;
    Float_t  		eProfileBTdens_vs_PID_source_meanX,eProfileBTdens_vs_PID_source_meanY;
    Float_t  		eProfileBTdens_vs_PID_source_rmsX,eProfileBTdens_vs_PID_source_rmsY;
    // TProfile: BT dens/mm2 versus PID()	target histogram
    TProfile* 		eProfileBTdens_vs_PID_target;
    Float_t  		eProfileBTdens_vs_PID_target_meanX,eProfileBTdens_vs_PID_target_meanY;
    Float_t  		eProfileBTdens_vs_PID_target_rmsX,eProfileBTdens_vs_PID_target_rmsY;

    // Variables related for cut Issues
    // eCutMethod == 0: Constant BT density
    // eCutMethod == 1: Constant Chi2W quality
    Float_t		eCutp0[114];
    Float_t		eCutp1[114];
    Float_t		eCutDistChi2[114];
    Float_t		eCutDistW[114];
    Float_t 	eAgreementChi2WDistCut[114];
    Float_t 	eAgreementChi2CutMeanChi2;
    Float_t		eAgreementChi2CutRMSChi2;
    Float_t 	eAgreementChi2CutMeanW;
    Float_t 	eAgreementChi2CutRMSW;

protected:

    void 								Set0();
    void 								Init();

public:

    EdbShowPVRQuality();
    EdbShowPVRQuality(EdbPVRec* ali);
    EdbShowPVRQuality(EdbPVRec* ali, Float_t BTDensityTargetLevel);

    void SetCutMethod(Int_t CutMethod);
    inline void SetBTDensityLevel(Float_t BTDensityLevel) {
        eBTDensityLevel=BTDensityLevel;
    }

    inline void SetBTDensityLevelCalcMethodMC(Bool_t BTDensityLevelCalcMethodMC) {
        eBTDensityLevelCalcMethodMC=BTDensityLevelCalcMethodMC;
    }
    inline void SetBTDensityLevelCalcMethodMCConfirmation(Int_t BTDensityLevelCalcMethodMCConfirmationNumber) {
        eBTDensityLevelCalcMethodMCConfirmationNumber=BTDensityLevelCalcMethodMCConfirmationNumber;
    }
    inline Bool_t GetBTDensityLevelCalcMethodMC() {
        return eBTDensityLevelCalcMethodMC;
    }
    inline Int_t GetBTDensityLevelCalcMethodMCConfirmation() {
        return eBTDensityLevelCalcMethodMCConfirmationNumber;
    }



    inline EdbPVRec* GetEdbPVRec() {
        return eAli_orig;
    }
    inline EdbPVRec* GetEdbPVRec(Int_t EdbPVRecType) {
        cout << "Inline EdbPVRecType= " <<  EdbPVRecType << endl;
        if (EdbPVRecType==1) {
            return eAli_modified;
        }
        else {
            return eAli_orig;
        }
    }

    inline EdbPVRec* GetEdbPVRec_orig() {
        return GetEdbPVRec(0);
    }
    inline EdbPVRec* GetEdbPVRec_modified() {
        return GetEdbPVRec(1);
    }

    inline void				SetEdbPVRec(EdbPVRec* Ali_orig) {
        eAli_orig=Ali_orig;
        eIsSource=kTRUE;
        eAli_maxNpatterns=Ali_orig->Npatterns();
    }

    inline   TH2F* GetHistChi2W() {
        return eHistChi2W;
    }
    inline   TH2F* GetHistYX() {
        return eHistYX;
    }



    inline Int_t     GetCutMethod() {
        return eCutMethod;
    }
    inline Bool_t    GetCutMethodIsDone(Int_t type) {
        if (type>1) return 0;
        return eCutMethodIsDone[type];
    }
    inline Float_t   GetBTDensityLevel() {
        return eBTDensityLevel;
    }

    inline Float_t*   GetCutp0() {
        return eCutp0;
    }
    inline Float_t*   GetCutp1() {
        return eCutp1;
    }
    inline Float_t    GetCutp0(Int_t patNR) {
        return eCutp0[patNR];
    }
    inline Float_t    GetCutp1(Int_t patNR) {
        return eCutp1[patNR];
    }

    inline Float_t    GetagreementChi2CutMeanChi2() {
        return eAgreementChi2CutMeanChi2;
    }
    inline Float_t    GetagreementChi2CutRMSChi2() {
        return eAgreementChi2CutRMSChi2;
    }
    inline Float_t    GetagreementChi2CutMeanW() {
        return eAgreementChi2CutMeanW;
    }
    inline Float_t    GetagreementChi2CutRMSW() {
        return eAgreementChi2CutRMSW;
    }

    inline Float_t*   GetagreementChi2Cut() {
        return eAgreementChi2WDistCut;
    }
    inline Float_t    GetagreementChi2Cut(Int_t patNR) {
        return eAgreementChi2WDistCut[patNR];
    }

    void SetHistGeometry_OPERA();
    void SetHistGeometry_MC();
    void SetHistGeometry_OPERAandMC();

    void CheckEdbPVRec();
    void Execute_ConstantBTDensity();
    void Execute_ConstantQuality();

    EdbPVRec* Remove_DoubleBT(EdbPVRec* aliSource);
    EdbPVRec* Remove_Passing(EdbPVRec* aliSource);

    // All these functions will call Remove_SegmentArray(..,..,..);
    EdbPVRec* Remove_SegmentArray(TObjArray* segarray, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_Segment(EdbSegP* seg, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_TrackArray(TObjArray* trackArray, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_Track(EdbTrackP* track, EdbPVRec* aliSource=NULL, Int_t Option=0);
    TObjArray* TrackToSegmentArray(EdbTrackP* track);
    TObjArray* TrackArrayToSegmentArray(TObjArray* trackArray);



    void CreateEdbPVRec();
    void CheckFilledXYSize();
    Int_t FindFirstBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    Int_t FindLastBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    TObjArray* GetTracksFromLinkedTracksRootFile();

    Bool_t CheckSegmentQualityInPattern_ConstBTDens(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    Bool_t CheckSegmentQualityInPattern_ConstQual(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);

    virtual ~EdbShowPVRQuality();          // virtual constructor due to inherited class

    void Print();
    void PrintCutType();
    void PrintCutType0();
    void PrintCutType1();
    void Help();
    ClassDef(EdbShowPVRQuality,1);         // Root Class Definition for EdbShowPVRQuality
};

//______________________________________________________________________________

#endif /* EdbShowPVRQuality */
