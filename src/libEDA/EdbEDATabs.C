#include"EdbEDATabs.h"
#include"EdbEDA.h"

using namespace EdbEDAUtil;

extern EdbEDA *gEDA;


ClassImp(EdbEDAListTab)
ClassImp(EdbEDAVertexTab)
ClassImp(EdbEDAMCTab)
ClassImp(EdbEDAPredTab)
ClassImp(EdbEDAOperationTab)
ClassImp(EdbEDATrackSetList)


void EdbEDAListTab::WriteListFile(){ 
	TString filename=gEDA->GetTrackSet("TS")->WriteListFile(NULL,kFALSE);
	fListFilesOpened->RemoveAll();
	
	fListFilesOpened->AddEntry(filename.Data(),0);
	ReviceFileList();
	fListFileEditor->LoadFile(const_cast<char*> (filename.Data()));
}

void EdbEDAListTab::ReviceFileList(){
	fListFiles->RemoveAll();
	void *dir = gSystem->OpenDirectory(".");
	const char *filename;
	int i=0;
	while ((filename = gSystem->GetDirEntry(dir))){
		TString s = filename;
		if(s.Contains(".lst")){
		if(s.Contains("lnk")==0){
			//printf("%s %d\n",filename,i);
			fListFiles->AddEntry(filename,i++);
		}}
	}
	gSystem->FreeDirectory(dir);
	fListFiles->MoveResize(fListFiles->GetX(), fListFiles->GetY(), fListFiles->GetWidth(), fListFiles->GetHeight());
}

void EdbEDAListTab::ReadSelectedList(){
	gEDA->StorePrevious();
	EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
	
	TGLBEntry *e = fListFiles->GetSelectedEntry();
	if(NULL==e) { printf("Please select list file.\n"); return;}
	printf("list file selected %d %s\n",fListFiles->GetSelected(), e->GetTitle());
	
	fListFileEditor->LoadFile(const_cast<char*> (e->GetTitle()));

	int clear_previous = fCheck_list_clear_previous->GetState()==kButtonDown ? 1 : 0;

	if(clear_previous) {
		set->ClearTracks();
		fListFilesOpened->RemoveAll();
	}
	
	set->ReadListFile(const_cast<char*> (e->GetTitle()), clear_previous);
	
	fListFilesOpened->AddEntry(e->GetTitle(),0);
	fListFilesOpened->MoveResize(fListFilesOpened->GetX(), fListFilesOpened->GetY(), fListFilesOpened->GetWidth(), fListFilesOpened->GetHeight());
	gEDA->Redraw();
}

void EdbEDAListTab::MakeGUI(){
	// Create minimal GUI for event navigation.

	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kLeft);

	TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
	frmMain->SetWindowName("XX GUI");
	frmMain->SetCleanup(kDeepCleanup);
	frmMain->SetLayoutBroken(kTRUE);
	int posy=5;
	TGLabel *fLabel = new TGLabel(frmMain,"List files");
	fLabel->MoveResize(5,posy,60,20);

	posy+=20;
	TGTextButton *fb = new TGTextButton(frmMain,"Update");
	fb->Connect("Clicked()", "EdbEDAListTab", this, "ReviceFileList()"); 
	fb->MoveResize(5,posy,50,20);
	//SetCommand("revice_file_list();	fListFiles->MoveResize(5,50,230,150);");
	
	fb = new TGTextButton(frmMain,"Read");
	fb->Connect("Clicked()", "EdbEDAListTab", this, "ReadSelectedList()"); 
	fb->MoveResize(60,posy,60,20);
	//fb->SetCommand("read_selected_list(); fListFilesOpened->MoveResize(5,230,230,80);");

	fCheck_list_clear_previous = new TGCheckButton(frmMain,"Clear previous");
	fCheck_list_clear_previous->MoveResize(125, posy, 105,20);
	fCheck_list_clear_previous->SetState(kButtonDown);


	posy+=25;
	int dy=100;
	//printf("%d %d\n", posy,dy);
	// list box
	fListFiles = new TGListBox(frmMain);
	frmMain->AddFrame(fListFiles);
	fListFiles->MoveResize(5,posy,230,dy);
	ReviceFileList();
	
	posy+=dy+10;
	dy   =10;
	fLabel = new TGLabel(frmMain,"Opened list");
	fLabel->MoveResize(5,posy,70,dy);

	//printf("%d %d\n", posy,dy);
	posy+=dy;
	dy   =60;
	fListFilesOpened = new TGListBox(frmMain);
	frmMain->AddFrame(fListFilesOpened);
	fListFilesOpened->MoveResize(5,posy,230,dy);

	posy+=dy+5;
	dy   =20;

	fb = new TGTextButton(frmMain,"Save current track list");
	//fb->SetCommand("write_list()");
	fb->Connect("Clicked()", "EdbEDAListTab", this, "WriteListFile()"); 
	fb->MoveResize(100,posy,130, dy);
	
	posy+=dy+10;
	dy   =370;
	fListFileEditor = new TGTextEditor((const char *)NULL,frmMain);
	frmMain->AddFrame(fListFileEditor);
	fListFileEditor->MoveResize(5,posy,230,dy);
	
	frmMain->MapSubwindows();
	frmMain->Resize();
	frmMain->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("Lists", 0);
}

