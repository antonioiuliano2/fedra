#ifndef __CMATRIX_HH
#define __CMATRIX_HH
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
// Description: HERA-B track covariance matrix class
//              Attention: Contructor makes transformation
//              sigma(Q/p) -> sigma(p) (a Vt-requirement).
//
// changes:
// 21 Aug 2000 (TG) creation
// 25 Aug 2000 (TG) replaced sgn() function by that from VtUtil.hh
// 06 Aug 2000 (TG) changed sigma(1/p) into sigma(p), renamed member functions
//                  x_pinv, y_pin, tx_pinv, ty_pinv, pinv into x_p, y_p, tx_p...
// 06 Aug 2000 (TG) removed tx_pinv_prop, ty_pinv_prop, pinv_prop
// 06 Aug 2000 (TG) propagate() needs now only one argument (dz).
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructors for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 20 Nov 2000 (TG) added #include cluefwd.hh
// 04 Sep 2001 (TG) SMatrix CTOR added
// 17 Okt 2001 (TG) added/improved #defines for ARTE
//
// *****************************************************************************

#include "VtSymMatrix.hh"

//template <class T> class ArtePointer;
#if defined USE_ROOT
//#include "clue/cluefwd.hh"
#ifndef __CINT__
#include "smatrix/SMatrix.hh"
#endif
#endif

namespace MATRIX {
  class VtVector;

  //  template <class T>
  //  inline const double sgn(const T& x) { return (x==0)? 0. : (x<0)? -1. : 1.; }
 
  
  /** $5\times 5$ symmetric matrix class
      @memo HERA-B Track covariance matrix class
  */
  //============================================================================
  // Class CMatrix: Covariance Matrix class (symmetric,pos definite, dim=5) 
  //============================================================================
  class CMatrix : public VtSymMatrix {
  public:
    /** @name --- Constructors --- */
    ///
    CMatrix() : VtSymMatrix(5) {}
    ///
    CMatrix(const CMatrix& rhs);
    ///
    CMatrix(const VtSymMatrix& rhs) : VtSymMatrix(rhs) {}
    ///
    CMatrix(const VtSqMatrix& rhs)  : VtSymMatrix(rhs) {}
    ///
    CMatrix(const VtMatrix& rhs)    : VtSymMatrix(rhs) {}
#if defined USE_ROOT && !defined __CINT__
    /// not available in CINT
    CMatrix(const SMatrix<double,5>& rhs);
#endif

    /** @name --- Access methods --- */
    /// rtra->cvf[0]
    double x()       const { return operator()(0,0); }
    /// rtra->cvf[1]
    double x_y()     const { return operator()(0,1); }
    /// rtra->cvf[3]
    double x_tx()    const { return operator()(0,2); }
    /// rtra->cvf[6]
    double x_ty()    const { return operator()(0,3); }
    /// rtra->cvf[10] $* Q * -p^2$
    double x_p()     const { return operator()(0,4); }
    /// rtra->cvf[2]
    double y()       const { return operator()(1,1); }
    /// rtra->cvf[4]
    double y_tx()    const { return operator()(1,2); }
    /// rtra->cvf[7]
    double y_ty()    const { return operator()(1,3); }
    /// rtra->cvf[11] $* Q * -p^2$
    double y_p()     const { return operator()(1,4); }
    /// rtra->cvf[5]
    double tx()      const { return operator()(2,2); }
    /// rtra->cvf[8]
    double tx_ty()   const { return operator()(2,3); }
    /// rtra->cvf[12] $* Q * -p^2$
    double tx_p()    const { return operator()(2,4); }
    /// rtra->cvf[9]
    double ty()      const { return operator()(3,3); }
    /// rtra->cvf[13] $* Q * -p^2$
    double ty_p()    const { return operator()(3,4); }
    /// rtra->cvf[14] $* p^4$
    double p()       const { return operator()(4,4); }

    /** @name --- Modification of matrix elements --- */
    ///
    void set_x      (const double x)    { operator()(0,0)                   = x;   }
    ///										   
    void set_x_y    (const double xy)   { operator()(1,0) = operator()(0,1) = xy;  }
    ///										   
    void set_x_tx   (const double xtx)  { operator()(0,2) = operator()(2,0) = xtx; }
    ///										   
    void set_x_ty   (const double xty)  { operator()(0,3) = operator()(3,0) = xty; }
    ///										   
    void set_x_p    (const double xp)   { operator()(0,4) = operator()(4,0) = xp;  }
    ///										   
    void set_y      (const double y)    { operator()(1,1)                   = y;   }
    ///										   
    void set_y_tx   (const double ytx)  { operator()(1,2) = operator()(2,1) = ytx; }
    ///										   
    void set_y_ty   (const double yty)  { operator()(1,3) = operator()(3,1) = yty; }
    ///										   
    void set_y_p    (const double yp)   { operator()(1,4) = operator()(4,1) = yp;  }
    ///										   
    void set_tx     (const double tx)   { operator()(2,2)                   = tx;  }
    ///										   
    void set_tx_ty  (const double txty) { operator()(2,3) = operator()(3,2) = txty;}
    ///										   
    void set_tx_p   (const double txp)  { operator()(2,4) = operator()(4,2) = txp; }
    ///										   
    void set_ty     (const double ty)   { operator()(3,3)                   = ty;  }
    ///										   
    void set_ty_p   (const double typ)  { operator()(3,4) = operator()(4,3) = typ; }
    ///										   
    void set_p      (const double p)    { operator()(4,4)                   = p;   }

    /** @name --- Error propagation --- */
    ///
    double x_prop      (double dz) const { return x() + 2. * dz * x_tx() + dz*dz * tx(); }
    ///
    double x_y_prop    (double dz) const { return x_y() + dz*(y_tx() + x_ty() + dz * tx_ty()); }
    ///
    double x_tx_prop   (double dz) const { return x_tx() + dz * tx(); }
    ///
    double x_ty_prop   (double dz) const { return x_ty() + dz * tx_ty(); }
    ///
    double x_p_prop    (double dz) const { return x_p() + dz * tx_p(); }
    ///
    double y_prop      (double dz) const { return y() + 2. * dz * y_ty() + dz*dz * ty(); }
    ///
    double y_tx_prop   (double dz) const { return y_tx() + dz * tx_ty(); }
    ///
    double y_ty_prop   (double dz) const { return y_ty() + dz * ty(); }
    ///
    double y_p_prop    (double dz) const { return y_p() + dz * ty_p(); }

    /// propagate all matrix elements
    void propagate(const double dz);

  }; // class CMatrix


} // namespace MATRIX
#endif
