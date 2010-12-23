#ifndef ROOT_EdbShowRec
#define ROOT_EdbShowRec

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TCut.h"
#include "TEventList.h"
#include "TSystem.h" 

#include "TMLPAnalyzer.h"


#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"

#include "EdbShowAlg.h"
#include "EdbShowAlg_NN.h"
#include "EdbShowAlgE.h"
#include "EdbShowerP.h"


using namespace std;










//______________________________________________________________________________

class EdbShowRec : public TObject {
  
  
  private:
    
    
    // -------------------------------------------------------
    // main private variables used for this Object:
    
    // What shall instance do?
    // Main big subs settings invoking the different modules:
    Int_t              eDoReconstruction;
    Int_t              eDoParametrization;
    Int_t              eDoId;
    Int_t              eDoEnergy;
    Int_t              eDoVtxAttach;
    
    // EdbPVRec object:
    EdbPVRec*          eAli;
    Int_t              eAliNpat;
    Bool_t             eAliLoaded;
    // TObjArray storing Initiator Basetracks:
    TObjArray*         eInBTArray;
    Int_t              eInBTArrayN;
    Bool_t             eInBTArrayLoaded;
    Int_t              eInBTArrayMaxSize;
    // TObjArray storing EdbShowAlg for the reconstructions:
    TObjArray*         eShowAlgArray;   
    Int_t              eShowAlgArrayN;
    Int_t              eShowAlgArraySingleN[10];
    Bool_t             eShowAlgArrayLoaded;
    Int_t              eShowAlgArrayMaxSize;
    // TObjArray storing reconstructed showers (objects of EdbShowerP class):
		TObjArray* 				 eRecoShowerArray;
		Int_t	    				 eRecoShowerArrayN;
    
    // TTree storing reconstructed showers  (objects of "treebranch" TTree, when reading Shower.root file):
    //                                      (backward compability)
    TTree*             eRecoShowerTreetreebranch;
    
    
    
    // variables used for setting the reconstruction "area", i.e. the plates.
    // note that these numbers refer to the  eAli  object and do not get bigger
    // than this object allows.
    // These variables are valid in the "global"  EdbShowRecFrame, which means
    // That the lowest Z of  eAli Object is labeled with eFirstPlate
    // whereas the highest Z of  eAli Object is labeled with eLastPlate
    Int_t              eFirstPlate;
    Int_t              eLastPlate;
    Int_t              eMiddlePlate;
    Int_t              eNumberPlate;
    
    // These variables are valid in the  eAli  frame. Therefore not necessary 
    // identical to the ones above.
    Int_t              eFirstPlate_eAliPID;
    Int_t              eLastPlate_eAliPID;
    Int_t              eMiddlePlate_eAliPID;
    Int_t              eNumberPlate_eAliPID;
    
    // These are proposed variables to store when default.par file is read via ReadShowRecPar()
    // Since the original values are not known at this time it is not possible to override them
    // without checking, that why they are saved interimly here
    Int_t              eProposedFirstPlate;
    Int_t              eProposedLastPlate;
    Int_t              eProposedMiddlePlate;
    Int_t              eProposedNumberPlate;
    
    
    Float_t            eAliZMax;
    Float_t            eAliZMin;
    Int_t              eAliZMaxPID;
    Int_t              eAliZMinPID;
    
    // "Upstream" or "Downstream"
    TString            eAliStreamType;
    Int_t              eAliStreamTypeStep;
    
