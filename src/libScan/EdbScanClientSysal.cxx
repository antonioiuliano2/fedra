// Author :  Valeri Tioukov   22/12/2006
// Revised:  Mykhailo Vladymyrov 18/07/2013
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanClientSysal                                                   //
//                                                                      //
// Sysal remote scanning class based on the code of scripts "scanlib.C" //
// written by Igor Kreslo and interacting with SySal BernScanDriver     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "EdbScanClient.h"
#include "EdbScanClientSysal.h"
#include "libDataConversion.h"

ClassImp(EdbScanClientSysal)
//----------------------------------------------------------------
void EdbScanClientSysal::Print()
{
  printf("EdbScanClient:\n");
  printf("Server: %s  PORT: %d\n",eServer.Data(),ePORT);
  printf("CMD:    %s\n",eCMD);
  printf("mess:   %s\n",eMess);
  printf("mess1:  %s\n",eMess1);
}

//----------------------------------------------------------------
int EdbScanClientSysal::RcvLine(TSocket* sock, char* line, int size)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return 0;
  // Receave the line from socket; return the number of bytes receaved
  char chr=0;
  int i=0;
  if(!(sock->IsValid())) return -1;
  while( chr!=10 ) { sock->RecvRaw(&chr,1); if(i<size-1) { line[i]=chr; i++;}  } 
  line[i]=0;
  return i;
}

//----------------------------------------------------------------
int EdbScanClientSysal::UnloadPlate()
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return 0;
  sprintf(eCMD,"202\n");
  printf("%s: UnloadPlate\n",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  return 1;  //!!! todo
}

//----------------------------------------------------------------
int EdbScanClientSysal::LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return 0;
  Int_t attempt=1;
  eMess1[0]=0;

  while( (attempt<nAttempts+1) &&  (eMess1[0]!='O') )
    {
      printf("Plate %d brick %d : load attempt %d (of %d)\n",PLATE,BRICK,attempt,nAttempts);
      sprintf(eCMD,"201 %d %d 0 0 gg %s\n",ShortBrick(BRICK),PLATE,mapext);
      printf("%s",eCMD);
      eSock->SendRaw(eCMD,strlen(eCMD));
      RcvLine(eSock,eMess,sizeof(eMess));
      RcvLine(eSock,eMess1,sizeof(eMess1));
      attempt++;
    }
  if(eMess1[0]!='O') {printf("Error loading the plate. \n"); return -1;}
  printf("Plate loaded\n");
  return 1;
}

//----------------------------------------------------------------
void EdbScanClientSysal::SetParameter(const char* Object, const char* Parameter, const char* Value)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return;
  memset(eCMD,0,256);
  sprintf(eCMD,"203 %s %s %s\n",Object,Parameter,Value);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
}

//----------------------------------------------------------------
void  EdbScanClientSysal::SetClusterThresholds(int TOP, int BOT)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return;
  sprintf(eCMD,"203 VertigoScanner TopClusterThreshold  %d\n",TOP);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
  sprintf(eCMD,"203 VertigoScanner BottomClusterThreshold %d\n",BOT);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
}

//----------------------------------------------------------------
void  EdbScanClientSysal::SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return;
  sprintf(eCMD,"203 Frame_Grabber VPProgram%d 0008000804F003F0%4.4X%02d%02d\n",itop,TOP,size,size);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
  sprintf(eCMD,"203 Frame_Grabber VPProgram%d 0008000804F003F0%4.4X%02d%02d\n",ibottom,BOT,size,size);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
}

//----------------------------------------------------------------
int EdbScanClientSysal::SetFragmentSize(int X, int Y)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return 0;
  eNXview = X;
  eNYview = Y;
  eCMD[0]=0;
  sprintf(eCMD,"203 VertigoScanner XFields %d\n",X);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
  sprintf(eCMD,"203 VertigoScanner YFields %d\n",Y);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess); 
  return 1; //todo!!!
}

