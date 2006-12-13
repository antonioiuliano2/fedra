//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqPlateChanger                                                      //
//                                                                      //
// 	Interface to Bern emulsion plate changer			        		//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "AcqPlateChanger.h"
#include "TROOT.h"
#include "TSystem.h"
#include "nidaqmx.h"
	TPlate* EmulsionOnTable;
    TBank Bank[5];
	TaskHandle writehandle;
	TaskHandle readhandle;
	TaskHandle writehandlez;
	TaskHandle readhandlez;


ClassImp(AcqPlateChanger);

//_____________________________________________________________________________ 
AcqPlateChanger::AcqPlateChanger()
{
		InitDone=false;
        Ready=false;
		PositionTimeout=50;
		NumberOfBanks=3;
		VacuumTimeout=10;
		LiftCorners=false;
		NumberOfAttempt=20;
		SeparationAttempts=2;
		ST=NULL;
		XLoadPosition=-84000; 
	    YLoadPosition=5000;
	    ZLoadPosition=5000;
		SourceBank=1;
	     DestinationBank=2;
		 StageError=false;



}
//_____________________________________________________________________________ 
void AcqPlateChanger::Print()
{
	// prints the content of emulsion banks
		for(int bnk=0;bnk<NumberOfBanks+1;bnk++) 
		{
			if(bnk==0) printf("\nTable:",Bank[bnk].Number);
			else printf("\nBank #%d content:",Bank[bnk].Number);
			if(Bank[bnk].NEmulsions==0 && Bank[bnk].NSeparators==0) printf(" Empty.\n\n");
			else printf("\n\n");

			for(int i=0;i<Bank[bnk].NEmulsions+Bank[bnk].NSeparators;i++)  
			{
                if(Bank[bnk].Plates[i]->IsSeparator) printf("---Separator---\n");
	            else printf("Emulsion %d %d %d %d\n",Bank[bnk].Plates[i]->id.Part[0],
					Bank[bnk].Plates[i]->id.Part[1],
					Bank[bnk].Plates[i]->id.Part[2],
					Bank[bnk].Plates[i]->id.Part[3]);
			}
		}


}
//_____________________________________________________________________________ 

AcqPlateChanger::~AcqPlateChanger()
{
	OnReset();
}


void AcqPlateChanger::OnReset() 
{
	// resets the power on platechanger arm, turns off the vacuums
		uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
		int32 read;
		//Writing to P5
		DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandle, 10.0);
		DAQmxStopTask (writehandle);
		//Writing to P4
		DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandlez, 10.0);
		DAQmxStopTask (writehandlez);
		InitDone=false;
        Ready=false;
}

int AcqPlateChanger::Initialize(AcqStage1* Stage)
{
	// initializes the arm drives, vacuum generators,
	// moves the arm to position 1 up,
	// reads the status file and sets flags InitDone, Ready
    ST=Stage;
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
	printf("Initializing Plate changer..\n");

	DAQmxSetDigitalPowerUpStates ("Dev1", "Dev1/port0/line0:7", DAQmx_Val_Low, NULL);
	DAQmxSetDigitalPowerUpStates ("Dev1", "Dev1/port1/line0:7", DAQmx_Val_Low, NULL);

	readhandle = 0;
	readhandlez = 0;
	writehandle = 0;
	writehandlez = 0;
	//Initialisation of reading X coord.
//	printf("readhandle "); fflush(stdout);
	DAQmxCreateTask ("read", &readhandle);
	DAQmxCreateDIChan (readhandle, "Dev1/port1/line0:7", "", DAQmx_Val_ChanForAllLines);
	//Initialisation of reading Z coord.
//	printf("readhandlez "); fflush(stdout);
	DAQmxCreateTask ("readz", &readhandlez);
	DAQmxCreateDIChan (readhandlez, "Dev1/port0/line0:7", "", DAQmx_Val_ChanForAllLines);
	//Initialisation of writing X coord.
//	printf("writehandle "); fflush(stdout);
	DAQmxCreateTask ("write", &writehandle);
	DAQmxCreateDOChan (writehandle, "Dev1/port5/line0:7", "", DAQmx_Val_ChanForAllLines);
	//Initialisation of writing Z coord.
//	printf("writehandlez "); fflush(stdout);
	DAQmxCreateTask ("writez", &writehandlez);
	DAQmxCreateDOChan (writehandlez, "Dev1/port4/line0:7", "", DAQmx_Val_ChanForAllLines);
	//printf("ok. \n"); fflush(stdout);
	InitDone=false;
    ResetBanks();

printf("Resetting ports.. "); fflush(stdout);
	//reset ports 4 & 5
	//Writing to P5
		DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandle, 10.0);
		DAQmxStopTask (writehandle);
		//Writing to P4
		DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandlez, 10.0);
		DAQmxStopTask (writehandlez);

