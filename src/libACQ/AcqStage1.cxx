//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqStage1                                                            //
//                                                                      //
// Interface to the MICOS stage NI7344 controller						//
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
//								                                        //
//////////////////////////////////////////////////////////////////////////

#include "AcqStage1.h"
#include "TROOT.h"
#include "TSystem.h"

AcqStage1 *gStage=0;

//GLOBALS AND LOCAL FUNCTIONS//////
//ErrorHandler - to print the error description to screen		
///////////////////////////////////////////////////////////////////////////////
// ErrorHandler - to print the error description to screen
//
//
// PARAMETERS: i32 errorCode
//					u16 commandID
//					u16 resourceID
//
// DESCRIPTION: This function takes the error code, command ID and resourceID
//	             and prints the error description to the screen.
//					 If commandID is zero then it just displays the error	- else it
//					 displays the complete description with the function name corresponding
//					 to the commandID
////////////////////////////////////////////////////////////////////////////////					
void ErrorHandler(i32 errorCode, u16 commandID, u16 resourceID){
	
	i8 *errorDescription;		//Pointer to i8's -  to get error description
	u32 sizeOfArray;				//Size of error description
	u16 descriptionType;			//The type of description to be printed
	i32 status;						//Error returned by function

	if(commandID == 0){
		descriptionType = NIMC_ERROR_ONLY;
	}else{
		descriptionType = NIMC_COMBINED_DESCRIPTION;
	}
	
	//First get the size for the error description
	sizeOfArray = 0; 
	errorDescription = NULL;//Setting this to NULL returns the size required
	status = flex_get_error_description(descriptionType, errorCode, commandID, resourceID, 
													errorDescription, &sizeOfArray );

	//Allocate memory on the heap for the description
	errorDescription = (i8*)malloc(sizeOfArray + 1);

	sizeOfArray++; //So that the sizeOfArray is size of description + NULL character
	// Get Error Description
	status = flex_get_error_description(descriptionType, errorCode, commandID, resourceID, 
													errorDescription, &sizeOfArray );

	if (errorDescription != NULL){
		printf("\n");
		printf(errorDescription);//Print description to screen
		free(errorDescription);//Free allocated memory
	}else{
		printf("Memory Allocation Error");
	}
}


ClassImp(AcqStage1);
ClassImp(AcqAxis);
ClassImp(AcqLight);
//_____________________________________________________________________________ 
// These classes manage Micos stages X,Y,Z
// Althow the code is based on SySal, the preference is made
// for NI native functions executed on board of NI7344 controller, rather than 
// host processor threads, like in SySal
// The Breakpoint generation functions are exploited and the buffered 
// BP generation is realized by on-board program to provide
// harware syncronization between stage and camera.
// Cable connecting DB25 "Digital IO" connector of MICOS amplifyer block
// with DB9 auxiliary digital connector of Odyssey Xpro must be present
// to provide syncronous stage-framegrabber operation
// the pinout is as follows:
//
//  DB9 Male		DB25 Female
//  Pin Signal		Pin Signal
//  1   "Trig0"		5   "Breakpoint3"
//  6   "GND"       16  "GND"

//_____________________________________________________________________________ 
AcqStage1::AcqStage1()
{
/*	BoardId=1;
	XYStepsRev=10000; ZStepsRev=10000;
	XYLinesRev=5000; ZLinesRev=10000;
	XYEncoderToMicrons=.1; ZEncoderToMicrons=.05;
	XYMicronsToSteps=XYStepsRev/XYLinesRev/XYEncoderToMicrons;
	ZMicronsToSteps=ZStepsRev/ZLinesRev/ZEncoderToMicrons;
	XYAccel=100000.000000; ZAccel=100000.000000;
	XYMaxSpeed=25000.000000; ZMaxSpeed=30000.000000;
	LimPol=0;
	HomePol=0;
	CtlModeIsCWCCW=0;
	TurnOffLightTime=60;
	InvertX=1; InvertY=0; InvertZ=1;
	ZBaseThickness=130;
	ZParkDisplacement=2000;
	ZHomingSpeed=10000;
*/	
	BoardId=1; 	
	ZHomingXPos=-78000; ZHomingYPos=5000;
	X=new AcqAxis(1); X->BoardId=1;
	Y=new AcqAxis(2); Y->BoardId=1;
	Z=new AcqAxis(3); Z->BoardId=1;
	L=new AcqLight(); L->BoardId=1;
	X->StepsRev=10000; Y->StepsRev=10000; Z->StepsRev=10000;
	X->LinesRev=5000;  Y->LinesRev=5000;  Z->LinesRev=10000;
	X->EncoderToMicrons=.1; Y->EncoderToMicrons=.1; Z->EncoderToMicrons=.05;
	X->MicronsToSteps=X->StepsRev/X->LinesRev/X->EncoderToMicrons;
	Y->MicronsToSteps=Y->StepsRev/Y->LinesRev/Y->EncoderToMicrons;
	Z->MicronsToSteps=Z->StepsRev/Z->LinesRev/Z->EncoderToMicrons;
	X->Accel=100000.000000; Y->Accel=100000.000000; Z->Accel=100000.000000;
	X->MaxSpeed=25000.000000; Y->MaxSpeed=25000.000000; Z->MaxSpeed=30000.000000;
	X->LimPol=0;Y->LimPol=0;Z->LimPol=0;
	X->HomePol=0;Y->HomePol=0;Z->HomePol=0;
	X->CtlModeIsCWCCW=0;Y->CtlModeIsCWCCW=0;Z->CtlModeIsCWCCW=0;
	X->Invert=1; Y->Invert=0; Z->Invert=1;
	X->HomingSpeed=X->MaxSpeed;
	Y->HomingSpeed=Y->MaxSpeed;
	Z->HomingSpeed=10000;
    
	L->TurnOffLightTime=60;
	L->LightLevel=28600;
	EmTopLayer=30;
	EmBotLayer=30;
	NominalEmLayer=30;
	NominalEmBase=130;
	EmBase=130;
	Z->Reference=317;
	//Z->Reference = 267;
	X->Reference=-122000;
	Y->Reference=33793;
	Z->NominalReference=317;
	//Z->NominalReference = 267;
	X->NominalReference=-122000;
	Y->NominalReference=33793;
	eAffine=NULL;
	AbsCoords=false;
	
	gStage=this;
}
//_____________________________________________________________________________ 
bool  AcqStage1::Initialize()
{
//Initialises the board with default OPERA stage parameters
	try
	{
		if (flex_clear_pu_status(BoardId) != NIMC_noError) throw(3);
	}
		catch (...)
	{
		return false;
		};
	if(!X->Initialize()) return false;
	if(!Y->Initialize()) return false;
	if(!Z->Initialize()) return false;
	if(!L->Initialize()) return false;
	X->MicronsToSteps=X->StepsRev/X->LinesRev/X->EncoderToMicrons;
	Y->MicronsToSteps=Y->StepsRev/Y->LinesRev/Y->EncoderToMicrons;
	Z->MicronsToSteps=Z->StepsRev/Z->LinesRev/Z->EncoderToMicrons;
	Dump();
	return true;

}


