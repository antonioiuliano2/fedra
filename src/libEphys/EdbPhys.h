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

 public:
  EdbPhysics(){}
  virtual ~EdbPhysics(){}

//	constants
  static  float kX0_Pb()  { return  5600.; }
  static  float kX0_Em()  { return 286000.;}
  static  float kX0_Cell(){ return 5810.;}

//	functions
  static double ThetaMCS( float p, float mass, float dx, float x0 );
  static double ThetaMS2( float p, float mass, float dx, float X0 );
  static double ThetaPb2( float p, float mass, float dx );
  static double DeAveragePb( float p, float mass, float dx);
  static void   DeAveragePbFastSet( float p, float mass);
  static double DeAveragePbFast(  float p, float mass, float dx);
  static double DeLandauPb( float p, float mass, float dx);
  
  ClassDef(EdbPhysics,2)  // some physics
};

#endif /* ROOT_EdbPhys */
