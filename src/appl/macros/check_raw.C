#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"

void check_z();
void check_view();
void check_surf();
void check_ang();
void check_puls();

TTree *Views=0;

//-------------------------------------------------------
void init()
{
  Views = (TTree*)gDirectory->Get("Views");
}

//-------------------------------------------------------
void check_raw()
{
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

  c->cd(1);       Views->Draw("eZ1:eXview:eYview");
  c->cd(2);       Views->Draw("eZ1:eAreaID*121+eViewID");
  c->cd(3);       Views->Draw("eNcl:eZframe-eZ2");
  c->cd(4);       Views->Draw("eZ2-eZ3");

  c->SaveAs("raw_z.gif");
}

//-------------------------------------------------------
void check_view()
{
  TCanvas *c = new TCanvas("cview","check view");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1); Views->Draw("eY0:eX0","eNframesTop==0","lego2");
  c->cd(2); Views->Draw("eY0:eX0","eNframesTop==0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");
  c->cd(3); Views->Draw("eY0:eX0","eNframesBot==0","lego2");
  c->cd(4); Views->Draw("eY0:eX0","eNframesBot==0&&(abs(eTy)>.006||abs(eTx)>.006)","colZ");

  c->SaveAs("raw_view.gif");
}

//-------------------------------------------------------
void check_surf()
{
  TCanvas *c = new TCanvas("surf","check surface");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);

  c->cd(1);       Views->Draw("eYview:eXview");
  c->cd(1);       Views->Draw("eYview+eY0:eXview+eX0","eNframesTop==0","samecontZ");
  c->cd(2);       Views->Draw("eYview:eXview");
  c->cd(2);       Views->Draw("eYview+eY0:eXview+eX0","eNframesBot==0","samecontZ");
  c->cd(3);       Views->Draw("eNsegments");
                  Views->SetLineColor(2);
                  Views->Draw("eNsegments>>hbot","eNframesTop==0","same");
                  Views->SetLineColor(3);
                  Views->Draw("eNsegments>>htop","eNframesBot==0","same");
		  TLegend *leg = new TLegend(300,0,400,200);
		  leg->AddEntry(htop," top","lp");
		  leg->AddEntry(hbot," bot","lp");
		  leg->Draw();

                  Views->SetLineColor(1);
  c->cd(4);       Views->Draw("eNclusters");
                  Views->SetLineColor(2);
                  Views->Draw("eNclusters","eNframesTop==0","same");
                  Views->SetLineColor(3);
                  Views->Draw("eNclusters","eNframesBot==0","same");
                  Views->SetLineColor(1);

  c->SaveAs("raw_surf.gif");
}

//-------------------------------------------------------
void check_puls()
{
  TCanvas *c = new TCanvas("cpuls","check segments puls");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1);       Views->Draw("segments.ePuls","eNframesTop==0");
  c->cd(2);       Views->Draw("segments.ePuls","eNframesBot==0");
  c->cd(3);       Views->Draw("segments.eSigmaX","eNframesTop==0");
  c->cd(4);       Views->Draw("segments.eSigmaX","eNframesBot==0");
  c->SaveAs("raw_puls.gif");
}

//-------------------------------------------------------
void check_ang()
{
  TCanvas *c = new TCanvas("cseg","check segments angle");
  c->Clear();
  c->Divide(2,2);
  gStyle->SetPalette(1);
  c->cd(1);  Views->Draw("segments.eTx");
  c->cd(2);  Views->Draw("segments.eTy");
  c->cd(3);  Views->Draw("eTy:eTx","eNframesTop==0&&((abs(eTy)>.01||abs(eTx)>.01)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");
  c->cd(4);  Views->Draw("eTy:eTx","eNframesBot==0&&((abs(eTy)>.01||abs(eTx)>.01)) && abs(eTy)<.6 && abs(eTx)<.6","colZ");


  c->SaveAs("raw_ang.gif");
}

//-------------------------------------------------------
