#include "iostream.h"
#include "EdbDataSet.h"

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      cout<< "usage: \n \trecset input_data_set_file \n\n";
      return 0;
    };
  char* name = argv[1];

  EdbDataProc proc(name);
  proc.Process();
  return 0;
}
