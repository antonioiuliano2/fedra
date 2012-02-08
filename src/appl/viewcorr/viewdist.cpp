//-- Author :  Valeri Tioukov   22/06/2011

#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbRun.h"
#include "EdbViewMatch.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  viewdist -f=FILE [-v=DEBUG -add=primary_correction.txt] \n";
  cout<< "\t\t  DEBUG    - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  cout<< "\nExample: \n";
  cout<< "\t  viewdist -f=one_layer.root -v=2\n";
  cout<< "\n If the parameters file (viewdist.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into viewdist.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for the new alignment
  cenv.SetValue("viewdist.env"             , "viewdist.rootrc");
  cenv.SetValue("viewdist.EdbDebugLevel"   ,  1    );
  cenv.SetValue("viewdist.NClMin" , 20);
  cenv.SetValue("viewdist.R2CenterMax" , 15.);
  cenv.SetValue("viewdist.Rmax" ,  1.);
  cenv.SetValue("viewdist.Xpix" , 0.30625);
  cenv.SetValue("viewdist.Ypix" , 0.30714);
  cenv.SetValue("viewdist.NXpix", 1280);
  cenv.SetValue("viewdist.NYpix", 1024);
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("viewenv");
  
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("viewdist.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("viewdist.env"            , "viewdist.rootrc");
  
  const char *addfile=0;
  const char *fname=0;
  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-f=",3)) 
      {
	if(strlen(key)>3)	fname = key+3;
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
    else if(!strncmp(key,"-add=",5))
      {
	if(strlen(key)>5)	addfile = key+5;
      }
  }

  if(!fname)   { print_help_message(); return 0; }

  cenv.SetValue("viewdist.env"            , env);
  cenv.ReadFile( cenv.GetValue("viewdist.env"   , "viewdist.rootrc") ,kEnvLocal);


  EdbRun run(fname);

  EdbViewMatch vm;
  vm.SetPar(cenv);
  vm.InitGMap();
  vm.InitCorrMap();

  int      n = run.GetEntries();
  printf("process file %s with %d views\n",fname,n);
  
  EdbView *v = run.GetView();

  for(int i=0; i<n; i++) {
    run.GetEntry(i,1,1);
    int ncl = v->Nclusters();
    printf("%6d ", ncl);

    for(int ic=0; ic<ncl; ic++) {
      EdbCluster *c = v->GetCluster(ic);
      vm.AddCluster( c->eX, c->eY, c->eZ, v->GetXview(), v->GetYview(), v->GetViewID(), c->GetFrame() );
    }
  }
  printf("\n");

  
  vm.CalculateGrRef();
  vm.CalculateCorr();
  vm.eOutputFile = new TFile("map.root","RECREATE");
  vm.DrawCorrMap();
  vm.GenerateCorrectionMatrix(addfile);
  vm.eOutputFile->Close();
  
  vm.Print();

  cenv.WriteFile("viewdist.save.rootrc");
  return 1;
}