//_____________________________________________________________________________ 
void  AcqStage1::Stop()
{
	//Stops all three axes
	X->Stop();
	Y->Stop();
	Z->Stop();
}

//_____________________________________________________________________________ 
  int AcqStage1::GetLastError(char * Description, int size)
  {
	  //Reads an error from error stack, returns 1 if there was an error, 0 if not.
      u32 sz;
	  i32 code;
		u16 command;
		u16 resource;
		sz=(u32)size;
		flex_get_last_error(BoardId,&command,&resource,&code);
		flex_get_error_description(NIMC_COMBINED_DESCRIPTION,code,command,resource,Description, &sz);
		flex_flush_rdb(BoardId);
        return code;
  }
//_____________________________________________________________________________ 
  int AcqStage1::GetLastModalError(char * Description, int size)
  {
	  //Reads an error from modal error stack, returns 1 if there was an error, 0 if not.
      u32 sz;
	  i32 code;
		u16 command;
		u16 resource;
		sz=(u32)size;
		flex_read_error_msg_rtn(BoardId,&command,&resource,&code);
		flex_flush_rdb(BoardId);
		flex_get_error_description(NIMC_COMBINED_DESCRIPTION,code,command,resource,Description, &sz);
		flex_flush_rdb(BoardId);
        return code;
  }
