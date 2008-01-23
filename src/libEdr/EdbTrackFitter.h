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

class EdbScanCond;

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

  // input parameters for PMS_Mag
  int eflagt;                // if eflagt=0, returns P, if =1 returns Px, if =2 returns Py (set in the function)
  float  eDT0,  eDT1,  eDT2; // detheta  = eDT0 *(1+ eDT1*theta0- eDT2*theta0*theta0);
  float eDTx0, eDTx1, eDTx2; // dethetaX = eDTx0*(1+eDTx1*theta0-eDTx2*theta0*theta0);
  float eDTy0, eDTy1, eDTy2; // dethetaY = eDTy0*(1+eDTy1*theta0-eDTy2*theta0*theta0);

  //the output of PMS_Mag
  float eP;
  float ePmin, ePmax;    // momentum and 90% (?)  errors range
  
 public:
  EdbTrackFitter();
  virtual ~EdbTrackFitter(){}

  void    SetNsegMax(int nseg) {eNsegMax=nseg;}
  void    SetDefaultBrick();
  int     FitTrackLine(EdbTrackP &tr);
  int     FitTrackLine(const EdbTrackP &tr, float &x,float &y,float &z,float &tx,float &ty,float &w);
  void    SetParPMS_Mag();
  float   PMS_Mag(EdbTrackP &tr);
  float*  GetDP(float P, int npl, float ang);
  double   Mat(float P, int npl, float ang);

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

  //  static EdbSegP   Chi2SegM( EdbSegP s1, EdbSegP s2, EdbScanCond &cond1, EdbScanCond &cond2);
  static float Chi2SegM( EdbSegP s1, EdbSegP s2, EdbSegP &s, EdbScanCond &cond1, EdbScanCond &cond2);

  void Print();

  ClassDef(EdbTrackFitter,1)  // track fitter
};

#endif /* ROOT_EdbTrackFitter */
