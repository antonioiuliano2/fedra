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

  static double Magnitude3( float Point1[3], float Point2[3] );
  static double DistancePointLine3( float Point[3], float LineStart[3], float LineEnd[3], bool inside );
  static bool LinFitDiag( int n, float *x, float *y, float *e, 
			  float p[2], float d[2][2], float *chi2);

  ClassDef(EdbMath,1)  // general matematical algorithms
};

#endif /* ROOT_EdbMath */
