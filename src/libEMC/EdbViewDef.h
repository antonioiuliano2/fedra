#ifndef ROOT_EdbViewDef
#define ROOT_EdbViewDef
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewDef - definition of microscope view parameters                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"

//______________________________________________________________________________
class EdbViewDef : public TObject {
private:

public:
  Int_t      eNframes;       // number of frames
  Float_t    eZmin,eZmax;    // limits in z
  Float_t    eZxy;           // intersect plane

  Float_t    eXmin,eXmax;    // limits of the view
  Float_t    eYmin,eYmax;    // limits of the view
  Float_t    eX0,eY0;        // geometrical center of the view

  Float_t    eX0opt,eY0opt;  // optical center of the view (the point vhere the grains are vertical)
  Float_t    eTXopt,eTYopt;  // koeff of the slopes: t(x) = tx*(x-X0opt), 
                             // x1=x+t(x)*(z1-z): valid for the clusters inside the grain

  Float_t    eClaSX,eClaSY,eClaSZ;          // smearing for clusters position inside grain
  Float_t    eClaSZvar;                     // variation (sigma) of the vertical grain size
  Float_t    eGrainSX,eGrainSY,eGrainSZ;    // smearing og CoG of the grain in z (relevant for tracking!)

  Float_t    eFogDens;       // per 10x10x10 microns**3
  Float_t    eFogGrainArea;  // mean area of the fog grain
  Float_t    eGrainArea;     // mean area of the signal grain

  Float_t    eZdead;         // the center of the dead layer
  Float_t    eDZdead;        // the thickness of the dead layer

public:
  EdbViewDef();
  ~EdbViewDef(){}

  void  SetDef();

  void Print();
  ClassDef(EdbViewDef,1)  // Definition of microscope view parameters
};

#endif /* ROOT_EdbViewDef */
