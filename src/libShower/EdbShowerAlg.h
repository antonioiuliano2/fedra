#ifndef ROOT_EdbShowerAlg
#define ROOT_EdbShowerAlg

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TF1.h"
#include "TH1.h"
#include "TSpline.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TBenchmark.h"
#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"
#include "EdbSegP.h"

// #include "EdbEDAUtil.h"
using namespace std;

//______________________________________________________________________________

class EdbShowerAlg : public TObject {

private:
// nothing...

protected:

    // A algorithm here should not have more than 10 different parameters, otherwise we
    // will have to use TObjArray implementations.
    TString      eAlgName;
    Int_t        eAlgValue;
    Float_t      eParaValue[10];
    TString      eParaString[10];
    Int_t				 eActualAlgParametersetNr; // Used when more sets of same algorithm


    // Pointer copies to be used for reconstruction of showers
    // EdbPVRec object:
    EdbPVRec*          eAli;
    Int_t              eAliNpat;
    // TObjArray storing Initiator Basetracks:
    TObjArray*         eInBTArray;
    Int_t              eInBTArrayN;
    // Variables describing plate number cuts.
    Int_t              eFirstPlate_eAliPID;
    Int_t              eLastPlate_eAliPID;
    Int_t              eMiddlePlate_eAliPID;
    Int_t              eNumberPlate_eAliPID;
    // TObjArray storing Reconstructed Showers:
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;

    // Transformed (smaller) EdbPVRec object:
    EdbPVRec*          eAli_Sub;
    Int_t              eAli_SubNpat;
    // Flag wheter to use the small EdbPvrec Object or not. Using small object, this will not be deleted ue to
    // pointer problems. See in implementation of Transform() for details.
    Int_t               eUseAliSub;




    // In constructor this is created on heap...
    //EdbShowerP*	        eRecoShower;
    EdbTrackP*	        eRecoShower;

    // Reset All Default Variables:
    void 								Set0();


public:

    EdbShowerAlg();
    EdbShowerAlg(TString AlgName, Int_t AlgValue);
// 		EdbShowerAlg(TString AlgName, Int_t AlgValue, Float_t* ParaValue[10], TString ParaString[10]);
// 		EdbShowerAlg(EdbPVRec* gAli, TObjArray* InBTArray);


    virtual ~EdbShowerAlg();          // virtual constructor due to inherited class


