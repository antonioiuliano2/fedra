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
// 10 Okt 2000 (TG) modified use_momentum member function
// 11 Okt 2000 (TG) added code for init() member function, modified constructor
// 16 Jan 2002 (TG) changed print() return type
//
// *****************************************************************************
#include <iostream>
#include "vt++/VtTrack.hh"
#include "vt++/VtVertex.hh"
#include "vt++/VtKalman.hh"
#include "vt++/VtNegMatrix.hh"
#include "vt++/VtRelation.hh"

using namespace MATRIX;

namespace VERTEX {

  //============================================================================
  // Kalman::Constructor
  //============================================================================
  Kalman::Kalman(const Relation* const relation):
    rel(relation),
    G(rel->track.G()),
    k_use_momentum(true),
    k_tx    (0.),
    k_ty    (0.),
    k_qvs   (MATRIX::VtVector(3)),
    k_xv    (MATRIX::VtVector(3)),
    k_qv    (MATRIX::VtVector(3)),
    k_pc    (MATRIX::VtVector(5)),
    k_xnk   (MATRIX::VtVector(3)),
    k_W     (MATRIX::VtSymMatrix(3)),
    k_GB    (MATRIX::VtMatrix(5,3)),
    k_WBG   (MATRIX::VtMatrix(3,5)),
    k_C     (MATRIX::VtSymMatrix(3)),
    k_CINV  (MATRIX::VtSymMatrix(3)),
    k_F     (MATRIX::VtSqMatrix(3)),
    k_ES    (MATRIX::VtSqMatrix(3)),
    k_DS    (MATRIX::VtSymMatrix(3)),
    k_alpc  (MATRIX::VtVector(3)),
    k_alp   (MATRIX::VtVector(3)),
    k_nalpc (MATRIX::VtVector(3)),
    k_erg   (0.),
    k_chi2  (0.),
    k_chi2s (0.)
    {}

  Kalman::~Kalman() {}

  //============================================================================
  // Kalman::print
  //============================================================================
  std::ostream& Kalman::print( std::ostream& os ) const {
    return os << k_C;
  }


  //============================================================================
  // Kalman::init
  //============================================================================
  void Kalman::init() {
    k_tx   = rel->track.tx();
    k_ty   = rel->track.ty();
    k_qvs  = VtVector(rel->track.tx(),rel->track.ty(),rel->track.p());
    k_qv   = k_qvs;
    k_xv   = rel->vertex.xvs();
    k_C    = rel->vertex.CS();
    k_CINV = k_C.dsinv();
    k_chi2 = k_chi2s = 0.;
    
    k_GB.clear();

    return;
  }

  //============================================================================
  // Kalman::use_momentum
  //============================================================================
  void Kalman::use_momentum(const bool use) {
    k_use_momentum = use;
    // select proper inverse cov. matrix
    if(k_use_momentum)
      G = rel->track.G();
    else
      G = rel->track.GM();
    return;
  }

  //============================================================================
  // Kalman::calc_AGpc compute Vector (A^T*G^B) * pc (in Vt: AGpc)
  //============================================================================
  const VtVector Kalman::calc_AGpc(void) const {
    VtVector AGpc(3);
    
    // row 1 + 2
    for(unsigned int i=0; i<2; ++i) {
      for(unsigned int j=0; j < k_Gb.ncol(); ++j) {
	AGpc[i] += k_Gb(i,j) * k_pc[j];
      }
    }
    // row 3
    for(unsigned int j=0; j < k_Gb.ncol(); ++j) {
      AGpc[2] += (-k_tx * k_Gb(0,j) - k_ty * k_Gb(1,j)) * k_pc[j];
    }

    return AGpc;
  }

  //============================================================================
  // Kalman::calc_dp calculate sub-expression (in Vt: dp)
  //============================================================================
  const VtVector Kalman::calc_dp(double z,
				 const VtVector& xk, 
				 const VtVector& qk) const {
    
    // compute p_k - c_k^(0) - A_k*x_k - B_k * q_k (in Vt: dp)
    VtVector dp = calc_pcAx(xk);
    dp[0] += z * qk[0];
    dp[1] += z * qk[1];
    dp[2] -=     qk[0];
    dp[3] -=     qk[1];
    
    if(k_use_momentum) {
      dp[4] -= qk[2];
    }
    
#ifdef VtDEBUG
    cout << "Vector dp: " << dp << endl;
    cout << " z: " << z << endl;
    cout << "Vector qk: " << qk << endl;
#endif
    
    return dp;
  }
  