    // Values for plate labeling of  eAli  is set?
    Bool_t            IsPlateValuesLabel;
    
    
    //  Values specifiying the actual algorithm and parameter set number
    EdbShowAlg*       eActualAlg;
    Int_t             eActualAlgParameterset[10];
        
    
    // Variables specifiying the possible parametrization types for the showers:
    Int_t             eParaTypes[8];
    TString           eParaNames[8];
    
    
    // Variables used for naming the files to read/write the specified 
    // data structures or specified objects.
		//	Read:
    TString           eFilename_LnkDef;
    TString           eFilename_LinkedTracks;
    TString           eFilename_EdbPVRecObject;
    TString           eFilename_In_shower;        // File containing showers in "EdbShowerP"
    TString           eFilename_In_treebranch;    // File containing showers in "treebranch"
    Bool_t            eReadFileShower;
    Bool_t            eReadFileTreebranch;
    TFile*            eFile_In_shower;            // File containing showers in "EdbShowerP" format
    TFile*            eFile_In_treebranch;        // File containing showers in "treebranch" format
    
    //	Write:
		TString           eFilename_Out_shower;       // File containing showers in "EdbShowerP" format
		TString           eFilename_Out_treebranch;   // File containing showers in "treebranch" format
    Bool_t            eWriteFileShower;
    Bool_t            eWriteFileTreebranch;
		TFile*     	      eFile_Out_shower;           // File containing showers in "EdbShowerP" format
		TFile*	          eFile_Out_treebranch;       // File containing showers in "treebranch" format
    
    
    
    // The names of reconstructed Objects follows the convention:
    // if only one algo,paraset, then its simply "shower" or "treebranch"
    // if more than one, then its followed by the algorithm acronym and paraset number:
    // for example "treebranch_SA_12"  ("SA" algorithm, paraset12)
    TString           eName_Out_shower;           // name of shower objects in "EdbShowerP" format
    TString           eName_Out_treebranch;       // name of shower objects in "EdbShowerP" format
    
    
    
    
    // variables, to tell wheter to take Initiabtor BT from:
    // a) from a linked_tracks.root file (LT)
    // b) from a gAli.root file (PVREC)
    // c) from the BaseTracks of the  eAli  object (AliBT)
    // d) from the linked tracks of the  eAli  object (AliLT)
    Bool_t            eUse_LT;
    Bool_t            eUse_PVREC;
    Bool_t            eUse_AliBT;
    Bool_t            eUse_AliLT;
    Int_t             eUseNr;
    
    TCut*             eInBTCuts[3];       //! root-style text cuts
    TTree*            eInBTTree;          // Tree for interim storing InBTs 
                                          // Used to apply TCut on segments.
    
    // Use the small eAli object (from ExtractSubpattern) or the whole
    // eAli object for reconstruction:
    Bool_t            eUseAliSub;	
    
    
    
    
    
    // -------------------------------------------------------
    // private functions:
    
    void              Set0(); // Default Reseting of object pointer adresses
    void              Init(); // Default Resetting of internal variables.
    
    
    // Check that consistency of the plate numbers with the given  eAli  object.
    void              CheckPlateValues();
        
    // Set plate numbers determined by the  eAli  object only.
    void              SetPlateNumberValues();
    
    

		
		// Function to create the  eShowAlgArray  if it is not already done (in Constructor or by SetShowAlgArray())
		void              Create_eShowAlgArray();
		
		 // Function to fill the  eShowAlgArray  if it is not already done (in Constructor or by SetShowAlgArray())
		void              Fill_eShowAlgArray();
		
    // Function to add algorithm instances to  the  eShowAlgArray 
		void              AddAlg(Int_t AlgType, Float_t* par);
    // Function to reset all added  algorithm instances to  the  eShowAlgArray 
    void              ResetShowAlgArray();
    
// 		AddAlg(Int_t AlgType, Float* par)
    
    
    // Function to reset the kind of filling  the  eInBTArray:
    void              ResetUse_InBTType();
    
		// Function to create the  eInBTArray  if it is not already done (in Constructor or by SetInBTArray())
		void              Create_eInBTArray();
		
// Check that consistency of Initiator BT array:
//     void              Check_eInBTArray();
    
    // Function to create the  eInBTTree  which is used for CutString when filling eInBTArray
    void              Create_eInBTTree();
    
    // Function to apply cuts on the  eInBTTree  and fill the eInBTArray
    void              Cut_eInBTTree_To_InBTArray();
		
