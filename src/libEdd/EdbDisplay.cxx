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
#include "TSystem.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGFont.h"
#include "TGFrame.h"
#include "TGResourcePool.h"
#include "TGClient.h"
#include "TRootEmbeddedCanvas.h"
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
const char *EdbVertexG::GetTitle() const
{
    static char title[80];
    sprintf(title, "Vertex ID %d, Prongs %d, Prob %f", eV->ID(), eV->N(), eV->V()->prob());
    return title;
}

//_____________________________________________________________________________
const char *EdbVertexG::GetName() const
{
    static char name[] = "Vertex";
    return name;
}

//_____________________________________________________________________________
char *EdbVertexG::GetObjectInfo(int px, int py) const
{
    static char coordinates[80];
    sprintf(coordinates, "X = %.1f, Y = %.1f, Z = %.1f", eV->VX(), eV->VY(), eV->VZ());
    return coordinates;
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
const char *EdbTrackG::GetTitle() const
{
    static char title[80];
    sprintf(title, "Track ID %d, Nsegments %d, Prob %f", eTr->ID(), eTr->N(), eTr->Prob());
    return title;
}

//_____________________________________________________________________________
const char *EdbTrackG::GetName() const
{
    static char name[] = "Track";
    return name;
}

//_____________________________________________________________________________
char *EdbTrackG::GetObjectInfo(int px, int py) const
{
    static char coordinates[80];
    sprintf(coordinates, "X = %.1f, Y = %.1f, Z = %.1f", eTr->X(), eTr->Y(), eTr->Z());
    return coordinates;
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

//_____________________________________________________________________________
const char *EdbSegG::GetTitle() const
{
    static char title[80];
    sprintf(title, "Segment ID %d, PID %d", eSeg->ID(), eSeg->PID());
    return title;
}

//_____________________________________________________________________________
const char *EdbSegG::GetName() const
{
    static char name[] = "Segment";
    return name;
}

//_____________________________________________________________________________
char *EdbSegG::GetObjectInfo(int px, int py) const
{
    static char coordinates[80];
    sprintf(coordinates, "X = %.1f, Y = %.1f, Z = %.1f", eSeg->X(), eSeg->Y(), eSeg->Z());
    return coordinates;
}

//________________________________________________________________________
void EdbDisplay::Set0()
{
  eVerRec = ((EdbVertexRec *)(gROOT->GetListOfSpecials()->FindObject("EdbVertexRec")));
  eArrSegP = 0;
  eArrTr   = 0;
  eArrV    = 0;
  eArrSegPSave = 0;
  eArrTrSave   = 0;
  eArrVSave    = 0;
  eDrawTracks = 1;
  eDrawVertex = 0;
  eColors  = 0;
  eDZs     = 0;
  eVertex  = 0;
  eWorking = 0;
  ePrevious= 0;
  eSegment = 0;
  eSegPM = 0;
  eWait_Answer = false;
  eIndVert = -1;
  eIndVertSave = -1;
  eRadMax = 1000.;
  eDpat = 2;
  eImpMax = 10000.;
  fNumericEntries[0] = 0;
  fNumericEntries[1] = 0;
  fNumericEntries[2] = 0;
  
  fCanvas->Connect("Closed()", "EdbDisplay", this, "Delete()");
}
//________________________________________________________________________
EdbDisplay *EdbDisplay::EdbDisplayExist(const char *title)
{
  EdbDisplay *ds=0;
  TSeqCollection *l = gROOT->GetListOfSpecials();
  if (!l) return false;
  ds = (EdbDisplay*)(l->FindObject(title));
  return ds;
}
//________________________________________________________________________
EdbDisplay::~EdbDisplay()
{
    DeleteModifiedVTX();

    if (gROOT->GetListOfSpecials()->FindObject(this))
    {
	gROOT->GetListOfSpecials()->Remove(this);
    }
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
    if (eWorking) delete eWorking;
    eWorking = 0;
    if (ePrevious) delete ePrevious;
    ePrevious = 0;
    if (eArrSegPSave && eArrSegP) delete eArrSegP;
    eArrSegP = 0;
    if (eArrVSave && eArrV) delete eArrV;
    eArrV = 0;
    if (eArrTrSave && eArrTr) delete eArrTr;
    eArrTr = 0;
}
//________________________________________________________________________
void EdbDisplay::Delete()
{
    DeleteModifiedVTX();

    if (gROOT->GetListOfSpecials()->FindObject(this))
    {
	gROOT->GetListOfSpecials()->Remove(this);
    }
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
    if (eWorking) delete eWorking;
    eWorking = 0;
    if (ePrevious) delete ePrevious;
    ePrevious = 0;
    if (eArrSegPSave && eArrSegP) delete eArrSegP;
    eArrSegP = 0;
    if (eArrVSave && eArrV) delete eArrV;
    eArrV = 0;
    if (eArrTrSave && eArrTr) delete eArrTr;
    eArrTr = 0;
}
//________________________________________________________________________
void EdbDisplay::SetArrSegP(TObjArray *arr)
{
  eArrSegP=arr;

  float x = 0., y = 0., z = 0;
  float wx0 = fVx0, wy0 = fVy0, wz0 = fVz0;
  float wx1 = fVx1, wy1 = fVy1, wz1 = fVz1;

  EdbSegP *seg=0;
  if( eArrSegP ) {
    int nseg = eArrSegP->GetEntries();
    for(int j=0;j<nseg;j++) {
      seg = (EdbSegP*)(eArrSegP->At(j));
      if (seg)
      {
	x = seg->X();
	y = seg->Y();
	z = seg->Z();
	if (fView == 0)
	{
	    wx0 = x < wx0 ? x : wx0;
	    wy0 = y < wy0 ? y : wy0;
	    wz0 = z < wz0 ? z : wz0;
	    wx1 = x > wx1 ? x : wx1;
	    wy1 = y > wy1 ? y : wy1;
	    wz1 = z > wz1 ? z : wz1;
	}
      }
    }
  }
  if (fView == 0)
  {
    if (wx0 < fVx0) fVx0 = wx0 - 500.;
    if (wy0 < fVy0) fVy0 = wy0 - 500.;
    if (wz0 < fVz0) fVz0 = wz0 - 500.;
    if (wx1 > fVx1) fVx1 = wx1 + 500.;
    if (wy1 > fVy1) fVy1 = wy1 + 500.;
    if (wz1 > fVz1) fVz1 = wz1 + 500.;
  }
}
//________________________________________________________________________
void EdbDisplay::SetArrTr(TObjArray *arr)
{
  eArrTr=arr;

  float x = 0., y = 0., z = 0;
  float wx0 = fVx0, wy0 = fVy0, wz0 = fVz0;
  float wx1 = fVx1, wy1 = fVy1, wz1 = fVz1;

  EdbTrackP *tr=0;
  if( eArrTr ) {
    int ntr = eArrTr->GetEntries();
    for(int j=0;j<ntr;j++) {
      tr = (EdbTrackP*)(eArrTr->At(j));
      if(tr)
      {
	if (fView == 0)
	{
	    if (tr->NF())
	    {
		x = tr->TrackZmin()->X();
		y = tr->TrackZmin()->Y();
		z = tr->TrackZmin()->Z();
	    }
	    else
	    {
		eRadMax = 0;
		x = tr->GetSegmentFirst()->X();
		y = tr->GetSegmentFirst()->Y();
		z = tr->GetSegmentFirst()->Z();
	    }
	    wx0 = x < wx0 ? x : wx0;
	    wy0 = y < wy0 ? y : wy0;
	    wz0 = z < wz0 ? z : wz0;
	    wx1 = x > wx1 ? x : wx1;
	    wy1 = y > wy1 ? y : wy1;
	    wz1 = z > wz1 ? z : wz1;
	    if (tr->NF())
	    {
		x = tr->TrackZmax()->X();
		y = tr->TrackZmax()->Y();
		z = tr->TrackZmax()->Z();
	    }
	    else
	    {
		eRadMax = 0;
		x = tr->GetSegmentLast()->X();
		y = tr->GetSegmentLast()->Y();
		z = tr->GetSegmentLast()->Z();
	    }
	    wx0 = x < wx0 ? x : wx0;
	    wy0 = y < wy0 ? y : wy0;
	    wz0 = z < wz0 ? z : wz0;
	    wx1 = x > wx1 ? x : wx1;
	    wy1 = y > wy1 ? y : wy1;
	    wz1 = z > wz1 ? z : wz1;
	}
      }
    }
  }
  if (fView == 0)
  {
    if (wx0 < fVx0) fVx0 = wx0 - 500.;
    if (wy0 < fVy0) fVy0 = wy0 - 500.;
    if (wz0 < fVz0) fVz0 = wz0 - 500.;
    if (wx1 > fVx1) fVx1 = wx1 + 500.;
    if (wy1 > fVy1) fVy1 = wy1 + 500.;
    if (wz1 > fVz1) fVz1 = wz1 + 500.;
  }
}
//________________________________________________________________________
void EdbDisplay::SetArrV(TObjArray *arrv)
{
  eArrV=arrv;

  float x = 0., y = 0., z = 0;
  float wx0 = fVx0, wy0 = fVy0, wz0 = fVz0;
  float wx1 = fVx1, wy1 = fVy1, wz1 = fVz1;

  EdbVertex *v=0;
  if( eArrV ) {
    int nv = eArrV->GetEntries();
    for(int j=0;j<nv;j++) {
      v = (EdbVertex*)(eArrV->At(j));
      if (v)
      {
	if (fView == 0)
	{
	    if (v->V())
	    {
		x = v->VX();
		y = v->VY();
		z = v->VZ();
	    }
	    else
	    {
		eRadMax = 0;
		x = v->X();
		y = v->Y();
		z = v->Z();
	    }
	    wx0 = x < wx0 ? x : wx0;
	    wy0 = y < wy0 ? y : wy0;
	    wz0 = z < wz0 ? z : wz0;
	    wx1 = x > wx1 ? x : wx1;
	    wy1 = y > wy1 ? y : wy1;
	    wz1 = z > wz1 ? z : wz1;
	}
      }
    }
  }
  if (fView == 0)
  {
    if (wx0 < fVx0) fVx0 = wx0 - 500.;
    if (wy0 < fVy0) fVy0 = wy0 - 500.;
    if (wz0 < fVz0) fVz0 = wz0 - 500.;
    if (wx1 > fVx1) fVx1 = wx1 + 500.;
    if (wy1 > fVy1) fVy1 = wy1 + 500.;
    if (wz1 > fVz1) fVz1 = wz1 + 500.;
  }
}
//________________________________________________________________________
void EdbDisplay::Refresh()
{
  EdbSegP *seg=0;
  if( eArrSegP ) {
    int nseg = eArrSegP->GetEntries();
    for(int j=0;j<nseg;j++) {
      seg = (EdbSegP*)(eArrSegP->At(j));
      if (seg)
      {
        SegLine(seg)->Draw();
      }
    }
  }

  EdbTrackP *tr=0;
  if( eArrTr ) {
    int ntr = eArrTr->GetEntries();
    for(int j=0;j<ntr;j++) {
      tr = (EdbTrackP*)(eArrTr->At(j));
      if(tr)
      {
        TrackDraw(tr);
      }
    }
  }

  EdbVertex *v=0;
  if( eArrV ) {
    int nv = eArrV->GetEntries();
    for(int j=0;j<nv;j++) {
      v = (EdbVertex*)(eArrV->At(j));
      if (v)
      {
        VertexDraw(v);
      }
    }
  }

  if (eSegment)
  {
    eSegPM = new TPolyMarker3D(1);
    eSegPM->SetMarkerStyle(20);
    float dz = TMath::Abs(eSegment->DZ()/2.);
    eSegPM->SetPoint(0, 
		        eSegment->X() + eSegment->TX()*dz,
			eSegment->Y() + eSegment->TY()*dz,
			eSegment->Z() +                dz);
    eSegPM->SetMarkerColor(kGreen);
    eSegPM->SetMarkerSize(1.2);
    eSegPM->SetBit(kCannotPick);
    eSegPM->Draw();
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
//  v->Draw();

  if (vv->V())
  {
    v = new EdbVertexG(this);
    v->SetVertex( vv );

    xv = vv->VX();
    yv = vv->VY();
    zv = vv->VZ();
    v->SetPoint(0, xv, yv, zv);
    if (eWorking == vv)
    {
	v->SetMarkerColor(kGreen);
    }
    else if (eVertex == vv)
    {
	if (!eWorking) v->SetMarkerColor(kGreen);
	else	       v->SetMarkerColor(kWhite);
    }
    else
    {
	v->SetMarkerColor(kWhite);
    }
    v->SetMarkerStyle(29);
    v->SetMarkerSize(1.2);
    v->Draw();
  }


  if(eDrawVertex>0) {
    TPolyLine3D *line=0;
    const EdbSegP *seg=0;
    EdbTrackP *tr=0;
    bool measured_segment = false;
    if (eVerRec) measured_segment = eVerRec->eUseSegPar;
    float dz = 0;
    for(int i=0; i<vv->N(); i++ ) {
      tr = vv->GetTrack(i);
      if (!(tr->NF())) measured_segment = true;
      dz = 0.;
      if( vv->Zpos(i)==0 )
      {
             seg = tr->TrackZmax(measured_segment);
	     dz = TMath::Abs(seg->DZ());
      }
      else
      {
             seg = tr->TrackZmin(measured_segment);
      }
      if(!seg) continue;
      line = new TPolyLine3D(2);
      line->SetPoint(0, xv,yv,zv );
      line->SetPoint(1, seg->X()+seg->TX()*dz, seg->Y()+seg->TY()*dz, seg->Z()+dz);
      line->SetLineColor(kWhite);
      line->SetLineWidth(1);
      line->SetBit(kCannotPick);
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
  //              4 - draw also measured segments
  //              5 - draw fitted line and  measured segments, no markers 
  //              6 - draw measured segments only
  //              7 - draw measured segments only
  //              8 - draw only solid white track line

  if (!tr) return;
  TPolyLine3D *line=0;
  const EdbSegP *seg=0;
  float dz = 0.;

  if(eDrawTracks>0 && eDrawTracks<6) {   // only dotted track line
    line = new TPolyLine3D(tr->N());
    if (tr->NF())    {
      for(int is=0; is<tr->NF(); is++) {
	seg = tr->GetSegmentF(is);
	if(seg) 
	{
	    if (is == 0)
	    {
		dz = TMath::Abs(seg->DZ());
		line->SetPoint(is, seg->X()+seg->TX()*dz,
				   seg->Y()+seg->TY()*dz,
				   seg->Z()+          dz );
	    }
	    else
	    {
		line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
	    }
	}
      }
    }
    else      {
      for(int is=0; is<tr->N(); is++) {
	seg = tr->GetSegment(is);
	if(seg) 
	{
	    if (is == 0)
	    {
		dz = TMath::Abs(seg->DZ());
		line->SetPoint(is, seg->X()+seg->TX()*dz,
				   seg->Y()+seg->TY()*dz,
				   seg->Z()+          dz);
	    }
	    else
	    {
		line->SetPoint(is, seg->X(), seg->Y(), seg->Z() );
	    }
	}
      }
    }
    line->SetLineColor(kWhite);
    line->SetLineWidth(1);
    line->SetBit(kCannotPick);
    if (tr->Flag() != -10)
	line->SetLineStyle(3);
    else
	line->SetLineStyle(4);
    line->Draw();
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

  if(eDrawTracks>1 && eDrawTracks<5) {
    EdbTrackG *pms = new EdbTrackG(1, this);
    pms->SetTrack( tr );
    pms->SetMarkerStyle(24);

    if (tr->NF())      seg = tr->TrackZmin();
    else               seg = tr->GetSegmentFirst();
    if(seg) {
      pms->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
      pms->SetMarkerColor(kWhite);
      pms->SetMarkerSize(1.2);
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
      dz = TMath::Abs(seg->DZ());
      pme->SetPoint(0, seg->X()+seg->TX()*dz,
    		       seg->Y()+seg->TY()*dz,
		       seg->Z()+          dz);
      pme->SetMarkerColor(kRed);
      pme->SetMarkerSize(1.2);
      pme->Draw();
    }
  }

}

///------------------------------------------------------------
EdbSegG *EdbDisplay::SegLine(const EdbSegP *seg)
{
  if (!seg) return 0;
  float dz = TMath::Abs(seg->DZ());
  EdbSegG *line = new EdbSegG(2, this);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  line->SetPoint(1,
                 seg->X() + seg->TX()*dz,
                 seg->Y() + seg->TY()*dz,
                 seg->Z() +           dz);

  int eNpieces=30;
  line->SetLineColor(gStyle->GetColorPalette(int(46.*(1.-1.*seg->PID()/eNpieces))));  
  //line->SetLineColor( 2+seg->PID()%7 );
  line->SetLineWidth(int(seg->W()/10.));
  line->SetSeg(seg);
  return line;
}

//_____________________________________________________________________________
void EdbSegG::AddAsTrack()
{
  char text[512];
  EdbVTA *vta = 0;
  if (eSeg && eD)
  {
    if (eD->eWait_Answer) return;
    if (!(eD->eVertex)) return;
    if (eSeg->Track() >= 0) return;
    EdbVertex *ePreviousSaved = eD->ePrevious;
    if (eD->eWorking == 0)
    {
	eD->eWorking = new EdbVertex();
	int ntr = eD->eVertex->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking), (eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i))))
	    {
		(eD->eVertex)->GetTrack(i)->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}

	if (!((eD->eVerRec)->MakeV(*(eD->eWorking))))
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}
    }
    else
    {
//	if (ePrevious) delete ePrevious;
	eD->ePrevious = eD->eWorking;
	eD->eWorking = new EdbVertex();
	int ntr = eD->ePrevious->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking),(eD->ePrevious)->GetTrack(i), (eD->ePrevious)->Zpos(i))))
	    {
		((eD->ePrevious)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
		eD->ePrevious = ePreviousSaved;
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}

	if (!((eD->eVerRec)->MakeV(*(eD->eWorking))))
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
		eD->ePrevious = ePreviousSaved;
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}
    }
    EdbTrackP *Tr = new EdbTrackP((EdbSegP *)eSeg, (float)0.139);
    Tr->FitTrackKFS();
    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking), Tr, 1)))
    {
	if ( Tr->Z() >= (eD->eWorking)->VZ() ) vta->SetZpos(1);
	else vta->SetZpos(0);
	Tr->AddVTA(vta);
	(eD->eArrTr)->Add(Tr);
	EdbVertex *eW = eD->eWorking;
	eW->SetID(eD->eVertex->ID());
	eW->V()->rmsDistAngle();
	sprintf(text,"New     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	eD->DrawOldBut("Original");
	if (eD->ePrevious)
	{
	    eD->DrawNewVTX(text);
	    eD->DrawNewBut("Modified");
	    eW = eD->ePrevious;
	    eW->V()->rmsDistAngle();
	    sprintf(text,"Pre     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	    eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	    eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Previous");
	}
	else
	{
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Modified");
	    eD->DrawAcc();
	    eD->DrawCan();
	    eD->DrawUnd();
	}
//	eD->DrawOldBut("Original");
	eD->DrawVTXTracks("Modified", eD->eWorking);
	if (eD->eIndVert >= 0) 
	{
//	    eD->eArrV->RemoveAt(eD->eIndVert);
	    eD->eArrV->AddAt(eD->eWorking, eD->eIndVert);
	}	
	(eD->eCreatedTracks).Add(Tr);
	eD->Draw();
	if (ePreviousSaved) delete ePreviousSaved;
	ePreviousSaved = 0;
    }
    else
    {
	printf("Track not added! May be Prob < ProbMin.\n");
	delete Tr;
	delete eD->eWorking;
	if (eD->ePrevious)
	{
	    eD->eWorking = eD->ePrevious;
	    (eD->eWorking)->ResetTracks();
	    eD->ePrevious = ePreviousSaved;
	}
	else
	{
	    eD->eWorking = 0;
	    (eD->eVertex)->ResetTracks();
	}
	return;
    }
  }
}

