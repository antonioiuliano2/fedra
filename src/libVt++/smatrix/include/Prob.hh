#ifndef __PROB_HH
#define __PROB_HH
// ********************************************************************
//
// source:
//
// type:      source code
//
// created:   19. Apr 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: Upper Tail Probability of Chi-Squared Distribution
//
// changes:
// 19 Apr 2001 (TG) creation
//
// ********************************************************************
//#include <cmath>
#include "Erf.hh"

/** Upper Tail Probability of Chi-Squared Distribution.
    \begin{displaymath}
    Q(X|N) = \frac{1}{\sqrt{2^N}\Gamma(\frac{1}{2}N)} \int^\infty_X
    e^{-\frac{1}{2}t} t^{\frac{1}{2}N-1} dt
    \end{displaymath}

    @author T. Glebe
*/
template <class T>
T Prob(const T& rhs, int n) {

  /* Local variables */
  static T e, h;
  static int i, m;
  static T s, t, u, w, fi;

  // maximum chi2 per df for df >= 2., if chi2/df > chipdf prob=0.
  u = rhs * .5;
  if (n <= 0) {
    h = 0.;
    // error
  } else if (rhs < 0.) {
    h = 0.;
    // error
  } else if (rhs == 0. ||  (n / 20) > rhs) {
    h = 1.;
  } else if (n == 1) {
    w = sqrt(u);
    if (w < 24.) {
      h = erfc(w);
    } else {
      h = 0.;
    }
  } else if (n > 300) {
    s = 1. / n;
    t = s * .22222222222222221;
    w = (pow(rhs*s, 1./3.) - (1 - t)) / sqrt(t * 2);
    if (w < -24.) {
      h = 1.;
    } else if (w < 24.) {
      h = erfc(w) * .5;
    } else {
      h = 0.;
    }
  } else {
    m = n / 2;
    if (u < 349.346 && rhs / n <= 100.) {
      s = exp(u * -.5);
      t = s;
      e = s;
      if (m << 1 == n) {
	fi = 0.;
	for (i = 1; i <= m-1; ++i) {
	  fi += 1;
	  t = u * t / fi;
	  s += t;
	}
	h = s * e;
      } else {
	fi = 1.;
	for (i = 1; i <= m-1; ++i) {
	  fi += 2;
	  t = t * rhs / fi;
	  s += t;
	}
	w = sqrt(u);
	if (w < 24.) {
	  h = w * 1.128379167095513 * s * e + erfc(w);
	} else {
	  h = 0.;
	}
      }
    } else {
      h = 0.;
    }
  }

  if (h > 1e-30) {
    return h;
  } else {
    return 0.;
  }
} // Prob
#endif
