#include "EGraphTool.h"
#include <TGDockableFrame.h>
#include <TGFileDialog.h>
#include <TGTab.h>
#include <TString.h>
#include <TSystem.h>

ClassImp(EGraphTool)

enum EGraphToolCommandIdentifiers {
  M_FILE_OPEN,
  M_FILE_SAVE_PICTURE,
  M_FILE_EXIT,
  M_SHOW_3D,
  M_ZOOM_IN,
  M_ZOOM_OUT
};

const char *EGraphToolXpmNames[] = {
  "open.xpm",
  "save.xpm",
  "",
  "view3d.xpm",
  "",
  "zoom-in.xpm",
  "zoom-out.xpm",
  "",
  "quit.xpm",
  0
};

ToolBarData_t EGraphToolTbData[] = {
  { "", "Open Root event file",     kFALSE, M_FILE_OPEN,         NULL },
  { "", "Save picture",             kFALSE, M_FILE_SAVE_PICTURE, NULL },
  { "",              0,             0,      -1,                  NULL },
  { "", "Open 3D viewer",           kFALSE, M_SHOW_3D,           NULL },
  { "",              0,             0,      -1,                  NULL },
  { "", "Zoom In",                  kFALSE, M_ZOOM_IN,           NULL },
  { "", "Zoom Out",                 kFALSE, M_ZOOM_OUT,          NULL },
  { "",              0,             0,      -1,                  NULL },
  { "", "Exit Application",         kFALSE, M_FILE_EXIT,         NULL },
  { NULL,            NULL,          0,      0,                   NULL }
};


EGraphTool::EGraphTool(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h)
{
  InitVariables();
  InitDrawVariables();

  fEmGraphRec = new EGraphRec(); // Init Graph Reconstruction Toolkit
  DrawFrame();                   // drawing main frame

  SetWindowName("Emulsion Graphical Tool");
  MapSubwindows();
  Resize(GetDefaultSize());
  MapWindow();
}


//----------------------------------------------------------------------------
EGraphTool::~EGraphTool()
{
  delete fMainFrame;
}


//----------------------------------------------------------------------------
void EGraphTool::ProcessEvent()
{
  // if (fEvtTree) fEmGraphRec->ProcessEvent(fEvtNumber);
  // fEmGraphRec->ProcessEvent(fEvtNumber);
}


//----------------------------------------------------------------------------
void EGraphTool::DrawFrame()
{
  // Correct close window

  Connect("CloseWindow()", "EGraphTool", this, "ExitApplication()");

  SetCleanup(kDeepCleanup);

  // Create main frame

  fMainFrame = new TGVerticalFrame(this, 1000, 850);
  fMainFrame->SetCleanup(kDeepCleanup);

  AddMenuBar(fMainFrame);                 // Create a menu bar
  TGHorizontalFrame *workframe = new TGHorizontalFrame(fMainFrame);

  // buttons frame

  TGVerticalFrame *ButtonFrame = new TGVerticalFrame(workframe);

  // create Brick Id frame

  fEmGraphRec->AddProcBrickFrame(ButtonFrame);

  // create tab for options frame

  TGTab *OptTab = new TGTab(ButtonFrame);
  // fEmGraphRec->AddRecOptFrame(OptTab); // Create a button frame
  fEmGraphRec->AddScanBackFrame(OptTab);  // Create scan back frame
  fEmGraphRec->AddVertexRecFrame(OptTab); // Craete vertex reconstruction frame
  ButtonFrame->AddFrame(OptTab, fLayout2);

  // create process list frame

  // fEmGraphRec->AddProcListFrame(ButtonFrame);

  // Process Event

//   TGTextButton *process = new TGTextButton(ButtonFrame, "Execute event");
//   process->Connect("Clicked()", "EGraphTool", this, "ProcessEvent()");
//   process->Associate(this);
//   ButtonFrame->AddFrame(process, fLayout1);
  workframe->AddFrame(ButtonFrame, new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2));

  TGVerticalFrame *CanvasFrame = new TGVerticalFrame(workframe);

  // create tab for canvas frame

  TGTab *CanvasTab = new TGTab(CanvasFrame);
  fEmGraphRec->AddCanvasFrame(CanvasTab); // Create a canvas frame
  CanvasFrame->AddFrame(CanvasTab, fLayout3);

  // create text info frame

  fEmGraphRec->AddInfoFrame(CanvasFrame);

  workframe->AddFrame(CanvasFrame, fLayout3);
  fMainFrame->AddFrame(workframe, fLayout3);
  AddFrame(fMainFrame, fLayout3);
}


