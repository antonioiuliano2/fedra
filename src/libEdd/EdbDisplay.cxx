//-- Author of drawing part : Igor Kreslo     27.11.2003
//   Based on AliDisplay class (AliRoot framework - ALICE CERN)
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//
//
// Usage: (script testd.C)
//
//	EdbDisplay*  ds;
//	void testd()
//	{
//	gSystem->Load("libEdd.so");
//	gStyle->SetPalette(1);
//	ds=new EdbDisplay("display-t",-6000.,6000.,-6000.,6000.,-20050,110.);
//	ds->SetCuts(-9000,9000,-9000,9000); //Coordinate cuts
//	  TTree *tree;
//	  TFile *f = new TFile("linked_tracks.root");
//	  if (f)  tree = (TTree*)f->Get("tracks");
//	  ds->SetAffine(1,0,0,1,0,0);
//	  ds->SetNsegmin(4);
//	  ds->AddTracksTree(tree,0.,-1.,1.,-1.,1.);
//	  ds->Draw();
//	}
//
//



#include "EdbDisplay.h"
#include "EdbPVRec.h"
#include "TSlider.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPolyMarker3D.h"
#include <cmath>


static const int kMAXZOOMS=20;

ClassImp(EdbDisplay);
ClassImp(EdbSegG);
ClassImp(EdbTrackG);

//_____________________________________________________________________________
void EdbTrackG::DumpTrack()
{
  if (eTr) eTr->Print();
}

//_____________________________________________________________________________
void EdbTrackG::InspectTrack()
{
  if (eTr) eTr->Inspect();
}


//_____________________________________________________________________________
void EdbSegG::DumpSegment()
{
  if (eSeg) eSeg->Print();
}

//_____________________________________________________________________________
void EdbSegG::InspectSegment()
{
  if (eSeg) eSeg->Inspect();
}


//________________________________________________________________________
EdbDisplay::EdbDisplay()
{
   //
  // Default constructor
  //
  fCanvas = 0;
  fTrigPad = 0;
  fCutPad = 0;
  fEtaPad = 0;
  fButtons = 0;
  fPad = 0;
  fCutSlider = 0;
  fEtaSlider = 0;
  fRangeSlider = 0;
  fPickButton = 0;
  fZoomButton = 0;
  fArcButton = 0;
  fFruits = 0;
       cutTX0=-.9;
      cutTX1=.9;
      cutTY0=-.9;
      cutTY1=.9;
      cutX0=-100000.;
      cutX1=100000.;
      cutY0=-100000.;
      cutY1=100000.;
      eNsegmax=100;
      eNsegmin=2;
      eNpieces=0;
      eTr_Co=kFALSE; // Default - show couples
      MaxChi=kTRUE;

      ePVR=0;
}

