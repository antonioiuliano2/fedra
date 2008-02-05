//----------------------------------------------------------------------------
//
// Program: EGraphHits
//
//          Drawing emulsion hits
//
//----------------------------------------------------------------------------

#include "EGraphHits.h"

// #include <TVector3.h>
// #include <TObjArray.h>
// #include <TPolyMarker3D.h>
// #include <TProfile.h>
// #include <TProfile2D.h>
// #include <TList.h>
// #include <TSystem.h>
// #include <TStyle.h>
// #include <TCanvas.h>
// #include <TChain.h>
// #include <TMath.h>
// #include <TF1.h>
// #include <TLegend.h>
#include <TPolyLine3D.h>

#include "EdbView.h"
#include "EdbSegment.h"

#include <iostream>

using namespace std;

ClassImp(EGraphHits)

//----------------------------------------------------------------------------
EGraphHits::EGraphHits() 
{
  //
  // Constructor
  //

//   fAllHits = new TPolyMarker3D;
//   fList = new TList;
  fAllSegments = new TObjArray();
}

//----------------------------------------------------------------------------
EGraphHits::~EGraphHits() 
{
  //
  // Destructor
  //

//   fList->Delete();
//   SafeDelete(fList);
  SafeDelete(fAllSegments);
}


//----------------------------------------------------------------------------
void EGraphHits::BuildEvent(EdbView *event)
{
  //
  // 
  //

  // fill the array by 3D line segments

  for (Int_t iseg = 0; iseg < event->Nsegments(); iseg++) {
    EdbSegment *seg = event->GetSegment(iseg);

    Double_t x1 = seg->GetX0();
    Double_t y1 = seg->GetY0();
    Double_t z1 = seg->GetZ0();
    Double_t x2 = seg->GetX0() + seg->GetDz()*seg->GetTx();
    Double_t y2 = seg->GetY0() + seg->GetDz()*seg->GetTy();
    Double_t z2 = seg->GetZ0() + seg->GetDz();

    TPolyLine3D *segment = new TPolyLine3D;
    segment->SetPoint(0, x1, y1, z1);
    segment->SetPoint(1, x2, y2, z2);
    fAllSegments->Add(segment);
  }



//   Int_t point = 0;
//   TObjArray *fDetResponse = fEvent->GetDetResponse();

//   if (!fDetResponse) {
//     Warning("BuildEvent()","No Detector Response is found");
//     return;
//   }

//   // make a special binning for Edep vs velocity

//   Double_t xbins[41];
//   Double_t step = 0.1; // MeV
//   Double_t xmin = 0.1; // MeV

//   for (Int_t bin(0); bin < 4; bin++) {
//     for (Int_t i(0); i <= 10; i++) xbins[10*bin+i] = xmin + i*step;
//     step *= 10.;
//     xmin *= 10.;
//   }

//   for (Int_t i(0); i < fDetResponse->GetEntries(); i++) {
//     DetectorResponse *dr = (DetectorResponse*)fDetResponse->At(i);
//     TObjArray *fPlates = dr->GetPlates();
//     if (!fPlates) {
//       Warning("BuildEvent()","No Plates are found. Very strange!!!");
//       return;
//     }

//     for (Int_t j(0); j < fPlates->GetEntries(); j++) {
//       VMCPlate *plate = (VMCPlate*)fPlates->At(j);
//       TObjArray *fHits = plate->GetHits();

//       if (!fHits) {
// 	Warning("BuildEvent()","No Hits are found. Very Very strange!!!");
// 	return;
//       }
      
//       TH1* h2 = GetHisto("hits_energy","Mean Energy of Hits","1D", 100, 0, 50);
//       TH1F* htmp = new TH1F("hits_energy_tmp","", 100, 0, 200);
//       TH1* HitsEnergy = GetHisto("hits_energy_in_track",
// 				 "Track Energy Release in One Plate", "1D",
// 				 100, 0, 50);
	      
//       map <Int_t, Double_t> NumOfHits;
//       map <Int_t, Double_t> LengthOfTrack;

//       NumOfHits.clear(); LengthOfTrack.clear();

//       for (Int_t k(0); k < fHits->GetEntries(); k++) {

// 	VMCHit *hit = (VMCHit*)fHits->At(k);

// 	NumOfHits[hit->TrackId()] += hit->EnergyReleased();
		   
// 	TVector3 pos1 = hit->BegPosition();
// 	TVector3 pos2 = hit->EndPosition();
// 	TVector3 pos = 0.5*(pos1 + pos2);
// 	Double_t dl = (pos2-pos1).Mag();
// 	LengthOfTrack[hit->TrackId()] += dl;

// 	TPolyLine3D *segment = new TPolyLine3D;
// 	segment->SetPoint(0,pos1.X(),pos1.Y(),pos1.Z());
// 	segment->SetPoint(1,pos2.X(),pos2.Y(),pos2.Z());
// 	fAllSegments->Add(segment);
// 	fAllHits->SetPoint(point++,pos.X(),pos.Y(),pos.Z());

//     }
//   }

//   if (!fEvent->GetTracks()) Warning("BuildEvent()", "No Tracks is found");
}


//----------------------------------------------------------------------------
void EGraphHits::DrawHits()
{
  //
  // 
  //
//   fAllHits->SetMarkerColor(kRed);
//   fAllHits->SetMarkerStyle(8);
//   fAllHits->SetMarkerSize(msize);
//   fAllHits->Draw();

  // draw segments

  for (Int_t i = 0; i < fAllSegments->GetEntries(); i++) {
    TPolyLine3D *segment = (TPolyLine3D*)fAllSegments->At(i);
    if (segment) segment->Draw();
  }
}


//----------------------------------------------------------------------------
void EGraphHits::ClearEvent() 
{
  //
  // Clear
  //

//   SafeDelete(fAllHits); fAllHits = new TPolyMarker3D;

//   for (Int_t i = 0; i < fList->GetEntries(); i++) {
//     TH1 *h = (TH1*)fList->At(i);
//     h->Reset();
//   }

  fAllSegments->Delete();
}

