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
    EdbPVRec* eAli_pointer; // to be implermented: pointer to the
    // desired eAli object: orig or modified  TOODOO....
    Bool_t    eNeedModified;
    Bool_t    eIsSource;
    Bool_t    eIsTarget;
    Int_t     eHistGeometry;
    Int_t     eAli_maxNpatterns;

    // Variables related for calculation Issues
    Int_t		eCutMethod;
    TString		eCutMethodString;
    Bool_t		eCutMethodIsDone[8];

    Float_t		eBTDensityLevel;
    Float_t		eBTDensityLevelAngularSpace[20];

    Bool_t		eBTDensityLevelCalcMethodMC;
    Int_t		eBTDensityLevelCalcMethodMCConfirmationNumber;

    Float_t		ePatternBTDensity_orig[114];
    Float_t		ePatternBTDensity_modified[114];

    // Histograms for calculations and crosscheck drawings
    // Are either filled per plate (FillHistosPattern)
    // for the whole pattern (CheckEdbPVRec)
    TH2F*		eHistWChi2;
    TH2F*		eHistXY;
    TH2F*		eHistTXTY;
    TH1F*		eHistTT;
    TH1F*		eHistChi2;
    TH1F*		eHistW;
    TH1F*		eHistWTilde;

    TH1F*		eHistTTFillcheck;
    TH1F*		eHistBTDensityPattern;
    TH1F*		eHistBTDensityVolume;
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
    // TProfile: BT dens/mm2 versus PID()	generic histogram
    TProfile* 		eProfileBTdens_vs_PID_generic;

    // Variables related for cut issues:

    // For all eCutMethod in TanTheta Space:
    // Global CutFactor ...
    Float_t	eCutTTSqueezeFactor;
    // Each TT bin gets its own reduction factor for each pattern seperately
    Float_t	eCutTTReductionFactor[12];
    Int_t	eBinTT;
    // Additional GlobalTTReductionFactorC in case the TT cutting still yields a too high BG density
    Float_t eGlobalTTReductionFactorC;

    // For the specific cut methods:
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


    // Arrays for each pattern where Source Basetracks, Recjected
    // and Accepted Basetracks are stored.
    // Up to now: refilled for each pattern new.
    // Constructed in such a way that 10 (+2) bins
    // cover the TT-space
    TObjArray* eArrayPatternTTSource[12]; // should have same entries as the corresponding EdbPattern
    TObjArray* eArrayPatternTTRejected[12]; // after specific cut, these BTs wont be taken
    TObjArray* eArrayPatternTTAccepted[12]; // after specific cut, BTs here will be kept

    TObjArray* eArrayPatternAllTTSource; // should have same entries as the corresponding EdbPattern
    TObjArray* eArrayPatternAllTTRejected; // after specific cut, these BTs wont be taken
    TObjArray* eArrayPatternAllTTAccepted; // after specific cut, BTs here will be kept

    // Arrays of segments which are to be excluded, when creating the new
    // EdbPVRec volume (see BG Note description)
    // Nota bene: these arrays contain all pattern BTs, so add them
    // _after_ pattern looping
    TObjArray* eArrayPatternAllExcluded[4];
    // 0: FakeDoubletBTs
    // 1: High Density BTs
    // 2: Cosmics Passing Through BTs
    // 3: Event Related BTs

    // TODO: MAYBE MOVE THIS ARRAY TO ONLY ONE OBJECT, SO THAT IT CAN BE FILLED AND ADDED
    // WITHOUT FURTHER CONSTRAINST.....
    // OR SIMPLY ADD A FIFTH ARRAY, WHERE ALL excluded SEGMENTS CAN BE PUT IN ANYWAY????

    // This array is containing all pattern all different source arrays:
    TObjArray* eArrayAllExcludedSegments;



protected:

    void Set0();
    void Init();
    void ResetHistos();
    void ResetHistosSinglePattern();

