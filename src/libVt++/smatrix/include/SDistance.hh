#ifndef __SDISTANCE_HH
#define __SDISTANCE_HH
// *****************************************************************************
//
// source:
//
// type:      source code
//
// created:   24. Apr 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: an (incomplete) collection of distance functions
//
// changes:
// 24 Apr 2001 (TG) creation
// 04 Mai 2001 (TG) Tdistance() added
// 06 Jul 2001 (TG) corrected Tdistance()
// 27 Jul 2001 (TG) added Sdistance(SVector,SVector)
// 15 Aug 2001 (TG) added Sdistance(Vertex,Vertex)
// 28 Sep 2001 (TG) declared TDistance() and SGNdistance() inline, renamed all
//                  distance() functions which calculate spatial distances to Sdistance()
// 02 Okt 2001 (TG) added SGNVVdistance()
//
// *****************************************************************************
#include "smatrix/SVector.hh"
#include "smatrix/Functions.hh"
#include "smatrix/SVertex.hh"
#include "smatrix/BinaryOperators.hh"

/** Spatial distance between two space points.
    \begin{displaymath}
    d = |\vec{x}_1 - \vec{x}_2|
    \end{displaymath}
*/
//============================================================================
// distance: spatial distance between SpacePoints
//============================================================================
inline double Sdistance(const SVector<double,3>& x1, const SVector<double,3>& x2) {
  return mag(x1 - x2);
}

/** Spatial distance between two Vertex objects.
    \begin{displaymath}
    d = |\vec{v}_1 - \vec{v}_2|
    \end{displaymath}
*/
//============================================================================
// distance: spatial distance between Vertex objects
//============================================================================
inline double Sdistance(const Vertex& v1, const Vertex& v2) {
  return mag(v1.vpos() - v2.vpos());
}

/** Signed spatial distance between two Vertex objects.
    \begin{displaymath}
    d = |\vec{v}_1 - \vec{v}_2|\times\texttt{sgn}(v_{z,1} - v_{z,2})
    \end{displaymath}
*/
//============================================================================
// distance: spatial distance between Vertex objects
//============================================================================
inline double SGNVVdistance(const Vertex& v1, const Vertex& v2) {
  return mag(v1.vpos() - v2.vpos()) * sign(v1.vpos()[2] - v2.vpos()[2]);
}

/** Spatial distance between Vertex and space point.
    \begin{displaymath}
    d = |\vec{x}_v - \vec{x}|
    \end{displaymath}
*/
//============================================================================
// distance: spatial distance between Vertex and SpacePoint
//============================================================================
inline double Sdistance(const Vertex& v, const SVector<double,3>& x) {
  return mag(v.vpos() - x);
}

/** Clostest spatial distance between Track and Vertex object.
    \begin{displaymath}
    d = \frac{|(\vec{x}_t - \vec{x}_v) \times \vec{t}|}{|\vec{t}|}
    \end{displaymath}
    $g: \vec{x}_t + m\cdot\vec{t}$ track definition\\
    $\vec{x}_v$: vertex position
*/
//============================================================================
// distance: spatial distance between Track and Vertex
//============================================================================
inline double TVdistance(const Track& t, const Vertex& v) {
  return mag(cross(t.xvec()-v.vpos(), t.evec()));
}

/** Clostest spatial distance between two Track objects.
    \begin{eqnarray*}
    d\vec{x} = \vec{x}_2 - \vec{x}_1\\
    f_1 = d\vec{x}\cdot\frac{\vec{t}_1}{|\vec{t}_1|}\\
    f_2 = d\vec{x}\cdot\frac{\vec{t}_2}{|\vec{t}_2|}
    \end{eqnarray*}
    \begin{displaymath}
    d = \left| d\vec{x} + f_2\cdot\frac{\vec{t}_2}{|\vec{t}_2|} -
    f_1\cdot\frac{\vec{t}_1}{|\vec{t}_1|}\right|
    \end{displaymath}
    $g: \vec{x}_1 + m\cdot\vec{t}_1$ track definition 1st track\\
    $g: \vec{x}_2 + m'\cdot\vec{t}_2$ track definition 2nd track\\
*/
//============================================================================
// distance: spatial distance between Track and Track
//============================================================================
inline double Tdistance(const Track& t1, const Track& t2) {
  const double a  = t1.tx();
  const double b  = t1.ty();
  const double c  = 1;
  const double a1 = t2.tx();
  const double b1 = t2.ty();
  const double c1 = 1;

  const double det = square(a*b1 - b*a1) + square(b*c1 - c*b1) + square(c*a1 - a*c1);
  const SVector<double,3> dx = t2.xvec() - t1.xvec();
  // are tracks parallel?
  if(det==0) return mag(cross(dx,t1.evec()));

  const double det2 = dx[0]*(b*c1 - c*b1) + dx[1]*(c*a1 - a*c1) + dx[2]*(a*b1 - b*a1);
  
  return fabs(det2/sqrt(det));
}

