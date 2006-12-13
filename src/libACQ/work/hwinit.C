AcqStage1 *s;
AcqOdyssey *o;
AcqMarksScanner *FM;
AcqTrackScanner *TS;
AcqSerialPlateChanger *PC;
	int LLMIN=23000; //minimum light level to use
	int LLMAX=32767; //maximum light level
	int LDelay=200; // lamp delay in ms

float FPS=150; //camera frame rate to use
int ThrTop=60; //Thresholds an ADC counts for clustering
int ThrBot=30;
int Cluster_Threshold=400; //threshold on number of clusters insude the em. layer

int FragNx=5; //fragment size in number of views
int FragNy=5; 

//FIR filter kernel definitions
static const int KernelSize=6;
///int Norm=3;
int Norm=12;
char Kernel[KernelSize * KernelSize]=
/*{
	-1,-1,-1,-1,-1,
	-1, 1,2,1,-1,
	-1, 2,4,2,-1,
	-1, 1,2,1,-1,
	-1,-1,-1,-1,-1
};

{
	-4,  -4, -4, -4, -4,
	-4, 7,7,7, -4,
	-4, 7,7,7, -4,
	-4, 7,7,7, -4,
	-4,  -4, -4, -4, -4
};

{ 
  -5,  -5,  -5,  -5,  -5,  -5, -5
  -5,  4,   4,    4,   4,  4,  -5,
  -5,  4,   6,    6,   6,  4,  -5,
  -5,  4,   6,    8,   6,  4,  -5,
  -5,  4,   6,    6,   6,  4,  -5,
  -5,  4,   4,    4,   4,  4,  -5,
  -5,  -5,  -5,  -5,  -5,  -5, -5};

*/
{ 
  -4,  -4,  -4,  -4,  -4,  -4,
  -4,  5,  5,  5,  5,  -4,
  -4,  5,  5,  5,  5,  -4,
  -4,  5,  5,  5,  5,  -4,
  -4,  5,  5,  5,  5,  -4,
  -4,  -4,  -4,  -4,  -4,  -4  };
/*
{
	{1, 1, 1, 1, 1, 1},
	{1,-1,-1,-1,-1, 1},
	{1,-1,-2,-2,-1, 1},
	{1,-1,-2,-2,-1, 1},
	{1,-1,-1,-1,-1, 1},
	{1, 1, 1, 1, 1, 1}
};*/
    int Layers=16; //number of layers to scan
	float DZ=35; //microns in stage space
	float Z0=648;
	float FPS=150;
	float Speed=1.*FPS*DZ/(Layers-1);
	int Cluster_Threshold=300;