void EdbEDAVertexTab::ApplyParams(){
	eNtrk        = eGNEVNtrk->GetIntNumber();
	eVtxTrk      = eGCBVtxTrk->GetSelected();
	eDZmax       = eGNEDZmax->GetNumber();
	eProbMin     = eGNEProbMinV->GetNumber();
	eImpMax      = eGNEImpMax->GetNumber();
	eUseMom      = eGCBUseMom->IsOn();
	eUseSegPar   = eGCBUseSegPar->IsOn();
	eQualityMode = eQualityMode;

	eAngularCut  = eGCBAngularCut->IsOn();
	eTX = gEDA->GetMainTab()->GetTX();
	eTY = gEDA->GetMainTab()->GetTY();
	eDT = gEDA->GetMainTab()->GetDT();
}

void EdbEDAVertexTab::Redraw(){
	SetVertices();
	gEDA->Redraw();
}


void EdbEDAVertexTab::DoVertexing(EdbEDATrackSet *set){
	if(set != NULL) eSet = set;
	else eSet=gEDA->GetTrackSet("TS");
	// Select tracks for vertexing
	
	ApplyParams();
	printf("----------------------------------------------------------\n");
	printf(" Do vertexing, ");
	if(eVtxTrk==USE_CURRENT_TRACKS) {
		printf("using tracks in current view.\n");
		eEdbTracks = eSet->GetTracks();
	}
	else {
		printf("using tracks all tracks with Nseg, MaxPlate cuts\n");
		// apply cuts on Nseg and Penetration.
		gEDA->GetMainTab()->ApplyParams();
		eSet->SetImpactSearch   (kFALSE);
		eSet->SetNeighborSearch (kFALSE);
		eSet->SetAngularCut     (kFALSE);
		eSet->ClearTracks();
		eSet->DoSelection();
		eEdbTracks = eSet->GetTracks();
	}
	
	// Setting parameters
	eVTX = new TObjArray;
	if( eSet->GetPVRec()==NULL) {
		EdbPVRec *rec = new EdbPVRec;
		eSet->SetPVRec(rec);
	}
	if(eSet->GetPVRec()->GetScanCond()==NULL) eSet->GetPVRec()->SetScanCond(new EdbScanCond);
	SetPVRec(eSet->GetPVRec());
	
	// Vertexing
	printf("%d tracks for vertexing\n",  eEdbTracks->GetEntries() );
	int nvtx = FindVertex();
	printf("%d 2-track vertexes was found\n",nvtx);
	int nadd;
	if(nvtx != 0) nadd =  ProbVertexN();
	
	eSet->GetPVRec()->eVTX = eVTX;

}

void EdbEDAVertexTab::DoVertexingButton(){
	DoVertexing();
	gEDA->GetVertexSet()->Clear();
	gEDA->GetVertexSet()->AddVertices(eVTX);
	eSet->SetTracksVertices(eVTX);
	Redraw();
}

void EdbEDAVertexTab::SetVertices(){
	gEDA->ClearSelected();
	gEDA->ClearSelectedVertex();
	gEDA->ClearVertices();
	ApplyParams();

	if(eVTX==NULL) DoVertexing();

	for(int  j=0 ; j < Nvtx() ; j++){
		EdbVertex *v = (EdbVertex *) (eVTX->At(j));
		
		// ntrack cut
		if(v->N()<eNtrk) continue;

		// cut vertices which doesn't have a track matching the angle.
		if ( eAngularCut){
			int flag=0;
			for(int i=0;i<v->N();i++){
				EdbTrackP *t = v->GetTrack(i);
				if(	fabs(t->TX() - eTX) < eDT &&
					fabs(t->TY() - eTY) < eDT) flag++;
			}
			if(flag==0) continue;
		}
		gEDA->AddVertex(v);
	}
	
	eSet->SetTracksVertices(gEDA->GetVertices());
}


void EdbEDAVertexTab::Clear(){ gEDA->ClearVertices(); gEDA->Redraw(); }