//_____________________________________________________________________________ 
  void AcqStage1::ScanToZ0AndGotoXYZ(float Z0, float Z0Speed, float Z0Acc, float X0, float Y0,  float XYSpeed, float XYAcc, float Z1,  float ZSpeed, float ZAcc, u32 Timeout)
  {
	  // stores the program #2 on NI7344 to go to Z0, then when reached, goto X,Y,Z
	  // and starts the program
	u16 MComplete;
	long ActPos;
  	long retval;	
    int ActSpeed;
    int ActAccel;
    flex_set_op_mode(BoardId, 1, NIMC_ABSOLUTE_POSITION);
    flex_set_op_mode(BoardId, 2, NIMC_ABSOLUTE_POSITION);
    flex_set_op_mode(BoardId, 3, NIMC_ABSOLUTE_POSITION);
	  flex_begin_store(BoardId,2);
	// Calc Z0 params
        ActPos = Z0 * Z->MicronsToSteps;
		if (Z->Invert) ActPos = -ActPos;
        if (Z0Speed > Z->MaxSpeed) Z0Speed = Z->MaxSpeed;
        if (Z0Acc > Z->Accel) Z0Acc = Z->Accel;
        ActSpeed = abs(Z0Speed * Z->MicronsToSteps);
        ActAccel = abs(Z0Acc * Z->MicronsToSteps);
   // Z0 params ready

	  flex_load_target_pos(BoardId, 3, ActPos, 0xFF);
      flex_load_velocity(BoardId, 3, ActSpeed, 0xFF);
	  flex_load_acceleration(BoardId, 3, NIMC_ACCELERATION, ActAccel, 0xFF);
	  flex_load_acceleration(BoardId, 3, NIMC_DECELERATION, ActAccel, 0xFF);
	// Calc X params
        ActPos = X0 * X->MicronsToSteps;
		if (X->Invert) ActPos = -ActPos;
        if (XYSpeed > X->MaxSpeed) XYSpeed = X->MaxSpeed;
        if (XYAcc > X->Accel) XYAcc = X->Accel;
        ActSpeed = abs(XYSpeed * X->MicronsToSteps);
        ActAccel = abs(XYAcc * X->MicronsToSteps);
   // X params ready
	  flex_load_target_pos(BoardId, 1, ActPos, 0xFF);
      flex_load_velocity(BoardId, 1, ActSpeed, 0xFF);
	  flex_load_acceleration(BoardId, 1, NIMC_ACCELERATION, ActAccel, 0xFF);
	  flex_load_acceleration(BoardId, 1, NIMC_DECELERATION, ActAccel, 0xFF);
	// Calc Y params
        ActPos = Y0 * Y->MicronsToSteps;
		if (Y->Invert) ActPos = -ActPos;
        if (XYSpeed > Y->MaxSpeed) XYSpeed = Y->MaxSpeed;
        if (XYAcc > Y->Accel) XYAcc = Y->Accel;
        ActSpeed = abs(XYSpeed * Y->MicronsToSteps);
        ActAccel = abs(XYAcc * Y->MicronsToSteps);
   // Y params ready
	  flex_load_target_pos(BoardId, 2, ActPos, 0xFF);
      flex_load_velocity(BoardId, 2, ActSpeed, 0xFF);
	  flex_load_acceleration(BoardId, 2, NIMC_ACCELERATION, ActAccel, 0xFF);
	  flex_load_acceleration(BoardId, 2, NIMC_DECELERATION, ActAccel, 0xFF);
//Now move to Z0
	  flex_start(BoardId, 3, 0);
	  //and wait till complete
	  flex_wait_on_condition(BoardId, 0,NIMC_WAIT,NIMC_CONDITION_MOVE_COMPLETE,0x08,0x00,NIMC_MATCH_ALL,Timeout, 0);
// Now move to XY
	  flex_start(BoardId, 1, 0);
	  flex_start(BoardId, 2, 0);
	// Calc Z params
        ActPos = Z1 * Z->MicronsToSteps;
		if (Z->Invert) ActPos = -ActPos;
        if (ZSpeed > Z->MaxSpeed) ZSpeed = Z->MaxSpeed;
        if (ZAcc > Z->Accel) ZAcc = Z->Accel;
        ActSpeed = abs(ZSpeed * Z->MicronsToSteps);
        ActAccel = abs(ZAcc * Z->MicronsToSteps);
   // Z params ready
	  flex_load_target_pos(BoardId, 3, ActPos, 0xFF);
      flex_load_velocity(BoardId, 3, ActSpeed, 0xFF);
	  flex_load_acceleration(BoardId, 3, NIMC_ACCELERATION, ActAccel, 0xFF);
	  flex_load_acceleration(BoardId, 3, NIMC_DECELERATION, ActAccel, 0xFF);
//Now move to Z
	  flex_start(BoardId, 3, 0);
	 // and wait for complete on all 3 axes
//	  flex_wait_on_condition(BoardId, 0,NIMC_WAIT,NIMC_CONDITION_MOVE_COMPLETE,0x02+0x04+0x08,0x00,NIMC_MATCH_ALL,Timeout, 0);
	  flex_end_store(BoardId,2);
	  flex_run_prog(BoardId,2);
      
  }


