#ifndef __VTUTIL_HH
#define __VTUTIL_HH
// *****************************************************************************
//
// source:
//
// type:      source code
//
// created:   21. Aug 2000
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: Some helper routines
//
// changes:
// 21 Aug 2000 (TG) creation
// 20 Feb 2001 (TG) max, min templates added
// 17 Okt 2001 (TG) added/improved #defines for ARTE
//
// *****************************************************************************

namespace VERTEX {
  class abc; // to fake doc++

  /** @name --- Global utility functions --- */
  //==============================================================================
  // sqr: x*x
  //==============================================================================
  /// compute the square of a number: $x*x$
  template <class T>
  inline const T sqr(const T& x) { return x*x; }

  //==============================================================================
  // sgn: sign
  //==============================================================================
  /// compute the sign of a number
  template <class T>
  inline const short int sgn(const T& x) { return (x==0)? 0 : (x<0)? -1 : 1; }
 

  //==============================================================================
  // max: maximum
  //==============================================================================
  template <class T>
  inline const T max(const T& rhs, const T& lhs) {
    return (rhs > lhs) ? rhs : lhs;
  }


  //==============================================================================
  // min: minimum
  //==============================================================================
  template <class T>
  inline const T min(const T& rhs, const T& lhs) {
    return (rhs < lhs) ? rhs : lhs;
  }


} // end of namespace VERTEX
#endif
