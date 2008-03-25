#ifndef EGRAPHRECPROC_H
#define EGRAPHRECPROC_H 1

#include <TROOT.h>

class TThread;
class EdbScanProc;
class EdbScanSet;
class EdbScanCond;
class EdbVertexRec;
class EdbPVRec;

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

// vertex reconstruction options

typedef struct {
  Int_t   QualityMode;  // vertex quality estimation method 
                        // (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average 
                        // track-vertex distance)
  Bool_t  UseMom;       // use or not track momentum for vertex calculations
  Bool_t  UseSegPar;    // use only the nearest measured segments for 
                        // vertex fit (as Neuchatel)
  Float_t DZmax;        // maximum z-gap in the track-vertex group
  Float_t ProbMinV;     // minimum acceptable probability for chi2-distance 
                        // between tracks
  Float_t ImpMax;       // maximal acceptable impact parameter [microns] 
                        // (for preliminary check)
} VertexRecOpt_t;


// TThread functions

void *ThSBProcess(void *ptr);
void *ThSBCheckProcess(void *ptr);

class EGraphRecProc {
 public:

  EGraphRecProc();
  virtual ~EGraphRecProc();

  void SetScanProc(EdbScanProc *scanProc)       {fScanProc = scanProc;}
  void SetBrickToProc(ProcBrick_t &brickToProc) {fBrickToProc = brickToProc;}
  void SetProcId(ProcId_t &procId)              {fProcId = procId;}
  void SetVertexRecOpt(VertexRecOpt_t &recOpt)  {fVertexRecOpt = recOpt;}

  EdbPVRec *VertexRec();

 private:

  // tracks reconstruction options

  Int_t   fNsegmin;   // minimal number of segments to propagate this track
  Int_t   fNgapmax;   // maximal gap for propagation
  Float_t fMomentum;  // GeV
  Float_t fMass;      // particle mass
  Float_t fProbMinP;  // minimal probability to accept segment on propagation

  EdbPVRec      *fPVRec;
  EdbVertexRec  *fVertexRec;
  EdbScanProc   *fScanProc;
  EdbScanSet    *fScanSetVTX;
  EdbScanCond   *fScanCond;
  ProcId_t       fProcId;
  ProcBrick_t    fBrickToProc;
  VertexRecOpt_t fVertexRecOpt;

  void SetCondBT();
  void PropagateTracks();
  void SetEVR();

  ClassDef(EGraphRecProc,0)
};

#endif
