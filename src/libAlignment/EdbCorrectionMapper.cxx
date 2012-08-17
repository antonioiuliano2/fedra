//-- Author :  Valeri Tioukov   18-12-2011

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCorrectionMap                                                      //
//                                                                      //
// tracks correction map                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TROOT.h"
#include "TStyle.h"

#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TPad.h>
#include <TArrow.h>
#include "EdbLog.h"
#include "EdbCorrectionMapper.h"

ClassImp(EdbCorrectionBin)
ClassImp(EdbCorrectionMapper)
using namespace TMath;

//---------------------------------------------------------------------
EdbCorrectionBin::EdbCorrectionBin()
{
}
//---------------------------------------------------------------------
EdbCorrectionBin::~EdbCorrectionBin()
{
}

//---------------------------------------------------------------------
void EdbCorrectionBin::AddBin(EdbCorrectionBin &bin)
{
  int n = bin.eAl.Ncp();
  for(int i=0; i<n; i++) {
    EdbSegP *s1 = (EdbSegP*)(bin.eAl.eS[0].At(i));
    EdbSegP *s2 = (EdbSegP*)(bin.eAl.eS[1].At(i));
    eAl.AddSegCouple(s1,s2);
  }
}
//---------------------------------------------------------------------
void EdbCorrectionBin::CalculateCorrections()
{
   float dzold = eAl.eCorr[0].V(2);
   eAl.CalcApplyMeanDiff();
//   eAl.CalcApplyFractMeanDiff();
   eAl.Corr2Aff(eLayer);
   Log(2,"EdbCorrectionBin::CalculateCorrections","%6d %9.2f (dx:dy:dz:dtx:dty:shr:phy):  %9.3f %9.3f %11.3f %9.5f %9.5f %9.5f %9.5f",eAl.Ncp(), dzold, 
          eAl.eCorr[0].V(0), eAl.eCorr[0].V(1), eAl.eCorr[0].V(2)-dzold, eAl.eCorr[0].V(3), eAl.eCorr[0].V(4), eAl.eCorr[0].V(5), eAl.eCorr[0].V(6) );
}