    // Function to fill the  eInBTArray  if it is not already done (in Constructor or by SetInBTArray())
    void              Fill_eInBTArray(Int_t FilType);
    void              Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();
    void              Fill_eInBTArray_ByRecoLinkTracks_eAli();
    void              Fill_eInBTArray_ByBaseTracksOf_eAli();
    
    // Function to add InBT cut, which is used to select the initiator basetracks:
    void              Add_INBTCut(Int_t layer, TCut &cut);
    
    
    // Function to set names of treebranch and shower objects used to write in file.
    void              SetOutNames();
 
		
    // Make Parametrizations for the stored showers in the eRecoShowerArray  
    void              BuildParametrizations();
    void              BuildParametrizationsAll();
    
    
		
    
    
    
		/// Write Reconstructed Showers to File:
		void							Write_RecoShowerArray(TObjArray* RecoShowerArray, TString Filename_Out_EdbShowerP );
    /// NOT WORKLING YET::::  still segmentation faults..../// solved but i dont know why....
    
    
    
    
    
    
    
    // Main functions for using this ShowerReconstruction Object.
    // Structure is made similar to OpRelease, where 
    //    Initialize,   Execute,  Finalize 
    // give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();
    


	protected:

		
    
  
  public:
        
    
    EdbShowRec();
    EdbShowRec(EdbPVRec* gAli);
    EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray);
    EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, EdbShowAlg* StandardAlg);
    EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray ,TObjArray* ShowAlgArray);