/*	//checking 24 V
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
		if (datain[7] == 0)//checking 24 V
	{
		printf("Please check 24V on drive\n");
		return;
	}
	*/
	//Set AH on both drives
		printf("Setting AH.. "); fflush(stdout);
	dataout[0]=1;
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	gSystem->Sleep(20);
	//Check RDY signal
	printf("Checking H RDY.."); fflush(stdout);
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	if(datain[0]==0) 
	{
		printf("Vertical drive controller error!\n");
		OnReset();
		return 0;
	}
	printf("OK..Checking H RDY.."); fflush(stdout);

	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandle);
	if(datain[0]==0) 
	{
		printf("Horizontal drive controller error!\n");
		OnReset();
		return 0;
	}
	printf("OK.."); fflush(stdout);

	gSystem->Sleep(500);
	printf("Arm up.."); fflush(stdout);

	if(GoOnVAxis(VPOS_UP,PositionTimeout )<0)
	{
		printf("Vertical drive position is not reached!\n");
		OnReset();
		return 0;
	}
	printf("Arm to pos 2.."); fflush(stdout);
	if(GoOnHAxis(2,PositionTimeout)<0)
	{
		printf("Horizontal drive position is not reached!\n");
		OnReset();
		return 0;
	}
	InitDone=true;
    Ready=false;
	printf("Reading status file.."); fflush(stdout);

    ReadStatusFile();

   if(EmulsionOnTable) TableVacuumControl(true,1);
	printf("Done!"); fflush(stdout);
	Dump();
	PowerOFF();
	return 1;
}