//_____________________________________________________________________________ 
void  AcqStage1::Monitor()
{
    EdbMark *xyz=new EdbMark();
	char hit=' ';
	float SpeedFactor=100; //defines speed with respect to MAxSpeed
	bool MustStop=false;
	int XMoving=0;
	int YMoving=0;
	int ZMoving=0;
	printf("Stage Monitor started. for key help press 'h'.\n");
 //   TCanvas * c=new TCanvas();
	while(!MustStop){
//	printf("returned %d\n",num);
		while(!kbhit()) { 
				xyz->SetX(X->GetPos());
				xyz->SetY(Y->GetPos());
				xyz->SetZ(Z->GetPos()); 
			if(eAffine && AbsCoords) xyz->Transform(eAffine);
			for(int ch=0;ch<80;ch++) printf("\b");
			if(eAffine && AbsCoords) printf("A"); else printf("S");
			if(X->GetLimiter()==1) printf("L");
			if(X->GetLimiter()==2) printf("H");
			printf("X:%9.1f   ",xyz->X());
			if(eAffine && AbsCoords) printf("A"); else printf("S");
			if(Y->GetLimiter()==1) printf("L");
			if(Y->GetLimiter()==2) printf("H");
			printf("Y:%9.1f   ",xyz->Y());
			if(Z->GetLimiter()==1) printf("L");
			if(Z->GetLimiter()==2) printf("H");
			printf("Z:%9.1f   Speed: max/%d   Light: %d  ",Z->GetPos(),int(SpeedFactor),L->LightLevel);
			fflush(stdout);
			gSystem->Sleep(20);
		}
	hit=getch(); //printf("%d\n",hit); 
	if(kbhit())
	{hit=getch();// printf("%d\n",hit);
	}
	switch(hit)
	{
	case 'h' :
		printf("\n\nX-Y movement initiated by arrows\nSPACE to stop all axes\n");
		printf("+ Speed Up\n- Speed Down\n");
		printf("1 go to the bottom of emulsion Bottom layer\n");
		printf("2 go to the top of emulsion Bottom layer\n");
		printf("3 go to the bottom of emulsion Top layer\n");
		printf("4 go to the top of emulsion Top layer\n");
		printf("R go to the reference corner on X,Y\n");
		printf("x and z to uncrease and decrease light\n");
		printf("ESC or q to exit monitor\n");
		printf("Limiter active - 'H' or 'L' displayed at the corresponding axis letter\n\n ");
		break;
	case '=': SpeedFactor/=5; if(SpeedFactor<1) SpeedFactor=1;
				if(XMoving!=0) X->Move(XMoving*X->MaxSpeed/SpeedFactor,1e6);
				if(YMoving!=0) Y->Move(YMoving*Y->MaxSpeed/SpeedFactor,1e6);
				if(ZMoving!=0) Z->Move(ZMoving*Z->MaxSpeed/SpeedFactor,1e6);
	//	printf("\nSpeed set to Max/%f\n",SpeedFactor);
				break;
	case '-': SpeedFactor*=5; if(SpeedFactor>10000) SpeedFactor=10000;
	//	printf("\nSpeed set to Max/%f\n",SpeedFactor);
				if(XMoving!=0) X->Move(XMoving*X->MaxSpeed/SpeedFactor,1e6);
				if(YMoving!=0) Y->Move(YMoving*Y->MaxSpeed/SpeedFactor,1e6);
				if(ZMoving!=0) Z->Move(ZMoving*Z->MaxSpeed/SpeedFactor,1e6);
				break;
	case 75 : X->Move(-X->MaxSpeed/SpeedFactor,1e6);
		        XMoving=-1;
				break;
	case 77 : X->Move(X->MaxSpeed/SpeedFactor,1e6);
		        XMoving=1;
				break;
	case 72 : Y->Move(Y->MaxSpeed/SpeedFactor,1e6);
		        YMoving=1;
				break;
	case 80 : Y->Move(-Y->MaxSpeed/SpeedFactor,1e6);
		        YMoving=-1;
				break;
	case 73 : Z->Move(Z->MaxSpeed/SpeedFactor,1e6);
		        ZMoving=1;
				break;
	case 81 : Z->Move(-Z->MaxSpeed/SpeedFactor,1e6);
		        ZMoving=-1;
				break;
	case '1' : Z->PosMove(Z->Reference,Z->MaxSpeed/SpeedFactor,1e6);
				Z->WaitForMoveComplete(1e4);
				break;
	case '2' : Z->PosMove(Z->Reference+EmBotLayer,Z->MaxSpeed/SpeedFactor,1e6);
				Z->WaitForMoveComplete(1e4);
				break;
	case '3' : Z->PosMove(Z->Reference+EmBotLayer+EmBase,Z->MaxSpeed/SpeedFactor,1e6);
				Z->WaitForMoveComplete(1e4);
				break;
	case '4' : Z->PosMove(Z->Reference+EmBotLayer+EmTopLayer+EmBase,Z->MaxSpeed/SpeedFactor,1e6);
				Z->WaitForMoveComplete(1e4);
				break;
	case 'r' :
	case 'R' : X->PosMove(X->Reference,1e9,1e9); Y->PosMove(Y->Reference,1e9,1e9);
				X->WaitForMoveComplete(1e4); Y->WaitForMoveComplete(1e4);
				break;
	case 'x' : (L->LightLevel)+=10;L->ON();
				break;
	case 'z' : (L->LightLevel)-=10;L->ON();
				break;
	case 'a' :  AbsCoords=true;
				break;
	case 's' : AbsCoords=false;
				break;
	case 76 : 
	case ' ': 
	case 'q': 
	case 27 : Stop();
				XMoving=0; YMoving=0;ZMoving=0;
//	case 72 : {}
	}
	if(hit==27 || hit=='q') MustStop=true;
	}
    for(int ch=0;ch<40;ch++) printf("\b");
	printf("\n");
	fflush(stdout);
	delete xyz;
	printf("Monitor stoped. \n");

//	c->~TCanvas();

}



//_____________________________________________________________________________ 
AcqAxis::AcqAxis()
{
	BoardId=1;
	StepsRev=10000;
	LinesRev=10000;
	EncoderToMicrons=.05;
	MicronsToSteps=StepsRev/LinesRev/EncoderToMicrons;
	Accel=100000.000000; 
	MaxSpeed=25000.000000; 
	LimPol=0;
	HomePol=0;
	CtlModeIsCWCCW=0;
	Invert=1; 
	HomingSpeed=10000;
	Reference=0;

}
//_____________________________________________________________________________ 
AcqAxis::AcqAxis(int Ax)
{
	Axis=Ax;
}


