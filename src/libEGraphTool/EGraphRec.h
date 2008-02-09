#ifndef EGRAPHREC_H
#define EGRAPHREC_H 1

#include <TROOT.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TTree.h>
#include "EdbView.h"
#include "EGraphHits.h"

class TThread;
class TGTab;
class TGTextView;
class TRootEmbeddedCanvas;
class TGLSAViewer;
class EdbScanProc;

// brick to process

typedef struct {
  Int_t brickId;
  Int_t firstPlate;
  Int_t lastPlate;
  Int_t ver;
} ProcBrick_t;

// processes IDs

typedef struct {
  Int_t interCalib;
  Int_t volumeScan;
  Int_t predScan;
  Int_t scanForth;
} ProcId_t;

class EGraphRec {
 public:

  EGraphRec();
  virtual ~EGraphRec();

  void ProcessEvent();
  void AddProcBrickFrame(TGVerticalFrame *workframe);
  void AddProcListFrame(TGVerticalFrame *workframe);
  void AddRecOptFrame(TGTab *worktab);
  void AddCanvasFrame(TGTab *worktab);
  void AddInfoFrame(TGVerticalFrame *workframe);
  void ResetProcess();
  void SetTree(TTree *tree);
  void Set3DViewer();
  void ZoomIn();
  void ZoomOut();
  void ClearEvent();

  Int_t        *GetBrickToProc()             {return fBrickToProc;}
  EdbScanProc  *GetScanProc()      const     {return fSproc;}
  TThread      *GetThLinkProcess() const     {return fThLinkProcess;}
  TGTextButton *GetTextProcEvent()           {return fTextProcEvent;}

  void SetDataDir(TString &dataDir)         {fDataDir = dataDir;}
  void SetProcId(ProcId_t &procId)          {fProcId = procId;}
  void SetProcBrick(ProcBrick_t &procBrick) {fProcBrick = procBrick;}

 private:

  Int_t                fBrickToProc[4];
  TTree               *fEvtTree;
  TString              fDataDir;
  EdbView             *fEvent;
  EdbScanProc         *fSproc;
  EGraphHits          *fGraphHits;
  ProcId_t             fProcId;
  ProcBrick_t          fProcBrick;
  TThread             *fThLinkProcess;
  TThread             *fThCheckLinkProcess;

  TGLayoutHints       *fLayout1;
  TGLayoutHints       *fLayout2;
  TGLayoutHints       *fLayout3;
  TGLayoutHints       *fLayoutLeftExpY;
  TGLayoutHints       *fLayoutRightExpY;
  TGNumberEntry       *fRecOptEntry[6];
  TGNumberEntry       *fProcBrickEntry[4];
  TGTextView          *fTextInfo;
  TGCheckButton       *fCheckProcScan;
  TGCheckButton       *fCheckProcLink;
  TGCheckButton       *fCheckProcAlgn;
  TGCheckButton       *fCheckProcTrks;
  TGCheckButton       *fCheckProcVrtx;
  TGTextButton        *fTextProcEvent;

  TRootEmbeddedCanvas *fDisplayHits;
  TRootEmbeddedCanvas *fDisplayHitsGL;
  TGLSAViewer         *fGLViewer;

  void InitVariables();
  void ReadCmdConfig();
  void InitDrawVariables();
  void ReconstructionOptionsDlg(TGCompositeFrame *DlgRec);
  void DrawEvent(Int_t nentries);
  void WriteInfo();

  ClassDef(EGraphRec,0)

};

#endif
