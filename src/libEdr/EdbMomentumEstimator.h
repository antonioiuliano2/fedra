#ifndef ROOT_EdbMomentumEstimator
#define ROOT_EdbMomentumEstimator
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbMomentumEstimator                                                 //
//                                                                      //
// definitions and functions for track momentum estimation              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbTrackFitter.h"

class TGraphErrors;
class TF1;

//______________________________________________________________________________
class EdbMomentumEstimator : public EdbTrackFitter {
 
 public:
  // input parameters for PMS_mag
  int   eFlagt;              // if =0, returns P, if =1 returns Px, if =2 returns Py (set in the function)
  float eDT0,  eDT1,  eDT2;  // detheta  = eDT0 *(1+ eDT1*theta0- eDT2*theta0*theta0);
  float eDTx0, eDTx1, eDTx2; // dethetaX = eDTx0*(1+eDTx1*theta0-eDTx2*theta0*theta0);
  float eDTy0, eDTy1, eDTy2; // dethetaY = eDTy0*(1+eDTy1*theta0-eDTy2*theta0*theta0);

  // the fit results
  float ePx,ePy;             // the estimated momentum
  float eDPx,eDPy;           // the fit error

  float ePXmin, ePXmax;      // momentum 90% errors range
  float ePYmin, ePYmax;      // momentum 90% errors range

  // the output of PMSang
  float eP;
  float ePmin, ePmax;         // momentum 90% errors range

  TF1          *eF1X;         //! fit function
  TF1          *eF1Y;         //!
  TGraphErrors *eGX;          //! longitudianl component of the momentum
  TGraphErrors *eGY;          //! transverse   component of the momentum

 public:
  EdbMomentumEstimator();
  virtual ~EdbMomentumEstimator();

  void    SetParPMS_Mag();
  float   PMSang(EdbTrackP tr);
  float   CellWeight(int npl, int m);
  void    EstimateMomentumError(float P, int npl, float ang, float &pmin, float &pmax);
  double  Mat(float P, int npl, float ang);
  TF1    *MCSErrorFunction(const char *name, float x0, float dtx);

  float   PMS_Mag(EdbTrackP &tr);
  float   PMS_Mag(EdbTrackP &tr,TGraphErrors *h_p,TF1 *itf,TF1 *itf_min,TF1 *itf_max);
  float*  GetDP(float P, int npl, float ang);

  float   P_MS(EdbTrackP &tr);
  void    DrawPlots();

  void    Print();

  ClassDef(EdbMomentumEstimator,1)  // track momentum estimator
};

#endif /* ROOT_EdbMomentumEstimator */
