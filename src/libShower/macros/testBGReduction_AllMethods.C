{
TFile* f = new TFile("ScanVolume_Ali.root");
EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
ali->Print();
// return;

// gEDBDEBUGLEVEL=3;
 p = new EdbPVRQuality(ali);
 
 TObjArray* objarr = ali->eTracks;
 EdbSegP* ob = (EdbSegP*)objarr->At(0);
 ob->Print();
return;
 
 p->SetBTDensityLevelCalcMethodMC(kTRUE);
 p->SetBTDensityLevelCalcMethodMCConfirmation(18);
 p->CheckEdbPVRec();
//  return;
//   p->Print();
// p->SetBTDensityLevel(20);
 p->Execute_ConstantBTQuality();
//   p->Execute_ConstantBTX2Hat(); // seems OK, check chi2/W distro after cutting...
//     p->Execute_RandomCut(); // seems OK.
   return;
//   p->CreateEdbPVRec();
//  p->Execute_ConstantBTDensity();
//  p->Execute_ConstantBTDensityInAngularBins();
// p->Execute_EqualizeTanThetaSpace(); 
 
// p-> Execute_EqualizeTanThetaSpace_ConstantBTDensity();
// p-> Execute_EqualizeTanThetaSpace_ConstantBTX2Hat();
// p-> Execute_EqualizeTanThetaSpace_RandomCut();
 

 p->FillHistograms(0);
  new TCanvas();
 TH2F* eHistChi2W = p->GetHistChi2W();
  eHistChi2W->Draw("colz");
 
 
//  p->PrintCutValues(1);
// return;
 
//  gEDBDEBUGLEVEL=3;
 p->CreateEdbPVRec();
 
 p->FillHistograms(1);
   new TCanvas();
 TH2F* eHistChi2W = p->GetHistChi2W();
  eHistChi2W->Draw("colz");
 
 return;
 
 ali2 = p->GetEdbPVRec(1);
 q = new EdbPVRQuality(ali2);
  return;
}