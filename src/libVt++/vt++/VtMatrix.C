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
// Description: Matrix base class
//
// changes:
// 21 Aug 2000 (TG) creation
// 10 Okt 2000 (TG) added code for copy() member function
// 11 Okt 2000 (TG) added code for clear() member function
// 23 Okt 2000 (TG) code for place_at(VtVector,...) added
//
// *****************************************************************************
#include "Riostream.h"
#include <assert.h>
#include <string.h> // for memcpy
#include "vt++/VtMatrix.hh"
#include "vt++/VtNegMatrix.hh"
#include "vt++/VtVector.hh"

namespace MATRIX {

  //============================================================================
  // VtMatrix Constructors
  //============================================================================
  VtMatrix::VtMatrix(const unsigned int row, const unsigned int col):
    m_nrow(row),
    m_ncol(col),
    m_size(row*col) {
    m    = new double[m_size];
    work = new double[m_size];
    memset(m, 0, m_size * sizeof(double));
  }

  VtMatrix::VtMatrix(const VtMatrix& rhs):
    m_nrow(rhs.nrow()),
    m_ncol(rhs.ncol()),
    m_size(rhs.size()) {
    m    = new double[m_size];
    work = new double[m_size];
    memcpy(m, rhs.m, m_size * sizeof(double));
  }

  VtMatrix::~VtMatrix() {
    delete[] m;
    delete[] work;
  }
  

  //============================================================================
  // VtMatrix::place_at()  place a smaller Matrix in a bigger one
  //============================================================================
  void VtMatrix::place_at(const VtMatrix& rhs,
			  const unsigned int row,
			  const unsigned int col) {
#ifndef VtFAST
    assert(nrow() >= rhs.nrow()+row && ncol() >= rhs.ncol()+col);
#endif

    const unsigned int rnrow = rhs.nrow();
    const unsigned int rncol = rhs.ncol();

    for(unsigned int i=0; i<rnrow; ++i) {
      const unsigned int nr = i + row;
      for(unsigned int j=0, nc=col; j<rncol; ++j,++nc) {
	operator()(nr,nc) = rhs(i,j);
      }
    }

    return;
  }

  //============================================================================
  // VtMatrix::place_at()  place a smaller Matrix in a bigger one
  //============================================================================
  void VtMatrix::place_at(const VtVector& rhs,
			  const unsigned int row,
			  const unsigned int col) {

#ifndef VtFAST
    assert(nrow() >= rhs.nrow()+row && ncol() >= col+1);
#endif

    const unsigned int rnrow = rhs.nrow();

    for(unsigned int i=0; i<rnrow; ++i) {
      operator()(i+row,col) = rhs(i);
    }

    return;
  }


  //============================================================================
  // VtMatrix::operator()
  //============================================================================
  double  VtMatrix::operator()(unsigned int row, unsigned int col) const {
    return *(m + row*m_ncol + col);
  }

  double& VtMatrix::operator()(const unsigned int row, const unsigned int col) {
    return *(m + row*m_ncol + col);
  }
    
  //============================================================================
  // VtMatrix::operator=
  //============================================================================
  const VtMatrix& VtMatrix::operator=(const VtMatrix& rhs) {
    if(this == &rhs) return *this;

#ifndef VtFAST
    assert(size() == rhs.size());
#endif

    m_nrow = rhs.nrow();
    m_ncol = rhs.ncol();
    m_size = rhs.size();
    
    memcpy(m, rhs.m, m_size * sizeof(double)); 
    
    return *this;
  }
  

  //============================================================================
  // VtMatrix::operator=
  //============================================================================
  const VtMatrix& VtMatrix::operator=(const VtNegMatrix& rhs) {
    
#ifndef VtFAST
    assert(size() == rhs.size());
#endif
    
    m_nrow = rhs.nrow();
    m_ncol = rhs.ncol();
    m_size = rhs.size();
    
    for(int i=0; i<m_size; ++i)  m[i] = -rhs.m[i];
    
    return *this;
  }

  //============================================================================
  // VtMatrix::operator+= for Scalars
  //============================================================================
  const VtMatrix& VtMatrix::operator+=(const double rhs) {
    for(int i=0; i<m_size; ++i) m[i] += rhs;
    return *this;
  }

  //============================================================================
  // VtMatrix::operator+= for Matrices
  //============================================================================
  const VtMatrix& VtMatrix::operator+=(const VtMatrix& rhs) {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif

    for(int i=0; i<m_size; ++i) m[i] += rhs.m[i];
    return *this;
  }

  //============================================================================
  // VtMatrix::operator-= for Scalars
  //============================================================================
  const VtMatrix& VtMatrix::operator-=(const double rhs) {
    for(int i=0; i<m_size; ++i) m[i] -= rhs;
    return *this;
  }

  //============================================================================
  // VtMatrix::operator-= for Matrices
  //============================================================================
  const VtMatrix& VtMatrix::operator-=(const VtMatrix& rhs) {

#ifndef VtFAST
    assert(size() == rhs.size());
#endif

    for(int i=0; i<m_size; ++i) m[i] -= rhs.m[i];
    return *this;
  }

