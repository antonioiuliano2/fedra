//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPhys                                                              //
//                                                                      //
// Class for Emulsion Physics                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "EdbPhys.h"

ClassImp(EdbPhysics);

//______________________________________________________________________________

const float EdbPhysics::kX0_Pb   =   5600.; // pure Pb [microns]
const float EdbPhysics::kX0_Em   = 286000.; // pure Emulsion (?)
const float EdbPhysics::kX0_Cell =   5810.; // Pb+emulsion mixture [microns]

//______________________________________________________________________________
double EdbPhysics::ThetaMS2( float p, float mass, float dx, float X0 )
{
  // calculate the square of multiple scattering angle theta (in one projection)
  // after the distance dx [microns] in the media with radiation length X0

  if (p < 0.0000001) return 2.;
  double k = 0.0136*0.0136;    // [GeV]
  double p2 = p*p;
  double p4=p2*p2;
  return  abs(k*(mass*mass+p2)*dx/p4/X0);
}

//________________________________________________________________________
double EdbPhysics::ThetaPb2( float p, float mass, float dx ) 
    { return ThetaMS2(p,mass,dx, kX0_Pb); }

