//-- Author :  Valeri Tioukov   11/06/2008

#include <string.h>
#include <iostream>
#include "EdbLog.h"
#include "EdbScanProc.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  makescanset -set=setID  [-suff=suffix -dz=DZ -o=DATA_DIRECTORY -v=DEBUG] \n";
  cout<< "\t\t  suffix      - file suffix to be searched - (default: raw.root \n)";
  cout<< "\t\t  DZ          - plate-to-plate step (default = -1300.)\n";
  cout<< "\t\t  DEBUG       - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  cout<< "\t\t -noaff       - do not read aff-files - the default is yes \n";
  cout<< "\t\t -resetaff    - recreate aff-files using values from set.root\n";
  cout<< "\t\t -refplate=id - recalculate set.root in respect to plate [id] and update it\n";
  cout<< "\t\t -resetpar    - reset pxxx/x.x.x.x.par-files (shrinkage correction, etc) using values from set.root\n";
  cout<< "\t\t -reset       - reset *.aff.par and *.par files\n";
  cout<< "\t\t -updateaff=\"aff\"   - update scanset with a given aff\n";
  cout<< "\t\t -updatesetrawaff -A=ida -B=idb   - update scanset with affine transformations found between plates of the scansets A and B\n";
  cout<< "\t\t -updatesetaff -A=ida   - update scanset with affine transformations of set A\n";
  cout<< "\t\t -copyset      -A=ida   - copy A into set - keep geometry, modify only ID's\n";
  cout<< "\nExample: \n";
  cout<< "\t makescanset -set=4554.0.1.1000 -o=/scratch/BRICKS \n";
  cout<< "\n If the data location directory if not explicitly define\n";
  cout<< "\t the current directory will be assumed to be the brick directory \n";
  cout<<endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  gEDBDEBUGLEVEL        = 2;

  const char *outdir    = "..";
  const char *suff    = "raw.root";
  bool        do_set      = false;
  bool        noaff       = false;
  bool        reset       = false;
  bool        resetaff    = false;
  bool        resetpar    = false;
  EdbID       id;
  int         from_plate  = 57;
  int         to_plate    =  1;
  float       z0          =  0;
  float       dz          = -1300;
  float       dzbase      = 210;
  int refplate = -1;

  EdbAffine2D affup;
  bool do_updateaff=0;
  bool do_updatesetrawaff=0;
  bool do_updatesetaff=0;
  bool do_copyset=0;
  EdbID idA, idB;
  
  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if(!strncmp(key,"-set=",5))
    {
      if(strlen(key)>5)	id.Set(key+5);
      do_set=true;
    }
    else if(!strncmp(key,"-o=",3)) 
    {
      if(strlen(key)>3)	outdir=key+3;
    }
    else if(!strncmp(key,"-suff=",6)) 
    {
      if(strlen(key)>6)	suff=key+6;
    }
    else if(!strncmp(key,"-noaff",6)) 
    {
      noaff=true;
    }
    else if(!strncmp(key,"-resetaff",9)) 
    {
      resetaff=true; 
    }
    else if(!strncmp(key,"-refplate=",10)) 
    {
      refplate= atoi(key+10);
      printf("\n******** refplate = %d *********** \n",refplate);
    }
    else if(!strncmp(key,"-resetpar",9)) 
    {
      resetpar=true;
    }
    else if(!strncmp(key,"-reset",6)) 
    {
      reset=true;
    }
    else if(!strncmp(key,"-v=",3))
    {
      if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
    }
    else if(!strncmp(key,"-updateaff=",11)) 
    {
      if(strlen(key)>11) {
        affup.Set(key+11);
        affup.Print();
        do_updateaff=1;
      }
    }
    else if(!strncmp(key,"-updatesetrawaff",16)) 
    {
        do_updatesetrawaff=1;
    }
    else if(!strncmp(key,"-updatesetaff",13)) 
    {
      do_updatesetaff=1;
    }
    else if(!strncmp(key,"-copyset",8)) 
    {
      do_copyset=1;
    }
    else if(!strncmp(key,"-A=",3))
    {
      if(strlen(key)>3) idA.Set(key+3);
    }
    else if(!strncmp(key,"-B=",3))
    {
      if(strlen(key)>3) idB.Set(key+3);
    }
    else if(!strncmp(key,"-dz=",4))
    {
      if(strlen(key)>4)	dz = atof(key+4);
    }
    else if(!strncmp(key,"-dzbase=",8))
    {
      if(strlen(key)>8)	dzbase = atof(key+8);
    }
  }

  if(!do_set) print_help_message();
  else {
    
    EdbScanProc sproc;
    sproc.eProcDirClient=outdir;
    if(id.eBrick<1) return 0;
    Log(1,"makescanset"," for id %s",  id.AsString());
    
    if     (resetaff) {
      EdbScanSet *sc = sproc.ReadScanSet(id);
      sproc.MakeAFFSet(*sc);
    }
    else if(refplate>-1) {
      EdbScanSet *sc = sproc.ReadScanSet(id);
      sc->SetAsReferencePlate(refplate);
      //sc->Print();
      sproc.WriteScanSet(id,*sc);
    }
    else if(do_updateaff)
    {
      printf("updateaff: %s\n",affup.AsString());
      sproc.UpdateSetWithAff(id,affup);
    }
    else if(do_updatesetrawaff)
    {
      printf("Update set raw aff: %s with %s_%s\n",id.AsString(), idA.AsString(), idB.AsString() );
      sproc.UpdateSetWithAff(id,idA,idB);
    }
    else if(do_updatesetaff)
    {
      printf("Update set aff: %s with %s\n",id.AsString(), idA.AsString() );
//      sproc.UpdateSetWithAff(id,idA);
      EdbScanSet  *ss  = sproc.ReadScanSet(id);     if(!ss)  return 0;
      EdbScanSet *ssu  = sproc.ReadScanSet(idA);    if(!ssu)  return 0;
      ss->TransformBrick(*ssu);
      sproc.WriteScanSet( id ,*ss );
      SafeDelete(ss);  SafeDelete(ssu);
    }
    else if(do_copyset)
    {
      printf("Copy set: %s into %s\n", idA.AsString(),id.AsString() );
      EdbScanSet *ssu  = sproc.ReadScanSet(idA);    if(!ssu)  return 0;
      EdbScanSet ss;
      printf("1\n");
      ss.Copy(*ssu);
      printf("2\n");
      ss.UpdateIDS(id.eBrick, id.eMajor, id.eMinor);
      printf("3\n");
      sproc.WriteScanSet( id ,ss );
      printf("4\n");
      sproc.PrepareSetStructure(ss);
      //SafeDelete(ssu);
    }
    else {
      EdbScanSet sc(id);
      sc.MakeNominalSet(id,from_plate, to_plate, z0, dz, 1, dzbase);
      sproc.MakeScannedIDList( id, sc, 60, 0, suff);
      sproc.WriteScanSet(id,sc);
      if(resetpar) sproc.MakeParSet(sc);
      else if(reset)    sproc.PrepareSetStructure(sc);
      else if(!noaff)  {
        sproc.UpdateSetWithPlatePar(id);
        sproc.UpdateSetWithAff(id,id);
      }
    }
    if(gEDBDEBUGLEVEL>1) {
      EdbScanSet *ss = sproc.ReadScanSet(id);
      if(ss) ss->Print();
    }
  }

}


