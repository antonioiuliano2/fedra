#ifndef __VTVERTEX_HH
#define __VTVERTEX_HH
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
// Description: Vertex class
//
// changes:
// 21 Aug 2000 (TG) creation
// 25 Aug 2000 (TG) remove_worst added
// 04 Sep 2000 (TG) member function ndf() added. chi2() returns now chi2
//                  and not chi2/ndf any more
// 04 Sep 2000 (TG) Modified argument of VtRemoveTrack()
// 05 Sep 2000 (TG) declared some member functions as inline
// 05 Sep 2000 (TG) erase() member function added
// 12 Sep 2000 (TG) private member function bigdim() added
// 12 Sep 2000 (TG) member function bigcov() added
// 13 Sep 2000 (TG) added xmass member and add() member function
// 14 Sep 2000 (TG) added v_CINV member
// 14 Sep 2000 (TG) bug fix in documentation
// 02 Okt 2000 (TG) added chi2l() member function
// 04 Okt 2000 (TG) removed v_use_momentum data member
// 17 Okt 2000 (TG) modified comments
// 18 Okt 2000 (TG) MassConstr() member function added
// 24 Okt 2000 (TG) invalid() member function added, v_covn, v_Mvalid member added,
//                  changed return type of bigcov()
// 25 Okt 2000 (TG) covn() member function added
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 14 Nov 2000 (TG) added constructors for CLUE (RecoSegment, ArtePointer<RecoTrack>)
// 31 Jan 2001 (TG) added prob() member function
// 02 Feb 2001 (TG) added global ClMember declarations
// 20 Feb 2001 (TG) pmaxfrac() added, ClMember pmaxfrac added
// 27 Feb 2001 (TG) improved ndf() (mass constrained vertex fit case added)
// 28 Feb 2001 (TG) added default argument to calc_mother()
// 06 Mar 2001 (TG) v_angdist added, angle() and dist() check now whether they
//                  are computed.
// 12 Mar 2001 (TG) set_invalid() added
// 09 Jul 2001 (TG) changed return type of ntracks() to unsigned int, added clear()
// 04 Sep 2001 (TG) added push_back(TrackIf), added VertexIf inheritance
// 17 Okt 2001 (TG) added/improved #defines for ARTE
//
// *****************************************************************************

#include <iosfwd>
#include <vector>
#include "CMatrix.hh"
#include "VtVector.hh"
#include "VtTrack.hh"
#include "VtMassC_list.hh"


namespace VERTEX {
  //
  // Some Declarations
  //
  class Track;
  class Vertex;

  typedef std::vector<double>                 Vector_d;

  typedef std::vector<Track*>                 Track_v;
  typedef std::vector<Track*>::iterator       Track_it;
  typedef std::vector<Track*>::const_iterator Track_cit;

  /** Vertex class */
  //============================================================================
  // Class Vertex
  //============================================================================
  class Vertex: public Track
 {
  public:
    
    /**@name --- Constructors --- */
    ///
    Vertex();
    ///
    Vertex(const Vertex& rhs);
    ///
    Vertex(const Track_v& v);
    ///
    Vertex(Track& t1, Track& t2);
    ///
    ~Vertex();

    /** @name --- VertexIf access functions --- */
    /// $x$ of vertex
    float vx()      const;
    /// $y$ of vertex
    float vy()      const;
    /// $z$ of vertex
    float vz()      const;
    /// $\chi^2$ of vertex fit
    float chi2()    const;
    /// upper tail $\chi^2$ probability
    float prob()    const;
    /// degrees of freedom of vertex fit
    unsigned short int ndf() const;
    /// no of tracks in vertex $n$
    unsigned short int ntracks() const;

#if defined USE_ROOT && !defined __CINT__
    /// $\vec{x} = (x,y,z)$
    SVector<double,3> vpos() const;
    /// vertex cov. matrix
    const SMatrix<double,3>& VCOV() const;
#endif

    /// $\sigma_x^2$ of vertex
    float vtx_cov_x()   const;
    /// $\sigma_y^2$ of vertex
    float vtx_cov_y()   const;
    /// $\sigma_z^2$ of vertex
    float vtx_cov_z()   const;

    /** @name -- Other access methods --- */
    /** $\sqrt{n\cdot\sum_it_{x,i}^2 - (\sum_i t_{x,i})^2 + n\cdot\sum_i t_{y,i}^2 -
	(\sum_i t_{y,i})^2} / n$ */
    double angle()   const;
    /// available after call to rmsDistAngle()
    double dist()    const;
    /// $\sqrt{\sigma_{vx}^2}$ vertex $x$-error
    double vxerr()   const;
    /// $\sqrt{\sigma_{vy}^2}$ vertex $y$-error
    double vyerr()   const;
    /// $\sqrt{\sigma_{vz}^2}$ vertex $z$-error
    double vzerr()   const;

    /// $\max|p_i/p_j|$ max. momentum fraction
    double pmaxfrac() const;

    /// $3\times3$ Vertex covariance matrix
    const MATRIX::VtSymMatrix&  CS()   const;
    /// $(3\cdot n+3)\times(3\cdot n+3)$ general covariance matrix
    const MATRIX::VtSymMatrix&  covn() const;
    /// is vertex valid?
    bool valid() const;
    /// mark vertex as invalid
    void set_invalid();

