#include <iostream>
#include "EdbRunAccess.h"

//
//   checkrun runfile.root logfile
//

using namespace std;

int checkrun( char* name )
{
   EdbRunAccess a(name);
   if( !a.InitRun() )  
      return 0;
   a.CheckRunLine();
   return 1;
}

//___________________________________________________________________
#ifndef __CINT__
int main(int argc, char* argv[])
{
   if (argc < 2)
   {
      cout<< "usage: \n \tcheckrun runfile.root > logfile \n";
      cout<< "return values: -1-not found, 0-bad, 1-ok\n";
      cout<<endl;
      return 0;
   };

   char *name = argv[1];
 
   return checkrun( name ) ;
}
#endif
