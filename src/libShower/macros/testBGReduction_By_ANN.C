{
 EdbDataProc *dproc = new EdbDataProc;
	ePVR = new EdbPVRec;
	dproc->ReadTracksTree(*ePVR, "lt_AllForANNBGSeparation.root", "nseg>2");	
	EdbPVRec* ali =  ePVR;

// TFile* f = new TFile("ScanVolume_Ali.root");
// EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
ali->Print();
// return;

TTree* simu = new TTree();
TTree* simu2 = new TTree();
Float_t chi2, TT; Double_t ANNOUT;
Int_t W,type;
simu->Branch("chi2", &chi2, "chi2/F");
simu->Branch("W",   &W,   "W/I");
simu->Branch("TT", &TT, "TT/F");
simu->Branch("type",   &type,   "type/I");
simu2->Branch("chi2", &chi2, "chi2/F");
simu2->Branch("W",   &W,   "W/I");
simu2->Branch("TT", &TT, "TT/F");
simu2->Branch("ANNOUT",   &ANNOUT,   "ANNOUT/D");
simu2->Branch("type",   &type,   "type/I");

for (int n=0; n<ali->Npatterns(); n++) {

EdbPattern* pat = (EdbPattern*)ali->GetPattern(n);
            Int_t npat=pat->N();
            EdbSegP* seg=0;
   for (int j=0; j<npat; j++) {
                seg=pat->GetSegment(j);
		TT = seg->Theta();
		chi2 = seg->Chi2();
		W = seg->W();
		if (seg->MCEvt()>0) type = 1;
		if (seg->MCEvt()<0) type = 0;
       simu->Fill();
   }
 n++;
}

   TCanvas* canv = new TCanvas();
   canv->Divide(3,1);
   canv->cd(1);
   simu->Draw("TT","type==0");
   simu->Draw("TT","type==1","same");
    canv->cd(2);
    simu->Draw("chi2","type==0");
    simu->Draw("chi2","type==1","same");
    canv->cd(3);
    simu->Draw("W","type==0");
    simu->Draw("W","type==1","same");
    canv->cd();
   TCanvas* canv2 = new TCanvas();
   simu->Draw("chi2:W","","box");
   simu->Draw("chi2:W","type==0","samebox");
   simu->Draw("chi2:W","type==1","boxsame");
// return;

   TMultiLayerPerceptron *mlp = new TMultiLayerPerceptron("@chi2,@W,@TT:5:3:type",simu,"Entry$%2","(Entry$+1)%2");
//    mlp->Train(50, "text,graph,update=10");
   mlp->LoadWeights("testweights.txt");
   
   // Use TMLPAnalyzer to see what it looks for
   TCanvas* mlpa_canvas = new TCanvas("mlpa_canvas","Network analysis");
   mlpa_canvas->Divide(2,2);
   TMLPAnalyzer ana(mlp);
   // Initialisation
   ana.GatherInformations();
   // output to the console
   ana.CheckNetwork();
   mlpa_canvas->cd(1);
   // shows how each variable influences the network
   ana.DrawDInputs();
   mlpa_canvas->cd(2);
   // shows the network structure
   mlp->Draw();
   mlpa_canvas->cd(3);
   // draws the resulting network
   ana.DrawNetwork(0,"type==1","type==0");
   mlpa_canvas->cd(4);
// // // // // // // // // // // //    
   Double_t params[3];

   
for (int n=1; n<ali->Npatterns(); n++) {
    EdbPattern* pat = (EdbPattern*)ali->GetPattern(0);
            Int_t npat=pat->N();
            EdbSegP* seg=0;
   for (int j=0; j<npat; j++) {
                seg=pat->GetSegment(j);
		TT = seg->Theta();
		chi2 = seg->Chi2();
		W = seg->W();
		params[0]=chi2;
		params[1]=W;
		params[2]=TT;
		ANNOUT = mlp->Evaluate(0,params);
		if (seg->MCEvt()>0) type = 1;
		if (seg->MCEvt()<0) type = 0;
		simu2->Fill();
   }
 n++;  
}
   
   TCanvas* canv = new TCanvas();
   simu2->Draw("ANNOUT");
   simu2->Draw("ANNOUT","type==0","same");
   simu2->Draw("ANNOUT","type==1","same");
    TCanvas* canv = new TCanvas();
   canv->Divide(3,1);
   canv->cd(1);
   simu2->Draw("TT","ANNOUT<0.2");
   simu2->Draw("TT","ANNOUT>0.2","same");
    canv->cd(2);
    simu2->Draw("chi2","ANNOUT<0.2");
    simu2->Draw("chi2","ANNOUT>0.2","same");
    canv->cd(3);
    simu2->Draw("W","ANNOUT<0.2");
    simu2->Draw("W","ANNOUT>0.2","same");
    canv->cd();
   TCanvas* canv2all = new TCanvas();
   simu2->Draw("chi2:W>>h2all(100,0,50,100,0,5)","","colz");
   TCanvas* canv2bg = new TCanvas();
   simu2->Draw("chi2:W>>h2bg(100,0,50,100,0,5)","ANNOUT<0.2","colz");
   TCanvas* canv2sg = new TCanvas();
   simu2->Draw("chi2:W>>h2sg(100,0,50,100,0,5)","ANNOUT>0.2","colz");
    
}