#ifndef ROOT_EdbRunAccess
#define ROOT_EdbRunAccess

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunAccess                                                         //
//                                                                      //
// OPERA data Run Access helper class                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH2F.h"
#include "TCut.h"
#include "EdbRun.h"
#include "EdbCell2.h"
#include "EdbPattern.h"
#include "EdbLayer.h"
#include "EdbSegmentCut.h"
#include "EdbScanCond.h"

//______________________________________________________________________________
class EdbRunAccess : public TObject {

 public:
  Int_t     eAFID;        // if =1 - use affine transformations of the fiducial marks
  Int_t     eCLUST;       // 1-use clusters, 0 - do not
  Bool_t    eUseExternalSurface;  // if true - set segment position corrisponding to the very external cluster

  Bool_t    eDoViewAnalysis;     // fill or not the histograms for optional view analysis
  EdbH2     eHViewXY[3];         // XY segments distribution in a view local coords
  Bool_t    eInvertSides;        // 0 -do nothing, 1-invert sides
  Bool_t    eUseDensityAsW;      // in case of LASSO tracking possible to use eSigmaY as eW
  
  Int_t       eDoPixelCorr;        // apply or not pix/mic correction  when read data (default is 0)
  Float_t     ePixelCorrX;         // pixel/micron correction factor to be applied for data
  Float_t     ePixelCorrY;

  TCut         eHeaderCut;  // header cut to be applied in run initialization
  Int_t        eTracking;   // to test tracking alorithm: -1-ignored(def),0/1 - trackings to accept
  
 private:
  TString  eRunFileName;
  EdbRun   *eRun;        // pointer to the run to be accessed

  Int_t eFirstArea;
  Int_t eLastArea;
  Int_t eNareas;

  EdbLayer    *eLayers[3]; // base(0),up(1),down(2)  layers
  EdbScanCond *eCond[3];   //
  TObjArray   *eCuts[3];   // arrays of cuts to be applied to segments
  EdbPattern  *eVP[3];     // base/up/down side patterns (0,1,2) with 
                           // the views coordinates inside

  Float_t      eCutGR;     // grain cut (chi)

  Float_t eXmin,eXmax,eYmin,eYmax;  //run limits

  Float_t eXstep[3];
  Float_t eYstep[3];
  Float_t eViewXmin[3], eViewXmax[3];
  Float_t eViewYmin[3], eViewYmax[3];
  
 public:
  EdbRunAccess();
  EdbRunAccess(EdbRun *run);
  EdbRunAccess(const char *fname);
  virtual ~EdbRunAccess();

  EdbSegment  *GetRawSegment( int vid, int sid, int rs=0 );
  EdbSegment  *GetRawSegment( EdbView &v, int sid, int rs=0 );
  
  void Set0();
  void SetPixelCorrection(const char *str);
  void ClearCuts();
  bool InitRun(const char *runfile=0, bool do_update=false);
  bool InitRunFromRWC(char *rwcname, bool bAddRWD=true, const char* options="");
  bool AddRWDToRun(char *rwdname, const char* options="");
  void Print();
  void PrintStat();
  
  void  CheckRunLine();
  int   Check0Views(EdbPattern &pat, int thres=1);
  int   CheckEmptyViews(EdbPattern &pat);
  float CheckMeanSegsPerView(EdbPattern &pat);
  void  CheckViewStep();
  void  CheckViewStep(int ud);
  TH2F  *CheckUpDownOffsets();
  void  CheckViewSize();

  EdbRun *GetRun() const {return eRun;}
  EdbLayer *GetMakeLayer(int id);
  EdbLayer *GetLayer(int id)
    { if(eLayers[id]) return (EdbLayer *)eLayers[id]; else return 0; }
 
  int FillVP();
  EdbPattern *GetVP(int ud) const { if(ud>-1&&ud<3) return eVP[ud]; else return 0; }

  int FirstArea() const { return eFirstArea;}
  int LastArea()  const { return eLastArea;}

  //return the entries of views in the run tree:

  int GetVolumeArea(EdbPatternsVolume &vol, int area);
  int GetVolumeData(EdbPatternsVolume &vol, int nviews, TArrayI &srt, int &nrej);

  int GetViewsArea(int ud, TArrayI &entr, int area, 
		   float &xmin, float &xmax, float &ymin, float &ymax );
  int GetViewsArea(int ud, TArrayI &entr,  
		   float xmin, float xmax, float ymin, float ymax );
  int GetViewsAreaMarg(int ud, TArrayI &entr, int area, float xmarg, float ymarg);

  int GetViewsXY(int ud, TArrayI &entr, float x, float y, float r=200.);
  int GetEntryXY(int ud, float x, float y);
  int GetVolumeXY(EdbSegP &s, EdbPatternsVolume &vol);

  int GetPatternXYcut(EdbSegP &s, int side, EdbPattern &pat, float dr, float dt);
  int GetPatternXY(EdbSegP &s, int side, EdbPattern &pat, float rmin=200);
  int GetPatternData(EdbPattern &pat, int side, int nviews, TArrayI &srt, int &nrej);
  int GetPatternDataForPrediction( int id, int side, EdbPattern &pat );

  bool  AcceptRawSegment(EdbView *view, int ud, EdbSegP &segP, int side);

  int ViewSide(EdbView *view) const;
//    {    if(view->GetNframesTop()==0) return 2;         // 2- bottom
 //   else if(view->GetNframesBot()==0) return 1;         // 1- top
  //  else return 0; }

  ///////
  float SegmentWeight(const EdbSegment &s);
      
  bool  PassCuts(int ud, EdbSegment &seg);
  int  NCuts(int ud) {
    if(!eCuts[ud])  return 0;
    return eCuts[ud]->GetEntriesFast();
  }

  bool  SetSegmentAtExternalSurface( EdbSegment *seg, int side );
  float GetRawSegmentPix( EdbSegment *seg );
  float CalculateSegmentChi2( EdbSegment *seg, float sx, float sy, float sz );

  void           AddSegmentCut(int xi, const char *cutline );
  void           AddSegmentCut(int ud, int xi, float var[10]);
  void           AddSegmentCut(int ud, int xi, float min[5], float max[5]);
  EdbSegmentCut *GetCut(int ud, int i)
    { return (EdbSegmentCut *)(eCuts[ud]->UncheckedAt(i)); }
  float GetCutGR() const {return eCutGR;}

  void SetCond(int ud, EdbScanCond &cond) { 
    if(ud<0) return;
    if(ud>2) return;
    if(eCond[ud]) delete (eCond[ud]);
    (eCond[ud]) = new EdbScanCond(cond);
  }

  EdbScanCond *GetMakeCond(int ud) {return 0;} //TODO?
  EdbScanCond *GetCond(int ud)
    { if(eCond[ud]) return (EdbScanCond *)eCond[ud]; else return 0; }

  float OverlapX(int ud) {return (ud>0&&ud<3)? (eViewXmax[ud]-eViewXmin[ud] - eXstep[ud]): 0.; }
  float OverlapY(int ud) {return (ud>0&&ud<3)? (eViewYmax[ud]-eViewYmin[ud] - eYstep[ud]): 0.; }

  bool CopyRawDataXY( float x0, float y0, float dR, const char *file );

  void SetCutLeft(   int ud, float wmin );
  void SetCutRight(  int ud, float wmin );
  void SetCutTop(    int ud, float wmin );
  void SetCutBottom( int ud, float wmin );

  ClassDef(EdbRunAccess,1)  // helper class for access to the run data
};

#endif /* ROOT_EdbRunAccess */

