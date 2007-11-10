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
#include "vt++/VtRelation.hh"
#include "vt++/VtKalman.hh"

class EdbPVRec;

//_________________________________________________________________________
class EdbVTA: public TObject {

 private:

  EdbTrackP *eTrack;          // pointer to track (or segment)
  EdbVertex *eVertex; 	      // pointer to vertex

  Int_t     eZpos;            // 1-track start, 0-track end connect to the vertex
  Int_t	    eFlag;            // 0-neighbor track;
		              // 1-neighbor segment;
			      // 2-direct (attached) track connection
			      // 3-neighbor vertex;
  Float_t   eImp;             // impact parameter
  Float_t   eDist;            // distance from vertex to the nearest track point
  
 public:
  EdbVTA();
  EdbVTA(EdbVTA &vta);
  EdbVTA(EdbTrackP *tr, EdbVertex *v );
  virtual ~EdbVTA();

  void Set0();
  void SetZpos(int zpos)   {eZpos=zpos;}
  void SetFlag(int flag)   {eFlag=flag;}
  void SetImp(float imp)   {eImp=imp;}
  void SetDist(float dist) {eDist=dist;}

  int   Zpos() const  {return eZpos;}
  int   Flag() const  {return eFlag;}
  float Imp()  const  {return eImp;}
  float Dist() const  {return eDist;}

  void  SetTrack(EdbTrackP *tr)   {eTrack=tr;}
  void  SetVertex(EdbVertex *v)   {eVertex=v;}

  EdbTrackP *GetTrack()    const  {return eTrack;}
  EdbVertex *GetVertex()   const  {return eVertex;}

  void AddVandT();

  ClassDef(EdbVTA,1)  // vertex-track association
};

//_________________________________________________________________________
class EdbVertex: public TObject {
 private:

  TList       eVTn;      // vertex neighborhood tracks and segments
  TList       eVTa;      // attached tracks

  Float_t eX;            // for generated vertexes - the real vertex position 
  Float_t eY;            // for reconstructed ones - average of track connection  
  Float_t eZ;            // points, used as local coordiantes origin (0,0,0) 
			 // to avoid accuracy problem

  Int_t	    eFlag;	 // 0 - neutral (tracks starts attached only)
			 // 1 - charge (tracks ends&starts attached)
			 // 2 - back neutral (tracks ends attached only)
			 // 3 - neutral, linked (has common track with other vertex)
			 // 4 - charge, linked
			 // 5 - back neutral, linked
  Int_t	    eID;
  Float_t   eQuality;	 // Probability/(vsigmax**2+vsigmay**2)

  VERTEX::Vertex *eV;    // pointer to VtVertex object
  
 public:
  EdbVertex();
  //EdbVertex(EdbVertex &v);
  virtual ~EdbVertex();

  //  int MakeV( bool usemom = true, bool usesegpar=false );

  void    SetV(VERTEX::Vertex *v) {eV=v;}
  VERTEX::Vertex *V() const {return eV;}
  void Clear();
  void ClearNeighborhood();

  int N()  const {return eVTa.GetSize();}
  int Nn() const {return eVTn.GetSize();}
  int Nv();

  Float_t X() const { return eX;}
  Float_t Y() const { return eY;}
  Float_t Z() const { return eZ;}
  Float_t VX() const { if (eV) return (eV->vx() + eX); else return 1000000.;}
  Float_t VY() const { if (eV) return (eV->vy() + eY); else return 1000000.;}
  Float_t VZ() const { if (eV) return (eV->vz() + eZ); else return 1000000.;}
  
  void SetXYZ( float x, float y, float z) { eX=x; eY=y; eZ=z;} 

  void SetFlag( int flag = 0 ) { eFlag = flag; }
  Int_t Flag() { return eFlag; }

  void SetQuality( float q = 0 ) { eQuality = q; }
  Float_t Quality() { return eQuality; }

  void SetID( int ID = 0 ) { eID = ID; }
  Int_t ID() { return eID; }

  TList *VTa() { return &eVTa; }
  TList *VTn() { return &eVTn; } 
  EdbVTA *GetVTa(int i) { return (EdbVTA*)(eVTa.At(i)); }
  EdbVTA *GetVTn(int i) { return (EdbVTA*)(eVTn.At(i)); }
  void AddVTA(EdbVTA *vta);
  void ResetTracks();
  //  EdbVTA *AddTrack(EdbTrackP *track, int zpos, float ProbMin = 0.);
  EdbTrackP *GetTrack(int i) { return GetVTa(i)->GetTrack(); }
  Int_t      Zpos(int i)     { return GetVTa(i)->Zpos(); }
  EdbVertex *GetConnectedVertex(int nv);
  EdbVertex *GetConnectedVertexForTrack(int it);
  EdbVTA *CheckImp(const EdbTrackP *tr, float ImpMax, int zpos, float dist);
  float Impact(int i);
  float Chi2Track(EdbTrackP *tr, int zpos, float X0 = 0.);
  float DistTrack(EdbTrackP *tr, int zpos, float X0 = 0.);
  float DistSeg(EdbSegP *seg, float X0 = 0.);
  float ImpTrack(int i);
  float MaxAperture();

  bool EstimateVertexMath( float& xv, float& yv, float& zv, float& d );

  bool Edb2Vt( const EdbTrackP& tr, VERTEX::Track& t, float X0 = 0., float m = 0.139 );
  bool Edb2Vt( const EdbSegP& s, VERTEX::Track& t,float X0 = 0., float m = 0.139 );