    /** @name --- Mass calculation routines ---
	@memo $m = \sqrt{\sum_i p_i^2 + m_i^2 - \sum_i \vec{p}_i^2}$
     */
    /// rest-mass 0 or track rest mass
    double mass(const bool use = false) const;
    /// rest-mass 0 or conjugated track rest mass
    double massCC(const bool use = false) const;
    /// single rest-mass calc.,
    double mass(double m1) const;
    /// 2-track mass with particle rest-masses
    double mass(double m1, double m2) const;
    /// 3-track mass with rest-masses
    double mass(double m1, double m2, double m3) const;
    /// 4-track mass with rest-masses
    double mass(double m1, double m2, double m3, double m4) const;
    /// specify $n$ rest masses
    double mass(const Vector_d& m02, bool use = false, bool CC = false)  const;

    /**@name --- Vertexing routines --- */
    /// unweighted 2-D min. dist.
    const bool findVertex2D(void);
    /// unweighted 3-D min. dist.
    const bool findVertex3D(void);
    /// Vt Kalman-filter
    const bool findVertexVt    ();
    /// weighted 2-D min. dist.
    const bool VtEstimateVertex();
    /// estimate Vertex without changing Vertex object
    const bool VtEstimateVertexMath(double& x, double& y, double& z);
    /// Kalman filter with mass constraints
    const bool VtMass();

    /**@name --- Adding tracks --- */
    ///
    void push_back(Track& track);
#if defined USE_ROOT
    ///
//    void push_back(TrackIf& track);
#endif

    /**@name --- Misc functions --- */

    /// calc rms dist and rms angle
    double rmsDistAngle() const;
    /// calc track params of mother particle
    bool calc_mother(bool use_kalman = true);
    /// calc cov. matrix of mother particle
    bool calc_mother_cov();

    /// remove last track from vertex, do inverse Kalman filter
    const bool remove_last();
    /// remove track with biggest $\chi^2$ contribution, do inverse Kalman filter
    void remove_worst();
    /// set for all tracks whether momentum should be used or not
    void  use_momentum(const bool use);
    /// use refitted track parameters or not
    void  use_kalman(const bool use);
    ///
    bool  use_kalman() const;
    /// $\chi^2$ distance to space point, $ndf = 3$
    double distance(double x, double y, double z) const;
    /// $\chi^2$ distance to track, $ndf = 2$
    double distance(Track& t) const;
    /// $\chi^2$ distance to vertex, $ndf = 3$
    double distance(const Vertex& rhs) const;

    /** @name --- Expert routines --- */
    /// clear all track-vertex relations, makes vertex invalid
    void clear();
    ///
    const bool VtFilter        ();
    ///
    const bool VtInverseFilter () const;
    /// smooth vertex position
    void       VtSmoothX       ();
    /// smooth momenta
    void       VtSmoothQ       ();
    /// remove track using inverse Kalman filter
    const bool VtRemoveTrack   (Relation& track);
    /// Kalman filter + smoother
    const bool VtFit           ();
    ///
    const MATRIX::VtVector& xv()  const;
    /// vector of vertex-positions
    const MATRIX::VtVector& xvs() const;
    /// called by operator<<()
    std::ostream& print(std::ostream& os) const;
    /// $\chi^2$ after last filter step
    double chi2n() const;
    /// $\chi^2$ contribution of last filter step
    double chi2l() const;
    /// unbook relation
    virtual void remove(Relation* r);
    /// erase a relation without refit
    virtual const iterator erase(const iterator& pos);
    /// compute general $(3\cdot n+3) \times (3\cdot n+3)$ covariance matrix 
    const MATRIX::VtSymMatrix& bigcov();

    /** @name --- Mass Constraint routines --- */
    /// add a mass constraint for all tracks, $m$ = mass of mother particle
    void MassConstr(double m);
    /// add a mass constraint
    MassC& addMassConstr(double m=0.);
    /// delete the mass constraints
    void clearMassConstr();
    /// number of mass constraints
    unsigned int nMassConstr() const;

    /** @name --- Operators --- */
    ///
    const Vertex& operator= (const Vertex& rhs);
    ///
    const bool    operator==(const Vertex& rhs) const;
    ///
    const double  operator- (const Vertex& rhs) const;

  private:
    const unsigned int bigdim() const;
    // flag vertex as invalid 
    void invalid();                       
    
    bool           v_use_kalman;          // use refitted track parameters
    bool           v_valid;               // is vertex information valid?
    bool           v_Mvalid;              // is mass constrained vertex information valid?
    bool           v_mother;              // is the mother particle calculated?
    mutable bool   v_angdist;             // is angle and dist calculated?
    mutable double v_angle;               // rms of opening angle
    mutable double v_dist;                // rms of distance to vertex
    double         v_bk13;                // vtx-z at begin of iteration step
                                          // (in Vt: Bk13(iv), b13)
    double         v_chi2;                // chi2 of vertex-fit
    MATRIX::VtVector     kal_xv;          // x,y,z of vtx from kalman filter
    MATRIX::VtVector     kal_xvs;         // pos of Vertex after smooth
                                          // (xv-smoothed) (in Vt: xvs(1..3,...))
    MATRIX::VtSymMatrix  v_CS;            // v_C smoothed (=v_C of last
                                          // filtered track) (in Vt: CS)
    MATRIX::VtSymMatrix  v_CINV;          // inverse of CS at initialization time
    MATRIX::VtSymMatrix* v_covn;          // (3*n+3)x(3*n+3) big cov. matrix
    Track_v    tracks;                    // tracks owned by vertex
    MassC_v    xmass;                     // vector of mass constraint pointer

    ClassDef(Vertex, 1);

  }; // class Vertex


  //==============================================================================
  // operator<<
  //==============================================================================
  inline std::ostream& operator<<(std::ostream& s, const Vertex& a) {
    return a.print(s);
  }

} // namespace VERTEX

#ifndef __CINT__
#include "VtVertex.icc"
#endif

#endif
