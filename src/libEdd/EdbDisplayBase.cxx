//-- Author of drawing part : Igor Kreslo     27.11.2003
//   Based on AliDisplay class (AliRoot framework - ALICE CERN)
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplayBase                                                       //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbDisplayBase.h"
#include "TROOT.h"
#include "TGeoManager.h"
//Doesn't work in old versions of ROOT 
//#include "TVirtualViewer3D.h"

ClassImp(EdbDisplayBase);
ClassImp(Edb3DView);

const Int_t kCARTESIAN   = 1;
const Int_t kPOLAR       = 2;

//=============================================================================
EdbDisplayBase::EdbDisplayBase()
{
  //
  // Default constructor
  //
  Set0();
}

//________________________________________________________________________
EdbDisplayBase::EdbDisplayBase(const char *title, 
			       Float_t x0, Float_t x1,
			       Float_t y0, Float_t y1, 
			       Float_t z0, Float_t z1)
{
   char VPadName[140], TPadName[140];
   char ZoomButName[140], PickButName[140], UnZoomButName[140];
   char CanTitle[140];
   char *pres = 0, *ptitle = 0;

   Set0();
   fVx0=x0; fVx1=x1; fVy0=y0; fVy1=y1; fVz0=z0; fVz1=z1;
   printf("create display in limits: \nX = %f : %f   \nY = %f : %f   \nZ = %f : %f\n",x0,x1,y0,y1,z0,z1);

   // Set front view by default
//   fTheta = 0;
//   fPhi   = -90;
//   fPsi   = 0;
   fTheta = 90;
   fPhi   = 180;
   fPsi   = 90;

   fDrawAllViews  = kFALSE;
   fZoomMode      = 1;
   fZooms         = 0;

   if ((pres = strstr(title, "Presentation"))) SetStyle(1); // presentation style
   else if ((pres = strstr(title, "presentation"))) SetStyle(1);
   strcpy(fTitle, title);
   ptitle = fTitle;
   while (*ptitle) { if (*ptitle == ' ') *ptitle = '_'; ptitle++; }
   this->SetName(&fTitle[0]);
   this->SetTitle("FEDRA Event Display");
   strcpy(fCanvasName, "Canvas-");
   strcat(fCanvasName, title);
   ptitle = fCanvasName;
   while (*ptitle) { if (*ptitle == ' ') *ptitle = '_'; ptitle++; }
   strcpy(CanTitle, "FEDRA Event Display: ");
   strcat(CanTitle, title);
   fCanvas = new TCanvas(fCanvasName, CanTitle, 14, 47, 800, 700);
   fCanvas->ToggleEventStatus();

  // Create main display pad
   strcpy(VPadName, "ViewPad-");
   strcat(VPadName, title);
   fPad = new TPad(VPadName, "FEDRA Event Display",0.15,0.,1.,1.);
   fPad->Draw();
   fPad->Modified(kTRUE);
   fPad->SetFillColor(kBlack);
   fPad->SetBorderSize(2);

   // Create view type control pad
   DisplayButtons();
 
   // Create Control pad
   fCanvas->cd();
   Float_t dxtr     = 0.15;
   Float_t dytr     = 0.35;
   strcpy(TPadName, "ControlPad-");
   strcat(TPadName, title);
   fTrigPad = new TPad(TPadName, "Control Pad",0,0,dxtr,dytr);
   fTrigPad->Draw();
   fTrigPad->cd();

   if (fStyle/2 == 1)
	fTrigPad->SetFillColor(38); // blue shades
   else
	fTrigPad->SetFillColor(22); // brown shades

   fTrigPad->SetBorderSize(2);

   Int_t butcolor = 33; // blue shades
   if (fStyle/2 == 1)
	butcolor = 33; // blue shades
   else
	butcolor = 38; // blue shades

   char undov[256];
   sprintf(undov,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->UndoModifiedVTX()",fTitle);
   Float_t db = 0.09;
   fUndButton = new TButton("Undo  VTX",undov,0.05,0.85,0.95,0.85+db);
   fUndButton->SetToolTipText("Undo last vertex modification");
   fUndButton->SetFillColor(butcolor);
   fUndButton->Draw();

   char cancelv[256];
   sprintf(cancelv,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->CancelModifiedVTX()",fTitle);
   fCanButton = new TButton("Cancel VTX",cancelv,0.05,0.74,0.95,0.74+db);
   fCanButton->SetToolTipText("Cancel vertex modifications");
   fCanButton->SetFillColor(butcolor);
   fCanButton->Draw();

   char acceptv[256];
   sprintf(acceptv,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->AcceptModifiedVTX()",fTitle);
   fAccButton = new TButton("Accept VTX",acceptv,0.05,0.63,0.95,0.63+db);
   fAccButton->SetToolTipText("Accept vertex modifications");
   fAccButton->SetFillColor(butcolor);
   fAccButton->Draw();

   char envv[256];
   sprintf(envv,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVertexEnvironment()",fTitle);
   fEnvButton = new TButton("Neighbor",envv,0.05,0.47,0.95,0.47+db);
   fEnvButton->SetToolTipText("Draw vertex neighborhood");
   fEnvButton->SetFillColor(butcolor);
   fEnvButton->Draw();

   char allv[256];
   sprintf(allv,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawAllObjects()",fTitle);
   fAllButton = new TButton("AllObjcts",allv,0.05,0.47,0.95,0.47+db);
   fAllButton->SetToolTipText("Draw all objects");
   fAllButton->SetFillColor(butcolor);
   fAllButton->Draw();

   char pickmode[256];
   sprintf(pickmode,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetPickMode()",fTitle);
   fPickButton = new TButton("Pick",pickmode,0.05,0.32,0.65,0.32+db);
   fPickButton->SetToolTipText("Set Pickup mode");
   fPickButton->SetFillColor(butcolor);
   strcpy(PickButName, "PickBut-");
   strcat(PickButName, title);
   fPickButton->SetName(PickButName);
   fPickButton->Draw();

   char zoommode[256];
   sprintf(zoommode,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetZoomMode()",fTitle);
   fZoomButton = new TButton("Zoom",zoommode,0.05,0.21,0.65,0.21+db);
   fZoomButton->SetToolTipText("Set Zooming mode");
   fZoomButton->SetFillColor(butcolor);
   strcpy(ZoomButName, "ZoomBut-");
   strcat(ZoomButName, title);
   fZoomButton->SetName(ZoomButName);
   fZoomButton->Draw();

   fArcButton = new TArc(.8,fZoomButton->GetYlowNDC()+0.5*db,0.33*db);
   fArcButton->SetFillColor(kGreen);
   fArcButton->Draw();

   char butUnzoom[256];
   sprintf(butUnzoom,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->UnZoom()",fTitle);
   fUnZoomButton = new TButton("UnZoom",butUnzoom,0.05,0.05,0.95,0.14);
   fUnZoomButton->SetToolTipText("Undo previous zooming");
   fUnZoomButton->SetFillColor(butcolor);
   strcpy(UnZoomButName, "UnZoomBut-");
   strcat(UnZoomButName, title);
   fUnZoomButton->SetName(UnZoomButName);
   fUnZoomButton->Draw();

   fTrigPad->SetEditable(kFALSE);
   fButtons->SetEditable(kFALSE);

   fCanvas->cd();
   fCanvas->Update();

   gROOT->GetListOfSpecials()->Add(this);

   TList *li = fTrigPad->GetListOfPrimitives();
   li->Remove(fUndButton);
   li->Remove(fAccButton);
   li->Remove(fCanButton);
   li->Remove(fEnvButton);
   li->Remove(fAllButton);
}
//=============================================================================
EdbDisplayBase::~EdbDisplayBase()
{
  //
  // Default destructor
  //
    if (fMain) delete fMain;
    fMain = 0;
    if (fView) delete fView;
    fView = 0;
    if (fCanvas) delete fCanvas;
    fCanvas = 0;
    if (fDetector) delete fDetector;
    fDetector = 0;
}
//=============================================================================
void EdbDisplayBase::Set0()
{
  fVx0=fVy0=fVz0=fVx1=fVy1=fVz1=0;
  fCanvas = 0;
  fCanvasVTX = 0;
  fCanvasTRK = 0;
  fTrigPad = 0;
  fButtons = 0;
  fPad = 0;
  fAllButton = 0;
  fEnvButton = 0;
  fUndButton = 0;
  fCanButton = 0;
  fAccButton = 0;
  fPickButton = 0;
  fZoomButton = 0;
  fUnZoomButton = 0;
  fArcButton = 0;
  fHdrVTX = 0;
  fOldVTX = 0;
  fNewVTX = 0;
  fPreVTX = 0;
  fOldBut = 0;
  fNewBut = 0;
  fPreBut = 0;
  fCanvasName[0] = '\0';
  fTitle[0] = '\0';
  fVTXTracks = 0;
  fVTXTRKInfo = 0;
  fMain      = 0;
  fView      = 0;
  fStyle     = 0;
  fLineWidth = 1;
  if(!fDetector){fDetector = 0;}
  fDrawDet = 0;
  for (int i=0; i<50; i++) fRemBut[i] = 0;
}
//=============================================================================
void EdbDisplayBase::SetStyle(int Style)
{
    if (Style == 0)
    {
	fStyle = Style;
	fLineWidth = 1;
	fPad->SetFillColor(kBlack);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("Rotate")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("OpenGL")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("X3D")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("NeighParms")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("TrackParms")))->SetFillColor(38);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetFillColor(50);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetTextColor(kYellow);
	fButtons->Draw();
	fUndButton->SetFillColor(38); // blue shades
	fAccButton->SetFillColor(38);
	fCanButton->SetFillColor(38);
	fEnvButton->SetFillColor(38);
	fAllButton->SetFillColor(38);
	fPickButton->SetFillColor(38);
	fZoomButton->SetFillColor(38);
	fUnZoomButton->SetFillColor(38);
	fTrigPad->SetFillColor(22); // brown shades
	fTrigPad->Draw();
	fPad->cd();
    }
    else if (Style == 1)
    {
	fStyle = Style;
	fLineWidth = 2;
	fPad->SetFillColor(kBlack);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("Rotate")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("OpenGL")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("X3D")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("NeighParms")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("TrackParms")))->SetFillColor(38);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetFillColor(50);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetTextColor(kYellow);
	fButtons->Draw();
	fUndButton->SetFillColor(38); // blue shades
	fAccButton->SetFillColor(38);
	fCanButton->SetFillColor(38);
	fEnvButton->SetFillColor(38);
	fAllButton->SetFillColor(38);
	fPickButton->SetFillColor(38);
	fZoomButton->SetFillColor(38);
	fUnZoomButton->SetFillColor(38);
	fTrigPad->SetFillColor(22); // brown shades
	fTrigPad->Draw();
	fPad->cd();
    }
    else if (Style == 2)
    {
	fStyle = Style;
	fLineWidth = 1;
	fPad->SetFillColor(kWhite);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("Rotate")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("OpenGL")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("X3D")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("NeighParms")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("TrackParms")))->SetFillColor(33);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetFillColor(kYellow);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetTextColor(kRed);
	fButtons->Draw();
	fUndButton->SetFillColor(33); // blue shades
	fAccButton->SetFillColor(33);
	fCanButton->SetFillColor(33);
	fEnvButton->SetFillColor(33);
	fAllButton->SetFillColor(33);
	fPickButton->SetFillColor(33);
	fZoomButton->SetFillColor(33);
	fUnZoomButton->SetFillColor(33);
	fTrigPad->SetFillColor(38); // blue shades
	fTrigPad->Draw();
	fPad->cd();
    }
    else if (Style == 3)
    {
	fStyle = Style;
	fLineWidth = 2;
	fPad->SetFillColor(kWhite);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("Rotate")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("OpenGL")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("X3D")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("NeighParms")))->SetFillColor(33);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("TrackParms")))->SetFillColor(33);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetFillColor(kYellow);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetTextColor(kRed);
	fButtons->Draw();
	fUndButton->SetFillColor(33); // blue shades
	fAccButton->SetFillColor(33);
	fCanButton->SetFillColor(33);
	fEnvButton->SetFillColor(33);
	fAllButton->SetFillColor(33);
	fPickButton->SetFillColor(33);
	fZoomButton->SetFillColor(33);
	fUnZoomButton->SetFillColor(33);
	fTrigPad->SetFillColor(38); // blue shades
	fTrigPad->Draw();
	fPad->cd();
    }
    else
    {
	fStyle = Style;
	fLineWidth = 1;
	fPad->SetFillColor(kBlack);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("Rotate")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("OpenGL")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("X3D")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("NeighParms")))->SetFillColor(38);
	((TButton *)(fButtons->GetListOfPrimitives()->FindObject("TrackParms")))->SetFillColor(38);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetFillColor(50);
	((TDiamond *)(fButtons->GetListOfPrimitives()->FindObject("Diamond")))->SetTextColor(kYellow);
	fButtons->Draw();
	fUndButton->SetFillColor(38); // blue shades
	fAccButton->SetFillColor(38);
	fCanButton->SetFillColor(38);
	fEnvButton->SetFillColor(38);
	fAllButton->SetFillColor(38);
	fPickButton->SetFillColor(38);
	fZoomButton->SetFillColor(38);
	fUnZoomButton->SetFillColor(38);
	fTrigPad->SetFillColor(22); // brown shades
	fTrigPad->Draw();
	fPad->cd();
    }
}
//_____________________________________________________________________________
void EdbDisplayBase::DrawTitle(Option_t *option)
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
      title->SetFillColor(42); // red shades
      title->Draw();
      char ptitle[100];
      sprintf(ptitle,"OPERA emulsion view");
      title->AddText(ptitle);
   } else {
      TPaveLabel *label = new TPaveLabel(xmin +0.01*dx, ymax-0.07*dy, xmin +0.2*dx, ymax-0.01*dy,option);
      label->SetBit(kCanDelete);
      label->SetFillColor(42); // red shades
      label->Draw();
   }
}

