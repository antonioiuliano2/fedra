#ifndef ROOT_EdbSEQ
#define ROOT_EdbSEQ
#include "TObject.h"
#include "TVector2.h"
#include "TF1.h"
#include "EdbSigma.h"
#include "EdbCell1.h"
#include "EdbPattern.h"

//---------------------------------------------------------------------------
class EdbSEQ : public EdbSigma
{

 public:
  Double_t eS0mt;   //   = 270.*340.;     // area unit for Nseg calculation
  Double_t eNsigma; //=4
  Double_t eArea;  // effective area of the pattern to be equalized
  
  int eNP;        // number of points for the functions calculation
  
  TObjArray   eExcludeThetaRange;   // can be added EdbSegP with tx,ty, sigmaTX,sigmaTY to be excluded
  EdbH1       eHEq;
  
  private:
  TVector2    *eXLimits;            // [min,max] area limits for the preselection procedure
  TVector2    *eYLimits;            // [min,max]
  TVector2    *eThetaLimits;        // [min,max]
  TVector2    *eWLimits;            // [min,max]
  TVector2    *eChiLimits;          // [min,max]

 public:
  EdbSEQ() { ((EdbSigma*)this)->Set0(); Set0();}
  virtual ~EdbSEQ();

  void   Set0();
  
  void   SetXLimits(float xmin, float xmax) {SafeDelete(eXLimits); eXLimits=new TVector2(xmin,xmax);}
  void   SetYLimits(float ymin, float ymax) {SafeDelete(eYLimits); eYLimits=new TVector2(ymin,ymax);}
  void   SetWLimits(float wmin, float wmax) {SafeDelete(eWLimits); eWLimits=new TVector2(wmin,wmax);}
  void   SetThetaLimits(float tmin, float tmax) {SafeDelete(eThetaLimits); eThetaLimits=new TVector2(tmin,tmax);}
  void   SetChiLimits(float cmin, float cmax) {SafeDelete(eChiLimits); eChiLimits=new TVector2(cmin,cmax);}

  double DNmt(double t);
  double FDNmt(double *x, double *par) {return DNmt(*x);}

  double DNbt(double t);
  double FDNbt(double *x, double *par) {return DNbt(*x);}

  void   CalculateDensityMT( EdbH1 &hEq );
  
  void   PreSelection( EdbPattern &pi, TObjArray &po );

  void   EqualizeMT(TObjArray &mti, TObjArray &mto, Double_t area);
  
  void   ResetExcludeThetaRange() {eExcludeThetaRange.Delete();}
  void   AddExcludeThetaRange(EdbSegP &s);
  void   ExcludeThetaRange(TObjArray &mti, TObjArray &mto);
  bool   IsInsideThetaRange(EdbSegP *s);
  
  float  Wmt(EdbSegP &s) { return s.W()*50. + s.DZ(); }
  
  TH1F  *ThetaPlot(TObjArray &arr, const char *name="theta", const char *title="EdbSEQ theta distribution normalised to area");
  TH1F  *ThetaPlot(EdbPattern &p, const char *name="theta", const char *title="EdbSEQ theta distribution normalised to area");
  
  void Draw();
  void PrintLimits();

  ClassDef(EdbSEQ,1)  // segments equalizer
};

#endif /* ROOT_EdbSEQ */
