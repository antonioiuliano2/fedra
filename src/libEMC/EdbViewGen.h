#ifndef ROOT_EdbViewGen
#define ROOT_EdbViewGen
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewGen - Generation of one microscope view                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbView.h"

//______________________________________________________________________________
class EdbViewGen : public TObject {
private:

public:
  Int_t      eNframes;       // number of frames
  Float_t    eZmin,eZmax;    // limits in z
  Float_t    eZxy;           // intersect plane

  Float_t    eXmin,eXmax;    // limits of the view
  Float_t    eYmin,eYmax;    // limits of the view
  Float_t    eX0,eY0;        // center of the view

  Float_t    eSx,eSy,eSz;    // grain size

  Float_t    eFogDens;       // per 10x10x10 microns**3
  Float_t    eFogGrainArea;  // mean area of the fog grain
  Float_t    eGrainArea;     // mean area of the signal grain

  Float_t    eZdead;         // the center of the dead layer
  Float_t    eDZdead;        // the thickness of the dead layer

public:
  EdbViewGen();
  ~EdbViewGen(){}

  void  SetDef();
  int   GenGrains(EdbView &v);         // generate grains in the view
  int   GenSegGrains(EdbSegment &s);   // generate grains for one segment
  int   GenFogGrains(EdbSegment &sfog);
  int   GenAlfaGrains(){return 0;}   // bg from alfa-particles
  int   GenDeltaGrains(){return 0;}  // bg from delta-electrons
  int   GenGrainClusters(EdbView &v, EdbCluster &g);
  float GrainPathMip();

  ClassDef(EdbViewGen,1)  // Generation of one microscope view
};

#endif /* ROOT_EdbViewGen */
