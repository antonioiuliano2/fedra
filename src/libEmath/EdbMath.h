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
#include "TArrayD.h"

//______________________________________________________________________________
class EdbMath {

 public:
  EdbMath(){}  
  virtual ~EdbMath(){}

  static double Magnitude3( float Point1[3], float Point2[3] );
  static double DistancePointLine3( float Point[3], 
				    float LineStart[3], float LineEnd[3], 
				    bool inside );

  static bool LineLineIntersect( float p1[3], float p2[3], float p3[3], float p4[3],
				 float pa[3], float pb[3],
				 double &mua, double &mub );

  static bool LinFitDiag( int n, float *x, float *y, float *e, 
			  float p[2], float d[2][2], float *chi2);
  static bool LinFitCOV( int n, float *x, float *y, float *c, 
			 float p[2], float d[2][2], float *chi2);
  static void LFITW( float *X, float *Y, float *W, int L, int KEY, float &A, float &B, float &E );

  ClassDef(EdbMath,1)  // general matematical algorithms
};

//______________________________________________________________________________
class TIndex2 : public TArrayD {
 private:

 public:
  TIndex2() : TArrayD() {}
  TIndex2(int size) : TArrayD(size) {}
  virtual ~TIndex2(){}
  
  static Double_t BuildValue( Int_t major, Int_t minor ) {return major+minor*1e-9;}
  void   BuildIndex( int n, double *w );
  Int_t  Find(Int_t major, Int_t minor); 
  Int_t  FindIndex(Int_t major);

  ClassDef(TIndex2,1)  // simple index class for fast search in linear cases
};

#endif /* ROOT_EdbMath */
