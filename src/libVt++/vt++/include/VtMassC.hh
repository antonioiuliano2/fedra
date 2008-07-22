#ifndef __VTMASSCONSTR
#define __VTMASSCONSTR

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
// 12 Sep 2000 (TG) creation
// 18 Okt 2000 (TG) removed #include "VtVector.hh"
// 18 Okt 2000 (TG) added print() member function, added operator<<()
// 18 Okt 2000 (TG) added member m_galp, calc_Sl() member function
// 25 Okt 2000 (TG) check_kinematic() added
//
// *****************************************************************************

#include <iosfwd>
#include <vector>
#include "VtVector.hh"

#ifdef WIN32
#   include "Rtypes.h"
#endif

namespace VERTEX {
  class Relation;

  typedef std::vector<Relation*>                 Rel_v;
  typedef std::vector<Relation*>::iterator       Rel_it;
  typedef std::vector<Relation*>::const_iterator Rel_cit;

  /** Mass Constraint class */
  //============================================================================
  // Class MassC
  //============================================================================
  class MassC {
  public:
    /** @name --- Constructors --- */
    ///
    MassC();
    /// construct from mass of mother particle
    MassC(double m);
    ///
    ~MassC();

    /** @name --- Access functions --- */
    /// add Track to mass constraint
    inline void push_back(Relation& r) { push_back(&r); }
    /// add Track to mass contraint
    void push_back(Relation* r);
    /// get mass constraint
    inline double mass() const { return m_mass; }
    /// set mass constraint
    inline void mass(double m) { m_mass = m; }
    /// constraint $\vec{g}(\vec{\alpha}^{(0)})$
    inline double galp() const { return m_galp; }

    /** @name --- Expert functions --- */
    /// called by operator<<()
    void print(std::ostream& os) const;
    /// compute mass constraint $\vec{g}(\vec{\alpha}^{(0)})$
    void calc_Sl();
    /// check kinematic boundary
    const bool check_kinematic() const;
    /// return $\Gamma_{li}$
    const MATRIX::VtVector gam(const Relation&) const;

  private:
    double           m_mass;  // mass for mass constraint (in Vt: xmc)
    double           m_SE;    // S_l^E in Vt (eq. 41)
    MATRIX::VtVector m_Sl;    // S_l   in Vt (eq. 42)
    Rel_v            m_rel;   // list of tracks participating im mass constr.
    double           m_galp;  // constraint (in Vt: eq. 43)

#ifdef WIN32
   ClassDef(MassC,0)
#endif
  };

  //==============================================================================
  // operator<<
  //==============================================================================
  inline std::ostream& operator<<(std::ostream& s, const MassC& a) {
    a.print(s);
    return s;
  }

} // end of namespace VERTEX
#endif
