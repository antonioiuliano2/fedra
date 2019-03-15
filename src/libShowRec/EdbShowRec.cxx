#include "EdbShowRec.h"

using namespace std;

ClassImp(EdbShowRec)


//______________________________________________________________________________

EdbShowRec::EdbShowRec()
{
    // Default Constructor
    Log(2,"EdbShowRec::EdbShowRec()","Default Constructor");
    Set0();
    Init();
    Print();
    Log(2,"EdbShowRec::EdbShowRec()","Default Constructor...done.");
}

//______________________________________________________________________________

EdbShowRec::EdbShowRec(EdbPVRec* gAli)
{
    // Default Constructor using a EdbPVRec object (usually called gAli):
    // By default, do _not_ do a reconstruction of showers at this stage
    // Reconstruction() call is to made by hand at the time being

    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli)","Default Constructor");
    Set0();

    eAli=gAli;
    eAliNpat=eAli->Npatterns();
    eAliLoaded=kTRUE;

    // Create already the RecoShowerArray at the very beginning.
    // Recheck if this is good style please...
    eRecoShowerArray=new TObjArray();
    SetRecoShowerArrayN(0);

    // Set Shower Algorithm Array to zero
    SetShowAlgArrayN(0);

    Init();

    SetPlateNumberValues();
    CheckPlateValues();

    // In this constructor no InBT Array is given by default, but we search
    // for possible InBTs automatically<f.
    Check_eInBTArray();

    Print();
    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli)","Default Constructor...done.");
}

//______________________________________________________________________________

EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray)
{
    // Default Constructor using
    //  a EdbPVRec object (usually called gAli)
    //  a TObjArray containing segments (basetracks) for the Initiator BTs:
    // By default, do _not_ do a reconstruction of showers at this stage
    // Reconstruction() call is to made by hand at the time being

    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray)","Default Constructor");
    Set0();

    eAli=gAli;
    eAliNpat=eAli->Npatterns();
    eAliLoaded=kTRUE;

    // Preliminary check if array is already existing:
    eInBTArray = new TObjArray();
    eInBTArray=InBTArray;
    SetInBTArrayN(eInBTArray->GetEntries());
    eInBTArrayMaxSize=999999;
    eInBTArrayLoaded=kTRUE;

    // Create already the RecoShowerArray at the very beginning.
    // Recheck if this is good style please...
    eRecoShowerArray=new TObjArray();
    SetRecoShowerArrayN(0);

    // Set Shower Algorithm Array to zero
    SetShowAlgArrayN(0);

    Init();

    /// STILL DEBUG STATUS....... IS THIS TRUE???-------------------------------
    // If Initiator BT Array and gAli is given then we can directly proceed to check plate values!!
    // Initiator BT Array  is already filled, so every other method to fill this must be kFALSE;
    eUse_LT=kFALSE;
    eUse_AliBT=kFALSE;
    eUse_AliLT=kFALSE;
    eUse_PVREC=kFALSE;
    eUseNr=-1;
    SetPlateNumberValues();
    CheckPlateValues();
    /// STILL DEBUG STATUS....... IS THIS TRUE???-------------------------------

    Print();
    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray)","Default Constructor...done.");
}

//______________________________________________________________________________

EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, EdbShowAlg* StandardAlg)
{
    // Default Constructor using
    //  a EdbPVRec object (usually called gAli)
    //  a TObjArray containing segments (basetracks) for the Initiator BTs
    //  a EdbShowAlg object (StandardAlg):

    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, EdbShowAlg* StandardAlg)","Default Constructor");
    Set0();

    eAli=gAli;
    eAliNpat=eAli->Npatterns();
    eAliLoaded=kTRUE;

    // Preliminary check if array is already existing:
    eInBTArray = new TObjArray();
    eInBTArray=InBTArray;
    SetInBTArrayN(eInBTArray->GetEntries());
    eInBTArrayMaxSize=999999;
    eInBTArrayLoaded=kTRUE;

    eShowAlgArray = new TObjArray();
    eShowAlgArray->Add(StandardAlg);
    SetShowAlgArrayN(eShowAlgArray->GetEntries());
    eShowAlgArrayMaxSize=9999;
    eShowAlgArrayLoaded=kTRUE;

    // Create already the RecoShowerArray at the very beginning.
    // Recheck if this is good Style please...
    eRecoShowerArray=new TObjArray();

    Init();

    /// STILL DEBUG STATUS....... IS THIS TRUE???-------------------------------
    // If Initiator BT Array and gAli is given then we can directly proceed to check plate values!!
    // Initiator BT Array  is already filled, so every other method to fill this must be kFALSE;
    eUse_LT=kFALSE;
    eUse_AliBT=kFALSE;
    eUse_AliLT=kFALSE;
    eUse_PVREC=kFALSE;
    eUseNr=-1;
    SetPlateNumberValues();
    CheckPlateValues();
    /// STILL DEBUG STATUS....... IS THIS TRUE???-------------------------------

//     if (gEDBDEBUGLEVEL>0) Print();
    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, EdbShowAlg* StandardAlg)","Default Constructor...done.");
}

//______________________________________________________________________________


EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, TObjArray* ShowAlgArray)
{
    // Default Constructor using
    //  a EdbPVRec object (usually called gAli)
    //  a TObjArray containing segments (basetracks) for the Initiator BTs
    //  a EdbShowAlg object (StandardAlg):

    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, TObjArray* ShowAlgArray)","Default Constructor");
    Set0();

    eAli=gAli;
    eAliNpat=eAli->Npatterns();
    eAliLoaded=kTRUE;

    // Preliminary check if array is already existing:
    //  if(!eInBTArray)
    eInBTArray = new TObjArray();
    eInBTArray=InBTArray;
    SetInBTArrayN(eInBTArray->GetEntries());
    eInBTArrayMaxSize=999999;
    eInBTArrayLoaded=kTRUE;
    //  if(!eShowAlgArray)
    eShowAlgArray = new TObjArray();
    eShowAlgArray=ShowAlgArray;
    SetShowAlgArrayN(eShowAlgArray->GetEntries());
    eShowAlgArrayMaxSize=9999;
    eShowAlgArrayLoaded=kTRUE;

    // Create already the RecoShowerArray at the very beginning.
    // Recheck if this is good Style please...
    eRecoShowerArray=new TObjArray();

    Init();

    //if (gEDBDEBUGLEVEL>0) Print();
    Log(2,"EdbShowRec::EdbShowRec(EdbPVRec* gAli, TObjArray* InBTArray, TObjArray* ShowAlgArray)","Default Constructor...done.");
}


//______________________________________________________________________________
/// Strangely this constructor does not work... If I use like this:
/// EdbShowRec* ee = new EdbShowRec("Shower.root") then at the end
/// The object ee  calls its contructor and the objects isnt there anymore....
/// Some c++ tutorials say calling constructor from constructor inst possible so easy...
/// http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.3
/// For now we take this opportunity out... maybe someday we find a better one...
/// Ok, for now we copy the code from the '
/// EdbShowRec::EdbShowRec(TString treebranchFileName, TString treebranchName="treebranch")
/// ' constructor. See always that these codes then are equal:
// EdbShowRec::EdbShowRec(TString treebranchFileName)
// {
//    EdbShowRec::EdbShowRec(treebranchFileName,"treebranch");
// }

//______________________________________________________________________________

EdbShowRec::EdbShowRec(TString treebranchFileName, TString treebranchName="treebranch")
{
    //----------------------------------------------------------
    // Default Constructor using
    // a file which contains "treebranch" reconstructed showers
    // Converts them to the RecoShowerArray.
    // Mainly used for backward compability!
    //----------------------------------------------------------
    Log(2,"EdbShowRec::EdbShowRec(TString treebranchFileName, TString treebranchName=treebranch)","Default Constructor");


    Set0();
    Init();

    eRecoShowerArray=new TObjArray();
    SetRecoShowerArrayN(999999);

    // Just open given file:
    cout << "EdbShowRec::EdbShowRec() Just open given file:  "<< treebranchFileName << endl;
    TFile* file=new TFile(treebranchFileName);

    // Try to find ParasetNumber from Filename....(useful only for the reconstruction
    // where the file was named like:
    // "ShowRecShower__PADI_0_BTPA_0_BGTP_0_ALTP_4_FP_1_MP_21_NP_30_LT_0_MC_1_FZHP_1__ParaSet_-1_To_-1.txt.root"
// 	Bool_t isSpecialName=kTRUE;
    Bool_t isSpecialName=kFALSE;

    if (isSpecialName) {
        TString buf=treebranchFileName;
        cout << "buf = " <<  buf << endl;
        int paset;
        sscanf(buf.Data()+buf.Index("ParaSet_")+8,"%d", &paset);
        cout << "  paset = " << paset << endl;
        treebranchName = TString(Form(treebranchName+"_%d",paset));
    }
    else {
        // nothing, take normal treebranchName
    }
    cout << "treebranchName   = " << treebranchName   << endl;
    TTree* treebranchTree=(TTree*)file->Get(treebranchName);


    // Get treebranch tree out of it:
    if (!eRecoShowerTreetreebranch) {
        eRecoShowerTreetreebranch= new TTree();
    }
    else {
        ; // do nothing....
    }

    /*
      cout << "EdbShowRec::EdbShowRec() Get treebranch tree out of it:"<< endl;
      TTree* treebranchTree=(TTree*)file->Get(treebranchName);
    	if (treebranchTree==NULL && treebranchName=="treebranch") {
    		cout << "treebranchTree==NULL"<< endl;
    		treebranchName = TString(Form(treebranchName+"_%d",paset));
    		cout << treebranchName  << endl;
    		TTree* treebranchTree=(TTree*)file->Get(treebranchName);
    		if (treebranchTree==NULL) cout << "treebranchTree==NULL"<< endl;
    	}
    */

    // Clone it to have it also when closing the file(s):
    eRecoShowerTreetreebranch=(TTree*)treebranchTree->Clone();

    treebranchTree->Print();
    eRecoShowerTreetreebranch->Print();

    cout << "treebranchTree  " << treebranchTree << endl;
    cout << "eRecoShowerTreetreebranch  " << eRecoShowerTreetreebranch << endl;
//     return;


    // Try version with tree as  new  operator, in order not to loose it when closing file
    if (gEDBDEBUGLEVEL>2) treebranchTree->Show(0);


    // Write Treebranch_To_RecoShowerArray
    cout << "EdbShowRec::EdbShowRec() Write Treebranch_To_RecoShowerArray:"<< endl;
    Treebranch_To_RecoShowerArray(eRecoShowerArray, treebranchTree);

    if (gEDBDEBUGLEVEL>3) cout << "EdbShowRec::EdbShowRec()  eRecoShowerArray->Print();"<< endl;
    if (gEDBDEBUGLEVEL>3) eRecoShowerArray->Print();
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowRec::EdbShowRec()  eRecoShowerArray->Print()...done."<< endl;

//    cout << "EdbShowRec::EdbShowRec()  do now: delete file;"<< endl;
//    delete file; file=0;
//    cout << "EdbShowRec::EdbShowRec()  delete file done...;"<< endl;
    // This may be the reason why the histogram creation for shower crashes....
    // to be investigated....(maybe it is due to histogram creation in root, cause it wants to
    // Append the THI when creating  to a file.; Otherwise the crash happens only for a shower
    // file with 1 entry, for a file with 5 entries, for example it works....Very strange...)
    // maybe the histogram    TH1F*          eProfileLongitudinal;TH1F*         eProfileTransversal;
    // is still connected to this file!

    Log(2,"EdbShowRec::EdbShowRec()","EdbShowRec(TString treebranchFileName)... Do  NOT delete file because the Histograms are still connected with");
    Log(2,"EdbShowRec::EdbShowRec()","EdbShowRec(TString treebranchFileName)... the file.... THIS IS TO BE DONE CORRECTLY .... ");
    if (gEDBDEBUGLEVEL>1) Print();

    Log(2,"EdbShowRec::EdbShowRec(TString treebranchFileName, TString treebranchName=treebranch)","Default Constructor...done.");
}


//______________________________________________________________________________

EdbShowRec::EdbShowRec(TString TxtFileName, Int_t TxtFileType)
{
    //----------------------------------------------------------
    // Default Constructor using
    // a Txt File which contains segments
    // Converts them to the RecoShowerArray.
    // There are different types of listing segments (eda,manual check...)
    // This will be defined....
    //----------------------------------------------------------
    Log(2,"EdbShowRec::EdbShowRec(TString TxtFileName, Int_t TxtFileType)","Default Constructor  STILL  EMPTY  CONSTRUCTOR ");

    Set0();
    Init();
    eRecoShowerArray=new TObjArray();

    // Just open given txt file:
    cout << "EdbShowRec::EdbShowRec()   Just open given txt file:  "<< TxtFileName << endl;

    // Convert File To Reco Shower Array ...
    TxtToRecoShowerArray(TxtFileName,TxtFileType);

    Log(2,"EdbShowRec::EdbShowRec(TString TxtFileName, Int_t TxtFileType)","Default Constructor...done.");
}


//______________________________________________________________________________



EdbShowRec::~EdbShowRec()
{
    // Default Destructor
    Log(2,"EdbShowRec::~EdbShowRec()","Destructor called.");
}

//______________________________________________________________________________