/** Closest spatial distance between Track and Wire.
    \begin{displaymath}
    d = \frac{(\vec{t}_w\times\vec{t}_t) \cdot (\vec{x}_t -
    \vec{x}_w)}{|\vec{t}_w\times\vec{t}_t|^2}
    \end{displaymath}
    $g: \vec{x}_t + m\cdot\vec{t}_t$ track definition\\
    $w: \vec{x}_w + m'\cdot\vec{t}_w$ wire definition
*/
//============================================================================
// distance: spatial distance between Track and Wire
//============================================================================
//inline double Sdistance(const Track& t, const Wire& w) {
//  const SVector<double,3> cr = cross(w.dvec(),t.tvec());
//  return dot(cr, t.xvec() - w.xvec())/mag2(cr);
//}


/** Signed closest distance between Track and Vertex object.
    The sign is determined by the z component and tells whether the point
    of closest approach is upstream or downstream of the vertex position.
    \begin{displaymath}
    \rho = \frac{\vec{t}\cdot(\vec{x}_t-\vec{x}_v)}{|\vec{t}|}
    \end{displaymath}
    \begin{displaymath}
    d\vec{x} = \vec{x}_t - \rho\cdot\vec{t} - \vec{x}_v
    \end{displaymath}
    \begin{displaymath}
    d = \textrm{sgn}(d\vec{x}_z)\cdot|d\vec{x}|
    \end{displaymath}
    $g: \vec{x}_t + m\cdot\vec{t}$ track definition\\
    $\vec{x}_v$: vertex position\\
    $\vec{x}_t - \rho\cdot\vec{t}$: point of track closest to vertex
*/
//============================================================================
// SGNdistance: signed spatial distance between Track and Vertex
//============================================================================
inline double SGNdistance(const Track& t, const Vertex& v) {
  // dx = point of track closest to vertex - vertex position
  const SVector<double,3> dx = 
    t.xvec() - t.tvec() * dot(t.evec(), t.xvec()-v.vpos()) - v.vpos();
  return sign(dx[2]) * mag(dx);
}

/** $\chi^2$ distance between two tracks.
    Determined by Kalman filter vertex fit.
*/
//============================================================================
// Chi2distance: chi2 distance between two tracks
//============================================================================
inline double Chi2distance(const Track& t1, const Track& t2) {
  SVertex<2> vtx(t1,t2);
  if(vtx.findVertexVt() == true)
    return vtx.chi2();
  else
    return 1.e10;
}

/** $\chi^2$ distance between SVertex and space point.\\
    Assumption: SVertex has been fitted with findVertexVt().
*/
//============================================================================
// Chi2distance: chi2 distance between SVertex and Space Point
//============================================================================
template <unsigned int NTR>
inline double Chi2distance(const SVertex<NTR>& v, const SVector<double,3>& x) {
  return product(v.kalman(0).KCINV(),v.vposR()-x);
}

/** $\chi^2$ distance between two SVertex objects.\\
    Assumption: Both SVertex objects have been fitted with findVertexVt().
*/
//============================================================================
// Chi2distance: chi2 distance between two SVertex objects
//============================================================================
template <unsigned int NTR, unsigned int NTR2>
inline double Chi2distance(const SVertex<NTR>& v1, const SVertex<NTR2>& v2) {
  const SMatrix<double,3>& cinv1 = v1.kalman(0).KCINV();
  const SMatrix<double,3>& cinv2 = v2.kalman(0).KCINV();
  const SMatrix<double,3> Cmat = cinv1 + cinv2;

  if(Cmat.sinvert() == false) { return 1.e10; }

  const SVector<double,3> xav = Cmat * (cinv1*v1.vposR() + cinv2*v2.vposR());
  return product(cinv1, xav - v1.vposR()) + product(cinv2, xav - v2.vposR());
}
#endif