//_____________________________________________________________________________ 
bool  AcqAxis::Initialize()
{
//Initialises the axis
	try
	{

		if (flex_enable_axes( BoardId, 0, 2, 0x00) != NIMC_noError) throw(3);
		if (flex_config_axis( BoardId, Axis, 0x20+Axis, 0, 0x40+Axis, 0) != NIMC_noError) throw(3);
		if (flex_set_stepper_loop_mode( BoardId, Axis, 1) != NIMC_noError) throw(3);
		if (flex_config_step_mode_pol( BoardId, Axis, 0x04 | ((! CtlModeIsCWCCW) ? 1 : 0)) != NIMC_noError) throw(3);
		if (flex_load_counts_steps_rev( BoardId, Axis, NIMC_COUNTS,  LinesRev) != NIMC_noError) throw(3);
		if (flex_load_counts_steps_rev( BoardId, Axis, NIMC_STEPS,  StepsRev) != NIMC_noError) throw(3);
		if (flex_load_follow_err( BoardId, Axis, 0, 0xFF) != NIMC_noError) throw(3);
		if (flex_enable_home_inputs( BoardId, 0x0E) != NIMC_noError) throw(3);
		if (flex_set_home_polarity( BoardId, ( HomePol) ? 0x0E : 0) != NIMC_noError) throw(3);
		if (flex_set_limit_polarity( BoardId, ( LimPol) ? 0x0E : 0, ( LimPol) ? 0x0E : 0) != NIMC_noError) throw(3);
		if (flex_enable_limits( BoardId, NIMC_LIMIT_INPUTS, 0x0E, 0x0E) != NIMC_noError) throw(3);
		if (flex_set_op_mode( BoardId, Axis, NIMC_ABSOLUTE_POSITION) != NIMC_noError) throw(3);

	    if (flex_load_acceleration( BoardId, Axis, NIMC_ACCELERATION, abs( Accel *  MicronsToSteps), 0xFF) != NIMC_noError) throw(3);
		if (flex_load_acceleration( BoardId, Axis, NIMC_DECELERATION, abs( Accel *  MicronsToSteps), 0xFF) != NIMC_noError) throw(3);
//       if (flex_configure_breakpoint( BoardId, Axis, NIMC_ABSOLUTE_BREAKPOINT, NIMC_SET_BREAKPOINT,0) != NIMC_noError) throw(3);
       if (flex_configure_breakpoint( BoardId, Axis, NIMC_ABSOLUTE_BREAKPOINT, NIMC_SET_BREAKPOINT,NIMC_OPERATION_SINGLE) != NIMC_noError) throw(3);
		if (flex_enable_axes( BoardId, 0, 2, 0x0E) != NIMC_noError) throw(3);
	}
		catch (...)
	{
		return false;
	};
		SetFilterFrequency(0); //Must be set to maximum 25.6MHz to avoid missing lines at high speed
		printf("Axis %d initialized siccessfully with parameters:\n",Axis);
		Dump();
	return true;

}



//_____________________________________________________________________________ 
void AcqAxis::Stop()
{
//Stops a given axis
	flex_stop_motion(BoardId, Axis, NIMC_HALT_STOP, 0);
}
//_____________________________________________________________________________ 
void AcqAxis::Reset()
{
//Resets a given axis
	flex_reset_pos(BoardId, Axis, 0, 0, 0xFF);
}
//_____________________________________________________________________________ 
int AcqAxis::WaitForMoveComplete(u32 Timeout)
{
//Waits for a movement complete on a given axis ar timeout [ms], 
//returns 1 if movement complete, 0 if timeout happened
	u16 MComplete;
	flex_wait_for_move_complete(BoardId, Axis,0,Timeout,10, &MComplete);
return MComplete;
}

