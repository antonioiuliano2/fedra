#include <windows.h>
#include <iostream>
#include <math.h>
#include <fstream>

#include "vscan_ds.h"  
#include "SySalDataIO.h"

#include <EdbRun.h>
#include <EdbRunHeader.h>
#include <EdbView.h>
#include <EdbCluster.h>
#include <EdbSegment.h>

using namespace std;

double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem);
int AddRWC(EdbRun* run, char* rwcname, int bAddRWD=TRUE);
int AddRWD(EdbRun* run, char* rwdname, int fragID=0);
int AddMAP(EdbRun* run, char* mapname);
int AddTLG(EdbRun* run, char* mapname);
int AddGrainsTXT(EdbRun* run, char* txtname);
