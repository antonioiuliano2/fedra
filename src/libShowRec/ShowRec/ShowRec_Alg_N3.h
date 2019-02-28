//---------------------------------------------------------------------------------
//-     VARIABLES: related to specific ReconstructionAlgorithms:
//-     N3 ALG:
//---------------------------------------------------------------------------------
TMultiLayerPerceptron*  N3_TMLP_ANN;
Bool_t      N3_DoTrain=kTRUE;
// Bool_t      N3_DoTrain=kFALSE;
Double_t    N3_Inputvar[24]; // 24 maximal input neurons
// This array is larger than possible used array for evaluation.
// The array with the correct size is created, when the N3_ANN_INPUTNEURONS
// variable is fixed (TMLP class demands arraysize = #inputneurons)
Int_t       N3_Type;  // 0: BG, 1: SG
Int_t       N3_ANN_NInput; // number of Inputvariables in total
TString     N3_ANN_Layout="";
Double_t    N3_OutputValue=0;
Int_t       N3_TrainNMax;

//=C=	Values valid for ShowerReco_Algorithm 11 = N3 ALG: NeuralNetwork
// Brick data related inputs
Int_t        N3_ANN_PLATE_DELTANMAX; // 0,1,2,3
// algorithm method related inputs
Int_t        N3_ANN_NTRAINEPOCHS; // 1,2,3,4 = 50,100,150,200
Int_t        N3_ANN_NHIDDENLAYER; // 1,2,3,4 = 2,3,5,7
Double_t     N3_ANN_OUTPUTTHRESHOLD; // 0..10 = 0.5, 0.55, 0.6 ...
Int_t        N3_ANN_EQUALIZESGBG;
// 1: about same numer of SG and BG tracks are used for training, 0: all BG Tracks are taken
// this is dependent by the other variables, thus it
// is explicitely calculated for ease of view
Int_t        N3_ANN_INPUTNEURONS;

// -------------------------------
// Calculating Functions:
// -------------------------------
void 		N3_Create_ALG_MLP(TTree* inputtree, Int_t parasetnr);
void 		N3_Load_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr);
void 		N3_Dump_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr);
void        N3_FindBestCompliments(EdbSegP* seg, EdbPattern* TestPattern, EdbPVRec* local_gAli, Int_t Downstream, Float_t&  mindeltaZ, Float_t& mindT, Float_t& mindR, Float_t& mindMinDist, Int_t &nDifferentSegs );
void        N3_ReadOptionFile();
