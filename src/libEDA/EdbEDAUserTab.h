#ifndef __EDA_UserTAB_H__
#define __EDA_UserTAB_H__

class EdbEDAUserTab{
	private:
	TGTextButton *eButton1, *eButton2, *eButton3;
	public:
	EdbEDAUserTab(){ 
		MakeGUI();
	}
	~EdbEDAUserTab(){}
	void DoButton1();
	void DoButton2(){ printf("Button2\n");}
	void DoButton3(){ printf("Button3\n");}
};

void EdbEDAUserTab::DoButton1(){
	printf("\nUserTab::DoButton1()\n");
	printf("%d segments are selected\n", gEDA->NSelected());
	for(int i=0;i<gEDA->NSelected();i++) gEDA->GetSelected(i)->PrintNice();

	printf("%d tracks are selected\n",   gEDA->NSelectedTracks());
	for(int i=0;i<gEDA->NSelectedTracks();i++) {
		EdbTrackP *t = gEDA->GetSelectedTrack(i);
		EdbEDATrackSet *set = gEDA->GetTrackSet(t);
		printf("TrackSet : %s\n", set->GetName());
		t->PrintNice();
	}
}

void EdbEDAUserTab::MakeGUI(){
	
	printf("EdbEDAUserTab example.\n");
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGMainFrame* frame = new TGMainFrame(gClient->GetRoot());
	frame->SetWindowName("XX GUI");
	frame->SetCleanup(kDeepCleanup);
	frame->SetLayoutBroken(kTRUE);
	
	int posy=10, posx=10, dx;

	TGLabel *fLabel = new TGLabel(frame,"User GUI : ");
	frame->AddFrame(fLabel);
	fLabel->MoveResize(posx,posy,dx=80,20);
	posx+=dx+10;

	eButton1 = new TGTextButton(frame,"B1. Print Selected");
	frame->AddFrame(eButton1);
	eButton1->MoveResize(posx,posy,dx=150,20);
	eButton1->Connect("Clicked()","EdbEDAUserTab", this,"DoButton1()");

	posx+=dx+10;
	eButton2 = new TGTextButton(frame,"Button2");
	frame->AddFrame(eButton2);
	eButton2->MoveResize(posx,posy,dx=80,20);
	eButton2->Connect("Clicked()","EdbEDAUserTab", this,"DoButton2()");

	posx+=dx+10;
	eButton3 = new TGTextButton(frame,"Button3");
	frame->AddFrame(eButton3);
	eButton3->MoveResize(posx,posy,dx=80,20);
	eButton3->Connect("Clicked()","EdbEDAUserTab", this,"DoButton3()");

	frame->MapSubwindows();
	frame->Resize();
	frame->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("User", 2);
}


#endif //__EDA_UserTAB_H__
