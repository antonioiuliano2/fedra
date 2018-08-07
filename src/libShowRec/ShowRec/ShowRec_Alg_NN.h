//---------------------------------------------------------------------------------
//-     VARIABLES: related to specific ReconstructionAlgorithms:
//-     NN ALG:
//---------------------------------------------------------------------------------
Bool_t var_NN_DoTrain=kFALSE;
Bool_t var_NN_DoRUN=kTRUE;
TH1F* var_NN__BG__SpatialDist_TestBT_To_InBT;
TH1F* var_NN__SG__SpatialDist_TestBT_To_InBT;
TH1F* var_NN__ALL__SpatialDist_TestBT_To_InBT;
TH1F* var_NN__BG__zDiff_TestBT_To_InBT;
TH1F* var_NN__SG__zDiff_TestBT_To_InBT;
TH1F* var_NN__ALL__zDiff_TestBT_To_InBT;
//--------
TH1F* var_NN__BG__dT_InBT_To_TestBT;
TH1F* var_NN__SG__dT_InBT_To_TestBT;
TH1F* var_NN__ALL__dT_InBT_To_TestBT; //--------
TH1F* var_NN__SG__dR_InBT_To_TestBT;
TH1F* var_NN__BG__dR_InBT_To_TestBT;
TH1F* var_NN__ALL__dR_InBT_To_TestBT; //--------
TH1F* var_NN__SG__dR_TestBT_To_InBT;
TH1F* var_NN__BG__dR_TestBT_To_InBT;
TH1F* var_NN__ALL__dR_TestBT_To_InBT; //--------
// -------------------------------
TH1F*   var_NN__SG__nseg_TestBT_ToBeforePlate;
TH1F*   var_NN__BG__nseg_TestBT_ToBeforePlate;
TH1F*   var_NN__ALL__nseg_TestBT_ToBeforePlate;
TH1F*   var_NN__SG__nseg_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__nseg_TestBT_To2BeforePlate;
TH1F*   var_NN__ALL__nseg_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__nseg_TestBT_ToSamePlate;
TH1F*   var_NN__SG__nseg_TestBT_ToSamePlate;
TH1F*   var_NN__ALL__nseg_TestBT_ToSamePlate;
TH1F*   var_NN__SG__nseg_TestBT_ToAfterPlate;
TH1F*   var_NN__BG__nseg_TestBT_ToAfterPlate;
TH1F*   var_NN__ALL__nseg_TestBT_ToAfterPlate;
TH1F*   var_NN__SG__nseg_TestBT_To2AfterPlate;
TH1F*   var_NN__BG__nseg_TestBT_To2AfterPlate;
TH1F*   var_NN__ALL__nseg_TestBT_To2AfterPlate;
TH1F*   var_NN__SG__mean_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__BG__mean_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__ALL__mean_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__SG__mean_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__mean_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__ALL__mean_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__SG__mean_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__BG__mean_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__ALL__mean_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__SG__mean_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__mean_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__ALL__mean_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__SG__mean_dR_TestBT_ToSamePlate;
TH1F*   var_NN__BG__mean_dR_TestBT_ToSamePlate;
TH1F*   var_NN__ALL__mean_dR_TestBT_ToSamePlate;
TH1F*   var_NN__SG__mean_dT_TestBT_ToSamePlate;
TH1F*   var_NN__BG__mean_dT_TestBT_ToSamePlate;
TH1F*   var_NN__ALL__mean_dT_TestBT_ToSamePlate;
TH1F*   var_NN__SG__mean_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__BG__mean_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__ALL__mean_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__SG__mean_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__BG__mean_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__ALL__mean_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__SG__mean_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__BG__mean_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__ALL__mean_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__SG__mean_dT_TestBT_To2AfterPlate;
TH1F*   var_NN__BG__mean_dT_TestBT_To2AfterPlate;
TH1F*   var_NN__ALL__mean_dT_TestBT_To2AfterPlate;
TH1F*   var_NN__SG__min_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__BG__min_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__ALL__min_dR_TestBT_ToBeforePlate;
TH1F*   var_NN__SG__min_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__min_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__ALL__min_dR_TestBT_To2BeforePlate;
TH1F*   var_NN__SG__min_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__BG__min_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__ALL__min_dT_TestBT_ToBeforePlate;
TH1F*   var_NN__SG__min_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__BG__min_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__ALL__min_dT_TestBT_To2BeforePlate;
TH1F*   var_NN__SG__min_dR_TestBT_ToSamePlate;
TH1F*   var_NN__BG__min_dR_TestBT_ToSamePlate;
TH1F*   var_NN__ALL__min_dR_TestBT_ToSamePlate;
TH1F*   var_NN__SG__min_dT_TestBT_ToSamePlate;
TH1F*   var_NN__BG__min_dT_TestBT_ToSamePlate;
TH1F*   var_NN__ALL__min_dT_TestBT_ToSamePlate;
TH1F*   var_NN__SG__min_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__BG__min_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__ALL__min_dR_TestBT_ToAfterPlate;
TH1F*   var_NN__SG__min_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__BG__min_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__ALL__min_dT_TestBT_ToAfterPlate;
TH1F*   var_NN__SG__min_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__BG__min_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__ALL__min_dR_TestBT_To2AfterPlate;
TH1F*   var_NN__SG__min_dT_TestBT_To2AfterPlate;
TH1F*   var_NN__BG__min_dT_TestBT_To2AfterPlate;
TH1F*   var_NN__ALL__min_dT_TestBT_To2AfterPlate;
// -------------------------------
Int_t GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
Int_t GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
Int_t GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
void Create_NN_Alg_Histograms();
TMultiLayerPerceptron*		TMlpANN;
void 		Get_NN_ALG_MLP(TTree* inputtree, Int_t parasetnr);
void 		Load_NN_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr);
//---------------------------------------------------------------------------------
