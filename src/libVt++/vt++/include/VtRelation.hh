#ifndef __VTRELATION_HH
#define __VTRELATION_HH
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
// Description: Relation class
//
// changes:
// 21 Aug 2000 (TG) creation
// 13 Sep 2000 (TG) added MassC member
// 20 Okt 2000 (TG) #include VtVassC_list.hh added, changed type of massc
//                  from MassC* to MassC_v
// 14 Nov 2000 (TG) added getTrack(), getVertex(), getKalman() member functions,
//                  hide reference members in case of CINT (CINT limitation)
//
// *****************************************************************************

#include <iosfwd>
#include "vt++/VtMassC_list.hh"

namespace VERTEX {

  class Track;
  class Vertex;
  class Kalman;
  class Relation;

  /** Class for Track-Vertex-Kalman object relations.
      @memo for internal use only
  */
  //============================================================================
  // Class Relation - class for Vertex-Track-Kalman relations
  //============================================================================
  class Relation {
  public:
    /** @name --- Constructors --- */
    ///
    Relation(Track&  ttrack, Vertex& vvertex);
    ///
    ~Relation();

    /** @name --- Expert functions --- */
    ///
    void print( std::ostream& os ) const;

    ///
    inline Track&  getTrack()  { return track; }
    ///
    inline Vertex& getVertex() { return vertex; }
    ///
    inline Kalman& getKalman() { return kalman; }

#ifndef __CINT__
    // reference members are currently a CINT limitation
    Track&   track;  // reference to track
    Vertex&  vertex; // reference to vertex 
    Kalman&  kalman; // reference to Kalman-object
#endif
    MassC_v  massc;  // vector of mass-constraint objects
  };

  //==============================================================================
  // operator <<
  //==============================================================================
  inline std::ostream& operator<< ( std::ostream& os, const Relation& r ) {
    r.print(os);
    return os;
  }

} // end of namespace VERTEX
#endif
