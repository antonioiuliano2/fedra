//-- Author :  Valeri Tioukov   27.06.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbTest                                                            //
//                                                                      //
// head Edb class for online data taking                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "TSystem.h"

#ifndef ROOT_EdbTest
#include "EdbTest.h"
#endif

ClassImp(EdbTest)

//______________________________________________________________________________
EdbTest::EdbTest( char *name, char *title ) 
#ifndef __CINT__
#ifndef __MAKECINT__
  : TROOT(name, title)
#endif /* __MAKECINT__ */
#endif /* __CINT__ */
{
  //  Init();
}

//______________________________________________________________________________
EdbTest::~EdbTest()
{
  if( eRun )  delete eRun;
}
