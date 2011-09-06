#include "EdbEDA.h"
#include "EdbEDAShowerTab.h"

extern EdbEDA *gEDA;

ClassImp(EdbEDAShowerTab)

void EdbEDAShowerTab::MakeGUI() {
//   gSystem->Load("libEve");
    TEveBrowser* browser = gEve->GetBrowser();
    browser->StartEmbedding(TRootBrowser::kBottom);

    TGMainFrame* frame = new TGMainFrame(gClient->GetRoot());
    frame->SetWindowName("XX GUI");
    frame->SetCleanup(kDeepCleanup);
    frame->SetLayoutBroken(kTRUE);

    int posy=10;
    int posx=10;
    int dx;
    int posx_start=posx;
    int posy_start=posy;
    int posx_after_vline=posx;

    int posx_1,posx_2,posx_3,posx_4;

    //------------------------------------------------------------------
    TGTextButton *eButton1 = new TGTextButton(frame,"RECO");
    eButton1->MoveResize(posx,posy,dx=60,20);
    eButton1->Connect("Clicked()","EdbEDAShowerTab", this,"Reco()");

    posx+=dx+5;
    TGTextButton *eButtonHelp = new TGTextButton(frame,"HELP");
    eButtonHelp->MoveResize(posx,posy,dx=60,20);
    eButtonHelp->Connect("Clicked()","EdbEDAShowerTab", this,"Help()");

    posx+=dx+5;
    TGVertical3DLine* eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,55);
    posx+=dx+5;
    posx_1=posx;

    //------------------------------------------------------------------
    eCheckButtonUseTSData = new TGCheckButton(frame,"UseTS");
    eCheckButtonUseTSData->SetState(kButtonDown);
    eCheckButtonUseTSData->MoveResize(posx,posy,dx=80,20);

    posx+=dx+5;
    eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,60);
    posx+=dx+5;
    posx_2=posx;

    //------------------------------------------------------------------
    eCheckButtonDrawShowers = new TGCheckButton(frame,"Draw");
    eCheckButtonDrawShowers->SetState(kButtonDown);
    eCheckButtonDrawShowers->MoveResize(posx,posy,dx=50,20);
    eCheckButtonDrawShowers->Connect("Toggled(bool)","EdbEDAShowerTab", this,"DrawShowers()");

    posx+=dx+5;
//   TGLabel *eLabel1 = new TGLabel(frame,"Nr= ");
//   eLabel1->MoveResize(posx,posy,dx=40,20);
    TGTextButton *eButton = new TGTextButton(frame,"Plot");
    eButton->MoveResize(posx,posy,dx=40,20);
    eButton->Connect("Clicked()","EdbEDAShowerTab", this,"PlotShower()");

    posx+=dx+5;
    eTGNumberEntryDrawShowerNr = new TGNumberEntry(frame,0,11,-1,(TGNumberFormat::EStyle) 5);
    eTGNumberEntryDrawShowerNr->MoveResize(posx,posy,dx=40,20);

    posx+=dx+5;
    eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,60);
    posx+=dx+5;
    posx_3=posx;

    //------------------------------------------------------------------
    TGTextButton *eButton4 = new TGTextButton(frame,"Chk PVR");
    eButton4->MoveResize(posx,posy,dx=60,20);
    eButton4->Connect("Clicked()","EdbEDAShowerTab", this,"CheckPVRec()");

    posx+=dx+5;
    TGTextButton *eButton5 = new TGTextButton(frame,"Set PVR");
    eButton5->MoveResize(posx,posy,dx=60,20);
    eButton5->Connect("Clicked()","EdbEDAShowerTab", this,"SetPVRecFromFile()");

    posx+=dx+5;
    eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,55);
    posx+=dx+5;
    posx_4=posx;

    //------------------------------------------------------------------
    TGTextButton* fb = new TGTextButton(frame,"PairSearch");
    fb->MoveResize(posx,posy,dx=60,20);
//   fb->Connect("Clicked()","EdbEDAShowerTab", this,"FindPairingsNewAlgo()"); // uses algo _GS from libShower... // should be included but doesnt work at the moment
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"FindPairings()"); // uses algo coded in here

    posx+=dx+5;
    eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,55);
    posx+=dx+5;

    //--------------------------------------------------------
    // Parameters of the Reconstruction itsself:
    // "Expert Buttons...."
    // WHAT A PITTY, I AM NOT ALLOWED TO PLACE THESE BUTTONS HERE!!!

    posx+=dx+5;
    eVertLine1 = new TGVertical3DLine(frame);
    eVertLine1->MoveResize(posx,posy,dx=5,55);
    posx_after_vline=posx;

    posx+=dx+5;
    eComboBox_Parameter = new TGComboBox(frame,"Parameters");
    eComboBox_Parameter->AddEntry("ConeRadius",0);
    eComboBox_Parameter->AddEntry("ConeAngle",1);
    eComboBox_Parameter->AddEntry("ConnectionDR",2);
    eComboBox_Parameter->AddEntry("ConnectionDT",3);
    eComboBox_Parameter->AddEntry("Nhole",4);
    eComboBox_Parameter->Select(-1);
    eComboBox_Parameter->MoveResize(posx,posy,dx=80,20);
    frame->AddFrame(eComboBox_Parameter);
    eComboBox_Parameter->Connect("Selected(const char *)", "EdbEDAShowerTab", this, "ReadComboBoxParameter()");

    posx+=dx+5;
    eNumberEntry_ParaValue = new TGNumberEntry(frame, (Double_t) 0,5,-1,(TGNumberFormat::EStyle) 5);
    frame->AddFrame(eNumberEntry_ParaValue, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
    eNumberEntry_ParaValue->MoveResize(posx,posy,dx=50,20);

    posx+=dx+5;
    TGTextButton* eButton3 = new TGTextButton(frame,"Change");
    eButton3->MoveResize(posx,posy,dx=45,20);
    eButton3->Connect("Clicked()","EdbEDAShowerTab", this,"ChangeShowerRecParameters()");

    // -------------------------------------------------------------------------------------------
    // Second line in the tab:
    posx=posx_start;
    posy=posy_start+30;

    eButton3 = new TGTextButton(frame,"PRINT");
    eButton3->MoveResize(posx,posy,dx=60,20);
    eButton3->Connect("Clicked()","EdbEDAShowerTab", this,"PrintShower()");

    posx+=dx+5;
    TGTextButton *eButtonGO = new TGTextButton(frame,"GO!");
    eButtonGO->MoveResize(posx,posy,dx=60,20);
    eButtonGO->Connect("Clicked()","EdbEDAShowerTab", this,"GO()");

    posx=posx_1;
    eCheckButtonCheckQualitycut = new TGCheckButton(frame,"");
    eCheckButtonCheckQualitycut->SetState(kButtonDown);
    eCheckButtonCheckQualitycut->MoveResize(posx,posy,dx=20,20);
    eCheckButtonCheckQualitycut->Connect("Toggled(bool)","EdbEDAShowerTab", this,"CheckQualitycut()");

		posx+=dx+5;
		TGTextButton *eButtonQualCutSettings = new TGTextButton(frame,"Quality");
    eButtonQualCutSettings->MoveResize(posx,posy,dx=55,20);
    eButtonQualCutSettings->Connect("Clicked()","EdbEDAShowerTab", this,"MakeParameterWindowQualitySettings()");


    //------------------------------------------------------------------
    posx=posx_2;
    fb = new TGTextButton(frame,"Prev");
    fb->MoveResize(posx,posy,dx=40,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"DrawShowerPrevious()");

    posx+=dx+5;
    fb = new TGTextButton(frame,"All");
    fb->MoveResize(posx,posy,dx=40,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"DrawShowerAll()");

    posx+=dx+5;
    fb = new TGTextButton(frame,"Next");
    fb->MoveResize(posx,posy,dx=40,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"DrawShowerNext()");

    //------------------------------------------------------------------
    posx=posx_3;
    fb = new TGTextButton(frame,"ChkBTDens");
    fb->MoveResize(posx,posy,dx=60,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"CheckBTDensityCanv()");

    posx+=dx+5;
    fb = new TGTextButton(frame,"CkCosRec");
    fb->MoveResize(posx,posy,dx=60,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"CheckCosmicReco()");

    //------------------------------------------------------------------
    posx=posx_4;
    fb = new TGTextButton(frame,"Pi0Search");
    fb->MoveResize(posx,posy,dx=60,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"FindPairingsNewAlgo()");


    //------------------------------------------------------------------
    // WHAT A PITTY, I AM NOT ALLOWED TO PLACE THESE BUTTONS HERE!!!
    posx=posx_after_vline+10;
    eButton3 = new TGTextButton(frame,"Reset");
    eButton3->MoveResize(posx,posy,dx=40,20);
    eButton3->Connect("Clicked()","EdbEDAShowerTab", this,"ResetShowerRecParameters()");

    posx+=dx+5;
    eButton3 = new TGTextButton(frame,"Info");
    eButton3->MoveResize(posx,posy,dx=40,20);
    eButton3->Connect("Clicked()","EdbEDAShowerTab", this,"PrintShowerRecParameters()");

    posx+=dx+5;
    fb = new TGTextButton(frame,"HighNBT");
    fb->MoveResize(posx,posy,dx=40,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"SetParaSetHighNBT()");

    posx+=dx+5;
    fb = new TGTextButton(frame,"HighPur");
    fb->MoveResize(posx,posy,dx=40,20);
    fb->Connect("Clicked()","EdbEDAShowerTab", this,"SetParaSetHighPur()");




    //------------------------------------------------------------------
    frame->MapSubwindows();
    frame->Resize();
    frame->MapWindow();

    browser->StopEmbedding();
    browser->SetTabTitle("Shower", 2);
}



