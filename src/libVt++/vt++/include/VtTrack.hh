#ifndef __VTTRACK_HH
#define __VTTRACK_HH
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
// 24 Aug 2000 (TG) added ArtePointer declaration
// 04 Sep 2000 (TG) added pt(), theta(), phi(), eta() member function
// 05 Sep 2000 (TG) declared some member functions inline
// 06 Sep 2000 (TG) added xvec(), tvec(), pvec(), E() member functions
// 06 Sep 2000 (TG) added comments
// 12 Sep 2000 (TG) added ex(), ey(), ez(), evec(), rm() member functions
// 10 Okt 2000 (TG) data member t_GM and member function GM() added
// 26 Okt 2000 (TG) added #include <cmath>
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructors for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 25 Nov 2000 (TG) added RecoTrack constructor
// 24 Feb 2001 (TG) operator= added, added restmass as default CTOR argument
// 04 Sep 2001 (TG) added TrackIf CTOR
// 17 Okt 2001 (TG) added/improved #defines for ARTE
// 05 Okt 2001 (TG) bug fix: added CINTOBJECT to get cintdict compilation right
// 09 Jan 2002 (TG) changed return type of propagate() to bool
//
// *****************************************************************************

#include <iosfwd>
#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "vt++/VtRelationList.hh"

#if defined USE_ROOT
//#include "arte/artefwd.hh"
//#include "clue/cluefwd.hh"
//#include "interfaces/TrackIf.hh"

#ifndef __CINT__
#include "smatrix/SMatrix.hh"
#include "smatrix/SVector.hh"
#endif
#endif

namespace VERTEX {
  class Kalman;

