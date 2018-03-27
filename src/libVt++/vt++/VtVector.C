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
// Description: vector class
//
// changes:
// 21 Aug 2000 (TG) creation
// 05 Sep 2000 (TG) mag2() added
// 14 Sep 2000 (TG) place_at() added
// 23 Okt 2000 (TG) added code for copy()
// 27 Dec 2000 (TG) operator+(), operator-() simplified
// 09 Mar 2001 (TG) unit(), operator*(double) added
//
// *****************************************************************************
//#include <math.h>
#include "Riostream.h"
#include <assert.h>
#include <string.h> // for memcpy
#include "vt++/VtVector.hh"

namespace MATRIX {

  //============================================================================
  // Constructors
  //============================================================================
  VtVector::VtVector(const unsigned int rows):
    v_size(rows),
    v(new double[size()]) {
    memset(v, 0, size() * sizeof(double));
  }

  VtVector::VtVector(double v1, double v2):
    v_size(2), v(new double[2]) {
    v[0] = v1;
    v[1] = v2;
  }

  VtVector::VtVector(double v1, double v2, double v3):
    v_size(3), v(new double[3]) {
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
  }

  VtVector::VtVector(double v1, double v2, double v3, double v4):
    v_size(4), v(new double[4]) {
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    v[3] = v4;
  }

  VtVector::VtVector(double v1, double v2, double v3, double v4, double v5):
    v_size(5), v(new double[5]) {
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    v[3] = v4;
    v[4] = v5;
  }

  VtVector::VtVector(double v1, double v2, double v3, double v4,
		     double v5, double v6):
    v_size(6), v(new double[6]) {
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    v[3] = v4;
    v[4] = v5;
    v[5] = v6;
  }

  VtVector::VtVector(const VtVector& rhs):
    v_size(rhs.size()),
    v(new double[size()]) {
    memcpy(v, rhs.v, size() * sizeof(double));
  }

  VtVector::~VtVector() {
    //    pthread_mutex_lock(&VTmutex);
    delete[] v;
    //    pthread_mutex_unlock(&VTmutex);
  }
  

  //============================================================================
  // squared length of vector
  //============================================================================
  double VtVector::mag2() const {
    double sum=0;
    for(unsigned int i=0; i<size(); ++i)
      sum += v[i] * v[i];
    return sum;
  }

  //============================================================================
  // operator=
  //============================================================================
  const VtVector& VtVector::operator=(const VtVector& rhs) {
    if(this == &rhs) return *this;

#ifndef VtFAST
    assert(size() == rhs.size());
#endif

    v_size = rhs.size();

    memcpy(v, rhs.v, size() * sizeof(double)); 

    return *this;
  }

  //============================================================================
  // operator+= for Scalars
  //============================================================================
  const VtVector& VtVector::operator+=(const double rhs) {
    for(unsigned int i=0; i<size(); ++i) v[i] += rhs;
    return *this;
  }

  //============================================================================
  // operator+= for Vectors
  //============================================================================
  const VtVector& VtVector::operator+=(const VtVector& rhs) {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif
    
    for(unsigned int i=0; i<size(); ++i) v[i] += rhs.v[i];
    return *this;
  }

  //============================================================================
  // operator-= for Scalars
  //============================================================================
  const VtVector& VtVector::operator-=(const double rhs) {
    for(unsigned int i=0; i<size(); ++i) v[i] -= rhs;
    return *this;
  }

  //============================================================================
  // operator-= for Vectors
  //============================================================================
  const VtVector& VtVector::operator-=(const VtVector& rhs) {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif

    for(unsigned int i=0; i<size(); ++i) v[i] -= rhs.v[i];
    return *this;
  }

  //============================================================================
  // operator*=
  //============================================================================
  const VtVector& VtVector::operator*=(const double rhs) {
    for(unsigned int i=0; i<size(); ++i) v[i] *= rhs;
    return *this;
  }

  //============================================================================
  // operator/=
  //============================================================================
  const VtVector& VtVector::operator/=(const double rhs) {
    for(unsigned int i=0; i<size(); ++i) v[i] /= rhs;
    return *this;
  }

  //============================================================================
  // operator+
  //============================================================================
  const VtVector VtVector::operator+(const VtVector& rhs) const {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif
    
//      VtVector tmp(size());
    
//      for(unsigned int i=0; i<size(); ++i) {
//        tmp[i] = *(v+i) + rhs[i];
//      }

    VtVector tmp(*this);
    return tmp += rhs;
  }

  //============================================================================
  // operator-
  //============================================================================
  const VtVector VtVector::operator-(const VtVector& rhs) const {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif
    
//      VtVector tmp(size());

//      for(unsigned int i=0; i<size(); ++i) {
//        tmp[i] = *(v+i) - rhs[i];
//      }

    VtVector tmp(*this);
    return tmp -= rhs;
  }

  //============================================================================
  // operator*  Scalar product between Vectors
  //============================================================================
  const double VtVector::operator*(const VtVector& rhs) const {

    assert(size() == rhs.size());
    
    double sum = 0.;
    for(unsigned int i=0; i<size(); ++i) {
      sum += *(v+i) * rhs[i];
    }

    return sum;
  }

  //============================================================================
  // operator* Multiplication with scalar
  //============================================================================
  VtVector VtVector::operator*(const double rhs) const {
    VtVector tmp(*this);
    return tmp *= rhs;
  }

  //============================================================================
  // VtVector::place_at()  place a smaller Vector in a bigger one
  //============================================================================
  void VtVector::place_at(const VtVector& rhs,
			  const unsigned int row) {
#ifndef VtFAST
    assert(nrow() >= rhs.nrow()+row);
#endif

    for(unsigned int i=0; i<rhs.nrow(); ++i) {
      operator()(i+row) = rhs(i);
    }

    return;
  }

  //============================================================================
  // VtVector::print
  //============================================================================
  void VtVector::print(std::ostream& os) const {
    /* Fixed format needs 3 extra characters for field, while scientific needs 7 */
    int width;
    os << std::endl;
    if(os.flags() & std::ios::fixed)
      width = os.precision()+3;
    else
      width = os.precision()+7;
    for(unsigned int row = 0; row < nrow(); ++row) {
      os.width(width);
      os << operator()(row) << std::endl;
    }
    return;
  }

  //============================================================================
  // VtVector::clear
  //============================================================================
  void VtVector::clear() {
    memset(v, 0, v_size * sizeof(double));
    return;
  }

  //============================================================================
  // VtMatrix::copy in case dimensions differ
  //============================================================================
  void VtVector::copy(const VtVector& rhs, const unsigned int offset) {

#ifndef VtFAST
    assert(offset <= rhs.nrow());
#endif

    const unsigned int rows = rhs.nrow() - offset;
    const unsigned int nrow = (v_size < rows) ? v_size : rows;

    memcpy(v, rhs.v+offset, nrow * sizeof(double));

    return;
  }

  //============================================================================
  // VtVector::unit return a unit vector
  //============================================================================
  VtVector VtVector::unit() const {
    VtVector tmp(*this);
    tmp /= tmp.mag();
    return tmp;
  }

} // namespace MATRIX
