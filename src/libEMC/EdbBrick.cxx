//-- Author :  Valeri Tioukov   03.01.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBrick - OPERA Brick structure definition                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbBrick.h"

ClassImp(EdbBrickP)
ClassImp(EdbPlateP)

//====================================================================================
EdbPlateP::EdbPlateP()
{
  for(int i=0; i<3; i++) eLayers.Add(new EdbLayer());
}

//____________________________________________________________________________________
void EdbPlateP::SetPlateLayout(float z0, float z1, float z2)
{
  GetLayer(0)->SetZlayer(     0. ,  -z0/2.     ,  z0/2.    );
  GetLayer(1)->SetZlayer( -z0/2. ,  -z0/2.- z1 , -z0/2.    );
  GetLayer(2)->SetZlayer(  z0/2. ,   z0/2.     ,  z0/2.+z2 );
}

//____________________________________________________________________________________
void EdbPlateP::SetDXDY(float dx, float dy)
{
  ((EdbLayer*)this)->SetDXDY(dx,dy);
  for(int i=0; i<3; i++) GetLayer(i)->SetDXDY(dx,dy);
}

//====================================================================================
EdbBrickP::EdbBrickP()
{
}

//____________________________________________________________________________________
void EdbBrickP::SetPlatesLayout(float z0, float z1, float z2)
{
  int n = Npl();
  for(int i=0; i<n; i++) GetPlate(i)->SetPlateLayout(z0, z1, z2);
}

//____________________________________________________________________________________
void EdbBrickP::SetDXDY(float dx, float dy)
{
  ((EdbLayer*)this)->SetDXDY(dx,dy);
  int n = Npl();
  for(int i=0; i<n; i++) GetPlate(i)->SetDXDY(dx,dy);
}

//____________________________________________________________________________________
void EdbBrickP::Print()
{
  printf("EdbBrickP with %d plates:\n",Npl());
  ((EdbLayer*)this)->Print();
}
