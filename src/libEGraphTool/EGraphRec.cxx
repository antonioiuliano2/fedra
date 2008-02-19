//----------------------------------------------------------------------------
// Program: Emulsion Graphical Reconstraction Toolkit - library
//
// Author:  Artem Chukanov (chukanov at nusun.jinr.ru)
//          31.01.2008
//
//----------------------------------------------------------------------------

#include "EGraphRec.h"
#include <TEnv.h>
#include <TGTab.h>
#include <TGLabel.h>
#include <TGTextView.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TBranchClones.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGLSAViewer.h>
#include <TView.h>
#include <TThread.h>
#include <iostream>

#include "EdbScanProc.h"

using namespace std;

ClassImp(EGraphRec)


//----------------------------------------------------------------------------
void *ThProcessEvent(void *ptr)
{
  EGraphRec   *egraph      = (EGraphRec*) ptr;
  EdbScanProc *sproc       = egraph->GetScanProc();
  EdbScanSet  *predScan    = egraph->GetPredScanProc();
  Int_t       *brickToProc = egraph->GetBrickToProc();
  ProcId_t     procId      = egraph->GetProcId();

  Int_t brickCalibCurr[4], brickCalibPrev[4];
  Int_t brickPredCurr[4],  brickPredPrev[4];

  brickPredCurr[0] = brickPredPrev[0] = brickCalibCurr[0] = 
    brickCalibPrev[0] = brickToProc[0]; // brick Id
  brickPredCurr[2] = brickPredPrev[2] = brickCalibCurr[2] = 
    brickCalibPrev[2] = brickToProc[2]; // version
  
  Int_t firstPlate = brickToProc[1];
  Int_t lastPlate  = brickToProc[3];

  Int_t step = (lastPlate >= firstPlate) ? 1 : -1;

  EdbPattern *predP = egraph->GetPredTracks();
  EdbPVRec   *foundTracks = egraph->GetFoundTracks();

  predP->Reset();
  
  for (Int_t plate = firstPlate; plate != lastPlate + step; plate += step) {

    brickPredCurr[1]  = brickCalibCurr[1] = plate;
    brickPredPrev[1]  = brickCalibPrev[1] = plate - step;
    brickCalibPrev[3] = brickCalibCurr[3] = procId.interCalib;
    brickPredPrev[3]  = brickPredCurr[3]  = procId.predScan;

    // linking process

    if (egraph->IsToProcLink()) {
      brickCalibCurr[3] = procId.interCalib;
      sproc->LinkRunAll(brickCalibCurr);
    }

    // alignment process

    if (egraph->IsToProcAlgn() && plate != firstPlate) {
      brickCalibCurr[3] = brickCalibPrev[3] = procId.interCalib;
      sproc->SetAFFDZ(brickCalibPrev, brickCalibCurr, 1300.*step);
      sproc->AlignAll(brickCalibPrev, brickCalibCurr, 1, 4, "-z");
    }

    // Scan back mode. Search predictions.

    sproc->CopyPar(brickCalibCurr, brickPredCurr);

    if (plate != firstPlate) {
      sproc->CopyAFFPar(brickCalibPrev, brickCalibCurr, 
			brickPredPrev,  brickPredCurr);
      sproc->ProjectFound(brickPredPrev, brickPredCurr);
    }

    // adding plate to the brick

    if (!predScan->FindPlateID(plate)) {
      predScan->AddPlate(new EdbID(brickPredCurr), step);

      // read affine and assemble "EdbBrickP" object

      sproc->AssembleScanSet(*predScan);
      predScan->SetAsReferencePlate(firstPlate);
    }

    // Put the reset according plate number!

    // sproc->ReadPred(*predP, brickPredCurr);

    // after scann

    foundTracks->ResetTracks();
    sproc->ReadFoundTracks(*predScan, *foundTracks);
    egraph->DrawEvent();

  }

  return 0;
}


//----------------------------------------------------------------------------
void *ThCheckProcessEvent(void *ptr)
{
  // Set enable "Execute Event" button after finishing process

  EGraphRec *egraph = (EGraphRec*) ptr;
  egraph->GetThProcessEvent()->Join();            // Join ThProcessEvent
  egraph->GetTextProcEvent()->SetEnabled(kTRUE);  // Enable button
  return 0;
}


