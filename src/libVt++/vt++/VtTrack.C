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
// Description: Track class
//
// changes:
// 21 Aug 2000 (TG) creation
// 24 Aug 2000 (TG) include arte.hh header file
// 06 Sep 2000 (TG) changed call to t_V.propagate()
// 24 Sep 2000 (TG) delete_mom() member function added temporarily for
//                  debugging. Not for public use!!
// 10 Okt 2000 (TG) fill member t_GM in constructors
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructor code for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 20 Nov 2000 (TG) added using namespace CLUE
// 24 Nov 2000 (TG) removed using namespace CLUE
// 25 Nov 2000 (TG) code for RecoTrack constructor added
// 15 Dec 2000 (TG) bug fix in Track(ArtePointer<RecoTrack>) constructor
// 24 Feb 2001 (TG) operator= added, added restmass as default CTOR argument
// 04 Sep 2001 (TG) added TrackIf CTOR
// 17 Okt 2001 (TG) added/improved #defines for ARTE
// 09 Jan 2002 (TG) changed return type of propagate() to bool
//
// *****************************************************************************
#include "Riostream.h"
#include <iomanip>
//#include <cmath>

#include "vt++/VtTrack.hh"
#include "vt++/VtKalman.hh"
#include "vt++/VtRelation.hh"

#if defined USE_ROOT
#include "smatrix/SMatrix.hh"
#endif

ClassImp(VERTEX::Track);

using namespace std;
using namespace MATRIX;

namespace VERTEX {

#if defined USE_ROOT
  static const SMatrix<double,5> COV_;
#endif

  //============================================================================
  // Track::Constructors
  //============================================================================
  Track::Track():
    t_Q      (0),
    t_rm     (0.),
    t_rmCC   (0.),
    t_p      (VtVector(6))
  {}

  Track::Track(const Track* const track):
    t_Q      (track->t_Q),
    t_rm     (track->t_rm),
    t_rmCC   (track->t_rmCC),
    t_p      (track->t_p),
    t_V      (track->t_V)
  {
    propagate(0.);           // propagate track to z=0.
    t_G = t_V.dsinv();
    t_GM.copy(t_V.dsinv(4)); // inverse without momentum
  }

  Track::Track(const Track& rhs):
    t_Q      (rhs.t_Q),
    t_rm     (rhs.t_rm),
    t_rmCC   (rhs.t_rmCC),
    t_p      (rhs.t_p),
    t_V      (rhs.t_V)
  {
    propagate(0.);           // propagate track to z=0.
    t_G = t_V.dsinv();
    t_GM.copy(t_V.dsinv(4)); // inverse without momentum
  }

  Track::~Track() {}

#if defined USE_ROOT

  Track::Track(const VtVector& v, const MATRIX::CMatrix& c) :
    t_Q      (1),
    t_rm     (0.),
    t_rmCC   (0.),
    t_p      (v),
    t_V      (c) {
    propagate(0.);           // propagate track to z=0.
    t_G = t_V.dsinv();
    t_GM.copy(t_V.dsinv(4)); // inverse without momentum
  }
#endif

  void Track::set( double x,double y,double z,double tx,double ty,double p, 
	      const MATRIX::CMatrix& c) {
    t_p[0] = x;
    t_p[1] = y;
    t_p[2] = z;
    t_p[3] = tx;
    t_p[4] = ty;
    t_p[5] = p;
    t_V.copy(c);
    propagate(0.);           // propagate track to z=0.
    t_G = t_V.dsinv();
    t_GM.copy(t_V.dsinv(4)); // inverse without momentum
  }
  
  //============================================================================
  // Track::operator=
  //============================================================================
  Track& Track::operator=(const Track& rhs) {
    if(this == &rhs) { return *this; }

    RelationList::operator=(rhs);

    t_Q       = rhs.t_Q;
    t_rm      = rhs.t_rm;
    t_rmCC    = rhs.t_rmCC;

    t_p       = rhs.t_p;
    t_V       = rhs.t_V;
    t_G       = rhs.t_G;
    t_GM      = rhs.t_GM;

    return *this;
  }