//_____________________________________________________________________________ 
void AcqAxis::PosMove(float Pos, float Speed, float Acc)
{
//Moves stage axis to position [mic] with given speed [mic/s] and acceleration [mic/s2]
	long ActPos;
  	long retval;	
    int ActSpeed;
    int ActAccel;
    flex_set_op_mode(BoardId, Axis, NIMC_ABSOLUTE_POSITION);
        ActPos = Pos * MicronsToSteps;
		if (Invert) ActPos = -ActPos;
        if (Speed > MaxSpeed) Speed = MaxSpeed;
        if (Acc > Accel) Acc = Accel;
        ActSpeed = abs(Speed * MicronsToSteps);
        ActAccel = abs(Acc * MicronsToSteps);
	flex_read_pos_rtn(BoardId, Axis, &retval);
//	flex_flush_rdb(BoardId);
	if(retval==ActPos) return; //Already at position
	flex_load_target_pos(BoardId, Axis, ActPos, 0xFF);
    flex_load_velocity(BoardId, Axis, ActSpeed, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_ACCELERATION, ActAccel, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_DECELERATION, ActAccel, 0xFF);
	flex_start(BoardId, Axis, 0);

}//_____________________________________________________________________________ 
void AcqAxis::Move(float Speed, float Acc)
{
//Moves stage axis infinitely with given speed [mic/s] and acceleration [mic/s2]
  	long retval;	
    int ActSpeed;
    int ActAccel;
    flex_set_op_mode(BoardId, Axis, NIMC_VELOCITY);
        if (Speed > MaxSpeed) Speed = MaxSpeed;
        if (Acc > Accel) Acc = Accel;
        ActSpeed = Speed * MicronsToSteps;
		if(Invert) ActSpeed=-ActSpeed;
        ActAccel = Acc * MicronsToSteps;
    flex_load_velocity(BoardId, Axis, ActSpeed, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_ACCELERATION, ActAccel, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_DECELERATION, ActAccel, 0xFF);
	flex_start(BoardId, Axis, 0);

}
//_____________________________________________________________________________ 
void AcqAxis::LoadMoveCompleteCriteria( bool MotorOff, bool Stop, bool Delay, bool InPosition, float DeadBand, int sdelay, int minPulse)
{
// Defines conditions under which the Move Complete flag is set:
// Motor Off - says for itself
// Stop - Run/Stop flag must be Stopped
// Delay - MoveComplete flag is set after a given sdelay [ms]
// InPosition - when stage is within DeadBand [mic] from the target position.
//
// sdelay - settling time delay [ms]
// minPulse [ms] - minimum time for which MoveComplete status must be true
// even if the axis re-started, the flag stays for this time anyway
	int criteria=1;
	if(MotorOff) criteria+=2;
	if(Stop) criteria+=4;
	if(Delay) criteria+=8;
	if(InPosition) criteria+=16;
	int DBand;
    DBand=DeadBand/EncoderToMicrons;
//	printf("flex_config_mc_criteria(%d,%d,%d,%d,%d,%d)\n",BoardId, Axis, criteria, DBand, Delay, minPulse);
	flex_config_mc_criteria(BoardId, Axis, criteria, DBand, sdelay, minPulse);

}
//_____________________________________________________________________________ 
bool AcqAxis::IsMoveComplete()
{
// chacks if Move Complete Flag is set
	u16 mc;
	flex_check_move_complete_status(BoardId,Axis,0,&mc);
	if(mc==1) return true;
	else return false;
}
//_____________________________________________________________________________ 
float  AcqAxis::GetPos()
{
// returns position in [mic] of a given axis
	long retval;	
	Double_t fretval;
	flex_read_pos_rtn(BoardId, Axis, &retval);
//	flex_flush_rdb(BoardId);

	if (Invert) retval = -retval;
    fretval= retval * EncoderToMicrons * LinesRev / StepsRev;
	return fretval;
;
}
//_____________________________________________________________________________ 
int AcqAxis::GetLimiter()
{
//returns status of limiters on a given axis: 
// 0 - no limiters reached
// 1 - "-" limiter reached 
// 2- "+" limiter reached
	unsigned char fwdStatus, revStatus;
	flex_read_limit_status_rtn(BoardId, NIMC_LIMIT_INPUTS, &fwdStatus, &revStatus);
	//	flex_flush_rdb(BoardId);

	DWORD a = 0;
	bool invert = Invert;
	if (revStatus & (0x02 << (Axis-1))) a |= invert ? 2 : 1;
	if (fwdStatus & (0x02 << (Axis-1))) a |= invert ? 1 : 2;
	return a;
}

