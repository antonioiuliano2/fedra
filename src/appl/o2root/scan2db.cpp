//-- Author :  Valeri Tioukov   10/01/2017

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include "TOracleServerE2WX.h"
#include "EdbScanProc.h"
#include "EdbLog.h"

//---------------------------------------------------------------------------------------
using namespace std;
bool InitDB(   EdbScan2DB &s2d, TEnv &cenv, int do_commit);
int  AddBrick( EdbScan2DB &s2d, int BRICK, const char *dir, TEnv &cenv );
int  ParseFileName( const char *filename, ULong64_t &brick, ULong64_t &event );
int  ParseFileName( const char *filename, ULong64_t &brick, ULong64_t &event , TString &dir );
bool LoadEventBrick( TEnv &cenv, int do_commit, const char *file);
void CheckBrick(TEnv &cenv, int brick);
void MakeSets(TEnv &cenv, const char *file);
void PublicateBrick( TEnv &cenv, Int_t brick );

//-------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\nUsage: \n\t  scan2db -file=bXXX_eYYY.scan2db.rootrc [-commit -v=V]\n";
  cout<< "\t\t Assumed that feedbackfile name is like bBBBB_eEEEE.feedback as b010234_e11293015645.feedback\n";
  cout<< "\t\t brick id and event id extracted from the file name are used for data insertion into db\n";
  
  cout<< "\n\t scan2db -file=bXXX_eYYY.scan2db.rootrc -makesets \n";
  cout<< "\t\t make *.set.root files for the involved datasets (use default OPERA geometry settings)\n";
  
  cout<< "\n\t scan2db -checkbrick=BRICK \n";
  cout<< "\t\t check if the brick structure was already loaded";
  
  cout<< "\n\t scan2db -publicate=BRICK \n";
  cout<< "\t\t replicate brick in opfra";
  
  cout<< "\n\n\t-----------------------------------------------------------------------------------------";
  cout<< "\n\t By default the application is started in test mode and does not commit transactions";
  cout<< "\n\t TO LOAD DATA USE -commit OPTION";
  cout<< "\n\t-------------------------------------------------------------------------------------------";
 
  cout<< "\n\n\t If the input parameters file (scan2db.rootrc) is not present in the current dir - the default";
  cout<< "\n\t parameters used. After the execution them are saved into scan2db.save.rootrc file\n";
  
  cout<< "\n\t Options: \n";
  cout<< "\t\t -v=V verbosity level (0-4) \n";
  
  cout <<"\nExample of b010234_e11293015645.feedback file:\n";
  cout <<"\
##-------------------------------------------------------------\n\
## - EventBrick operations and datasets definitions\n\
## - Used by scan2db as the input cards\n\
## - To be placed into the brick folder as\n\
##    \"bXXXXXX_eYYYYYYYYYYY.scan2db.rootrc\" (b102143_e12186044883.scan2db.rootrc)\n\
## - All datasets should be correctly assembled using\n\
##   the \"makescanset\" application before scan2db starts (*.set.root)\n\
##\n\
## scan2db.X_MARKS:\n\
##   2=in case lateral X-rays marks, followed by a calibration scan;\n\
##   1=in case lateral X-rays marks only;\n\
##   0=calibration scan only\n\
##\n\
## scan2db.BlackCS:\n\
##    1 - fill TB_SCANBACK_PATHS only\n\
##    0 - try to fill also TB_B_CSCANDS_SBPATHS\n\
##\n\
## scan2db.VOLUME.IDPATH:\n\
##     <0 - (default) do not connect path and volume (table TB_B_SBPATHS_VOLUMES). This is useful when volume\n\
##      was scanned because of a vertex hunting  and not because it's related to a SB track\n\
##    >=0 - is prediction path id to be connected\n\
##\n\
##-------------------------------------------------------------\n\
scan2db.X_MARKS:                           1\n\
scan2db.BlackCS:                           1\n\
scan2db.CALIBRATION:                       76735.0.1.1000\n\
scan2db.CALIBRATION.raw:                   0\n\
scan2db.PREDICTION:                        76735.0.1.100\n\
scan2db.VOLUME:                            76735.0.1.1000\n\
scan2db.VOLUME.IDPATH:                     1\n";

  cout<<endl;
}

