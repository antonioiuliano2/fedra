
#include "dataIO/dataIO.h"  

#include <math.h>
#include <EdbRun.h>
#include <EdbRunHeader.h>
#include <EdbView.h>
#include <EdbCluster.h>
#include <EdbSegment.h>

using namespace std;

double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem);
int AddRWC(EdbRun* run, char* rwcname, int bAddRWD=true, const char* options="");
int AddRWD(EdbRun* run, char* rwdname, int fragID=0, const char* options="");
//int AddTLG(EdbRun* run, char* mapname);
int AddGrainsTXT(EdbRun* run, char* txtname);
int AddMAP(EdbRun* run, char* mapname);