  //============================================================================
  // TrackIf access methods
  //============================================================================
  float Track::x()        const { return t_p[0]; }
  float Track::y()        const { return t_p[1]; }
  float Track::z()        const { return t_p[2]; }
  float Track::x(float z) const { return t_p[0] + t_p[3] * (z-t_p[2]); }
  float Track::y(float z) const { return t_p[1] + t_p[4] * (z-t_p[2]); }
  float Track::tx()       const { return t_p[3]; }
  float Track::ty()       const { return t_p[4]; }
  float Track::p()        const { return t_p[5]; }
  float Track::pt()       const { return sqrt(sqr(px()) + sqr(py())); }
  float Track::pz()       const { return t_p[5] * ez(); }
  float Track::chi2() const     { return 0; }
  unsigned short int Track::ndf()  const { return 0; }
  float Track::phi()      const { return (180. + atan2(t_p[3],t_p[4])*57.29577957); }
  float Track::theta()    const { return acos(ez()); }
  float Track::eta()      const { return -log(tan(theta()/2.)); }
  int   Track::charge()   const { return t_Q; }
  float Track::energy(double rm) const { return sqrt(sqr(t_p[5]) + sqr(rm)); }
  float Track::xf(double mass)   const { return (pz() - energy(mass)*0.99898)/0.93827; }
  float Track::rap(double mass)  const { return 0.5*log((energy(mass)+pz())/(energy(mass)-pz())); }

#if defined USE_ROOT
  SVector<double,3> Track::xvec() const
    { return SVector<double,3>(t_p[0],t_p[1],t_p[2]); }
  SVector<double,3> Track::tvec() const
    { return SVector<double,3>(t_p[3],t_p[4],1); }
  SVector<double,3> Track::evec() const {
    return SVector<double,3>(1./mag(tvec()) * tvec());
  }
  SVector<double,3> Track::pvec() const {
    return SVector<double,3>(t_p[5]/mag(tvec()) * tvec());
  }
  const SMatrix<double,5>& Track::COV() const { return COV_; }

  const SMatrix<double,5>& Track::CINV() const { return COV_; }
#endif

  float Track::cov_x(double dz) const {
    return t_V.x_prop(dz);
  }

  float Track::cov_y(double dz) const {
    return t_V.y_prop(dz);
  }

  float Track::cov_tx()  const { return t_V(2,2); }
  float Track::cov_ty()  const { return t_V(3,3); }
  float Track::cov_p()   const { return t_V.p(); }
  bool  Track::isValid() const { return true; }
  void  Track::valid() { return; }
  void  Track::invalid() { return; }

#if defined USE_ROOT
  bool  Track::operator==( const Track& rhs ) const {
    return &rhs == this;
  }
#endif

  const MATRIX::CMatrix&     Track::V()  const { return t_V; }
  const MATRIX::CMatrix&     Track::G()  const { return t_G; }
  const MATRIX::CMatrix&     Track::GM() const { return t_GM; }


  //============================================================================
  // Track::print
  //============================================================================
  std::ostream& Track::print( std::ostream& os ) const {

    return os  << " x: "  << x()
	       << " y: "  << y()
	       << " z: "  << z()
	       << " tx: " << tx()
	       << " ty: " << ty()
	       << " p: "  << p() 
	       << endl
	       << " Cov Matrix: " << V()
	       << " inverse: " << G()
	       << " inverse without momentum: " << GM();
  }

  //============================================================================
  // Track::propagate propagate Track to z 
  //============================================================================
  bool Track::propagate(const double zz) {
    if( zz == t_p[2] ) return true;

    const double dz = zz - t_p[2];
    t_p[0] += dz * t_p[3];
    t_p[1] += dz * t_p[4];
    t_p[2]  = zz;
    t_V.propagate(dz); // propagate Track covariance matrix
    return true;
  }

  //============================================================================
  // Track::delete_mom for debugging only
  //============================================================================
  void Track::delete_mom() {
    t_V.set_x_p(0.);
    t_V.set_y_p(0.);
    t_V.set_tx_p(0.);
    t_V.set_ty_p(0.);
    t_V.set_p(1e5);
    t_G = t_V;
    if(t_G.VtDsinv()==false)
      cout << " error inverting matrix!!" << endl;
	
  }

} // namespace VERTEX
