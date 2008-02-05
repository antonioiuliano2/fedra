#ifndef EGRAPHTOOL_H
#define EGRAPHTOOL_H 1

#include <TApplication.h>
#include <TGClient.h>
#include <TGToolBar.h>
#include <TGMenu.h>
#include <TFile.h>
#include <TTree.h>
#include "EGraphRec.h"
#include <iostream>

using namespace std;
using namespace TMath;

class EGraphTool : public TGMainFrame {
 public:

  EGraphTool (const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~EGraphTool();

  void   ProcessEvent();
  void   ExitApplication();
  Bool_t ProcessMessage(Long_t msg, Long_t param, Long_t);

 private:

  Int_t            fEventId;
  Int_t            fEvtNumber;
  TFile           *fInputTreeFile;
  TTree           *fEvtTree;

  TGVerticalFrame *fMainFrame;
  TGLayoutHints   *fLayout1;
  TGLayoutHints   *fLayout2;
  TGLayoutHints   *fLayout3;
  EGraphRec       *fEmGraphRec;
  TGPopupMenu     *fMenuFile;
  TGToolBar       *fToolBar;

  void  DrawFrame();
  void  AddMenuBar(TGVerticalFrame *frame);
  void  InitVariables();
  void  InitDrawVariables();
  void  InitInputFile(TString inputFileName);
  void  CloseInputFile();

  ClassDef(EGraphTool,0)
};

#endif
