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
#include "TCut.h"

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

  void   GetClustPFile( const char *file );
  void   InitTree(const char *file="grain_chains.root");
  void   CheckViewGrains(const char* options = "");
  void   CheckViewGrains(int vid,const char* options = "");
  void   VerticalChains( TClonesArray *clusters, TIndexCell &chains);
  void   VerticalChainsA( TClonesArray *clusters );
  int    MakeGrainsTree(TClonesArray *clust, TIndexCell &chains,const char* options = "");
  void   GrainStat( TClonesArray *clusters, float &x0, float &y0, float &z0 );
  int    GrainStat2( TClonesArray *clusters, float &x0, float &y0, float &z0, 
          float &vol, float &amin, float &amax, float &zmin, float &zmax, int &fmin, int &fmax);
  void   SelectGrains(TCut c1, const char* outfile="grains_chains_selection.root");
  void   SelectGrains(const char* selection, const char* outfile="grains_chains_selection.root");

  TTree* GetTree(void) {return eGrains;}

  ClassDef(EdbGA,1)  // grains analysys

};

#endif /* ROOT_EdbGA */
