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
#include "TCanvas.h"
#include "TF1.h"

class TGraphErrors;
class TString;
class TGraphAsymmErrors;

//______________________________________________________________________________
class EdbMomentumEstimator : public EdbTrackFitter {
 
 public:

  int   eAlg;                // select the algorithm for PMS estimation
  int   eStatus;             // status of the estimation (-1 nothing was done)

  int   eMinEntr;            // min number of entries in the cell to accept it for fitting (def=1)

  // input parameters for PMS_mag
  TF1 eDTxErrorFun; 
  TF1 eDTyErrorFun;
  TF1 eDTsErrorFun; 

  // the fit results
  float ePx,ePy;             // the estimated momentum
  float eDPx,eDPy;           // the fit error

  float ePXmin, ePXmax;      // momentum 90% errors range
  float ePYmin, ePYmax;      // momentum 90% errors range

  // the output of PMSang
  float eP, eDP;
  float ePmin, ePmax;         // momentum 90% errors range

  EdbTrackP    eTrack;        // the copy of the track to be used for plots

  bool          eVerbose;
  TF1          *eF1X;         //! fit function
  TF1          *eF1Y;         //!
  TF1          *eF1;          //!
  TGraphErrors *eG;           //! 3D component of the momentum
  TGraphErrors *eGX;          //! longitudianl component of the momentum
  TGraphErrors *eGY;          //! transverse   component of the momentum

  TGraphAsymmErrors *eGA;           //! 3D component of the momentum
  TGraphAsymmErrors *eGAX;          //! longitudianl component of the momentum
  TGraphAsymmErrors *eGAY;          //! transverse   component of the momentum

 public:
  EdbMomentumEstimator();
  virtual ~EdbMomentumEstimator();

  void    SetParPMS_Mag();
  void    SetParPMS_Mag(Int_t type, Int_t parNumber, Double_t parvalue);
  void    Set0();
  float   PMS(EdbTrackP &tr);
  float   PMSang(EdbTrackP &tr);
  float   PMSang_corr(EdbTrackP &tr);
  float   PMScoordinate(EdbTrackP &tr);
  float   CellWeight(int npl, int m);
  void    EstimateMomentumError(float P, int npl, float ang, float &pmin, float &pmax);
  double  Mat(float P, int npl, float ang);
  TF1    *MCSErrorFunction(const char *name, float x0, float dtx);
  TF1    *MCSCoordErrorFunction(const char *name, float tmean, float x0);

  void    SetDTxErrorFunction(TF1& f){eDTxErrorFun=f;}
  void    SetDTyErrorFunction(TF1& f){eDTyErrorFun=f;};
  void    SetDTsErrorFunction(TF1& f){eDTsErrorFun=f;};
  
  double  GetDTx(double Tx){return eDTxErrorFun.Eval(Tx);}
  double  GetDTy(double Ty){return eDTyErrorFun.Eval(Ty);}
  double  GetDTs(double Ts){return eDTsErrorFun.Eval(Ts);}

  int     PMSang_base(EdbTrackP &tr);
  int     PMSang_base_A(EdbTrackP &tr);
  TF1    *MCSErrorFunction_base(const char *name, float x0, float dtx);

  float   P_MS(EdbTrackP &tr);
//  void    DrawPlots();
  void    DrawPlots(TCanvas *c1=NULL);
  
  TString AlgStr(int alg);
  void    Print();

  ClassDef(EdbMomentumEstimator,1)  // track momentum estimator
};

#endif /* ROOT_EdbMomentumEstimator */
