//-- Author of drawing part : Igor Kreslo     27.11.2003
//   Based on AliDisplay class (AliRoot framework - ALICE CERN)
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TStyle.h"
#include "EdbDisplay.h"

ClassImp(EdbDisplay);
ClassImp(EdbSegG);
ClassImp(EdbTrackG);
ClassImp(EdbVertexG);

//_____________________________________________________________________________
void EdbVertexG::DumpVertex()
{
  if (eV) eV->Print();
}

//_____________________________________________________________________________
void EdbVertexG::InspectVertex()
{
  if (eV) eV->Inspect();
}

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
  eArrV    = 0;
  eDrawTracks=1;
  eDrawVertex=0;
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
      if (seg) SegLine(seg)->Draw();
    }
  }

  EdbTrackP *tr=0;
  if( eArrTr ) {
    int ntr = eArrTr->GetEntries();
    for(int j=0;j<ntr;j++) {
      tr = (EdbTrackP*)(eArrTr->At(j));
      if(tr) TrackDraw(tr);
    }
  }

  EdbVertex *v=0;
  if( eArrV ) {
    int nv = eArrV->GetEntries();
    for(int j=0;j<nv;j++) {
      v = (EdbVertex*)(eArrV->At(j));
      if (v) VertexDraw(v);
    }
  }

}

//________________________________________________________________________
void EdbDisplay::VertexDraw(EdbVertex *vv)
{
  float xv,yv,zv;
  EdbVertexG *v = new EdbVertexG();
  v->SetVertex( vv );

  xv = vv->X();
  yv = vv->Y();
  zv = vv->Z();
  v->SetPoint(0, 
	       xv, 
	       yv, 
	       zv );
  v->SetMarkerStyle(21);
  v->SetMarkerColor(kBlue);
  v->Draw();


  if (vv->V())
  {
    v = new EdbVertexG();
    v->SetVertex( vv );

    xv = vv->V()->vx() + xv;
    yv=  vv->V()->vy() + yv;
    zv=  vv->V()->vz() + zv;
    v->SetPoint(0, xv,yv,zv);
    v->SetMarkerStyle(21);
    v->SetMarkerColor(kWhite);
    v->Draw();
  }


  if(eDrawVertex>0) {
    TPolyLine3D *line=0;
    const EdbSegP *seg=0;
    EdbTrackP *tr=0;
    for(int i=0; i<vv->N(); i++ ) {
      tr = vv->GetTrack(i);
      if (tr->NF())
      {
        if( vv->Zpos(i)==0 )       seg = vv->GetTrack(i)->TrackZmax();
        else                       seg = vv->GetTrack(i)->TrackZmin();
      }
      else
      {
        if( vv->Zpos(i)==0 )       seg = vv->GetTrack(i)->GetSegmentFirst();
        else                       seg = vv->GetTrack(i)->GetSegmentLast();
      }
      if(!seg) continue;
      line = new TPolyLine3D(2);
      line->SetPoint(0, xv,yv,zv );
      line->SetPoint(1, seg->X(), seg->Y(), seg->Z() );
      line->SetLineColor(kWhite);
      line->SetLineWidth(1);
      line->Draw();
    }
  }

}

//________________________________________________________________________
void EdbDisplay::TrackDraw(EdbTrackP *tr)
{
  // eDrawTracks: 1 - draw fitted track dotted line only
  //              2 - draw also white marker at zmin
  //              3 - draw also red marker at zmax
  //              4 - draw also raw segments
  //
  //              8 - draw only solid white track line

  TPolyLine3D *line=0;
  const EdbSegP *seg=0;

  if(eDrawTracks>0 && eDrawTracks<6) {   // only dotted track line
    line = new TPolyLine3D(tr->N());
    if (tr->NF())    {
      for(int is=0; is<tr->NF(); is++) {
	seg = tr->GetSegmentF(is);
	if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
      }
    }
    else      {
      for(int is=0; is<tr->N(); is++) {
	seg = tr->GetSegment(is);
	if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
      }
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

    if (tr->NF())      seg = tr->TrackZmin();
    else               seg = tr->GetSegmentFirst();
    if(seg) {
      pms->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
      pms->SetMarkerColor(kWhite);
      pms->Draw();
    }
  }

  if(eDrawTracks>2 && eDrawTracks<5) {
    EdbTrackG *pme = new EdbTrackG(1);
    pme->SetTrack( tr );
    pme->SetMarkerStyle(24);

    if (tr->NF())      seg = tr->TrackZmax();
    else               seg = tr->GetSegmentLast();
    if(seg) {
      pme->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
      pme->SetMarkerColor(kRed);
      pme->Draw();
    }
  }

  if(eDrawTracks>3 && eDrawTracks<8 && tr->N()>0 ) {
    for(int is=0; is<tr->N(); is++) {
      seg = tr->GetSegment(is);
      if (seg) SegLine(seg)->Draw();
    }
  }

  if(eDrawTracks>7) {
    line = new TPolyLine3D(tr->N());
    if (tr->NF()){
      for(int is=0; is<tr->NF(); is++) {
	seg = tr->GetSegmentF(is);
	if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
      }
    }    else    {
      for(int is=0; is<tr->N(); is++) {
	seg = tr->GetSegment(is);
	if(seg) line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
      }
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
  if (!seg) return 0;
  EdbSegG *line = new EdbSegG(2);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  line->SetPoint(1,
                 seg->X() + seg->TX()*seg->DZ(),
                 seg->Y() + seg->TY()*seg->DZ(),
                 seg->Z() +           seg->DZ() );

  int eNpieces=30;
  line->SetLineColor(gStyle->GetColorPalette(int(46.*(1.-1.*seg->PID()/eNpieces))));  
  //line->SetLineColor( 2+seg->PID()%7 );
  line->SetLineWidth(int(seg->W()/10.));
  line->SetSeg(seg);
  return line;
}