//-------------------------------------------------------------------
int EdbScanClientSysal::ScanAreas(ScanType st, int id[4], EdbPattern &areas, EdbRun *run, const char* options)
{
  // this function scan the list of areas and save into  EdbRun
  // the run should be already correctly opened
  // prediction id will be used for the temporary file name construction and so should be unique
  // return number of correctly scanned and converted areas

  int n = areas.N();
  printf("ScanAreas: %d \n",n);
  int scanned=0, failed=0;
  EdbSegP *s = 0;
  EdbSegP *sn = 0;
  char str[256];
  for(int i=0; i<n; i++) {
    s = areas.GetSegment(i);

    if( (s->SX() < 0.5*eNXview*eXstep) || (s->SY() < 0.5*eNYview*eYstep) )  // set the fragments size for sysal
      s->SetErrors(0.5*eNXview*eXstep, 0.5*eNYview*eYstep, 0., .1, .1);

    if(i<n-1) sn = areas.GetSegment(i+1); 
    else  sn = areas.GetSegment(i);

#ifdef WIN32
    sprintf(str,"del %s/raw.%d.%d.%d.%d.*",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID()); // s->ID() must be unique!
#else
    sprintf(str,"rm -f %s/raw.%d.%d.%d.%d.*",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID()); // s->ID() must be unique!
#endif

    gSystem->Exec(str);
    printf("ScanAreas: scan progress: %d out of %d (%4.1f%%)\n",i,n,100.*i/n);
    sprintf(str,"%s/raw.%d.%d.%d.%d",eRawDirServer.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
    
    if( !ScanPreloadAreaS( id[0], id[1], id[2], s->ID(),
			   s->X()-s->SX(), s->X()+s->SX(), s->Y()-s->SY(), s->Y()+s->SY(), 
			   str,sn->X()-sn->SX(), sn->X()+sn->SX(), sn->Y()-sn->SY(), sn->Y()+sn->SY() ) )  {
      //      i--; //? Igor added this line?
      printf("EdbScanClient::ScanAreas: WARNING!!! scanning failed for area %d (%d.%d.%d.%d)!\n",
	     i,  id[0], id[1], id[2], s->ID());
      if(++failed > eMAXFAILS)  {
	printf("EdbScanClient::ScanAreas: ERROR!!! too many failures - stop scanning!\n");
	break;
      }
      continue;  // still try to scan other predictions
    }
    sprintf(str,"%s/raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
    run->GetHeader()->SetFlag(9, s->ID());
    run->GetHeader()->SetFlag(8, s->MCEvt());
    AddRWC(run,str,true,options);
    scanned++;
  }
  
  return scanned;
}

//----------------------------------------------------------------
void EdbScanClientSysal::AsyncScanAreaS(  int id1, int id2, int id3, int id4, 
								float x1, float x2, float y1, float y2, 
								const char *fname)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return;
  // send the instruction for area scan and do not wait for the result
  memset(eCMD,0,256);
  sprintf(eCMD,"101 %d %d %d %d %f %f %f %f %s\n", ShortBrick(id1),id2,id3,id4,x1,x2,y1,y2,fname);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  return;
}

//----------------------------------------------------------------
void EdbScanClientSysal::AsyncScanPreloadAreaS(  int id1, int id2, int id3, int id4, 
					    float x1, float x2, float y1, float y2, const char *fname,
					    float x1n, float x2n, float y1n, float y2n)
{
  if(!eSock)
    InitializeSocket();
  if(!eSock)
    return;
  memset(eCMD,0,256);
  sprintf(eCMD,"102 %d %d %d %d %f %f %f %f %s %f %f %f %f\n", 
	  ShortBrick(id1),id2,id3,id4,x1,x2,y1,y2,fname,x1n,x2n,y1n,y2n);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  return;
}

//----------------------------------------------------------------
int EdbScanClientSysal::AsyncWaitForScanResult()
{
  memset(eMess,0,256);
  memset(eMess1,0,256);
  RcvLine(eSock,eMess,sizeof(eMess));
  RcvLine(eSock,eMess,sizeof(eMess));
  RcvLine(eSock,eMess1,sizeof(eMess1));
  if(eMess1[0]!='O') {printf("Error scanning the plate! return 0\n"); return 0;}
  printf("Area scan complete\n");
  return 1;
}
