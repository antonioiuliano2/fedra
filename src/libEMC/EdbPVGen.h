#ifndef ROOT_EdbPVGen
#define ROOT_EdbPVGen
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVGen - Patterns Volume Generator                                 //
//                                                                      //
//  collection of agorithms                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TGenPhaseSpace.h"
#include "EdbPVRec.h"

//______________________________________________________________________________
class EdbPVGen : public TObject {
 
 private:

  EdbPatternsVolume *ePVolume;

  EdbScanCond *eScanCond;

 public:
  TObjArray   *eTracks;
  TObjArray   *eVTX;          //

 public:
  EdbPVGen();
  ~EdbPVGen();

  void SetVolume( EdbPatternsVolume *pv) {ePVolume=pv;}
  void SetScanCond( EdbScanCond *scan) {eScanCond=scan;}
  EdbPatternsVolume *GetVolume() const { return ePVolume;}

  void SmearSegments();
  void SmearPatterns(float shift, float rot);

  void GenerateBeam( int n, float x[4], float sx[4], float lim[4], float z0, int flag=0 );
  void GenerateBackground( int n, float x[4], float sx[4], int flag=0 );

  void GeneratePulsPoisson( float mean, float amp=1., float wmin=0, float wmax=0., int flag=0 );
  void GeneratePulsGaus( float amp, float mean, float sigma, float wmin=0, float wmax=0., int flag=0 );

  void AddTrack(EdbTrackP *track) {
    if(!eTracks) eTracks = new TObjArray();
    eTracks->Add(track);
  }

  void AddVertex(EdbVertex *vtx) {
    if(!eVTX) eVTX = new TObjArray();
    eVTX->Add((TObject*)vtx);
  }

  void  SmearSegment( EdbSegP &s, EdbScanCond &cond );
  float PropagateSegment( EdbSegP &s, float dz,
			 float X0,
			 float m, 
			 int eloss_flag );
  int TrackMC2( EdbTrackP   &tr,
		EdbLayer    &lim,
		int eloss_flag, float PGap);

  void TrackMC( float zlim[2], float lim[4], 
		EdbTrackP &tr, int eloss_flag = 0, float PGap = 0. );

  void GenerateUncorrelatedSegments(int nb, float lim[4],
				    float TetaMax, int flag );

  void GenerateBackgroundTracks(int nb, float vlim[4], float lim[4],
				float plim[2], float TetaMax,
				float ProbGap, int eloss_flag );

  void GeneratePhaseSpaceEvents( int nv, TGenPhaseSpace *pDecay, float vzlim[2],
				 float vlim[4],  float lim[4], float ProbGap,
				 int eloss_flag, int *charges );

  int MakeTracksMC(int nsegmin, TObjArray *tracks);

  ClassDef(EdbPVGen,1)  // PatternsVolume Generator
};

#endif /* ROOT_EdbPVGen */
