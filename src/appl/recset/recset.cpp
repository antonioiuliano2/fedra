#include "iostream.h"
#include "EdbDataSet.h"

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
      cout<< "usage: \n \trecset { link | align | track} input_data_set_file \n\n";
      return 0;
    };
  char *key  = argv[1];
  char *name = argv[2];

  EdbDataProc proc(name);

  if(!strcmp(key,"link")  ) {
    proc.Link();
  } else if(!strcmp(key,"align")) {
    proc.Align();
  } else if(!strcmp(key,"track")) {
    proc.LinkTracks();
  }
  return 0;
}
