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
//
// *****************************************************************************
#include "Riostream.h"
#include "vt++/VtRelation.hh"
#include "vt++/VtTrack.hh"
#include "vt++/VtVertex.hh"
#include "vt++/VtKalman.hh"

namespace VERTEX {

  //============================================================================
  // Relation::Constructor
  //============================================================================
  Relation::Relation(Track&  ttrack, Vertex& vvertex):
    track(ttrack),
    vertex(vvertex),
    kalman(*(new Kalman(this))) {}
  
  Relation::~Relation() { delete &kalman; }

  //============================================================================
  // Relation::print
  //============================================================================
  void Relation::print( std::ostream& os ) const {
    os << "[tr: "  << &track 
       << " vtx: " << &vertex
       << " kal: " << &kalman
       << "]";
  }

} // end of namespace VERTEX
