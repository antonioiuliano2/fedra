#ifndef ROOT_EdbViewRec
#define ROOT_EdbViewRec
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewRec - Generation of one microscope view                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TArrayF.h"
#include "TArrayI.h"
#include "TArrayC.h"

#include "EdbView.h"
#include "EdbViewDef.h"

class EdbView;

//______________________________________________________________________________
class EdbViewCell : public TObject {

public:
  Float_t   eXmin,eXmax;
  Float_t   eYmin,eYmax;
  Float_t   eSx;       // bin along x
  Float_t   eSy;       // bin along y
  Int_t     eNfr;      // number of frames (to be setted!)

  Int_t     eNx;       // "raw"    - divisions along x
  Int_t     eNy;       // "column" - divisions along y
  Int_t     eNcellXY;  // eNx*eNy
  Int_t     eNcell;    // eNfr*eNx*eNy
  Int_t     eNcl;      // total number of clusters

 private:
  TObjArray **eCells;   //! array of clusters arrays
  Int_t      eNeib[9];  // 3x3 neighborhood

 public:
  EdbViewCell();
  virtual ~EdbViewCell();

  void SetLimits( float xmin, float xmax, float ymin, float ymax)
    { eXmin=xmin; eXmax=xmax; eYmin=ymin; eYmax=ymax; }
  void SetS( float sx, float sy)
    {eSx=sx; eSy=sy;}
  void Init();
  void SetNfr(int nfr) {eNfr=nfr;}
  
  int  FillCell( TClonesArray &v );
  void CleanCell();

  int IXcell(float x) const {return (int)((x-eXmin)/eSx); }
  int IYcell(float y) const {return (int)((y-eYmin)/eSy); }
  int JcellXY(float x, float y) const {return IYcell(y)*eNx+IXcell(x); }
  int Jcell(int ifr, float x, float y) const {return ifr*eNcellXY + IYcell(y)*eNx+IXcell(x); }
  int Jneib(int i) const {return eNeib[i];}

  TObjArray *GetCell(int j) const {return eCells[j];}
  TObjArray *GetCell(int ifr, float x, float y) const {return eCells[Jcell(ifr,x,y)];}
  TObjArray *GetCell(int ifr, int ix, int iy) const {return eCells[ifr*eNcellXY + iy*eNx+ix];}
  TObjArray *GetCell(int ifr, int j) const {return eCells[ifr*eNcellXY + j];}

  void Print();
  ClassDef(EdbViewCell,1)  // service class for view reconstruction
};

//______________________________________________________________________________
class EdbViewRec : public EdbViewDef {
public:

  bool        eAddGrainsToView;    // if true: save grains to eView as a segments
  bool        eDoGrainsProcessing; // if true: when reconstruct segments first find grains ->(eGCla)
  bool        eCheckSeedThres;     // if true: use adaptive seeds threshold (based on eNseedMax)
  bool        ePropagateToBase;    // if true: segments are propagated to base position

private:
  EdbView    *eView;               //! pointer to the input  view currently in processing

  Int_t         eNgrMax;           // grains limit (for mem alloc)
  Int_t         eNsegMax;          // segments limit (for mem alloc)

  EdbViewCell *eVC;                //! pointer to eVCC or eVCG
  EdbViewCell eVCC;                //! cells with raw clusters
  EdbViewCell eVCG;                //! cells with grains

  TClonesArray  *eGSeg;            //! [eNgrMax] array of grains represented as segments
  TClonesArray  *eG;               //! pointer to eView->GetSegments() or to eGSeg as the output for grain search

  TClonesArray  *eGCla;            //! [eNgrMax] array of grains represented as clusters
  TClonesArray  *eCL;              //! pointer to eView->GetClusters() or to eGCla as the input for tracking

  TClonesArray  *eSA;              //! pointer to array of segments (output of tracking)

  Int_t     eClMinA;      //  rejected cl->eArea < eClMinA (image noise)
  Int_t     eClMaxA;      //  rejected cl->eArea > eClMaxA (blobs)

  //--------grains rec-----------
  Int_t         eNgr;              // grains counter
  TTree         *eGrainsTree;      //! debug tree
  int            eNclGrMin;        // min number of clusters/grain for saving it
  int            eNclGrMax;        // max number of clusters/grain for consider it a single grain

//-----------segments rec--------

  Float_t       eDZmin;        // minimal dz - should be in agreement with eStepFrom
  Float_t       eThetaLim;     // absolute theta limit

  Int_t         eStep;        // frame step size for seed search: default=1
  Int_t         eStepFrom;    // low   limit (ifr) for seed search
  Int_t         eStepTo;      // upper limit (ifr) for seed search

