//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqSerialPlateChanger                                                      //
//                                                                      //
// 	Interface to Bern emulsion plate changer			        		//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//#include "TROOT.h"
#include "TSystem.h"

//#ifndef __CINT__
//#include "nidaqmx.h"
//#endif

//#include "serialport.h"


#include "AcqSerialPlateChanger.h"



//#include "nidaqmx.h"
	TPlateS* EmulsionOnTableS;
    TBankS BankS[5];

ClassImp(AcqSerialPlateChanger);

//_____________________________________________________________________________ 
AcqSerialPlateChanger::AcqSerialPlateChanger()
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
		XLoadPosition=-63082; 
	    YLoadPosition=12485;
	    ZLoadPosition=5000;
		SourceBank=1;
	     DestinationBank=2;
		 StageError=false;
		 ComPort=1; //default COM1
		 ComBaud=19200;
         VacRegister=0;
		 Debug=0; // default - no debug messages produced


}
//_____________________________________________________________________________ 
void AcqSerialPlateChanger::Print()
{
	// prints the content of emulsion banks
		for(int bnk=0;bnk<NumberOfBanks+1;bnk++) 
		{
			if(bnk==0) printf("\nTable:",BankS[bnk].Number);
			else printf("\nBank #%d content:",BankS[bnk].Number);
			if(BankS[bnk].NEmulsions==0 && BankS[bnk].NSeparators==0) printf(" Empty.\n\n");
			else printf("\n\n");

			for(int i=0;i<BankS[bnk].NEmulsions+BankS[bnk].NSeparators;i++)  
			{
                if(BankS[bnk].Plates[i]->IsSeparator) printf("---Separator---\n");
	            else printf("Emulsion %d %d %d %d\n",BankS[bnk].Plates[i]->id.Part[0],
					BankS[bnk].Plates[i]->id.Part[1],
					BankS[bnk].Plates[i]->id.Part[2],
					BankS[bnk].Plates[i]->id.Part[3]);
			}
		}


}
//_____________________________________________________________________________ 

AcqSerialPlateChanger::~AcqSerialPlateChanger()
{
	OnReset();
}


void AcqSerialPlateChanger::OnReset() 
{
	// resets the power on platechanger arm, turns off the vacuums
/*		uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
		int32 read;
		//Writing to P5
		DAQmxWriteDigitalLines (writehandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandle, 10.0);
		DAQmxStopTask (writehandle);
		//Writing to P4
		DAQmxWriteDigitalLines (writehandlez, 1, 1, 10.0, DAQmx_Val_GroupByChannel , dataout, &read, NULL);
		DAQmxWaitUntilTaskDone (writehandlez, 10.0);
		DAQmxStopTask (writehandlez);
*/		if(Debug) printf("OnReset called\n");
	    InitDone=false;
        Ready=false;
		PowerOFF(100);
		if(Debug) printf("OnReset done.\n");
}

int AcqSerialPlateChanger::Initialize(AcqStage1* Stage)
{
	// initializes the arm drives, vacuum generators,
	// moves the arm to position 1 up,
	// reads the status file and sets flags InitDone, Ready
    ST=Stage;
	int tme=0;
	int res;
//	int32 read;
//	int32 bytesPerSamp;
//	uInt8 datain[8];
//	uInt8 dataout[8] = {0,0,0,0,0,0,0,0};
	printf("Initializing Plate changer..\n");
    port=new AcqCOM();
	port->Close();
	if(!port->Open(ComPort,ComBaud)) {printf("Can't open COM port! PlateChanger Init failed.\n"); return 0;}
	PowerON();
		 res=GoOnVAxis(VPOS_UP,PositionTimeout);
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
    Park(1);
	InitDone=true;
    Ready=false;
	printf("Reading status file.."); fflush(stdout);

    ReadStatusFile();

   if(EmulsionOnTableS) TableVacuumControl(true,1);
	printf("Done!"); fflush(stdout);
	Dump();
	PowerOFF();
	return 1;
}

