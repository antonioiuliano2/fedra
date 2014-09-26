//-- Author :  Valeri Tioukov   30/08/2014

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include "TOracleServerE2WFB.h"
#include "EdbScanProc.h"
#include "EdbLog.h"

using namespace std;
bool InitDB( EdbFeedback &fb, TEnv &cenv, int test);
int  AddBrick( EdbFeedback &fb, int BRICK, const char *dir, TEnv &cenv );
int  AddList( EdbFeedback &fb, const char *listfile, TEnv &cenv);
int ParseFileName( const char *filename, ULong64_t &brick, ULong64_t &event , TString &dir );
int AddFeedback( EdbFeedback &fb, const char *fname, TEnv &cenv );

//-------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\nUsage: \n\t  fb2db -file=feedback_file [-commit -v=V -lab=LA]\n";
  cout<< "\t\t Assumed that feedbackfile name is like bBBBB_eEEEE.feedback as b010234_e11293015645.feedback\n";
  cout<< "\t\t brick id and event id extracted from the file name are used for data insertion into db\n";
  
  cout<< "\n\t fb2db -addbrick=BRICK -online=onlinedir\n";
  cout<< "\t\t assumed that in onlinedir/bBRICK there are all necessary files";
  
  cout<< "\n\n\t-----------------------------------------------------------------------------------------";
  cout<< "\n\t By default the application is started in test mode and do not change the database content";
  cout<< "\n\t TO LOAD DATA USE -commit OPTION";
  cout<< "\n\t-------------------------------------------------------------------------------------------";
 
  cout<< "\n\n\t If the input parameters file (fb2db.rootrc) is not present in the current dir - the default";
  cout<< "\n\t parameters are used. After the execution them are saved into fb2db.save.rootrc file\n";
  
  cout<< "\n\t Options: \n";
  cout<< "\t\t -lab=LA default settings for a given lab (now defined for NA,BE) \n";
  cout<< "\t\t -v=V verbosity level (0-4) \n";
  cout<<endl;
}

//-------------------------------------------------------------------------------------
void set_default(TEnv &cenv)
{
  cenv.SetValue("fb2db.dbname"             , "connection_string");
  cenv.SetValue("fb2db.username"           , "username");
  cenv.SetValue("fb2db.password"           , "password");
  cenv.SetValue("fb2db.rdb"                , "");
  cenv.SetValue("fb2db.X_MARKS"            , 1);
  cenv.SetValue("fb2db.labName"            , "LAB");
  cenv.SetValue("fb2db.labN"               , "LA");
  cenv.SetValue("fb2db.BS_ID"              , "'OPERA LA SET  XX'");
  cenv.SetValue("fb2db.id_machine"         , "0000000000000000");
  cenv.SetValue("fb2db.id_programsettings" , "00000000000000000");
  cenv.SetValue("fb2db.id_requester"       , "0000000000000000");
  cenv.SetValue("fb2db.EdbDebugLevel"      , 1);
}
 
//-------------------------------------------------------------------------------------
void set_default_NA(TEnv &cenv)
{
  cenv.SetValue("fb2db.labName"            , "NAPOLI");
  cenv.SetValue("fb2db.labN"               , "NA");
  cenv.SetValue("fb2db.BS_ID"              , "'OPERA NA SET  04'");
  cenv.SetValue("fb2db.id_machine"         , "6000000000010002");
  cenv.SetValue("fb2db.id_programsettings" , "81000100000000087");
  cenv.SetValue("fb2db.id_requester"       , "6000000000100375");
}
 
//-------------------------------------------------------------------------------------
void set_default_BE(TEnv &cenv)
{
  cenv.SetValue("fb2db.labName"            , "BERN");
  cenv.SetValue("fb2db.labN"               , "BE");
  cenv.SetValue("fb2db.BS_ID"              , "'OPERA BE SET  04'");
  cenv.SetValue("fb2db.id_machine"         , "5000000000000022");
  cenv.SetValue("fb2db.id_programsettings" , "81000100000000087");
  cenv.SetValue("fb2db.id_requester"       , "5000000000328005");
}
 
