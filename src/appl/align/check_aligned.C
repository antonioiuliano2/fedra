void check_aligned()
{
  check_shrinkage();
}

//-----------------------------------------------------------------
void check_shrinkage()
{
  // this function check shrinkage and/or distance between linked planes 
  // Note: do not use s.* (linked segment parameters), because them could 
  // be different from the "base angle" calculated here directly

  TCanvas *cshr = new TCanvas("cshr","shrinkage");
  cshr->Clear();
  cshr->Divide(2,2);
  gStyle->SetOptFit(0001);

  cshr_1->cd();   couples->Draw("s1.eTX-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)");
  cshr_2->cd();   couples->Draw("s2.eTX-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)");

  cshr_3->cd();   
  couples->Draw("s1.eTX-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)","","prof");
  htemp->Fit("pol1","w","",-.4,.4);

  cshr_4->cd();   
  couples->Draw("s2.eTX-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)","","prof");
  htemp->Fit("pol1","w","",-.4,.4);
  
}

//-----------------------------------------------------------------
void a()
{
  cshr_1->cd();   couples->Draw("(s2.eY-s1.eY)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)");
  cshr_3->cd();   couples->Draw("eCHI2");
}

//-----------------------------------------------------------------