public:

    EdbPVRQuality();
    EdbPVRQuality(EdbPVRec* ali);
    EdbPVRQuality(EdbPVRec* ali, Float_t BTDensityTargetLevel);
    EdbPVRQuality(EdbPVRec* ali,  Float_t BTDensityTargetLevel, Int_t BG_CutMethod);

    void SetCutMethod(Int_t CutMethod);
    inline void SetCutMethodIsDone(Int_t  CutMethod) {
        eCutMethodIsDone[CutMethod]=kTRUE;
    }
    inline void SetBTDensityLevel(Float_t BTDensityLevel) {
        eBTDensityLevel=BTDensityLevel;
    }
    inline void SetCutTTSqueezeFactor(Float_t CutTTSqueezeFactor) {
        eCutTTSqueezeFactor=CutTTSqueezeFactor;
    }
    inline Float_t GetCutTTSqueezeFactor() {
        return eCutTTSqueezeFactor;
    }

    inline void SetCutTTReductionFactor(Int_t binTT,Float_t CutTTReductionFactor) {
        eCutTTReductionFactor[binTT]=CutTTReductionFactor;
    }
    inline Float_t GetCutTTReductionFactor(Int_t binTT) {
        return eCutTTReductionFactor[binTT];
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
    EdbPVRec* GetEdbPVRecNew();


    inline void	SetEdbPVRec(EdbPVRec* Ali_orig) {
        eAli_orig=Ali_orig;
        eIsSource=kTRUE;
        eAli_maxNpatterns=Ali_orig->Npatterns();
        if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
        if (eAli_maxNpatterns>114) {
            cout << "WARNING    EdbPVRQuality::SetEdbPVRec  eAli_orig->Npatterns() = " << eAli_maxNpatterns << " is greater than possible basetrack data of two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114 !!!" << endl;
            eAli_maxNpatterns=114;
        }
    }

    inline   TH2F* GetHistChi2W() {
        return eHistWChi2;
    }


    inline   TH1F* GetHistChi2() {
        return eHistChi2;
    }
    inline   TH1F* GetHistW() {
        return eHistW;
    }
    inline   TH1F* GetHistWTilde() {
        return eHistWTilde;
    }

    inline   TH2F* GetHistYX() {
        return eHistXY;
    }
    inline   TH1F* GetHistTT() {
        return eHistTT;
    }
    inline   TH2F* GetHistTYTX() {
        return eHistTXTY;
    }
    inline   TH1F* GetHistTTFillcheck() {
        return eHistTTFillcheck;
    }


    inline Float_t     GetBTDensity(Int_t patNR=-1) {
        return GetBTDensity_orig(patNR);
    }
    inline Float_t     GetBTDensity_orig(Int_t patNR=-1) {
        if (patNR==-1) return  eProfileBTdens_vs_PID_source_meanY;
        return ePatternBTDensity_orig[patNR];
    }
    inline Float_t     GetBTDensity_modified(Int_t patNR=-1) {
        if (patNR==-1) return  eProfileBTdens_vs_PID_target_meanY;
        return ePatternBTDensity_modified[patNR];
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

    inline TObjArray*   GetArrayAllExcludedSegments() {
        return eArrayAllExcludedSegments;
    }




    void SetHistGeometry_OPERA();
    void SetHistGeometry_MC();
    void SetHistGeometry_OPERAandMC();
    void SetHistGeometry_OPERAandMCBinArea625();
    void SetHistGeometry_OPERAandMCBinArea4mm2();

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

    TObjArray* 	FindFakeDoubleBTs(EdbPVRec* aliSource=NULL);
    void FindHighDensityBTs();
    void FindPassingBTs();
    void FindEventRelatedBTs();

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

    TCanvas* GetQualityPlots(Int_t CountNr=0, Int_t aliSourceType=0);
//     TCanvas* GetQualityPlotsSingle(Int_t aliSourceType=0, Int_t Plottype=-1, Int_t CountNr=0);
    TPad* GetQualityPlotsSingle(Int_t CountNr=0, Int_t aliSourceType=0, Int_t Plottype=-1);

    EdbPVRec* ExtractDataVolume(EdbPVRec* pvr, EdbSegP* seg, Float_t tolerance[4]);


    Int_t CheckFilledXYSize(TH2F* HistXY);

    Int_t GetAngularSpaceBin(EdbSegP* seg);
    Int_t FindFirstBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    Int_t FindLastBinAbove(TH1* hist, Double_t threshold, Int_t axis);
    Int_t FindFirstBinAboveTH2(TH2* hist, Double_t threshold, Int_t axis);
    Int_t FindLastBinAboveTH2(TH2* hist, Double_t threshold, Int_t axis);

    TObjArray* GetTracksFromLinkedTracksRootFile();

    Bool_t CheckSegmentQualityInPattern_ConstBTDens(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    Bool_t CheckSegmentQualityInPattern_ConstQual(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
//     Bool_t CheckSegmentQualityInPattern_ConstBTDensInAngularBins(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    /// HERE ANOTHER FUNCTION ???  WHICH ONE ???


    Bool_t Chi2WRelation(EdbSegP* seg, Float_t Cutp0, Float_t Cutp1, Int_t qualitycuttype);
    Bool_t X2HatCutRelation(EdbSegP* seg, Double_t CutValueX2Hat, Double_t  CutValueX2Hat_Chi2Mean, Double_t  CutValueX2Hat_Chi2Sigma, Double_t CutValueX2Hat_WTildeMean, Double_t CutValueX2Hat_WTildeSigma );



    void FillTanThetaTArrays(Int_t patNR);
    void DetermineCutTTReductionFactor(Int_t patNR);

    void FillHistosPattern(EdbPVRec* aliSource, Int_t patNR=0, Bool_t DoResetHistos=kTRUE, Float_t weightXY=1);
    void FillHistosVolume(EdbPVRec* aliSource);


    void CreateEdbPVRec_TT_Algorithms();

    void RebinTTHistogram(Int_t nbins=5);


    void Cut();
    void Cut_TTBin(Int_t TTbin);
    void Cut_ConstantBTDensity();
    void Cut_ConstantBTQuality();
    void Cut_ConstantBTX2Hat();
    void Cut_RandomCut();
    void MergeTTSegments();
    void MergeHighDensBTsLists();
    void MergeExclusionLists();

    virtual ~EdbPVRQuality();          // virtual constructor due to inherited class

    void Print();
    void PrintBTDensities();	// Print BT densities for each pattern
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