//-------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("fb2db");
  set_default(cenv);
  cenv.ReadFile( "fb2db.rootrc" ,kEnvLocal);

  gEDBDEBUGLEVEL       = cenv.GetValue("fb2db.EdbDebugLevel" , 1);
  
  int do_checkbrick=0;
  int do_addbrick=0, brick=0;
  char *dir=0;
  char *lab=0;
  
  int do_feedback=0;
  char *fname=0;
  
  char *listfile=0;
  int do_addlist=0;

  int do_testdb=1;
  
  for(int i=1; i<argc; i++ ) {
    
    char *key  = argv[i];

    if(!strncmp(key,"-file=",6))
    {
      do_feedback=1;
      if(strlen(key)>6)	fname=key+6;
    }
    else if(!strncmp(key,"-addbrick=",10))
    {
      if(strlen(key)>10) {
        brick = atoi(key+10);
        do_addbrick=1;
      }
    }
    else if(!strncmp(key,"-list=",6))
    {
      if(strlen(key)>6) {
        listfile = key+6;
        do_addlist=1;
      }
    }
    else if(!strncmp(key,"-checkbrick=",12))
    {
      if(strlen(key)>12) {
        brick = atoi(key+12);
        do_checkbrick=1;
      }
    }
    else if(!strncmp(key,"-online=",8))
    {
      if(strlen(key)>8) dir=key+8;
    }
    else if(!strncmp(key,"-commit",7))
    {
      do_testdb=0;
    }
    else if(!strncmp(key,"-lab=",5))
    {
      lab = key+5;
    }
    else if(!strncmp(key,"-v=",3))
    {
      gEDBDEBUGLEVEL = atoi(key+3);
    }
  }
  
  if(lab) 
  {
    if(!strncmp(lab,"NA",2) )      { set_default_NA(cenv); }
    else if(!strncmp(lab,"BE",2) ) { set_default_BE(cenv); }
  }
  cenv.WriteFile("fb2db.save.rootrc");

  EdbFeedback fb;
  if(do_feedback || do_addbrick || do_addlist || do_checkbrick)
  {   
    if( !InitDB( fb, cenv, do_testdb)) return 0;
  }
  
  if(do_feedback) 
  {
    if(!fname)   { print_help_message(); return 0; }
    AddFeedback( fb, fname, cenv );
  }
  else if(do_addbrick) 
  {
    AddBrick( fb, brick, dir, cenv);
  }
  else if(do_addlist) 
  {
    AddList(fb, listfile, cenv);
  } 
  else if(do_checkbrick) 
  {
    fb.eDB->eDebug=0;
    const char *BS_ID = cenv.GetValue("fb2db.BS_ID"  , "'OPERA NA SET  04'");
    fb.eDB->IfEventBrick(1000000+brick,BS_ID);
  }

  return 1;
}

//-------------------------------------------------------------------------------------
bool InitDB( EdbFeedback &fb, TEnv &cenv, int test)
{
  const char *dbname   = cenv.GetValue("fb2db.dbname"   , "connection_string");
  const char *username = cenv.GetValue("fb2db.username" , "username");
  const char *password = cenv.GetValue("fb2db.password" , "password");
  const char *rdb      = cenv.GetValue("fb2db.rdb"      , "");
  
  printf("Init database \t\t%s%s\n", dbname, rdb);
  if( fb.InitDB( dbname, username, password) ) {
    fb.eDB->eLab =  cenv.GetValue("fb2db.labName" , "NAPOLI");
    fb.eDB->eLa  =  cenv.GetValue("fb2db.labN"    , "NA");
    fb.eDB->eDebug=test;
    fb.eDB->SetTransactionRW();
    sscanf( cenv.GetValue("fb2db.id_machine"         , "6000000000010002"),  "%lld", &(fb.eIdMachine) );
    sscanf( cenv.GetValue("fb2db.id_programsettings" , "81000100000000087"), "%lld", &(fb.eIdProgramsettings) );
    sscanf( cenv.GetValue("fb2db.id_requester"       , "6000000000100375"),  "%lld", &(fb.eIdRequester) );
    fb.Print();
    return 1;
  }
  return 0;
}

//-------------------------------------------------------------------------------------
int AddList( EdbFeedback &fb, const char *listfile, TEnv &cenv)
{
  fprintf(stderr,"AddBrick started\n");
  FILE *f = fopen( listfile, "r" );
  char str[1024];
  
  while( fgets( str, sizeof(str), f) )
  {
    char fname[1024];
    if( 1 != sscanf(str,"%s", fname )) {
      Log(1,"fb2db::AddList","ERROR! bad filename line: %s", str);
      continue;
    }
    
    ULong64_t brickid=0,eventid=0;
    TString onlinedir;
    if( !ParseFileName( fname, brickid, eventid, onlinedir ) ) continue;
    Log(2,"fb2db::ParseFileName","brick = %lld, event = %lld, onlinedir=%s",brickid, eventid,onlinedir.Data());
    
    if( !fb.eDB->IfEventBrick(1000000+brickid, cenv.GetValue("fb2db.BS_ID"  , "'OPERA NA SET  04'") ) ) 
    {
      if( !AddBrick(fb, brickid, onlinedir, cenv) ) return 0;
    }
    
    AddFeedback( fb, fname, cenv );
    
  }
  return 0;
}

