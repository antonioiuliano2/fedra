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
// Description: class for symmetric & pos. def. matrices
//
// changes:
// 21 Aug 2000 (TG) creation
// 04 Sep 2000 (TG) added assert() to VtDsinv(), modified return type of dsinv()
// 05 Sep 2000 (TG) include <string.h> added
// 10 Okt 2000 (TG) modified dsinv(dim) member function: it returns now
//                  a VtSymMatrix of dimension dim x dim .
// 10 Okt 2000 (TG) added code for copy() member function.
// 17 Okt 2000 (TG) code for operators +=,-=,*=,/= (on skalars) added, code speedup
// 17 Okt 2000 (TG) copy constructor code added
// 24 Okt 2000 (TG) code for operator-=, operator+= (on matrices) added
// 27 Okt 2000 (TG) added code for print()
// 27 Dec 2000 (TG) simplified operator+(), operator-()
// 02 Jan 2001 (TG) bug fix in operator-()
// 05 Feb 2001 (TG) renamed Assert() to VtAssert()
//
// *****************************************************************************

#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string.h> // for memcpy
#include "vt++/VtSymMatrix.hh"
#include "vt++/VtVector.hh"

void Dsfact1(int *idim, double *a, int *n, int *ifail, double *det, int *jfail)
{
  /* Local variables */
  static unsigned int i, j, l;
  *ifail = 0;
  *jfail = 0;
  
  /* Function Body */
  if (*idim < *n || *n <= 0) {
    *ifail= -1;
    return;
  }


  /* Parameter adjustments */
  a -= *idim + 1;

  /* sfactd.inc */
  *det = 1.;
  for (j = 1; j <= (unsigned int)(*n); ++j) {
    const unsigned int ji = j * (*idim);
    const unsigned int jj = j + ji;

    if (a[jj] <= 0.) {
      *det = 0.;
      *ifail = -1;
      return;
    }

    const unsigned int jp1 = j + 1;
    const unsigned int jpi = jp1 * (*idim);

    (*det) *= a[jj];
    a[jj] = 1. / a[jj];

    for (l = jp1; l <= (unsigned int)(*n); ++l) {
      a[j + l * (*idim)] = a[jj] * a[l + ji];

      const unsigned int lj = l + jpi;

      for (i = 1; i <= j; ++i) {
	a[lj] -= a[l + i * (*idim)] * a[i + jpi];
      } // for i
    } // for l
  } // for j

  return;
} // end of Dsfact1

void Dsinv1(int *idim, double *a, int *n, int *ifail) {

  /* Local variables */
  static int i, j, k, l;
  static double s31, s32;
  static int jm1, jp1;
  *ifail = 0;
 
  /* Parameter adjustments */
  a -= *idim + 1;

  /* Function Body */
  if (*idim < *n || *n <= 1) {
    *ifail = -1;
    return;
  }

  /* sfact.inc */
  for (j = 1; j <= *n; ++j) {
    const int ja  = j * (*idim);
    const int jj  = j + ja;
    const int ja1 = ja + (*idim);

    if (a[jj] <= 0.) { *ifail = -1; return; }
    a[jj] = 1. / a[jj];
    if (j == *n) { break; }

    for (l = j + 1; l <= *n; ++l) {
      a[j + l * (*idim)] = a[jj] * a[l + ja];
      const int lj = l + ja1;
      for (i = 1; i <= j; ++i) {
	a[lj] -= a[l + i * (*idim)] * a[i + ja1];
      }
    }
  }

  /* sfinv.inc */
  // idim << 1 is equal to idim * 2
  // compiler will compute the arguments!
  a[(*idim << 1) + 1] = -a[(*idim << 1) + 1];
  a[*idim + 2] = a[(*idim << 1) + 1] * a[(*idim << 1) + 2];

  if(*n > 2) {

    for (j = 3; j <= *n; ++j) {
      const int jm2 = j - 2;
      const int ja = j * (*idim);
      const int jj = j + ja;
      const int j1 = j - 1 + ja;

      for (k = 1; k <= jm2; ++k) {
	s31 = a[k + ja];

	for (i = k; i <= jm2; ++i) {
	  s31 += a[k + (i + 1) * (*idim)] * a[i + 1 + ja];
	} // for i
	a[k + ja] = -s31;
	a[j + k * (*idim)] = -s31 * a[jj];
      } // for k
      a[j1] *= -1;
      //      a[j1] = -a[j1];
      a[jj - (*idim)] = a[j1] * a[jj];
    } // for j
  } // if (*n>2)

  j = 1;
  do {
    const int jad = j * (*idim);
    const int jj = j + jad;

    jp1 = j + 1;
    for (i = jp1; i <= *n; ++i) {
      a[jj] += a[j + i * (*idim)] * a[i + jad];
    } // for i

    jm1 = j;
    j = jp1;
    const int ja = j * (*idim);

    for (k = 1; k <= jm1; ++k) {
      s32 = 0.;
      for (i = j; i <= *n; ++i) {
	s32 += a[k + i * (*idim)] * a[i + ja];
      } // for i
      a[k + ja] = a[j + k * (*idim)] = s32;
    } // for k
  } while(j < *n);

  return;
} // end of Dsinv1

