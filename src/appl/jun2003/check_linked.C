void check_linked()
{
  check_ang0();
  check_ang2();
  check_surf();
  check_puls1();
  check_puls();
}

//-------------------------------------------------------
void check_ang0()
{
  TCanvas *c = new TCanvas("a0","angular distribution");
  c->Clear();
  couples->Draw("s.eTY:s.eTX");
  c->SaveAs("ang0.gif");
}

//-------------------------------------------------------
void check_ang2()
{
  TCanvas *c = new TCanvas("a2","angles");
  c->Clear();
  c->Divide(2,2);

  a2_1->cd();       couples->Draw("s.eTX");
  a2_2->cd();       couples->Draw("s.eTY");
  a2_3->cd();       couples->Draw("eCHI2");
  a2_4->cd();       couples->Draw("eCHI2:sqrt(s.eTX*s.eTX+s.eTY*s.eTY)","","",20000);

  c->SaveAs("ang2.gif");
}

//-------------------------------------------------------
void check_surf()
{
  TCanvas *c = new TCanvas("sf","track density on the plate");
  c->Clear();
  couples->Draw("s.eY:s.eX");
  c->SaveAs("surf.gif");
}

//-------------------------------------------------------
void check_puls1()
{
  TCanvas *c = new TCanvas("ph1","puls distributions",700,800);
  c->Clear();
  c->Divide(2,3);

  TCut  p = "abs(s.eTY)<.02&&abs(s.eTX+.3)<.02";
  TCut sp = "sqrt(s.eTX*s.eTX+s.eTY*s.eTY)<.33&&sqrt(s.eTX*s.eTX+s.eTY*s.eTY)>.27";

  ph1_1->cd();       couples->Draw("eCHI2", p );
  ph1_2->cd();       couples->Draw("eCHI2", sp && !p );
  ph1_3->cd();       couples->Draw("s1.eW", p );
  ph1_4->cd();       couples->Draw("s1.eW", sp && !p );
  ph1_5->cd();       couples->Draw("s1.eW", sp && !p && "eCHI2<1.2" );
  ph1_6->cd();       couples->Draw("s1.eW", sp && !p && "eCHI2>1.2" );

  c->SaveAs("puls1.gif");
}

//-------------------------------------------------------
void check_puls()
{
  TCanvas *c = new TCanvas("ph","puls distributions",700,800);
  c->Clear();
  c->Divide(2,3);

  TCut  p = "abs(s.eTY)<.02&&abs(s.eTX+.3)<.02";
  TCut sp = "sqrt(s.eTX*s.eTX+s.eTY*s.eTY)<.33&&sqrt(s.eTX*s.eTX+s.eTY*s.eTY)>.27";

  ph_1->cd();       couples->Draw("eCHI2", p );
  ph_2->cd();       couples->Draw("eCHI2", sp && !p );
  ph_3->cd();       couples->Draw("s.eW", p );
  ph_4->cd();       couples->Draw("s.eW", sp && !p );
  ph_5->cd();       couples->Draw("s.eW", sp && !p && "eCHI2<1.2" );
  ph_6->cd();       couples->Draw("s.eW", sp && !p && "eCHI2>1.2" );

  c->SaveAs("puls.gif");
}
