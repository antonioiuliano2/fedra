#ifndef ROOT_AcqOdyssey
#define ROOT_AcqOdyssey
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqOdyssey                                                           //
//                                                                      //
// Interface to MATROX ODYSSEY framegrabber								//
//
// Cable connecting DB25 "Digital IO" connector of MICOS amplifyer block
// with DB9 auxiliary digital connector of Odyssey Xpro must be present
// to provide syncronous stage-framegrabber operation.
// The pinout is as follows:
//
//  DB9 Male		DB25 Female
//  Pin Signal		Pin Signal
//  1   "Trig0"		5   "Breakpoint3"
//  6   "GND"       16  "GND"
//								                                        //
//////////////////////////////////////////////////////////////////////////
//#endif

#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

#include "TH1.h"
#include "TObject.h"
#ifndef __CINT__
#  include "Mil.h"
#  include "imapi.h"
//#   include "mconvol.h"
#endif

#include "EdbView.h"
#include "EdbImage.h"
#include "EdbFrame.h"
#include "EdbCluster.h"
#include "TClonesArray.h"
#include "EdbIP.h"


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
   MIL_ID   MilOverlayImage   ;
   long    TransparentColor ;
   MIL_ID   MilImage[MAX_NB_GRAB];
   MIL_ID   MilImageN;
   MIL_ID   MilBGImage;
   MIL_ID   MilGCImage;
   MIL_ID   MilConvImage;
   MIL_ID   MilConvImage2;
   MIL_ID   MilConvImage32;
   MIL_ID   MilImageDisp    ;
//   MIL_ID   MilKernel    ;
   MIL_ID   MilGCKernel    ;
	   MIL_ID MilProjResX;
	   MIL_ID MilProjResY;

   // ONL variables
    long System;
    long Device;
    long Camera;
    long Digitizer;
    long Control;
	long BufferN;
	long Overlay;
	long ConvImage;
	long ConvImage2;
	long ConvImage32;
	long Kernel;
	long BGImage;
	long GCImage;
	long Buffer[MAX_NB_GRAB];
	long HostBuffer[MAX_NB_GRAB];
#ifndef __CINT__
	void *pHostBuffer[MAX_NB_GRAB];
#endif
	long HostBufferPitch;
	long HostBufferLines;
	long ShiftBuffer[MAX_NB_GRAB];
	long ShiftList;
    long Thread, GrabThread, SeqThread;
    long ControlBuf;
    long GrabOSB;
    long CopyOSB;
    long CopyOSB1;
	char bNorm;



 public:

// Genetive functions
  AcqOdyssey();
  virtual ~AcqOdyssey();
  bool Initialize(char * DCF, int N); 
  void FreeResources(); 

// Image in-out functions
  void GetImage(int N, char* buf);
  void GetImageFast(int N, unsigned char** buf, long *Pitch, long* Lines);
  EdbImage* GetEdbImage(int N);
  EdbFrame* GetEdbFrame(int N);
  EdbClustersBox* GetEdbClustersBox(int N1, int N2);
  void PutImage(int N, char* buf);
  void CopyImageToBG(int N);
  void CopyImageToGC(int N, long ConstBG=0);
  void PutBGImage(short* buf);
  void GetBGImage(short* buf);
  void PutGCImage(short* buf);
  void GetGCImage(short* buf);

// Image and overlay display finctions
  void ShowImage(int N);
  void ShowBGImage();
  void ShowGCImage();
  void PlotImage1D(int N, TH1F* hist=NULL);
  void PlotImage2D(int N);
  void PlotBGImage();
  void PlotGCImage();
  void ShowImages(int N1, int N2, int TimeInterval, int Times);
  void DrawTrack(int N, float X, float Y, float TX, float TY);
  void DrawClusters(TClonesArray * Clusters=NULL);
  void DrawText(int Layer, int X, int Y, char *text);
  void DrawGrid(float MajorPitch=100., float MinorPitch=10.); //pitches are in microns!
  void DrawCircles(float RadPitch=100.); //pitches are in microns!
  void DrawClear();


  // Grab control functions 
  void SetFIRFKernel(char *Kern, int W, int H);
  void SetFIRFNorm(int Norm);
  int GrabImagesONL(int N, float *Zs=NULL, double Timeout=0);
  int GrabRawImagesONL(int N, float *Zs=NULL, double Timeout=0);
  long GrabAndFillView(int N, float *Zs=NULL, double Timeout=0, int Side=0, EdbView * View=NULL, bool FillImages=false);
  bool IsGrabComplete();
  int GrabOneImage(float Z, double Timeout=0);
  void StartLiveGrab();
  void StopLiveGrab();

// Image analisys functions
  void XProjectImage(int N, long *buf);
  void YProjectImage(int N, long *buf);
  int VFindEdge(int N, int Thresh, int MinWidth, int MaxWidth);
  int HFindEdge(int N, int Thresh, int MinWidth, int MaxWidth);
  void TracksFromNImages(int N, int DX, int DY, float TX, float TY, int Dest);
  long FindClusters(int N, TClonesArray * Clusters=NULL, int Side=0); //clusterisation is made onboard of Odyssey
  long Clusterize(int N, TClonesArray * Clusters=NULL, int Side=0);  //clusterisation is made by host processor
  long Clusterize(int N, unsigned char *buf, int nc,int nr, TClonesArray * Clusters=NULL, int Side=0);  //clusterisation is made by host processor
  long FillView(int N, int Side=0, EdbView * View=NULL, bool FillImages=false);

// Auxiliary functions
  void UARTSendString(const char *s, int N);
  // int BurnPix( unsigned char *buf, int ic, int ir, int xSize, int ySize, EdbClustP *cl );
 
  int SizeX, SizeY;   //Image size, aquired from digitizer settings
  bool BGImageValid;  // Flag set when BG is taken
  bool GCImageValid;  // Flag set when Gain Correction image is taken
  bool SubtractBG;    // Flag to be set by user, controls BG subtraction
  bool ApplyFIRF;     // Flag to be set by user, controls FIR filtering
  bool ApplyGainCorr; // Flag to be set by user, controls gain correction
  int Threshold;      // Parameter to be set by user, controls threshold cut on the image
  float ZBuffer[MAX_NB_GRAB]; //Contains Z coordinates of images after scan
  TClonesArray * eEdbClusters; //Contains clusters when FindClusters() or Clusterise() executed
  EdbView * eEdbView;  // contains EdbView when FillView() is executed
  float PixelToMicronX;
  float PixelToMicronY;
  float OpticalShrinkage;
  int MaxNbGrab; // set to MAX_NB_GRAB, don't modify!!!!

  ClassDef(AcqOdyssey,1)  
};



#endif /* ROOT_AcqOdyssey */
