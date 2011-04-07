#ifndef ROOT_EdbCell2
#define ROOT_EdbCell2

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCell2                                                             //
//                                                                      //
//  class to group 2-dim objects for the fast access                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"

//__________________________________________________________________________
class EdbH2 : public TObject {

 protected:

  Int_t     eN[2];    // divisions
  Float_t eMin[2];    // min
  Float_t eMax[2];    // max
  Float_t eBin[2];    // bin size

  Int_t   eNcell;     // eNx*eNy
  Int_t     *eNC;     //! [eNcell] number of objects/cell

 public:
  EdbH2();
  EdbH2(int nx, float minx, float maxx, int ny, float miny, float maxy) { Set0(); InitH2(nx, minx, maxx, ny, miny, maxy); }
  EdbH2( const EdbH2 &h );
  ~EdbH2();

  void Set0();
  void Copy( const EdbH2 &h );

  int   InitH2( const EdbH2 &h );
  int   InitH2(int n[2], float min[2], float max[2]);
  int   InitH2(int nx, float minx, float maxx, int ny, float miny, float maxy);
  void  CleanCells();
  void  PrintStat();
  void  Delete();

  int DiscardHighCells(int nmax);

  int Ncell() const {return eN[0]*eN[1];}
  int NX() const {return eN[0];}
  int NY() const {return eN[1];}
  int IX(float x)             const {return (int)((x-eMin[0])/eBin[0]); }
  int IY(float y)             const {return (int)((y-eMin[1])/eBin[1]); }
  int Jcell(int ix, int iy)   const {if(ix>=0&&ix<eN[0]&&iy>=0&&iy<eN[1]) return iy*eN[0]+ix; else return -1;}
  int Jcell(float x, float y) const {return Jcell( IX(x), IY(y)); }
  int Jcell(float v[2])       const {return Jcell(IX(v[0]),IY(v[1])); }
  float X(int i)              const {return eMin[0]+eBin[0]*(i+0.5);}
  float Y(int i)              const {return eMin[1]+eBin[1]*(i+0.5);}
  float Xmin()                const {return eMin[0];}
  float Xmax()                const {return eMax[0];}
  float Ymin()                const {return eMin[1];}
  float Ymax()                const {return eMax[1];}

  float Xbin()                const {return eBin[0];}
  float Ybin()                const {return eBin[1];}

  int   Bin(int ix, int iy) const {return (Jcell(ix,iy)>-1)? eNC[Jcell(ix,iy)] : 0; }
  int   Bin(int iv[2])      const {return Bin(iv[0],iv[1]);}

  int   MaxBin();

  void  AddBin(int jcell, int n) { if(jcell>=0&&jcell<eNcell) eNC[jcell]+=n; }
  int   Fill(float x, float y) { return Fill(x,y,1); }
  int   Fill(float x, float y, int n);
  void  SetBin(int ix, int iy, int n) { if(Jcell(ix,iy)>-1) eNC[Jcell(ix,iy)] = n; }
 
  Long_t  Integral();
  Long_t  Integral(int iv[2], int ir[2]);

  Float_t Mean() { return 1.*Integral()/eNcell; }

  TH1I   *DrawSpectrum( const char *name="plot1d", const char *title="EdbH2 DrawSpectrun");
  TH2F   *DrawH2(       const char *name="plot2d", const char *title="EdbH2plot2D");

  ClassDef(EdbH2,1)  // fast 2-dim histogram class (used as a basis for EdbCell2)
};

//__________________________________________________________________________
class EdbPeak2 : public EdbH2 {

 public:
  Int_t    eNpeaks;  // number of found peaks
  TArrayF  ePeak;    //
  TArrayF  eMean3;   //
  TArrayF  eMean;    //
  Float_t  eNorm;    // the norm-factor in case of the smoothing applied

 public:
  EdbPeak2() { InitPeaks(10); }
  EdbPeak2( const EdbH2 &h ) : EdbH2( h ) { InitPeaks(10); }
  ~EdbPeak2() {}

  void    Delete();
  void    Init(const EdbH2 &h, int npeaks=10);
  void    Print();
  void    InitPeaks( int npeaks);
  int     FindPeak(  int iv[2]);
  int     FindPeak(  float v[2]);
  int     FindPeak(  float &x, float &y);
  float   FindPeak9( float &x, float &y);
  float   FindGlobalPeak(float &x, float &y, float ratio=0.1);
  float   ProbPeak();
  float   ProbPeak(float &x, float &y);
  float   ProbPeak(int iv[2], int ir[2]);
  int     WipePeak(int iv[2], int ir[2]);
  int     ProbPeaks(int npeak);
  int     ProbPeaks(int npeak, int ir[2]);
  float   EstimatePeakVolume(int ipeak);
  float   EstimatePeakVolumeSafe(int ipeak);
  float   EstimatePeakMeanPosition(int iv[2], int ir[2], float &x, float &y);
  float   Smooth(Option_t *option="k5a");
  float   Xmean();
  float   Ymean();
  float   Peak(int i=0)  const { if(i>=0&&i<eNpeaks) return ePeak[i];  else return -1; }
  float   Mean3(int i=0) const { if(i>=0&&i<eNpeaks) return eMean3[i]; else return -1; }
  float   Mean(int i=0)  const { if(i>=0&&i<eNpeaks) return eMean[i];  else return -1; }

  ClassDef(EdbPeak2,1)  // peak analyser for EdbH2
};

//__________________________________________________________________________
class EdbCell2 : public EdbH2 {

 private:

  Int_t   eCellLim;   // max number of entries into one cell (for memory allocation)
  TObject   **epO;    //! pointers to objects [eNcell*eCellLim]
  TObject  ***epC;    //! pointers to cells   [eNcell]

 public:
  EdbCell2();
  ~EdbCell2();

  int   InitCell(int maxpercell, int n[2], float min[2], float max[2] );
  void  Delete();
  void  Reset() {CleanCells(); Delete();}

  bool  AddObject( float v[2], TObject *obj ) { return AddObject(v[0],v[1],obj); }
  bool  AddObject( float x, float y, TObject *obj );
  bool  AddObject( int ix, int iy, TObject *obj );
  bool  AddObject( int j, TObject *obj );

  int SelectObjects(TObjArray &arr);
  int SelectObjects(int   min[2], int max[2], TObjArray &arr);
  int SelectObjects(float min[2], float max[2], TObjArray &arr);
  int SelectObjectsC(int iv[2], int ir[2], TObjArray &arr);
  int SelectObjectsC(float v[2], int ir[2], TObjArray &arr) {
    int iv[2] = { IX(v[0]), IY(v[1]) }; 
    return SelectObjectsC(iv, ir, arr);
  }
  TObject *GetObject(int ix, int iy, int ientr) const { return GetObject( Jcell(ix,iy), ientr); }
  TObject *GetObject(int j, int ientr) const { 
    if(j>=0&&j<eNcell&&ientr>=0&&ientr<eCellLim) return epC[j][ientr];
    else return 0;
  }
  void  PrintStat();

  ClassDef(EdbCell2,1)  // class to group 2-dim objects
};


#endif /* EdbCell2 */