//----------------------------------------------------------------------------
EGraphRec::EGraphRec()
{
  InitVariables();
  InitDrawVariables();
  ReadCmdConfig();      // reading config file
}


//----------------------------------------------------------------------------
EGraphRec::~EGraphRec() 
{
  SafeDelete(fGraphHits);
  SafeDelete(fThProcessEvent);
  SafeDelete(fThCheckProcessEvent);
  SafeDelete(fPredTracks);
  SafeDelete(fFoundTracks);
}


//----------------------------------------------------------------------------
void EGraphRec::ProcessEvent()
{
  // DrawEvent(nentries);
  
  fSproc->eProcDirClient = fDataDir;

  fBrickToProc[0] = fProcBrickEntry[0]->GetIntNumber(); // Brick Id
  fBrickToProc[1] = fProcBrickEntry[1]->GetIntNumber(); // first plate
  fBrickToProc[2] = fProcBrickEntry[3]->GetIntNumber(); // version
  fBrickToProc[3] = fProcBrickEntry[2]->GetIntNumber(); // last plate

  if (fPredScanProc && fPredScanProc->Brick().ID() != fBrickToProc[0]) {
    SafeDelete(fPredScanProc);
  }

  if (!fPredScanProc) {
    fPredScanProc = new EdbScanSet();
    fPredScanProc->Brick().SetID(fBrickToProc[0]);
  }

  fThProcessEvent->Run();              // Running Event Process in Thread mode
  fThCheckProcessEvent->Run();         // Check the end of job
  fTextProcEvent->SetEnabled(kFALSE);  // Disable Execution button

  // ReconstructTracks();
}


//----------------------------------------------------------------------------
void EGraphRec::ResetProcess()
{
  // Reset process

//   cout << "Reset Process" << endl;
  
//   if (fThProcessEvent) {
//     TThread::Delete(fThProcessEvent);
//     SafeDelete(fThProcessEvent);
//     fThProcessEvent      = new TThread("ThProcessEvent", 
//  				       ThProcessEvent, (void*) this);
//   }
}


//----------------------------------------------------------------------------
void EGraphRec::DrawEvent()
{
  TCanvas *canvasHits = fDisplayHits->GetCanvas();
  canvasHits->cd(); canvasHits->Clear();

//   fGraphHits->ClearEvent();

//   for (Int_t evt = 0; evt < 2; evt++) {
//     fEvtTree->GetEntry(evt);
//     fGraphHits->BuildEvent(fEvent);
//   }

  // fGraphHits->DrawHits();

  // fGraphHits->BuildEvent(fPredTracks,  "predicted");
  fGraphHits->BuildEvent(fFoundTracks, "found");
  fGraphHits->DrawTracks("all");

  canvasHits->Update();
}


//----------------------------------------------------------------------------
void EGraphRec::ReconstructTracks()
{
  EdbScanSet *scanBack = new EdbScanSet();

  Int_t firstPlate = fProcBrickEntry[1]->GetIntNumber();
  Int_t lastPlate  = fProcBrickEntry[2]->GetIntNumber();
  Int_t step = (lastPlate >= firstPlate) ? 1 : -1;

  Int_t brickToProc[4] = {fProcBrickEntry[0]->GetIntNumber(),
			  0, 
			  fProcBrickEntry[3]->GetIntNumber(),
			  fProcId.predScan};

  scanBack->eB.SetID(brickToProc[0]);

  for (Int_t plate = firstPlate; plate != lastPlate + step; plate += step) {
    brickToProc[1] = plate;
    scanBack->eIDS.Add(new EdbID(brickToProc));
  }

  // read affine and assemble "EdbBrickP" object
  fSproc->AssembleScanSet(*scanBack); 
  scanBack->SetAsReferencePlate(firstPlate);
  scanBack->Print();


//   EdbScanSet sc10;     sc10.eB.SetID(BRICK);   
//   EdbScanSet sc100;    sc100.eB.SetID(BRICK);  
//   EdbScanSet sc20;     sc20.eB.SetID(BRICK);   
//   EdbScanSet sc200;    sc200.eB.SetID(BRICK);  
//   EdbScanSet sc400;    sc400.eB.SetID(BRICK);  

//   make_id_list_20(sc20);
//   make_id_list_400(sc400);

 
//   //make_id_list_10(sc10);
//   //make_id_list_100(sc100);

//   dproc = new EdbDataProc();
//   gAli  = dproc->PVR();
//   EdbScanCond *cond = new EdbScanCond();
//   SetCondBT(*cond);
//   gAli->SetScanCond(cond);

//   EdbSegP s(s1p);
//   float dxy=5000, dtxy=0.05;
//   TCut cut = "eN1==1&&eN2==1&&eCHI2P<3.5&&s.eW>16";
//   TCut QC = "s.eW>15+3*s.eChi2";
//   char spos[128]; sprintf(spos,"sqrt((s.eX-(%f))*(s.eX-(%f))+(s.eY-(%f))*(s.eY-(%f)))<%f", s1p.X(), s1p.X(), s1p.Y(), s1p.Y(),dxy);
//   char sang1[128]; sprintf(sang1,"sqrt((s.eTX-(%f))*(s.eTX-(%f))+(s.eTY-(%f))*(s.eTY-(%f)))<%f", s1p.TX(), s1p.TX(), s1p.TY(), s1p.TY(),dtxy);
//   cut+=QC;
//   sproc.ReadScanSetCP(sc20,  *gAli, cut);
}


