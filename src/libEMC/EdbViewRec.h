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
  Int_t      eNeib[9]; // 3x3 neighborhood

public:
	EdbViewCell();
  ~EdbViewCell(){}

  void SetLimits( float xmin, float xmax, float ymin, float ymax)
	{ eXmin=xmin; eXmax=xmax; eYmin=ymin; eYmax=ymax; }
  void SetS( float sx, float sy)
	{eSx=sx; eSy=sy;}
  void CalcN();
  void SetNfr(int nfr) {eNfr=nfr;}

  int  FillCell( EdbView &v );
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
private:

  Int_t     eClMinA;  //  rejected cl->eArea < eClMinA (image noise)
  Int_t     eClMaxA;  //  rejected cl->eArea > eClMaxA (blobs)

  EdbViewCell eVC;

  EdbView    *eView;       // point to view currently in processing
  Int_t       eNgr;        // number of grains
  Int_t       eNgrMax;
  TClonesArray *eG;         //! array of 3-d grains
  TTree      *eGrainsTree; // debug tree

  Int_t   eNtheta;    // divisions by theta
  TArrayF eR;         // R(it) limits definition
  TArrayF eSphi;      // phiStep(it)
  TArrayI eNphi;      // nphi(it)
  Int_t   eNphiTot;   // Sum of all nphi(it)
  TArrayF eTmask;     // 
  TArrayF ePmask;     // 
  Long_t  eNtot;      // eNphiTot*eNx*eNy
  Int_t   eStep;      // frame step size for seed search: default=1
  Int_t   eStepFrom;  // low   limit (ifr) for seed search
  Int_t   eStepTo;    // upper limit (ifr) for seed search
  TArrayS ePhist;     // "Phase histogram"
  EdbCluster **eC;    //!
  Int_t   eSeedThres; // threshold for the seed puls
  Short_t eSeedLim;   // limit for the seed pointers number
  Short_t eNseedMax;  // limit for the good seeds (segments) to be processed
  Int_t   ePulsThres; // threshold for the segment puls

  TObjArray *ePS;     //! array of segments

public:
  EdbViewRec();
  EdbViewRec(EdbViewDef &vd): EdbViewDef(vd) {SetDefRec();}
  ~EdbViewRec(){}

  void  SetDefRec();
  bool  SetView(EdbView *v);
  void  SetClThres(int mina, int maxa) { eClMinA=mina; eClMaxA=maxa; }
  void  SetSeedThres(Short_t mins, Short_t maxs=48, Short_t nseedmax=100) 
    { eSeedThres=mins; eSeedLim=maxs; eNseedMax=nseedmax; }
  void  SetStep(int sfrom, int sto, int step=1)    { eStepFrom=sfrom; eStepTo=sto; eStep=step; }

  void  ResetClustersSeg();
  int   FillViewCells();
  int   FindGrains();
  int   FitSegment(EdbSegment &s);
  int   CheckFramesShift();
  void  InitGrainsTree(const char *file="grains.root");
  int   FillGrainsTree();

  int   FindSeed();
  int   CheckSeedThres();
  int   SelectSegments();
  int   SeekBySeed(EdbSegment &s, float t, float p, int j);

  ClassDef(EdbViewRec,1)  // Generation of one microscope view
};

#endif /* ROOT_EdbViewRec */
