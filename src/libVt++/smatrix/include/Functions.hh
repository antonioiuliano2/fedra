#ifndef __FUNCTIONS_HH
#define __FUNCTIONS_HH
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
// Description: Functions which are not applied like a unary operator
//
// changes:
// 16 Mar 2001 (TG) creation
// 03 Apr 2001 (TG) minimum added, doc++ comments added
// 07 Apr 2001 (TG) Lmag2, Lmag added
// 19 Apr 2001 (TG) added #include <cmath>
// 24 Apr 2001 (TG) added sign()
// 26 Jul 2001 (TG) round() added
// 27 Sep 2001 (TG) added Expr declaration
//
// ********************************************************************
#include <cmath>
#include "Expression.hh"

template <class T, unsigned int D> class SVector;


/** square.
    Template to compute $x\cdot x$

    @author T. Glebe
*/
//==============================================================================
// square: x*x
//==============================================================================
template <class T>
inline const T square(const T& x) { return x*x; }

/** maximum.
    Template to compute $\max(i,j)$

    @author T. Glebe
*/
//==============================================================================
// maximum
//==============================================================================
template <class T>
inline const T maximum(const T& lhs, const T& rhs) { 
  return (lhs > rhs) ? lhs : rhs; 
}

/** minimum.
    Template to compute $\min(i,j)$

    @author T. Glebe
*/
//==============================================================================
// minimum
//==============================================================================
template <class T>
inline const T minimum(const T& lhs, const T& rhs) { 
  return (lhs < rhs) ? lhs : rhs; 
}

/** round.
    Template to compute nearest integer value.

    @author T. Glebe
*/
//==============================================================================
// round
//==============================================================================
template <class T>
inline int round(const T& x) {
  return (x-static_cast<int>(x) < 0.5) ? static_cast<int>(x) : static_cast<int>(x+1);
}


/** sign.
    Template to compute the sign of a number $\textrm{sgn}(i)$.

    @author T. Glebe
*/
//==============================================================================
// sign
//==============================================================================
template <class T>
inline const int sign(const T& x) { return (x==0)? 0 : (x<0)? -1 : 1; }

//==============================================================================
// meta_dot
//==============================================================================
template <unsigned int I>
struct meta_dot {
  template <class A, class B, class T>
  static inline T f(const A& lhs, const B& rhs, const T& x) {
    return lhs.apply(I) * rhs.apply(I) + meta_dot<I-1>::f(lhs,rhs,x);
  }
};


//==============================================================================
// meta_dot<0>
//==============================================================================
template <>
struct meta_dot<0> {
  template <class A, class B, class T>
  static inline T f(const A& lhs, const B& rhs, const T& x) {
    return lhs.apply(0) * rhs.apply(0);
  }
};


/** dot.
    Template to compute $\vec{a}\cdot\vec{b} = \sum_i a_i\cdot b_i$.

    @author T. Glebe
*/
//==============================================================================
// dot
//==============================================================================
template <class T, unsigned int D>
inline T dot(const SVector<T,D>& lhs, const SVector<T,D>& rhs) {
  return meta_dot<D-1>::f(lhs,rhs, T());
}

//==============================================================================
// dot
//==============================================================================
template <class A, class T, unsigned int D>
inline T dot(const SVector<T,D>& lhs, const Expr<A,T,D>& rhs) {
  return meta_dot<D-1>::f(lhs,rhs, T());
}

//==============================================================================
// dot
//==============================================================================
template <class A, class T, unsigned int D>
inline T dot(const Expr<A,T,D>& lhs, const SVector<T,D>& rhs) {
  return meta_dot<D-1>::f(lhs,rhs, T());
}


//==============================================================================
// dot
//==============================================================================
template <class A, class B, class T, unsigned int D>
inline T dot(const Expr<A,T,D>& lhs, const Expr<B,T,D>& rhs) {
  return meta_dot<D-1>::f(rhs,lhs, T());
}


//==============================================================================
// meta_mag
//==============================================================================
template <unsigned int I>
struct meta_mag {
  template <class A, class T>
  static inline T f(const A& rhs, const T& x) {
    return square(rhs.apply(I)) + meta_mag<I-1>::f(rhs, x);
  }
};


//==============================================================================
// meta_mag<0>
//==============================================================================
template <>
struct meta_mag<0> {
  template <class A, class T>
  static inline T f(const A& rhs, const T& x) {
    return square(rhs.apply(0));
  }
};


/** mag2.
    Template to compute $|\vec{v}|^2 = \sum_iv_i^2$.

    @author T. Glebe
*/
//==============================================================================
// mag2
//==============================================================================
template <class T, unsigned int D>
inline T mag2(const SVector<T,D>& rhs) {
  return meta_mag<D-1>::f(rhs, T());
}

//==============================================================================
// mag2
//==============================================================================
template <class A, class T, unsigned int D>
inline T mag2(const Expr<A,T,D>& rhs) {
  return meta_mag<D-1>::f(rhs, T());
}

/** mag.
    Length of a vector: $|\vec{v}| = \sqrt{\sum_iv_i^2}$.

    @author T. Glebe
*/
//==============================================================================
// mag
//==============================================================================
template <class T, unsigned int D>
inline T mag(const SVector<T,D>& rhs) {
  return sqrt(mag2(rhs));
}

//==============================================================================
// mag
//==============================================================================
template <class A, class T, unsigned int D>
inline T mag(const Expr<A,T,D>& rhs) {
  return sqrt(mag2(rhs));
}