//----------------------------------------------------------------------------
void EGraphTool::AddMenuBar(TGVerticalFrame *workframe)
{
  TGDockableFrame *MenuDock = new TGDockableFrame(workframe);

  // Section File

  fMenuFile = new TGPopupMenu(gClient->GetRoot());
  fMenuFile->AddEntry("Open...",         M_FILE_OPEN);
  fMenuFile->AddEntry("Save picture...", M_FILE_SAVE_PICTURE);
  fMenuFile->AddSeparator();
  fMenuFile->AddEntry("Exit",            M_FILE_EXIT);
  fMenuFile->DisableEntry(M_FILE_SAVE_PICTURE);
  fMenuFile->Associate(this);

  workframe->AddFrame(MenuDock, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 
					      0, 0, 1, 0));

  TGMenuBar *MenuBar = new TGMenuBar(MenuDock, 1, 1, kHorizontalFrame);
  MenuBar->AddPopup("File", fMenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0));
  MenuDock->AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));

  // Toolbar

  Int_t spacing = 8;
  fToolBar = new TGToolBar(workframe, 60, 20, kHorizontalFrame | kRaisedFrame);
  for (Int_t i = 0; EGraphToolXpmNames[i]; i++) {
    TString iconname = (TString)getenv("FEDRA_ROOT") + 
      "/src/appl/display/icons/" + EGraphToolXpmNames[i];
    EGraphToolTbData[i].fPixmap = iconname.Data();
    if (strlen(EGraphToolXpmNames[i]) == 0) {
      spacing = 8;
      continue;
    }
    fToolBar->AddButton(this, &EGraphToolTbData[i], spacing);
    spacing = 0;
  }

  fToolBar->GetButton(M_FILE_SAVE_PICTURE)->SetState(kButtonDisabled);
  // fToolBar->GetButton(M_SHOW_3D)->SetState(kButtonDisabled);
  // fToolBar->GetButton(M_ZOOM_IN)->SetState(kButtonDisabled);
  // fToolBar->GetButton(M_ZOOM_OUT)->SetState(kButtonDisabled);

  workframe->AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
}

//----------------------------------------------------------------------------
Bool_t EGraphTool::ProcessMessage(Long_t msg, Long_t param1, Long_t)
{
  // Handle messages send to the TDisplayMainFrame object.
  // E.g. all menu button messages.

  const char *filetypes[] = {"ROOT files", "*.root", 0, 0};

  switch (GET_MSG(msg)) {
  case kC_COMMAND:
    switch (GET_SUBMSG(msg)) {
    case kCM_BUTTON:
    case kCM_MENU:
      switch (param1) 
	{
	case M_FILE_OPEN:
	  {
	    static TString dir(".");
	    TGFileInfo fi;
	    fi.fFileTypes = filetypes;
	    fi.fIniDir    = StrDup(dir.Data());

	    new TGFileDialog(gClient->GetRoot(), fMainFrame, kFDOpen, &fi);
	    dir = fi.fIniDir;

	    // open new file

	    if (fi.fFilename) InitInputFile((TString)fi.fFilename);
	    break;
	  }
	case M_FILE_SAVE_PICTURE:
	  // save picture
	  break;    
	case M_FILE_EXIT:
	  ExitApplication();           // terminate the Application
	  break;
	case M_SHOW_3D:
	  fEmGraphRec->Set3DViewer();  // GL viewer
	  break;
	case M_ZOOM_IN:
	  fEmGraphRec->ZoomIn();       // zoom in
	  break;
	case M_ZOOM_OUT:
	  fEmGraphRec->ZoomOut();      // zoom out
	  break;
	}
      break;
    }
    break;
  }

  return kTRUE;
}


//----------------------------------------------------------------------------
void EGraphTool::InitVariables()
{
  fEventId       = 0;
  fEvtTree       = NULL;
  fInputTreeFile = NULL;
}


//----------------------------------------------------------------------------
void EGraphTool::InitDrawVariables()
{
  fLayout1 = new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2);
  fLayout2 = new TGLayoutHints(kLHintsExpandY, 2, 2, 2, 2);
  fLayout3 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2);
}


//----------------------------------------------------------------------------
void EGraphTool::InitInputFile(TString inputFileName)
{
  CloseInputFile(); // close previously initalized files

  // Open the input file

  fInputTreeFile = new TFile(inputFileName, "read");

  if (!fInputTreeFile->IsOpen()) return;

  // get the input tree

  fEvtTree = (TTree*)fInputTreeFile->Get("Views");

  if (fEvtTree) {
    fEmGraphRec->SetTree(fEvtTree);
    fEvtNumber = fEvtTree->GetEntries() - 1;
    cout << "number of events " << fEvtNumber << endl;
  }
  else 
    cout << "\"Views\" tree does not exist in this file" << endl;
}


//----------------------------------------------------------------------------
void EGraphTool::CloseInputFile()
{
  fEventId = fEvtNumber = 0;
  fEvtTree = NULL;
  fEmGraphRec->ClearEvent();
  if (fInputTreeFile) {SafeDelete(fInputTreeFile); fInputTreeFile = NULL;}
}


//----------------------------------------------------------------------------
void EGraphTool::ExitApplication()
{
  gApplication->Terminate(0);
}
