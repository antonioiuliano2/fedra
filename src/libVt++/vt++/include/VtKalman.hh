#ifndef __VTKALMAN_HH
#define __VTKALMAN_HH
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
// Description: Kalman filter class
//
// changes:
// 21 Aug 2000 (TG) creation
// 12 Sep 2000 (TG) member functions ex(), ey(), ez(), E() added
// 12 Sep 2000 (TG) declared some member functions inline
// 13 Sep 2000 (TG) added evec(), tvec(), pvec() member functions
// 04 Okt 2000 (TG) renamed set_use_momentum(const bool use) to
//                  use_momentum(const bool use)
// 04 Okt 2000 (TG) moved use_momentum() member function from class Vertex to
//                  class Kalman - now momentum usage can be set for tracks individually
// 10 Okt 2000 (TG) member G is not a reference to inv. track cov matrix any
//                  more but a full copy. Modified use_momentum member function.
//                  This fixes the bug that even with k_use_momentum = false
//                  the momentum correlated cov. matrix elements influenced the
//                  vertex fit.
// 11 Okt 2000 (TG) init() member function added
// 18 Okt 2000 (TG) k_alpc member added
// 18 Okt 2000 (TG) alpc(), alp(), alpc_init(), alp_init(), xn(), yn(), zn(),
//                  erg(), calc_ealpc() added
// 23 Okt 2000 (TG) k_erg member added, renamed k_ealpc to k_nalpc, nalpc(),
//                  qvs_nc() added
// 05 Okt 2001 (TG) bug fix: added CINTOBJECT to get cintdict compilation right
// 16 Jan 2002 (TG) moved inlines to VtKalman.icc
//
// *****************************************************************************

#include <cmath>
#include "CMatrix.hh"
#include "VtVector.hh"

#ifdef WIN32
#   include "Rtypes.h"
#endif

#if defined USE_ROOT && !defined __CINT__
#include "smatrix/SMatrix.hh"
#include "smatrix/SVector.hh"
#endif

namespace VERTEX {

  class Relation;

  /** This class contains the Vt Kalman filter matrices and vectors. Most
      of the member functions are for internal use only.
      @memo Kalman filter class
   */
  //============================================================================
  // Class Kalman - data structure for Kalman filtering
  //============================================================================
  class Kalman {
  public:
    /**@name --- Constructors --- */
    ///
    Kalman(const Relation* const relation);
    ///
    ~Kalman();

    /** @name --- Access methods --- */
    /// $\chi^2$ contribution of track
    double chi2()  const;
    /// smoothed $\chi^2$
    double chi2s() const;
    /// refitted $t_x$
    double tx()    const;
    /// refitted $t_y$
    double ty()    const;
    /// refitted $p$
    double p()     const;
    /// refitted $p_x$
    double px()    const;
    /// refitted $p_y$
    double py()    const;
    /// refitted $p_z$
    double pz()    const;
    /// refitted $e_x$
    double ex()    const;
    /// refitted $e_y$
    double ey()    const;
    /// refitted $e_z$
    double ez()    const;
    /// refitted Energy $E = \sqrt{p^2 + m^2}$
    double E(double rm = 0.) const;

#if defined USE_ROOT && !defined CINTOBJECT
    /// $\vec{v} = (t_x,t_y,1.)$ refitted slope vector
    SVector<double,3> tvec() const;
    /// $\vec{v} = (e_x,e_y,e_z)$ unit vector along refitted track
    SVector<double,3> evec() const;
    /// $\vec{v} = (p_x,p_y,p_z)$ refitted mom. vector
    SVector<double,3> pvec() const;
#else
    /// $\vec{v} = (e_x,e_y,e_z)$ unit vector along refitted track
    MATRIX::VtVector evec() const;
    /// $\vec{v} = (t_x,t_y,1.)$ refitted slope vector
    MATRIX::VtVector tvec() const;
    /// $\vec{v} = (p_x,p_y,p_z)$ refitted mom. vector
    MATRIX::VtVector pvec() const;
#endif

    /** @name --- Cov. matrices for internal use --- */
    ///
    const MATRIX::VtSymMatrix&     W() const;
    ///
    const MATRIX::VtSymMatrix&     C() const;
    ///
    const MATRIX::VtSymMatrix&  CINV() const;
    ///
    const MATRIX::VtSqMatrix&      F() const;
    ///
    const MATRIX::VtSqMatrix&     ES() const;
    ///
    const MATRIX::VtSymMatrix&    DS() const;
    ///
    const MATRIX::VtVector&       xv() const;
    ///
    const MATRIX::VtVector&      xnk() const;
    /// $\vec{v} = (t_x,t_y,p)$
    const MATRIX::VtVector&      qvs() const;

    /** @name --- Expert functions --- */
    /// set if track momentum should be used in fit
    void  use_momentum(const bool use);
    ///
    bool  use_momentum() const;
    ///
    double set_chi2(const double chi2);


    /// initialize the kalman structure
    void init();
    /// called by operator<<()
    std::ostream& print( std::ostream& os ) const;

