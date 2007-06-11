//-- Author :  Valeri Tioukov   22/12/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanClient                                                        //
//                                                                      //
// remote scanning library based on the code of scripts "scanlib.C"     //
// written by Igor Kreslo and interacting with BernScanDriver           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "EdbScanClient.h"
#include "libDataConversion.h"

ClassImp(EdbScanClient)
//----------------------------------------------------------------
EdbScanClient::EdbScanClient()
{
  eSock=0;
  eCMD[0]=0;
  eMess[0]=0;
  eMess1[0]=0;

  eMAXSCANATTEMPTS = 3; // max number of trials to scan an area 
  eMAXFAILS        = 5; // max number of predictions (areas) failed before abort scanning
  ePORT=1777;
}

//----------------------------------------------------------------
void EdbScanClient::Print()
{
  printf("EdbScanClient:\n");
  printf("Server: %s  PORT: %d\n",eServer.Data(),ePORT);
  printf("CMD:    %s\n",eCMD);
  printf("mess:   %s\n",eMess);
  printf("mess1:  %s\n",eMess1);
}

//----------------------------------------------------------------
int  EdbScanClient::InitializeSocket()
{
  // establish the connection to server using ePORT
  if(!eSock) eSock=new TSocket(eServer,ePORT);
  if(!(eSock->IsValid())) {
    eSock->Close(); eSock=0; 
    printf("No connection to server %s with port %d ;  Exiting.\n",eServer.Data(),ePORT);
    return 0;
  }
  return 1;
}

//----------------------------------------------------------------
int EdbScanClient::RcvLine(TSocket* sock, char* line, int size)
{
  // Receave the line from socket; return the number of bytes receaved
  char chr=0;
  int i=0;
  if(!(sock->IsValid())) return -1;
  while( chr!=10 ) { sock->RecvRaw(&chr,1); if(i<size-1) { line[i]=chr; i++;}  } 
  line[i]=0;
  return i;
}

