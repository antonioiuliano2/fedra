//----------------------------------------------------------------------------
// Program: Emulsion Graphical Reconstraction Toolkit - library
//
// Author:  Artem Chukanov (chukanov at nusun.jinr.ru)
//          31.01.2008
//
//----------------------------------------------------------------------------

#include "EGraphRec.h"
#include "EdbScanProc.h"
#include "EdbVertex.h"
#include <TThread.h>
#include <iostream>



using namespace std;
using namespace TMath;

ClassImp(EGraphRecProc)


//----------------------------------------------------------------------------
void *ThSBProcess(void *ptr)
{
  EGraphRec   *egraph      = (EGraphRec*) ptr;
  EdbScanProc *scanProc    = egraph->GetScanProc();    // Scan Proc
  EdbScanSet  *scanSet     = egraph->GetScanSet();     // Scan Set
  EdbPattern  *predTracks  = egraph->GetPredTracks();  // predicted tracks
  EdbPVRec    *foundTracks = egraph->GetFoundTracks(); // found tracks
  ProcBrick_t  brickToProc = egraph->GetBrickToProc(); // Brick to process
  ProcId_t     procId      = egraph->GetProcId();

  Int_t pID_IC[4]      = {brickToProc.brickId, 0, brickToProc.ver, 
			  procId.interCalib};
  Int_t pID_PS[4]      = {brickToProc.brickId, 0, brickToProc.ver, 
			  procId.predScan};
  Int_t pID_IC_prev[4] = {brickToProc.brickId, 0, brickToProc.ver, 
			  procId.interCalib};
  Int_t pID_PS_prev[4] = {brickToProc.brickId, 0, brickToProc.ver, 
			  procId.predScan};

  Int_t firstPlate = brickToProc.firstPlate;
  Int_t lastPlate  = brickToProc.lastPlate;
  Int_t step=(lastPlate>=firstPlate) ? brickToProc.step : -1*brickToProc.step;
  Int_t nFailurePlates = 0;

  predTracks->Reset();
  
  for (Int_t plate = firstPlate; plate != lastPlate + step; plate += step) {

    pID_PS[1]      = pID_IC[1]      = plate;
    pID_PS_prev[1] = pID_IC_prev[1] = plate - step;

    // get prediction for the current plate

    // scanProc->ReadPred(*predTrack, pID_PS);

    // scanning intercalibration zone

    // Checking directory (if there is no scanning)

    TString str;
    scanProc->MakeFileName(str, pID_PS, "root");

    if (!gSystem->OpenDirectory(gSystem->DirName(str))) {
      cout << "ERROR! Directory " << gSystem->DirName(str) 
	   << " does not exist.\n";
      cout << "Please run scanning procedure for the plate number " 
	   << pID_PS[1] << endl;
      continue;
    }

    // linking process

    if (egraph->IsSBToLink() && !scanProc->LinkRunAll(pID_IC)) return 0;

    // alignment process between previous and current plate

    if (egraph->IsSBToAlgn() && plate != firstPlate) {
      if (!scanProc->SetAFFDZ(pID_IC_prev, pID_IC, 1300.*step)) return 0;
      if (scanProc->AlignAll(pID_IC_prev, pID_IC, 1, 4, "-z") < 0) return 0;
    }

    // Scan back mode. Search predictions.

    scanProc->CopyPar(pID_IC, pID_PS);

    if (plate != firstPlate) {
      scanProc->CopyAFFPar(pID_IC_prev, pID_IC, pID_PS_prev,  pID_PS);
      scanProc->ProjectFound(pID_PS_prev, pID_PS);
    }

    // adding scanned plate to the brick
    // read affine and assemble "EdbBrickP" object

    if (scanSet->AddID(new EdbID(pID_PS), step)) {
      if (scanProc->AssembleScanSet(*scanSet) < 1) return 0;
      scanSet->SetAsReferencePlate(firstPlate);
    }

    // after scann

    foundTracks->ResetTracks();
    scanProc->ReadFoundTracks(*scanSet, *foundTracks);

    // come back to previous scanned plate + 1 if there are no tracks were
    // found

    EdbPattern pat;
    if (!scanProc->ReadFound(pat, pID_PS)) {
      if (Abs(step) != 1) {
	plate += -step + (step > 0 ? 1 : -1);
	if (!nFailurePlates) plate += step;
      }
      nFailurePlates++;
      cout << "Track does not found in the plate " << plate 
	   << ". starting scann plate " << plate + step << endl;
    }
    else nFailurePlates = 0;

    egraph->DrawEvent();

    if (nFailurePlates == 3) {
      cout << "There are no any tracks during " << nFailurePlates 
	   << "plates. Stop scanning" << endl;
      return 0;
    }
  }

  return 0;
}


//----------------------------------------------------------------------------
void *ThSBCheckProcess(void *ptr)
{
  // Set enable "Execute Event" button after finishing process

  EGraphRec *egraph = (EGraphRec*) ptr;
  egraph->GetThSBProcess()->Join();              // Join ThSBProcess
  egraph->GetButtonSBStart()->SetEnabled(kTRUE); // Enable button
  return 0;
}


