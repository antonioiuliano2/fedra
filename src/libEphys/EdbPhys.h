#ifndef ROOT_EdbPhys
#define ROOT_EdbPhys

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPhys                                                              //
//                                                                      //
// Some physics utilities and constants                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TObject.h>
#include <TRandom.h>

//_________________________________________________________________________
class EdbPhysics: public TObject {
 private:
  static const double geu=0.577215;
  static const double geu1=0.422785;
  static const double me=0.000511; // Gev
  static const double me_kg=9.10938188E-31; // kg
  static const double mpi=0.13957; // Gev
  static const double ro=11.35; // g/cm**3
  static const double Z=82.;
  static const double A=207.2;
  static const double Nav=6.02214199E+23; // 1/mol
  static const double I=844.408; // eV
  static const double Nel=2.705E+24;     // 1/cm**3
  static const double h=4.1357E-15; // eV*s
  static const double D=0.000153537; // (GeV * cm**2)/s 
  static const double ec=1.602176462E-19; // Coulombs
  static const double vp=1.5577E+11; // 1/s
  static const double C=-28.1723;
  static const double Xa=6.1164;
  static const double X0=7.6842;
  static const double X1=3.0;
  static const double mm=2.0;
  static const double aa=-0.3291;
 public:
  static const float kX0_Pb;
  static const float kX0_Em;
  static const float kX0_Cell;

  EdbPhysics(){}
  virtual ~EdbPhysics(){}

  static double ThetaMS2( float p, float mass, float dx, float X0 );
  static double ThetaPb2( float p, float mass, float dx );
  static double DeAveragePb( float p, float mass, float dx);
  static double DeLandauPb( float p, float mass, float dx);
  
  ClassDef(EdbPhysics,1)  // some physics
};

#endif /* ROOT_EdbPhys */