  //============================================================================
  // Kalman::calc_pcAx compute p_k-c_k^(0) - A_k*x_k  (in Vt: pcAx)
  //============================================================================
  const VtVector Kalman::calc_pcAx(const VtVector& xk) const {
    
    VtVector pcAx(5);
    pcAx[0] = k_pc[0] - xk[0] + k_tx * xk[2];
    pcAx[1] = k_pc[1] - xk[1] + k_ty * xk[2];
    pcAx[2] = k_pc[2];
    pcAx[3] = k_pc[3];
    
    if(k_use_momentum)
      pcAx[4] = k_pc[4];
    
#ifdef VtDEBUG
    cout << " xvs[1]: " << xk[0] 
	 << " xvs[2]: " << xk[1]
	 << " xvs[3]: " << xk[2]
	 << " a33: " << k_tx
	 << " a34: " << k_ty
	 << endl;
    cout << "Vector pc: " << k_pc << endl;
    cout << "Vector pcAx: " << pcAx << endl;
#endif
    return pcAx;
  }

  //============================================================================
  // Kalman::calc_qk calculcate smoothed track parameters (in Vt: qvs)
  //============================================================================
  const VtVector Kalman::calc_qk(const VtVector& xk) const {
    
    // compute p_k-c_k^(0) - A_k*x_k
    VtVector pcAx = calc_pcAx(xk);

    // compute q_k^n = W_k*B_k^T*G_k * (p_k - c_k^(0) - A_k*x_n) (in Vt: qvs)
    const VtVector qk = k_WBG * pcAx;
    
#ifdef VtDEBUG
    cout << "Vector qk: " << qk << endl;
#endif
    
    return qk;
  }

  //============================================================================
  // Kalman::calc_qvs compute q_k^n = W_k*B_k^T*G_k * (p_k - c_k^(0) - A_k*x_n)
  //============================================================================
  void Kalman::calc_qvs(const VtVector& xvs) {
    
    // compute q_k^n = W_k*B_k^T*G_k * (p_k - c_k^(0) - A_k*x_n) (in Vt: qvs)
    k_qvs = calc_qk(xvs);
    return;
  }

  //============================================================================
  // Kalman::calc_pc compute p_k - c_k^(0) (in Vt: pc)
  //============================================================================
  void Kalman::calc_pc(double z) {
    
    const Track& t = rel->track;

    // compute vector p_k - c_k^(0) (in Vt: pc)
    k_pc[0] = t.x() - z * k_tx;
    k_pc[1] = t.y() - z * k_ty;
    k_pc[2] = t.tx();
    k_pc[3] = t.ty();
    
    if(k_use_momentum)
      k_pc[4] = t.p();
    
#ifdef VtDEBUG
    cout << "Vector k_pc: " << k_pc << endl;
#endif

    return;
  }

  //============================================================================
  // Kalman::calc_chi2: compute expression (eq. 14/eq.23)
  //============================================================================
  double Kalman::calc_dchi2(double z,
			    const VtSymMatrix& prCINV,
			    const VtVector& xk,
			    const VtVector& prxk,
			    const VtVector& qk) const {
    
    // compute vector x_k - x_k-1
    const VtVector dx = xk - prxk;

#ifdef VtDEBUG
    cout << "Vector prxk: " << prxk << endl; 
    cout << "Vector dx: " << dx << endl;
#endif

    // compute vector p_k - c_k^(0) - A_k*x_k - B_k*q_k (eq. 14)
    VtVector dp = calc_dp(z, xk, qk);

    unsigned int dim = 4;
    if(k_use_momentum) {
      dim = 5;
    }

    // calculate dp^T * G_k * dp and add it to chi2 (in Vt: chi2f)
    double chi2 = G.product(dp, dim);
    
#ifdef VtDEBUG
    cout << "chi2 1st term: " << chi2 << endl;
#endif
    
    // compute term (x_k-x_k-1)^T * C_k-1^-1 * (x_k-x_k-1)
    double chi2b = prCINV.product(dx);
    
#ifdef VtDEBUG
    cout << "chi2 2nd term: " << chi2b << endl;
#endif

    return chi2 + chi2b;
  } // calc_dchi2

