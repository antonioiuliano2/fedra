#ifndef ROOT_EdbBrickGen
#define ROOT_EdbBrickGen

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBrickGen - OPERA Brick simulation                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbBrick.h"
#include "EdbPVGen.h"


//______________________________________________________________________________
class EdbBrickGen : public TObject {

 private:

  EdbBrickP *eBrick;
  EdbPVGen  *ePVG;

 public:
  EdbBrickGen();
  ~EdbBrickGen();

  EdbBrickP *Brick() const {return eBrick;}
  EdbPVGen  *PVG()   const {return ePVG;  }

  void Generate();
  void GenerateOperaBrick( int   npl=58,
			   float dx=60000., float dy=50000.,
			   float x0=0.,    float y0=0.,    float z0=0.
			   );
  void GenerateOperaBrick( EdbBrickP &br,
			   int   npl,
			   float dx, float dy,
			   float x0,    float y0,    float z0
			   );

  void GeneratePatternsVolumeBT(EdbPatternsVolume &v);
  void GeneratePatternsVolumeBT();

  void Print();

  ClassDef(EdbBrickGen,1)  // OPERA simulation
};

#endif /* ROOT_EdbBrickGen */
