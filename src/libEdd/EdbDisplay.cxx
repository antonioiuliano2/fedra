//-- Author of drawing part : Igor Kreslo     27.11.2003
//   Based on AliDisplay class (AliRoot framework - ALICE CERN)
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbDisplay.h"


ClassImp(EdbDisplay);
ClassImp(EdbSegG);
ClassImp(EdbTrackG);

//_____________________________________________________________________________
void EdbTrackG::DumpTrack()
{
  if (eTr) eTr->Print();
}

//_____________________________________________________________________________
void EdbTrackG::InspectTrack()
{
  if (eTr) eTr->Inspect();
}


//_____________________________________________________________________________
void EdbSegG::DumpSegment()
{
  if (eSeg) eSeg->Print();
}

//_____________________________________________________________________________
void EdbSegG::InspectSegment()
{
  if (eSeg) eSeg->Inspect();
}

//________________________________________________________________________
void EdbDisplay::Set0()
{
  ePVR     = 0;
  eArrSegP = 0;
  eArrTr   = 0;
  eDrawTracks=1;
  eColors  = 0;
  eDZs     = 0;
}

//________________________________________________________________________
void EdbDisplay::Refresh()
{
  EdbSegP *seg=0;
  if( eArrSegP ) {
    int nseg = eArrSegP->GetEntries();
    for(int j=0;j<nseg;j++) {
      seg = (EdbSegP*)(eArrSegP->At(j));
      SegLine(seg)->Draw();
    }
  }

  EdbTrackP *tr=0;
  if( eArrTr ) {
    int ntr = eArrTr->GetEntries();
    for(int j=0;j<ntr;j++) {
      tr = (EdbTrackP*)(eArrTr->At(j));
      TrackDraw(tr);
    }
  }

  /*
  for(int j=0;j<PVR()->Npatterns();j++) {
    PatternDraw(*(PVR()->GetPattern(j)));
  }


  TObjArray *trs = PVR()->eTracks;
  EdbTrackP *tr=0; 
  if( trs ) {
    for(int i=0; i<trs->GetEntriesFast(); i++) {
      tr = (EdbTrackP*)(trs->At(i));
      TrackDraw(tr);
    }
  }
  */
}

//________________________________________________________________________
void EdbDisplay::TrackDraw(EdbTrackP *tr)
{
  TPolyLine3D *line=0;
  const EdbSegP *seg=0;

  if(eDrawTracks>0 && eDrawTracks<6) {
    line = new TPolyLine3D(tr->N());
    for(int is=0; is<tr->NF(); is++) {
      seg = tr->GetSegmentF(is);
      if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
    }
    line->SetLineColor(kWhite);
    line->SetLineWidth(1);
    line->SetLineStyle(3);
    line->Draw();
  }

  if(eDrawTracks>1 && eDrawTracks<5) {
    EdbTrackG *pms = new EdbTrackG(1);
    pms->SetTrack( tr );
    pms->SetMarkerStyle(24);
    seg = tr->TrackZmin();
    pms->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
    pms->SetMarkerColor(kWhite);
    pms->Draw();
  }

  if(eDrawTracks>2 && eDrawTracks<5) {
    EdbTrackG *pme = new EdbTrackG(1);
    pme->SetTrack( tr );
    pme->SetMarkerStyle(24);
    seg = tr->TrackZmax();
    pme->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
    pme->SetMarkerColor(kRed);
    pme->Draw();
  }

  if(eDrawTracks>3 && eDrawTracks<8) {
    for(int is=0; is<tr->N(); is++) {
      seg = tr->GetSegment(is);
      SegLine(seg)->Draw();
    }
  }

  if(eDrawTracks>7) {
    line = new TPolyLine3D(tr->N());
    for(int is=0; is<tr->NF(); is++) {
      seg = tr->GetSegmentF(is);
      if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
    }
    line->SetLineColor(kWhite);
    line->SetLineWidth(1);
    line->SetLineStyle(1);
    line->Draw();
  }
}

///------------------------------------------------------------
EdbSegG *EdbDisplay::SegLine(const EdbSegP *seg)
{
  EdbSegG *line = new EdbSegG(2);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  line->SetPoint(1,
                 seg->X() + seg->TX()*seg->DZ(),
                 seg->Y() + seg->TY()*seg->DZ(),
                 seg->Z()+            seg->DZ() );

  line->SetLineColor( 2+seg->PID()%7 );
  line->SetLineWidth(int(seg->W()/10.));
  line->SetSeg(seg);
  return line;
}

/*
//________________________________________________________________________
void EdbDisplay::PatternDraw(EdbPattern &pat)
{
// Clears();
  EdbSegP* seg;
  Float_t x[2],y[2],z[2],DZ,x0,y0,z0;
  Float_t tx,ty;

  for(int i=0; i<pat.GetN();i++){
    seg=pat.GetSegment(i);
    /////if(MaxChi && seg->Flag()==0) continue;
    tx=seg->TX();
    ty=seg->TY();
    if(tx>cutTX1) continue;
    if(tx<cutTX0) continue;
    if(ty>cutTY1) continue;
    if(ty<cutTY0) continue;
    //  if((pat->ID()==2)&&!eTr_Co) continue;
      // if((pat->ID()==1)&&eTr_Co) continue;
    x0=seg->X();
    y0=seg->Y();
    z0=seg->Z();
    //  if((*DZs)[j]<=0) DZ=(seg->W())/2.;
    //   DZ=(seg->Volume())/2.;
    DZ=(seg->DZ())/2.;
    x[1]=x0+tx*DZ;
    y[1]=y0+ty*DZ;
    z[1]=z0+DZ;
    x[0]=x0-tx*DZ;
    y[0]=y0-ty*DZ;
    z[0]=z0-DZ;
    //  fPad->cd();
    pl= new TPolyLine3D(2,x,y,z);

    pl->SetLineColor( 2 + seg->PID() );
    pl->SetLineWidth(int(seg->W()/10.));
    if(seg->Flag()==-1) pl->SetLineColor(kWhite);
    pl->Draw();
  }
}
*/
