void check_all()
{
  check_z();
  check_view();
  check_surf();
  check_seg();
  check_puls();
}

//-------------------------------------------------------
void check_z()
{
  TCanvas *c = new TCanvas("cz","check Z");
  c->Clear();
  c->Divide(2,2);

  cz_4->cd();       Views->Draw("eZ2-eZ3");
  cz_2->cd();       Views->Draw("eZ1:eAreaID*121+eViewID");
  cz_1->cd();       Views->Draw("eZ1:eXview:eYview");
  cz_3->cd();       Views->Draw("eZ3-eZ2:eXview:eYview");

  c->SaveAs("z.gif");
}

//-------------------------------------------------------
void check_view()
{
  TCanvas *c = new TCanvas("cview","check view");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  cview_1->cd();       Views->Draw("eY0:eX0","eNframesTop==0","lego2");
  cview_3->cd();       Views->Draw("eY0:eX0","eNframesBot==0","lego2");
  cview_2->cd();       Views->Draw("eY0:eX0","eNframesTop==0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");
  cview_4->cd();       Views->Draw("eY0:eX0","eNframesBot==0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");

  c->SaveAs("view.gif");
}

//-------------------------------------------------------
void check_surf()
{
  TCanvas *c = new TCanvas("surf","check surface");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  //  surf_1->cd();       Views->Draw("eY0:eX0","","lego2");
  surf_1->cd();       Views->Draw("eYview:eXview");
  surf_1->cd();       Views->Draw("eYview+eY0:eXview+eX0","eNframesTop==0","samecontZ");
  surf_2->cd();       Views->Draw("eYview:eXview");
  surf_2->cd();       Views->Draw("eYview+eY0:eXview+eX0","eNframesBot==0","samecontZ");
  surf_3->cd();       Views->Draw("eNsegments");
                      Views->SetLineColor(2);
                      Views->Draw("eNsegments","eNframesTop==0","same");
                      Views->SetLineColor(3);
                      Views->Draw("eNsegments","eNframesBot==0","same");
                      Views->SetLineColor(1);
  surf_4->cd();       Views->Draw("eNclusters");
                      Views->SetLineColor(2);
                      Views->Draw("eNclusters","eNframesTop==0","same");
                      Views->SetLineColor(3);
                      Views->Draw("eNclusters","eNframesBot==0","same");
                      Views->SetLineColor(1);

  c->SaveAs("surf.gif");
}

//-------------------------------------------------------
void check_puls()
{
  TCanvas *c = new TCanvas("cpuls","check segments puls");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  cpuls_1->cd();       Views->Draw("segments.ePuls","eNframesTop==0");
  cpuls_2->cd();       Views->Draw("segments.ePuls","eNframesBot==0");
  cpuls_3->cd();       Views->Draw("segments.eSigmaX","eNframesTop==0");
  cpuls_4->cd();       Views->Draw("segments.eSigmaX","eNframesBot==0");
  c->SaveAs("puls.gif");
}

//-------------------------------------------------------
void check_seg()
{
  TCanvas *c = new TCanvas("cseg","check segments");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  cseg_1->cd();       Views->Draw("segments.eTx");
  cseg_2->cd();       Views->Draw("segments.eTy");
  cseg_3->cd();       Views->Draw("eTy:eTx","eNframesTop==0&&((abs(eTy)>.006||abs(eTx)>.006)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");
  cseg_4->cd();       Views->Draw("eTy:eTx","eNframesBot==0&&((abs(eTy)>.006||abs(eTx)>.006)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");


  c->SaveAs("seg.gif");
}

//-------------------------------------------------------
