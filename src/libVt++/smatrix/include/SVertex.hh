#ifndef __SVERTEX_HH
#define __SVERTEX_HH
// ********************************************************************
//
// source:
//
// type:      source code
//
// created:   06. Apr 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: A fixed size Vertex class
//
// changes:
// 06 Apr 2001 (TG) creation
// 07 Apr 2001 (TG) mass() added
// 10 Apr 2001 (TG) findVertexVt(), kalman_, v_bk13, template parameter use_mom
//                  added
// 12 Apr 2001 (TG) set_track, track(), calc_mother(), calc_mother_tr(),
//                  bigcov(), mother() added 
// 17 Apr 2001 (TG) TrackIf inheritance added, calc_mother_cov() added
// 18 Apr 2001 (TG) removed use_mom template parameter, added mass_tr
// 24 Apr 2001 (TG) valid_, valid(), invalid() added
// 24 Apr 2001 (TG) renamed calc_mother() to calc_mother_tr() and
//                  calc_mother_tr() to calc_mother_trtr(). Added new calc_mother().
// 24 Apr 2001 (TG) added prob()
// 30 Apr 2001 (TG) added pt() and xfabs()
// 02 Mai 2001 (TG) phi(), theta(), eta() added
// 03 Mai 2001 (TG) added read/write vposR(), added findVertexNe()
// 03 Mai 2001 (TG) CTOR added, size added
// 08 Jun 2001 (TG) added massError()
// 11 Jun 2001 (TG) added cov_tx(), cov_ty(), cov_p()
// 28 Jun 2001 (TG) added charge(), pz(), energy(), xf(), rap(), removed xfabs()
// 04 Jul 2001 (TG) moth_tr, moth_cov added
// 05 Jul 2001 (TG) collect() added
// 09 Jul 2001 (TG) added valid(), invalid(), isValid() from TrackIf, removed
//                  valid(), invalid() from VertexIf, added ntracks()
// 10 Jul 2001 (TG) operator==() added
// 23 Aug 2001 (TG) added CTOR, added copy CTOR
// 03 Sep 2001 (TG) added operator=()
// 02 Okt 2001 (TG) set_vpos() added
// 07 Nov 2001 (TG) documentation improved
// 03 Jan 2002 (TG) isMotherTr(), isMotherCov() added, renamed valid_ to
//                  validKalmanFit, changed return type of calcVertex2D
// 04 Jan 2002 (TG) added isKalmanFit()
// 09 Jan 2002 (TG) changed return type of propagate() to bool
// 11 Jan 2002 (TG) added v_ndf member
// 16 Jan 2002 (TG) added operator=(SVertex<NTR>)
//
// ********************************************************************
#include <iosfwd>
#include "interfaces/VertexIf.hh"
#include "interfaces/TrackIf.hh"
#include "smatrix/SVector.hh"
#include "smatrix/SKalman.hh"

/** Fixed size Vertex class.

    @author T. Glebe
*/
//==============================================================================
// SVertex
//==============================================================================
template <unsigned int NTR>
class SVertex : public VertexIf, public TrackIf {
public:
  /** @name --- Constructors --- */
  ///
  SVertex();
  ///
  SVertex(const SVertex<NTR>& rhs);
  /// create Vertex object \& set mother track by $t$
  SVertex(const TrackIf& t);
  ///
  SVertex(const TrackIf& t1, const TrackIf& t2);
  ///
  SVertex(const TrackIf& t1, const TrackIf& t2, const TrackIf& t3);
  ///
  SVertex(const TrackIf& t1, const TrackIf& t2, const TrackIf& t3,
	  const TrackIf& t4);
  /// add a track
  SVertex(const SVertex<NTR-1>& vtx, const TrackIf& t);

  ///
  SVertex<NTR>& operator=(const SVertex<NTR>& rhs);

  /** @name --- static variables --- */
  ///
  static const unsigned int size = NTR;

  /** @name --- VertexIf Access methods --- */
  /// vertex $x$ position $v_x$
  float vx()   const;
  /// vertex $y$ position $v_y$
  float vy()   const;
  /// vertex $z$ position $v_z$
  float vz()   const;
  /// vertex $\chi^2$
  float chi2() const;
  /// degrees of freedom of vertex fit
  unsigned short int ndf()  const;
  /// upper tail $\chi^2$ probability
  float prob() const;
  /// no of tracks in vertex
  unsigned short int ntracks() const;
  /// read only track access
  const TrackIf* track(unsigned int i) const;
  /// read/write track access
  const TrackIf*& track(unsigned int i);
  
