//-- Author :  Valeri Tioukov   03.01.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBrick - OPERA Brick structure definition                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbBrick.h"
#include "EdbLog.h"

ClassImp(EdbBrickP)
ClassImp(EdbPlateP)

//====================================================================================
EdbPlateP::EdbPlateP()
{
  for(int i=0; i<3; i++) eLayers.Add(new EdbLayer());
  for(int i=0; i<3; i++) GetLayer(i)->SetID(i);
}

//====================================================================================
void EdbPlateP::Copy(EdbPlateP &pp)
{
  *((EdbLayer*)this) = *((EdbLayer*)&pp);
  for(int i=0; i<3; i++) (*(GetLayer(i)))  = (*(pp.GetLayer(i)));
}

//====================================================================================
void EdbPlateP::TransformSidesIntoPlateRS()
{
  // this function should be applied only once!
  for(int i=0; i<3; i++) {
    GetLayer(i)->CopyCorr( *((EdbLayer *)this) );
    GetLayer(i)->ShiftZ( Z() );
  }
}

//====================================================================================
void EdbPlateP::Print()
{
  printf("\nEdbPlateP:\n");
  ((EdbLayer*)this)->Print();
  printf("has 3 layers:\n");
  for(int i=0; i<3; i++) GetLayer(i)->Print();
}

//====================================================================================
void EdbPlateP::PrintPlateLayout()
{
  printf("EdbPlateP limits: |%f   %f||%f   %f||%f  %f|\n",
	 GetLayer(1)->Zmin(), GetLayer(1)->Zmax(),
	 GetLayer(0)->Zmin(), GetLayer(0)->Zmax(),
	 GetLayer(2)->Zmin(), GetLayer(2)->Zmax() );
  printf("EdbPlateP references: | %f | %f | %f |\n\n", GetLayer(1)->Z(), GetLayer(0)->Z(), GetLayer(2)->Z()); 
}

//____________________________________________________________________________________
void EdbPlateP::SetPlateLayout(float z0, float z1, float z2)
{
  //layers:  |        1       |    0     |      2        |
  //        -z0/2.-z1      -z0/2   0.   z0/2       z0/2.+z2
  float zpl = Z();
  GetLayer(0)->SetZlayer(   0.   ,  -z0/2.     ,  z0/2.    );     // (reference_z, zstart, zfinish) in plate's coord
  GetLayer(1)->SetZlayer( -z0/2. ,  -z0/2.- z1 , -z0/2.    );
  GetLayer(2)->SetZlayer(  z0/2. ,   z0/2.     ,  z0/2.+z2 );
  SetZlayer( zpl, zpl-z0/2.- z1, zpl+z0/2.+z2 ); // total plate dimension and reference - can be resetted by the external function
  Log(3,"EdbPlateP::SetPlateLayout", "|%f  |%f  %f| %f|",-z0/2.- z1, -z0/2., z0/2., z0/2.+z2);
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
void EdbBrickP::Copy(EdbBrickP &b)
{
  *((EdbLayer*)this) = *((EdbLayer*)&b);
  int n = b.Npl();
  for(int i=0; i<n; i++) {
    EdbPlateP *p = new EdbPlateP();
    p->Copy( *(b.GetPlate(i)) );
    AddPlate(p);
  }
}

//____________________________________________________________________________________
void EdbBrickP::SetPlatesLayout(float z0, float z1, float z2)
{
  int n = Npl();
  for(int i=0; i<n; i++) GetPlate(i)->SetPlateLayout(z0, z1, z2);
}

//____________________________________________________________________________________
void EdbBrickP::RemovePlate(int pid)
{
  // remove plate with the id=pid
  int n = Npl();
  for(int i=n-1; i>=0; i--) {
    EdbPlateP *p = GetPlate(i);
    if(p->ID()==pid) ePlates.RemoveAt(i);
  }
  ePlates.Compress();
}

//____________________________________________________________________________________
void EdbBrickP::SetDXDY(float dx, float dy)
{
  ((EdbLayer*)this)->SetDXDY(dx,dy);
  int n = Npl();
  for(int i=0; i<n; i++) GetPlate(i)->SetDXDY(dx,dy);
}
//____________________________________________________________________________________
void EdbBrickP::ResetAffXY()
{
  for(int i=0; i<Npl(); i++) GetPlate(i)->ResetAffXY();
}
//____________________________________________________________________________________
void EdbBrickP::ResetAffTXTY()
{
  for(int i=0; i<Npl(); i++) GetPlate(i)->ResetAffTXTY();
}

//____________________________________________________________________________________
void EdbBrickP::Print()
{
  printf("EdbBrickP with %d plates:\n",Npl());
  ((EdbLayer*)this)->Print();
}

//____________________________________________________________________________________
void EdbBrickP::PrintSides()
{
  printf("EdbBrickP with %d plates:\n",Npl());
  int n = Npl();
  for(int i=0; i<n; i++) {
    printf("plate %d:\n",i);
    GetPlate(i)->GetLayer(1)->Print();
    GetPlate(i)->GetLayer(2)->Print();
  }
}
