#ifndef __CINT__

#include "EdbRun.h"
#include "EdbRunAccess.h"
#include "TSystem.h"
#include "Riostream.h"

#endif

int addRWD( char* rwdname, const char* rootname)
{
   EdbRun* r;
   if ( gSystem->AccessPathName( rootname ) )
      r = new EdbRun( rootname, "RECREATE" );
   else
      r = new EdbRun( rootname, "UPDATE" );
   r->GetTree()->SetMaxTreeSize(10e9);   
   EdbRunAccess ra(r);

   if ( gSystem->AccessPathName( rwdname ) )	//check disk access
   	return 1;
   else 
   	ra.AddRWDToRun( rwdname, "NOCL" );

   if ( gSystem->AccessPathName( rootname ) )	//check disk access
   	return 1;
   else 
      r->Close();

   return 0 ;
}

#ifndef __CINT__
int main( int argc, char *argv[] )
{
   if ( argc!=3) {
      cout << "addRWD.exe <rwdname> <rootname>\n";
      return 1;
   }

   cout << "rwdname  : " << argv[1]  << endl;
   cout << "rootname : " << argv[2] << endl;

   return addRWD( argv[1], argv[2] );
}
#endif
