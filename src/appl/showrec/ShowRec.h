
//---------------------------------------------------------------------------------
//-			gEDBDEBUGLEVEL
Int_t gEDBDEBUGLEVEL=2;
//---------------------------------------------------------------------------------
//-			VARIABLES
//---------------------------------------------------------------------------------
Int_t cmd_PADI=0;
Int_t cmd_BTPA=0;
Int_t cmd_BGTP=0;
Int_t cmd_ALTP=2;
Int_t cmd_PASTART=0;
Int_t cmd_PAEND=0;
Int_t cmd_CUTTP=0;
Int_t cmd_FP=1;
Int_t cmd_LP=30;
Int_t cmd_MP=1;
Int_t cmd_NP=30;
Int_t cmd_LT=0;
Int_t cmd_MC=0;
Int_t cmd_MCFL=0;
Int_t cmd_FZHP=0;
Int_t cmd_ALI=0;  // 0 gAli from cp, 1: gALI from Linkd_tracks
Int_t cmd_MCMIX=0;
Int_t cmd_vtx=0;
Int_t cmd_FILETP=0; // addition variable to distinguis different treebranches...

char *cmd_lnkdef_name = "lnk.def";
Int_t cmd_gEDBDEBUGLEVEL=2;   //  2: normal; 3: long, 4 very long; 1: none
Int_t cmd_OUTPUTLEVEL=1;      //  2:all (long), 2:reco+paraset (long) 1: only reconstruction (short): 0 only eff.(short)
Int_t cmd_STOPLEVEL=0;      //  0: Full Program, 1,2,3,4 return program at different steps of program.
//---------------------------------------------------------------------------------
Int_t     GLOBAL_NBT,GLOBAL_NBTallMC,GLOBAL_NBTeMC,GLOBAL_NBTMC;
Double_t  GLOBAL_purall,GLOBAL_pure;
Double_t  GLOBAL_effall,GLOBAL_effe;
Double_t  GLOBAL_trckdens;
//---------------------------------------------------------------------------------
// In = Initiator = BT from which the shower reconstruction starts
// Works for all Basetracks, MC or background
Int_t     GLOBAL_InBT_Flag,GLOBAL_InBT_MC;
Double_t  GLOBAL_InBT_E,GLOBAL_InBT_TanTheta;
//---------------------------------------------------------------------------------
// Evt = Event = highest energetic BT at lowest z-position for the
// corresponding InBT (does not have to be an electron,f.e.)
// Works of course only for MC events
Int_t     GLOBAL_EvtBT_Flag,GLOBAL_EvtBT_MC;
Double_t  GLOBAL_EvtBT_E,GLOBAL_EvtBT_TanTheta;
Int_t     GLOBAL_EvtBT_FlagArray[99999],GLOBAL_EvtBT_MCArray[99999],GLOBAL_EvtBT_ZArray[99999];
Double_t  GLOBAL_EvtBT_EArray[99999],GLOBAL_EvtBT_TanThetaArray[99999];
//-----------
TObjArray* GLOBAL_VtxArray;
Float_t  GLOBAL_VtxArrayX[99999];
Float_t  GLOBAL_VtxArrayY[99999];
Float_t  GLOBAL_VtxArrayZ[99999];
//---------------------------------------------------------------------------------
ofstream 		STREAM_ShowRecEff;
ofstream 		STREAM_ShowShower;
TFile*			FILE_ShowRecEff;
TTree*			TREE_ShowRecEff;
TFile*			FILE_ShowRecHistos;
TFile*			FILE_ParaSetDefinitions;
TTree*			TREE_ParaSetDefinitions;
TString     STREAM_ShowRecEffName;
TString     STRING_ShowShowerName;
TTree*			TREE_ShowShower;
TFile*			FILE_ShowShower;
TString     STRING_ShowTracksName;
TFile*			FILE_ShowTracks;
//---------------------------------------------------------------------------------
EdbPVRec* 	GLOBAL_gAli;
TObjArray* 	GLOBAL_InBTArray;
Int_t       GLOBAL_InBTArrayEntries;
TObjArray* 	GLOBAL_ShowerSegArray;
//---------------------------------------------------------------------------------
Double_t		CUT_PARAMETER[10];
Int_t       GLOBAL_PARASETNR;
Int_t       GLOBAL_INBTSHOWERNR;
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//-			Internal Histograms
//---------------------------------------------------------------------------------
TProfile* NBTeMC_pure;
TH2F* Hist_NBTeMC_pure;
TProfile* NBTallMC_purall;
TH2F* Hist_NBTallMC_purall;
TProfile* NBTeMC_NBTMC;
TH2F* Hist_NBTeMC_NBTMC;
TProfile* NBTeMC_NBT;
TH2F* Hist_NBTeMC_NBT;
TProfile* NBT_InBTE;
TH2F* Hist_NBT_InBTE;
TProfile* NBTeMC_InBTE;
TH2F* Hist_NBTeMC_InBTE;
TProfile* pure_InBTE;
TH2F* Hist_pure_InBTE;
TProfile* purall_InBTE;
TH2F* Hist_purall_InBTE;
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//-			FUNCTIONS
//---------------------------------------------------------------------------------
void CheckInputParameters();
void SetDefaultValues_CommandLine();
void TakeDefaultValues_CommandLine();
void CreateOutPutStructures();
EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory();
Int_t	Check_ParasetDefinitionFile();
Int_t Open_ParasetDefinitionFile();
void Read_ParasetDefinitionTree();
void FillGlobalInBTArray();
void GetEvent_ParasetDefinitionTree(Int_t nr);
void RewriteSegmentPIDs(EdbPVRec* gAli);
void RewriteSegmentPIDs_SGPID_To_BGPID(EdbPVRec* gAli);
void RewriteSegmentPIDs_BGPID_To_SGPID(EdbPVRec* gAli);
// -------------------------------
void ReconstructShowers(Int_t nr);
void ReconstructShowers_CT(); // ALTP=0  // This is now the same as CA alg....