  //============================================================================
  // Kalman::filter: filter the Covariance Matrix at estimated Vertex-z (result is W)
  //============================================================================
  bool Kalman::filter(double z, 
		      const VtSymMatrix& prCINV,
		      const VtVector& prkal_xv) {
    // task: propagate inverse cov. Matrix to estimated z of Vertex
    // and project it into (tx,ty,p) space. Finally invert the matrix.
    // z = -bk13 in Vt package

    const double z2 = z*z;
#ifdef VtDEBUG
    cout << " z: " << z << endl;
#endif

    // save tx,ty at begin of filter step (needed in case
    // of refit by track removal from vertex)
    k_tx = k_qvs[0];
    k_ty = k_qvs[1];

#ifdef VtDEBUG
    cout << "k_tx: " << k_tx << " k_ty: " << k_ty << endl;
#endif

    // compute Matrix W = (B^T*G*B)^-1
    k_W(0,0) = z2 * G(0,0) - 2. * z * G(0,2) + G(2,2);
    k_W(1,1) = z2 * G(1,1) - 2. * z * G(1,3) + G(3,3);
    k_W(0,1) = 
      k_W(1,0) = z2 * G(1,0) - z * ( G(1,2) + G(3,0) ) + G(3,2);

    if(k_use_momentum) {
      k_W(0,2) = k_W(2,0) = G(4,2) - z * G(4,0);
      k_W(1,2) = k_W(2,1) = G(4,3) - z * G(4,1);
      k_W(2,2) = G(4,4);
    }
    
#ifdef VtDEBUG
    cout << "Matrix G: " << G << endl;
    cout << "Matrix k_W: " << k_W << endl;
#endif

    if(k_W.invert(k_use_momentum) == false)
      return false;

#ifdef VtDEBUG
    cout << "Matrix inverted k_W: " << k_W << endl;
#endif

    // compute Matrix G_k*B_k = B_k^T*G_k (in Vt: AGB)
    for(unsigned int i=0; i < G.nrow(); ++i) {
      k_GB(i,0) = G(i,2) - z * G(i,0);
      k_GB(i,1) = G(i,3) - z * G(i,1);
    }
    if(k_use_momentum) {
      for(unsigned int i=0; i < G.nrow(); ++i)
	k_GB(i,2) = G(i,4);
    }

#ifdef VtDEBUG
    cout << "Matrix k_GB: " << k_GB << endl;
#endif

    // compute Matrix W*(GB)^T = W*B^T*G (in Vt: WBG)
    k_WBG = k_W * k_GB.T();

#ifdef VtDEBUG
    cout << "Matrix k_WBG: " << k_WBG << endl;
#endif

    // compute Matrix (G*B) * (W*B^T*G)  (in Vt: GBWBG)
    const VtSymMatrix GBWBG = k_GB * k_WBG;

#ifdef VtDEBUG
    cout << "Matrix GBWBG: " << GBWBG << endl;
#endif

    // compute Matrix G^B = G - GBWB^TG (in Vt: GB, eq. 10)
    k_Gb = G - GBWBG;

#ifdef VtDEBUG
    cout << "Matrix G: " << G;
    cout << "Matrix k_Gb: " << k_Gb;
#endif

    // compute Matrix A*G^B*A (in Vt: AGA)
    VtSymMatrix AGA(3);
    AGA(0,0)            = k_Gb(0,0);
    AGA(0,1) = AGA(1,0) = k_Gb(0,1);
    AGA(1,1)            = k_Gb(1,1);
    AGA(0,2) = AGA(2,0) = -k_tx*k_Gb(0,0) - k_ty*k_Gb(1,0);
    AGA(1,2) = AGA(2,1) = -k_tx*k_Gb(0,1) - k_ty*k_Gb(1,1);
    AGA(2,2) = k_tx*k_tx*k_Gb(0,0) + 2.*k_tx*k_ty*k_Gb(0,1) +
      k_ty*k_ty*k_Gb(1,1);
    
#ifdef VtDEBUG
    cout << "Matrix AGA: " << AGA << endl;
    cout << "Matrix prCINV: " << prCINV << endl;
#endif

    // compute k_C, in Vt: C_k = (C_k-1^-1 + A^T_k G^B_k A_k)^-1 (eq. 10)
    k_CINV = prCINV + AGA;

#ifdef VtDEBUG
    cout << "Matrix k_CINV: " << k_CINV << endl;
#endif

    k_C = k_CINV;
    if( k_C.VtDsinv() == false )
      return false; // track does not fit onto vertex

#ifdef VtDEBUG
    cout << "Matrix k_C: " << k_C << endl;
    cout << "k_C*k_CINV: " << k_C*k_CINV << endl;
#endif

    // --------> Determine filtered state vector <-----------

    // compute equation 12 (see Vt docu)
    // compute vector p_k - c_k^(0) (in Vt: pc)
    calc_pc(z);

    // compute Vector (A^T*G^B) * pc (in Vt: AGpc)
    const VtVector AGpc = calc_AGpc();

#ifdef VtDEBUG
    cout << "Vector AGpc: " << AGpc << endl;
#endif

    
    // compute x_k = C_k * [C_k-1^-1*x_k-1 + A^TG^B (p_k - c_k^(0))] (eq. 12)
    // Cx = expression in brackets [...]
    const VtVector Cx = AGpc + prCINV * prkal_xv;

#ifdef VtDEBUG
    cout << "prkal_xv: " << prkal_xv << endl;
    cout << "Vector Cx: " << Cx << endl;
#endif

    // compute k_xv (in Vt: x_k): filtered vtx position
    k_xv = k_C * Cx;

#ifdef VtDEBUG
    cout << "Vector k_xv: " << k_xv << endl;
#endif
    
    // compute equation 13
    // compute q_k = W_kB^T_kG_k * [p_k - c_k^(0) - A_k*x_k] (in Vt: qv)
    k_qv = calc_qk(k_xv);

#ifdef VtDEBUG
    cout << "Vector k_qv: " << k_qv << endl;
#endif

    return true;
  } // filter