void EdbEDAVertexTab::MakeGUI(){
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot());
	frmMain->SetWindowName("XX GUI");
	frmMain->SetCleanup(kDeepCleanup);
	frmMain->SetLayoutBroken(kTRUE);

	TGLabel *fLabel;
	int posy=10;
	int posx=10;
	int dx=50;

	fLabel = new TGLabel(frmMain,"ImpMax =");
	fLabel->MoveResize(posx,posy,dx,20);
	posx+=dx+5;
	eGNEImpMax = new TGNumberEntryField(frmMain, -1, eImpMax);
	eGNEImpMax->MoveResize(posx,posy,dx=30,20);

	posx+=dx+10;
	fLabel = new TGLabel(frmMain,"DZmax =");
	fLabel->MoveResize(posx,posy,dx=50,20);
	posx+=dx+5;
	eGNEDZmax = new TGNumberEntryField(frmMain, -1, eDZmax);
	eGNEDZmax->MoveResize(posx,posy,dx,20);

	posx+=dx+10;
	fLabel = new TGLabel(frmMain,"ProbMinV =");
	fLabel->MoveResize(posx,posy,dx=70,20);
	posx+=dx+5;
	eGNEProbMinV = new TGNumberEntryField(frmMain, -1, eProbMin);
	eGNEProbMinV->MoveResize(posx,posy,dx=50,20);

	posx+=dx+15;
	eGCBUseMom = new TGCheckButton(frmMain,"UseMom");
	eGCBUseMom->MoveResize(posx,posy,dx=70,20);
	eGCBUseMom->SetState(eUseMom?kButtonDown:kButtonUp);
	
	posx+=dx+10;
	eGCBUseSegPar = new TGCheckButton(frmMain,"UseSegPar");
	eGCBUseSegPar->MoveResize(posx,posy,dx=90,20);
	eGCBUseSegPar->SetState(eUseSegPar?kButtonDown:kButtonUp);

	// combo box
	posx+=dx+10;
	eGCBVtxTrk = new TGComboBox(frmMain,-1,kHorizontalFrame | kSunkenFrame | kDoubleBorder | kOwnBackground);
	frmMain->AddFrame(eGCBVtxTrk);
	eGCBVtxTrk->AddEntry("All Tracks",  USE_ALL_TRACKS);
	eGCBVtxTrk->AddEntry("Current View",USE_CURRENT_TRACKS);
	eGCBVtxTrk->Select(USE_ALL_TRACKS);
	eGCBVtxTrk->MoveResize(posx,posy,dx=100,20);

	
	posx+=dx+10;
	TGTextButton *fb = new TGTextButton(frmMain,"Do Vertexing");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAVertexTab", this, "DoVertexingButton()");

	posy+=30;
	posx=20;
	fLabel = new TGLabel(frmMain,"Ntracks");
	fLabel->MoveResize(posx,posy,dx,20);
	posx+=dx+5;
	eGNEVNtrk = new TGNumberEntry(frmMain,eNtrk,11,-1,(TGNumberFormat::EStyle) 5);
	eGNEVNtrk->MoveResize(posx,posy,dx=50,20);

	posx+=dx+10;
	eGCBAngularCut = new TGCheckButton(frmMain,"Angular cut");
	eGCBAngularCut->MoveResize(posx,posy,dx=90,20);
	eGCBAngularCut->SetToolTipText("Select the vertices including a given-angle track.\nAngle and tolerances in Main tab will be used.");

	posx+=dx+10;
	fb = new TGTextButton(frmMain,"Show vertex");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAVertexTab", this, "Redraw()");

	posx+=dx+10;
	fb = new TGTextButton(frmMain,"Clear");
	fb->MoveResize(posx,posy,dx=50,20);
	fb->Connect("Clicked()","EdbEDAVertexTab", this, "Clear()");
	
	frmMain->MapSubwindows();
	frmMain->Resize();
	frmMain->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("Vertexing", 2);
}

void EdbEDAMCTab::Draw(int iMCEvt){
	
	TObjArray *segments = new TObjArray;
	EdbPVRec *ali = gEDA->GetTrackSet("TS")->GetPVRec();
	for(int i=0;i<ali->Npatterns();i++){
		EdbPattern *pat = ali->GetPattern(i);
		for(int j=0;j<pat->N();j++){
			EdbSegP *s = pat->GetSegment(j);
			if(s->MCEvt()==iMCEvt) {
				segments->Add( s);
			}
		}
	}

	EdbEDATrackSet *set = gEDA->GetTrackSet("BT");
	set->Clear();
	set->AddSegments(segments);
	
	gEDA->GetTrackSet("TS")->SetDraw(kFALSE);
	gEDA->GetTrackSet("SB")->SetDraw(kFALSE);
	gEDA->GetTrackSet("SF")->SetDraw(kFALSE);
	gEDA->GetTrackSet("MN")->SetDraw(kFALSE);
	
	gEDA->Redraw();
	return;
}

void EdbEDAMCTab::ShowMCEvt(){
	int iMCEvt = fMCEvt->GetIntNumber();
	Draw(iMCEvt);
}

void EdbEDAMCTab::ShowMCEvtPP(){
	int iMCEvt = fMCEvt->GetIntNumber();
	iMCEvt++;
	fMCEvt->SetNumber(iMCEvt);
	ShowMCEvt();
}
void EdbEDAMCTab::ShowMCEvtMM(){
	int iMCEvt = fMCEvt->GetIntNumber();
	iMCEvt--;
	fMCEvt->SetNumber(iMCEvt);
	ShowMCEvt();
}

void EdbEDAMCTab::MakeGUI(){
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot());
	frmMain->SetWindowName("XX GUI");
	frmMain->SetCleanup(kDeepCleanup);
	frmMain->SetLayoutBroken(kTRUE);

	TGLabel *fLabel;
	int posy=10;
	int posx=10;
	int dx=50;

	fLabel = new TGLabel(frmMain,"MCEvt = ");
	fLabel->MoveResize(posx,posy,dx,20);
	posx+=dx+5;
	fMCEvt = new TGNumberEntry(frmMain,0,11,-1,(TGNumberFormat::EStyle) 5);
	fMCEvt->MoveResize(posx,posy,dx,20);

	posx+=dx+10;
	TGTextButton *fb = new TGTextButton(frmMain,"Previous");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAMCTab", this,"ShowMCEvtMM()");

	posx+=dx+10;
	fb = new TGTextButton(frmMain,"Show");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAMCTab", this,"ShowMCEvt()");

	posx+=dx+10;
	fb = new TGTextButton(frmMain,"Next");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAMCTab", this,"ShowMCEvtPP()");

	frmMain->MapSubwindows();
	frmMain->Resize();
	frmMain->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("MC", 2);
}


