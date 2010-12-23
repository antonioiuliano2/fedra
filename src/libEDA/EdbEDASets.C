#include"EdbEDASets.h"
#include"EdbEDA.h"
#include"TEveVSDStructs.h"
extern EdbEDA *gEDA;

using namespace EdbEDAUtil;

ClassImp(EdbEDAArea)
ClassImp(EdbEDAAreaSet)
ClassImp(EdbEDAVertexSet)
ClassImp(EdbEDAExtentionSet)



void EdbEDAAreaSet::SetAreas(EdbPVRec *pvr){
	if(pvr==NULL) return;
	for(int i=0;i<pvr->Npatterns(); i++){
		EdbPattern *p = pvr->GetPattern(i);
		if(p==NULL) continue;
		if(p->N()==0) continue;
		
		int ipl = p->GetSegment(0)->Plate();
		EdbEDAArea *area = new EdbEDAArea( ipl, p->Xmin(), p->Xmax(), p->Ymin(), p->Ymax(), p->Z());
		eAreas->Add(area);
	}
}

void EdbEDAAreaSet::SetAreas(EdbScanSet *ss){
	if(ss){
		EdbBrickP *b = &ss->eB;
		for(int i=0;i<b->Npl();i++){
			EdbPlateP *pl = b->GetPlate(i);
			EdbLayer *l = pl->GetLayer(0);
			EdbEDAArea *a = new EdbEDAArea(pl->ID(), l->Xmin(), l->Xmax(), l->Ymin(), l->Ymax(), l->Z());
			eAreas->Add(a);
		}
		return;
	}
}

void EdbEDAAreaSet::SetAreas(TObjArray *tracks){
	printf("Set scanning areas from TS tracks %d\n", tracks->GetEntries());
	
	TObjArray *areas = new TObjArray;
	EdbScanSet *ss = gEDA->GetScanSet();
	if(ss){
		EdbBrickP *b = &ss->eB;
		for(int i=0;i<b->Npl();i++){
			EdbPlateP *pl = b->GetPlate(i);
			//EdbLayer *l = pl->GetLayer(0);
			//EdbEDAArea *a = new EdbEDAArea(pl->ID(), l->Xmin(), l->Xmax(), l->Ymin(), l->Ymax(), l->Z());
			EdbEDAArea *a = new EdbEDAArea;
			a->SetPlate(pl->ID());
			a->SetZ(pl->Z());
			areas->Add(a);
		}
		for(int i=0;i<tracks->GetEntries();i++){
			EdbTrackP *t = (EdbTrackP *)tracks->At(i);
			if(t==NULL) continue;
			for(int j=0;j<t->N();j++){
				EdbSegP *s = t->GetSegment(j);
				EdbEDAArea *a=NULL;
				for(int k=0;k<areas->GetEntries();k++){
					a = (EdbEDAArea *) areas->At(k);
					if(a->Plate()==s->Plate()) break;
				}
					
				if(s->X()<a->Xmin()) a->SetXmin(s->X());
				if(s->X()>a->Xmax()) a->SetXmax(s->X());
				if(s->Y()<a->Ymin()) a->SetYmin(s->Y());
				if(s->Y()>a->Ymax()) a->SetYmax(s->Y());
			}
		}
		
		for(int i=0;i<areas->GetEntries();i++){ AddArea( (EdbEDAArea *)areas->At(i));}
		return;
	}
	
	
	EdbDataSet *dset = gEDA->GetDataSet();
	for(int i=0;i<dset->N();i++){
		EdbEDAArea *a = new EdbEDAArea;
		a->SetZ(gEDA->GetZPID(i));
		a->SetPlate(gEDA->GetIPL(i));
		areas->Add(a);
	}
	
	for(int i=0;i<tracks->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *)tracks->At(i);
		if(t==NULL) continue;
		for(int j=0;j<t->N();j++){
			EdbSegP *s = t->GetSegment(j);
			EdbEDAArea *a = GetAreaIPL(s->Plate());
			if(s->X()<a->Xmin()) {a->SetXmin(s->X()); if(s->PID()==0)printf("xmin %lf\n", s->X());}
			if(s->X()>a->Xmax()) {a->SetXmax(s->X()); if(s->PID()==0)printf("xman %lf\n", s->X());}
			if(s->Y()<a->Ymin()) a->SetYmin(s->Y());
			if(s->Y()>a->Ymax()) a->SetYmax(s->Y());
		}
	}
	
	for(int i=0;i<areas->GetEntries();i++){ AddArea( (EdbEDAArea *)areas->At(i));}
//	areas->Delete();
}