int AcqSerialPlateChanger::PowerON(int timeout)
{
	// Turns arm drives power ON.
//	StX=ST->X->GetPos();
//	StY=ST->Y->GetPos();
//	StZ=ST->Z->GetPos(); //save current stage positions
	if(Debug) printf("PowerON called\n");
	StageError=false;
	int tme=0;
	int res;
	char str[8];
	char rep[128];
   	str[0]='V';
	VacRegister= VacRegister | 0x10;
	str[1]=VacRegister;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[1]=0x%x \n",res,rep[0],rep[1]);
	  if(res==0) continue;
	  if( rep[0]=='D' && (rep[1] & 0x08)) Reached=true;
	}
	if(Debug) printf("PowerON reached %d, timeout is %d. Done.\n",tme,timeout);
	if(tme>=timeout) return -tme;
	else return tme;

}

int AcqSerialPlateChanger::PowerOFF(int timeout)
{
	// Turns arm drives power OFF.
	if(Debug) printf("PowerOFF called.\n");

	int tme=0;
	int res;
	char str[8];
	char rep[128];
   	str[0]='V';
	VacRegister = VacRegister & 0xEF;
	str[1]=VacRegister;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[1]=0x%x \n",res,rep[0],rep[1]);
	  if(res==0) continue;
	  if(rep[0]=='D' && !(rep[1] & 0x08)) Reached=true;
	}
		if(Debug) printf("PowerOFF reached %d, timeout is %d. Done.\n",tme,timeout);

	if(tme>=timeout) return -tme;
	else return tme;
	// check if stage coordinate changed during arm movements and rise StageError
//	if(fabs(StX - ST->X->GetPos()) >10.) StageError=true;
//	if(fabs(StY - ST->Y->GetPos()) >10.) StageError=true;
//	if(fabs(StZ - ST->Z->GetPos()) >5.)  StageError=true;

}
int AcqSerialPlateChanger::ArmVacuumControl(bool VacOn, int timeout)
{
	// Switches the vacuum on/off at arm holder, if vacuum is not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach vacuum
	if(Debug) printf("ArmVacuumControl(%d,%d) called.\n",VacOn,timeout);
	int tme=0;
	int res;
	char str[8];
	char rep[128];
   	str[0]='V';
	if(VacOn) VacRegister= VacRegister | 0x01; else VacRegister = VacRegister & 0xFE;
	str[1]=VacRegister;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[4]=0x%x \n",res,rep[0],((unsigned)(rep[4])));
//	  port->CleanAll();
	  if(res==0) continue;
	 // if(res==5 && rep[0]=='D' && (rep[4] & 0x01)) Reached=true;
	  if(rep[0]=='D' && (rep[4] & 0x01)) Reached=true;
	}
	if(Debug) printf("ArmVacuumControl reached %d, timeout is %d. Done.\n",tme,timeout);
	if(tme>=timeout) return -tme;
	else return tme;

}

int AcqSerialPlateChanger::TableVacuumControl(bool VacOn, int timeout, bool BlowOn)
{
	// Switches the vacuum on/off at the table, if vacuum is not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach vacuum
	if(Debug) printf("TableVacuumControl(%d,%d,%d) called.\n",VacOn,timeout,BlowOn);
	if(VacOn) BlowOn=false;
	int tme=0;
	int res;
	char str[8];
	char rep[128];
	str[0]='V';
	if(VacOn) VacRegister= VacRegister | 0x02; else VacRegister = VacRegister & 0xFD;
	if(BlowOn) VacRegister= VacRegister | 0x04; else VacRegister = VacRegister & 0xFB;
	str[1]=VacRegister;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[4]=0x%x \n",res,rep[0],((unsigned)(rep[4])));
	  if(res==0) continue;
	  if(rep[0]=='D' && (rep[4] & 0x02)) Reached=true;
	}
	if(Debug) printf("TableVacuumControl reached %d, timeout is %d. Done.\n",tme,timeout);
	if(tme>=timeout) return -tme;
	else return tme;


}

