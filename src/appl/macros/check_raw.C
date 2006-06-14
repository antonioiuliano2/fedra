#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"

void check_z();
void check_view();
void check_surf();
void check_ang();
void check_puls();

TTree *cr_tree=0;

//-------------------------------------------------------
void init()
{
  cr_tree = (TTree*)gDirectory->Get("Views");
}

//-------------------------------------------------------
void check_raw()
{
  printf("Red  line: Top    side\n");
  printf("Blue line: Bottom side\n");
  init();
  check_z();
  check_view();
  check_surf();
  check_ang();
  check_puls();
}

//-------------------------------------------------------
void check_z()
{
  TCanvas *c = new TCanvas("cz","check Z");
  c->Clear();
  c->Divide(2,2);

  c->cd(1);       cr_tree->Draw("eZ1:eXview:eYview");
  c->cd(2);       cr_tree->Draw("eZ1:eAreaID*121+eViewID");
  c->cd(3);       cr_tree->Draw("eNcl:eZframe-eZ2");
  c->cd(4);       cr_tree->Draw("eZ2-eZ3");

  c->SaveAs("raw_z.gif");
}

//-------------------------------------------------------
void check_view()
{
  TCanvas *c = new TCanvas("cview","check view");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1); cr_tree->Draw("eY0:eX0","eNframesTop==0&&ePuls>0","lego2");
  c->cd(2); cr_tree->Draw("eY0:eX0","eNframesTop==0&&ePuls>0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");
  c->cd(3); cr_tree->Draw("eY0:eX0","eNframesBot==0&&ePuls>0","lego2");
  c->cd(4); cr_tree->Draw("eY0:eX0","eNframesBot==0&&ePuls>0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");

  c->SaveAs("raw_view.gif");
}

//-------------------------------------------------------
void check_surf()
{
  TCanvas *c = new TCanvas("surf","check surface");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);

  c->cd(1);       cr_tree->Draw("eYview:eXview");
  c->cd(1);       cr_tree->Draw("eYview+eY0:eXview+eX0","eNframesTop==0&&ePuls>0","samecontZ");
  c->cd(2);       cr_tree->Draw("eYview:eXview");
  c->cd(2);       cr_tree->Draw("eYview+eY0:eXview+eX0","eNframesBot==0&&ePuls>0","samecontZ");
  c->cd(3);       cr_tree->Draw("eNsegments");
                  cr_tree->SetLineColor(kBlue);
                  cr_tree->Draw("eNsegments>>hbot","eNframesTop==0","same");
                  cr_tree->SetLineColor(kRed);
                  cr_tree->Draw("eNsegments>>htop","eNframesBot==0","same");
		  TLegend *leg = new TLegend(300,0,400,200);
		  leg->AddEntry(htop," top","lp");
		  leg->AddEntry(hbot," bot","lp");
		  leg->Draw();

                  cr_tree->SetLineColor(1);
  c->cd(4);       cr_tree->Draw("eNclusters");
                  cr_tree->SetLineColor(kBlue);
                  cr_tree->Draw("eNclusters","eNframesTop==0","same");
                  cr_tree->SetLineColor(kRed);
                  cr_tree->Draw("eNclusters","eNframesBot==0","same");
                  cr_tree->SetLineColor(1);

  c->SaveAs("raw_surf.gif");
}

//-------------------------------------------------------
void check_puls()
{
  TCanvas *c = new TCanvas("cpuls","check segments puls");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1);       cr_tree->Draw("segments.ePuls","eNframesTop==0&&ePuls>0");
  c->cd(2);       cr_tree->Draw("segments.ePuls","eNframesBot==0&&ePuls>0");
  c->cd(3);       cr_tree->Draw("segments.eSigmaY","eNframesTop==0&&ePuls>0");
  c->cd(4);       cr_tree->Draw("segments.eSigmaY","eNframesBot==0&&ePuls>0");
  c->SaveAs("raw_puls.gif");
}

//-------------------------------------------------------
void check_ang()
{
  TCanvas *c = new TCanvas("cseg","check segments angle");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1);  cr_tree->Draw("segments.eTx","ePuls>0");
  c->cd(2);  cr_tree->Draw("segments.eTy","ePuls>0");
  c->cd(3);  cr_tree->Draw("eTy:eTx","eNframesTop==0&&ePuls>0&&((abs(eTy)>.01||abs(eTx)>.01)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");
  c->cd(4);  cr_tree->Draw("eTy:eTx","eNframesBot==0&&ePuls>0&&((abs(eTy)>.01||abs(eTx)>.01)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");

  c->SaveAs("raw_ang.gif");
}

//-------------------------------------------------------
