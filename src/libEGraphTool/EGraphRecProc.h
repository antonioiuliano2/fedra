#ifndef EGRAPHRECPROC_H
#define EGRAPHRECPROC_H 1

#include <TROOT.h>

class TThread;
class EdbScanProc;
class EdbScanSet;
class EdbScanCond;
class EdbVertexRec;

// brick to process

typedef struct {
  Int_t brickId;
  Int_t firstPlate;
  Int_t lastPlate;
  Int_t ver;
  Int_t step;
} ProcBrick_t;

// processes IDs

typedef struct {
  Int_t interCalib;
  Int_t volumeScan;
  Int_t predScan;
  Int_t scanForth;
} ProcId_t;

// TThread functions

void *ThSBProcess(void *ptr);
void *ThSBCheckProcess(void *ptr);

class EGraphRecProc {
 public:

  EGraphRecProc();
  virtual ~EGraphRecProc();

  void SetScanProc(EdbScanProc *scanProc)       {fScanProc = scanProc;}
  void SetScanSet(EdbScanSet *scanSet)          {fScanSet = scanSet;}
  void SetBrickToProc(ProcBrick_t &brickToProc) {fBrickToProc = brickToProc;}
  void SetProcId(ProcId_t & procId)             {fProcId = procId;}

  EdbPVRec *VertexRec();

 private:

  // tracks reconstruction options

  Int_t   fNsegmin;   // minimal number of segments to propagate this track
  Int_t   fNgapmax;   // maximal gap for propagation
  Float_t fMomentum;  // GeV
  Float_t fMass;      // particle mass
  Float_t fProbMinP;  // minimal probability to accept segment on propagation

  // vertex reconstruction options

  Int_t   fQualityMode;
  Bool_t  fUseMom;
  Bool_t  fUseSegPar;
  Float_t fDZmax;
  Float_t fProbMinV;
  Float_t fImpMax;

  EdbPVRec     *fFoundTracks;
  EdbVertexRec *fVertexRec;
  EdbScanProc  *fScanProc;
  EdbScanSet   *fScanSet;
  ProcId_t      fProcId;
  ProcBrick_t   fBrickToProc;

  void SetCondBT(EdbScanCond *cond);
  void PropagateTracks();
  void SetEVR();

  ClassDef(EGraphRecProc,0)
};

#endif
