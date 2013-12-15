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
  if(!strcmp(Object, "VertigoScanner"))
    return;
    //prevent too many fake sysal stuff
	printf("%s[%s]=%s\n", Object, Parameter, Value);
	m_mm.AddSetModuleParamsNode(Object, Parameter, Value, NULL_TRM);
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
int EdbScanClientPav::ScanAreas(EdbScanClientBase::ScanType st, int id[4], EdbPattern &areas, EdbRun *run, const char* options)
{
  // this function scans the FIRST area from the list. Saving is done by server-side
  // for propper interaction the run  filename should be set in EdbScanProc::SetServerRunName fn
  // returns 1 if scan was successful

  int n = areas.N();
  printf("ScanAreas: %d \n",n);
  if(n == 0){
    printf("Nothing to scan.\n");
    return 0;
  }
  int scanned=0, failed=0;
  EdbSegP *s = 0;
  char str[256];
  if(st == stVolume){
    if(n!=1)
      printf("WARNING! Only first prediction is scanned in Volume mode");
    s = areas.GetSegment(0);
    printf("ScanAreas: scan progress: 0 out of 1\n");
    sprintf(str,"%s/raw.%d.%d.%d.%d",eRawDirServer.Data(),ShortBrick(id[0]), id[1], id[2], s->ID());
    while( !ScanPreloadAreaS( id[0], id[1], id[2], s->ID(),
        s->X()-s->SX(), s->X()+s->SX(), s->Y()-s->SY(), s->Y()+s->SY(), 
        str,s->X()-s->SX(), s->X()+s->SX(), s->Y()-s->SY(), s->Y()+s->SY() ) && failed < eMAXFAILS)
      failed++;
    if(failed >= eMAXFAILS)
      printf("EdbScanClient::ScanAreas: ERROR!!! too many failures - stop scanning!\n");
    else
      scanned = 1;
  }else{
    s = areas.GetSegment(0);
    float dx = s->SX(), dy = s->SY();
    for(int i=1; i<n; i++){
      s = areas.GetSegment(1);
      if(s->SX() > dx)
        dx = s->SX();
      if(s->SY() > dy)
        dy = s->SY();
    }
    while( !ScanFromPrediction( id[0], id[1], id[2], id[3], dx, dy) && failed < eMAXFAILS)
      failed++;
    if(failed >= eMAXFAILS)
      printf("EdbScanClient::ScanAreas: ERROR!!! too many failures - stop scanning!\n");
    else
      scanned = n;
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

  SetServerTarget();
  
	char area[100];
	sprintf(area, "%f %f %f %f", x1, y1, x2, y2); // here goes xmin, ymin, xmax, ymax
	m_mm.AddSetPathParamsNode(m_pathLib.c_str(), m_pathName.c_str(), "area", area, NULL_TRM);

	m_mm.AddStartNode(m_pathLib.c_str(), m_pathName.c_str(), "");
	m_mm.FillBuff();

  AsyncStartScan();
}

//----------------------------------------------------------------
bool EdbScanClientPav::ScanFromPrediction(int id1, int id2, int id3, int id4, float dx, float dy){
  if(!eSock)
		InitializeSocket();
	if(!eSock)
		return false;

  SetServerTarget();

	char pars[200];
	sprintf(pars, "%f %f", dx, dy);
	m_mm.AddSetPathParamsNode(m_pathLib.c_str(), m_predPathName.c_str(), "pred_sigma", pars, NULL_TRM);

	sprintf(pars, "%u %u %u %u", id1, id2, id3, id4);
	m_mm.AddSetPathParamsNode(m_pathLib.c_str(), m_predPathName.c_str(), "pred_id", pars, NULL_TRM);

	m_mm.AddStartNode(m_pathLib.c_str(), m_predPathName.c_str(), "");
	m_mm.FillBuff();

  AsyncStartScan();
  
  if(AsyncWaitForScanResult()==1)
    return true;
  else
    return false;
}

//----------------------------------------------------------------
void EdbScanClientPav::AsyncStartScan()
{
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
		printf("bad server response, stopping\n");
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
		printf("scanning not started, some error occurred\n");
		m_scanningStarted = false;
		break;

	}
}

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

void EdbScanClientPav::SetServerTarget(){
  std::string spath(eRawPthServer.Data());         //   /opera/ONLINE/b800000/p009/asd.root
  if(spath.find(eRawDirClient.Data()) == 0){
    spath = spath.substr(eRawDirClient.Length());  //   /b800000/p009/asd.root
    while(spath.find("/") == 0)
      spath = spath.substr(1);                     //   b800000/p009/asd.root

    std::string path(eRawDirServer.Data());        //   S:/
    int f1 = path.find("/", path.length()-1);
    int f2 = path.find("\\", path.length()-1);
    if(f1 == -1 && f2 == -1)
      path += "/";
    path += spath;                                 //   S:\b800000/p009/asd.root

    m_mm.AddSetModuleParamsNode("processor", "clprc.output_file", path.c_str(), NULL_TRM);

  }else{
    m_createdTarget = false;
    printf("Error in target path format, raw dir client prefix not found.\n");
    return;
  }

  m_mm.AddGetModuleParamsNode("processor", "clprc.output_file", NULL_TRM);
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

	std::string key = PAVPROT_NODE_GMP;
	key += "R.processor.clprc.output_file";
	if(m_mr.GetValueForKey(key).length() != 0){
	    printf("Server creates target file.\n");
      m_createdTarget = true;
	}else{
	    m_createdTarget = false;
	    printf("Server doesn't create target file. Please update to speedup.\n");
  }
};