//-------------------------------------------------------------------------------------------


EdbEDAShowerTab::EdbEDAShowerTab() {

    // Create ShowerRec Object
    eShowerRec = new EdbShowerRec();

    // Create Initiator BT array:
    eInBTArray=new TObjArray();

    eIsAliLoaded=kFALSE;  // this means EdbPVrec object using ALL cp.roor Basetrack info!
    eUseTSData=kTRUE;
    eDrawShowers=kTRUE;
    eCheckQualitycut=kTRUE;
		eQualityCutSetting=0;

    RecoShowerArray= new TObjArray();

    // Two Pointers for the EdbPVRec object:
    pvrec_linkedtracks=0;
    pvrec_cpfiles=0;
    pvrec_generic=0;


		// Reset actual shower:
		eShower=0;

		// Make the tab:
    MakeGUI();

}


//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::SetPVRecFromFile() {

    Log(2, "EdbEDAShowerTab", "--- SetPVRecFromFile()");
    TFile* file = new TFile("File_EdbPVRec.root");
    pvrec_cpfiles = (EdbPVRec*)file->Get("EdbPVRec");
    if (NULL==pvrec_cpfiles) return;

    if (pvrec_cpfiles->eTracks == NULL) cout << "EdbEDAShowerTab::SetPVRecFromFile()  WARNING This file has no eTracks! "<<endl;
    if (pvrec_cpfiles->eVTX == NULL) cout << "EdbEDAShowerTab::SetPVRecFromFile()  WARNING This file has no eVTX! "<<endl;

    pvrec_generic=pvrec_cpfiles;
    pvrec_generic->Print();

    eIsAliLoaded=kTRUE;

    // Directly set it for the ShowerRec:
    eShowerRec->SetEdbPVRec(pvrec_generic);

    Log(2, "EdbEDAShowerTab", "--- SetPVRecFromFile()...done.");
}


//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::CheckPVRec() {

    Log(2, "EdbEDAShowerTab", "--- CheckPVRec()");

    cout << " -----    Why we need this?   During the data storage, sometimes the PID of the " << endl;
    cout << " -----    EdbPVRec object is always zero. For example, when using o2root downloaded database data. " << endl;
    cout << " -----    (Not when using simulation data with ORFEO)" << endl;
    cout << " -----    In this case we have to tell the shower library to use EdbPVRec::ID() instead of  EdbPVRec::PID()" << endl;

    if (NULL==pvrec_generic) LoadShowerRecEdbPVRec();

    cout << "EdbEDAShowerTab::CheckPVRec()   pvrec_generic = " << pvrec_generic << endl;

    int npat=pvrec_generic->Npatterns();
    cout << pvrec_generic->Npatterns() << endl;
    Bool_t PIDallSame=kFALSE;

    // Quick check:
    // Check if all are of the same value....
    for (int i=1; i<npat; i++) {
        if (pvrec_generic->GetPattern(i)->PID()==pvrec_generic->GetPattern(i-1)->PID()) {
            PIDallSame=kTRUE;
            break;
        }
    }
    if (PIDallSame) cout << "PIDallSame ... !!! .... ONe should take rather ID() instead of PID()" << endl;
    if (!PIDallSame) cout << "PID are different. One can take PIDs for Reconstruction/eAliSub creation..." << endl;

    ///Log(2, "EdbEDAShowerTab", "--- CheckPVRec()   eShowerRec->SetUsePattern_ID(kTRUE)");
    ///if (PIDallSame) eShowerRec->SetUsePattern_ID(kTRUE);


   // Check PVRec with EdbPVRQualityCut:
//   EdbPVRQuality* qual = new EdbPVRQuality(pvrec_generic);
// cout << "EdbEDAShowerTab::CheckPVRec()   pvrec_generic = " pvrec_generic << endl;



    Log(2, "EdbEDAShowerTab", "--- CheckPVRec()...done.");
    return;
}


//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::LoadShowerRecEdbPVRec() {
    // Either the TS data is already loaded for EDA Object, or we have to reload it
    // for the ShowerRec object:
    Log(2, "EdbEDAShowerTab", "--- LoadShowerRecEdbPVRec()");

    Bool_t UseTSData=eCheckButtonUseTSData->GetState();
    Bool_t found_pvrec_linkedtracks=kFALSE;
    Bool_t found_pvrec_cpfiles=kFALSE;

    if (gEDBDEBUGLEVEL>2) {
        cout << "UseTSData =  " << UseTSData ;
        cout << ", gEDA->GetDataType() =  " << gEDA->GetDataType();
        cout << ",  eShowerRec->GetAliLoaded();   " <<  eShowerRec->GetAliLoaded();
        cout << ",  eIsAliLoaded   " <<  eIsAliLoaded << endl;

        cout << "--- Adresses for the PVrec objects:  pvrec_cpfiles: " << pvrec_cpfiles << "  pvrec_linkedtracks:  " << pvrec_linkedtracks << endl;
        cout << "gEDA->GetDataType() "        <<   gEDA->GetDataType() <<  endl;
        cout << "gEDA->GetTrackSet(TS)->N() " <<   gEDA->GetTrackSet("TS")->N() <<  endl;

        cout << "gEDA->GetTrackSet(TS)->GetPVRec()->Print(); :" << endl;
        gEDA->GetTrackSet("TS")->GetPVRec()->Print();
    }


    if (gEDA->GetDataType()==100 && UseTSData==kTRUE && eIsAliLoaded==kFALSE) {
        Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- Read in EdbPVRec from cp.root files. This may take a while. PLEASE BE PATIENT...");
        eShowerRec->LoadEdbPVRec();
        eIsAliLoaded=kTRUE;
        // This is the EdbPVRec object from cp.root files:
        pvrec_cpfiles=eShowerRec->GetEdbPVRec();
        Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- ...ok. Set: eShowerRec->LoadEdbPVRec() to use cp files for reconstruction");
    }
    else {

        // Check if the PVR for "TS" exists:
        if (gEDA->GetTrackSet("TS")->N()==0) {
            cout << "if (gEDA->GetTrackSet(TS)->N()==0) {"<<endl;
            cout << "now try to get pvrec_linkedtracks from the trackset of the selected track" << endl;
						cout << "gEDA->GetSelectedTrack() = " << gEDA->GetSelectedTrack() << endl;
						if (NULL == gEDA->GetSelectedTrack()) {
							cout << "EdbEDAShowerTab::LoadShowerRecEdbPVRec()   ERROR    No Selected Track!!! " << endl;
							cout << "EdbEDAShowerTab::LoadShowerRecEdbPVRec()   ERROR    Return Now: Check Your TrackSet Data Please!" << endl;
							pvrec_linkedtracks=0;
							pvrec_cpfiles=0;
							pvrec_generic=0;
							return;
						}
            pvrec_linkedtracks=gEDA->GetTrackSet(gEDA->GetSelectedTrack())->GetPVRec();
            cout << "done:   pvrec_linkedtracks=gEDA->GetTrackSet(gEDA->GetSelectedTrack())->GetPVRec();" << endl;
            //gEDA->GetSelectedTrack()->Print();
            gEDA->GetTrackSet(gEDA->GetSelectedTrack())->Print();
            cout <<  gEDA->GetTrackSet(gEDA->GetSelectedTrack())->N() << endl;
        }
        else {
            cout << "Trying to get PVRecObject now: "<<endl;
            pvrec_linkedtracks=gEDA->GetTrackSet("TS")->GetPVRec();
            cout << "pvrec_linkedtracks " <<  pvrec_linkedtracks <<  endl;
            if (pvrec_linkedtracks==NULL) {
                cout << "pvrec_linkedtracks is NULL. Obviously, the line " << endl;
                cout << "pvrec_linkedtracks=gEDA->GetTrackSet(TS)->GetPVRec();"<< endl;
                cout << "did NOT work, try somethign different now:" << endl;
                cout << "pvrec_linkedtracks=gEDA->GetPVR();"<< endl;
                pvrec_linkedtracks=gEDA->GetPVR();
                cout << "pvrec_linkedtracks " <<  pvrec_linkedtracks <<  endl;
            }
        }

        // Check now which have found.
        if (pvrec_linkedtracks!=NULL) found_pvrec_linkedtracks=kTRUE;
        if (pvrec_cpfiles!=NULL) found_pvrec_cpfiles=kTRUE;


        if (UseTSData==kTRUE && found_pvrec_cpfiles) {
            cout << "UseTSData==kTRUE && found_pvrec_cpfiles" << endl;
            Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- Set: eShowerRec->pvrec_cpfiles() to use cp files for reconstruction:");
            eShowerRec->SetEdbPVRec(pvrec_cpfiles);
            pvrec_generic=pvrec_cpfiles;
            Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- Set: eShowerRec->pvrec_cpfiles() to use cp files for reconstruction...done.");
        }
        else {
            cout << "neither UseTSData==kTRUE  nor found_pvrec_cpfiles. take pvrec_linkedtracks !" << endl;
            Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- Set: eShowerRec->pvrec_linkedtracks() to use cp files for reconstruction");
            eShowerRec->SetEdbPVRec(pvrec_linkedtracks);
            pvrec_generic=pvrec_linkedtracks;
            Log(2, "EdbEDAShowerTab::LoadShowerRecEdbPVRec", "--- Set: eShowerRec->pvrec_linkedtracks() to use cp files for reconstruction...don.");
        }


    } // of else  from // (gEDA->GetDataType()==100 && UseTSData==kTRUE && eIsAliLoaded==kFALSE)


    if (gEDBDEBUGLEVEL>2) {
        cout << "Print Once Again the eShowerRec->GetEdbPVRec()->Print();"<<endl;
        eShowerRec->GetEdbPVRec()->Print();
        cout << "--- Adresses for the PVrec objects:  pvrec_cpfiles: " << pvrec_cpfiles << "  pvrec_linkedtracks:  " << pvrec_linkedtracks << endl;
        cout << "--- We take " << eShowerRec->GetEdbPVRec() << " for  pvrec_generic " << endl;
    }

    pvrec_generic=eShowerRec->GetEdbPVRec();


    Log(2, "EdbEDAShowerTab", "--- LoadShowerRecEdbPVRec()...Done.");
    return;
}

