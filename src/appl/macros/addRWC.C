#ifndef __CINT__
#include "EdbRun.h"
#include "EdbRunAccess.h"
#include "TSystem.h"
#include "Riostream.h"
#endif

int addRWC( char* rwcname, const char* rootname)
{
   EdbRunAccess ra( rootname );

   if ( gSystem->AccessPathName( rwcname ) )	//check disk access
      return 1;
   else 
      ra.InitRunFromRWC( rwcname, false, "NOCL" );
   
   if ( gSystem->AccessPathName( rootname ) )	//check disk access
   	return 1;
   else {
      ra.GetRun()->GetTree()->SetMaxTreeSize(10e9);
      ra.GetRun()->Close();
   }
   
  return 0;
}

#ifndef __CINT__
int main( int argc, char *argv[] )
{
   if ( argc!=3) {
      cout << "addRWC.exe <rwcname> <rootname>\n";
      return 1;
   }

   cout << "rwcname  : " << argv[1]  << endl;
   cout << "rootname : " << argv[2] << endl;

   return addRWC( argv[1] , argv[2] ) ;
}
#endif
