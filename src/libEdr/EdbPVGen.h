#ifndef ROOT_EdbPVGen
#define ROOT_EdbPVGen
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVGen - Patterns Volume Generator                                 //
//                                                                      //
//  collection of agorithms                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbPVRec.h"

//______________________________________________________________________________
class EdbPVGen : public TObject {
 
 private:

  EdbPatternsVolume *ePVolume;

  EdbScanCond *eScanCond;

 public:
  EdbPVGen(){}

  void SetVolume( EdbPatternsVolume *pv) {ePVolume=pv;}
  void SetScanCond( EdbScanCond *scan) {eScanCond=scan;}
  EdbPatternsVolume *GetVolume() const { return ePVolume;}

  void SmearSegments();
  void SmearPatterns(float shift, float rot);

  void GenerateBeam( int n, float x[4], float sx[4], float lim[4], float z0, int flag=0 );
  void GenerateBackground( int n, float x[4], float sx[4], int flag=0 );

  void GeneratePulsPoisson( float mean, float amp=1., float wmin=0, float wmax=0., int flag=0 );
  void GeneratePulsGaus( float amp, float mean, float sigma, float wmin=0, float wmax=0., int flag=0 );

  ClassDef(EdbPVGen,1)  // PatternsVolume Generator
};

#endif /* ROOT_EdbPVGen */
