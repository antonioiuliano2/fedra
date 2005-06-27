#ifndef ROOT_EdbDisplay
#define ROOT_EdbDisplay

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TGNumberEntry.h"
#include "EdbDisplayBase.h"
#include "EdbPVRec.h"
#include "EdbVertex.h"

class EdbSegG;

//_________________________________________________________________________
class EdbDisplay: public EdbDisplayBase {

 private:

  Int_t      eDrawTracks;  // tracks drawing option

  Int_t      eDrawVertex;  // vertex drawing option

  TArrayI    *eColors;
  TArrayF    *eDZs;

  TGNumberEntry *fNumericEntries[3];

 public:

  EdbVertexRec *eVerRec;
  TObjArray *eArrSegP;     // array of segments to be drawn
  TObjArray *eArrTr;       // array of tracks to be drawn
  TObjArray *eArrV;        // array of vertexes to be drawn
  TObjArray *eArrSegPSave;     // saved array of segments to be drawn
  TObjArray *eArrTrSave;   // saved array of tracks to be drawn
  TObjArray *eArrVSave;    // saved array of vertexes to be drawn
  EdbVertex *eWorking;     // working vertex
  EdbVertex *eVertex;      // current selected vertex
  EdbVertex *ePrevious;    // saved previous vertex modifications
  EdbSegP   *eSegment;     // working segment (for segment neighborhood)
  TPolyMarker3D *eSegPM;   // green mark for segment selected as working
  Bool_t eWait_Answer;	   // set TRUE when answer received
  Int_t eIndVert;	   // Index of selected vertex in ArrV
  Int_t eIndVertSave;	   // Index of selected vertex in ArrV (seved)
  TList eCreatedTracks;    // list of tracks, created during vertex operations 
  Double_t eRadMax;        // Maximal Radius for neighborhood
  Double_t eDpat;          // +/- patterns for neighborhood
  Double_t eImpMax;        // Maximal impact for neighborhood

 public:

  EdbDisplay() : EdbDisplayBase() { Set0(); };
  ~EdbDisplay();

  EdbDisplay(const char *title, 
	     Float_t x0, Float_t x1, 
	     Float_t y0, Float_t y1, 
	     Float_t z0, Float_t z1) : 
  EdbDisplayBase(title, x0, x1, y0, y1, z0, z1)  { Set0(); };

  EdbDisplay(const char *title, EdbLayer &la) : 

	EdbDisplayBase(title, la.Xmin(), la.Xmax(), la.Ymin(), la.Ymax(), 
		       la.Zmin(), la.Zmax())
		    				 { Set0(); };

  static EdbDisplay *EdbDisplayExist(const char *title);
  void Delete();
  void Set0();
  void SetVerRec(EdbVertexRec *evr) { eVerRec = evr; };

  void Refresh();
  void SetArrSegP(TObjArray *arr);
  void SetArrTr(TObjArray *arr);
  void SetDrawTracks(int opt) {eDrawTracks=opt;}

  EdbVertexRec *VerRec() const {return eVerRec;}
  //void PatternDraw(EdbPattern &pat);
  void TrackDraw(EdbTrackP *tr);
  EdbSegG *SegLine(const EdbSegP *seg);

  void SetArrV(TObjArray *arrv);
  void VertexDraw(EdbVertex *v);
  void SetDrawVertex(int opt) {eDrawVertex=opt;}
  void CancelModifiedVTX();
  void DeleteModifiedVTX();
  void AcceptModifiedVTX();
  void DialogModifiedVTX();
  void CloseDialogModifiedVTX();
  void UndoModifiedVTX();
  void DrawVertexEnvironment();
  void DrawAllObjects();
  void DrawVTXTracks(char *type, EdbVertex *v = 0);
  void RemoveTrackFromTable( int ivt = 0 );
  void DialogNeighborParameters();
  void AcceptModifiedParams();
  void CloseDialogModifiedParams();
  void CancelDialogModifiedParams();
  void ClearSegmentEnv();
 
  ClassDef(EdbDisplay,1) //FEDRA Event Display
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
  virtual const char *	GetTitle() const;
  virtual const char *	GetName() const;
  virtual char *	GetObjectInfo(int px, int py) const;

 ClassDef(EdbVertexG,1)  //Vertex
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
  virtual const char *	GetTitle() const;
  virtual const char *	GetName() const;
  virtual char *	GetObjectInfo(int px, int py) const;

 ClassDef(EdbTrackG,1)  //Track
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
  virtual void		AddAsTrack();     // *MENU*
  virtual void		SetAsWorking();   // *MENU*
  virtual const char *	GetTitle() const;
  virtual const char *	GetName() const;
  virtual char *	GetObjectInfo(int px, int py) const;

 ClassDef(EdbSegG,1) //Segment
};


#endif /* ROOT_EdbDisplay */
