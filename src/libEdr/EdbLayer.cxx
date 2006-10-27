#include "EdbLayer.h"
#include "EdbPhys.h"

ClassImp(EdbLayer)
ClassImp(EdbSegmentCut)
ClassImp(EdbScanCond)

  using namespace TMath;

///==============================================================================
EdbLayer::EdbLayer()
{
  eID=0;
  eZ=0; eZmin=0; eZmax=0;
  eX=0; eY=0; eTX=0; eTY=0;
  eDX=eDY=kMaxInt;
  eShr = 1.;
}

///______________________________________________________________________________
bool EdbLayer::IsInside(float x, float y, float z)
{
  if( x<Xmin() )  return false;
  if( x>Xmax() )  return false;
  if( y<Ymin() )  return false;
  if( y>Ymax() )  return false;
  if( z<Zmin() )  return false;
  if( z>Zmax() )  return false;
  return true;
}

///______________________________________________________________________________
bool EdbLayer::IsInside(float x, float y)
{
  if( x<Xmin() )  return false;
  if( x>Xmax() )  return false;
  if( y<Ymin() )  return false;
  if( y>Ymax() )  return false;
  return true;
}

///______________________________________________________________________________
void EdbLayer::Print()
{
  printf("Layer:\t%d\n",eID);
  printf("X0Y0:\t%f %f\n", eX,eY );
  printf("DXDY:\t%f %f\n", eDX,eDY );
  printf("ZLAYER\t%f %f %f\n",eZ,eZmin,eZmax);
  printf("SHR\t%f\n",eShr);
  printf("AFFXY\t");        eAffXY.Print();
  printf("AFFTXTY\t");      eAffTXTY.Print();
}

///==============================================================================
EdbSegmentCut::EdbSegmentCut(int xi, float var[10])
{
  eXI=xi; 
  for(int i=0;i<5;i++) {
    eMin[i]=var[i*2]; 
    eMax[i]=var[i*2+1];
  }
}

///______________________________________________________________________________
int EdbSegmentCut::PassCut(float var[5])
{
  if     (eXI==0)  return PassCutX(var);
  else if(eXI==1)  return PassCutI(var);
  return 0;
}

///______________________________________________________________________________
int EdbSegmentCut::PassCutX(float var[5])
{
  // exclusive cut: if var is inside cut volume - return 0

  for(int i=0; i<5; i++) {
    if(var[i]<eMin[i])  return 1;
    if(var[i]>eMax[i])  return 1;
  }
  return 0;
}

///______________________________________________________________________________
const char *EdbSegmentCut::CutLine(char *str, int i, int j) const
{
  if(eXI==0) {

    sprintf(str,
	  "  TCut x%1d%2.2d = \"!(eX0>%f&&eX0<%f && eY0>%f&&eY0<%f && eTx>%f&&eTx<%f && eTy>%f&&eTy<%f && ePuls>%f&&ePuls<%f)\";\n"
	  ,i,j,eMin[0],eMax[0], eMin[1],eMax[1], eMin[2],eMax[2], eMin[3],eMax[3], eMin[4],eMax[4]);

  } else if(eXI==1) {

    sprintf(str,
	  "  TCut i%1d%2.2d = \"(eX0>%f&&eX0<%f && eY0>%f&&eY0<%f && eTx>%f&&eTx<%f && eTy>%f&&eTy<%f && ePuls>%f&&ePuls<%f)\";\n"
	  ,i,j,eMin[0],eMax[0],eMin[1],eMax[1],eMin[2],eMax[2],eMin[3],eMax[3],eMin[4],eMax[4]);
  }
  return str;
}

///______________________________________________________________________________
int EdbSegmentCut::PassCutI(float var[5])
{
  // inclusive cut: if var is inside cut volume - return 1

  for(int i=0; i<5; i++) {
    if(var[i]<eMin[i])  return 0;
    if(var[i]>eMax[i])  return 0;
  }
  return 1;
}

///______________________________________________________________________________
void EdbSegmentCut::Print()
{
}

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
  eSigmaTX0 = .008; // [mrad]
  eSigmaTY0 = .008; // [mrad]

  ePuls0[0]     =  8;    // microtrack puls height (grains)
  ePuls0[1]     = 10;    // at 0 angle
  ePuls04[0]    =  5;    // microtrack puls height (grains)
  ePuls04[1]    =  9;    // at 0.4 rad angle

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
  
  double Phi = -ATan2(ty,tx);
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
  TMatrixD c(5,5);
  cov = t*(cov*tt);
}
