#ifndef __SKALMAN_HH
#define __SKALMAN_HH
// ********************************************************************
//
// source:
//
// type:      source code
//
// created:   10. Apr 2001
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: A fixed size class for Kalman filter Vertex fit
//
// changes:
// 10 Apr 2001 (TG) creation
// 12 Apr 2001 (TG) p(), tvec(), evec(), pvec(), F(), ES(), DS() added
// 18 Apr 2001 (TG) doc++ comments added, removed use_mom template parameter
// 24 Apr 2001 (TG) tx(), ty() added
// 04 Mai 2001 (TG) added Track inheritance, some functions are dummy
// 11 Jun 2001 (TG) added COV(), cov_tx(), cov_ty(), cov_p()
// 29 Jun 2001 (TG) added charge(), pz(), energy(), xf(), rap(), xfabs() removed
// 05 Jul 2001 (TG) collect() added
// 09 Jul 2001 (TG) added isValid(), valid(), invalid() from Track
// 10 Jul 2001 (TG) operator==() added
// 09 Jan 2002 (TG) changed return type of propagate() to bool
//
// ********************************************************************
#include <iosfwd>
#include "vt++/VtTrack.hh"

template <unsigned int NTR> class SVertex;

/** Fixed size Kalman filter class.

    @author T. Glebe
*/
//==============================================================================
// SKalman
//==============================================================================
template <unsigned int NTR>
class SKalman : public Track {
public:
  /** @name --- Constructors --- */
  ///
  SKalman();
  ///
  SKalman(const Track& t, const SVertex<NTR>& v);

  /** @name --- Track methods --- */
  /// refitted Track $x$ position (= vertex $x$ position)
  float x()   const;
  /// refitted Track $y$ position (= vertex $y$ position)
  float y()   const;
  /// refitted Track $x$ position at $z$
  float x(float z)   const;
  /// refitted Track $y$ position at $z$
  float y(float z)   const;
  /// refitted Track $z$ position (= vertex $z$ position)
  float z()   const;
  /// refitted track slope $t_x$
  float tx() const;
  /// refitted track slope $t_y$
  float ty() const;
  /// refitted momentum
  float p() const;
  /// refitted transverse Track momentum $p_t$
  float pt()  const;
  /// momentum along $z$: $p / \sqrt{t_x^2+t_y^2+1}$ 
  float pz()    const;
  /// Kalman $\chi^2$. Use only if you know what it is!
  float chi2() const;
  /// refitted azimuthal angle $\phi$ [deg]
  float phi() const;
  /// refitted polar angle $\theta = \cos^{-1}(e_z)$ [deg]
  float theta() const;
  /// refitted rapidity $\eta = -\log\tan(\theta/2.)$
  float eta() const;
  /// particle charge: -1 for neg. +1 for pos. \& neutrals
  int   charge() const;

  /// $E = \sqrt{m^2 + p^2}$
  float energy(double mass = 0.) const;
  /** $x_f = (p_z - E(m)\cdot\sqrt{1 - \frac{2m_p}{920 GeV/c + m_p}})/m_p$,
      $m_p$ = Proton mass, $m$ = Particle mass */
  float xf(double mass = 0.) const;
  /// Rapidity $y = \frac{1}{2}\ln(\frac{E+p_z}{E-p_z})$
  float rap(double mass = 0.) const;

  /// vertex position
  SVector<double,3> xvec() const;
  /// $\vec{v} = (t_x,t_y,1.)$ refitted slope vector
  SVector<double,3> tvec() const;
  /// $\vec{v} = (e_x,e_y,e_z)$ unit vector along refitted track
  SVector<double,3> evec() const;
  /// $\vec{v} = (p_x,p_y,p_z)$ refitted mom. vector
  SVector<double,3> pvec() const;

  /// collect pointers
  void collect(vector<Track*>& c) const;

  /** @name --- Dummy Track functions --- */
  /// returns always true
  bool isValid() const;
  /// does nothing
  void valid();
  /// does nothing
  void invalid();
  /// returns a 0 matrix
  const SMatrix<double,5>& COV()  const;
  /// returns a 0 matrix
  const SMatrix<double,5>& CINV() const;
  /// dummy function: do nothing
  bool propagate(const double z);
  /// returns always 0
  unsigned short int ndf()  const;
  /// returns 0
  float cov_x(double dz=0.)        const;
  /// returns 0
  float cov_y(double dz=0.)        const;
  /// returns 0
  float cov_tx()                   const;
  /// returns 0
  float cov_ty()                   const;
  /// returns 0
  float cov_p()                    const;

  /** @name --- Operators --- */
  /// compare Track pointers
  bool operator==(const Track& rhs) const;

  /** @name --- Access methods --- */
  ///
  const SMatrix<double,3>& KCOV()  const;
  ///
  const SMatrix<double,3>& KCINV() const;
  ///
  const SVector<double,3>& xv()    const;
  ///
  const SMatrix<double,3>& F()     const;
  ///
  const SMatrix<double,3>& ES()    const;
  ///
  const SMatrix<double,3>& DS()    const;

  /** @name --- Expert methods --- */
  /// filter step
  bool filter(const unsigned int I);
  /// smoother step
  bool smooth();
  /// cov. matrices of smoothed parameters
  bool smoothC();
  /// used by operator<<()
  std::ostream& print( std::ostream& ) const;
  
private:
  const Track*      track_;
  const SVertex<NTR>* vtx_;
  double k_tx;
  double k_ty;
  double k_chi2;
  SVector<double,3>   k_qvs;
  SVector<double,3>   k_xv;
  SVector<double,3>   k_qv;
  SMatrix<double,3>   k_C;
  SMatrix<double,3>   k_CINV;
  SVector<double,5>   k_pc;
  SMatrix<double,3,5> k_WBG;
  SMatrix<double,3>   k_W;
  SMatrix<double,3>   k_F;
  SMatrix<double,3>   k_ES;
  SMatrix<double,3>   k_DS;
};


//==============================================================================
// operator <<
//==============================================================================
template <unsigned int NTR>
inline std::ostream& operator<< (std::ostream& os, const SKalman<NTR>& vt) {
  return vt.print( os );
}

#include "SKalman.icc"
#endif