int  AcqSerialPlateChanger::GoOnHAxis(int position, int timeout)
{
	// Moves arm horisontally to position <position>, if  not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach position
	// ************ WARNING!!!!!!!  This low level function is not mechanically safe!! 
	// ************                 It may damage the plate changer
	// ************                 if used without caution to set the arm into UPMOST POSITION!

	int tme=0;
	int res;
	char str[8];
	char rep[128];
if(Debug)	printf("GoOnHAxis(%d, %d) called.\n",position,timeout);
	str[0]='X';
	str[1]=position & 0x07;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[1]=0x%x  rep[4]=0x%x \n",res,rep[0],rep[1],((unsigned)(rep[4])));
	  if(res==0) continue;
	  if(rep[0]=='D' && (rep[1] & 0x40) && (rep[1] & 0x10) && !(rep[1] & 0x80) && !(rep[1] & 0x20)) Reached=true;
	}
	if(Debug) printf("GoOnHAxis reached %d, timeout is %d. Done.\n",tme,timeout);
	if(tme>=timeout) return -tme;
	else return tme;


}
int  AcqSerialPlateChanger::GoOnVAxis(int position, int timeout)
{
	// Moves arm vertically to position <position>, if  not reached within given timeout returns -timeout,
	// otherwise returns actual time spent to reach position
	int tme=0;
	int res;
	char str[8];
	char rep[128];
if(Debug)	printf("GoOnVAxis(%d, %d) called.\n",position,timeout);
	str[0]='Y';
	str[1]=position & 0x07;
	bool Reached=false;
	if(port->SendString(str,2)!=2) return -tme;
		while( !Reached && tme<timeout)
	{
	  str[0]='Q';
	if(port->SendString(str,1)!=1) return -tme;
	  port->Flush();
      gSystem->Sleep(50);tme++;
	  port->Flush();
	  res=port->RecvString(rep,5);
	  if(res==0) continue;
if(Debug)	  printf("res=0x%x rep[0]=0x%x  rep[2]=0x%x  rep[4]=0x%x \n",res,rep[0],rep[2],((unsigned)(rep[4])));
	  if(rep[0]=='D' && (rep[2] & 0x40) && (rep[2] & 0x10) && !(rep[2] & 0x80) && !(rep[2] & 0x20)) Reached=true;
	}
	if(Debug) printf("GoOnVAxis reached %d, timeout is %d. Done.\n",tme,timeout);
	if(tme>=timeout) return -tme;
	else return tme;


}


void AcqSerialPlateChanger::UpdateStatusFile()
{
	// Syncronizes the status file with the current banks content
if(Debug) printf(" UpdateStatusFile() called.\n");
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
    fprintf(fp,"SEPARATORS %d\n",BankS[bnk].NSeparators);
    fprintf(fpl,"SEPARATORS %d\n",BankS[bnk].NSeparators);
    fprintf(fp,"PLATES %d\n",BankS[bnk].NEmulsions);
    fprintf(fpl,"PLATES %d\n",BankS[bnk].NEmulsions);
    for(int i=0;i<BankS[bnk].NEmulsions+BankS[bnk].NSeparators;i++) 
		if(BankS[bnk].Plates[i]->IsSeparator){ fprintf(fp,"SEPARATOR\n");fprintf(fpl,"SEPARATOR\n");}
		else {
			fprintf(fp,"PLATE %d %d %d %d\n",BankS[bnk].Plates[i]->id.Part[0],BankS[bnk].Plates[i]->id.Part[1],BankS[bnk].Plates[i]->id.Part[2],BankS[bnk].Plates[i]->id.Part[3]);
			fprintf(fpl,"PLATE %d %d %d %d\n",BankS[bnk].Plates[i]->id.Part[0],BankS[bnk].Plates[i]->id.Part[1],BankS[bnk].Plates[i]->id.Part[2],BankS[bnk].Plates[i]->id.Part[3]);
		}
		fprintf(fp,"EndBank %d\n",bnk);
		fprintf(fpl,"EndBank %d\n",bnk);
   
  }

  fclose(fp);
  fclose(fpl);
  if(Debug) printf(" UpdateStatusFile() done.\n");

}

