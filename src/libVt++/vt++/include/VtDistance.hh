#ifndef __VTDISTANCE
#define __VTDISTANCE
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
// 14 Sep 2000 (TG) distance() functions returning chi2 renamed to distanceChi2()
// 26 Okt 2000 (TG) #ifndef NO_ARTE added (for integration into CLUE)
//
// *****************************************************************************

#ifdef USE_ROOT
//class RecoVertex;
#endif
//class VertexIf;

namespace VERTEX {
  class Track;
  class Vertex;

  /** @name --- Distance functions --- */

  /// $\chi^2$ distance track - track, $ndf = 1$
  double distanceChi2(Track& t1, Track& t2);

  /// spatial distance track - vertex
  double distance(const Track& t, const Vertex& v);
  // spatial distance track - vertex
  inline double distance(const Vertex& v, const Track& t) { return distance(t,v); }

  /// signed spatial distance track - vertex
  double SGNdistance(const Track& t, const Vertex& v);
  // signed spatial distance track - vertex
  inline double SGNdistance(const Vertex& v, const Track& t) {
    return SGNdistance(t,v);
  }

  /// signed spatial distance track - space point
  double SGNdistance(const Track& t, double xv, double yv, double zv);

  /// $\chi^2$ distance between two vertices, $ndf = 3$
  double distanceChi2(const Vertex& v1, const Vertex& v2);

  /// $\chi2$ distance vertex - space point, $ndf = 3$
  double distanceChi2(const Vertex& v, double x, double y, double z);

  /// signed spatial distance track - VertexIf
//  double SGNdistance(const Track& t, const VertexIf& ap);
} // end of namespace VERTEX
#endif
