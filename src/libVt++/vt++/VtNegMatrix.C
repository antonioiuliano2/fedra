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
// Description: A class for negative matrices (to be used internally only)
//
// changes:
// 21 Aug 2000 (TG) creation
//
// *****************************************************************************
#include <iostream>
#include <assert.h>
#include <string.h> // for memcpy
#include "vt++/VtNegMatrix.hh"
#include "vt++/VtVector.hh"

namespace MATRIX {

  //============================================================================
  // VtNegMatrix Constructors
  //============================================================================
  VtNegMatrix::VtNegMatrix(const unsigned int row, const unsigned int col):
    VtMatrix(row,col) {}

  VtNegMatrix::VtNegMatrix(const VtNegMatrix& rhs):
    VtMatrix(rhs) {}

  VtNegMatrix::VtNegMatrix(const VtMatrix& rhs):
    VtMatrix(rhs) {}

  double VtNegMatrix::operator()(unsigned int row, unsigned int col) const {
    //    cout << "VtNegMatrix::operator() const called!" << endl;
    return -*(m + row*m_ncol + col);
  }

  double& VtNegMatrix::operator()(const unsigned int row, const unsigned int col) {
    //    cout << "VtNegMatrix::operator() const called!" << endl;
    return *(m + row*m_ncol + col);
  }

  //============================================================================
  // VtNegMatrix::operator+= for Scalars
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator+=(const double rhs) {
    VtMatrix::operator-=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator+= for Matrices
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator+=(const VtMatrix& rhs) {
    cout << "VtNegMatrix::operator+=(VtMatrix) called" << endl;
    VtMatrix::operator-=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator+= for Matrices
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator+=(const VtNegMatrix& rhs) {
    cout << "VtNegMatrix::operator+=(VtNegMatrix) called" << endl;
    VtMatrix::operator+=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator-= for Scalars
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator-=(const double rhs) {
    VtMatrix::operator+=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator-= for Matrices
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator-=(const VtMatrix& rhs) {
    cout << "VtNegMatrix::operator-=(VtMatrix) called" << endl;
    VtMatrix::operator+=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator-= for Matrices
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator-=(const VtNegMatrix& rhs) {
    cout << "VtNegMatrix::operator-=(VtNegMatrix) called" << endl;
    VtMatrix::operator-=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator*=
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator*=(const double rhs) {
    VtMatrix::operator*=(rhs);
    return *this;
  }

  //============================================================================
  // operator/=
  //============================================================================
  const VtNegMatrix& VtNegMatrix::operator/=(const double rhs) {
    VtMatrix::operator/=(rhs);
    return *this;
  }

  //============================================================================
  // VtNegMatrix::operator+
  //============================================================================
  const VtMatrix VtNegMatrix::operator+(const VtMatrix& rhs) const {

    return VtMatrix(rhs - *this);
  }

  //============================================================================
  // VtNegMatrix::operator+
  //============================================================================
  const VtMatrix VtNegMatrix::operator+(const VtNegMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif

    VtMatrix tmp(rhs.nrow(),rhs.ncol());
    
    for(unsigned int i=0; i<nrow(); ++i)
      for(unsigned int j=0; j<ncol(); ++j)
  	tmp(i,j) = operator()(i,j) + rhs(i,j);
    
    return tmp;
  }

  //============================================================================
  // VtNegMatrix::operator- (binary)
  //============================================================================
  const VtMatrix VtNegMatrix::operator-(const VtMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif
    
    VtMatrix tmp(rhs.nrow(),rhs.ncol());

    for(unsigned int i=0; i<nrow(); ++i)
      for(unsigned int j=0; j<ncol(); ++j)
	tmp(i,j) = operator()(i,j) - rhs(i,j);

    return tmp;
  }


  //============================================================================
  // VtNegMatrix::operator- (unary)
  //============================================================================
  const VtMatrix VtNegMatrix::operator-() const {

#ifndef VtDEBUG
    cout << "VtNegMatrix::operator- (unary) called!" << endl;
#endif

    return VtMatrix(*this);
  }
  
  //============================================================================
  // VtNegMatrix::operator* for Matrices 
  //============================================================================
  const VtMatrix VtNegMatrix::operator*(const VtMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    const unsigned int lnrow = nrow();
    const unsigned int rncol = rhs.ncol();
    const unsigned int rnrow = rhs.nrow();

    VtMatrix tmp(lnrow, rncol);

    // the trick is that operator()(i,k) returns the
    // negative!
    for(unsigned int i=0; i<lnrow; ++i) {
      for(unsigned int j=0; j<rncol; ++j) {
	for(unsigned int k=0; k<rnrow; ++k) {
	  // tmp(i,j) += operator()(i,k) * rhs(k,j);
	  tmp.get(i,j) += nget(i,k) * rhs.get(k,j);
	}
      }
    }

    return tmp;
  }

  //============================================================================
  // VtMatrix::operator* for Vectors
  //============================================================================
  const VtVector VtNegMatrix::operator*(const VtVector& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    VtVector tmp(nrow());
    
    for(unsigned int i=0; i<nrow(); ++i) {
      for(unsigned int j=0; j<ncol(); ++j) {
	tmp[i] += operator()(i,j) * rhs[j];
      }
    }

    return tmp;
  }

  //============================================================================
  // VtNegMatrix::T: return transpose Matrix without changing internal state
  //============================================================================
  const VtNegMatrix VtNegMatrix::T(void) const {
    VtNegMatrix tmp(*this);
    tmp.VtT();
    return tmp;
  }


} // namespace MATRIX