int AcqPlateChanger::ArmVacuumControl(bool VacOn, int timeout)
{
	// Switches the vacuum on/off at arm holder, if vacuum is not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach vacuum
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
	//Read whatever is set on Output Port X
	DAQmxReadDigitalLines (writehandle, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	// Toggle Vacuum valve bit (Bit7 on X )
	if(VacOn) dataout[7]=1; else dataout[7]=0;
	//Write back to port
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
    gSystem->Sleep(100);    
    //Wait for vacuum sensor (Bit 7)
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	while(datain[7]==0 && tme<timeout)
	{
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
      gSystem->Sleep(100);tme++;
	}
	if(tme>=timeout)
    {
		return -tme;
	}
	return tme;


}

int AcqPlateChanger::TableVacuumControl(bool VacOn, int timeout)
{
	// Switches the vacuum on/off at the table, if vacuum is not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach vacuum
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
	//Read whatever is set on Output Port Z
	DAQmxReadDigitalLines (writehandlez, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	// Toggle Vacuum valve bit (Bit7 on Z )
	if(VacOn) dataout[7]=1; else dataout[7]=0;
	//Write back to port
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
    gSystem->Sleep(100);    
    //Wait for vacuum sensor (Bit 7)
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
	while(datain[7]==0 && tme<timeout)
	{
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
      gSystem->Sleep(100);tme++;
	}
	if(tme>=timeout)
    {
		return -tme;
	}
	return tme;


}

int  AcqPlateChanger::GoOnHAxis(int position, int timeout)
{
	// Moves arm horisontally to position <position>, if  not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach position
	// ************ WARNING!!!!!!!  This low level function is not mechanically safe!! 
	// ************                 It may damage the plate changer
	// ************                 if used without caution to set the arm into UPMOST POSITION!

	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
//	if(PowerON()==-1) return -timeout;
	//Read whatever is set on Output Port X
	DAQmxReadDigitalLines (writehandle, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	//Move to position on Axis
	if(position & 1) dataout[1]=1; else dataout[1]=0;//Bank mask bit 1
	if(position & 2) dataout[2]=1; else dataout[2]=0;//Bank mask bit 2
	if(position & 4) dataout[3]=1; else dataout[3]=0;//Bank mask bit 3
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	gSystem->Sleep(10);
    dataout[6]=abs(1-dataout[6]);
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
    gSystem->Sleep(100);    
    dataout[6]=abs(1-dataout[6]);
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	//Check if position is reached
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	tme=0;
//	while(datain[0]*datain[1]*datain[2]==0 && tme<timeout)
	while(datain[0]*datain[2]==0 && tme<timeout)
	{
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
      gSystem->Sleep(100);tme++;
	}
	if(tme==timeout)
    {
		return -tme;
	}
	return tme;

}
int  AcqPlateChanger::GoOnVAxis(int position, int timeout)
{
	// Moves arm vertically to position <position>, if  not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach position
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
	//Read whatever is set on Output Port Z
	DAQmxReadDigitalLines (writehandlez, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	//Move to position on Axis
	if(position & 1) dataout[1]=1; else dataout[1]=0;//Bank mask bit 1
	if(position & 2) dataout[2]=1; else dataout[2]=0;//Bank mask bit 2
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	gSystem->Sleep(10);
    dataout[6]=abs(1-dataout[6]);
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
    gSystem->Sleep(100);    
    dataout[6]=abs(1-dataout[6]);
 //   dataout[6]=0;
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	//Check if position is reached
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
	tme=0;
//	while(datain[0]*datain[1]*datain[2]==0 && tme<timeout)
	while(datain[0]*datain[2]==0 && tme<timeout)
	{
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
      gSystem->Sleep(100);tme++;
	}
	if(tme==timeout)
    {
		return -tme;
	}
	return tme;

}


void AcqPlateChanger::UpdateStatusFile()
{
	// Syncronizes the status file with the current banks content
         char dateStr [9];
          char timeStr [9];
          _strdate( dateStr);
         _strtime( timeStr );
		 FILE *fp=fopen("c:\\ChangerStatus.txt","w");
  FILE *fpl=fopen("c:\\ChangerLog.txt","a");
  fprintf(fp,"Plate Changer Status on %s at %s.\n",dateStr,timeStr);
  fprintf(fpl,"Plate Changer Status on %s at %s.\n",dateStr,timeStr);
  for(int bnk=0;bnk<NumberOfBanks+1;bnk++){
    fprintf(fp,"Bank %d\n",bnk);
    fprintf(fpl,"Bank %d\n",bnk);
    fprintf(fp,"SEPARATORS %d\n",Bank[bnk].NSeparators);
    fprintf(fpl,"SEPARATORS %d\n",Bank[bnk].NSeparators);
    fprintf(fp,"PLATES %d\n",Bank[bnk].NEmulsions);
    fprintf(fpl,"PLATES %d\n",Bank[bnk].NEmulsions);
    for(int i=0;i<Bank[bnk].NEmulsions+Bank[bnk].NSeparators;i++) 
		if(Bank[bnk].Plates[i]->IsSeparator){ fprintf(fp,"SEPARATOR\n");fprintf(fpl,"SEPARATOR\n");}
		else {
			fprintf(fp,"PLATE %d %d %d %d\n",Bank[bnk].Plates[i]->id.Part[0],Bank[bnk].Plates[i]->id.Part[1],Bank[bnk].Plates[i]->id.Part[2],Bank[bnk].Plates[i]->id.Part[3]);
			fprintf(fpl,"PLATE %d %d %d %d\n",Bank[bnk].Plates[i]->id.Part[0],Bank[bnk].Plates[i]->id.Part[1],Bank[bnk].Plates[i]->id.Part[2],Bank[bnk].Plates[i]->id.Part[3]);
		}
		fprintf(fp,"EndBank %d\n",bnk);
		fprintf(fpl,"EndBank %d\n",bnk);
   
  }

  fclose(fp);
  fclose(fpl);
}

int AcqPlateChanger::ReadStatusFile()
{
	//restores the banks content from the status file
  char line[128];
  char key[32];
  int I,bnk,seps,plts,a,b,c,d,ip;
  FILE *fp=fopen("c:\\ChangerStatus.txt","r");
  if(fp==NULL) return 0;
  if(fgets(line,128,fp)==NULL) return 0; //date/time line is gisregarded
  ResetBanks();
  
  while(fgets(line,128,fp)!=NULL){
	  sscanf(line,"%s %d",key,&I);
	  if(strcmp(key,"Bank")==0)  bnk=I; 
	  if(strcmp(key,"SEPARATORS")==0) seps=I;
	  if(strcmp(key,"PLATES")==0) plts=I;
	  if(strcmp(key,"PLATE")==0){
		sscanf(line,"%s %d %d %d %d",key,&a,&b,&c,&d);
		ip=Bank[bnk].NEmulsions+Bank[bnk].NSeparators;
        Bank[bnk].Plates[ip]=new TPlate; //add plate
		Bank[bnk].Plates[ip]->SeqNumber=ip;
		Bank[bnk].Plates[ip]->InBank=bnk;
		Bank[bnk].Plates[ip]->IsSeparator=false;
		Bank[bnk].Plates[ip]->id.Part[0]=a;
		Bank[bnk].Plates[ip]->id.Part[1]=b;
		Bank[bnk].Plates[ip]->id.Part[2]=c;
		Bank[bnk].Plates[ip]->id.Part[3]=d;
		Bank[bnk].NEmulsions++;
	  }
	  if(strcmp(key,"SEPARATOR")==0){
		ip=Bank[bnk].NEmulsions+Bank[bnk].NSeparators;
        Bank[bnk].Plates[ip]=new TPlate; //add separator
		Bank[bnk].Plates[ip]->SeqNumber=ip;
		Bank[bnk].Plates[ip]->InBank=bnk;
		Bank[bnk].Plates[ip]->IsSeparator=true;
		Bank[bnk].Plates[ip]->id.Part[0]=0;
		Bank[bnk].Plates[ip]->id.Part[1]=0;
		Bank[bnk].Plates[ip]->id.Part[2]=0;
		Bank[bnk].Plates[ip]->id.Part[3]=0;
		Bank[bnk].NSeparators++;
	  }
	  if(strcmp(key,"EndBank")==0){ 
		  if((bnk!=I) || (Bank[bnk].NSeparators!=seps) || (Bank[bnk].NEmulsions!=plts))
		  {
			  printf("Plate Changer Error: ChangerStatus.txt file is corrupted!!! Resetting banks.\n");
			  ResetBanks();
///			  UpdateBanksView();
			  return 0;
		  }
	  }

  }
  fclose(fp);
  EmulsionOnTable=Bank[0].Plates[0];
  Ready=true;
  UpdateStatusFile();
  return 1;
}

void AcqPlateChanger::UnloadBrick()
{
	// Moves all emulsions and separators back to
	// the source bank to allow user to withdraw the brick.
	// Doesnt' reset banks!!!! Make sure to execute LoadBrick if you change the brick!
	UnloadPlate(false);
	printf("Moving emulsions from bank %d back to %d..",DestinationBank,SourceBank); fflush(stdout);
	while( (Bank[DestinationBank].NEmulsions + Bank[DestinationBank].NSeparators) > 0 )
	{
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return; else printf(" Continue.\n");}
		if(MoveEmulsion(DestinationBank,SourceBank)!=1) { printf("MoveEmulsion returned FAILED!\n"); Park(DestinationBank); return;};
		AddToBank(SourceBank,TakeFromBank(DestinationBank));
	}
	Park(DestinationBank);
	printf("Done. \nPlease take the brick from the bank %d\n", SourceBank);
}

void AcqPlateChanger::LoadBrick(int Brick, int Nemulsions, int isrc, int idest, bool Separators, bool FirstIsSeparator) 
{
	// Lets user load the emulsion pile into <isrc> bank, updates StatusFile.
	int res,inumem,ibrick;
	ibrick=Brick;
	inumem=Nemulsions;
	if(isrc<1 || isrc>3 || idest<1 || idest>3 || isrc==idest || inumem<=0) {
		printf("Wrong bank assignements!\n");
        return;
	}
if(!InitDone) Initialize();
    PowerON();
Park(idest);
printf("Load source bank, top sheet must be "); 
if( FirstIsSeparator) printf("the separator sheet "); 
else printf("the first emulsion ");
printf(", then press any key :"); fflush(stdout);
getchar();
printf("\n");

res=TableVacuumControl(true,VacuumTimeout); TableVacuumControl(false,1);
while(res>=0){
	printf("Please remove emulsion from the stage, then press any key :");
	getchar();
    printf("\n");
    res=TableVacuumControl(true,VacuumTimeout); TableVacuumControl(false,1);
}
//for(int bnk=0; bnk<NumberOfBanks+1;bnk++){ 
//	Bank[bnk].NEmulsions=0;
//	Bank[bnk].NSeparators=0;
//	}
ResetBanks();
SourceBank=isrc;
DestinationBank=idest;

Bank[isrc].NEmulsions=inumem;	
if(   Separators    ) Bank[isrc].NSeparators=inumem;
if( FirstIsSeparator ) { 
	(Bank[isrc].NSeparators)++;
  Bank[isrc].Plates[0]=new TPlate; //add sep
  Bank[isrc].Plates[0]->SeqNumber=0;
  Bank[isrc].Plates[0]->InBank=isrc;
  Bank[isrc].Plates[0]->IsSeparator=true;
  Bank[isrc].Plates[0]->id.Part[0]=ibrick;
  Bank[isrc].Plates[0]->id.Part[1]=0;
  Bank[isrc].Plates[0]->id.Part[2]=0;
  Bank[isrc].Plates[0]->id.Part[3]=0;

}
int Fsep;
Fsep=FirstIsSeparator?1:0;
if(   Separators   ) for(int i=0;i<inumem;i++) {
  Bank[isrc].Plates[i*2+Fsep]=new TPlate; //add plate
  Bank[isrc].Plates[i*2+Fsep]->SeqNumber=i*2+Fsep;
  Bank[isrc].Plates[i*2+Fsep]->InBank=isrc;
  Bank[isrc].Plates[i*2+Fsep]->IsSeparator=false;
  Bank[isrc].Plates[i*2+Fsep]->id.Part[0]=ibrick;
  Bank[isrc].Plates[i*2+Fsep]->id.Part[1]=i+1;
  Bank[isrc].Plates[i*2+Fsep]->id.Part[2]=0;
  Bank[isrc].Plates[i*2+Fsep]->id.Part[3]=0;

  Bank[isrc].Plates[i*2+1+Fsep]=new TPlate; //add sep
  Bank[isrc].Plates[i*2+1+Fsep]->SeqNumber=i*2+1+Fsep;
  Bank[isrc].Plates[i*2+1+Fsep]->InBank=isrc;
  Bank[isrc].Plates[i*2+1+Fsep]->IsSeparator=true;
  Bank[isrc].Plates[i*2+1+Fsep]->id.Part[0]=ibrick;
  Bank[isrc].Plates[i*2+1+Fsep]->id.Part[1]=i+1;
  Bank[isrc].Plates[i*2+1+Fsep]->id.Part[2]=0;
  Bank[isrc].Plates[i*2+1+Fsep]->id.Part[3]=0;

}
else for(int i=0;i<inumem;i++) {
  Bank[isrc].Plates[i]=new TPlate; //add emulsion
  Bank[isrc].Plates[i]->SeqNumber=i;
  Bank[isrc].Plates[i]->InBank=isrc;
  Bank[isrc].Plates[i]->IsSeparator=false;
  Bank[isrc].Plates[i]->id.Part[0]=ibrick;
  Bank[isrc].Plates[i]->id.Part[1]=i+1;
  Bank[isrc].Plates[i]->id.Part[2]=0;
  Bank[isrc].Plates[i]->id.Part[3]=0;

}


Ready=true;

UpdateStatusFile();
PowerOFF();
}


void AcqPlateChanger::ResetBanks()
{
	//resets the banks content
    if(EmulsionOnTable) delete EmulsionOnTable;
	EmulsionOnTable=NULL;
	for(int i=0;i<NumberOfBanks+1;i++) {
		Bank[i].Number=i; //0- microscope, 1-3 banks 
        Bank[i].NEmulsions=0; //not known how many plates it contains
        Bank[i].NSeparators=0; 
		for(int j=0;j<MAX_NUMBER_OF_PLATES*2;j++) {if(Bank[i].Plates[j]) delete Bank[i].Plates[j]; Bank[i].Plates[j]=NULL; }
	}
    Ready=false;
//UpdateStatusFile();
}


int AcqPlateChanger::PowerON()
{
	// Turns arm drives power ON.
	StX=ST->X->GetPos();
	StY=ST->Y->GetPos();
	StZ=ST->Z->GetPos(); //save current stage positions
	StageError=false;
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 datain[8];
	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};

	//Set AH on both drives
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	dataout[0]=1;
	DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandle, 10.0);
	DAQmxStopTask (writehandle);
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
	dataout[0]=1;
	DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
	DAQmxWaitUntilTaskDone (writehandlez, 10.0);
	DAQmxStopTask (writehandlez);
	gSystem->Sleep(20);
	//Check RDY signal
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	if(datain[0]==0) 
	{
		printf("Vertical drive controller error!\n");
		OnReset();
		return 0;
	}

	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , datain, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandle);
	if(datain[0]==0) 
	{
		printf("Horizontal drive controller error!\n");
		OnReset();
		return 0;
	}
	gSystem->Sleep(500);
	return 1;

}