int hwinit()
{
	int res;

// STAGE initialization and configuration
	/*
	For each axis:

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


	Once for the stage object:
  float ZHomingXPos, ZHomingYPos;  //Positions on X and Y where Z homing can be made safely.
  float EmTopLayer;  // user measured Thickness of top emulsion layer (by focusing procedure)
  float EmBotLayer;  // user measured Thickness of bottom emulsion layer (by focusing procedure)
  float EmBase;   // user measured Thickness of emulsion base [mic]
  float NominalEmLayer;  // nominal Thickness of emulsion layer
  float NominalEmBase;   // nominal Thickness of emulsion base [mic]
	*/
	s=new AcqStage1();
	s->Initialize();
    s->X->LoadMoveCompleteCriteria(0,1,0,1,2.,0,0);
    s->Y->LoadMoveCompleteCriteria(0,1,0,1,2.,0,0);
//    s->X->LoadMoveCompleteCriteria(0,1,0,1,1.,0,0);
//    s->Y->LoadMoveCompleteCriteria(0,1,0,1,1.,0,0);
    s->Z->LoadMoveCompleteCriteria(0,1,0,1,1.,0,0);
	s->X->SetFilterFrequency(0); //Max 25.6MHz
	s->Y->SetFilterFrequency(0);
	s->Z->SetFilterFrequency(0);
	s->Z->Reference=200;
	s->Z->NominalReference=200;
	s->X->Reference=-137000;
	s->X->NominalReference=-137000;
	s->Y->Reference=26595;
	s->Y->NominalReference=26595;
	s->L->LightLevel=24567;
	s->L->ON();

// ODYSSEY init and config
	/*
	  int SizeX, SizeY;   //Image size, aquired from digitizer settings
  bool SubtractBG;    // Flag to be set by user, controls BG subtraction
  bool ApplyFIRF;     // Flag to be set by user, controls FIR filtering
  bool ApplyGainCorr; // Flag to be set by user, controls gain correction
  int Threshold;      // Parameter to be set by user, controls threshold cut on the image
  float PixelToMicronX;
  float PixelToMicronY;
  float OpticalShrinkage;

	*/
	o=new AcqOdyssey();
	o->PixelToMicronX=-0.293;
	o->PixelToMicronY=0.293;
	if(!o->Initialize("..\\mc1310_8channels_ASYNC_TRIG.dcf",48)) return 0;
    o->UARTSendString(":r60B0",6);  //to set camera to async pulsewidth mode
	o->SetFIRFKernel((char*)Kernel,KernelSize,KernelSize);
	o->SetFIRFNorm(Norm);

// Fiducial Mark scanner init and config
	/*
	  float FPS; //Frames per second to use when scanning FM's (max ~220 presently, 200 is safe)
  int Layers;  // Layers to scan ( default 10)
  int Threshold;  // Pixel pulsehight threshold to use for clustering (0-255)
  float MarkDiameter; // Nominal mark diameter [mic]
  float MarkDiameterTolerance;  // tolerance [mic]
  int ViewsToScan;   // Number ov views to scan when searching for FM (in spiral pattern)
  float StepX,StepY;  // steps between adjacent views in spiral pattern
  float XYScanSpeed;  // view-to-view move speed [mic/s]

	*/
	FM=new AcqMarksScanner();
	FM->SetPointers(s,o);
	FM->FPS=FPS;
	FM->MarkDiameter=70; 
	FM->MarkDiameterTolerance=10; //new opera marks
//	FM->SetMapext("mapext: 0 0 0 0; 3 0 0 125000 100000; 1 10000 10000 10000 10000 1 1 0; 2 115000 10000 115000 10000 1 1 0; 3 109800 90200 109800 90200 1 1 0;"); //B100235
	FM->SetMapext("mapext: 0 0 0 0; 3 0 0 125000 100000; 0 9490.468750 10242.023438 9490.468750 10242.023438 1 1 0; 1 114791.359375 10212.367188 114791.359375 10212.367188 1 1 0; 2 109716.664063 90540.507813 109716.664063 90540.507813 1 1 0;");
// Track scanner init and config
/*
  int Layers;                 // Number of layers to scan
  int FPS;                    // Frames per second for Z scan (200 is safe so far)
  float XYViewToViewSpeed;    // [mic/s]
  float ZBackStrokeSpeed;     //[mic/s]
  float ViewStepX, ViewStepY; // step between views in [mic]
  int Nx,Ny;                  // Fragment size in views

*/
	TS=new AcqTrackScanner();
	TS->SetPointers(s,o);
	TS->FPS=FPS;
	TS->Layers=Layers;
	TS->Nx=FragNx; TS->Ny=FragNy;

// PlateChanger init and config
	/*
	// COM port settings
   int ComPort;
   int ComBaud;
 
	int SourceBank;					// source bank (1 to NumberOfBanks) 
	int DestinationBank;			// source bank (1 to NumberOfBanks   
	int PositionTimeout;			// Timeout in x100ms to position the arm
	int VacuumTimeout;				// Timeout in x100ms to reach the vacuum on table or arm
	int NumberOfBanks;              // Number of banks on the table
	bool LiftCorners;				// if set - arm tries first to lift corners of the emulsion
									// when taking it from the stage
	int NumberOfAttempt;			// max number of attempts to take emulsion from the plate
	int SeparationAttempts;			// number of up-down movements against brushes in bank 
									// to separate emulsion from separator

	float XLoadPosition; //stage positions for safe emulsion load/unload
	float YLoadPosition;
	float ZLoadPosition;

	*/
	PC=new AcqSerialPlateChanger();
	PC->ComPort=2;
	PC->Initialize(s);
	PC->XLoadPosition=-76548;
	PC->YLoadPosition=3677;
	PC->Print();

// AOB
//	inittracker();
	return 1;
}