  Int_t         eSeedThres0;   // default threshold for the seed puls
  TArrayI       eSeedThres;    // thresholds[enT] for the seed puls
  TArrayF       eR;            // R(it) limits definition (may be setted manually?)
  Int_t         enSeedsLim;    // limit for the number of seeds
  Short_t       eSeedLim;      // limit for the number of clusters/seed
  Int_t         eNseedMax0;    // starting limit for the good seeds (segments) to be processed
  TArrayI       eNseedMax;     // [enT] limit for the good seeds (segments) to be processed
  Int_t         ePulsMin;      // min threshold for the segment puls
  Int_t         ePulsMax;      // max threshold for the segment puls

  Float_t       eZcenter;      // estimated center of the emulsion layer (for segments rec)

  Int_t      enT;        // number of Theta divisions
  TArrayI    enP;        // number of Phi divisions [enT]
  Int_t      enPtot;     // total number of phi divisions
  TArrayI    enY;        // number of Y divisions [enPtot]
  Int_t      enYtot;     // total number of Y divisions
  TArrayI    enX;        // number of X divisions [enYtot]
  Int_t      enXtot;     // total number of X divisions

  TArrayF    eTheta;     // theta(it) mean value
  //TArrayF    esT;        // step of Theta divisions [enT]
  TArrayF    esP;        // step of Phi   divisions [enT]
  TArrayF    esY;        // step of Y     divisions [enPtot]
  TArrayF    esX;        // step of X     divisions [enYtot]

  Short_t ****epT;       //! [enT]    - pointers to the first phi[it]
  Short_t  ***epP;       //! [enPtot] - pointers to the first y[ip]
  Short_t   **epY;       //! [enYtot] - pointers to the first x[iy]
  Short_t    *ehX;       //! [enXtot] - phase histogram

  EdbCluster ***epS;     //! pointers to seeds list
  EdbCluster **epC;      //! pointers to clusters
  Float_t      eDmax;    //  max distance inside the view (diameter)
  Float_t      eFact;    //  occupancy correction factor 
  Float_t      eRmax;    //  limit for coupling (3-dim)
//----------------------

public:
  EdbViewRec();
  EdbViewRec(EdbViewDef &vd): EdbViewDef(vd) {SetPrimary();}
  virtual ~EdbViewRec();

  void  SetPrimary();
  bool  Init();
  void  SetNclGrLim(int mincl, int maxcl) { eNclGrMin=mincl; eNclGrMax=maxcl; }
  void  SetPulsThres(int minp, int maxp=100) { ePulsMin=minp; ePulsMax=maxp;  }
  void  SetNgrMax0(Int_t ngr)             {eNgrMax=ngr;}
  void  SetAddGrainsToView(bool yesno)    {eAddGrainsToView=yesno;}
  void  InitR();
  bool  SetView(EdbView *v);
  void  SetClThres(int mina, int maxa) { eClMinA=mina; eClMaxA=maxa; }
  void  SetSeedsLim( int nseedslim=100000, int seedlim=48 ) { enSeedsLim=nseedslim; eSeedLim=seedlim; }
  int   SetStep(int sfrom, int sto, int step=1)    { eStepFrom=sfrom; eStepTo=sto; eStep=step; return (sto-sfrom)/step; }
  void  SetRmax(float rmax)            { eRmax=rmax; }
  void  SetThetaLim(float t)            { eThetaLim=t; }
  void  SetNSeedMax0(int n)            { eNseedMax0=n; }
  void  SetNSeedMax(int nt, int th[])  { if(nt>0){ enT=nt; eNseedMax.Set(nt,th);} };
  void  SetSeedThres0(Short_t mins)    { eSeedThres0=mins;}
  void  SetSeedThres(int nt, int th[]) { if(nt>0){ enT=nt; eSeedThres.Set(nt,th);} };

  void  ResetClustersSeg();
  int   FindGrains(int option=0);
  static int   FitSegment(EdbSegment &s, int wkey=0);
  static int   FitSegmentToCl(EdbSegment &s, EdbCluster &c, int wkey=0);
  float CalculateSegmentChi2( EdbSegment &seg, float sx, float sy, float sz );
  int   CheckFramesShift();
  void  InitGrainsTree(const char *file="grains.root");
  int   FillGrainsTree();

  int   FindSeeds();
  int   CheckSeedThres();
  bool  GoodSegment( EdbSegment &s, int wkey=0 );
  float Chi2Seg( EdbSegment &s1, EdbSegment &s2 );
  int   SelectSegments();
  int   MergeSegments();
  int   RefillSegment(EdbSegment &s);
  int   RefitSegments(int wkey=0);

  int   ReconstructGrains();
  int   ReconstructSegments();
  bool  SaveToOutputView(EdbView &vout, int do_h=1, int do_c=2, int do_s=2, int do_tr=0, int do_f=2);

  float SThetaGr(float theta, float phi, float dz, float sx, float sy, float sz);
  float SPhiGr(float theta, float phi, float dz, float sx, float sy, float sz);

  ClassDef(EdbViewRec,1)  // Generation of one microscope view
};

#endif /* ROOT_EdbViewRec */
