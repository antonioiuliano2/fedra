#include "iostream.h"
#include "EdbDataSet.h"

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
      cout<< "usage: \n \trecset { l | a | t | at | ...} input_data_set_file \n\n";
      cout<< "\t\t  ccd - to remove ccd defects (update par/xxx.par file)\n";
      cout<< "\t\t  l   - link up/down\n";
      cout<< "\t\t  ang - correct up/down angles offset and rotations\n";
      cout<< "\t\t  a   - plate to plate alignment\n";
      cout<< "\t\t  at  - alignment+tracking (could be used for prealigned data)\n";
      cout<< "\t\t  f   - fine alignment based on passed-throw tracks\n";
      cout<< "\t\t  t   - tracking \n";
      //      cout<< "\t\t  rt  - raw tracking \n";
      cout<<endl;
      return 0;
    };
  char *key  = argv[1];
  char *name = argv[2];

  EdbDataProc proc(name);

  if(!strcmp(key,"l")  ) {
    proc.Link();
  } else if(!strcmp(key,"a")) {
    proc.Align();
  } else if(!strcmp(key,"t")) {
    proc.LinkTracks(0);
  } else if(!strcmp(key,"t1")) {
    proc.LinkTracks(1);
  } else if(!strcmp(key,"at")) {
    proc.AlignLinkTracks(0);
  } else if(!strcmp(key,"at1")) {
    proc.AlignLinkTracks(1);
  } else if(!strcmp(key,"ccd")) {
    proc.CheckCCD();
  } else if(!strcmp(key,"ang")) {
    proc.CorrectAngles();
  } else if(!strcmp(key,"f")) {
    proc.FineAlignment();
  } else if(!strcmp(key,"rt")) {
    proc.LinkRawTracks(0);
  }
  return 0;
}
