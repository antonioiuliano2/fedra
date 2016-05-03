#ifndef ROOT_EdbPVRQuality
#define ROOT_EdbPVRQuality
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
#include "TSpectrum.h"
#include "TPolyMarker.h"
#include "TRandom3.h"

#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"
// #include "EdbShowerP.h"
using namespace std;

//______________________________________________________________________________
//___
//___			Declaration of EdbPVRQuality Class:
//______________________________________________________________________________


//______________________________________________________________________________

class EdbPVRQuality : public TObject {

private:

    // The source and target EdbPVRec objects:
    EdbPVRec* eAli_orig;
    EdbPVRec* eAli_modified;
    Bool_t    eNeedModified;
    Bool_t    eIsSource;
    Bool_t    eIsTarget;
    Int_t     eHistGeometry;
    Int_t     eAli_maxNpatterns;

    // Variables related for calculation Issues
    Int_t		eCutMethod;
    TString		eCutMethodString;
    Bool_t		eCutMethodIsDone[7];

    Float_t		eBTDensityLevel;
    Float_t		eBTDensityLevelAngularSpace[20];

    Bool_t		eBTDensityLevelCalcMethodMC;
    Int_t		eBTDensityLevelCalcMethodMCConfirmationNumber;

    Float_t		ePatternBTDensity_orig[114];
    Float_t		ePatternBTDensity_modified[114];

    // Histograms for calculations and crosscheck drawings
    TH2F*		eHistChi2W;
    TH2F*		eHistYX;
    TH2F*		eHistTYTX;
    TH1F*		eHistTT;
    TH2F*		eHistTanTheta;
    Int_t		NbinsX,NbinsY;
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
    // For all eCutMethod in TanTheta Space:
    Float_t	eCutTTSqueezeFactor;
    // eCutMethod == 0: Constant BT density
    Float_t	eCutp0[114];
    Float_t	eCutp1[114];
    // eCutMethod == 1: Constant BT density also in tangens theta space
    Float_t	eCutTTp0[114][20];
    Float_t	eCutTTp1[114][20];
    // eCutMethod == 2: Constant Chi2W quality
    Float_t	eCutDistChi2[114];
    Float_t	eCutDistW[114];
    Float_t 	eAgreementChi2WDistCut[114];
    Float_t 	eAgreementChi2CutMeanChi2;
    Float_t	eAgreementChi2CutRMSChi2;
    Float_t 	eAgreementChi2CutMeanW;
    Float_t 	eAgreementChi2CutRMSW;
    // eCutMethod == 3: Constant Chi2W quality also in tangens theta space
    /// TO BE IMPLEMENTED HERE ...
    /// ... ... ...
    /// ... ... ...
    /// ... ... ...
    // eCutMethod == 4: Constant X2Hat BT density
    Float_t	eX2Hat;
    Float_t	eX2HatCut[114];
    Float_t	eXi2Hat_m_chi2[114];
    Float_t     eXi2Hat_s_chi2[114];
    Float_t     eXi2Hat_m_WTilde[114];
    Float_t     eXi2Hat_s_WTilde[114];
    // eCutMethod == 5: Constant X2Hat BT density also in tangens theta space
    // Uses also cutvariables eCutTTp0[114][20] and eCutTTp1[114][20]
    Float_t	eXi2HatTT_m_chi2[114][20];
    Float_t     eXi2HatTT_s_chi2[114][20];
    Float_t     eXi2HatTT_m_WTilde[114][20];
    Float_t     eXi2HatTT_s_WTilde[114][20];
    // eCutMethod == 6: Random Cut
    Float_t	eRandomCutThreshold;
    // eCutMethod == 7: Random Cut also in tangens theta space
    // Uses also cutvariables eCutTTp0[114][20] and eCutTTp1[114][20]

protected:

    void Set0();
    void Init();

public:

    EdbPVRQuality();
    EdbPVRQuality(EdbPVRec* ali);
    EdbPVRQuality(EdbPVRec* ali, Float_t BTDensityTargetLevel);

