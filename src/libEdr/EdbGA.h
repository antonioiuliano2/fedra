#ifndef ROOT_EdbGA
#define ROOT_EdbGA
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbGA                                                                //
//                                                                      //
// grains analysys stuff                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbRun.h"
class TIndexCell;

//______________________________________________________________________________
class EdbGA : public TObject {
 
 private:

  EdbRun *eRun;    //

  Float_t  eBinX;  //
  Float_t  eBinY;  //
  Float_t  eBinZ;  //

  Int_t    eVid;   // view under prcessing

  TFile *eGrainsFile;
  TTree *eGrains;
  TClonesArray *eClusters;

 public:
  EdbGA(){eRun=0;}
  EdbGA(char *fname, float bx, float by, float bz);
  virtual ~EdbGA(); 

  void   SetRun( char *fname )
    { if(eRun) delete eRun; 
      eRun=new EdbRun(fname); }
  void   SetBin( float bx, float by, float bz ) 
    { eBinX=bx; eBinY=by; eBinZ=bz; } 

  void   InitTree(const char *file="grain_chains.root");
  void   CheckViewGrains();
  void   CheckViewGrains(int vid);
  void   VerticalChains( TClonesArray *clusters, TIndexCell &chains);
  int    MakeGrainsTree( TClonesArray *clust, TIndexCell &chains);
  void   GrainStat( TClonesArray *clusters, float &x0, float &y0, float &z0 );

  ClassDef(EdbGA,1)  // grains analysys
};

#endif /* ROOT_EdbGA */
