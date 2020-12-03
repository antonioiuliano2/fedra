#ifndef ROOT_EdbShowAlgE
#define ROOT_EdbShowAlgE

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TCanvas.h"
#include "TObjString.h"

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
//___     Declaration of EdbShowAlgE Class:
//___
//___
//___		STILL TO WRITE
//___
//______________________________________________________________________________






//______________________________________________________________________________

class EdbShowAlgE : public TObject {

private:

    // Parametrization Name: this name sets imprtant variables depending on the parametrization:
    TString                     eShowAlgEParaName;
    Int_t                       eShowAlgEParaNr;
    Int_t                       eShowAlgEPlatesN; // used only to set names and distingiush. Not used for any reconstructon
    //or measurement!


    TArrayI*		eShowAlgEPlateBinning;



    // TObjArray storing Reconstructed Showers:
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;


    // TObjArray storing TMultiLayerPerceptron for different shower film length:
    TObjArray*         eMLPArray;
    Int_t              eMLPArrayN; 			// (==eMLP_InputNeuronsArrayN)

    // Wheter this is used as Training or RunMethod:
    Bool_t              eDoANNTrain;
    Bool_t              eDoANNRun;

    // Variables as arrays for the In/Outut of the ANN.
    // They have to be given in Float_t* array, to root specifications of
    // the implementation...  (?? where to read this ??)

    // TODO CHECK THIS !!!!!
    // With the new gcc --version 7.30 (shipped with Ubuntu 18.04)
    // compilation does stop here with an error (as of 2018/03/28).
    // Why?

    // It seems like (found in an old root post) when putting 
    // the ! behind the commend behind the variable, then 
    // somehow it ist made transient and the cint (now cling) can handle
    // this.

//     Float_t             eInANN[70];
 Float_t* eInANN = new Float_t[70]; //!
  //  Float_t*             eInANN;  // worked with gcc 4.4, but with gcc 7.3
    Float_t             eOutANN;
  //  Float_t*             eInfoANN; // worked with gcc 4.4, but with gcc 7.3
    Float_t*       eInfoANN = new Float_t[70]; //!

    // Histograms storing the  eInANN  distributions:
    TH1F*               eInANN_Hist[70];




    TMultiLayerPerceptron*      eMLP_Simple;

    Bool_t                      eMLP_SimpleIsDone;
    TTree*                      eMLP_SimpleTree;

    TString                     eMLP_Layout;
    TObjArray*			eMLP_LayoutArray;
    Bool_t                      eMLP_LayoutArrayIsDone;


    Bool_t                      eMLP_WriteWeightFile;
    TString                     eMLP_WeightFileName;
    TString                     eMLP_WeightFileNameArray[10];

    Int_t                       eMLP_TrainEpochs;

    Int_t                       eMLP_InputNeurons;
    TArrayI*			eMLP_InputNeuronsArray;
    Int_t			eMLP_InputNeuronsArrayN;

    TString                     eMLP_InputFileName;
    Bool_t                      eMLP_WriteMLPInputFile;
    Bool_t                      eMLP_ReadMLPInputFile;



    // Reset All Default Variables:
    void                Set0();

    // Set Default Variables
    void                Init();
    void                Init(TString ParaName);

    // Create Input Histos
    void                CreateInputHistos();

    // Fill Input Histos with Entries by eMLP_SimpleTree:
    void                FillInputHistosPara_XX();

    // Simple Layout Creation function:  n:n+1:n:inputtype
    TString 		CreateMLPLayout(Int_t NrOfANNInputNeurons);





    void ClearPlateBinning();
    void ResetWeightFileNameArray();
    void AddWeightFilePlate(TString tmpString, Int_t dospec);

    Bool_t              CheckStringToShowAlgEParaName(TString tmpString2);

    // Helper Function:
    int                 GetLastBinHistoFilles(TH1* h);


public:

    EdbShowAlgE();
    EdbShowAlgE(TString ShowAlgEParaName);
    EdbShowAlgE(TObjArray* RecoShowerArray);
    EdbShowAlgE(TObjArray* RecoShowerArray, TString ShowAlgEParaName);


    virtual ~EdbShowAlgE();          // virtual constructor due to inherited class








