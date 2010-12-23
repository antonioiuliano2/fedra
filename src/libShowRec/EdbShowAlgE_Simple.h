#ifndef ROOT_EdbShowAlgE_Simple
#define ROOT_EdbShowAlgE_Simple

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TSpline.h"
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
//___			Declaration of EdbShowAlgE_Simple Class:
//______________________________________________________________________________
//______________________________________________________________________________

class EdbShowAlgE_Simple : public TObject {

private:

    // nothing;

protected:

    // TObjArray storing Reconstructed Showers:
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;// nothing...
    Int_t		eParaName;

    // Variables related for calculation Issues
    Float_t		eCalibrationOffset;
    Float_t		eCalibrationSlope;

    Int_t		ePlateNumberType;
    // = switch wheter to take the weightfile trained for the next longer/or next shower number of plates...

    Int_t		ePlateNumber;
    // either equal to shower->Npl() or is fixed manually.

    TString		eWeightFileString;
    // = generic weightFileString.

    // Array for the specific specification: see GetSpecifications() for explanation...
    Int_t		eSpecificationType[6];
    TString	eSpecificationTypeString[6];
    TString	eSpecificationTypeStringArray[6][6];
    Bool_t 	eSpecificationIsChanged;

    // Generic Input Array:
    // This Plate Binnning is the _final_binning and will not be refined furthermore!
    Int_t ePlateBinning[15];

    // Generic Input Arrays for the ANN:
    TMultiLayerPerceptron *ANN_MLP;
    TMultiLayerPerceptron *ANN_MLP_ARRAY[15];
    TTree *ANNTree;
    Double_t   inANN[70];
    Double_t   outANN;
    Int_t			 ANN_nPlates_ARRAY[15];
    Int_t			 ANN_n_InputNeurons_ARRAY[15];
    Int_t			 ANN_n_InputNeurons;
    TString 	 ANN_Layout;
    TString 	 ANN_WeightFile_ARRAY[15];

    // Generic Correction factors:
    Float_t		eANN_MLP_CORR_0[15];
    Float_t		eANN_MLP_CORR_1[15];

    // Dammit! On some architecures (pc56 in Bern, for example) this structure stuff
    // gives a crash, sometimes. Lets try it with simple Private Variables:
    // (though now I know the reason was different to it..)
    Float_t eParaShowerAxisAngle;
    Int_t eParanseg;
    Float_t eParaBT_deltaR_mean;
    Float_t eParaBT_deltaR_rms;
    Float_t eParaBT_deltaT_mean;
    Float_t eParaBT_deltaT_rms;
    Int_t eParalongprofile[57];

    TH1D* eHisto_nbtk_av;
    TH1D* eHisto_longprofile_av;
    TH1D* eHisto_transprofile_av;
    TH1D* eHisto_deltaR_mean;
    TH1D* eHisto_deltaT_mean;
    TH1D* eHisto_deltaR_rms;
    TH1D* eHisto_deltaT_rms;
    TH1D* eHisto_nbtk;
    TH1D* eHisto_longprofile;
    TH1D* eHisto_transprofile;
    TH1D* eHisto_deltaR;
    TH1D* eHisto_deltaT;

    // Efficiency/Angle parametrisation functions
    TF1 *EffFunc_all;
    TF1 *EffFunc_edefault;
    TF1 *EffFunc_elletroni;
    TF1 *EffFunc_neuchmicro;
    TF1 *EffFunc_MiddleFix;
    TF1 *EffFunc_LowEff;
    TF1 *EffFunc_UserEfficiency;

    // Efficiency/Angle parametrisation function,
    // obtained from the data; either by EDA, or to be set by
    // hand (nseg-2/npl-2)
    TSpline3* eEfficiencyParametrisation;

    // Variables for the output:
    TArrayF*   eEnergyArray;
    TArrayF*   eEnergyArrayUnCorrected;
    TArrayF*   eEnergyArraySigmaCorrected;
    Float_t    eEnergy;
    Float_t    eEnergyCorr;
    Float_t    eEnergyUnCorr;
    Float_t    eEnergySigmaCorr;

    Int_t eEnergyArrayCount;

    // Splines for calculation the interpolation of stat. and sys. errors:
    TObjArray* eSplineArray_Energy_Stat_Electron;
    TObjArray* eSplineArray_Energy_Stat_Gamma;
    TObjArray* eSplineArray_Energy_Sys_Electron;
    TObjArray* eSplineArray_Energy_Sys_Gamma;
    TSpline3*  eSplineCurrent;