void AcqPlateChanger::PowerOFF()
{
	// Turns arm drives power OFF.
	int32 tme=0;
	int32 read;
	int32 bytesPerSamp;
	uInt8 data[8];
	//reset ports 4 & 5 bit 0
	DAQmxReadDigitalLines (readhandle, 1, 10.0, DAQmx_Val_GroupByChannel , data, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandle, 10.0);
	DAQmxStopTask (readhandle);
	data[0]=0;
		DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , data, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandle, 10.0);
		DAQmxStopTask (writehandle);
	DAQmxReadDigitalLines (readhandlez, 1, 10.0, DAQmx_Val_GroupByChannel , data, 8, &read, &bytesPerSamp, NULL);
	DAQmxWaitUntilTaskDone (readhandlez, 10.0);
	DAQmxStopTask (readhandlez);
	data[0]=0;
		DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , data, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandlez, 10.0);
		DAQmxStopTask (writehandlez);
		// chack if stage coordinate changed during arm movements and rise StageError
	if(fabs(StX - ST->X->GetPos()) >10.) StageError=true;
	if(fabs(StY - ST->Y->GetPos()) >10.) StageError=true;
	if(fabs(StZ - ST->Z->GetPos()) >5.)  StageError=true;

}


int AcqPlateChanger::MoveEmulsion(int isrc, int idest)
{
	// Moves one sheet from bank to bank, whatever it is : emulsion or separator
	int res,attempt=0;
	int cornpos;
	if((isrc<0) || (isrc>3)) {printf("Wring Source Position assignement!\n");return 0;}
	if((idest<0) || (idest>3)) {printf("Wring Scanned Position assignement!\n");return 0;}
	if(isrc==idest) {printf("Same Source and Scanned Position assignement!\n");return 0;}
//	if(!PowerON()) return -1;
	printf("\n%d >   %d",isrc,idest); fflush(stdout);
    if(isrc==0) { 
        if(!LiftCorners) {
			res=GoOnHAxis(isrc,PositionTimeout);
			if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
			res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); 
		    if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
			TableVacuumControl(false,1);
			res=ArmVacuumControl(true,VacuumTimeout);
			if(res<0) {printf("ArmVacuum is not reached in 2 seconds!\n");}
			if(res<0) { GoOnVAxis(VPOS_UP,PositionTimeout); return 0;} //error
		}
		TableVacuumControl(false,1);
		attempt=0;
		while(attempt<NumberOfAttempt){
		 attempt++;
         if((!LiftCorners) || (attempt>1)) {
			 res=GoOnVAxis(VPOS_ABOVE_STAGE,2*PositionTimeout); 
		     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
//		     res=GoOnVAxis(VPOS_UP,PositionTimeout); 
		     res=ArmVacuumControl(true,VacuumTimeout);
		     if(res>=0) break; //The emulsion is taken succesfully
		 }
//if not: try to lift up corners
         res=ArmVacuumControl(false,1); // leave emulsion
//		 if((attempt/2.)==int(attempt/2.)) cornpos=4; else cornpos=5; //try to lift the corner = pos #4 or #5
		 cornpos=4; //try to lift the corner = pos #4 or #5
		 res=GoOnHAxis(cornpos,PositionTimeout); //try to lift the corner = pos #4 or #5
	     if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout);
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
         res=ArmVacuumControl(true,VacuumTimeout); //vacuum ON
		 res=GoOnVAxis(VPOS_ABOVE_STAGE,2*PositionTimeout); //lift the corner
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}

		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); //back down
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
         res=ArmVacuumControl(false,1); //Vacuum OFF
		 res=GoOnVAxis(VPOS_UP,PositionTimeout); // Go UP
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}

		  cornpos=5; //try to lift the corner = pos #4 or #5
		 res=GoOnHAxis(cornpos,PositionTimeout); //try to lift the corner = pos #4 or #5
	     if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout);
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
         res=ArmVacuumControl(true,VacuumTimeout); //vacuum ON
		 res=GoOnVAxis(VPOS_ABOVE_STAGE,2*PositionTimeout); //lift the corner
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}

		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); //back down
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
         res=ArmVacuumControl(false,1); //Vacuum OFF
		 res=GoOnVAxis(VPOS_UP,PositionTimeout); // Go UP
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}
      //   res=ArmVacuumControl(true,VacuumTimeout); //Vacuum ON


		 res=GoOnHAxis(isrc,PositionTimeout); //go back to 0 position
	     if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