//_____________________________________________________________________________
void EdbDisplayBase::DisplayButtons()
{
//    Create the user interface buttons
   char ViewTypePadName[140];

   strcpy(ViewTypePadName, "ViewTypePad-");
   strcat(ViewTypePadName, fCanvasName);
   fButtons = new TPad(ViewTypePadName, "View Type Pad",0,0.35,0.15,1);
   fButtons->Draw();
   fButtons->SetFillColor(38); // blue shades
   fButtons->SetBorderSize(2);
   fButtons->cd();

   Int_t butcolor = 33; // blue shades
   Float_t dbutton = 0.05;
//   Float_t y  = 0.96;
   Float_t y  = 1.044;
   Float_t dy = 0.010;
   Float_t x0 = 0.05;
   Float_t x1 = 0.95;

   TButton *button;

//   y -= dbutton +dy;

   y -= dbutton +dy;

   char but3[256];
   sprintf(but3,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetView(90,90,90)",fTitle);
   button = new TButton("Top View",but3,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but4[256];
   sprintf(but4,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetView(90,180,90)",fTitle);
   button = new TButton("Side View",but4,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but5[256];
   sprintf(but5,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetView(0,-90,0)",fTitle);
   button = new TButton("Front View",but5,x0,y-dbutton,x1,y);
   button->SetFillColor(butcolor);
   button->Draw();

   if (fStyle/2 == 1)
	butcolor = 33; // blue shades
   else
	butcolor = 38; // blue shades

   ////y -= dbutton +dy;  ????
   y -= dbutton +dy;
   char butA[256];
   sprintf(butA,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SwDrawDet()",fTitle);
   button=new TButton("Draw Detector",butA,x0,y-dbutton,x1,y);
   button->SetToolTipText("Toggle drawing detector on/off");
   button->SetFillColor(38);
   button->Draw();

   y -= dbutton +dy;
   char but6[256];
   sprintf(but6,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->SetRotate()",fTitle);
   button = new TButton("Rotate",but6,x0,y-dbutton,x1,y);
   button->SetName("Rotate");
   button->SetToolTipText("Rotate 3-D view");
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but7[256];
   sprintf(but7,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawViewGL()",fTitle);
   button = new TButton("OpenGL",but7,x0,y-dbutton,x1,y);
   button->SetName("OpenGL");
   button->SetToolTipText("Show 3-D view with OpenGL");
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but8[256];
   sprintf(but8,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawViewX3D()",fTitle);
   button = new TButton("X3D",but8,x0,y-dbutton,x1,y);
   button->SetName("X3D");
   button->SetToolTipText("Show 3-D view with X3D");
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but9[256];
   sprintf(but9,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DialogNeighborParameters()",fTitle);
   button = new TButton("NeighParms",but9,x0,y-dbutton,x1,y);
   button->SetName("NeighParms");
   button->SetToolTipText("Set Parameters for Neighborhood");
   button->SetFillColor(butcolor);
   button->Draw();

   y -= dbutton +dy;
   char but10[256];
   sprintf(but10,
   "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DialogTrackParameters()",fTitle);
   button = new TButton("TrackParms",but10,x0,y-dbutton,x1,y);
   button->SetName("TrackParms");
   button->SetToolTipText("Set Mometum and Mas for tracks creation and propagation");
   button->SetFillColor(butcolor);
   button->Draw();

   // display logo
   TDiamond *diamond = new TDiamond(0.05,0.017,0.95,0.21);
   diamond->SetFillColor(50); // red shades
   diamond->SetTextAlign(23);
   diamond->SetTextColor(kYellow);
   diamond->SetTextSize(0.09);
   diamond->Draw();
   diamond->AddText(".");
   diamond->AddText("ROOT");
   diamond->AddText("- OPERA -");
   diamond->AddText("FEDRA");
   diamond->AddText(".");
   diamond->SetName("Diamond");
}

//_____________________________________________________________________________
void EdbDisplayBase::SetDrawDet(Bool_t fflag)
{
  if(!fDetector) { printf("WARNING: EdbDisplayBase::SetDrawDet: fDetector do not defined!\n"); return; }
  fDrawDet=fflag;
  TGeoVolume* vol=(TGeoVolume*)fPad->GetListOfPrimitives()->FindObject(fDetector);
  vol->SetVisibility(fDrawDet);
  printf("\nDrawDet is %i\n",vol->IsVisible());
  fPad->Modified(kTRUE);
}

//_____________________________________________________________________________
void EdbDisplayBase::DrawDetector()
{
  //if(!fDrawDet){return;}
  if(fDetector){
      fPad->GetListOfPrimitives()->Add(fDetector);
  } else {
    printf("\nError:no Detector!\n");
  }
}

//_____________________________________________________________________________
void EdbDisplayBase::SetRange(Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1)
{
   
   fVx0=x0; fVx1=x1; fVy0=y0; fVy1=y1; fVz0=z0; fVz1=z1;

   if (!fPad) return;
   fPad->Clear();
   Draw();
}

//_____________________________________________________________________________
void EdbDisplayBase::DrawView(Float_t theta, Float_t phi, Float_t psi)
{
//    Draw a view of DataSet

   gPad->SetCursor(kWatch);
   if (fStyle/2 == 1) gPad->SetFillColor(kWhite);
   else               gPad->SetFillColor(kBlack);

//   TList *li = gPad->GetListOfPrimitives();
//   int np = li->GetSize();
//   TObject *o = 0;
//   for (int i=0; i<np; i++)
//   {
//    o = li->At(i);
//    if (o && o->TestBit(kCanDelete)) delete o;
//   }
//   li->Clear("nodelete");

   gPad->Clear("nodelete");

   fView = new Edb3DView();

   if (gPad->GetView() != fView || fView == 0) printf("Error!\n");

   fView->SetLongitude((double)phi);
   fView->SetLatitude((double)theta);
   fView->SetPsi((double)psi);
   fView->SetRange((double)fVx0,(double)fVy0,(double)fVz0,(double)fVx1,(double)fVy1,(double)fVz1);

   fZoomX0[0] = -1;
   fZoomY0[0] = -1;
   fZoomX1[0] =  1;
   fZoomY1[0] =  1;

   Refresh();
   AppendPad();

   gPad->Range(fZoomX0[fZooms],fZoomY0[fZooms],fZoomX1[fZooms],fZoomY1[fZooms]);
   
   gPad->Modified(kTRUE);
}

//________________________________________________________________________
void EdbDisplayBase::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
//  Execute action corresponding to the mouse event

   static Double_t x0, y0, x1, y1;

   static Int_t pxold, pyold;
   static Int_t px0, py0;
   static Int_t linedrawn;
   Double_t temp;

//   if (px == 0 && py == 0) { //when called by sliders
//      if (event == kButton1Up) {
//         Draw();
//      }
//      return;
//   }
//   if (!fZoomMode && gPad->GetView()) {
//      gPad->GetView()->ExecuteRotateView(event, px, py);
//      return;
//   }

   // something to zoom ?

   if (!fZoomMode && gPad->GetView())
   {
	gPad->SetCursor(kPointer);
	return;
   }

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

}
//_____________________________________________________________________________
void EdbDisplayBase::SetView(Float_t theta, Float_t phi, Float_t psi)
{
//  change viewing angles for current event

   if (fView) fView->SetRotateMode(false);

   fPad->cd();
   fDrawAllViews = kFALSE;
   fPhi   = phi;
   fTheta = theta;
   fPsi   = psi;
   Int_t iret = 0;

   TView *view = gPad->GetView();
   if (view) view->SetView(fPhi, fTheta, fPsi, iret);
   else      Draw();

   gPad->Modified(kTRUE);
}

//_____________________________________________________________________________
void EdbDisplayBase::DrawAllViews()
{
//    Draw front,top,side and 30 deg views

   fDrawAllViews = kTRUE;
   fPad->cd();
   fPad->SetFillColor(15);  // gray shades
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
   DrawView(90, 90, 90);
   DrawTitle("Top");

   // draw side view
   fPad->cd(4);
   DrawView(90, 180, 90);
   DrawTitle("Side");

   fPad->cd(2);
}
//_____________________________________________________________________________
void EdbDisplayBase::DrawViewGL()
{
//    Draw current view using OPENGL

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   pad->GetViewer3D("OPENGL");
   //pad->x3d("OPENGL");
}

//_____________________________________________________________________________
void EdbDisplayBase::DrawViewX3D()
{
//    Draw current view using X3D

   TPad *pad = (TPad*)gPad->GetPadSave();
   pad->cd();
   TView *view = pad->GetView();
   if (!view) return;
   //pad->x3d();
   pad->GetViewer3D("x3d");
}


//_____________________________________________________________________________
void EdbDisplayBase::Draw(Option_t *)
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
void EdbDisplayBase::SetPickMode()
{
  //
  // Set Pick Mode -- disable zoom
  //
   if (fView) fView->SetRotateMode(false);

   fZoomMode = 0;

   fArcButton->SetY1(fPickButton->GetYlowNDC()+0.5*fPickButton->GetHNDC());
   fTrigPad->Modified(kTRUE);
   gPad->SetCursor(kPointer);
}

//_____________________________________________________________________________
void EdbDisplayBase::SetZoomMode()
{
  //
  // Set Zoom Mode -- disable pick
  //
   if (fView) fView->SetRotateMode(false);

   fZoomMode = 1;

   fArcButton->SetY1(fZoomButton->GetYlowNDC()+0.5*fZoomButton->GetHNDC());
   fTrigPad->Modified(kTRUE);
   gPad->SetCursor(kCross);
}

//_____________________________________________________________________________
void EdbDisplayBase::SetRotate()
{
  //
  // Set 3-D View Rotate Mode
  //
  
   fZoomMode = 0;
  
   if (fView) fView->SetRotateMode(true);

   fArcButton->SetY1(fPickButton->GetYlowNDC()+0.5*fPickButton->GetHNDC());
   fTrigPad->Modified(kTRUE);
}

//______________________________________________________________________________
void EdbDisplayBase::UnZoom()
{
  //
  // Resets ZOOM
  //
  if (fZooms <= 0)
  {
    fCanvas->Paint();
    fCanvas->Update();
    return;
  }
  fZooms--;
  fPad->Range(fZoomX0[fZooms],fZoomY0[fZooms], fZoomX1[fZooms],fZoomY1[fZooms]);
  fPad->Modified();
  fPad->cd();
  fPad->Draw();
  fCanvas->Paint();
  fCanvas->Update();
}

//______________________________________________________________________________
Int_t EdbDisplayBase::DistancetoPrimitive(Int_t px, Int_t py)
{
// Compute distance from point px,py to objects in event


   if (gPad == fTrigPad) return 9999;
   if (gPad == fButtons) return 9999;

//   gPad->SetCursor(kCross);

   const Int_t kbig = 9999;
   Int_t dist   = kbig;
   Float_t xmin = gPad->GetX1();
   Float_t xmax = gPad->GetX2();
   Float_t dx   = 0.002*(xmax - xmin);
   Float_t x    = gPad->AbsPixeltoX(px);
   if (x < xmin+dx || x > xmax-dx) return dist;
/*
   Float_t ymin = gPad->GetY1();
   Float_t ymax = gPad->GetY2();
   Float_t dy   = 0.02*(ymax - ymin);
   Float_t y    = gPad->AbsPixeltoX(py);
   if (y < ymin+dy || y > ymax-dy) return dist;
*/
   if (fZoomMode) return 0;
   else           return 7;
}
//_____________________________________________________________________________
char *EdbDisplayBase::GetObjectInfo(int px, int py) const
{
    static char rcoordinates[]="Rotated coordinates";
    static char coordinates[80];
    if (!fPad || !fView) return rcoordinates;
    float nxyz[3] = {fPad->AbsPixeltoX(px), fPad->AbsPixeltoY(py), 0.}, wxyz[3] = {0., 0., 0.};
    if (fView) fView->NDCtoWC(nxyz, wxyz);
    if (fView->GetLatitude() == 90.)
    {
	if (fView->GetLongitude() == 180.)
	    sprintf(coordinates, "Y = %.1f, Z = %.1f", wxyz[1], wxyz[2]);
	else if (fView->GetLongitude() == 90.)
	    sprintf(coordinates, "X = %.1f, Z = %.1f", wxyz[0], wxyz[2]);
	else
	    return rcoordinates;
    }
    else if (fView->GetLatitude() == 0.)
	sprintf(coordinates, "X = %.1f, Y = %.1f", wxyz[0], wxyz[1]);
    else
	return rcoordinates;
    return coordinates;
}

//=============================================================================
void EdbDisplayBase::DrawOldVTX(char *ptitle)
{
  fCanvasVTX->cd();
  if (fOldVTX)
  {
    fOldVTX->Clear();
    fOldVTX->SetText(0.05, 0.88, ptitle);
  }
  else
  {
    fOldVTX = new TText(0.05, 0.88, ptitle);
    fOldVTX->ResetBit(kCanDelete);
    fOldVTX->SetTextColor(kBlack);
    fOldVTX->SetTextSize(0.04);
    fOldVTX->SetTextAlign(12);
    fOldVTX->SetTextFont(102);
  }
  fOldVTX->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::DrawPreVTX(char *ptitle)
{
  fCanvasVTX->cd();
  if (fPreVTX)
  {
    fPreVTX->Clear();
    fPreVTX->SetText(0.05, 0.82, ptitle);
  }
  else
  {
    fPreVTX = new TText(0.05, 0.82, ptitle);
    fPreVTX->ResetBit(kCanDelete);
    fPreVTX->SetTextColor(kBlack);
    fPreVTX->SetTextSize(0.04);
    fPreVTX->SetTextAlign(12);
    fPreVTX->SetTextFont(102);
  }
  fPreVTX->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::DrawNewVTX(char *ptitle)
{
  fCanvasVTX->cd();
  if (fNewVTX)
  {
    fNewVTX->Clear();
    fNewVTX->SetText(0.05, 0.76, ptitle);
  }
  else
  {
    fNewVTX = new TText(0.05, 0.76, ptitle);
    fNewVTX->ResetBit(kCanDelete);
    fNewVTX->SetTextColor(kBlack);
    fNewVTX->SetTextSize(0.04);
    fNewVTX->SetTextAlign(12);
    fNewVTX->SetTextFont(102);
  }
  fNewVTX->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::CreateCanvasVTX()
{
  static char CanvasVTXName[140];
  strcpy(CanvasVTXName, "VTX-");
  strcat(CanvasVTXName, fCanvasName);
  if ((fCanvasVTX = (TCanvas *)(gROOT->GetListOfCanvases()->FindObject(CanvasVTXName))))
  {
    fCanvasVTX->Clear();
  }
  else
  {
    int xpos = fCanvas->GetWindowTopX()+fCanvas->GetWw();
    int ypos = fCanvas->GetWindowTopY();
    fCanvasVTX = new TCanvas(CanvasVTXName, "Vertex Data Display",
			     -xpos, ypos, 640, 350);
    fCanvasVTX->ToggleEventStatus();
    for (int i=0; i<50; i++) fRemBut[i] = 0;
  }
  if (fHdrVTX)
  {
    fHdrVTX->Clear();
    fHdrVTX->SetText(0.05, 0.94,
    "Vertex  ID    Mult  X          Y          Z          Dist   Chi2     Prob");
  }
  else
  {
    fHdrVTX = new TText(0.05, 0.94,
    "Vertex  ID    Mult  X          Y          Z          Dist   Chi2     Prob");
    fHdrVTX->ResetBit(kCanDelete);
    fHdrVTX->SetTextColor(kBlue);
    fHdrVTX->SetTextSize(0.04);
    fHdrVTX->SetTextAlign(12);
    fHdrVTX->SetTextFont(102);
  }
  fHdrVTX->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::ClearNewVTX()
{
  fCanvasVTX->cd();
  fNewVTX->Clear();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::ClearPreVTX()
{
  fCanvasVTX->cd();
  fPreVTX->Clear();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::DrawOldBut(char *type)
{
  fCanvasVTX->cd();
  if (!fOldBut)
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fOldBut = new TButton("TR",but,0.01,0.86,0.04,0.90);
    fOldBut->SetToolTipText("Show tracks table");
    fOldBut->ResetBit(kCanDelete);
    fOldBut->SetFillColor(38); // blue shades
    fOldBut->SetName("DrawOldVTX");
  }
  else
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fOldBut->SetMethod(but);
  }
  fOldBut->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::DrawPreBut(char *type)
{
  fCanvasVTX->cd();
  if (!fPreBut)
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fPreBut = new TButton("TR",but,0.01,0.80,0.04,0.84);
    fPreBut->SetToolTipText("Show tracks table");
    fPreBut->ResetBit(kCanDelete);
    fPreBut->SetFillColor(38); // blue shades
    fPreBut->SetName("DrawPreVTX");
  }
  else
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fPreBut->SetMethod(but);
  }
  fPreBut->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//=============================================================================
void EdbDisplayBase::DrawNewBut(char *type)
{
  fCanvasVTX->cd();
  if (!fNewBut)
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fNewBut = new TButton("TR",but,0.01,0.74,0.04,0.78);
    fNewBut->SetToolTipText("Show tracks table");
    fNewBut->ResetBit(kCanDelete);
    fNewBut->SetFillColor(38); // blue shades
    fNewBut->SetName("DrawNewVTX");
  }
  else
  {
    char but[256];
    sprintf(but,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->DrawVTXTracks(\"",fTitle);
    strcat(but,type);
    strcat(but,"\")");
    fNewBut->SetMethod(but);
  }
  fNewBut->Draw();
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
//______________________________________________________________________________
void EdbDisplayBase::DrawUnd()
{
    fTrigPad->cd();
    if (fStyle/2 == 1)
	fUndButton->SetFillColor(33);
    else
	fUndButton->SetFillColor(38);
    fTrigPad->GetListOfPrimitives()->Add(fUndButton);
    fUndButton->SetPad(0.05,0.85,0.95,0.94);
    fUndButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
}
//______________________________________________________________________________
void EdbDisplayBase::DrawAcc()
{
    fTrigPad->cd();
    if (fStyle/2 == 1)
	fAccButton->SetFillColor(33);
    else
	fAccButton->SetFillColor(38);
    fTrigPad->GetListOfPrimitives()->Add(fAccButton);
    fAccButton->SetPad(0.05,0.63,0.95,0.72);
    fAccButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
}
//______________________________________________________________________________
void EdbDisplayBase::DrawCan()
{
    fTrigPad->cd();
    if (fStyle/2 == 1)
	fCanButton->SetFillColor(33);
    else
	fCanButton->SetFillColor(38);
    fTrigPad->GetListOfPrimitives()->Add(fCanButton);
    fCanButton->SetPad(0.05,0.74,0.95,0.83);
    fCanButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
}
//______________________________________________________________________________
void EdbDisplayBase::DrawEnv()
{
    fTrigPad->cd();
    if (fStyle/2 == 1)
	fEnvButton->SetFillColor(33);
    else
	fEnvButton->SetFillColor(38);
    fTrigPad->GetListOfPrimitives()->Add(fEnvButton);
    fEnvButton->SetPad(0.05,0.47,0.95,0.56);
    fEnvButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
}
//_____________________________________________________________________________
const char *Edb3DView::GetTitle() const
{
    static char title[80];
    sprintf(title, "%s", "FEDRA Event Display");
    return title;
}

//_____________________________________________________________________________
const char *Edb3DView::GetName() const
{
    static char name[] = "3-D View";
    return name;
}
//_____________________________________________________________________________
char *Edb3DView::GetObjectInfo(int px, int py) const
{
    static char rcoordinates[]="Rotated coordinates";
    static char coordinates[80];
    if (!gPad) return rcoordinates;
    float nxyz[3] = {gPad->AbsPixeltoX(px), gPad->AbsPixeltoY(py), 0.}, wxyz[3] = {0., 0., 0.};
    ((TView *)this)->NDCtoWC(nxyz, wxyz);
    if (((TView *)this)->GetLatitude() == 90.)
    {
	if (((TView *)this)->GetLongitude() == 180.)
	    sprintf(coordinates, "Y = %.1f, Z = %.1f", wxyz[1], wxyz[2]);
	else if (((TView *)this)->GetLongitude() == 90.)
	    sprintf(coordinates, "X = %.1f, Z = %.1f", wxyz[0], wxyz[2]);
	else
	    return rcoordinates;
    }
    else if (((TView *)this)->GetLatitude() == 0.)
	sprintf(coordinates, "X = %.1f, Y = %.1f", wxyz[0], wxyz[1]);
    else
	return rcoordinates;
    return coordinates;
}

//______________________________________________________________________________
void Edb3DView::ExecuteRotateView(Int_t event, Int_t px, Int_t py)
{
//*-*-*-*-*-*-*-*-*Execute action corresponding to one event*-*-*-*-*-*-*-*-*-*
//*-*              =========================================                    *
//*-*  This member function is called when a object is clicked with the locator *
//*-*                                                                           *
//*-*  If Left button clicked in the object area, while the button is kept down *
//*-*  the cube representing the surrounding frame for the corresponding        *
//*-*  new latitude and longitude position is drawn.                                         *
//*-*                                                                           *
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
   static Int_t system, framewasdrawn;
   static Double_t xrange, yrange, xmin, ymin, longitude1, latitude1, longitude2, latitude2;
   static Double_t newlatitude, newlongitude, oldlatitude, oldlongitude;
   Double_t dlatitude, dlongitude, x, y;
   Int_t irep = 0;
   Double_t psideg;

   // all coordinate transformation are from absolute to relative

   if (!fRotateMode)
   {
	gPad->SetCursor(kPointer);
	return;
   }
   if (!gPad->IsEditable()) return;
   gPad->AbsCoordinates(kTRUE);

   switch (event) {

   case kKeyPress :
       fChanged = kTRUE;
       MoveViewCommand(Char_t(px), py);
       break;
   case kMouseMotion:
      gPad->SetCursor(kRotate);
      break;

   case kButton1Down:

      // remember position of the cube
      xmin   = gPad->GetX1();
      ymin   = gPad->GetY1();
      xrange = gPad->GetX2() - xmin;
      yrange = gPad->GetY2() - ymin;
      x      = gPad->PixeltoX(px);
      y      = gPad->PixeltoY(py);
      system = GetSystem();
      framewasdrawn = 0;
      if (system == kCARTESIAN || system == kPOLAR || IsPerspective()) {
         longitude1 = 180*(x-xmin)/xrange;
         latitude1  =  90*(y-ymin)/yrange;
      } else {
         latitude1  =  90*(x-xmin)/xrange;
         longitude1 = 180*(y-ymin)/yrange;
      }
      newlongitude = oldlongitude = -90 - gPad->GetPhi();
      newlatitude  = oldlatitude  =  90 - gPad->GetTheta();
      psideg       = GetPsi();

      // if outline isn't set, make it look like a cube
      if(!fOutline)
          SetOutlineToCube();
      break;

   case kButton1Motion:
   {
      // draw the surrounding frame for the current mouse position
      // first: Erase old frame
      fChanged = kTRUE;
      if (framewasdrawn) fOutline->Paint();
      framewasdrawn = 1;
      x = gPad->PixeltoX(px);
      y = gPad->PixeltoY(py);
      if (system == kCARTESIAN || system == kPOLAR || IsPerspective()) {
         longitude2 = 180*(x-xmin)/xrange;
         latitude2  =  90*(y-ymin)/yrange;
      } else {
         latitude2  =  90*(x-xmin)/xrange;
         longitude2 = 180*(y-ymin)/yrange;
      }
      dlongitude   = longitude2   - longitude1;
      dlatitude    = latitude2    - latitude1;
      newlatitude  = oldlatitude  + dlatitude;
      newlongitude = oldlongitude - dlongitude;
      psideg       = GetPsi();
      ResetView(newlongitude, newlatitude, psideg, irep);
      fOutline->Paint();

      break;
   }
   case kButton1Up:

      // Temporary fix for 2D drawing problems on pad. fOutline contains
      // a TPolyLine3D object for the rotation box. This will be painted
      // through a newly created TViewer3DPad instance, which is left
      // behind on pad. This remaining creates 2D drawing problems.
      //
      // This is a TEMPORARY fix - will be removed when proper multiple viewers
      // on pad problems are resolved.
//Doesn't work in old versions of ROOT 
//      if (gPad) {
//         TVirtualViewer3D *viewer = gPad->GetViewer3D();
//         if (viewer && !strcmp(viewer->IsA()->GetName(),"TViewer3DPad")) {
//            gPad->ReleaseViewer3D();
//            delete viewer;
//         }
//      }
      // End fix

      // Recompute new view matrix and redraw
      psideg = GetPsi();
      SetView(newlongitude, newlatitude, psideg, irep);
      gPad->SetPhi(-90-newlongitude);
      gPad->SetTheta(90-newlatitude);
      gPad->Modified(kTRUE);

      // Set line color, style and width
      gVirtualX->SetLineColor(-1);
      gVirtualX->SetLineStyle(-1);
      gVirtualX->SetLineWidth(-1);
      break;
   }

   // set back to default transformation mode
   gPad->AbsCoordinates(kFALSE);
}
//_____________________________________________________________________________
void Edb3DView::SetRotateMode(bool mode)
{
  //
  // Set or unset 3-D View Rotate Mode
  //
   fRotateMode = mode;
}

