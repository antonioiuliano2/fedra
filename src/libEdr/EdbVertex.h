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
#include "vt++/VtTrack.hh"
#include "vt++/VtVertex.hh"

//_________________________________________________________________________
class EdbVertex: public TObject {
 private:

  TObjArray eEdbTracks;       // pointers to data tracks
  TArrayI   eZpos;            // 0-track start, 1-track end connect to the vertex
  Int_t     eNtr;

  Float_t eX;          // for generated vertexes - the real vertex position 
  Float_t eY;          // for reconstructed tracks - average point used as  
  Float_t eZ;          // local coordiantes origin (0,0,0) to avoid accuracy problem

  VERTEX::Vertex *eV;
  
 public:
  EdbVertex();
  EdbVertex(EdbVertex &v);
  virtual ~EdbVertex();

  int MakeV( bool usemom = true );

  VERTEX::Vertex *V() const {return eV;}
  void Clear();
//  int N() const {return eEdbTracks.GetEntries();}
  int N() const {return eNtr;}
  Float_t X() const { return eX;}
  Float_t Y() const { return eY;}
  Float_t Z() const { return eZ;}
  void SetXYZ( float x, float y, float z) { eX=x; eY=y; eZ=z;} 

  bool AddTrack(EdbTrackP *track, int zmin, float ProbMin = 0.);

  TObjArray &GetEdbTracks() {return eEdbTracks;}
  EdbTrackP *GetTrack(int i) const { return (EdbTrackP *)(eEdbTracks.At(i)); }
  Int_t	     Zpos(int i) const { return eZpos.At(i); }
  void       SetTracksVertex();
  bool EstimateVertexMath( float& xv, float& yv, float& zv, float& d );

  bool Edb2Vt( const EdbTrackP& tr, VERTEX::Track& t );
  bool Edb2Vt( const EdbSegP& s, VERTEX::Track& t );

  void Print() const 
    {
      printf("%d edbtracks:\n",N());
      EdbSegP *tr=0;
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

  //VERTEX::Track& GetVtTrack( const int index, VERTEX::Track& t);

  ClassDef(EdbVertexRec,2) //reconstruct vertexes in OPERA emulsion data
};

#endif /* ROOT_EdbVertex */