void EdbEDAShowerTab::SetInBTFromSelected() {
    new TGMsgBox(gClient->GetRoot(), gEve->GetMainWindow(),"EDA", "Not yet supported! Here will come possibility to set Inititator BaseTracks either from reconstructed track, scanback tracks, BTs attaced to vertex, or similar...");//, 4, 4);
    return;
}

void EdbEDAShowerTab::CheckInBT() {
    if (eShowerRec->GetInBTArrayN()==0) {
        new TGMsgBox(gClient->GetRoot(), gEve->GetMainWindow(),"EDA", "No Initiator Basetrack selected!");//, 4, 4);
        return;
    }
}

void EdbEDAShowerTab::CheckQualitycut() {
    eCheckQualitycut=eCheckButtonCheckQualitycut->GetState();
    cout << "EdbEDAShowerTab::CheckQualitycut() Set To: "<< eCheckQualitycut << endl;
    return;
}


//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::Reco() {
    cout << " void Reco()" << endl;

// Reset eShowerRec Arrays: InBTArray and RecoShowerArray....
    eShowerRec->ResetInBTArray();
    eShowerRec->ResetRecoShowerArray();


    cout << " void Reco()   Clear  gEDA->GetTrackSet(BT); Array..." << endl;
    EdbEDATrackSet *set = gEDA->GetTrackSet("BT");
    set->Clear();

    cout << " void Reco()   Set InBTArray..." << endl;
    cout << gEDA->GetSelected()  <<   " gEDA->GetSelected() " << endl;

    eInBTArray=(TObjArray *) gEDA->GetSelected()->Clone();
    TObjArray* arr=(TObjArray*)eInBTArray->Clone();
//arr->Print();
    eShowerRec->SetInBTArray(arr);

    cout << " void Reco()   Check For CheckInBT..." << endl;
    CheckInBT();


// Recheck if anything is selected:
    if (eShowerRec->GetInBTArrayN()==0) return;

    if (gEDBDEBUGLEVEL>2) cout << " void Reco()   PrintShowerRecInBT..." << endl;
    if (gEDBDEBUGLEVEL>2) PrintShowerRecInBT();

    cout << " void Reco()   Check For gAli..." << endl;
    if (eIsAliLoaded==kFALSE) LoadShowerRecEdbPVRec();

    cout << " void Reco()   CheckPVRec..." << endl;
    CheckPVRec();

    cout << " void Reco()   Check For suited BT density cut if needed..." << endl;
    CheckBTDensity(eShowerRec->GetEdbPVRec(),NULL);

		cout << " void Reco()   eShowerRec->SetUseAliSub(0)..." << endl;
		eShowerRec->SetUseAliSub(0);
		
    cout << " void Reco()   eShowerRec->Execute()..." << endl;
    eShowerRec->Execute();


    if (gEDBDEBUGLEVEL>2) cout << " void Reco()   eShowerRec->PrintRecoShowerArray()..." << endl;
    if (gEDBDEBUGLEVEL>2) eShowerRec->PrintRecoShowerArray();

    eNShowers=eShowerRec->GetRecoShowerArrayN();
    RecoShowerArray=eShowerRec->GetRecoShowerArray();

    if (eDrawShowers) {
        for (Int_t i=0; i<eNShowers; ++i) {
            DrawShower(i);
        }
    }

// Set Factor values back:
    cout << "// Set Factor values back: eShowerRec->SetQualityCutValues(0.15,1.0); " << endl;
    eShowerRec->SetQualityCutValues(0.15,1.0);

    return;
}

//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::ChangeShowerRecParameters() {
    // Read Value from the value field and pass it over to eShowerRec
    Double_t Val =  eNumberEntry_ParaValue->GetNumber();
    Double_t IdVal = eComboBox_Parameter->GetSelectedEntry()->EntryId();
    eShowerRec->SetAlgoParameter(Val,IdVal);
    eShowerRec->PrintParameters();
}

void EdbEDAShowerTab::ReadComboBoxParameter() {
    // Read Name from the Combobox and put the corresponging value in value field:
    cout << "eComboBox_Parameter->GetSelectedEntry->EntryId()" << endl;
    Double_t IdVal = eComboBox_Parameter->GetSelectedEntry()->EntryId();
    cout << "IdVal = " << IdVal << endl;
    Double_t Val = eShowerRec->GetAlgoParameter(IdVal);
    cout << "Val = " << Val << endl;
    SetNumberEntryShowerRecParameter(Val);
}



void EdbEDAShowerTab::SetNumberEntryShowerRecParameter(Double_t val) {
    eNumberEntry_ParaValue->SetNumber(val);
}

void EdbEDAShowerTab::SetParaSetHighNBT() {
    eShowerRec->SetAlgoParameter(1200,0);
    eShowerRec->SetAlgoParameter(0.2,1);
    eShowerRec->SetAlgoParameter(300,2);
    eShowerRec->SetAlgoParameter(0.2,3);
    eShowerRec->PrintParameters();
}

void EdbEDAShowerTab::SetParaSetHighPur() {
    eShowerRec->SetAlgoParameter(600,0);
    eShowerRec->SetAlgoParameter(0.02,1);
    eShowerRec->SetAlgoParameter(200,2);
    eShowerRec->SetAlgoParameter(0.1,3);
    eShowerRec->PrintParameters();
}

//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::DrawShowers() {
    cout << "void EdbEDAShowerTab::DrawShowers()   eCheckButtonDrawShowers->GetState() " << eCheckButtonDrawShowers->GetState()<<endl;
    eDrawShowers=eCheckButtonDrawShowers->GetState();
}

void EdbEDAShowerTab::DrawShower() {
    int nr = eTGNumberEntryDrawShowerNr->GetNumber();
    DrawShower(nr,1);
}

void EdbEDAShowerTab::DrawShowerAll() {
    for (int i=0; i<eNShowers; ++i) DrawShower(i,0);
}

void EdbEDAShowerTab::DrawShowerPrevious() {
    int nr = eTGNumberEntryDrawShowerNr->GetNumber()-1;
    if (nr<0) return;
    DrawShower(nr,1);
    eTGNumberEntryDrawShowerNr->SetNumber(nr);
}

void EdbEDAShowerTab::DrawShowerNext() {
    int nr = eTGNumberEntryDrawShowerNr->GetNumber()+1;
    if (nr>=eNShowers) return;
    DrawShower(nr,1);
    eTGNumberEntryDrawShowerNr->SetNumber(nr);
}

