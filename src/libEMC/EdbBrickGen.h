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
class EdbBeamGen : public TObject {

 public:

  Float_t   eMass;    // particle mass to be generated
  EdbLayer *eLimits;  // tracks will be generated inside this limits

  Float_t   eX0, eSigmaX;    // position Gaus
  Float_t   eY0, eSigmaY;
  Float_t   eZ0, eSigmaZ;

  Float_t   eTX0, eSigmaTX;  // angular Gaus
  Float_t   eTY0, eSigmaTY;

  Float_t   eP0, eSigmaP;    // momentum smearing

 public:
  EdbBeamGen();
  ~EdbBeamGen(){}

  void SetPositionGaus(float x0, float sx, float y0, float sy)
    { eX0=x0; eSigmaX=sx; eY0=y0; eSigmaY=sy; }
  void SetAngularGaus(float tx0, float stx, float ty0, float sty)
    { eTX0=tx0; eSigmaTX=stx; eTY0=ty0; eSigmaTY=sty; }

  void SetM(float mass)    { eMass=mass;}
  void SetP(float p, float sp=0)    { eP0=p; eSigmaP=sp; }

  EdbTrackP *NextTrack(int id=0);

  ClassDef(EdbBeamGen,1)  // Beam Generator
};


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
