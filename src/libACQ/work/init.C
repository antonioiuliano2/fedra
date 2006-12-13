EdbRun* run=0;
#include "hwinit.C"
//#include "run.C"

int imageat(float Z0) //Take one image at a given Z
{
	int res;
	int Taken;
	float DZ=s->NominalEmLayer; //microns in stage space
	float Speed=1.*FPS*DZ/(Layers-1);
	printf("Taking BG: Z Started to %f..\n",Z0-50);
	s->Z->PosMove(Z0-50,1e4,1e4);
	res=s->Z->WaitForMoveComplete(1e4);
	s->Z->ArmBreakpoint(Z0);
	printf("Taking BG: Grab armed,Z Started to %f..\n",Z0+10);
	s->Z->PosMove(Z0+10,Speed,50000);
	Taken=o->GrabImagesONL(1,&Z0,200./Speed); //grab one image with timeout
	res=s->Z->WaitForMoveComplete(1e4);
    o->CopyImageToBG(0);
    return Taken;
}

void hist()
{
	o->ApplyGainCorr=1;
	o->ApplyFIRF=0;
	o->Threshold=0;
	o->SubtractBG=false;
	Z0=480;
	DZ=s->EmTopLayer/4.;
	TS->Layers=3;
    TS->FPS=FPS;
	TH1F* h=new TH1F("PPH","PPH",256,0,256);
	TCanvas *c=new TCanvas();
	c->SetLogy();
	c->Draw();
	while(1)
	{
	TS->ScanView(Z0,DZ);
	h->Reset();
	o->PlotImage1D(0,h);
	o->ShowImage(0);
	h->Draw();
    c->Update();
	}

}
int scantop()
{
//	o->ApplyGainCorr=false;
//	o->ApplyFIRF=false;
//	o->Threshold=0;
//	o->SubtractBG=false;
	Z0=s->Z->Reference+s->EmBotLayer+s->EmBase;
	DZ=s->EmTopLayer;
	return scanview();
}
int scanbot()
{
	Z0=s->Z->Reference;
	DZ=s->EmBotLayer;
	return scanview();
}
int scanview()
{

	TS->Layers=Layers;
    TS->FPS=FPS;
	return TS->ScanView(Z0,DZ);
}




float ThTol=0.5; //Emulsion thickness tolerance
int focus(int clu_thre=300)
{
	// finds emulsion starting from nominal values
	int res;
	int Taken;
	float z0=0,z1=0;

	o->Threshold=ThrTop;
	Z0=s->Z->NominalReference +s->NominalEmBase ;
	DZ=3*s->NominalEmLayer;
	res=TS->FindLayer(Z0,DZ,clu_thre,&z0,&z1);
	if(res<3 ) {printf("res=%d :One or both surfaces not detected!\n",res); return 0;};

	float z2=0,z3=0;
		o->Threshold=ThrBot;
	Z0=s->Z->NominalReference-s->NominalEmLayer;
	DZ=3*s->NominalEmLayer;
	res=TS->FindLayer(Z0,DZ,clu_thre,&z2,&z3);
	if(res<3 ) {printf("res=%d :One or both surfaces not detected!\n",res); return 0;};
	if( (z0-z1)/s->NominalEmLayer <1-ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z0-z1)/s->NominalEmLayer >1+ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z2-z3)/s->NominalEmLayer <1-ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z2-z3)/s->NominalEmLayer >1+ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z1-z2)/s->NominalEmBase  <1-ThTol) { printf("Base thickness out of tolerance!\n"); return 0;}
	if( (z1-z2)/s->NominalEmBase  >1+ThTol) { printf("Base thickness out of tolerance!\n"); return 0;}
	s->Z->Reference=z3;
    s->EmTopLayer=z0-z1;
    s->EmBotLayer=z2-z3;
    s->EmBase=z1-z2;
	printf("Reference set to %f, Emulsion(Top:Base:Bot)  %0.1f : %0.1f : %0.1f \n",s->Z->Reference,s->EmTopLayer,s->EmBase,s->EmBotLayer);

    return 1;
}
int finefocus(int clu_thre=300, float fineness=4.)
{
// finds emulsion starting from current values of Z.
// fineness defines search depth: DZ=(1+2./fineness)*s->NominalEmLayer
	int res;
	int Taken;
	int FLayers=o->MaxNbGrab;
	TS->Layers=FLayers;
	float z0=0,z1=0;

		o->Threshold=ThrTop;
	Z0=s->Z->Reference +s->EmBotLayer +s->EmBase -s->EmTopLayer/fineness;
	if(Z0<=0) Z0=10.;// safety margin
	DZ=(1+2./fineness)*s->EmTopLayer;
	res=TS->FindLayer(Z0,DZ,clu_thre,&z0,&z1);
	if(res<3 ) {printf("res=%d :One or both surfaces not detected!\n",res); return 0;};
	float z2=0,z3=0;
		o->Threshold=ThrBot;
	Z0=s->Z->Reference-s->EmBotLayer/fineness;
	if(Z0<=0) Z0=10.;// safety margin
	DZ=(1+2./fineness)*s->NominalEmLayer;
    res=TS->FindLayer(Z0,DZ,clu_thre,&z2,&z3);
	if(res<3 ) {printf("res=%d :One or both surfaces not detected!\n",res); return 0;};
	if( (z0-z1)/s->NominalEmLayer <1-ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z0-z1)/s->NominalEmLayer >1+ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z2-z3)/s->NominalEmLayer <1-ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z2-z3)/s->NominalEmLayer >1+ThTol) { printf("Layer thickness out of tolerance!\n"); return 0;}
	if( (z1-z2)/s->NominalEmBase  <1-ThTol) { printf("Base thickness out of tolerance!\n"); return 0;}
	if( (z1-z2)/s->NominalEmBase  >1+ThTol) { printf("Base thickness out of tolerance!\n"); return 0;}
	s->Z->Reference=z3;
    s->EmTopLayer=z0-z1;
    s->EmBotLayer=z2-z3;
    s->EmBase=z1-z2;
	printf("Reference set to %f, Emulsion(Top:Base:Bot)  %0.1f : %0.1f : %0.1f \n",s->Z->Reference,s->EmTopLayer,s->EmBase,s->EmBotLayer);
    return 1;
}


