#ifndef ROOT_EdbTest
#define ROOT_EdbTest
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbTest                                                            //
//                                                                      //
// head Edb class for online scanning                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#ifndef __MAKECINT__
#include "TROOT.h"
#endif /* __MAKECINT__ */
#endif /* __CINT__ */

#ifndef ROOT_EdbRun
#include "EdbRun.h"
#endif


//______________________________________________________________________________
class EdbTest 
#ifndef __CINT__
#ifndef __MAKECINT__
: TROOT 
#endif /* __MAKECINT__ */
#endif /* __CINT__ */
{

 private:

  EdbRun        *eRun;                   // pointer to the run object

  TString       eOnlineDirectory;        // directory for online parameters
  TString       eDataDirectory;          // directory for scanning data

  Int_t         eRunID;                  // current run id

 public:
  EdbTest( char *name="OnlineRoot", 
	     char *title="Root object for online scanning" );
  virtual ~EdbTest();


  EdbRun  *GetRun() const { return eRun; }

  void     SetDataDirectory(   const char *dir) { eDataDirectory=dir;  }
  void     SetOnlineDirectory( const char *dir) { eOnlineDirectory=dir;  }
  int      GetRunID() { return eRunID; }

  ClassDef(EdbTest,1)  // Online application class
};


#endif /* ROOT_EdbTest */


