#include "TSocket.h"
#include "TString.h"
#include "EdbRun.h"
#include "EdbPattern.h"

#ifndef ROOT_EdbScanClientSysal
#define ROOT_EdbScanClientSysal

#include "EdbScanClient.h"
#include "MessageManager.h"
#include "MessageReader.h"

class EdbScanClientSysal:public EdbScanClientCommon
{
public: 
	EdbScanClientSysal(){};
  virtual ~EdbScanClientSysal(){};

	int    RcvLine(TSocket* sock, char* line, int size);

	virtual int    UnloadPlate();
	virtual int    LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts=1);
  virtual void   SetParameter(const char* Object, const char* Parameter, const char* Value);
  virtual void   SetClusterThresholds(int TOP, int BOT);
  virtual void   SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT);
  virtual int    SetFragmentSize(int X, int Y);

  virtual int    ScanAreas(ScanType st, int id[4], EdbPattern &areas, EdbRun *run, const char* options="");
  virtual void   AsyncScanAreaS( int id1, int id2, int id3, int id4,	
						 float x1, float x2, float y1, float y2,	const char *fname);
  virtual void   AsyncScanPreloadAreaS( int id1, int id2, int id3, int id4,	
								 float x1, float x2, float y1, float y2,	const char *fname, 
								 float x1n, float x2n, float y1n, float y2n);
  virtual int    AsyncWaitForScanResult();
	virtual void   Print();
  virtual bool ServerCreatesTarget(){return false;};


  ClassDef(EdbScanClientSysal,1)  // remote scanning from Bern
};

#endif
