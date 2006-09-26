//-- Author :  Valeri Tioukov   6.03.2006

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewDef - microscope view definition                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbViewDef.h"

ClassImp(EdbViewDef)


  //  TODO: take into account the pincushion distortion?
  //
  // h'- h = a*h^3 + b*h^5 + ... 
  // Relatively, D varies as (h'-h)/h = ah2 + bh4.
  // The coefficient a is positive for pincushion and negative for barrel distortion. 
  // Usually the quadratic term outweighs the higher-order terms and D varies with h squared, 
  // getting progressively worse toward the image corners. However, in certain wideangle designs 
  // the quartic term is large enough to overcome the quadratic term

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

  eX0opt=0;
  eY0opt=0;      // optical center of the view (the point vhere the grains are vertical)
  eTXopt=0;      // koeff of the slopes: t(x) = tx*(x-X0opt), 
  eTYopt=0;      // x1=x+t(x)*(z1-z): valid for the clusters inside the grain

  eGrainSX= 0.25;
  eGrainSY= 0.25;
  eGrainSZ= 3.;       // grain size

  eFogGrainArea = 6;  // mean area of the fog grain
  eGrainArea    = 9;  // mean area of the signal grain

  eFogDens = 3;       // per 10x10x10 microns**3

  eZdead = (eZmin+eZmax)/2.;
  eDZdead = 1.;                 //micron
}

//____________________________________________________________________________________
void EdbViewDef::Print()
{
  printf("----------View definition:----------\n");
  printf("X: (%f %f) \t%f\n",eXmin,eXmax,eX0);
  printf("Y: (%f %f) \t%f\n",eYmin,eYmax,eY0);
  printf("Z: (%f %f) \t%f\n",eZmin,eZmax,eZxy);
  printf("Zdead: %f (%f) \n",eZdead,eDZdead);
  printf("Grain Sigmas: %f %f %f\n",eGrainSX,eGrainSY,eGrainSZ);
  printf("Fog: %f \n",eFogDens);
  printf("Frames: %d \n",eNframes);
  printf("------------------------------------\n");
}
