//-- Author :  Valeri Tioukov   19/10/2010
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSigma                                                             //
//                                                                      //
//  segments accuracy functions                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TMath.h"
#include "TCanvas.h"
#include "TF1.h"
#include "EdbSigma.h"

ClassImp(EdbSigma)

//------------------------------------------------------------------------
void EdbSigma::Set0()
{
  // defines the mt accuracy:
  eSxy =  0.35;   // mean in-view grain accuracy xy 
  eSz  =  2.;     // grains z-uncertainty "field depth"
  eDZem = 45.;    // emulsion thickness

  // defines the bt accuracy:
  eDZbase  = 210.; //emulsion base 
  eDZcell  =1300.; // plate-to plate distance

  eSxyZone  =  2.;    //"in-zone" microtrack position tolerance (local deformations)
  eSaZone   =  0.01;  //"in-zone" microtrack angular tolrance (incorrect shrinkage+offsets)
  eSxyPlate =  5.;    //"in-plate" basetrack position tolerance (XY plate deformations)
  eSaPlate  =  0.003; //"in-plate" basetrack angular tolrance (nonplanarity)
}

//------------------------------------------------------------------------
double EdbSigma::DAL(double t, double sxy, double sz, double dz)
{
  // return the estimation of the longitudinal angular error of segment (theta error) due to points uncertanty
  return Sqrt( sxy*Cos(t)*sxy*Cos(t) + sz*Sin(t)*sz*Sin(t)) / (dz/Cos(t));
}

//------------------------------------------------------------------------
double EdbSigma::DAT(double t, double sxy, double dz )
{
  // return the estimation of the transverse angular error of segment due to points uncertanty
  return sxy/(dz/Cos(t));
}

//------------------------------------------------------------------------
double EdbSigma::DP(double t, double sxy, double da, double dz)
{
  // return the estimation of the position error of segment propagated to dz
  double dpt = dz/Cos(t)*da /Cos(t);
  return Sqrt(sxy*sxy + dpt*dpt);
}

//------------------------------------------------------------------------
double EdbSigma::DALmt(double t)
{
  // return the estimation of the longitudinal angular error of microtrack (theta error)
  return DAL(t,eSxy, eSz, eDZem);
}

//------------------------------------------------------------------------
double EdbSigma::DATmt(double t)
{
  // return the estimation of the transverse angular error of microtrack
  return DAT(t,eSxy,eDZem);
}

//------------------------------------------------------------------------
double EdbSigma::DPLmt(double t)
{
  // return the estimation of the longitudinal position error of microtrack
  return DP(t, eSxy, DALmt(t), eDZem);
}

//------------------------------------------------------------------------
double EdbSigma::DPTmt(double t)
{
  // return the estimation of the transverse position error of microtrack
  return DP(t, eSxy, DATmt(t), eDZem);
}


//------------------------------------------------------------------------
double EdbSigma::DALbt(double t)
{
  // return the estimation of the longitudinal angular error of basetrack (theta error)
  return DAL(t, DPLmt(t), eSz, eDZbase);
}

//------------------------------------------------------------------------
double EdbSigma::DATbt(double t)
{
  // return the estimation of the transverse angular error of basetrack
  return DAT(t, DATmt(t), eDZbase);
}

//------------------------------------------------------------------------
double EdbSigma::DPLbt(double t)
{
  // return the estimation of the longitudinal position error of basetrack

  return DPLmt(t);
}

//------------------------------------------------------------------------
double EdbSigma::DPTbt(double t)
{
  // return the estimation of the transverse position error of basetrack
  return DPTmt(t);
}

//------------------------------------------------------------------------
void EdbSigma::Draw()
{
  double fmin=0, fmax=1.;
  TCanvas *cmt = new TCanvas("mt","Intrinsic emulsion resolutions",800,600);
  cmt->Divide(2,2);

  EdbSigma *pts = new EdbSigma();
  
  cmt->cd(1);
  TF1 *dal = new TF1("dal",pts,&EdbSigma::FDAL,fmin,fmax,3,"EdbSigma","FDAL");
  dal->SetParameter(0, eSxy);
  dal->SetParameter(1, eSz);
  dal->SetParameter(2, eDZem);
  dal->SetTitle("angle longitudinal microtrack resolution");
  dal->Draw();

  cmt->cd(2);
  TF1 *dat = new TF1("dat",pts,&EdbSigma::FDAT,fmin,fmax,2,"EdbSigma","FDAT");
  dat->SetParameter(0, eSxy);
  dat->SetParameter(1, eDZem);
  dat->SetTitle("angle transverse microtrack resolution");
  dat->Draw();

  cmt->cd(4);
  TF1 *dplmt = new TF1("dplmt",pts,&EdbSigma::FDPLmt,fmin,fmax,0,"EdbSigma","FDPLmt");
  dplmt->SetTitle("position longitudinal microtrack resolution");
  dplmt->Draw();


  cmt->cd(3);
  TF1 *dalbt = new TF1("dalbt",pts,&EdbSigma::FDALbt,fmin,fmax,0,"EdbSigma","FDALbt");
  dalbt->SetTitle("angle longitudinal basetrack resolution");
  dalbt->Draw();

}
