#ifndef EGRAPHREC_H
#define EGRAPHREC_H 1

#include <TROOT.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TTree.h>
#include "EdbView.h"
#include "EGraphHits.h"

class TGTab;
class TRootEmbeddedCanvas;
class TGLSAViewer;

class EGraphRec {
 public:

  EGraphRec();
  virtual ~EGraphRec();

  void ProcessEvent(Int_t nentries);
  void AddRecOptFrame(TGTab *worktab);
  void AddCanvasFrame(TGTab *worktab);
  void SetTree(TTree *tree);
  void Set3DViewer();
  void ZoomIn();
  void ZoomOut();
  void ClearEvent();

 private:

  TTree               *fEvtTree;
  EdbView             *fEvent;
  EGraphHits          *fGraphHits;

  TGLayoutHints       *fLayout1;
  TGLayoutHints       *fLayout2;
  TGLayoutHints       *fLayout3;
  TGLayoutHints       *fLayoutLeftExpY;
  TGLayoutHints       *fLayoutRightExpY;
  TGNumberEntry       *fRecOptEntry[6];

  TRootEmbeddedCanvas *fDisplayHits;
  TRootEmbeddedCanvas *fDisplayHitsGL;
  TGLSAViewer         *fGLViewer;

  void InitVariables();
  void InitDrawVariables();
  void ReconstructionOptionsDlg(TGCompositeFrame *DlgRec);
  void DrawEvent(Int_t nentries);

  ClassDef(EGraphRec,0)

};

#endif
