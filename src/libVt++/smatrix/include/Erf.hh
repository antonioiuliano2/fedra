#ifndef __ERF_HH
#define __ERF_HH
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
// Description: Error (errf) and complementary error (errfc).
//
// changes:
// 19 Apr 2001 (TG) creation
//
// ********************************************************************
#include <cmath>

/** erf\_0\_.
    Routine which does the basic work.

    @author T. Glebe
*/
template <class T>
T erf_0_(int n, const T& rhs) {

  /* Initialized data */
  static const T p10   = 3.6767877;
  static const T q10   = 3.2584593;
  static const T p11   = -.097970465;
  static const T p2[5] = { 7.3738883,6.8650185,3.0317993,.56316962,4.3187787e-5 };
  static const T q2[5] = { 7.3739609,15.184908,12.79553,5.3542168,1. };
  static const T p30   = -.12436854;
  static const T q30   = .44091706;
  static const T p31   = -.096821036;

  /* Local variables */
  static T h;
  static int i;
  static T hc, ap, aq;
  static bool lef;
  
  /*     Above the value of VMAX any calculation is pointless. The value is 
   */
  /*     choosen with a big safety margin - even the double precision */
  /*     version only returns 1. for V (=ABS(X)) >= 5.9 */
  /*     The value for SWITCH is badly chosen for the single precision */
  /*     version, which returns 1. already for V >= 3.9 */

  if(n==1)
    lef = false;
  else
    lef = true;

  const T v = fabs(rhs);
  const T v2 = v*v;

  if (v < .5) {
    /* Computing 2nd power */
    h = rhs * (p10 + p11 * v2) / (q10 + v2);
    hc = 1 - h;
  } else {
    if (v < 4.) {
      ap = p2[4];
      aq = q2[4];
      for (i = 3; i >= 0; --i) {
	ap = p2[i] + v * ap;
	aq = q2[i] + v * aq;
      }
      /* Computing 2nd power */
      hc = exp(-v2) * ap / aq;
      h = 1 - hc;
    } else if (v < 7.) {
      /* Computing 2nd power */
      const T y = 1 / v2;
      /* Computing 2nd power */
      hc = exp(-v2) * (y * (p30 + p31 * y) / (q30 + y) + .56418958) / v;
      h = 1 - hc;
      /*     for very big values we can save us any calculation, and the
       */
      /*     FP-exceptions we would get from EXP. */
    } else {
      h  = 1.;
      hc = 0.;
    }
    if (rhs < 0.) {
      h  = -h;
      hc = 2 - hc;
    }
  }

  if (lef) {
    return h;
  } else {
    return hc;
  }
}

/** Error function.
    \begin{displaymath}
    \textrm{erf}(x) = \frac{2}{\sqrt{\pi}} \int^x_0 e^{-t^2} dt
    \end{displaymath}

    @author T. Glebe
*/
template <class T>
T erf(const T& rhs) {
  return erf_0_(0, rhs);
}

/** Error function.
    \begin{displaymath}
    \textrm{erfc}(x) = \frac{2}{\sqrt{\pi}} \int^\infty_x e^{-t^2} dt
    \end{displaymath}

    @author T. Glebe
*/
template <class T>
T erfc(const T& rhs) {
  return erf_0_(1, rhs);
}
#endif