//________________________________________________________________________
EdbDisplay::EdbDisplay(const char *title, Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1)
{
      ePVR=0;

      fPad=0;
 //     SetRange();
      vx0=x0;vx1=x1;vy0=y0;vy1=y1;vz0=z0;vz1=z1;
       cutTX0=-.9;
      cutTX1=.9;
      cutTY0=-.9;
      cutTY1=.9;
      cutX0=-100000.;
      cutX1=100000.;
      cutY0=-100000.;
      cutY1=100000.;
      eNsegmax=100;
      eNsegmin=2;
      eNpieces=0;
      eTr_Co=kFALSE; // Default - show couples
      MaxChi=kTRUE;
/*/     (new TButton("Best Chi2","((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kTRUE;((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Refresh();",0.1,0.96,0.3,0.99))->Draw();
 //     (new TButton("All Chi2","((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kFALSE;((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Refresh();",0.4,0.96,0.7,0.99))->Draw();
      fTXSlider = new TSlider("xslider","x",0.1,0.01,0.98,0.045);
      fTXSlider->SetObject(this);
      fTXSlider->SetRange(.5+cutTX0/2.,.5+cutTX1/2.);
      fTYSlider = new TSlider("yslider","y",0.01,0.1,0.045,0.98);
      fTYSlider->SetObject(this);
      fTYSlider->SetRange(.5+cutTY0/2.,.5+cutTY1/2.);
*/
      colors = new TArrayI();
      DZs = new TArrayF();

   fPad = 0;
   // Set front view by default
   fTheta = 0;
   fPhi   = -90;
   fPsi   = 0;
   fDrawAllViews  = kFALSE;
   fZoomMode      = 1;
   fZooms         = 0;
   fCanvas = new TCanvas("Canvas", "Emulsion Data Display",14,47,800,700);
   fCanvas->ToggleEventStatus();
  // Create main display pad
   fPad = new TPad("viewpad", "Emulsion display",0.15,0,0.97,0.96);
   fPad->Draw();
   fPad->Modified();
   fPad->SetFillColor(1);
   fPad->SetBorderSize(2);
  // Create user interface control pad
   DisplayButtons();
   fCanvas->cd();
   // Create Range and mode pad
   Float_t dxtr     = 0.15;
   Float_t dytr     = 0.45;
   fTrigPad = new TPad("trigger", "range and mode pad",0,0,dxtr,dytr);
   fTrigPad->Draw();
   fTrigPad->cd();
   fTrigPad->SetFillColor(22);
   fTrigPad->SetBorderSize(2);
//   fRangeSlider = new TSlider("range","range",0.7,0.42,0.9,0.98);
//   fRangeSlider->SetObject(this);
   char pickmode[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->SetPickMode()";
   Float_t db = 0.09;
   fPickButton = new TButton("Pick",pickmode,0.05,0.32,0.65,0.32+db);
   fPickButton->SetFillColor(38);
   fPickButton->Draw();
   char zoommode[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->SetZoomMode()";
   fZoomButton = new TButton("Zoom",zoommode,0.05,0.21,0.65,0.21+db);
   fZoomButton->SetFillColor(38);
   fZoomButton->Draw();
   fArcButton = new TArc(.8,fZoomButton->GetYlowNDC()+0.5*db,0.33*db);
   fArcButton->SetFillColor(kGreen);
   fArcButton->Draw();
   char butUnzoom[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->UnZoom()";
   TButton *button = new TButton("UnZoom",butUnzoom,0.05,0.05,0.95,0.15);
   button->SetFillColor(38);
   button->Draw();

   fTrigPad->SetEditable(kFALSE);
   fButtons->SetEditable(kFALSE);


   fCanvas->cd();
   fCanvas->Update();
   gROOT->GetListOfSpecials()->Add(this);
//       SetRange(x1-x0,z1-z0);
 //      fPad->cd();
 //  AppendPad(); // append display object as last object to force selection
//    Draw();


}


//_____________________________________________________________________________
void EdbDisplay::DisplayButtons()
{
//    Create the user interface buttons

   fButtons = new TPad("buttons", "newpad",0,0.45,0.15,1);
   fButtons->Draw();
   fButtons->SetFillColor(38);
   fButtons->SetBorderSize(2);
   fButtons->cd();

   Int_t butcolor = 33;
   Float_t dbutton = 0.07;
   Float_t y  = 0.96;
   Float_t dy = 0.014;
   Float_t x0 = 0.05;
   Float_t x1 = 0.95;

   TButton *button;
   char but1[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kTRUE; ((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Draw();";
   button = new TButton("Best CHI2",but1,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
//   button->Draw();

   y -= dbutton +dy;
   char but2[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kFALSE; ((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Draw();";
   button = new TButton("All Chi2",but2,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
//   button->Draw();

   y -= dbutton +dy;
   char but3[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->SetView(90,-90,90)";
   button = new TButton("Top View",but3,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but4[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->SetView(90,0,90)";
   button = new TButton("Side View",but4,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but5[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->SetView(0,-90,0)";
   button = new TButton("Front View",but5,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but6[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->DrawAllViews()";
   button = new TButton("All Views",but6,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
//   button->Draw();

   y -= dbutton +dy;
   char but7[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->DrawViewGL()";
   button = new TButton("OpenGL",but7,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but8[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->DrawViewX3D()";
   button = new TButton("X3D",but8,x0,y-dbutton,x1,y);
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but9[] = "((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->DrawTracks()";
   button = new TButton("TRACKS",but9,x0,y-dbutton,x1,y);
   button->SetFillColor(30);
//   button->Draw();

   // display logo
   TDiamond *diamond = new TDiamond(0.05,0.015,0.95,0.22);
   diamond->SetFillColor(50);
   diamond->SetTextAlign(22);
   diamond->SetTextColor(5);
   diamond->SetTextSize(0.11);
   diamond->Draw();
   diamond->AddText(".. ");
   diamond->AddText("ROOT");
   diamond->AddText("OPERA");
   diamond->AddText("... ");
   diamond->AddText(" ");
}



//_____________________________________________________________________________
void EdbDisplay::DrawTitle(Option_t *option)
{
//    Draw the event title

   Float_t xmin = gPad->GetX1();
   Float_t xmax = gPad->GetX2();
   Float_t ymin = gPad->GetY1();
   Float_t ymax = gPad->GetY2();
   Float_t dx   = xmax-xmin;
   Float_t dy   = ymax-ymin;

   if (strlen(option) == 0) {
      TPaveText *title = new TPaveText(xmin +0.01*dx, ymax-0.09*dy, xmin +0.5*dx, ymax-0.01*dy);
      title->SetBit(kCanDelete);
      title->SetFillColor(42);
      title->Draw();
      char ptitle[100];
      sprintf(ptitle,"OPERA emulsion view");
      title->AddText(ptitle);
 //     Int_t nparticles = gAlice->Particles()->GetEntriesFast();
//      sprintf(ptitle,"Nparticles = %d  Nhits = %d",nparticles, fHitsCuts);
//      title->AddText(ptitle);
   } else {
      TPaveLabel *label = new TPaveLabel(xmin +0.01*dx, ymax-0.07*dy, xmin +0.2*dx, ymax-0.01*dy,option);
      label->SetBit(kCanDelete);
      label->SetFillColor(42);
      label->Draw();
   }
}


//_____________________________________________________________________________
void EdbDisplay::SetRange(Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1)
{
   
   vx0=x0;vx1=x1;vy0=y0;vy1=y1;vz0=z0;vz1=z1;

   if (!fPad) return;
   fPad->Clear();
   Draw();
}


//_____________________________________________________________________________
void EdbDisplay::SetView(Float_t theta, Float_t phi, Float_t psi)
{
//  change viewing angles for current event

   fPad->cd();
   fDrawAllViews = kFALSE;
   fPhi   = phi;
   fTheta = theta;
   fPsi   = psi;
   Int_t iret = 0;

   TView *view = gPad->GetView();
   if (view) view->SetView(fPhi, fTheta, fPsi, iret);
   else      Draw();

   gPad->Modified();
}



//_____________________________________________________________________________

void EdbDisplay::DrawView(Float_t theta, Float_t phi, Float_t psi)
{
//    Draw a view of DataSet

   gPad->SetCursor(kWatch);
   gPad->SetFillColor(1);
   gPad->Clear();

   Int_t iret;
   TView *view = new TView(1);
 //  Float_t range = fRrange*fRangeSlider->GetMaximum();
//  Float_t range = fRrange;
   view->SetRange(vx0,vy0,vz0,vx1,vy1,vz1);
   fZoomX0[0] = -1;
   fZoomY0[0] = -1;
   fZoomX1[0] =  1;
   fZoomY1[0] =  1;
//   fZooms = 0;


   Refresh();
//   gPad->GetListOfPrimitives()->AddFirst(this);
   AppendPad();

   view->SetView(phi, theta, psi, iret);

   gPad->Range(fZoomX0[fZooms],fZoomY0[fZooms],fZoomX1[fZooms],fZoomY1[fZooms]);
   gPad->Modified(kTRUE);
//   fPad->cd();
}



//_____________________________________________________________________________
void EdbDisplay::DrawViewGL()
{
//    Draw current view using OPENGL

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   pad->x3d("OPENGL");
}

//_____________________________________________________________________________
void EdbDisplay::DrawViewX3D()
{
//    Draw current view using X3D

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   pad->x3d();
}


//_____________________________________________________________________________
void EdbDisplay::Draw(Option_t *)
{
//    Display current event

   if (fDrawAllViews) {
      DrawAllViews();
      return;
   }

   fPad->cd();

   DrawView(fTheta, fPhi, fPsi);

   // Display the event number and title   TODO???
   //fPad->cd();
   //DrawTitle();
//   fPad->cd();

}


//_____________________________________________________________________________
void EdbDisplay::DrawAllViews()
{
//    Draw front,top,side and 30 deg views

   fDrawAllViews = kTRUE;
   fPad->cd();
   fPad->SetFillColor(15);
   fPad->Clear();
   fPad->Divide(2,2);

   // draw 30 deg view
   fPad->cd(1);
   DrawView(30, 30, 0);
   DrawTitle();

   // draw front view
   fPad->cd(2);
   DrawView(0, -90,0);
   DrawTitle("Front");

   // draw top view
   fPad->cd(3);
   DrawView(90, -90, 90);
   DrawTitle("Top");

   // draw side view
   fPad->cd(4);
   DrawView(90, 0, -90);
   DrawTitle("Side");

   fPad->cd(2);
}
//_____________________________________________________________________________
void EdbDisplay::SetPickMode()
{
  //
  // Set Pick Mode -- disable zoom
  //
   fZoomMode = 0;

   fArcButton->SetY1(fPickButton->GetYlowNDC()+0.5*fPickButton->GetHNDC());
   fTrigPad->Modified();
}

//_____________________________________________________________________________
void EdbDisplay::SetZoomMode()
{
  //
  // Set Zoom Mode -- disable pick
  //
   fZoomMode = 1;

   fArcButton->SetY1(fZoomButton->GetYlowNDC()+0.5*fZoomButton->GetHNDC());
   fTrigPad->Modified();
}


//______________________________________________________________________________
void EdbDisplay::UnZoom()
{
  //
  // Resets ZOOM
  //
  if (fZooms <= 0) return;
  fZooms--;
  TPad *pad = (TPad*)gPad->GetPadSave();
  pad->Range(fZoomX0[fZooms],fZoomY0[fZooms], fZoomX1[fZooms],fZoomY1[fZooms]);
  pad->Modified();
  pad->cd();
}

//________________________________________________________________________
void EdbDisplay::Clears()
{
/*
//  TObject* obj;
  TList* lst=fImagepad->GetListOfPrimitives();
  Int_t N=lst->GetSize();
  for(int e=0;e<N;e++) {lst->RemoveAt(e);}
//  TIter next(lst);
//  while (obj = next()) {lst->Remove(obj);};
//  fImagepad->GetListOfPrimitives()->Changed();
 //     fImagepad->GetListOfPrimitives()->Clear();
      fImagepad->cd();
//       TView* view = new TView(1);
     AppendPad();
 //    fImagepad->Draw();
//      view->SetRange(vx0,vy0,vz0,vx1,vy1,vz1);

// fImagepad->Clear();
  //    fImagepad->cd();
    // AppendPad();
*/

}

//________________________________________________________________________
void EdbDisplay::Refresh()
{
  EdbSegP *seg=0;
  if( eArrSegP ) {
    for(int j=0;j<eArrSegP->GetEntriesFast();j++) {
      seg = (EdbSegP*)(eArrSegP->At(j));
      SegLine(seg)->Draw();
    }
  }

  EdbTrackP *tr=0;
  if( eArrTr ) {
    for(int j=0;j<eArrTr->GetEntriesFast();j++) {
      tr = (EdbTrackP*)(eArrTr->At(j));
      TrackDraw(tr);
    }
  }

  /*
  for(int j=0;j<PVR()->Npatterns();j++) {
    PatternDraw(*(PVR()->GetPattern(j)));
  }


  TObjArray *trs = PVR()->eTracks;
  EdbTrackP *tr=0; 
  if( trs ) {
    for(int i=0; i<trs->GetEntriesFast(); i++) {
      tr = (EdbTrackP*)(trs->At(i));
      TrackDraw(tr);
    }
  }
  */
}

//________________________________________________________________________
void EdbDisplay::TrackDraw(EdbTrackP *tr)
{
  TPolyLine3D *line=0;

  EdbTrackG *pm = new EdbTrackG(1);
  pm->SetTrack( tr );
  pm->SetMarkerStyle(24);
  pm->SetPoint(0, tr->X(), tr->Y(), tr->Z() );
  pm->SetMarkerColor(kWhite);
  pm->Draw();

  EdbSegP *seg=0;
  line = new TPolyLine3D(tr->N());
  for(int is=0; is<tr->NF(); is++) {
    seg = tr->GetSegmentF(is);
    if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
  }
  line->SetLineColor(kWhite);
  line->SetLineWidth(1);
  line->SetLineStyle(3);
  line->Draw();
}

///------------------------------------------------------------
EdbSegG *EdbDisplay::SegLine(EdbSegP *seg)
{
  EdbSegG *line = new EdbSegG(2);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  line->SetPoint(1,
                 seg->X() + seg->TX()*seg->DZ(),
                 seg->Y() + seg->TY()*seg->DZ(),
                 seg->Z()+            seg->DZ() );

  line->SetLineColor( 2+seg->PID()%7 );
  line->SetLineWidth(int(seg->W()/10.));
  line->SetSeg(seg);
  return line;
}

//________________________________________________________________________
void EdbDisplay::PatternDraw(EdbPattern &pat)
{
// Clears();
  EdbSegP* seg;
  Float_t x[2],y[2],z[2],DZ,x0,y0,z0;
  Float_t tx,ty;

  for(int i=0; i<pat.GetN();i++){
    seg=pat.GetSegment(i);
    /////if(MaxChi && seg->Flag()==0) continue;
    tx=seg->TX();
    ty=seg->TY();
    if(tx>cutTX1) continue;
    if(tx<cutTX0) continue;
    if(ty>cutTY1) continue;
    if(ty<cutTY0) continue;
    //  if((pat->ID()==2)&&!eTr_Co) continue;
      // if((pat->ID()==1)&&eTr_Co) continue;
    x0=seg->X();
    y0=seg->Y();
    z0=seg->Z();
    //  if((*DZs)[j]<=0) DZ=(seg->W())/2.;
    //   DZ=(seg->Volume())/2.;
    DZ=(seg->DZ())/2.;
    x[1]=x0+tx*DZ;
    y[1]=y0+ty*DZ;
    z[1]=z0+DZ;
    x[0]=x0-tx*DZ;
    y[0]=y0-ty*DZ;
    z[0]=z0-DZ;
    //  fPad->cd();
    pl= new TPolyLine3D(2,x,y,z);

    pl->SetLineColor( 2 + seg->PID() );
    pl->SetLineWidth(int(seg->W()/10.));
    if(seg->Flag()==-1) pl->SetLineColor(kWhite);
    pl->Draw();
  }

}



//______________________________________________________________________________
Int_t EdbDisplay::DistancetoPrimitive(Int_t px, Int_t)
{
// Compute distance from point px,py to objects in event

   gPad->SetCursor(kCross);

   if (gPad == fTrigPad) return 9999;
   if (gPad == fCutPad)  return 9999;
   if (gPad == fEtaPad)  return 9999;

   const Int_t kbig = 9999;
   Int_t dist   = kbig;
   Float_t xmin = gPad->GetX1();
   Float_t xmax = gPad->GetX2();
   Float_t dx   = 0.02*(xmax - xmin);
   Float_t x    = gPad->AbsPixeltoX(px);
   if (x < xmin+dx || x > xmax-dx) return dist;

   if (fZoomMode) return 0;
   else           return 7;
}



//________________________________________________________________________
void EdbDisplay::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//  Execute action corresponding to the mouse event

   static Float_t x0, y0, x1, y1;

   static Int_t pxold, pyold;
   static Int_t px0, py0;
   static Int_t linedrawn;
   Float_t temp;

   if (px == 0 && py == 0) { //when called by sliders
      if (event == kButton1Up) {
         Draw();
      }
      return;
   }
   if (!fZoomMode && gPad->GetView()) {
      gPad->GetView()->ExecuteRotateView(event, px, py);
      return;
   }

   // something to zoom ?
//   fPad->SetCursor(kCross);
   gPad->SetCursor(kCross);

   switch (event) {

   case kButton1Down:
      gVirtualX->SetLineColor(-1);
      gPad->TAttLine::Modify();  //Change line attributes only if necessary
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0   = px; py0   = py;
      pxold = px; pyold = py;
      linedrawn = 0;
      return;

   case kButton1Motion:
      if (linedrawn) gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
      pxold = px;
      pyold = py;
      linedrawn = 1;
      gVirtualX->DrawBox(px0, py0, pxold, pyold, TVirtualX::kHollow);
      return;

   case kButton1Up:
      gPad->GetCanvas()->FeedbackMode(kFALSE);
      if (px == px0) return;
      if (py == py0) return;
      x1 = gPad->AbsPixeltoX(px);
      y1 = gPad->AbsPixeltoY(py);

      if (x1 < x0) {temp = x0; x0 = x1; x1 = temp;}
      if (y1 < y0) {temp = y0; y0 = y1; y1 = temp;}
      gPad->Range(x0,y0,x1,y1);
      if (fZooms < kMAXZOOMS-1) {
         fZooms++;
         fZoomX0[fZooms] = x0;
         fZoomY0[fZooms] = y0;
         fZoomX1[fZooms] = x1;
         fZoomY1[fZooms] = y1;
      }
      gPad->Modified(kTRUE);
      return;
   }


/* if(px==0 && py==0 && event==kButton1Up){
  cutTX0=fTXSlider->GetMinimum()*2.-1.;
  cutTX1=fTXSlider->GetMaximum()*2.-1.;
  cutTY0=fTYSlider->GetMinimum()*2.-1.;
  cutTY1=fTYSlider->GetMaximum()*2.-1.;
  printf("Ang cuts: %f %f %f %f\n",cutTX0,cutTX1,cutTY0,cutTY1);
  Refresh();
 }
 */
}
