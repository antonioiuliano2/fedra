//-- Author :  Valeri Tioukov   27/05/2012

#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbPlateAlignment.h"

using namespace std;
using namespace TMath;

void AlignRawSet(EdbID idset1, EdbID idset2, TEnv &cenv);
int AlignRaw(EdbID id1, EdbID id2, TEnv &cenv, EdbAffine2D *applyAff);

void print_help_message()
{
  cout<< "\nUsage: \n";
  cout<< "\t  emalignraw  -A=idA    -B=idB    [-v=DEBUG] \n";
  cout<< "\t  emalignraw  -setA=IDA -setB=IDB [-v=DEBUG] \n\n";

  cout<< "\t\t  idA      - id of the first piece formed as BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t  idB      - id of the second piece formed as BRICK.PLATE.MAJOR.MINOR \n";
  
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<< "\n If the parameters file (alignraw.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into alignraw.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for the new alignrawment
  
  cenv.SetValue("fedra.alignRaw.offsetMax" , 500. );
  cenv.SetValue("fedra.alignRaw.DZ"        ,  25.   );
  cenv.SetValue("fedra.alignRaw.DPHI"      ,   0.02 );
  cenv.SetValue("fedra.alignRaw.AFID1"     , 1);
  cenv.SetValue("fedra.alignRaw.AFID2"     , 1);
  cenv.SetValue("fedra.alignRaw.side1"     , 1);
  cenv.SetValue("fedra.alignRaw.side2"     , 1);
  cenv.SetValue("fedra.alignRaw.Z1"        , 0);
  cenv.SetValue("fedra.alignRaw.Z2"        , 0);
  cenv.SetValue("fedra.alignRaw.sigmaR"    , 2.5 );
  cenv.SetValue("fedra.alignRaw.sigmaT"    , 0.005 );
  
  cenv.SetValue("fedra.alignRaw.ICUT1"           , "-1     -500. 500.   -500.   500.    -1.   1.      -1.   1.       8.  50.");
  cenv.SetValue("fedra.alignRaw.ICUT2"           , "-1     -500. 500.   -500.   500.    -1.   1.      -1.   1.       8.  50.");
  
  cenv.SetValue("fedra.alignRaw.path1"      , -1 );
  cenv.SetValue("fedra.alignRaw.path2"      , -1 );

  cenv.SetValue("fedra.alignRaw.DoFine"      ,  1    );
  cenv.SetValue("fedra.alignRaw.SaveCouples" ,  1    );

  cenv.SetValue("emalignRaw.outdir"          , ".."  );
  cenv.SetValue("emalignRaw.env"             , "alignraw.rootrc");
  cenv.SetValue("emalignRaw.EdbDebugLevel"   ,  1    );
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("alignrawenv");
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("emalignraw.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("emalignraw.env"            , "alignraw.rootrc");
  const char *outdir = cenv.GetValue("emalignraw.outdir"         , "..");
  
  bool      do_single   = false;
  bool      do_set      = false;
  EdbID     idA,idB;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if(!strncmp(key,"-setA=",6))
    {
       if(strlen(key)>6) if(idA.Set(key+6))   do_set=true;
    }
    else if(!strncmp(key,"-setB=",6))
    {
      if(strlen(key)>6) if(idB.Set(key+6))   do_set=true;
    }
    else if(!strncmp(key,"-A=",3))
    {
      if(strlen(key)>3) if(idA.Set(key+3))   do_single=true;
    }
    else if(!strncmp(key,"-B=",3))
    {
      if(strlen(key)>3) if(idB.Set(key+3))   do_single=true;
    }
    else if(!strncmp(key,"-env=",5)) 
    {
      if(strlen(key)>5)	env=key+5;
    }
    else if(!strncmp(key,"-o=",3)) 
    {
      if(strlen(key)>3)	outdir = key+3;
    }
    else if(!strncmp(key,"-v=",3))
    {
      if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
    }
  }

  if(!(do_single||do_set))   { print_help_message(); return 0; }
  if(  do_single&&do_set )   { print_help_message(); return 0; }

  cenv.SetValue("emalignraw.env"            , env);
  cenv.ReadFile( cenv.GetValue("emalignraw.env"   , "alignraw.rootrc") ,kEnvLocal);
  cenv.SetValue("emalignraw.outdir"         , outdir);

  EdbScanProc sproc;
  sproc.eProcDirClient = cenv.GetValue("emalignraw.outdir","..");
  cenv.WriteFile("alignraw.save.rootrc");

  printf("\n----------------------------------------------------------------------------\n");
  printf("alignraw  %s and  %s\n"      ,idA.AsString(),idB.AsString()	   );
  printf(  "----------------------------------------------------------------------------\n\n");

  if(do_set) 
  {
    AlignRawSet(idA,idB,cenv);
  }
  else if(do_single) 
  {
    AlignRaw(idA,idB,cenv, 0);
  }

  cenv.WriteFile("alignraw.save.rootrc");
  return 1;
}