void ReconstructShowers_CL(); // ALTP=1    // TESTING

void ReconstructShowers_CA(); // ALTP=2
void ReconstructShowers_OI(); // ALTP=4
void ReconstructShowers_NN(); // ALTP=5
void ReconstructShowers_OI2(); // DEPRECIATED !!!!
void ReconstructShowers_SA();  // ONLY MC ALGORITHM
void ReconstructShowers_TC(); // ALTP=6
void ReconstructShowers_RC(); // ALTP=7
void ReconstructShowers_BW(); // ALTP=8 BackWard Algorithm
void ReconstructShowers_AG(); // ALTP=9 Advanced Gamm Algorithm

void ReconstructShowers_OPENMP();  /// DEBUG OPENMP TEST!!!!!!!
void ReconstructShowers_OPENMP_TEST(EdbSegP* InBT, TObjArray array);  /// DEBUG OPENMP TEST!!!!!!!
// -------------------------------

EdbPVRec* TransformEdbPVRec(EdbPVRec* gAli, EdbSegP* InitiatorBT);
EdbPVRec* TransformEdbPVRec_SA(EdbPVRec* gAli, EdbSegP* InitiatorBT);
EdbPVRec* TransformEdbPVRec_BackWard(EdbPVRec* gAli, EdbSegP* InitiatorBT);
Float_t local_halfpatternsize=1250;


Bool_t FindPrecedingBTs(EdbSegP* s1, EdbSegP* s2, EdbPVRec *gAli, TObjArray* showersegarray); //searches all BTs from showersegarray only
Bool_t FindPrecedingBTsSingleThetaAngle(EdbSegP* s1, EdbSegP* s2, EdbPVRec *gAli, TObjArray* showersegarray); //searches all BTs from showersegarray only
Bool_t FindPrecedingBTsSingleThetaAngleTCDEBUG(EdbSegP* s1, EdbSegP* s2, EdbPVRec *gAli, TObjArray* showersegarray); //searches all BTs from showersegarray only


Bool_t FindPrecedingBTs_local_gAli(EdbSegP* s1, EdbSegP* s2, EdbPVRec *gAli, TObjArray* showersegarray); //searches all BTs from gAli
Bool_t FindFollowingBTs(EdbSegP* s1, EdbSegP* s2, EdbPVRec *gAli, TObjArray* showersegarray);
Bool_t GetConeOrTubeDistanceToInBT(EdbSegP* sa, EdbSegP* s, Double_t ConeAngle, Double_t CylinderRadius);
Bool_t GetConeOrTubeDistanceToBTOfShowerArray(EdbSegP* sa, EdbSegP* InBT, TObjArray* showersegarray, Double_t CylinderRadius, Double_t ConeAngle);
Double_t GetdeltaTheta(EdbSegP* s1,EdbSegP* s2);
Double_t GetdeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2);
Double_t GetdeltaRWithPropagation(EdbSegP* s1,EdbSegP* s2);
Double_t GetdeltaRNoPropagation(EdbSegP* s,EdbSegP* stest);
Double_t GetSpatialDist(EdbSegP* s1,EdbSegP* s2);
Double_t GetMinimumDist(EdbSegP* s1,EdbSegP* s2);
inline Double_t GetdT(EdbSegP* s1,EdbSegP* s2) {
    return GetdeltaTheta(s1,s2);
}
inline Double_t GetdR(EdbSegP* s1,EdbSegP* s2) {
    return GetdeltaRWithPropagation(s1,s2);
}
inline Double_t GetdMinDist(EdbSegP* s1,EdbSegP* s2) {
    return GetMinimumDist(s1,s2);
}
Bool_t IsSameSegment(EdbSegP* seg1,EdbSegP* seg2);

Double_t CalcIP(EdbSegP* s, EdbVertex* v);
Double_t CalcIP(EdbSegP *s, double x, double y, double z);


Bool_t AddBTToArrayWithCeck(EdbSegP* tryAttachedSegment, TObjArray* GLOBAL_ShowerSegArray);