using namespace std;

namespace MATRIX {
  //============================================================================
  // VtSymMatrix::Constructor which sets diagonal elements
  //============================================================================
  VtSymMatrix::VtSymMatrix(const unsigned int dim, double diag): VtSqMatrix(dim) {
    for(unsigned int i=0; i<nrow(); ++i)
      operator()(i,i) = diag;
  }

  //============================================================================
  // VtSymMatrix::Copy constructor
  //============================================================================
  VtSymMatrix::VtSymMatrix(const VtSymMatrix& rhs): VtSqMatrix(rhs.nrow()) {
    const unsigned int nrow = rhs.nrow();

    for(unsigned int i=0; i<nrow; ++i) {
      //      operator()(i,i) = rhs(i,i);
      get(i,i) = rhs.get(i,i);
      for(unsigned int j=i+1; j<nrow; ++j) {
	//	operator()(i,j) = operator()(j,i) = rhs(i,j);
	get(i,j) = get(j,i) = rhs.get(i,j);
      }
    }
  }

  VtSymMatrix::~VtSymMatrix() {}

  //============================================================================
  // VtSymMatrix::operator=
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator=(const VtSymMatrix& rhs) {
    if(this == &rhs) return *this;

#ifndef VtFAST
    assert(ncol() == rhs.ncol());
#endif

#ifdef VtDEBUG
    cout << "use VtSymMatrix::operator=" << endl;
#endif
    const unsigned int nrow = ncol();

    for(unsigned int i=0; i<nrow; ++i) {
      //      operator()(i,i) = rhs(i,i);
      get(i,i) = rhs.get(i,i);
      for(unsigned int j=i+1; j<nrow; ++j) {
	//	operator()(i,j) = operator()(j,i) = rhs(i,j);
	get(i,j) = get(j,i) = rhs.get(i,j);
      }
    }

    return *this;
  }

  //============================================================================
  // VtSymMatrix::Assert  assure matrix is symmetric (only needed in development)
  //============================================================================
  void VtSymMatrix::VtAssert(void) {
    for(unsigned int i=0; i<nrow(); ++i)
      for(unsigned int j=i+1; j<ncol(); ++j) {
	cout.setf(ios::scientific,ios::floatfield);
	double ij = operator()(i,j);
	double ji = operator()(j,i);
	double dd = 0.;
	if(fabs(ij) < 1e-20) 
	  dd = ij - ji;
	else
	  dd = (ij - ji)/ij;
//  	  cout << "i: " << i << " j: " << j 
//  	       << "(i,j): " << ij << " "
//  	       << "(j,i): " << ji << " "
//  	       << "diff: "  << fabs(dd) << endl;
	assert( dd < 1.e-6 );
      }
    return;
  }

  //============================================================================
  // VtSymMatrix::invert
  //============================================================================
  bool VtSymMatrix::invert(const bool use_momentum) {
    if(use_momentum)
      return VtDsinv(nrow());
    else
      return VtDsinv(nrow() - 1);
  }
  
  //============================================================================
  // VtSqMatrix::dsinv: return inverse without changing internal states
  //============================================================================
  const VtSymMatrix VtSymMatrix::dsinv(int dim) const {
    if(dim==0) {
      VtSymMatrix tmp(*this);
      tmp.VtDsinv(dim);
      return tmp;
    } else {
      VtSymMatrix tmp(dim);
      tmp.copy(*this);
      tmp.VtDsinv();
      return tmp;
    }
    //    return VtSymMatrix(*this).VtDsinv(dim);
  }

  //============================================================================
  // VtSqMatrix::VtDsinv: matrix inversion using dsinv_ from CERNLIB
  //============================================================================
  bool VtSymMatrix::VtDsinv(int dim) {
    bool valid = false;

    if(dim == 0) dim = nrow();
#ifndef VtFAST
    else {
      assert(dim <= static_cast<int>(nrow()));
    }
#endif

    const size_t msize = size() * sizeof(double);

    memcpy( work, m, msize );

    int ifail = 0;
    int isize = nrow();
    Dsinv1(&dim, work, &isize, &ifail);
    
    if(ifail != -1) {
      memcpy( m, work, msize );
      valid = true;
    }
#ifdef VtDEBUG
    else {
      cout << "VtSymMatrix::dsinv() error: Matrix inversion failed!!!"
	   << " (nrow=" << nrow() << ",ncol=" << ncol() 
	   << ",size=" << size()
	   << ",ifail=" << ifail << ")" << endl;
    } 
#endif

    return valid;
  }

