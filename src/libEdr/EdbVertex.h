#ifndef ROOT_EdbVertex
#define ROOT_EdbVertex

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertex                                                            //
//                                                                      //
// Class to reconstruct vertexes                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TObject.h>
#include <TTree.h>
#include <TVector3.h>
#include <TGraphErrors.h>
#include "EdbPattern.h"
#include "EdbPVGen.h"
#include "vt++/VtTrack.hh"
#include "vt++/VtVertex.hh"

//_________________________________________________________________________
class EdbVertex: public TObject {
 private:

  TObjArray eEdbTracks;       // pointers to data tracks

  VERTEX::Vertex *eV;
  
 public:
  EdbVertex();
  EdbVertex(EdbVertex &v);
  virtual ~EdbVertex();

  int MakeV( TObjArray &segs );

  VERTEX::Vertex *V() const {return eV;}
  void Clear();
  int N() const {return eEdbTracks.GetEntries();}
  void AddTrack(EdbTrackP *track)     { eEdbTracks.Add(track); }

  TObjArray &GetEdbTracks() {return eEdbTracks;}
  EdbTrackP *GetTrack(int i) const { return (EdbTrackP *)(eEdbTracks.At(i)); }

  bool EstimateVertexMath( float& xv, float& yv, float& zv, float& d );

  void Print() const 
    {
      printf("%d edbtracks:\n",N());
      EdbTrackP *tr=0;
      for(int i=0; i<N(); i++) {
	tr = GetTrack(i);
	if(tr)  tr->Print();
      }
    }

  ClassDef(EdbVertex,1)  // vertex class for OPERA emulsion data
};

//_________________________________________________________________________
class EdbVertexRec: public TObject {
 private:
  TObjArray *eEdbTracks;

 public:
  EdbVertexRec();
  virtual ~EdbVertexRec()
  {
   if (eEdbTracks)
   {
    delete eEdbTracks;
    eEdbTracks = 0;
   }
  }

  TTree *init_tracks_tree(const char *file_name, EdbTrackP *track);
  int BuildTracksArr(const char *file_name="linked_tracks.root", int nsegMin=2 );
  double Tdistance(const VERTEX::Track& t1, const VERTEX::Track& t2);
  double Tdistance(const EdbSegP& s1, const EdbSegP& s2);

  EdbTrackP *GetEdbTrack( const int index );

  static double ProbeSeg( const EdbTrackP *s1, EdbTrackP *s2, 
			  const float X0=5810. );
  static double ProbeSeg( const EdbTrackP *s1, EdbSegP *s2, 
			  const float X0=5810. );
  static double ProbeSeg( const EdbSegP *s1, EdbSegP *s2, 
			  const float X0=5810., const float mass=0.1396 );
  static float  Chi2Seg( EdbSegP *s1, EdbSegP *s2);

  static bool AttachSeg(  EdbTrackP& tr, EdbSegP *s,
			  const float X0, const float ProbMin, float &prob );
  static bool Edb2Vt( const EdbTrackP& tr, VERTEX::Track& t );
  static bool Edb2Vt( const EdbSegP& s, VERTEX::Track& t );

  static void  TrackMC( EdbPatternsVolume &pv, float zlim[2],
			float lim[4], float sigma[4], 
			EdbTrackP &tr, int eloss_flag=0);

  static float  P_MS(EdbTrackP &tr, float X0=5810., bool draw=false);


  //VERTEX::Track& GetVtTrack( const int index, VERTEX::Track& t);

  ClassDef(EdbVertexRec,2) //reconstruct vertexes in OPERA emulsion data
};

#endif /* ROOT_EdbVertex */
