#ifndef __VTNEGMATRIX_HH
#define __VTNEGMATRIX_HH
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

#include <iosfwd>
#include <assert.h>
#include "VtMatrix.hh"

namespace MATRIX {
  class VtVector;

  /** A class for negative matrices to optimize expressions like
      $C = -A * B$. For internal use only!
      @memo Negative matrix class
  */
  //============================================================================
  // Class VtNegMatrix: simple negative Matrix class
  //============================================================================
  class  VtNegMatrix : public VtMatrix {
  public:
    /** @name --- Constructors --- */
    ///
    VtNegMatrix(const unsigned int row, const unsigned int col);
    ///
    VtNegMatrix(const VtNegMatrix& rhs);
    ///
    VtNegMatrix(const VtMatrix& rhs);
    ///
    ~VtNegMatrix() {}

    double  operator()(unsigned int row, unsigned int col) const;
    double& operator()(const unsigned int row, const unsigned int col);

    inline double nget(unsigned int row, unsigned int col) const {
      return -*(m + row*m_ncol + col);
    }
    inline double& nget(unsigned int row, unsigned int col) {
      return *(m + row*m_ncol + col);
    }

    /** @name --- Matrix operations --- */
    /// return transposed matrix
    const VtNegMatrix T(void) const;

    ///
    const VtNegMatrix& operator+=(const double       rhs);
    ///
    const VtNegMatrix& operator-=(const double       rhs);
    ///
    const VtNegMatrix& operator*=(const double       rhs);
    ///
    const VtNegMatrix& operator/=(const double       rhs);
    ///
    const VtNegMatrix& operator+=(const VtMatrix&    rhs);
    ///
    const VtNegMatrix& operator+=(const VtNegMatrix& rhs);
    ///
    const VtNegMatrix& operator-=(const VtMatrix&    rhs);
    ///
    const VtNegMatrix& operator-=(const VtNegMatrix& rhs);

    ///
    const VtMatrix     operator+ (const VtMatrix&    rhs) const;
    ///
    const VtMatrix     operator+ (const VtNegMatrix& rhs) const;
    ///
    const VtMatrix     operator- (const VtMatrix&    rhs) const;
    //not needed:    const VtMatrix     operator- (const VtNegMatrix& rhs) const;
    ///
    const VtMatrix     operator- (void) const;

    ///
    const VtMatrix     operator* (const VtMatrix& rhs) const;
    ///
    const VtVector     operator* (const VtVector& rhs) const;
    // not needed: const VtMatrix operator*(const VtNegMatrix& rhs) const;
  }; // class VtNegMatrix


  //==============================================================================
  // operator <<
  //==============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const VtNegMatrix& t ) {
    t.print(os);
    return os;
  }


} // namespace MATRIX
#endif
