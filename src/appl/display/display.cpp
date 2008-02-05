//----------------------------------------------------------------------------
// Program: Emulsion Graphical Toolkit - main program
//
// Author:  Artem Chukanov (chukanov at nusun.jinr.ru)
//          31.01.2008
//
//----------------------------------------------------------------------------

#include <TROOT.h>
#include "EGraphTool.h"

int main(int argc, char* argv[])
{
  TApplication Application("Application", &argc, argv);
  EGraphTool   display(gClient->GetRoot(), 1280, 1000);
  Application.Run();

  return 0;
}




