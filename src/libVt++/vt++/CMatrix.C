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
//              Attention: Consturctor makes transformation
//              sigma(Q/p) -> sigma(p) (a Vt-requirement).
//
// changes:
// 21 Aug 2000 (TG) creation
// 06 Sep 2000 (TG) include VtUtil.hh added
// 06 Sep 2000 (TG) transform sigma(Q/p) into sigma(p) already in constructor
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructor code for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 20 Nov 2000 (TG) added using namespace CLUE
// 24 Nov 2000 (TG) removed using namespace CLUE
// 24 Jan 2001 (TG) removed obsolete include directives
// 04 Sep 2001 (TG) SMatrix CTOR added
// 13 Sep 2001 (TG) RecoSegment/RecoTrack CTOR: changed cov-matrix calls
// 17 Okt 2001 (TG) added/improved #defines for ARTE
//
// *****************************************************************************

#include "Riostream.h"
#include <assert.h>
#include "vt++/CMatrix.hh"
#include "vt++/VtUtil.hh"

using namespace VERTEX;

namespace MATRIX {
  //============================================================================
  // CMatrix Constructors
  //============================================================================
  CMatrix::CMatrix(const CMatrix& rhs):VtSymMatrix(rhs) {}

#if defined USE_ROOT
  CMatrix::CMatrix(const SMatrix<double,5>& rhs) :
    VtSymMatrix(5) {

    set_x      (rhs(0,0));
    set_x_y    (rhs(1,0));
    set_y      (rhs(1,1));
    set_x_tx   (rhs(0,2));
    set_y_tx   (rhs(1,2));
    set_tx     (rhs(2,2));
    set_x_ty   (rhs(0,3));
    set_y_ty   (rhs(1,3));
    set_tx_ty  (rhs(2,3));
    set_ty     (rhs(3,3));
    set_x_p    (rhs(0,4));
    set_y_p    (rhs(1,4));
    set_tx_p   (rhs(2,4));
    set_ty_p   (rhs(3,4));
    set_p      (rhs(4,4));
  }
#endif

  //============================================================================
  // CMatrix::propagate: propagate Matrix by dz
  //============================================================================
  void CMatrix::propagate(const double dz) {
    set_x      ( x_prop(dz)    );
    set_x_y    ( x_y_prop(dz)  );
    set_x_tx   ( x_tx_prop(dz) );
    set_x_ty   ( x_ty_prop(dz) );
    set_x_p    ( x_p_prop(dz)  );
    set_y      ( y_prop(dz)    );
    set_y_tx   ( y_tx_prop(dz) );
    set_y_ty   ( y_ty_prop(dz) );
    set_y_p    ( y_p_prop(dz)  );

    // tx     is invariant
    // tx_ty  is invariant
    // ty     is invariant
    // tx_p   is invariant
    // ty_p   is invariant
    // p      is invariant
    return;
  }

} // namespace MATRIX
