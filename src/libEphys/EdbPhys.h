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

#ifdef WIN32
#ifdef _PHYS_EXPORT
#define _DLL_EX_IMP __declspec(dllexport)
#endif
#ifdef _PHYS_IMPORT
#define _DLL_EX_IMP __declspec(dllimport)
#endif
#endif

//_________________________________________________________________________
class EdbPhysics: public TObject {
 private:


 public:
//	Pb constants

  static const double geu;
  static const double geu1;
  static const double me;
  static const double me_kg;
  static const double mpi;
  static const double ro;
  static const double Z;
  static const double A;
  static const double Nav;
  static const double I;
  static const double Nel;
  static const double h;
  static const double D;
  static const double ec;
  static const double vp;
  static const double C;
  static const double Xa;
  static const double X0;
  static const double X1;
  static const double mm;
  static const double aa;

  static const float kX0_Pb;
  static const float kX0_Em;
  static const float kX0_Cell;

  EdbPhysics(){}
  virtual ~EdbPhysics(){}

  static double ThetaMS2( float p, float mass, float dx, float X0 );
  static double ThetaPb2( float p, float mass, float dx );
  static double DeAveragePb( float p, float mass, float dx);
  static void   DeAveragePbFastSet( float p, float mass);
  static double DeAveragePbFast(  float p, float mass, float dx);
  static double DeLandauPb( float p, float mass, float dx);
  
  ClassDef(EdbPhysics,2)  // some physics
};

#endif /* ROOT_EdbPhys */