//-------------------------------------------------------------------------------------
int ParseFileName( const char *fname, ULong64_t &brickid, ULong64_t &eventid , TString &onlinedir )
{
  if( gSystem->AccessPathName(fname) ) {
    Log(1,"fb2db::ParseFileName","ERROR: can not access file: %s", fname);
    return 0;
  }
  if(2 != sscanf(gSystem->BaseName(fname),"b%lld_e%lld.feedback",&brickid,&eventid)) {
    Log(1,"Error parsing feedback filename (expected bXXXX_eXXXXX.feedback): %s", fname);
    return 0;
  }
  const char *dir = gSystem->DirName(fname);
  int n = strlen(dir) - strlen( Form("/b%6.6d",int(brickid)) );
  onlinedir=dir;
  onlinedir.Resize( n );
  return 1;
}

//-------------------------------------------------------------------------------------
int AddFeedback( EdbFeedback &fb, const char *fname, TEnv &cenv )
{
  TString onlinedir;
  ULong64_t brickid=0,eventid=0;
  if( !ParseFileName( fname, brickid, eventid, onlinedir ) ) return 0;
  if(brickid>0&&brickid<1000000) brickid+=1000000;
  printf("\n-------------------------------------------------------\n");
  printf("Load feedback file  %s into database for brick %lld, event %lld\n", fname, brickid, eventid);
  printf("---------------------------------------------------------\n");
  fb.eEventBrick=brickid;
  fb.eEvent=eventid;
  fb.ReadFBFile(fname);
  if(gEDBDEBUGLEVEL>2) fb.PrintFB();
    
  fb.LoadFBintoDB();
  fb.eDB->Commit();
  return 1;
}

//-------------------------------------------------------------------------------------
int AddBrick( EdbFeedback &db, int BRICK, const char *dir, TEnv &cenv )
{
  fprintf(stderr,"AddBrick started\n");
  EdbScanProc sproc;
  sproc.eProcDirClient=dir;
  time_t ti = time(NULL);
  
  /*********** Loading X ray mark sets in global RS to get ZEROX,ZEROY ***********/
  EdbMarksSet msXG;
  sproc.ReadMarksSet(msXG,BRICK,"map.XG",'_','S');
  float ZEROX = msXG.eXmin;
  float ZEROY = msXG.eYmin;

  /*********** Loading Optical ray mark sets in local (and corrected) RS to get brick dimension ***********/
  int X_MARKS = cenv.GetValue("fb2db.X_MARKS"  , 1);
  
  EdbMarksSet msOL;
  if (X_MARKS) sproc.ReadMarksSet(msOL,BRICK,"map.LL",'_','L');
  else sproc.ReadMarksSet(msOL,BRICK,"map.OL",'_','S');
  float MINX = msOL.eXmin + ZEROX;
  float MAXX = msOL.eXmax + ZEROX;
  float MINY = msOL.eYmin + ZEROY;
  float MAXY = msOL.eYmax + ZEROY;
  float MINZ = -72800.;
  float MAXZ = 0.;
  float ZEROZ = MAXZ;
  const char *BS_ID = cenv.GetValue("fb2db.BS_ID"  , "'OPERA NA SET  04'");

  char databrick[500];
  sprintf(databrick,"%d, %f, %f, %f, %f, %f, %f, %s, %d, %f, %f, %f", 1000000+BRICK, MINX, MAXX, MINY, MAXY, MINZ, MAXZ, BS_ID, BRICK, ZEROX, ZEROY, ZEROZ);
  db.eDB->AddEventBrick(databrick);
  
  /*********** Getting brick id ***********/
  int id_eventbrick = db.eDB->GetId_EventBrick( Form("%d",BRICK), BS_ID, 0);
  Log(1,"fb2db","The brick %d %s is inside database now!\n",id_eventbrick, BS_ID);
 
  /***********************************/
  /********** Adding plates **********/
  /***********************************/
  
  char filename[255];
  sprintf(filename,"%s/b%06d/b%06d.geometry",sproc.eProcDirClient.Data(),BRICK,BRICK);
  
  FILE *fp;
  
  if ((fp=fopen(filename,"r"))==NULL) {
    fprintf(stderr,"Sorry, I cannot open the file %s\n",filename); return 0;
  }
  
  while (!feof(fp))
  {
    int iplate;
    float z;
    if (fscanf(fp,"%d %f",&iplate,&z)!=2) continue;
      
    char dataplate[50];
    sprintf(dataplate,"%d, %f",iplate, z);
    db.eDB->AddPlate( id_eventbrick, dataplate);
  }
  db.eDB->Commit();

  time_t tf = time(NULL);
  fprintf(stderr,"AddBrick completed\n");
  fprintf(stderr,"Elapsed time = %ld seconds\n",tf-ti);
  return 1;
}
