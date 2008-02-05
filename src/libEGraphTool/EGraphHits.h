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

// class TPolyMarker3D;
// class TPolyLine3D;
// class TList;
// class TH1;
class EdbView;


class EGraphHits: public TObject {
 public:
  EGraphHits();
  virtual         ~EGraphHits();

  void             DrawHits();
  void             BuildEvent(EdbView *event);
  void             ClearEvent();

 private:
  
  //    TPolyMarker3D   *fAllHits;                          // collection of middle points of each hit
  TObjArray       *fAllSegments;                      // array of lines between two points (begin and end) of each hit
  //    TList           *fList;                             // list of histos to be done
  ClassDef(EGraphHits,1)
};

#endif  // EGRAPHHITS_H

