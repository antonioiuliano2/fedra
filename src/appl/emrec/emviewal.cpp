//-- Author :  Valeri Tioukov   8/04/2016

#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbViewMap.h"

using namespace std;
using namespace TMath;

void ViewAlPlate( const EdbID id, TEnv &cenv );
void ViewAlPlate(const char *fin, const char *fout, TEnv &env );

void print_help_message()
{
  cout<< "\nCalculate view coordinate corrections using View-to-View alignment information calculated by LASSO. \nStore the result in the id.va.root files by default. The corrections applied by emlink if AFID=3\n\n";
  cout<< "\nUsage: \n";
  cout<< "\t  emviewal  -id=ID     [-alg=ALG  -v=DEBUG] \n";
  cout<< "\t  emviewal  -set=IDSET [-alg=ALG  -v=DEBUG] \n";

  cout<< "\n If the data location directory is not explicitly defined\n";
  cout<< " the current directory assumed to be the brick directory \n";
  cout<< "\n If the parameters file (viewal.rootrc) is not present - the default \n";
  cout<< " ones are used. After the execution the used parameters saved into viewal.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters
  cenv.SetValue("fedra.viewal.algorithm" , 0);
  cenv.SetValue("fedra.viewal.cut"       , "1");
  cenv.SetValue("viewal.outdir"          , ".."  );
  cenv.SetValue("viewal.env"             , "viewal.rootrc");
  cenv.SetValue("viewal.EdbDebugLevel"   ,  1    );
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("viewalenv");
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("viewal.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("viewal.env"            , "viewal.rootrc");
  const char *outdir = cenv.GetValue("viewal.outdir"         , "..");
  
  bool      do_single   = false;
  bool      do_set      = false;
  EdbID     id;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if(!strncmp(key,"-id=",4))
    {
      if(strlen(key)>4) if(id.Set(key+4))   do_single=true;
    }
    else if(!strncmp(key,"-set=",5))
    {
      if(strlen(key)>5) if(id.Set(key+5))   do_set=true;
    }
    else if(!strncmp(key,"-alg=",5))
    {
      if(strlen(key)>5) cenv.SetValue("fedra.viewal.algorithm" , atoi(key+5) );
    }
    else if(!strncmp(key,"-v=",3))
    {
      if(strlen(key)>3) gEDBDEBUGLEVEL = atoi(key+3);
    }
  }

  if(!(do_single||do_set))   { print_help_message(); return 0; }
  if(  do_single&&do_set )   { print_help_message(); return 0; }

  cenv.SetValue("viewal.env"            , env);
  cenv.ReadFile( cenv.GetValue("viewal.env"   , "viewal.rootrc") ,kEnvLocal);
  cenv.SetValue("viewal.outdir"         , outdir);

  if(do_single) 
  {
    ViewAlPlate(id, cenv);
  }

  cenv.WriteFile("viewal.save.rootrc");
  return 1;
}

//-----------------------------------------------------------------------
void ViewAlPlate( const EdbID id, TEnv &cenv )
{
  EdbScanProc proc; proc.eProcDirClient=cenv.GetValue("viewal.outdir"         , "..");
  TString rfile;   proc.MakeFileName(rfile,id,"raw.root");
  TString vafile;  proc.MakeFileName(vafile,id,"va.root");
  ViewAlPlate( rfile.Data(), vafile.Data(), cenv);
}

//-----------------------------------------------------------------------
void ViewAlPlate(const char *fin, const char *fout, TEnv &cenv )
{
  int algorithm = cenv.GetValue("fedra.viewal.algorithm" , 0);
  TCut cut("cut", cenv.GetValue("fedra.viewal.cut"       , "1") );
  
  Log(2,"\nViewAlPlate","with alg=%d, cut=%s   %s -> %s\n", algorithm, cut.GetTitle(),fin,fout );

  EdbViewMap vm;
  vm.ReadViewsHeaders(fin, cut);    // read headers from runfile, fill eViewHeaders
  vm.ReadPinViewsHeaders(fin); // read headers from runfile, fill ePinViewHeaders
  vm.ReadViewAlign(fin);       // read ViewAlign from runfile, fill eALcp
  vm.eAlgorithm=algorithm;
  
  if(algorithm==2) {
    vm.MakeAHTnocorr();          // create tree with alignment parameters and views headers
    vm.SaveLinesCorrToRun(fin);   // correct hysteresis only
  }
  else if(algorithm==3) {
    vm.MakeAHTnocorr();          // create tree with alignment parameters and views headers
    vm.SaveColsCorrToRun(fin);   // correct hysteresis only (colomns)
  }
  else {
    vm.AddInverseAlign();
  
    vm.InitAR();               // init eAR structure
    vm.FillAR();               // fill eAR structure (assumed that view entry==h.GetViewID())
    vm.CheckViewFrameAlignQuality(fin);
  
    vm.FillALcp();                        //fill neighbouring
  //vm.CheckView(0,0,225);
  //vm.CheckView(0,0,112);
  
    vm.DoCorrection();
    vm.CorrectToStage();
  //vm.ConvertRun(fin,fout);
    vm.SaveCorrToRun(fin);
  }
}
