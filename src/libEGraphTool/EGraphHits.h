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

class EGraphHits: public TObject {
 public:
  EGraphHits();
  virtual ~EGraphHits();

  // void DrawHits();
  void DrawTracks(const TString status = "all");
  void BuildEvent(EdbPVRec *tracks, const TString status);
  // void BuildEvent(EdbView *event);
  // void ClearEvent();

 private:
  
  // TObjArray       *fAllSegments;
  TObjArray       *fAllPredTracks;
  TObjArray       *fAllFoundTracks;

  ClassDef(EGraphHits,1)
};

#endif  // EGRAPHHITS_H

