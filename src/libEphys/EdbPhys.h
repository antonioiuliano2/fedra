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

//_________________________________________________________________________
class EdbPhysics: public TObject {
 private:

 public:
  static const float kX0_Pb;
  static const float kX0_Em;
  static const float kX0_Cell;

  EdbPhysics(){}
  virtual ~EdbPhysics(){}

  static double ThetaMS2( float p, float mass, float dx, float X0 );
  static double ThetaPb2( float p, float mass, float dx );
  
  ClassDef(EdbPhysics,1)  // some physics
};

#endif /* ROOT_EdbPhys */
