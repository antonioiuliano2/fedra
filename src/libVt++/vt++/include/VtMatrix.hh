#ifndef __VTMATRIX_HH
#define __VTMATRIX_HH
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
// 11 Sep 2000 (TG) added documentation
// 10 Okt 2000 (TG) added copy() member function
// 11 Okt 2000 (TG) added clear() member function
// 17 Okt 2000 (TG) declared some members virtual
// 23 Okt 2000 (TG) added place_at(VtVector,...)
// 27 Okt 2000 (TG) made print() virtual
// 04 Sep 2001 (TG) added array()
//
// *****************************************************************************

#include <iosfwd>
#include <assert.h>

namespace MATRIX {
  class VtVector;
  class VtNegMatrix;

  /** Simple Matrix computation base class
   */
  //============================================================================
  // Class VtMatrix: simple Matrix class
  //============================================================================
  class  VtMatrix {
  public:
    /** @name --- Constructors --- */
    ///
    VtMatrix(const unsigned int row, const unsigned int col);
    ///
    VtMatrix(const VtMatrix& rhs);
    ///
    virtual ~VtMatrix();

    /** @name --- Access functions --- */
    /// no of rows $n$
    inline unsigned int nrow() const { return m_nrow; }
    /// no of columns $m$
    inline unsigned int ncol() const { return m_ncol; }
    /// $m\times n$
    inline int size() const { return m_size; }

    // helper class to implement m[i][j]
    class VtMatrix_row {
    public:
      inline VtMatrix_row(VtMatrix& a, int row):
	r_a(a), r_r(row) {}
      double& operator[](int col) {
	return r_a.operator()(r_r,col);
       	// return *(r_a.m + r_r*r_a.ncol() + col); 
      }
    private:
      VtMatrix& r_a;
      int       r_r;
    }; // VtMatrix_row

    // const version
    class VtMatrix_row_const {
    public:
      inline VtMatrix_row_const(const VtMatrix& a, int row):
	r_a(a), r_r(row) {}
      const double operator[](int col) const {
	return r_a.operator()(r_r,col);
       	// return *(r_a.m + r_r*r_a.ncol() + col);
      }
    private:
      const VtMatrix& r_a;
      int             r_r;
    }; // VtMatrix_row_const

    inline VtMatrix_row operator[](int row) {
      return VtMatrix_row(*this, row);
    }
    inline VtMatrix_row_const operator[](int row) const {
      return VtMatrix_row_const(*this, row);
    }

    virtual double  operator()(unsigned int row, unsigned int col) const;
    virtual double& operator()(const unsigned int row, const unsigned int col);

    inline double get(unsigned int row, unsigned int col) const {
      return *(m + row*m_ncol + col);
    }

    inline double& get(unsigned int row, unsigned int col) {
      return *(m + row*m_ncol + col);
    }

    /** @name --- Matrix operations --- */
    /// transform into transpose matrix
    void VtT(void);
    /// return transpose
    const VtMatrix T(void) const;
    /// copy a smaller matrix at a certain place
    virtual void place_at(const VtMatrix& rhs,
			  const unsigned int row,
			  const unsigned int col);
    /// copy a vector at a certain place
    virtual void place_at(const VtVector& rhs,
			  const unsigned int row,
			  const unsigned int col);
    /// to be used if matrix dimensions are not equal
    void copy(const VtMatrix& rhs);
    /// set matrix elements to 0
    void clear(void);

    /// $\textbf{A} = \textbf{B}$
    const VtMatrix&   operator= (const VtMatrix& rhs);
    ///
    const VtMatrix&   operator= (const VtNegMatrix& rhs);
    /// $\textbf{A} = (a_{\mu\nu} + \alpha)$
    virtual const VtMatrix&   operator+=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} - \alpha)$
    virtual const VtMatrix&   operator-=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} \cdot\alpha)$
    virtual const VtMatrix&   operator*=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} / \alpha)$
    virtual const VtMatrix&   operator/=(const double rhs);
    /// $\textbf{A} = (a_{\mu\nu} + b_{\mu\nu})$
    const VtMatrix&   operator+=(const VtMatrix& rhs);
    /// $\textbf{A} = (a_{\mu\nu} - b_{\mu\nu})$
    const VtMatrix&   operator-=(const VtMatrix& rhs);

    /// $\textbf{A} + \textbf{B}$
    const VtMatrix    operator+ (const VtMatrix&    rhs) const;
    ///
    const VtMatrix    operator+ (const VtNegMatrix& rhs) const;
    /// $\textbf{A} - \textbf{B}$
    const VtMatrix    operator- (const VtMatrix&    rhs) const;
    ///
    const VtMatrix    operator- (const VtNegMatrix& rhs) const;
    /// $-\textbf{A}$
    const VtNegMatrix operator- (void)                   const;
    /// $\textbf{A}\cdot\textbf{B} = \sum_{\nu=1}^n a_{\mu\nu}b_{\nu\lambda}$
    const VtMatrix    operator* (const VtMatrix&    rhs) const; 
    /// $\textbf{A}\cdot\vec{v} = (\sum_{\nu=1}^n a_{\mu\nu}v_{\nu})$
    const VtVector    operator* (const VtVector&    rhs) const;
    // not needed: const VtMatrix    operator* (const VtNegMatrix& rhs) const; 

    /** @name --- Expert functions --- */
    /// return pointer to internal array
    inline double* array() const { return m; }
    ///
    virtual void print(std::ostream& os) const;

  protected:
    double*      m;      // pointer to matrix array
    double*      work;   // workspace for CERNLIB routines 
    unsigned int m_nrow; // no of rows
    unsigned int m_ncol; // no of col

  private:
    friend class VtMatrix_row;
    friend class VtMatrix_row_const;
    int          m_size; // size of matrix array
  }; // class VtMatrix

  //==============================================================================
  // operator <<
  //==============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const VtMatrix& t ) {
    t.print(os);
    return os;
  }

} // namespace MATRIX
#endif
