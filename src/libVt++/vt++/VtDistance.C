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
// Description: an (incomplete) collection of distance functions
//
// changes:
// 21 Aug 2000 (TG) creation
// 04 Sep 2000 (TG) SGNdistance(const Track&, const Vertex&) function added
// 06 Sep 2000 (TG) SGNdistance(const Track&, double x, double y, double z) added
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
// 17 Okt 2001 (TG) added/improved #defines for ARTE
//
// *****************************************************************************
#include <iostream>

#include "vt++/VtUtil.hh"
#include "vt++/VtVertex.hh"
#include "vt++/VtTrack.hh"
#include "vt++/VtDistance.hh"

#if defined USE_ROOT
//#include "clue/cluearte.hh"
//#include "interfaces/VertexIf.hh"
//#include "clue/RecoVertex.hh"
#endif

namespace VERTEX {


  //============================================================================
  // distance: Chi2 distance between tracks
  //============================================================================
  double distanceChi2(Track& t1, Track& t2) {
    Vertex tmp(t1,t2);
    tmp.findVertexVt();
    return tmp.chi2();
  }

  //============================================================================
  // distance: spatial distance between Track and Vertex
  //============================================================================
  double distance(const Track& t, const Vertex& v) {
    double dx = v.vx() - t.x();
    double dy = v.vy() - t.y();
    double dz = v.vz() - t.z();
    double tx = t.tx();
    double ty = t.ty();
    double nom = sqr(dx*ty - dy*tx) + sqr(dy - dz*ty) + sqr(dz*tx - dx);
    double denom = sqr(tx) + sqr(ty) + 1.;
    return sqrt(nom/denom);
  }
 
  //============================================================================
  // SGNdistance: signed spatial distance between Track and Vertex
  //============================================================================
  double SGNdistance(const Track& t, const Vertex& v) {
    return SGNdistance(t, v.vx(), v.vy(), v.vz());
  }

  //============================================================================
  // SGNdistance: signed spatial distance between Track and space point
  //============================================================================
  double SGNdistance(const Track& t, double xv, double yv, double zv) {

    double A  = t.tx();
    double B  = t.ty();
    double C  = 1.;
    double xt = t.x();
    double yt = t.y();
    double zt = t.z();

    double denom = A*A + B*B + C*C;
    double nom   = A*(xt-xv) + B*(yt-yv) + C*(zt-zv);
    double rho   = nom / denom;

    // point of track closest to Vertex
    double x = xt - A*rho;
    double y = yt - B*rho;
    double z = zt - C*rho;

    double dx = x - xv;
    double dy = y - yv;
    double dz = z - zv;

    // The sign tells if the point of track closest to 
    // vertex is upstream or downstream the vertex
    return sgn(dz) * sqrt(dx*dx + dy*dy + dz*dz);
  }

  //============================================================================
  // distance: Chi2 distance between two vertices
  //============================================================================
  double distanceChi2(const Vertex& v1, const Vertex& v2) {
    return v1.distance(v2);
  }

  //============================================================================
  // distance: Chi2 distance between vertex and space point
  //============================================================================
  double distanceChi2(const Vertex& v, double x, double y, double z) {
    return v.distance(x,y,z);
  }

#if defined USE_ROOT

  //============================================================================
  // SGNdistance: signed spatial distance between Track and RecoVertex
  //============================================================================
//  double SGNdistance(const Track& t, const VertexIf& v) {
//    return SGNdistance(t, v.vx(), v.vy(), v.vz());
//  }
#endif

} // end of namespace VERTEX