void EdbEDAPredTab::WritePred(){
	int i;
	TObjArray *selected = gEDA->GetSelected(); // set TObjArray *selected with EdbSegP
	
	pred_use_angle = fUseAngle->IsOn();
	pred_ipl_from  = fIplFrom->GetIntNumber();
	pred_ipl_to    = fIplTo->GetIntNumber();
	pred_run       = fVolumeRun->GetIntNumber();
	pred_dx   = fVolumeDX->GetIntNumber();
	pred_dy   = fVolumeDY->GetIntNumber();
	
	printf("-----Make prediction------------------\n");
	printf(" run=%d from PL%02d to PL%02d for DX=%d DY=%d, %s\n",
		pred_run, pred_ipl_from, pred_ipl_to, pred_dx, pred_dy,
		pred_use_angle? "using track angle" : "not using track angle");
	
	if(selected->GetEntries()==0){
		printf("Please select segments\n");
		gEve->SetStatusLine(" Error. Please select segments. stop.");
		return;
	}
	
	for(i=0;i<selected->GetEntries();i++){
		EdbSegP *s = (EdbSegP*) selected->At(i);
		s->PrintNice();
	}
	printf(" under construction\n");
	
}


void EdbEDAPredTab::MakeManChkPred(){
	TObjArray *selected = gEDA->GetSelected(); // set TObjArray *selected with EdbSegP
	int i;
	
	IplPropagateTo = fIplPropagateTo->GetIntNumber();
	strcpy(manchkfile,fManChkFile->GetText());
	FILE *fp = fopen(manchkfile,"at");
	
	printf("-----Calc Segment propagation (for Manual Check) -----\n");
	printf(" propagate to PL %d ", IplPropagateTo);
	EdbAffine2D *aff=new EdbAffine2D();
	printf("on Plate coordinate\n");
	if(gEDA->GetAffine(IplPropagateTo)==NULL) {
		printf(" error! no affine parameter for PL%d\n",IplPropagateTo);
		return;
	}
	*aff = *gEDA->GetAffine(IplPropagateTo);
	
	aff->Invert();
	printf(" affine from global to plate\n");
	aff->Print();
	double z=gEDA->GetZ(IplPropagateTo);
	
	for(i=0;i<selected->GetEntries();i++){
		EdbSegP *s0 = (EdbSegP *)selected->At(i);
		EdbTrackP *t = gEDA->GetTrack(s0);
		EdbSegP *s = new EdbSegP(*s0);
		s->PropagateTo(z);
		s->Transform(aff);
		printf("  %8d %8.1lf %8.1lf %7.4lf %7.4lf 0\n", t->ID(),s->X(),s->Y(),s->TX(),s->TY());
		if(NULL!=fp) fprintf(fp,"%8d %8.1lf %8.1lf %7.4lf %7.4lf 0\n", t->ID(),s->X(),s->Y(),s->TX(),s->TY());
		delete s;
	}
	if(fp!=NULL) {
		printf(" values are written in %s\n",manchkfile);
		fclose(fp);
	}
	
}

void EdbEDAPredTab::MakePredScanPred(){
	TObjArray *selected = gEDA->GetSelected(); // set TObjArray *selected with EdbSegP
	int i;
	
	IplPropagateTo = fIplPropagateTo->GetIntNumber();
	
	printf("-----Calc Segment propagation (for Manual Check) -----\n");
	printf(" propagate to PL %d ", IplPropagateTo);
	EdbAffine2D *aff=new EdbAffine2D();
	printf("on Plate coordinate\n");
	if(gEDA->GetAffine(IplPropagateTo)==NULL) {
		ErrorMessage(Form(" error! no affine parameter for PL%d\nStop.",IplPropagateTo));
		return;
	}
	*aff = *gEDA->GetAffine(IplPropagateTo);

	aff->Invert();
	printf(" affine from global to plate\n");
	aff->Print();

	double z=gEDA->GetZ(IplPropagateTo);
	
	char filename[200];
	int ver=1;
	if(gSystem->AccessPathName(Form("../p%03d", IplPropagateTo))){
		ErrorMessage(Form("There is no directory : ../p%03d\nCheck the directory structure.\n Stop", IplPropagateTo));
		return;
	}


	sprintf(filename, "../p%03d/%d.%d.%d.%d.pred.root", IplPropagateTo, gEDA->GetBrick(), IplPropagateTo, ver, (int)fPredRun->GetIntNumber());
	if(!gSystem->AccessPathName(filename)){
		int ret;
		new TGMsgBox(gClient->GetRoot(),gEve->GetMainWindow(),"Confirmation", 
			Form("%s already exist.\nDo you want to overwrite?",filename), kMBIconAsterisk, kMBYes|kMBNo,&ret);
		if(ret==kMBNo) return;
	}

	TFile f(filename,"recreate");
	EdbPattern pat;
	for(i=0;i<selected->GetEntries();i++){
		EdbSegP *s0 = (EdbSegP *) selected->At(i);
		EdbTrackP *t = gEDA->GetTrack(s0);
		EdbSegP *s = new EdbSegP(*s0);
		s->SetID(t->ID());
		s->PropagateTo(z);
		s->Transform(aff);
		s->SetErrors(50.,50.,0.,0.6,0.6);
		
		pat.AddSegment(*s);
		
		delete s;
	}
	
	pat.Write("pat");
	f.Close();
	
	gEve->SetStatusLine(Form("%s is written.",filename));
	printf("%s is written.",filename);
	
}

