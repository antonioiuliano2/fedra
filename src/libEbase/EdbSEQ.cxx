//-- Author :  Valeri Tioukov   19/10/2010
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSEQ                                                               //
//                                                                      //
//  segments density equalizer                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TMath.h"
#include "TCanvas.h"
#include "TArrayF.h"
#include "TClonesArray.h"
#include "EdbLog.h"
#include "EdbSegP.h"
#include "EdbSEQ.h"

ClassImp(EdbSEQ)
using namespace TMath;

//------------------------------------------------------------------------
EdbSEQ::~EdbSEQ()
{
  SafeDelete(eXLimits);
  SafeDelete(eYLimits);
  SafeDelete(eWLimits);
  SafeDelete(eThetaLimits);
  SafeDelete(eChiLimits);
}

//------------------------------------------------------------------------
void EdbSEQ::Set0()
{
  eS0mt   = 270.*340.;   // area unit for Nseg calculation in microns
  //eNviews = 1;         // number of views used for total area calculation
  eArea   = eS0mt;       // effective area of the pattern to be equalize
  eNsigma=4;
  eNP = 100;
  eExcludeThetaRange.SetOwner(1);
  eXLimits    =0;
  eYLimits    =0;
  eThetaLimits=0;
  eWLimits    =0;
  eChiLimits  =0;
}

//-------------------------------------------------------------------------
void EdbSEQ::PrintLimits()
{
  if(eXLimits)     printf("XLimits:     %f %f\n",eXLimits->X(),eXLimits->Y());
  if(eYLimits)     printf("YLimits:     %f %f\n",eYLimits->X(),eYLimits->Y());
  if(eWLimits)     printf("WLimits:     %f %f\n",eWLimits->X(),eWLimits->Y());
  if(eChiLimits)   printf("ChiLimits:   %f %f\n",eChiLimits->X(),eChiLimits->Y());
  if(eThetaLimits) printf("ThetaLimits: %f %f\n",eThetaLimits->X(),eThetaLimits->Y());
}

//-------------------------------------------------------------------------
TH1F *EdbSEQ::ThetaPlot(EdbPattern &arr, const char *name, const char *title )
{
  int n = arr.N();
  if(!n) return 0;
  TH1F *h = new TH1F( name, Form("%s normalised to area of %8.1f mm²",title,eArea/1000./1000.), 80, 0., 0.8 );
  for(int i=0; i<n; i++) {
    EdbSegP *s = arr.GetSegment(i);
    //h->Fill(s->Theta()*s->Theta());
    h->Fill(s->Theta());
  }
  h->GetXaxis()->SetTitle("theta");
  //h->GetXaxis()->SetTitle("theta²");
  if(eArea>0) h->Scale(1000000./eArea);
  return h;
}

//-------------------------------------------------------------------------
TH1F *EdbSEQ::ThetaPlot(TObjArray &arr, const char *name, const char *title )
{
  int n = arr.GetEntries();
  if(!n) return 0;
  TH1F *h = new TH1F( name, Form("%s normalised to area of %8.1f mm²",title,eArea/1000./1000.), 80, 0., 0.8 );
  for(int i=0; i<n; i++) {
    EdbSegP *s = (EdbSegP*)(arr.At(i));
    //h->Fill(s->Theta()*s->Theta());
    h->Fill(s->Theta());
  }
  //h->GetXaxis()->SetTitle("theta²");
  h->GetXaxis()->SetTitle("theta");
  if(eArea>0) h->Scale(1000000./eArea);
  return h;
}

//-------------------------------------------------------------------------
double EdbSEQ::DNmt(double t)
{
  // return dN/dTheta : the critical number of microtracks for the given
  //     theta range for a view*nviews area

  double dpl = DP(t,DPLmt(t),DALmt(t),eDZbase);
  double dpt = DP(t,DPTmt(t),DATmt(t),eDZbase);
  double nang = Sin(t)/DATmt(t) * DALmt(0)/DALmt(t);
  //return nang * eNviews*eS0mt/dpl/dpt /eNsigma/eNsigma/eNsigma/eNsigma;
  return nang * eArea/dpl/dpt /eNsigma/eNsigma/eNsigma/eNsigma;
}