//-------------------------------------------------------------------------------------
void set_default(TEnv &cenv)
{
  cenv.SetValue("scan2db.dbname"                     , "oracle://mysrv/MYDB");
  cenv.SetValue("scan2db.username"                   , "username");
  cenv.SetValue("scan2db.password"                   , "password");
  cenv.SetValue("scan2db.rdb"                        , "");
  cenv.SetValue("scan2db.X_MARKS"                    , 1);
  cenv.SetValue("scan2db.labName"                    , "LAB");
  cenv.SetValue("scan2db.labN"                       , "LA");
  cenv.SetValue("scan2db.BS_ID"                      , "'OPERA LA SET  XX'");
  cenv.SetValue("scan2db.IdMachine"                  , "0000000000000000");
  cenv.SetValue("scan2db.IdRequester"                , "0000000000000000");
  cenv.SetValue("scan2db.HeaderProgramsettings"      , "0000000000000000");
  cenv.SetValue("scan2db.CalibrationProgramsettings" , "0000000000000000");
  cenv.SetValue("scan2db.PredictionProgramsettings"  , "0000000000000000");
  cenv.SetValue("scan2db.VolumeProgramsettings"      , "0000000000000000");
  cenv.SetValue("scan2db.FeedbackProgramsettings"    , "0000000000000000");
  cenv.SetValue("scan2db.TestLoad"                   , 0);
  cenv.SetValue("scan2db.TestLoadN"                  , 10);
  cenv.SetValue("scan2db.EdbDebugLevel"              , 1);
}

//-------------------------------------------------------------------------------------
void set_default_NA(TEnv &cenv)
{
  cenv.SetValue("scan2db.labName"                    , "NAPOLI");
  cenv.SetValue("scan2db.labN"                       , "NA");
  cenv.SetValue("scan2db.BS_ID"                      , "'OPERA NA SET  01'");
  cenv.SetValue("scan2db.IdMachine"                  , "6000000000010002");
  cenv.SetValue("scan2db.IdRequester"                , "6000000000100379");
  cenv.SetValue("scan2db.HeaderProgramsettings"      , "6000000000700006");
  cenv.SetValue("scan2db.CalibrationProgramsettings" , "6000000000700004");
  cenv.SetValue("scan2db.PredictionProgramsettings"  , "6000000000700005");
  cenv.SetValue("scan2db.VolumeProgramsettings"      , "6000000000100374");
}

//-------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("scan2db");
  set_default(cenv);
  cenv.ReadFile( "scan2db.rootrc" ,kEnvLocal);

  gEDBDEBUGLEVEL       = cenv.GetValue("scan2db.EdbDebugLevel" , 1);
  
  int  do_checkbrick=0;
  int  brick=0;
  char *dir=0;
  char *lab=0;
  
  int do_addeventbrick=0;
  char *fname=0;
  
  char *listfile=0;
  int do_commit=0;
  int do_makesets=0;
  int do_publicate=0;

  for(int i=1; i<argc; i++ ) {

    char *key  = argv[i];

    if(!strncmp(key,"-file=",6))
    {
      do_addeventbrick=1;
      if(strlen(key)>6) fname=key+6;
    }
    else if(!strncmp(key,"-checkbrick=",12))
    {
      if(strlen(key)>12) {
        brick = atoi(key+12);
        do_checkbrick=1;
      }
    }
    else if(!strncmp(key,"-commit",7))
    {
      do_commit=1;
    }
    else if(!strncmp(key,"-publicate=",11))
    {
      if(strlen(key)>11) {
        brick = atoi(key+11);
        do_publicate=1;
      }
    }
    else if(!strncmp(key,"-makesets",9))
    {
      do_makesets=1;
    }
    else if(!strncmp(key,"-v=",3))
    {
      gEDBDEBUGLEVEL = atoi(key+3);
    }
  }

  cenv.WriteFile("scan2db.save.rootrc");

  if(do_makesets)
  {
    MakeSets(cenv, fname);
  }
  else 
  {
    if(do_addeventbrick)
    {
      LoadEventBrick( cenv, do_commit, fname );
    }
    if(do_checkbrick)
    {
      CheckBrick(cenv, brick );
    }
    if(do_publicate)
    {
      PublicateBrick(cenv, brick );
    }
  }
  return 1;
}

//-------------------------------------------------------------------------------------
void CheckBrick(TEnv &cenv, int brick )
{
  EdbScan2DB s2d;
  if( InitDB( s2d, cenv, 0))
  {
    s2d.eDB->PrintBrickInfoFull(brick,0);
  }
}

//-------------------------------------------------------------------------------------
void PublicateBrick(TEnv &cenv, int brick )
{
  Int_t eventbrick = brick>1000000? brick: 1000000+brick;
  EdbScan2DB s2d;
  if( InitDB( s2d, cenv, 1) )
  {
    s2d.eDB->MyQuery(
        Form("call operapub.xp_replicate_NA(0,%d)",eventbrick)
                    );
  }
}