int AcqSerialPlateChanger::ReadStatusFile()
{
	//restores the banks content from the status file
if(Debug) printf(" ReadStatusFile() called.\n");
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
		ip=BankS[bnk].NEmulsions+BankS[bnk].NSeparators;
        BankS[bnk].Plates[ip]=new TPlateS; //add plate
		BankS[bnk].Plates[ip]->SeqNumber=ip;
		BankS[bnk].Plates[ip]->InBank=bnk;
		BankS[bnk].Plates[ip]->IsSeparator=false;
		BankS[bnk].Plates[ip]->id.Part[0]=a;
		BankS[bnk].Plates[ip]->id.Part[1]=b;
		BankS[bnk].Plates[ip]->id.Part[2]=c;
		BankS[bnk].Plates[ip]->id.Part[3]=d;
		BankS[bnk].NEmulsions++;
	  }
	  if(strcmp(key,"SEPARATOR")==0){
		ip=BankS[bnk].NEmulsions+BankS[bnk].NSeparators;
        BankS[bnk].Plates[ip]=new TPlateS; //add separator
		BankS[bnk].Plates[ip]->SeqNumber=ip;
		BankS[bnk].Plates[ip]->InBank=bnk;
		BankS[bnk].Plates[ip]->IsSeparator=true;
		BankS[bnk].Plates[ip]->id.Part[0]=0;
		BankS[bnk].Plates[ip]->id.Part[1]=0;
		BankS[bnk].Plates[ip]->id.Part[2]=0;
		BankS[bnk].Plates[ip]->id.Part[3]=0;
		BankS[bnk].NSeparators++;
	  }
	  if(strcmp(key,"EndBank")==0){ 
		  if((bnk!=I) || (BankS[bnk].NSeparators!=seps) || (BankS[bnk].NEmulsions!=plts))
		  {
			  printf("Plate Changer Error: ChangerStatus.txt file is corrupted!!! Resetting banks.\n");
			  ResetBanks();
			  if(Debug) printf(" ReadStatusFile() done. returns 0.\n");

///			  UpdateBanksView();
			  return 0;
		  }
	  }

  }
  fclose(fp);
  EmulsionOnTableS=BankS[0].Plates[0];
  Ready=true;
  UpdateStatusFile();
  if(Debug) printf(" ReadStatusFile() done. returns 1.\n");
  return 1;
}

void AcqSerialPlateChanger::UnloadBrick()
{
	// Moves all emulsions and separators back to
	// the source bank to allow user to withdraw the brick.
	// Doesnt' reset banks!!!! Make sure to execute LoadBrick if you change the brick!
if(Debug) printf(" UnloadBrick() called.\n");
UnloadPlate(false);
	printf("Moving emulsions from bank %d back to %d..",DestinationBank,SourceBank); fflush(stdout);
	while( (BankS[DestinationBank].NEmulsions + BankS[DestinationBank].NSeparators) > 0 )
	{
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return; else printf(" Continue.\n");}
		if(MoveEmulsion(DestinationBank,SourceBank)!=1) { printf("MoveEmulsion returned FAILED!\n"); Park(DestinationBank); return;};
		AddToBank(SourceBank,TakeFromBank(DestinationBank));
	}
	Park(DestinationBank);
	printf("Done. \nPlease take the brick from the bank %d\n", SourceBank);
}

