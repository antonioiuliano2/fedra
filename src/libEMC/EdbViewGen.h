#ifndef ROOT_EdbViewGen
#define ROOT_EdbViewGen
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewGen - Generation of one microscope view                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbView.h"
#include "EdbViewDef.h"

//______________________________________________________________________________
class EdbViewGen : public EdbViewDef {
private:

public:
  EdbViewGen();
  EdbViewGen(EdbViewDef &vd): EdbViewDef(vd) {}
  ~EdbViewGen(){}

  //  void  SetDef();
  int   GenGrains(EdbView &v);         // generate grains in the view
  int   GenSegGrains(EdbSegment &s);   // generate grains for one segment
  int   GenFogGrains(EdbSegment &sfog);
  int   GenAlfaGrains(){return 0;}   // bg from alfa-particles
  int   GenDeltaGrains(){return 0;}  // bg from delta-electrons
  int   GenGrainClusters(EdbView &v, EdbCluster &g);
  float GrainPathMip(float lambda=2.);

  ClassDef(EdbViewGen,1)  // Generation of one microscope view
};

#endif /* ROOT_EdbViewGen */
