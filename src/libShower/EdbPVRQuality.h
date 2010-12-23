#ifndef ROOT_EdbPVRQuality
#define ROOT_EdbPVRQuality
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TF1.h"
#include "TH2.h"
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
    Bool_t		eIsSource;

    // Variables related for calculation Issues
    Int_t			eCutMethod;
    Float_t		eBTDensityLevel;
    Bool_t		eCutMethodIsDone[2];

    Float_t		ePatternBTDensity_orig[57];
    Float_t		ePatternBTDensity_modified[57];


    // Histograms for calculations
    TH2F*			eHistChi2W;
    TH2F*			eHistYX;
    Int_t			NbinsX,NbinsY;
// 		Int_t			NbinsX,NbinsY;
    Float_t		minX,maxX;
    Float_t		minY,maxY;

    // Variables related for cut Issues
    // eCutMethod == 0: Constant BT density
    // Condition:  Chi2() < p1* W() - p0
    Float_t		eCutp0[57];
    Float_t		eCutp1[57];
    // eCutMethod == 1: Constant Chi2W quality
    Float_t		eCutDistChi2[57];
    Float_t		eCutDistW[57];
    Float_t 	eAggreementChi2WDistCut[57];
    Float_t 	eAggreementChi2CutMeanChi2;
    Float_t		eAggreementChi2CutRMSChi2;
    Float_t 	eAggreementChi2CutMeanW;
    Float_t 	eAggreementChi2CutRMSW;

protected:

    // Reset All Default Variables:
    void 								Set0();
    void 								Init();

public:

    EdbPVRQuality();
    EdbPVRQuality(EdbPVRec* ali);

    void SetCutMethod(Int_t CutMethod);
    inline void SetBTDensityLevel(Float_t BTDensityLevel) {
        eBTDensityLevel=BTDensityLevel;
    }

    inline EdbPVRec* GetEdbPVRec() {
        return eAli_orig;
    }
    inline EdbPVRec* GetEdbPVRec(Int_t EdbPVRecType) {
        if (EdbPVRecType==1) {
            return eAli_modified;
        }
        else {
            return eAli_orig;
        }
    }

    inline void				SetEdbPVRec(EdbPVRec* Ali_orig) {
        eAli_orig=eAli_orig;
        eIsSource=kTRUE;
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

    inline Float_t    GetAggreementChi2CutMeanChi2() {
        return eAggreementChi2CutMeanChi2;
    }
    inline Float_t    GetAggreementChi2CutRMSChi2() {
        return eAggreementChi2CutRMSChi2;
    }
    inline Float_t    GetAggreementChi2CutMeanW() {
        return eAggreementChi2CutMeanW;
    }
    inline Float_t    GetAggreementChi2CutRMSW() {
        return eAggreementChi2CutRMSW;
    }

    inline Float_t*   GetAggreementChi2Cut() {
        return eAggreementChi2WDistCut;
    }
    inline Float_t    GetAggreementChi2Cut(Int_t patNR) {
        return eAggreementChi2WDistCut[patNR];
    }

    void SetHistGeometry_OPERA();
    void SetHistGeometry_MC();

    void CheckEdbPVRec();
    void Execute_ConstantBTDensity();
    void Execute_ConstantQuality();

    EdbPVRec* Remove_DoubleBT(EdbPVRec* aliSource);
    EdbPVRec* Remove_Passing(EdbPVRec* aliSource);
    void Remove_SegmentArray(TObjArray* segArray); // segment array
    void Remove_TrackArray(TObjArray* trackArray); // track array
    void Remove_Segment(EdbSegP* seg); // segment
    void Remove_Track(EdbTrackP* track); // track


    void CreateEdbPVRec();

    Bool_t CheckSegmentQualityInPattern_ConstBTDens(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);
    Bool_t CheckSegmentQualityInPattern_ConstQual(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg);

    virtual ~EdbPVRQuality();          // virtual constructor due to inherited class

    void Print();
    void PrintCutType0();
    void PrintCutType1();
    void Help();
    ClassDef(EdbPVRQuality,1);         // Root Class Definition for my Objects
};

//______________________________________________________________________________

#endif /* EdbPVRQuality */