//_____________________________________________________________________________ 
int AcqAxis::FindHome(int Direction)
{
// Finds home switch using the NI7344 onboard sequence 
// the initial seacrh is performed with XY(Z)MaxSpeed 
// in a given Direction (+1 or -1);
// the final approach is done with the fixed speed of 1/4 RPS to provide good
// accuracy.
// returns immidiately 1 if the sequence started, and 0 if parameters are wrong.
// To wait for the end use WaitForHome() function
	int ActSpeed;
    int ActAccel;
    flex_set_op_mode(BoardId, Axis, NIMC_ABSOLUTE_POSITION);
        ActSpeed = abs(HomingSpeed * MicronsToSteps);
        ActAccel = abs(Accel * MicronsToSteps);
    flex_load_velocity(BoardId, Axis, ActSpeed, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_ACCELERATION, ActAccel, 0xFF);
	flex_load_acceleration(BoardId, Axis, NIMC_DECELERATION, ActAccel, 0xFF);
/// here the homing sequence itself starts
	int DirMap;
	if(Direction>0) DirMap=0x0004;
	else DirMap=0x0003;
	flex_find_home(BoardId, Axis, DirMap);
//	flex_find_reference(BoardId, Axis, 0, NIMC_FIND_HOME_REFERENCE);//Flex6.1.2
	printf("Home Finding is started on axis %d..\n",Axis);
	 return 1;
}

  int AcqAxis::WaitForHome( int Timeout)
  {
	  int res;
	  printf("Waiting Home Finding Complete on axis %d..\n",Axis);
	  res=WaitForMoveComplete(Timeout);
	  gSystem->Sleep(100);
	  res=WaitForMoveComplete(Timeout);
	  gSystem->Sleep(100);
	  res=WaitForMoveComplete(Timeout);
	  if(res!=1) return 0;
	  u16 AxisStatus;
	  flex_read_axis_status_rtn(BoardId, Axis, &AxisStatus);
	 // 	flex_flush_rdb(BoardId);

	  if(AxisStatus & 0x0400) {
		  printf("Home Finding Complete on axis %d..\n",Axis);
		  return 1;
	  }
	  else {
		  printf("*** Home Finding Failed on axis %d..\n",Axis);
		  return 0;
	  }
 //Flex5.x
/*	  	  u16 found, finding;
	  flex_check_reference(BoardId, Axis, 0, &found, &finding);
	  while(finding) flex_check_reference(BoardId, Axis, 0, &found, &finding);
	  if(found) {
		  printf("Home Finding Complete on axis %d..\n",Axis);
		  return 1;
	  }
	  else {
		  printf("*** Home Finding Failed on axis %d..\n",Axis);
		  return 0;
	  }
*/ //Flex6.1.2
  }


  void AcqAxis::ArmBreakpoint(float Pos)
  {
//Sets breakpoint at position [mic] on an axis
	long ActPos;
     ActPos = Pos / EncoderToMicrons ;
		if (Invert) ActPos = -ActPos;
 	  flex_load_pos_bp(BoardId,0x20+Axis,ActPos,0xFF);
	  flex_enable_breakpoint(BoardId,0x20+Axis,0x01);
  } 
  void AcqAxis::DisarmBreakpoint()
  {
	  flex_enable_breakpoint(BoardId,Axis,0x00);
  } 

  bool AcqAxis::IsBreakpointOccured()
  {
	  u16 status;
	  flex_read_breakpoint_status_rtn(BoardId,0x20,0,&status);
	//  	flex_flush_rdb(BoardId);

	  printf("Breakpoint Status: %d\n",status);
      if(status & (0x01 << Axis))
	  return true;
	  else return false;
  }

 void AcqAxis::LoadBreakpointSequence(float *Pos, int N)
  {
//Sets breakpoint sequence and store the BP grab program.
	  u16 mask;
	  int label1=1;
	  int label2=2;
	  if(N>100) N=100;
	  if(N<=0) return;
	i32 ActPos[120];
	i32 NPos=N;
	flex_load_var(BoardId,NPos,0x76); //store number of breakpoints in Var76 (save)
	flex_load_var(BoardId,NPos,0x78); //store number of breakpoints in Var78 (index)
	flex_load_var(BoardId,1,0x77); //store decrement=1 in Var77
	for(u8 i=0;i<N;i++)
	 {
      ActPos[i] = Pos[i] / EncoderToMicrons ;
		if (Invert) ActPos[i] = -ActPos[i];
		flex_load_var(BoardId,ActPos[i],i+1); //store breakpoints in Vars from 0x01 up
	 }
     mask=(0x01 << Axis);
	  flex_begin_store(BoardId,1);
//	  flex_load_program_time_slice(BoardId,1,20,0xFF);
	  // Start the loop, index is in var 0x01 decrementing down to 0
 	  flex_insert_program_label(BoardId,label1);
	  flex_load_pos_bp(BoardId,0x20+Axis,0,0xF8); //load indirect from the index Var78
	  flex_enable_breakpoint(BoardId,0x20+Axis,0x01); //arm breakpoint
 	  flex_insert_program_label(BoardId,label2); //loop till bp is occured
      flex_jump_label_on_condition(BoardId,0x20+Axis,NIMC_CONDITION_POSITION_BREAKPOINT,0,mask,NIMC_MATCH_ALL,label2);
 //     flex_wait_on_event(BoardId,0,NIMC_WAIT,NIMC_CONDITION_POSITION_BREAKPOINT,(u8)(1<<Axis),0,NIMC_MATCH_ALL,65535,0);//FlexMotion6.1.2
	  //OK, BP is occured, decrementing index Var78 by Var77=1
	  flex_sub_vars(BoardId,0x78,0x77,0x78);
	  //Now if N=0 flag EQUALTO must be set... checking
      flex_jump_label_on_condition(BoardId,0,NIMC_CONDITION_NOT_EQUAL,0,0,0,label1);
	  flex_set_breakpoint_momo(BoardId,0x20+Axis,0,mask,0xFF); //reset output
	//  flex_enable_breakpoint(BoardId,0x20+Axis,0x00); //disable breakpoint
	  flex_end_store(BoardId,1);

  } 
  void AcqAxis::DisarmBreakpointSequence()
  {
	  //stops BP grab sequence
	  flex_stop_prog(BoardId,1);
  } 
 void AcqAxis::ArmBreakpointSequence()
  {
      //refresh index Var78 from Var76, where it's saved
	  // and starts BP grab sequence
	  // Doesn't check if the program is loaded!!!! - TODO
	  flex_xor_vars(BoardId,0x78,0x78,0x78);  //set index to 0 to be sure
//	  DumpBreakpointSequence();
	  flex_add_vars(BoardId,0x78,0x76,0x78);  //set index to Var76
//	  DumpBreakpointSequence();
	  flex_run_prog(BoardId,1);
	  //gSystem->Sleep(500);
  } 

  int AcqAxis::IsBreakpointSequenceArmed()
  {
	  //Returns number of breakpoints to occur in sequence.
	  // if 0 - sequence is not active or finished
	  i32 N78;
	 flex_read_var_rtn(BoardId,0x78,&N78);
	// 	flex_flush_rdb(BoardId);

     return int(N78);
  }
 void AcqAxis::DumpBreakpointSequence()
 {
	 // Debug function
	 i32 var;
	 i32 N78,N77,N76;
	 flex_read_var_rtn(BoardId,0x78,&N78);
	 flex_read_var_rtn(BoardId,0x77,&N77);
	 flex_read_var_rtn(BoardId,0x76,&N76);
	// 	flex_flush_rdb(BoardId);

	 printf("0x76:%d 0x77:%d 0x78:%d\n",N76,N77,N78);
//	 flex_read_var_rtn(BoardId,0x78,&N78);
 }
  void AcqAxis::Test(float *Pos, int N)
  {
	  u16 mask;
	  int label1=1;
	  int label2=2;
	  if(N>100) N=100;
	  if(N<=0) return;
	i32 ActPos[120];
	i32 NPos=N;
	flex_load_var(BoardId,NPos,0x78); //store number of breakpoints in Var78
	flex_load_var(BoardId,1,0x77); //store decrement=1 in Var77
	for(u8 i=0;i<N;i++)
	 {
      ActPos[i] = Pos[i] / EncoderToMicrons ;
		if (Invert) ActPos[i] = -ActPos[i];
		flex_load_var(BoardId,ActPos[i],i+1); //store breakpoints in Vars from 0x01 up
	 }
      mask=(0x01 << Axis);

	  flex_begin_store(BoardId,1);
	  // Start the loop, index is in var 0x01 decrementing down to 0
 	  flex_insert_program_label(BoardId,label1);
	  flex_load_pos_bp(BoardId,0x20+Axis,0,0xF8); //load indirect from the index Var78
	  flex_enable_breakpoint(BoardId,0x20+Axis,0x01); //arm breakpoint
 	  flex_insert_program_label(BoardId,label2); //loop till bp is occured
      flex_jump_label_on_condition(BoardId,0x20+Axis,NIMC_CONDITION_POSITION_BREAKPOINT,mask,0,NIMC_MATCH_ALL,label1);
	  //OK, BP is occured, decrementing index Var78 by Var77=1
	  flex_sub_vars(BoardId,0x78,0x77,0x78);
	  //Now if N=0 flag EQUALTO must be set... checking
      flex_jump_label_on_condition(BoardId,0,NIMC_CONDITION_NOT_EQUAL,0,0,0,label1);
	  flex_end_store(BoardId,1);
	  flex_run_prog(BoardId,1);

  }