void EdbEDAPredTab::MakeManChkPredGivenAngleFromVertex(int id, double ax, double ay){
	EdbVertex *v=gEDA->GetSelectedVertex();
	if(NULL==v) {
		printf("select a vertex!\n");
		return;
	}
	if(ax>100){
		ax = fTx->GetNumber();
		ay = fTy->GetNumber();
	}
	
	IplPropagateTo = fIplPropagateTo->GetIntNumber();
	strcpy(manchkfile,fManChkFile->GetText());
	FILE *fp = fopen(manchkfile,"at");
	
	printf("-----Make Manual Check Prediction for given angle track -----\n");
	printf(" propagate to PL %d ", IplPropagateTo);
	EdbAffine2D *aff=new EdbAffine2D();
	printf("on Plate coordinate\n");
	if(aff==NULL) {
		printf(" error! no affine parameter for PL%d\n",IplPropagateTo);
		return;
	}
	*aff = *gEDA->GetAffine(IplPropagateTo);
	aff->Invert();
	printf(" affine from global to plate\n");
	aff->Print();

	double z=gEDA->GetZ(IplPropagateTo);
	
	EdbSegP *s = new EdbSegP(id, v->X(), v->Y(), ax,ay);
	s->SetZ(v->Z());
	s->PropagateTo(z);
	s->Transform(aff);
	printf("  %8d %8.1lf %8.1lf %7.4lf %7.4lf 0\n", s->ID(),s->X(),s->Y(),s->TX(),s->TY());
	if(NULL!=fp) fprintf(fp,"%8d %8.1lf %8.1lf %7.4lf %7.4lf 0\n", s->ID(),s->X(),s->Y(),s->TX(),s->TY());

	if(fp!=NULL) {
		printf(" values are written in %s\n",manchkfile);
		fclose(fp);
	}
	
}



void EdbEDAPredTab::RemoveFile(){
	gSystem->Exec(Form("rm %s",fManChkFile->GetText()));
	printf(Form("rm %s\n",fManChkFile->GetText()));
}

void EdbEDAPredTab::SetManChkFile(Long_t ipl){
	fManChkFile->SetText( Form("manchkpred_pl%02d.txt", fIplPropagateTo->GetIntNumber()) );
}


void EdbEDAPredTab::MakeGUI(){
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot());
	frmMain->SetWindowName("XX GUI");
	frmMain->SetCleanup(kDeepCleanup);

	TGLabel *fLabel;
	int posy=5;
	int posx=10;
	int dx=50;

	fLabel = new TGLabel(frmMain,"ManChk pred");
	fLabel->MoveResize(posx,posy,dx=90,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"Propagate to PL =");
	fLabel->MoveResize(posx,posy,dx=100,20);
	posx+=dx+5;
	fIplPropagateTo = new TGNumberEntry(frmMain,IplPropagateTo,11,-1,(TGNumberFormat::EStyle) 5);
	fIplPropagateTo->MoveResize(posx,posy,dx=40,20);
	fIplPropagateTo->Connect("ValueSet(Long_t)", "EdbEDAPredTab", this, "SetManChkFile(Long_t)");

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"Output >>");
	fLabel->MoveResize(posx,posy,dx=60,20);
	posx+=dx+5;
	fManChkFile = new TGTextEntry(frmMain, Form("manchkpred_pl%02d.txt", IplPropagateTo));
	fManChkFile->MoveResize(posx,posy,dx=140,20);


	posx+=dx+5;
	TGTextButton *fb = new TGTextButton(frmMain,"Add");
	fb->MoveResize(posx,posy,dx=50,18);
	fb->Connect("Clicked()","EdbEDAPredTab",this,"MakeManChkPred()");

	posx+=dx+5;
	fb = new TGTextButton(frmMain,"rm file");
	fb->MoveResize(posx,posy,dx=50,18);
	fb->Connect("Clicked()","EdbEDAPredTab",this,"RemoveFile()");

	posx+=dx+10;
	fLabel = new TGLabel(frmMain,"Run");
	fLabel->MoveResize(posx,posy,dx=25,20);
	posx+=dx+5;
	fPredRun = new TGNumberEntryField(frmMain,-1, 200);
	fPredRun->MoveResize(posx,posy,dx=35,20);
	fPredRun->SetToolTipText("For ScanForth, 200, 300 is recommended.");

	posx+=dx+5;
	fb = new TGTextButton(frmMain,"Pred Scan");
	fb->MoveResize(posx,posy,dx=70,18);
	fb->Connect("Clicked()","EdbEDAPredTab",this,"MakePredScanPred()");
	fb->SetToolTipText("Make Prediction-Scan prediction\nin ../plnum/ibrick.ipl.1.run.root");
	

	posy+=22;
	posx=150;

	fLabel = new TGLabel(frmMain,"Tx, Ty =");
	fLabel->MoveResize(posx,posy,dx=50,20);
	
	posx+=dx+10;
	fTx = new TGNumberEntryField(frmMain,-1, 0.0);
	fTx->SetFormat(TGNumberFormat::kNESRealThree);
	fTx->MoveResize(posx,posy,dx=45,18);
	posx+=dx+10;
	fTy = new TGNumberEntryField(frmMain,-1, 0.0);
	fTy->SetFormat(TGNumberFormat::kNESRealThree);
	fTy->MoveResize(posx,posy,dx=45,18);

	posx+=dx+20;
	fb = new TGTextButton(frmMain,"Pred with given angle from vertex");
	fb->MoveResize(posx,posy,dx=200,18);
	fb->Connect("Clicked()","EdbEDAPredTab",this,"MakeManChkPredGivenAngleFromVertex()");


	posy+=22;
	posx=10;
	fLabel = new TGLabel(frmMain,"Volume Scan");
	fLabel->MoveResize(posx,posy,dx=90,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"Run");
	fLabel->MoveResize(posx,posy,dx=30,20);
	posx+=dx+5;
	fVolumeRun = new TGNumberEntryField(frmMain,-1, pred_run);
	fVolumeRun->MoveResize(posx,posy,dx=45,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"from PL ");
	fLabel->MoveResize(posx,posy,dx=50,20);
	posx+=dx+5;
	fIplFrom = new TGNumberEntry(frmMain,pred_ipl_from,11,-1,(TGNumberFormat::EStyle) 5);
	fIplFrom->MoveResize(posx,posy,dx=40,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"to PL");
	fLabel->MoveResize(posx,posy,dx=30,20);
	posx+=dx+5;
	fIplTo = new TGNumberEntry(frmMain,pred_ipl_to,11,-1,(TGNumberFormat::EStyle) 5);
	fIplTo->MoveResize(posx,posy,dx=40,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"DX=");
	fLabel->MoveResize(posx,posy,dx=30,20);
	posx+=dx+5;
	fVolumeDX = new TGNumberEntryField(frmMain,-1, pred_dx);
	fVolumeDX->MoveResize(posx,posy,dx=45,20);

	posx+=dx+5;
	fLabel = new TGLabel(frmMain,"DY=");
	fLabel->MoveResize(posx,posy,dx=30,20);
	posx+=dx+5;
	fVolumeDY = new TGNumberEntryField(frmMain,-1, pred_dy);
	fVolumeDY->MoveResize(posx,posy,dx=45,20);


	posx+=dx+5;
	fUseAngle = new TGCheckButton(frmMain,"Use angle");
	fUseAngle->MoveResize(posx,posy,dx=90,20);
	fUseAngle->SetState(pred_use_angle ? kButtonDown : kButtonUp);
	
	posx+=dx+5;
	fb = new TGTextButton(frmMain,"Write");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()", "EdbEDAPredTab", this, "WritePred()");