//----------------------------------------------------------------------------
void EGraphRec::SetTree(TTree *tree)
{
  fEvent = new EdbView();
  fEvtTree = tree;

  TBranch       *headerBranch   = fEvtTree->GetBranch("headers");
  TBranchClones *clustersBranch = (TBranchClones*)(fEvtTree->GetBranch("clusters"));
  TBranchClones *segmentsBranch = (TBranchClones*)(fEvtTree->GetBranch("segments"));
  TBranchClones *tracksBranch   = (TBranchClones*)(fEvtTree->GetBranch("tracks"));
  TBranchClones *framesBranch   = (TBranchClones*)(fEvtTree->GetBranch("frames"));

  clustersBranch->ResetReadEntry();
  segmentsBranch->ResetReadEntry();
  tracksBranch->ResetReadEntry();
  framesBranch->ResetReadEntry();

  headerBranch->SetAddress(fEvent->GetHeaderAddr());
  clustersBranch->SetAddress(fEvent->GetClustersAddr());
  segmentsBranch->SetAddress(fEvent->GetSegmentsAddr());
  tracksBranch->SetAddress(fEvent->GetTracksAddr());
  framesBranch->SetAddress(fEvent->GetFramesAddr());
}


//----------------------------------------------------------------------------
void EGraphRec::AddProcBrickFrame(TGVerticalFrame *workframe)
{
  TGLabel *label;
  TGHorizontalFrame *frame;

  TGVButtonGroup *GroupProcBrick = new TGVButtonGroup(workframe,
						      "Processing brick");

  // BrickId

  frame = new TGHorizontalFrame(GroupProcBrick);
  label = new TGLabel(frame, "Brick Id");
  fProcBrickEntry[0] = new TGNumberEntry(frame, fProcBrick.brickId, 7, 0, 
					 TGNumberEntry::kNESInteger,
					 TGNumberEntry::kNEANonNegative,
					 TGNumberEntry::kNELLimitMin, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fProcBrickEntry[0], fLayoutRightExpY);
  GroupProcBrick->AddFrame(frame, fLayout1);

  // First plate to process

  frame = new TGHorizontalFrame(GroupProcBrick);
  label = new TGLabel(frame, "First plate");
  fProcBrickEntry[1] = new TGNumberEntry(frame, fProcBrick.firstPlate, 3, 1, 
					 TGNumberEntry::kNESInteger,
					 TGNumberEntry::kNEANonNegative,
					 TGNumberEntry::kNELLimitMinMax, 0, 58);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fProcBrickEntry[1], fLayoutRightExpY);
  GroupProcBrick->AddFrame(frame, fLayout1);

  // Last plate to process

  frame = new TGHorizontalFrame(GroupProcBrick);
  label = new TGLabel(frame, "Last plate");
  fProcBrickEntry[2] = new TGNumberEntry(frame, fProcBrick.lastPlate, 3, 2, 
					 TGNumberEntry::kNESInteger,
					 TGNumberEntry::kNEANonNegative,
					 TGNumberEntry::kNELLimitMinMax,0,58);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fProcBrickEntry[2], fLayoutRightExpY);
  GroupProcBrick->AddFrame(frame, fLayout1);

  // Version

  frame = new TGHorizontalFrame(GroupProcBrick);
  label = new TGLabel(frame, "Base version");
  fProcBrickEntry[3] = new TGNumberEntry(frame, fProcBrick.ver, 3, 3, 
					 TGNumberEntry::kNESInteger,
					 TGNumberEntry::kNEANonNegative,
					 TGNumberEntry::kNELLimitMin, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fProcBrickEntry[3], fLayoutRightExpY);
  GroupProcBrick->AddFrame(frame, fLayout1);

  workframe->AddFrame(GroupProcBrick, fLayout1);
}


