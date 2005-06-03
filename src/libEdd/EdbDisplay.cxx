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
#include "TGClient.h"
#include "TSystem.h"
#include "TGButton.h"
#include "TGFrame.h"
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
  eWait_Answer = false;
  eIndVert = -1;
  eIndVertSave = -1;
  fCanvas->Connect("Closed()", "EdbDisplay", this, "Delete()");
}
//________________________________________________________________________
bool EdbDisplay::EdbDisplayExist(const char *title)
{
    if (gROOT->GetListOfSpecials()->FindObject(title)) return true;
    return false;
}
//________________________________________________________________________
EdbDisplay::~EdbDisplay()
{
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
    yv=  vv->VY();
    zv=  vv->VZ();
    v->SetPoint(0, xv,yv,zv);
    v->SetMarkerStyle(29);
    v->SetMarkerColor(kWhite);
    v->SetMarkerSize(1.2);
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
      float dxv = (seg->X() - xv)/10.;
      float dyv = (seg->Y() - yv)/10.;
      float dzv = (seg->Z() - zv)/10.;
      line->SetPoint(0, xv+dxv,yv+dyv,zv+dzv );
      line->SetPoint(1, seg->X()-dxv, seg->Y()-dyv, seg->Z()-dzv );
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
	if(seg) 
	{
	    if (is == 0)
	    {
		line->SetPoint(is, seg->X()+seg->TX()*seg->DZ(),
				   seg->Y()+seg->TY()*seg->DZ(),
				   seg->Z()+seg->DZ() );
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
		line->SetPoint(is, seg->X()+seg->TX()*seg->DZ(),
				   seg->Y()+seg->TY()*seg->DZ(),
				   seg->Z()+seg->DZ() );
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
      pme->SetPoint(0, seg->X()+seg->TX()*seg->DZ(),
    		       seg->Y()+seg->TY()*seg->DZ(),
		       seg->Z()+seg->DZ() );
      pme->SetMarkerColor(kRed);
      pme->SetMarkerSize(1.2);
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

//_____________________________________________________________________________
void EdbVertexG::SetAsWorking()
{
  char text[512];
  if (eD && eV)
  {
    if (eD->eWait_Answer) return;
    if (eD->eWorking)
    {
	eD->DialogModifiedVTX();
	return;
    }
    eD->eVertex = eV;
    eD->CreateCanvasVTX();
    eV->V()->rmsDistAngle();
    sprintf(text,"Orig    %-4d  %-4d  %-8.1f   %-8.1f   %-8.1f   %-6.1f %-7.1f  %-7.5f",
    eV->ID(), eV->N(), eV->VX(), eV->VY(), eV->VZ(), eV->V()->dist(),
    eV->V()->chi2()/eV->V()->ndf(), eV->V()->prob());
    eD->DrawOldVTX(text);
    eD->DrawVTXTracks("Original", eD->eVertex);
    eD->DrawEnv();
    eD->eIndVert = eD->eArrV->IndexOf(eV);
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
	    if (eD->eVertex->GetTrack(i) == eTr) continue;
	    if ((vta = (eD->eWorking)->AddTrack((eD->eVertex)->GetTrack(i), (eD->eVertex)->Zpos(i), 0.0)))
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
	    if (eD->ePrevious->GetTrack(i) == eTr) continue;
	    if ((vta = (eD->eWorking)->AddTrack((eD->ePrevious)->GetTrack(i), (eD->ePrevious)->Zpos(i), 0.0)))
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
    bool usemom = (eD->eVertex)->V()->back()->kalman.use_momentum();
    if ((eD->eWorking)->MakeV(usemom))
    {
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
	eD->DrawOldBut("Original");
	eD->DrawVTXTracks("Modified", eD->eWorking);
	if (ePreviousSaved) delete ePreviousSaved;
	if (eD->eIndVert >= 0) 
	{
	    eD->eArrV->RemoveAt(eD->eIndVert);
	    eD->eArrV->AddAt(eD->eWorking, eD->eIndVert);
	}	
	eD->Draw();
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
void EdbTrackG::AddTrack()
{
  char text[512];
  int zpos = 1;
  float ProbMinV = 0.00;
  EdbVTA *vta = 0;
  // ask for zpos and ProbMinV
  if (eTr && eD)
  {
    if (eD->eWait_Answer) return;
    if (!(eD->eVertex)) return;
    if (GetMarkerColor() == kRed) zpos = 0;
    if (eTr->VertexS() && (zpos == 1)) return; 
    if (eTr->VertexE() && (zpos == 0)) return; 
    EdbVertex *ePreviousSaved = eD->ePrevious;
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
	bool usemom = (eD->eVertex)->V()->back()->kalman.use_momentum();
	if (!((eD->eWorking)->MakeV(usemom)))
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
	    if ((vta = (eD->eWorking)->AddTrack((eD->ePrevious)->GetTrack(i), (eD->ePrevious)->Zpos(i), 0.0)))
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
	bool usemom = (eD->eVertex)->V()->back()->kalman.use_momentum();
	if (!((eD->eWorking)->MakeV(usemom)))
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
    if ((vta = (eD->eWorking)->AddTrack(eTr, zpos, ProbMinV)))
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
	eD->DrawOldBut("Original");
	eD->DrawVTXTracks("Modified", eD->eWorking);
	if (ePreviousSaved) delete ePreviousSaved;
	if (eD->eIndVert >= 0) 
	{
	    eD->eArrV->RemoveAt(eD->eIndVert);
	    eD->eArrV->AddAt(eD->eWorking, eD->eIndVert);
	}	
	eD->Draw();
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
    if (eArrVSave)
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
	eIndVert = eIndVertSave;
	Draw();    
    }
    else
    {
	li->Remove(fEnvButton);
	if (eIndVert >= 0) 
	{
	    eArrV->RemoveAt(eIndVert);
	    eArrV->AddAt(eVertex, eIndVert);
	}	
    }
    fTrigPad->cd();
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
    if (eWait_Answer) CloseDialogModifiedVTX();
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
    Draw();
}
//_____________________________________________________________________________
void EdbDisplay::UndoModifiedVTX()
{
    char text[512];
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
	DrawVTXTracks("Modified", eWorking);
	ePrevious = 0;
	if (eIndVert >= 0) 
	{
	    eArrV->RemoveAt(eIndVert);
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
	DrawVTXTracks("Original", eVertex);
	eWorking = 0;
	if (eIndVert >= 0) 
	{
	    eArrV->RemoveAt(eIndVert);
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
    if (ePrevious && (ePrevious != eVertex) && (ePrevious != eWorking))
    {
	delete ePrevious;
	ePrevious = 0;
    }
    if (eWorking && eVertex)
    {
	if (!ePVR) ePVR = ((EdbPVRec*)(gROOT->FindObject("EdbPVRec")));
        EdbVertex *eW = eWorking;
	int ind = eVertex->ID();
	eW->SetID(ind);
	eW->SetQuality(eW->V()->prob()/
			   (eW->V()->vtx_cov_x()+eW->V()->vtx_cov_y()));
	if (ePVR)
	{
	    int ntr = eW->N();
	    int i = 0;
	    for(i=0; i<ntr; i++)
	    {
		(ePVR->eVTA).Remove(eVertex->GetVTa(i));
	    }
	}
	int indd = -1;
/*	indd = eArrV->IndexOf(eVertex);
	if (indd >= 0)
	{
	    eArrV->RemoveAt(indd);
	    eArrV->AddAt(eW, indd);
	}
	if (eArrVSave)
	{
	    indd = eArrVSave->IndexOf(eVertex);
	    if (indd >= 0)
	    {
		eArrVSave->RemoveAt(indd);
		eArrVSave->AddAt(eW, indd);
	    }
	}*/
	delete eVertex;
	eW->ResetTracks();
	int ntr = eW->N();
	int i = 0, ifl = 0;
	EdbTrackP *tr = 0;
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
		    if (eArrTrSave)
		    {
			indd = eArrTrSave->IndexOf(tr);
			if (indd < 0)
			{
			    eArrTrSave->Add(tr);
			}
		    }
		}
	}
	ifl = 4 - ifl;
	if (ifl == 3) ifl = 0;
	if (eW->Nv()) ifl += 3;
	eW->SetFlag(ifl);
	if (ePVR)
	{
	    ePVR->eVTX->RemoveAt(ind);
	    ePVR->eVTX->AddAt(eW, ind);
	    for(i=0; i<ntr; i++)
	    {
		ePVR->AddVTA(eW->GetVTa(i));
	    }
	}
	eVertex = 0;
	eWorking = 0;
    }
    TList *li = fTrigPad->GetListOfPrimitives();
    li->Remove(fUndButton);
    li->Remove(fAccButton);
    li->Remove(fCanButton);
    if (eArrVSave)
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
	eIndVert = eIndVertSave;
    }
    else
    {
	li->Remove(fEnvButton);
    }
    fTrigPad->cd();
    fTrigPad->Update();
    fTrigPad->Draw();
    fPad->cd();
    if (eWait_Answer) CloseDialogModifiedVTX();
    if (fCanvasVTX) fCanvasVTX->Close();
    fCanvasVTX = 0;
}
//_____________________________________________________________________________
void EdbDisplay::DrawVertexEnvironment()
{
    if (!ePVR) ePVR = ((EdbPVRec *)(gROOT->FindObject("EdbPVRec")));
    if (!ePVR) return;
    if (!eVertex) return;
    
    fTrigPad->cd();
    fTrigPad->GetListOfPrimitives()->Remove(fEnvButton);
    fTrigPad->GetListOfPrimitives()->Add(fAllButton);
    fAllButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fPad->cd();

    EdbVertex *eW = eVertex;
    if (eWorking) eW = eWorking;
    float Rmax = 1000., ImpMax = 10000.;
    int Dpat = 2;
    ePVR->VertexNeighboor(eW, Rmax, Dpat, ImpMax);
    eArrTrSave  = eArrTr; 
    eArrSegPSave  = eArrSegP; 
    eArrVSave  = eArrV;
    eIndVertSave = eIndVert; 
    eArrV = new TObjArray(20);
    eArrTr = new TObjArray(20);
    eArrSegP = new TObjArray(20);
    eArrV->Add(eW);
    int nntr = eW->N();
    for(int i=0; i<nntr; i++)
    {
	eArrTr->Add(eW->GetTrack(i));
    }
    nntr = eW->Nn();
    EdbVTA *vta = 0;
    for(int i=0; i<nntr; i++)
    {
	if ((vta = eW->GetVTn(i)))
	{
	    if (vta->Flag() == 0) //track
	    {
		eArrTr->Add(vta->GetTrack());
	    }
	    else if (vta->Flag() == 1) //segment
	    {
		eArrSegP->Add((EdbSegP *)(vta->GetTrack()));
	    }
	}
    }
    eIndVert = eArrV->IndexOf(eW);
    Draw();    
}
//_____________________________________________________________________________
void EdbDisplay::DrawAllObjects()
{
    fTrigPad->cd();
    fTrigPad->GetListOfPrimitives()->Remove(fAllButton);
    fTrigPad->GetListOfPrimitives()->Add(fEnvButton);
    fEnvButton->Draw();
    fTrigPad->Modified(kTRUE);
    fTrigPad->Update();
    fPad->cd();

    if (eArrVSave)
    {
	delete eArrV;
	eArrV = eArrVSave;
	eArrVSave = 0;
    }
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
    eIndVert = eIndVertSave;
    if (eWorking) 
    {
	eArrV->RemoveAt(eIndVert);
	eArrV->AddAt(eWorking, eIndVert);
    }
    Draw();    
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
  fCanvasVTX->cd();
  if (fVTXTracks)
  {
    fVTXTracks->Clear();
  }
  else
  {
    fVTXTracks = new TPaveText(0.05, 0.05, 0.95, 0.72);
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
  fCanvasVTX->Modified(kTRUE);
  fCanvasVTX->Update();
}
