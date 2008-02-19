#ifndef ROOT_AcqSerialPlateChanger
#define ROOT_AcqSerialPlateChanger
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqSerialPlateChanger                                                      //
//                                                                      //
// 	Interface to Bern emulsion plate changer			        		//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

#include <cmath>


#include "TObject.h"
#include "AcqStage1.h"
#include "AcqCOM.h"


#define MAX_NUMBER_OF_PLATES		100
#define VPOS_UP		0
#define VPOS_DOWN		1
#define VPOS_DOWN_TO_STAGE		2
#define VPOS_ABOVE_STAGE		3

struct IdentifierS
{
	int Part[4];
};



struct TPlateS
{
	IdentifierS id;
	int SeqNumber;
	int InBank;
	bool IsSeparator;

};

struct TBankS
{
	int Number; //0- microscope, 1-3 banks 
    int NEmulsions; //how many plates it contains
    int NSeparators; //how many plates it contains
	TPlateS* Plates[MAX_NUMBER_OF_PLATES*2];
};



//______________________________________________________________________________
class AcqSerialPlateChanger : public TObject {

private:
	float StX,StY,StZ; //stage coordinates to save before arm power-on
	AcqCOM *port;
	char VacRegister;
 public:
  AcqSerialPlateChanger();
  virtual ~AcqSerialPlateChanger();

// Low level hardware control functions
void OnReset(); //Here we write all 0 in ports P4 and P5
int Initialize(AcqStage1* Stage=NULL);
int PowerON(int timeout=100);
int PowerOFF(int timeout=100);
int ArmVacuumControl(bool VacOn, int timeout);
int TableVacuumControl(bool VacOn, int timeout, bool BlowOn=false);
int  GoOnHAxis(int position, int timeout);
int  GoOnVAxis(int position, int timeout);

// Consistency and bank information management
int ReadStatusFile();
void UpdateStatusFile();
void ResetBanks();
virtual void Print();  
TPlateS* TakeFromBank(int bnk);
void AddToBank(int bnk, TPlateS* plate);
int FindEmulsion(IdentifierS id, int Bank);
void LoadBrick(int Brick, int Nemulsions=57, int SrcBank=1, int DestBank=2, bool Separators=true, bool FirstIsSeparator=true); 
void UnloadBrick();
int GetLoadedId(int *id0=NULL, int *id1=NULL,int *id2=NULL,int *id3=NULL); //returns 0 if not loaded

// COM port settings
   int ComPort;
   int ComBaud;

// Emulsion and arm manipulation methods
    int Park(int idst);
    int MoveEmulsion(int isrc, int idst);
	int UnloadPlate(bool TurnPowerOff=true);
	int LoadPlate(IdentifierS id);
	int LoadPlate(int id0, int id1, int id2=0, int id3=0);


	bool InitDone;					// flag set when Initialize() is done
	bool Ready;				        // flag set when banks are filled from status file 
	bool StageError;                // flag is set if the stage position changed during robot operation
									// this is to catch a "stage jump" bug in stage firmware
									// when this condition is detected stage must be re-homed by host script
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

  AcqStage1 *ST;	// pointer to stage

// Auxiliary members
  bool Debug;
   ClassDef(AcqSerialPlateChanger,1)  

};



#endif /* ROOT_AcqSerialPlateChanger */