  //============================================================================
  // Kalman::filter_chi2: filter the chi2 contribution of the track (eq. 14)
  //============================================================================
  double Kalman::filter_chi2(double z,
			     double prChi2,
			     const VtSymMatrix& prCINV,
			     const VtVector& prkal_xv) {
    
    k_chi2 = prChi2 + calc_dchi2(z, prCINV, k_xv, prkal_xv, k_qv);

#ifdef VtDEBUG
    cout << "k_chi2: " << k_chi2 << endl;
#endif
    return k_chi2;
  } // filter_chi2

  //============================================================================
  // Kalman::smooth: smooth track params (eq. 17, 18, 19)
  //============================================================================
  void Kalman::smooth(double z,
		      const VtVector& xvs,
		      const VtSymMatrix& Cn) {

    // compute q_k^n = W_k*B_k^T*G_k * (p_k - c_k^(0) - A_k*x_n) (in Vt: qvs)
    calc_qvs(xvs);

#ifdef VtDEBUG
    cout << "Vector k_qvs: " << k_qvs << endl;
    cout << "smooth: Matrix WBG:" << k_WBG << endl;
#endif
    
    // compute Matrix B_k^T*G*A_k (eq. 11)
    VtSqMatrix BGA(3);

    BGA(0,0) = G(2,0) - z * G(0,0);
    BGA(0,1) = G(2,1) - z * G(0,1);
    BGA(1,0) = G(3,0) - z * G(1,0);
    BGA(1,1) = G(3,1) - z * G(1,1);

    BGA(0,2) = -k_tx*BGA(0,0) - k_ty*BGA(0,1);
    BGA(1,2) = -k_tx*BGA(1,0) - k_ty*BGA(1,1);

    if(k_use_momentum) {
      BGA(2,0) = G(4,0);
      BGA(2,1) = G(4,1);
      BGA(2,2) = -k_tx*BGA(2,0) - k_ty*BGA(2,1);
    }

#ifdef VtDEBUG
    cout << "Matrix BGA: " << BGA << endl;
#endif

    // compute Matrix F_k = W_kB_k^TG_kA_k (eq. 11) (in Vt: F)
    k_F = k_W * BGA;
    
#ifdef VtDEBUG
    cout << "Matrix k_F: " << k_F << endl;
#endif

    // compute Matrix E_k^n = -F_kC_n (eq. 18) (in Vt: ES)
    k_ES = -k_F * Cn;

#ifdef VtDEBUG
    cout << "Marix k_ES: " << k_ES << endl;
#endif

    // compute Matrix D_k^n = W_k - E_k^n*F_k^T (eq. 18) (in Vt: DS)
    k_DS = k_W - k_ES * k_F.T();
    
#ifdef VtDEBUG
    cout << "Matrix k_DS: " << k_DS << endl;
#endif
    return;
  } // smooth

