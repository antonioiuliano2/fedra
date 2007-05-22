#ifndef ROOT_EdbTrackFitter
#define ROOT_EdbTrackFitter
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbTrackFitter                                                       //
//                                                                      //
// definitions and functions for track fit                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbPattern.h"

//______________________________________________________________________________
class EdbTrackFitter : public TNamed {
 
 private:
  int   eNsegMax;        // max number of segments (for arrays allocation)

 public:
  float eX0;             // rad length of the media [microns]
  float eM;              // mass of the particle (if negative - use the mass setted in the track)
  float eTPb;            // ?
  float ePcut;           // minimal momentum?
  bool  eDE_correction;  // take into account the energy loss or not
  
 public:
  EdbTrackFitter();
  virtual ~EdbTrackFitter(){}

  void    SetNsegMax(int nseg) {eNsegMax=nseg;}
  void    SetDefaultBrick();
  int     FitTrackLine(EdbTrackP &tr);
  float   PMS_Mag(EdbTrackP &tr, float detheta);
  float   P_MS(EdbTrackP &tr);
  static float   MaxKink(EdbTrackP &tr);
  static float   MeanKink(EdbTrackP &tr);
  static float   Theta( EdbSegP &s, EdbSegP &s1 );
  static float   Chi2Seg( EdbSegP *s1, EdbSegP *s2);
  static float   MaxChi2Seg(EdbTrackP &tr);
  static float   MeanChi2Seg(EdbTrackP &tr);
  bool    SplitTrack( EdbTrackP &t, EdbTrackP &t1, int isplit );
  int     SplitTrackByKink( EdbTrackP *t, TObjArray &tracks, float maxkink );
  float   PMS_KF(EdbTrackP &t, float p0=10., float probbest=0.5);

  void Print();

  ClassDef(EdbTrackFitter,1)  // track fitter
};

#endif /* ROOT_EdbTrackFitter */