void check_aligned()
{
  check_shrinkage();
}

//-----------------------------------------------------------------
void check_shrinkage()
{
  TCanvas *cshr = new TCanvas("cshr","shrinkage");
  cshr->Clear();
  cshr->Divide(2,2);

  cshr_1->cd();       couples->Draw("s.eTY:s.eTX");
  cshr_3->cd();       couples->Draw("eCHI2");
  cshr_2->cd();       couples->Draw("s1.eTX-s.eTX:s.eTX","abs(s.eTY)<.1");
  cshr_4->cd();       couples->Draw("s2.eTX-s.eTX:s.eTX","abs(s.eTY)<.1");
}

//-----------------------------------------------------------------
