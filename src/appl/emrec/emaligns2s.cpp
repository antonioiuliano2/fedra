//-- Author :  Valeri Tioukov   27/06/2012

#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  emaligns2s -A=idA  -B=idB [ -dz=DZ -v=DEBUG] \n";
  cout<< "\t Standalone alignment between 2 couples trees independent to datasets\n";
  cout<< "\t do not requires the presence of dataset files.\n";
  cout<< "\t Input:  idA.cp.root idB.cp.root align.rootrc\n";
  cout<< "\t Output: AFF/idA_idB.al.root .par\n";
  cout<< "\nExample: \n";
  cout<< "\t  emaligns2s -idA=4554.11.1.1 -idB=4554.10.1.1 -dz=-1300 -v=2\n";
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<< "\n If the parameters file (align.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into align.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for the new alignment
  cenv.SetValue("fedra.align.OffsetMax"   , 1000. );
  cenv.SetValue("fedra.align.DZ"          ,  250. );
  cenv.SetValue("fedra.align.DPHI"        ,  0.02 );
  cenv.SetValue("fedra.align.SigmaR"      ,  25.  );
  cenv.SetValue("fedra.align.SigmaT"      ,  0.012);
  cenv.SetValue("fedra.align.DoFine"      ,  1    );
  cenv.SetValue("fedra.readCPcut"         , "eCHI2P<2.0&&s.eW>10&&eN1==1&&eN2==1&&s.Theta()>0.05&&s.Theta()<0.99");
  cenv.SetValue("fedra.align.SaveCouples" ,  1    );
  cenv.SetValue("emaligns2s.applyAff"         , "1 0 0 1 0 0");
  
  cenv.SetValue("emalign.outdir"          , ".."  );
  cenv.SetValue("emalign.env"             , "align.rootrc");
  cenv.SetValue("emalign.EdbDebugLevel"   ,  1    );
}


int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("alignenv");
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("emalign.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("emalign.env"            , "align.rootrc");
  const char *outdir = cenv.GetValue("emalign.outdir"         , "..");
  
  bool      do_ida      = false;
  bool      do_idb      = false;
  EdbID     idA,idB;
  float       DZ=0;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
  
    if     (!strncmp(key,"-A=",3)) 
    {
      if(strlen(key)>3) if(idA.Set(key+3))   do_ida=true;
    }
    else if     (!strncmp(key,"-B=",3)) 
    {
      if(strlen(key)>3) if(idB.Set(key+3))   do_idb=true;
    }
    else if     (!strncmp(key,"-dz=",4)) 
    {
      if(strlen(key)>4) DZ = atof(key+4);
    }
    else if(!strncmp(key,"-v=",3))
    {
      if(strlen(key)>3) gEDBDEBUGLEVEL = atoi(key+3);
    }
  }
  
  if(!(do_ida&&do_idb))   { print_help_message(); return 0; }

  cenv.SetValue("emalign.env"            , env);
  cenv.ReadFile( cenv.GetValue("emalign.env"   , "align.rootrc") ,kEnvLocal);
  cenv.SetValue("emalign.outdir"         , outdir);

  EdbScanProc sproc;
  sproc.eProcDirClient = cenv.GetValue("emalign.outdir","..");

  EdbAffine2D aff(cenv.GetValue("emaligns2s.applyAff"         , "1 0 0 1 0 0") );

  if(do_ida&&do_idb) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("align  %s and  %s  with dz = %f\n"        ,idA.AsString(), idB.AsString(), DZ);
    printf("----------------------------------------------------------------------------\n\n");

    sproc.AlignNewNopar(idA,idB,cenv,&aff, DZ);
  }

  cenv.WriteFile("align.save.rootrc");
  return 1;
}