    // Hand over  eAli  eInBTArray  from EdbShowerRec
    inline void         SetEdbPVRec( EdbPVRec* Ali )          {
        eAli = Ali;
        eAliNpat=eAli->Npatterns();
    }
    inline void         SetInBTArray( TObjArray* InBTArray ) {
        eInBTArray = InBTArray;
        eInBTArrayN=eInBTArray->GetEntries();
    }
    inline void         SetEdbPVRecPIDNumbers(Int_t FirstPlate_eAliPID, Int_t LastPlate_eAliPID, Int_t MiddlePlate_eAliPID, Int_t NumberPlate_eAliPID) {
        eFirstPlate_eAliPID=FirstPlate_eAliPID;
        eLastPlate_eAliPID=LastPlate_eAliPID;
        eMiddlePlate_eAliPID=MiddlePlate_eAliPID;
        eNumberPlate_eAliPID=NumberPlate_eAliPID;
    }
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray) {
        eRecoShowerArray=RecoShowerArray;
    }
    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN) {
        eRecoShowerArrayN=RecoShowerArrayN;
    }


    inline void 				SetActualAlgParameterset(Int_t ActualAlgParametersetNr) {
        eActualAlgParametersetNr=ActualAlgParametersetNr;
    }

    inline Int_t        GetAlgValue()  const {
        return eAlgValue;
    }
    inline TString      GetAlgName()  const {
        return eAlgName;
    }
    inline Int_t        GetRecoShowerArrayN()     const       {
        return eRecoShowerArrayN;
    }
    inline TObjArray*   GetRecoShowerArray()     const       {
        return eRecoShowerArray;
    }

    inline void         SetUseAliSub(Bool_t UseAliSub) {
        eUseAliSub=UseAliSub;
    }

    Double_t            DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest);
    Double_t            DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest);
    Double_t            DeltaR_NoPropagation(EdbSegP* s,EdbSegP* stest);

    Double_t            DeltaTheta(EdbSegP* s1,EdbSegP* s2);
    Double_t            DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2);
    Double_t            DeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2);
    Double_t            GetSpatialDist(EdbSegP* s1,EdbSegP* s2);
    Double_t 						GetMinimumDist(EdbSegP* seg1,EdbSegP* seg2);


    void         				SetParameters(Float_t* par);
    //void                Transform_eAli(EdbSegP* InitiatorBT);
    void                Transform_eAli( EdbSegP* InitiatorBT, Float_t ExtractSize);
    Bool_t              IsInConeTube(EdbSegP* sTest, EdbSegP* sStart, Double_t CylinderRadius, Double_t ConeAngle);
    void 								Convert_EdbPVRec_To_InBTArray();

    EdbVertex * 				CalcVertex(TObjArray *segments);


    void Print();
    void PrintParameters();
    void PrintParametersShort();
    void PrintMore();
    void PrintAll();


    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    // Since these functions depend on the algorithm type they are made virtual
    // and implemented in the inherited classes.
    virtual void Initialize();
    virtual void Execute();
    virtual void Finalize();

    ClassDef(EdbShowerAlg,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

class EdbShowerAlg_GS : public EdbShowerAlg {

private:
    // TObjArray storing Initiator Vertices:
    TObjArray*         eInVtxArray;
    Int_t              eInVtxArrayN;
    Bool_t						 eInVtxArraySet;

    // Variable to clean the found parings once more.
    Bool_t 						eSetCleanPairs;

    // Neural Network to Improve Fake BG pair rejection rate
    // (part of gamma pair reco)
    TMultiLayerPerceptron* 	eANNPair;
    TTree* 									eANNPairTree;
    Float_t									eANNPairCut;
    // Neural Network input feed variables
    Float_t eValueGSNN_var00;
    Float_t eValueGSNN_var01;
    Float_t eValueGSNN_var02;
    Float_t eValueGSNN_var03;
    Float_t eValueGSNN_var04;
    Float_t eValueGSNN_var05;
    Float_t eValueGSNN_var06;
    Float_t eValueGSNN_varInput;
    Float_t eValueGSNN_varOutput;


public:

    EdbShowerAlg_GS();
    virtual ~EdbShowerAlg_GS();          // virtual constructor due to inherited class

    void    Set0();
    void    Init();

    inline void         SetInVtxArray( TObjArray* InVtxArray ) {
        eInVtxArray = InVtxArray;
        eInVtxArrayN = eInVtxArray->GetEntries();
        cout << "SetInVtxArray:: " << eInVtxArrayN << "  entries set"<<endl;
    }
    void         	    SetInVtx( EdbVertex* vtx );
    void         	    AddInVtx( EdbVertex* vtx );

    inline Int_t        GetInVtxArrayN()     	const       {
        return eInVtxArrayN;
    }
    inline TObjArray*   GetInVtxArray()     	const       {
        return eInVtxArray;
    }

    inline void        SetCleanPairs(Bool_t CleanPairs) {
        eSetCleanPairs = CleanPairs;
    }


    // Helper Functions for this class:
    Bool_t	CheckInput();
    void 		Convert_InVtxArray_To_InBTArray();
    Bool_t		CheckPairDuplications(Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray);
    Double_t 	CalcIP(EdbSegP *s, double x, double y, double z);
    Double_t 	CalcIP(EdbSegP *s, EdbVertex *v);
    Bool_t 		IsPossibleFakeDoublet(EdbSegP* s1,EdbSegP* s2);


    TObjArray* 	FindPairs(EdbSegP* InBT, EdbPVRec* eAli_Sub);
    TObjArray* 	CheckCleanPairs(EdbSegP* InBT, TObjArray* RecoShowerArrayFromFindPairs);


    void CreateANNPair();
    void ReloadANNWeights(Bool_t VtxArray_Or_InBTArray);
    void CreateANNPlots();

    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowerAlg_GS,1);         // Root Class Definition for my Objects
};

//______________________________________________________________________________






