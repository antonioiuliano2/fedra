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
// Description: class for square matrices
//
// changes:
// 21 Aug 2000 (TG) creation
// 04 Sep 2000 (TG) added assert() to VtDinv(), modified return type of dinv()
// 05 Sep 2000 (TG) include <string.h> added
//
// *****************************************************************************
#include "Riostream.h"
#include <string.h> // for memcpy
#include "vt++/VtSqMatrix.hh"
#include "vt++/VtVector.hh"

namespace MATRIX {
  

  //============================================================================
  // Constructors/Destructors
  //============================================================================
  VtSqMatrix::~VtSqMatrix() {}

  //============================================================================
  // VtSqMatrix::det: calc determinant with CERNLIB dfact_ routine
  //============================================================================
  double VtSqMatrix::det(void) const {
    
    int ifail, jfail;
    int n = nrow();
    double det;

    memcpy( work, m, size() * sizeof(double) );
    double *work2 = new double[n];

    dfact_(&n, work, &n, work2, &ifail, &det, &jfail);
    if(jfail != 0) {
      det = 0;
#ifdef VtDEBUG
      cout << "VtMatrix::det() error: Determinant computation failed!" << endl;
#endif
    }

    delete[] work2;
   
    return det;
  }


  //============================================================================
  // VtSqMatrix::dinv: return inverse without changing internal states
  //============================================================================
  const VtSqMatrix VtSqMatrix::dinv(int dim) const {
    VtSqMatrix tmp(*this);
    tmp.VtDinv(dim);
    return tmp;
  }

  //============================================================================
  // VtSqMatrix::VtDinv: matrix inversion using DINV_ from CERNLIB
  //============================================================================
  bool VtSqMatrix::VtDinv(int dim) {
    bool valid = false;

    if(dim == 0) dim = nrow();
#ifndef VtFAST
    else {
      assert(dim <= static_cast<int>(nrow()));
    }
#endif

    const size_t msize = size() * sizeof(double);

    memcpy( work, m, msize );

    int    ifail = 0;
    double *work2 = new double[dim];
    dinv_(&dim, work, &dim, work2, &ifail);
    delete[] work2;

    if(ifail != -1) {
      memcpy( m, work, msize );
      valid = true;
    }
#ifdef VtDEBUG
    else {
      cout << "VtMatrix::dinv() error: Matrix inversion failed!!!" << endl;
    }
#endif

    return valid;
  } 

  //============================================================================
  // VtSqMatrix::operator* with VtMatrix objects
  //============================================================================
  const VtMatrix VtSqMatrix::operator*(const VtMatrix& rhs) {
    return VtMatrix::operator*(rhs);
  }

  //============================================================================
  // VtSqMatrix::operator* with VtVector objects
  //============================================================================
  const VtVector VtSqMatrix::operator*(const VtVector& rhs) const {
    return VtMatrix::operator*(rhs);
  }

} // end of namespace MATRIX