    void SetCutMethod(Int_t CutMethod);
    inline void SetBTDensityLevel(Float_t BTDensityLevel) {
        eBTDensityLevel=BTDensityLevel;
    }
    inline void SetCutTTSqueezeFactor(Float_t CutTTSqueezeFactor) {
        eCutTTSqueezeFactor=CutTTSqueezeFactor;
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
        return GetEdbPVRec(eNeedModified);
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
    inline EdbPVRec* GetEdbPVRec(Bool_t NeedModified) {
        cout << "Inline EdbPVRecType= " <<  NeedModified << endl;
        if (NeedModified==1) {
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

    inline void	SetEdbPVRec(EdbPVRec* Ali_orig) {
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
        if (type>7) return 0;
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
    void SetHistGeometry_OPERAandMCBinArea625();

    void CheckEdbPVRec();
    void CheckEdbPVRecThetaSpace(Int_t AliType);

    void Execute();
    void Execute(Int_t CutType);
    void Execute_ConstantBTDensity();
    void Execute_ConstantBTDensityInAngularBins();
    void Execute_ConstantBTQuality();
    void Execute_ConstantBTQualityInAngularBins();
    void Execute_ConstantBTX2Hat();
    void Execute_ConstantBTX2HatInAngularBins();
    void Execute_RandomCut();
    void Execute_RandomCutInAngularBins();
    void Execute_EqualizeTanThetaSpace();
    void Execute_EqualizeTanThetaSpace_ConstantBTDensity();
    void Execute_EqualizeTanThetaSpace_ConstantBTQuality();
    void Execute_EqualizeTanThetaSpace_ConstantBTX2Hat();
    void Execute_EqualizeTanThetaSpace_RandomCut();

    TObjArray* 	FindDoubleBT(EdbPVRec* aliSource);
    EdbPVRec* 	Remove_DoubleBT(EdbPVRec* aliSource);
    EdbPVRec* 	Remove_Passing(EdbPVRec* aliSource);

    // All these functions will call Remove_SegmentArray(..,..,..);
    EdbPVRec* Remove_SegmentArray(TObjArray* segarray, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_Segment(EdbSegP* seg, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_TrackArray(TObjArray* trackArray, EdbPVRec* aliSource=NULL, Int_t Option=0);
    EdbPVRec* Remove_Track(EdbTrackP* track, EdbPVRec* aliSource=NULL, Int_t Option=0);
    TObjArray* TrackToSegmentArray(EdbTrackP* track);
    TObjArray* TrackArrayToSegmentArray(TObjArray* trackArray);


    // This function may be merged later with the CreateEdbPVRec(); function!!! to be done...
    EdbPVRec* CreatePVRWithExcludedSegmentList(EdbPVRec* aliSource, TObjArray *SegmentArray);
    EdbPVRec* CreateEdbPVRec();

    Int_t CheckFilledXYSize();
    Int_t GetAngularSpaceBin(EdbSegP* seg);
    Int_t FindFirstBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    Int_t FindLastBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    TObjArray* GetTracksFromLinkedTracksRootFile();

    Bool_t CheckSegmentQualityInPattern_ConstBTDens(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    Bool_t CheckSegmentQualityInPattern_ConstQual(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
//     Bool_t CheckSegmentQualityInPattern_ConstBTDensInAngularBins(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    /// HERE ANOTERH FUNCTION ???


    virtual ~EdbPVRQuality();          // virtual constructor due to inherited class

    void Print();
    void PrintCutType();
    void PrintCutType0(); // Constant BT density
    void PrintCutType1(); // Constant BT density in Angular Bins
    void PrintCutType2(); // Constant BT quality
    void PrintCutType3(); // Constant BT quality in Angular Bins
    void PrintCutType4(); // Constant BT X2Hat
    void PrintCutType5(); // Constant BT X2Hat in Angular Bins
    void PrintCutType6(); // Random Test Cut
    void PrintCutType7(); // Random Test Cut in Angular Bins
    void PrintCutValues(Int_t CutType);
    void Help();

    ClassDef(EdbPVRQuality,1);         // Root Class Definition for EdbPVRQuality
};

//______________________________________________________________________________

#endif /* EdbPVRQuality */