void EdbEDAShowerTab::DrawShower(int iShowerNr,bool eClearShower) {
    cout << "void EdbEDAShowerTab::DrawShower(  " << iShowerNr << endl;

		// Set Actual Shower Array:
    TObjArray *ShowerArray = RecoShowerArray;

		// Set Actual Shower:
		EdbTrackP* Shower = (EdbTrackP*)ShowerArray->At(iShowerNr);
		eShower= Shower;

		// Print It:
		PrintActualShower();

		// Create Segment array for the TrackSet:
    TObjArray *segments = new TObjArray();
    for (int j=0; j<Shower->N(); j++) {
        //cout << "j= " << j << endl;
        EdbSegP *s = Shower->GetSegment(j);
        //s->PrintNice();
        segments->Add( s);
    }

    cout << "EdbEDATrackSet *set = gEDA->GetTrackSet(BT);"<<endl;
    EdbEDATrackSet *set = gEDA->GetTrackSet("BT");
    set->SetExtendMode(kExtendUpDown);
    cout << "set->N(); " << set->N() <<  endl;
    //set->Print();

    // If Checkbox Overlay is Clicked, the old shower will not be erased,
    // the new one will be overlayn!
    if (set->N()>0 && eClearShower) set->Clear();
    cout << "set->Clear();"<<endl;
    set->AddSegments(segments);

    // Draw tracks
    gEDA->Redraw();

    // Draw Shower starting points as blue circles:
    DrawSingleShower( Shower );

    return;
}

void EdbEDAShowerTab::Reset() {

    cout << "-----     void EdbEDAShowerTab::Reset()     -----"<<endl;
    ResetShowerRecParameters();
    EdbEDATrackSet *set = gEDA->GetTrackSet("BT");
    set->Clear();
    gEDA->Redraw();
    return;
}

void EdbEDAShowerTab::PrintShower() {
    /// QUICK and DIRTY solution !!!
    TFile* file=TFile::Open("Shower.root");
    TTree* ShowerTree = (TTree *)file->Get("treebranch");
    ShowerTree->Scan("xb[0]:yb[0]:zb[0]:txb[0]:tyb[0]:sizeb:eProb1:eProb90:Energy:EnergySigma:trackdensb");
    return;
}

void EdbEDAShowerTab::PrintActualShower() {
	if (NULL == eShower) {
		cout << "EdbEDAShowerTab::PrintActualShower()    WARNING Actual Shower: eShower == NULLPOINTER. Do nothing.  " << endl;
		return;
	}
	eShower->PrintNice();
}


void EdbEDAShowerTab::DrawSingleShower(EdbTrackP* shower) {

    Log(3, "EdbEDAShowerTab", "--- void DrawSingleShower()");

    EdbSegP *s = shower->GetSegmentFirst();
    // Draw single vertex.
    TEvePointSet *ps = new TEvePointSet();
    ps->SetElementName(Form("Shower (Id, nseg, X,Y,Z) %3d %d %8.1lf %8.1lf %8.1lf", shower->ID(), shower->N(), s->X(), s->Y(), s->Z()));
    ps->SetMarkerStyle(25);
    ps->SetNextPoint(s->X(), s->Y(), s->Z()*gEDA->GetScaleZ());
    ps->SetMarkerColor(kBlue);
    ps->SetMarkerSize(ps->GetMarkerSize()*1.8);
    ps->SetUserData(shower);
    gEve->AddElement(ps);
    gEve->Redraw3D();
    return;
}

//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::CheckBTDensity(EdbPVRec *pvr, TObjArray *tracks) {
    // Check Basetrack density of totalscan for each plate
    printf("EdbEDAShowerTab::CheckBTDensity...\n");
    if (!eCheckQualitycut) return;

    if (!eIsAliLoaded) LoadShowerRecEdbPVRec();
    // Whole cp files data:
    pvr= eShowerRec->GetEdbPVRec();
    pvr=pvrec_generic;

    //pvr->Print();
    EdbPattern* 	pat = (EdbPattern*)pvr->GetPattern(0);
    Int_t 				patN= pat->N();
    Int_t 				pvrN= pvr->Npatterns()-1;
    EdbSegP* 			seg=NULL;
    Float_t 			BTpermm2;

    TH2F* h_plate = new TH2F("All Plates","h_plate",125,0,125000,100,0,100000);
    TH2F* BTDens = new TH2F("BTDens","BTDens",125,0,125000,100,0,100000);
    TH2F* BTDensity[10];
    TProfile* BTDensProfile  = new TProfile("BTDensityProfile","BTDensityProfile",57,0,57,0,200);
    TProfile* BTDensityProfile[10];
    TH1F* h_BinContentDistribution = new TH1F("h_BinContentDistribution","h_BinContentDistribution",50,0,50);
    TH1F* h_BinCont[58];

    for (int l=0; l<10; ++l) {
        BTDensity[l]=(TH2F*)BTDens->Clone();
        BTDensityProfile[l]=(TProfile*)BTDensProfile->Clone();
        BTDensityProfile[l]->SetLineColor(21+2*l);
    }
    for (int l=0; l<pvrN; ++l) {
        h_BinCont[l]=(TH1F*)h_BinContentDistribution->Clone();
    }

    for (int j=0; j<pvrN; ++j) {
        pat = (EdbPattern*)pvr->GetPattern(j);
        patN= pat->N();
        // Get all Segments of the pattern and Fill Area Histos:
        for (int i=0; i<patN; ++i) {
            seg=(EdbSegP*)pat->GetSegment(i);
            h_plate->Fill(seg->X(),seg->Y());
            // Fill for 4 different Chi2 Cut sets the for histograms
            for (int l=0; l<10; ++l)  {
                float factor=0.15-(float)l*0.01;
                if (seg->Chi2()<seg->W()*factor-1.0) BTDensity[l]->Fill(seg->X(),seg->Y());
            }
        }
        // Get Bin Entries and fill profile Histos:
        for (int ki=1; ki<h_plate->GetNbinsX(); ki++ ) {
            for (int kj=1; kj<h_plate->GetNbinsY(); kj++ ) {
                for (int l=0; l<10; ++l) {
                    BTpermm2=(Float_t)BTDensity[l]->GetBinContent(ki,kj); // ONLY WHEN BINAREA=1000*1000
                    if (BTpermm2==0) continue;
                    BTDensityProfile[l]->Fill(j,BTpermm2,1);
                    if (l==3) h_BinCont[j]->Fill(BTpermm2);
                }
            }
        }
        for (int l=0; l<10; ++l)  {
            BTDensity[l]->Reset();
        }
    }
    // end of pattern loop

    // Estimate Maximum by constant fit (just threshold determination).
    Bool_t warning=kFALSE;
    Int_t last_l=0;
    float factor=0.15-(float)last_l*0.01;
    TString Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);

    for (int l=9; l>=0; --l) {
        if (BTDensityProfile[l]->GetMaximum()<1) continue;
        BTDensityProfile[l]->Fit("pol0","Q0");
        TF1 *myfunc = BTDensityProfile[l]->GetFunction("pol0");
        //cout << 0.15-(float)l*0.01 << " " << myfunc->GetParameter(0) << endl;
        if (myfunc->GetParameter(0)>20 && !warning)  {
            warning=kTRUE;
            last_l=l+1;
            factor=0.15-(float)last_l*0.01;
            Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);
            cout << " WARNING BT DENSITY GREATER THAN 20 !! Appearing after maximum Chi2Cut of : " <<  Chi2Cut.Data() <<  endl;
        }
    }
    factor=0.15-(float)last_l*0.01;
    Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);
    if (warning) cout << "Maximum Chi2Cut = " << Chi2Cut.Data() << endl;

    if (eCheckQualitycut==kTRUE) {
        cout << "----- Set Parameters for Chi2Cut --------"<<endl;
        cout << "----- " << factor << " , 1.0 "<< endl;
        eShowerRec->SetQualityCutValues(factor,1.0);
    }
    else {
        cout << "----- No high BT density observed. --------"<<endl;
    }
    return;
}

