//-- Author :  Mykhailo Vladymyrov   18/07/2013
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanClientPav                                                     //
//                                                                      //
// remote scanning client class for LASSO				//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "EdbScanClientPav.h"
#include "libDataConversion.h"
#include "MessageManager.h"
#include "MessageReader.h"
#include <math.h>

ClassImp(EdbScanClientPav)
//----------------------------------------------------------------
void EdbScanClientPav::Print()
{
  printf("EdbScanClientPav:\n");
  printf("Server: %s  PORT: %d\n",eServer.Data(),ePORT);
}

//----------------------------------------------------------------
int EdbScanClientPav::UnloadPlate()
{
	printf("UnloadPlate: not implemented yet.\n");
	return 1;
}

//----------------------------------------------------------------
int EdbScanClientPav::LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts)
{
	m_mm.AddMarksNode(BRICK, PLATE, mapext);
	return 1;
}

//----------------------------------------------------------------
void EdbScanClientPav::SetParameter(const char* Object, const char* Parameter, const char* Value)
{
	printf("%s[%s]=%s\n", Object, Parameter, Value);
	m_mm.AddSetModuleParamsNode(Object, Parameter, Value, NULL_TRM);
	printf("+\n");
}

//----------------------------------------------------------------
void  EdbScanClientPav::SetClusterThresholds(int TOP, int BOT)
{
  printf("Default values are used from PAVICOM config.\n");
}

//----------------------------------------------------------------
void  EdbScanClientPav::SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT)
{
	char val[20];
	sprintf(val, "%d %d", BOT, TOP);
  printf("only TOP and BOT thresholds are used in PAVICOM.\n");
	SetParameter("processor", "clz_thres", val);
}

//----------------------------------------------------------------
int EdbScanClientPav::SetFragmentSize(int X, int Y)
{
	return 1;
}

//-------------------------------------------------------------------
int EdbScanClientPav::ScanAreas(int id[4], EdbPattern &areas, EdbRun *run, const char* options)
{
  // this function scans the FIRST area from the list. Saving is done by server-side
  // for propper interaction the run  filename should be set in EdbScanProc::SetServerRunName fn
	// predictions are assumed to be in the stage coord
  // returns 1 if scan was successful

  int n = areas.N();
  printf("ScanAreas: %d \n",n);
  if(n>1)
		printf("WARNING: only first area will be scanned!\n");
  int scanned=0, failed=0;
  EdbSegP *s = 0;
  EdbSegP *sn = 0;
  char str[256];
  for(int i=0; i<1; i++) {
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
    scanned++;
  }
  
  return scanned;
}

//----------------------------------------------------------------
void EdbScanClientPav::AsyncScanAreaS(  int id1, int id2, int id3, int id4, 
								float x1, float x2, float y1, float y2, 
								const char *fname)
{
	printf("AsyncScanAreaS: not implemented yet, use AsyncScanPreloadAreaS\n");
}

//----------------------------------------------------------------
void EdbScanClientPav::AsyncScanPreloadAreaS(  int id1, int id2, int id3, int id4, 
					    float x1, float x2, float y1, float y2, const char *fname,
					    float x1n, float x2n, float y1n, float y2n)
{
	if(!eSock)
		InitializeSocket();
	if(!eSock)
		return;


	char area[100];
	sprintf(area, "%f %f %f %f", x1, y1, x2, y2); // here goes xmin, ymin, xmax, ymax
	m_mm.AddSetPathParamsNode(m_pathLib.c_str(), m_pathName.c_str(), "area", area, NULL_TRM);

	m_mm.AddStartNode(m_pathLib.c_str(), m_pathName.c_str(), "");
	m_mm.FillBuff();
	uint32 len = m_mm.GetBufSize();

	int res;
	if(eSock && eSock->IsValid()){
		eSock->SendRaw(&len, sizeof(len));
		eSock->SendRaw(m_mm.GetBuf(), len);
		m_mm.CreateMessage();

		int recvBytes = eSock->RecvRaw(&m_insize, sizeof(m_insize));
		if(recvBytes<=0){
			CloseSocket();
			return;
		}
		if(m_inbuf.size()<m_insize)
			m_inbuf.resize(m_insize);
		recvBytes = eSock->RecvRaw(&m_inbuf[0], m_insize);
		if(recvBytes<=0){
			CloseSocket();
			return;
		}
		if(m_insize>0 && m_inbuf.size()){
			printf("msg='%s'\n", &m_inbuf[8]);
			bool res1 = m_mr.Load(&m_inbuf[0], m_insize);
			if(!res1){
				CloseSocket();
				return;
			}
			m_mr.Process();
		}

	};

	std::string startkey = PAVPROT_NODE_START;
	startkey += "R";
	int startcode;
	const char *startstr = m_mr.GetValueForKey(startkey).c_str();
	res = sscanf(startstr, "%d", &startcode);
	if(!strlen(startstr))
		startcode = PAVPROT_OK;
	else if(res != 1){
		printf("bad server responce, stopping\n");
		return;
	};
	switch(startcode){
	case PAVPROT_OK:
		printf("scanning started, waiting it to finish...\n");
		m_scanningStarted = true;
		break;
		
	case PAVPROT_BUSY:
		printf("Scanning not started: server is busy\n");
		m_scanningStarted = false;
		break;

	case PAVPROT_ERROR:
	default:
		printf("scanning not started, some error occured\n");
		m_scanningStarted = false;
		break;

	}
}

//----------------------------------------------------------------
int EdbScanClientPav::AsyncWaitForScanResult()
{
	if(!m_scanningStarted)
		return 0;
	if(eSock) if(eSock->IsValid()){
		int recvBytes = eSock->RecvRaw(&m_insize, sizeof(m_insize));
		if(recvBytes<=0){
			CloseSocket();
			return 0;
		}
		
		if(m_inbuf.size()<m_insize)
			m_inbuf.resize(m_insize);
		recvBytes = eSock->RecvRaw(&m_inbuf[0], m_insize);
		if(recvBytes<=0){
			CloseSocket();
			return 0;
		}

		if(m_insize>0 && m_inbuf.size()){
			printf("msg='%s'\n", &m_inbuf[8]);
			bool res = m_mr.Load(&m_inbuf[0], m_insize);
			if(!res){
				CloseSocket();
				return 0;
			}
			m_mr.Process();
		}
	};
	m_scanningStarted = false;

	return 1;
}