//	posx+=dx+10;
	
	frmMain->MapSubwindows();
	frmMain->Resize();
	frmMain->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("Prediction", 2);
}

EdbEDAOperationTab :: EdbEDAOperationTab (void) : eLastOperation(kNone){
	browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	frame = new TGMainFrame(gClient->GetRoot());
	frame->SetWindowName("XX GUI");
	frame->SetCleanup(kDeepCleanup);

	TGLabel *fLabel;
	TGTextButton *fb;

	int posy=10;
	int posx=10;
	int dx=0;
	fLabel = new TGLabel(frame,"For Track :");
	fLabel->MoveResize(posx,posy,dx=80,20);

	posx+=dx+10;
	fb = new TGTextButton(frame,"Connect");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"ConnectTracksButton()");

	posx+=dx+10;
	fb = new TGTextButton(frame,"Disconnect");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"DisconnectTracks()");

	posx+=dx+10;
	fb = new TGTextButton(frame,"RemoveSegment");
	fb->MoveResize(posx,posy,dx=100,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"RemoveSegment()");

	posx+=dx+20;
	fb = new TGTextButton(frame,"Attach to Vtx");
	fb->MoveResize(posx,posy,dx=100,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"AttachToVertexButton()");
	fb->SetToolTipText(	"Attach selected tracks to the selected vertex (red).\n"
						"the vertex position will not be re-calculated.");


	posx+=dx+20;
	fb = new TGTextButton(frame,"Move to TrackSet");
	fb->MoveResize(posx,posy,dx=100,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"MoveToTrackSetButton()");
	fb->SetToolTipText("Move current selected tracks to a different track set.");

	posx+=dx+30;
	fb = new TGTextButton(frame,"Dump text");
	fb->MoveResize(posx,posy,dx=100,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"DumpTracksButton()");
	fb->SetToolTipText("Dump all segments of the selected tracks to a text file.\nthis would be usefull when you want to show a volume scan data with another volume scan data.");

/*	posx+=dx+10;
	fLabel = new TGLabel(frame,"Run");
	fLabel->MoveResize(posx,posy,dx=25,20);
	posx+=dx+5;
	fRun = new TGNumberEntryField(frame,-1, 2000);
	fRun->MoveResize(posx,posy,dx=45,20);
	fRun->SetToolTipText("Run number.");
*/


	posy+=20;
	posx=10;
	fb = new TGTextButton(frame,"Redo");
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDAOperationTab",this,"Redo()");
	
	frame->MapSubwindows();
	frame->Resize();
	frame->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("Operation", 2);
}


void EdbEDAOperationTab::AttachToVertex(EdbVertex *v, TObjArray *tracks){
	ErrorMessage("Under construction.");
}
void EdbEDAOperationTab::AttachToVertexButton(){
	ErrorMessage("Under construction.");
}

void EdbEDAOperationTab::MoveToTrackSetButton(){
	
	if( gEDA->NSelectedTracks()==0) {
		ErrorMessage("Please select one or more tracks.");
		return;
	}
	
	int isel;
	new EdbEDATrackSetList(gEDA->GetTrackSets(), &isel, "Select Track set");
	EdbEDATrackSet *set = isel!=-1 ? gEDA->GetTrackSet(isel) : NULL;
	
	
	if(set==NULL){
		ErrorMessage("TrackSet is not selected. stop.");
		return;
	}
	
	printf("Selected = %s\n", set->GetName());
	for(int i=0;i<gEDA->NSelectedTracks(); i++){
		EdbTrackP *t = gEDA->GetSelectedTrack(i);
		EdbEDATrackSet *set0 = gEDA->GetTrackSet(t);
		if(set==set0) continue;
		set->AddTrack(t);
		set0->RemoveTrack(t);
		set0->RemoveTrackBase(t);
		printf("track %5d move from TrackSet %s to %s.\n", t->ID(), set0->GetName(), set->GetName());
	}
	gEDA->Redraw();
	
}


