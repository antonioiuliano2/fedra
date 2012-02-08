//-- Author :  Valeri Tioukov   16/01/2012

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbScanTracking.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  empred -set1=ID1 -plate=P [ -set2=ID2 -o=DATA_DIRECTORY -v=DEBUG] \n\n";
  cout<< "\t\t  ID1             - id of the tracks tree formed as BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t    P             - id of the prediction plate\n";
  cout<< "\t\t  ID2             - optional: id of the target dataset \n";
  cout<< "\t\t  -o              - the data directory\n";
  cout<< "\nExample: \n";
  cout<< "\t  emtra -set1=4554.0.1.0 -plate=1 -set2=4554.0.1.100 \n";
  cout<< "\t\t In this example predictions produced starting from 4554.10.1.0.trk.root for plate 1,\n\t\t applied the affine transformation (if any) 4554.0.1.0_4554.0.1.100.aff.par \n\t\t and produce  predictions as p001/4554.1.1.100.pred.root \n";
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for predictions preparation
  cenv.SetValue("empred.trcut"           , "nseg>2");
  cenv.SetValue("empred.outdir"          , "..");
  cenv.SetValue("empred.env"             , "empred.rootrc");
  cenv.SetValue("empred.EdbDebugLevel"   , 1);
}

int main(int argc, char* argv[])
{
  TEnv cenv("predenv");
  set_default(cenv);
  gEDBDEBUGLEVEL        = cenv.GetValue("empred.EdbDebugLevel" , 1);
  const char *outdir    = cenv.GetValue("empred.outdir"        , "..");
  
  if (argc < 2)   { print_help_message();  cenv.Print(); return 0; }

  bool      do_set1  = false;
  bool      do_set2  = false;
  int       plate=0;
  EdbID     idset1,idset2;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-set1=",6))
      {
	if(strlen(key)>6)	if(idset1.Set(key+6))   do_set1=true;
      }
    else if(!strncmp(key,"-set2=",6))
      {
	if(strlen(key)>6)	if(idset2.Set(key+6))   do_set2=true;
      }
    else if(!strncmp(key,"-plate=",7))
      {
	if(strlen(key)>4) { plate = atoi(key+7); }
      }
    else if(!strncmp(key,"-o=",3)) 
      {
	if(strlen(key)>3)	outdir=key+3;
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }

  if(!do_set1)   { print_help_message(); cenv.Print(); return 0; }

  cenv.ReadFile( cenv.GetValue("empred.env"   , "empred.rootrc") ,kEnvLocal);
  cenv.SetValue("empred.outdir"               , outdir);
  cenv.WriteFile("empred.save.rootrc");

    EdbScanProc sproc;
    sproc.eProcDirClient=outdir;
    printf("\n----------------------------------------------------------------------------\n");
    printf("prepare predictions %s  for the plate %d\n", idset1.AsString(), plate );
    printf("----------------------------------------------------------------------------\n\n");

    EdbScanSet *ss1 = sproc.ReadScanSet(idset1);

    TCut cut = cenv.GetValue("empred.trcut" ,"nseg>2");
    EdbPVRec ali;
    sproc.ReadTracksTree(idset1, ali, cut);

    if(do_set2)
    {
      EdbID fromid(idset1);   fromid.ePlate = plate;
      EdbID predid(idset2);   predid.ePlate = plate;
      EdbLayer la;
      sproc.ReadAffToLayer( la, fromid, predid );
      EdbLayer *layer  = ss1->GetPlate(plate);
      layer->SubstructCorrections(la);
      int n = sproc.MakeTracksPred( *(ali.eTracks), predid, *layer);
      Log(1,"empred","%d predictions found using cut %s",n,cut.GetTitle());
    } 
    else
    {
      EdbID predid(idset1);   predid.ePlate = plate;
      EdbLayer *layer  = ss1->GetPlate(plate);
      int n = sproc.MakeTracksPred( *(ali.eTracks), predid, *layer);
      layer->Print();
      Log(1,"empred","%d predictions found using cut %s",n,cut.GetTitle());
    }

  cenv.WriteFile("empred.save.rootrc");

  return 1;
}
