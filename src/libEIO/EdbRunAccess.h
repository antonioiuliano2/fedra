#ifndef ROOT_EdbRunAccess
#define ROOT_EdbRunAccess

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunAccess                                                         //
//                                                                      //
// OPERA Run Access helper class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbRun.h"
#include "EdbPattern.h"
#include "EdbLayer.h"

//______________________________________________________________________________
class EdbRunAccess : public TObject {

 private:

  TString  eRunFileName;
  EdbRun  *eRun;        // pointer to the run to be accessed

  Int_t eFirstArea;
  Int_t eLastArea;
  Int_t eNareas;

  Int_t     eAFID;        // if =1 - use affine transformations of the fiducial marks
  Int_t     eCLUST;       // 1-use clusters, 0 - do not

  EdbLayer *eLayers[3];  // base(0),up(1),down(2)  layers

  EdbPattern  *eVP[3];    // base/up/down side patterns (0,1,2) with 
                          // the views coordinates inside

  TObjArray   *eCuts[3];  // arrays of cuts to be applied to segments

  Float_t eXstep;
  Float_t eYstep;

 public:
  EdbRunAccess();
  EdbRunAccess(EdbRun *run);
  EdbRunAccess(const char *fname);
  virtual ~EdbRunAccess() {}

  void Set0();
  bool InitRun();
  void PrintStat();

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

  int GetViewsArea(int ud, TArrayI &entr, int area);
  int GetViewsAreaMarg(int ud, TArrayI &entr, int area, float xmarg, float ymarg);

  int GetViewsXY(int ud, TArrayI &entr, float x, float y);

  bool  AcceptRawSegment(EdbView *view, int id, EdbSegP &segP, int side) {return true;}

  int ViewSide(EdbView *view) const 
    { if(view->GetNframesTop()==0) return 2;            // 2- bottom
    else if(view->GetNframesBot()==0) return 1;         // 1- top
    else return 0; }

  ClassDef(EdbRunAccess,1)  // helper class for access to the run data
};

#endif /* ROOT_EdbRunAccess */