//_____________________________________________________________________________
void EdbSegG::SetAsWorking()
{
  static EdbDisplay *eDs = 0;
  static EdbSegP    *eSegs = 0;
  eDs = eD;
  eSegs = (EdbSegP *)eSeg;
  if (eDs && eSegs)
  {
    if (eDs->eSegment == eSegs) return;
    if (eDs->eWait_Answer) return;
    if (eDs->eVertex)
    {
	if (eDs->eWorking)
	{
	    eDs->DialogModifiedVTX();
	    return;
	}
	else 
	{
	    eDs->CancelModifiedVTX();
	}
    }
    if (eDs->eSegPM)
    { 
	if (eDs->fPad->GetListOfPrimitives()->FindObject(eDs->eSegPM))
	{
	    eDs->fPad->GetListOfPrimitives()->Remove(eDs->eSegPM);
	}
	delete eDs->eSegPM;
	eDs->eSegPM = 0;
    }
    if (!(eDs->eArrSegP)) 
    {
	eDs->eArrSegP = new TObjArray(20);
	eDs->eArrSegP->Add((TObject *)eSegs);
	eDs->Draw();
    }
    else
    { 

	if (!(eDs->eArrSegP->FindObject(eSegs)))
	{
	    eDs->eArrSegP->Add((TObject *)eSegs);
	    eDs->Draw();
	}
    }
    eDs->eSegment = (EdbSegP *)eSegs;
    eDs->DrawEnv();
    eDs->eSegPM = new TPolyMarker3D(1);
    eDs->eSegPM->SetMarkerStyle(20);
    if(eSegs) {
      float dz = TMath::Abs(eSegs->DZ()/2.);
      eDs->eSegPM->SetPoint(0, 
    		    eSegs->X() + eSegs->TX()*dz,
		    eSegs->Y() + eSegs->TY()*dz,
		    eSegs->Z() +            dz);
      eDs->eSegPM->SetMarkerColor(kGreen);
      eDs->eSegPM->SetMarkerSize(1.2);
      eDs->eSegPM->AppendPad();
      eDs->eSegPM->Draw();
    }
  }
}

