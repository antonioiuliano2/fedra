#ifndef __VTSYMMATRIX
#define __VTSYMMATRIX
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
// Description: class for symmetric matrices
//
// changes:
// 21 Aug 2000 (TG) creation
// 04 Sep 2000 (TG) modified return type of dsinv()
// 10 Okt 2000 (TG) copy() member function added.
// 17 Okt 2000 (TG) copy constructor added, comments added
// 24 Okt 2000 (TG) added operator+=, operator-= (on matrices)
// 27 Okt 2000 (TG) modified operator<< for CINT
// 05 Feb 2001 (TG) renamed Assert() to VtAssert()
//
// *****************************************************************************

#include "VtSqMatrix.hh"

void Dsinv1(int *idim, double *a, int *n, int *ifail);
void Dsfact1(int *idim, double *a, int *n, int *ifail, double *det, int *jfail);

namespace MATRIX {
  class VtVector;

  /** Class for symmetric matrices
   */
  //============================================================================
  // Class VtSymMatrix: base class for covariance matrices (symm, pos. def.)
  //============================================================================
  class VtSymMatrix : public VtSqMatrix {
  public:
    /** @name --- Constructors --- */
    ///
    VtSymMatrix(const unsigned int dim) : VtSqMatrix(dim) {}
    /// initalize diagonal elements
    VtSymMatrix(const unsigned int dim, double diag);
    /// copy constructor
    VtSymMatrix(const VtSymMatrix& rhs);
    ///
    VtSymMatrix(const VtSqMatrix& rhs) : VtSqMatrix(rhs) {
      //      Assert();
    }
    ///
    VtSymMatrix(const VtMatrix& rhs) : VtSqMatrix(rhs) {
      //      Assert();
    }
    virtual ~VtSymMatrix();
   
    /** @name --- Matrix operations --- */
    /// calc inverse using momentum or not
    bool invert(const bool use_momentum);
    /// transform to inverse
    bool VtDsinv(int dim = 0);
    /// return inverse
    const VtSymMatrix dsinv(int dim = 0) const;
    /// compute determinant via CERNLIB dsfact()
    virtual double det() const;
    /// compute $v^t*A*v$
    double product(const VtVector& rhs, unsigned int dim = 0) const;
    /// compute $B^t*A*B$
    const VtSymMatrix product(const VtMatrix& rhs) const;
    /// copy a smaller matrix at a certain place
    virtual void place_at(const VtMatrix& rhs,
			  const unsigned int row,
			  const unsigned int col);
    /// in case matrix dimensions differ
    void copy(const VtSymMatrix& rhs);

    /// copy 
    const VtSymMatrix& operator=(const VtSymMatrix& rhs);

    /// $\textbf{A} = (a_{\mu\nu} + \alpha)$
    const VtSymMatrix&   operator+=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} - \alpha)$
    const VtSymMatrix&   operator-=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} \cdot\alpha)$
    const VtSymMatrix&   operator*=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} / \alpha)$
    const VtSymMatrix&   operator/=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} + b_{\mu\nu})$
    const VtSymMatrix& operator+=(const VtSymMatrix& rhs);
    /// $\textbf{A} = (a_{\mu\nu} - b_{\mu\nu})$
    const VtSymMatrix& operator-=(const VtSymMatrix& rhs);

    ///
    const VtSymMatrix  operator+(const VtSymMatrix& rhs) const;
    ///
    const VtSqMatrix   operator+(const VtSqMatrix&  rhs) const;
    ///
    const VtMatrix     operator+(const VtMatrix&    rhs) const;
    ///
    const VtSymMatrix  operator-(const VtSymMatrix& rhs) const;
    ///
    const VtSymMatrix  operator-(void)                   const;
    ///
    const VtSqMatrix   operator-(const VtSqMatrix&  rhs) const;
    ///
    const VtMatrix     operator-(const VtMatrix&    rhs) const;
    ///
    const VtSqMatrix   operator*(const VtSymMatrix& rhs) const;
    ///
    const VtSqMatrix   operator*(const VtSqMatrix&  rhs) const;
    ///
    const VtMatrix     operator*(const VtMatrix&    rhs) const;
    ///
    const VtVector     operator*(const VtVector&    rhs) const;

    /** @name --- Expert functions --- */
    ///
    virtual void print(std::ostream& os) const;

  private:
    void VtAssert(void);
  };
  
  //============================================================================
  // operator<<
  //============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const VtSymMatrix& t ) {
    t.print(os);
    return os;
  }

} // end of namespace MATRIX
#endif