//=====================================================================================================
EdbCorrectionMapper::EdbCorrectionMapper()
{
  eNcpMin = 100;
}
//---------------------------------------------------------------------
EdbCorrectionMapper::~EdbCorrectionMapper()
{
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::SetCorrDZ()
{
    eAl.eCorr[0].SetV( 2, eZ2-eZ1 );
    int n=eMapAl.Ncell();
    for(int i=0;i<n; i++) {
      EdbCorrectionBin *bin = GetBin(i);
      bin->eAl.eCorr[0].SetV( 2, eZ2-eZ1 );
    }
}

//---------------------------------------------------------------------
EdbH2 *EdbCorrectionMapper::MapDZ() 
{
  EdbH2 *h = new EdbH2();
  h->InitH2(eMapAl);
  int n=eMapAl.Ncell();
  for(int i=0;i<n; i++) {
      EdbCorrectionBin *bin = GetBin(i);
      //h->SetBin( i, (int)(1000*bin->eAl.CalcMeanDZ()) );
      h->SetBin( i, (int)(1000*bin->eAl.eCorr[0].V(2)) );  // assumed that all corrections are calculated
  }
  return h;
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::CalculateCorrections()
{
   EdbCorrectionBin::CalculateCorrections();
   int n=eMapAl.Ncell();
   for(int i=0;i<n; i++) {
      EdbCorrectionBin *bin = GetBin(i);
      bin->CalculateCorrections();
  }
}

//---------------------------------------------------------------------
EdbH2 *EdbCorrectionMapper::MapDV(int ivar) 
{
  EdbH2 *h = new EdbH2();
  h->InitH2(eMapAl);
  int n=eMapAl.Ncell();
  for(int i=0;i<n; i++) {
      EdbCorrectionBin *bin = GetBin(i);
      h->SetBin( i, (int)(1000*bin->eAl.CalcMeanDiff(ivar)) );
  }
  return h;
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::Write(const char *suffix)
{
    bool batch = gROOT->IsBatch();
    gROOT->SetBatch();
    TCanvas *csum = DrawSum(suffix);
    csum->Write();
    TCanvas *cdiff = DrawMap(eMap, suffix);
    cdiff->Write();
    gROOT->SetBatch(batch);
}

//---------------------------------------------------------------------
TCanvas   *EdbCorrectionMapper::DrawSum(const char *suffix)
{
    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptDate(1);

    TCanvas   *csum = new TCanvas( Form("csum_%s",suffix), Form("csum_%s",suffix), 800,960 );
    TPaveText *tsum = new TPaveText(0.01,0.943,0.99,0.998);
    tsum->AddText( Form("%s -> %s", eID1.AsString(), eID2.AsString() ) );
    tsum->AddText( Form("z2-z1 = %12.1f -  %12.1f  = %12.1f", eZ2,eZ1, eZ2-eZ1 ) );
    tsum->Draw();
    
    TPad *sum = new TPad("sum","plots",0.01,0.03,0.99,0.94);
    sum->Divide(2,4);    sum->Draw();

    sum->cd(1);    eHxy1.DrawH2(    Form("Hxy1_%s"    ,suffix),"Pattern X vs Y" )->Draw("colz");
    sum->cd(2);    eHtxty1.DrawH2(  Form("Htxty1_%s"  ,suffix),"Pattern TX vs TY" )->Draw("colz");
    
    sum->cd(3);    eHdxy.DrawH2(    Form("Hdxy_%s"    ,suffix),"Map DX vs DY" )->Draw("colz");
    sum->cd(4);    eHdtxy.DrawH2(   Form("Hdtxy_%s"   ,suffix),"Map DTX vs DTY" )->Draw("colz");
    
    gStyle->SetOptStat(1);
    sum->cd(5);    
    TH1F *hdz = eHdz.DrawH1(     Form("Hdz_%s"     ,suffix),"Map DZ" );
    hdz->Draw();
    sum->cd(6);    
    TH1F *hshr = eHshr.DrawH1(    Form("Hshr_%s"    ,suffix),"Map Shr" );
    hshr->Draw();
    //printf("meanZ = %f meanShr= %f \n",hdz->GetMean(), hshr->GetMean());
    gStyle->SetOptStat(0);
    
    sum->cd(7);    eHdty_ty.DrawH2( Form("Hdty_ty_%s" ,suffix),"Map DTY vs TY" )->Draw("colz");
    sum->cd(8);
    
    return csum;
}

//---------------------------------------------------------------------
TCanvas   *EdbCorrectionMapper::DrawMap(EdbCorrectionMap &map, const char *suffix)
{
    TCanvas   *corrmap = new TCanvas( Form("corrmap_%s",suffix), Form("corrmap_%s",suffix), 800,800 );
    corrmap->SetRightMargin(0.14);
    corrmap->SetLeftMargin(0.08);
    //TH2F *h2f = MapDZ()->DrawH2( Form("mapDZ_%s" ,suffix),"Map DZ" );
    TH2F *h2f = map.DrawH2( Form("mapDZ_%s" ,suffix),"Map DZ" );
    h2f->GetXaxis()->SetLabelSize(0.025);
    h2f->GetYaxis()->SetLabelSize(0.025);
    h2f->GetZaxis()->SetLabelSize(0.025);
    
    int n=map.Ncell();
    for( int i=0; i<n; i++ ) {
       //EdbSegCorr *s = map.CorrLoc(i);
       //h2f->Fill(map.Xj(i), map.Yj(i), s->Z()-1);
       EdbSegCorr scorr = map.CorrLoc(i);
       h2f->Fill(map.Xj(i), map.Yj(i), scorr.V(2)-1);
    }
    h2f->Draw("colz");
    
    float xbin = map.Xbin();
    float ybin = map.Ybin();
    float scalexy = 300.;
    float scaletxty = 100000.;
    for( int i=0; i<n; i++ ) {
       //EdbSegP *s = map.CorrLoc(i);
       //float dx   = s->X();
       //float dy   = s->Y();
       //float dtx  = s->TX();
       //float dty  = s->TY();
       
       EdbSegCorr scorr = map.CorrLoc(i);
       float dx   = scorr.V(0);
       float dy   = scorr.V(1);
       float dtx  = scorr.V(3);
       float dty  = scorr.V(4);
       
       float x = map.Xj(i);
       float y = map.Yj(i);

       TArrow *arrowxy = new TArrow(x,y,x+scalexy*dx,y+scalexy*dy,0.005);
       arrowxy->SetLineWidth(2);
       arrowxy->SetLineColor(1);
       arrowxy->Draw();
       
       TArrow *arrowtxty = new TArrow(x,y,x+scaletxty*dtx,y+scaletxty*dty,0.005);
       arrowtxty->SetLineWidth(2);
       arrowtxty->SetLineColor(0);
       arrowtxty->Draw();
       
       TText *t = new TText(x-xbin/3,y-ybin/2.5,Form("%d",map.GetLayer(i)->Ncp()));
       t->SetTextSize(0.02);
       t->Draw();
       //delete s;
    }

    // Draw scale
    float x= map.Xj(0)+xbin/6;
    float y= map.Yj(0)+ybin/6;
    TArrow *arrowxy = new TArrow(x,y,x+scalexy*10,y,0.005);
    arrowxy->SetLineWidth(3);
    arrowxy->SetLineColor(1);
    arrowxy->Draw();
    TText *text = new TText(x,y+ybin/10,"10 microns");
    text->SetTextSize(0.02);
    text->Draw();
    
    //x= map.Xj(0)+xbin/6 + scalexy*10 + xbin/6;
    x= map.Xj(0)+xbin/6;
    y= map.Yj(0)+ybin/6 + ybin;
    TArrow *arrowtxty = new TArrow(x,y,x+scaletxty*0.01,y,0.005);
    arrowtxty->SetLineWidth(3);
    arrowtxty->SetLineColor(0);
    arrowtxty->Draw();
    TText *textt = new TText(x,y+ybin/10,"10 mrad");
    textt->SetTextSize(0.02);
    textt->Draw();

    return corrmap;
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::UpdateLayerWithLocalCorr( EdbLayer &la )
{
  la.Map().ApplyCorrections(eMap);
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::MakeCorrectionsTable()
{
  // assuming that all corrections are calculated produce the output eMap object
  int n = eMapAl.Ncell();
  for(int i=0; i<n; i++) {
       EdbCorrectionBin *bin = GetBin(i);
       EdbLayer *l = eMap.GetLayer(i);
       l->ApplyCorrections( bin->eLayer );
       l->SetNcp(bin->eAl.Ncp());
       l->SubstructCorrections(eLayer);
  }
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::SmoothCorrections()
{
  // in case of low pattern: add segments from 3x3 neighbouring and calculate mean alignment
  int n = eMapAl.Ncell();
  int ncpGood = 0;
  for(int i=0; i<n; i++) if( GetBin(i)->eAl.Ncp() >= eNcpMin ) ncpGood++;
  Log(2,"EdbCorrectionMapper::SmoothCorrections","%d / %d = %6.4f fraction of good alignments (>%d)",
        ncpGood,n, 1.*ncpGood/n, eNcpMin);
  for(int j=0; j<n; j++) {
    EdbCorrectionBin *bin = GetBin(j);
    if(bin->eAl.Ncp() >= eNcpMin)   continue;
    TObjArray arr;
    int ne = eMapAl.SelectObjectsCJ(j,1,arr);
    for(int ie=0; ie<ne; ie++) {
      EdbCorrectionBin *bine = (EdbCorrectionBin *)(arr.At(ie));
      if(bine != bin)  bin->AddBin(*bine);
    }
  }
  
}

//---------------------------------------------------------------------
int EdbCorrectionMapper::InitMap(int nx, float minx, float maxx, int ny, float miny, float maxy )
{
  // assumed that the own histograms of EdbCorrectionMapper are correctly defined
  int n = eMapAl.InitCell(nx, minx, maxx, ny, miny, maxy, 1);
  for(int i=0; i<n; i++) {
    EdbCorrectionBin *bin = new EdbCorrectionBin();
    eMapAl.AddObject(i, bin );
  }
  eMap.Init(eMapAl);
  
  return n;
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::AddSegCouple(EdbSegP *s1, EdbSegP *s2)
{
  EdbCorrectionBin::AddSegCouple(s1,s2);
  EdbCorrectionBin *bin = GetBin(s1->X(),s1->Y());
  if(bin) bin->AddSegCouple(s1,s2);
}

//---------------------------------------------------------------------
void EdbCorrectionMapper::Fill(EdbSegP &s1, EdbSegP &s2)
{
  eHxy1.Fill( s1.X(),s1.Y() );
  eHtxty1.Fill( s1.TX(),s1.TY() );
  eHxy2.Fill( s2.X(),s2.Y() );
  eHtxty2.Fill( s2.TX(),s2.TY() );
  eHdxy.Fill( s2.X()-(s1.X()+s1.TX()*(s2.Z()-s1.Z())), s2.Y()-(s1.Y()+s1.TY()*(s2.Z()-s1.Z())) );
  eHdtxy.Fill( s2.TX()-s1.TX(), s2.TY()-s1.TY() );
  eHdty_ty.Fill( s1.TY(), s2.TY()-s1.TY() );
      
  float t1  = s1.Theta();
  if(t1>.1) {
      float dz = s2.Z()-s1.Z();
      float tx = (s2.X()-s1.X())/dz;
      float ty = (s2.Y()-s1.Y())/dz;
      float t = TMath::Sqrt(tx*tx+ty*ty);
      float dzn = dz*t/t1;
      eHdz.Fill(dzn-dz);
  }
  
  if(t1>.1) {
    float t2  = s2.Theta();
    float shr = t2/t1;
    eHshr.Fill(shr);
  }

}