takebggc(float X0, float Y0, float Z0)
{
	s->L->ON();
	float ZC=s->Z->GetPos();
	s->Z->PosMove(ZC+10000,1e6,1e6);
	s->Z->WaitForMoveComplete(1e4);

	s->X->PosMove(X0,1e6,1e6);
	s->Y->PosMove(Y0,1e6,1e6);
	s->X->WaitForMoveComplete(1e4);
	s->Y->WaitForMoveComplete(1e4);
	s->Z->PosMove(Z0,1e6,1e6);
	s->Z->WaitForMoveComplete(1e4);

//// Take BG image for GC map
	o->ApplyGainCorr=false;
	o->ApplyFIRF=false;
	o->Threshold=0;
	o->SubtractBG=false;
	if(imageat(Z0)<1) {printf("Can't take image! Abort.\n"); return 0 ;};
    o->CopyImageToGC(0,0xC8);

//// Take BG image 
	o->ApplyGainCorr=true;
	o->ApplyFIRF=true;
	o->Threshold=0;
	o->SubtractBG=false;
	if(imageat(Z0)<1) {printf("Can't take image! Abort.\n"); return 0 ;};
    return 1;
}

takebggchere()
{
	float Z0=s->Z->GetPos();
//// Take BG image for GC map
	o->ApplyGainCorr=false;
	o->ApplyFIRF=false;
	o->Threshold=0;
	o->SubtractBG=false;
	if(imageat(Z0)<1) {printf("Can't take image! Abort.\n"); return 0 ;};
    o->CopyImageToGC(0,0xC8);

//// Take BG image 
	o->ApplyGainCorr=true;
	o->ApplyFIRF=true;
	o->Threshold=0;
	o->SubtractBG=false;
	if(imageat(Z0)<1) {printf("Can't take image! Abort.\n"); return 0 ;};
    return 1;
}


