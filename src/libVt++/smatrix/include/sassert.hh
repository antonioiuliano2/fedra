#ifndef __SASSERT_HH
#define __SASSERT_HH
// ********************************************************************
//
// source:
//
// type:      source code
//
// created:   04. Apr 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: An assert macro which is invoced only once (in contrast
//              to standard assert(), which is repeatedly called).
//              Sassert is more performant in case it is placed in a routine
//              which is called more than once. Otherwise assert() is more
//              performant.
//
// changes:
// 04 Apr 2001 (TG) creation
//
// ********************************************************************

/** errfunc.
    Function which prints an error message and then exits the program.

    @author T. Glebe
*/
//==============================================================================
// errfunc
//==============================================================================
static bool errfunc(const char* expr, const char* function,
		    const unsigned int line) {
    cerr << "Error in " << __FILE__ << ", function " << function
	 << ", Line " << line << ": assertion " << expr << " failed!" << endl;
    exit(-1);
  return false;
}

/** Sassert.
    An assert macro which is invoced only once (in contrast
    to standard assert(), which is repeatedly called).
    Sassert is more performant in case it is placed in a routine
    which is called more than once. Otherwise assert() is more
    performant.

    @author T. Glebe
*/
//==============================================================================
// Sassert
//==============================================================================
#define Sassert(expr)  static const bool sassertrc = (expr) ? true : errfunc(__STRING(expr),__FUNCTION__,__LINE__)
#endif
