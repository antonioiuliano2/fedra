#ifndef ROOT_EdbScanSet
#define ROOT_EdbScanSet

#include "EdbBrick.h"

class EdbScanSet
{
public:
  EdbBrick eB;        // all layers of the brick defined here

public: 
  EdbScanSet();
  virtual ~EdbScanSet(){}

  

  void   Print();

  ClassDef(EdbScanSet,1)  // remote scanning from Bern
};

#endif /* ROOT_EdbScanSet */