//-------------------------------------------------------------------------
double EdbSEQ::DNbt(double t)
{
  // return dN/dTheta : the critical number of microtracks for the given
  //     theta range for a view area

  double dal0 = SqSum(DALbt(0),eSaPlate);
  double dal  = SqSum(DALbt(t),eSaPlate);
  double dat  = SqSum(DATbt(t),eSaPlate);
  double dpl  = DP( t, SqSum( DPLbt(t),eSxyPlate ), dal, eDZcell );
  double dpt  = DP( t, SqSum( DPTbt(t),eSxyPlate ), dat, eDZcell );
  double nang = Sin(t)/dat * dal0/dal;
  return nang * eArea/dpl/dpt /eNsigma/eNsigma/eNsigma/eNsigma;
}

//------------------------------------------------------------------------
void EdbSEQ::Draw()
{
  TCanvas *cn = new TCanvas("cn","Critical density",800,600);
  cn->Divide(2,2);

  TF1 *fnmt = new TF1("fnmt",this,&EdbSEQ::FDNmt,0.,1.,0,"EdbSEQ","FDNmt");
  fnmt->SetTitle("dNmt/dTheta in 1 view density function");
  fnmt->SetNpx(eNP);

  cn->cd(1);
  fnmt->Draw();
  cn->cd(2);
  fnmt->DrawIntegral();

  TF1 *fnbt = new TF1("fnbt",this,&EdbSEQ::FDNbt,0.,1.,0,"EdbSEQ","FDNbt");
  fnbt->SetTitle("dNbt/dTheta in 1 view density function");
  fnbt->SetNpx(eNP);

  cn->cd(3);
  fnbt->Draw();
  cn->cd(4);
  fnbt->DrawIntegral();
}

//------------------------------------------------------------------------
void EdbSEQ::CalculateDensityMT( EdbH1 &hEq)
{
  Log(3,"EdbSEQ::CalculateDensityMT","hEq.N(): %d", hEq.N());
  TF1 *fnmt = new TF1("fnmt",this,&EdbSEQ::FDNmt,hEq.Xmin(),hEq.Xmax(),0,"EdbSEQ","FDNmt");
  fnmt->SetTitle("dNmt/dTheta in 1 view density function");
  fnmt->SetNpx(eNP);
  double *x=new double[eNP];
  double *w=new double[eNP];
  fnmt->CalcGaussLegendreSamplingPoints(eNP,x,w,1e-15);
  int n = hEq.N();
  for(int i=0; i<n; i++) {
    float tmi=i*hEq.Xbin();
    float tma=(i+1)*hEq.Xbin();
    double sbin = fnmt->IntegralFast(eNP,x,w,tmi,tma);
    Log(3,"EdbSEQ::CalculateDensityMT","i,eNP,x,w,tmi,tma: %d %d %f %f %f %f   %lf", i,eNP,x,w,tmi,tma, sbin );
    if(sbin>0 && sbin<kMaxInt-2) hEq.SetBin(i, int(sbin)+1 );
    //Log(3,"EdbSEQ::CalculateDensityMT","Integral(%f,%f) = %f", tmi, tma, float(sbin) );
  }
  Log(3,"EdbSEQ::CalculateDensityMT","Integral(0,0.6) = %f", float(fnmt->IntegralFast(eNP,x,w,0,0.6)) );
}

//------------------------------------------------------------------------
void EdbSEQ::PreSelection(EdbPattern &pi, TObjArray &po)
{
  int nseg = pi.N();
  for(int i=0; i<nseg; i++) {
    EdbSegP *s = pi.GetSegment(i);
    if(eChiLimits) {
      if( s->Chi2() < eChiLimits->X() )  continue;
      if( s->Chi2() > eChiLimits->Y() )  continue;
    }
    if(eXLimits) {
      if( s->X() < eXLimits->X() )  continue;
      if( s->X() > eXLimits->Y() )  continue;
    }
    if(eYLimits) {
      if( s->Y() < eYLimits->X() )  continue;
      if( s->Y() > eYLimits->Y() )  continue;
    }
    if(eWLimits) {
      if( s->W() < eWLimits->X() )  continue;
      if( s->W() > eWLimits->Y() )  continue;
    }
    if(eThetaLimits) {
      if( s->Theta() < eThetaLimits->X() )  continue;
      if( s->Theta() > eThetaLimits->Y() )  continue;
    }
    po.Add(s);
  }
}

