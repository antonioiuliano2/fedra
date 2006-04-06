//-- Author :  Valeri Tioukov   6.03.2006

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewDef - microscope view definition                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbViewDef.h"

ClassImp(EdbViewDef)

//____________________________________________________________________________________
EdbViewDef::EdbViewDef()
{
	SetDef();
}

//____________________________________________________________________________________
void EdbViewDef::SetDef()
{
  eNframes =16;   // number of frames
  eZmin=0;
  eZmax=45;       // limits in z
  eZxy=0;         // intersect plane

  eXmin=-200;
  eXmax= 200;    // limits of the view
  eYmin=-200;
  eYmax= 200;    // limits of the view
  eX0=0;
  eY0=0;         // center of the view

  eSx= 0.25;
  eSy= 0.25;
  eSz= 3.;       // grain size

  eFogGrainArea = 6;  // mean area of the fog grain
  eGrainArea    = 9;  // mean area of the signal grain

  eFogDens = 3;       // per 10x10x10 microns**3

  eZdead = (eZmin+eZmax)/2.;
  eDZdead = 1.;                 //micron
}

//____________________________________________________________________________________
void EdbViewDef::Print()
{
  printf("\n");
  printf("X: (%f %f) \t%f\n",eXmin,eXmax,eX0);
  printf("Y: (%f %f) \t%f\n",eYmin,eYmax,eY0);
  printf("Z: (%f %f) \t%f\n",eZmin,eZmax,eZxy);

  printf("Zdead: %f (%f) \n",eZdead,eDZdead);

  printf("Sigmas: %f %f %f\n",eSx,eSy,eSz);

  printf("Fog: %f \n",eFogDens);
}