void lightcurve(int INCREMENT)
{
	int L0=s->L->LightLevel;
	TCanvas *c=new TCanvas();
		TH1F* h=new TH1F("PPH","PPH",256,0,256);

	TH1F* Uh=new TH1F("Uh","",(LLMAX-LLMIN)/INCREMENT,LLMIN,LLMAX);
	Uh->SetLineColor(kBlue); //Underflows
	TH1F* Oh=new TH1F("Oh","",(LLMAX-LLMIN)/INCREMENT,LLMIN,LLMAX);
	Oh->SetLineColor(kRed); //Overflows
	TH1F* Mh=new TH1F("Mh","",(LLMAX-LLMIN)/INCREMENT,LLMIN,LLMAX);
	Mh->SetLineColor(kGreen); //Mean
	TH1F* Rh=new TH1F("Rh","",(LLMAX-LLMIN)/INCREMENT,LLMIN,LLMAX);
	Rh->SetLineColor(kBlack); //RMS

  for(int LL=LLMIN; LL<LLMAX; LL+=INCREMENT)
  {
    s->L->LightLevel=LL; 
	s->L->ON();
	gSystem->Sleep(LDelay);
//// Take a raw image
	o->ApplyGainCorr=false;
	o->ApplyFIRF=false;
	o->Threshold=0;
	o->SubtractBG=false;
	o->GrabOneImage(s->Z->GetPos(),5);
// Try to figure out what the image looks like...
	h->Reset();
	o->PlotImage1D(0, h);
//	h->Draw(); c->Update();
	float Underflows=h->GetBinContent(0)+h->GetBinContent(1)+h->GetBinContent(2);
	float Overflows=h->GetBinContent(257)+h->GetBinContent(256)+h->GetBinContent(255);
	float Mean=h->GetMean();
	float RMS=h->GetRMS();
	Uh->Fill(LL,TMath::Log(Underflows+1)/TMath::Log(10.)*20.);
	Oh->Fill(LL,TMath::Log(Overflows+1)/TMath::Log(10.)*20.);
	Mh->Fill(LL,Mean);
	Rh->Fill(LL,RMS);
	Mh->Draw(); Rh->Draw("same"); Oh->Draw("same"); Uh->Draw("same");
	c->Update();
  }
  	s->L->LightLevel=L0; s->L->ON();

}

int tunelight()
{
	TH1F* h=new TH1F("PPH","PPH",256,0,256);
    bool Stop=false;
	int LL;
	LL=LLMAX;
	int Dir=-1; // start to decrease light
	TCanvas *c=new TCanvas();
while(!Stop)
	{
    s->L->LightLevel=LL; 
	s->L->ON();
	gSystem->Sleep(LDelay);
//// Take a raw image
	o->ApplyGainCorr=false;
	o->ApplyFIRF=false;
	o->Threshold=0;
	o->SubtractBG=false;
	o->GrabOneImage(s->Z->GetPos(),5);
// Try to figure out what the image looks like...
	h->Reset();
	o->PlotImage1D(0, h);
	h->Draw(); c->Update();
	float Underflows=h->GetBinContent(0)+h->GetBinContent(1)+h->GetBinContent(2);
	float Overflows=h->GetBinContent(257)+h->GetBinContent(256)+h->GetBinContent(255);
	float Mean=h->GetMean();
	float RMS=h->GetRMS();
    
	if(Underflows>10000) { LL+=Dir*500; continue; }
	if(Underflows>100) { LL+=Dir*200; continue; }
	if(Underflows>10) { LL+=Dir*20; continue; }
 	if(Overflows>100) { printf("Image profile saturates from both sides! Can't set up light..\n"); return 0; }
	Stop=1;
	}
	printf("Light is set to :                     %d\n",LL);
	return 1;
}


int init()
{
	int res;
	hwinit();
	printf("Please set the focus to the middle of the base, then type 'q'..\n");
	monitor();
    tunelight();
	takebggchere(); //take background and equalization map
	/// Configure Odyssey for taking image with processing

	o->ApplyGainCorr=true;
//	o->ApplyGainCorr=false;
	o->ApplyFIRF=true;
	o->Threshold=ThrTop;
	o->SubtractBG=true;
	return 1;

}


xy(float X, float Y)
{
	s->X->PosMove(X,1e9,1e9);
	s->Y->PosMove(Y,1e9,1e9);
}

xyabs(float X, float Y)
{
	EdbMark *p=new EdbMark();
	p->SetX(X);
	p->SetY(Y);
	if(FM->eFoundMarksSet)
	{
		p->Transform(FM->eFoundMarksSet->Abs2Stage());
	}

	s->X->PosMove(p->X(),1e9,1e9);
	s->Y->PosMove(p->Y(),1e9,1e9);
	delete p;
}

