#ifndef EGRAPHREC_H
#define EGRAPHREC_H 1

#include <TROOT.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TTree.h>
#include "EdbView.h"
#include "EGraphRecProc.h"

class TThread;
class TGTab;
class TGTextView;
class TRootEmbeddedCanvas;
class TGLSAViewer;
class EdbScanProc;
class EdbPattern;
class EdbScanSet;
class EdbPVRec;
class EdbVertexRec;
class EdbDisplay;


class EGraphRec {
 public:

  EGraphRec();
  virtual ~EGraphRec();

  void ProcessEvent();
  void AddProcBrickFrame(TGVerticalFrame *workframe);
  void AddProcListFrame(TGVerticalFrame *workframe);
  void AddRecOptFrame(TGTab *worktab);
  void AddScanBackFrame(TGTab *worktab);
  void AddVertexRecFrame(TGTab *worktab);
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

  void ReadSBPred();
  void StartScanBack();
  void StartVertexRec();


  Bool_t        IsSBToLink()        const   {return fCheckSBLink->IsDown();}
  Bool_t        IsSBToAlgn()        const   {return fCheckSBAlgn->IsDown();}
  ProcBrick_t   GetBrickToProc()    const   {return fBrickToProc;}
  ProcId_t      GetProcId()         const   {return fProcId;}
  EdbScanProc  *GetScanProc()       const   {return fScanProc;}
  EdbScanSet   *GetScanSet()        const   {return fScanSet;}
  EdbPattern   *GetPredTracks()     const   {return fPredTracks;}
  EdbPVRec     *GetFoundTracks()    const   {return fFoundTracks;}
  TThread      *GetThSBProcess()    const   {return fThSBProcess;}
  TGTextButton *GetButtonSBStart()          {return fButtonSBStart;}

  void SetDataDir(TString &dataDir)         {fDataDir = dataDir;}
  void SetProcId(ProcId_t &procId)          {fProcId = procId;}
  void SetProcBrick(ProcBrick_t &procBrick) {fProcBrick = procBrick;}

 private:

  ProcBrick_t          fBrickToProc;
  TTree               *fEvtTree;
  TString              fDataDir;
  EdbView             *fEvent;
  EdbScanProc         *fScanProc;
  EdbPattern          *fPredTracks;
  EdbPVRec            *fFoundTracks;
  EdbVertexRec        *fVertexRec;
  EdbScanSet          *fScanSet;
  EdbDisplay          *fDisplay;
  ProcId_t             fProcId;
  ProcBrick_t          fProcBrick;
  TThread             *fThSBProcess;
  TThread             *fThSBCheckProcess;

  TGLayoutHints       *fLayout1;
  TGLayoutHints       *fLayout2;
  TGLayoutHints       *fLayout3;
  TGLayoutHints       *fLayoutLeftExpY;
  TGLayoutHints       *fLayoutRightExpY;
  TGNumberEntry       *fRecOptEntry[6];
  TGNumberEntry       *fEntryProcBrickId;
  TGNumberEntry       *fEntryProcVer;
  TGTextView          *fTextInfo;
  TGCheckButton       *fCheckProcScan;
  TGCheckButton       *fCheckProcLink;
  TGCheckButton       *fCheckProcAlgn;
  TGCheckButton       *fCheckProcTrks;
  TGCheckButton       *fCheckProcVrtx;
  TGTextButton        *fTextProcEvent;

  // scan back parameters

  TGNumberEntry       *fEntrySBFirstPlate;
  TGNumberEntry       *fEntrySBLastPlate;
  TGNumberEntry       *fEntrySBStep;
  TGTextButton        *fButtonSBStart;
  TGCheckButton       *fCheckSBScan;
  TGCheckButton       *fCheckSBLink;
  TGCheckButton       *fCheckSBAlgn;
  TGCheckButton       *fCheckSBTrks;

  TRootEmbeddedCanvas *fDisplayHits;
  // TRootEmbeddedCanvas *fDisplayHitsGL;
  TRootEmbeddedCanvas *fDisplayFedra;
  TGLSAViewer         *fGLViewer;

  void InitScanSet();
  void InitVariables();
  void ReadCmdConfig();
  void InitDrawVariables();
  void ReconstructionOptionsDlg(TGCompositeFrame *DlgRec);
  void WriteInfo();

  ClassDef(EGraphRec,0)

};

#endif