  //============================================================================
  // Kalman::inverse_filterX get new state vectors & cov. matric with a track removed
  //============================================================================
  bool Kalman::inverse_filter(double z, 
			      const VtSymMatrix& CINVn,
			      const VtSymMatrix& prCINV,
			      const VtVector& k_xvn) {
    
    // compute C_k^n* (eq. 20)
    // it is: C_k = (C_k-1^-1 + A_k^TG_k^BA_k)^-1
    // => A_kG_k^BA_k = C_k^-1 - C_k-1^-1
    // => C_k^n* = (C_n^-1 - (C_k^-1 - C_k-1^-1))^-1
    const VtSymMatrix CnkINV = CINVn - (k_CINV - prCINV);

#ifdef VtDEBUG
    cout << "CnkINV: " << CnkINV << endl;
#endif

    VtSymMatrix Cnk = CnkINV;
    if(Cnk.VtDsinv() == false)
      return false;

    // compute vector p_k - c_k^(0) (in Vt: pc)
    calc_pc(z);

    // compute Vector (A^T*G^B) * pc (in Vt: AGpc)
    const VtVector AGpc = calc_AGpc();

    // compute x_k^n* = C_k^n* * [C_n^-1*x_n - A_k^TG_k^B (p_k - c_k^(0))] (eq. 21)
    // Cx = expression in brackets [...]
    const VtVector Cx = CINVn * k_xvn - AGpc;

#ifdef VtDEBUG
    cout << "Vector Cx: " << Cx << endl;
#endif
    
    // in Vt: xnk, (eq. 21)
    k_xnk = Cnk * Cx;


    // ------------> inverse filter of vertex-chi2
    k_chi2s = calc_dchi2(z, prCINV, k_xvn, k_xnk, k_qvs);


    // compute x_n - x_k^n* (in Vt: dx)
    VtVector dx = k_xvn - k_xnk;

    // compute p_k - c_k^(0) - A_k*x_n - B_k * q_k^n (in Vt: dp)
    // q_k^n are the smoothed track parameters
    VtVector dp = calc_dp(z, k_xvn, k_qvs);

    unsigned int dim = 4;
    if(k_use_momentum) {
      dim = 5;
    }

    // compute dp^T*G_k*dp
    double chi2sr = G.product(dp,dim);

    // compute chi2_k,S = dp^T*G_k*dp + dx^T*(C_k^n*)^-1*dx (eq. 23)
    k_chi2s = CnkINV.product(dx) + chi2sr;

    return true;
  }

  //============================================================================
  // Kalman::calc_ealpc
  //============================================================================
  void Kalman::calc_ealpc(void){

    k_nalpc[2] = 1./sqrt(1. + sqr(k_alpc[0]) + sqr(k_alpc[1]));
    k_nalpc[0] = k_alpc[0] * k_nalpc[2];
    k_nalpc[1] = k_alpc[1] * k_nalpc[2];

    k_erg = sqrt(sqr(rel->track.rm()) + sqr(k_alpc[2]));
    return;
  }
} // end of namespace VERTEX
