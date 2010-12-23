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
#include "EdbSegP.h"
#include "EdbSEQ.h"

ClassImp(EdbSEQ)
using namespace TMath;

//------------------------------------------------------------------------
EdbSEQ::~EdbSEQ()
{
}

//------------------------------------------------------------------------
void EdbSEQ::Set0()
{
  eS0mt   = 270.*340.; // area unit for Nseg calculation in microns
  eDZcell = 1300.;
  eNsigma=4;
  eNP = 100;
}

//-------------------------------------------------------------------------
double EdbSEQ::DNmt(double t)
{
  // return dN/dTheta : the critical number of microtracks for the given
  //     theta range for a view area

  double dpl = DP(t,DPLmt(t),DALmt(t),eDZbase);
  double dpt = DP(t,DPTmt(t),DATmt(t),eDZbase);
  double nang = Sin(t)/DATmt(t) * DALmt(0)/DALmt(t);
  return nang * eS0mt/dpl/dpt /eNsigma/eNsigma/eNsigma/eNsigma;
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
  return nang * eS0mt/dpl/dpt /eNsigma/eNsigma/eNsigma/eNsigma;
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
void EdbSEQ::CalculateDensityMT( EdbH1 &hEq, float area )
{
  TF1 *fnmt = new TF1("fnmt",this,&EdbSEQ::FDNmt,hEq.Xmin(),hEq.Xmax(),0,"EdbSEQ","FDNmt");
  fnmt->SetTitle("dNmt/dTheta in 1 view density function");
  fnmt->SetNpx(eNP);
  double *x=new double[eNP];
  double *w=new double[eNP];
  fnmt->CalcGaussLegendreSamplingPoints(eNP,x,w,1e-15);
  for(int i=0; i<hEq.N(); i++) {
    float tmi=i*hEq.Xbin();
    float tma=(i+1)*hEq.Xbin();
    hEq.SetBin(i, int(fnmt->IntegralFast(eNP,x,w,tmi,tma))+1 );
    printf("Integral(%f,%f) = %f\n", tmi, tma, float(fnmt->IntegralFast(eNP,x,w,tmi,tma)) );
  }
  printf("Integral(0,0.6) = %f\n",float(fnmt->IntegralFast(eNP,x,w,0,0.6)) );
}

//------------------------------------------------------------------------
void EdbSEQ::EqualizeMT(TObjArray &mti, TObjArray &mto, float area)
{
  // Input:  mti  - microtracks array
  //         area - effective scanned area
  // Output: mto  - selected microtracks array

  EdbH1 hEq;  hEq.InitH1( 16, 0, 0.8 );
  CalculateDensityMT(hEq,area);         // define critical theta density
  hEq.Print();
  hEq.DrawH1()->Draw();

  EdbH1   hw[hEq.N()];                  // create W-hist per each theta bin
  TArrayF thres(hEq.N());               // W-thresholds to be defined
  for(int i=0; i<hEq.N(); i++) {
    hw[i].InitH1( 200, 0., 20. );
  }

  int nseg = mti.GetEntriesFast();
  for(int i=0; i<nseg; i++) {
    EdbSegP *s = (EdbSegP*)(mti.UncheckedAt(i));
    int jtheta = hEq.IX(s->Theta());
    if(jtheta>hEq.N()-1)  continue;
    float w = s->W();                               // TODO: smear with chi2 or with other criteria?
    hw[jtheta].Fill(w);
  }

  for(int i=0; i<hEq.N(); i++) {
    int eccess = hw[i].Integral() - hEq.Bin(i);
    if( eccess <= 0 ) continue;  // the threshold remains 0

    int nsum=0;
    for(int j=0; j<hw[i].N(); j++) {
      nsum+=hw[i].Bin(j);
      if( nsum >= eccess ) { thres[i] = hw[i].X(j)-hw[i].Xbin(); break; }
    }    
  }

  for(int i=0; i<nseg; i++) {
    EdbSegP *s = (EdbSegP*)(mti.UncheckedAt(i));
    int jtheta = hEq.IX(s->Theta());
    if(jtheta>hEq.N()-1)  continue;
    float w = s->W();                               // TODO: smear with chi2 or with other criteria?
    if( w > thres[jtheta] ) mto.Add(s);
  }
}