/** Lmag2.
    Template to compute $|\vec{v}|^2 = v_0^2 - v_1^2 - v_2^2 -v_3^2$.
    
    @author T. Glebe
*/
//==============================================================================
// Lmag2
//==============================================================================
template <class T>
inline T Lmag2(const SVector<T,4>& rhs) {
  return square(rhs[0]) - square(rhs[1]) - square(rhs[2]) - square(rhs[3]);
}

//==============================================================================
// Lmag2
//==============================================================================
template <class A, class T>
inline T Lmag2(const Expr<A,T,4>& rhs) {
  return square(rhs.apply(0)) 
    - square(rhs.apply(1)) - square(rhs.apply(2)) - square(rhs.apply(3));
}

/** Lmag.
    Length of a vector Lorentz-Vector: $|\vec{v}| = \sqrt{v_0^2 -
    v_1^2 - v_2^2 -v_3^2}$.

    @author T. Glebe
*/
//==============================================================================
// Lmag
//==============================================================================
template <class T>
inline T Lmag(const SVector<T,4>& rhs) {
  return sqrt(Lmag2(rhs));
}

//==============================================================================
// Lmag
//==============================================================================
template <class A, class T>
inline T Lmag(const Expr<A,T,4>& rhs) {
  return sqrt(Lmag2(rhs));
}


/** cross.
    Cross product of two 3-dim vectors: $\vec{c} = \vec{a}\times\vec{b}$.

    @author T. Glebe
*/
//==============================================================================
// cross product
//==============================================================================
template <class T>
inline SVector<T,3> cross(const SVector<T,3>& lhs, const SVector<T,3>& rhs) {
  return SVector<T,3>(lhs.apply(1)*rhs.apply(2) -
		      lhs.apply(2)*rhs.apply(1),
		      lhs.apply(2)*rhs.apply(0) -
		      lhs.apply(0)*rhs.apply(2),
		      lhs.apply(0)*rhs.apply(1) -
		      lhs.apply(1)*rhs.apply(0));
}

//==============================================================================
// cross product
//==============================================================================
template <class A, class T>
inline SVector<T,3> cross(const Expr<A,T,3>& lhs, const SVector<T,3>& rhs) {
  return SVector<T,3>(lhs.apply(1)*rhs.apply(2) -
		      lhs.apply(2)*rhs.apply(1),
		      lhs.apply(2)*rhs.apply(0) -
		      lhs.apply(0)*rhs.apply(2),
		      lhs.apply(0)*rhs.apply(1) -
		      lhs.apply(1)*rhs.apply(0));
}

//==============================================================================
// cross product
//==============================================================================
template <class T, class A>
inline SVector<T,3> cross(const SVector<T,3>& lhs, const Expr<A,T,3>& rhs) {
  return SVector<T,3>(lhs.apply(1)*rhs.apply(2) -
		      lhs.apply(2)*rhs.apply(1),
		      lhs.apply(2)*rhs.apply(0) -
		      lhs.apply(0)*rhs.apply(2),
		      lhs.apply(0)*rhs.apply(1) -
		      lhs.apply(1)*rhs.apply(0));
}

//==============================================================================
// cross product
//==============================================================================
template <class A, class B, class T>
inline SVector<T,3> cross(const Expr<A,T,3>& lhs, const Expr<B,T,3>& rhs) {
  return SVector<T,3>(lhs.apply(1)*rhs.apply(2) -
		      lhs.apply(2)*rhs.apply(1),
		      lhs.apply(2)*rhs.apply(0) -
		      lhs.apply(0)*rhs.apply(2),
		      lhs.apply(0)*rhs.apply(1) -
		      lhs.apply(1)*rhs.apply(0));
}


/** unit.
    Return a vector of unit lenght: $\vec{e}_v = \vec{v}/|\vec{v}|$.

    @author T. Glebe
*/
//==============================================================================
// unit: returns a unit vector
//==============================================================================
template <class T, unsigned int D>
inline SVector<T,D> unit(const SVector<T,D>& rhs) {
  return SVector<T,D>(rhs).unit();
}

//==============================================================================
// unit: returns a unit vector
//==============================================================================
template <class A, class T, unsigned int D>
inline SVector<T,D> unit(const Expr<A,T,D>& rhs) {
  return SVector<T,D>(rhs).unit();
}

#ifdef XXX
//==============================================================================
// unit: returns a unit vector (worse performance)
//==============================================================================
template <class T, unsigned int D>
inline Expr<BinaryOp<DivOp<T>, SVector<T,D>, Constant<T>, T>, T, D>
 unit(const SVector<T,D>& lhs) {
  typedef BinaryOp<DivOp<T>, SVector<T,D>, Constant<T>, T> DivOpBinOp;
  return Expr<DivOpBinOp,T,D>(DivOpBinOp(DivOp<T>(),lhs,Constant<T>(mag(lhs))));
}

//==============================================================================
// unit: returns a unit vector (worse performance)
//==============================================================================
template <class A, class T, unsigned int D>
inline Expr<BinaryOp<DivOp<T>, Expr<A,T,D>, Constant<T>, T>, T, D>
 unit(const Expr<A,T,D>& lhs) {
  typedef BinaryOp<DivOp<T>, Expr<A,T,D>, Constant<T>, T> DivOpBinOp;
  return Expr<DivOpBinOp,T,D>(DivOpBinOp(DivOp<T>(),lhs,Constant<T>(mag(lhs))));
}
#endif

#endif