//------------------------------------------------------------------------
void EdbSEQ::AddExcludeThetaRange( EdbSegP &s )
{
  eExcludeThetaRange.Add( new EdbSegP(s) );
}

//------------------------------------------------------------------------
void EdbSEQ::ExcludeThetaRange(TObjArray &mti, TObjArray &mto)
{
   // exclude 1 sigma in theta around all requested segments
  int nseg   = mti.GetEntriesFast();
  for(int i=0; i<nseg; i++) {
    EdbSegP *s = (EdbSegP*)(mti.UncheckedAt(i));
    if(!IsInsideThetaRange(s)) mto.Add(s);
  }
}

//------------------------------------------------------------------------
bool EdbSEQ::IsInsideThetaRange(EdbSegP *s)
{
   // exclude 1 sigma in theta around all requested segments
   int nrange = eExcludeThetaRange.GetEntriesFast();
   for(int j=0; j<nrange; j++) {
     EdbSegP *sex = (EdbSegP*)(eExcludeThetaRange.UncheckedAt(j));
     float dtx = s->TX() - sex->TX();
     float dty = s->TY() - sex->TY();
     if( (dtx*dtx/sex->STX()/sex->STX()+dty*dty/sex->STY()/sex->STY())<1. ) return 1;
  }
  return 0;
}

//------------------------------------------------------------------------
void EdbSEQ::EqualizeMT(TObjArray &mti, TObjArray &mto, Double_t area)
{
  // Input:  mti  - microtracks array
  // Output: mto  - selected microtracks array

  eArea=area;
  EdbH1 hEq;  hEq.InitH1( 40, 0, 0.8 );
  CalculateDensityMT(hEq);         // define critical theta density
  if(gEDBDEBUGLEVEL>2) hEq.Print();
  //hEq.DrawH1()->Draw();

  TClonesArray  hw("EdbH1",hEq.N());    // create W-hist per each theta bin
  TArrayF thres(hEq.N());               // W-thresholds to be defined
  for(int i=0; i<hEq.N(); i++) {
	new(hw[i]) EdbH1();
	((EdbH1*)hw[i])->InitH1( 500, 5.*50, 17.*50. );
  }

  int nseg = mti.GetEntriesFast();
  for(int i=0; i<nseg; i++) {
    EdbSegP *s = (EdbSegP*)(mti.UncheckedAt(i));
    int jtheta = hEq.IX(s->Theta());
    if(jtheta>hEq.N()-1)  continue;
    float w = Wmt(*s);
    ((EdbH1*)hw[jtheta])->Fill(w);
  }

  for(int i=0; i<hEq.N(); i++) {
    int eccess = ((EdbH1*)hw[i])->Integral() - hEq.Bin(i);
    if( eccess <= 0 ) continue;  // the threshold remains 0

    int nsum=0;
    for(int j=0; j<((EdbH1*)hw[i])->N(); j++) {
      nsum+=((EdbH1*)hw[i])->Bin(j);
      if( nsum >= eccess ) { thres[i] = ((EdbH1*)hw[i])->X(j)-((EdbH1*)hw[i])->Xbin(); break; }
    }    
  }

  for(int i=0; i<nseg; i++) {
    EdbSegP *s = (EdbSegP*)(mti.UncheckedAt(i));
    int jtheta = hEq.IX(s->Theta());
    if(jtheta>hEq.N()-1)  continue;
    float w = Wmt(*s);
    if( w > thres[jtheta] ) 
      if(!IsInsideThetaRange(s)) 
        mto.Add(s);
  }
  
  Log(2,"EdbSEQ::EqualizeMT","selected %3.1f\%  (%d of %d) segments in area of %7.1f cm²", 100.*mto.GetEntriesFast()/nseg, mto.GetEntriesFast(), nseg, (float)(eArea/1000./1000./10./10.) );
}
