#ifndef ROOT_EdbVertexComb
#define ROOT_EdbVertexComb
#include "EdbVertex.h"
#include "EdbScanCond.h"

//_________________________________________________________________________
class EdbTopology: public TObject {

  private:
    TObjArray eVertices;
    TObjArray eTracks;     // detached tracks - part of the topology but not participated in any vtx
    
  public:
    EdbTopology() {}
    virtual ~EdbTopology(){}
    
    Int_t       Nvtx() const {return eVertices.GetEntries();}
    Int_t       Ntr() const {return eTracks.GetEntries();}
    Float_t     Probability();
    Float_t     DZ();
    EdbVertex  *AddVertex( EdbVertex *v ) { eVertices.Add(v); return v; }
    EdbVertex  *GetVertex( int i ) const { return (EdbVertex *)eVertices.At(i); }
    EdbTrackP  *GetTrack( int i ) const { return (EdbTrackP *)eTracks.At(i); }
    void        AddSingleTracks( TObjArray &tracks ) { eTracks.AddAll(&tracks); }
    void        Print();
    void        PrintTracks();
    char       *TracksStr();
    char       *VertexStr(EdbVertex &v);
    void        OrderVtxByZ();
    bool        IsEqual(EdbTopology &t);
       
    ClassDef(EdbTopology,1) //class to keep the reconstructed topology
};

//_________________________________________________________________________
class EdbVertexComb: public TObject {

  public:
    TObjArray    eTracks;       // input EdbTracks
    
    Int_t        eRecursionMax; // number of vertex/event to be searched
    Int_t        eRecursion;    // recursion level (0 - no recursion)
    EdbScanCond  eCond;
    EdbVertexPar eVPar;
    Float_t      eZ0;           // input parameter: the first approximation for the vertex z-position
    
    Float_t      eProbMinV;     // min probability to accept the vertex for topology calculation
    Int_t        eNProngMinV;   // min prongs to accept the vertex for topology calculation
    
    Float_t      eProbDetached; // default probability for the detached track (1-prong vertex)
    
    TObjArray eVertices;        // output vertices
    TObjArray eOther;           // recursive array of EdbVertexComb objects in multyvertex case

    TObjArray eTopologies;    // found EdbTopologies sorted by rating

  public:
    EdbVertexComb() { Set0(); }
    EdbVertexComb( TObjArray &tracks );
    virtual ~EdbVertexComb();

    void        Set0();
    void        CopyPar( EdbVertexComb &comb );
    void        AddTrack( EdbTrackP *t ) { eTracks.Add(t); }
    void        SetTracksErrors(EdbScanCond &cond) { eCond = cond; SetTracksErrors(); }
    void        SetTracksErrors() { SetTracksErrors(eTracks,eCond); }
    void        SetTracksErrors(TObjArray &tracks, EdbScanCond &cond);
//    void        SelectSortVertices();

    void       FormVertices();
    void       FindTopologies();
    
    int         Ntr() const {return eTracks.GetEntries();}
    int         Nvtx() const {return eVertices.GetEntries();}
    EdbVertex  *GetVertex( int i ) const { return (EdbVertex *)eVertices.At(i); }
    EdbVertexComb  *GetVertexComb( int i ) const { return (EdbVertexComb *)eOther.At(i); }
    
    bool        IsAcceptable(EdbVertex &v);
    EdbVertex  *CheckVTX(TObjArray &tracks);
    void        PrintTracks();
    void        SelectTopologies(TObjArray &topoarr);
    void        SortTopologies(TObjArray &topoarr);
    void        ClearDoublets(TObjArray &topoarr);
    void        PrintTeoricalCombinations(Int_t n);
    
    ClassDef(EdbVertexComb,1) //combinatorial selection of the best vertex combinations
};
#endif /* ROOT_EdbVertexComb */
