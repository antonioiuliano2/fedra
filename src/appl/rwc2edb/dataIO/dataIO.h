#ifdef _USESYSAL
   #include <windows.h>
   #include "SySalDataIO.h"
   #include "vscan_ds.h"  
#else
   #include <inttypes.h>
   #include "vscan_ds.linux.h"  
#endif

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int ReadMap(void** ppData,  char* name);
int ReadFragment(void** ppData,  char* name);
int ReadCatalog (void** ppData,  char* name);
int FreeMemory  (void** ppData);

#ifdef _USESYSAL
#else
   void   TRY_READ_B(FILE* HFile, int br, void* dst, int size);
   char*  ED_COPY_AND_DISPLACE(void* dst, void* src, int size);
#endif

