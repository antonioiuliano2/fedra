#ifndef ROOT_EdbVertexComb
#define ROOT_EdbVertexComb
#include "EdbVertex.h"
#include "EdbScanCond.h"

//_________________________________________________________________________
class EdbVertexComb: public TObject {

 public:
  TObjArray    eTracks;          // input EdbTracks
  EdbScanCond  eCond;
  EdbVertexPar eVPar;
  Float_t      eZ0;              // input parameter: the first approximation for thr vertex z-position
  
  TObjArray eVertices;        // vertices
//  TObjArray eOther;           // 

  EdbVertexComb() {}
  EdbVertexComb( TObjArray &tracks );
  virtual ~EdbVertexComb();
  
  void        SetTracksErrors(EdbScanCond &cond) { eCond = cond; SetTracksErrors(); }
  void        SetTracksErrors() { SetTracksErrors(eTracks,eCond); }
  void        SetTracksErrors(TObjArray &tracks, EdbScanCond &cond);
  float       SelectSortVertices(int nprongMin, float probMin);
  EdbVertex  *CheckVTX(TObjArray &tracks);
  void        PrintSelectedVTX();
  

  ClassDef(EdbVertexComb,1) //combinatorial selection of the best vertex combinations
};

#endif /* ROOT_EdbVertexComb */
