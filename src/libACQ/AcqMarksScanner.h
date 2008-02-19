#ifndef ROOT_AcqMarksScanner
#define ROOT_AcqMarksScanner
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqMarksScanner                                                      //
//                                                                      //
// Interface to Fiducial Marks Scanner							        //
//								                                        //
//////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

#include "AcqStage1.h"
#include "TObject.h"
#include "AcqOdyssey.h"
#include "EdbFiducial.h"
#include "EdbAffine.h"

//______________________________________________________________________________
class AcqMarksScanner : public TObject {

private:
// AcqStage1* ST;
// AcqOdyssey* FG;
 
 public:
  AcqMarksScanner();
  virtual ~AcqMarksScanner();
  bool SetPointers(AcqStage1* stg, AcqOdyssey* ody ){ST=stg ; FG=ody; return 1;};
  bool SetMapext(char * mapext);
  bool GotoMark(int Id);
  int ScanView(float Z0, float DZ);
  bool SearchMark(float *X=NULL, float *Y=NULL, float *Z=NULL);
  bool SearchMarkFast(float *X=NULL, float *Y=NULL, float *Z=NULL);
  bool ScanMark( float *X=NULL, float *Y=NULL, float *Z=NULL);
  bool ScanMarks();
  EdbArea *eArea;
  EdbMarksSet  *eMarksSet;
  EdbMarksSet  *eFoundMarksSet;
  EdbAffine2D *eAffine2D;
  AcqStage1* ST;
  AcqOdyssey* FG;
  int Side;
  bool IsValid;
  float FPS; //Frames per second to use when scanning FM's (max ~220 presently, 200 is safe)
  int Layers;  // Layers to scan ( default 10)
  int Threshold;  // Pixel pulsehight threshold to use for clustering (0-255)
  float MarkDiameter; // Nominal mark diameter [mic]
  float MarkDiameterTolerance;  // tolerance [mic]
  int ViewsToScan;   // Number ov views to scan when searching for FM (in spiral pattern)
  float StepX,StepY;  // steps between adjacent views in spiral pattern
  float XYScanSpeed;  // view-to-view move speed [mic/s]



  ClassDef(AcqMarksScanner,1)  
};



#endif /* ROOT_AcqMarksScanner */
