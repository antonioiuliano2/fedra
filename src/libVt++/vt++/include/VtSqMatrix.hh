#ifndef __VTSQMATRIX_HH
#define __VTSQMATRIX_HH
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
// 04 Sep 2000 (TG) modified return type of dinv()
// 17 Okt 2000 (TG) added comment
//
// *****************************************************************************

#include <iosfwd>
#include <assert.h>
#include <math.h>
#include "VtMatrix.hh"

void Dfactir1(int *n, double *a, int *idim, int *ir, int *ifail, double *det, int *jfail);
void Dfact1(int *n, double *a, int *idim, double *ir, int *ifail, double *det, int *jfail);
void Dfinv1(int *n, double *a, int *idim, int *ir, int *ifail);
void Dinv1(int *n, double *a, int *idim, double *ir, int *ifail);

namespace MATRIX {
  class VtVector;

  /** Class for quadratic matrices
   */
  //============================================================================
  // Class VtSqMatrix: quadratic Matrix class
  //============================================================================
  class  VtSqMatrix : public VtMatrix {
  public:
    /** @name --- Constructors --- */
    ///
    VtSqMatrix(const int row) : VtMatrix(row,row) {}
    ///
    VtSqMatrix(const VtSqMatrix& rhs) : VtMatrix(rhs) {}
    ///
    VtSqMatrix(const VtMatrix& rhs) : VtMatrix(rhs) { 
#ifndef VtFAST
      assert(rhs.ncol()==rhs.nrow()); 
#endif
    }
    ///
    virtual ~VtSqMatrix();

    /** @name --- Matrix operations --- */
    /// transform to inverse
    bool VtDinv(int dim = 0);
    /// return inverse
    const VtSqMatrix dinv(int dim = 0) const;
    /// return determinant
    virtual double det() const;
    
    // to avoid conversion VtMatrix->VtSqMatrix in argument
    virtual const VtMatrix  operator*(const VtMatrix& rhs);
    ///
    const VtVector  operator*(const VtVector& rhs) const;
  }; // class VtSqMatrix

  //============================================================================
  // operator<<
  //============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const VtSqMatrix& t ) {
    t.print(os);
    return os;
  }

} // end of namespace MATRIX
#endif
