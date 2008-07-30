#ifndef ROOT_EdbScanClient
#define ROOT_EdbScanClient

#include "TSocket.h"
#include "TString.h"
#include "EdbRun.h"
#include "EdbPattern.h"

class EdbScanClient
{
public:
  TSocket*  eSock;          // socket for connection to scanning mashine
  char      eCMD[256];      // command line
  char      eMess[256];     // message line
  char      eMess1[256];    // message line

public:
  float       eNXview, eNYview;   // the fragment size in view
  float       eXstep, eYstep;     // between views in the fragment - to calculate the fragment size

  int       eMAXSCANATTEMPTS;
  int       eMAXFAILS;          // the max number for predictions failed to scan before break
  int       ePORT;              // port number where BernScanDriver expect connection (default 1777)
  TString   eServer;            // address of the scanning mashine

  TString eRawDirServer;     // directory path for raw data files visible from the Scan Server (i.e. "o:/MIC5")
  TString eRawDirClient;     // directory path for raw data files visible from processing comp (i.e. "./raw")

public: 
  EdbScanClient();
  virtual ~EdbScanClient(){}

  int    InitializeSocket();
  int    RcvLine(TSocket* sock, char* line, int size);
  int    UnloadPlate();
  int    LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts=1);
  void   SetParameter(char* Object, char* Parameter, char* Value);
  void   SetClusterThresholds(int TOP, int BOT);
  void   SetOdysseyThresholds(int TOP, int BOT);
  int    SetFragmentSize(int X, int Y);
  void   AsyncScanAreaS( int id1, int id2, int id3, int id4,	
						 float x1, float y1, float x2, float y2,	const char *fname);
  int    ScanAreaS( int id1, int id2, int id3, int id4,	
					float x1, float y1, float x2, float y2,	const char *fname);
  void   AsyncScanPreloadAreaS( int id1, int id2, int id3, int id4,	
								float x1, float y1, float x2, float y2,	const char *fname, 
								float x1n, float y1n, float x2n, float y2n);
  int    ScanPreloadAreaS( int id1, int id2, int id3, int id4,
							float x1, float y1, float x2, float y2,	const char *fname, 
							float x1n, float y1n, float x2n, float y2n);
  int    AsyncWaitForScanResult();

  int   ScanAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options="");
  int   ScanAreasAsync(int id[4], EdbPattern &areas, EdbRun &run, const char* options="");
  int   ConvertAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options="");

// "short" version of brickID to be passed into sysal command line via BernScanDriver
  Short_t  ShortBrick(Int_t brick){ return brick%10000; }

  static int     AddRWC_(EdbRun* run, char* rwcname, int bAddRWD=true, const char* options="");

  void   Print();

  ClassDef(EdbScanClient,1)  // remote scanning from Bern
};

#endif /* ROOT_EdbScanClient */
