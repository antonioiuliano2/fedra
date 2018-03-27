#ifndef __VTVECTOR_HH
#define __VTVECTOR_HH
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
// 05 Sep 2000 (TG) mag(), mag2() added
// 11 Sep 2000 (TG) added documentation
// 14 Sep 2000 (TG) place_at() added
// 11 Okt 2000 (TG) added clear() member function
// 09 Mar 2001 (TG) unit(), operator*(double) added
// 08 Jan 2002 (TG) moved inline functions to VtVector.icc file
//
// *****************************************************************************

#include <cmath>
#include <iosfwd>
#ifdef WIN32
#   include "Rtypes.h"
#endif

namespace MATRIX {
  class VtVector; // to fake DOC++

  /** Vector class */
  //============================================================================
  // Class VtVector
  //============================================================================
  class VtVector {
  public:
    /** @name --- Constructors --- */
    ///
    VtVector(const unsigned int size);
    ///
    VtVector(double v1, double v2);
    ///
    VtVector(double v1, double v2, double v3);
    ///
    VtVector(double v1, double v2, double v3, double v4);
    ///
    VtVector(double v1, double v2, double v3, double v4, double v5);
    ///
    VtVector(double v1, double v2, double v3, double v4, double v5,
	     double v6);
    ///
    VtVector(const VtVector& rhs);
    ///
    ~VtVector();

    /** @name --- Operators --- */
    ///
    double  operator[](unsigned int row) const;
    ///
    double& operator[](const unsigned int row);
    ///
    double  operator()(unsigned int row) const;
    ///
    double& operator()(const unsigned int row);

    /// $\vec{v} = \vec{w}$
    const VtVector& operator= (const VtVector& rhs);
    /// $\vec{v} = (v_\mu + \alpha)$
    const VtVector& operator+=(const double rhs);
    /// $\vec{v} = (v_\mu - \alpha)$
    const VtVector& operator-=(const double rhs);
    /// $\vec{v} = (v_\mu \cdot \alpha)$
    const VtVector& operator*=(const double rhs);
    /// $\vec{v} = (v_\mu / \alpha)$
    const VtVector& operator/=(const double rhs);
    /// $\vec{v} = (v_\mu + w_\mu)$
    const VtVector& operator+=(const VtVector& rhs);
    /// $\vec{v} = (v_\mu - w_\mu)$
    const VtVector& operator-=(const VtVector& rhs);
    /// $\vec{v} + \vec{w}$
    const VtVector  operator+ (const VtVector& rhs) const;
    /// $\vec{v} - \vec{w}$
    const VtVector  operator- (const VtVector& rhs) const;
    /// $\vec{v} * \vec{w} = \sum_\mu v_\mu \cdot w_\mu$
    const double    operator* (const VtVector& rhs) const;
    /// $\vec{v} = (v_\mu \cdot \alpha)$
    VtVector operator*(const double rhs) const;

    /** @name --- Access methods --- */
    /// vector dimension
    unsigned int size() const;
    /// vector dimension
    unsigned int nrow() const;
    /// $\sum_i v_i^2$
    double mag2() const;
    /// $\sqrt{\sum_i v_i^2}$ vector length
    double mag() const;
    /// return a unit vector
    VtVector unit() const;
    
    /** @name --- Expert functions --- */
    /// called by cout
    void print(std::ostream& os) const;
    /// copy a smaller vector at a certain place
    void place_at(const VtVector& rhs, const unsigned int row);
    /// to be used if vector dimensions differ
    void copy(const VtVector& rhs, const unsigned int offset);
    /// set vector elements to 0
    void clear(void);

  private:
    unsigned int v_size;  // no of rows
    double*      v;       // data array

#ifdef WIN32
  ClassDef(VtVector,0)
#endif
  };

  //==============================================================================
  // operator <<
  //==============================================================================
  std::ostream& operator<< ( std::ostream& os, const VtVector& t );

#include "VtVector.icc"
} // namespace MATRIX
#endif