  /// vertex position $\vec{v} = (v_x,v_y,v_z)$
  SVector<double,3> vpos() const;
  /// vertex cov. matrix
  const SMatrix<double,3>& VCOV() const;

  /// $\sigma_x^2$ of vertex
  float vtx_cov_x()   const;
  /// $\sigma_y^2$ of vertex
  float vtx_cov_y()   const;
  /// $\sigma_z^2$ of vertex
  float vtx_cov_z()   const;

  /** @name --- SVertex members --- */
  /// vertex position $\vec{v} = (v_x,v_y,v_z)$ (fast readonly access)
  const SVector<double,3>& vposR() const;
  /// vertex position $\vec{v} = (v_x,v_y,v_z)$ (fast read/write access)
  SVector<double,3>& vposR();
  /// set vertex position manually
  void set_vpos(const SVector<double,3>& pos);
  
  /// set track list
  void set_track(unsigned int i, const TrackIf& t);
  /// read only access to Kalman objects
  const SKalman<NTR>& kalman(unsigned int i) const;
  /// read/write access to Kalman objects
  SKalman<NTR>& kalman(unsigned int i);
  /// inverse vertex cov. matrix
  const SMatrix<double,3>& VCINV() const;
  /// mother track parameters $\vec{m}=(x,y,z,t_x,t_y,p)$
  const SVector<double,6>& mother() const;
  /// for internal use only
  const SMatrix<double,3*NTR+3>& covn() const;

  /** @name --- Vertex computations --- */
  /// return vertex computed with 2D analytical method
  SVector<double,3> findVertex2D() const;
  /// compute vertex with a 2D analytical method
  bool calcVertex2D();
  /// return vertex computed with 3D analytical method
  bool findVertex3D(SVector<double,3>& pos) const;
  /// Vt based Kalman filter vertex fit
  bool findVertexVt();
  /// Vt based Kalman filter vertex fit, without first estimation
  bool findVertexNe();
  /// return rms distance and rms opening angle of tracks in vertex
  SVector<double,2> rmsDistAngle() const;
  /// compute vertex with error weighted 2D analytical method
  SVector<double,3> EstimateVertex() const;
  /// compute invariant mass using refitted track momenta
  double mass(const SVector<double,NTR>& rm) const;
  /// compute invariant mass error caused by momentum error
  double massError(const SVector<double,NTR>& rm) const;
  /// compute invariant mass using measured track momenta
  double mass_tr(const SVector<double,NTR>& rm) const;

  /** @name --- TrackIf Access methods ---\\
   The following functions apply only to the mother track and give only
   useful results in case you reconstructed the mother track (see section of
   expert methods). */
  /// $x$ of reconstructed track
  float x()        const;
  ///  $y$ of reconstructed track
  float y()        const;
  /// linear extrapolation of $x$ to z
  float x(float z) const;
  /// linear extrapolation of $y$ to z
  float y(float z) const;
  /// $z$ of reconstructed track
  float z()        const;
  /// mother track rack slope $t_x$
  float tx()       const;
  /// mother track slope $t_y$
  float ty()       const;
  /// mother track momentum $p$
  float p()        const;
  /// transverse mother track momentum $p_t$
  float pt()       const;
  /// mother track momentum along $z$: $p / \sqrt{t_x^2+t_y^2+1}$ 
  float pz()    const;
  /// azimuthal angle $\phi$ [deg]
  float phi() const;
  /// polar angle $\theta = \cos^{-1}(e_z)$ [deg]
  float theta() const;
  /// rapidity $\eta = -\log\tan(\theta/2.)$
  float eta() const;
  /// track charge (=sum of track charges in vertex)
  int   charge() const;

  /// $E = \sqrt{m^2 + p^2}$
  float energy(double mass = 0.) const;
  /** $x_f = (p_z - E(m)\cdot\sqrt{1 - \frac{2m_p}{920 GeV/c + m_p}})/m_p$,
      $m_p$ = Proton mass, $m$ = Particle mass */
  float xf(double mass = 0.) const;
  /// Rapidity $y = \frac{1}{2}\ln(\frac{E+p_z}{E-p_z})$
  float rap(double mass = 0.) const;

