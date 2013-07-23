//-- Author :  Mykhailo Vladymyrov   18/07/2013
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanClientBase, EdbScanClientCommon                               //
//                                                                      //
// remote scanning library base class that privides library interface   //
// and common class with implementation of clients' common functions    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "EdbScanClient.h"
#include "libDataConversion.h"

ClassImp(EdbScanClientCommon)
//----------------------------------------------------------------
EdbScanClientCommon::EdbScanClientCommon()
{
  eSock=0;
  eCMD[0]=0;
  eMess[0]=0;
  eMess1[0]=0;

  eMAXSCANATTEMPTS = 3; // max number of trials to scan an area 
  eMAXFAILS        = 5; // max number of predictions (areas) failed before abort scanning
  ePORT=1777;

  eNXview = 1;
  eNYview = 1;
  eXstep = 360; // This numbers should be equal or a bit smalle then the correspondent sysal settings
  eYstep = 275;
}

//----------------------------------------------------------------

EdbScanClientCommon::~EdbScanClientCommon(){
  CloseSocket();
}

void EdbScanClientCommon::CloseSocket(){
	if(eSock){
		eSock->Close();
		delete eSock;
		eSock=0;
	} 
}

int EdbScanClientCommon::InitializeSocket()
{
  // establish the connection to server using ePORT
  if(!eSock) eSock=new TSocket(eServer,ePORT);
  if(!(eSock->IsValid())) {
		 CloseSocket();
    printf("No connection to server %s with port %d ;  Exiting.\n",eServer.Data(),ePORT);
    return 0;
  }
  return 1;
}

//----------------------------------------------------------------
int EdbScanClientCommon::ScanPreloadAreaS( int id1, int id2, int id3, int id4,  
				     float x1, float x2, float y1, float y2, const char *fname,
				     float x1n, float x2n, float y1n, float y2n)
{
  eMess1[0]=0;
  for(int attempt=0; attempt<eMAXSCANATTEMPTS; attempt++)
    {
      AsyncScanPreloadAreaS(id1,id2,id3,id4,x1,x2,y1,y2,fname,x1n,x2n,y1n,y2n);
      if(AsyncWaitForScanResult()==1)  return 1;
    }
  return 0;
}

//----------------------------------------------------------------
/*int EdbScanClientCommon::ScanAreaS( int id1, int id2, int id3, int id4,  
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
*/
//-------------------------------------------------------------------
/*int EdbScanClientCommon::ScanAreasAsync(int id[4], EdbPattern &areas, EdbRun &run, const char* options)
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
	sprintf(str,"%s\\raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
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

#ifdef WIN32
    sprintf(str,"del %s/raw.%d.%d.%d.%d.*",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID()); // s->ID() must be unique!
#else
    sprintf(str,"rm -f %s/raw.%d.%d.%d.%d.*",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID()); // s->ID() must be unique!
#endif

    gSystem->Exec(str);
    sprintf(str,"%s\\raw.%d.%d.%d.%d",eRawDirServer.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
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
*/
//----------------------------------------------------------------
int EdbScanClientCommon::ConvertAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options)
{
  // this function converts all scanned areas and save them into the given EdbRun
  int n = areas.N();
  printf("ConvertAreas: %d \n",n);
  EdbSegP *s = 0;
  char str[256];
  int ic=0;
  for(int i=0; i<n; i++) {
    s = areas.GetSegment(i);
    sprintf(str,"%s/raw.%d.%d.%d.%d.rwc",eRawDirClient.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
    run.GetHeader()->SetFlag(9, s->ID());
    run.GetHeader()->SetFlag(8, s->MCEvt());
    AddRWC(&run,str,true,options);
    ic++;
  }
  return ic;
}
//-------------------------------------------------------------------
int EdbScanClientCommon::AddRWC_(EdbRun* run, char* rwcname, int bAddRWD, const char* options)
    { return AddRWC(run,rwcname, bAddRWD, options); }

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

