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


void Dfactir1(int *n, double *a, int *idim, int *ir, int *ifail, double *det, int *jfail)
{
  /* Local variables */
  static int nxch, i, j, k, l;
  static double p, q, tf;

  *ifail = 0;
  *jfail = 0;

  if (*idim < *n || *n <= 0) {
    *ifail = -1;
    return;
  }

  
  /* Parameter adjustments */
  a -= *idim + 1;
  --ir;

  /* Function Body */
  
  // fact.inc
  nxch = 0;
  *det = 1.;
  for (j = 1; j <= *n; ++j) {
    const int ji = j * (*idim);
    const int jj = j + ji;

    k = j;
    p = fabs(a[jj]);

    if (j != *n) {
      for (i = j + 1; i <= *n; ++i) {
	q = fabs(a[i + ji]);
	if (q > p) {
	  k = i;
	  p = q;
	}
      } // for i

      if (k != j) {
	for (l = 1; l <= *n; ++l) {
	  const int li = l*(*idim);
	  const int jli = j + li;
	  const int kli = k + li;
	  tf = a[jli];
	  a[jli] = a[kli];
	  a[kli] = tf;
	} // for l
	++nxch;
	ir[nxch] = (j << 12) + k;
      } // if k != j
    } // if j!=n

    if (p <= 0.) {
      *det = 0;
      *ifail = -1;
      return;
    }

    *det *= a[jj];
//    t = fabs(*det);
//    if (t < 1e-19 || t > 1e19) {
//      *det = 0;
//	*ifail = -1;
//      return;
//    }

    a[jj] = 1. / a[jj];
    if (j == *n) {
      continue;
    }

    const int jm1 = j - 1;
    const int jpi = (j + 1) * (*idim);
    const int jjpi = j + jpi;

    for (k = j + 1; k <= *n; ++k) {
      const int ki  = k * (*idim);
      const int jki = j + ki;
      const int kji = k + jpi;
      if (j != 1) {
	for (i = 1; i <= jm1; ++i) {
	  const int ii = i * (*idim);
	  a[jki] -= a[i + ki] * a[j + ii];
	  a[kji] -= a[i + jpi] * a[k + ii];
	} // for i
      }
      a[jki] *= a[jj];
      a[kji] -= a[jjpi] * a[k + ji];
    } // for k
  } // for j

  if (nxch % 2 != 0) {
    *det = -(*det);
  }
  ir[*n] = nxch;
  return;
} // end of Dfactir1

void Dfinv1(int *n, double *a, int *idim, int *ir, int *ifail)
{
  /* Local variables */
  static int nxch, i, j, k, m, ij;
  static int im2, nm1, nmi;
  static double s31, s34, ti;

  *ifail = 0;

  if (*idim < *n || *n <= 0 || *n==1) {
    *ifail = -1;
    return;
  }

  
  /* Parameter adjustments */
  a -= *idim + 1;
  --ir;
  
  /* Function Body */
  
  /* finv.inc */

  a[*idim + 2] = -a[(*idim << 1) + 2] * (a[*idim + 1] * a[*idim + 2]);
  a[(*idim << 1) + 1] = -a[(*idim << 1) + 1];

  if (*n != 2) {
    for (i = 3; i <= *n; ++i) {
      const int ii   = i * (*idim);
      const int iii  = i + ii;
      const int imi  = ii - *idim;
      const int iimi = i + imi;
      im2 = i - 2;
      for (j = 1; j <= im2; ++j) {
	const int ji  = j * (*idim);
	const int jii = j + ii;
	s31 = 0.;
	for (k = j; k <= im2; ++k) {
	  s31 += a[k + ji] * a[i + k * (*idim)];
	  a[jii] += a[j + (k + 1) * (*idim)] * a[k + 1 + ii];
	} // for k
	a[i + ji] = -a[iii] * (a[i - 1 + ji] * a[iimi] + s31);
	a[jii] *= -1;
      } // for j
      a[iimi] = -a[iii] * (a[i - 1 + imi] * a[iimi]);
      a[i - 1 + ii] *= -1;
    } // for i
  } // if n!=2

  nm1 = *n - 1;
  for (i = 1; i <= nm1; ++i) {
    const int ii = i * (*idim);
    nmi = *n - i;
    for (j = 1; j <= i; ++j) {
      const int ji  = j * (*idim);
      const int iji = i + ji;
      for (k = 1; k <= nmi; ++k) {
	a[iji] += a[i + k + ji] * a[i + (i + k) * (*idim)];
      } // for k
    } // for j

    for (j = 1; j <= nmi; ++j) {
      const int ji = j * (*idim);
      s34 = 0.;
      for (k = j; k <= nmi; ++k) {
	s34 += a[i + k + ii + ji] * a[i + (i + k) * (*idim)];
      } // for k
      a[i + ii + ji] = s34;
    } // for j
  } // for i

  nxch = ir[*n];
  if (nxch == 0) {
    return;
  }

  for (m = 1; m <= nxch; ++m) {
    k = nxch - m + 1;
    ij = ir[k];
    i = ij / 4096;
    j = ij % 4096;
    const int ii = i * (*idim);
    const int ji = j * (*idim);
    for (k = 1; k <= *n; ++k) {
      ti = a[k + ii];
      a[k + ii] = a[k + ji];
      a[k + ji] = ti;
    } // for k
  } // for m

  return;
} // Dfinv1

