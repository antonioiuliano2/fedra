#ifndef ROOT_EdbScanSet
#define ROOT_EdbScanSet

#include "TIndexCell.h"
#include "EdbBrick.h"

class EdbScanSet
{
public:
  EdbBrickP eB;        // all layers of the brick defined here
  TIndexCell ePID;     // correspondance between index in eB and the plate id

  TObjArray ePC;      // Plate Couples. Each couple represented as EdbPlateP 
                      // where 2 sides in reality corresponds to 2 plates 

public: 
  EdbScanSet();
  virtual ~EdbScanSet(){}

  int AssembleBrickFromPC();
  bool GetAffP2P(int p1, int p2, EdbAffine2D &aff);

  void   Print();

  ClassDef(EdbScanSet,1)  // remote scanning from Bern
};

#endif /* ROOT_EdbScanSet */
