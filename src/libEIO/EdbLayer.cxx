#include "EdbLayer.h"

ClassImp(EdbLayer)

///==============================================================================
EdbLayer::EdbLayer()
{
  eID=0;
  eZ=0; eZmin=0; eZmax=0;
  eX=0; eY=0; eTX=0; eTY=0;
  eDX=0; eDY=0; kMaxInt;
  eShr = 1.;
}

///==============================================================================
void EdbLayer::Print()
{
  printf("Layer:\t %d\n",eID);
  printf("ZLAYER\t %f %f %f\n",eZ,eZmin,eZmax);
  printf("SHR\t %f\n",eShr);
  printf("AFFXY\t");        eAffXY.Print();
  printf("AFFTXTY\t");      eAffTXTY.Print();
}

///==============================================================================
