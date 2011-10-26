//-- Author :  Valeri Tioukov   11/06/2008

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbScanTracking.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  emtra -set=ID [ -o=DATA_DIRECTORY -v=DEBUG] \n\n";
  cout<< "\t\t  ID    - id of the dataset formed as BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t  DEBUG - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  cout<< "\t\t  -m    - make the affine files starting from EdbScanSet\n";
  cout<< "\t\t  -new  - apply new tracking algorithm\n";
  cout<< "\t\t  -o    - the data directory\n";
  cout<< "\nExample: \n";
  cout<< "\t  emtra -id=4554.10.1.0 -o=/scratch/BRICKS \n";
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for tracking
  cenv.SetValue("fedra.readCPcut"       , "eCHI2P<2.5&&s.eW>13&&eN1==1&&eN2==1&&s1.eFlag>=0&&s2.eFlag>=0");
  cenv.SetValue("fedra.track.minPlate"  ,-999 );
  cenv.SetValue("fedra.track.maxPlate"  , 999 );
  cenv.SetValue("fedra.track.refPlate"  , 999 );
  cenv.SetValue("fedra.track.nsegmin"   , 2 );
  cenv.SetValue("fedra.track.ngapmax"   , 4 );
  cenv.SetValue("fedra.track.DRmax"     , 45. );
  cenv.SetValue("fedra.track.DTmax"     , 0.07 );
  cenv.SetValue("fedra.track.Sigma0" , "3 3 0.005 0.005");
  cenv.SetValue("fedra.track.probmin"   , 0.01 );
  cenv.SetValue("fedra.track.momentum"  , 2 );
  cenv.SetValue("fedra.track.mass"      , 0.14 );
  cenv.SetValue("fedra.track.erase"     , false );
  cenv.SetValue("fedra.track.do_misalign",false );
  cenv.SetValue("fedra.track.misalign_offset", 500.);
  cenv.SetValue("emtra.outdir"          , "..");
  cenv.SetValue("emtra.env"             , "track.rootrc");
  cenv.SetValue("emtra.EdbDebugLevel"   , 1);
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("trackenv");
  set_default(cenv);
  gEDBDEBUGLEVEL        = cenv.GetValue("emtra.EdbDebugLevel" , 1);
  const char *env       = cenv.GetValue("emtra.env"            , "track.rootrc");
  const char *outdir    = cenv.GetValue("emtra.outdir"         , "..");

  bool      do_set      = false;
  bool      do_new      = false;
  bool      do_pred     = false;
  bool      do_VSB      = false;
  Int_t     pred_plate  = 0, to_plate=0;
  Int_t     brick=0, plate=0, major=0, minor=0;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if(!strncmp(key,"-set=",5))
      {
	if(strlen(key)>5)	sscanf(key+5,"%d.%d.%d.%d",&brick,&plate,&major,&minor);
	do_set=true;
      }
    else if(!strncmp(key,"-o=",3)) 
      {
	if(strlen(key)>3)	outdir=key+3;
      }
    else if(!strncmp(key,"-new",4)) 
      {
	do_new=true;;
      }
    else if(!strncmp(key,"-pred=",6))
      {
	if(strlen(key)>6)	{
	  pred_plate = atoi(key+6);
	  do_pred=true;
	}
      }
    else if(!strncmp(key,"-VSB=",5))
      {
	if(strlen(key)>5)  {
	  do_VSB=true;
	  to_plate = atoi(key+5);
	}
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }

  if(!do_set)   { print_help_message(); return 0; }

  cenv.SetValue("emtra.env"                  , env);
  cenv.ReadFile( cenv.GetValue("emtra.env"   , "track.rootrc") ,kEnvLocal);
  cenv.SetValue("emtra.outdir"               , outdir);
  cenv.WriteFile("track.save.rootrc");

  if(do_set) {
      EdbScanProc sproc;
      sproc.eProcDirClient=outdir;
      printf("\n----------------------------------------------------------------------------\n");
      printf("tracking set %d.%d.%d.%d\n", brick,plate, major,minor);
      printf("----------------------------------------------------------------------------\n\n");

      EdbID id(brick,plate,major,minor);
      EdbScanSet *ss = sproc.ReadScanSet(id);
      ss->Brick().SetID(brick);
      if(do_new) 
      {
          EdbScanTracking est;
          est.eSproc=&sproc;
          est.TrackSetBT(id,cenv);
      }
      else {
    //ss->MakePIDList();
    //sproc.AssembleScanSet(*ss);

//    TCut c = gEnv->GetValue("emtra.cpcut" ,"s.eW>13&&eCHI2P<2.5&&s1.eFlag>=0&&s2.eFlag>=0&&eN1==1&&eN2==1");
//    EdbScanCond cond;
//    cond.Print();
//    sproc.TrackSetBT(*ss,cond,c);
    
          sproc.TrackSetBT(*ss,cenv);
      }
  }
  /*
  if(do_pred) {
    EdbScanProc sproc;
    sproc.eProcDirClient=outdir;
    printf("\n----------------------------------------------------------------------------\n");
    printf("prepare predictions %d.%d.%d.%d  for the plate %d\n", brick,plate, major,minor, pred_plate);
    printf("----------------------------------------------------------------------------\n\n");

    EdbID id(brick,plate,major,minor);
    EdbScanSet *ss = sproc.ReadScanSet(id);
    ss->Brick().SetID(brick);
    sproc.AssembleScanSet(*ss);

    TCut c = gEnv->GetValue("emtra.trcut" ,"nseg>2");

    EdbPVRec ali;
    sproc.ReadTracksTree(id, ali, c);

    EdbID predid(id);
    predid.ePlate = pred_plate;
    EdbLayer *layer  = ss->GetPlate(pred_plate);
    sproc.MakeTracksPred( *(ali.eTracks), predid, *layer);

  }
  */
  if(do_VSB) {
    EdbScanProc sproc;
    sproc.eProcDirClient=outdir;
    printf("\n----------------------------------------------------------------------------\n");
    printf("prepare predictions %d.%d.%d.%d  for the plate %d\n", brick,plate, major,minor, pred_plate);
    printf("----------------------------------------------------------------------------\n\n");

    EdbID id(brick,plate,major,minor);
    EdbScanSet *ss = sproc.ReadScanSet(id);
    ss->Brick().SetID(brick);
    sproc.AssembleScanSet(*ss);


    EdbID predid(id);
    predid.ePlate = pred_plate;

    //sproc.FindPredictionsRaw(predid,predid);

    sproc.FindPredictionsRawSet(predid, *ss, to_plate);
    EdbPVRec ali;
    sproc.ReadFoundTracks(*ss, ali);
    sproc.WriteSBTracks(*(ali.eTracks), id);
    

    /*
    TCut c = gEnv->GetValue("emtra.trcut" ,"nseg>2");


    EdbLayer *layer  = ss->GetPlate(pred_plate);

    EdbPattern pred, fnd;
    sproc.ReadPred(pred, predid);
    EdbRunAccess ra;
    sproc.InitRunAccess(ra, predid);

    EdbScanCond condBT, condMT;
    sproc.FindPredictionsRaw( pred, fnd, ra, condBT, condMT );
    
    */

  }
  cenv.WriteFile("track.save.rootrc");

  return 1;
}
