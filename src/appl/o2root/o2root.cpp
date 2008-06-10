//-- Author :  Valeri Tioukov   6/06/2008

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "TOracleServerE2.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  o2root -vVOLUME [-maMAJOR -miMINOR -oOUTDIR -rdbRDB] \n\n";
  cout<< "\t\t  VOLUME  - volume id as stored in database \n";
  cout<< "\t\t  MAJOR   - major version (default is 0)\n";
  cout<< "\t\t  MINOR   - minor version (default is 0)\n";
  cout<< "\t\t  OUTDIR  - output directory - should exist and be writable (default is ./)\n";
  cout<< "\t\t  RDB     - link to the remote database (default is \"\")\n";
  cout<< "\nExample: \n";
  cout<< "\t  o2root -v7000000000360012 -ma20 -mi10 -o/scratch/BRICKS \n";
  cout<< "\n\t  the output of this example will be stored in the root files named as: \n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/p042/1029351.42.20.10.raw.root\n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/p043/1029351.43.20.10.raw.root\n";
  cout<< "\t\t  /scratch/BRICKS/b1029351/ ....\n";
  cout<< "\nThe database connection parameters if not explicitly defined will be taken from .rootrc as: \n";
  cout<< "\t  o2root.dbname:      oracle://my_server/my_database_service \n";
  cout<< "\t  o2root.username:    my_username \n";
  cout<< "\t  o2root.password:    my_password \n";
  cout<< "\t  o2root.rdb:         @opita01   <- (define it if you connect via remote db link)\n";
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

  bool      do_volume=false;
  ULong64_t id_volume = 0LL;
  int       major=0;
  int       minor=0;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-v",2)) 
      {
	if(strlen(key)>2)       
#if defined(R__WIN32)
         id_volume=_atoi64(key+2);
#else
         id_volume=atoll(key+2);
#endif
   do_volume=true;
      }
    else if(!strncmp(key,"-o",2)) 
      {
	if(strlen(key)>2)	outdir=key+2;
      }
    else if(!strncmp(key,"-ma",3)) 
      {
	if(strlen(key)>3)	major=atoi(key+3);
      }
    else if(!strncmp(key,"-mi",3)) 
      {
	if(strlen(key)>3)	minor=atoi(key+3);
      }
    else if(!strncmp(key,"-rdb",4))
      {
	if(strlen(key)>4)	rdb=key+4;
      }
  }

  printf("\n----------------------------------------------------------------------------\n");
  printf("Read volume: \t\t\t%lld \nfrom the database: \t\t%s %s \nand save into directory: \t%s \nwith versions: \t\t\t%d.%d\n",
	 id_volume, dbname, rdb, outdir, major,minor);
  printf("----------------------------------------------------------------------------\n\n");

  if(!do_volume) { print_help_message();  return 0; }

  TOracleServerE2 *db = new TOracleServerE2(dbname,user,password);
  printf("Server info: %s\n", db->ServerInfo());
  if(!db) return 0;
  db->eRTS=rdb;
  db->ConvertMicrotracksVolumeToEdb(id_volume, outdir, major, minor);
  delete db;
  return 0;
}
