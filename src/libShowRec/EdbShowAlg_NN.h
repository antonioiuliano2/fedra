#ifndef ROOT_EdbShowAlg_NN
#define ROOT_EdbShowAlg_NN

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TMultiLayerPerceptron.h"
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
//___     Declaration of EdbShowAlg Class:
//___       Following structure is foreseen: The (inherited) class instances of
//___       EdbShowAlg DO the reconstruction. They DONT manage the data.
//___       Following the principle "separate algorithms and data" (like ATHENA,
//___       the ATLAS framework). Therefore the  EdbPVRec Object, the Initiator
//___       Base Tracks and the shower arrays are only handed over either as
//___       reference or as pointers. Saving/manipulating/preparing of these
//___       objects will only be done in the EdbShowRec class instances!
//___
//______________________________________________________________________________







//______________________________________________________________________________

class EdbShowAlg_NN : public EdbShowAlg {

    // Neural Network algorithm distinguishing 
    // Signal from Backgroundbasetracks of a shower 
    // using topological variables


private:
    TString eWeightFileString;
    TString eWeightFileLayoutString;
    TTree*  eANNTree;
    TMultiLayerPerceptron*    eTMlpANN;


    // variables for the eANN Branches:
    Float_t eANN_var_InBT_To_TestBT;
    Float_t eANN_var_dT_InBT_To_TestBT;
    Float_t eANN_var_dR_InBT_To_TestBT;
    Float_t eANN_var_dR_TestBT_To_InBT;
    Float_t eANN_var_zDist_TestBT_To_InBT;
    Float_t eANN_var_SpatialDist_TestBT_To_InBT;
    Float_t eANN_var_zDiff_TestBT_To_InBT;
    Float_t eANN_var_dT_NextBT_To_TestBT;
    Float_t eANN_var_dR_NextBT_To_TestBT;
    Float_t eANN_var_mean_dT_2before;
    Float_t eANN_var_mean_dR_2before;
    Float_t  eANN_var_mean_dT_before;
    Float_t eANN_var_mean_dR_before;
    Float_t  eANN_var_mean_dT_same;
    Float_t eANN_var_mean_dR_same;
    Float_t  eANN_var_mean_dT_after;
    Float_t eANN_var_mean_dR_after;
    Float_t  eANN_var_mean_dT_2after;
    Float_t  eANN_var_mean_dR_2after;
    Float_t eANN_var_min_dT_2before;
    Float_t eANN_var_min_dR_2before;
    Float_t  eANN_var_min_dT_before;
    Float_t eANN_var_min_dR_before;
    Float_t  eANN_var_min_dT_same;
    Float_t eANN_var_min_dR_same;
    Float_t  eANN_var_min_dT_after;
    Float_t eANN_var_min_dR_after;
    Float_t  eANN_var_min_dT_2after;
    Float_t  eANN_var_min_dR_2after;
    Int_t eANN_var_nseg_1before;
    Int_t eANN_var_nseg_2before;
    Int_t eANN_var_nseg_3before;
    Int_t eANN_var_nseg_1after;
    Int_t eANN_var_nseg_2after;
    Int_t eANN_var_nseg_3after;
    Int_t eANN_var_nseg_same;
    Int_t eANN_var_type;


public:

    EdbShowAlg_NN();
    virtual ~EdbShowAlg_NN();          // virtual constructor due to inherited class

    void    Init();
    void    CreateANNTree();


    Int_t   GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
    Int_t   GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
    Int_t   GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);


    TMultiLayerPerceptron*    Create_NN_ALG_MLP(TTree* inputtree, Int_t inputneurons);


    void    LoadANNWeights();
    void    LoadANNWeights(TMultiLayerPerceptron* TMlpANN, TString WeightFileString);

    void    SetANNWeightString();


    inline  void SetWeightFileString(TString WeightFileString) {
        eWeightFileString=WeightFileString;
        return;
    }

    inline  TString GetWeightFileString() {
        return eWeightFileString;
    }


    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_NN,1);         // Root Class Definition for my Objects
};




//______________________________________________________________________________

class EdbShowAlg_N3 : public EdbShowAlg {

    // New Neural Network (3N) algorithm distinguishing 
    // Signal from Backgroundbasetracks of a shower 
    // using topological variables

private:
    TString eWeightFileString;
    TString eWeightFileLayoutString;
    TTree*  eANNTree;
    TMultiLayerPerceptron*    eTMlpANN;

    // Variables for the eANN Branches:
    Bool_t      N3_DoTrain=kTRUE;
    Double_t    N3_Inputvar[29]; // 29 maximal input neurons
    Int_t       N3_Type;  // 0: BG, 1: SG
    Int_t       N3_ANN_NInput; // number of Inputvariables in total
    Double_t    N3_OutputValue=0;
    // Values valid for ShowerReco Algorithm 11 = N3 ALG: NeWNeuralNetwork
    // Brick data related inputs
    Int_t        N3_ANN_PLATE_DELTANMAX; // 0,1,2,3
    // Algorithm method related inputs
    Int_t        N3_ANN_NTRAINEPOCHS; // 1,2,3,4 = 50,100,150,200 
    Int_t        N3_ANN_NHIDDENLAYER; // 1,2,3,4 = 2,3,5,7
    Double_t     N3_ANN_OUTPUTTHRESHOLD; // 0..10 = 0.5, 0.55, 0.6 ...
    // This is dependent by the other variables, thus it
    // is explicitely calculated for ease of view
    Int_t        N3_ANN_INPUTNEURONS;


public:

    EdbShowAlg_N3();
    virtual ~EdbShowAlg_N3();          // virtual constructor due to inherited class

    void    Init();
    void    CreateANNTree();


    TMultiLayerPerceptron*    Create_NN_ALG_MLP(TTree* inputtree, Int_t inputneurons);

    void    LoadANNWeights();
    void    LoadANNWeights(TMultiLayerPerceptron* TMlpANN, TString WeightFileString);

    void    SetANNWeightString();


    inline  void SetWeightFileString(TString WeightFileString) {
        eWeightFileString=WeightFileString;
        return;
    }

    inline  TString GetWeightFileString() {
        return eWeightFileString;
    }

    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_N3,1);         // Root Class Definition for my Objects
};

#endif /* ROOT_EdbShowAlg_NN */