//_____________________________________________________________________________
void EdbVertexG::SetAsWorking()
{
  char text[512];
  EdbTrackP *tr = 0;
  static EdbDisplay *eDs = 0;
  static EdbVertex  *eVs = 0;
  eDs = eD;
  eVs = eV;
  if (eDs && eVs)
  {
    if (eDs->eVertex == eVs) return;
    if (eDs->eWait_Answer) return;
    if (eDs->eWorking)
    {
	eDs->DialogModifiedVTX();
	return;
    }
    if (eDs->eVertex)
    {
    	eDs->CancelModifiedVTX();
    }
    if (eDs->eSegPM)
    { 
	eDs->ClearSegmentEnv();
    }
    SetMarkerColor(kGreen);
    if (!(eDs->eArrV)) 
    {
	eDs->eArrV = new TObjArray(20);
	eDs->eArrV->Add((TObject *)eVs);
	if (!(eDs->eArrTr))  eDs->eArrTr = new TObjArray(20);
	for (int i=0; i<eVs->N(); i++)
	{
	    tr = eVs->GetTrack(i);
	    if(!(eDs->eArrTr->FindObject(tr))) eDs->eArrTr->Add(tr);
	}
	eDs->Draw();
    }
    else
    {
	if (!(eDs->eArrV->FindObject(eVs)))
	{
	    eDs->eArrV->Add(eVs);
	    if (!(eDs->eArrTr))  eDs->eArrTr = new TObjArray(20);
	    for (int i=0; i<eVs->N(); i++)
	    {
		tr = eVs->GetTrack(i);
		if(!(eDs->eArrTr->FindObject(tr))) eDs->eArrTr->Add(tr);
	    }
	    eDs->eVertex = 0;
	    eDs->Draw();
	}
    }
    eDs->eSegment = 0;
    eDs->eVertex = eVs;
    eDs->ePrevious = 0;
    eDs->CreateCanvasVTX();
    eVs->V()->rmsDistAngle();
    sprintf(text,"Orig    %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
    eVs->ID(), eVs->N(), eVs->VX(), eVs->VY(), eVs->VZ(), eVs->V()->dist(),
    eVs->V()->chi2()/eVs->V()->ndf(), eVs->V()->prob());
    eDs->DrawOldVTX(text);
    eDs->DrawVTXTracks("Original", eDs->eVertex);
    eDs->DrawEnv();
    eDs->eIndVert = eD->eArrV->IndexOf(eVs);
    (eDs->eCreatedTracks).Clear();
    eDs->Draw();
  }
}