//         res=ArmVacuumControl(true,VacuumTimeout); //Vacuum ON
		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); //and DOWN again, ready for the next attempt
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
         res=ArmVacuumControl(true,VacuumTimeout); //Vacuum ON
		}

		if(attempt==NumberOfAttempt) {
			ArmVacuumControl(false,1);
			GoOnVAxis(VPOS_UP,PositionTimeout); 
			printf("Robot can't take plate from the stage!\n");
			return 0;}
	}
	else { //taking from the bank - perform attempts to separate top emulsion from the rest
	res=GoOnHAxis(isrc,PositionTimeout);
	if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
	if(isrc==0) res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); else res=GoOnVAxis(VPOS_DOWN,PositionTimeout);
    if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	if(isrc==0) TableVacuumControl(false,1);
	res=ArmVacuumControl(true,VacuumTimeout);
	if(res<0 && isrc==0) {printf("ArmVacuum is not reached in 2 seconds!\n");}
	if(res<0) { GoOnVAxis(VPOS_UP,PositionTimeout); return 0;} //no more emulsion in the bank
		for(int att=0;att<SeparationAttempts;att++){
		 res=GoOnVAxis(VPOS_UP,PositionTimeout); // Go UP
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); //back down
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		}
	}
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%d  >  %d",isrc,idest); fflush(stdout);

	res=GoOnVAxis(VPOS_UP,PositionTimeout); 
	if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	res=GoOnHAxis(idest,PositionTimeout);
	if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
	if(idest==0) res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); else res=GoOnVAxis(VPOS_DOWN,PositionTimeout);
	if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%d   > %d",isrc,idest); fflush(stdout);
	res=ArmVacuumControl(false,1);
	if(idest==0) {
		res=TableVacuumControl(true,VacuumTimeout);
        if(res<0) 
		 {
			printf("TableVacuum is not reached in 5 seconds!\n");
			res=GoOnVAxis(VPOS_UP,PositionTimeout);
	        if(res<0) printf("Vpos is not reached in 5 seconds!\n");
			return 0;

		 }

		}
	res=GoOnVAxis(VPOS_UP,PositionTimeout);
	if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"); fflush(stdout);

	return 1;

}