//     EdbShowRec(TString treebranchFileName); // DEBUG // doesnt work, see explanation in .cxx file
    EdbShowRec(TString treebranchFileName, TString treebranchName);
    EdbShowRec(TString TxtFileName, Int_t TxtFileType);
    
    virtual ~EdbShowRec();          // virtual constructor due to inherited class
    
    // Reconstruct showers: invoke all methods availible for all reconstruction things.
    void              Reconstruct();
		void 							ReconstructTEST();
    void              ReconstructTESTSHORT();
    void              ReconstructTESTSTANDARD();
    void              ReconstructTEST_CA();
    void              ReconstructTEST_OI();
    void              ReconstructTEST_NN();
    

		// Reset Functions:

		// reset all values to the standard ones.
    void              Reset();
		void 							ResetRecoShowerArray();
		void 							ResetInBTArray();

    
    // Set Functions:
    
    // General Tasks of Class
    inline void         SetDoReconstruction( Int_t dospec )            { eDoReconstruction = dospec; }
    inline void         SetDoParametrisation( Int_t dospec )           { eDoParametrization = dospec; }
    inline void         SetDoId( Int_t dospec )                        { eDoId = dospec; }
    inline void         SetDoEnergy( Int_t dospec )                    { eDoEnergy = dospec; }
    inline void         SetDoVtxAttach( Int_t dospec )                 { eDoVtxAttach = dospec; }
    
    // Specific Tasks of Class
    inline void         SetEdbPVRec( EdbPVRec* Ali )          { eAli = Ali; eAliNpat=eAli->Npatterns(); }
    inline void         SetInBTArray( TObjArray* InBTArray ) { eInBTArray = InBTArray; eInBTArrayN=eInBTArray->GetEntries(); }
    inline void         SetShowAlgArray( TObjArray* ShowAlgArray ) { eShowAlgArray = ShowAlgArray; eShowAlgArrayN=eShowAlgArray->GetEntries(); }
    inline void         SetRecoShowerArray( TObjArray* RecoShowerArray)      { eRecoShowerArray = RecoShowerArray; eRecoShowerArrayN=eRecoShowerArray->GetEntries(); }
    inline void         SetInBTArrayN(Int_t InBTArrayN)      { eInBTArrayN = InBTArrayN; }
    inline void         SetShowAlgArrayN(Int_t ShowAlgArrayN)      { eShowAlgArrayN = ShowAlgArrayN; }
    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN)      { eRecoShowerArrayN = RecoShowerArrayN; }
    
    
    
    
    inline void         SetFirstPlate( Int_t FirstPlate )          { eFirstPlate = FirstPlate; }
    inline void         SetLastPlate( Int_t LastPlate )            { eLastPlate = LastPlate; }
    inline void         SetMiddlePlate( Int_t MiddlePlate )          { eMiddlePlate = MiddlePlate; }
    inline void         SetNumberPlate( Int_t NumberPlate )          { eNumberPlate = NumberPlate; }
    
    void                SetFirstPlate_eAliPID(Int_t FP, Int_t StreamTypeStep);
    void                SetLastPlate_eAliPID(Int_t plate, Int_t StreamTypeStep);
    void                SetMiddlePlate_eAliPID(Int_t plate, Int_t StreamTypeStep);
    void                SetNumberPlate_eAliPID(Int_t plate, Int_t StreamTypeStep);
        
    inline void         SetShowName(TString Name_Out_shower)      { eName_Out_shower = Name_Out_shower; }
    inline void         SetWriteFileShower(Int_t WriteFileShower) { eWriteFileShower = WriteFileShower; } 
    inline void         SetTreebranchName(TString Name_Out_treebranch)      { eName_Out_treebranch = Name_Out_treebranch; }
   
    inline void         SetShowFileName(TString Filename_Out_shower)      { eFilename_Out_shower = Filename_Out_shower; }
    inline void         SetWriteFileTreebranch(Int_t WriteFileTreebranch) { eWriteFileTreebranch = WriteFileTreebranch; } 
    inline void         SetTreebranchFileName(TString Filename_Out_treebranch)      { eFilename_Out_treebranch = Filename_Out_treebranch; }
    
    void                SetSimpleFileName(Int_t type, Int_t dotype);
    
    void                SetDoParaType(Int_t type);
    void                SetDoParaType(TString typestring);
    void                SetUseAliSub(Int_t type);
    
    inline Int_t        SetDoParaType( Int_t type )      const       { return   eParaTypes[type]; }
    
    
    // Add Functions:
    void         AddEdbPVRec( EdbPVRec* Ali );
    void         AddInBTArray( TObjArray* InBTArray );
    void         AddShowAlgArray( TObjArray* ShowAlgArray );
    void         AddRecoShowerArray( TObjArray* RecoShowerArray);
    
    
    
    // Get Functions:
    inline EdbPVRec*    GetEdbPVRec( )      const       { return eAli; }
    inline Bool_t       GetEdbPVRecLoaded()      const  { return eAliLoaded; }
    inline TObjArray*   GetInBTArray( )     const       { return eInBTArray; }
    inline TObjArray*   GetShowAlgArray( )  const       { return eShowAlgArray; }
    inline TObjArray*   GetRecoShowerArray( )     const       { return eRecoShowerArray; }
    inline EdbShowerP*  GetShower(Int_t i)     const       { return (EdbShowerP*)eRecoShowerArray->At(i); }
    
    
    inline Int_t        GetInBTArrayN()     const       { return eInBTArrayN; }
    inline Int_t        GetShowAlgArrayN()  const       { return eShowAlgArrayN; }
    inline Int_t        GetRecoShowerArrayN()     const       { return eRecoShowerArrayN; }
    
    
    
    inline Int_t        GetFirstPlate( )      const       { return eFirstPlate; }
    inline Int_t        GetLastPlate( )      const       { return eLastPlate; }
    inline Int_t        GetMiddlePlate( )      const       { return eMiddlePlate; }
    inline Int_t        GetNumberPlate( )      const       { return eNumberPlate; }
		
		inline Int_t        GetProposedFirstPlate( )      const       	{ return eProposedFirstPlate; }
		inline Int_t        GetProposedLastPlate( )     	 const       { return eProposedLastPlate; }
		inline Int_t        GetProposedMiddlePlate( )      const       { return eProposedMiddlePlate; }
		inline Int_t        GetProposedNumberPlate( )      const       { return eProposedNumberPlate; }
    
    inline TString      GetShowName( )      const       { return eName_Out_shower; }
    inline TString      GetTreebranchName( )      const       { return eName_Out_treebranch; }
    
    
    
    
    // Get Functions:
    inline void Update() { cout << "EdbShowRec::Update ONLY showers!"<< endl; for (int i=0;i<GetRecoShowerArrayN();++i) GetShower(i)->Update(); } 
    inline void UpdateX() { cout << "EdbShowRec::Update ONLY showers!"<< endl; for (int i=0;i<GetRecoShowerArrayN();++i) GetShower(i)->UpdateX(); } 
    
    
    
    ///--------------------------------------------------------------------------------
    /// --- TEMPORARY PUBLIC FUNCTIONS.... LATER THESE SHOULD PUT INTO PRIVATE....
    /// --- TEMPORARY PUBLIC FUNCTIONS.... NOW ONLY FOR DEBUGGING PURPOSES....
    	// Check that consistency of Initiator BT array:
    void              Check_eInBTArray();
    ///--------------------------------------------------------------------------------
    
    
    
    // -------------------------------------------------------------    
    // Public functions to be used from any class instances:
    
    // Function to convert the  eRecoShowerArray  into an  "treebranch"  tree
    void              RecoShowerArray_To_Treebranch();
    // Function to convert a  "treebranch"  tree  into an  eRecoShowerArray
    void              Treebranch_To_RecoShowerArray(TObjArray* showarr, TTree* treebranch);
    
    // Function to convert a txtfile with segments lists  into an  eRecoShowerArray
    void 		TxtToRecoShowerArray();
    void		TxtToRecoShowerArray(TString TxtFileName, Int_t TxtFileType);
    void 		TxtToRecoShowerArray_FeedBack(TString TxtFileName);
    void 		TxtToRecoShowerArray_SimpleList(TString TxtFileName);
    void 		TxtToRecoShowerArray_SimpleListNagoya(TString TxtFileName);
    void 		TxtToRecoShowerArray_EDAList(TString TxtFileName);
    
    
    // test:  Write the Parametrisation from the eRecoShowerArray into a tree.
    //        (Satisfies Backward compability).
    void      WriteParametrisation_FJ(); //0
    void      WriteParametrisation_LT(); //1
    void      WriteParametrisation_YC(); //2
		///void      WriteParametrisation_JC(); //3
    void      WriteParametrisation_XX(); //4
    void      WriteParametrisation_YY(); //5
		///void      WriteParametrisation_PP(); //6
    void      WriteParametrisation_AS(); //7
		void  		WriteParametrisation_SE(); //8

    void      WriteParametrisation_ExtraInfo();
    
    // Make MCInfo structures for the stored showers in the eRecoShowerArray:
    void              BuildParametrizationsMCInfo(TString MCInfoFilename, Int_t type);
    void              BuildParametrizationsMCInfo_PGun(TString MCInfoFilename);
    void              BuildParametrizationsMCInfo_Event(TString MCInfoFilename);
    void              WriteParametrisation_MCInfo_PGun();
    void 	      ExtendParametrisation_ExtraInfo(); // works only if there is a WriteParametrisation_MCInfo_PGun done.
    
    
		// File Status Functions
    void CheckFilePresence();
    bool FileExists(string strFilename);

		// Read parameters out of default.par file
    int               ReadShowRecPar(const char *file);
    

    // Many Print Functions:
    void Print();
    void PrintMore();
    void PrintAll();
    void PrintRecoShowerArray();
    void PrintParametrisation(Int_t ParaNr);
    
    // Print functions (mainly for debug mode)
    void              Print_UseInBTType();
    void              Print_InBTCut(Int_t layer);
    void              Print_InBTArray();
    

    //-C- HELP function , in case you dont know anything about it .....
    void Help();
    
    ClassDef(EdbShowRec,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

#endif /* ROOT_EdbShowRec */
