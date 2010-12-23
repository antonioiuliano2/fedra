#ifndef ROOT_EdbSigma
#define ROOT_EdbSigma
#include "TObject.h"
using namespace TMath;

//---------------------------------------------------------------------------
class EdbSigma : public TObject
{
 public:

  // grains measurement accuracy defines the "in view" bt and mt errors
  double eSxy;     // =  0.35 mean in-view grain accuracy xy
  double eSz;      // =  2.   grains z-uncertainty "field depth"

  // geometry defines the "in-view" mt/bt accuracy:
  double eDZem;    // = 45.;  emulsion thickness
  double eDZbase;  // = 210. emulsion base
  double eDZcell;  // =1300. plate-to plate distance

  // additional terms defines "in-zone" resolution
  double eSxyZone;   // "in-zone" microtrack position tolerance (local deformations)
  double eSaZone;    // "in-zone" microtrack angular tolrance (incorrect shrinkage+offsets)

  // additional terms defines "in-plate" resolution
  double eSxyPlate;   // "in-plate" basetrack position tolerance (XY plate deformations)
  double eSaPlate;    // "in-plate" basetrack angular tolrance (nonplanarity)

  // RS agreement: 0 - in-view (default); 1 - in-zone; 2-in-plate; 3-in-brick

 public:
  EdbSigma() { Set0(); }
  virtual ~EdbSigma() {}

  void  Set0();

  double SqSum(double a, double b) {return Sqrt(a*a + b*b);}

  double DAL(double t, double sxy, double sz, double dz);
  double DAT(double t, double sxy, double dz );
  double DP (double t, double sxy, double da, double dz);

  double DALmt(double t );
  double DATmt(double t );
  double DPLmt(double t );
  double DPTmt(double t );

  double DALbt(double t );
  double DATbt(double t );
  double DPLbt(double t );
  double DPTbt(double t );

  double FDAL(double *x, double *par) { return DAL(*x, par[0], par[1], par[2]);}
  double FDAT(double *x, double *par) { return DAT(*x, par[0], par[1]);}
  double FDPLmt(double *x, double *par) {return DPLmt(*x);}
  double FDALbt(double *x, double *par) {return DALbt(*x);}

  void Draw();

  ClassDef(EdbSigma,1)  // segments accuracy functions
};

#endif /* ROOT_EdbSigma */
