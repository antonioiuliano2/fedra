#include <iostream>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbProcPars.h"

using namespace std;

//----------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\n Complete reconstruction of one volume \n";
  cout<< "\t a) preliminary operations:\n";
  cout<< "\t    checks, linking, alignment, tracking and corrections search\n";
  cout<< "\t b) final operations:\n";
  cout<< "\t    tracking with microtracks, predictions search\n";
  cout<< "\t    vertexing and decay search\n";
  
  cout<< "\nUsage: \n\t  emrec -set=ID [-v=DEBUG -o=DATA_DIRECTORY] \n";
  cout<< "\t\t  ID    - id of the data piece or dataset formed as BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t  DEBUG - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  
  cout<< "\nExample: \n";
  cout<< "\t  emrec -set=4554.10.1.0 -v=2 -o/scratch/BRICKS \n";
  
  cout<< "\n If the data location directory is not explicitly defined\n";
  cout<< " the current directory assumed to be the brick directory \n";
  
  cout<< "\n If the parameters file (link.rootrc) is not presented - the default \n";
  cout<< " parameters are used. After the execution them will be saved into emrec.save.rootrc\n";
  cout<<endl;
}

//-----------------------------------------------------------------------------
TEnv        cenv("emrec");
EdbID       idset;
bool        do_set    = 0;
const char *outdir    = "..";

//-----------------------------------------------------------------------------
void process()
{
  EdbScanProc sproc;
  EdbScanSet *ss = sproc.ReadScanSet(idset);
  
  Log(1,"emrec::process","Setting linking definitions via: EdbProcPars::SetLinkDef(cenv);");
  EdbProcPars::SetLinkDef(cenv);
  
  Log(1,"emrec::process","Do the linking of the ScanProc instance via sproc.LinkSetNewTest(*ss, cenv);");
  sproc.LinkSetNewTest(*ss, cenv);   
  
  sproc.MakeLinkSetSummary(idset);

}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
    if(!strncmp(key,"-set=",5))
      {
	if(strlen(key)>5)   idset.Set(key+5);	do_set=true;
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
    else if(!strncmp(key,"-o=",3)) 
      {
	if(strlen(key)>3)	outdir=key+3;
      }
  }
  
  cenv.ReadFile("emrec.rootrc" ,kEnvLocal);
  cenv.SetValue("emrec.outdir", outdir);
  
  if(do_set) process();
   
  cenv.WriteFile("emrec.save.rootrc");
 
  return 0;
}
