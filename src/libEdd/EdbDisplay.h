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

class EdbSegG;

//_________________________________________________________________________
class EdbDisplay: public EdbDisplayBase {
 private:

  EdbPVRec  *ePVR;
  TObjArray *eArrSegP;     // array of segments to be drawn
  TObjArray *eArrTr;       // array of tracks to be drawn
  Int_t      eDrawTracks;  // tracks drawing option

  TObjArray *eArrV;        // array of vertexes to be drawn
  Int_t      eDrawVertex;  // vertex drawing option

  TArrayI* eColors;
  TArrayF* eDZs;

 public:

  EdbVertex *eWorking;     // working vertex
  EdbVertex *eVertex;      // current selected vertex

 public:

  EdbDisplay() : EdbDisplayBase() { Set0(); };
  EdbDisplay(const char *title, 
	     Float_t x0, Float_t x1, 
	     Float_t y0, Float_t y1, 
	     Float_t z0, Float_t z1) : 
    EdbDisplayBase(title, x0, x1, y0, y1, z0, z1)  { Set0(); };
  EdbDisplay(const char *title, EdbLayer &la) : 
    EdbDisplayBase(title, la.Xmin(), la.Xmax(), la.Ymin(), la.Ymax(), 
		   la.Zmin(), la.Zmax())  { Set0(); };

  ~EdbDisplay() { if (eWorking) delete eWorking; };


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
  void SetDrawVertex(int opt) {eDrawVertex=opt;}

  ClassDef(EdbDisplay,1) //class to display OPERA emulsion data
};
//_________________________________________________________________________
class EdbVertexG : public TPolyMarker3D {
 private:

  EdbVertex *eV;
  EdbDisplay *eD;

 public:
  EdbVertexG():TPolyMarker3D(1) {eV=0; eD=0;}
  EdbVertexG(EdbDisplay *D):TPolyMarker3D(1) {eV=0; eD=D;}
  virtual ~EdbVertexG(){}

  void SetVertex(EdbVertex *v) {eV=v;}

  virtual void          DumpVertex();    // *MENU*
  virtual void          InspectVertex(); // *MENU*
  virtual void		SetAsWorking();  // *MENU*

 ClassDef(EdbVertexG,1)  //EdbVertex graphics
};

//_________________________________________________________________________
class EdbTrackG : public TPolyMarker3D {
 private:

  EdbTrackP *eTr;
  EdbDisplay *eD;
  
 public:
  EdbTrackG() {eTr=0; eD=0;}
  EdbTrackG(EdbDisplay *D) {eTr=0; eD=D;}
  EdbTrackG(Int_t nhits, EdbDisplay *D):TPolyMarker3D(nhits) {eTr=0; eD=D;}
  virtual ~EdbTrackG(){}

  void SetTrack(EdbTrackP *tr) {eTr=tr;}

  virtual void          DumpTrack();    // *MENU*
  virtual void          InspectTrack(); // *MENU*
  virtual void		RemoveTrack();  // *MENU*
  virtual void		AddTrack();  // *MENU*

 ClassDef(EdbTrackG,1)  //EdbTrack graphics
};

//_________________________________________________________________________
class EdbSegG : public TPolyLine3D {
 private:

  const EdbSegP *eSeg;
  EdbDisplay *eD;

 public:
  EdbSegG() {eSeg=0; eD=0;}
  EdbSegG(EdbDisplay *D) {eSeg=0; eD=D;}
  EdbSegG(Int_t nhits):TPolyLine3D(nhits) {eSeg=0;}
  EdbSegG(Int_t nhits, EdbDisplay *D):TPolyLine3D(nhits) {eSeg=0; eD=D;}
  virtual ~EdbSegG(){}

  void SetSeg(const EdbSegP *s) {eSeg=s;}

  virtual void          DumpSegment(); 	  // *MENU*
  virtual void          InspectSegment(); // *MENU*

 ClassDef(EdbSegG,1) //
};


#endif /* ROOT_EdbDisplay */
