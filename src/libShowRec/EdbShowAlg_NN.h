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
    Int_t eANN_Inputtype;


public:

    EdbShowAlg_NN();
    EdbShowAlg_NN(Bool_t ANN_DoTrain);
    virtual ~EdbShowAlg_NN();          // virtual constructor due to inherited class

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

    // Helper Functions:
    Int_t   GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
    Int_t   GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
    Int_t   GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);

    // Print Generic Information about the Algorithm
    void    Print();


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
    // Signal from Background Basetracks of a shower
    // using topological variables

private:
    TString eWeightFileString;
    TString eWeightFileLayoutString;
    TTree*  eANNTree;
    TMultiLayerPerceptron*    eTMlpANN;

    // Variables for the eANN Branches:
    Bool_t      eANN_DoTrain=kTRUE;
    Double_t    eANN_Inputvar[24];    // 24 maximal input neurons
    Int_t       eANN_Inputtype;        // 0: BG-Track, 1: SG-Track
    Double_t    eANN_OutputValue=0;   // result from the network
    // Values valid for ShowerReco Algorithm 11 = N3 ALG: NewNeuralNetwork ("N":3-times)
    // Brick data related inputs
    // The number of Input Variables is calculated only from this:
    Int_t        eANN_PLATE_DELTANMAX; // 0,1,2,3,4,5.
    // 0: no separate information BT(i,j) is used.
    // 1: additional information BT(i,j) for DeltNPL==0  (same plate) is used.
    // 2: additional information BT(i,j) for DeltNPL==-1 ( one plate upstream) is used.
    // 3: additional information BT(i,j) for DeltNPL==+1 ( one plate downstream) is used.
    // 4: additional information BT(i,j) for DeltNPL==-2 ( two plate upstream) is used.
    // 5: additional information BT(i,j) for DeltNPL==+2 ( two plate downstream) is used.
    // Order: same plate, one plate upstream,  one plate downstream,  two plate upstream,  two plate downstream

    // Algorithm method related inputs
    Int_t        eANN_NTRAINEPOCHS; // 1,2,3,4 = 50,100,150,200
    Int_t        eANN_NHIDDENLAYER; // 1,2,3,4 = 2,3,5,7
    Double_t     eANN_OUTPUTTHRESHOLD; // 0..10 = 0.5, 0.55, 0.6 ...
    Int_t        eANN_EQUALIZESGBG; // 0: no, 1: yes
    // 1: not all BG BTs will be taken for NN training, some are randomly left out of the trainingssample so that N(SG) ca. N(BG) for one shower event (for training)

    // This is dependent by the other variables, thus it
    // is explicitely calculated for ease of view
    Int_t        eANN_INPUTNEURONS;


public:

    EdbShowAlg_N3();
    EdbShowAlg_N3(Bool_t ANN_DoTrain);
    virtual ~EdbShowAlg_N3();          // virtual constructor due to inherited class

    void    Init();
    void    CreateANNTree();
    TMultiLayerPerceptron*    Create_NN_ALG_MLP(TTree* inputtree, Int_t inputneurons);
    TString eLayout;

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

    // Print Generic Information about the Algorithm
    void    Print();

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