//#include<TEveGeoNode.h>
//#include<TGeoBBox.h>
void EdbEDAAreaSet::Draw(int redraw){
	if(eCompound) UnDraw();
	if(eDraw==kFALSE||eDrawG==kFALSE) return;
	if(eAreas->GetEntries()==0) return;
	eCompound = new TEveCompound;
	eCompound->SetElementName("Scanning Areas");
	gEve->AddElement(eCompound);

	for(int i=0;i<eAreas->GetEntries();i++){
		EdbEDAArea *area = (EdbEDAArea *) eAreas->At(i);
		if(area->GetDraw()==kFALSE) continue;
		TEveLine *l = new TEveLine;
		l->SetLineColor(eLineColor);
		l->SetLineWidth(eLineWidth);
		if(gEDA->Japanese()) l->SetName(
			Form("PL %2d (%2d), X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %8.1lf, Area : %6d x %6d = %.1lfcm2", 
			area->Plate(), 58-area->Plate(), area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z(),
			(int)(area->Xmax()-area->Xmin()), (int)(area->Ymax()-area->Ymin()), 
			(area->Xmax()-area->Xmin())*(area->Ymax()-area->Ymin())/1e8));
		else l->SetName(
			Form("PL %2d, X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %8.1lf, Area : %6d x %6d = %.1lfcm2", 
			area->Plate(),area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z(),
			(int)(area->Xmax()-area->Xmin()), (int)(area->Ymax()-area->Ymin()), 
			(area->Xmax()-area->Xmin())*(area->Ymax()-area->Ymin())/1e8));
		l->SetNextPoint(area->Xmin(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmin(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmax(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmax(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmin(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		eCompound->AddElement(l);
		
		TEveText* t = new TEveText;
		t->SetText(Form("%d",area->Plate()));
		t->PtrMainTrans()->SetPos( area->Xmax(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		t->SetFontSize(eTextSize);
		t->SetMainColor(eTextColor);
		t->SetFontMode(TGLFont::kBitmap);
		l->AddElement(t);

		/*
		// Trial using transparent geometories.
		// Make Shape
		TEveGeoShape *s = new TEveGeoShape(Form("PL %2d, X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %6.1lf", 
			area->Plate(),area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z()));
		// register Box
		s->SetShape(new TGeoBBox("BOX", area->RX(), area->RY(), 300));
		
		// Set position
		s->PtrMainTrans()->SetPos(area->X(), area->Y(), area->Z()*gEDA->GetScaleZ());
		
		// Set Color
		s->SetMainColor(kCyan);
		
		// Set Transparency. 
		s->SetMainTransparency( 90 );
		
		eCompound->AddElement(s);
		*/
	}
	if(redraw) gEve->Redraw3D();
}


void EdbEDAVertexSet::DrawSingleVertex(EdbVertex *v){
	// Draw single vertex.
	TEvePointSet *ps = new TEvePointSet();
	ps->SetElementName(Form("vtx %3d %d %8.1lf %8.1lf %8.1lf", v->ID(), v->N(), v->X(), v->Y(), v->Z()));
	ps->SetMarkerStyle(eVertexMarkerStyle);
	ps->SetNextPoint(v->X(), v->Y(), v->Z()*gEDA->GetScaleZ());
	
	
	// in case the coloring of selected vertex is not done EdbEDA::SetSelection()
	EdbVertex *selected_vertex = gEDA->GetSelectedVertex();
	if(selected_vertex==v) {
		ps->SetMarkerColor(kRed);
	} else {
		ps->SetMarkerColor(eVertexColor);
	}
	
	
	// coloring of selected vertex is done in EdbEDA::SetSelection()
	//ps->SetMarkerColor(eVertexColor);
	ps->SetUserData(v);
	
	gEve->AddElement(ps);
	
}

void EdbEDAVertexSet::Draw(int redraw){
	UnDraw();
	
	int nvertices = eVertices->GetEntries();
	printf("VertexSet   Draw %4d %s\n", nvertices, nvertices<=1 ? "vertex" : "vertices");
	for(int i=0;i<nvertices;i++){
		EdbVertex *v = (EdbVertex *) eVertices->At(i);
		DrawSingleVertex(v);
	}
	
	if(redraw) gEve->Redraw3D();
}

void EdbEDAVertexSet::UnDraw(void){
	TEveEventManager *ev = gEve->GetCurrentEvent();
	if(ev==NULL) return;
	if(ev->NumChildren()==0) return;
	
	TEveElement::List_i it = ev->BeginChildren();
	for(;;it++){
		if(it==ev->EndChildren()) break;
		TEveElement *e = *it;
		if(IsVertex(e)){
			e->Destroy();
			it--;
		}
	}
}


void EdbEDAExtentionSet::Draw(int redraw, int fix){
	if(!eEnable) return;
	if(eCompound) UnDraw();
	
	TObjArray *selected = gEDA->GetSelected();
	if(selected->GetEntries()==0) return;
	eCompound = new TEveCompound;
	eCompound->SetName("ExtentionSet");
	gEve->AddElement(eCompound);

	for(int i=0;i<selected->GetEntries();i++){
		EdbSegP   *s = (EdbSegP *)   selected->At(i);
		EdbTrackP *t = gEDA->GetTrack(s);
		TEveLine *l = new TEveLine;
		l->SetName(Form("seg itrk %4d flag %4d pl %2d id %6d ph %2d %8.1lf %8.1lf %8.1lf %7.4lf %7.4lf %4d", 
			t?t->ID():0, t?t->Flag():0, s->Plate(), (int)s->ID(), (int)s->W(), 
			s->X(), s->Y(), s->Z(), s->TX(), s->TY(), s->Flag()));
		l->SetLineColor(eColor);
		l->SetNextPoint(s->X(), s->Y(), s->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(s->X()+s->TX()*(eZ-s->Z()),
						s->Y()+s->TY()*(eZ-s->Z()),
						(eZ)*gEDA->GetScaleZ());
		//l->SetUserData(s);
		if(fix) gEve->AddElement(l);
		else eCompound->AddElement(l);
	}
	
	if(redraw) gEve->Redraw3D();
}

void EdbEDAExtentionSet::UnDraw(int redraw){
	if(gEve->GetCurrentEvent()!=NULL){
		TEveElement *ext = gEve->GetCurrentEvent()->FindChild("ExtentionSet");
		if(ext) ext->Destroy();
		//if(ext) gEve->GetCurrentEvent()->RemoveElement(ext);
	}
	eCompound=NULL;
	if(redraw) gEve->Redraw3D();
}

void EdbEDAExtentionSet::DrawResolution(int redraw){
	TObjArray *selected = gEDA->GetSelected();
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	
	for(int i=0;i<selected->GetEntries();i++){
		EdbSegP *s = (EdbSegP *) selected->At(i);
		EdbTrackP *t = (EdbTrackP*) selected_tracks->At(i); // temporary
		
		double p,pmin,pmax;
		if(t!=NULL){
			p=t->P();
			if(p<0)	CalcP(t, p, pmin, pmax);
			if(p<0.01) p = 1.0;
		} else p = 1.0;
		
		
		// definition of cone set.
		TEveBoxSet* cones = new TEveBoxSet("ConeSet");
		cones->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);
		
		// setting for colors. seems necessary.
		gStyle->SetPalette(-1);
		TEveRGBAPalette* pal = new TEveRGBAPalette(0, 50);
		cones->SetPalette(pal);
		
		TEveVector dir, pos;
		int direction = eZ<s->Z() ? -1 : 1; // parity of cone direction.

		double angle  = sqrt(s->TX()*s->TX()+s->TY()*s->TY());
		double dtheta = 0.002+ 0.002*angle;
		double dr     = 0.5 + angle;
		
		double step = 20;
		for(double dz=10;dz<fabs(eZ-s->Z())+step;dz+=step){ // dz : at the circle.
			int flag_last=0; // if it is the last cone, put cap on it.
			if(dz > fabs(eZ-s->Z())) {
				dz = fabs(eZ-s->Z());
				flag_last=1;
			}
			dir.Set( s->TX(), s->TY(), (Float_t)1.0);  // direction of cone.
			double height = dz<300? dz : 300; // hight of cone.
			dir *= direction * height;
			
			// resolutions. currently calculated as all in lead.
			double dz2 = direction*fabs(dz-height);       // dz2 at the vertex.
			double l = dz*sqrt(1+angle*angle);           // length from segment
			double dr_theta = dtheta*l;                   // dr from angle resolution
			double dr_scatt = 0.0136/p*sqrt(l/3/5700.0)*l; // dr from scatterning
			double r = sqrt(dr*dr+dr_theta*dr_theta+dr_scatt*dr_scatt);  // radius = 1sigma of error

			pos.Set(s->X()+dz2*s->TX(), s->Y()+dz2*s->TY(), s->Z()+dz2); // position of vertex
			cones->AddCone(pos, dir, r);
			cones->DigitValue(18); 
			
			if(flag_last){ // Cap
				dir.Set( s->TX(), s->TY(), (Float_t)1.0);
				dir *= -direction*r;
				dz2 = direction*fabs(dz+r);
				pos.Set(s->X()+dz2*s->TX(), s->Y()+dz2*s->TY(), s->Z()+dz2);
				cones->AddCone(pos, dir, r);
				cones->DigitValue(18);
				printf("Extention with resolution, track %4d p = %.2lf, resolution = %.1lf um at z = %.0lf (dz = %.0lf)\n", t->ID(), p, r, s->Z()+direction*dz, dz);
			}
		}
		gEve->AddElement(cones);
	}
	printf("draw\n");
	if(redraw) gEve->Redraw3D();
}