void AcqSerialPlateChanger::LoadBrick(int Brick, int Nemulsions, int isrc, int idest, bool Separators, bool FirstIsSeparator) 
{
	// Lets user load the emulsion pile into <isrc> bank, updates StatusFile.
if(Debug) printf(" LoadBrick(%d,%d,%d,%d,%d,%d) called.\n",Brick,Nemulsions, isrc,idest,Separators,FirstIsSeparator);
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
//	BankS[bnk].NEmulsions=0;
//	BankS[bnk].NSeparators=0;
//	}
ResetBanks();
SourceBank=isrc;
DestinationBank=idest;

BankS[isrc].NEmulsions=inumem;	
if(   Separators    ) BankS[isrc].NSeparators=inumem;
if( FirstIsSeparator ) { 
	(BankS[isrc].NSeparators)++;
  BankS[isrc].Plates[0]=new TPlateS; //add sep
  BankS[isrc].Plates[0]->SeqNumber=0;
  BankS[isrc].Plates[0]->InBank=isrc;
  BankS[isrc].Plates[0]->IsSeparator=true;
  BankS[isrc].Plates[0]->id.Part[0]=ibrick;
  BankS[isrc].Plates[0]->id.Part[1]=0;
  BankS[isrc].Plates[0]->id.Part[2]=0;
  BankS[isrc].Plates[0]->id.Part[3]=0;

}
int Fsep;
Fsep=FirstIsSeparator?1:0;
if(   Separators   ) for(int i=0;i<inumem;i++) {
  BankS[isrc].Plates[i*2+Fsep]=new TPlateS; //add plate
  BankS[isrc].Plates[i*2+Fsep]->SeqNumber=i*2+Fsep;
  BankS[isrc].Plates[i*2+Fsep]->InBank=isrc;
  BankS[isrc].Plates[i*2+Fsep]->IsSeparator=false;
  BankS[isrc].Plates[i*2+Fsep]->id.Part[0]=ibrick;
  BankS[isrc].Plates[i*2+Fsep]->id.Part[1]=i+1;
  BankS[isrc].Plates[i*2+Fsep]->id.Part[2]=0;
  BankS[isrc].Plates[i*2+Fsep]->id.Part[3]=0;

  BankS[isrc].Plates[i*2+1+Fsep]=new TPlateS; //add sep
  BankS[isrc].Plates[i*2+1+Fsep]->SeqNumber=i*2+1+Fsep;
  BankS[isrc].Plates[i*2+1+Fsep]->InBank=isrc;
  BankS[isrc].Plates[i*2+1+Fsep]->IsSeparator=true;
  BankS[isrc].Plates[i*2+1+Fsep]->id.Part[0]=ibrick;
  BankS[isrc].Plates[i*2+1+Fsep]->id.Part[1]=i+1;
  BankS[isrc].Plates[i*2+1+Fsep]->id.Part[2]=0;
  BankS[isrc].Plates[i*2+1+Fsep]->id.Part[3]=0;

}
else for(int i=0;i<inumem;i++) {
  BankS[isrc].Plates[i]=new TPlateS; //add emulsion
  BankS[isrc].Plates[i]->SeqNumber=i;
  BankS[isrc].Plates[i]->InBank=isrc;
  BankS[isrc].Plates[i]->IsSeparator=false;
  BankS[isrc].Plates[i]->id.Part[0]=ibrick;
  BankS[isrc].Plates[i]->id.Part[1]=i+1;
  BankS[isrc].Plates[i]->id.Part[2]=0;
  BankS[isrc].Plates[i]->id.Part[3]=0;

}


Ready=true;

UpdateStatusFile();
PowerOFF();
 if(Debug) printf(" LoadBrick() done.\n");

}


