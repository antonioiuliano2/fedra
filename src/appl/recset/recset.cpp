#include <iostream>
#include "EdbDataSet.h"

//
//   recset -ccd -l -ang -a -f -t5 -raw -nu
//

using namespace std;

int main(int argc, char* argv[])
{
  int doCCD=0, doLink=0, doAlign=0, doTrack=0, 
    doFine=0, doAngles=0, doRaw=0, noUpdate=0, doMerge=0;

  if (argc < 3)
    {
      cout<< "usage: \n \trecset { -l | -a | -t | -f | ...} input_data_set_file \n\n";
      cout<< "\t\t  -ccd  - to remove ccd defects (update par/xxx.par file)\n";
      cout<< "\t\t  -l    - link up/down\n";
      cout<< "\t\t  -ang  - correct up/down angles offset and rotations\n";
      cout<< "\t\t  -a    - plate to plate alignment\n";
      cout<< "\t\t  -f    - fine alignment based on passed-throw tracks\n";
      cout<< "\t\t  -t[n] - tracking \n";
      cout<< "\t\t  -m[n] - tracks merging (n - the maximal permitted gap in planes)\n";
      //      cout<< "\t\t  -rt  - raw tracking \n";
      cout<< "\t\t  -nu   - suppress the update of par files\n";
      cout<<endl;
      return 0;
    };

  char *name = argv[argc-1];

  for(int i=1; i<argc-1; i++ ) {
    char *key  = argv[i];
    if(!strcmp(key,"-l"))    doLink=1;
    if(!strcmp(key,"-a"))    doAlign=1;
    if(!strncmp(key,"-t",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%d",&doTrack);
      if(doTrack==0) doTrack=1;
    }
    if(!strncmp(key,"-m",2)) {
      if(strlen(key)>2)
	sscanf(key+2,"%d",&doMerge);
    }
    if(!strcmp(key,"-ccd"))  doCCD=1;
    if(!strcmp(key,"-ang"))  doAngles=1;
    if(!strcmp(key,"-f"))    doFine=1;
    if(!strcmp(key,"-rt"))   doRaw=1;
    if(!strcmp(key,"-nu"))   noUpdate=1;
  }

  printf("%d %d %d %d %d %d %d %d %s\n",
	 doCCD, doLink, doAlign, doTrack, doFine, doAngles, doRaw, noUpdate, name);

  EdbDataProc proc(name);

  proc.SetNoUpdate(noUpdate);
  if(doCCD)              { proc.CheckCCD();               doCCD=0; }
  if(doLink)             { proc.Link();                   doLink=0; }
  if(doTrack&&doAlign)   { proc.AlignLinkTracks(doTrack); doTrack=0; doAlign=0; }
  if(doAlign)            { proc.Align();                  doAlign=0; }
  if(doAngles)           { proc.CorrectAngles();          doAngles=0; }
  if(doTrack)            { proc.LinkTracks(doTrack, doMerge); doTrack=0; doMerge=0; }
  if(doFine)             { proc.FineAlignment();          doFine=0; }
  if(doRaw)              { proc.LinkRawTracks(doRaw);     doRaw=0; }

  return 0;
}
