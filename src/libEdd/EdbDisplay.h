#ifndef ROOT_EdbDisplay
#define ROOT_EdbDisplay

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbDisplayBase.h"
#include "EdbPVRec.h"
#include "EdbVertex.h"

//_________________________________________________________________________
class EdbVertexG : public TPolyMarker3D {
 private:

  EdbVertex *eV;

 public:
  EdbVertexG():TPolyMarker3D(1) {eV=0;}
  virtual ~EdbVertexG(){}

  void SetVertex(EdbVertex *v) {eV=v;}

  //virtual void          ExecuteEvent(Int_t event, Int_t px, Int_t py);
  //virtual void          InspectParticle(); // *MENU*
  virtual void          DumpVertex();    // *MENU*
  virtual void          InspectVertex(); // *MENU*

 ClassDef(EdbVertexG,1)  //EdbTrack graphics
};

//_________________________________________________________________________
class EdbTrackG : public TPolyMarker3D {
 private:

  EdbTrackP *eTr;

 public:
  EdbTrackG() {eTr=0;}
  EdbTrackG(Int_t nhits):TPolyMarker3D(nhits) {eTr=0;}
  virtual ~EdbTrackG(){}

  void SetTrack(EdbTrackP *tr) {eTr=tr;}

  //virtual void          ExecuteEvent(Int_t event, Int_t px, Int_t py);
  //virtual void          InspectParticle(); // *MENU*
  virtual void          DumpTrack();    // *MENU*
  virtual void          InspectTrack(); // *MENU*

 ClassDef(EdbTrackG,1)  //EdbTrack graphics
};

//_________________________________________________________________________
class EdbSegG : public TPolyLine3D {
 private:

  const EdbSegP *eSeg;

 public:
  EdbSegG() {eSeg=0;}
  EdbSegG(Int_t nhits):TPolyLine3D(nhits) {eSeg=0;}
  virtual ~EdbSegG(){}

  void SetSeg(const EdbSegP *s) {eSeg=s;}

  //virtual void          ExecuteEvent(Int_t event, Int_t px, Int_t py);
  //virtual void          InspectParticle(); // *MENU*
  virtual void          DumpSegment(); // *MENU*
  virtual void          InspectSegment(); // *MENU*

 ClassDef(EdbSegG,1) //
};

//_________________________________________________________________________
class EdbDisplay: public EdbDisplayBase {
 private:

  EdbPVRec  *ePVR;
  TObjArray *eArrSegP;     // array of segments to be drawn
  TObjArray *eArrTr;       // array of tracks to be drawn
  Int_t      eDrawTracks;  // tracks drawing option

  TObjArray *eArrV;        // array of vertexes to be drawn

  TArrayI* eColors;
  TArrayF* eDZs;

 public:

  EdbDisplay() : EdbDisplayBase(){ Set0(); };

  EdbDisplay(const char *title, 
	     Float_t x0, Float_t x1, 
	     Float_t y0, Float_t y1, 
	     Float_t z0, Float_t z1) : 
    EdbDisplayBase(title, x0, x1, y0, y1, z0, z1)  { Set0(); };
  //  ~EdbDisplay(){};

  void Set0();

  void Refresh();
  void SetPVR(EdbPVRec *pvr) {ePVR=pvr;}
  void SetArrSegP(TObjArray *arr) {eArrSegP=arr;}
  void SetArrTr(TObjArray *arr) {eArrTr=arr;}
  void SetDrawTracks(int opt) {eDrawTracks=opt;}

  EdbPVRec *PVR() const {return ePVR;}
  //void PatternDraw(EdbPattern &pat);
  void TrackDraw(EdbTrackP *tr);
  EdbSegG *SegLine(const EdbSegP *seg);

  void SetArrV(TObjArray *arrv) {eArrV=arrv;}
  void VertexDraw(EdbVertex *v);

  ClassDef(EdbDisplay,1) //class to display OPERA emulsion data
};

#endif /* ROOT_EdbDisplay */