void AcqAxis::SetFilterFrequency(int Freq)
{
	//sets digital filter cutoff frequency for the encoder
	// F=25.6 /(2**Freq) MHz
    flex_configure_encoder_filter(BoardId,Axis,Freq);
}
int AcqAxis::ReadFollowingError()
{
	// returns current position following error
	// useful for debugging of the stage
	short retval;
	flex_read_follow_err_rtn(BoardId,Axis, &retval);
	//	flex_flush_rdb(BoardId);

   return retval;
}
void AcqAxis::SetFollowingError(u16 Error)
{
	flex_load_follow_err(BoardId,Axis,Error,0xFF); 
}
  
//_____________________________________________________________________________ 
AcqLight::AcqLight()
{
	BoardId=1;
	TurnOffLightTime=60;
	LightLevel=28600;
//	T=new TTimer(TurnOffLightTime,kFALSE);
//	TThread *Th = new TThread(Hook,0);
	IsON=false;
}
//_____________________________________________________________________________ 
bool  AcqLight::Initialize()
{
//Initialises the light controller
	try
	{

		if (flex_enable_axes( BoardId, 0, 2, 0x00) != NIMC_noError) throw(3);
		if (flex_clear_pu_status( BoardId) != NIMC_noError) throw(3);
		if (flex_enable_axes( BoardId, 0, 0, 0) != NIMC_noError) throw(3);
		if (flex_config_axis( BoardId, 4, 0, 0, 0, 0) != NIMC_noError) throw(3);
		if (flex_load_dac( BoardId, 0x34, LightLevel, 0xFF) != NIMC_noError) throw(3);
		if (flex_enable_axes( BoardId, 0, 2, 0x0E) != NIMC_noError) throw(3);
		if (flex_set_adc_range( BoardId, 0x54, NIMC_ADC_BIPOLAR_5) != NIMC_noError) throw(3); //+-5V -> +-2047
	}
		catch (...)
	{
		return false;
		};
//	T->Connect("Timeout()","AcqLight",this,"OFF()");

    ON();
	return true;

}
//_____________________________________________________________________________ 
void  AcqLight::ON()
{
		flex_load_dac(BoardId, 0x34, LightLevel, 0xFF);
			IsON=true;
     //   gSystem->Sleep(1000);
		ResetTimer();
}
//_____________________________________________________________________________ 
void  AcqLight::OFF()
{
		flex_load_dac(BoardId, 0x34, 0, 0xFF);
			IsON=false;

}
//_____________________________________________________________________________ 
void  AcqLight::ResetTimer()
{
//Resets lamp timeout
//	T->Reset();
//	T->Start(TurnOffLightTime*1000,kTRUE);
}
//_____________________________________________________________________________ 
int AcqLight::ReadTemperature()
{
	// Reads ADC channel 4 normally connected to termometer
	i16 ADC;
	flex_read_adc_rtn(BoardId, 0x54, &ADC);
	return ADC;
}

//_____________________________________________________________________________ 
int AcqLight::ReadLightLevel()
{
	// Reads ADC channel 4 normally connected to termometer
	i16 ADC;
	flex_read_dac_rtn(BoardId, 0x34, &ADC);
	LightLevel=ADC;
	return ADC;
}