//----------------------------------------------------------------------------
void EGraphRec::AddProcListFrame(TGVerticalFrame *workframe)
{
  TGVButtonGroup *GroupProcList = new TGVButtonGroup(workframe,"Process list");
  fCheckProcScan = new TGCheckButton(GroupProcList, "Scanning");
  fCheckProcLink = new TGCheckButton(GroupProcList, "Linking tracks");
  fCheckProcAlgn = new TGCheckButton(GroupProcList, "Alignment plates");
  fCheckProcTrks = new TGCheckButton(GroupProcList, "Reconstruct tracks");
  fCheckProcVrtx = new TGCheckButton(GroupProcList, "Reconstruct vertex");

//   fCheckProcLink->SetState(kButtonDown);
//   fCheckProcAlgn->SetState(kButtonDown);
  fCheckProcScan->SetEnabled(kFALSE);
  fCheckProcTrks->SetEnabled(kFALSE);
  fCheckProcVrtx->SetEnabled(kFALSE);

  workframe->AddFrame(GroupProcList, fLayout1);

  // Break process

  TGTextButton *endprocess = new TGTextButton(workframe, "Reset process");
  endprocess->Connect("Clicked()", "EGraphRec", this, "ResetProcess()");
  // endprocess->Associate(this);
  workframe->AddFrame(endprocess, fLayout1);

  // Running process

  fTextProcEvent = new TGTextButton(workframe, "Execute event");
  fTextProcEvent->Connect("Clicked()", "EGraphRec", this, "ProcessEvent()");

  workframe->AddFrame(fTextProcEvent, fLayout1);
}


//----------------------------------------------------------------------------
void EGraphRec::AddRecOptFrame(TGTab *worktab)
{
  TGCompositeFrame *tf;

  // Create a tab with reconstruction options buttons

  tf = worktab->AddTab("Reconstruction options");

  ReconstructionOptionsDlg(tf);
}