//_____________________________________________________________________________
void EdbTrackG::RemoveTrack()
{
  char text[512];
  if (eTr && eD)
  {
    if (eD->eWait_Answer) return;
    if (!(eD->eVertex)) return;
    EdbVTA *vta = 0;
    EdbVertex *ePreviousSaved = eD->ePrevious;
    int n = 0;
    int ntr = 0;
    if (eD->eWorking == 0)
    {
	ntr = eD->eVertex->N();
	if (ntr < 3) return;
	eD->eWorking = new EdbVertex();
	int i = 0;
	for(i=0; i<ntr; i++)
	{
	    if (eD->eVertex->GetTrack(i) == eTr)
	    {
		eTr->ClearVTA(eD->eVertex->GetVTa(i));
		continue;
	    }
	    if ((vta = (eD->eVerRec)->AddTrack( *(eD->eWorking), (eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i))))
	    {
		((eD->eVertex)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
    }
    else
    {
	ntr = eD->eWorking->N();
	if (ntr < 3) return;
	eD->ePrevious = eD->eWorking;
	eD->eWorking = new EdbVertex();
	int i = 0;
	for(i=0; i<ntr; i++)
	{
	    if (eD->ePrevious->GetTrack(i) == eTr)
	    {
		eTr->ClearVTA(eD->ePrevious->GetVTa(i));
		continue;
	    }
	    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking),(eD->ePrevious)->GetTrack(i), (eD->ePrevious)->Zpos(i))))
	    {
		((eD->ePrevious)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
    }
    if ((n < 2)||(n == ntr))
    {
	delete eD->eWorking;
	if (eD->ePrevious)
	{
	    eD->eWorking = eD->ePrevious;
	    (eD->eWorking)->ResetTracks();
	    eD->ePrevious = ePreviousSaved;
	}
	else
	{
	    eD->eWorking = 0;
	    (eD->eVertex)->ResetTracks();
	}
	return;
    }

    if ((eD->eVerRec)->MakeV(*(eD->eWorking)))
    {
	EdbVertex *eW = eD->eWorking;
	eW->ResetTracks();
	eW->SetID(eD->eVertex->ID());
	eW->V()->rmsDistAngle();
	sprintf(text,"New     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	eD->DrawOldBut("Original");
	if (eD->ePrevious)
	{
	    eD->DrawNewVTX(text);
	    eD->DrawNewBut("Modified");
	    eW = eD->ePrevious;
	    sprintf(text,"Pre     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	    eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	    eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Previous");
	}
	else
	{
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Modified");
	    eD->DrawAcc();
	    eD->DrawCan();
	    eD->DrawUnd();
	}
//	eD->DrawOldBut("Original");
	eD->DrawVTXTracks("Modified", eD->eWorking);
	if (eD->eIndVert >= 0) 
	{
//	    eD->eArrV->RemoveAt(eD->eIndVert);
	    eD->eArrV->AddAt(eD->eWorking, eD->eIndVert);
	}	
	if ((eD->eCreatedTracks).FindObject(eTr))
	{
	    (eD->eCreatedTracks).Remove(eTr);
	    if ( eD->eArrTr)
		if ((eD->eArrTr)->FindObject(eTr)) (eD->eArrTr)->Remove(eTr);
	    if ( eD->eArrTrSave)
		if ((eD->eArrTrSave)->FindObject(eTr)) (eD->eArrTrSave)->Remove(eTr);
	    delete eTr;
	}
	eD->Draw();
	if (ePreviousSaved) delete ePreviousSaved;
	ePreviousSaved = 0;
    }
    else
    {
	delete eD->eWorking;
	if (eD->ePrevious)
	{
	    eD->eWorking = eD->ePrevious;
	    (eD->eWorking)->ResetTracks();
	    eD->ePrevious = ePreviousSaved;
	}
	else
	{
	    eD->eWorking = 0;
	    (eD->eVertex)->ResetTracks();
	}
    }
  }
}


//_____________________________________________________________________________
void EdbDisplay::RemoveTrackFromTable(int ivt)
{
    char text[512];
    EdbTrackP *etr = 0;
    if (eWait_Answer) return;
    if (!(eVertex)) return;
    EdbVTA *vta = 0;
    EdbVertex *ePreviousSaved = ePrevious;
    int n = 0;
    int ntr = 0;
    if (eWorking == 0)
    {
	ntr = eVertex->N();
	if (ntr < 3) return;
	eWorking = new EdbVertex();
	int i = 0;
	etr = eVertex->GetTrack(ivt);
	for(i=0; i<ntr; i++)
	{
	    if (i == ivt)
	    {
		etr->ClearVTA(eVertex->GetVTa(i));
		continue;
	    }
	    if ((vta = (eVerRec)->EdbVertexRec::AddTrack( *(eWorking), (eVertex)->GetTrack(i), (eVertex)->Zpos(i))))
	    {
		((eVertex)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
    }
    else
    {
	ntr = eWorking->N();
	if (ntr < 3) return;
	etr = eWorking->GetTrack(ivt);
	ePrevious = eWorking;
	eWorking = new EdbVertex();
	int i = 0;
	for(i=0; i<ntr; i++)
	{
	    if (i == ivt)
	    {
		etr->ClearVTA(ePrevious->GetVTa(i));
		continue;
	    }
	    if ((vta = (eVerRec)->EdbVertexRec::AddTrack(*(eWorking),(ePrevious)->GetTrack(i), (ePrevious)->Zpos(i))))
	    {
		((ePrevious)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
    }
    if ((n < 2)||(n == ntr))
    {
	delete eWorking;
	if (ePrevious)
	{
	    eWorking = ePrevious;
	    (eWorking)->ResetTracks();
	    ePrevious = ePreviousSaved;
	}
	else
	{
	    eWorking = 0;
	    (eVertex)->ResetTracks();
	}
	return;
    }

    if ((eVerRec)->MakeV(*(eWorking)))
    {
	EdbVertex *eW = eWorking;
	eW->ResetTracks();
	eW->SetID(eVertex->ID());
	eW->V()->rmsDistAngle();
	sprintf(text,"New     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	DrawOldBut("Original");
	if (ePrevious)
	{
	    DrawNewVTX(text);
	    DrawNewBut("Modified");
	    eW = ePrevious;
	    sprintf(text,"Pre     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	    eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	    eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	    DrawPreVTX(text);
	    DrawPreBut("Previous");
	}
	else
	{
	    DrawPreVTX(text);
	    DrawPreBut("Modified");
	    DrawAcc();
	    DrawCan();
	    DrawUnd();
	}
//	DrawOldBut("Original");
	TButton *rm = fRemBut[ivt];
	fRemBut[ivt] = 0;
	DrawVTXTracks("Modified", eWorking);
	if (eIndVert >= 0) 
	{
//	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eWorking, eIndVert);
	}	
	if (eCreatedTracks.FindObject(etr))
	{
	    eCreatedTracks.Remove(etr);
	    if ( eArrTr)
		if (eArrTr->FindObject(etr)) eArrTr->Remove(etr);
	    if ( eArrTrSave)
		if (eArrTrSave->FindObject(etr)) eArrTrSave->Remove(etr);
	    delete etr;
	} 
	fCanvas->cd();
	Draw();
	fPad->Update(); 
	if (ePreviousSaved) delete ePreviousSaved;
	ePreviousSaved = 0;
	fCanvasVTX->cd();
	if (rm) delete rm;
    }
    else
    {
	delete eWorking;
	if (ePrevious)
	{
	    eWorking = ePrevious;
	    (eWorking)->ResetTracks();
	    ePrevious = ePreviousSaved;
	}
	else
	{
	    eWorking = 0;
	    (eVertex)->ResetTracks();
	}
    }
}

//_____________________________________________________________________________
void EdbTrackG::AddTrack()
{
  char text[512];
  int zpos = 1;
  EdbVTA *vta = 0;
  EdbVertex *old = 0;
  if (eTr && eD)
  {
    if (eD->eWait_Answer) return;
    if (!(eD->eVertex)) return;
    if (GetMarkerColor() == kRed) zpos = 0;
    if ((old = eTr->VertexS()) && (zpos == 1)) return;
//    {
//	if (old != eD->eVertex && old != eD->ePrevious && old != eD->eWorking) return;
//    } 
    if ((old = eTr->VertexE()) && (zpos == 0)) return;
//    {
//	if (old != eD->eVertex && old != eD->ePrevious && old != eD->eWorking) return;
//    } 
    EdbVertex *ePreviousSaved = eD->ePrevious;
    if (eD->eWorking == 0)
    {
	eD->eWorking = new EdbVertex();
	int ntr = eD->eVertex->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking), (eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i))))
	    {
		(eD->eVertex)->GetTrack(i)->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}

	if (!((eD->eVerRec)->MakeV(*(eD->eWorking))))
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}
    }
    else
    {
//	if (ePrevious) delete ePrevious;
	eD->ePrevious = eD->eWorking;
	eD->eWorking = new EdbVertex();
	int ntr = eD->ePrevious->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking),(eD->ePrevious)->GetTrack(i), (eD->ePrevious)->Zpos(i))))
	    {
		((eD->ePrevious)->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
		eD->ePrevious = ePreviousSaved;
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}

	if (!((eD->eVerRec)->MakeV(*(eD->eWorking))))
	{
	    delete eD->eWorking;
	    if (eD->ePrevious)
	    {
		eD->eWorking = eD->ePrevious;
		(eD->eWorking)->ResetTracks();
		eD->ePrevious = ePreviousSaved;
	    }
	    else
	    {
		eD->eWorking = 0;
		(eD->eVertex)->ResetTracks();
	    }
	    return;
	}
    }
    if ((vta = (eD->eVerRec)->AddTrack(*(eD->eWorking), eTr, zpos)))
    {
	eTr->AddVTA(vta);
	EdbVertex *eW = eD->eWorking;
	eW->SetID(eD->eVertex->ID());
	eW->V()->rmsDistAngle();
	sprintf(text,"New     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	eD->DrawOldBut("Original");
	if (eD->ePrevious)
	{
	    eD->DrawNewVTX(text);
	    eD->DrawNewBut("Modified");
	    eW = eD->ePrevious;
	    eW->V()->rmsDistAngle();
	    sprintf(text,"Pre     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	    eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	    eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Previous");
	}
	else
	{
	    eD->DrawPreVTX(text);
	    eD->DrawPreBut("Modified");
	    eD->DrawAcc();
	    eD->DrawCan();
	    eD->DrawUnd();
	}
//	eD->DrawOldBut("Original");
	eD->DrawVTXTracks("Modified", eD->eWorking);
	if (eD->eIndVert >= 0) 
	{
//	    eD->eArrV->RemoveAt(eD->eIndVert);
	    eD->eArrV->AddAt(eD->eWorking, eD->eIndVert);
	}	
	eD->Draw();
	if (ePreviousSaved) delete ePreviousSaved;
	ePreviousSaved = 0;
    }
    else
    {
	delete eD->eWorking;
	if (eD->ePrevious)
	{
	    eD->eWorking = eD->ePrevious;
	    (eD->eWorking)->ResetTracks();
	    eD->ePrevious = ePreviousSaved;
	}
	else
	{
	    eD->eWorking = 0;
	    (eD->eVertex)->ResetTracks();
	}
	return;
    }
  }
}

//_____________________________________________________________________________
void EdbDisplay::CancelModifiedVTX()
{
    if (eArrVSave || eArrTrSave || eArrSegPSave) DrawAllObjects();
    if (eWorking)
    {
	delete eWorking;
	eWorking = 0;
    }
    if (ePrevious)
    {
	delete ePrevious;
	ePrevious = 0;
    }
    if (eVertex)
    {
	(eVertex)->ResetTracks();
    }
    TList *li = fTrigPad->GetListOfPrimitives();
    li->Remove(fUndButton);
    li->Remove(fAccButton);
    li->Remove(fCanButton);

    EdbTrackP *tr = 0;

    for (int i = 0; i < eCreatedTracks.GetSize(); i++)
    {
	tr = (EdbTrackP *)eCreatedTracks.At(i);
	if (tr)
	{
	    if (eArrTr) if (eArrTr->FindObject(tr)) eArrTr->Remove(tr);
	    if (eArrTrSave) if (eArrTrSave->FindObject(tr)) eArrTrSave->Remove(tr);
	    delete tr;
	} 
    }
    eCreatedTracks.Clear();

    li->Remove(fEnvButton);
    if (eIndVert >= 0 && eVertex) 
    {
//	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eVertex, eIndVert);
    }	
    fTrigPad->cd();
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
    if (eWait_Answer) CloseDialogModifiedVTX();
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
    eIndVert = -1;
    eVertex = 0;
    Draw();
}

//_____________________________________________________________________________
void EdbDisplay::DeleteModifiedVTX()
{
    if (eWorking)
    {
	delete eWorking;
	eWorking = 0;
    }
    if (ePrevious)
    {
	delete ePrevious;
	ePrevious = 0;
    }
    if (eVertex)
    {
	(eVertex)->ResetTracks();
    }

    EdbTrackP *tr = 0;

    for (int i = 0; i < eCreatedTracks.GetSize(); i++)
    {
	tr = (EdbTrackP *)eCreatedTracks.At(i);
	if (tr)
	{
	    if (eArrTr) if (eArrTr->FindObject(tr)) eArrTr->Remove(tr);
	    if (eArrTrSave) if (eArrTrSave->FindObject(tr)) eArrTrSave->Remove(tr);
	    delete tr;
	} 
    }
    eCreatedTracks.Clear();

    if (eArrVSave || eArrTrSave || eArrSegPSave)
    {
	delete eArrV;
	eArrV = eArrVSave;
	eArrVSave = 0;
	delete eArrTr;
	eArrTr = eArrTrSave;
	eArrTrSave = 0;
	delete eArrSegP;
	eArrSegP = eArrSegPSave;
	eArrSegPSave = 0;
	eIndVert = eIndVertSave;
    }
    if (eIndVert >= 0 && eVertex) 
    {
//	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eVertex, eIndVert);
	    eIndVert = -1;
    }	
}
//_____________________________________________________________________________
void EdbDisplay::UndoModifiedVTX()
{
    char text[512];
    EdbTrackP *tr = eWorking->GetTrack(eWorking->N() - 1);
    if (tr && eCreatedTracks.FindObject(tr))
    {
	    eCreatedTracks.Remove(tr);
	    if (eArrTr) if (eArrTr->FindObject(tr)) eArrTr->Remove(tr);
	    if (eArrTrSave) if (eArrTrSave->FindObject(tr)) eArrTrSave->Remove(tr);
	    delete tr;
    } 
    if (ePrevious)
    {
	delete eWorking;
	eWorking = ePrevious;
	(eWorking)->ResetTracks();
	EdbVertex *eW = eWorking;
	eW->V()->rmsDistAngle();
	sprintf(text,"New     %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	DrawPreVTX(text);
	DrawPreBut("Modified");
	ClearNewVTX();
//	if (fNewBut) (fCanvasVTX->GetListOfPrimitives())->Remove(fNewBut);
	if (fNewBut)
	{
	    delete fNewBut;
	    fNewBut = 0;
	}
	ePrevious = 0;
	DrawVTXTracks("Modified", eWorking);
	if (eIndVert >= 0) 
	{
//	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eWorking, eIndVert);
	}
	Draw();	
    }
    else if (eWorking)
    {
	delete eWorking;
	(eVertex)->ResetTracks();
	EdbVertex *eW = eVertex;
	eW->V()->rmsDistAngle();
	sprintf(text,"Orig    %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
	eW->ID(), eW->N(), eW->VX(), eW->VY(), eW->VZ(), eW->V()->dist(),
	eW->V()->chi2()/eW->V()->ndf(), eW->V()->prob());
	DrawOldVTX(text);
	ClearPreVTX();
	if (fPreBut)
	{
//	    (fCanvasVTX->GetListOfPrimitives())->Remove(fPreBut);
	    delete fPreBut;
	    fPreBut = 0;
	}
	if (fOldBut)
	{
//	    (fCanvasVTX->GetListOfPrimitives())->Remove(fOldBut);
	    delete fOldBut;
	    fOldBut = 0;
	}
	eWorking = 0;
	DrawVTXTracks("Original", eVertex);
	if (eIndVert >= 0) 
	{
//	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eVertex, eIndVert);
	}
	TList *li = fTrigPad->GetListOfPrimitives();
	li->Remove(fUndButton);
	li->Remove(fAccButton);
	li->Remove(fCanButton);
	Draw();	
    }
}
//_____________________________________________________________________________
void EdbDisplay::AcceptModifiedVTX()
{
    EdbVertex *eVe = 0;
    if (eArrVSave || eArrTrSave || eArrSegPSave) DrawAllObjects();
    if (ePrevious && (ePrevious != eVertex) && (ePrevious != eWorking))
    {
	delete ePrevious;
	ePrevious = 0;
    }
    if (eWorking && eVertex)
    {
	if (!eVerRec) eVerRec = ((EdbVertexRec*)(gROOT->GetListOfSpecials()->FindObject("EdbVertexRec")));
        EdbVertex *eW = eWorking;
	int ind = eVertex->ID();
	eW->SetID(ind);
	eW->SetQuality(eW->V()->prob()/
			   (eW->V()->vtx_cov_x()+eW->V()->vtx_cov_y()));
	if (eVerRec)
	{
	    int ntr = eVertex->N();
	    int i = 0;
	    for(i=0; i<ntr; i++)
	    {
		(eVerRec->eVTA).Remove(eVertex->GetVTa(i));
	    }
	}
	int indd = -1;
	indd = eArrV->IndexOf(eVertex);
	if (indd >= 0)
	{
//	    eArrV->RemoveAt(indd);
	    eArrV->AddAt(eW, indd);
	}
/*	if (eArrVSave)
	{
	    indd = eArrVSave->IndexOf(eVertex);
	    if (indd >= 0)
	    {
//		eArrVSave->RemoveAt(indd);
		eArrVSave->AddAt(eW, indd);
	    }
	}*/
	eVe = eVertex;
	EdbTrackP *tr = 0;

	TObjArray *etr = 0;
	if (eVerRec) etr = eVerRec->eEdbTracks;
	int trind = 0;
	if (etr) trind = etr->GetEntries();
	for (int i = 0; i < eCreatedTracks.GetSize(); i++)
	{
	    tr = (EdbTrackP *)(eCreatedTracks.At(i));
	    if (tr)
	    {
		tr->SetID(trind++);
		if (etr) etr->Add(tr);
		eArrTr->Add(tr);
	    } 
	}
	eCreatedTracks.Clear();

	eW->ResetTracks();
	int ntr = eW->N();
	int i = 0, ifl = 0;
	for(i=0; i<ntr; i++)
	{
		if (eW->Zpos(i)) ifl = ifl | 1; 
		else		 ifl = ifl | 2; 
		if ((tr = eW->GetTrack(i)) && eArrTr)
		{
		    indd = eArrTr->IndexOf(tr);
		    if (indd < 0)
		    {
			eArrTr->Add(tr);
		    }
		}
	}
	ifl = 4 - ifl;
	if (ifl == 3) ifl = 0;
	if (eW->Nv()) ifl += 3;
	eW->SetFlag(ifl);
	if (eVerRec)
	{
//	    eVerRec->eVTX->RemoveAt(ind);
	    eVerRec->eVTX->AddAt(eW, ind);
	    for(i=0; i<ntr; i++)
	    {
		eVerRec->AddVTA(eW->GetVTa(i));
	    }
	}
	eWorking = 0;
    }
    eVertex = 0;
    TList *li = fTrigPad->GetListOfPrimitives();
    li->Remove(fUndButton);
    li->Remove(fAccButton);
    li->Remove(fCanButton);
    li->Remove(fEnvButton);
    fTrigPad->cd();
    fTrigPad->Update();
    fTrigPad->Draw();

    fPad->cd();
    if (eWait_Answer) CloseDialogModifiedVTX();
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
    Draw();
    if (eVe) delete eVe;
    eIndVert = -1;
}
//_____________________________________________________________________________
void EdbDisplay::DrawVertexEnvironment()
{
    if (eRadMax == 0)
    {
	printf("No neighborhood in EdbPVGen object!\n");
	fflush(stdout);
	return;
    }
    if (!eVerRec) eVerRec = ((EdbVertexRec *)(gROOT->GetListOfSpecials()->FindObject("EdbVertexRec")));
    if (!eVerRec) return;
    if (!eVertex && !eSegment) return;
    
    fTrigPad->cd();
    fTrigPad->GetListOfPrimitives()->Remove(fEnvButton);
    fTrigPad->GetListOfPrimitives()->Add(fAllButton);
    fAllButton->SetPad(0.05,0.47,0.85,0.56);
    fAllButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fPad->cd();

    EdbVertex *eW = eVertex;
    EdbTrackP *tr = 0;
    if (eWorking) eW = eWorking;
    float Rmax = eRadMax, ImpMax = eImpMax;
    int Dpat = (int)eDpat;

    eArrTrSave  = eArrTr; 
    eArrSegPSave  = eArrSegP; 
    eArrVSave  = eArrV;
    eIndVertSave = eIndVert; 
    eArrV = new TObjArray(20);
    eArrTr = new TObjArray(20);
    eArrSegP = new TObjArray(20);

    if (eW)
    {
	eVerRec->VertexNeighbor(eW, Rmax, Dpat, ImpMax);
	eArrV->Add(eW);
	int ntr = eW->N();
	for(int i=0; i<ntr; i++)
	{
	    eArrTr->Add((tr = eW->GetTrack(i)));
	}
	int nntr = eW->Nn();
	EdbVTA *vta = 0;
	for(int i=0; i<nntr; i++)
	{
	if ((vta = eW->GetVTn(i)))
	{
	    if (vta->Flag() == 0) //track
	    {
		eArrTr->Add((tr = vta->GetTrack()));
	    }
	    else if (vta->Flag() == 1) //segment
	    {
		eArrSegP->Add((EdbSegP *)(vta->GetTrack()));
	    }
	    else if (vta->Flag() == 3) //vertex
	    {
		eArrV->Add((EdbVertex *)(vta->GetTrack()));
	    }
	}
	}
	eIndVert = eArrV->IndexOf(eW);
    }
    else if (eSegment)
    {
	eIndVert = -1;
	eVerRec->SegmentNeighbor(eSegment, Rmax, Dpat, eArrSegP, eArrTr, eArrV);
	if (eArrV->GetEntries()) eDrawVertex = 1;
    }
    Draw();
}
//_____________________________________________________________________________
void EdbDisplay::DrawAllObjects()
{
    fTrigPad->cd();
    fTrigPad->GetListOfPrimitives()->Remove(fAllButton);
    fTrigPad->GetListOfPrimitives()->Add(fEnvButton);
    fEnvButton->SetPad(0.05,0.47,0.85,0.56);
    fEnvButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fPad->cd();

    if (eArrVSave || eArrTrSave || eArrSegPSave)
    {
	delete eArrV;
	eArrV = eArrVSave;
	eArrVSave = 0;
	eIndVert = eIndVertSave;
	delete eArrTr;
	eArrTr = eArrTrSave;
	eArrTrSave = 0;
	delete eArrSegP;
	eArrSegP = eArrSegPSave;
	eArrSegPSave = 0;
    }
    if (eWorking && eIndVert >= 0) 
    {
//	eArrV->RemoveAt(eIndVert);
	eArrV->AddAt(eWorking, eIndVert);
    }
    Draw();    
}
//_____________________________________________________________________________
void EdbDisplay::DialogNeighborParameters()
{
    if (eRadMax == 0)
    {
	printf("No neighborhood in EdbPVGen object!\n");
	fflush(stdout);
	return;
    }

    eWait_Answer = true;

    fMain = new TGMainFrame(gClient->GetRoot(), 260, 340);
    TGMainFrame *fTra = fMain;

//    fTra->Connect("CloseWindow()", "EdbDisplay", this, "CloseDialogModifiedParams()");

   // use hierarchical cleaning
//    fTra->SetCleanup(kDeepCleanup);

    TGHorizontalFrame *fF[3] = {0,0,0};
    TGLabel *fLabel[3] = {0,0,0};
    Double_t parinit[3] = {eRadMax, eDpat,  eImpMax};
    Double_t parmax[3] =  { 50000.,   25.,  100000.};
    Double_t parmin[3] =  {    10.,    0.,      10.};
    if (eVerRec->ePVR) parmax[1] = (eVerRec->ePVR->Npatterns()-1)/2.;
    char *parlabel[3] = {"Maximal radius","+/- patterns", "Maximal impact"};

    TGGC myGC = *(gClient->GetResourcePool()->GetFrameGC());
    TGFont *myfont = gClient->GetFont("-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
    if (myfont) myGC.SetFont(myfont->GetFontHandle());

    TGVerticalFrame *fF1 = new TGVerticalFrame(fTra, 240, 250);
    TGLayoutHints *fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
    fTra->AddFrame(fF1, fL1);
    TGLayoutHints *fL2 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);
    for (int i = 0; i < 3; i++) {
      fF[i] = new TGHorizontalFrame(fF1, 220, 35);
      fF1->AddFrame(fF[i], fL2);
      fNumericEntries[i] = new TGNumberEntry(fF[i], parinit[i], 12, i+1000,
                                 TGNumberFormat::kNESInteger,
				 TGNumberFormat::kNEANonNegative,
				 TGNumberFormat::kNELLimitMinMax,
				 parmin[i], parmax[i]);
      fF[i]->AddFrame(fNumericEntries[i], fL2);
      fLabel[i] = new TGLabel(fF[i], parlabel[i], myGC(), myfont->GetFontStruct());
      fF[i]->AddFrame(fLabel[i], fL2);
    }

    TGHorizontalFrame *fFrame = new TGHorizontalFrame(fTra, 220, 30);
    char cmda[256];
    sprintf(cmda,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->AcceptModifiedParams()",fTitle);
    TGTextButton *abut = new TGTextButton(fFrame, "&Accept", cmda);
    fFrame->AddFrame(abut,
		     new TGLayoutHints(kLHintsCenterY | kLHintsLeft,
		     5,5,3,4));

    char cmdc[256];
    sprintf(cmdc,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->CancelDialogModifiedParams()",fTitle);
    TGTextButton *cbut = new TGTextButton(fFrame, "&Cancel", cmdc);
    fFrame->AddFrame(cbut,
		     new TGLayoutHints(kLHintsCenterY | kLHintsRight,
    		     5, 5, 4, 4));
//    fFrame->Resize(220, 30);

    fTra->AddFrame(fFrame,
		   new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,
		   5, 5, 4, 4));

    fTra->MapSubwindows();
    fTra->Resize(260, fTra->GetDefaultHeight());
    fTra->SetWindowName("Neighborhood Parameters");
    fTra->MapWindow();
    gClient->WaitFor(fTra);
}
//_____________________________________________________________________________
void EdbDisplay::AcceptModifiedParams()
{
    eWait_Answer = false;
    if(fNumericEntries[0]) eRadMax = fNumericEntries[0]->GetIntNumber();
    if(fNumericEntries[1]) eDpat   = fNumericEntries[1]->GetIntNumber();
    if(fNumericEntries[2]) eImpMax = fNumericEntries[2]->GetIntNumber();
    if(fNumericEntries[0]) delete fNumericEntries[0];  
    if(fNumericEntries[1]) delete fNumericEntries[1];  
    if(fNumericEntries[2]) delete fNumericEntries[2];  
    fMain->SendCloseMessage();
    fMain = 0;
}
//_____________________________________________________________________________
void EdbDisplay::CloseDialogModifiedParams()
{
    eWait_Answer = false;
    fMain = 0;
}
//_____________________________________________________________________________
void EdbDisplay::CancelDialogModifiedParams()
{
    eWait_Answer = false;
    if(fNumericEntries[0]) delete fNumericEntries[0];  
    if(fNumericEntries[1]) delete fNumericEntries[1];  
    if(fNumericEntries[2]) delete fNumericEntries[2];  
    fMain->SendCloseMessage();
    fMain = 0;
}
//_____________________________________________________________________________
void EdbDisplay::DialogModifiedVTX()
{
    eWait_Answer = true;
    fMain = new TGMainFrame(gClient->GetRoot(), 250, 40);
    TGMainFrame *fTra = fMain;
    TGHorizontalFrame *fFrame = new TGHorizontalFrame(fTra, 220, 30);
    char cmda[256];
    sprintf(cmda,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->AcceptModifiedVTX()",fTitle);
    TGTextButton *abut = new TGTextButton(fFrame, "&Accept", cmda);
    fFrame->AddFrame(abut,
		     new TGLayoutHints(kLHintsCenterY | kLHintsLeft,
		     5,5,3,4));

    char cmdc[256];
    sprintf(cmdc,
    "((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->CancelModifiedVTX()",fTitle);
    TGTextButton *cbut = new TGTextButton(fFrame, "&Cancel", cmdc);
    fFrame->AddFrame(cbut,
		     new TGLayoutHints(kLHintsCenterY | kLHintsRight,
    		     5, 5, 4, 4));
    fFrame->Resize(210, 30);

    fTra->AddFrame(fFrame,
		   new TGLayoutHints(kLHintsCenterY | kLHintsCenterX,
		   5, 5, 4, 4));

    fTra->MapSubwindows();
    fTra->Resize(220, fTra->GetDefaultHeight());
    fTra->SetWindowName("Modified Vertex Exist!");
    fTra->MapWindow();
}
//_____________________________________________________________________________
void EdbDisplay::CloseDialogModifiedVTX()
{
    eWait_Answer = false;
    fMain->SendCloseMessage();
    fMain = 0;
}
//=============================================================================
void EdbDisplay::DrawVTXTracks(char *type, EdbVertex *v)
{
  EdbVertex *vv = v;
  if (!vv)
  {
    if (!strcmp(type, "Original") && eVertex)
    {
	vv = eVertex;
    }
    else if (!strcmp(type, "Previous") && ePrevious)
    {
	vv = ePrevious;
    }
    else if (!strcmp(type, "Modified") && eWorking)
    {
	vv = eWorking;
    }
  }
  if (!vv) return;
  bool drawrem = true;
  if (vv == eVertex && eWorking) drawrem = false;
  if (vv == ePrevious) drawrem = false;

  fCanvasVTX->cd();

  if (fVTXTracks)
  {
    fVTXTracks->Clear();
  }
  else
  {
    fVTXTracks = new TPaveText(0.05, 0.05, 0.95, 0.72);
    fVTXTracks->ResetBit(kCanDelete);
  }
  char line[128];
  EdbTrackP *tr = 0;  
  TText *t = 0;
  int ntr = vv->N();
  sprintf(line, "           Tracks parameters for %s vertex", type);
  t = fVTXTracks->AddText(line);
  t->SetTextColor(4);
  t->SetTextSize(0.03);
  t->SetTextAlign(12);
  t->SetTextFont(102);
  strcpy(line, "    #   ID   Nseg   Mass       P       Chi2/ndf    Prob     Chi2Contrib     Impact");
  t = fVTXTracks->AddText(line);
  t->SetTextColor(4);
  t->SetTextSize(0.03);
  t->SetTextAlign(12);
  t->SetTextFont(102);
  for (int i=0; i<ntr; i++)
  {
    tr = vv->GetTrack(i);
    sprintf(line,"%4d  %4d  %4d   %7.4f  %7.2f    %5.2f     %7.4f       %6.3f    %7.2f",
		   i, tr->ID(), tr->N(), tr->M(), tr->P(),
		      tr->Chi2()/tr->N(), tr->Prob(), vv->V()->track_chi2(i), vv->Impact(i));
    t = fVTXTracks->AddText(line);
    t->SetTextColor(1);
    t->SetTextSize(0.03);
    t->SetTextAlign(12);
    t->SetTextFont(102);
  }
  t = fVTXTracks->AddText("");
  t->SetTextColor(1);
  t->SetTextSize(0.03);
  t->SetTextAlign(12);
  t->SetTextFont(102);
  fVTXTracks->Draw();

  float dy = 0.67/(2+ntr+1);
  for (int i=0; i<50; i++)
  {
    if (!drawrem)
    {
	if (fRemBut[i])
	{
	    fRemBut[i]->Delete();
	    fRemBut[i] = 0;
	}
	continue;
    }
    if ( i < ntr )
    {
      if (!fRemBut[i])
      {
	char but[256], tit[32]="Rem";
	sprintf(but,
	"((EdbDisplay*)(gROOT->GetListOfSpecials()->FindObject(\"%s\")))->RemoveTrackFromTable(%d)",fTitle,i);
	fRemBut[i] = new TButton(tit, but, 0.88, 0.635-(i+2)*dy, 0.94, 0.675-(i+2)*dy);
	fRemBut[i]->SetToolTipText("Remove corresponding track");
	fRemBut[i]->ResetBit(kCanDelete);
	fRemBut[i]->SetFillColor(38);
	sprintf(tit,"RemTr%d",i);
	fRemBut[i]->SetName(tit);
	fRemBut[i]->Draw();
      }
      else
      {
        fRemBut[i]->SetPad(0.88,0.635-(i+2)*dy,0.94,0.675-(i+2)*dy);
	fRemBut[i]->Draw();
      }
    }
    else if (fRemBut[i])
    {
	fRemBut[i]->Delete();
	fRemBut[i] = 0;
    }
  }

  gPad->Modified(kTRUE);
  gPad->Update();
}
//=============================================================================
void EdbDisplay::ClearSegmentEnv()
{
	if (fPad->GetListOfPrimitives()->FindObject(eSegPM))
	{
	    fPad->GetListOfPrimitives()->Remove(eSegPM);
	}
	delete eSegPM;
	eSegPM = 0;
	TList *li = fTrigPad->GetListOfPrimitives();
	if (eArrVSave || eArrSegPSave || eArrTrSave)
	{
	    li->Remove(fAllButton);
	    delete eArrV;
	    eArrV = eArrVSave;
	    eArrVSave = 0;
	    if (eArrTrSave)
	    {
		delete eArrTr;
		eArrTr = eArrTrSave;
		eArrTrSave = 0;
	    }
	    if (eArrSegPSave)
	    {
		delete eArrSegP;
		eArrSegP = eArrSegPSave;
		eArrSegPSave = 0;
	    }
	    Draw();    
	}
	else li->Remove(fEnvButton);
}