//-------------------------------------------------------------------------------------
void MakeSets( TEnv &cenv, const char *fname)
{
  TEnv cardenv("eventscan2db");
  if( cardenv.ReadFile( fname ,kEnvLocal) <0 )
  {
    Log(1,"MakeSets","card file %s not found! exit", fname);
    return;
  }
  ULong64_t br=0, ev=0;
  TString   dir;
  ParseFileName( fname, br, ev , dir );
  EdbScanProc sproc;
  sproc.eProcDirClient=dir;
  int         from_plate  = 57;
  int         to_plate    =  1;
  float       z0          =  0;
  float       dz          = -1300;
  float       dzbase      = 210;

  if(cardenv.Lookup("scan2db.CALIBRATION")) 
  {
    EdbID id           = cardenv.GetValue("scan2db.CALIBRATION", "0.0.0.0");
    EdbScanSet sc(id);
    sc.MakeNominalSet(id,from_plate, to_plate, z0, dz, 1, dzbase);
    sproc.MakeScannedIDList( id, sc, from_plate, to_plate, "raw.root");
    sproc.WriteScanSet(id,sc);
    sproc.UpdateSetWithPlatePar(id);
    sproc.UpdateSetWithAff(id,id);
    if(gEDBDEBUGLEVEL>1) {
      EdbScanSet *ss = sproc.ReadScanSet(id);
      ss->Print();
    }
  }
  if(cardenv.Lookup("scan2db.PREDICTION")) 
  {
    EdbID id           = cardenv.GetValue("scan2db.PREDICTION", "0.0.0.0");
    EdbScanSet sc(id);
    sc.MakeNominalSet(id,from_plate, to_plate, z0, dz, 1, dzbase);
    sproc.MakeScannedIDList( id, sc, from_plate, to_plate, "sbt.root");
    sproc.WriteScanSet(id,sc);
    sproc.UpdateSetWithPlatePar(id);
    sproc.UpdateSetWithAff(id,id);
    if(gEDBDEBUGLEVEL>1) {
      EdbScanSet *ss = sproc.ReadScanSet(id);
      ss->Print();
    }
  }
  if(cardenv.Lookup("scan2db.VOLUME"))
  {
    EdbID id           = cardenv.GetValue("scan2db.VOLUME", "0.0.0.0");
    EdbScanSet sc(id);
    sc.MakeNominalSet(id,from_plate, to_plate, z0, dz, 1, dzbase);
    sproc.MakeScannedIDList( id, sc, from_plate, to_plate, "raw.root");
    sproc.WriteScanSet(id,sc);
    sproc.UpdateSetWithPlatePar(id);
    sproc.UpdateSetWithAff(id,id);
    if(gEDBDEBUGLEVEL>1) {
      EdbScanSet *ss = sproc.ReadScanSet(id);
      ss->Print();
    }
  }
}