//----------------------------------------------------------------
int EdbScanClient::UnloadPlate()
{
  sprintf(eCMD,"202\n");
  printf("%s: UnloadPlate\n",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  return 1;  //!!! todo
}

//----------------------------------------------------------------
int EdbScanClient::LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts)
{
  Int_t attempt=1;
  eMess1[0]=0;
  while( (attempt<nAttempts+1) &&  (eMess1[0]!='O') )
    {
      printf("Plate %d brick %d : load attempt %d (of %d)\n",PLATE,BRICK,attempt,nAttempts);
      sprintf(eCMD,"201 %d %d 0 0 gg %s\n",BRICK,PLATE,mapext);
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
void EdbScanClient::SetParameter(char* Object, char* Parameter, char* Value)
{
  memset(eCMD,0,256);
  sprintf(eCMD,"203 %s %s %s\n",Object,Parameter,Value);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  RcvLine(eSock,eMess,sizeof(eMess));
  printf("%s",eMess);
}

//----------------------------------------------------------------
void  EdbScanClient::SetClusterThresholds(int TOP, int BOT)
{
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
int EdbScanClient::SetFragmentSize(int X, int Y)
{
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

//----------------------------------------------------------------
void EdbScanClient::AsyncScanAreaS(  int id1, int id2, int id3, int id4, 
								float x1, float y1, float x2, float y2, 
								const char *fname)
{
  // send the instruction for area scan and do not wait for the result
  memset(eCMD,0,256);
  sprintf(eCMD,"101 %d %d %d %d %f %f %f %f %s\n", id1,id2,id3,id4,x1,y1,x2,y2,fname);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  return;
}

//----------------------------------------------------------------
int EdbScanClient::ScanAreaS( int id1, int id2, int id3, int id4,  
						float x1, float y1, float x2, float y2, 
						const char *fname)
{
  // scan area and wait for the result
  eMess1[0]=0;
  for(int attempt=0; attempt<eMAXSCANATTEMPTS; attempt++)
    {
	  printf("attempt %d\n",attempt);
	  AsyncScanAreaS(id1,id2,id3,id4,x1,y1,x2,y2,fname);
      if(AsyncWaitForScanResult()==1)  return 1;
    }
  return 0;
}

//----------------------------------------------------------------
void EdbScanClient::AsyncScanPreloadAreaS(  int id1, int id2, int id3, int id4, 
					    float x1, float y1, float x2, float y2, const char *fname,
					    float x1n, float y1n, float x2n, float y2n)
{
  memset(eCMD,0,256);
  sprintf(eCMD,"102 %d %d %d %d %f %f %f %f %s %f %f %f %f\n", 
	  id1,id2,id3,id4,x1,y1,x2,y2,fname,x1n,y1n,x2n,y2n);
  printf("%s",eCMD);
  eSock->SendRaw(eCMD,strlen(eCMD));
  return;
}

//----------------------------------------------------------------
int EdbScanClient::ScanPreloadAreaS( int id1, int id2, int id3, int id4,  
				     float x1, float y1, float x2, float y2, const char *fname,
				     float x1n, float y1n, float x2n, float y2n)
{
  eMess1[0]=0;
  for(int attempt=0; attempt<eMAXSCANATTEMPTS; attempt++)
    {
      AsyncScanPreloadAreaS(id1,id2,id3,id4,x1,y1,x2,y2,fname,x1n,y1n,x2n,y2n);
      if(AsyncWaitForScanResult()==1)  return 1;
    }
  return 0;
}

//----------------------------------------------------------------
int EdbScanClient::AsyncWaitForScanResult()
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

//-------------------------------------------------------------------
int EdbScanClient::ScanAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options)
{
  // this function scan the list of areas and save into  EdbRun
  // the run should be already correctly opened, predictions are assumed to be in the stage coord
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
    if(i<n-1) sn = areas.GetSegment(i+1); 
    else  sn = areas.GetSegment(i);
    sprintf(str,"rm -f %s/raw.%d.%d.%d.%d.*",eRawDirClient.Data(),id[0], id[1], id[2], s->ID()); // s->ID() must be unique!
    gSystem->Exec(str);
    printf("ScanAreas: scan progress: %d out of %d (%4.1f%%)\n",i,n,100.*i/n);
    sprintf(str,"%s/raw.%d.%d.%d.%d",eRawDirServer.Data(),id[0], id[1], id[2], s->ID());
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
    sprintf(str,"%s/raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),id[0], id[1], id[2], s->ID());
    run.GetHeader()->SetFlag(9, s->ID());
    run.GetHeader()->SetFlag(8, s->MCEvt());
    AddRWC(&run,str,true,options);
    scanned++;
  }
  
  return scanned;
}

//-------------------------------------------------------------------
int EdbScanClient::ScanAreasAsync(int id[4], EdbPattern &areas, EdbRun &run, const char* options)
{
  // this function scan the list of areas and save into the same EdbRun
  // the run should be already correctly opened, predictions are assumed to be in the stage coord
  // prediction id will be used for the temporary file name construction
  // return number of scanned areas

  int n = areas.N();
  printf("ScanAreasAsync: %d \n",n);
  if(n<1) return 0;
  EdbSegP *s = 0, *s1=0;
  char str[256];
  int ic=0;
  for(int i=0; i<=n; i++) {
    if(s) {
      if(AsyncWaitForScanResult()==1) {
	sprintf(str,"%s\\raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),id[0], id[1], id[2], s->ID());
	run.GetHeader()->SetFlag(9, s->ID());
	run.GetHeader()->SetFlag(8, s->MCEvt());
	AddRWC(&run,str,true,options);
      } else printf("Error: ScanAreasAsync: result %s didn't receaved!\n",str);
    }
    if(i>=n) break;
    else {
      s = areas.GetSegment(i);
      if(i+1<n) s1=areas.GetSegment(i+1);
      else      s1=0;
    }
    sprintf(str,"del %s\\raw.%d.%d.%d.%d.*",eRawDirClient.Data(),id[0], id[1], id[2], s->ID());
    gSystem->Exec(str);
    sprintf(str,"%s\\raw.%d.%d.%d.%d",eRawDirServer.Data(),id[0], id[1], id[2], s->ID());
    if(s1) {
      AsyncScanPreloadAreaS( id[0], id[1], id[2], s->ID(),  
			     s->X()-s->SX(), s->X()+s->SX(), s->Y()-s->SY(), s->Y()+s->SY(), str,
			     s1->X()-s1->SX(), s1->X()+s1->SX(), s1->Y()-s1->SY(), s1->Y()+s1->SY());
    } else {
      AsyncScanAreaS( id[0], id[1], id[2], s->ID(),  
		      s->X()-s->SX(), s->X()+s->SX(), s->Y()-s->SY(), s->Y()+s->SY(), 
		      str );
    }
    ic++;    
  }
  return ic;
}

//-------------------------------------------------------------------
int EdbScanClient::ConvertAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options)
{
  // this function converts all scanned areas and save them into the given EdbRun
  int n = areas.N();
  printf("ConvertAreas: %d \n",n);
  EdbSegP *s = 0;
  char str[256];
  int ic=0;
  for(int i=0; i<n; i++) {
    s = areas.GetSegment(i);
    sprintf(str,"%s/raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),id[0], id[1], id[2], s->ID());
    run.GetHeader()->SetFlag(9, s->ID());
    run.GetHeader()->SetFlag(8, s->MCEvt());
    AddRWC(&run,str,true,options);
    ic++;
  }
  return ic;
}
