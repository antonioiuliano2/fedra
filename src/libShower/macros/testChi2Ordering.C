{
TFile* f = new TFile("ScanVolume_Ali.root");
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

TH1F* h_WTilde = new TH1F("h_WTilde","h_WTilde",40,0,0.1);
TH1F* h_WTildeNormalized = new TH1F("h_WTildeNormalized","h_WTildeNormalized",100,0,5);
Float_t WTilde=0;
Float_t WTildeNormalized=0;

Float_t X2=0;
TH1F* h_X2 = new TH1F("h_X2","h_X2",100,0,5);



TProfile* po = new TProfile("name","title",20,0,1);
TProfile* po2 = new TProfile("name2","title2",20,0,1);



TObjArray* arr_s = new TObjArray();

for (int j=0; j<1;j++) {
pat = ali->GetPattern(j);
//----------------------------------------------
for (int i=0; i<30000;i++) {
EdbSegP* s = (EdbSegP*)pat->GetSegment(i);
TTheta = TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY());
// cout << abs(TTheta-0.05) << endl;
// if (TMath::Abs(TTheta-0.2)>0.05) continue;

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

for (int i=0; i<30000;i++) {
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


//----------------------------------------------
/*
// // Loop over bin entries of the profile histogram:
for (int jj=1; jj<2;jj++) {
Float_t mu= po->GetBinContent(jj);
Float_t sigma= po->GetBinError(jj);
for (int i=0; i<30000;i++) {
EdbSegP* s = (EdbSegP*)pat->GetSegment(i);
if (s->Chi2() > mu+1*sigma) continue;
po2->Fill(TMath::Sqrt(s->TX()*s->TX()+s->TY()*s->TY()),s->Chi2());
} // i
}
*/



} // j
// po->Draw("colz");
// po->Draw("");
// po2->SetLineColor(2);
// po2->Draw("same");

h_chi2_v_ttheta->Draw("colz");
new TCanvas();
h_WTilde_v_ttheta->Draw("colz");
new TCanvas();
h_chi2->Draw();
new TCanvas();
h_WTilde->Draw();
new TCanvas();
h_X2->Draw();
new TCanvas();
h_chi2Normalized->Draw();
new TCanvas();
h_WTildeNormalized->Draw();

return;


// p->Execute_ConstantBTDensityInAngularBins();
// p->CreateEdbPVRec();
// ali2 = p->GetEdbPVRec(1);
// q = new EdbPVRQuality(ali2);

}









// // Loop over bin entries of the profile histogram:
// for (int j=1; j<2;j++) {
//   // Loop over different factors away from the mean of the profile histo bin:
//   for (int nj=10; nj>-10; nj--) {
//     if (s->Chi2() > mu-nj*sigma) continue;
//   }
// }