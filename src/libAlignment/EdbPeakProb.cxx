//-- Author :  Valeri Tioukov   02/10/2008
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPeakProb                                                          //
//                                                                      //
// class to estimate the peak significance                              //
//                                                                      //
// The different cases to be studied:
// 1) peak size << histogram size (ex 1 bin)
//     Method: compare the singnal bin with the BG fluctuation
// 2) peak size comparable with the histogram size (many bins)
//
//////////////////////////////////////////////////////////////////////////

#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "EdbLog.h"
#include "EdbPeakProb.h"

using namespace TMath;

ClassImp(EdbPeakProb)
//----------------------------------------------------------------
EdbPeakProb::EdbPeakProb()
{
  ePeakRMSmin = 0;
  ePeakRMSmax = 100;   // must be set!
  ePeakNmin   = 3;     // must be set!  
  ePeakNmax   = kMaxInt;
  eVerbosity  = 2;     // 0-no any message, 1-print, 2-plot; default=2

  eProb= ePeakX= ePeakY= ePeakRMS= 0;
  eHbin=0;
  eHpeak=0;
}

//----------------------------------------------------------------
EdbPeakProb::~EdbPeakProb()
{
  SafeDelete(eHbin);
  SafeDelete(eHpeak);
}

//----------------------------------------------------------------
Float_t EdbPeakProb::Prob()
{
  float prob = Probability2D( eHD, ePeakX, ePeakY);
  return prob;
}

//---------------------------------------------------------------------
float  EdbPeakProb::Probability2D(TH2F *hd, float &xpeak, float &ypeak)
{
  // estimate the probability that the highest bin of the histogram do not belong to the background
  // Method: estimate the deviation of the tail of the coincidence distribution from exponential shape

  float prob=0;

  float maxbin = hd->GetMaximum();
  eHbin = new TH1F("hbin","hbin", (int)maxbin+1, 0, maxbin+1);
  int nx = hd->GetNbinsX();
  int ny = hd->GetNbinsY();
  for(int ix=1; ix<=nx; ix++)
    for(int iy=1; iy<=ny; iy++)
      eHbin->Fill( hd->GetBinContent(ix,iy) );

  float tailbin =  eHbin->GetMean()+eHbin->GetRMS();
  eHbin->Fit("expo","QR0","",tailbin,maxbin);
  float a = eHbin->GetFunction("expo")->GetParameter(0);
  float b = eHbin->GetFunction("expo")->GetParameter(1);

  float lowlim = (int)((-a)/b+0.99999);
  if(lowlim<1) lowlim=1;

  float real = eHbin->GetBinContent((int)maxbin+1);
  if( lowlim>maxbin )    prob=0;
  else                   prob = 1.- eHbin->GetFunction("expo")->Integral(maxbin,2*(maxbin+1)+10) / real;

  // calculate mean and rms for bins>lowlim to check the peak integrity

  eHpeak = (TH2F*)hd->Clone("hdpeak");
  eHpeak->Reset();

  float bin;
  for(int ix=1; ix<=nx; ix++)
    for(int iy=1; iy<=ny; iy++) {
      bin  = hd->GetBinContent(hd->GetBin(ix, iy));
      if(bin<lowlim) continue;
      eHpeak->Fill( hd->GetXaxis()->GetBinCenter( ix ),
		    hd->GetYaxis()->GetBinCenter( iy ),
		    bin );
    }
  Log(3, "AlignmentProbability", "peak: ( %f %f )   rms: ( %f %f )",  
	 eHpeak->GetMean(1), eHpeak->GetMean(2), eHpeak->GetRMS(1),eHpeak->GetRMS(2));

  if(eVerbosity>1) {
    TCanvas *c = new TCanvas("aa","aa");
    c->Divide(2,2);
    c->cd(1); hd->Draw("colz");
    c->GetPad(2)->SetLogy();
    c->cd(2); eHbin->Draw();    eHbin->GetFunction("expo")->Draw("same");
    //c->cd(3); hd->SetMinimum(lowlim); hd->Draw();
    c->cd(3);
    eHpeak->Draw("colz");
  } else {
    SafeDelete(eHbin);
    SafeDelete(eHpeak);
  }

  Log(2,"Probability2D","%f   BGlim=%6.1f, maxbin=%6.1f",prob,lowlim,maxbin);
  return prob;
}