//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::CheckBTDensityCanv(EdbPVRec *pvr, TObjArray *tracks) {
    // Plot Basetrack density of totalscan for each plate
    // Using the tracks in the given PVRec object.
    printf("EdbEDAShowerTab::CheckBTDensityCanv...\n");
    if (!eIsAliLoaded) LoadShowerRecEdbPVRec();

		
    // Genergic edbPVrec data (means either cp or lt file, depends on how the user set it:)
    // pvr= eShowerRec->GetEdbPVRec();
    pvr=pvrec_generic;


    // In Case this is set we directly use pvrec_cleaned of the EdbPVRQuality class.
    if (eQualityCutSetting==1 || eQualityCutSetting==2 ) {
	cout << "EdbEDAShowerTab::CheckBTDensityCanv   INFO :   eQualityCutSetting==1 || eQualityCutSetting==2    We directly use pvrec_cleaned of the EdbPVRQuality class." << endl;
	pvr=pvrec_cleaned;
    }

		// Check if exists:
		if ( NULL == pvr ) {
			cout << "EdbEDAShowerTab::CheckBTDensityCanv   ERROR :   NULL == pvr    Return now." << endl;
			return;
		}

    TCanvas *c1 = CreateCanvas("CheckBTDensity");
    c1->Divide(2,2);
    c1->cd(1);

    EdbPattern* 	pat = (EdbPattern*)pvr->GetPattern(0);
    Int_t 				patN= pat->N();
    Int_t 				pvrN= pvr->Npatterns()-1;
    Int_t 				pvrNMiddle= 0+(pvr->Npatterns()-1)/2;
    EdbSegP* 			seg=NULL;
    Float_t 			BTpermm2;

    printf("EdbEDAShowerTab::Create Histos...\n");
    TH2F* h_plate = new TH2F("All Plates","h_plate",125,0,125000,100,0,100000);
    TH2F* BTDens = new TH2F("BTDens","BTDens",125,0,125000,100,0,100000);
    TH2F* BTDensity[10];
    TProfile* BTDensProfile  = new TProfile("BTDensityProfile","BTDensityProfile",57,0,57,0,200);
    TProfile* BTDensityProfile[10];
    TH1F* h_BinContentDistribution = new TH1F("h_BinContentDistribution","h_BinContentDistribution",50,0,50);
    TH1F* h_BinCont[58];

    for (int l=0; l<10; ++l) {
        BTDensity[l]=(TH2F*)BTDens->Clone();
        BTDensityProfile[l]=(TProfile*)BTDensProfile->Clone();
        BTDensityProfile[l]->SetLineColor(21+2*l);
    }
    for (int l=0; l<pvrN; ++l) {
        h_BinCont[l]=(TH1F*)h_BinContentDistribution->Clone();
    }

    for (int j=0; j<pvrN; ++j) {
        pat = (EdbPattern*)pvr->GetPattern(j);
        patN= pat->N();
        // Get all Segments of the pattern and Fill Area Histos:
        for (int i=0; i<patN; ++i) {
            seg=(EdbSegP*)pat->GetSegment(i);
            h_plate->Fill(seg->X(),seg->Y());
            // Fill for 4 different Chi2 Cut sets the for histograms
            for (int l=0; l<10; ++l)  {
                float factor=0.15-(float)l*0.01;
                if (seg->Chi2()<seg->W()*factor-1.0) BTDensity[l]->Fill(seg->X(),seg->Y());
            }
        }
        // Get Bin Entries and fill profile Histos:
        for (int ki=1; ki<h_plate->GetNbinsX(); ki++ ) {
            for (int kj=1; kj<h_plate->GetNbinsY(); kj++ ) {
                for (int l=0; l<10; ++l) {
                    BTpermm2=(Float_t)BTDensity[l]->GetBinContent(ki,kj); // ONLY WHEN BINAREA=1000*1000
                    if (BTpermm2==0) continue;
                    BTDensityProfile[l]->Fill(j,BTpermm2,1);
                    if (l==3) h_BinCont[j]->Fill(BTpermm2);
                }
            }
        }
        for (int l=0; l<10; ++l)  {
            BTDensity[l]->Reset();
        }
    }
    // end of pattern loop.


    printf("EdbEDAShowerTab::Draw Histos...\n");
    // Draw Part:
    c1->cd(1);
    h_plate->Draw("colz");
    c1->cd(2);
    h_BinCont[0]->DrawNormalized();
    h_BinCont[pvrNMiddle]->SetLineColor(2);
    h_BinCont[pvrN-1]->SetLineColor(4);
    h_BinCont[pvrNMiddle]->DrawNormalized("same");
    h_BinCont[pvrN-1]->DrawNormalized("same");
    TLegend* leg= new TLegend(0.77,0.6,0.98,0.8,"","brNDC");
    leg->AddEntry(h_BinCont[0],"First plate","LP");
    leg->AddEntry(h_BinCont[pvrNMiddle],"Middle plate","LP");
    leg->AddEntry(h_BinCont[pvrN-1],"Last plate","LP");
    leg->SetBorderSize(1);
    leg->Draw();
    c1->cd(3);
    BTDensityProfile[0]->Draw("profileZ");
		int xnpat=pvrN+5;
		BTDensityProfile[0]->GetXaxis()->SetRangeUser(0,xnpat);
    BTDensityProfile[3]->Draw("profileZsame");
    BTDensityProfile[9]->Draw("profileZsame");
    BTDensityProfile[3]->SetLineColor(2);
    BTDensityProfile[3]->SetLineWidth(2);
    BTDensityProfile[9]->SetLineColor(6);
    BTDensityProfile[0]->GetYaxis()->SetRangeUser(0.0,1.2* BTDensityProfile[0]->GetMaximum());
    TLegend* leg2= new TLegend(0.15,0.75,0.4,0.88,"","brNDC");
    leg2->AddEntry(BTDensityProfile[0],"Chi2<W*0.15-1","LP");
    leg2->AddEntry(BTDensityProfile[3],"Chi2<W*0.12-1","LP");
    leg2->AddEntry(BTDensityProfile[9],"Chi2<W*0.06-1","LP");
    leg2->SetBorderSize(1);
    leg2->Draw();

		TLegend* leg3= new TLegend(0.7,0.15,0.98,0.75,"","brNDC");
    leg3->AddEntry(BTDensityProfile[0],"Chi2<W*0.15-1","LP");
		leg3->AddEntry(BTDensityProfile[1],"Chi2<W*0.14-1","LP");
		leg3->AddEntry(BTDensityProfile[2],"Chi2<W*0.13-1","LP");
    leg3->AddEntry(BTDensityProfile[3],"Chi2<W*0.12-1","LP");
		leg3->AddEntry(BTDensityProfile[4],"Chi2<W*0.11-1","LP");
		leg3->AddEntry(BTDensityProfile[5],"Chi2<W*0.10-1","LP");
		leg3->AddEntry(BTDensityProfile[6],"Chi2<W*0.09-1","LP");
		leg3->AddEntry(BTDensityProfile[7],"Chi2<W*0.08-1","LP");
		leg3->AddEntry(BTDensityProfile[8],"Chi2<W*0.07-1","LP");
    leg3->AddEntry(BTDensityProfile[9],"Chi2<W*0.06-1","LP");
    leg3->SetBorderSize(1);
    
		
    c1->cd(4);
		BTDensityProfile[0]->Draw("profileZ");
		BTDensityProfile[0]->GetXaxis()->SetRangeUser(0,xnpat);
    BTDensityProfile[0]->DrawCopy("profileZsame");
    for (int l=0; l<10; ++l) {
        BTDensityProfile[l]->DrawCopy("profileZsame");
    }
    leg3->Draw();
    c1->Update();
    // Draw Done....

    printf("EdbEDAShowerTab::Estimate Maximum by constant fit...\n");
    // Estimate Maximum by constant fit (just threshold determination).
    Bool_t warning=kFALSE;
    Int_t last_l=0;
    float factor=0.15-(float)last_l*0.01;
    TString Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);

    for (int l=9; l>=0; --l) {
        if (BTDensityProfile[l]->GetMaximum()<1) continue;
        BTDensityProfile[l]->Fit("pol0","Q0");
        TF1 *myfunc = BTDensityProfile[l]->GetFunction("pol0");
        if (myfunc->GetParameter(0)>20 && !warning)  {
            warning=kTRUE;
            last_l=l+1;
            factor=0.15-(float)last_l*0.01;
            Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);
            cout << " WARNING BT DENSITY GREATER THAN 20 !! Appearing after maximum Chi2Cut of : " <<  Chi2Cut <<  endl;
        }
    }
    factor=0.15-(float)last_l*0.01;
    Chi2Cut=Form("s.eChi2<s.eW*%f-1.0",factor);
    if (warning) {
        cout << "Maximum Chi2Cut = " << Chi2Cut << endl;
    }
    else {
        cout << "----- No high BT density observed. --------"<<endl;
    }

    return;
}




//-------------------------------------------------------------------------------------------