void AcqPlateChanger::AddToBank(int bnk, TPlate *plate)
{
	// puts the info on plate to the container (nothing is done physically)
	// to keep track of real world situation
    for(int i=Bank[bnk].NEmulsions+Bank[bnk].NSeparators;i>0;i--) {
		Bank[bnk].Plates[i]=Bank[bnk].Plates[i-1];
	}
 plate->InBank=bnk;
 plate->SeqNumber=0;
 Bank[bnk].Plates[0]=plate;
 if(plate->IsSeparator) Bank[bnk].NSeparators++;
 else Bank[bnk].NEmulsions++;
 UpdateStatusFile();

}


TPlate* AcqPlateChanger::TakeFromBank(int bnk)
{
	// takes info on plate from the container (nothing is done physically)
	// to keep track of real world situation
 TPlate* retvalue;
 if(Bank[bnk].NEmulsions+Bank[bnk].NSeparators<=0) return NULL;
 retvalue=Bank[bnk].Plates[0];
     for(int i=0;i<Bank[bnk].NEmulsions+Bank[bnk].NSeparators-1;i++) {
		Bank[bnk].Plates[i]=Bank[bnk].Plates[i+1];

	}
 if(retvalue->IsSeparator) Bank[bnk].NSeparators--;
 else Bank[bnk].NEmulsions--;
 //if(bnk==0) EmulsionOnTable=NULL;
 UpdateStatusFile();
 retvalue->InBank=-1; //unknown
 return retvalue;
}