  /** Track representation class
   */
  //============================================================================
  // Class Track
  //============================================================================
  class Track: public RelationList
#if defined USE_ROOT && !defined CINTOBJECT
//    , public TrackIf
#endif
 {
  public:
    /**@name --- Constructors --- */
    ///
    Track();
    ///
    Track(const Track* const track);
    ///
    Track(const Track& rhs);
    ///
    Track(const MATRIX::VtVector& v, const MATRIX::CMatrix& c);
    ///
    virtual ~Track();
#if defined USE_ROOT
    ///
//    Track(const TrackIf& rhs, double ms = 0., double msCC = 0.);
#endif
    ///
   void set( double x,double y,double z,double tx,double ty,double p, 
	const MATRIX::CMatrix& c);
    Track& operator=(const Track& rhs);

    /**@name --- TrackIf access methods --- */
    /// x at z=0 (in Vt: p(3,..))
    float x()      const;
    ///  y at z=0 (in Vt: p(4,..))
    float y()      const;
    /// Track $x$ position at $z$
    float x(float z)   const;
    /// Track $y$ position at $z$
    float y(float z)   const;
    /// z = 0  in Vt
    float z()      const;
    ///  slope    (in Vt: p(1,..))
    float tx()     const;
    ///  slope    (in Vt: p(2,..))
    float ty()     const;
    /// momentum (in Vt: p(5,..))
    float p()      const;
    /// transv. momentum $\sqrt{p_x^2 + p_y^2}$
    float pt()     const;
    /// $p \cdot e_z$
    float pz()     const;
    /// dummy function: always return 0
    float chi2() const;
    /// dummy function: always return 0
    unsigned short int ndf()  const;
    /// azimuthal angle $\phi$
    float phi()    const;
    /// polar angle $\theta = \cos^{-1}(e_z)$
    float theta()  const;
    /// rapidity $-\log\tan(\theta/2.)$
    float eta()    const;
    /// charge
    int   charge()   const;
    /// Energy with given rest-mass (in GeV) $E = \sqrt{p^2 + m^2}$
    float energy(double rm = 0.) const;
    /// Feynman Variable $x_F = \frac{E+p_z}{(E+p_z)_{max}}$
    float xf(double rm = 0.) const;
    /// Rapidity $y = \frac{1}{2}\ln(\frac{E+p_z}{E-p_z})$
    float rap(double rm = 0.) const;

#if defined USE_ROOT && !defined CINTOBJECT
    /// $\vec{v} = (x,y,z)$
    SVector<double,3> xvec() const;
    /// $\vec{v} = (t_x,t_y,1.)$ refitted slope vector
    SVector<double,3> tvec() const;
    /// $\vec{v} = (e_x,e_y,e_z)$ unit vector along refitted track
    SVector<double,3> evec() const;
    /// $\vec{v} = (p_x,p_y,p_z)$ refitted mom. vector
    SVector<double,3> pvec() const;
    /// dummy function: return 0 matrix
    const SMatrix<double,5>& COV() const;
    /// dummy function: return 0 matrix
    const SMatrix<double,5>& CINV() const;
#else
    /// $\vec{v} = (e_x,e_y,e_z)$ unit vector along refitted track
    inline MATRIX::VtVector evec() const { return MATRIX::VtVector(ex(),ey(),ez()); }
    /// $\vec{v} = (t_x,t_y,1.)$ refitted slope vector
    inline MATRIX::VtVector tvec() const { return MATRIX::VtVector(tx(),ty(),1.); }
    /// $\vec{v} = (p_x,p_y,p_z)$ refitted mom. vector
    inline MATRIX::VtVector pvec() const { return MATRIX::VtVector(px(),py(),pz()); }
#endif

    /// get $\sigma_x^2|_{z+dz}$
    float cov_x(double dz=0.)        const;
    /// get $\sigma_y^2|_{z+dz}$
    float cov_y(double dz=0.)        const;
    /// get $\sigma_{t_x}^2$
    float cov_tx()                   const;
    /// get $\sigma_{t_y}^2$
    float cov_ty()                   const;
    /// get $\sigma_p^2$
    float cov_p()                    const;
    
    /// dummy function: always return true
    bool isValid() const;
    /// dummy function: do nothing
    void valid();
    /// dummy function: do nothing
    void invalid();

#if defined USE_ROOT
    /// comparison between Track objects
    bool operator==(const Track& rhs) const;

    /// collect TrackIf pointer
//    void collect(vector<Track*>& c) const;
#endif
    /// propagate track to $z$
    bool propagate(const double zz); 

    /** @name --- Other access methods --- */
    /// $p \cdot e_x$
    double px()     const;
    /// $p \cdot e_y$
    double py()     const;
    /// $t_x \cdot e_z$
    double ex()     const;
    /// $t_y \cdot e_z$
    double ey()     const;
    /// $1/\sqrt{1+t_x^2+t_y^2}$
    double ez()     const;

    /// $\sqrt{\sigma_x^2(z+dz)}$
    double xerr(double dz = 0)   const;
    /// $\sqrt{\sigma_y^2(z+dz)}$
    double yerr(double dz = 0)   const;
    /// $\sqrt{\sigma^2_{tx}}$
    double txerr()               const;
    /// $\sqrt{\sigma^2_{ty}}$
    double tyerr()               const;
    /// $\sqrt{\sigma^2_p}$
    double perr()                const;


    /// covariance matrix
    const MATRIX::CMatrix&     V()  const;
    /// inverse $5\times5$ covariance matrix
    const MATRIX::CMatrix&     G()  const;
    /// inverse $4\times4$ covariance matrix, without momentum
    const MATRIX::CMatrix&     GM() const;

    /**@name --- Expert functions --- */
    /// called by cout
    std::ostream& print( std::ostream& os ) const;
    /// rest-mass
    double rm() const;
    /// set rest-mass (needed for mass constrained fits)
    void rm(const double mass);
    /// conjugated rest-mass
    double rmCC() const;
    /// set conjugated rest-mass 
    void rmCC(const double mass);

    void delete_mom();

    /** @name --- Operators --- */
    ///
   //bool operator< (const Track& rhs);
    ///
   //int  operator==(const Track& rhs);

  private:
    short int           t_Q;      // charge
    double              t_rm;     // rest-mass (in Vt: xmass)
    double              t_rmCC;   // a conjugated mass hypothesis
  protected:
    MATRIX::VtVector    t_p;      // track parameter vector (x,y,z,tx,ty,p)
			  	  // (in Vt: p(1,...,5) 
    MATRIX::CMatrix     t_V;      // 5x5 covariance matrix of Track
    MATRIX::CMatrix     t_G;      // inverse of t_cov (5x5)
    MATRIX::CMatrix     t_GM;     // inverse of t_cov (4x4, without momentum)
 }; // class Track


  //==============================================================================
  // operator <<
  //==============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const Track& t ) {
    return t.print(os);
  }

} // end of namespace VERTEX

#ifndef __CINT__
#include "VtTrack.icc"
#endif
#endif