    ///
    bool                      filter(double z,
				     const MATRIX::VtSymMatrix& prCINV,
				     const MATRIX::VtVector& prkal_xv);
    ///
    double               filter_chi2(double z,
				     double prChi2,
				     const MATRIX::VtSymMatrix& prCINV,
				     const MATRIX::VtVector& prkal_xv);
    ///
    bool              inverse_filter(double z, 
				     const MATRIX::VtSymMatrix& CINVn,
				     const MATRIX::VtSymMatrix& prCINV,
				     const MATRIX::VtVector& kal_xvn);
    ///
    void                      smooth(double z,
				     const MATRIX::VtVector& xvs,
				     const MATRIX::VtSymMatrix& Cn);
    ///
    const MATRIX::VtVector   calc_dp(double z,
				     const MATRIX::VtVector& xk, 
				     const MATRIX::VtVector& qk) const;
    ///
    const MATRIX::VtVector calc_pcAx(const MATRIX::VtVector& xk) const;
    ///
    const MATRIX::VtVector calc_AGpc(void) const;
    ///
    const MATRIX::VtVector   calc_qk(const MATRIX::VtVector& xk) const;
    ///
    void                    calc_qvs(const MATRIX::VtVector& xvs);
    ///
    double                calc_dchi2(double z,
				     const MATRIX::VtSymMatrix& prCINV,
				     const MATRIX::VtVector& xk,
				     const MATRIX::VtVector& prxk,
				     const MATRIX::VtVector& qk) const;
    ///
    void                     calc_pc(double z);

    /** @name --- Mass constraint fit functions --- */
    /// fill $\vec{\alpha}_c^{(0)}$
    void alpc_init(void);
    /// fill $\vec{\alpha}^{(0)}$
    void alp_init(void);
    /// calculate unit vector $\vec{\alpha}_c/|\vec{\alpha}_c|$, energy from alpc
    void calc_ealpc(void);
    /// state vector $\vec{\alpha_c}=(t_x,t_y,p)$
    const MATRIX::VtVector& alpc() const;
    /// state vector $\vec{\alpha}=(t_x,t_y,p)$
    const MATRIX::VtVector& alp()  const;
    /// $n_x = t_x * e_z$
    double xn()  const;
    /// $n_y = t_y * e_z$
    double yn()  const;
    /// $n_z = 1/\sqrt{1 + t_x^2 + t_y^2}$
    double zn()  const;
    /// $E_i=\sqrt{m^2 + p^2}$, $m$ = track rest-mass
    double erg() const;
    /// $\vec{n} = (n_x,n_y,n_z)$
    const MATRIX::VtVector& nalpc() const;
    /// return non-const reference to qvs
    MATRIX::VtVector& qvs_nc();

  private:
    const Relation*     rel;    // relation to Track & Vertex
    MATRIX::CMatrix     G;      // copy of inverse track cov. Matrix
    bool                k_use_momentum;
    double              k_tx;   // in Vt: Ak33(iv,k) = -qvs(1,..) before filter step
    double              k_ty;   // in Vt: Ak34(iv,k) = -qvs(2,..) before filter step
    MATRIX::VtVector    k_qvs;  // smoothed tx,ty,p (in Vt: qvs(1..3,..))
    MATRIX::VtVector    k_xv;   // in Vt: xv(..)
                                // xv(1,..) = kal_xv[0] = kalman vertex x - pos
                                // xv(2,..) = kal_xv[1] = kalman vertex y - pos
                                // xv(3,..) = kal_xv[2] = kalman vertex z - pos
    MATRIX::VtVector    k_qv;   // filtered tx,ty,p (in Vt: qv(1..3,..))
    MATRIX::VtVector    k_pc;   // vector p_k-c_k^(0) (in Vt: pc)
    MATRIX::VtVector    k_xnk;  // inverse kalman vertex position (in Vt: xnk)
    MATRIX::VtSymMatrix k_W;    // 3x3 matrix of filtered (tx,ty,p) cov. values
    MATRIX::VtMatrix    k_GB;   // 5x3 matrix GB = G_k*B_k (in Vt: AGB)
    MATRIX::VtMatrix    k_WBG;  // 3x5 matrix WBG = W_k*B_k^T*G_k (in Vt: WBG)
    MATRIX::CMatrix     k_Gb;   // 5x5 Gb = G - GBWBG (in Vt: GB, G^B)
    MATRIX::VtSymMatrix k_C;    // 3x3 C_k: contribution to vertex covariance matrix
    MATRIX::VtSymMatrix k_CINV; // 3x3 inverse C_k = (C_k-1^-1 + A^T_kG^B_kA_k)^-1
    MATRIX::VtSqMatrix  k_F;    // 
    MATRIX::VtSqMatrix  k_ES;   // track-vertex correlation matrix
    MATRIX::VtSymMatrix k_DS;   // track param. covariance matrix
    MATRIX::VtVector    k_alpc; // (in Vt: alpc): (tx,ty,p) for mass constraint fit
    MATRIX::VtVector    k_alp;  // (in Vt: alp)
    MATRIX::VtVector    k_nalpc;// (in Vt: xn,yn,zn)
    double              k_erg;  // (in Vt: erg)
    double              k_chi2; // (in Vt: chi^2_k) chi2 contrib. to vertex
    double              k_chi2s;// (in Vt: chi^2_k,S) smoothed chi2 contrib. to vertex

#ifdef WIN32
  ClassDef(Kalman,0)
#endif
  };

  //==============================================================================
  // operator <<
  //==============================================================================
  std::ostream& operator<< ( std::ostream& os, const Kalman& k );

} // end of namespace VERTEX

#include "VtKalman.icc"
#endif
