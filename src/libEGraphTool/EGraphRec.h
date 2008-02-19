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
class EdbPattern;
class EdbScanSet;
class EdbPVRec;

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
  void ReconstructTracks();
  void DrawEvent();
  void ResetProcess();
  void SetTree(TTree *tree);
  void Set3DViewer();
  void ZoomIn();
  void ZoomOut();
  void ClearEvent();

  Bool_t        IsToProcLink()      const   {return fCheckProcLink->IsDown();}
  Bool_t        IsToProcAlgn()      const   {return fCheckProcAlgn->IsDown();}
  Int_t        *GetBrickToProc()            {return fBrickToProc;}
  ProcId_t      GetProcId()         const   {return fProcId;}
  EdbScanProc  *GetScanProc()       const   {return fSproc;}
  EdbPattern   *GetPredTracks()     const   {return fPredTracks;}
  EdbPVRec     *GetFoundTracks()    const   {return fFoundTracks;}
  EdbScanSet   *GetPredScanProc()   const   {return fPredScanProc;}
  TThread      *GetThProcessEvent() const   {return fThProcessEvent;}
  TGTextButton *GetTextProcEvent()          {return fTextProcEvent;}

  void SetDataDir(TString &dataDir)         {fDataDir = dataDir;}
  void SetProcId(ProcId_t &procId)          {fProcId = procId;}
  void SetProcBrick(ProcBrick_t &procBrick) {fProcBrick = procBrick;}

 private:

  Int_t                fBrickToProc[4];
  TTree               *fEvtTree;
  TString              fDataDir;
  EdbView             *fEvent;
  EdbScanProc         *fSproc;
  EdbPattern          *fPredTracks;
  EdbPVRec            *fFoundTracks;
  EdbScanSet          *fPredScanProc;
  EGraphHits          *fGraphHits;
  ProcId_t             fProcId;
  ProcBrick_t          fProcBrick;
  TThread             *fThProcessEvent;
  TThread             *fThCheckProcessEvent;

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
  void WriteInfo();

  ClassDef(EGraphRec,0)

};

#endif