    // Set Functions
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray) {
        eRecoShowerArray=RecoShowerArray;
    }

    inline void         SetMLPLayout(TString layout)      {
        eMLP_Layout = layout;
    }
    inline void         SetMLPWeightFileName(TString weightfilename)      {
        eMLP_WeightFileName = weightfilename;
    }
    inline void         SetMLPInputFileName(TString MLP_InputFileName)  {
        eMLP_InputFileName = MLP_InputFileName;
    }
    inline void         SetMLPTrainEpochs(Int_t nep)      {
        eMLP_TrainEpochs = nep;
    }
    inline void         SetMLPArrayN(Int_t MLPArrayN)      {
        eMLPArrayN = MLPArrayN;
    }

    inline void         SetPlatesN(Int_t ShowAlgEPlatesN)      {
        eShowAlgEPlatesN = ShowAlgEPlatesN;
    }


    void                SetDoTrain(Int_t type);
    void                SetDoRun(Int_t type);

    void                SetInputHistosPara(Int_t ParaType);
    void                SetInputHistosPara_FJ();
    void                SetInputHistosPara_YC();
    void                SetInputHistosPara_XX();
    void		SetInputHistosPara_YY();

    void                 SetWriteMLPWeightFile(Int_t type);
    void                 SetWriteMLPInputFile(Int_t type);
    void                 SetReadMLPInputFile(Int_t type);

    void                ReadMLPInputFile();
    void                WriteMLPInputFile();


    // Get Functions
    inline TMultiLayerPerceptron*         GetMLP() const     {
        return eMLP_Simple;
    }

    inline TH1F*        GetHistInANN(Int_t nr)   const {
        return eInANN_Hist[nr];
    }

    inline TString      GetMLPLayout()              {
        return eMLP_Layout;
    }
    inline TString      GetMLPWeightFileName()      {
        return eMLP_WeightFileName;
    }
    inline TString      GetMLPInputFileName()       {
        return eMLP_InputFileName;
    }
    inline Int_t        SetMLPTrainEpochs()         {
        return eMLP_TrainEpochs;
    }


    TCanvas*            GetHistoCanvas();
    void Print();



    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    // Since these functions depend on the algorithm type they are made virtual
    // and implemented in the inherited classes.
    virtual void Initialize();
    virtual void Execute();
    virtual void Finalize();



    /// DEBUG ------------------------------  TEST  DEVOPLEMENT

//     void TESTTRAIN();
    void Create_MLP();
    void Create_MLPTree();
    void Fill_MLPTree();
//     void Train_MLP();
//     void Run_MLP();

    void TESTRUN();
    void SetInputHistos(TH1F* h, Int_t nbin, Double_t lower, Double_t upper);





    // CreateStandardWeightFileStrings
    void 		CreateStandardWeightFileStrings();
    void                 CreateStandardMLPArrayN();
    void                 PrintWeightFileStrings();
    void                 ResetPlateBinning();

    Int_t                CheckPlateAndPlateBinning(Int_t Plate);
    void                 AddWeightFile(TString tmpString, Int_t Plate, TString tmpString2);


    void                 CreateMLPLayouts();
    void                 CreateMLPLayouts_FJ();
    void                 CreateMLPLayouts_LT();
    void                 CreateMLPLayouts_YC();
    void                 CreateMLPLayouts_JC();
    void                 CreateMLPLayouts_XX();
    void                 CreateMLPLayouts_YY();
    void                 CreateMLPLayout_YC(Int_t nr);
    void                 CreateMLPLayout_XX(Int_t nr);
    void                 CreateMLPLayout_YY(Int_t nr);


    void 								FillInputNeuronsArray_YC();
    void 								FillInputNeuronsArray_XX();
    void 								FillInputNeuronsArray_YY();

    void 								CreateInputNeuronsArray_YC();
    void 								CreateInputNeuronsArray_XX();
    void 								CreateInputNeuronsArray_YY();

    void 								CreateInputNeuronsArray();


    void 								CreateMLPs();
    void                 CreateMLPArray();
    void                 LoadMLPWeights();
    void                 LoadMLPWeights(Int_t nr);

    void                 SetBinsInputHistos();
    void                 SetBinsInputHistos_YC();
    void                 SetBinsInputHistos_XX();
    void                 SetBinsInputHistos_FJ();
    void                 SetBinsInputHistos_YY();


    void                 FillMLP_SimpleTree_YC(EdbShowerP* show, Int_t ShowAlgEParaNr);
    void                 FillMLP_SimpleTree_YC();
    void                 FillMLP_SimpleTree_XX(EdbShowerP* show, Int_t ShowAlgEParaNr);
    void                 FillMLP_SimpleTree_XX();
    void                 FillMLP_SimpleTree_YY(EdbShowerP* show, Int_t ShowAlgEParaNr);
    void                 FillMLP_SimpleTree_YY();
    void                 FillMLP_SimpleTree();

    void                 WriteMLP_SimpleTree(TString filename);

    void                 Train();
    void                 Run();
    void                 DumpWeights();


    int 			            GetMLPArrayNr(Int_t NPlToCheck);

    /// DEBUG ------------------------------  TEST  DEVOPLEMENT


    // Read parameters out of default.par file
    int                   ReadShowAlgEPar(const char *file);



    ClassDef(EdbShowAlgE,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

#endif /* ROOT_EdbShowAlg */
