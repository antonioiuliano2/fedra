//-- Author :  Valeri Tioukov   11/06/2008

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"

using namespace std;

void print_help_message()
{
  cout<< "\nUsage: \n\t  emlink -id=ID [-p=NPRE -f=NFULL -o=DATA_DIRECTORY -a -v=DEBUG] \n";
  cout<< "\t  emlink -set=ID [-p=NPRE -f=NFULL -o=DATA_DIRECTORY -a -v=DEBUG] \n\n";
  cout<< "\t\t  ID    - id of the data piece or dataset formed as BRICK.PLATE.MAJOR.MINOR \n";
  cout<< "\t\t  NPRE  - number of the prealignments (default is 0)\n";
  cout<< "\t\t  NFULL - number of the fullalignments (default is 0)\n";
  cout<< "\t\t  -a    - apply the angular correction when do prelinking (default is not)\n";
  cout<< "\t\t  -new  - new linking\n";
  cout<< "\t\t  DEBUG - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  cout<< "\nExample: \n";
  cout<< "\t  o2root -id4554.10.1.0 -o/scratch/BRICKS \n";
  cout<< "\nThe data location directory if not explicitly defined will be taken from .rootrc as: \n";
  cout<< "\t  emrec.outdir:      /scratch/BRICKS \n";
  cout<< "\t  emrec.EdbDebugLevel:      1\n";
  cout<<endl;
}

int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  const char *outdir    = gEnv->GetValue("emrec.outdir"   , "./");
  gEDBDEBUGLEVEL        = gEnv->GetValue("emrec.EdbDebugLevel" , 1);

  bool      do_id      = false;
  bool      do_set     = false;
  bool      do_new     = false;
  Int_t     brick=0, plate=0, major=0, minor=0;
  Int_t     npre=0,  nfull=0;
  Int_t     correct_ang=0;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-id=",4)) 
      {
	if(strlen(key)>4)	sscanf(key+4,"%d.%d.%d.%d",&brick,&plate,&major,&minor);
	do_id=true;
      }
    else if(!strncmp(key,"-set=",5))
      {
	if(strlen(key)>5)	sscanf(key+5,"%d.%d.%d.%d",&brick,&plate,&major,&minor);
	do_set=true;
      }
    else if(!strncmp(key,"-o=",3)) 
      {
	if(strlen(key)>3)	outdir=key+3;
      }
    else if(!strncmp(key,"-p=",3))
      {
	if(strlen(key)>3)	npre = atoi(key+3);
      }
    else if(!strncmp(key,"-f=",3))
      {
	if(strlen(key)>3)	nfull = atoi(key+3);
      }
    else if(!strncmp(key,"-a",2))
      {
	correct_ang=1;
      }
    else if(!strncmp(key,"-new",4))
      {
	do_new=true;
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }

  if(!(do_id||do_set))   { print_help_message(); return 0; }

  EdbScanProc sproc;
  sproc.eProcDirClient=outdir;
  if(do_id) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("link run with id= %d.%d.%d.%d\n", brick,plate, major,minor);
    printf("----------------------------------------------------------------------------\n\n");

    EdbID id(brick,plate,major,minor);
    sproc.LinkRunAll(id, npre, nfull, correct_ang);
  }
  if(do_set) {
    printf("\n----------------------------------------------------------------------------\n");
    printf("link set with id= %d.%d.%d.%d\n", brick,plate, major,minor);
    printf("----------------------------------------------------------------------------\n\n");

    EdbID id(brick,plate,major,minor);
    EdbScanSet *ss = sproc.ReadScanSet(id);
    if(ss) {
      if(do_new) sproc.LinkSetNew(*ss, *gEnv);
      else       sproc.LinkSet(*ss, npre, nfull, correct_ang);
    }
  }

  return 1;
}



/*

//--------------------------------------------------------------------
void FedraLink::LinkPlate( int iplate )
{
  EdbPattern p1(0,0,0,500000),p2(0,0,0,500000);

  ReadPlate(iplate, p1, p2);

  float  z1 = eEnv.GetValue( "fedra.link.z1", 214.);
  float  z2 = eEnv.GetValue( "fedra.link.z2",   0.);
  EdbPlateP plate;
  float dz   = z2-z1;
  int   pm = (int)(dz/Abs(dz));
  plate.SetPlateLayout( dz, pm*45, pm*45);  if(gEDBDEBUGLEVEL>2) plate.PrintPlateLayout();

  p1.SetZ( plate.GetLayer(1)->Z() );  p1.SetSegmentsZ();
  p2.SetZ( plate.GetLayer(2)->Z() );  p2.SetSegmentsZ();

  Log(1,"\nFedraLink::LinkPlate", "with %d and %d segments at dz = %6.1f", p1.N(), p2.N(), dz );

  EdbAlignmentMap amap(Form("link%2.2d.root", iplate), "RECREATE");
  amap.eEnv   = &eEnv;
  amap.ePat1  = &p1;
  amap.ePat2  = &p2;
  amap.ePlate = &plate;
  //p1.Print();
  //p2.Print();
  amap.Link();
  //amap.SaveAll();
}


*/
