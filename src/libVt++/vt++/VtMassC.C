// *****************************************************************************
//
// source:
//
// type:      source code
//
// created:   12. Sept 2000
//
// author:    Thorsten Glebe
//            HERA-B Collaboration
//            Max-Planck-Institut fuer Kernphysik
//            Saupfercheckweg 1
//            69117 Heidelberg
//            Germany
//            E-mail: T.Glebe@mpi-hd.mpg.de
//
// Description: class for a mass constraint
//
// changes:
// 13 Sep 2000 (TG) creation
// 18 Okt 2000 (TG) code for calc_Sl() added
// 25 Okt 2000 (TG) added code for check_kinematic()
// 26 Dec 2000 (TG) end() optimization
// 16 Jan 2002 (TG) added #include <cmath>, #include "VtTrack.hh"
//
// *****************************************************************************

#include <cmath>
#include "vt++/VtMassC.hh"
#include "vt++/VtTrack.hh"
#include "vt++/VtSymMatrix.hh"
#include "vt++/VtRelation.hh"
#include "vt++/VtKalman.hh"

using namespace MATRIX;

namespace VERTEX {

  //==============================================================================
  // Constructors
  //==============================================================================
  MassC::MassC():
    m_mass(0.),
    m_SE  (0.),
    m_Sl  (MATRIX::VtVector(3)),
    m_galp(0.)
  {}

  MassC::MassC(double m):
    m_mass(m),
    m_SE  (0.),
    m_Sl  (MATRIX::VtVector(3)),
    m_galp(0.)
  {}

  MassC::~MassC() {
    const Rel_cit end = m_rel.end();
    for(Rel_cit it = m_rel.begin(); it != end; ++it)
      (*it)->massc.remove(this);
  }

  //==============================================================================
  // push_back
  //==============================================================================
  void MassC::push_back(Relation* r) {
    m_rel.push_back(r);
    r->massc.push_back(this);
  }

  //==============================================================================
  // check_kinematic: check kinematic boundary
  //==============================================================================
  const bool MassC::check_kinematic() const {
    double sum =  0.;

    // sum up rest-masses
    const Rel_cit end = m_rel.end();
    for(Rel_cit it = m_rel.begin(); it != end; ++it) {
      sum += (*it)->track.rm();
    }

    // if sum of track rest-masses is greater than rest-mass of mother
    // particle, something is wrong
    return ((sqr(sum) - sqr(m_mass))>=0.) ? false : true;
  }

  //==============================================================================
  // calc_Sl  (in Vt: eq. 41 and eq. 42)
  //==============================================================================
  void MassC::calc_Sl() {

    m_SE = 0.;
    m_Sl.clear();

    // copute S_l^E, S_l (int Vt: eq. 41,42)
    const Rel_cit end = m_rel.end();
    for(Rel_cit it = m_rel.begin(); it != end; ++it) {
      const Kalman& kal = (*it)->kalman;
      double p = kal.alpc()[2];
      m_SE    += kal.erg();    // in Vt: SE
      m_Sl[0] += p * kal.xn(); // in Vt: Sx
      m_Sl[1] += p * kal.yn(); // in Vt: Sy
      m_Sl[2] += p * kal.zn(); // in Vt: Sz
    }

    // compute g_l(alpha) (in Vt: eq. 43)
    m_galp = sqr(m_SE) - m_Sl*m_Sl - sqr(m_mass);

#ifdef VtDEBUG
    cout << " m_mass: " << m_mass << endl;
    cout << " SE: " << m_SE    << endl;
    cout << " Sx: " << m_Sl[0] << endl;
    cout << " Sy: " << m_Sl[1] << endl;
    cout << " Sz: " << m_Sl[2] << endl;
    cout << " m_galp: " << m_galp << endl;
#endif

    return;
  }

  //==============================================================================
  // gam   copute partial derivatives Gamma_li (in Vt: eq. 34,44,45,46)
  //==============================================================================
  const VtVector MassC::gam(const Relation& rel) const {
    const Kalman& kal = rel.kalman;
    double p = kal.alpc()[2];
    
    // vector dg_l/dalpha_i
    VtVector gam(3);

    double fac = -2. * p * kal.zn();

    // compute eq. 44
    VtVector nn(1.-sqr(kal.xn()), -kal.xn()*kal.yn(), -kal.xn()*kal.zn());
    gam[0] = fac * (nn * m_Sl);

    // comupte eq. 45
    VtVector nm(-kal.xn()*kal.yn(), 1.-sqr(kal.yn()), -kal.yn()*kal.zn());
    gam[1] = fac * (nm * m_Sl);

    // compute eq. 43
    gam[2] = 2. * (p * m_SE / kal.erg() - kal.nalpc() * m_Sl);

#ifdef VtDEBUG
    cout << " gam: " << gam << endl;
#endif
    return gam;
  }

  //==============================================================================
  // print
  //==============================================================================
  void MassC::print(std::ostream& os) const {
    os.setf(ios::right,ios::adjustfield);
    os.setf(ios::fixed,ios::floatfield);
    
    os << "mass: " << mass() << ", no. of tracks: " << m_rel.size();
    return;
  }
} // end of namespace VERTEX
