#ifndef ROOT_EdbDisplayBase
#define ROOT_EdbDisplayBase

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplayBase                                                       //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TObject.h>
#include "TCanvas.h"
#include "TSlider.h"
#include "TView.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TClonesArray.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "TButton.h"

#include <TGClient.h>
#include <TGButton.h>

#include <TTree.h>
#include <TButton.h>
#include <TCanvas.h>
#include <TView.h>
#include <TPaveLabel.h>
#include <TPaveText.h>
#include <TDiamond.h>
#include <TArc.h>
#include <TSliderBox.h>
#include <TGaxis.h>
#include <TVirtualX.h>
#include <TMath.h>

static const int kMAXZOOMS=30;

//_________________________________________________________________________
class Edb3DView: public TView {

protected:

   bool fRotateMode;

public:

   Edb3DView() : TView(1) { fRotateMode = false;};
   ~Edb3DView() {};

   virtual const char *	GetTitle() const;
   virtual const char *	GetName() const;
   virtual char *    GetObjectInfo(int px, int py) const;
   virtual void  ExecuteRotateView(Int_t event, Int_t px, Int_t py);
   virtual void  SetRotateMode( bool Mode = true );

   ClassDef(Edb3DView,1) //Event Display
};

//_________________________________________________________________________
class EdbDisplayBase: public TNamed {

protected:
  Float_t vx0,vy0,vz0,vx1,vy1,vz1;

protected:
   Int_t             fZoomMode;             //=1 if in zoom mode
   Bool_t            fDrawAllViews;         //Flag True if AllViews selected
   Float_t           fTheta;                //Viewing angle theta
   Float_t           fPhi;                  //Viewing angle phi
   Float_t           fPsi;                  //Viewving angle psi (rotation on display)
   Double_t          fZoomX0[kMAXZOOMS];    //Low x range of zoom number i
   Double_t          fZoomY0[kMAXZOOMS];    //Low y range of zoom number i
   Double_t          fZoomX1[kMAXZOOMS];    //High x range of zoom number i
   Double_t          fZoomY1[kMAXZOOMS];    //High y range of zoom number i
   Int_t             fZooms;                //Number of zooms
   Int_t             fIndVert;		    //Index of selected vertex in ArrV
   TCanvas          *fCanvas;               //Pointer to the display canvas
   Edb3DView        *fView;                 //Main View object
   char		    fCanvasName[128];       //Name of main canvas
   char       	    fTitle[128];            //EdbDisplay Object Name
   TPad             *fTrigPad;              //Pointer to the trigger pad
   TPad             *fButtons;              //Pointer to the buttons pad
   TButton          *fAllButton;            //Button to draw all objects
   TButton          *fEnvButton;            //Button to draw vertex environment
   TButton          *fUndButton;            //Button to undo vertex modofication
   TButton          *fAccButton;            //Button to accept modified vertex
   TButton          *fCanButton;            //Button to cancel modified vertex
   TButton          *fPickButton;           //Button to activate Pick mode
   TButton          *fZoomButton;           //Button to activate Zoom mode
   TButton          *fUnZoomButton;         //Button to Undo previous Zoom
   TArc             *fArcButton;            //Gren/Red button to show Pick/Zoom mode
   TGMainFrame      *fMain;                 //Dialog frame
   TText	    *fHdrVTX;		    //Vertex data header
   TText	    *fOldVTX;		    //Old Vertex data
   TText	    *fNewVTX;		    //Modified Vertex data
   TText	    *fPreVTX;		    //Previous Vertex data
   TPaveText        *fVTXTracks;	    //Vertex tracks information
   TButton	    *fOldBut;		    //Old Vertex display button
   TButton	    *fNewBut;		    //Modified Vertex display button
   TButton	    *fPreBut;		    //Previous Vertex display button
   TButton          *fRemBut[50];           //track removing buttons
   
public:

   TCanvas          *fCanvasVTX;            //Pointer to the vertex canvas
   TPad             *fPad;                  //Pointer to the event display main pad

public:

   EdbDisplayBase();
   EdbDisplayBase( const char *title, 
		   Float_t x0, Float_t x1, 
		   Float_t y0, Float_t y1, 
		   Float_t z0, Float_t z1 );
   ~EdbDisplayBase();

//   virtual TCanvas   *GetCanvas() { return fCanvas; }
//   virtual TCanvas   *GetCanvasVTX() { return fCanvasVTX; }
   virtual void      Set0();
   virtual void      Refresh() {}
   virtual void      ExecuteEvent(Int_t event, Int_t px, Int_t py);
   virtual void      DisplayButtons();
   virtual void      SetView(Float_t theta, Float_t phi, Float_t psi=0);
   virtual void      DrawAllViews();
   virtual void      DrawViewGL();
   virtual void      DrawViewX3D();
   virtual void      DrawTitle(Option_t *option="");
   virtual void      DrawPreVTX(char *text);
   virtual void      DrawOldVTX(char *text);
   virtual void      DrawNewVTX(char *text);
   virtual void      DrawPreBut(char *text);
   virtual void      DrawOldBut(char *text);
   virtual void      DrawNewBut(char *text);
   virtual void      ClearPreVTX();
   virtual void      ClearNewVTX();
   virtual void      CreateCanvasVTX();
   virtual void      DrawUnd();
   virtual void      DrawAcc();
   virtual void      DrawCan();
   virtual void      DrawEnv();
   virtual void      DrawView(Float_t theta, Float_t phi, Float_t psi=0);
   virtual void      SetPickMode();
   virtual void      SetRange(Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1);
   virtual void      SetZoomMode();
   virtual void      UnZoom(); // *MENU*
   virtual void      Draw(Option_t *option="");
   virtual Int_t     DistancetoPrimitive(Int_t px, Int_t py);
   virtual char *    GetObjectInfo(int px, int py) const;
   virtual void      SetRotate();

  ClassDef(EdbDisplayBase,1) //basic class for FEDRA Event Display
};

#endif /* ROOT_EdbDisplayBase */
