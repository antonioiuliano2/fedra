#include <iostream>
#include "EdbDataSet.h"

//
//   recset -ccd -l -ang -a -f -t5 -raw -nu
//

using namespace std;

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
      cout<< "usage: \n \trecset { -l | -a | -t | -f | ...} input_data_set_file \n\n";
      cout<< "\t\t  -ccd  - to remove ccd defects (update par/xxx.par file)\n";
      cout<< "\t\t  -l    - link up/down\n";
      cout<< "\t\t  -ang  - correct up/down angles offset and rotations\n";
      cout<< "\t\t  -a[n] - plate to plate alignment (if n=2: rigid patterns) \n";
      cout<< "\t\t  -f[n] - fine alignment based on passed-throw tracks (if n=2: rigid patterns)\n";
      cout<< "\t\t  -t[n] - tracking (if n>1, holes isertion started - historical option - do not recommended!)\n";
      cout<< "\t\t  -t -p[p] - tracking&propagation (p is the momentum of the particle)\n";
      //      cout<< "\t\t  -rt  - raw tracking \n";
      cout<< "\t\t  -nu   - suppress the update of par files\n";
      cout<<endl;
      return 0;
    };

  int doCCD=0, doLink=0, doAlign=0, doTrack=0, 
    doFine=0, doAngles=0, doRaw=0, noUpdate=0;

  float doPropagation=-1;

  char *name = argv[argc-1];

  for(int i=1; i<argc-1; i++ ) {
    char *key  = argv[i];

    if(!strcmp(key,"-l"))                doLink=1;

    if(!strncmp(key,"-a",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%d",&doAlign);
      if(doAlign==0)                     doAlign=1;
    }

    if(!strncmp(key,"-f",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%d",&doFine);
      if(doFine==0)                     doFine=1;
    }

    if(!strncmp(key,"-t",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%d",&doTrack);
      if(doTrack==0)                     doTrack=1;
    }

    if(!strncmp(key,"-p",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%f",&doPropagation);
    }

    if(!strcmp(key,"-ccd"))  doCCD=1;
    if(!strcmp(key,"-ang"))  doAngles=1;
    if(!strcmp(key,"-rt"))   doRaw=1;
    if(!strcmp(key,"-nu"))   noUpdate=1;
  }

  printf("%d %d %d %d %d %d %d %f %d %s\n",
	 doCCD, doLink, doAlign, doTrack, doFine, doAngles, doRaw, doPropagation, noUpdate, name);

  EdbDataProc proc(name);

  proc.SetNoUpdate(noUpdate);
  if(doCCD)              { proc.CheckCCD();               doCCD=0; }
  if(doLink)             { proc.Link();                   doLink=0; }
  if(doTrack&&doAlign)   { proc.AlignLinkTracks(doTrack,doAlign); doTrack=0; doAlign=0; }
  if(doAlign)            { proc.Align(doAlign);           doAlign=0; }
  if(doAngles)           { proc.CorrectAngles();          doAngles=0; }
  if(doTrack)            { proc.LinkTracks(doTrack, doPropagation); doTrack=0; doPropagation=-1; }
  if(doFine)             { proc.FineAlignment(doFine);    doFine=0; }
  if(doRaw)              { proc.LinkRawTracks(doRaw);     doRaw=0; }

  return 0;
}
