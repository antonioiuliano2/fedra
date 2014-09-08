//-- Author :  Valeri Tioukov   30/08/2014

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "TOracleServerE2WFB.h"
#include "EdbScanProc.h"
#include "EdbLog.h"

using namespace std;
bool InitDB( EdbFeedback &fb, TEnv &cenv, int test );
int  AddBrick( int BRICK,const char *dir, TEnv &cenv, int test  );

void print_help_message()
{
  cout<< "\nUsage: \n\t  fb2db -file=feedback_file\n";
  cout<<"\t\t fb2db -addbrick -brick=id -online=dir\n";
  cout<<"\t\t assumed that in brickdir there are all necessary files";
  cout<< "\n If the parameters file (fb2db.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into fb2db.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  cenv.SetValue("fb2db.dbname"   , "connection_string");
  cenv.SetValue("fb2db.username" , "username");
  cenv.SetValue("fb2db.password" , "password");
  cenv.SetValue("fb2db.rdb"      , "");
  cenv.SetValue("fb2db.labName" , "NAPOLI");
  cenv.SetValue("fb2db.labN"    , "NA");
  cenv.SetValue("fb2db.X_MARKS"  , 1);
  cenv.SetValue("fb2db.BS_ID"    , "'OPERA NA SET  04'");
  cenv.SetValue("fb2db.EdbDebugLevel" , 1);
}
 
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("fb2db");
  set_default(cenv);
  cenv.ReadFile( "fb2db.rootrc" ,kEnvLocal);

  gEDBDEBUGLEVEL       = cenv.GetValue("fb2db.EdbDebugLevel" , 1);
  
  int do_addbrick=0, brick=0;
  char *dir=0;
  
  int do_feedback=0;
  char *fname=0;
  
  int do_testdb=0;
  
  for(int i=1; i<argc; i++ ) {
    
    char *key  = argv[i];

    if(!strncmp(key,"-file=",6))
    {
      do_feedback=1;
      if(strlen(key)>6)	fname=key+6;
    }
    else if(!strncmp(key,"-addbrick=",9))
    {
      do_addbrick=1;
    }
    else if(!strncmp(key,"-brick=",7))
    {
      if(strlen(key)>7) brick = atoi(key+7);
    }
    else if(!strncmp(key,"-online=",8))
    {
      if(strlen(key)>8) dir=key+8;
    }
    else if(!strncmp(key,"-testdb",7))
    {
      do_testdb=1;
    }
  }
  
  cenv.WriteFile("fb2db.save.rootrc");
    
  if(!fname)   { print_help_message(); return 0; }
    
  
  if(do_feedback) {
    printf("\n-------------------------------------------------------\n");
    printf("Load feedback file  %s into database\n", fname);
    printf("---------------------------------------------------------\n");
    EdbFeedback fb;
    fb.eEventBrick=1010234;
    fb.eEvent=11293015645;
    fb.ReadFBFile("b010234_e11293015645.feedback");
  //fb.PrintFB();
    if( !InitDB( fb, cenv, do_testdb )) return 0;
  //fb.eDB->eLab =  cenv.GetValue("fb2db.labName" , "NAPOLI");
  //fb.eDB->eLa  =  cenv.GetValue("fb2db.labN"    , "NA");

  //fb.eDB->Print();
  //fb.eDB->SetTransactionRW();
    fb.LoadFBintoDB();
  //fb.eDB->Commit();
  }  else if(do_addbrick) {
    AddBrick(brick,dir,cenv, do_testdb);
  }
  return 1;
}

bool InitDB( EdbFeedback &fb, TEnv &cenv, int do_testdb )
{
  const char *dbname   = cenv.GetValue("fb2db.dbname"   , "connection_string");
  const char *username = cenv.GetValue("fb2db.username"   , "username");
  const char *password = cenv.GetValue("fb2db.password"   , "password");
  const char *rdb      = cenv.GetValue("fb2db.rdb"   , "");
  printf("Init database \t\t%s%s\n", dbname, rdb);
  fb.eDB->eDebug=do_testdb;
  return fb.InitDB( dbname, username, password);
}

int AddBrick( int BRICK,const char *dir, TEnv &cenv, int test  )
{
  fprintf(stderr,"AddBrick started\n");
  EdbFeedback db;
  if( !InitDB( db, cenv , test)) return 0;

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
  sprintf(databrick,"%d %f, %f, %f, %f, %f, %f, %s, %d, %f, %f, %f", 1000000+BRICK, MINX, MAXX, MINY, MAXY, MINZ, MAXZ, BS_ID, BRICK, ZEROX, ZEROY, ZEROZ);
  db.eDB->AddEventBricks(databrick);
  
  /*********** Getting brick id ***********/
  char *id_eventbrick=0;
  db.eDB->GetId_EventBrick( Form("%d",BRICK), BS_ID, id_eventbrick);
  printf("%s\n",id_eventbrick);
 
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
    db.eDB->AddPlate( atoi(id_eventbrick), dataplate);
  }

  time_t tf = time(NULL);
  fprintf(stderr,"AddBrick completed\n");
  fprintf(stderr,"Elapsed time = %ld seconds\n",tf-ti);
  return 1;
}

