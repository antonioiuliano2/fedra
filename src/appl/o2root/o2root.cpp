//-- Author :  Valeri Tioukov   6/06/2008

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "TOracleServerE2.h"
#include "EdbLog.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  o2root -v=VOLUME [-ma=MAJOR -mi=MINOR -o=OUTDIR -rdb=RDB] \n";
  cout<< "\t  o2root -p=PROCESSOPERATION [-ma=MAJOR -mi=MINOR -o=OUTDIR -rdb=RDB] \n";
  cout<< "\t  o2root -P=PARENTPROCESSOPERATION [-ma=MAJOR -mi=MINOR -o=OUTDIR -rdb=RDB] \n";
  cout<< "\t  o2root -sb=PATH -brick=BRICK [-ma=MAJOR -mi=MINOR -o=OUTDIR -rdb=RDB] \n";
  cout<< "\t  o2root -brick=BRICK -info \n\n";
  cout<< "\t\t  VOLUME  - volume id as stored in database \n";
  cout<< "\t\t  PATH    - the scanback path id \n";
  cout<< "\t\t  BRICK   - the brick ID (if scanback path should be extracted)\n";
  cout<< "\t\t  MAJOR   - major version (default is 0)\n";
  cout<< "\t\t  MINOR   - minor version (default is 0)\n";
  cout<< "\t\t  OUTDIR  - output directory - should exist and be writable (default is ./)\n";
  cout<< "\t\t  RDB     - link to the remote database (default is \"\")\n";
  cout<< "\nExample: \n";
  cout<< "\t  o2root -v=7000000000360012 -ma=20 -mi=10 -o=/scratch/BRICKS \n";
  cout<< "\n\t  the output of this example will be stored in the root files named as: \n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/p042/1029351.42.20.10.raw.root\n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/p043/1029351.43.20.10.raw.root\n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/ ....\n";
  cout<< "\nThe database connection parameters if not explicitly defined will be taken from .rootrc as: \n";
  cout<< "\t  o2root.dbname:      oracle://my_server/my_database_service \n";
  cout<< "\t  o2root.username:    db_username \n";
  cout<< "\t  o2root.password:    db_password \n";
  cout<< "\t  o2root.rdb:         @opita   <- (define it if you connect via remote db link)\n";
  cout<< "\t  o2root.outdir:      /scratch/BRICKS \n";
  cout<<endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  const char *dbname    = gEnv->GetValue("o2root.dbname"   , "dbname");
  const char *user      = gEnv->GetValue("o2root.username" , "username");
  const char *password  = gEnv->GetValue("o2root.password" , "password");
  const char *rdb       = gEnv->GetValue("o2root.rdb"      , "");
  const char *outdir    = gEnv->GetValue("o2root.outdir"   , "./");
  gEDBDEBUGLEVEL        = gEnv->GetValue("o2root.EdbDebugLevel" , 1);

  bool      do_volume   = false;
  bool      do_processoperation   = false;
  bool      do_parentprocessoperation   = false;
  bool      do_scanback = false;
  bool      do_brickinfo = false;
  ULong64_t id_volume = 0LL;
  ULong64_t processoperation = 0LL;
  ULong64_t parentprocessoperation = 0LL;
  int       major=0;
  int       minor=0;

  Long_t    id_brick=0;
  Int_t     path    =0;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-v=",3)) 
      {
	if(strlen(key)>3)       sscanf(key+3,"%lld",&id_volume);
	do_volume=true;
      }
    else if(!strncmp(key,"-p=",3)) 
      {
	if(strlen(key)>3)       sscanf(key+3,"%lld",&processoperation);
	do_processoperation=true;
      }
    else if(!strncmp(key,"-P=",3))
      {
	if(strlen(key)>3)       sscanf(key+3,"%lld",&parentprocessoperation);
	do_parentprocessoperation=true;
      }
    else if(!strncmp(key,"-o=",3)) 
      {
	if(strlen(key)>3)	outdir=key+3;
      }
    else if(!strncmp(key,"-ma=",4)) 
      {
	if(strlen(key)>4)	major=atoi(key+4);
      }
    else if(!strncmp(key,"-mi=",4)) 
      {
	if(strlen(key)>4)	minor=atoi(key+4);
      }
    else if(!strncmp(key,"-rdb=",5))
      {
	if(strlen(key)>5)	rdb=key+5;
      }
    else if(!strncmp(key,"-brick=",7))
      {
	if(strlen(key)>7)	id_brick=atol(key+7);
      }
    else if(!strncmp(key,"-sb=",4))
      {
	if(strlen(key)>4)	path=atol(key+4);
	do_scanback=true;
      }
     else if(!strncmp(key,"-info",5))
      {
        do_brickinfo=true;
      }
 }

  if(!(do_volume||do_scanback||do_processoperation||do_parentprocessoperation||do_brickinfo))   { print_help_message(); return 0; }

  TOracleServerE2 *db = new TOracleServerE2(dbname,user,password);
  if(!db) { Log(1,"o2root","ERROR: the database connection is failed!"); return 0; }
  db->eRTS=rdb;
  if(do_volume) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("Read volume: \t\t\t%lld \nfrom the database: \t\t%s%s \nand save into directory: \t%s \nwith versions: \t\t\t%d.%d\n",
	   id_volume, dbname, rdb, outdir, major,minor);
    printf("----------------------------------------------------------------------------\n\n");
    db->ConvertMicrotracksVolumeToEdb(id_volume, outdir, major, minor);
  }
   if(do_processoperation) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("Read processoperation: \t\t\t%lld \nfrom the database: \t\t%s%s \nand save into directory: \t%s \nwith versions: \t\t\t%d.%d\n",
	   processoperation, dbname, rdb, outdir, major,minor);
    printf("----------------------------------------------------------------------------\n\n");
    db->ConvertMicrotracksProcessToEdb(processoperation, outdir, major, minor);
  }
   if(do_parentprocessoperation) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("Read parentprocessoperation: \t\t\t%lld \nfrom the database: \t\t%s%s \nand save into directory: \t%s \nwith versions: \t\t\t%d.%d\n",
	   parentprocessoperation, dbname, rdb, outdir, major,minor);
    printf("----------------------------------------------------------------------------\n\n");
    db->ConvertMicrotracksParentProcessToEdb(parentprocessoperation, outdir, major, minor);
  }
 if(do_scanback) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("Read scanback path: \t\t%d of brick %ld \nfrom the database: \t\t%s%s \nand save into directory: \t%s \nwith versions: \t\t\t%d.%d\n",
	   path, id_brick, dbname, rdb, outdir, major,minor);
    printf("----------------------------------------------------------------------------\n\n");
    db->ConvertScanbackPathToEdb(id_brick, path, outdir, major, minor);
  }
 if(do_brickinfo) {
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("Print info for brick: \t%ld from the database: \t\t%s%s\n",
	   id_brick, dbname, rdb);
    printf("---------------------------------------------------------------------------------------------\n");
    db->PrintBrickInfoFull(id_brick,0);
  }
  delete db;
  return 1;
}
