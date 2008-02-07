#ifndef ROOT_AcqOdyssey
#define ROOT_AcqOdyssey
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqOdyssey                                                           //
//                                                                      //
// Interface to MATROX ODYSSEY framegrabber								//
//								                                        //
//////////////////////////////////////////////////////////////////////////
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>



#include "TObject.h"
#ifndef __CINT__
#include "Mil.h"
# include "imapi.h"
//#include "mconvol.h"
#endif

#include "EdbImage.h"



/* Max Number of buffers in the sequence. */
#define MAX_NB_GRAB            50
typedef long MIL_ID; 


//______________________________________________________________________________
class AcqOdyssey : public TObject {

private:
   MIL_ID   MilApplication  ;
   MIL_ID   MilSystem       ;
   MIL_ID   MilDigitizer    ;
   MIL_ID   MilDisplay      ;
   MIL_ID   MilImage[MAX_NB_GRAB];
   MIL_ID   MilImageN;
   MIL_ID   MilBGImage;
   MIL_ID   MilConvImage;
   MIL_ID   MilConvImage1;
   MIL_ID   MilImageDisp    ;
   MIL_ID   MilKernel    ;
   int SizeX,SizeY;  //Image size, aquired from digitizer settings
// ONL variables
    long System;
    long Device;
    long Camera;
    long Digitizer;
    long Control;
	long BufferN;
	long ConvImage;
	long Kernel;
	long BGImage;
	long Buffer[MAX_NB_GRAB];
	long ShiftBuffer[MAX_NB_GRAB];
	long ShiftList;
    long Thread, GrabThread, SeqThread;
    long ControlBuf;
    long GrabOSB;
    long CopyOSB;
	char bNorm;



 public:


  AcqOdyssey();
  virtual ~AcqOdyssey();
  bool Initialize(char * DCF, int N); 
  void FreeResources(); 
  void SetFIRFKernel(char *Kern, int W, int H);
  void SetFIRFNorm(int Norm);
  void GrabImages(int N);
  void GrabImagesONL(int N);
  void ShowImage(int N);
  void ShowBGImage();
  void GetImage(int N, char* buf);
  EdbImage* GetEdbImage(int N);
  void PutImage(int N, char* buf);
  void CopyImageToBG(int N);
  void PutBGImage(short* buf);
  void ShowImages(int N1, int N2, int TimeInterval, int Times);
  void TracksFromNImages(int N, float TX, float TY, int Dest);
  bool IsGrabComplete();
  void FindClusters(int N);
  
  bool BGImageValid;
  bool SubtractBG;
  bool ApplyFIRF;
  char Threshold;

  ClassDef(AcqOdyssey,1)  
};



#endif /* ROOT_AcqOdyssey */
