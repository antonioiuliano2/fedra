#ifndef __RMINFC
#define __RMINFC
// **********************************************************************
//
// source:
//
// type:      source code
//
// created:   26.07.2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: Function which finds the minimum of a function with one
//              variable. The "golden section search" is applied. The method
//              uses a fixed number of function evaluations.
//
// changes:
// 26 Jul 2001 (TG) creation from CERNLIB minfc.F file (f2c + handoptimization)
//
// **********************************************************************
#include <cmath>
#include "Functions.hh"

/** Rminfc. 
    Function which finds a single, local minimum of a function with one
    variable in a given interval. The "golden section search" is applied. The
    method uses a fixed number $n$ of function evaluations, where 
    $n = [2.08\cdot\ln(|a-b|/\epsilon)+1/2] + 1$. 

    @param f     one-dimensional function
    @param a,b   end-points of search interval
    @param eps   accuracy parameter $\epsilon$
    @param delta tolerance interval $\delta$ near $a$ and $b$. Suggested value: $\delta = 10\epsilon$
    @param x     computed approximation to the abscissa of a minimum of the function $f$
    @param y     value of $f(x)$
*/
template <class T, class P>
bool Rminfc( T (*f)(P), P a, P b, double eps, double delta, P& x, P& y) {

  // Local variables
  static P c, d, h;
  static int n;
  static P v, w, fv, fw;
  bool lge = true, llt = true;
  

  n = -1;
  if (a != b) {
    n = round(log(fabs(a - b) / eps) * 2.08);
  }
  c = a;
  d = b;
  if (a > b) {
    c = b;
    d = a;
  }

  do {
    h = d - c;
    if (llt == true) {
      v = c + h * 0.3819660112501051;
      fv = (*f)(v);
    }
    if (lge == true) {
      w = c + h * 0.6180339887498949;
      fw = (*f)(w);
    }
    if (fv < fw) {
      llt = true;
      lge = false;
      d = w;
      w = v;
      fw = fv;
    } else {
      llt = false;
      lge = true;
      c = v;
      v = w;
      fv = fw;
    }
    --n;
  } while (n >= 0);

  x = (c + d) * .5;
  y = (*f)(x);
  return (fabs(x - a) > delta) && (fabs(x - b) > delta);
} // Rminfc


/** RminfcM. 
    Rminfc version for const class member functions.

    Function which finds a single, local minimum of a function with one
    variable in a given interval. The "golden section search" is applied. The
    method uses a fixed number $n$ of function evaluations, where 
    $n = [2.08\cdot\ln(|a-b|/\epsilon)+1/2] + 1$. 

    @param f     one-dimensional function
    @param a,b   end-points of search interval
    @param eps   accuracy parameter $\epsilon$
    @param delta tolerance interval $\delta$ near $a$ and $b$. Suggested value: $\delta = 10\epsilon$
    @param x     computed approximation to the abscissa of a minimum of the function $f$
    @param y     value of $f(x)$
*/
template <class C, class T, class P>
bool RminfcM(const C& part, T (C::*f)(P) const, P a, P b, double eps, double delta, P& x, P& y) {

  // Local variables
  static P c, d, h;
  static int n;
  static P v, w, fv, fw;
  bool lge = true, llt = true;
  

  n = -1;
  if (a != b) {
    n = round(log(fabs(a - b) / eps) * 2.08);
  }
  c = a;
  d = b;
  if (a > b) {
    c = b;
    d = a;
  }

  do {
    h = d - c;
    if (llt == true) {
      v = c + h * 0.3819660112501051;
      fv = (part.*f)(v);
    }
    if (lge == true) {
      w = c + h * 0.6180339887498949;
      fw = (part.*f)(w);
    }
    if (fv < fw) {
      llt = true;
      lge = false;
      d = w;
      w = v;
      fw = fv;
    } else {
      llt = false;
      lge = true;
      c = v;
      v = w;
      fv = fw;
    }
    --n;
  } while (n >= 0);

  x = (c + d) * .5;
  y = (part.*f)(x);
  return (fabs(x - a) > delta) && (fabs(x - b) > delta);
} // Rminfc

#endif
