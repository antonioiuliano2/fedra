#ifndef ROOT_AcqStage1
#define ROOT_AcqStage1
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqStage1                                                            //
//                                                                      //
// Interface to the stage controller				         					//
// These classes manage Micos stages X,Y,Z
// Althow the code is based on SySal, the preference is made
// for NI native functions executed on board of NI7344 controller, rather than 
// host processor threads, like in SySal
// The Breakpoint generation functions are exploited and the buffered 
// BP generation is realized by on-board program to provide
// harware syncronization between stage and camera.
// Cable connecting DB25 "Digital IO" connector of MICOS amplifyer block
// with DB9 auxiliary digital connector of Odyssey Xpro must be present
// to provide syncronous stage-framegrabber operation.
// The pinout is as follows:
//
//  DB9 Male		DB25 Female
//  Pin Signal		Pin Signal
//  1   "Trig0"		5   "Breakpoint3"
//  6   "GND"       16  "GND"            
//								                                                //
//////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

// fix rootcint errors with Visual C++ 2005
#ifndef __CINT__     
#  include <conio.h>
#endif

// kbhit, getch POSIX functions are deprecated beginning in Visual C++ 2005. 
// Use the ISO C++ conformant _kbhit and _getch instead.
#if MSC_VER >= 1400        // Visual C++ 2005      MSC_VER=1400 
#  define kbhit _kbhit
#  define getch _getch
#endif

#include "TObject.h"
#include "TCanvas.h"
#include "EdbAffine.h"
#include "EdbFiducial.h"

#include "NI/include/flexmotn.h"

//______________________________________________________________________________
class AcqAxis : public TObject {

 public:

  AcqAxis();
  AcqAxis(int Ax);  // Main constructor, Ax is from 1 to 3
  virtual ~AcqAxis() {} 

	int Axis;      // Axis number (1 to 3)
	int BoardId;   // NI7344 Board number (Default is 0)
	int StepsRev;  // Step motor parameter : steps per revolution
	int LinesRev;  // Encoder parameter : lines per revolution (in case of linear encoder defines line-to-step ratio only).
	float EncoderToMicrons;  // Microns per one line of encoder
	float MicronsToSteps;    // Microns per one step of step motor
	float Accel;             // Max acceleration [mic/s2]
	float MaxSpeed;          // Max speed [mic/s]
	int LimPol,HomePol;      // Limiter switch and Home switch polarities
	int CtlModeIsCWCCW;      // Stepper control mode: Step/Direction (0) or Clockwise/Counterclockwise (1)
	int TurnOffLightTime; //Not implemented yet!
	int Invert;  // defines axis direction inversion
	float HomingSpeed;   // Speed when perfoming home finding
	float Reference;     // user derived Reference value (usually emulsion corner position for X,Y and glass surface for Z)
	float NominalReference;     // Nominal Reference value (usually emulsion corner position for X,Y and glass surface for Z)


  bool Initialize();
  void PosMove( float Pos, float Speed, float Acc); 
  void Move( float Speed, float Acc); 
  void Stop(); 
  void Reset();
  int FindHome(int Direction);
  int WaitForHome(int Timeout);
  int WaitForMoveComplete( u32 Timeout); 
  float GetPos();
  int GetLimiter();
  bool IsMoveComplete();
  void LoadMoveCompleteCriteria(bool MotorOff, bool Stop, bool Delay, bool InPosition, float DeadBand, int sdelay, int minPulse);
  void ArmBreakpoint(float Pos);
  void LoadBreakpointSequence(float *Pos, int N);
  void DumpBreakpointSequence();
  void DisarmBreakpointSequence();
  void ArmBreakpointSequence();
  int IsBreakpointSequenceArmed();
  void DisarmBreakpoint();
  bool IsBreakpointOccured();
  void Test( float *Pos, int N);
  void SetFilterFrequency(int Freq);
  int ReadFollowingError();
  void SetFollowingError(u16 Error);


  ClassDef(AcqAxis,1)  
};


//______________________________________________________________________________
class AcqLight : public TObject {


 public:

  AcqLight();
  virtual ~AcqLight() {} 

	int BoardId;             // NI7344 Board number (0)
	int TurnOffLightTime;    // Light must be turned off if system is idle for a given time (Not implemented)
	int LightLevel;          // Light level (0-32767)
	bool IsON;                
//	TTimer* T;
//	TThread* Thread;

  bool Initialize(); 
  void ResetTimer();  //Not implemented yet
  void ON();
  void OFF();
  int ReadTemperature();

  ClassDef(AcqLight,1)  
};
	

//______________________________________________________________________________
class AcqStage1 : public TObject {


 public:
  int BoardId;    //NI7344 Board number (0)
  float ZHomingXPos, ZHomingYPos;  //Positions on X and Y where Z homing can be made safely.
  float EmTopLayer;  // user measured Thickness of top emulsion layer (by focusing procedure)
  float EmBotLayer;  // user measured Thickness of bottom emulsion layer (by focusing procedure)
  float EmBase;   // user measured Thickness of emulsion base [mic]
  float NominalEmLayer;  // nominal Thickness of emulsion layer
  float NominalEmBase;   // nominal Thickness of emulsion base [mic]
  AcqAxis* X;     // Pointers to Axes and light controller
  AcqAxis* Y;
  AcqAxis* Z;
  AcqLight* L;

  EdbAffine2D* eAffine;
	bool AbsCoords;


  AcqStage1();
  virtual ~AcqStage1() {X->Stop(); Y->Stop(); Z->Stop();} 
  bool Initialize(); 
  void Stop();
  int GetLastError(char * Description, int size);
  int GetLastModalError(char * Description, int size);
  void Monitor();
  void ScanToZ0AndGotoXYZ(float Z0, float Z0speed, float Z0acc, float X, float Y,  float XYSpeed, float XYAcc, float Z,  float Zspeed, float Zacc, u32 Timeout);

  ClassDef(AcqStage1,1)  
};



#endif /* ROOT_AcqStage1 */
