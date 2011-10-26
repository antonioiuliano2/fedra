/**********************************************************************/
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
// Description: Vt++ initialization
//
// changes:
// 26 Okt 2000 (TG) creation
//
/**********************************************************************/


#include "vt++/CVStag.vt++"
#include "vt++/VtIni.hh"
#include "Riostream.h"
using namespace std;

namespace VERTEX {

  VtIni::initializer VtIni::doIni;
  
  const char *VtIni::Version = xcvsVERSION_vt;
  const char *VtIni::Name    = xcvsMODULE_vt;
  const char *VtIni::Date    = xcvsDAYTIME_vt;
  bool        VtIni::Debug   = false;
  
  //==============================================================================
  // Constructors
  //==============================================================================
  VtIni::VtIni() {}

  //==============================================================================
  // Constructor for private class initializer
  //==============================================================================
  VtIni::initializer::initializer() {
/*    cout << xcvsMODULE_vt 
	 << " version " << xcvsVERSION_vt 
	 << " loaded (" << xcvsDAYTIME_vt << ")." 
	 << endl;*/
  }
  
  
  //============================================================================
  // VtIni::print
  //============================================================================
  void VtIni::print(std::ostream& os) const {
    cout << name() 
	 << " version " << version() 
	 << " loaded (" << date() << ")." 
	 << endl;
  }
  
  //==============================================================================
  // debug
  //==============================================================================
  const bool VtIni::debug() {
    Debug = ((Debug==true) ? false : true); 
    return Debug;
  }
  
  
} // end of namespace VERTEX
