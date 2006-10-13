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

  void  SetNsegMax(int nseg) {eNsegMax=nseg;}
  void  SetDefaultBrick();
  int   FitTrackLine(EdbTrackP &tr);
  float PMS_Mag(EdbTrackP &tr, float detheta);
  float P_MS(EdbTrackP &tr);

  void Print();

  ClassDef(EdbTrackFitter,1)  // track fitter
};

#endif /* ROOT_EdbTrackFitter */
