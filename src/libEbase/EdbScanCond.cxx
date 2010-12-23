#include "EdbScanCond.h"
#include "EdbPhys.h"

ClassImp(EdbScanCond)

  using namespace TMath;

///==============================================================================
EdbScanCond::EdbScanCond()
{
  SetDefault();
}

//______________________________________________________________________________
void EdbScanCond::SetDefault()
{
  eSigmaXgr=  .1;
  eSigmaYgr=  .1;
  eSigmaZgr= 3.;

  eDegrad = .6;  // means that at .4 mrad degradation is 24 % in respect to 0 angle

  eSigmaX0  = 8.;   // [microns]
  eSigmaY0  = 8.;   // [microns]
  eSigmaZ0  = 3.;   // [microns]
  eSigmaTX0 = .008; // [mrad]
  eSigmaTY0 = .008; // [mrad]
  

  ePuls0[0]     =  8;    // microtrack puls height (grains)
  ePuls0[1]     = 10;    // at 0 angle
  ePuls04[0]    =  5;    // microtrack puls height (grains)
  ePuls04[1]    =  9;    // at 0.4 rad angle
  eBinX=eBinY   = 3;
  eBinTX=eBinTY = 3;

  eChi2Max  = 3.5;
  eChi2PMax = 3.5;
  eChi2Mode = 0;
  eOffX=0;
  eOffY=0;

  eRadX0 = EdbPhysics::kX0_Cell();
}

//______________________________________________________________________________
void EdbScanCond::Print() const
{
  printf( "******************************************************\n");
  printf( "Scanning Conditions Parametres: %s\n", GetName() );
  printf( "Sigma Grain: \t%f \t%f \t%f \n", 
	  eSigmaXgr,eSigmaYgr,eSigmaZgr);
  printf( "Sigma0 x,y,tx,ty: \t%f \t%f \t%f \t%f \n", 
	  eSigmaX0,eSigmaY0,eSigmaTX0,eSigmaTY0);
  printf( "Angular degradation: \t%f \n", eDegrad);
  printf( "Acceptance bins:   \t%f \t%f \t%f \t%f\n",BinX(),BinY(),BinTX(),BinTY());
  printf( "Puls ramp at 0   angle: \t%f \t%f \n", ePuls0[0],  ePuls0[1]  );
  printf( "Puls ramp at 0.4 angle: \t%f \t%f \n", ePuls04[0], ePuls04[1] );
  printf( "Chi2Max: \t %f\n",Chi2Max());
  printf( "Chi2PMax:\t %f\n",Chi2PMax());
  printf( "eOffX = %f eOffY = %f\n",eOffX,eOffY);
  printf( "******************************************************\n");
}

//______________________________________________________________________________
float EdbScanCond::StepX(float dz) const
{
  float sigma = Sqrt( eSigmaX0*eSigmaX0 + eSigmaTX0*dz*eSigmaTX0*dz );
  return eBinX*sigma;
}

//______________________________________________________________________________
float EdbScanCond::StepY(float dz) const
{
  float sigma = Sqrt( eSigmaY0*eSigmaY0 + eSigmaTY0*dz*eSigmaTY0*dz );
  return eBinY*sigma;
}

//______________________________________________________________________________
float EdbScanCond::Ramp(float x, float x1, float x2) const
{
  float pmin=.01;
  float pmax=1.;
  if(x2<=x1)   return 1.; //error value
  if(x<x1)     return pmin;
  if(x>x2)     return pmax;
  float p = (x-x1)/(x2-x1);
  if(p<pmin)   return pmin;
  if(p>pmax)   return pmax;
  return p;
}

//______________________________________________________________________________
float EdbScanCond::ProbSeg(float tx, float ty, float puls) const
{
  float t = Sqrt(tx*tx + ty*ty);
  return ProbSeg(t, puls);
}

//______________________________________________________________________________
float EdbScanCond::ProbSeg(float t, float puls) const 
{
  // use linear puls ramp approximation
  float pa1 = ePuls0[0] - t/.4*(ePuls0[0]-ePuls04[0]);
  float pa2 = ePuls0[1] - t/.4*(ePuls0[1]-ePuls04[1]);
  return Ramp(puls,pa1,pa2);
}

//______________________________________________________________________________
void EdbScanCond::FillErrorsCov( float tx,float ty, TMatrixD &cov )
{
  // Fill the non-diagonal covariance matrix of errors for the segment with angles tx,ty
  // considering the input sigma being in the track plane (Y - is transversal axis)

  //TODO: remove EdbSegP::SetErrorsCOV

  float theta = Sqrt( tx*tx + ty*ty );
  float sx    = SigmaX(theta);      //TODO the coord error can have different dependance on ang
  float sy    = SigmaY(0);
  float stx   = SigmaTX(theta);
  float sty   = SigmaTY(0);

  cov(0,0) = (double)(sx*sx);
  cov(1,1) = (double)(sy*sy); 
  cov(2,2) = (double)(stx*stx);
  cov(3,3) = (double)(sty*sty);
  cov(4,4) = 1L;                  //TODO sp
  
  //double Phi = -ATan2(ty,tx);    // "-" seems to be a bug
  double Phi = ATan2(ty,tx);
  TMatrixD t(5,5);
  TMatrixD tt(5,5);
  t(0,0) =  Cos(Phi);
  t(0,1) = -Sin(Phi);
  t(1,0) =  Sin(Phi);
  t(1,1) =  Cos(Phi);
  t(2,2) =  Cos(Phi);
  t(2,3) = -Sin(Phi);
  t(3,2) =  Sin(Phi);
  t(3,3) =  Cos(Phi);
  t(4,4) =  1.;
  tt(0,0) =  t(0,0);
  tt(1,0) =  t(0,1);
  tt(0,1) =  t(1,0);
  tt(1,1) =  t(1,1);
  tt(2,2) =  t(2,2);
  tt(3,2) =  t(2,3);
  tt(2,3) =  t(3,2);
  tt(3,3) =  t(3,3);
  tt(4,4) =  t(4,4);
  cov = t*(cov*tt);
}