  //============================================================================
  // VtMatrix::operator*=
  //============================================================================
  const VtMatrix& VtMatrix::operator*=(const double rhs) {
    for(int i=0; i<m_size; ++i) m[i] *= rhs;
    return *this;
  }

  //============================================================================
  // operator/=
  //============================================================================
  const VtMatrix& VtMatrix::operator/=(const double rhs) {
    for(int i=0; i<m_size; ++i) m[i] /= rhs;
    return *this;
  }

  //============================================================================
  // VtMatrix::operator+
  //============================================================================
  const VtMatrix VtMatrix::operator+(const VtMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif
    
    VtMatrix tmp(*this);

    return tmp += rhs;
  }

  //============================================================================
  // VtNegMatrix::operator+
  //============================================================================
  const VtMatrix VtMatrix::operator+(const VtNegMatrix& rhs) const {
    
#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif

    VtMatrix tmp(*this);
    return tmp -= rhs;
  }

  //============================================================================
  // VtMatrix::operator- (binary)
  //============================================================================
  const VtMatrix VtMatrix::operator-(const VtMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif
    
    VtMatrix tmp(*this);

    return tmp -= rhs;
  }

  //============================================================================
  // VtMatrix::operator- (binary)
  //============================================================================
  const VtMatrix VtMatrix::operator-(const VtNegMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol() && nrow() == rhs.nrow());
#endif
    
    VtMatrix tmp(*this);

    return tmp += rhs;
  }

  //============================================================================
  // VtMatrix::operator- (unary)
  //============================================================================
  const VtNegMatrix VtMatrix::operator-() const {

#ifdef VtDEBUG
    cout << "VtMatrix::operator- (unary) called!" << endl;
#endif

    return VtNegMatrix(*this);
  }

  //============================================================================
  // VtMatrix::operator* for Matrices 
  //============================================================================
  const VtMatrix VtMatrix::operator*(const VtMatrix& rhs) const {
    
#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    const unsigned int rncol = rhs.ncol();

    VtMatrix tmp(m_nrow, rncol);
    
    for(unsigned int i=0; i<m_nrow; ++i) {
      for(unsigned int j=0; j<rncol; ++j) {
	for(unsigned int k=0; k<m_ncol; ++k) {
	  //	  tmp(i,j) += operator()(i,k) * rhs(k,j);
	  tmp.get(i,j) += get(i,k) * rhs.get(k,j);
	}
      }
    }

    return tmp;
  }

  //============================================================================
  // VtMatrix::operator* for Vectors
  //============================================================================
  const VtVector VtMatrix::operator*(const VtVector& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    VtVector tmp(m_nrow);
    
    for(unsigned int i=0; i<m_nrow; ++i) {
      for(unsigned int j=0; j<m_ncol; ++j) {
	//	tmp[i] += operator()(i,j) * rhs[j];
	tmp[i] += get(i,j) * rhs[j];
      }
    }

    return tmp;
  }


  //============================================================================
  // VtMatrix::VtT: transpose Matrix
  //============================================================================
  void VtMatrix::VtT(void) {

    memcpy( work, m, size() * sizeof(double) );
    for(unsigned int i=0; i<m_nrow; ++i) {
      for(unsigned int j=0; j<m_ncol; ++j) {
	m[j*m_nrow+i] = work[i*m_ncol+j];
      }
    }
    const int tmp = m_nrow;
    m_nrow = m_ncol;
    m_ncol = tmp;

    return;
  }


  //============================================================================
  // VtMatrix::T: return transpose Matrix without changing internal state
  //============================================================================
  const VtMatrix VtMatrix::T(void) const {
    VtMatrix tmp(*this);
    tmp.VtT();
    return tmp;
  }

  //============================================================================
  // VtMatrix::print
  //============================================================================
  void VtMatrix::print(std::ostream& os) const {
    /* Fixed format needs 3 extra characters for field, while scientific needs 7 */
    int width;
    os << std::endl;
    if(os.flags() & std::ios::fixed)
      width = os.precision()+3;
    else
      width = os.precision()+7;
    for(unsigned int row = 0; row < m_nrow; ++row) {
      for(unsigned int col = 0; col < m_ncol; ++col) {
	os.width(width);
	os << operator()(row,col) << " ";
      }
      os << std::endl;
    }
    return;
  }

  //============================================================================
  // VtMatrix::copy in case dimensions differ, start at (0,0)
  //============================================================================
  void VtMatrix::copy(const VtMatrix& rhs) {

    const unsigned int nrow = (m_nrow < rhs.nrow()) ? m_nrow : rhs.nrow();
    const unsigned int ncol = (m_ncol < rhs.ncol()) ? m_ncol : rhs.ncol();

    for(unsigned int i = 0; i < nrow; ++i)
      for(unsigned int j = 0; j < ncol; ++j)
	operator()(i,j) = rhs(i,j);

    return;
  }

  //============================================================================
  // VtMatrix::clear 
  //============================================================================
  void VtMatrix::clear(void) {
    memset(m, 0, m_size * sizeof(double));
    return;
  }
} // namespace MATRIX