homeall()
{
	int resx,resy;
	printf("Homing XY.., \n");
	s->X->FindHome(-1);
	s->Y->FindHome(-1);
	resy=s->Y->WaitForHome(1e4);
	resx=s->X->WaitForHome(1e4);
	printf("Reached %f,%f on XY, resx=%d resy=%d\n",s->X->GetPos(),s->Y->GetPos(),resx,resy);
    if(resx!=1) return 0;
    if(resy!=1) return 0;
    printf("resetting X,Y..\n");
	s->X->Reset();
	s->Y->Reset();
    printf("Moving to -79000,20000..\n");
	s->X->PosMove(-79000,1e6,1e6);
	s->Y->PosMove(20000,1e6,1e6);
	resx=s->X->WaitForMoveComplete(1e4);
	resy=s->Y->WaitForMoveComplete(1e4);
	printf("Read %f,%f on XY\n",s->X->GetPos(),s->Y->GetPos());
	printf("Homing Z.., \n");
	s->Z->FindHome(1);
	resy=s->Z->WaitForHome(1e4);
	printf("Reached %f on Z, resy=%d\n",s->Z->GetPos(),resy);
    if(resy!=1) return 0;
    printf("resetting Z..\n");
	s->Z->Reset();
	printf("Read %f on Z, resy=%d\n",s->Z->GetPos(),resy);
	s->Z->PosMove(5000,1e4,1e4);
	s->Z->WaitForMoveComplete(1e4);
//	s->Y->PosMove(70000,1e6,1e6);
//	resy=s->Y->WaitForMoveComplete(3e3);
	printf("homall() complete \n");

}

monitor()
{ 
	o->StartLiveGrab();
	s->Monitor();
	o->StopLiveGrab();
}


int scanmarks()
{
	printf("Point1\n");
		if(FM->eFoundMarksSet) { /*delete FM->eFoundMarksSet;*/}
	printf("Point2\n");
         FM->eFoundMarksSet=new EdbMarksSet();
	printf("Point3\n");
	for(int m=0;m<FM->eMarksSet->GetN();m++)
	{
	printf("Point m=%d\n",m);
		float X,Y,Z;
		float Xn,Yn;
		FM->GotoMark(m);
		if(!focus(300)) continue;
		if(!FM->ScanMark(&X,&Y,&Z)) { o->StopLiveGrab(); continue; }
		o->StopLiveGrab();
		Xn=FM->eMarksSet->GetAbsolute()->GetMark(m)->X();
		Yn=FM->eMarksSet->GetAbsolute()->GetMark(m)->Y();
		FM->eFoundMarksSet->GetStage()->AddMark(m,X,Y);
		FM->eFoundMarksSet->GetAbsolute()->AddMark(m,Xn,Yn);
	}
	FM->eFoundMarksSet->Print();
	if(FM->eFoundMarksSet->GetN()<3) {printf("Less than 3 marks found! Aborting.\n"); s->eAffine=0; return 0;}
	printf("Affine transformation from stage to nominal :\n");
	FM->eFoundMarksSet->Stage2Abs()->Print();
	s->eAffine=FM->eFoundMarksSet->Stage2Abs();
	return 1;
}

void showgain()
{
	short aa[1280*1024];
	o->GetGCImage(aa);
	TH2S *hg = new TH2S("hg","hg",1280,0,1280,1024,0,1024);
	for(int iy=10; iy<1014; iy++)
		for(int ix=10; ix<1270; ix++) hg->Fill(ix,iy,aa[iy*1280+ix]);

	gStyle->SetPalette(1);
	hg->Draw("colZ");
	c1->SaveAs("c1.gif");
}

genmapext()
{
int Nm=FM->eMarksSet->GetN();
float Xm,Ym;
	printf("mapext: 0 0 0 0; %d 0 0 125000 100000; ",Nm);
	for(int i=0;i<Nm;i++)
	{
		Xm=FM->eFoundMarksSet->GetStage()->GetMark(i)->X();
		Xm=Xm-s->X->NominalReference;
		Ym=FM->eFoundMarksSet->GetStage()->GetMark(i)->Y();
		Ym=Ym-s->Y->NominalReference;
    printf("%d %f %f %f %f 1 1 0; ",i,Xm,Ym,Xm,Ym );
	}
}