void EdbEDAShowerTab::FindPairingsNewAlgo() {
    cout << "void EdbEDAShowerTab::FindPairings() " << endl;

    if (NULL == pvrec_generic )	{
        cout << "ERROR:   NULL == pvrec_generic  " << endl;
        return;
    }

    /*
    	// Get possible Initiator BT Array.
    	TObjArray* InBTArray=gEDA->GetSelected();
    	Int_t eInBTArrayN=InBTArray->GetEntries();
    	cout << eInBTArrayN << endl;

    	if (eInBTArrayN==0) {
    	cout << " NO Vertex or BT Selected! Do nothing and return. "<< endl;
    	return;
    	}

    // Get possible selected Vertex BT Array.
    EdbVertex* eVertex=gEDA->GetSelectedVertex();
    if (eVertex==NULL) {
      cout << "WARNING:::gEDA->GetSelectedVertex() is NULL POINTER .... DONT USE A VERTEX AS INPUT. Do nothing and return." << endl;
      return;
    }

    // return;


    // For Pairings Search we want to have the full cp files data:
    // LoadShowerRecEdbPVRec();
    // eShowerRec->GetEdbPVRec()->Print();
    // EdbPVRec* eAli_Sub=eShowerRec->GetEdbPVRec();
    // eAli_Sub->Print();


    EdbPVRec* eAli_Sub=pvrec_generic;
    eNShowers=0;
    RecoShowerArray=NULL;
    // eVertex->Print();
    // return;


    EdbShowAlg_GS* Alg = new EdbShowAlg_GS();
    Alg->SetEdbPVRec(eAli_Sub);
    if (eVertex!=NULL) Alg->SetInVtx(eVertex);
    else Alg->SetInVtxArray(InBTArray);
    cout << Alg->GetInVtxArrayN() << endl;
    // gEDBDEBUGLEVEL=3;
    Alg->Execute();

    eNShowers=Alg->GetRecoShowerArrayN();
    RecoShowerArray=Alg->GetRecoShowerArray();

      if (eDrawShowers) {
       //cout << " void Reco()   Draw All Reconstructed Showers" << endl;
       for (Int_t i=0; i<eNShowers; ++i) {
         //cout << " void Reco()   Draw Reconstructed Showers   i= " << i <<  endl;
         DrawShower(i);
       }
     }

    */
}


//-------------------------------------------------------------------------------------------


void EdbEDAShowerTab::FindPairings() {

    cout << "void EdbEDAShowerTab::FindPairings() " << endl;

    if (NULL == pvrec_generic )	{
        cout << "ERROR:   NULL == pvrec_generic  " << endl;
        return;
    }

    EdbSegP* InBT=NULL;
    EdbSegP* Segment=NULL;
    EdbSegP* Segment2=NULL;
    EdbSegP* Segment_Sum=new EdbSegP(0,0,0,0,0,0);
    Float_t	 IP_Pair_To_InBT=0;
    Float_t x_av,y_av,z_av,tx_av,ty_av,distZ;
    Int_t RecoShowerArrayN=0;
    TArrayI* SegmentPIDArray = new TArrayI(99999999);
    TArrayI* SegmentIDArray = new TArrayI(99999999);
    TArrayI* Segment2PIDArray = new TArrayI(99999999);
    TArrayI* Segment2IDArray = new TArrayI(99999999);

    TObjArray* InBTArray=gEDA->GetSelected();
    Int_t eInBTArrayN=InBTArray->GetEntries();
    cout << eInBTArrayN << endl;

    if (eInBTArrayN==0) {
        cout << " NO Vertex or BT Selected! Do nothing and return. "<< endl;
        return;
    }

// For Pairings Search we want to have the full cp files data:
// LoadShowerRecEdbPVRec();
// eShowerRec->GetEdbPVRec()->Print();

// gEDA->GetPVRec() doesnt exist anymore, now its splitted via the different
// track sets:
// but ususally this has only entries of linked tracks (even if labelled as "TS")
// EdbPVRec* eAli_Sub=gEDA->GetTrackSet("TS")->GetPVRec();
// Whole cp files data:
// EdbPVRec* eAli_Sub=eShowerRec->GetEdbPVRec();
// eAli_Sub->Print();
// return;


    EdbPVRec* eAli_Sub=pvrec_generic;

    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

        // Get InitiatorBT from eInBTArray  InBT
        InBT=(EdbSegP*)InBTArray->At(i);
        if (gEDBDEBUGLEVEL>1) InBT->PrintNice();

        Int_t npat=eAli_Sub->Npatterns()-1;
        Int_t pat_one_bt_cnt_max,pat_two_bt_cnt_max=0;
        EdbPattern* pat_one=0;
        EdbPattern* pat_two=0;

        for (Int_t pat_one_cnt=0; pat_one_cnt<npat; ++pat_one_cnt) {
            cout << "Looping over pattern one, counter=" << pat_one_cnt << endl;
            for (Int_t pat_two_cnt=0; pat_two_cnt<npat; ++pat_two_cnt) {
                cout << "	Looping over pattern two, counter=" << pat_two_cnt << endl;

                // Now apply cut conditions: GS  GAMMA SEARCH Alg:
                if (TMath::Abs(pat_one_cnt-pat_two_cnt)>1) continue;

                pat_one=(EdbPattern*)eAli_Sub->GetPattern(pat_one_cnt);
                pat_two=(EdbPattern*)eAli_Sub->GetPattern(pat_two_cnt);
                pat_one_bt_cnt_max=eAli_Sub->GetPattern(pat_one_cnt)->GetN();
                pat_two_bt_cnt_max=eAli_Sub->GetPattern(pat_two_cnt)->GetN();

                if (pat_one->Z()-InBT->Z()>25000) continue;
                if (pat_one->Z()-InBT->Z()<0) continue;
                if (pat_two->Z()-InBT->Z()>25000) continue;
                if (pat_two->Z()-InBT->Z()<0) continue;

                for (Int_t pat_one_bt_cnt=0; pat_one_bt_cnt<pat_one_bt_cnt_max; ++pat_one_bt_cnt) {
                    for (Int_t pat_two_bt_cnt=0; pat_two_bt_cnt<pat_two_bt_cnt_max; ++pat_two_bt_cnt) {
                        // 	    cout << pat_one_cnt <<" " <<  pat_two_cnt <<" " <<  pat_one_bt_cnt <<" " <<  pat_two_bt_cnt <<" " << endl;

                        Segment =  (EdbSegP*)pat_one->GetSegment(pat_one_bt_cnt);
                        Segment2 = (EdbSegP*)pat_two->GetSegment(pat_two_bt_cnt);
                        if (Segment2==Segment) continue;

                        // Now apply cut conditions: GS  GAMMA SEARCH Alg  --------------------
                        // At last: Remove // better: check for duplicated pairings:
                        if (CheckPairDuplications(RecoShowerArrayN,Segment->PID(),Segment->ID(),Segment2->PID(),Segment2->ID(), SegmentPIDArray,SegmentIDArray,Segment2PIDArray,Segment2IDArray)) continue;
                        // d) Check if dist Z to vtx (BT) is ok:
                        // TestSegment has to have greater Z than Vertex/InBT
                        distZ=Segment->Z()-InBT->Z();
                        if (distZ>25000) continue;
                        // a) Check dR between tracks:
                        if (CalcdR_NoPropagation(Segment,Segment2)>100) continue;
                        // b) Check dT between tracks:
                        if (CalcdTheta(Segment,Segment2)>0.1) continue;
                        // c) Check dMinDist between tracks:
                        if (CalcDmin(Segment,Segment2)>30.0) continue;

                        x_av=Segment2->X()+(Segment->X()-Segment2->X())/2.0;
                        y_av=Segment2->Y()+(Segment->Y()-Segment2->Y())/2.0;
                        z_av=Segment2->Z()+(Segment->Z()-Segment2->Z())/2.0;
                        tx_av=Segment2->TX()+(Segment->TX()-Segment2->TX())/2.0;
                        ty_av=Segment2->TY()+(Segment->TY()-Segment2->TY())/2.0;
                        Segment_Sum->SetX(x_av);
                        Segment_Sum->SetY(y_av);
                        Segment_Sum->SetTX(tx_av);
                        Segment_Sum->SetTY(ty_av);
                        Segment_Sum->SetZ(z_av);

                        // d) Check if IP to vtx (BT) is ok:
                        IP_Pair_To_InBT=CalcIP(Segment_Sum, InBT->X(),InBT->Y(),InBT->Z());
                        if (IP_Pair_To_InBT>200) continue;

                        SegmentPIDArray->AddAt(Segment->PID(),RecoShowerArrayN);
                        SegmentIDArray->AddAt(Segment->ID(),RecoShowerArrayN);
                        Segment2PIDArray->AddAt(Segment2->PID(),RecoShowerArrayN);
                        Segment2IDArray->AddAt(Segment2->ID(),RecoShowerArrayN);
                        // end of    cut conditions: GS  GAMMA SEARCH Alg  --------------------


                        // Create new EdbShowerP Object for storage;
// 						// See EdbShowerP why I have to call the Constructor as "unique" ideficable value
                        EdbTrackP* RecoShower = new EdbTrackP();
// 						// Using this algorithm we search only pairs, so shower has by definition only two segments:
                        RecoShower -> AddSegment(Segment);
                        RecoShower -> AddSegment(Segment2);
// 						// Obligatory when Shower Reconstruction is finished:
// 						RecoShower ->Update();
// 						// Print:
                        RecoShower ->PrintNice();
// 						RecoShower ->PrintSegments();
// 						Segment_Sum->PrintNice();
// 						cout <<"------------"<< endl;
//

                        // Add Shower to Array:
                        RecoShowerArray->Add(RecoShower);
                        RecoShowerArrayN++;

                    }
                }

            }
        }


    } // Loop over InBT

    // Delete unnecessary objects:
    delete SegmentPIDArray;
    delete Segment2PIDArray;
    delete SegmentIDArray;
    delete Segment2IDArray;


    InBT->PrintNice();
    cout << "EdbShowAlg_GS::Execute()...In total we found (raw) Gamma Candidates:    " << RecoShowerArrayN << endl;
    cout << "EdbShowAlg_GS::Execute()...done." << endl;

    cout << "...TO DO... Return the eRecoShowerArray  and Draw it...." << endl;

    if (eDrawShowers) {
        cout << " void Reco()   Draw All Reconstructed Showers" << endl;
        for (Int_t i=0; i<RecoShowerArrayN; ++i) {
            cout << " void Reco()   Draw Reconstructed Showers   i= " << i <<  endl;
            DrawShower(i);
        }
    }
    return;
}