void EdbShowRec::Set0()
{
    Log(2,"EdbShowRec::Set0()","Default reset function");
    // Default reset function
    eDoReconstruction=0;
    eDoParametrization=0;
    eDoId=0;
    eDoEnergy=0;
    eDoVtxAttach=0;

    eAli=0;
    eAliNpat=0;
    eAliLoaded=kFALSE;

    eInBTArrayFraction=1;

    eUseAliSub=0;

    eAliZMax=0;
    eAliZMin=0;

    eFirstPlate_eAliPID=-1;
    eMiddlePlate_eAliPID=-1;
    eLastPlate_eAliPID=-1;
    eNumberPlate_eAliPID=-1;
    IsPlateValuesLabel=kFALSE;

    eInBTArray=0;
    eInBTArrayN=0;
    eInBTArrayLoaded=kFALSE;
    eInBTArrayMaxSize=999999;

    eShowAlgArray=0;
    eShowAlgArrayN=0;
    eShowAlgArrayLoaded=kFALSE;
    eShowAlgArrayMaxSize=9999;

    eRecoShowerArray=0;
    eRecoShowerArrayN=0;

    eRecoShowerTreetreebranch=0;


    for (Int_t i=0; i<3; ++i) eInBTCuts[i]=0;
    for (Int_t i=0; i<7; ++i) eParaTypes[i]=0;

    eParaNames[0]="FJ";
    eParaNames[1]="LT";
    eParaNames[2]="YC";
    eParaNames[3]="JC";
    eParaNames[4]="XX";
    eParaNames[5]="YY";
    eParaNames[6]="PP";
    eInBTTree=0;

    Log(2,"EdbShowRec::Set0()","Default reset function...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Init()
{
    // Default Init() Function: Setting Pointers, Variables and so on:

    Log(2,"EdbShowRec::Init()","Default Init function");

    eFilename_LinkedTracks    ="linked_tracks.root";
    eFilename_LnkDef          ="lnk_all.def";
    eFilename_EdbPVRecObject  ="gAli.root";
    eFilename_Out_shower      ="ShowerReco.root";
    eFilename_Out_treebranch  ="ShowerTreebranch.root";


    eWriteFileShower=0;
    eWriteFileTreebranch=0;
    eFile_Out_shower=0;
    eFile_Out_treebranch=0;


    eUseNr=0;
    eUse_LT=kTRUE;
    eUse_AliBT=kFALSE;
    eUse_AliLT=kFALSE;
    eUse_PVREC=kFALSE;

    eInBTArrayFraction=1;

    eActualAlg=0;
    for (int k=0; k<10; k++) {
        eActualAlgParameterset[k]=0;
    }

    eProposedFirstPlate=-1;
    eProposedLastPlate=-1;
    eProposedMiddlePlate=-1;
    eProposedNumberPlate=-1;


    Log(2,"EdbShowRec::Init()","Default Init function...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::ResetRecoShowerArray()
{
    Log(2,"EdbShowRec::ResetRecoShowerArray()","Clear RecoShowerArray");
    // Clear RecoShowerArray:
    if (!eRecoShowerArray) return;
    eRecoShowerArray->Clear();
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    Log(2,"EdbShowRec::ResetRecoShowerArray()","Clear RecoShowerArray...done.");
    return;
}


//______________________________________________________________________________


void EdbShowRec::ResetInBTArray()
{
    Log(2,"EdbShowRec::ResetInBTArray()","Clear eInBTArray");
    // Clear eInBTArray:
    if (!eInBTArray) return;
    eInBTArray->Clear();
    SetInBTArrayN(eInBTArray->GetEntries());
    Log(2,"EdbShowRec::ResetInBTArray()","Clear eInBTArray...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::CheckFilePresence()
{
    // Check if relevant files are present in current directory:
    //  file for  linked_tracks, lnk.def, EdbPVR volume.
    Log(2,"EdbShowRec::CheckFilePresence()","Check if relevant files are present in current directory");

    Bool_t eFilename_LinkedTracks_Exists;
    Bool_t eFilename_LnkDef_Exists;
    Bool_t eFilename_EdbPVRecObject_Exists;

    eFilename_LinkedTracks_Exists=FileExists(std::string(eFilename_LinkedTracks));
    eFilename_LnkDef_Exists=FileExists(std::string(eFilename_LnkDef));
    eFilename_EdbPVRecObject_Exists=FileExists(std::string(eFilename_EdbPVRecObject));

    if (!FileExists(std::string(eFilename_LinkedTracks)))   Log(2,"EdbShowRec::CheckFilePresence()","WARNING: File  eFilename_LinkedTracks  does not exist ");
    if (!FileExists(std::string(eFilename_LnkDef)))         Log(2,"EdbShowRec::CheckFilePresence()","ERROR: File eFilename_LnkDef  does not exist ");
    if (!FileExists(std::string(eFilename_EdbPVRecObject))) Log(2,"EdbShowRec::CheckFilePresence()","WARNING: File  eFilename_EdbPVRecObject does not exist ");

    Log(2,"EdbShowRec::CheckFilePresence()","No action taken. TO BE IMPLEMENTED THAT IN CASE FILES ARE NOT PRESENT, that PROGRAM IS STOPPED");

    Log(2,"EdbShowRec::CheckFilePresence()","Check if relevant files are present in current directory...done.");
    return;
}


//______________________________________________________________________________



void EdbShowRec::ReconstructTESTSHORT()
{
    return;
}


//______________________________________________________________________________



void EdbShowRec::ReconstructTESTSTANDARD()
{
    // Standard reco function using ...
    cout << endl << endl;
    ReadShowRecPar("default.par");
    // this->SetPlateNumberValues();
    // not necessary now anymore since the ePropoaseNumberPlates are now incorporated
    // in ReadShowRecPar and CheckPlates()
    if (!eInBTArray)  Create_eInBTArray();
    if (!eInBTTree)   Create_eInBTTree();
    Fill_eInBTArray(eUseNr);
    cout << "eShowAlgArrayN =  " << eShowAlgArrayN << endl;
    cout << "eShowAlgArray =  " << eShowAlgArray << endl;
    if (!eShowAlgArray)   {
        Create_eShowAlgArray();
        Fill_eShowAlgArray();
    }

    cout << "DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG " << endl;
    eUseAliSub=kTRUE;
    cout << "EdbShowRec::SetUseAliSub   eUseAliSub  " <<  eUseAliSub << endl;

    cout << "DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG " << endl;
    eUseAliSub=kFALSE;
    cout << "EdbShowRec::SetUseAliSub   eUseAliSub  " <<  eUseAliSub << endl;

    // --------------------- Main Print Function -----------------------
    Print();
    // --------------------- Main Print Function -----------------------

    cout << eFirstPlate_eAliPID << endl;
    cout << eLastPlate_eAliPID << endl;
    cout << eMiddlePlate_eAliPID << endl;
    cout << eNumberPlate_eAliPID << endl;

//   return;
    // Casting to the parent object is done to have general properties behaviour.
    // But the Execute() function still calls the function of the inherited class:
    // Example: EdbShowAlg_CA -> Execute_CA

    for (int k=0; k<eShowAlgArrayN; k++) {

        ResetRecoShowerArray();

        EdbShowAlg* RecoAlg = (EdbShowAlg*) eShowAlgArray->At(k);

        eActualAlg=RecoAlg;
        ++eActualAlgParameterset[eActualAlg->GetAlgValue()];

        Int_t 	ActualAlgParametersetNr=eActualAlgParameterset[eActualAlg->GetAlgValue()];
        RecoAlg->SetActualAlgParameterset(ActualAlgParametersetNr);

        RecoAlg->SetEdbPVRec(eAli);
        RecoAlg->SetUseAliSub(eUseAliSub);  /// ealisub is deleted.. see adressing issues in transform!!!
        RecoAlg->SetEdbPVRecPIDNumbers(eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);

        RecoAlg->SetInBTArray( eInBTArray );
        RecoAlg->SetRecoShowerArray( eRecoShowerArray );

        if (gEDBDEBUGLEVEL>2)  eInBTArray->Print();
        if (gEDBDEBUGLEVEL>2) RecoAlg->Print();
        RecoAlg->PrintMore();

        cout<<"--------------------------------------  EXECUTE    RecoAlg->At(" << k << ")-----------------------------------"<<endl;
        RecoAlg->Execute();
        cout<<"--------------------------------------  EXECUTE    done. --------------------------"<<endl;

        // Important to update the local variable  eRecoShowerArrayN
        // since the instance of  EdbShowAlg  has its own variable filled.
        // If this is not done, we may have crashes because eRecoShowerArrayN is not filled!
        SetRecoShowerArrayN(RecoAlg->GetRecoShowerArrayN());

        // Parametriye Showers now, according to the set Parametrizations:
        BuildParametrizations();

        // Set OutNames like "treebranch_CA_1", "treebranch_OI_1,", ....
        SetOutNames();


        // Shortly Print Showers
        //  		EdbShowerP* sh_a=(EdbShowerP*)eRecoShowerArray->At(0); sh_a->PrintSegments();
        // 		sh_a->Finalize();
        // 		EdbShowerP* sh_b=(EdbShowerP*)eRecoShowerArray->At(eRecoShowerArrayN-1); sh_b->PrintSegments();

        // Convert the shower array (objects of EdbShowerP) into the "treebranch" style.
        RecoShowerArray_To_Treebranch();

        cout << endl << endl;
    }  // ShowerRecoAlg Loop.

    /// STILL TO DO
    //   EdbShowAlgE* ENERGYMODULE = new EdbShowAlgE(eRecoShowerArray);
    //   ENERGYMODULE->Create_MLP();
    //   ENERGYMODULE->Create_MLPTree();
    //   ENERGYMODULE->TEST();
    return;
}



//______________________________________________________________________________

void EdbShowRec::ReconstructTEST()
{
    // Was part of reconstruct() before ... not used at the moment
    /*

      ///-----------------------------------
      //   NEW   ALG:
      // CLEAN    SHowerArray before !!!
      // ?????????????

      cout <<"  NEW   ALG:  CLEAN    SHowerArray before !!!" << endl;
      eRecoShowerArray->Clear();
      cout <<"  NEW   ALG:  CLEAN    SHowerArray before !!!" << endl;
      ///-----------------------------------

    //   delete RecoAlg;
      EdbShowAlg_SA*   NewRecoAlg = new EdbShowAlg_SA();

      //StandardAlg->Print();

      cout << "-----------PRINT done" << endl;
      //eShowAlgArray->Print();
      cout << "eShowAlgArray->GetEntries() " <<eShowAlgArray->GetEntries() << endl;
      cout << "-----------PRINT done2" << endl;
      eShowAlgArray->Add(NewRecoAlg);
      cout << "eShowAlgArray->GetEntries() " <<eShowAlgArray->GetEntries() << endl;

      SetShowAlgArrayN(eShowAlgArray->GetEntries());

      eActualAlg=NewRecoAlg;
      eActualAlgParameterset[4]=1;

      //  eAli
      NewRecoAlg->SetEdbPVRec(eAli);
      //  eAli  numbers
      NewRecoAlg->SetEdbPVRecPIDNumbers( eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);
      //  InBTArray, now correctly filled.
      NewRecoAlg->SetInBTArray( eInBTArray );
      //  RecoShowerArray, to be filled by the algorithm:
      NewRecoAlg->SetRecoShowerArray( eRecoShowerArray);

      NewRecoAlg->Print();

      //
      NewRecoAlg->Execute();
      //
      //-------------------------------------------------------------------


      // Sets the names correctly when having more than one alg or parameterset.
      SetOutNames();

      RecoShowerArray_To_Treebranch();
      return;



      Log(2,"EdbShowRec::Reconstruct()","Default reconstruction function...done.");
      return;
     * */
    return;
}


//______________________________________________________________________________

void EdbShowRec::ReconstructTEST_CA()
{
    Log(2,"EdbShowRec::ReconstructTEST_CA()","Default reconstruction function CA Algorithm");

    if (!eInBTArray)  Create_eInBTArray();
    if (!eInBTTree)   Create_eInBTTree();
    Fill_eInBTArray(eUseNr);

    cout << "eShowAlgArrayN =  " << eShowAlgArrayN << endl;
    cout << "eShowAlgArray =  " << eShowAlgArray << endl;

    if (!eShowAlgArray)   {
        Create_eShowAlgArray();
        Fill_eShowAlgArray();
    }

    // --------------------- Main Print Function -----------------------
    Print();
    // --------------------- Main Print Function -----------------------

    // Casting to the parent object is done to have general properties behaviour.
    // But the Execute() function still calls the function of the inherited class:
    // Example: EdbShowAlg_CA -> Execute_CA

    for (int k=0; k<eShowAlgArrayN; k++) {
        EdbShowAlg* RecoAlg = (EdbShowAlg*) eShowAlgArray->At(k);

        eActualAlg=RecoAlg;
        ++eActualAlgParameterset[eActualAlg->GetAlgValue()];

        RecoAlg->SetEdbPVRec(eAli);
        RecoAlg->SetEdbPVRecPIDNumbers(eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);
        RecoAlg->SetInBTArray( eInBTArray );
        RecoAlg->SetRecoShowerArray( eRecoShowerArray );

        if (gEDBDEBUGLEVEL>2) RecoAlg->Print();

        cout<<"--------------------------------------  EXECUTE -----------------------------------"<<endl;
        RecoAlg->Execute();
        cout<<"--------------------------------------  EXECUTE    done. --------------------------"<<endl;


        // Important to update the local variable  eRecoShowerArrayN
        // since the instance of  EdbShowAlg  has its own variable filled.
        // If this is not done, we may have crashes because eRecoShowerArrayN is not filled!
        SetRecoShowerArrayN(RecoAlg->GetRecoShowerArrayN());

        // Parametriye Showers now, according to the set Parametrizations:
        BuildParametrizations();

        SetOutNames();
        RecoShowerArray_To_Treebranch();

    }  // ShowerRecoAlg Loop.

    Log(2,"EdbShowRec::ReconstructTEST_CA()","Default reconstruction function CA Algorithm...done.");
    return;
}



//______________________________________________________________________________



void EdbShowRec::ReconstructTEST_OI()
{
    Log(2,"EdbShowRec::ReconstructTEST_OI()","Default reconstruction function OI Algorithm");

    if (!eInBTArray)  Create_eInBTArray();
    if (!eInBTTree)   Create_eInBTTree();

    if (eInBTArrayN==0) Fill_eInBTArray(eUseNr);

    // test
    Fill_eInBTArray_ByBaseTracksOf_RootFile();

    if (!eShowAlgArray)   {
        Create_eShowAlgArray();
        Fill_eShowAlgArray();
    }
    // --------------------- Main Print Function -----------------------
    Print();
    // --------------------- Main Print Function -----------------------

    EdbShowAlg_OI*   StandardAlg = new EdbShowAlg_OI();
    eShowAlgArray->Add(StandardAlg);
    SetShowAlgArrayN(eShowAlgArray->GetEntries());

    eActualAlg=StandardAlg;
    ++eActualAlgParameterset[eActualAlg->GetAlgValue()];

    eActualAlg->SetEdbPVRec(eAli);
    eActualAlg->SetEdbPVRecPIDNumbers(eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);
    eActualAlg->SetInBTArray( eInBTArray );
    eActualAlg->SetRecoShowerArray( eRecoShowerArray );

    eActualAlg=StandardAlg;
    eActualAlgParameterset[5]=0;

    eActualAlg->Print();
    eActualAlg->PrintMore();

    cout<<"--------------------------------------  EXECUTE -----------------------------------"<<endl;
    eActualAlg->Execute();
    cout<<"--------------------------------------  EXECUTE    done. --------------------------"<<endl;


    // Important to update the local variable  eRecoShowerArrayN
    // since the instance of  EdbShowAlg  has its own variable filled.
    // If this is not done, we may have crashes because eRecoShowerArrayN is not filled!
    SetRecoShowerArrayN(eActualAlg->GetRecoShowerArrayN());

    // Parametriye Showers now, according to the set Parametrizations:
    BuildParametrizations();

    SetOutNames();
    RecoShowerArray_To_Treebranch();

    Log(2,"EdbShowRec::ReconstructTEST_OI()","Default reconstruction function OI Algorithm...done.");
    return;
}




//______________________________________________________________________________



void EdbShowRec::ReconstructTEST_NN()
{

    Log(2,"EdbShowRec::ReconstructTEST_NN()","Default reconstruction function NN Algorithm");

    if (!eInBTArray)  Create_eInBTArray();
    if (!eInBTTree)   Create_eInBTTree();
    Fill_eInBTArray(eUseNr);

    if (!eShowAlgArray)   {
        Create_eShowAlgArray();
        Fill_eShowAlgArray();
    }
    // --------------------- Main Print Function -----------------------
    Print();
    // --------------------- Main Print Function -----------------------

    EdbShowAlg_NN*   StandardAlg = new EdbShowAlg_NN();
    eShowAlgArray->Add(StandardAlg);
    SetShowAlgArrayN(eShowAlgArray->GetEntries());

    eActualAlg=StandardAlg;
    ++eActualAlgParameterset[eActualAlg->GetAlgValue()];

    eActualAlg->SetEdbPVRec(eAli);
    eActualAlg->SetEdbPVRecPIDNumbers(eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);
    eActualAlg->SetInBTArray( eInBTArray );
    eActualAlg->SetRecoShowerArray( eRecoShowerArray );

    eActualAlg=StandardAlg;
    eActualAlgParameterset[3]=0;

    eActualAlg->Print();

    cout<<"--------------------------------------  EXECUTE -----------------------------------"<<endl;
    eActualAlg->Execute();
    cout<<"--------------------------------------  EXECUTE    done. --------------------------"<<endl;

    Log(2,"EdbShowRec::ReconstructTEST_NN()","Default reconstruction function NN Algorithm...done.");
    return;
}




//______________________________________________________________________________



void EdbShowRec::ReconstructTEST_N3()
{

    Log(2,"EdbShowRec::ReconstructTEST_N3()","Default reconstruction function N3 Algorithm");

    if (!eInBTArray)  Create_eInBTArray();
    if (!eInBTTree)   Create_eInBTTree();
    Fill_eInBTArray(eUseNr);

    if (!eShowAlgArray)   {
        Create_eShowAlgArray();
        Fill_eShowAlgArray();
    }
    // --------------------- Main Print Function -----------------------
    Print();
    // --------------------- Main Print Function -----------------------

    Bool_t TrainANN=kTRUE;

    EdbShowAlg_N3*   StandardAlg = new EdbShowAlg_N3(TrainANN); // no empty constructor in this class...
    eShowAlgArray->Add(StandardAlg);
    SetShowAlgArrayN(eShowAlgArray->GetEntries());

    eActualAlg=StandardAlg;
    ++eActualAlgParameterset[eActualAlg->GetAlgValue()];

    eActualAlg->SetEdbPVRec(eAli);
    eActualAlg->SetEdbPVRecPIDNumbers(eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID);
    eActualAlg->SetInBTArray( eInBTArray );
    eActualAlg->SetRecoShowerArray( eRecoShowerArray );

    eActualAlg=StandardAlg;
    eActualAlg->Print();

    cout<<"--------------------------------------  EXECUTE -----------------------------------"<<endl;
    eActualAlg->Execute();
    cout<<"--------------------------------------  EXECUTE    done. --------------------------"<<endl;

    Log(2,"EdbShowRec::ReconstructTEST_N3()","Default reconstruction function N3 Algorithm...done.");
    return;
}







//______________________________________________________________________________

void EdbShowRec::Reconstruct()
{
    Log(2,"EdbShowRec::Reconstruct()","Default reconstruction using only one shower algorithm (default: OI) with default values.");

    // Check on different things to have:
    Int_t isThere=0;
    if (eAli)  isThere = 1;
    Log(2,"EdbShowRec::Reconstruct()","Check if  eAli           is there: %d", isThere);
    isThere=0;
    if (eInBTArray)  isThere = 1;
    Log(2,"EdbShowRec::Reconstruct()","Check if  eInBTArray     is there: %d", isThere);
    isThere=0;
    if (eShowAlgArray)  isThere = 1;
    Log(2,"EdbShowRec::Reconstruct()","Check if  eShowAlgArray  is there: %d", isThere);

    // CHECK IF WE DO HAVE TO DO IT AGAIN, WHEN IT WAS ALREADY CHECKED IN THE
    // CONSTRUCTOR OF  EdbShowRec ...
    // To set eAli plate numbers right:
    if (!IsPlateValuesLabel) SetPlateNumberValues();


    //-------------------------------------------------------------------
    // Part Filling the Initiator BaseTracks:
    if (!eInBTArray) Create_eInBTArray();
    //  If  __NO__  eInBTArray:  create manually: three options:
    //  0)  (eUseNr==0) getting the BaseTracks out of eAli. If they are not stored, goto next
    //  1)  (eUseNr==1) getting the BaseTracks out of tracks out of eFilename_LinkedTracks if they are stored
    //  2)  (eUseNr==2) use only (but all) Basetracks within [eFirstPlate,eMiddlePlate]
    //  3)  (eUseNr==3) from a (manually) written root file containing a TObjArray of EdbSegP segments (InBT)
    // Default is 0)

    // If it is not filled, then fill it now...
    if (eInBTArrayN==0) Fill_eInBTArray(eUseNr);

    // Create the Initiator Basetrack Tree... Whats the difference to Create_eInBTArray?
    // Maybe Cutstrings that can be written in the default.par file ....
    if (!eInBTTree) Create_eInBTTree();

    // 	return;

    //-------------------------------------------------------------------
    // Part Filling the Shower RecoAlg Array: first create the array:
    if (!eShowAlgArray) Create_eShowAlgArray();

    // If no eShowAlgArray:  create manually one using the
    // ConeAdvanced Algorithm as standard algorithm.
    if (eShowAlgArrayN==0) Fill_eShowAlgArray();

    Log(2,"EdbShowRec::Reconstruct()","eShowAlgArray  =%d",eShowAlgArray);
    Log(2,"EdbShowRec::Reconstruct()","eShowAlgArrayN =%d",eShowAlgArrayN);


    // Get Reconstruction Algorithm Object from the TObjArray storage:
    Log(2,"EdbShowRec::Reconstruct()","Get ShowAlg from TObjArray storage");

    // CAN IT ALSO BE DONE WITH A CAST TO THE PARENT CLASS ???
    // Seems so:
    // EdbShowAlg*  RecoAlg = (EdbShowAlg*) eShowAlgArray->At(0);
    // But I rather check it correctly, just to be safe....
    EdbShowAlg_OI*  RecoAlg = (EdbShowAlg_OI*) eShowAlgArray->At(0);
    eActualAlg=RecoAlg;


    // Hand over the important objects to the RecoAlg itsself.
    //  eAli  (the volume)
    RecoAlg->SetEdbPVRec(eAli);
    //  eAli  numbers
    RecoAlg->SetEdbPVRecPIDNumbers( eFirstPlate_eAliPID,  eLastPlate_eAliPID,  eMiddlePlate_eAliPID,  eNumberPlate_eAliPID );
    //  InBTArray, now correctly filled.
    RecoAlg->SetInBTArray(eInBTArray);
    //  RecoShowerArray, to be filled by the algorithm:
    RecoAlg->SetRecoShowerArray(eRecoShowerArray);
    //-------------------------------------------------------------------

    // Print relevant information for the algorithm:
    RecoAlg->Print();


    //-------------------------------------------------------------------
    // Main Reconstruction Part for the algorithm:
    //
    //
    RecoAlg->Execute();
    //
    //
    //-------------------------------------------------------------------


    // Get RecoShowerArray from Reco Alg back!
    // This has to be done, because EdbShowAlg and EdbShowRec class have
    // each a RecoShowerArray on their own. (It is by construction so...).
    // The storage for the arrays is therefore the same.
    // (Calls automatically SetRecoShowerArrayN)
    SetRecoShowerArray(RecoAlg->GetRecoShowerArray());

    Log(2,"EdbShowRec::Reconstruct()", "RecoAlg->Execute() finished. Reconstructed %d showers.", eRecoShowerArrayN);
    if (gEDBDEBUGLEVEL>2) PrintRecoShowerArray();


    // Parametrize Showers now, according to the set parametrizations:
    Log(2,"EdbShowRec::Reconstruct()", "Parametrize Showers now, according to the set parametrizations");
    SetDoParaType(1);
    BuildParametrizations();


    // Set the names correctly when having more than one alg or parameterset.
    Log(2,"EdbShowRec::Reconstruct()", "Set Output names correctly");
    SetOutNames();


    // Convert Array of EdbShowerP objects into old treebranch file format
    // (backward compability)
    Log(2,"EdbShowRec::Reconstruct()", "Convert Array of EdbShowerP objects into old treebranch file format");
    RecoShowerArray_To_Treebranch();


    // Write Shower in the new EdbShowerP format file
    Log(2,"EdbShowRec::Reconstruct()", "Write Shower in the new EdbShowerP format file");
    Write_RecoShowerArray(eRecoShowerArray);



    Log(2,"EdbShowRec::Reconstruct()","Default reconstruction function...done.");
    return;
}


//______________________________________________________________________________











//______________________________________________________________________________


void  EdbShowRec::Create_eInBTArray()
{
    Log(2,"EdbShowRec::Create_eInBTArray()","Create the Initiator Basetrack array");

    if (eInBTArray) {
        cout << "EdbShowRec::Create_eInBTArray   eInBTArray already there. Just Clear it." << endl;
        eInBTArray->Clear();
    }
    else {
        eInBTArray = new TObjArray(eInBTArrayMaxSize);
        cout << "EdbShowRec::Create_eInBTArray   New  eInBTArray  created." << endl;
    }

    eInBTArrayLoaded=kTRUE;

    Log(2,"EdbShowRec::Create_eInBTArray()","Create the Initiator Basetrack array...done");
    return;
}

//______________________________________________________________________________


void EdbShowRec::Check_eInBTArray()
{
    Log(2,"EdbShowRec::Check_eInBTArray()","Check the Initiator Basetrack array");
    Log(2,"EdbShowRec::Check_eInBTArray()","Check if and wheter to set the Array if InBTs");
    Log(2,"EdbShowRec::Check_eInBTArray()","The order is in the way:");
    Log(2,"EdbShowRec::Check_eInBTArray()","    a)  if eAli has eTracks, take eTracks.");
    Log(2,"EdbShowRec::Check_eInBTArray()","    b)  if a  linked_tracks.root file is existing, take tracks from there-");
    Log(2,"EdbShowRec::Check_eInBTArray()","    c)  take all BT from eAli from [firstplate..middleplate]");
    Log(2,"EdbShowRec::Check_eInBTArray()","    d)  from a (manually) written root file containing a TObjArray of EdbSegP segments (InBT)");
    Log(2,"EdbShowRec::Check_eInBTArray()","    e)  take N randomly selected BTs from eAli from [firstplate..middleplate]");

    // Check if and wheter to set the Array if InBTs.
    // The order is in the way (if the corresponding stuff is there:
    // (of course the settings in default.par will still be present):
    // a)  if eAli has eTracks, take eTracks.
    // b)  if a  linked_tracks.root file is existing, take tracks from there-
    // c)  take all BT from eAli from [firstplate..middleplate]
    // d)  from a (manually) written root file containing a TObjArray of EdbSegP segments (InBT)
    // e)  e)  take N randomly selected BTs from eAli from [firstplate..middleplate]
    if (!eAli) {
        Log(2,"EdbShowRec::Check_eInBTArray","No eAli created yet. For now: return;");
        return;
    }

    if (!eInBTArray) {
        Log(2,"EdbShowRec::Check_eInBTArray","No eInBTArray created yet. Create it.");
        Create_eInBTArray();
        Log(2,"EdbShowRec::Check_eInBTArray","No eInBTArray created yet. Create it....done.");
    }

    if (!eInBTTree) {
        Log(2,"EdbShowRec::Check_eInBTArray","No eInBTTree created yet. Create it.");
        Create_eInBTTree();
        Log(2,"EdbShowRec::Check_eInBTArray","No eInBTTree created yet. Create it...done");
    }

    // -------------------------------------------------
    // TO BE PRECISE, THE FOLLOWING CODE IS __NOT__ PART OF A CHECK FUNTION,
    // SINCE IT DOES ALSO FILLING AT ONCE.
    // SO FOR FURTHER PURPOSES, IT SHOULD BE TAKEN OUT.... (???)
    // AN BE PLACED IN A SEPERATE FUNCTION ....
    // -------------------------------------------------

    Check_eInBTArray_ByRecoLinkTracks_eAli();
    Check_Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();
    Check_Fill_eInBTArray_ByBaseTracksOf_eAli();
    Check_Fill_eInBTArray_ByBaseTracksOf_RootFile();

    /*

    // Case a:
    cout << "EdbShowRec::Check_eInBTArray Case a:  if eAli has eTracks, take eTracks." << endl;
    Fill_eInBTArray_ByRecoLinkTracks_eAli();
    if (eInBTArrayN!=0) return;
    cout << "EdbShowRec::Check_eInBTArray Case a: Fill_eInBTArray_ByRecoLinkTracks_eAli() was NOT successful. Go on with method b.  " << endl;

    // Case b:
    cout << "EdbShowRec::Check_eInBTArray Case b:  " << endl;
    Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();
    if (eInBTArrayN!=0) return;
    cout << "EdbShowRec::Check_eInBTArray Case b: Fill_eInBTArray_ByRecoLinkTracks_eAli() was NOT successful. Go on with method c.  " << endl;

    // Case c:
    cout << "EdbShowRec::Check_eInBTArray Case c:  " << endl;
    Fill_eInBTArray_ByBaseTracksOf_eAli();
    if (eInBTArrayN!=0) return;
    cout << "EdbShowRec::Check_eInBTArray Case c: Fill_eInBTArray_ByRecoLinkTracks_eAli() was NOT successful. Go on with method c." << endl;

    // Case d:
    cout << "EdbShowRec::Check_eInBTArray Case d:  " << endl;
    Fill_eInBTArray_ByBaseTracksOf_RootFile();
    if (eInBTArrayN!=0) return;
    cout << "EdbShowRec::Check_eInBTArray Case d: Fill_eInBTArray_ByBaseTracksOf_RootFile() was NOT successful. Stop here." << endl;

    */

    Log(2,"EdbShowRec::Check_eInBTArray()","Check the Initiator Basetrack array...done.");
    return;
}

//______________________________________________________________________________

Bool_t EdbShowRec::Check_eInBTArray_ByRecoLinkTracks_eAli()
{
    // Check if eAli has already fitted tracks.
    // Make tracks using standard track fitting/propagation routines.
    Log(2,"EdbShowRec::Check_eInBTArray_ByRecoLinkTracks_eAli()","Do tracks in the eAli object exist...");
    if (eAli->eTracks==NULL) {
        Log(2,"EdbShowRec::Check_eInBTArray_ByRecoLinkTracks_eAli()","Do tracks in the eAli object exist...No.");
        return kFALSE;
    }
    Log(2,"EdbShowRec::Check_eInBTArray_ByRecoLinkTracks_eAli()","Do tracks in the eAli object exist...Yes.");
    Log(2,"EdbShowRec::Check_eInBTArray_ByRecoLinkTracks_eAli()","Do tracks in the eAli object exist...done.");
    return kTRUE;
}

//______________________________________________________________________________
//             TO BE IMPLEMENTED HERE...
//     Check_Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();
//     Check_Fill_eInBTArray_ByBaseTracksOf_eAli();
//     Check_Fill_eInBTArray_ByBaseTracksOf_RootFile();
//______________________________________________________________________________

Bool_t EdbShowRec::Check_Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks() {
    cout << "TO BE IMPLEMENTED HERE..." << endl;
    return kFALSE;
}
Bool_t EdbShowRec::Check_Fill_eInBTArray_ByBaseTracksOf_eAli() {
    cout << "TO BE IMPLEMENTED HERE..." << endl;
    return kFALSE;
}
Bool_t EdbShowRec::Check_Fill_eInBTArray_ByBaseTracksOf_RootFile() {
    cout << "TO BE IMPLEMENTED HERE..." << endl;
    return kFALSE;
}


//______________________________________________________________________________

void EdbShowRec::Create_eInBTTree()
{
    Log(2,"EdbShowRec::Create_eInBTArray()","Create the Initiator Basetrack tree...done");

    if (eInBTTree) {
        delete eInBTTree;
        eInBTTree=0;
        Log(2,"EdbShowRec::Create_eInBTArray()","Deletion of the odl Initiator Basetrack tree done.");
    }
    EdbSegP* seg=0;

    // For now we have to create a dummy root file before creating the TTree,
    // otherwise the "error" message:
// //  Error in <TTree::Fill>: Failed filling branch:eInBTTree.s, nbytes=-1, entry=69960
// //  This error is symptomatic of a Tree created as a memory-resident Tree
// //  Instead of doing:
// //     TTree *T = new TTree(...)
// //     TFile *f = new TFile(...)
// //  you should do:
// //     TFile *f = new TFile(...)
// //     TTree *T = new TTree(...)
    //
    // will apear many times

    TFile* dummyFile = new TFile("dummyFile.root","RECREATE");
    if (!eInBTTree) {
        eInBTTree = new TTree("eInBTTree","eInBTTree");
        cout << "EdbShowRec::Create_eInBTTree()   Constructor done"<<endl;
        eInBTTree->Branch("s","EdbSegP",&seg,32000,99);
        cout << "EdbShowRec::Create_eInBTTree()   Branch done"<<endl;
    }

    Log(2,"EdbShowRec::Create_eInBTArray()","Create the Initiator Basetrack tree...done");
    return;
}

//______________________________________________________________________________

void  EdbShowRec::Fill_eInBTArray(Int_t FillType)
{
    Log(2,"EdbShowRec::Fill_eInBTArray()","Fill the Initiator Basetrack Array. Using FillType= %d",FillType);

    switch (FillType)
    {
    case -1 :
        // should never happen:
        Log(2,"EdbShowRec::Fill_eInBTArray()","Nothing done by default!");
        break;
    case 0 :
        Fill_eInBTArray_ByRecoLinkTracks_eAli();
        break;
    case 1 :
        Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();
        break;
    case 2 :
        Fill_eInBTArray_ByBaseTracksOf_eAli();
        break;
    case 3 :
        Fill_eInBTArray_ByBaseTracksOf_RootFile();
        break;
    default :
        Fill_eInBTArray_ByRecoLinkTracks_eAli();
        break;
    }

    cout <<

         Log(2,"EdbShowRec::Fill_eInBTArray()","Fill the Initiator Basetrack Array...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()
{
    // Check if  eAli  has already fitted tracks.
    // Make tracks using standard track fitting/propagation routines.
    Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Fill Initiator Basetrack Array from the stored Tracks.");

    // Object with the stored tracks...
    TObjArray* trackArray =  eAli->eTracks;

    // this should not happen, the check for Filled Tracks should be done in the routine
    // one level up. This is
    if (trackArray==NULL) {
        Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Attention! No Entries in the TrackArray!");
        Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Dont Fill InBTracks! Return!");
        return;
    }

    Int_t AliTracksN = trackArray->GetEntries();
    Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Initiator Basetrack Array: N=%d stored Tracks.",AliTracksN);

    /*
    // To have the "cut" features available, we still have to pass the source array via the eInBTTree
    // into the InBTArray. Not done yet, will maybe in time ...
    cout << "EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli() Transferrring them into the eInBTTree now ..." << endl;
    cout << "EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli() Transferrring them into the eInBTTree now NOT IMPLEMENTED YET...)" << endl;
    */
//     OR DONT PASS THEM OVER AND FILL THE ARRAY DIRECTLY ???
//     SEEMS MUCH MORE EASIER AT THE MOMENT !!!

    // Attention: eTracks is an array of Tracks, but the InBT array has to be filled with segments, i.e.
    // the first segment (EdbSegP) of a track (EdbTrackP). Therefore some conversion is necessary:
    EdbTrackP* track;
    TObjArray* segmentArray = new TObjArray();
    for (Int_t i=0; i<AliTracksN; ++i) {
        track= (EdbTrackP*) trackArray->At(i);
        EdbSegP* segment = track->GetSegmentFirst();
        segmentArray->Add(segment);
    }
    SetInBTArray( segmentArray );

    Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Initiator Basetrack Array: GetInBTArrayN()=%d filled tracks.",GetInBTArrayN());
    Log(2,"EdbShowRec::Fill_eInBTArray_ByRecoLinkTracks_eAli()","Fill Initiator Basetrack Array from the stored Tracks...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()
{
    Log(2,"EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()","EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks...");

    if (!eInBTTree) {
        cout << "EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   eInBTTree not yet created...Return." << endl;
        return;
    }

    /// -----------------
    // Check if file  eFilename_LinkedTracks  exits and if it has tracks inside....
    /// -----------------  tODO
    cout << "EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   TODO::CHECK IF " << eFilename_LinkedTracks   <<" exists and if it has tracks inside!!!!" << endl;
    /// -----------------

    EdbSegP* segment=0;
    TBranch* b_s=0;                        // !!!
    b_s  = eInBTTree->GetBranch("s");     // !!!
    b_s->SetAddress(  &segment);

    EdbSegP*    s2=0;
    EdbTrackP*  t=0;

    TFile* fil = new TFile(eFilename_LinkedTracks);
    TTree* tr= (TTree*)fil->Get("tracks");
    TClonesArray *seg= new TClonesArray("EdbSegP",60);
    int nentr = int(tr->GetEntries());

    //   check if tracks has entries: if so then ok, otherwise return directly:
    if (nentr>0) {
        cout << "EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   tracks file has " <<  nentr << "  entries total" << endl;
    }
    else {
        cout << "EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   tracks file has  NO  entries...Return." << endl;
        return;
    }

    int nseg,n0,npl;
    tr->SetBranchAddress("t.", &t  );
    tr->SetBranchAddress("s", &seg  );
    tr->SetBranchAddress("nseg", &nseg  );
    tr->SetBranchAddress("n0", &n0  );
    tr->SetBranchAddress("npl", &npl  );

    for (Int_t i=0; i<nentr; ++i ) {
        tr->GetEntry(i);
        // Take only Basetracks from tracks which pass 3 or more plates:
        // WHY 3 plates? This would exclude all initiator tracks with two consecutive segments anyway...???
        /// TO BE CHECKED ..!!!!
        if (npl<3) continue;
        // Take first BT of the tracks:
        s2=(EdbSegP*) seg->At(0);

        // Check via PID if this segment is within the desired range:
        if (s2->PID()>TMath::Max(eFirstPlate_eAliPID,eMiddlePlate_eAliPID)||s2->PID()<TMath::Min(eFirstPlate_eAliPID,eMiddlePlate_eAliPID)) continue;

        segment=(EdbSegP*)s2->Clone();

        // Incremently ordered cuts..
        // To be implemented via  TCut  method just like in default.par.....
        // This is done in further down.
        // Now we fill completely the  eInBTTree  first. On this tree we can implement the
        // TCuts then.
        eInBTTree->Fill();
    }
    delete tr;
    delete fil;

    // Update Entry numbers.
    SetInBTArrayN(eInBTTree->GetEntries());

    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   Before TCut: InBT N=%d", eInBTArrayN );

    // Now do the cut function from all read BTs to those which pass the TCut string given in default.par file.
    Cut_eInBTTree_To_InBTArray();

    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks   Printing InBTs" << endl;
        EdbSegP* testseg=0;
        for (Int_t i=0; i<eInBTArrayN; ++i ) {
            testseg=(EdbSegP*)eInBTArray->At(i);
            testseg->PrintNice();
        }
    }

    Log(2,"EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()","EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks.   After TCut: InBT N=%d", eInBTArrayN );
    Log(2,"EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()","EdbShowRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks...done.");
    return;
}


//______________________________________________________________________________


void EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()
{
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.");

    int count=0;
    Bool_t StillToLoop=kTRUE;
    Int_t nr=eMiddlePlate_eAliPID;

    if (!eInBTTree) {
        cout << "EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()   eInBTTree not yet created...Return." << endl;
        return;
    }

    EdbSegP* segment=0;
    TBranch*b_s=0;                        // !!!
    b_s  = eInBTTree->GetBranch("s");     // !!!
    b_s->SetAddress(  &segment);

    while (StillToLoop) {

        if (gEDBDEBUGLEVEL>3) {
            cout <<"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli() nr=  " << nr << endl;
            cout <<"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli() eAli->GetPattern(nr)->Z()=  " << eAli->GetPattern(nr)->Z() << endl;
            cout <<"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli() eAli->GetPattern(nr)->GetN()=  " << eAli->GetPattern(nr)->GetN() << endl;
        }

        for (Int_t n=0; n<eAli->GetPattern(nr)->GetN(); ++n) {

            if (count > eInBTArrayMaxSize-1) continue; // maximumSize of TObjArray: eInBTArray
            segment=eAli->GetPattern(nr)->GetSegment(n);

            // Incremently ordered cuts..
            // To be implemented via  TCut  method just like in default.par.....
            // This is done in further down.
            // Now we fill completely the  eInBTTree  first. On this tree we can implement the
            // TCuts then.
            eInBTTree->Fill();
            ++count;
        }

        // Reduce/Increas nr:
        nr=nr+eAliStreamTypeStep;
        //Check if still to loop:
        if (eAliStreamType=="DOWNSTREAM" && nr<eFirstPlate_eAliPID) StillToLoop=kFALSE;
        if (eAliStreamType=="UPSTREAM" && nr>eFirstPlate_eAliPID) StillToLoop=kFALSE;
    }

    // Update Entry numbers.
    SetInBTArrayN(eInBTTree->GetEntries());

    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   Before TCut: InBT N=%d", eInBTArrayN );

    // Now do the cut function from all read BTs to those which pass the TCut string given in default.par file.
    Cut_eInBTTree_To_InBTArray();


    if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()   Printing InBTs" << endl;
    EdbSegP* testseg=0;
    for (Int_t i=0; i<eInBTArrayN; ++i ) {
        if (gEDBDEBUGLEVEL>2) {
            testseg=(EdbSegP*)eInBTArray->At(i);
            testseg->PrintNice();
        }
    }

    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   After TCut: InBT N=%d", eInBTArrayN );
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile()
{
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile.");
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile()","Special Method to read manually selected Initiator BaseTracks.");
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile()","Read root file <<InBT.root>>");


    int count=0;
    Bool_t StillToLoop=kTRUE;
    Int_t nr=eMiddlePlate_eAliPID;

    if (!eInBTTree) {
        cout << "EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_RootFile()   eInBTTree not yet created...Return." << endl;
        return;
    }

    TFile* file = new TFile("InBT.root");
    TObjArray* InBTArray = (TObjArray* )file->Get("TObjArray");
    for (Int_t i=0; i<InBTArray->GetEntries(); ++i) {
        EdbSegP* s = (EdbSegP*)InBTArray->At(i);
        s->PrintNice();
    }

    // Update Entry numbers.
    SetInBTArray(InBTArray);
    SetInBTArrayN(InBTArray->GetEntries());

    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   Before TCut: InBT N=%d", eInBTArrayN );

    // Now do the cut function from all read BTs to those which pass the TCut string given in default.par file.
    // Cut_eInBTTree_To_InBTArray();




    if (gEDBDEBUGLEVEL>1) cout << "EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()   Printing InBTs" << endl;
    EdbSegP* testseg=0;
    for (Int_t i=0; i<eInBTArrayN; ++i ) {
        if (gEDBDEBUGLEVEL>2) {
            testseg=(EdbSegP*)eInBTArray->At(i);
            testseg->PrintNice();
        }
    }

    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   After TCut: InBT N=%d", eInBTArrayN );
    Log(2,"EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli()","EdbShowRec::Fill_eInBTArray_ByBaseTracksOf_eAli...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::Cut_eInBTTree_To_InBTArray()
{
    Log(2,"EdbShowRec::Cut_eInBTTree_To_InBTArray()","EdbShowRec::Cut_eInBTTree_To_InBTArray.");

    cout << "EdbShowRec::Cut_eInBTTree_To_InBTArray()   NEED TO EXPLAIN HERE...WHAT DOES THIS FUNCTION DO ???" << endl;

    EdbSegP* segment=0;
    TBranch*b_s=0;                        // !!!
    b_s  = eInBTTree->GetBranch("s");     // !!!
    b_s->SetAddress(  &segment);

    // These lines apply the cut in the  eInBTTree  and fill then the  eInBTArray:
    // This may be posible to put in an extra routine since all Fillxxx function will
    // have to have this included... todo...
    int nseg = 0;
    TCut *cut = eInBTCuts[0];
    TEventList *lst =0;
    if (cut)       eInBTTree->Draw(">>lst", *cut );
    else          eInBTTree->Draw(">>lst", "" );
    lst = (TEventList*)(gDirectory->GetList()->FindObject("lst"));
    //lst->Dump();
    int nlst =lst->GetN();

    int entr=0;
    for (Int_t i=0; i<nlst; ++i ) {
        entr = lst->GetEntry(i);

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%100)==0) cout << nlst <<" InBT without cuts in total. Already passed cut:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

        b_s->GetEntry(entr);        // !!!
        // segment->PrintNice();
        // segment has passed cuts, so add it now.
        // CLONE SEGMENT otherwise adding doesnt WORK!
        eInBTArray->Add((EdbSegP*)segment->Clone());
        nseg++;
    }

    // Update Entry numbers.
    SetInBTArrayN(nseg);

    if (gEDBDEBUGLEVEL>3) Print_InBTArray();

    Log(2,"EdbShowRec::Cut_eInBTTree_To_InBTArray()","EdbShowRec::Cut_eInBTTree_To_InBTArray.   InBT N=%d", eInBTArrayN );
    Log(2,"EdbShowRec::Cut_eInBTTree_To_InBTArray()","EdbShowRec::Cut_eInBTTree_To_InBTArray...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::SetPlateNumberValues()
{
    Log(2,"EdbShowRec::SetPlateNumberValues()","SetPlateNumberValues");

    // Possibly there are four (!!) combinations. But dont know yet. todo...

    // The most seen labelling up to now (in data) is this:
    // EdbPatternsVolume with 28 patterns
    // id=  0 pid=  0   x:y:z =         0.000        0.000   -35395.020  n=     1326
    // id=  1 pid=  1   x:y:z =         0.000        0.000   -34085.070  n=     1322
    // ...
    // id= 26 pid= 26   x:y:z =         0.000        0.000    -1342.368  n=     3588
    // id= 27 pid= 27   x:y:z =         0.000        0.000        0.000  n=     3788
    // This means: highest (absolut) Z value is gets PID = npat-1 , whereas lowest Z value is PID = 0,
    //
    // In other words:
    // eFirstPlate = 1  <=> eFirstPlate_eAliPID = 0
    // eLastPlate = npat  <=> eLastPlate_eAliPID = npat-1

    // So we try ko keep this scheme (also in simulation);

    cout << "EdbShowRec::SetPlateNumberValues()   eAliNpat  = " << eAliNpat << endl;

    EdbSegP* seg0;
    EdbSegP* segNPATm1;

    // Maybe this is not safe: when Pattern(0) or Pattern(eAliNpat-1) has no segments this pointer
    // will create a wrong assignment and crash. Better use PID of the Pattern itsself.
    // Changed here.
    Int_t PID0=eAli->GetPattern(0)->PID();
    Int_t PIDNPATM1=eAli->GetPattern(eAliNpat-1)->PID();

    Float_t Z_PatternID_0       = eAli->GetPattern(0)->Z();
    Float_t Z_PatternID_NPATm1  = eAli->GetPattern(eAliNpat-1)->Z();

    if (eAli->GetPattern(0)->N()>0)  {
        seg0=(EdbSegP*)eAli->GetPattern(0)->At(0);
        cout << "EdbShowRec::SetPlateNumberValues()   seg0->PID()= " << seg0->PID() << endl;
    }

    if (eAli->GetPattern(eAliNpat-1)->N()>0)  {
        segNPATm1=(EdbSegP*)eAli->GetPattern(eAliNpat-1)->At(0);
        cout << "EdbShowRec::SetPlateNumberValues()   segNPATm1->PID() = " << segNPATm1->PID() << endl;
    }

    cout << "EdbShowRec::SetPlateNumberValues()   Z_PatternID_NPATm1 = " << Z_PatternID_NPATm1 << endl;
    cout << "EdbShowRec::SetPlateNumberValues()   Z_PatternID_0 = " << Z_PatternID_0 << endl;

    cout << "EdbShowRec::SetPlateNumberValues()   PID0 = " << PID0 << endl;
    cout << "EdbShowRec::SetPlateNumberValues()   PIDNPATM1 = " << PIDNPATM1 << endl;


    // Usually if nothing else is given, then (and only then!)
    //  eFirstPlate is the first plate (from beam view== peam hits this plate first)
    //  eLastPlate is the last plate (from beam view== peam hits this plate first)
    //  eMiddlePlate is 3 plates before last plate (from beam view== peam hits this plate first)
    //  eNumberPlate is number of plates of  eAli  object.
    eFirstPlate=1;
    eLastPlate=eAliNpat;
    eMiddlePlate=eAliNpat-3;
    eNumberPlate=eAliNpat;


    if (Z_PatternID_NPATm1<Z_PatternID_0) {
        cout << "EdbShowRec::SetPlateNumberValues()   case:   FP is at lowest Z, and has PID(0).....UPSTREAM...."<<endl;

        eAliZMaxPID=PID0;
        eAliZMinPID=PIDNPATM1;
        eAliZMax=Z_PatternID_0;
        eAliZMin=Z_PatternID_NPATm1;

        eFirstPlate_eAliPID=PIDNPATM1;
        eLastPlate_eAliPID=PID0;
        eNumberPlate_eAliPID=eAliNpat;
        eMiddlePlate_eAliPID=PID0+3;

        eAliStreamType="UPSTREAM";
        eAliStreamTypeStep=+1;
    }

    if (Z_PatternID_NPATm1>Z_PatternID_0) {
        cout << "EdbShowRec::SetPlateNumberValues()   case:   FP is at highest Z, and has PID(0).....DOWNSTREAM...."<<endl;

        eAliZMaxPID=PIDNPATM1;
        eAliZMinPID=PID0;
        eAliZMax=Z_PatternID_NPATm1;
        eAliZMin=Z_PatternID_0;

        eFirstPlate_eAliPID=PIDNPATM1;
        eLastPlate_eAliPID=PID0;
        eNumberPlate_eAliPID=eAliNpat;
        eMiddlePlate_eAliPID=PIDNPATM1-3;

        eAliStreamType="DOWNSTREAM";
        eAliStreamTypeStep=-1;
    }

    if (Z_PatternID_NPATm1==Z_PatternID_0)  {
        cout << "EdbShowRec::SetPlateNumberValues()   Strange case:   Z_PatternID_NPATm1==Z_PatternID_0  should NEVER happen...????"<<endl;
        return;
    }

    Log(2,"EdbShowRec::SetPlateNumberValues()","eAliZMaxPID = %02d", eAliZMaxPID);
    Log(2,"EdbShowRec::SetPlateNumberValues()","eFirstPlate_eAliPID = %02d", eFirstPlate_eAliPID);
    Log(2,"EdbShowRec::SetPlateNumberValues()","eLastPlate_eAliPID = %02d", eLastPlate_eAliPID);
    Log(2,"EdbShowRec::SetPlateNumberValues()","eMiddlePlate_eAliPID = %02d", eMiddlePlate_eAliPID);
    Log(2,"EdbShowRec::SetPlateNumberValues()","eNumberPlate_eAliPID = %02d", eNumberPlate_eAliPID);
    Log(2,"EdbShowRec::SetPlateNumberValues()","eAliStreamTypeStep = %02d", eAliStreamTypeStep);

    IsPlateValuesLabel=kTRUE;

    Log(2,"EdbShowRec::SetPlateNumberValues()","SetPlateNumberValues...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::Create_eShowAlgArray()
{
    Log(2,"EdbShowRec::SetPlateNumberValues()","Create_eShowAlgArray");
    if (eShowAlgArray) {
        cout << "EdbShowRec::Create_eShowAlgArray()   eShowAlgArray already there. Just Clear it." << endl;
        eShowAlgArray->Clear();
    }
    else {
        eShowAlgArray = new TObjArray(eShowAlgArrayMaxSize);
        cout << "EdbShowRec::Create_eShowAlgArray()   New  eShowAlgArray  created." << endl;
    }

    eShowAlgArrayLoaded=kTRUE;
    Log(2,"EdbShowRec::SetPlateNumberValues()","Create_eShowAlgArray...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Fill_eShowAlgArray()
{
    Log(2,"EdbShowRec::Fill_eShowAlgArray()","Fill_eShowAlgArray");

    if (!eShowAlgArray) return;

    //EdbShowAlg_CA*   StandardAlg = new EdbShowAlg_CA();
    EdbShowAlg_OI*   StandardAlg = new EdbShowAlg_OI();

    eShowAlgArray->Add(StandardAlg);
    SetShowAlgArrayN(eShowAlgArray->GetEntries());

    StandardAlg->SetUseAliSub(eUseAliSub);

    eActualAlg=StandardAlg;
    eActualAlgParameterset[2]=0;

    Log(2,"EdbShowRec::Fill_eShowAlgArray()","Fill_eShowAlgArray...done.");
    return;
}



//______________________________________________________________________________

void EdbShowRec::Write_RecoShowerArray(TObjArray* RecoShowerArray)
{

    Log(2,"EdbShowRec::Write_RecoShowerArray()","Write the array of reconstructed showers to a TTree");
    if (!eWriteFileShower) {
        Log(2,"EdbShowRec::Write_RecoShowerArray()","WARNING: eWriteFileShower = %d. Will not write. Return.", eWriteFileShower );
        return;
    }

    TFile* file= new TFile(eFilename_Out_shower,"RECREATE");
    TTree* tree= new TTree("treeArrayEdbShowerP","treeArrayEdbShowerP");

    EdbShowerP* shower=0;
    tree->Branch("shower","EdbShowerP",&shower,128000);

    int nbyte;
    for (int itr=0; itr<RecoShowerArray->GetEntries(); itr++) {
        Log(3,"EdbShowRec::Write_RecoShowerArray()","Doing entry %d.", itr );
        shower=(EdbShowerP*)RecoShowerArray->At(itr);
        //shower->PrintNice();
        Log(3,"EdbShowRec::Write_RecoShowerArray()","Do tree->Fill.");
        nbyte=tree->Fill();
    }

    Log(2,"EdbShowRec::Write_RecoShowerArray()","tree->Fill loop done.");

    tree->Print();
    cout << " tree->GetEntries() " <<   tree->GetEntries()  << endl;
    tree->Write();
    cout << " tree->Write()   done." << endl;
    file->Close();
    cout << " file->Close()   done."    << endl;

    delete file;
    file=0;

    Log(2,"EdbShowRec::Write_RecoShowerArray()","Write the array of reconstructed showers to a TTree...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Reset()
{
    Log(2,"EdbShowRec::Reset()","Delete all internal objects and set them to NULL pointer.");

    cout << "EdbShowRec::Reset()   Delete eAli." << endl;
    if (eAli)                             {
        eAli->Clear();
        delete eAli;
        eAli=0;
    }
    cout << "EdbShowRec::Reset()   Delete eInBTArray." << endl;
    if (eInBTArray)                       {
        eInBTArray->Clear();
        delete eInBTArray;
        eInBTArray=0;
    }
    cout << "EdbShowRec::Reset()   Delete eShowAlgArray." << endl;
    if (eShowAlgArray)                       {
        eShowAlgArray->Clear();
        delete eShowAlgArray;
        eShowAlgArray=0;
    }
    Log(2,"EdbShowRec::Reset()","Delete all internal objects and set them to NULL pointer...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Initialize()
{
    Log(2,"EdbShowRec::Initialize()","Initialize ... NOTHING YET HERE");

    // Here is gonna set up all neccessary variables which are needed
    // to run the algorithm later on the  eAli  object.

    Log(2,"EdbShowRec::Initialize()","Initialize ... NOTHING YET HERE...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Execute()
{
    Log(2,"EdbShowRec::Execute()","Execute ... NOTHING YET HERE");
    // Here the algorithm(s) are doing the reconstruction
    // of showers on the  eAli  object.
    // Later after reconstruction other things like gamma-reconstruction
    // can be done also here.
    Log(2,"EdbShowRec::Execute()","Execute ... NOTHING YET HERE...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::Finalize()
{
    Log(2,"EdbShowRec::Finalize()","Execute ... NOTHING YET HERE");
    Log(2,"EdbShowRec::Finalize()","Execute ... NOTHING YET HERE...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Print()
{
    Log(2,"EdbShowRec::Print()","Print important stuff...");
    for (Int_t i=0; i<100; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print()" << endl;
    cout << "-";
    cout << endl;
    for (Int_t i=0; i<100; ++i) cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- GENERAL SETTINGS: ---" << endl;
    cout << "EdbShowRec::Print() --- eDoReconstruction = " <<       eDoReconstruction << endl;
    cout << "EdbShowRec::Print() --- eDoParametrization = " <<       eDoParametrization << endl;
    cout << "EdbShowRec::Print() --- eDoId = " <<       eDoId << endl;
    cout << "EdbShowRec::Print() --- eDoEnergy = " <<       eDoEnergy << endl;
    cout << "EdbShowRec::Print() --- eDoVtxAttach = " <<       eDoVtxAttach << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- SETTINGS: Input data:" << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File EdbDataProc Filename:" << eFilename_LnkDef << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File Tracks      Filename:" << eFilename_LinkedTracks << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File EdbPVRec    Filename:" << eFilename_EdbPVRecObject << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Input data type:" << eUseNr << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Input data type eUse_LT:" << eUse_LT << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Input data type eUse_PVREC:" << eUse_PVREC << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Input data type eUse_AliBT :" << eUse_AliBT << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Input data type eUse_AliLT:" << eUse_AliLT << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object NPatterns:" << eAliNpat << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object First PID:" << eFirstPlate_eAliPID << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object Middle PID:" << eMiddlePlate_eAliPID << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object Last PID:" << eLastPlate_eAliPID << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object FirstPlate:" << eFirstPlate << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object MiddlePlate:" << eMiddlePlate << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object LastPlate:" << eLastPlate << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object NumberPlate:" << eNumberPlate << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object Max Z:" << eAliZMax << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "EdbPVRec Object Min Z:" << eAliZMin << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "Initiator Basetracks: Cut:" << eInBTCuts[0] << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "Initiator Basetracks: N:" << eInBTArrayN << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- SETTINGS: Output data:" << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File containing EdbShowerP Array Write:" << eWriteFileShower << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File containing EdbShowerP Array:" << eFilename_Out_shower << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File containing treebranch tree Write :" << eWriteFileTreebranch << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "File containing treebranch tree:" << eFilename_Out_treebranch << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- SETTINGS: Reconstruction:" << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "Reconstruction Use gAliSub:" << eUseAliSub << endl;
    cout << "EdbShowRec::Print() --- " << setw(20) << "Reconstruction Algs: N:" << eShowAlgArrayN << endl;
    for (Int_t i=0; i<eShowAlgArrayN; ++i)  {
        cout << "EdbShowRec::Print() --- " << setw(20) << "Reconstruction Algs: ";
        EdbShowAlg* RecoAlg = (EdbShowAlg*) eShowAlgArray->At(i);
        RecoAlg->PrintParametersShort();
    }
    cout << "-";
    cout << endl;

    cout << "EdbShowRec::Print() --- SETTINGS: Id:" << endl;
    cout << "-";
    cout << endl;

    cout << "EdbShowRec::Print() --- SETTINGS: Parametrisation:" << endl;
    for (Int_t i=0; i<6; ++i)  {
        cout << "EdbShowRec::Print() --- " << setw(20) << "Parametrisation Do " <<  eParaNames[i] <<": "<< eParaTypes[i] << endl;
    }
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print() --- SETTINGS: Energy:" << endl;
    cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbShowRec::Print()....done." << endl;
    for (Int_t i=0; i<100; ++i) cout << "-";
    cout << endl;
    Log(2,"EdbShowRec::Print()","Print important stuff...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::Print_InBTArray()
{
    Log(2,"EdbShowRec::Print_InBTArray()","Print the information on the Initiator Basetrack array");
    if (!eInBTArray) return;
    Log(2,"EdbShowRec::Print_InBTArray()","N=%d",eInBTArrayN);
    EdbSegP* s=0;
    for (Int_t i=0; i<eInBTArrayN; ++i) {
        s=(EdbSegP*) eInBTArray->At(i);
        s->PrintNice();
    }
    Log(2,"EdbShowRec::Print_InBTArray()","Print the information on the Initiator Basetrack array...done.");
    return;
}



//______________________________________________________________________________


int EdbShowRec::ReadShowRecPar(const char *file="default.par")
{
    // read parameters from par-file
    // return: 0 if ok
    //        -1 if file access failed
    char buf[256];
    char key[256];
    char name[256];

    FILE *fp = fopen(file,"r");
    if (!fp) {
        Log(1,"EdbShowRec::ReadShowRecPar","ERROR open file: %s", file);
        return -1;
    }
    else Log(2,"EdbShowRec::ReadShowRecPar","Read shower parameters from file: %s", file );

    Int_t id;
    Int_t int_eUse;
    int   dospec;
    TString tmpString;
    char tmpchar[256];

    while (fgets(buf, sizeof(buf), fp)) {
        for (Int_t i = 0; i < (Int_t)strlen(buf); ++i)
            if (buf[i]=='#')  {
                buf[i]='\0';                       // cut out comments starting from #
                break;
            }

        if ( sscanf(buf,"%s",key)!=1 ) continue;

        if      ( !strcmp(key,"INCLUDE")   )
        {
            sscanf(buf+strlen(key),"%s",name);
            ReadShowRecPar(name);
        }


        /// Main class instance step settings:
        /// These settings are still to include in the Initialize(),Execute(),Finalize() Function Troika
        else if ( !strcmp(key,"SHOW_DO_RECONSTRUCTION")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_RECONSTRUCTION = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoReconstruction(dospec);
        }
        else if ( !strcmp(key,"SHOW_DO_PARAMETRISATION")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_PARAMETRISATION = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoParametrisation(dospec);
        }
        else if ( !strcmp(key,"SHOW_DO_ID")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_ID = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoId(dospec);
        }
        else if ( !strcmp(key,"SHOW_DO_ENERGY")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_ENERGY = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoEnergy(dospec);
        }
        else if ( !strcmp(key,"SHOW_DO_VERTEXATTACH")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_VERTEXATTACH = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoVtxAttach(dospec);
        }

        /// These settings are still to include in the Initialize(),Execute(),Finalize() Function Troika

        /// Main settings preparating the reconstruction object input data:
        else if ( !strcmp(key,"SHOW_USE_LT")   )
        {
            sscanf(buf+strlen(key),"%d",&int_eUse);
            ResetUse_InBTType();
            if (int_eUse==1) {
                eUse_LT=kTRUE;
                eUse_PVREC=kFALSE;
                eUse_AliBT=kFALSE;
                eUse_AliLT=kFALSE;
                eUse_ROOTInBT=kFALSE;
                eUseNr=0;
            }
            else {
                eUse_LT=kFALSE;
            }
            cout << "eUse_LT  now  " << eUse_LT << endl;
        }

        else if ( !strcmp(key,"SHOW_USE_ALIBT")   )
        {
            sscanf(buf+strlen(key),"%d",&int_eUse);
            ResetUse_InBTType();
            if (int_eUse==1) {
                eUse_LT=kFALSE;
                eUse_PVREC=kFALSE;
                eUse_AliBT=kTRUE;
                eUse_AliLT=kFALSE;
                eUse_ROOTInBT=kFALSE;
                eUseNr=2;
            }
            else {
                eUse_AliBT=kFALSE;
            }
            cout << "eUse_AliBT  now  " << eUse_AliBT << endl;
        }


        else if ( !strcmp(key,"SHOW_USE_ROOTInBT")   )
        {
            sscanf(buf+strlen(key),"%d",&int_eUse);
            ResetUse_InBTType();
            if (int_eUse==1) {
                eUse_LT=kFALSE;
                eUse_PVREC=kFALSE;
                eUse_AliBT=kFALSE;
                eUse_AliLT=kTRUE;
                eUse_ROOTInBT=kFALSE;
                eUseNr=3;
            }
            else {
                eUse_AliLT=kFALSE;
            }
            cout << "eUse_AliLT  now  " << eUse_AliLT << endl;
        }


        else if ( !strcmp(key,"SHOW_USE_PVREC")   )
        {
            sscanf(buf+strlen(key),"%d",&int_eUse);
            ResetUse_InBTType();
            if (int_eUse==1) {
                eUse_LT=kFALSE;
                eUse_PVREC=kTRUE;
                eUse_AliBT=kFALSE;
                eUse_AliLT=kFALSE;
                eUse_ROOTInBT=kFALSE;
                eUseNr=1;
            }
            else {
                eUse_PVREC=kFALSE;
            }
            cout << "eUse_PVREC  now  " << eUse_PVREC << endl;
        }

        else if ( !strcmp(key,"SHOW_INBTCUT")  )
        {
            char rcut[256];
            sscanf(buf+strlen(key),"%d %s",&id, rcut );
            cout << "SHOW_INBTCUT  =   " <<   rcut << endl;
            TCut cut(rcut);
            Add_INBTCut(id,cut);
        }


        /// Main settings preparating the reconstruction object output data:
        else if ( !strcmp(key,"SHOW_WRITE_TREEBRANCH")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            Bool_t dospec_bool=(Bool_t)dospec;
            cout << "EdbShowRec::ReadShowRecPar   SHOW_WRITE_TREEBRANCH = " << dospec_bool << endl;
            SetWriteFileTreebranch(dospec_bool);
        }
        else if ( !strcmp(key,"SHOW_WRITE_SHOWERS")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            Bool_t dospec_bool=(Bool_t)dospec;
            cout << "EdbShowRec::ReadShowRecPar   SHOW_WRITE_SHOWERS = " << dospec_bool << endl;
            SetWriteFileShower(dospec_bool);
        }

        else if ( !strcmp(key,"SHOW_FILE_TREEBRANCH")   )
        {
            sscanf(buf+strlen(key),"%s",tmpchar);
            tmpString=TString(tmpchar);
            SetTreebranchFileName(tmpString);
            cout << "SHOW_FILE_TREEBRANCH  = " << tmpString << endl;
        }

        else if ( !strcmp(key,"SHOW_FILE_SHOWERS")   )
        {
            sscanf(buf+strlen(key),"%s",tmpchar);
            tmpString=TString(tmpchar);
            SetShowFileName(tmpString);
            cout << "SHOW_FILE_SHOWERS  = " << tmpString << endl;
        }

        else if ( !strcmp(key,"SHOW_USE_SIMPLEFILENAME")  )
        {
            Int_t   filetype, dofiletype;
            sscanf(buf+strlen(key),"%d %d",&filetype, &dofiletype);
            cout << "EdbShowRec::ReadShowRecPar   filetype = " << filetype << endl;
            cout << "EdbShowRec::ReadShowRecPar   dofiletype = " << dofiletype << endl;
            SetSimpleFileName(filetype, dofiletype);
        }

        else if ( !strcmp(key,"SHOW_FIRSTPLATE")  )
        {
            int ProposedFirstPlate;
            sscanf(buf+strlen(key),"%d",&ProposedFirstPlate);
            cout << "EdbShowRec::ReadShowRecPar   ProposedFirstPlate = " << ProposedFirstPlate << endl;
            eProposedFirstPlate=ProposedFirstPlate;
        }
        else if ( !strcmp(key,"SHOW_LASTPLATE")  )
        {
            int ProposedLastPlate;
            sscanf(buf+strlen(key),"%d",&ProposedLastPlate);
            cout << "EdbShowRec::ReadShowRecPar   ProposedLastPlate = " << ProposedLastPlate << endl;
            eProposedLastPlate=ProposedLastPlate;
        }
        else if ( !strcmp(key,"SHOW_MIDDLEPLATE")  )
        {
            int ProposedMiddlePlate;
            sscanf(buf+strlen(key),"%d",&ProposedMiddlePlate);
            cout << "EdbShowRec::ReadShowRecPar   ProposedMiddlePlate = " << ProposedMiddlePlate << endl;
            eProposedMiddlePlate=ProposedMiddlePlate;
        }
        else if ( !strcmp(key,"SHOW_NUMBERPLATE")  )
        {
            int ProposedNumberPlate;
            sscanf(buf+strlen(key),"%d",&ProposedNumberPlate);
            cout << "EdbShowRec::ReadShowRecPar   ProposedNumberPlate = " << ProposedNumberPlate << endl;
            eProposedNumberPlate=ProposedNumberPlate;
        }

        /// INDICATION The latest string here to be written is in developement status.
        /// all others above should work....

        /// THE PARASETS STILL NEED CONFIRMATION if THE NUMBERS_ for labelling are correctly filled!!!

        /// Main settings preparating the reconstruction algorithm settings:
        else if ( !strcmp(key,"SHOW_ADDPARASET")  )
        {
            float par[10];
            for (Int_t i=0; i<10; ++i) {
                par[i]=-9999.0;
            }
            int type;
            int n=sscanf(buf+strlen(key),"%d %f %f %f %f %f",&type, &par[0], &par[1], &par[2], &par[3], &par[4] );
            cout << "n= " << n << endl;
            if (n==5)  {
                AddAlg(type,par);
            }
            else {
                cout << "EdbShowRec::ReadShowRecPar   WRONG FORMAT of SHOW_ADDPARASET. 4 Parameters at least!" << endl;
            }
        }


        else if ( !strcmp(key,"SHOW_SETPARASET")  )
        {
            float par[10];
            for (Int_t i=0; i<10; ++i) {
                par[i]=-9999.0;
            }
            int type;
            int n=sscanf(buf+strlen(key),"%d %f %f %f %f %f",&type, &par[0], &par[1], &par[2], &par[3], &par[4] );
            cout << "n= " << n << endl;
            if (n==5)  {
                ResetShowAlgArray();
                AddAlg(type,par);
            }
            else {
                cout << "EdbShowRec::ReadShowRecPar   WRONG FORMAT of SHOW_ADDPARASET. Need 4 Parameters at least!" << endl;
            }
        }

        else if ( !strcmp(key,"SHOW_DO_PARA")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_PARA = " << dospec << endl;
            SetDoParaType(dospec);
        }

        else if ( !strcmp(key,"SHOW_DO_PARA_NAME")  )
        {
            sscanf(buf+strlen(key),"%s",tmpchar);
            tmpString=TString(tmpchar);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_DO_PARA_NAME = " << tmpString << endl;
            SetDoParaType(tmpString);
        }

        else if ( !strcmp(key,"SHOW_ALG_USE_ALISUB")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowRecPar   SHOW_ALG_USE_ALISUB = " << dospec << endl;
            SetUseAliSub(dospec);
        }


    }
    fclose(fp);


    if (gEDBDEBUGLEVEL>2) Print_UseInBTType();
    if (gEDBDEBUGLEVEL>2) Print_InBTCut(0);

    // Update of Plate Number values is now here necessary:
    SetPlateNumberValues();    // Set Before to have proper eAli_Plate values filled.
    CheckPlateValues();


    Log(2,"EdbShowRec::ReadShowRecPar","Read shower parameters...done.");
    return 0;
}

//______________________________________________________________________________

void EdbShowRec::ResetUse_InBTType()
{
    Log(2,"EdbShowRec::ResetUse_InBTType()","ResetUse_InBTType");
    // Use no type to fill InBT Array at all(!) (i.e. reconstruct 0 showers).
    eUse_LT=kFALSE;
    eUse_PVREC=kFALSE;
    eUse_AliBT=kFALSE;
    eUse_AliLT=kFALSE;
    eUse_ROOTInBT=kFALSE;
    eUseNr=-1;
    Log(2,"EdbShowRec::ResetUse_InBTType()","ResetUse_InBTType...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Print_UseInBTType()
{
    Log(2,"EdbShowRec::PrintUse_InBTType","EdbShowRec::PrintUse_InBTType");
    // Use no type to fill InBT Array at all(!) (i.e. reconstruct 0 showers).
    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowRec::PrintUse_InBTType   eUse_LT= " << eUse_LT<< endl;
        cout << "EdbShowRec::PrintUse_InBTType   eUse_PVREC= " << eUse_PVREC<< endl;
        cout << "EdbShowRec::PrintUse_InBTType   eUse_AliBT= " << eUse_AliBT<< endl;
        cout << "EdbShowRec::PrintUse_InBTType   eUse_AliLT= " << eUse_AliLT<< endl;
        cout << "EdbShowRec::PrintUse_InBTType   eUse_ROOTInBT= " << eUse_ROOTInBT<< endl;
        cout << "EdbShowRec::PrintUse_InBTType   eUseNr= " << eUseNr<< endl;
    }
    Log(2,"EdbShowRec::PrintUse_InBTType","EdbShowRec::PrintUse_InBTType...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::Add_INBTCut(int layer, TCut &cut)
{
    Log(2,"EdbShowRec::Add_INBTCut","EdbShowRec::Add_INBTCut");
    if (layer<0) return;
    if (layer>2) return;
    if (!eInBTCuts[layer]) {
        cout << " EdbShowRec::Add_INBTCut   No eInBTCuts[layer] yet"<<endl;
        eInBTCuts[layer] = new TCut(cut);
    }
    else (*(eInBTCuts[layer]))+=cut;
    Log(2,"EdbShowRec::Add_INBTCut","EdbShowRec::Add_INBTCut...done.");
}

//______________________________________________________________________________

void EdbShowRec::Print_InBTCut(Int_t layer=0)
{
    Log(2,"EdbShowRec::Print_InBTCut","EdbShowRec::Print_InBTCut");
    if (!eInBTCuts[layer]) return;
    eInBTCuts[layer]->Print();
    Log(2,"EdbShowRec::Print_InBTCut","EdbShowRec::Print_InBTCut...done.");
    return;
}

//______________________________________________________________________________


//______________________________________________________________________________

void EdbShowRec::TxtToRecoShowerArray()
{
    Log(2,"EdbShowRec::TxtToRecoShowerArray","EdbShowRec::TxtToRecoShowerArray");

    cout << "EdbShowRec::TxtToRecoShowerArray   Reading a txt file with a list of segments"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   There are different ways of reading segment list:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   0) Simple Style:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   0)      X  Y  Z  TX  TY"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   1) EdaStyle:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   1)      ID  PL  PH  X  Y  TX  TY  0 :"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2) __Official__ (18.05.2010) Feedback Style:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      // VID     X        Y        Z 1ry charm tau Ndw Nup OutOfBrick :"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      //    tid v1 v2        x        y        z      tx      ty    ip1    ip2     p  pmin  pmax mn pa sb dk of ps  n RmaxT RmaxL    rmsT    rmsL pl1 pl2 res:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      //       ipl        x        y        z      tx      ty type irec ngrains:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   3) NagoyaTxtStyle:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   3)      PL (NG-order)  TX  TY  X  Y  Z  ID  W (NG-style)"<<endl;
    cout << endl;

    Log(2,"EdbShowRec::TxtToRecoShowerArray","EdbShowRec::TxtToRecoShowerArray...done.");
}


//______________________________________________________________________________

void EdbShowRec::TxtToRecoShowerArray(TString TxtFileName, Int_t TxtFileType)
{
    Log(2,"EdbShowRec::TxtToRecoShowerArray","EdbShowRec::TxtToRecoShowerArray");

    cout << "EdbShowRec::TxtToRecoShowerArray   Reading a txt file with a list of segments"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   There are different ways of reading segment list:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   0) Simple Style:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   0)      X  Y  Z  TX  TY"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   1) EdaStyle:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   1)      ID  PL  PH  X  Y  TX  TY  0 :"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2) __Official__ (18.05.2010) Feedback Style:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      // VID     X        Y        Z 1ry charm tau Ndw Nup OutOfBrick :"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      //    tid v1 v2        x        y        z      tx      ty    ip1    ip2     p  pmin  pmax mn pa sb dk of ps  n RmaxT RmaxL    rmsT    rmsL pl1 pl2 res:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   2)      //       ipl        x        y        z      tx      ty type irec ngrains:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   3) NagoyaTxtStyle:"<<endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   3)      PL (NG-order)  TX  TY  X  Y  Z  ID  W (NG-style)"<<endl;
    cout << endl;
    cout << "EdbShowRec::TxtToRecoShowerArray   TxtFileType = "<< TxtFileType << endl;

    switch (TxtFileType)
    {
    case 0:
        cout << "READ 0) Simple Style." << endl;
        TxtToRecoShowerArray_SimpleList(TxtFileName);
        break;

    case 1:
        cout << "READ 1) EdaStyle." << endl;
        TxtToRecoShowerArray_EDAList(TxtFileName);
        break;

    case 2:
        cout << "READ 2) __Official__ (18.05.2010) Feedback Style." << endl;
        TxtToRecoShowerArray_FeedBack(TxtFileName);
        break;

    case 3:
        cout << "READ 3) NagoyaTxtStyle." << endl;
        TxtToRecoShowerArray_SimpleListNagoya(TxtFileName);
        break;

    default:
        cout << "DO NOTHING!" << endl;
    }
    Log(2,"EdbShowRec::TxtToRecoShowerArray","EdbShowRec::TxtToRecoShowerArray...done.");
}

//______________________________________________________________________________


void EdbShowRec::TxtToRecoShowerArray_FeedBack(TString TxtFileName)
{
    Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","TxtToRecoShowerArray_FeedBack");
    Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","TxtToRecoShowerArray_FeedBack...done");
    // Copied the skeleton from Ariga-San code from EdbEDAIO class,
    // and took out some unnnecessary stuff...
    // Read feedback file format (ver 2009 Oct).
    // For the showers read only track-segment list...

    cout << "// Copied the skeleton from Ariga-San code from EdbEDIO class,"<<endl;
    cout << "// and took out some unnnecessary stuff..."<<endl;
    cout << "// Read feedback file format (ver 2009 Oct)."<<endl;
    cout << "// For the showers read only track-segment list..."<<endl;

    const char *filename = TxtFileName.Data();

    Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","File to be opened = %s",filename);
//     filename = "ds.feedback";

    if (NULL==filename) {
        Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","EdbShowRec::File open error. Filename wrong. Return.");
        return;
    }

    FILE *fp = fopen(filename,"rt");
    if (fp==NULL) {
        Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","EdbShowRec::File open error. File not there (?). Return.");
        return;
    }



    EdbPVRec *pvr = new EdbPVRec;
    EdbScanCond cond;
    EdbTrackP *t=0;

    char buf[256];

    while (fgets(buf,sizeof(buf),fp)) {
        TString line=buf;
        //printf("%s", line.Data());

        // track comment = ""
        if (line.Contains("track comment")) {
            int istart = line.Index("\"");
            int iend   = line.Index("\"",1,istart+1, TString::kExact);
            if (istart!=kNPOS && iend!=kNPOS && istart+1!=iend) {
                char buf[256];
                strncpy(buf, line.Data()+istart+1, iend-istart-1);
                buf[iend-istart-1]='\0';
                //tx->AddText(buf);
            }
        }

        if (line.Contains("Area information :")) {
            continue; // this we dont need here.
        }
        if (line.Contains(Form("area"))) {
            continue; // this we dont need here.
        }

        // Remove comments
        line.ReplaceAll("\t"," ");
        int iposcmt = line.Index("//");
        if (iposcmt!=kNPOS) line.Remove(iposcmt, line.Length()-iposcmt);

        // Check number of columns.
        TObjArray *tokens = line.Tokenize(" ");
        int ntokens = tokens->GetEntries();
        delete tokens;
        printf("tokens %d \n",ntokens);

        if (ntokens==0) continue;

        else if ( ntokens == 10 ) {
            // Vertices
            float x,y,z;
            int id,isprimary,istau, ischarm;
            sscanf(line.Data(),"%d %f %f %f %d %d %d", &id, &x, &y, &z, &isprimary, &ischarm, &istau);
            EdbVertex *v = new EdbVertex();
            v->SetXYZ(x,y,z);
            v->SetID(id);
            // Add Vertex to pvr object.
            pvr->AddVertex(v);
            printf("Vertex %d %f %f %f\n",v->ID(), v->X(), v->Y(), v->Z());
        }

        else if ( ntokens == 27 ) {
            // Tracks
            float x,y,z,ax,ay, ip_upvtx, ip_downvtx,  p,pmin,pmax;
            int id_track, id_upvtx, id_downvtx,  manual, particle_id, scanback, darkness;
            int OutOfBrick, LastPlate, nseg, iplRmax1, iplRmax2, result;
            float RmaxT, RmaxL, rmst, rmsl;

            int n,nn;
            sscanf(line.Data(),         "%d %d %d %f %f %f %f %f %f%n", &id_track, &id_upvtx, &id_downvtx, &x, &y, &z, &ax, &ay, &ip_upvtx, &nn);
            sscanf(line.Data()+(n=nn),  "%f %f %f %f %d %d %d %d %d%n", &ip_downvtx, &p,&pmin,&pmax, &manual, &particle_id, &scanback, &darkness, &OutOfBrick, &nn);
            sscanf(line.Data()+(n+=nn), "%d %d %f %f %f %f %d %d %d",   &LastPlate, &nseg, &RmaxT, &RmaxL, &rmst, &rmsl, &iplRmax1, &iplRmax2, &result);

            // Create Track. "t" is defined out of main loop.
            t = new EdbTrackP;
            t->Set(id_track, x, y, ax, ay, 0, 0);
            t->SetZ(z);
            t->SetTrack(id_track);
            t->SetErrors();
            // Add Track to pvr object.
            pvr->AddTrack(t);
        }

        else if ( ntokens ==  9 ) {
            // Segments
            int ipl, type, irec, ph;
            float x,y,z,ax,ay;
            sscanf(line.Data(),"%d %f %f %f %f %f %d %d %d", &ipl, &x, &y, &z, &ax, &ay, &type, &irec, &ph);
// 	    552 1 -1  10206.1  70129.5 -38891.2 -0.0610  0.1518   7.76 -1.0   1.54  1.09      2.7 0 0 0 0
            EdbSegP *s = new EdbSegP(t->ID(),x,y,ax,ay,0,0);
            s->SetZ(z);
            s->SetPID(ipl);
            s->SetPlate(ipl);
            s->SetW(ph);
            s->SetTrack(t->ID());
            //s->SetSide(type);
            s->SetErrors();

            t->AddSegment(s);
            pvr->AddSegment(*s);
        }
    }

    for (Int_t i=0; i<pvr->Npatterns(); ++i) pvr->GetPattern(i)->SetPID(i);
    for (Int_t i=0; i<pvr->Npatterns(); ++i) pvr->GetPattern(i)->SetSegmentsPID();

    printf("\nEdbPVRec summary. %d vertices, %d tracks.\n", pvr->Nvtx(), pvr->Ntracks());
    pvr->Print();

    fclose(fp);


    Log(2,"EdbShowRec::TxtToRecoShowerArray_FeedBack","EdbShowRec::TxtToRecoShowerArray_FeedBack...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::TxtToRecoShowerArray_SimpleList(TString TxtFileName)
{
    Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleList","EdbShowRec::TxtToRecoShowerArray_SimpleList");
    // Copied the skeleton from Ariga-San code from EdbEDAIO class,
    // and took out some unnnecessary stuff...
    // Read simple list file format (originally sent by Napoli People.).

    cout << "// Copied the skeleton from Ariga-San code from EdbEDAIO class,"<<endl;
    cout << "// and took out some unnnecessary stuff..."<<endl;
    cout << "// Read simple lsit file format (originally sent by Napoli People.)."<<endl;

    TObjArray* array=new TObjArray();
    EdbShowerP* shower= new EdbShowerP();

    int segID=0;
    int shID=0;

    const char* filename=TxtFileName.Data();//"shower_test.txt";
    if (NULL==filename) return;
    FILE *fp = fopen(filename,"r");
    if (fp==NULL) {
        Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleList","EdbShowRec::File open error.");
        return;
    }
    char buf[256];
    while (fgets(buf,sizeof(buf),fp)) {
        TString line=buf;
        printf("%s", line.Data());

        // Remove comments
        line.ReplaceAll("\t"," ");
        int iposcmt = line.Index("//");
        if (iposcmt!=kNPOS) line.Remove(iposcmt, line.Length()-iposcmt);

        // Check number of columns.
        TObjArray *tokens = line.Tokenize(" ");
        int ntokens = tokens->GetEntries();
        delete tokens;
        printf("tokens %d \n",ntokens);
        if (ntokens==0) continue;

        else if ( ntokens ==  5 ) {
            // Segments list in the format:
            // 91460.5     40155.50 36646.02  -0.0618   0.0214
            // 	X	Y	Z	TX	TY
            float x,y,z,ax,ay;
            sscanf(line.Data(),"%f %f %f %f %f", &x, &y, &z, &ax, &ay);
            EdbSegP *s = new EdbSegP(segID,x,y,ax,ay,0,0);
            s->SetZ(z);
            s->SetPID(abs(z)/1300);
            s->SetPlate(abs(z)/1300);
            s->SetW(50); // Artificially
            s->SetChi2(0.0001); // Artificially
            s->SetErrors();
            s->PrintNice();
            shower->AddSegment(s);
            ++segID;
        }

        else {
            // Add new shower in case the file has any line that doesnt match....
            // (yes, very simple statement).
            // First put old shower into array:
            //shower->PrintNice();
            array->Add(shower);
            // Then create new shower.
            ++shID;
            EdbShowerP* shower= new EdbShowerP();
            shower->SetID(shID);
        }
        // Next Line.
    }
    fclose(fp);

    // Add last shower
    //shower->PrintNice();
    array->Add(shower);

    cout << "EdbShowRec::TxtToRecoShowerArray_SimpleList   array->GetEntries() " << array->GetEntries() << endl;

    // Hand over array to the eRecoShowerArray:
    // (eRecoShowerArrayN is automatically adapted in SetRecoShowerArray().)
    SetRecoShowerArray(array);

    Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleList","EdbShowRec::TxtToRecoShowerArray_SimpleList...done");
}

//______________________________________________________________________________

void EdbShowRec::TxtToRecoShowerArray_EDAList(TString TxtFileName)
{
    // Copied the skeleton from Ariga-San code from EdbEDAIO class,
    // and took out some unnnecessary stuff...
    // Read simple list file format (originally sent by Napoli People.).

    Log(2,"EdbShowRec::TxtToRecoShowerArray_EDAList","EdbShowRec::TxtToRecoShowerArray_EDAList");


    cout << "// Copied the skeleton from Ariga-San code from EdbEDAIO class,"<<endl;
    cout << "// and took out some unnnecessary stuff..."<<endl;
    cout << "// Read EDA list file format."<<endl;

    TObjArray* array=new TObjArray();
    EdbShowerP* shower= new EdbShowerP();

    int segID=0;
    int shID=0;

    const char* filename=TxtFileName.Data();//"shower_test.txt";
    if (NULL==filename) return;
    FILE *fp = fopen(filename,"r");
    if (fp==NULL) {
        Log(2,"EdbShowRec::TxtToRecoShowerArray_EDAList","EdbShowRec::File open error.");
        return;
    }
    char buf[256];
    while (fgets(buf,sizeof(buf),fp)) {
        TString line=buf;
        printf("%s", line.Data());

        // Remove comments
        line.ReplaceAll("\t"," ");
        int iposcmt = line.Index("//");
        if (iposcmt!=kNPOS) line.Remove(iposcmt, line.Length()-iposcmt);

        // Check number of columns.
        TObjArray *tokens = line.Tokenize(" ");
        int ntokens = tokens->GetEntries();
        delete tokens;
        printf("tokens %d \n",ntokens);
        if (ntokens==0) continue;

        else if ( ntokens ==  8 || ntokens ==  9 ) {
            // Segments list in the EDA format:
            // 999 47  18   91391.7  38782.4     -0.0201  -0.0603 	0  DUMMY
            //     ID  PL  PH  X  Y  TX  TY  0 DUMMY
            float x,y,z,ax,ay;
            int id,pl,w,dummy;
            if (ntokens ==  8) sscanf(line.Data(),"%d %d %d %f %f %f %f %d", &id, &pl, &w, &x, &y, &ax, &ay, &dummy); // for ntokens ==  8
            if (ntokens ==  9) sscanf(line.Data(),"%d %d %d %f %f %f %f %d %d", &id, &pl, &w, &x, &y, &ax, &ay, &dummy, &dummy); // for ntokens ==  9
            EdbSegP *s = new EdbSegP(id,x,y,ax,ay,0,0);
            // s->SetZ(z); // no z info in this kind of format.
            // z in eda format:
            // pl57 == z=0
            // pl1 == z=-72800
            z=0+(pl-57)*1300;
            s->SetZ(z);
            s->SetPID(pl);
            s->SetPlate(pl);
            s->SetW(w);
            s->SetChi2(0.0001); // Artificially
            s->SetErrors();
            //s->PrintNice();
            shower->AddSegment(s);
            ++segID;
        }

        else if ( ntokens ==  11 ) {
            // Segments list in the EDA format:
            //  200  22     200    50  93345.2  39320.9 -47700.9 -0.0474  0.0100  0.00    0
            //     ID   PL  ID  PH  X  Y  Z  TX  TY  Chi2 DUMMY
            float x,y,z,ax,ay;
            int id,pl,w,dummy;
            sscanf(line.Data(),"%d %d %d %d %f %f %f %f %f %d %d", &id,  &pl, &id, &w, &x, &y, &z, &ax, &ay, &dummy, &dummy); // for ntokens ==  11
            EdbSegP *s = new EdbSegP(id,x,y,ax,ay,0,0);
            // s->SetZ(z); // no z info in this kind of format.
            // Here (ntokens ==  11) z info is written
            // z in eda format:
            // pl57 == z=0
            // pl1 == z=-72800
            // z=0+(pl-57)*1300;
            s->SetZ(z);
            s->SetPID(pl);
            s->SetPlate(pl);
            s->SetW(w);
            s->SetChi2(0.0001); // Artificially
            s->SetErrors();
            //s->PrintNice();
            shower->AddSegment(s);
            ++segID;
        }

        else {
            // Add new shower in case the file has any line that doesnt match....
            // (yes, very simple statement).
            // First put old shower into array:
            //shower->PrintNice();
            array->Add(shower);
            // Then create new shower.
            ++shID;
            EdbShowerP* shower= new EdbShowerP();
            shower->SetID(shID);
        }
        // Next Line.
    }
    fclose(fp);

    // Add last shower
    //shower->PrintNice();
    array->Add(shower);

    cout << "EdbShowRec::TxtToRecoShowerArray_EDAList   array->GetEntries() " << array->GetEntries() << endl;

    // Hand over array to the eRecoShowerArray:
    // (eRecoShowerArrayN is automatically adapted in SetRecoShowerArray().)
    SetRecoShowerArray(array);

    Log(2,"EdbShowRec::TxtToRecoShowerArray_EDAList","EdbShowRec::TxtToRecoShowerArray_EDAList...done");
}

//______________________________________________________________________________

void EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya(TString TxtFileName)
{

    // Copied the skeleton from Ariga-San code from EdbEDAIO class,
    // and took out some unnnecessary stuff...
    // Read simple list file format (originally sent by Napoli People.).
    Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya","EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya");


    cout << "// Copied the skeleton from Ariga-San code from EdbEDAIO class,"<<endl;
    cout << "// and took out some unnnecessary stuff..."<<endl;
    cout << "// Read simple lsit file format (originally sent by Nagoya People.)."<<endl;

    TObjArray* array=new TObjArray();
    EdbShowerP* shower= new EdbShowerP();

    int segID=0;
    int shID=0;

    const char* filename=TxtFileName.Data();//"shower_test.txt";
    if (NULL==filename) return;
    FILE *fp = fopen(filename,"r");
    if (fp==NULL) {
        Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya","EdbShowRec::File open error.");
        return;
    }
    char buf[256];
    while (fgets(buf,sizeof(buf),fp)) {
        TString line=buf;
        printf("%s", line.Data());

        // Remove comments
        line.ReplaceAll("\t"," ");
        int iposcmt = line.Index("//");
        if (iposcmt!=kNPOS) line.Remove(iposcmt, line.Length()-iposcmt);

        // Check number of columns.
        TObjArray *tokens = line.Tokenize(" ");
        int ntokens = tokens->GetEntries();
        delete tokens;
        printf("tokens %d \n",ntokens);
        if (ntokens==0) continue;

        else if ( ntokens ==  8 ) {
            // Segments list in the format:
// 			$1 : plate (Nagoya plate#)
// 			$2 : ax (ax,ay,x,y,z)
// 			$3 : ay
// 			$4 : x
// 			$5 : y
// 			$6 : z
// 			$7 : BaseTrackID (scan data)
// 			$8 : PH (scan data)
            // 15  -0.0537   0.0313  91575.5  38876.6  54339    5337769  240059
            float x,y,z,ax,ay;
            int id,pid,w;
            sscanf(line.Data(),"%d %f %f  %f %f %f  %d %d", &pid, &ax, &ay, &x, &y, &z, &id,&w);
            EdbSegP *s = new EdbSegP(id,x,y,ax,ay,0,0);
            s->SetZ(z);
            s->SetPID(58-pid); // nagoya pid is labelled reversely!
            s->SetPlate(58-pid);  // nagoya pid is labelled reversely!
            s->SetW(int(w/10000));
            s->SetChi2(0.0001); // Artificially
            s->SetErrors();
            //s->PrintNice();
            shower->AddSegment(s);
            ++segID;
        }

        else {
            // Add new shower in case the file has any line that doesnt match....
            // (yes, very simple statement).
            // First put old shower into array:
            //shower->PrintNice();
            array->Add(shower);
            // Then create new shower.
            ++shID;
            EdbShowerP* shower= new EdbShowerP();
            shower->SetID(shID);
        }
        // Next Line.
    }
    fclose(fp);

    // Add last shower
    //shower->PrintNice();
    array->Add(shower);

    cout << "EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya   array->GetEntries() " << array->GetEntries() << endl;

    // Hand over array to the eRecoShowerArray:
    // (eRecoShowerArrayN is automatically adapted in SetRecoShowerArray().)
    SetRecoShowerArray(array);

    Log(2,"EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya","EdbShowRec::TxtToRecoShowerArray_SimpleListNagoya...done");
}

//______________________________________________________________________________

void EdbShowRec::RecoShowerArray_To_Treebranch()
{
    Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","Convert the RecoShowerArray into <<treebranch>> style tree entries (backward compability).");
    Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","Reminder: a shower may have maxium of 5000 segments. - Limitation, my be discarded later, when necessary.");

    // Declaring ObjArray which are needed. Use out of private member variables of this class.
    TObjArray* showarray =eRecoShowerArray;
    Int_t      showarrayN=eRecoShowerArrayN;
    EdbShowerP* show=0;

    // VARIABLES: shower_  "treebranch"  reconstruction
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
    Float_t shower_purb; // purity of shower
    Int_t   shower_numberofholes;
    Int_t   shower_numberofholesconseq;


    // Creating the file at this position, that the eShowerTree
    // will be connected to this file (otherwise flushing issues...)
    if (!eFile_Out_treebranch) eFile_Out_treebranch = new TFile(eFilename_Out_treebranch,"UPDATE");
    eFile_Out_treebranch->cd();

    // Now create the tree. The tree is not made persistent, since at the end
    // of this routine, it will be destructed anyway.

    TString treename=GetTreebranchName();
    TTree* eShowerTree = new TTree(treename,treename);

    // Is the basket size of the branches relevant? I dont know ...
    // To be safe, I chose a "Basket Size" bigger than the
    // actual "Total Size", i.e. Compression = 1.00
    eShowerTree->Branch("number_eventb",&shower_number_eventb,"number_eventb/I");
    eShowerTree->Branch("sizeb",&shower_sizeb,"sizeb/I");
    eShowerTree->Branch("sizeb15",&shower_sizeb15,"sizeb15/I");
    eShowerTree->Branch("sizeb20",&shower_sizeb20,"sizeb20/I");
    eShowerTree->Branch("sizeb30",&shower_sizeb30,"sizeb30/I");
    eShowerTree->Branch("isizeb",&shower_isizeb,"isizeb/I");
    eShowerTree->Branch("xb",shower_xb,"xb[sizeb]/F",256000);
    eShowerTree->Branch("yb",shower_yb,"yb[sizeb]/F",256000);
    eShowerTree->Branch("zb",shower_zb,"zb[sizeb]/F",256000);
    eShowerTree->Branch("txb",shower_txb,"txb[sizeb]/F",256000);
    eShowerTree->Branch("tyb",shower_tyb,"tyb[sizeb]/F",256000);
    eShowerTree->Branch("nfilmb",shower_nfilmb,"nfilmb[sizeb]/I",256000);
    eShowerTree->Branch("ntrace1simub",shower_ntrace1simub,"ntrace1simu[sizeb]/I",256000);  // s.eMCEvt
    eShowerTree->Branch("ntrace2simub",shower_ntrace2simub,"ntrace2simu[sizeb]/I",256000); // s.eW
    eShowerTree->Branch("ntrace3simub",shower_ntrace3simub,"ntrace3simu[sizeb]/F",256000); // s.eP
    eShowerTree->Branch("ntrace4simub",shower_ntrace4simub,"ntrace4simu[sizeb]/I",256000); // s.eFlag
    eShowerTree->Branch("chi2btkb",shower_chi2btkb,"chi2btkb[sizeb]/F",256000);
    eShowerTree->Branch("deltarb",shower_deltarb,"deltarb[sizeb]/F",256000);
    eShowerTree->Branch("deltathetab",shower_deltathetab,"deltathetab[sizeb]/F",256000);
    eShowerTree->Branch("deltaxb",shower_deltaxb,"deltaxb[sizeb]/F",256000);
    eShowerTree->Branch("deltayb",shower_deltayb,"deltayb[sizeb]/F",256000);
    eShowerTree->Branch("tagprimary",shower_tagprimary,"tagprimary[sizeb]/F",256000);
    eShowerTree->Branch("energy_shot_particle",&shower_energy_shot_particle,"energy_shot_particle/F");
    eShowerTree->Branch("E_MC",&shower_energy_shot_particle,"E_MC/F");
    eShowerTree->Branch("showerID",&shower_showerID,"showerID/I");
    eShowerTree->Branch("idb",shower_idb,"idb/I");
    eShowerTree->Branch("plateb",shower_plateb,"plateb[sizeb]/I",256000);
    eShowerTree->Branch("deltasigmathetab",shower_deltasigmathetab,"deltasigmathetab[59]/F",256000);
    eShowerTree->Branch("lenghtfilmb",&shower_numberofilms,"lenghtfilmb/I");
    eShowerTree->Branch("purb",&shower_purb,"purb/F"); // shower purity
    eShowerTree->Branch("nholesb",&shower_numberofholes,"nholesb/I"); // #of (single) empty plates
    eShowerTree->Branch("nholesmaxb",&shower_numberofholesconseq,"nholesmaxb/I"); // #of (consecutive) empty plates

    EdbSegP* seg;
    EdbSegP* Inseg;
    Int_t diff_pid;
    Float_t min_shower_deltathetab=99999;
    Float_t min_shower_deltar=99999;
    Float_t test_shower_deltathetab=99999;
    Float_t test_shower_deltar=99999;
    Float_t test_shower_deltax,test_shower_deltay;
    Int_t max_diff_pid=0;
    Float_t extrapol_x,extrapol_y, extrapo_diffz;

    Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","showarray->GetEntries()  = %d", showarrayN);


    for (int i_shower=0; i_shower<showarrayN; i_shower++) {
        // START OVER LOOP of   ONE SHOWER:
        show=(EdbShowerP*)showarray->At(i_shower);

        if (i_shower%100==0) Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","Converting shower  %05d with %03d basetracks.", i_shower,show->N());
        // cout << "	EdbShowRec::RecoShowerArray_To_Treebranch   Address of shower: " << show << endl;
        //cout << "	EdbShowRec::RecoShowerArray_To_Treebranch   UpdateX() of shower: " << endl;
        //show->UpdateX();

        // Initialize arrays...
        min_shower_deltathetab=99999;
        min_shower_deltar=99999;
        test_shower_deltathetab=99999;
        test_shower_deltar=99999;
        test_shower_deltax=0;
        test_shower_deltay=0;
        max_diff_pid=0;
        extrapol_x=0;
        extrapol_y=0;
        extrapo_diffz=0;
        shower_sizeb15=0;
        shower_sizeb20=0;
        shower_sizeb30=0;
        shower_sizeb=0;
        shower_energy_shot_particle=0.0;
        shower_numberofilms=0;
        shower_numberofholes=0;
        shower_numberofholesconseq=0;
        for (int ii=0; ii<5000; ii++)  {
            shower_xb[ii]=0;
            shower_yb[ii]=0;
            shower_zb[ii]=0;
            shower_txb[ii]=0;
            shower_tyb[ii]=0;
            shower_nfilmb[ii]=0;
            shower_tagprimary[ii]=0;
            shower_ntrace1simub[ii]=0;
            shower_ntrace2simub[ii]=0;
            shower_ntrace3simub[ii]=0;
            shower_ntrace3simub[ii]=0;
            shower_deltaxb[ii]=0;
            shower_deltayb[ii]=0;
            shower_chi2btkb[ii]=0;
            shower_idb[ii]=0;
            shower_plateb[ii]=0;
        }
        for (Int_t i=1; i<59; ++i) {
            shower_deltasigmathetab[i]=0;
        }

        Log(3,"EdbShowRec::RecoShowerArray_To_Treebranch","Arrays reseted");


        // Part to calculate the TransfereedVariables....
        shower_sizeb=show->N();
        shower_numberofholes=show->N0();
        shower_numberofholesconseq=show->N00();

        Inseg=(EdbSegP*)show->GetSegment(0);
        shower_energy_shot_particle=Inseg->P();
        shower_purb=show->GetPurity();

        if (gEDBDEBUGLEVEL>3) cout << "---   ---   ---------------------"<<endl;

        //-------------------------------------
        for (int ii=0; ii<shower_sizeb; ii++)  {
            if (ii>=5000) {
                cout << "WARNING: shower_sizeb ( " << shower_sizeb<< ") greater than SHOWERARRAY.   Set sizeb to 4999 and  Stop filling!."<<endl;
                shower_sizeb=4999;
                break;
            }
            seg=(EdbSegP*)show->GetSegment(ii);
            if (gEDBDEBUGLEVEL>3) {
                cout << "====== --- DOING  segment "  << ii << endl;
                seg->PrintNice();
            }

            //-------------------------------------
            shower_xb[ii]=seg->X();
            shower_yb[ii]=seg->Y();
            shower_txb[ii]=seg->TX();
            shower_tyb[ii]=seg->TY();
            shower_zb[ii]=seg->Z();
            shower_chi2btkb[ii]=seg->Chi2();
            shower_deltathetab[ii]=0.5;
            shower_deltarb[ii]=200;
            shower_tagprimary[ii]=0;
            if (ii==0) shower_tagprimary[ii]=1;
            shower_isizeb=1; // always 1, not needed anymore
            if (seg->MCEvt()>0) shower_number_eventb=seg->MCEvt();
            shower_ntrace1simub[ii]=0;
            if (seg->MCEvt()>0) shower_ntrace1simub[ii]=seg->MCEvt();
            shower_ntrace2simub[ii]=seg->W();
            shower_ntrace3simub[ii]=seg->P();
            shower_ntrace4simub[ii]=seg->Flag();
            shower_idb[ii]=seg->ID();
            shower_plateb[ii]=seg->PID();
            //-------------------------------------
            // PUT HERE:  deltarb,deltarb, nflimb, sizeb15......
            diff_pid=TMath::Abs( Inseg->PID()-seg->PID() )+1;
            // (does this work for up/downsream listing??)
            // (yes, since InBT->PID is also changed.)

            // InBT:
            if (ii==0) {
                shower_deltathetab[0]=0.5;
                shower_deltarb[0]=200;
                shower_nfilmb[0]=1;
            }
            // All other BTs:
            if (ii>0) {
                // its correct like this, since this is the way it is done in
                // the official FJ-Algorithm:
                shower_nfilmb[ii]=diff_pid;
                if (gEDBDEBUGLEVEL>3) cout << "--- ---Inseg->PID() seg->PID() ii diif_pid shower_nfilmb[ii]  " << Inseg->PID()<< "   "  <<  seg->PID() << "   " << ii<< "  " << diff_pid<<"  "<< shower_nfilmb[ii]<<"  " << endl;

                if (diff_pid >= 15 ) shower_sizeb15++;
                if (diff_pid >= 20 ) shower_sizeb20++;
                if (diff_pid >= 30 ) shower_sizeb30++;

                // PUT HERE:  calculation routine for shower_deltasigmathetab
                // see referenc in thesis of Luillo Esposito, page 109.
                shower_deltasigmathetab[diff_pid]=shower_deltasigmathetab[diff_pid]+(TMath::Power(shower_txb[ii]-shower_txb[0],2)+TMath::Power(shower_tyb[ii]-shower_tyb[0],2));

                // PUT HERE:  calculation routine for shower_deltathetab, shower_deltarb
                // Exrapolate the BT [ii] to the position [jj] and then calc the
                // position and slope differences for the best matching next segment.
                // For the backward extrapolation of the   shower_deltathetab and shower_deltarb
                // calulation for BaseTrack(ii), Basetrack(jj)->Z() hast to be smaller.
                min_shower_deltathetab=9999;   // Reset
                min_shower_deltar=9999;        // Reset
                for (int jj=0; jj<shower_sizeb; jj++)  {

                    if (ii==jj) continue;
                    // since we do not know if BTs are ordered by their Z positions:
                    // and cannot cut directly on the number in the shower entry:
                    // if (shower_zb[ii]<shower_zb[jj]) cout << "WARNING :: shower_zb[ii]<shower_zb[jj] : shower_zb[ii]= " << shower_zb[ii] << "  shower_zb[jj] =  " <<  shower_zb[jj] << "   THIS LEADS TO CONTINUE THE LOOP!!!"<< endl;
                    if (shower_zb[ii]<shower_zb[jj]) continue; // ok, since we calculate deltarb and deltathetab backwards (in Z)

                    extrapo_diffz=shower_zb[ii]-shower_zb[jj];
                    //if (TMath::Abs(extrapo_diffz)>4*1300+1.0) continue; // max 4 plates backpropagation  // change to an average delat z of 1350.
                    if (TMath::Abs(extrapo_diffz)>4*1350+1.0) continue; // max 4 plates backpropagation
// 					if (TMath::Abs(extrapo_diffz)>7*1300+1.0) continue; // max 7 plates backpropagation
// 					if (TMath::Abs(extrapo_diffz)>9*1300+1.0) continue; // max 9 plates backpropagation
                    if (TMath::Abs(extrapo_diffz)<1.0) continue; // remove same positions.

                    extrapol_x=shower_xb[ii]-shower_txb[ii]*extrapo_diffz; // minus, because its ii after jj.
                    extrapol_y=shower_yb[ii]-shower_tyb[ii]*extrapo_diffz; // minus, because its ii after jj.

                    // Delta radius we need to extrapolate.
                    test_shower_deltax=extrapol_x;//shower_txb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_xb[ii];
                    test_shower_deltay=extrapol_y;//shower_tyb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_yb[ii];
                    test_shower_deltax=test_shower_deltax-shower_xb[jj];
                    test_shower_deltay=test_shower_deltay-shower_yb[jj];
                    test_shower_deltar=TMath::Sqrt(test_shower_deltax*test_shower_deltax+test_shower_deltay*test_shower_deltay);

                    // Delta theta we do not need to extrapolate. (old version...)
                    //test_shower_deltathetab=TMath::Sqrt(shower_txb[ii]*shower_txb[ii]+shower_tyb[ii]*shower_tyb[ii]);
                    //test_shower_deltathetab=test_shower_deltathetab-TMath::Sqrt(shower_txb[jj]*shower_txb[jj]+shower_tyb[jj]*shower_tyb[jj]);
                    //test_shower_deltathetab=TMath::Abs(test_shower_deltathetab);
                    //----
                    // As before in ShowRec this way of calculation is not equivalent as calculating
                    // DeltaTheta domponentwise:
                    // Code from libShower:
                    // delta = sqrt((SX0-a->GetTXb(l2))*(SX0-a->GetTXb(l2))+((SY0-a->GetTYb(l2))*(SY0-a->GetTYb(l2))));
                    test_shower_deltathetab=TMath::Sqrt(TMath::Power(shower_txb[ii]-shower_txb[jj],2)+TMath::Power(shower_tyb[ii]-shower_tyb[jj],2));

                    if (gEDBDEBUGLEVEL>3) cout << "RecoShowerArray_To_Treebranch--- ---ii = " << ii << " jj= " << jj << "  test_shower_deltar = "<< test_shower_deltar << "    test_shower_deltathetab = "<< test_shower_deltathetab <<  endl;

                    // Check if both dr,dt match parameter criteria and then just take these values.....
                    // Maybe a change is necessary because it is not exactly the same as in the off. algorithm.
                    // Make these values equal to the one in the "official algorithm"..... 150microns and 100mrad.
// 	if (test_shower_deltar<150 && test_shower_deltathetab<0.15 ) {

                    //cout << "DEBUG DEBUG DEBUG    CHANGED THE  (test_shower_deltar<150 && test_shower_deltathetab<0.15 )  FOR TES TING !! " << endl;
                    //cout << "DEBUG DEBUG DEBUG    SET THEM BACK AS SOON AS YOURE DONE TES TING!!! " << endl;
                    if (test_shower_deltar<1000 && test_shower_deltathetab<2.0 ) { // open cut values for best combifinding
                        /// -----     IMPORTANT::  these areopen cut values for best combifinding of pair BT deltaR/Theta values
                        /// -----     IMPORTANT::  then you do NOT necessarily get back your values which you put in durign
                        /// -----     IMPORTANT::  your shower reconstruction cone ( deltaR/Theta cutvalues could be NO cutvalues
                        /// -----     IMPORTANT::  for some reconstruction algorithms for example, but we wanna have these values anyway.
                        ///  In Any Case:
                        ///  Frederics Cut looks only for best min_shower_deltar so we do also.
                        if (test_shower_deltar<min_shower_deltar) {
                            min_shower_deltathetab=test_shower_deltathetab;
                            min_shower_deltar=test_shower_deltar;
                            shower_deltathetab[ii]=min_shower_deltathetab;
                            shower_deltarb[ii]=min_shower_deltar;
                        }
                    } // if (test_shower_deltar<1000 && test_shower_deltathetab<2.0 )

                } //for (int jj=0;jj<shower_sizeb;jj++)
            } // if (ii>0)

            //-------------------------------------
        }  // for (int ii=0;ii<shower_sizeb;ii++
        //-------------------------------------

        //-------------------------------------
        for (Int_t i=1; i<59; ++i) {
            shower_deltasigmathetab[i]=TMath::Sqrt(shower_deltasigmathetab[i]);
        }
        shower_numberofilms=shower_nfilmb[shower_sizeb-1]; // there we assume (this is correct always?) when
        // the last shower BT is in the last film...

        // DEBUG test :
        if (shower_numberofilms!=show->Npl()) cout << "ERROR   ERROR   shower_numberofilms!=show->Npl()" << endl;


        if (gEDBDEBUGLEVEL>3) cout << "	EdbShowRec::RecoShowerArray_To_Treebranch   Loop done. Before ..Fill().. now."<<endl;

        // Fill Tree:
        eShowerTree->Fill();

        // DEBUG
        if (gEDBDEBUGLEVEL>2) eShowerTree->Show(eShowerTree->GetEntries()-1);

    } //  end of    for (int i_shower=0; i_shower<showarray->GetEntries(); i_shower++)

    Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","Loop over showerArray finished.");




    /*
        // Directly write the file here. No special function is made for this:
        // this specific function makes only sense when used here in this specific
        // context:
        // Write this  treebranch  to the specified file, use UPDATE option to store all in case there are more treebranches
        // dirty solution, but for now it may work (encapsulated in if statements)..
        // Update: OR IT MAY NOT WORK .....

        ERROR MESSAGE: OCTOBER: 9th 2017
        Attaching file ShowerTreebranch.root as _file0...
    root [1] treebranch->Show(0)
    Error in <TBasket::Streamer>: The value of fNbytes is incorrect (-1091633152) ; trying to recover by setting it to zero
    Error in <TBranch::GetBasket>: File: ShowerTreebranch.root at byte:262144, branch:number_eventb, entry:0, badread=1, nerrors=1, basketnumber=0
    Error in <TTree::Show()>: Cannot read entry 0 (I/O error)
    maybe size of tbranch is wrong ??? t byte:262144,

        // POSSIBLE ? solutiuon:
        https://root-forum.cern.ch/t/read-ttree-from-tfile-fill-it-again-and-save-to-new-tfile/17899
        OR:
        pcanal@fnal.govNo presence information
    > it could be that part of the tree gets "improperly" written because ROOT gets
    > confused when to write to the file. By cd-ing before creating the tree,
    > the "confusion" is resolved somehow.
    Yes, this is the right solution.   Beyond a certain size, it starts flushing
    the basket to the file the TTree is attached to.   Without the explicit cd
    the TTree gets associated to which ever file was the current file at the
    time.
    Cheers,
    Philippe.
    */

    /*
    cout << "eShowerTree          has entries     :  " <<  eShowerTree->GetEntries() << endl;
    cout << "eShowerTree->GetCurrentFile()" << endl;
    cout << eShowerTree->GetCurrentFile() << endl;
    cout << "eShowerTree->GetCurrentFile()->GetName()" << endl;
    cout << eShowerTree->GetCurrentFile()->GetName() << endl;
    cout << "-------------------------------------------------------------------_" << endl;
    */

    eShowerTree->Dump();


    if (eWriteFileTreebranch==kTRUE) {
        eFile_Out_treebranch->Print();
        if (gEDBDEBUGLEVEL>2)  eShowerTree->Print();
        eFile_Out_treebranch->cd();
        // Why the option "kWriteDelete"? I had had a reason for this, but I dont know anymore ...
        eShowerTree->Write("",TObject::kWriteDelete);
        cout << "DONE:  ---------------------------------  eShowerTree->Write("",TObject::kWriteDelete);    ----------------------------------_" << endl;
        // Strange, when I close the file directly here, it crashes ....
        // Probably, because the Tree is still connected to the file ....
        // eFile_Out_treebranch->Close();
    }

    /*
    cout << "eShowerTree          has entries     :  " <<  eShowerTree->GetEntries() << endl;
    cout << "eFile_Out_treebranch          ->ls() :  " << endl;
    eFile_Out_treebranch    ->ls() ;
    cout << "eFile_Out_treebranch->IsOpen()? :  " << eFile_Out_treebranch->IsOpen() << endl;
    cout << "eShowerTree          is object at memory:  " <<  eShowerTree << endl;
    cout << "eFile_Out_treebranch is object at memory:  " <<  eFile_Out_treebranch << endl;
    */

    delete eShowerTree;
    eShowerTree=0;

    eFile_Out_treebranch->Close();
    //cout << "eFile_Out_treebranch->IsOpen()? :  " << eFile_Out_treebranch->IsOpen() << endl;

    delete eFile_Out_treebranch;
    eFile_Out_treebranch=0;

    Log(2,"EdbShowRec::RecoShowerArray_To_Treebranch","RecoShowerArray_To_Treebranch...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::Treebranch_To_RecoShowerArray(TObjArray* showarr, TTree* treebranch)
{
    Log(2,"EdbShowRec::Treebranch_To_RecoShowerArray","EdbShowRec::Treebranch_To_RecoShowerArray");

    // Declaring ObjArray which are needed. Use out of private member variables of this class.
    TObjArray* showarray;
    showarray=showarr;
    cout << "showarray given over:" << showarr << endl;
    TTree* eShowerTree = treebranch;

    EdbShowerP* show=0;
    EdbSegP*    showseg=0;

    //-     VARIABLES: shower_  "treebranch"  reconstruction
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
    Float_t shower_purb; // purity of shower


    // Set Addresses of treebranch tree:
    eShowerTree->SetBranchAddress("number_eventb",&shower_number_eventb);
    eShowerTree->SetBranchAddress("sizeb",&shower_sizeb);
    eShowerTree->SetBranchAddress("sizeb15",&shower_sizeb15);
    eShowerTree->SetBranchAddress("sizeb20",&shower_sizeb20);
    eShowerTree->SetBranchAddress("sizeb30",&shower_sizeb30);
    eShowerTree->SetBranchAddress("isizeb",&shower_isizeb);
    eShowerTree->SetBranchAddress("xb",shower_xb);
    eShowerTree->SetBranchAddress("yb",shower_yb);
    eShowerTree->SetBranchAddress("zb",shower_zb);
    eShowerTree->SetBranchAddress("txb",shower_txb);
    eShowerTree->SetBranchAddress("tyb",shower_tyb);
    eShowerTree->SetBranchAddress("nfilmb",shower_nfilmb);
    eShowerTree->SetBranchAddress("ntrace1simub",shower_ntrace1simub);  // s.eMCEvt
    eShowerTree->SetBranchAddress("ntrace2simub",shower_ntrace2simub); // s.eW
    eShowerTree->SetBranchAddress("ntrace3simub",shower_ntrace3simub); // s.eP
    eShowerTree->SetBranchAddress("ntrace4simub",shower_ntrace4simub); // s.eFlag
    eShowerTree->SetBranchAddress("chi2btkb",shower_chi2btkb);
    eShowerTree->SetBranchAddress("deltarb",shower_deltarb);
    eShowerTree->SetBranchAddress("deltathetab",shower_deltathetab);
    eShowerTree->SetBranchAddress("deltaxb",shower_deltaxb);
    eShowerTree->SetBranchAddress("deltayb",shower_deltayb);
    eShowerTree->SetBranchAddress("tagprimary",shower_tagprimary);
    eShowerTree->SetBranchAddress("energy_shot_particle",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("E_MC",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("showerID",&shower_showerID);
    eShowerTree->SetBranchAddress("idb",shower_idb);
    eShowerTree->SetBranchAddress("plateb",shower_plateb);
    eShowerTree->SetBranchAddress("deltasigmathetab",shower_deltasigmathetab);
    eShowerTree->SetBranchAddress("lenghtfilmb",&shower_numberofilms);
    eShowerTree->SetBranchAddress("purityb",&shower_purb); // shower purity


    Int_t nent=eShowerTree->GetEntries();
    cout << "nent = " << nent << endl;

    Int_t nenttotalcount=0;
    cout << "nenttotalcount = " << nenttotalcount << endl;

    Bool_t QUICK_READ=kFALSE;
// 	QUICK_READ=kTRUE; cout << "QUICK_READ=kTRUE; "<< endl;
    if (QUICK_READ) nent=1; // read only first shower... (mainly for debug purposes).

    // Loop over Tree Entries (==different showers):
    for (Int_t i=0; i<nent; ++i) {
        eShowerTree->GetEntry(i);
        //cout << "i = " << i << endl;
        //cout << "shower_tagprimary[0] = " << shower_tagprimary[0] << endl;
        if (gEDBDEBUGLEVEL>3) eShowerTree->Show(i);


        // Since we do not take showers with below 1segment
        if (shower_sizeb<=1) continue;

        show = new EdbShowerP(shower_sizeb);

        // For Frederics algorithm, shower_number_eventb gives NOT -999 as for a BG track,
        // but "0". So we need to distinguish also for "0" cases:
        show ->SetMC(shower_number_eventb,shower_number_eventb);
        if (shower_number_eventb==0) show ->SetMC(-999,999);

        // To Have unique profile histograms in the memory:
        // show = new EdbShowerP(0,eRecoShowerArrayN,shower_sizeb);

        //cout << "------------------- shower_tagprimary[0]  shower_tagprimary[shower_sizeb-1]  " << shower_tagprimary[0] << "  " <<  shower_tagprimary[shower_sizeb-1] << endl;

        // Loop over shower Entries (==different basetracks):
        for (int j=0; j<shower_sizeb; ++j) {

            // Keep Notice of the order!!!
            // shower_tagprimary  has to be added first!!
            // temporary solution: if that is not the case as below, no segment will be added....
            // ...
            if (shower_tagprimary[0]!=1) continue;

            // int id, float x, float y, float tx, float ty, float w = 0, int flag = 0
            showseg = new EdbSegP(j,shower_xb[j],shower_yb[j],shower_txb[j],shower_tyb[j],shower_ntrace2simub[j],0);

            showseg->SetZ(shower_zb[j]);
            showseg->SetChi2(shower_chi2btkb[j]);
            showseg->SetP(shower_ntrace3simub[j]);
            showseg->SetFlag(shower_ntrace4simub[j]);
            showseg->SetMC(shower_ntrace1simub[j],shower_ntrace1simub[j]);
            // For Frederics algorithm, ntrace1simub gives NOT -999 as for a BG track,
            // but "0". So we need to distinguish also for "0" cases:
            if (shower_ntrace1simub[j]==0) showseg->SetMC(-999,-999);

            showseg->SetPID(shower_plateb[j]);
            // in real data plateb is 57 for emulsion 57...(sim may be different, depending on orfeo...)
            showseg->SetID(shower_idb[j]);
            showseg->SetProb(1);

            //showseg->Print();

            show->AddSegment(showseg);
        } // Loop over shower Entries (==different basetracks):


        // This ID should be unique!
        // (Means every shower has one different!)
        show->SetID(nenttotalcount);
        nenttotalcount++;

        show->Update();
        //cout << "EdbShowRec::Treebranch_To_RecoShowerArray   show->PrintNice(); ...   done." << endl;

        // Add Shower Object to Shower Reco Array.
        // Not, if its empty,
        // Not, if its containing only one BT:
        if (show->N()>1) {
            showarray->Add(show);
            ++eRecoShowerArrayN;
            //cout << "EdbShowRec::Treebranch_To_RecoShowerArray   showarray->Add(show) ...   done." << endl;
        }

        if (gEDBDEBUGLEVEL>3) show->PrintBasics();

    }// Loop over Tree Entries (==different showers):



    eShowerTree->Show(0);
    eShowerTree->Show(nent-1);

    cout << "EdbShowRec::Treebranch_To_RecoShowerArray   Loop over Tree Entries (==different showers)  done." << endl;
    cout << "EdbShowRec::Treebranch_To_RecoShowerArray   showarray->N()" << showarray->GetEntries() <<endl;
    SetRecoShowerArrayN(showarray->GetEntries());


    Log(2,"EdbShowRec::Treebranch_To_RecoShowerArray","EdbShowRec::Do now Write_RecoShowerArray....");
    Write_RecoShowerArray(showarray);

    Log(2,"EdbShowRec::Treebranch_To_RecoShowerArray","EdbShowRec::Treebranch_To_RecoShowerArray...done.");
    return;
}




//______________________________________________________________________________


void EdbShowRec::PrintRecoShowerArrayFast(Int_t entry)
{
    Log(2,"EdbShowRec::PrintRecoShowerArrayFast","Printing overview for all entries from the reco shower array");

    if (entry<0) {
        Log(2,"EdbShowRec::PrintRecoShowerArray","Print full array.",entry);
    }
    if (entry>= GetRecoShowerArrayN()) {
        Log(2,"EdbShowRec::PrintRecoShowerArray","Requested entry %d is greater than entries in the array. Print full array.",entry);
        entry=-1;
    }

    EdbShowerP* show=0;
    for (Int_t i=0; i<GetRecoShowerArrayN(); ++i) {
        if (entry!=i && entry>=0) continue;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        show->PrintBasics();
    }

    Log(2,"EdbShowRec::PrintRecoShowerArrayFast","EdbShowRec::PrintRecoShowerArrayFast...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::PrintRecoShowerArray(Int_t entry)
{
    Log(2,"EdbShowRec::PrintRecoShowerArray","Printing entry %d from the reco shower array",entry);

    if (entry<0) {
        Log(2,"EdbShowRec::PrintRecoShowerArray","Print full array.",entry);
    }
    if (entry>= GetRecoShowerArrayN()) {
        Log(2,"EdbShowRec::PrintRecoShowerArray","Requested entry %d is greater than entries in the array. Print full array.",entry);
        entry=-1;
    }

    EdbShowerP* show=0;
    for (Int_t i=0; i<GetRecoShowerArrayN(); ++i) {
        if (entry!=i && entry>=0) continue;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        show->PrintNice();
        show->PrintSegments();
    }

    Log(2,"EdbShowRec::PrintRecoShowerArray","EdbShowRec::PrintRecoShowerArray...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::SetOutNames()
{
    Log(2,"EdbShowRec::SetOutNames","EdbShowRec::SetOutNames");

    int       AlgParasetNr=eActualAlg->GetAlgValue();
    TString   AlgName=eActualAlg->GetAlgName();

    // Check if more than one alg/paraset is present:
    if (GetShowAlgArrayN()==1) {
        SetShowName("shower");
        SetTreebranchName("treebranch");
    }
    else {
        SetShowName(TString(Form("shower_%s_%d", (const char*)eActualAlg->GetAlgName(),eActualAlgParameterset[AlgParasetNr])));
        SetTreebranchName(TString(Form("treebranch_%s_%d", (const char*)eActualAlg->GetAlgName(),eActualAlgParameterset[AlgParasetNr])));
    }
    cout << "EdbShowRec::SetOutNames() "<< GetShowName() << endl;
    cout << "EdbShowRec::SetOutNames() "<< GetTreebranchName() << endl;

    Log(2,"EdbShowRec::SetOutNames","EdbShowRec::SetOutNames...done.");
    return;
}


//______________________________________________________________________________



void EdbShowRec::AddAlg(Int_t AlgType, Float_t* par)
{
    Log(2,"EdbShowRec::AddAlg","EdbShowRec::AddAlg");

    if (!eShowAlgArray)  {
        Create_eShowAlgArray();
    }

    cout << "CT, CL, NN are not yet implemented "<<endl;
// switch command gives compiler errors.... change to simple if statements:

    if (AlgType==0) {
        cout << "= = = = = CT not yet implemented = = = = = "<<endl;
        return;
    }; // CT
    if (AlgType==1) {
        cout << "= = = = = CL not yet implemented = = = = = "<<endl;
        return;
    }; // CL
    if (AlgType==2) {
        // C_one A_dvanced  algorithm
        EdbShowAlg_CA* StandardAlg = new EdbShowAlg_CA();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };
    if (AlgType==3) {
        // N_eural N_et  algorithm
        cout << "= = = = = NN not yet implemented = = = = = "<<endl;
        return;
    }; // NN
    if (AlgType==4) {
        // O_fficial I_mplementation  algorithm
        EdbShowAlg_OI* StandardAlg = new EdbShowAlg_OI();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };
    if (AlgType==5) {
        // S_??? A_???  algorithm
        EdbShowAlg_SA* StandardAlg = new EdbShowAlg_SA();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };
    if (AlgType==6) {
        // T_rack C_one algorithm
        cout << "= = = = = TC not yet implemented = = = = = "<<endl;
        return;
    }; // TC
    if (AlgType==7) {
        // R_ecursive C_one algorithm
        EdbShowAlg_RC* StandardAlg = new EdbShowAlg_RC();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };
    if (AlgType==8) {
        // B_ack W_ard algorithm
        EdbShowAlg_BW* StandardAlg = new EdbShowAlg_BW();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };
    if (AlgType==10) {
        // G_amma S_earch algorithm
        EdbShowAlg_GS* StandardAlg = new EdbShowAlg_GS();
        eShowAlgArray->Add(StandardAlg);
        StandardAlg->SetParameters(par);
        StandardAlg->Print();
    };

    cout << "==================  AFTER ADDING there is eShowAlgArray->GetEntries() =  " <<  eShowAlgArray->GetEntries() << endl;

    SetShowAlgArrayN(eShowAlgArray->GetEntries());
    ++eShowAlgArraySingleN[AlgType];

    cout << "GetShowAlgArrayN()"<< GetShowAlgArrayN()<< endl;

    if (gEDBDEBUGLEVEL>2) {
        for (Int_t i=0; i<8; ++i) cout << "EdbShowRec::AddAlg   eShowAlgArraySingleN("<<i<<" =eShowAlgArraySingleN["<<i<<"]"<< endl;
    }

    Log(2,"EdbShowRec::AddAlg","EdbShowRec::AddAlg...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::ResetShowAlgArray()
{
    Log(2,"EdbShowRec::ResetShowAlgArray","EdbShowRec::ResetShowAlgArray");
    if (!eShowAlgArray)  {
        Create_eShowAlgArray();
    }

    eShowAlgArray->Clear();
    SetShowAlgArrayN(eShowAlgArray->GetEntries());
    for (Int_t i=0; i<10; ++i) eShowAlgArraySingleN[i]=0;

    Log(2,"EdbShowRec::ResetShowAlgArray","EdbShowRec::ResetShowAlgArray...done.");
}


//______________________________________________________________________________

void EdbShowRec::CheckPlateValues() {

    Log(2,"EdbShowRec::CheckPlateValues()","CheckPlateValues()");

    // Restore from default values to logical ones:
    if (eProposedNumberPlate==-1) eProposedNumberPlate=eAliNpat;
    if (eProposedFirstPlate==-1) eProposedFirstPlate=1;
    if (eProposedMiddlePlate==-1) eProposedMiddlePlate=eAliNpat-3;
    if (eProposedLastPlate==-1) eProposedLastPlate=eAliNpat;

    // eAli->Print();
    /*
    cout << "eFirstPlate= "  << eFirstPlate << endl;
    cout << "eProposedFirstPlate= "  << eProposedFirstPlate << endl;
    cout << "eFirstPlate_eAliPID= "  << eFirstPlate_eAliPID << endl;
    cout<<endl;
    cout << "eLastPlate= "  << eLastPlate << endl;
    cout << "eProposedLastPlate= "  << eProposedLastPlate << endl;
    cout << "eLastPlate_eAliPID= "  << eLastPlate_eAliPID << endl;
    cout<<endl;
    cout << "eMiddlePlate= "  << eMiddlePlate << endl;
    cout << "eProposedMiddlePlate= "  << eProposedMiddlePlate << endl;
    cout << "eMiddlePlate_eAliPID= "  << eMiddlePlate_eAliPID << endl;
    cout<<endl;
    cout << "eAliNpat= "  << eAliNpat << endl;
    */

    if (eProposedFirstPlate<1 || eProposedFirstPlate>eAliNpat-2 || eProposedFirstPlate==eFirstPlate) {
        // Do nothing!
        Log(2,"EdbShowRec::CheckPlateValues()","Do nothing for  eProposedFirstPlate.");
    }
    else {
        SetFirstPlate(eProposedFirstPlate);
        Log(2,"EdbShowRec::CheckPlateValues","SetFirstPlate() = %d", eProposedFirstPlate);
    }

    if (eProposedMiddlePlate<eFirstPlate || eProposedMiddlePlate>eAliNpat-2 || eProposedMiddlePlate==eMiddlePlate) {
        // Do nothing!
        Log(2,"EdbShowRec::CheckPlateValues()","Do nothing for  eProposedMiddlePlate.");
    }
    else {
        SetMiddlePlate(eProposedMiddlePlate);
        Log(2,"EdbShowRec::CheckPlateValues","SetMiddlePlate() = %d", eProposedMiddlePlate);
    }

    if (eProposedLastPlate<=2 || eProposedLastPlate>=eAliNpat || eProposedLastPlate==eLastPlate ) {
        // Do nothing!
        Log(2,"EdbShowRec::CheckPlateValues()","Do nothing for  eProposedLastPlate.");
    }
    else {
        SetLastPlate(eProposedLastPlate);
        Log(2,"EdbShowRec::CheckPlateValues","SetLastPlate() = %d", eProposedLastPlate);
    }

    if (eProposedNumberPlate>eAliNpat || eProposedNumberPlate<=1 || eProposedNumberPlate==eNumberPlate )  {
        // Do nothing!
        Log(2,"EdbShowRec::CheckPlateValues()","Do nothing for  eProposedNumberPlate.");
    }
    else {
        SetNumberPlate(eProposedNumberPlate);
        Log(2,"EdbShowRec::CheckPlateValues","SetNumberPlate() = %d", eProposedNumberPlate);
    }

    SetFirstPlate_eAliPID(eFirstPlate, eAliStreamTypeStep);
    SetLastPlate_eAliPID(eLastPlate, eAliStreamTypeStep);
    SetMiddlePlate_eAliPID(eMiddlePlate, eAliStreamTypeStep);
    SetNumberPlate_eAliPID(eNumberPlate, eAliStreamTypeStep);

    Log(2,"EdbShowRec::CheckPlateValues()","eFirstPlate           =  %02d ",eFirstPlate);
    Log(2,"EdbShowRec::CheckPlateValues()","eFirstPlate_eAliPID   =  %02d ",eFirstPlate_eAliPID);
    Log(2,"EdbShowRec::CheckPlateValues()","eLastPlate            =  %02d ",eLastPlate);
    Log(2,"EdbShowRec::CheckPlateValues()","eLastPlate_eAliPID    =  %02d ",eLastPlate_eAliPID);
    Log(2,"EdbShowRec::CheckPlateValues()","eMiddlePlate          =  %02d ",eMiddlePlate);
    Log(2,"EdbShowRec::CheckPlateValues()","eMiddlePlate_eAliPID  =  %02d ",eMiddlePlate_eAliPID);
    Log(2,"EdbShowRec::CheckPlateValues()","eNumberPlate          =  %02d ",eNumberPlate);
    Log(2,"EdbShowRec::CheckPlateValues()","eNumberPlate_eAliPID  =  %02d ",eNumberPlate_eAliPID);

    Log(2,"EdbShowRec::CheckPlateValues()","CheckPlateValues()...done.");
    return;
}

//______________________________________________________________________________

void EdbShowRec::PrintMore() {
    return;
}

//______________________________________________________________________________

void EdbShowRec::PrintAll()
{
    return;
}

//______________________________________________________________________________

void EdbShowRec::BuildParametrizations()
{
    Log(2,"EdbShowRec::BuildParametrizations()","BuildParametrizations");

    if (eRecoShowerArrayN==0) {
        cout << "EdbShowRec::BuildParametrizations   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    EdbShowerP* show=0;
    int maxParametrizationType=6;
    Bool_t doMakePara=kFALSE;

    Log(2,"EdbShowRec::BuildParametrizations()","Will build the following parametrizations:");
    for ( int j=0; j<maxParametrizationType; j++ ) {
        if (eParaTypes[j]==1) {
            doMakePara=kTRUE;
            cout << "Shower parametrization j = " << eParaTypes[j] << " name: " << eParaNames[j].Data() << endl;
        }
    }

    if (doMakePara==kFALSE) {
        Log(2,"EdbShowRec::BuildParametrizations()","No parametrization set to be done!");
        Log(2,"EdbShowRec::BuildParametrizations()","BuildParametrizations...done.");
        return;
    }

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (gEDBDEBUGLEVEL>3) cout << "EdbShowRec::BuildParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << ")"<< endl;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>3) show->PrintNice();
        for ( int j=0; j<maxParametrizationType; j++ ) {
            if (eParaTypes[j]==1) show->BuildParametrisation(j);
        }
    }

    Log(2,"EdbShowRec::BuildParametrizations()","BuildParametrizations...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::BuildParametrizationsAll()
{
    cout << "NOT YET IMPLEMENTED...."<<endl;
    cout << "use .... EdbShowRec::BuildParametrizations()   instead ...."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowRec::SetFirstPlate_eAliPID(Int_t FP, Int_t StreamTypeStep)
{
    if (StreamTypeStep==1) {
        eFirstPlate_eAliPID = eAliNpat-FP;
    }
    if (StreamTypeStep==-1) {
        eFirstPlate_eAliPID = FP-1;
    }
    return;
}

//______________________________________________________________________________

void EdbShowRec::SetMiddlePlate_eAliPID(Int_t Plate, Int_t StreamTypeStep)
{
    if (StreamTypeStep==1) {
        eMiddlePlate_eAliPID = eAliNpat-Plate;
    }
    if (StreamTypeStep==-1) {
        eMiddlePlate_eAliPID = Plate-1;
    }
    return;
}

//______________________________________________________________________________

void EdbShowRec::SetLastPlate_eAliPID(Int_t Plate, Int_t StreamTypeStep)
{
    if (StreamTypeStep==1) {
        eLastPlate_eAliPID = eAliNpat-Plate;
    }
    if (StreamTypeStep==-1) {
        eLastPlate_eAliPID = Plate-1;
    }
    return;
}


//______________________________________________________________________________

void EdbShowRec::SetNumberPlate_eAliPID(Int_t Plate, Int_t StreamTypeStep)
{
    eNumberPlate_eAliPID = eAliNpat;
    return;
}

//______________________________________________________________________________

void EdbShowRec::SetSimpleFileName(Int_t type, Int_t dotype)
{
    Log(2,"EdbShowRec::SetSimpleFileName()","SetSimpleFileName");
    //type = 0: treebranch
    //type = 1: showers
    if (type==0) if (dotype==1) eFilename_Out_treebranch="treebranch.root";
    if (type==1) if (dotype==1) eFilename_Out_shower="Showers.root";
    // "Showers.root" was chosen not to conflict with the output file
    // of the old libShower library, which gave "Shower.root" as output.

    cout << "EdbShowRec::SetSimpleFileName()  eFilename_Out_treebranch=  " << eFilename_Out_treebranch << "   eFilename_Out_shower = " << eFilename_Out_shower <<  endl;
    Log(2,"EdbShowRec::SetSimpleFileName()","SetSimpleFileName...done.");
    return;
}


//______________________________________________________________________________

void EdbShowRec::SetDoParaType(Int_t type)
{
    Log(2,"EdbShowRec::SetDoParaType()","SetDoParaType");
    // Set internal array values for which parametrisation shall be calculated:
    //type = 0: FJ
    //type = 1: LT
    //type = 2: YC
    //type = 3: JC
    //type = 4: XX
    //type = 5: YY
    //type = 6: PP  // to be implemented ???  !!!!!!  ???
    //type = 7: AS  // to be implemented ???  !!!!!!  ???
    //type = 8: SE  // to be implemented ???  !!!!!!  ???

    int maxParametrizationType=6; // max parametrizations available
    if (type>=0 && type<maxParametrizationType) eParaTypes[type]=1;
    Log(2,"EdbShowRec::SetDoParaType()","SetDoParaType...done.");
    return;
}

//______________________________________________________________________________


void EdbShowRec::SetDoParaType(TString typestring)
{
    if (typestring=="FJ") SetDoParaType(0);
    if (typestring=="LT") SetDoParaType(1);
    if (typestring=="YC") SetDoParaType(2);
    if (typestring=="JC") SetDoParaType(3);
    if (typestring=="XX") SetDoParaType(4);
    if (typestring=="YY") SetDoParaType(5);
    if (typestring=="PP") SetDoParaType(6); // To be implemented ???
    if (typestring=="AS") SetDoParaType(7); // To be implemented ???
    if (typestring=="SE") SetDoParaType(8); // To be implemented ???
    return;
}

//______________________________________________________________________________

void EdbShowRec::SetUseAliSub(Int_t type)
{
    if (type==0) eUseAliSub=kFALSE;
    if (type==1) eUseAliSub=kTRUE;
    cout << "EdbShowRec::SetUseAliSub   eUseAliSub = " << type << " set to " <<  eUseAliSub << endl;
    cout << "EdbShowRec::SetUseAliSub   WARNING! The use of the eAliSub volume is only recommended "<<endl;
    cout << "EdbShowRec::SetUseAliSub   for _FEW_ Initiator Basetracks, since the usage of  eAliSub volume "<<endl;
    cout << "EdbShowRec::SetUseAliSub   is not memory safe. "<<endl;
    cout << "SEE THE COMMMENT IN THE CODE OF THE FUNCTIOn " << endl;
    cout <<  "void EdbShowAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)  " << endl;
    cout << "FOR THE REASON............  TO  BE  FIXED   " << endl;
    cout << "For the moment, we will just set back to use full volume in reco:" << endl;

    // for the moment, we will just set back to use full volume in reco:
    eUseAliSub=kFALSE;
    cout << "EdbShowRec::SetUseAliSub   eUseAliSub = " << type << " set to " <<  eUseAliSub << endl;
    return;
}


//______________________________________________________________________________

bool EdbShowRec::FileExists(string strFilename) {

    std::ifstream LogFile(strFilename.c_str());
    if (!LogFile) {
        std::cout << "File not found." << std::endl;
        return 0;
    }
    // Code for this function copied verbatim from
    // http://www.techbytes.ca/techbyte103.html
    /// This code does not compile anymore with
    /// gcc version 4.3.5 (Ubuntu 4.3.5-3ubuntu1)
    /// Therefore I skipped it.
    return 1;
}

//______________________________________________________________________________


void EdbShowRec::WriteParametrisation_FJ() {

    Log(2,"EdbShowRec::WriteParametrisation_FJ()","WriteParametrisation_FJ()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_FJ   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    // Steps for creating the file:
    // Because in root we have error messagens "Failed filling branch" we have to:
    // First step into the file and then create the Tree:

    Log(2,"EdbShowRec::WriteParametrisation_FJ()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_FJ()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_FJ()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_FJ()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_FJ";
    Float_t ShowerAxisAngle;
    Float_t BT_deltaR_mean,BT_deltaR_rms,BT_deltaT_mean,BT_deltaT_rms;
    Int_t nseg;
    Int_t longprofile[57];

    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("BT_deltaR_mean",&BT_deltaR_mean,"BT_deltaR_mean/F");
    treevar->Branch("BT_deltaR_rms",&BT_deltaR_rms,"BT_deltaR_rms/F");
    treevar->Branch("BT_deltaT_mean",&BT_deltaT_mean,"BT_deltaT_mean/F");
    treevar->Branch("BT_deltaT_rms",&BT_deltaT_rms,"BT_deltaT_rms/F");
    treevar->Branch("longprofile",longprofile,"longprofile[57]/I");

    EdbShowerP* show=0;
    EdbShowerP::Para_FJ para_FJ; // IMPORTANT HINWEIS !!!  this was original version,
    // but the EdbShowerP Object cannot be stored in a root file (crahses somehow..)
    // we try to put the parametrisation into a seperate class and see if this works ....
    // ... Solved now: Error was not due to struct problem, but to an empty, uninitialized
    // pointer (eReco_ID_Array)

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(0); // FJ==0
    cout << "EdbShowRec::WriteParametrisation_FJ()   show->GetParametrisationIsDone(0): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_FJ()   Start Building BuildParametrisation(0);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        // Get Shower:
        show=(EdbShowerP*)eRecoShowerArray->At(i);
//     show->GetSegment(0)->PrintNice();
//     gEDBDEBUGLEVEL=3;
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(0) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation(0);

        // Print it:
        if (gEDBDEBUGLEVEL>3)  show->PrintParametrisation_FJ();

        // Get it:
        para_FJ=show->GetPara_FJ();


        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_FJ.ShowerAxisAngle;
        nseg=para_FJ.nseg;
        BT_deltaR_mean=para_FJ.BT_deltaR_mean;
        BT_deltaR_rms=para_FJ.BT_deltaR_rms;
        BT_deltaT_mean=para_FJ.BT_deltaT_mean;
        BT_deltaT_rms=para_FJ.BT_deltaT_rms;
        for (int k=0; k<57; k++) longprofile[k]=(Int_t)para_FJ.longprofile[k];

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }
    //------------------------------------------------------------------

    Log(2,"EdbShowRec::WriteParametrisation_FJ()","Writing tree for para_FJ parametrisation into file...");
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_FJ()","Writing tree for para_FJ parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_FJ()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_FJ()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_FJ()","WriteParametrisation_FJ()...done.");
    return;
}




//______________________________________________________________________________


void EdbShowRec::WriteParametrisation_YC() {

    Log(2,"EdbShowRec::WriteParametrisation_YC()","WriteParametrisation_YC()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_YC   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    // Steps for creating the file:
    // Because in root we have error messagens "Failed filling branch" we have to:
    // First stepp into the file and then create the Tree:
    Log(2,"EdbShowRec::WriteParametrisation_YC()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_YC()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_YC()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_YC()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_YC";
    Float_t ShowerAxisAngle,C1,a1,alpha,nmax;
    Int_t nseg;
    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("C1",&C1,"C1/F");
    treevar->Branch("a1",&a1,"a1/F");
    treevar->Branch("alpha",&alpha,"alpha/F");
    treevar->Branch("nmax",&nmax,"nmax/F");
    treevar->Branch("nseg",&nseg,"nseg/I");

    EdbShowerP* show=0;
    EdbShowerP::Para_YC para_YC;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(2); // YC==2
    cout << "EdbShowRec::WriteParametrisation_YC()   show->GetParametrisationIsDone(2): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_YC()   Start Building BuildParametrisation(2);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(2) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation(2);

        // Print Parametrisation:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_YC();

        // Get it:
        para_YC=show->GetPara_YC();

        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_YC.ShowerAxisAngle;
        C1=para_YC.C1;
        a1=para_YC.a1;
        alpha=para_YC.alpha;
        nmax=para_YC.nmax;
        nseg=para_YC.nseg;

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }


    Log(2,"EdbShowRec::WriteParametrisation_YC()","Writing tree for para_YC parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_YC()","Writing tree for para_YC parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_YC()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_YC()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_YC()","WriteParametrisation_YC()...done.");
    return;
}
//______________________________________________________________________________




//______________________________________________________________________________


void EdbShowRec::WriteParametrisation_JC() {

    Log(2,"EdbShowRec::WriteParametrisation_JC()","WriteParametrisation_JC()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_JC   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    // Steps for creating the file:
    // Because in root we have error messagens "Failed filling branch" we have to:
    // First step into the file and then create the Tree:
    Log(2,"EdbShowRec::WriteParametrisation_JC()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_JC()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_JC()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_JC()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_JC";

    // Variables part from Para_YC:
    Float_t ShowerAxisAngle,C1,a1,alpha,nmax;
    Int_t nseg;
    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    // Branches part from Para_YC:
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("C1",&C1,"C1/F");
    treevar->Branch("a1",&a1,"a1/F");
    treevar->Branch("alpha",&alpha,"alpha/F");
    treevar->Branch("nmax",&nmax,"nmax/F");
    treevar->Branch("nseg",&nseg,"nseg/I");

    // Variables part from Para_FJ:
    Float_t BT_deltaR_mean,BT_deltaR_rms,BT_deltaT_mean,BT_deltaT_rms;
    Int_t longprofile[57];
    // Branches part from Para_FJ:
    treevar->Branch("BT_deltaR_mean",&BT_deltaR_mean,"BT_deltaR_mean/F");
    treevar->Branch("BT_deltaR_rms",&BT_deltaR_rms,"BT_deltaR_rms/F");
    treevar->Branch("BT_deltaT_mean",&BT_deltaT_mean,"BT_deltaT_mean/F");
    treevar->Branch("BT_deltaT_rms",&BT_deltaT_rms,"BT_deltaT_rms/F");
    treevar->Branch("longprofile",longprofile,"longprofile[57]/I");

    EdbShowerP* show=0;
    EdbShowerP::Para_JC para_JC;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(3); // JC==3
    cout << "EdbShowRec::WriteParametrisation_JC()   show->GetParametrisationIsDone(3): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_JC()   Start Building BuildParametrisation(3);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(3) << endl;

        // Do parametrisation if necessary:
        // BuildParametrisation(3) (JC) automatically invokes BuildParametrisation(FJ) and BuildParametrisation(YC)
        if (!ToDoPara) show->BuildParametrisation(3);

        // Print Parametrisation:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_JC();

        // Get it:
        para_JC=show->GetPara_JC();

        // Fill tree with values from parametrisation:
        ShowerAxisAngle=para_JC.ShowerAxisAngle;
        // YC-Part:
        C1=para_JC.C1;
        a1=para_JC.a1;
        alpha=para_JC.alpha;
        nmax=para_JC.nmax;
        nseg=para_JC.nseg;
        // FJ-Part:
        BT_deltaR_mean=para_JC.BT_deltaR_mean;
        BT_deltaR_rms=para_JC.BT_deltaR_rms;
        BT_deltaT_mean=para_JC.BT_deltaT_mean;
        BT_deltaT_rms=para_JC.BT_deltaT_rms;
        for (int k=0; k<57; k++) longprofile[k]=(Int_t)para_JC.longprofile[k];

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }


    Log(2,"EdbShowRec::WriteParametrisation_JC()","Writing tree for para_JC parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_JC()","Writing tree for para_JC parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_JC()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_JC()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_JC()","WriteParametrisation_JC()...done.");
    return;
}
//______________________________________________________________________________



void EdbShowRec::WriteParametrisation_LT() {

    Log(2,"EdbShowRec::WriteParametrisation_LT()","WriteParametrisation_LT()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_LT   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_LT()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_LT()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_LT()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_LT()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_LT";

    Float_t ShowerAxisAngle;
    Float_t BT_deltaR_mean,BT_deltaR_rms,BT_deltaT_mean,BT_deltaT_rms;
    Int_t nseg;
    Int_t longprofile[57];
    Int_t transprofile[8];

    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("BT_deltaR_mean",&BT_deltaR_mean,"BT_deltaR_mean/F");
    treevar->Branch("BT_deltaR_rms",&BT_deltaR_rms,"BT_deltaR_rms/F");
    treevar->Branch("BT_deltaT_mean",&BT_deltaT_mean,"BT_deltaT_mean/F");
    treevar->Branch("BT_deltaT_rms",&BT_deltaT_rms,"BT_deltaT_rms/F");
    treevar->Branch("longprofile",longprofile,"longprofile[57]/I");
    treevar->Branch("transprofile",transprofile,"transprofile[8]/I");

    EdbShowerP* show=0;
    EdbShowerP::Para_LT para_LT;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(1); // LT==1
    cout << "EdbShowRec::WriteParametrisation_LT()   show->GetParametrisationIsDone(1): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_LT()   Start Building BuildParametrisation(1);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(1) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation(1);

        // Print it:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_LT();

        // Get it:
        para_LT=show->GetPara_LT();

        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_LT.ShowerAxisAngle;
        nseg=para_LT.nseg;
        BT_deltaR_mean=para_LT.BT_deltaR_mean;
        BT_deltaR_rms=para_LT.BT_deltaR_rms;
        BT_deltaT_mean=para_LT.BT_deltaT_mean;
        BT_deltaT_rms=para_LT.BT_deltaT_rms;
        for (int k=0; k<57; k++ ) longprofile[k]=(Int_t)para_LT.longprofile[k];
        for (int k=0; k<8; k++ ) transprofile[k]=(Int_t)para_LT.transprofile[k];


        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )

    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }


    Log(2,"EdbShowRec::WriteParametrisation_LT()","Writing tree for para_LT parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_LT()","Writing tree for para_LT parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_LT()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_LT()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_LT()","WriteParametrisation_LT()...done.");
    return;
}
//______________________________________________________________________________







//______________________________________________________________________________
void EdbShowRec::WriteParametrisation_XX() {

    Log(2,"EdbShowRec::WriteParametrisation_XX()","WriteParametrisation_XX()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_XX   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_XX()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_XX()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_XX()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_XX()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_XX";

    Int_t nseg;
    Float_t ShowerAxisAngle;
    Float_t  Mean_ProfileLongitudinal,RMS_ProfileLongitudinal,Max_ProfileLongitudinal;
    Float_t  Mean_ProfileTransversal,RMS_ProfileTransversal,Max_ProfileTransversal;
    Float_t   Last_ProfileLongitudinal, Last_ProfileTransversal;

    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("Mean_ProfileLongitudinal",&Mean_ProfileLongitudinal,"Mean_ProfileLongitudinal/F");
    treevar->Branch("RMS_ProfileLongitudinal",&RMS_ProfileLongitudinal,"RMS_ProfileLongitudinal/F");
    treevar->Branch("Max_ProfileLongitudinal",&Max_ProfileLongitudinal,"Max_ProfileLongitudinal/F");
    treevar->Branch("Last_ProfileLongitudinal",&Last_ProfileLongitudinal,"Last_ProfileLongitudinal/F");
    treevar->Branch("Mean_ProfileTransversal",&Mean_ProfileTransversal,"Mean_ProfileTransversal/F");
    treevar->Branch("RMS_ProfileTransversal",&RMS_ProfileTransversal,"RMS_ProfileTransversal/F");
    treevar->Branch("Max_ProfileTransversal",&Max_ProfileTransversal,"Max_ProfileTransversal/F");
    treevar->Branch("Last_ProfileTransversal",&Last_ProfileTransversal,"Last_ProfileTransversal/F");

    EdbShowerP* show=0;
    EdbShowerP::Para_XX para_XX;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(4); // XX==4
    cout << "EdbShowRec::WriteParametrisation_XX()   show->GetParametrisationIsDone(4): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_XX()   Start Building BuildParametrisation(4);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(4) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation(4);

        // Print it:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_XX();

        // Get it:
        para_XX=show->GetPara_XX();

        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_XX.ShowerAxisAngle;
        nseg=para_XX.nseg;
        Mean_ProfileLongitudinal=para_XX.Mean_ProfileLongitudinal;
        RMS_ProfileLongitudinal=para_XX.RMS_ProfileLongitudinal;
        Max_ProfileLongitudinal=para_XX.Max_ProfileLongitudinal;
        Last_ProfileLongitudinal=para_XX.Last_ProfileLongitudinal;
        Mean_ProfileTransversal=para_XX.Mean_ProfileTransversal;
        RMS_ProfileTransversal=para_XX.RMS_ProfileTransversal;
        Max_ProfileTransversal=para_XX.Max_ProfileTransversal;
        Last_ProfileTransversal=para_XX.Last_ProfileTransversal;

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }

    Log(2,"EdbShowRec::WriteParametrisation_XX()","Writing tree for para_XX parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_XX()","Writing tree for para_XX parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_XX()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_XX()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_XX()","WriteParametrisation_XX()...done.");
    return;
}
//______________________________________________________________________________






//______________________________________________________________________________
void EdbShowRec::WriteParametrisation_YY() {

    Log(2,"EdbShowRec::WriteParametrisation_YY()","WriteParametrisation_YY()");

    /// DEBUG -------------------------------------------------------------------------------
    cout << "DEBUG =================== DEBUG ===============  DEBUG ================="<<endl;
    /// DEBUG
    /// NOT FINISHED YET !!!!

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_YY   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_YY()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_YY()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_YY()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_YY()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_YY";

    // Variables used for YY parametrisation
    Float_t ShowerAxisAngle;
    Int_t     nseg;
    Int_t     ProfileLongitudinalBincontent[57];
    Int_t     ProfileTransversalBincontent[8];


    TTree* treevar=0;
    cout << "After creating the  TTree* treevar=0;  pointer:  treevar= " << treevar << endl;
    if (!treevar) {
        cout << "Creating new Tree with NameTitle: " << targettreename << endl;
        treevar = new TTree(targettreename,targettreename);
    }
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("ProfileLongitudinalBincontent",ProfileLongitudinalBincontent,"ProfileLongitudinalBincontent[57]/I");
    treevar->Branch("ProfileTransversalBincontent",ProfileTransversalBincontent,"ProfileTransversalBincontent[8]/I");

    ///=================
// cout << "   IRGENDAWS IST HIER FALSCH MIT BEM BRACH FILLING !!!  SEEMS TO BE SOLVED SINCE ARRAY FILLING WAS WRONG! "<< endl;
    ///=================


    // temporary method to store MC information...
    // we probably try to extent it to an own structure:
    // See method and structure: BuildParametrizationsMCInfo

    EdbShowerP* show=0;
    EdbShowerP::Para_YY para_YY;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(5); // YY==5
    //cout << "EdbShowRec::WriteParametrisation_YY()   show->GetParametrisationIsDone(5): " << ToDoPara << endl;
    if (!ToDoPara) {
        //cout << "EdbShowRec::WriteParametrisation_YY()   Start Building BuildParametrisation(5);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(5) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) {
            show->BuildParametrisation(5);
        }

        // Print it:
        if (gEDBDEBUGLEVEL>2) show->PrintParametrisation_YY();

        // Get it:
        para_YY=show->GetPara_YY();


        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_YY.ShowerAxisAngle;
        nseg=para_YY.nseg;
        for (int k=0; k<57; k++) ProfileLongitudinalBincontent[k]=para_YY.ProfileLongitudinalBincontent[k];
        for (int k=0; k<8; k++) ProfileTransversalBincontent[k]=para_YY.ProfileTransversalBincontent[k];

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);

    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }



    Log(2,"EdbShowRec::WriteParametrisation_YY()","Writing tree for para_YY parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_YY()","Writing tree for para_YY parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_YY()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_YY()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...

// 	cout << "EdbShowRec::WriteParametrisation_YY()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;

    Log(2,"EdbShowRec::WriteParametrisation_YY()","WriteParametrisation_YY()...done.");
    return;
}
//______________________________________________________________________________








//______________________________________________________________________________
void EdbShowRec::WriteParametrisation_AS() {

    Log(2,"EdbShowRec::WriteParametrisation_AS()","WriteParametrisation_AS()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_AS   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_AS()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_AS()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_AS()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_AS()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_AS";


    Int_t nseg;
    Double_t edIP[50];
    Double_t edMin[50];
    Double_t edRLong[50];
    Double_t edRTrans[50];
    Double_t edeltarb[50];
    Double_t edeltathetab[50];

    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("edIP",edIP,"edIP[50]/D");
    treevar->Branch("edMin",edMin,"edMin[50]/D");
    treevar->Branch("edRLong",edRLong,"edRLong[50]/D");
    treevar->Branch("edRTrans",edRTrans,"edRTrans[50]/D");
    treevar->Branch("edeltarb",edeltarb,"edeltarb[50]/D");
    treevar->Branch("edeltathetab",edeltathetab,"edeltathetab[50]/D");

    EdbShowerP* show=0;
    EdbShowerP::Para_AS para_AS;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(7); // AS==7
    cout << "EdbShowRec::WriteParametrisation_AS()   show->GetParametrisationIsDone(7): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_AS()   Start Building BuildParametrisation(7);" << endl;
    }

//   return;
//
    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        //if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(7) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) {
            show->BuildParametrisation(7);
            //cout << "show->GetParametrisationIsDone(7) = " << show->GetParametrisationIsDone(7)  << endl;
        }

        // Print it:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_AS();

        // Get it:
        para_AS=show->GetPara_AS();

        //Fill tree with values from parametrisation:
        nseg=para_AS.nseg;
        for (int j=0; j<50; j++) {
            edIP[j]=para_AS.edIP[j];
            edMin[j]=para_AS.edMin[j];
            edRLong[j]=para_AS.edRLong[j];
            edRTrans[j]=para_AS.edRTrans[j];
            edeltarb[j]=para_AS.edeltarb[j];
            edeltathetab[j]=para_AS.edeltathetab[j];
        }

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }

    Log(2,"EdbShowRec::WriteParametrisation_AS()","Writing tree for para_AS parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_AS()","Writing tree for para_AS parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_AS()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_AS()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_AS()","WriteParametrisation_AS()...done.");
    return;
}
//______________________________________________________________________________




//______________________________________________________________________________
void EdbShowRec::WriteParametrisation_SE() {

    Log(2,"EdbShowRec::WriteParametrisation_SE()","WriteParametrisation_SE()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_SE   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_SE()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_SE()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_SE()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_SE()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_SE";

    Int_t nseg,npl;
    Float_t ShowerAxisAngle,efficiencyAtShowerAxisAngle;

    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("ShowerAxisAngle",&ShowerAxisAngle,"ShowerAxisAngle/F");
    treevar->Branch("nseg",&nseg,"nseg/I");
    treevar->Branch("npl",&npl,"npl/I");
    treevar->Branch("efficiencyAtShowerAxisAngle",&efficiencyAtShowerAxisAngle,"efficiencyAtShowerAxisAngle/F");

    EdbShowerP* show=0;
    EdbShowerP::Para_SE para_SE;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= show->GetParametrisationIsDone(8); // SE==8
    cout << "EdbShowRec::WriteParametrisation_SE()   show->GetParametrisationIsDone(8): " << ToDoPara << endl;
    if (!ToDoPara) {
        cout << "EdbShowRec::WriteParametrisation_SE()   Start Building BuildParametrisation(8);" << endl;
    }

    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). BuildParametrization IsDone?"<< show->GetParametrisationIsDone(8) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation(8);

        // Print it:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_SE();

        // Get it:
        para_SE=show->GetPara_SE();

        //Fill tree with values from parametrisation:
        ShowerAxisAngle=para_SE.ShowerAxisAngle;
        efficiencyAtShowerAxisAngle=para_SE.efficiencyAtShowerAxisAngle;
        nseg=para_SE.nseg;
        npl=para_SE.npl;

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }

    Log(2,"EdbShowRec::WriteParametrisation_SE()","Writing tree for para_SE parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_SE()","Writing tree for para_SE parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_SE()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_SE()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_SE()","WriteParametrisation_SE()...done.");
    return;
}
//______________________________________________________________________________




//______________________________________________________________________________
void EdbShowRec::WriteParametrisation_ExtraInfo() {

    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","WriteParametrisation_ExtraInfo()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_ExtraInfo   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_ExtraInfo()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

// Go into file (if necessary)
    eFile_Out_treebranch->cd();

    TString targettreename="Para_ExtraInfo";
    Float_t   InBT_E;	// InBT->P()
    Float_t   InBT_Flag;	// InBT->Flag()
    Float_t   InPairBT_E;	// In0BT->P()+In1BT->P() if sum of Flag() ==0 (photon=electron+positron); else identical to InBT_E
    Int_t	    ShowerLength; // npl() of the shower.
    Float_t	  InBT_IPToVtx; // IP from InBT to generic vertex. Commonly used: Vertex of G4 position vertex.
    Float_t	  InBT_DeltaZToVtx; // DeltaZ from InBT to generic vertex. Commonly used: Vertex of G4 position vertex.


    TTree* treevar=0;
    if (!treevar) treevar = new TTree(targettreename,targettreename);
    treevar->Branch("InBT_E",&InBT_E,"InBT_E/F");
    treevar->Branch("InBT_Flag",&InBT_Flag,"InBT_Flag/F");
    treevar->Branch("InPairBT_E",&InPairBT_E,"InPairBT_E/F");
    treevar->Branch("ShowerLength",&ShowerLength,"ShowerLength/I");
    treevar->Branch("InBT_IPToVtx",&InBT_IPToVtx,"InBT_IPToVtx/F");
    treevar->Branch("InBT_DeltaZToVtx",&InBT_DeltaZToVtx,"InBT_DeltaZToVtx/F");

    EdbShowerP* show=0;
    EdbShowerP::Para_ExtraInfo para_ExtraInfo;

    // Check for first shower if desired parametrisation was already done:
    show=(EdbShowerP*)eRecoShowerArray->At(0);
    Bool_t ToDoPara= kFALSE;
    Int_t statuscounter=eRecoShowerArrayN/10;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {
        if (i%statuscounter==0) cout << "EdbShowRec::WriteParametrizations   10% more done." << endl;

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << "). GetExtraInfoIsDone IsDone?"<< show->GetExtraInfoIsDone(0) << endl;

        // Do parametrisation if necessary:
        if (!ToDoPara) show->BuildParametrisation_ExtraInfo();

        // Print it:
        if (gEDBDEBUGLEVEL>3) show->PrintParametrisation_ExtraInfo();

        // Get it:
        para_ExtraInfo=show->GetPara_ExtraInfo();


        InBT_E=para_ExtraInfo.InBT_E;
        InBT_Flag=para_ExtraInfo.InBT_Flag;
        ShowerLength=para_ExtraInfo.ShowerLength;

        InBT_IPToVtx=para_ExtraInfo.InBT_IPToVtx;
        InBT_DeltaZToVtx=para_ExtraInfo.InBT_DeltaZToVtx;

        treevar->Fill();
        if (gEDBDEBUGLEVEL>3) treevar->Show(treevar->GetEntries()-1);
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }

    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Writing tree for para_ExtraInfo parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Writing tree for para_ExtraInfo parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_ExtraInfo()","WriteParametrisation_ExtraInfo()...done.");
    return;
}
//______________________________________________________________________________







//______________________________________________________________________________
void EdbShowRec::BuildParametrizationsMCInfo(TString MCInfoFilename, Int_t type) {

    Log(2,"EdbShowRec::BuildParametrizationsMCInfo()","BuildParametrizationsMCInfo()");
    cout << "BuildParametrizationsMCInfo::MCInfoFilename = " << MCInfoFilename << endl;
    cout << "BuildParametrizationsMCInfo::type = " << type << endl;

    // First Check if showers are stored, otherwise it makes no sense to give
    // them the event monte carlo information:
    if (eRecoShowerArrayN==0) {
        cout << "BuildParametrizationsMCInfo::eRecoShowerArrayN = " << eRecoShowerArrayN << ". So BuildParametrizationsMCInfo() for showers makes no sense. Return."<< endl;
        return;
    }

    if (type==0) BuildParametrizationsMCInfo_PGun(  MCInfoFilename);
    if (type==1) BuildParametrizationsMCInfo_Event( MCInfoFilename);

    return;
}

//______________________________________________________________________________

void EdbShowRec::BuildParametrizationsMCInfo_PGun(TString MCInfoFilename) {

    Log(2,"EdbShowRec::BuildParametrizationsMCInfo_PGun()","BuildParametrizationsMCInfo_PGun()");

    //Declare Tree Variables
    Int_t MCEvt, PDGId;
    Float_t energy, tantheta,dirx,diry,dirz,vtxposx,vtxposy,vtxposz;
    Float_t TX,TY,Y,X,Z;

    // Check File Existence:
    // FileExists(TString&) doesnt work yet...
    /*  if (!FileExists(MCInfoFilename)) {
        cout << "BuildParametrizationsMCInfo_PGun: FileExists(MCInfoFilename)=0: File doesnt seem to exist!"<<endl;
        return;
      }*/

    // Read Tree with File:
    TTree* PGunTree = new TTree();
    PGunTree->ReadFile(MCInfoFilename,"MCEvt/I:energy/F:tantheta/F:dirx/F:diry/F:dirz/F:vtxposx/F:vtxposy/F:vtxposz/F:TX/F:TY/F:X/F:Y/F:Z/F:PDGId/I");
    PGunTree->SetBranchAddress("MCEvt",&MCEvt);
    PGunTree->SetBranchAddress("PDGId",&PDGId);
    PGunTree->SetBranchAddress("energy",&energy);
    PGunTree->SetBranchAddress("tantheta",&tantheta);
    PGunTree->SetBranchAddress("dirx",&dirx);
    PGunTree->SetBranchAddress("diry",&diry);
    PGunTree->SetBranchAddress("dirz",&dirz);
    PGunTree->SetBranchAddress("vtxposx",&vtxposx);
    PGunTree->SetBranchAddress("vtxposy",&vtxposy);
    PGunTree->SetBranchAddress("vtxposz",&vtxposz);
    PGunTree->SetBranchAddress("TX",&TX);
    PGunTree->SetBranchAddress("TY",&TY);
    PGunTree->SetBranchAddress("X",&X);
    PGunTree->SetBranchAddress("Z",&Z);
    PGunTree->SetBranchAddress("Y",&Y);
    if (gEDBDEBUGLEVEL>2) PGunTree->Print();

// 	PGunTree->Show(0);
// 	cout << "PGunTree->GetEntries();    " <<  PGunTree->GetEntries() <<  endl;

    // 69999 // why this number???
    Int_t PGunTreeEntry_MCEvt_Correspondance[69999]; // PGunTreeEntry_MCEvt_Correspondance[0]=TreeEntry(0)->MC()
    for (Int_t i=0; i<PGunTree->GetEntries(); ++i) {
        PGunTree->GetEntry(i);
        PGunTreeEntry_MCEvt_Correspondance[MCEvt]=i;
    }

    if (gEDBDEBUGLEVEL<3) {
        cout << "VERY IMPORTANT:" << endl;
        cout << "To have a 1:1 correspondance we have to store the MC para for EACH shower MCEvt number." << endl;
        cout << "If for example three showers are there which have the same MCEvtNumber but MCInfo is written" << endl;
        cout << "only once, then there will be a shift and a MISMATCHING OCCURS and all the data will be useless!" << endl;
    }

    if (eRecoShowerArrayN<1) {
        cout << "EdbShowRec::BuildParametrizationsMCInfo_PGun():   NO SHOWERS IN eRecoShowerArray!! return." << endl;
        return;
    }


    // If showers are stored, get them and fill the values according to PGunTree  MCEvent number:
    for (Int_t i=0; i<eRecoShowerArrayN; ++i) {
        EdbShowerP* shower = (EdbShowerP*) eRecoShowerArray->At(i);
        // Get right corresponding Treeentry:
        int nMCEvt=shower->MCEvt();
        //cout << " // Get right corresponding Treeentry:   nMCEvt=shower->MCEvt();   " <<  nMCEvt<< endl;
        //cout << " // Get right corresponding Treeentry:  PGunTreeEntry_MCEvt_Correspondance[nMCEvt]   " <<  PGunTreeEntry_MCEvt_Correspondance[nMCEvt] << endl;
        PGunTree->GetEntry(PGunTreeEntry_MCEvt_Correspondance[nMCEvt]);
        // Show it:
        if (gEDBDEBUGLEVEL>3) PGunTree->Show(PGunTreeEntry_MCEvt_Correspondance[nMCEvt]);
        // Get Parametrisation:
        EdbShowerP::MCInfo_PGun fMCInfo_PGun = shower->GetMCInfo_PGun();
        // Set Values:
        fMCInfo_PGun.MCEvt=nMCEvt;
        fMCInfo_PGun.PDGId=PDGId;  // not Set up to now...one has to set it manually....///tODO
        fMCInfo_PGun.energy=energy;
        fMCInfo_PGun.tantheta=tantheta;
        fMCInfo_PGun.dirx=dirx;
        fMCInfo_PGun.diry=diry;
        fMCInfo_PGun.dirz=dirz;
        fMCInfo_PGun.vtxposx=vtxposx;
        fMCInfo_PGun.vtxposy=vtxposy;
        fMCInfo_PGun.vtxposz=vtxposz;
        fMCInfo_PGun.TX=TX;
        fMCInfo_PGun.TY=TY;
        fMCInfo_PGun.X=X;
        fMCInfo_PGun.Y=Y;
        fMCInfo_PGun.Z=Z;
        shower->SetMCInfo_PGun(fMCInfo_PGun);

        // Finally, tell the shower that MCInfo_PGun  was done.
        shower->SetMCInfoIsDone(0,kTRUE);

        if (gEDBDEBUGLEVEL>3) shower->PrintMCInfo_PGun();
    }


    /// PGunTree still in memory... toDO: delete after Tree written to file, make Private variable? .... ????

    return;
}

//______________________________________________________________________________

void EdbShowRec::BuildParametrizationsMCInfo_Event(TString MCInfoFilename) {

    Log(2,"EdbShowRec::BuildParametrizationsMCInfo_Event()","BuildParametrizationsMCInfo_Event()");
    Log(2,"EdbShowRec::BuildParametrizationsMCInfo_Event()","BuildParametrizationsMCInfo_Event()....NOT YET IMPLEMENTED.");

    return;
}

//______________________________________________________________________________

void EdbShowRec::ExtendParametrisation_ExtraInfo()
{
    Log(2,"EdbShowRec::ExtendParametrisation_ExtraInfo()","ExtendParametrisation_ExtraInfo()");

    if (eRecoShowerArrayN<1) return;

    EdbVertex* vtx= new EdbVertex();
    vtx->SetXYZ(0,0,0);

    // If showers are stored, get them and fill the values:
    for (Int_t i=0; i<eRecoShowerArrayN; ++i) {
//   for (Int_t i=0; i<3; ++i) {
        EdbShowerP* shower = (EdbShowerP*) eRecoShowerArray->At(i);
        EdbSegP* seg =shower->GetSegmentFirst();

        if (shower->GetMCInfoIsDone(0) == kTRUE ) {
            cout << "shower->SetMCInfoIsDone() == kTRUE" << endl;
        }
        if (shower->GetMCInfoIsDone(0) == kFALSE ) {
            cout << "shower->SetMCInfoIsDone() == kFALSE" << endl;
            return;
        }

        // Only in case the MCInfoIsDone we can continue and calculate the IP:
        EdbShowerP::MCInfo_PGun  fMCInfo_PGun;
        fMCInfo_PGun=shower->GetMCInfo_PGun();

        Float_t Y=fMCInfo_PGun.Y;
        Float_t X=fMCInfo_PGun.X;
        Float_t Z=fMCInfo_PGun.Z;

        cout << "X,Y,Z" << X << " " <<Y << "  " << Z << endl;
        seg->PrintNice();

        vtx->SetXYZ(X,Y,Z);
        cout << "  EdbEDAUtil::CalcIP(EdbSegP* s, EdbVertex* v)  " <<  endl;
        cout << EdbEDAUtil::CalcIP(seg,vtx) << endl;

        shower->SetParaVariable(EdbEDAUtil::CalcIP(seg,vtx),4,0);
        shower->SetParaVariable(TMath::Abs(seg->Z()-vtx->Z()),5,0);
        shower->PrintParametrisation_ExtraInfo();
    }
    return;
}




//______________________________________________________________________________

void EdbShowRec::WriteParametrisation_MCInfo_PGun()
{
    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","WriteParametrisation_MCInfo_PGun()");

    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::WriteParametrisation_MCInfo_PGun   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }

    /// Check for first shower if desired parametrisation was already done: do it before at beginning of function!!!

    TString targettreename="MCInfo_PGun";

    //Declare Tree Variables
    Int_t MCEvt, PDGId;
    Float_t energy, tantheta,dirx,diry,dirz,vtxposx,vtxposy,vtxposz;
    Float_t TX,TY,Y,X,Z;


    TTree* treevar=0;
    cout << "After creating the  TTree* treevar=0;  pointer:  treevar= " << treevar << endl;
    if (!treevar) {
        cout << "Creating new Tree with NameTitle: " << targettreename << endl;
        treevar = new TTree(targettreename,targettreename);
    }
    treevar->Branch("MCEvt",&MCEvt,"MCEvt/I");
    treevar->Branch("PDGId",&PDGId,"PDGId/I");
    treevar->Branch("energy",&energy,"energy/F");
    treevar->Branch("tantheta",&tantheta,"tantheta/F");
    treevar->Branch("dirx",&dirx,"dirx/F");
    treevar->Branch("diry",&diry,"diry/F");
    treevar->Branch("dirz",&dirz,"dirz/F");
    treevar->Branch("vtxposx",&vtxposx,"vtxposx/F");
    treevar->Branch("vtxposy",&vtxposy,"vtxposy/F");
    treevar->Branch("vtxposz",&vtxposz,"vtxposz/F");
    treevar->Branch("TX",&TX,"TX/F");
    treevar->Branch("TY",&TY,"TY/F");
    treevar->Branch("X",&X,"X/F");
    treevar->Branch("Y",&Y,"Y/F");
    treevar->Branch("Z",&Z,"Z/F");

    EdbShowerP* show=0;
    EdbShowerP::MCInfo_PGun fMCInfo_PGun;

    for (Int_t i=0; i<eRecoShowerArrayN; ++i ) {

        show=(EdbShowerP*)eRecoShowerArray->At(i);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowRec::WriteParametrizations   i= " << i <<  " of (" << eRecoShowerArrayN << ")." << endl;

        // Get it:
        fMCInfo_PGun=show->GetMCInfo_PGun();

        //Fill tree with values from parametrisation:
        MCEvt=fMCInfo_PGun.MCEvt;
        PDGId=fMCInfo_PGun.PDGId;
        energy=fMCInfo_PGun.energy;
        tantheta=fMCInfo_PGun.tantheta;
        dirx=fMCInfo_PGun.dirx;
        diry=fMCInfo_PGun.diry;
        dirz=fMCInfo_PGun.dirz;
        vtxposx=fMCInfo_PGun.vtxposx;
        vtxposy=fMCInfo_PGun.vtxposy;
        vtxposz=fMCInfo_PGun.vtxposz;
        X=fMCInfo_PGun.X;
        Y=fMCInfo_PGun.Y;
        Z=fMCInfo_PGun.Z;
        TX=fMCInfo_PGun.TX;
        TY=fMCInfo_PGun.TY;

        treevar->Fill();
    } //for(Int_t i=0; i<eRecoShowerArrayN; ++i )


    if (gEDBDEBUGLEVEL>3) {
        treevar->Print();
    }
    treevar->Show(treevar->GetEntries()-1);

    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Writing Tree in  eFilename_Out_treebranch  = ");
    cout << "eFile_Out_treebranch = " << eFile_Out_treebranch << endl;
    cout << "eFilename_Out_treebranch = " << eFilename_Out_treebranch << endl;

    if (!eFile_Out_treebranch) {
        Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Creating new  eFile_Out_treebranch.");
        eFile_Out_treebranch= new TFile(eFilename_Out_treebranch,"UPDATE");
    }
    else {
        Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Open existing  eFile_Out_treebranch.");
        cout << "EdbShowRec::WriteParametrisation_MCInfo_PGun()   Is File  eFile_Out_treebranch  already open? :" << eFile_Out_treebranch->IsOpen() << endl;
        if (!eFile_Out_treebranch->IsOpen()) eFile_Out_treebranch->Open(eFilename_Out_treebranch,"UPDATE");
    }

    // Go into file (if necessary)
    eFile_Out_treebranch->cd();

    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Writing tree for MCInfo_PGun parametrisation into file...");
//   treevar->Write();
    treevar->Write("",TObject::kWriteDelete);
    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Writing tree for MCInfo_PGun parametrisation into file...done.");

    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Delete treevar...");
    delete treevar;
    treevar=0;
    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","Delete treevar...done.");

//   eFile_Out_treebranch->Close(); // gives crash... dont know why, some tree has to be still connected...
    Log(2,"EdbShowRec::WriteParametrisation_MCInfo_PGun()","WriteParametrisation_MCInfo_PGun()...done.");
    return;
}


//______________________________________________________________________________



//______________________________________________________________________________
void EdbShowRec::PrintParametrisation(Int_t ParaNr)
{
    Log(2,"EdbShowRec::PrintParametrisation()","PrintParametrisation().");
    if (eRecoShowerArrayN==0 || eRecoShowerArray==NULL) {
        cout << "EdbShowRec::PrintParametrisation   WARNING ::  eRecoShowerArrayN = 0.    Return!" << endl;
        return;
    }
    for (Int_t i=0; i<eRecoShowerArrayN; ++i) {
        EdbShowerP* shower=(EdbShowerP*)eRecoShowerArray->At(i);
        if (shower->GetParametrisationIsDone(ParaNr)==kFALSE) continue;
        shower->PrintParametrisation(ParaNr);
    }
    Log(2,"EdbShowRec::PrintParametrisation()","PrintParametrisation()...done.");
    return;
}







//______________________________________________________________________________
void EdbShowRec::AddEdbPVRec( EdbPVRec* Ali )
{
    Log(2,"EdbShowRec::AddEdbPVRec( EdbPVRec* Ali )","WARNING    More than ONE EdbPVRec is NOT supported! Do nothing.");
    return;
}

//______________________________________________________________________________
void EdbShowRec::AddInBT( EdbSegP* seg )
{
    // cout << " AddInBT(EdbSegP* seg): Add Segment (" <<  seg  << ") to eInBTArray:" <<endl;
    if (NULL==eInBTArray) {
        cout << " AddRecoShowerArray(): NULL==eInBTArray. Create it first!" <<endl;
        eInBTArray=new TObjArray();
        SetInBTArrayN(0);
    }
    // Now add it:
    eInBTArray->Add(seg);
    ++eInBTArrayN;
    return;
}

//______________________________________________________________________________
void EdbShowRec::AddInBTArray( EdbSegP* seg )
{
    AddInBT( seg );
    return;
}

//______________________________________________________________________________
void EdbShowRec::AddInBTArray( EdbPattern* pattern )
{
    // Loop over all segments of the pattern, and add its segments
    Log(2,"EdbShowRec::AddInBTArray( EdbPattern* pattern )","Add pattern (ID=%d)", pattern->ID());

    cout << " pattern->ID() = " <<  pattern->ID() << "     pattern->Z() = " <<  pattern->Z() << endl;
    // check if pattern is not NULL object
    if (NULL == pattern) {
        Log(2,"EdbShowRec::AddInBTArray( EdbPattern* pattern )","WARNING! Requested pattern is NULL object. Dont add. Return.");
        return;
    }

//     cout << "Loop over all segments of the pattern, and add its segments" << endl;
    Int_t patN = pattern->N();

//     patN=1;
//     cout << "DEBUG DEBUG DEBUG    Add only first segment!" << endl;

    for (Int_t i=0; i<patN; ++i) {
        EdbSegP* seg = pattern->GetSegment(i);
        // And if not every seg is whished to be added: check for
        if (gRandom->Uniform()>eInBTArrayFraction) continue;
        AddInBT(seg);
    }

    //cout << " AddInBTArray(): After adding, we have now " << GetInBTArrayN() << " entries." << endl;
    Log(2,"EdbShowRec::AddInBTArray( EdbPattern* pattern )","AddInBTArray()...done.");
    return;
}

//______________________________________________________________________________
void EdbShowRec::AddInBTArray( EdbPVRec* volume )
{
    // Loop over all patterns of the volume, and add its segments
    Log(2,"EdbShowRec::AddInBTArray( EdbPVRec* volume )","Add all patterns of the volume...");

    // check if volume is not NULL object
    if (NULL == volume) {
        Log(2,"EdbShowRec::AddInBTArray( EdbPVRec* volume )","WARNING! Requested volume is NULL object. Dont add. Return.");
        return;
    }

    Int_t volumeN = volume->Npatterns();
    for (Int_t i=0; i<volumeN; ++i) {
        EdbPattern* pat = volume->GetPattern(i);
        AddInBTArray(pat);
    }
    Log(2,"EdbShowRec::AddInBTArray( EdbPVRec* volume )","After adding all patterns, we have now total InBTs:", GetInBTArrayN() );
    Log(2,"EdbShowRec::AddInBTArray( EdbPVRec* volume )","Add all patterns of the volume...done.");
    return;
}


//______________________________________________________________________________
void EdbShowRec::AddInBTArray( TObjArray* InBTArray )
{
    Log(2,"EdbShowRec::AddInBTArray( TObjArray* InBTArray )","AddInBTArray()...");

    if (NULL==eInBTArray) {
        cout << " AddRecoShowerArray(): NULL==eInBTArray. Create it first!" <<endl;
        eInBTArray=new TObjArray();
        SetInBTArrayN(0);
    }

    // check if volume is not NULL object
    if (NULL == InBTArray) {
        Log(2,"EdbShowRec::AddInBTArray( TObjArray* InBTArray )","WARNING! Requested TObjArray is NULL object. Dont add. Return.");
        return;
    }


    // Now add it:
    // There is NO check yet if the InBTArray has segments in it, or if they are from the type "EdbSegP"
    // To be done a check....
    for (Int_t i=0; i<InBTArray->GetEntries(); ++i) {
        EdbSegP* seg=(EdbSegP*)InBTArray->At(i);
        // Here it needs to check if that object is really a EdbSegP object...
        // And if not every seg is whished to be added: check for
        if (gRandom->Uniform()>eInBTArrayFraction) continue;
        eInBTArray->Add(seg);
    }
    SetInBTArrayN(eInBTArray->GetEntries());
    cout << " AddInBTArray(): After adding, we have now " << GetInBTArrayN() << " entries." << endl;
    eInBTArray->Print();
    Log(2,"EdbShowRec::AddInBTArray( TObjArray* InBTArray )","AddInBTArray()...done");
    return;
}


//______________________________________________________________________________
void EdbShowRec::AddShowAlgArray( TObjArray* ShowAlgArray ) {
    cout << " AddShowAlgArray(): Add ShowAlgArray Array to eShowAlgArray:" <<endl;
    if (NULL==eShowAlgArray) {
        cout << " AddRecoShowerArray(): NULL==eRecoShowerArray. Create it forst!" <<endl;
        eShowAlgArray=new TObjArray();
        SetShowAlgArrayN(0);
    }
    // Now add it:
    // There is NO check yet if the ShowAlgArray has algs in it!
    // To be done a check....
    for (Int_t i=0; i<ShowAlgArray->GetEntries(); ++i) {
        EdbShowAlg* alg=(EdbShowAlg*)ShowAlgArray->At(i);
        // Here it needs to check if that object is really a EdbShowAlg object...
        eShowAlgArray->Add(alg);
    }
    SetShowAlgArrayN(eShowAlgArray->GetEntries());
    cout << " AddShowAlgArray(): After adding, we have now " << GetShowAlgArrayN() << " entries." << endl;
    eShowAlgArray->Print();
    return;
}
//______________________________________________________________________________
void EdbShowRec::AddRecoShowerArray( TObjArray* RecoShowerArray)      {
    cout << " AddRecoShowerArray(): Add RecoShower Array to eRecoShowerarray:" <<endl;
    if (NULL==eRecoShowerArray) {
        cout << " AddRecoShowerArray(): NULL==eRecoShowerArray. Create it forst!" <<endl;
        eRecoShowerArray=new TObjArray();
        SetRecoShowerArrayN(0);
    }
    // Now add it:
    // There is NO check yet if the RecoShowerArray has showers in it!
    // To be done a check....
    for (Int_t i=0; i<RecoShowerArray->GetEntries(); ++i) {
        EdbShowerP* shower=(EdbShowerP*)RecoShowerArray->At(i);
        // Here it needs to check if that object is really a EdbShowerP object...
        eRecoShowerArray->Add(shower);
    }
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    cout << " AddRecoShowerArray(): After adding, we have now " << GetRecoShowerArrayN() << "entries." << endl;
    eRecoShowerArray->Print();
    return;
}
//______________________________________________________________________________


//______________________________________________________________________________

void EdbShowRec::Help()
{
    cout << "---------------------------------------------------------" <<
         endl;
    cout << "EdbShowRec::Help()" << endl << endl;
    cout << "  What is this?" << endl;
    cout << "        MAIN object of libShowRec." << endl;
    cout << "        Manages (hopefully) all stuff that is related to showers." << endl;
    cout << endl;
    cout << "  What can we do with this?" << endl;
    cout << "        Primary purpose: Shower:" << endl;
    cout << "        ....Reconstruction" << endl;
    cout << "        ....Identification" << endl;
    cout << "        ....Energy" << endl;
    cout << "        ....Electron/Photon/Pi0 reconstruction" << endl;
    cout << endl;
    cout << "   Technical details, information about the class:" << endl;
    cout << "        ...." << endl;
    cout << "        Assuming your data is in an EdbPVRec object (gAli) , then do:" << endl;
    cout << "        EdbShowRec* showrec = new EdbShowRec(gAli); " << endl;
    cout << "        ...." << endl;
    cout << "        And see what happens." << endl;
    cout << "        You can also write specific parameters /and/or cuts in the default.par" << endl;
    cout << "        (Note that every par value has to have the prefix SHOW_ )." << endl;
    cout << "        ...." << endl;
    cout << endl;
    cout << "---------------------------------------------------------" <<
         endl;
    return;
}