  //============================================================================
  // VtSqMatrix::det: calc determinant with CERNLIB dsfact_ routine
  //============================================================================
  double VtSymMatrix::det(void) const {
    
    int ifail, jfail;
    int n = nrow();
    double det;

    memcpy( work, m, size() * sizeof(double) );

    Dsfact1(&n, work, &n, &ifail, &det, &jfail);
    if(jfail != 0) {
      det = 0;
#ifdef VtDEBUG
      cout << "VtSymMatrix::det() error: Determinant computation failed!" << endl;
#endif
    }

    return det;
  }

  //============================================================================
  // VtSymMatrix::product  compute scalar v^T * C * v
  //============================================================================
  double VtSymMatrix::product(const VtVector& rhs, unsigned int dim) const {

#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    if(dim == 0) dim = ncol();
#ifndef VtFAST
    else
      assert(dim <= ncol());
#endif

    double result = 0.;

    // contribution from diagonal elements
    for(unsigned int i=0; i<dim; ++i)
      result += get(i,i) * rhs[i]*rhs[i];

    // make use of fact that Matrix is symmetric
    for(unsigned int i=0; i<(dim-1); ++i) {
      double sum = 0.;
      for(unsigned int j=i+1; j<dim; ++j) {
	sum += get(i,j) * rhs[j];
      }
      result += 2. * rhs[i] * sum;
    }

    return result;
  }

  //============================================================================
  // VtSymMatrix::product  compute Matrix A^T * C * A
  //============================================================================
  const VtSymMatrix VtSymMatrix::product(const VtMatrix& rhs) const {
    
#ifndef VtFAST
    assert(ncol() == rhs.nrow());
#endif

    const unsigned int rncol = rhs.ncol();

    VtSymMatrix tmp(rncol);
    
    for(unsigned int i=0; i<rncol; ++i) {
      for(unsigned int j=i; j<rncol; ++j) {
	for(unsigned int k=0; k<m_ncol; ++k) {
	  double sum = 0;
	  for(unsigned int l=0; l<m_ncol; ++l)
	    sum += get(k,l) * rhs(l,j);
	  tmp(i,j) += rhs(k,i) * sum;
	}
	tmp(j,i) = tmp(i,j);
      }
    }

    return tmp;
  }