int AcqPlateChanger::LoadPlate(int id0, int id1, int id2, int id3)
{
	// Loads the plate with given indices (id0 is brick number, id1 is plate number)
	// returns 0 if failed,
	// 1 if succeeded,
	// 2 if plate is already on the table
Identifier id;
id.Part[0]=id0;
id.Part[1]=id1;
id.Part[2]=id2;
id.Part[3]=id3;
return LoadPlate(id);
}

int AcqPlateChanger::LoadPlate(Identifier id)
{
	// Loads the plate with given Id
	// returns 0 if failed,
	// 1 if succeeded,
	// 2 if plate is already on the table
	printf("LoadPlate is in progress...\n"); fflush(stdout);
	int SeqN=-1,sn,BankN;
	int Mfrom, Mto;
	int res;
	if(FindEmulsion(id,0)>=0) {printf("Load Plate is complete (plate was already on the stage).\n"); return 2; }//Already loaded
	for(int bnk=1; bnk<NumberOfBanks+1;bnk++) {
	  sn=FindEmulsion(id,bnk);
	  if(sn>=0){ SeqN=sn; BankN=bnk; break;}
	}
	if(SeqN<0) {printf("Load Plate is failed. Emulsion is not found in banks!\n"); return 0; }//Emulsion is not found anywhere
	printf("Required emulsion is found in bank #%d at position %d\n",BankN,SeqN+1);
    // Emulsion is in bank  BankN at the position SeqN, SeqN=0 means at the top
    //Now put it onto the table
     if(ST)
	 { //Move stage to exchange position
		ST->Z->PosMove(ZLoadPosition,1e9,1e9); 
		ST->X->PosMove(XLoadPosition,1e9,1e9); 
		ST->Y->PosMove(YLoadPosition,1e9,1e9);
		printf("Waiting for stage to reach exchange position..\n"); fflush(stdout);
		res=ST->X->WaitForMoveComplete(5e3);
		res*=ST->Y->WaitForMoveComplete(5e3);
		res*=ST->Z->WaitForMoveComplete(3e3);
		if(res==0) {printf("Stage didn't reach exchange position! Unload Plate is failed. \n"); return 0; }
	 }
	 else { printf("No connection with the stage (stage pointer is not set)!!! Unload Plate is failed. \n"); return 0; }
	 printf("OK.\n"); //reached position;

    if(BankN==SourceBank) { Mfrom=BankN; Mto=DestinationBank;} 
	else                 { Mto=SourceBank; Mfrom=BankN;} 
    PowerON();
	if(EmulsionOnTable!=NULL) {
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}
		printf("OK..\nMoving emulsion from the stage..\n"); fflush(stdout);
		if(MoveEmulsion(0,Mto)!=1) { Park(Mto); return 0;};
		EmulsionOnTable=TakeFromBank(0);
		if(EmulsionOnTable) { AddToBank(Mto,EmulsionOnTable); EmulsionOnTable=NULL; }
		printf("Done.\n"); fflush(stdout);
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}

	}
	if(SeqN>0) printf("Moving emulsions from bank #%d to #%d\n", Mfrom,Mto); fflush(stdout);
	for(int num=0;num<SeqN;num++){ //move from one bank to another all unnecessary plates
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}
		AddToBank(Mto,TakeFromBank(Mfrom));
		if(MoveEmulsion(Mfrom,Mto)!=1) { Park(Mto); return 0;};
	}
			printf("Done.\n"); fflush(stdout);
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}

	//Now top in Mfrom bank must be emulsion that we search for
	if(FindEmulsion(id,Mfrom)!=0) {
		printf("Brick structure incosistent!!! Reload the brick!\n");
		return 0; //Inconsistency!!!!!
	}
		printf("OK..moving emulsion to the stage.."); fflush(stdout);
	EmulsionOnTable=TakeFromBank(Mfrom); //if movement is OK update database
    AddToBank(0,EmulsionOnTable);
	if(MoveEmulsion(Mfrom,0)!=1) {  Park(Mto); return 0;};
		printf("Done.\n"); fflush(stdout);
	Park(Mfrom);
 
	printf("Load Plate is complete.\n");
	UpdateStatusFile();
    PowerOFF();
	return 1;
}

