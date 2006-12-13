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

class TTree;
class EdbView;

//______________________________________________________________________________
class EdbViewCell : public TObject {

public:
  Float_t   eXmin,eXmax;
  Float_t   eYmin,eYmax;
  Float_t   eZmin,eZmax;
  Float_t   eBinZ;        // the distance between layers: eBinZ = (eZmax-eZmin)/eNfr
  Float_t   eBinX;        // bin along x
  Float_t   eBinY;        // bin along y
  Int_t     eNfr;         // number of frames (to be setted!)

  Int_t     eNx;          // "raw"    - divisions along x
  Int_t     eNy;          // "column" - divisions along y
  Int_t     eNcellXY;     // eNx*eNy
  Int_t     eNcell;       // eNfr*eNx*eNy
  Int_t     eNcl;         // total number of clusters

  Int_t     eIFZ;         // cell filling flag: if IFZ=0 (default) use c->eFrame else use c->eZ


  Int_t     *eNC;         //! [eNcell] number of clusters/cell

 private:
  Int_t    eNcellsLim;         // max number of cells (need for memory allocation)
  Int_t    eCellLim;           // max number of clusters/cell

  EdbCluster    **epC;        //! pointers to clusters [eNcellsLim*eCellLim]
  EdbCluster   ***epCell;     //! pointers to cells    [eNcellsLim]
  Int_t          *eFrame;     //! index of the first cell of the frame in epCell array

  Int_t      eNeib[9];    // 3x3 neighborhood

 public:
  EdbViewCell();
  virtual ~EdbViewCell();

  void SetLimits( float xmin, float xmax, float ymin, float ymax, float zmin, float zmax )
    { eXmin=xmin; eXmax=xmax; eYmin=ymin; eYmax=ymax; eZmin=zmin; eZmax=zmax; }
  void SetBin( float binx, float biny, float binz=-1)
    {eBinX=binx; eBinY=biny; eBinZ=binz;}
  void SetCellLimits(int ncell, int ncl) { eNcellsLim=ncell; eCellLim=ncl; }

  void SetNfr(int nfr, float zmin, float zmax, int ifz=0);
  void CalcN();
  void InitMem();
  void Init();

  int  AddCluster(EdbCluster *c)
    {
      int j;
      if(!eIFZ) j = Jcell(c->eX,c->eY,c->eFrame);
      else      j = Jcell(c->eX,c->eY,c->eZ);
      if( j<0 || j>eNcellsLim )                    return 0;
      if( eNC[j] >= eCellLim )                     return 0;
      epCell[j][eNC[j]] = c;
      eNC[j]++;
      return 1;
    }

  int  FillCell( TClonesArray &v );
  void CleanCell();
  void Delete();

  void CalcStat();

  int IXcell(float x) const {return (int)((x-eXmin)/eBinX); }
  int IYcell(float y) const {return (int)((y-eYmin)/eBinY); }
  int JcellXY(float x, float y) const {return IYcell(y)*eNx+IXcell(x); }
  int Jcell(float x, float y, int ifr ) const {return eFrame[ifr] + IYcell(y)*eNx+IXcell(x); }
  int Jcell(float x, float y, float z ) const {return TMath::Nint((z-eZmin)/eBinZ)*eNcellXY + IYcell(y)*eNx+IXcell(x);}
  int Jcell(int  ixy, int ifr)          const {return eFrame[ifr]+ixy;}
  int Jcell(int  ix, int iy, int ifr)   const {return eFrame[ifr]+iy*eNx + ix;}
  int Jneib(int i) const {return eNeib[i];}

  EdbCluster  **GetCell(int j) const {return epCell[j];}

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
  Float_t        eGrainNbin;       // acceptance for grain preprocessing = eGrainNbin*eGrainSX(Y)

private:
  EdbView    *eView;               //! pointer to the input  view currently in processing

  Int_t         eNgrMax;           // grains limit (for mem alloc)
  Int_t         eNsegMax;          // segments limit (for mem alloc)

  EdbViewCell *eVC;                //! pointer to eVCC or eVCG
  EdbViewCell eVCC;                //! cells with raw clusters
  EdbViewCell eVCG;                //! cells with grains

  //  TClonesArray  *eGSeg;            //! [eNgrMax] array of grains represented as segments
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
  Float_t       eSigmaMin;     // min and max values for segment acceptance
  Float_t       eSigmaMax; 

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
  void  SetPulsThres(int minp, int maxp=500) { ePulsMin=minp; ePulsMax=maxp;  }
  void  SetSigmaThres(float smin, float smax) { eSigmaMin=smin; eSigmaMax=smax;  }
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
