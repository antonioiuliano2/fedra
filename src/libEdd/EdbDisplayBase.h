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

//_________________________________________________________________________
class EdbDisplayBase: public TObject {

protected:
  Float_t vx0,vy0,vz0,vx1,vy1,vz1;

protected:
   Int_t             fZoomMode;             //=1 if in zoom mode
   Bool_t            fDrawAllViews;         //Flag True if AllViews selected
   Float_t           fTheta;                //Viewing angle theta
   Float_t           fPhi;                  //Viewing angle phi
   Float_t           fPsi;                  //Viewving angle psi (rotation on display)
   Float_t           fZoomX0[20];           //Low x range of zoom number i
   Float_t           fZoomY0[20];           //Low y range of zoom number i
   Float_t           fZoomX1[20];           //High x range of zoom number i
   Float_t           fZoomY1[20];           //High y range of zoom number i
   Int_t             fZooms;                //Number of zooms
   TCanvas          *fCanvas;               //Pointer to the display canvas
   TPad             *fTrigPad;              //Pointer to the trigger pad
   TPad             *fButtons;              //Pointer to the buttons pad
   TPad             *fPad;                  //Pointer to the event display main pad
   TButton          *fPickButton;           //Button to activate Pick mode
   TButton          *fZoomButton;           //Button to activate Zoom mode
   TArc             *fArcButton;            //Gren/Red button to show Pick/Zoom mode
   TPaveText	    *fProb;		    //Vertex probability
   TPaveText	    *fNewProb;		    //Modified Vertex probability

public:

   EdbDisplayBase();
   EdbDisplayBase( const char *title, 
		   Float_t x0, Float_t x1, 
		   Float_t y0, Float_t y1, 
		   Float_t z0, Float_t z1 );
   ~EdbDisplayBase(){};

   virtual void      Set0();
   virtual void      Refresh() {}
   virtual void      ExecuteEvent(Int_t event, Int_t px, Int_t py);
   virtual void      DisplayButtons();
   virtual void      SetView(Float_t theta, Float_t phi, Float_t psi=0);
   virtual void      DrawAllViews();
   virtual void      DrawViewGL();
   virtual void      DrawViewX3D();
   virtual void      DrawTitle(Option_t *option="");
   virtual void      DrawProb(double prob);
   virtual void      DrawNewProb(double prob);
   virtual void      ClearNewProb();
   virtual void      DrawView(Float_t theta, Float_t phi, Float_t psi=0);
   virtual void      SetPickMode();
   virtual void      SetRange(Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1);
   virtual void      SetZoomMode();
   virtual void      UnZoom(); // *MENU*
   virtual void      Draw(Option_t *option="");
   virtual Int_t     DistancetoPrimitive(Int_t px, Int_t py);

  ClassDef(EdbDisplayBase,1) //basic class for Display
};

#endif /* ROOT_EdbDisplayBase */
