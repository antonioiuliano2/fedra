#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "vscan_ds.h"  

#include <EdbRun.h>
#include <EdbRunHeader.h>
#include <EdbView.h>
#include <EdbCluster.h>
#include <EdbSegment.h>

using namespace std;

double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem);
int AddRWC(EdbRun* run, char* rwcname, int bAddRWD=true);
int AddRWD(EdbRun* run, char* rwdname, int fragID=0);
int AddMAP(EdbRun* run, char* mapname);
int AddTLG(EdbRun* run, char* mapname);
int AddGrainsTXT(EdbRun* run, char* txtname);
int ReadFragment(void** ppData,  char* name);
int ReadCatalog(void** ppData,  char* name);
void TRY_READ_B(FILE* HFile, int br, void* dst, int size);
char*  ED_COPY_AND_DISPLACE(void* dst, void* src, int size);
	