Bool_t EdbEDAShowerTab::CheckPairDuplications(Int_t nmax, Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray)
{

    for (Int_t i=0; i<nmax; i++) {
        // PID and ID of Seg and Seg2 to be exchanged for duplications
        if ( SegPID==Segment2PIDArray->At(i) && Seg2PID==SegmentPIDArray->At(i) && SegID==Segment2IDArray->At(i) && Seg2ID==SegmentIDArray->At(i)) {
            //cout << "Found duplocation for... return true"<<endl;
            return kTRUE;
        }
    }
    return kFALSE;
}


double EdbEDAShowerTab::CalcdR_NoPropagation(EdbSegP* s,EdbSegP* stest) {
    return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
}

double EdbEDAShowerTab::CalcdR_WithPropagation(EdbSegP* s,EdbSegP* stest) {
    if (s->Z()==stest->Z()) CalcdR_NoPropagation(s,stest);
    Double_t zorig;
    Double_t dR;
    zorig=s->Z();
    s->PropagateTo(stest->Z());
    dR=TMath::Sqrt( (s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()) );
    s->PropagateTo(zorig);
    return dR;
}

double EdbEDAShowerTab::CalcdTheta(EdbSegP* s1,EdbSegP* s2) {
    Double_t tx1,tx2,ty1,ty2;
    tx1=s1->TX();
    tx2=s2->TX();
    ty1=s1->TY();
    ty2=s2->TY();
    Double_t dt= TMath::Sqrt( (tx1-tx2)*(tx1-tx2) + (ty1-ty2)*(ty1-ty2) );
    return dt;
}


void EdbEDAShowerTab::CheckCosmicReco() {

    if (NULL == pvrec_generic )	{
        cout << "ERROR:   NULL == pvrec_generic  " << endl;
        return;
    }

    cout << "" << endl;

    // Get track set of TS, this is our Referecne Cosmic Track Set
    cout << "// Get track set of TS, this is our Referecne Cosmic Track Set" << endl;

    cout << "gEDA->GetTrackSet(TS)->N() " <<   gEDA->GetTrackSet("TS")->N() <<  endl;

    cout << "gEDA->GetTrackSet(TS)->GetPVRec()->Print(); :" << endl;
    gEDA->GetTrackSet("TS")->GetPVRec()->Print();

    EdbEDATrackSet* reftrackset = gEDA->GetTrackSet("TS"); //snew EdbEDATrackSet("ReferencetrackSet");
    TObjArray* tracksArray=reftrackset->GetTracksBase();
    EdbTrackP* track;
    int maxTrackSegments=0;

    // Loop to find longest track npl:
    cout << "// Loop to find longest track npl:" << endl;
    for (int i=0; i<tracksArray->GetEntries(); i++) {
        track=(EdbTrackP*) tracksArray->At(i);
        //track->Print();
        if (track->N()>maxTrackSegments) maxTrackSegments=track->N();
    }
    cout << "maxTrackSegments=" <<  maxTrackSegments << endl;


    if (NULL == pvrec_generic )	cout << "ERROR:   NULL == pvrec_generic  " << endl;

    // Get number of patterns for edbpvrec objects
    cout << "// Get number of patterns for edbpvrec objects" << endl;
    int maxNpat=pvrec_generic->Npatterns();
    cout << "maxNpat=" <<  maxNpat << endl;

    // Get number of patterns for edbpvrec objects
    TObjArray* TestRecoCosmicInBTArray = new TObjArray();

    for (int i=0; i<tracksArray->GetEntries(); i++) {
        track=(EdbTrackP*) tracksArray->At(i);
        if (track->N()<maxNpat-6) continue;
        TestRecoCosmicInBTArray->Add(track);
    }

    cout << "// Get number of TestRecoCosmicInBTArray" << TestRecoCosmicInBTArray->GetEntries() << endl;

    eInBTArray=TestRecoCosmicInBTArray;

    eShowerRec->SetInBTArray(eInBTArray);
    eShowerRec->Execute();

    cout << "" << endl;

    cout << " void CheckCosmicReco()   eShowerRec->PrintRecoShowerArray()..." << endl;
    eShowerRec->PrintRecoShowerArray();

    eNShowers=eShowerRec->GetRecoShowerArrayN();
    RecoShowerArray=eShowerRec->GetRecoShowerArray();
    eShowerRec->SetQualityCutValues(0.15,1.0);

    if (eDrawShowers) {
        for (Int_t i=0; i<eNShowers; ++i) {
            DrawShower(i);
        }
    }


    cout << "// Get number of TestRecoCosmicInBTArray" << TestRecoCosmicInBTArray->GetEntries() << endl;
    cout << "// Get number of RecoShowerArray" << RecoShowerArray->GetEntries() << endl;

    EdbTrackP* showertrack=0;
    cout << "// Starting Tracks with Reconstructed Showers...:" << endl;

    int nseg_track_minus_shower=0;
    TH1F* h_nseg_track_minus_shower=new TH1F("track_shower_difference","track_shower_difference",100,-50,50);

    for (int i=0; i<TestRecoCosmicInBTArray->GetEntries(); i++) {
        track=(EdbTrackP*) TestRecoCosmicInBTArray->At(i);
// 	 track->PrintNice();
        for (int j=i; j<RecoShowerArray->GetEntries(); j++) {
            showertrack=(EdbTrackP*) RecoShowerArray->At(j);
// 			showertrack->PrintNice();
            if (TMath::Abs(track->Z()-showertrack->Z())>10) continue;
            if (TMath::Abs(track->X()-showertrack->X())>10) continue;
            if (TMath::Abs(track->Y()-showertrack->Y())>10) continue;

            cout << "Comparing Starting Track i=" << i << "with RecoShower j=" << j <<":";
            cout << "Nseg:  " << track->N() << "  <>  " << showertrack->N() << endl;

            nseg_track_minus_shower = track->N() - showertrack->N();
            h_nseg_track_minus_shower->Fill(nseg_track_minus_shower);
        }
    }

    cout << "Drawing now histogram: positive Entries Mean More Tracks thank shower segemnts." << endl;
    cout << "Drawing now histogram: negative Entries Mean More shower segemnts than tracks." << endl;
    cout << "Drawing now histogram: In the  ideal  case, this histogram should peak at 0. If it has large negatie number than check, because the shower contains then toooo much tracks...." << endl;

    TCanvas *c1 = CreateCanvas("CosmicCheck");
    c1->cd(1);
    h_nseg_track_minus_shower -> Draw();
    c1->cd();

    if (h_nseg_track_minus_shower->GetMean()<0) cout << "WARNING !!  MAYBE TOO MUCH BACKGROUND !!! " << endl;
}





void EdbEDAShowerTab::PlotShower() {

		// Decide wheter to plot the histograms for one shower,
		// or for whole Shower Array:
		// ....

    TH1F* h_nseg= new TH1F("h_nseg","h_nseg",100,0,100);
    TH1F* h_npl= new TH1F("h_npl","h_npl",58,0,58);
    for (int i=0; i<RecoShowerArray->GetEntries(); i++) {
        EdbTrackP* sh = (EdbTrackP*)RecoShowerArray->At(i);
        h_nseg->Fill(sh->N());
        h_npl->Fill(sh->Npl());
    }
    TCanvas *c1 = CreateCanvas("ShowerPlots");
    c1->Divide(1,2);
    c1->cd(1);
    h_nseg->Draw();
    c1->cd(2);
    h_npl->Draw();
    c1->cd();
}

//____________________________________________________________________________________________


