/* ********************************************************************/
//
// source:
//
// type:      source code
//
// created:   26. Okt 2000
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: Class to initialize the Vt++ library
//
// changes:
// 26 Okt 2000 (TG) creation
// 14 Nov 2000 (TG) declared some member functions static
//
/* ********************************************************************/
#ifndef VT_INI_HH
#define VT_INI_HH

#include <iosfwd>

namespace VERTEX {
  class abc;  // to fake doc++

  /**
     The initialization of the package is done automatically when the
     shared library is loaded into memory. An instance should only be created to
     obtain the information provided by the member functions.
     @memo Initialization of Vt++ package
     @author T. Glebe
  */
  //==============================================================================
  // VtIni
  //==============================================================================
  class VtIni {
  public:
    /// constructor
    VtIni();

    /// get string with version number
    inline static const char* const version() { return Version; }
    /// get string with package name
    inline static const char* const name()    { return Name; }
    /// get birthday \& time of package version
    inline static const char* const date()    { return Date; }
    /// toggle debug flag
    static const bool debug(); 
    /// return debug flag
    inline static const bool isDebug() { return Debug; }

    /** @name --- Expert functions --- */
    ///
    void print(std::ostream& os) const;

  private:
#ifndef __CINT__
    /// not available in CINT
    class initializer {
    public:
      initializer();
    };
    
    /// constructor call for initalization
    static initializer doIni; 
#endif
    static const char *Version;
    static const char *Name;
    static const char *Date;
    static bool        Debug;
  }; // end of class VtIni


  //============================================================================
  // operator<<
  //============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const VtIni& t ) {
    t.print(os);
    return os;
  }
} // end of namespace VERTEX
#endif
