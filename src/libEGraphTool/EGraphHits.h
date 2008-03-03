//----------------------------------------------------------------------------
//
// Program: EGraphHits
//
//          Drawing emulsion hits
//
//----------------------------------------------------------------------------

#ifndef EGRAPHHITS_H
#define EGRAPHHITS_H

#include <TObject.h>
#include <TString.h>

class EdbView;
class EdbPVRec;
class EdbVertexRec;
class EdbTrackP;

class EGraphHits: public TObject {
 public:
  EGraphHits();
  virtual ~EGraphHits();

  // void DrawHits();
  void DrawTracks(const TString status = "all");
  void BuildEvent(EdbPVRec *tracks, const TString status);
  void BuildVertex(EdbVertexRec *vertexRec);
  void DrawVertex();

  // void BuildEvent(EdbView *event);
  // void ClearEvent();

 private:
  
  void FillSegmentsArray(const EdbTrackP *track, TObjArray *AllTracks);

  // TObjArray       *fAllSegments;
  TObjArray *fAllPredTracks;
  TObjArray *fAllFoundTracks;
  TObjArray *fAllVerticesRec;

  ClassDef(EGraphHits,1)
};

#endif  // EGRAPHHITS_H