int AcqPlateChanger::UnloadPlate(bool TurnPowerOff)
{
	// Unloads the plate from the stage to the source bank
	// returns 1 if OK, 
	// 0 in case of error.
    PowerON();
printf("Unload plate is in progress...\n");
    int isrc,idest;
 EmulsionOnTable=TakeFromBank(0);
 if(!EmulsionOnTable) return 1;
	isrc=SourceBank;
	idest=DestinationBank;
	if(isrc<1 || isrc>3 || idest<1 || idest>3 || isrc==idest) {
		printf("Wrong bank assignements!\n");
		printf("Unload Plate is failed.\n");
        return 0;
	}
//All is ok, now move plate to src bank
	int res;
    if(ST){ //Move stage to exchange position
	ST->Z->PosMove(ZLoadPosition,1e9,1e9); 
	ST->X->PosMove(XLoadPosition,1e9,1e9); 
	ST->Y->PosMove(YLoadPosition,1e9,1e9);
	res=ST->X->WaitForMoveComplete(5e3);
	res*=ST->Y->WaitForMoveComplete(5e3);
	res*=ST->Z->WaitForMoveComplete(3e3);
	if(res==0) {printf("Stage didn't reach exchange position! Unload Plate is failed. \n"); return 0; }
	}
	AddToBank(isrc,EmulsionOnTable);
	EmulsionOnTable=NULL;

	if(MoveEmulsion(0,isrc)!=1) {	printf("Unload Plate is failed. "); Park(idest);  return 0;};

printf("Unload Plate is complete.\n");
UpdateStatusFile();
if(TurnPowerOff)PowerOFF();
return 1;

}

int AcqPlateChanger::FindEmulsion(Identifier id, int bnk) 
{
	// returns the sequence number of the given emulsion in the bank
	// if not found, returns -1
    for(int i=0;i<Bank[bnk].NEmulsions+Bank[bnk].NSeparators;i++) {
	 if(Bank[bnk].Plates[i]->IsSeparator==false)
	 if(Bank[bnk].Plates[i]->id.Part[0]==id.Part[0])
	 if(Bank[bnk].Plates[i]->id.Part[1]==id.Part[1])
	 if(Bank[bnk].Plates[i]->id.Part[2]==id.Part[2])
	 if(Bank[bnk].Plates[i]->id.Part[3]==id.Part[3])
		 return i;
	}

 return -1;
}
int AcqPlateChanger::Park(int idest)
{
	// Moves the arm up and to the idest position
    PowerON();
	int res;
	if((idest<0) || (idest>3)) {printf("Wrong Scanned Position assignement!\n");return 0;}
	res=GoOnVAxis(VPOS_UP,PositionTimeout); 
	if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	res=GoOnHAxis(idest,PositionTimeout);
	if(res<0) {printf("Hpos is not reached in 5 seconds!\n");return 0;}
	PowerOFF();
	return 1;
}