//______________________________________________________________________________
void AlignRawSet(EdbID idset1, EdbID idset2, TEnv &cenv)
{
  // assuming that exist the scan sets for idset1 and idset2 
  // for each plate do the alignment raw from 1 to 2 and write id1_id2.aff.par
  EdbScanProc sproc;
  sproc.eProcDirClient=cenv.GetValue("emalignraw.outdir"         , "..");
  
  EdbAffine2D aff(cenv.GetValue("emalignRaw.applyAff"         , "1 0 0 1 0 0") );

  EdbScanSet *ss1 = sproc.ReadScanSet(idset1);   if(!ss1) return;
  EdbScanSet *ss2 = sproc.ReadScanSet(idset2);   if(!ss2) return;
  int n = ss1->eIDS.GetEntries();
  for(int i=0; i<n; i++) {
    EdbID *id1  = ss1->GetID(i);
    EdbID *id2  = ss2->FindPlateID(id1->ePlate);
    if(id2) AlignRaw(*id1,*id2,cenv, &aff);
  }
}

//______________________________________________________________________________
int AlignRaw(EdbID id1, EdbID id2, TEnv &cenv, EdbAffine2D *applyAff)
{
  // Align raw segments patterns. Typical application: align 2 different scannings of the same emulsion plate.

  EdbScanProc sproc;
  sproc.eProcDirClient=cenv.GetValue("emalignraw.outdir"         , "..");
  
  int npat=0;
  Log(1,"\nAlignRaw","%s_%s", id1.AsString(), id2.AsString() );
  int       side1      = cenv.GetValue("fedra.alignRaw.side1"     , 1);
  int       side2      = cenv.GetValue("fedra.alignRaw.side2"     , 1);
  float        z1      = cenv.GetValue("fedra.alignRaw.Z1"        , 0);
  float        z2      = cenv.GetValue("fedra.alignRaw.Z2"        , 0);
  float      sigmaR    = cenv.GetValue("fedra.alignRaw.sigmaR"    , 2.5 );
  float      sigmaT    = cenv.GetValue("fedra.alignRaw.sigmaT"    , 0.005 );
  float      offsetMax = cenv.GetValue("fedra.alignRaw.offsetMax" , 500. );
  int        path1     = cenv.GetValue("fedra.alignRaw.path1"      , -1 );
  int        path2     = cenv.GetValue("fedra.alignRaw.path2"      , -1 );

  float      DZ     = cenv.GetValue("fedra.alignRaw.DZ"     ,     25.   );
  float      DPHI   = cenv.GetValue("fedra.alignRaw.DPHI"   ,     0.02 );

  EdbID idset1 =id1; idset1.ePlate =0;
  EdbID idset2 =id2; idset2.ePlate =0;
  EdbRunAccess r1;  if(!sproc.InitRunAccessNew(r1,idset1,id1.ePlate)) return 0;
  EdbRunAccess r2;  if(!sproc.InitRunAccessNew(r2,idset2,id2.ePlate)) return 0;
  r1.eAFID = cenv.GetValue("fedra.alignRaw.AFID1"   , 1);
  r2.eAFID = cenv.GetValue("fedra.alignRaw.AFID2"   , 1);

  r1.AddSegmentCut(1,cenv.GetValue("fedra.alignRaw.ICUT1"      , "-1") );
  r2.AddSegmentCut(1,cenv.GetValue("fedra.alignRaw.ICUT2"      , "-1") );

  EdbPattern p1, p2;
  //r1.Print();
  r1.GetPatternDataForPrediction( path1, side1, p1 );
  
  
  EdbScanSet *set1  = sproc.ReadScanSet(idset1);
  if(set1) {
    EdbPlateP *p = set1->GetPlate(id1.ePlate);
    p->GetAffineXY()->Print();
    if(p)       p1.Transform( p->GetAffineXY() );
  }

  if(applyAff) p1.Transform(applyAff);
  
  //r2.Print();
  if(path2>=0)  r2.GetPatternDataForPrediction( path2, side2, p2 );
  else  {
    float xmin=p1.Xmin(),  xmax=p1.Xmax();
    float ymin=p1.Ymin(),  ymax=p1.Ymax();
    EdbSegP s(0, (xmin+xmax)/2., (ymin+ymax)/2., 0,0);
    float dx=(xmax-xmin)/2., dy=(ymax-ymin)/2.;
    float size = Sqrt( dx*dx+dy*dy ) + offsetMax+200.;
    if(applyAff) s.Transform(applyAff);
    r2.GetPatternXY(s, side2, p2, size);
  }

  EdbPlateAlignment av;
  av.SetSigma(sigmaR,sigmaT);
  av.eOffsetMax = offsetMax;
  av.eDZ        = DZ;
  av.eDPHI      = DPHI;
  av.eDoFine = cenv.GetValue("fedra.alignRaw.DoFine"      ,  1    );
  av.eSaveCouples = cenv.GetValue("fedra.alignRaw.SaveCouples" , 1);
  TString dataout;  sproc.MakeAffName(dataout,id1,id2,"al.root");
  av.InitOutputFile( dataout );

  av.Align( p1, p2, z2-z1);
  av.CloseOutputFile();

  //av.eCorrL[0].Print();
  sproc.UpdateAFFPar( id1, id2, av.eCorrL[0] );
  return npat;
}