    // Reset All Default Variables:
    void 	Set0();
    void 	Init();


public:

    EdbShowAlgE_Simple();
    //   EdbShowAlgE_Simple(EdbShowerP* shower); // in libShowRec
    EdbShowAlgE_Simple(EdbTrackP* track);   // in libShower
    EdbShowAlgE_Simple(TObjArray* RecoShowerArray);

    virtual ~EdbShowAlgE_Simple();          // virtual constructor due to inherited class

    // Hand over  eAli  eInBTArray  from EdbShowerRec
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray) {
        eRecoShowerArray=RecoShowerArray;
        eRecoShowerArrayN=eRecoShowerArray->GetEntriesFast();
    }
    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN) {
        eRecoShowerArrayN=RecoShowerArrayN;
    }
    inline Int_t        GetRecoShowerArrayN()     const       {
        return eRecoShowerArrayN;
    }
    inline TArrayF*        GetEnergyArray()      {
        return eEnergyArray;
    }
    inline TArrayF*        GetEnergyArrayUnCorrected()      {
        return eEnergyArrayUnCorrected;
    }
    inline Float_t        GetEnergy() {
        return eEnergy;
    }
    inline Float_t        GetEnergy(EdbTrackP* track) {
        return track->P();
    }

    // Get Track Parametrisation variables which is input for the ANN calculation
    void GetPara(EdbTrackP* track);

    // Get  Efficiency/Angle parametrisation functions
    inline TF1* GetEffFunc_all() {
        return EffFunc_all;
    }
    inline TF1* GetEffFunc_edefault() {
        return EffFunc_edefault;
    }
    inline TF1* GetEffFunc_elletroni() {
        return EffFunc_elletroni;
    }
    inline TF1* GetEffFunc_neuchmicro() {
        return EffFunc_neuchmicro;
    }
    inline TF1* GetEffFunc_MiddleFix() {
        return EffFunc_MiddleFix;
    }
    inline TF1* GetEffFunc_LowEff() {
        return EffFunc_LowEff;
    }
    inline TF1* GetEffFunc_UserEfficiency() {
        return EffFunc_UserEfficiency;
    }

    inline void SetCalibrationOffset(Float_t CalibrationOffset) {
        eCalibrationOffset=CalibrationOffset;
    }
    inline void SetCalibrationSlope(Float_t CalibrationSlope) {
        eCalibrationSlope=CalibrationSlope;
    }

    inline void SetPlateNumberType(Int_t PlateNumberType) {
        ePlateNumberType=PlateNumberType;
    }
    inline void SetPlateNumber (Int_t PlateNumber) {
        ePlateNumber=PlateNumber;
    }
    inline Int_t GetSpecType(Int_t SpecificationType) {
        return eSpecificationType[SpecificationType];
    }

    void SetEfficiencyParametrisationValues(Double_t* Angles, Double_t* EffValuesAtAngles);

    inline void SetEfficiencyParametrisation(TSpline3* EfficiencyParametrisation) {
        eEfficiencyParametrisation=EfficiencyParametrisation;
        EfficiencyParametrisation->Print();
    }
    inline TSpline3* GetEfficiencyParametrisation() {
        return eEfficiencyParametrisation;
    }
    void PrintEfficiencyParametrisation();
    void SetEfficiencyParametrisationAngles();

    void SetWeightFileString(TString weightstring);
    void InitStrings();

    Int_t FindClosestEfficiencyParametrization(Double_t TestAngle,Double_t ReferenceEff);

    void GetNplIndexNr(Int_t sh_npl,Int_t &check_Npl_index, Int_t ePlateNumberType);

    void GetSpecifications();
    void SetSpecifications(Int_t sp0, Int_t sp1, Int_t sp2, Int_t sp3, Int_t sp4, Int_t sp5);
    void SetSpecificationType(Int_t SpecificationType, Int_t SpecificationTypeVal);

    void PrintSpecifications();
    void LoadSpecificationWeightFile();
    void ReadCorrectionFactors(TString weigthstring, Float_t &p0, Float_t &p1);

    void CreateANN();

    void Update();
    void DoRun();
    void DoRun(TObjArray* trackarray);
    void DoRun(EdbTrackP* shower);

    void ReadTables();
    void ReadTables_Energy();
    //void ReadTables_ID(); // to be implemented...


    void WriteNewRootFile();
    void Print();
    void Help();
    ClassDef(EdbShowAlgE_Simple,1);         // Root Class Definition for my Objects
};

#endif /* ROOT_EdbShowAlgE_Simple */