  /// get $\sigma_x^2|_{z+dz}$
  float cov_x(double dz=0.) const;
  /// get $\sigma_y^2|_{z+dz}$
  float cov_y(double dz=0.) const;
  /// get $\sigma_{t_x}^2$
  float cov_tx()            const;
  /// get $\sigma_{t_y}^2$
  float cov_ty()            const;
  /// get $\sigma_p^2$
  float cov_p()             const;

  /** Kalman fit: reconstructed track and covariance matrix are computed and
      propagated to $z=0$?\\
      else: is reconstructed track computed?
   */
  bool isValid() const;
  /// mark mother track as valid
  void valid();
  /// mark mother track as invalid
  void invalid();

  /// position $\vec{x} = (x,y,z)$ of mother track
  SVector<double,3> xvec() const;
  /// slope $\vec{t} = (t_x,t_y,1)$ of mother track
  SVector<double,3> tvec() const;
  /// $\vec{e} = (e_x,e_y,ez)$ unit vector along $\vec{p}$ of mother track
  SVector<double,3> evec() const;
  /// mother track momentum vector $\vec{p} = (p_x,p_y,p_z)$
  SVector<double,3> pvec() const;

  /// covariance matrix of mother track
  const SMatrix<double,5>& COV()   const;
  /// inverse cov. matrix of mother track
  const SMatrix<double,5>& CINV()  const;

  /// collect pointers
  void collect(vector<TrackIf*>& c) const;
  /// propagate reconsructed track to $z$
  bool propagate(const double z);
  /// used by operator<<()
  std::ostream& print( std::ostream& ) const;

  /** @name --- Operators --- */
  /// compare TrackIf pointers
  bool operator==(const TrackIf& rhs) const;

  /** @name --- SVertex methods --- */
  /// is mother track calculated?
  bool isMotherTr() const;
  /// is mother cov. matrix calculated?
  bool isMotherCov() const;
  /// has Kalman fit succeeded?
  bool isKalmanFit() const;

  /// do a filter step
  bool filter();
  /// smooth vertex parameters
  bool smooth();
  /// needed for bigcov()
  bool smoothC();
  ///
  inline double bk13() const { return v_bk13; }
  /// calculate mother track + cov. matrix
  bool calc_mother();
  /// calculate mother track using kalman info
  bool calc_mother_tr();
  /// calculate mother track using track info
  bool calc_mother_trtr();
  /// calculate covariance matrix of mother track
  bool calc_mother_cov();
  /// construct $(3\cdot n+3)\times(3\cdot n+3)$ cov. matrix
  bool bigcov();
  
private:
  SVector<double,3>    vpos_;          // vertex position
  double               v_bk13;         // vtx-z before kalman filter could be vz?
  const TrackIf*       tracks[NTR];    // track pointers
  SKalman<NTR>         kalman_[NTR];   // kalman objects
  SMatrix<double,3>    v_CS;           // vertex cov. matrix
  SMatrix<double,3>    v_CINV;         // inverse vertex cov. matrix
  double               v_chi2;         // chi2 of vertex fit
  unsigned int         v_ndf;          // no of degrees of vertex fit
  SVector<double,6>    t_p;            // track parameter of mother particle
  SMatrix<double,3*NTR+3> v_covn;      // cov. matrix of all track parameters
  SMatrix<double,5>    cov_;           // cov. matrix of reconstructed track
  SMatrix<double,5>    cinv_;          // inv. cov. matrix of reconstructed track
  bool                 validKalmanFit; // was Kalman fit successful?
  bool                 valid_;         // is vertex valid?
  bool                 moth_tr;        // is mother track calculated?
  bool                 moth_cov;       // is mother covariance matrix calculated?
  bool                 validity;       // vertex validity, manually set
}; // class SVertex


//==============================================================================
// operator <<
//==============================================================================
template <unsigned int NTR>
inline std::ostream& operator<< (std::ostream& os, const SVertex<NTR>& vt) {
  return vt.print( os );
}

#include "SVertex.icc"
#endif