//----------------------------------------------------------------------------
EGraphRecProc::EGraphRecProc()
{
  fNsegmin  = 1;     // minimal number of segments to propagate this track
  fNgapmax  = 2;     // maximal gap for propagation
  fMomentum = 0.5;   // GeV
  fMass     = 0.139; // particle mass
  fProbMinP = 0.001; // minimal probability to accept segment on propagation

  fQualityMode = 0;     // vertex quality estimation method 
                        // (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average 
                        // track-vertex distance)
  fUseMom      = true;  // use or not track momentum for vertex calculations
  fUseSegPar   = false; // use only the nearest measured segments for 
                        // vertex fit (as Neuchatel)
  fDZmax       = 3000.; // maximum z-gap in the track-vertex group
  fProbMinV    = 0.001; // minimum acceptable probability for chi2-distance 
                        // between tracks
  fImpMax      = 20.;   // maximal acceptable impact parameter [microns] 
                        // (for preliminary check)
}


//----------------------------------------------------------------------------
EGraphRecProc::~EGraphRecProc() 
{

}


//----------------------------------------------------------------------------
EdbVertexRec *EGraphRecProc::VertexRec()
{
  Int_t pID_VS[4] = {fBrickToProc.brickId, 0, fBrickToProc.ver,
		     fProcId.volumeScan};

  Int_t firstPlate = fBrickToProc.firstPlate;
  Int_t lastPlate  = fBrickToProc.lastPlate;
  Int_t step=(lastPlate>=firstPlate)?fBrickToProc.step : -1*fBrickToProc.step;

  // make a brick assuming that all plates are linked and aligned

  for (Int_t plate = firstPlate; plate != lastPlate + step; plate += step) {
    pID_VS[1] = plate;
    fScanSet->AddID(new EdbID(pID_VS), step);
  }

  fScanProc->AssembleScanSet(*fScanSet);
  fScanSet->SetAsReferencePlate(firstPlate);

  // get the tracks passing through our cuts

  EdbDataProc *dproc = new EdbDataProc();
  EdbScanCond *cond  = new EdbScanCond();
  SetCondBT(cond);

  fFoundTracks = dproc->PVR();
  fFoundTracks->SetScanCond(cond);

  TCut cut = "eN1==1&&eN2==1&&eCHI2P<3.5&&s.eW>16";
  TCut QC = "s.eW>15+3*s.eChi2";
  cut += QC;

  fScanProc->ReadScanSetCP(*fScanSet, *fFoundTracks, cut);

  // tracking

  fFoundTracks->SetCouplesPeriodic(0,1);
  dproc->LinkTracksWithFlag(fFoundTracks, fMomentum, fProbMinP, fNsegmin,
			    fNgapmax, 0);

  PropagateTracks();
  fFoundTracks->FillCell(30,30,0.009,0.009);
  SetEVR();

  cout << fVertexRec->eEdbTracks->GetEntries() << " tracks for vertexing" 
       << endl;
  cout << fVertexRec->FindVertex() << " 2-track vertexes was found" << endl;
  fVertexRec->ProbVertexN();

  return fVertexRec;
}


//----------------------------------------------------------------------------
void EGraphRecProc::PropagateTracks()
{
  // example of additional propagation and 
  // other tracking operations if necessary

  Int_t ntracks = fFoundTracks->eTracks->GetEntries();

  // set tracks IDs

  for (Int_t i = 0; i < ntracks; i++) {
    EdbTrackP *track = (EdbTrackP*)(fFoundTracks->eTracks->At(i));
    track->SetID(i);
    track->SetSegmentsTrack();
    track->SetErrorP(0.2*0.2*fMomentum*fMomentum);

    if (track->Flag() < 0) continue;
    fFoundTracks->PropagateTrack(*track, true, 0.001, 3, 0);
  }
}


//----------------------------------------------------------------------------
void EGraphRecProc::SetEVR()
{
  fVertexRec = new EdbVertexRec();

  fVertexRec->eEdbTracks   = fFoundTracks->eTracks;
  fVertexRec->eVTX         = fFoundTracks->eVTX;
  fVertexRec->SetPVRec(fFoundTracks);
  fVertexRec->eDZmax       = fDZmax;
  fVertexRec->eProbMin     = fProbMinV;
  fVertexRec->eImpMax      = fImpMax;
  fVertexRec->eUseMom      = fUseMom;
  fVertexRec->eUseSegPar   = fUseSegPar;
  fVertexRec->eQualityMode = fQualityMode;
}

//----------------------------------------------------------------------------
void EGraphRecProc::SetCondBT(EdbScanCond *cond)
{
  cond->SetSigma0(5., 5., 0.002, 0.002); // sigma0 "x, y, tx, ty" at zero angle
  cond->SetDegrad(5.);                   // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(3, 3, 3, 3);             // bins in [sigma] for checks
  cond->SetPulsRamp0(5., 5.);            // in range (Pmin:Pmax) Signal/All 
                                         // is nearly linear
  cond->SetPulsRamp04(5., 5.);
  cond->SetChi2Max(6.5);
  cond->SetChi2PMax(6.5);
  cond->SetChi2Mode(3);
  cond->SetRadX0(5810.);
  cond->SetName("OPERA_basetrack");
}
