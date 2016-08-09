{
TFile* f = new TFile("test.root");
EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
ali->Print();

//  p = new EdbPVRQuality(ali);
//  p->Print();
//  return;

Float_t TTheta=0;

TH2F* h_chi2_v_ttheta = new TH2F("h_chi2_v_ttheta","h_chi2_v_ttheta",100,0,1,100,0,2);
TH2F* h_WTilde_v_ttheta = new TH2F("h_WTilde_v_ttheta","h_WTilde_v_ttheta",100,0,1,40,0,0.1);

TH1F* h_chi2 = new TH1F("h_chi2","h_chi2",100,0,2);
TH1F* h_chi2Normalized = new TH1F("h_chi2Normalized","h_chi2Normalized",100,0,5);
Float_t chi2Normalized=0;

TH1F* h_WTilde = new TH1F("h_WTilde","h_WTilde",50,0,0.1);
TH1F* h_WTildeNormalized = new TH1F("h_WTildeNormalized","h_WTildeNormalized",100,0,5);
Float_t WTilde=0;
Float_t WTildeNormalized=0;

Float_t X2=0;
TH1F* h_X2 = new TH1F("h_X2","h_X2",100,0,5);





TObjArray* arr_s = new TObjArray();

Int_t npat = ali->Npatterns();
// npat=1;

for (int j=0; j<npat;j++) {
pat = ali->GetPattern(j);
Int_t Nseg = pat->N();
//----------------------------------------------
for (int i=0; i<Nseg;i++) {
EdbSegP* s = (EdbSegP*)pat->GetSegment(i);
TTheta = TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY());

h_chi2_v_ttheta->Fill(TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY()),s->Chi2());
h_chi2->Fill(s->Chi2());
WTilde=1./(s->W());
h_WTilde_v_ttheta->Fill(TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY()),WTilde);
h_WTilde->Fill(WTilde);


} // i
// // // // // // // // // // // // // // // // // // // // // 

Float_t m_chi2=h_chi2->GetMean();
Float_t s_chi2=h_chi2->GetRMS();
Float_t m_WTilde=h_WTilde->GetMean();
Float_t s_WTilde=h_WTilde->GetRMS();

for (int i=0; i<Nseg;i++) {
EdbSegP* s = (EdbSegP*)pat->GetSegment(i);
TTheta = TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY());

chi2Normalized=TMath::Power((s->Chi2()-m_chi2)/s_chi2,2);
h_chi2Normalized->Fill(chi2Normalized);
WTilde=1./(s->W());
WTildeNormalized=TMath::Power((WTilde-m_WTilde)/s_WTilde,2);
h_WTildeNormalized->Fill(WTildeNormalized);
// s->PrintNice();
// cout << TMath::Power((WTilde-mWTilde)/s_WTilde,2) << endl;
X2=chi2Normalized + WTildeNormalized;
h_X2->Fill(X2);
} // i
// // // // // // // // // // // // // // // // // // // // // // // // // // // // 


} // j



new TCanvas();
h_chi2_v_ttheta->Draw("colz");
h_chi2_v_ttheta->GetYaxis()->SetTitle("chi2");
h_chi2_v_ttheta->GetXaxis()->SetTitle("ttheta");
new TCanvas();
h_WTilde_v_ttheta->Draw("colz");
h_WTilde_v_ttheta->GetYaxis()->SetTitle("WTilde");
h_WTilde_v_ttheta->GetXaxis()->SetTitle("ttheta");
new TCanvas();
h_chi2->Draw();
h_chi2->GetXaxis()->SetTitle("chi2");
new TCanvas();
h_WTilde->Draw();
h_WTilde->GetXaxis()->SetTitle("WTilde");
new TCanvas();
h_X2->Draw();
h_X2->GetXaxis()->SetTitle("X2=chi2Normalized + WTildeNormalized");
new TCanvas();
h_chi2Normalized->Draw();
h_chi2Normalized->GetXaxis()->SetTitle("chi2Normalized");
new TCanvas();
h_WTildeNormalized->Draw();
h_WTildeNormalized->GetXaxis()->SetTitle("WTildeNormalized");
return;



}