Bool_t CalcConeOrTubeDistanceToInBT(EdbSegP* sa, EdbSegP* InBT, Double_t CylinderRadius, Double_t ConeAngle);
EdbSegP* BuildShowerAxis(TObjArray* ShowerSegArray);
void 	FillShowerAxis();
Bool_t IsShowerSortedZ(TObjArray* showerarray);
void SortShowerZ(TObjArray* showerarray);
void Fill2GlobalInBTArray();
void BuildParametrizationsMCInfo_PGun(TString MCInfoFilename);


// -------------------------------
void CalcTrackDensity(EdbPattern* pat_interim,Float_t pat_interim_halfsize,Int_t& npat_int,Int_t& npat_total,Int_t& npatN);
void CalcEffPurOfShower(TObjArray* arr, Int_t &NBT, Int_t &NBTMC, Int_t &NBTallMC, Int_t &NBTeMC, Double_t &purall, Double_t  &pure);
void CalcEffPurOfShower2(TObjArray* arr, Int_t &NBT, Int_t &NBTMC, Int_t &NBTallMC, Int_t &NBTeMC, Double_t &purall, Double_t  &pure,Int_t n1, Int_t n2, Int_t n3);
void CalcEfficencyNumbers(EdbPattern* pat_interim, Int_t MCCheck, Int_t& NBT_Neff,Int_t& NBTMC_Neff,Int_t& NBTMCe_Neff);
void PrintShowerObjectArray(TObjArray* segarray);
void FillOutPutStructures();
// -------------------------------
TTree* CreateTreeBranchShowerTree(Int_t ParaSetNr);
void TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TTree* treebranchtree, TObjArray* segarray);
void MakeTracksTree(TTree* treebranch);
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//-     VARIABLES: related to specific ReconstructionAlgorithms:
//---------------------------------------------------------------------------------
//-     NN ALG:
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


Int_t GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
Int_t GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
Int_t GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter);
void Create_NN_Alg_Histograms();

TMultiLayerPerceptron*		TMlpANN;
void 		Get_NN_ALG_MLP(TTree* inputtree, Int_t parasetnr);
void 		Load_NN_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr);
//---------------------------------------------------------------------------------

//   CL ALG   ---------------------------------------------------------------------------------
// Histograms which are needed...
TH2F* Hist2DimOnlyBGAllPlates;
TH2F* Hist2DimOnlySimOneEventAllPlates;
TH2F* Hist2DimBGAndSimOneEventAllPlates;
TH2F* Hist2DimOnlyBGOneGroupedPlate[50];
TH2F* Hist2DimOnlySimOneEventOneGroupedPlate[50];
TH2F* Hist2DimBGAndSimOneEventOneGroupedPlate[50];
TH3F* Hist3DimOnlyBG;
TH3F* Hist3DimRecoEvent_gAli;
TH3F* Hist3DimRecoEvent_gAli_Dummy;
TSpectrum2 *spectrum[50];
TSpectrum2 *spectrum2dim;
TH2F* spectrum_interim;
// Functions which are needed...
void Reco_CL_BuildGlobalHistogramsOnHeap();
Int_t Reco_CL_AssignZValueToGroup(Double_t z, Double_t z0, Int_t NGroupedPLATES, Int_t PLATESPerGroup);
//   CL ALG   ---------------------------------------------------------------------------------




//---------------------------------------------------------------------------------
//-			VARIABLES: shower_  "treebranch"  reconstruction
//---------------------------------------------------------------------------------
Int_t shower_number_eventb, shower_sizeb, shower_isizeb,shower_showerID;
Int_t shower_sizeb15, shower_sizeb20, shower_sizeb30;
Float_t shower_energy_shot_particle;
Float_t shower_xb[5000];
Float_t shower_yb[5000];
Float_t shower_zb[5000];
Float_t shower_txb[5000];
Float_t shower_tyb[5000];
Float_t shower_deltarb[5000];
Float_t shower_deltathetab[5000];
Float_t shower_deltaxb[5000];
Float_t shower_deltayb[5000];
Int_t   shower_nfilmb[5000];
Int_t   shower_ngrainb[5000];
Float_t shower_chi2btkb[5000];
Int_t shower_ntrace1simub[5000]; // MCEvt
Int_t shower_ntrace2simub[5000]; // s->W()
Float_t shower_ntrace3simub[5000]; // s->P()
Int_t shower_ntrace4simub[5000]; // s->Flag()
Float_t shower_tagprimary[5000];
Int_t   shower_idb[5000];
Int_t   shower_plateb[5000];
Float_t shower_deltasigmathetab[58];
Int_t   shower_numberofilms;
Float_t shower_purb;
Float_t shower_trackdensb; //Track Density around the shower area, in BT/mm2
Float_t shower_axis_xb;
Float_t shower_axis_yb;
Float_t shower_axis_zb;
Float_t shower_axis_txb;
Float_t shower_axis_tyb;
// -- new: in case you want to merge different shower.root files, they can be distingued:
Int_t shower_filetype;
// -- new:  hole plates and consequitve hole plates:
Int_t shower_numberofholesconseq=0;
Int_t shower_numberofholes=0;
//---------------------------------------------------------------------------------




