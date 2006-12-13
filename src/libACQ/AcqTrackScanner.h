#ifndef ROOT_AcqTrackScanner
#define ROOT_AcqTrackScanner
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqTrackScanner                                                      //
//                                                                      //
// Interface to Microtracks Scanner							        //
//								                                        //
//////////////////////////////////////////////////////////////////////////

#include "AcqStage1.h"
#include "TObject.h"
#include "AcqOdyssey.h"
#include "EdbAffine.h"
#include "EdbRun.h"

//______________________________________________________________________________
class AcqTrackScanner : public TObject {

 
 public:
  AcqTrackScanner();
  virtual ~AcqTrackScanner();
  bool SetPointers(AcqStage1* stg, AcqOdyssey* ody ){ST=stg ; FG=ody; return 1;};
  int ScanView(float Z0, float DZ, float GotoX=0, float GotoY=0, bool RawImages=false);
  long ScanAndFillView(float Z0, float DZ,  int Side=0, EdbView * View=NULL, bool FillImages=false, float GotoX=0, float GotoY=0);
  int FindLayer(float Z0, float DZ, int ClusterThreshold, float *z0,float *z1);
  int FastScanFragment(bool IsTop, int AreaId, bool AbsCoord, float X0, float Y0, EdbRun *run, bool RawImages=false);

  int Layers;                 // Number of layers to scan
  int FPS;                    // Frames per second for Z scan (200 is safe so far)
  float XYViewToViewSpeed;    // [mic/s]
  float ZBackStrokeSpeed;     //[mic/s]
  float ViewStepX, ViewStepY; // step between views in [mic]
  int Nx,Ny;                  // Fragment size in views
  AcqStage1* ST;
  AcqOdyssey* FG;

  ClassDef(AcqTrackScanner,1)  
};



#endif /* ROOT_AcqTrackScanner */