void AcqSerialPlateChanger::ResetBanks()
{
	//resets the banks content
	 if(Debug) printf(" ResetBanks() called.\n");
    if(EmulsionOnTableS) delete EmulsionOnTableS;
	EmulsionOnTableS=NULL;
	for(int i=0;i<NumberOfBanks+1;i++) {
		BankS[i].Number=i; //0- microscope, 1-3 banks 
        BankS[i].NEmulsions=0; //not known how many plates it contains
        BankS[i].NSeparators=0; 
		for(int j=0;j<MAX_NUMBER_OF_PLATES*2;j++) {if(BankS[i].Plates[j]) delete BankS[i].Plates[j]; BankS[i].Plates[j]=NULL; }
	}
    Ready=false;
//UpdateStatusFile();
	 if(Debug) printf(" ResetBanks() done.\n");

}


int AcqSerialPlateChanger::GetLoadedId(int *id0, int *id1,int *id2,int *id3)
{
	//returns 0 if no emulsion on the stage 
    if(!EmulsionOnTableS) return 0;
    if(id0) (*id0)=EmulsionOnTableS->id.Part[0];
    if(id1) (*id1)=EmulsionOnTableS->id.Part[1];
    if(id2) (*id2)=EmulsionOnTableS->id.Part[2];
    if(id3) (*id3)=EmulsionOnTableS->id.Part[3];
	return 1;
}





int AcqSerialPlateChanger::MoveEmulsion(int isrc, int idest)
{
	// Moves one sheet from bank to bank, whatever it is : emulsion or separator
	 if(Debug) printf(" --- MoveEmulsion(%d,%d)\n",isrc,idest);
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
			res=ArmVacuumControl(true,VacuumTimeout);
			TableVacuumControl(false,1,true);
			if(res<0) {printf("ArmVacuum is not reached in 2 seconds!\n");}
			if(res<0) { GoOnVAxis(VPOS_UP,PositionTimeout); return 0;} //error
		}
		TableVacuumControl(false,1,true);
		attempt=0;
		while(attempt<NumberOfAttempt){
		 attempt++;
         if((!LiftCorners) || (attempt>1)) {
		TableVacuumControl(false,1,true);
		     gSystem->Sleep(100);
			 res=GoOnVAxis(VPOS_ABOVE_STAGE,2*PositionTimeout); 
		     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
//		     res=GoOnVAxis(VPOS_UP,PositionTimeout); 
		     res=ArmVacuumControl(true,VacuumTimeout);
		     if(res>=0) break; //The emulsion is taken succesfully
		 }
//if not: try to lift up corners
         res=ArmVacuumControl(false,1); // leave emulsion
			TableVacuumControl(false,1);
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
		TableVacuumControl(false,1);

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
	if(Debug) printf("Arm vac seems ok... trying to separate emulsions..\n");
		for(int att=0;att<SeparationAttempts;att++){
		 if(Debug) printf("Attempt %d..going up..\n",att+1);
		 res=GoOnVAxis(VPOS_UP,PositionTimeout); // Go UP
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		 if(Debug) printf("..going down..\n",att+1);
		 res=GoOnVAxis(VPOS_DOWN_TO_STAGE,PositionTimeout); //back down
	     if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
		}
		if(Debug) printf("Separation loop is complete..\n");

	}
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%d  >  %d",isrc,idest); fflush(stdout);

	res=GoOnVAxis(VPOS_UP,PositionTimeout); 
	if(res<0) {printf("Vpos is not reached in 5 seconds!\n");return 0;}
	if(Debug) printf("Ready to go to bank idest=%d\n",idest);
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
	 if(Debug) printf(" --- MoveEmulsion is done, returns 1.\n");

	return 1;

}


void AcqSerialPlateChanger::AddToBank(int bnk, TPlateS *plate)
{
	// puts the info on plate to the container (nothing is done physically)
	// to keep track of real world situation
    for(int i=BankS[bnk].NEmulsions+BankS[bnk].NSeparators;i>0;i--) {
		BankS[bnk].Plates[i]=BankS[bnk].Plates[i-1];
	}
 plate->InBank=bnk;
 plate->SeqNumber=0;
 BankS[bnk].Plates[0]=plate;
 if(plate->IsSeparator) BankS[bnk].NSeparators++;
 else BankS[bnk].NEmulsions++;
 UpdateStatusFile();

}