// Connect 2 tracks in one.
// t1 is connected into t0, and t1's segment will be cleared.
void EdbEDAOperationTab::ConnectTracks(EdbTrackP *t0, EdbTrackP *t1){

	printf("Connect track %d (pl %02d -> %02d) and track %d (pl %02d -> %02d), remove %d\n",
		t0->ID(), t0->GetSegmentFirst()->Plate(), t0->GetSegmentLast()->Plate(),
		t1->ID(), t1->GetSegmentFirst()->Plate(), t1->GetSegmentLast()->Plate(), t1->ID());
	int i,j;
	
	// store values for redo
	eT0org=new EdbTrackP;
	eT0org->Copy(*t0);
	eT0p = t0;
	eT1p = t1;
	eSet = gEDA->GetTrackSet(t1);
	eLastOperation = kConnect;
	
	// add segments from t1 into t0.
	for(i=0;i<t1->N();i++) {
		EdbSegP *s1 = t1->GetSegment(i);
		EdbSegP *s0;
		int flag_double=0;
		for(j=0;j<t0->N();j++) { // check double entry in a plate.
			s0 = t0->GetSegment(j);
			if(s1->PID()==s0->PID()) {
				flag_double=1;
				break;
			}
		}
		if(flag_double){ 
			// doesn't do anything.
			
			/*
			// if 2 segment in a plate, put priority on smaller flag. as result, priority SF>SB>MAN>TS.
			if(t0->Flag()<=t1->Flag()) {} 
			else {
				t0->RemoveSegment(s0);
				t0->AddSegment(s1);
			}
			*/
		}
		else t0->AddSegment(s1);
	}
	
	t0->SetNpl();
	t0->SetSegmentsTrack();
	t0->SetCounters();
	
	// set t1->N() = 0
	gEDA->RemoveTrackBase(t1);
}

// Connect 2 or more tracks into one.
// all track will be sorted and upstream track will be the parent.
void EdbEDAOperationTab::ConnectTracksButton(void){
	
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	if(selected_tracks->GetEntries()<=1){
		printf("Select 2 tracks (segments) to be connected.\n");
		return;
	}
	
	// t0 is the parent
	EdbTrackP *t0 = (EdbTrackP *) selected_tracks->At(0);
	EdbTrackP *t1 = (EdbTrackP *) selected_tracks->At(1);

	EdbEDATrackSet *set0 = gEDA->GetTrackSet(t0);
	EdbEDATrackSet *set1 = gEDA->GetTrackSet(t1);
	
	// if 2 tracks are from same TrackSet, sort by Z
	if( set0==set1 ){
		// soarted by the Z of first segment. (EdbTrackP is sortable)
		selected_tracks->Sort();
		// most upstream segment is the parent by default
		t0 = (EdbTrackP *) selected_tracks->At(0);
		t1 = (EdbTrackP *) selected_tracks->At(1);
	}
	
	// if the t0 is from BT && t1 from TS, set parent as TS.
	if(set0==gEDA->GetTrackSet("BT")&&set1==gEDA->GetTrackSet("TS")){
		EdbTrackP *temp=t1;
		t1=t0; t0=temp;
	}
	
	ConnectTracks(t0,t1); // merge t1 into t0.
	
	gEDA->Redraw();
}


// Separate a track in to 2 tracks.

void EdbEDAOperationTab::DisconnectTracks(void){
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	TObjArray *selected = gEDA->GetSelected();
	EdbTrackP *t = (EdbTrackP *) selected_tracks->At(0);
	EdbSegP *ss = (EdbSegP *) selected->At(0);
	
	
	if(t==NULL||ss==NULL) {
		ErrorMessage("Select a segment! (most upstream segment of downstream track)");
		return;
	}
	if(!IsSegment(ss)){
		ErrorMessage("Select a segment! (most upstream segment of downstream track)");
		return;
	}
	
	if(ss==t->GetSegmentFirst()) {
		ErrorMessage("Select a segment! (most upstream segment of downstream track)");
		return;
	}
	
	printf("Disconnect a track into 2 tracks by a selected segment.\n");
	
	
	EdbEDATrackSet *set = gEDA->GetTrackSet(t);
	
	EdbTrackP *tnew = new EdbTrackP;
	set->AddTrack(tnew);
	
	tnew->Set(set->N()-1, ss->X(),ss->Y(),ss->TX(),ss->TY(),ss->W(),ss->Flag());
	tnew->SetZ(ss->Z());
	tnew->SetPID(ss->PID());
	
	eT0org=new EdbTrackP;
	eT0org->Copy(*t); // store values for Redo.
	eT0p = t;
	eT1p = tnew;
	eLastOperation = kDisconnect;
	
	for(int i=0;i<t->N();i++){
		EdbSegP *s = t->GetSegment(i);
		EdbSegP *sf = t->GetSegmentF(i);
		if(s->Compare(ss)>=0) {
			t->RemoveSegment(s);
			//t->RemoveSegmentF(s);
			tnew->AddSegment(s);
			tnew->AddSegmentF(sf);
			i--;
		}
	}
	tnew->SetSegmentsTrack();
	tnew->SetCounters();
	
	gEDA->Redraw();
}

