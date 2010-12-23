#ifndef ROOT_EdbSEQ
#define ROOT_EdbSEQ
#include "TObject.h"
#include "TF1.h"
#include "EdbSigma.h"
#include "EdbCell1.h"

//---------------------------------------------------------------------------
class EdbSEQ : public EdbSigma
{

 public:
  double eS0mt;   //   = 270.*340.;     // area unit for Nseg calculation
  double eDZcell; // = 1300.;
  double eNsigma; //=3

  int eNP;  // number of points for the functions calculation

 public:
  EdbSEQ() { ((EdbSigma*)this)->Set0(); Set0();}
  virtual ~EdbSEQ();

  void   Set0();

  double DNmt(double t);
  double FDNmt(double *x, double *par) {return DNmt(*x);}

  double DNbt(double t);
  double FDNbt(double *x, double *par) {return DNbt(*x);}

  void   CalculateDensityMT( EdbH1 &hEq, float area );

  void   EqualizeMT(TObjArray &mti, TObjArray &mto, float area);

  void Draw();

  ClassDef(EdbSEQ,1)  // segments equalizer
};

#endif /* ROOT_EdbSEQ */