//______________________________________________________________________________
//___
//___			Declaration of EdbShowerAlgESimple Class:
//______________________________________________________________________________

class EdbShowerAlgESimple : public TObject {

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
    Int_t		eSpecificationType[7];
    TString	eSpecificationTypeString[7];
    TString	eSpecificationTypeStringArray[7][7];
    Bool_t 	eSpecificationIsChanged;
    Bool_t  eForceSpecificationReload;

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

    EdbShowerAlgESimple();
    //   EdbShowAlgE_Simple(EdbShowerP* shower); // in libShowRec
    EdbShowerAlgESimple(EdbTrackP* track);   // in libShower
    EdbShowerAlgESimple(TObjArray* RecoShowerArray);

    virtual ~EdbShowerAlgESimple();          // virtual constructor due to inherited class

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

    inline TMultiLayerPerceptron* GetNeuralNetwork(Int_t ANNType=0) {
        if (ANNType>=15) ANNType=14;
        return ANN_MLP_ARRAY[ANNType];
    }

    inline void TrainNeuralNetwork(TString weight, Int_t ANNType=0) {
        if (ANNType>=15) ANNType=14;
        ANN_MLP_ARRAY[ANNType]->Train(100);
        return;
    }

    inline void DumpNeuralNetworkWeight(TString weight, Int_t ANNType=0) {
        if (ANNType>=15) ANNType=14;
        ANN_MLP_ARRAY[ANNType]->DumpWeights(weight);
        return;
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
    inline void SetForceSpecificationReload() {
        eForceSpecificationReload=kTRUE;
    }
    inline void UnSetForceSpecificationReload() {
        eForceSpecificationReload=kFALSE;
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


    inline void WriteNewRootFile() {
        WriteNewRootFile("Shower.root","treebranch");
    }
    inline void WriteNewRootFile(TString sourcefilename) {
        WriteNewRootFile(sourcefilename,"treebranch");
    }
    void WriteNewRootFile(TString sourcefilename, TString treename);

    void Print();
    void Help();
    ClassDef(EdbShowerAlgESimple,1);         // Root Class Definition for my Objects
};



//______________________________________________________________________________
//___
//___			Declaration of EdbShowerAlgIDSimple Class:
//______________________________________________________________________________

class EdbShowerAlgIDSimple : public EdbShowerAlgESimple {

private:

    // nothing;

protected:

    // Variables for the output:
    Double_t   eIDEnergySigmaCorr;
    Double_t   eANNIDCutValue;
    // Type for the ID cut: 0: BestMinDist to (1,1) in the ROI curve
    // Type for the ID cut: 1: ca. 90% eff
    // Type for the ID cut: 2: ca.  1% cont.
    Double_t	eIDCutTypeValue[3];

public:

    EdbShowerAlgIDSimple();
    //   EdbShowerAlgIDSimple(EdbShowerP* shower); // in libShowRec
    EdbShowerAlgIDSimple(EdbTrackP* track);   // in libShower
    EdbShowerAlgIDSimple(TObjArray* RecoShowerArray);
    EdbShowerAlgIDSimple(EdbTrackP* track, EdbVertex* vtx);   // in libShower
    EdbShowerAlgIDSimple(TObjArray* RecoShowerArray, TObjArray* VtxArray);

    virtual ~EdbShowerAlgIDSimple();          // virtual constructor due to inherited class

    void    Init();

    inline void SetANNIDCutValue(Double_t ANNIDCutValue) {
        eANNIDCutValue=ANNIDCutValue;
    }
    void SetSpecificationType(Int_t SpecificationType, Int_t SpecificationTypeVal);
    void PrintSpecifications();

    void CreateANN();

    void Update();
    void InitStrings();

    void ReadTables();
    void ReadTables_ID(); // to be implemented...

    void DoRun(EdbTrackP*);
    void DoRun(TObjArray* trackarray);
    void DoRun();

    Int_t DetermineIDType(Double_t val, Int_t SpecificationType);
    Int_t DetermineIDFlag(Int_t IDType);

    void Print();
    void Help();
    ClassDef(EdbShowerAlgIDSimple,1);         // Root Class Definition for my Objects
};

#endif /* ROOT_EdbShowerAlg */