//--------------------------------------------------------------------------
void EGraphRec::ReconstructionOptionsDlg(TGCompositeFrame *DlgRec)
{
  TGLabel *label;
  TGHorizontalFrame *frame;

  // fRecParams = fGR->GetRecParameters();

  // Momentum

  frame = new TGHorizontalFrame(DlgRec);
  label = new TGLabel(frame, "momentum");
  fRecOptEntry[0] = new TGNumberEntry(frame, 1, //fRecParams->p, 
				      7, 0, 
				      TGNumberEntry::kNESRealTwo,
				      TGNumberEntry::kNEANonNegative,
				      TGNumberEntry::kNELLimitMin, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[0], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1);

  // Prob min

  frame = new TGHorizontalFrame(DlgRec);
  label  = new TGLabel(frame, "probmin");
  fRecOptEntry[1] = new TGNumberEntry(frame, 1, // fRecParams->probmin, 
				      7, 1,
				      TGNumberEntry::kNESRealTwo,
				      TGNumberEntry::kNEANonNegative,
				      TGNumberEntry::kNELLimitMinMax, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[1], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1);

  // Nseg min

  frame = new TGHorizontalFrame(DlgRec);
  label  = new TGLabel(frame, "nsegmin");
  fRecOptEntry[2] = new TGNumberEntry(frame, 1, // fRecParams->nsegmin, 
				      7, 2,
				      TGNumberEntry::kNESInteger,
				      TGNumberEntry::kNEAPositive,
				      TGNumberEntry::kNELLimitMin, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[2], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1); 

  // Max gap

  frame = new TGHorizontalFrame(DlgRec);
  label  = new TGLabel(frame, "maxgap");
  fRecOptEntry[3] = new TGNumberEntry(frame, 1, // fRecParams->maxgap, 
				      7, 3,
				      TGNumberEntry::kNESInteger,
				      TGNumberEntry::kNEAPositive,
				      TGNumberEntry::kNELLimitMin, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[3], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1);

  // Scanning conditions

  label = new TGLabel(DlgRec,"Scanning conditions:");
  DlgRec->AddFrame(label, fLayout1);

  // Coords sigma

  frame = new TGHorizontalFrame(DlgRec);
  label  = new TGLabel(frame, "Coords Sigma");
  fRecOptEntry[4] = new TGNumberEntry(frame, 1, //fGR->GetScanCond()->SigmaX(0), 
				      7, 4,
				      TGNumberEntry::kNESRealTwo,
				      TGNumberEntry::kNEANonNegative,
				      TGNumberEntry::kNELLimitMinMax, 0, 10);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[4], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1);

  // Tangents sigma

  frame = new TGHorizontalFrame(DlgRec);
  label  = new TGLabel(frame, "Tangents Sigma");
  fRecOptEntry[5] = new TGNumberEntry(frame,1, // fGR->GetScanCond()->SigmaTX(0), 
				      7, 5,
				      TGNumberEntry::kNESRealFour,
				      TGNumberEntry::kNEANonNegative,
				      TGNumberEntry::kNELLimitMinMax, 0, 1);
  frame->AddFrame(label, fLayoutLeftExpY);
  frame->AddFrame(fRecOptEntry[5], fLayoutRightExpY);
  DlgRec->AddFrame(frame, fLayout1);

  // buttons
    
  TGHorizontalFrame *Frame1 = new TGHorizontalFrame(DlgRec);

  TGTextButton *Apply = new TGTextButton(Frame1, "Apply", 2);
  // Apply->Connect("Clicked()", "EEViewer", this, "DoApplyRec()");
  Frame1->AddFrame(Apply, fLayout1);

  TGTextButton* Reset = new TGTextButton(Frame1, "Reset", 3);
  // Reset->Connect("Clicked()", "EEViewer", this, "DoResetRec()");
  Frame1->AddFrame(Reset, fLayout1);

  DlgRec->AddFrame(Frame1, fLayout1);
}


//----------------------------------------------------------------------------
void EGraphRec::AddCanvasFrame(TGTab *worktab)
{
  TGCompositeFrame *tf;

  tf = worktab->AddTab("cRec");
  fDisplayHits = new TRootEmbeddedCanvas("Reconstruction", tf, 900, 650);
  tf->AddFrame(fDisplayHits, fLayout3);

//   tf = worktab->AddTab("GL Viewer");
//   fDisplayHitsGL = new TRootEmbeddedCanvas("Reconstruction GL", tf, 900, 650);
//   fGLViewer = new TGLSAViewer(tf, fDisplayHitsGL->GetCanvas());

  // tf->AddFrame(fDisplayHitsGL, fLayout3);
}


//----------------------------------------------------------------------------
void EGraphRec::AddInfoFrame(TGVerticalFrame *workframe)
{
  fTextInfo = new TGTextView(workframe, 900, 200, kFixedHeight);
  const TGFont *font = gClient->GetFont("-*-courier-bold-r-*-*-18-*-*-*-*-*-*-*");
  fTextInfo->SetFont(font->GetFontStruct());
  workframe->AddFrame(fTextInfo, fLayout1);

  WriteInfo();
}

//----------------------------------------------------------------------------
void EGraphRec::Set3DViewer()
{
  fGLViewer = (TGLSAViewer*)fDisplayHits->GetCanvas()->GetViewer3D("ogl");
  fGLViewer->SetClearColor(38);
}


//----------------------------------------------------------------------------
void EGraphRec::ZoomIn()
{
  fDisplayHits->GetCanvas()->cd();
  fDisplayHits->GetCanvas()->GetView()->Zoom();
  fDisplayHits->GetCanvas()->Modified();
  fDisplayHits->GetCanvas()->Update();
}


