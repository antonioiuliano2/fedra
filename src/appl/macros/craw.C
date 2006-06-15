#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TTree.h"

TTree *cr_tree=0;

//-------------------------------------------------------
void init()
{
  cr_tree = (TTree*)gDirectory->Get("Views");
  gStyle->SetPalette(1);
}

//-------------------------------------------------------
void craw()
{
  printf("Check of the very raw (clusters only) data\n\n");
  printf("Red  line: Top    side\n");
  printf("Blue line: Bottom side\n");
  init();
  check_z();
  check_a();
  check_ss();
  check_e();
  //check_view();
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

  //c->SaveAs("raw_z.gif");
}

//-------------------------------------------------------
void check_view()
{
  TCanvas *c = new TCanvas("cview","check view");
  c->Divide(2,2);
  c->cd(3); cr_tree->Draw("eY:eX>>hvb(42,-210,210,36,-180,180)","eNframesBot>0","colZ",300);
  c->cd(1); cr_tree->Draw("eY:eX>>hvt(42,-210,210,36,-180,180)","eNframesTop>0","colZ",300);
  c->cd(4); cr_tree->Draw("eY:eX>>hvb1(42,-210,210,36,-180,180)","eNframesBot>0","lego2",300);
  c->cd(2); cr_tree->Draw("eY:eX>>hvt1(42,-210,210,36,-180,180)","eNframesTop>0","lego2",300);

  //c->SaveAs("raw_view.gif");
}

//-------------------------------------------------------
void check_a(float meanw=2200.)
{
  TCanvas *c = new TCanvas("check_a","check area",600,800);
  c->Divide(1,2);

  cr_tree->SetAlias("nclm","Sum$(eNcl)/Length$(eNcl)");

  cr_tree->Draw("eYview:eXview>>h(200,0,1000,200,0,1000)","","goff");

  c->cd(1);
  cr_tree->SetMarkerStyle(1);
  cr_tree->Draw("eYview:eXview","nclm*(eNframesTop>0)");
  int   nraws = cr_tree->GetSelectedRows();
  Double_t *x = cr_tree->GetV1();
  Double_t *y = cr_tree->GetV2();
  Double_t *w = cr_tree->GetW();
  printf("%d raws: %d\n", nraws);
  TMarker *m;
  for(int i=0; i<nraws; i++) {
    m= new TMarker( y[i],x[i], 24);
    m->SetMarkerSize( w[i]/meanw );
    m->SetMarkerColor(kRed);
    m->Draw();
  }
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->SetMarkerStyle(6);
  cr_tree->Draw("eYview:eXview","eNframesBot>0","same");

  c->cd(2);
  cr_tree->SetMarkerStyle(1);
  cr_tree->Draw("eYview:eXview","nclm*(eNframesBot>0)");
  int   nraws = cr_tree->GetSelectedRows();
  Double_t *x = cr_tree->GetV1();
  Double_t *y = cr_tree->GetV2();
  Double_t *w = cr_tree->GetW();
  printf("%d raws: %d\n", nraws);
  TMarker *m;
  for(int i=0; i<nraws; i++) {
    m= new TMarker( y[i],x[i], 24);
    m->SetMarkerSize( w[i]/meanw );
    m->SetMarkerColor(kBlue);
    m->Draw();
  }
  cr_tree->SetMarkerColor(kRed);
  cr_tree->SetMarkerStyle(6);
  cr_tree->Draw("eYview:eXview","eNframesTop>0","same");
}

//-------------------------------------------------------
void check_ss()
{
  TCanvas *c = new TCanvas("check_s","check view",600,800);
  c->Divide(2,3);

  c->cd(1);
  cr_tree->SetLineColor(1);
  cr_tree->Draw("nclm");
  cr_tree->SetLineColor(kRed);
  cr_tree->Draw("nclm","eNframesTop>0","same");
  cr_tree->SetLineColor(kBlue);
  cr_tree->Draw("nclm","eNframesBot>0","same");

  c->cd(2);
  cr_tree->SetMarkerStyle(6);
  cr_tree->Draw("eZframe:eYview:eXview:eNcl");

  c->cd(3);
  cr_tree->SetLineColor(1);
  cr_tree->Draw("eNcl");
  cr_tree->SetLineColor(kRed);
  cr_tree->Draw("eNcl","eNframesTop>0","same");
  cr_tree->SetLineColor(kBlue);
  cr_tree->Draw("eNcl","eNframesBot>0","same");

  c->cd(4);
  cr_tree->SetMarkerStyle(1);
  cr_tree->SetMarkerColor(1);
  cr_tree->Draw("eNcl:eZframe");
  cr_tree->SetMarkerStyle(6);
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->Draw("eNcl:eZframe","eNframesBot>0","same");
  cr_tree->SetMarkerColor(kRed);
  cr_tree->Draw("eNcl:eZframe","eNframesTop>0","same");

  c->cd(5);
  cr_tree->SetMarkerStyle(6);
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->Draw("eNcl:eZframe-eZ2","eNframesBot>0");
  c->cd(6);
  cr_tree->SetMarkerColor(kRed);
  cr_tree->Draw("eNcl:eZframe-eZ3","eNframesTop>0");


}

//-------------------------------------------------------
void check_e()
{
  TCanvas *c = new TCanvas("check_e","check entry",600,800);
  c->Divide(1,3);

  c->cd(1);
  cr_tree->SetMarkerColor(1);
  cr_tree->SetMarkerStyle(1);
  cr_tree->Draw("eNclusters:Entry$+0.5");
  cr_tree->SetMarkerStyle(6);
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->Draw("eNclusters:Entry$+0.5","eNframesBot>0","same");
  cr_tree->SetMarkerColor(kRed);
  cr_tree->Draw("eNclusters:Entry$+0.5","eNframesTop>0","same");

  c->cd(2);
  cr_tree->SetMarkerStyle(1);
  cr_tree->SetMarkerColor(1);
  cr_tree->Draw("eNcl:eFrameID+100*Entry$+0.5>>hee(10000)");
  cr_tree->SetMarkerStyle(6);
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->Draw("eNcl:eFrameID+100*Entry$+0.5","eNframesBot>0","same");
  cr_tree->SetMarkerColor(kRed);
  cr_tree->Draw("eNcl:eFrameID+100*Entry$+0.5","eNframesTop>0","same");

  c->cd(3);
  cr_tree->SetMarkerStyle(1);
  cr_tree->SetMarkerColor(1);
  cr_tree->Draw("eZframe:eFrameID+100*Entry$+0.5>>hzf(10000)");
  cr_tree->SetMarkerStyle(6);
  cr_tree->SetMarkerColor(kBlue);
  cr_tree->Draw("eZframe:eFrameID+100*Entry$+0.5","eNframesBot>0","same");
  cr_tree->SetMarkerColor(kRed);
  cr_tree->Draw("eZframe:eFrameID+100*Entry$+0.5","eNframesTop>0","same");
}
//-------------------------------------------------------
