#ifndef __SVECTOR_HH
#define __SVECTOR_HH
// ********************************************************************
//
// source:
//
// type:      source code
//
// created:   16. Mar 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: A fixed size Vector class
//
// changes:
// 16 Mar 2001 (TG) creation
// 21 Mar 2001 (TG) SVector::value_type added
// 21 Mar 2001 (TG) added operators +=, -=, *=, /=
// 26 Mar 2001 (TG) added place_at()
// 03 Apr 2001 (TG) Array() added
// 06 Apr 2001 (TG) CTORS added
// 07 Apr 2001 (TG) CTORS added
// 22 Aug 2001 (TG) CTOR(T*,len) added
// 04 Sep 2001 (TG) moved inlined functions to .icc file
// 14 Jan 2002 (TG) added operator==(), operator!=(), operator>(), operator<()
//
// ********************************************************************
#include <iosfwd>

template <class A, class T, unsigned int D, unsigned int D2> class Expr;

// expression engine
#include "Expression.hh"

/** SVector.
    A generic fixed size Vector class.

    @memo SVector
    @author T. Glebe
*/
//==============================================================================
// SVector
//==============================================================================
template <class T, unsigned int D>
class SVector {
public:
  /** @name --- Typedefs --- */
  ///
  typedef T  value_type;

  /** @name --- Constructors --- */
  ///
  SVector();
  ///
  template <class A>
  SVector(const Expr<A,T,D>& rhs);
  ///
  SVector(const SVector<T,D>& rhs);
  /// $D1\le D$ required!
  template <unsigned int D1>
  SVector(const SVector<T,D1>& rhs);
  /// $D1\le D-1$ required!
  template <unsigned int D1>
  SVector(const T& a1, const SVector<T,D1>& rhs);
  /// fill from array, len must be equal to D!
  SVector(const T* a, unsigned int len);
  ///
  SVector(const T& rhs);
  ///
  SVector(const T& a1, const T& a2);
  ///
  SVector(const T& a1, const T& a2, const T& a3);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5, const T& a6);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5, const T& a6, const T& a7);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5, const T& a6, const T& a7, const T& a8);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5, const T& a6, const T& a7, const T& a8,
	  const T& a9);
  ///
  SVector(const T& a1, const T& a2, const T& a3, const T& a4,
	  const T& a5, const T& a6, const T& a7, const T& a8,
	  const T& a9, const T& a10);

  ///
  SVector<T,D>& operator=(const T& rhs);
  ///
  template <class A>
  SVector<T,D>& operator=(const Expr<A,T,D>& rhs);

  /** @name --- Access functions --- */
  /// return dimension $D$
  inline static unsigned int dim() { return D; }
  /// access the parse tree
  T apply(unsigned int i) const;
  /// return read-only pointer to internal array
  const T* Array() const;
  /// return pointer to internal array
  T* Array();

  /** @name --- Operators --- */
  /// element wise comparison
  bool operator==(const T& rhs) const;
  /// element wise comparison
  bool operator!=(const T& rhs) const;
  /// element wise comparison
  bool operator==(const SVector<T,D>& rhs) const;
  /// element wise comparison
  bool operator!=(const SVector<T,D>& rhs) const;
  /// element wise comparison
  template <class A>
  bool operator==(const Expr<A,T,D>& rhs) const;
  /// element wise comparison
  template <class A>
  bool operator!=(const Expr<A,T,D>& rhs) const;
  
  /// element wise comparison
  bool operator>(const T& rhs) const;
  /// element wise comparison
  bool operator<(const T& rhs) const;
  /// element wise comparison
  bool operator>(const SVector<T,D>& rhs) const;
  /// element wise comparison
  bool operator<(const SVector<T,D>& rhs) const;
  /// element wise comparison
  template <class A>
  bool operator>(const Expr<A,T,D>& rhs) const;
  /// element wise comparison
  template <class A>
  bool operator<(const Expr<A,T,D>& rhs) const;

  /// read-only access
  const T& operator[](unsigned int i) const;
  /// read-only access
  const T& operator()(unsigned int i) const;
  /// read/write access
  T& operator[](unsigned int i);
  /// read/write access
  T& operator()(unsigned int i);

  ///
  SVector<T,D>& operator+=(const SVector<T,D>& rhs);
  ///
  template <class A>
  SVector<T,D>& operator+=(const Expr<A,T,D>& rhs);
  ///
  SVector<T,D>& operator-=(const SVector<T,D>& rhs);
  ///
  template <class A>
  SVector<T,D>& operator-=(const Expr<A,T,D>& rhs);
  ///
  SVector<T,D>& operator*=(const SVector<T,D>& rhs);
  ///
  template <class A>
  SVector<T,D>& operator*=(const Expr<A,T,D>& rhs);
  ///
  SVector<T,D>& operator/=(const SVector<T,D>& rhs);
  ///
  template <class A>
  SVector<T,D>& operator/=(const Expr<A,T,D>& rhs);


  /** @name --- Expert functions --- */
  /// transform vector into a vector of lenght 1
  SVector<T,D>& unit();
  /// place a sub-vector starting at <row>
  template <unsigned int D2>
  SVector<T,D>& place_at(const SVector<T,D2>& rhs, const unsigned int row);
  /// place a sub-vector starting at <row>
  template <class A, unsigned int D2>
  SVector<T,D>& place_at(const Expr<A,T,D2>& rhs, const unsigned int row);
  /// used by operator<<()
  std::ostream& print(std::ostream& os) const;

private:
  T array[D];
}; // end of class SVector

//==============================================================================
// operator<<
//==============================================================================
template <class T, unsigned int D>
std::ostream& operator<<(std::ostream& os, const SVector<T,D>& rhs);

// include implementation file
#include "SVector.icc"

// include operators and functions
#include "UnaryOperators.hh"
#include "BinaryOperators.hh"
#include "Functions.hh"
#endif