void EdbEDAOperationTab::RemoveSegment(){
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	TObjArray *selected = gEDA->GetSelected();
	EdbTrackP *t = (EdbTrackP *) selected_tracks->At(0);
	EdbSegP *ss = (EdbSegP *) selected->At(0);
	
	if(t->N()==1){
		ErrorMessage("There is only one segment. Cannot remove.");
		return;
	}
	
	if(t==NULL||ss==NULL) {
		ErrorMessage("Select a segment! \n");
		return;
	}
	if(!IsSegment(ss)){
		ErrorMessage("Select a segment! \n");
		return;
	}
	
	printf("Remove Segment Disconnect a track into 2 tracks by a selected segment.\n");
	
	eT0org=new EdbTrackP;
	eT0org->Copy(*t); // store values for Redo.
	eT0p=t;

	t->RemoveSegment(ss);
	eLastOperation = kRemoveSegment;
	
	t->SetSegmentsTrack();
	t->SetCounters();
	
	gEDA->Redraw();
}

void EdbEDAOperationTab::Redo(void){
	if(eLastOperation==kNone) return;
	if(eLastOperation==kConnect){
		*eT0p=*eT0org;
		eSet->AddTrack(eT1p); // 
	}
	if(eLastOperation==kDisconnect){
		*eT0p=*eT0org; // 
		gEDA->RemoveTrackBase(eT1p);
	}
	if(eLastOperation==kRemoveSegment){
		*eT0p=*eT0org;
	}
	
	eLastOperation = kNone;
	gEDA->Redraw();
}

void EdbEDAOperationTab::DumpTrack(EdbTrackP *t, FILE *fp, int run, int plate_coordinate){

	for(int i=0; i<t->N(); i++){
		EdbSegP *s0 = t->GetSegment(i);
		EdbSegP *s = new EdbSegP(*s0);
		int ipl = s->Plate();
		if(plate_coordinate){
			EdbAffine2D *aff0 = gEDA->GetAffine(ipl);
			EdbAffine2D *aff = new EdbAffine2D(*aff0);
			aff->Invert();
			s->Transform(aff);
			delete aff;
		}
		fprintf(fp,"%5d %2d %2d %8.1f %8.1f %7.4f %7.4f %d %4d\n",
			t->ID(), ipl, (int)s->W(), s->X(), s->Y(), s->TX(), s->TY(), plate_coordinate, run);
		delete s;
	}
		
}

void EdbEDAOperationTab::DumpTracksButton(void){
	FILE *fp;
	
	char defaultname[256];
	if(gEDA->NSelectedTracks()==1) sprintf(defaultname,"track%d.txt", gEDA->GetSelectedTrack()->ID());
	else sprintf(defaultname,"track.txt");
	
	TGFileInfo *fi=new TGFileInfo;
	fi->fIniDir    = StrDup(".");
	const char *filetypes[] = { "track text", "*.txt", "All files","*",0,0};
	fi->fFileTypes = filetypes;
	fi->fFilename = defaultname;
	new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);

	if(fi->fFilename==NULL) return;
	if(strlen(fi->fFilename)==0) return;

	int plate_coordinate = EdbEDAUtil::AskYesNo("Save in plate codinate?");
	
	
	fp=fopen(fi->fFilename,"wt");
	
	if(fp==NULL) {
		ErrorMessage(Form("File open error : %s\n", fi->fFilename));
		return;
	}
	
	for(int i=0; i<gEDA->NSelectedTracks();i++){
		DumpTrack(gEDA->GetSelectedTrack(i), fp, 99, plate_coordinate);
		//DumpTrack(gEDA->GetSelectedTrack(i), fRun->GetIntNumber(), fp);
	}
	
	fclose(fp);
	
	printf("%d tracks was writen in %s\n", gEDA->NSelectedTracks(), fi->fFilename);
	delete fi;
}

void EdbEDATrackSetList::SetSelected(){ 
	if(fSelected) *fSelected = fListBox->GetSelected(); 
	CloseWindow();
}

EdbEDATrackSetList::~EdbEDATrackSetList(){
	printf("Selected = %d %s\n", *fSelected, 
		*fSelected!=-1? ((TNamed *)(fTrackSets->At(*fSelected)))->GetName():"Not-selected");
	Cleanup();
}

EdbEDATrackSetList::EdbEDATrackSetList(TObjArray *tracksets, int *iselected, char *message) : 
	TGTransientFrame(gClient->GetRoot(),gEve?gEve->GetMainWindow():0, 10,10,kMainFrame | kVerticalFrame), 	fTrackSets(tracksets), fSelected(iselected){
	if(fSelected) *fSelected = -1;
	
	CenterOnParent();
	// vertical frame
	fVerticalFrame = new TGVerticalFrame(this,157,230,kVerticalFrame);

	// list box
	fListBox = new TGListBox(fVerticalFrame);
	
	for(int i=0;i<fTrackSets->GetEntries();i++){
		fListBox->AddEntry( ((TNamed *)fTrackSets->At(i))->GetName(), i);
	}
	fListBox->Resize(104,100);
	fVerticalFrame->AddFrame(fListBox, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop,10,2,10,2));
	
	TGTextButton *fb = new TGTextButton(fVerticalFrame,"OK");
	fVerticalFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop,10,2,10,2));
	fb->Connect("Clicked()", "EdbEDATrackSetList", this, "SetSelected()");
	
	fb = new TGTextButton(fVerticalFrame,"Cancel");
	fb->Resize(89,22);
	fVerticalFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsCenterX | kLHintsTop,10,2,5,2));
	fb->Connect("Clicked()", "TGMainFrame", this, "CloseWindow()");
	
	AddFrame(fVerticalFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
	
	SetMWMHints(kMWMDecorAll,
	                    kMWMFuncAll,
	                    kMWMInputModeless);
	MapSubwindows();
	Resize(GetDefaultSize());
	MapWindow();
	fListBox->Select(0);
	gClient->WaitFor(this);
}



