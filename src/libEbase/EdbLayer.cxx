#include "EdbLayer.h"
#include "EdbPhys.h"

ClassImp(EdbLayer)
ClassImp(EdbSegmentCut)

  using namespace TMath;

///==============================================================================
EdbLayer::EdbLayer()
{
  eID=0;
  eZ=0; eZmin=0; eZmax=0;
  eX=0; eY=0; eTX=0; eTY=0;
  eDX=eDY=kMaxInt;
  eShr = 1.;
  eZcorr=0;
}

///______________________________________________________________________________
void EdbLayer::Copy(EdbLayer &l)
{
  eID = l.ID();
  eZ = l.Z(); eZmin=l.Zmin(); eZmax=l.Zmax();
  eX=l.X(); eY=l.Y(); eTX=l.TX(); eTY=l.TY();
  eDX=l.DX(); eDY=l.DY();
  CopyCorr(l);
}

///______________________________________________________________________________
void EdbLayer::ResetCorr()
{
  eShr = 1.;
  eZcorr=0;
  eAffXY.Reset();
  eAffTXTY.Reset();
}

///______________________________________________________________________________
void EdbLayer::ShiftZ(float dz)
{
  eZ += dz;  eZmin +=dz;  eZmax +=dz;
}

///______________________________________________________________________________
void EdbLayer::CopyCorr(EdbLayer &l)
{
  ResetCorr();
  ApplyCorrections( l.Shr(), l.Zcorr(), *(l.GetAffineXY()), *(l.GetAffineTXTY()) );
}

///______________________________________________________________________________
void EdbLayer::ApplyCorrections(float shr, float zcorr, EdbAffine2D &affxy, EdbAffine2D &afftxty)
{
  eShr *= shr;
  eZcorr += zcorr;
  eAffXY.Transform(affxy);
  eAffTXTY.Transform(afftxty);
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
  //printf("X0Y0:\t%f %f\n", eX,eY );
  //printf("DXDY:\t%f %f\n", eDX,eDY );
  printf("ZLAYER\t%f %f %f\n",eZ,eZmin,eZmax);
  printf("SHR\t%f\n",eShr);
  printf("Zcorr\t%f\n",eZcorr);
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