  void Print();

  Int_t Compare(const TObject *o) const;
  Bool_t IsSortable() const { /*printf("Inside issortable\n");*/ return kTRUE;}
  Bool_t IsEqual(const TObject *o) const;
  ULong_t Hash() const { /*printf("Inside hash\n");*/ return eID; }
  ClassDef(EdbVertex,2)  // vertex class for OPERA emulsion data
};

//_________________________________________________________________________
class EdbVertexRec: public TObject {

 private:

  EdbVertex   *eVertex;
  EdbVertex   *eWorking;

 public:
  TList        eVTA;          // vertex-track associations

  TObjArray   *eEdbTracks;
  TObjArray   *eVTX;          // array of vertex
  EdbPVRec    *ePVR;          // patterns volume (optional)
  
  Float_t     eZbin;          // z- granularity (default is 100 microns)
  Float_t     eAbin;          // safety margin for angular aperture of vertex products
  Float_t     eDZmax;         // maximum z-gap in the track-vertex group
  Float_t     eProbMin;       // minimum acceptable probability for chi2-distance between tracks
  Float_t     eImpMax;        // maximal acceptable impact parameter (preliminary check)
  Bool_t      eUseMom;        // use or not track momentum for vertex calculations
  Bool_t      eUseSegPar;     // use only the nearest measured segments for vertex fit (as Neuchatel)
  Int_t       eQualityMode;   // vertex quality estimation method (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average track-vertex distance)

 public:
  EdbVertexRec();
  virtual ~EdbVertexRec();


  int Nvtx() const { if (eVTX) return eVTX->GetEntries(); else return 0; }

  int MakeV( EdbVertex &edbv );
  EdbVTA *AddTrack(EdbVertex &edbv, EdbTrackP *track, int zpos );

  int FindVertex();

  void FillTracksStartEnd(TIndexCell &starts, TIndexCell &ends );

  int LoopVertex( TIndexCell &list1, TIndexCell &list2, 
		  int zpos1, int zpos2);

  float CheckImpact( EdbSegP *s1,   EdbSegP *s2,
		     int zpos1,     int zpos2,    float pv[3] = 0  );

  int ProbVertex( EdbTrackP *tr1,   EdbTrackP *tr2,
		  int zpos1,        int zpos2,    float pv[3] = 0  );

  EdbVertex *ProbVertex1( EdbTrackP *tr1,   EdbTrackP *tr2,
		  int zpos1,        int zpos2,    float pv[3] = 0  );

  void AddVertex(EdbVertex *vtx) {
    if(!eVTX) eVTX = new TObjArray();
    eVTX->Add((TObject*)vtx);
  }

  void AddVTA(EdbVTA *vta) {
    eVTA.Add((TObject*)vta);
  }

  int     ProbVertexN();
  void    StatVertexN();
  int	  LinkedVertexes();

  int     AddSegmentToVertex(EdbSegP *s, float ImpMax = 25., float ProbMin = 0.01, float Mom = 0.3);
  int     VertexPolish(EdbVertex *v, int refill = 0, float RadMax = 1000., int Dpat = 2, float ImpMax = 25., float ProbMin = 0.01, float Mom = 0.3);
  void    AcceptPolish();
  void    RejectPolish();
  int	  VertexTuning(int seltype = 0);
  EdbVertex *AddTrackToVertex(EdbVertex *eVertex, EdbTrackP *eTr, int zpos);
  EdbVertex *RemoveTrackFromVertex(EdbVertex *eVertex, int itr);
  void	  AcceptModifiedVTX(EdbVertex *eVertex, EdbVertex *eWorking);
  void    CancelModifiedVTX(EdbVertex *eVertex, EdbVertex *eWorking);
  double  MoveTrackToOtherVertex(EdbVertex *v2, int it2max, EdbVertex *v1, int seltype,
				 EdbVertex **v2n, EdbVertex **v1n);

  int	  VertexNeighbor(float RadMax = 1000., int Dpat = 1, float ImpMax = 1000000.);
  int	  VertexNeighbor(EdbVertex *v, float RadMax = 1000., int Dpat = 1, float ImpMax = 1000000.);
  int	  SelVertNeighbor( EdbVertex *v, int seltype, float RadMax, int Dpat, TObjArray *ao);
  int	  SelSegNeighbor( EdbSegP *s, int seltype, float RadMax, int Dpat, TObjArray *ao);
  int	  SegmentNeighbor(EdbSegP *s, float RadMax = 1000., int Dpat = 1, float ImpMax = 1000000.,
			  TObjArray *aseg = 0, TObjArray *atr = 0, TObjArray *arv = 0);

  TTree *init_tracks_tree(const char *file_name, EdbTrackP *track);
  int BuildTracksArr(const char *file_name="linked_tracks.root", int nsegMin=2 );
  double Tdistance(const VERTEX::Track& t1, const VERTEX::Track& t2);
  double Tdistance(const EdbSegP& s1, const EdbSegP& s2);
  double TdistanceChi2(const EdbTrackP& tr1, const EdbTrackP& tr2);
  double TdistanceChi2(const EdbSegP& s1, const EdbSegP& s2, float m);

  EdbTrackP *GetEdbTrack( const int index );

  void SetPVRec(EdbPVRec *pvr) { ePVR =pvr; }

  ClassDef(EdbVertexRec,2) //reconstruct vertexes in OPERA emulsion data
};

#endif /* ROOT_EdbVertex */