//-------------------------------------------------------------------------------------
bool LoadEventBrick( TEnv &cenv, int do_commit, const char *fname)
{
  time_t ti = time(NULL);
  EdbScan2DB s2d;
  if( !InitDB( s2d, cenv, do_commit)) return 0;

  if( cenv.GetValue("scan2db.TestLoad",0) ) {
    s2d.eDB->SetTestLoad( cenv.GetValue("scan2db.NTestLoad",0));
    Log(1,"LoadEventBrick","TestLoad mode followed by ROLLBACK is active! NTestLoad = %d",
        cenv.GetValue("scan2db.NTestLoad",0) );
  }

  ULong64_t br=0, ev=0;
  TString   dir;
  ParseFileName( fname, br, ev , dir );
  TEnv cardenv("eventscan2db");
  if( cardenv.ReadFile( fname ,kEnvLocal) <0 )
  {
    Log(1,"LoadEventBrick","card file %s not found! exit", fname);
    return false;
  }
  printf("\n--------------------------------------------------------------------\n");
  printf(  "----------------------- scan2db ------------------------------------\n");
  printf(  "Process file: %s\n",fname);
  printf(  "--------------------------------------------------------------------\n");
  cardenv.Print();
  printf(  "--------------------------------------------------------------------\n");
  
  int id = br>1000000? br: 1000000+br;
  s2d.eBrick            = br;
  s2d.eEventBrick       = id;
  s2d.eEvent            = ev;
  s2d.eIsBlackCS        = cardenv.GetValue("scan2db.BlackCS", 0);
  
  s2d.eX_marks= cardenv.GetValue("scan2db.X_MARKS"  , 1); 
  // 2=in case lateral X-rays marks, followed by a calibration scan;
  // 1=in case lateral X-rays marks only;
  // 0=calibration scan only

  s2d.eID_SET = cenv.GetValue("scan2db.BS_ID"  , "'OPERA NA SET  01'");
  sscanf( cenv.GetValue("scan2db.HeaderProgramsettings",      "0000000000000000"),"%lld", &(s2d.eHeaderProgramsettings));
  sscanf( cenv.GetValue("scan2db.CalibrationProgramsettings", "0000000000000000"),"%lld", &(s2d.eCalibrationProgramsettings));
  sscanf( cenv.GetValue("scan2db.PredictionProgramsettings",  "0000000000000000"),"%lld", &(s2d.ePredictionProgramsettings));
  sscanf( cenv.GetValue("scan2db.VolumeProgramsettings",      "0000000000000000"),"%lld", &(s2d.eVolumeProgramsettings));
  sscanf( cenv.GetValue("scan2db.IdMachine",                  "0000000000000000"),"%lld", &(s2d.eIdMachine));
  sscanf( cenv.GetValue("scan2db.IdRequester",                "0000000000000000"),"%lld", &(s2d.eIdRequester));

  if(gEDBDEBUGLEVEL>0) s2d.Print();

  EdbScanProc sproc;
  sproc.eProcDirClient=dir;

  s2d.AddBrick( sproc );
 
  if(cardenv.Lookup("scan2db.CALIBRATION")) {
    printf(  "scan2db CALIBRATION ......\n");
    time_t ti_c = time(NULL);
    EdbID idcal           = cardenv.GetValue("scan2db.CALIBRATION", "0.0.0.0");
    s2d.eWriteRawCalibrationData = cardenv.GetValue("scan2db.CALIBRATION.raw", 0);
    s2d.LoadCalibration(sproc, idcal);
    time_t tf_c = time(NULL);
    printf(  "scan2db CALIBRATION finished, Elapsed time = %ld s\n", tf_c-ti_c);
  }
  if(cardenv.Lookup("scan2db.PREDICTION")) {
    printf(  "scan2db PREDICTION ......\n");
    time_t ti_p = time(NULL);
    EdbID idpred           = cardenv.GetValue("scan2db.PREDICTION", "0.0.0.0");
    s2d.LoadPrediction(sproc, idpred);
    time_t tf_p = time(NULL);
    printf(  "scan2db PREDICTION finished, Elapsed time = %ld s\n", tf_p-ti_p);
  }
  if(cardenv.Lookup("scan2db.VOLUME")) {
    printf(  "scan2db VOLUME ......\n");
    time_t ti_v = time(NULL);
    EdbID idvol           = cardenv.GetValue("scan2db.VOLUME"       , "0.0.0.0");
    EdbID idpred          = cardenv.GetValue("scan2db.PREDICTION"   , "0.0.0.0");
    s2d.eIDPATH           = cardenv.GetValue("scan2db.VOLUME.IDPATH", -1);
    s2d.LoadVolume(sproc, idvol, idpred);
    time_t tf_v = time(NULL);
    printf(  "scan2db VOLUME finished, Elapsed time = %ld s\n", tf_v-ti_v);
  }

  s2d.eDB->FinishTransaction();

  time_t tf = time(NULL);
  fprintf(stdout,"LoadEventBrick completed\n");
  fprintf(stdout,"Elapsed time = %ld seconds\n",tf-ti);
  return 1;
}


//-------------------------------------------------------------------------------------
bool InitDB( EdbScan2DB &s2d, TEnv &cenv, int do_commit)
{
  const char *dbname   = cenv.GetValue("scan2db.dbname"   , "connection_string");
  const char *username = cenv.GetValue("scan2db.username" , "username");
  const char *password = cenv.GetValue("scan2db.password" , "password");
  const char *rdb      = cenv.GetValue("scan2db.rdb"      , "");

  Log(2,"InitDB","Init database \t\t%s%s", dbname, rdb);
  if( s2d.InitDB( dbname, username, password) ) 
  {
    s2d.eDB->eLab =  cenv.GetValue("scan2db.labName" , "NAPOLI");
    s2d.eDB->eLa  =  cenv.GetValue("scan2db.labN"    , "NA");
    s2d.eDB->SetCommit(do_commit);
    return 1;
  }
  return 0;
}

//-------------------------------------------------------------------------------------
int ParseFileName( const char *fname, ULong64_t &brickid, ULong64_t &eventid )
{
  if( gSystem->AccessPathName(fname) ) {
    Log(1,"fb2db::ParseFileName","ERROR: can not access file: %s", fname);
    return 0;
  }
  if(2 != sscanf(gSystem->BaseName(fname),"b%lld_e%lld.scan2db.rootrc",&brickid,&eventid)) {
    Log(1,"Error parsing filename (expected bXXXX_eXXXXX.scan2db.rootrc): %s", fname);
    return 0;
  }
  return 1;
}

//-------------------------------------------------------------------------------------
int ParseFileName( const char *fname, ULong64_t &brickid, ULong64_t &eventid , TString &onlinedir )
{
  if(ParseFileName( fname, brickid, eventid) ) {
    const char *dir1 = gSystem->DirName(fname);
    if(!dir1) return 0;
    const char *dir  = gSystem->DirName(dir1);
    if(!dir) return 0;
    if( strlen(dir1) < 2 )  return 0;
    onlinedir=dir;
    return 1;
  }
  return 0;
}