//----------------------------------------------------------------------------
void EGraphRec::ZoomOut()
{
  fDisplayHits->GetCanvas()->cd();
  fDisplayHits->GetCanvas()->GetView()->UnZoom();
  fDisplayHits->GetCanvas()->Modified();
  fDisplayHits->GetCanvas()->Update();
}


//----------------------------------------------------------------------------
void EGraphRec::WriteInfo()
{
  fTextInfo->Clear();
  fTextInfo->AddLine("");
  fTextInfo->AddLine(" Read data from directory: " + fDataDir);
}

//----------------------------------------------------------------------------
void EGraphRec::InitVariables()
{
  fEvent               = NULL;
  fThProcessEvent      = NULL;
  fThCheckProcessEvent = NULL;
  fPredScanProc        = NULL;
  fDataDir             = "";

  fProcBrick.brickId = fProcBrick.firstPlate = fProcBrick.lastPlate = 
    fProcBrick.ver = -1;
  fProcId.interCalib = fProcId.volumeScan = fProcId.predScan = 
    fProcId.scanForth = -1;

  fGraphHits    = new EGraphHits();
  fSproc        = new EdbScanProc();
  fPredTracks   = new EdbPattern();
  fFoundTracks  = new EdbPVRec();

  // TThread functions

  fThProcessEvent      = new TThread("ThProcessEvent", 
				     ThProcessEvent, (void*) this);
  fThCheckProcessEvent = new TThread("ThCheckProcessEvent",
				     ThCheckProcessEvent, (void*) this);
}

//----------------------------------------------------------------------------
void EGraphRec::InitDrawVariables()
{
  fLayout1 = new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2);
  fLayout2 = new TGLayoutHints(kLHintsExpandY, 2, 2, 2, 2);
  fLayout3 = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2);
  fLayoutLeftExpY  = new TGLayoutHints(kLHintsLeft|kLHintsExpandY, 2, 4, 0, 0);
  fLayoutRightExpY = new TGLayoutHints(kLHintsRight|kLHintsExpandY,4, 2, 0, 0);
}


//----------------------------------------------------------------------------
void EGraphRec::ReadCmdConfig()
{
  TString configDir  = (TString)getenv("FEDRA_ROOT") + "/config/";
  TString configFile = configDir + "EGraphRec";

  if (gEnv->ReadFile(configFile + ".cfg", kEnvChange) == -1) 
    if (gEnv->ReadFile(configFile + "_default.cfg", kEnvChange) == -1) return;

  if (fDataDir == "")
    fDataDir = gEnv->GetValue("EGraphRec.DataDir", "");

  // brick Id

  if (fProcBrick.brickId < 0)
    fProcBrick.brickId = gEnv->GetValue("EGraphRec.ProcBrick.brickId", 0);
  if (fProcBrick.firstPlate < 0)
    fProcBrick.firstPlate = gEnv->GetValue("EGraphRec.ProcBrick.firstPlate", 0);
  if (fProcBrick.lastPlate < 0)
    fProcBrick.lastPlate = gEnv->GetValue("EGraphRec.ProcBrick.lastPlate", 0);
  if (fProcBrick.ver < 0)
    fProcBrick.ver = gEnv->GetValue("EGraphRec.ProcBrick.ver", 0);

  // processes Ids

  if (fProcId.interCalib < 0)
    fProcId.interCalib = gEnv->GetValue("EGraphRec.ProcId.interCalib", 0);
  if (fProcId.volumeScan < 0)
    fProcId.volumeScan = gEnv->GetValue("EGraphRec.ProcId.volumeScan", 0);
  if (fProcId.predScan < 0)
    fProcId.predScan = gEnv->GetValue("EGraphRec.ProcId.predScan", 0);
  if (fProcId.scanForth < 0)
    fProcId.scanForth = gEnv->GetValue("EGraphRec.ProcId.scanForth", 0);
}


//----------------------------------------------------------------------------
void EGraphRec::ClearEvent()
{
  if (fEvent) {SafeDelete(fEvent); fEvent = NULL;}
}