TPlateS* AcqSerialPlateChanger::TakeFromBank(int bnk)
{
	// takes info on plate from the container (nothing is done physically)
	// to keep track of real world situation
 TPlateS* retvalue;
 if(BankS[bnk].NEmulsions+BankS[bnk].NSeparators<=0) return NULL;
 retvalue=BankS[bnk].Plates[0];
     for(int i=0;i<BankS[bnk].NEmulsions+BankS[bnk].NSeparators-1;i++) {
		BankS[bnk].Plates[i]=BankS[bnk].Plates[i+1];

	}
 if(retvalue->IsSeparator) BankS[bnk].NSeparators--;
 else BankS[bnk].NEmulsions--;
 //if(bnk==0) EmulsionOnTableS=NULL;
 UpdateStatusFile();
 retvalue->InBank=-1; //unknown
 return retvalue;
}

int AcqSerialPlateChanger::LoadPlate(int id0, int id1, int id2, int id3)
{
	// Loads the plate with given indices (id0 is brick number, id1 is plate number)
	// returns 0 if failed,
	// 1 if succeeded,
	// 2 if plate is already on the table
IdentifierS id;
id.Part[0]=id0;
id.Part[1]=id1;
id.Part[2]=id2;
id.Part[3]=id3;
return LoadPlate(id);
}

int AcqSerialPlateChanger::LoadPlate(IdentifierS id)
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
	if(EmulsionOnTableS!=NULL) {
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return 0; else printf(" Continue.\n");}
		printf("OK..\nMoving emulsion from the stage..\n"); fflush(stdout);
		if(MoveEmulsion(0,Mto)!=1) { Park(Mto); return 0;};
		EmulsionOnTableS=TakeFromBank(0);
		if(EmulsionOnTableS) { AddToBank(Mto,EmulsionOnTableS); EmulsionOnTableS=NULL; }
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
	EmulsionOnTableS=TakeFromBank(Mfrom); //if movement is OK update database
    AddToBank(0,EmulsionOnTableS);
	if(MoveEmulsion(Mfrom,0)!=1) {  Park(Mto); return 0;};
		printf("Done.\n"); fflush(stdout);
	Park(Mfrom);
 
	printf("Load Plate is complete.\n");
	UpdateStatusFile();
    PowerOFF();
	return 1;
}

int AcqSerialPlateChanger::UnloadPlate(bool TurnPowerOff)
{
	// Unloads the plate from the stage to the source bank
	// returns 1 if OK, 
	// 0 in case of error.
    PowerON();
printf("Unload plate is in progress...\n");
    int isrc,idest;
 EmulsionOnTableS=TakeFromBank(0);
 if(!EmulsionOnTableS) return 1;
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
	AddToBank(isrc,EmulsionOnTableS);
	EmulsionOnTableS=NULL;

	if(MoveEmulsion(0,isrc)!=1) {	printf("Unload Plate is failed. "); Park(idest);  return 0;};

printf("Unload Plate is complete.\n");
UpdateStatusFile();
if(TurnPowerOff)PowerOFF();
return 1;

}

int AcqSerialPlateChanger::FindEmulsion(IdentifierS id, int bnk) 
{
	// returns the sequence number of the given emulsion in the bank
	// if not found, returns -1
    for(int i=0;i<BankS[bnk].NEmulsions+BankS[bnk].NSeparators;i++) {
	 if(BankS[bnk].Plates[i]->IsSeparator==false)
	 if(BankS[bnk].Plates[i]->id.Part[0]==id.Part[0])
	 if(BankS[bnk].Plates[i]->id.Part[1]==id.Part[1])
	 if(BankS[bnk].Plates[i]->id.Part[2]==id.Part[2])
	 if(BankS[bnk].Plates[i]->id.Part[3]==id.Part[3])
		 return i;
	}

 return -1;
}
int AcqSerialPlateChanger::Park(int idest)
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