  //============================================================================
  // VtMatrix::place_at()  place a smaller Matrix in a bigger one
  //============================================================================
  void VtSymMatrix::place_at(const VtMatrix& rhs,
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
	get(nr,nc) = get(nc,nr) = rhs(i,j);
      }
    }

    return;
  }

  //============================================================================
  // VtSymMatrix::operator+ for symmetric Matrices 
  //============================================================================
  const VtSqMatrix VtSymMatrix::operator+(const VtSqMatrix& rhs) const {

    return VtMatrix::operator+(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator+ for Matrices 
  //============================================================================
  const VtMatrix VtSymMatrix::operator+(const VtMatrix& rhs) const {
    
    return VtMatrix::operator+(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator- (unary)
  //============================================================================
  const VtSymMatrix VtSymMatrix::operator-() const {

#ifdef VtDEBUG
    cout << "VtSymMatrix::operator- (unary) called!" << endl;
#endif

    const unsigned int nrow = ncol();
    VtSymMatrix tmp(nrow);

    for(unsigned int i=0; i<nrow; ++i) {
      for(unsigned int j=i; j<nrow; ++j) {
	tmp(i,j) = tmp(j,i) = -get(i,j);
      }
    }
    
    return tmp;
  }

  //============================================================================
  // VtSyMatrix::operator+= for Scalars
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator+=(const double rhs) {
    const unsigned int nrow = ncol();

    for(unsigned int i=0; i<nrow; ++i){
      for(unsigned int j=i; j<nrow; ++j) {
	operator()(i,j) = operator()(j,i) += rhs;
      }
    }
    return *this;
  }

  //============================================================================
  // VtMatrix::operator+= for Matrices
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator+=(const VtSymMatrix& rhs) {

#ifndef VtFAST
    assert(ncol() == rhs.ncol());
#endif
    
    const unsigned int nrow = rhs.nrow();

    for(unsigned int i=0; i<nrow; ++i)
      for(unsigned int j=i; j<nrow; ++j)
	operator()(i,j) = operator()(j,i) += rhs(i,j);

    return *this;
  }


  //============================================================================
  // VtSymMatrix::operator+
  //============================================================================
  const VtSymMatrix VtSymMatrix::operator+(const VtSymMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol());
#endif

#ifdef VtDEBUG
    cout << "VtSymMatrix::operator+ called!" << endl;
#endif

    VtSymMatrix tmp(*this);
    return tmp += rhs;
  }

  //============================================================================
  // VtSymMatrix::operator-= for Scalars
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator-=(const double rhs) {
    const unsigned int nrow = ncol();

    for(unsigned int i=0; i<nrow; ++i){
      for(unsigned int j=i; j<nrow; ++j) {
	operator()(i,j) = operator()(j,i) -= rhs;
      }
    }
    return *this;
  }

  //============================================================================
  // VtMatrix::operator-= for Matrices
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator-=(const VtSymMatrix& rhs) {

#ifndef VtFAST
    assert(ncol() == rhs.ncol());
#endif
    
    const unsigned int nrow = rhs.nrow();

    for(unsigned int i=0; i<nrow; ++i)
      for(unsigned int j=i; j<nrow; ++j)
	operator()(i,j) = operator()(j,i) -= rhs(i,j);

    return *this;
  }

  //============================================================================
  // VtSymMatrix::operator- (binary)
  //============================================================================
  const VtSymMatrix VtSymMatrix::operator-(const VtSymMatrix& rhs) const {

#ifndef VtFAST
    assert(ncol() == rhs.ncol());
#endif
    
#ifdef VtDEBUG
    cout << "VtSymMatrix::operator- called!" << endl;
#endif

    VtSymMatrix tmp(*this);
    return tmp -= rhs;
  }

  //============================================================================
  // VtSymMatrix::operator*=
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator*=(const double rhs) {
    const unsigned int nrow = ncol();

    for(unsigned int i=0; i<nrow; ++i){
      for(unsigned int j=i; j<nrow; ++j) {
	operator()(i,j) = operator()(j,i) *= rhs;
      }
    }
    return *this;
  }

  //============================================================================
  // VtSymMatrix::operator/=
  //============================================================================
  const VtSymMatrix& VtSymMatrix::operator/=(const double rhs) {
    const unsigned int nrow = ncol();

    for(unsigned int i=0; i<nrow; ++i){
      for(unsigned int j=i; j<nrow; ++j) {
	operator()(i,j) = operator()(j,i) /= rhs;
      }
    }
    return *this;
  }

  //============================================================================
  // VtSymMatrix::operator- for symmetric Matrices 
  //============================================================================
  const VtSqMatrix VtSymMatrix::operator-(const VtSqMatrix& rhs) const {

    return VtMatrix::operator-(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator- for Matrices 
  //============================================================================
  const VtMatrix VtSymMatrix::operator-(const VtMatrix& rhs) const {
    
    return VtMatrix::operator-(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator* for cov. Matrices
  //============================================================================
  // Product of 2 symmetric matrices is not neccessarily symmetric!!
  const VtSqMatrix VtSymMatrix::operator*(const VtSymMatrix& rhs) const {

    return VtMatrix::operator*(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator* for Matrices 
  //============================================================================
  const VtSqMatrix VtSymMatrix::operator*(const VtSqMatrix& rhs) const {

    return VtMatrix::operator*(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator* for Matrices 
  //============================================================================
  const VtMatrix VtSymMatrix::operator*(const VtMatrix& rhs) const {
    
    return VtMatrix::operator*(rhs);
  }

  //============================================================================
  // VtSymMatrix::operator* for Vectors
  //============================================================================
  const VtVector VtSymMatrix::operator*(const VtVector& rhs) const {

    return VtMatrix::operator*(rhs);
  }


  //============================================================================
  // VtSymMatrix::copy in case dimensions differ
  //============================================================================
  void VtSymMatrix::copy(const VtSymMatrix& rhs) {

    const unsigned int nrow = (m_nrow < rhs.nrow()) ? m_nrow : rhs.nrow();

    for(unsigned int i=0; i<nrow; ++i) {
      operator()(i,i) = rhs(i,i);
      for(unsigned int j=i+1; j<nrow; ++j) {
	operator()(i,j) = operator()(j,i) = rhs(i,j);
      }
    }

    return;
  }

  //============================================================================
  // VtSymMatrix::print
  //============================================================================
  void VtSymMatrix::print(std::ostream& os) const {
    os.setf(ios::scientific,ios::floatfield);
    VtMatrix::print(os);
  }
} // end of namespace MATRIX
