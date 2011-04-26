#ifndef ROOT_EdbCell1
#define ROOT_EdbCell1

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCell1                                                             //
//                                                                      //
//  class to group 1-dim objects for the fast access                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TH1.h"
#include "TObjArray.h"

//__________________________________________________________________________
class EdbH1 : public TObject {

 protected:

  Int_t     eN;    // divisions
  Float_t eMin;    // min
  Float_t eMax;    // max
  Float_t eBin;    // bin size

  Int_t   eNcell;     // eN
  Int_t     *eNC;     //! [eNcell] number of objects/cell

 public:
  EdbH1();
  EdbH1(int n, float min, float max) { Set0(); InitH1(n,min,max); }
  EdbH1( const EdbH1 &h );
  ~EdbH1();

  void Set0();
  void Copy( const EdbH1 &h );

  int   InitH1( int n, float min, float max );
  void  CleanCells();
  void  PrintStat();
  void  Print();
  void  Delete();

  int DiscardHighCells(int nmax);

  int Ncell() const {return eNcell;}
  int N()     const {return eN;}

  int    IX(float x)        const { return (int)((x-eMin)/eBin); }
  int    Jcell(int ix)      const { if(ix>=0&&ix<eN) return ix; else return -1;}
  int    Jcell(float x)     const { return Jcell( IX(x) ); }
  float  X(int i)           const { return eMin+eBin*(i+0.5); }
  float  Xmin()             const { return eMin; }
  float  Xmax()             const { return eMax; }
  float  Xbin()             const { return eBin; }
  int    Bin(int ix)        const { if(Jcell(ix)>-1) return eNC[Jcell(ix)]; else return 0; }
  int    MaxBin();
  float  XminA(float level=0);
  float  XmaxA(float level=0);

  void  AddBin(int jcell, int n) { if(jcell>=0&&jcell<eNcell) eNC[jcell]+=n; }
  int   Fill(float x) { return Fill(x,1); }
  int   Fill(float x, int n);
  void  SetBin(int ix, int n) { if(Jcell(ix)>-1) eNC[Jcell(ix)] = n; }
 
  Long_t  Integral();
  Long_t  Integral(int iv, int ir);

  Float_t Mean() { return 1.*Integral()/eNcell; }

  TH1I   *DrawSpectrum(  const char *name="EdbH1spectrun" );
  TH1F   *DrawH1(        const char *name="EdbH1plot" , const char *title="EdbH1plot1D");

  ClassDef(EdbH1,1)  // fast 2-dim histogram class (used as a basis for EdbCell1)
};


/*
//__________________________________________________________________________
class EdbPeak1 : public EdbH1 {

 public:
  Int_t    eNpeaks;  // number of found peaks
  TArrayF  ePeak;    //
  TArrayF  eMean3;   //
  TArrayF  eMean;    //
  Float_t  eNorm;    // the norm-factor in case of the smoothing applied

 public:
  EdbPeak1() { InitPeaks(10); }
  EdbPeak1( const EdbH1 &h ) : EdbH1( h ) { InitPeaks(10); }
  ~EdbPeak1() {}

  void    Print();
  void    InitPeaks(int npeaks);
  int     FindPeak(int iv[2]);
  int     FindPeak(float v[2]);
  int     FindPeak(float &x, float &y);
  float   FindGlobalPeak(float &x, float &y, float ratio=0.1);
  float   ProbPeak();
  float   ProbPeak(int iv[2], int ir[2]);
  int     WipePeak(int iv[2], int ir[2]);
  int     ProbPeaks(int npeak);
  int     ProbPeaks(int npeak, int ir[2]);
  float   EstimatePeakVolume(int ipeak);
  float   EstimatePeakVolumeSafe(int ipeak);
  float   Smooth(Option_t *option="k5a");
  float   Xmean();
  float   Ymean();

  ClassDef(EdbPeak1,1)  // peak analyser for EdbH1
};
*/

//__________________________________________________________________________
class EdbCell1 : public EdbH1 {

 private:

  Int_t   eCellLim;   // max number of entries into one cell (for memory allocation)
  TObject   **epO;    //! pointers to objects [eNcell*eCellLim]
  TObject  ***epC;    //! pointers to cells   [eNcell]

 public:
  EdbCell1();
  ~EdbCell1();

  int   InitCell(int maxpercell, int n, float min, float max );
  void  Delete();
  void  Reset() {CleanCells(); Delete();}

  bool  AddObject( float x,  TObject *obj );
  bool  AddObject( int   j,  TObject *obj );

  int SelectObjects(TObjArray &arr);
  int SelectObjects(int   min, int max, TObjArray &arr);
  int SelectObjects(float min, float max, TObjArray &arr);
  int SelectObjectsC(int iv, int ir, TObjArray &arr);
  int SelectObjectsC(float v, int ir, TObjArray &arr) { return SelectObjectsC( IX(v), ir, arr); }

  TObject *GetObject(int j, int ientr) const { 
    if(j>=0&&j<eNcell&&ientr>=0&&ientr<eCellLim) return epC[j][ientr];
    else return 0;
  }
  void  PrintStat();

  ClassDef(EdbCell1,1)  // class to group 2-dim objects
};

#endif /* EdbCell1 */
