#include "iostream.h"
#include "EdbDataSet.h"

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
      cout<< "usage: \n \trecset { l | a | t | at} input_data_set_file \n\n";
      cout<< "\t\t  l  - link up/down\n";
      cout<< "\t\t  a  - plate to plate alignment\n";
      cout<< "\t\t  t  - tracking \n";
      cout<< "\t\t  at - alignment+tracking (could be used for prealigned data)\n\n";
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
    proc.LinkTracks();
  } else if(!strcmp(key,"at")) {
    proc.AlignLinkTracks();
  }
  return 0;
}
