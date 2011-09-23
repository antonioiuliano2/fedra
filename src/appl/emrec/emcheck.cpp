//-- Author :  Valeri Tioukov   28/06/2011

#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  emcheck -id=ID [-o=DATA_DIRECTORY -v=DEBUG -raw] \n";
  cout<< "\t  emcheck  -set=ID [-o=DATA_DIRECTORY -v=DEBUG ] \n\n";
  cout<< "\t\t  ID       - id of the run(set) BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t  DEBUG    - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  cout<< "\nExample: \n";
  cout<< "\t  emcheck -set=4554.0.1.1 -o=/scratch/BRICKS -raw -v=2\n";
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<< "\n If the parameters file (check.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into check.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for the new checkment
  cenv.SetValue("emcheck.outdir"          , ".."  );
  cenv.SetValue("emcheck.env"             , "check.rootrc");
  cenv.SetValue("emcheck.EdbDebugLevel"   ,  1    );
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("checkenv");
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("emcheck.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("emcheck.env"            , "check.rootrc");
  const char *outdir = cenv.GetValue("emcheck.outdir"         , "..");
  
  bool      do_id       = false;
  bool      do_set      = false;
  bool      do_raw      = false;
  EdbID     id;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-id=",4)) 
      {
	if(strlen(key)>3)      if(id.Set(key+4)) do_id=true;
      }
    else if(!strncmp(key,"-set=",5))
      {
	if(strlen(key)>5)      if(id.Set(key+5)) do_set=true;
      }
    else if(!strncmp(key,"-raw",4))
      {
	do_raw=true;
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }

  if(!(do_id||do_set))   { print_help_message(); return 0; }

  cenv.SetValue("emcheck.env"            , env);
  cenv.ReadFile( cenv.GetValue("emcheck.env"   , "check.rootrc") ,kEnvLocal);
  cenv.SetValue("emcheck.outdir"         , outdir);

  EdbScanProc sproc;
  sproc.eProcDirClient = cenv.GetValue("emcheck.outdir","..");

  if(do_id) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("check ID %s\n", id.AsString() );
    printf("----------------------------------------------------------------------------\n\n");

    sproc.CheckRunQualityRaw(id);
  }  
  else if(do_set) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("check set %s\n", id.AsString() );
    printf("----------------------------------------------------------------------------\n\n");
    cenv.WriteFile("check.save.rootrc");

    sproc.CheckSetQualityRaw(id);
  }

  cenv.WriteFile("check.save.rootrc");
  return 1;
}