void Dfact1(int *n, double *a, int *idim, double *ir, int *ifail, double *det, int *jfail)
{
  /* Local variables */
  static int nxch, i, j, k, l;
  static double p, q, tf;

  *ifail = 0;
  *jfail = 0;
  
  if (*idim < *n || *n <= 0) {
    *ifail =-1;
    return;
  }
  
  /* Parameter adjustments */
  a -= *idim + 1;

  /* Function Body */
  
  // fact.inc
  
  nxch = 0;
  *det = 1.;
  for (j = 1; j <= *n; ++j) {
    const int ji = j * (*idim);
    const int jj = j + ji;

    k = j;
    p = fabs(a[jj]);

    if (j != *n) {
      for (i = j + 1; i <= *n; ++i) {
	q = fabs(a[i + ji]);
	if (q > p) {
	  k = i;
	  p = q;
	}
      } // for i
      if (k != j) {
	for (l = 1; l <= *n; ++l) {
	  const int li = l*(*idim);
	  const int jli = j + li;
	  const int kli = k + li;
	  tf = a[jli];
	  a[jli] = a[kli];
	  a[kli] = tf;
	} // for l
	++nxch;
      } // if k != j
    } // if j!=n

    if (p <= 0.) {
      *det = 0;
      *ifail = -1;
      return;
    }

    *det *= a[jj];

//    t = fabs(*det);
//    if (t < 1e-19 || t > 1e19) {
//      *det = 0;
//	*ifail = -1;
//      return;
//    }

    a[jj] = 1. / a[jj];
    if (j == *n) {
      continue;
    }

    const int jm1 = j - 1;
    const int jpi = (j + 1) * (*idim);
    const int jjpi = j + jpi;

    for (k = j + 1; k <= *n; ++k) {
      const int ki  = k * (*idim);
      const int jki = j + ki;
      const int kji = k + jpi;
      if (j != 1) {
	for (i = 1; i <= jm1; ++i) {
	  const int ii = i * (*idim);
	  a[jki] -= a[i + ki] * a[j + ii];
	  a[kji] -= a[i + jpi] * a[k + ii];
	} // for i
      }
      a[jki] *= a[jj];
      a[kji] -= a[jjpi] * a[k + ji];
    } // for k
  } // for j

  if (nxch % 2 != 0) {
    *det = -(*det);
  }
  return;
} // end of Dfact1

void Dinv1(int *n, double *a, int *idim, double *ir, int *ifail)
{
      static double det = 0;

      *ifail = 0;

      if (*n < 1 || *n > *idim) {
	*ifail = -1;
	return;
      }

      if ( *n == 1)
      {
	if (a[0] == 0.) {
	    *ifail = -1;
    	    return;
	}
	a[0] = 1. / a[0];
	return;
      }

      if ( *n == 2)
      {
	det = a[0] * a[3] - a[2] * a[1];
    
	if (det == 0.) { *ifail = -1; return; }

	double s = 1. / det;
	double c11 = s * a[3];

	a[2] = -s * a[2];
	a[1] = -s * a[1];
	a[3] =  s * a[0];
	a[0] = c11;
	return;
      }



      if ( *n == 3 )
      {
	static double t1, t2, t3, temp, s;
	static double c11, c12, c13, c21, c22, c23, c31, c32, c33;
  
	/*     COMPUTE COFACTORS. */
	c11 = a[4] * a[8] - a[7] * a[5];
	c12 = a[7] * a[2] - a[1] * a[8];
	c13 = a[1] * a[5] - a[4] * a[2];
	c21 = a[5] * a[6] - a[8] * a[3];
	c22 = a[8] * a[0] - a[2] * a[6];
	c23 = a[2] * a[3] - a[5] * a[0];
	c31 = a[3] * a[7] - a[6] * a[4];
	c32 = a[6] * a[1] - a[0] * a[7];
	c33 = a[0] * a[4] - a[3] * a[1];

	t1 = fabs(a[0]);
	t2 = fabs(a[1]);
	t3 = fabs(a[2]);
    
	/*     (SET TEMP=PIVOT AND DET=PIVOT*DET.) */
	if(t1 < t2) {
    	    if (t3 < t2) {
	    /*        (PIVOT IS A21) */
		temp = a[1];
		det = c13 * c32 - c12 * c33;
    	    } else {
	    /*     (PIVOT IS A31) */
		temp = a[2];
		det = c23 * c12 - c22 * c13;
    	    }
	} else {
    	    if(t3 < t1) {
	    /*     (PIVOT IS A11) */
		temp = a[0];
		det = c22 * c33 - c23 * c32;
    	    } else {
	    /*     (PIVOT IS A31) */
		temp = a[2];
		det = c23 * c12 - c22 * c13;
    	    }
	}

	if (det == 0.) {
	    *ifail = -1;
    	    return;
	}

	/*     SET ELEMENTS OF INVERSE IN A. */
	s = temp / det;
	a[0] = s * c11;
	a[3] = s * c21;
	a[6] = s * c31;
	a[1] = s * c12;
	a[4] = s * c22;
	a[7] = s * c32;
	a[2] = s * c13;
	a[5] = s * c23;
	a[8] = s * c33;
	return;
      }

      /* Initialized data */

      static int work[100], jfail;
      for(int i=0; i<*n; ++i) work[i] = 0;

      /* Function Body */
      
      /*  N.GT.3 CASES.  FACTORIZE MATRIX AND INVERT. */
      Dfactir1(n, a, idim, work, ifail, &det, &jfail);
      if (*ifail != 0)
      {
	cerr << "Dfactir failed!!" << endl;
	return;
      }
      Dfinv1(n, a, idim, work, ifail);
      return;
} // Dinv1

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

    Dfact1(&n, work, &n, work2, &ifail, &det, &jfail);
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
    Dinv1(&dim, work, &dim, work2, &ifail);
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
