#ifndef ROOT_EdbMath
#define ROOT_EdbMath

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbMath                                                              //
//                                                                      //
// Collection of matematical algorithns                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"


//______________________________________________________________________________
class EdbMath {

 public:
  EdbMath(){}  
  virtual ~EdbMath(){}

  static float Magnitude3( float Point1[3], float Point2[3] );
  static float DistancePointLine3( float Point[3], float LineStart[3], float LineEnd[3], bool inside );

  ClassDef(EdbMath,1)  // general matematical algorithms
};

#endif /* ROOT_EdbMath */