void EdbEDAShowerTab::MakeParameterWindowQualitySettings(){

	// main frame
   TGMainFrame *fMainFrame = new TGMainFrame(gClient->GetRoot(),10,300,kMainFrame | kVerticalFrame);
   fMainFrame->SetLayoutBroken(kTRUE);

		eParamWindow = fMainFrame;

	int posy=10;
	int posx=10;
	int dx;

	TGLabel* fLabel = new TGLabel(fMainFrame,"QualitySettings Parameters");
	fMainFrame->AddFrame(fLabel);
	fLabel->MoveResize(posx,posy,dx=140,20);
	posx+=dx+10;

		//////////////////////////////////////////
		posy+=20;
		posx=5;

    eComboBox_ParameterQualitySetting = new TGComboBox(fMainFrame,"eQualitySetting");
    eComboBox_ParameterQualitySetting->AddEntry("No Qual Cut",-1);
    eComboBox_ParameterQualitySetting->AddEntry("Global Qual Cut",0);
    eComboBox_ParameterQualitySetting->AddEntry("PlateByPlate, Constant BT Density",1);
    eComboBox_ParameterQualitySetting->AddEntry("PlateByPlate, Constant Quality",2);
    eComboBox_ParameterQualitySetting->Select(0);
    eComboBox_ParameterQualitySetting->MoveResize(posx,posy,dx=140,20);
    fMainFrame->AddFrame(eComboBox_ParameterQualitySetting);
    eComboBox_ParameterQualitySetting->Connect("Selected(const char *)", "EdbEDAShowerTab", this, "ReadComboBoxParameterQualitySetting()");


		posx+=dx+5;
   TGTextButton *eButton = new TGTextButton(fMainFrame,"Check BT density directly");
    eButton->MoveResize(posx,posy,dx=120,20);
    eButton->Connect("Clicked()","EdbEDAShowerTab", this,"CheckBTDensityCanv()");

		

		fMainFrame->SetMWMHints(kMWMDecorAll,kMWMFuncAll,kMWMInputModeless);
   fMainFrame->MapSubwindows();
   fMainFrame->Resize(fMainFrame->GetDefaultSize());
   fMainFrame->MapWindow();
   fMainFrame->Resize(800,280);

   gClient->WaitFor(fMainFrame);

}

//____________________________________________________________________________________________


void EdbEDAShowerTab::PrintQualityCutSetting(){
	// Print important information about the QualityCut Setting:
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()" << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   eQualityCutSetting= " << eQualityCutSetting << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   ----- For information: -----" << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   eQualityCutSetting==-1 : No Quality Cut" << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   eQualityCutSetting==0  : Global Quality Cut by constant BT density." << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   eQualityCutSetting==1  : Plate by Plate Cut by constant BT density." << endl;
	cout << "EdbEDAShowerTab::PrintQualityCutSetting()   eQualityCutSetting==2  : Plate by Plate Cut by constant Quality." << endl;
}



void EdbEDAShowerTab::GO() {
    cout << "-----------------------------------------" << endl;
    cout << endl << endl;
    cout << "EdbEDAShowerTab::GO()... Not implemented yet." << endl;
    cout << "-----------------------------------------" << endl;
}


void EdbEDAShowerTab::Help() {
    cout << "-----------------------------------------" << endl;
    cout << endl << endl;
    cout << "EdbEDAShowerTab::Help()     Last date updated: 12th July, 2010;  frank.meisel@lhep.unibe.ch " << endl;
    cout << endl << endl;
    cout << "HowTo Use this tab and generally:" << endl;
    cout << "*		Click a BaseTrack and then click RecoButton." << endl;
    cout << "" << endl;
    cout << "*		UseTS:	Normally we have the linked_tracks dataset and the full cp files." << endl;
    cout << "*		UseTS:	Whereas standard track search is done on the volume of the linked_tracks, other..." << endl;
    cout << "*		UseTS:	operations like microtrack search or shower reconstruction can (and shall) operate..." << endl;
    cout << "*		UseTS:	on full cp dataset. To load the EdbPVrec object, toggle this button. Loading full..." << endl;
    cout << "*		UseTS:	EdbPVrec object may take a while, but it needs to be done only one time during session." << endl;
    cout << "" << endl;
    cout << "*		UseQuality:	Normally the standard shower reco algorithm is backgound stable up to a level..." << endl;
    cout << "*		UseQuality:	of 20 BT/mm2. In case the BG level is higher then here the readaption of ..." << endl;
    cout << "*		UseQuality:	the Chi2 vs Grain quality cut is adapted. For now it is working by constant BT density fit ..." << endl;
    cout << "*		UseQuality:	over the whole EdbPVRecObject (It may be extended Plate/by/Plate later)." << endl;
    cout << "" << endl;
    cout << "*		Draw: 	If toggled, then reconstructed showers will be stored in EDA TrackSet _BT_ ." << endl;
    cout << "*		Plot: 	Plot interesting quantities for the showers stored in the internal shower array." << endl;
		cout << "*		Plot: 	For the time being, it plots the quantities for all reconstructed showers at once." << endl;
    cout << "*		Nr: 	  Draw Shower Number ... ." << endl;
    cout << "*		Prev/All/Next: 	  Draw Shower: Previous, Next, or All again.." << endl;
    cout << "" << endl;
    cout << "*		CheckPVR:	... Check the EdbPVrec Object." << endl;
		cout << "" << endl;
    cout << "*		CheckBTDens:	Check BaseTrack Density of the generic PVRec object. It is strongly recommended that you" << endl;
		cout << "*		CheckBTDens:	click this button and watch the density not going to strong over 20-30 BT/mm2." << endl;
		cout << "" << endl;
		cout << "*		CheckCosmicReco:	This is used for a analysis wheter the background is acceptable. We start here from" << endl;
		cout << "*		CheckCosmicReco:	passing through cosmics and then run the reco on them. In the best case, " << endl;
		cout << "*		CheckCosmicReco:	the shower should contain only out the tracks (since they are cosmics) and therefore N_shower <= N_track."  << endl;
		cout << "*		CheckCosmicReco:	In the histogram which appears, the difference in number of segments is plotted." << endl;
		cout << "*		CheckCosmicReco:	If its largely in the negative region this is an indication that there is too much BG." << endl;
		cout << "" << endl;
    cout << "*		SetPVR:	Set the EdbPVrec Object from a (root)file. ..." << endl;
    cout << "" << endl;
    cout << "*		RecoParams:   This box is expert status, here you can change parameters of the reco algorithm itself." << endl;
		cout << "*		RecoParams:   Select Parameter, change to your desired value (units are mrad,microns) and click change button." << endl;
		cout << "*		RecoParams:   Three predefined sets are there: Standard; " << endl;
		cout << "*		RecoParams:   Three predefined sets are there: HighNBT (large acceptance, only good in case of loooow background.)"<< endl;
		cout << "*		RecoParams:   Three predefined sets are there: HighPur (tight cuts, good in case of large background.)"<< endl;
    cout << "" << endl;

    new TGMsgBox(gClient->GetRoot(), gEve->GetMainWindow(),"EDA", "HELP BOX! Please be so kind and read the shell output. Thank you.");
    cout << "-----------------------------------------" << endl;
		return;
}



void EdbEDAShowerTab::ReadComboBoxParameterQualitySetting() {
    // Read Name from the ReadComboBoxParameterQualitySetting and put the corresponging value in value field:
    cout << "eComboBox_Parameter->GetSelectedEntry->EntryId()" << endl;
    Double_t IdVal = eComboBox_ParameterQualitySetting->GetSelectedEntry()->EntryId();
		eQualityCutSetting=IdVal;
    cout << "eQualityCutSetting = " << eQualityCutSetting << endl;
}


void EdbEDAShowerTab::CheckBTDensityUsingEdbPVRQuality() {
		EdbPVRQuality* qual = new EdbPVRQuality(pvrec_generic);
		pvrec_cleaned =  qual->GetEdbPVRec(1);
		eIspvrec_cleaned=kTRUE;
		qual->Print();
}





void EdbEDAShowerTab::CheckAndSetPVRGeneric(){
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()" << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   pvrec_cpfiles=" << pvrec_cpfiles << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   pvrec_linkedtracks=" << pvrec_linkedtracks << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   pvrec_cleaned_cpfiles=" << pvrec_cleaned_cpfiles << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   pvrec_cleaned_linkedtracks=" << pvrec_cleaned_linkedtracks << endl;

	// Cases: LT/CP (eUseTSData);
	// Cases: Cleaned/Raw  (eIspvrec_cleaned);

	
	if (eUseTSData) {
		pvrec_generic=pvrec_cpfiles;
		if (eIspvrec_cleaned && pvrec_cleaned_cpfiles!=NULL) pvrec_generic=pvrec_cleaned_cpfiles;
	}
	else {
		pvrec_generic=pvrec_linkedtracks;
		if (eIspvrec_cleaned && pvrec_cleaned_linkedtracks!=NULL) pvrec_generic=pvrec_cleaned_linkedtracks;
	}
	
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   Since eUseTSData="<< eUseTSData << " and eIspvrec_cleaned=" << eIspvrec_cleaned << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   We set as pvrec_generic = " << pvrec_generic  << endl;
	cout << "EdbEDAShowerTab::CheckAndSetPVRGeneric()   ...Done." << endl;
	
	return;
}