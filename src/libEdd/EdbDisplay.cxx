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
void EdbVertexG::SetAsWorking()
{
  if (eD && eV)
  {
    if (eD->eWorking)
    {
	delete eD->eWorking;
	eD->eWorking = 0;
    }
    if (eD->eVertex)
    {
	(eD->eVertex)->ResetTracks();
    }
    eD->eVertex = eV;
    double prob = eV->V()->prob();
    eD->DrawProb(prob);
    eD->ClearNewProb();
  }
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
void EdbTrackG::RemoveTrack()
{
  if (eTr && eD)
  {
    if (!(eD->eVertex)) return;
    int ntr = eD->eVertex->N();
    if (ntr < 3) return;
    EdbVTA *vta = 0;
    int n = 0;
    if (eD->eWorking == 0)
    {
	eD->eWorking = new EdbVertex();
	int i = 0;
	for(i=0; i<ntr; i++)
	{
	    if (eD->eVertex->GetTrack(i) == eTr) continue;
	    if ((vta = (eD->eWorking)->AddTrack((eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i), 0.0)))
	    {
		(eD->eVertex)->GetTrack(i)->AddVTA(vta);
		(eD->eWorking)->AddVTA(vta);
		n++;
	    }
	}
    }
    if (n < 2)
    {
	delete eD->eWorking;
	eD->eWorking = 0;
	return;
    }
    bool usemom = (eD->eVertex)->V()->back()->kalman.use_momentum();
    if ((eD->eWorking)->MakeV(usemom))
    {
	double prob = (eD->eWorking)->V()->prob();
	eD->DrawNewProb(prob);
	//print old and new prob
	//accept result?
    }
    else
    {
	delete eD->eWorking;
	eD->eWorking = 0;
    }
  }
}

//_____________________________________________________________________________
void EdbTrackG::AddTrack()
{
  int zpos = 1;
  float ProbMinV = 0.00;
  EdbVTA *vta = 0;
  // ask for zpos and ProbMinV
  if (eTr && eD)
  {
    if (!(eD->eVertex)) return;
    if (GetMarkerColor() == kRed) zpos = 0;
    if (eTr->VertexS() && (zpos == 1)) return; 
    if (eTr->VertexE() && (zpos == 0)) return; 
    if (eD->eWorking == 0)
    {
	eD->eWorking = new EdbVertex();
	int ntr = eD->eVertex->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = (eD->eWorking)->AddTrack((eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i), 0.0)))
	    {
		(eD->eVertex)->GetTrack(i)->AddVTA(vta);
		(eD->eWorking)->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	    delete eD->eWorking;
	    eD->eWorking = 0;
	    return;
	}
	bool usemom = (eD->eVertex)->V()->back()->kalman.use_momentum();
	if (!((eD->eWorking)->MakeV(usemom)))
	{
	    delete eD->eWorking;
	    eD->eWorking = 0;
	    return;
	}
    }
    if ((vta = (eD->eWorking)->AddTrack(eTr, zpos, ProbMinV)))
    {
	eTr->AddVTA(vta);
	eD->eWorking->AddVTA(vta);
	double prob = (eD->eWorking)->V()->prob();
	eD->DrawNewProb(prob);
	//print old and new prob
	//accept result?
    }
  }
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

//=============================================================================
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
  eVertex  = 0;
  eWorking = 0;
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

  EdbVertex *v=0;
  if( eArrV ) {
    int nv = eArrV->GetEntries();
    for(int j=0;j<nv;j++) {
      v = (EdbVertex*)(eArrV->At(j));
      if (v) VertexDraw(v);
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
}

//________________________________________________________________________
void EdbDisplay::VertexDraw(EdbVertex *vv)
{
  float xv,yv,zv;
  if (!vv) return;
  if (vv->Flag() == -10) return;
  EdbVertexG *v = new EdbVertexG();
  v->SetVertex( vv );

  xv = vv->X();
  yv = vv->Y();
  zv = vv->Z();
  v->SetPoint(0, 
	       xv, 
	       yv, 
	       zv );
  v->SetMarkerStyle(20);
  v->SetMarkerColor(kWhite);
  v->Draw();

  if (vv->V())
  {
    v = new EdbVertexG(this);
    v->SetVertex( vv );

    xv = vv->VX();
    yv=  vv->VY();
    zv=  vv->VZ();
    v->SetPoint(0, xv,yv,zv);
    v->SetMarkerStyle(29);
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
        if( vv->Zpos(i)==0 )       seg = vv->GetTrack(i)->GetSegmentLast();
        else                       seg = vv->GetTrack(i)->GetSegmentFirst();
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

  if (!tr) return;
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
    if (tr->Flag() != -10)
	line->SetLineStyle(3);
    else
	line->SetLineStyle(4);
    line->Draw();
  }

  if(eDrawTracks>1 && eDrawTracks<5) {
    EdbTrackG *pms = new EdbTrackG(1, this);
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
    EdbTrackG *pme = new EdbTrackG(1, this);
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
  EdbSegG *line = new EdbSegG(2, this);
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

