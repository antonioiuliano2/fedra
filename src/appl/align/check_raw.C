void check_all()
{
  check_z();
  check_surf();
  check_seg();
}

void check_z()
{
  TCanvas *c = new TCanvas("cz","check Z");
  c->Clear();
  c->Divide(2,2);

  cz_4->cd();       Views->Draw("eZ3-eZ2");
  cz_2->cd();       Views->Draw("eZ1:eAreaID");
  cz_1->cd();       Views->Draw("eZ1:eXview:eYview");
  cz_3->cd();       Views->Draw("eZ3-eZ2:eXview:eYview");

  c->SaveAs("z.gif");
}

void check_surf()
{
  TCanvas *c = new TCanvas("surf","check surface");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  surf_1->cd();       Views->Draw("eY0:eX0","","lego2");
  surf_2->cd();       Views->Draw("eYview:eXview");
  surf_2->cd();       Views->Draw("eYview+eY0:eXview+eX0","","samecontZ");
  surf_3->cd();       Views->Draw("eNsegments");
  surf_4->cd();       Views->Draw("eNclusters");

  c->SaveAs("surf.gif");
}

void check_seg()
{
  TCanvas *c = new TCanvas("cseg","check segments");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  cseg_1->cd();       Views->Draw("segments.eTy");
  cseg_2->cd();       Views->Draw("eTy:eTx"," ((abs(eTy)>.006||abs(eTx)>.006)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");
  cseg_3->cd();       Views->Draw("segments.ePuls");
  cseg_4->cd();       Views->Draw("segments.eSide");

  c->SaveAs("seg.gif");
}

