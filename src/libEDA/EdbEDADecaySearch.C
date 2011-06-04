#include"EdbEDADecaySearch.h"
#include"EdbEDA.h"
extern EdbEDA *gEDA;

ClassImp(EdbEDATrackP)
ClassImp(EdbEDADecaySearch)
ClassImp(EdbEDADecaySearchTab)
ClassImp(EdbEDAFeedbackEditor)
ClassImp(EdbEDAFeedbackEntryT)

void EdbEDATrackP::Print(){
	printf("%6d %6d %4d %2d %2d %8.1lf %8.1lf %8.1lf %7.4lf %7.4lf ",
		gEDA->GetBrick(), ID(), Flag(),  GetSegmentFirst()->Plate(), N(),
		X(), Y(), Z(), TX(), TY());
	printf("%2d %2d %2d %2d %6.1lf %4d ",
		 SegmentFirst()->Plate(),
		Particle(), CSTrack(), Darkness(), IPUp(), Classification());
	printf("%c%c%c %s\n",
		TrackSearch()&EdbEDADecaySearch::kParentSearch       ? 'P' : '-',
		TrackSearch()&EdbEDADecaySearch::kBaseTrackSearch    ? 'B' : '-',
		TrackSearch()&EdbEDADecaySearch::kDaughterSearch     ? 'D' : '-',
		Comment());
}
void EdbEDATrackP::SetFlagsAuto(){
	// check if the track is a scanback track
	EdbTrackP *tsb = gEDA->CheckScanback(this);
	if(tsb) SetCSTrack(tsb->ID());

	int idmuon     = gEDA->IdMuon();
	if (CSTrack()==idmuon&&idmuon!=-1) {
		SetParticle(1); // 1: muon
		printf("      Track: %d is muon\n", ID());
	}
	double p, pmin, pmax;
	CalcP(this, p, pmin, pmax, 0); // pmin = pmin, pmax = pmax
	SetP(p,pmin,pmax);
}

ClassImp(EdbEDADecayVertex)

EdbEDADecayVertex::EdbEDADecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbEDATrackP *parent, EdbEDATrackP *daughter1, EdbEDATrackP *daughter2, EdbEDATrackP *daughter3, EdbEDATrackP *daughter4, EdbEDATrackP *daughter5){
	
	
	TObjArray daughters;
	if( daughter1 ) daughters.Add(daughter1);
	if( daughter2 ) daughters.Add(daughter2);
	if( daughter3 ) daughters.Add(daughter3);
	if( daughter4 ) daughters.Add(daughter4);
	if( daughter5 ) daughters.Add(daughter5);
	
	EdbEDADecayVertex( v2ry, v1ry, parent, &daughters);
	
}
	
	
	
EdbEDADecayVertex::EdbEDADecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbEDATrackP *parent, TObjArray *daughters) {
	EdbEDADecayVertex();
	if(v2ry) EdbVertex::SetXYZ( v2ry->X(), v2ry->Y(), v2ry->Z());
	ePrimaryVertex = v1ry;
	
	if(parent) SetParent( parent);
	if(daughters) SetDaughters( daughters);
	
}

EdbEDADecayVertex::~EdbEDADecayVertex(){
	if(eDaughters) delete eDaughters;
}

void EdbEDADecayVertex::AddTrack( EdbEDATrackP *t){
	EdbVTA *vta = new EdbVTA(t->GetOriginal(), (EdbVertex *)this);
	vta->SetZpos( t->Z()>this->Z() ? 1 : 0);
	vta->SetFlag(2);
	vta->SetImp( CalcIP( t, (EdbVertex *) this));
	AddVTA(vta);
}
	
void EdbEDADecayVertex::SetParent( EdbEDATrackP *parent){
	eParent = parent;
	AddTrack( eParent);
}

void EdbEDADecayVertex::SetDaughter( EdbEDATrackP *daughter){
	if(eDaughters==NULL) eDaughters=new TObjArray;
	if(!eDaughters->FindObject(daughter)) {
		eDaughters->Add(daughter);
		AddTrack(daughter);
	}
}

void EdbEDADecayVertex::SetDaughters( TObjArray *daughters){
	for(int i=0; i<daughters->GetEntries(); i++){
		SetDaughter( (EdbEDATrackP *) daughters->At(i));
	}
}


ClassImp(EdbEDASmallKink)


int EdbEDASmallKink::IPL1(){ return S1()->Plate();}
int EdbEDASmallKink::IPL2(){ return S2()->Plate();}

void EdbEDASmallKink::Draw(Option_t *option){
	TEvePointSet *ps = new TEvePointSet();
	ps->SetMarkerStyle(21);
	ps->SetNextPoint(eS1->X(), eS1->Y(), eS1->Z()*gEDA->GetScaleZ());
	ps->SetMarkerColor(kGreen);
	gEve->AddElement(ps);
}



double EdbEDADecaySearch::GetTSDauIP1(double dz){
	TH1F *h = eTSDauIPHist1;
	
	if(dz<h->GetXaxis()->GetXmin()) return h->GetBinContent(1);
	if(dz>h->GetXaxis()->GetXmax()) return h->GetBinContent(h->GetNbinsX());
	for(int i=1;i<h->GetNbinsX();i++){
		if(dz>=h->GetBinLowEdge(i)&&dz<h->GetBinLowEdge(i)+h->GetBinWidth(i)) return h->GetBinContent(i);
	}
	return 0.0;
}

double EdbEDADecaySearch::GetTSDauIP2(double dz){
	TH1F *h = eTSDauIPHist2;
	
	if(dz<h->GetXaxis()->GetXmin()) return h->GetBinContent(1);
	if(dz>h->GetXaxis()->GetXmax()) return h->GetBinContent(h->GetNbinsX());
	for(int i=1;i<h->GetNbinsX();i++){
		if(dz>=h->GetBinLowEdge(i)&&dz<h->GetBinLowEdge(i)+h->GetBinWidth(i)) return h->GetBinContent(i);
	}
	return 0.0;
}
	
void EdbEDADecaySearch::SmallKinkSearch(){
	for(int i=0;i<Ntracks();i++){
		EdbEDATrackP *t = GetTrack(i);
		TObjArray *kinks=CheckInTrackKink(t);
		if(kinks->GetEntries()!=0) {
			EdbEDAPlotTab *p = gEDA->GetPlotTab();
			if(p!=NULL) p->CheckKink(t);
		}
	}
}

TObjArray *EdbEDADecaySearch::CheckInTrackKink(EdbTrackP *trk){
	// Search kink. return TObjArray of EdbEDASmallKink
	
	if(trk->N()<2) {
		printf("Track %d nseg=%d, too short for kink search\n", trk->ID(), trk->N());
		return NULL;
	}
	EdbTrackP *t = CleanTrack(trk); // Remove fake segment/microtracks. 

	double rms,rmst,rmsl;
	DTRMSTL1Kink(t, &rms, &rmst, &rmsl); // DTRMS for each projection.

	if(t->N()<=2) {
		printf("Track %d nseg=%d. too short to calculate delta-theta rms. use 2.0mrad.\n", t->ID(), t->N());
		rms=rmst=rmsl=2.0;
	}
	
	TObjArray *kinks = new TObjArray;
	
	int vtxpl;
	if(gEDA) vtxpl=gEDA->GetIPLZ(eVertex->Z());
	else
	for(int i=0; i<ePVR->Npatterns(); i++){
		EdbPattern *pat = ePVR->GetPattern(i);
		if( eVertex->Z()<pat->Z()) {
			vtxpl = pat->Plate();
			break;
		}
	}
	
	int n=t->N();
	for(int j=0;j<n-1;j++){
		EdbSegP *s1 = t->GetSegment(j);
		EdbSegP *s2 = t->GetSegment(j+1);

		// if s1 is more downstream than 5 plates from vertex, skip. search only kink between 1-x,2-x,3-x,4-x.
		if(s1->Plate()>vtxpl+3) continue;

		// calculate kink angle in transverse and longitudinal projection.
		double dtt, dtl;
		CalcDTTransLongi(s1,s2, &dtt, &dtl);
		double dt = sqrt(dtt*dtt+dtl*dtl);
		
		// calculate position difference from 1st segment.
		double dxt, dxl;
		CalcDXTransLongi(t->GetSegmentFirst(),s2, &dxt, &dxl);
		
		if( fabs(dtt)>rmst*3 || fabs(dtl)>rmsl*3 ) {
			// if there is kink.
			// if kink angle is bigger than 3 times delta-theta rms in one of the 2 projection.

			// momentum calculation using downstream from s2.
			double p,pmin,pmax;
			CalcPPartial(t,s2,t->GetSegmentLast(), p, pmin, pmax);
			
			// calculate Pt at the kink, using downstream momentum.
			double Pt = p>0 ? p*dt : -1.;

			// calculate vertex point with 2 segments.
			TObjArray segs;
			segs.Add(s1);
			segs.Add(s2);
			EdbVertex *v = CalcVertex(&segs);
			
			int ndau = n-j-1;
			// put the data in a struct.
			EdbEDASmallKink *kink = new EdbEDASmallKink(v, t, s1, s2, dtt, dtl, dxt, dxl, ndau, p, pmin, pmax, Pt, rmst, rmsl);
			
			gEDA->AddVertex(v);
			kinks->Add(kink); // for this track
			eKinks->Add(kink); // for all tracks.
			
			printf("Kink candidate. itrk %d plate %d - %d kink angle %.4lf P %.3lf Pt %.3lf ", 
				t->ID(), s1->PID(), s2->PID(), dt, p, Pt);
			printf("(Trans, Longi) = ( %.4lf, %.4lf ) thr ( %.4lf, %.4lf )\n",
				dtt, dtl, rmst*3, rmsl*3);
			//gEDA->AddDrawObject(kink);
		}
	}
	
	printf("%d kink candidates are found.\n", kinks->GetEntries());
	
	return kinks;
}




void EdbEDADecaySearch::PrintTracks(){
	printf("ibrick trkid  flag pl ns x        y        z        tx      ty      pl pa cs dk ip1ry class PBDLU comment\n");
	for(int i=0;i<Ntracks();i++){
		EdbEDATrackP *t = GetTrack(i);
		t->Print();
	}
}

void EdbEDADecaySearch::MTSearch2seg(TObjArray *tracks){
	// Microtrack search for 2 segment track.
	// if eSet is not set, do nothing.
	
	// if eSet==NULL or Preparation is failed, do nothing.
	int ret = eSet ? eSet->PrepareScanSetForMT() : -1; 
	if(ret==-1) {
		printf("ScanSet error. stop.\n");
		return;
	}
	for(int i=0;i<tracks->GetEntries();i++){
		EdbTrackP *t = ((EdbEDATrackP *) tracks->At(i))->GetOriginal();
		if(t->N()==2) eSet->MicroTrackSearch(t);
	}
}
void EdbEDADecaySearch::MTSearchAll(TObjArray *tracks){
	// Microtrack search for all tracks.
	// if eSet is not set, do nothing.
	
	// if eSet==NULL or Preparation is failed, do nothing.
	int ret = eSet ? eSet->PrepareScanSetForMT() : -1; 
	if(ret==-1) {
		printf("ScanSet error. stop.\n");
		return;
	}
	for(int i=0;i<tracks->GetEntries();i++){
		gEve->SetStatusLine(Form("MicroTrackSearch %d/%d", i+1, tracks->GetEntries()));
		EdbTrackP *t = ((EdbEDATrackP *) tracks->At(i))->GetOriginal();
		eSet->MicroTrackSearch(t);
	}
}

TObjArray * EdbEDADecaySearch::DoSearch(){
	// Do track search
	
	
	// clear previous results.
	eTracks->Clear();
	eSegments->Clear();
	
	int nAll=-1;
	int nTSDau1=-1;
	int nTSDau2=-1;
	if(gEve) gEve->SetStatusLine("Start Track search");
	printf("######### Track Search #########\n");
	if(eVertex==NULL) {
		printf("Vertex is not selected!! End.\n");
		return eTracks;
	}
	if(ePVR==NULL){
		printf("No PVRec set\n");
		return eTracks;
	}
	
	int hall[11],hdau1[11];
	int hdau2[11],hmt[11];
	for(int i=0;i<11;i++) hall[i]=hdau1[i]=hdau2[i]=hmt[i]=0;
	
	printf(" target vertex = ( %8.1f, %8.1f, %8.1f) on pid%d.\n", eVertex->X(), eVertex->Y(), eVertex->Z(), eVertexPlatePID);
	printf("  with %d track\n", ePVR->Ntracks());
	
	TObjArray *base = ePVR->eTracks;
	
	for(int i=0;i<base->GetEntries();i++) hall[((EdbTrackP *) base->At(i))->N()>=10?10:((EdbTrackP *) base->At(i))->N()]++;
	nAll=base->GetEntries();
	
	// Daughter search 1st
	if(eTSDau) TSDaughterTracks(base);
	
	for(int i=0;i<eTracks->GetEntries();i++) {
		EdbTrackP *t = ((EdbEDATrackP *) eTracks->At(i))->GetOriginal();
		int nseg= t->N();
		if( nseg>10) nseg=10;
		hdau1[nseg]++;
	}
	nTSDau1 = eTracks->GetEntries();
	printf("%d\n", nTSDau1);
	
	// Microtrack search
	if(eMT2seg&&!eMTAll) MTSearch2seg(eTracks);
	if(eMTAll) {
		MTSearchAll(eTracks);
	}
	for(int i=0;i<eTracks->GetEntries();i++) hmt[((EdbEDATrackP *) eTracks->At(i))->GetOriginal()->N()>=10?10:((EdbEDATrackP *) eTracks->At(i))->GetOriginal()->N()]++;
	
	// Daughter cut 2nd.
	if(eTSDau2){
		TObjArray tmp = *eTracks;
		TSDaughterTracks2();
	}
	for(int i=0;i<eTracks->GetEntries();i++) hdau2[((EdbEDATrackP *) eTracks->At(i))->N()>=10?10:((EdbEDATrackP *) eTracks->At(i))->GetOriginal()->N()]++;

	nTSDau2 = eTracks->GetEntries();
	
	// small kin search
	if(eSmallKink) SmallKinkSearch();
	
	if(eTSPar) TSParentTracks(base);
	
	if(eBT){
		printf(" Base track search. ");
		printf("Upstream %d plates, Downstream %d plates ip<%.1lf w>%.1lf\n", 
				eBTPlateUp, eBTPlateDown, eBTIP, eBTPH);
		for(int dPlate=-eBTPlateUp+1; dPlate<=eBTPlateDown; dPlate++){
			int pid = eVertexPlatePID + dPlate -1;
			if(pid<0||ePVR->Npatterns()-1<pid) continue;
			TSBaseTracks(pid);
		}
	}
	
	PrintTracks();
	
	printf("------  SUMMARY  --------------\n");
	printf("                         nseg ");
	for(int i=1;i<11;i++) printf("%3d ", i);
	printf("\n");
	printf(" Total            %4d tracks ", nAll);
	for(int i=1;i<11;i++) printf("%3d ", hall[i]);
	printf("\n");
	printf(" Daughter search  %4d tracks ", nTSDau1);
	for(int i=1;i<11;i++) printf("%3d ", hdau1[i]);
	printf("\n");
	printf(" Microtrack search            ");
	for(int i=1;i<11;i++) printf("%3d ", hmt[i]);
	printf("\n");
	printf(" Nseg>=3 (inc.MT) %4d tracks ", nTSDau2);
	for(int i=1;i<11;i++) printf("%3d ", hdau2[i]);
	printf("\n");
	printf("-------------------------------\n");
	
	return eTracks;
}


void EdbEDADecaySearch::SetVertex (EdbVertex *v) { 
	eVertex = v;
	if(NULL==v) return;
	double vertexplz = 1e9;
	
	printf("vertex z = %f\n", v->Z());
	for(int i=0;i<ePVR->Npatterns();i++){
		EdbPattern *pat = ePVR->GetPattern(i);
		if(pat==NULL) continue;
		if(pat->Z()<v->Z()) continue;
		if(pat->Z()<vertexplz) {
			vertexplz    = pat->Z();
			eVertexPlatePID = pat->ID();   // Set eVertexPlatePID
		}
	}
	printf("VertexPlatePID=%d\n", eVertexPlatePID);
}

TObjArray *EdbEDADecaySearch::FindUpstreamVertices(){
	
	printf("Find upstream vertices. Nvtx_base = %d\n", ePVR->Nvtx());
	TObjArray *vertices= new TObjArray;
	
	for(int i=0;i<ePVR->Nvtx();i++){
		EdbVertex *v = ePVR->GetVertex(i);
		double dz = v->Z() - eVertex->Z();
		if( dz < -eVtxUpDZ || 0 < dz ) continue;
		
		int flag=0;
		for(int j=0;j<eVertex->N();j++){
			EdbSegP *s = eVertex->GetTrack(j)->GetSegmentFirst();
			if(CalcIP(s,v)<eVtxUpIP) flag++;
		}
		if(flag ==0) continue;
		vertices->Add(v);
	}
	
	return vertices;
}


TObjArray * EdbEDADecaySearch::TSDaughterTracks(TObjArray *base){
	// Search Tracks and set eTracks. return TObjArray filled with selected tracks.
	printf(" Daughter track search. ");
	printf("nseg>=%d, ip=func of dz, w>%.1lf\n", eTSDauNseg,eTSDauPH);
//	printf("nseg>=%d, start within %d plates, ip<%.1lf w>%.1lf\n", 
//			eTSDauNseg, eTSDauPlate, 0.0 /*eTSDauIP*/, eTSDauPH);
	
	TObjArray *filtered = new TObjArray;
	
	int inew=0;
	
	for(int i=0;i<base->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *) base->At(i);
		if(t==NULL) continue;
		EdbSegP *s = t->GetSegmentFirst();
		
		// Nseg cut
		if( t->N() < eTSDauNseg ) continue;

		// dPlate cut
		//int dPlate = s->PID() - eVertexPlatePID + 1; // 
		//if( dPlate < 1 || eTSDauPlate < dPlate ) continue; // 0 <= dPlate <= eTSDauPlate
		
		// IP cut
		double ip = CalcIP(s, eVertex);
		
		double dz = s->Z() - eVertex->Z();
		
		if( ip > GetTSDauIP1(dz) )  continue;
		
		// PH cut
		double ph = t->Wgrains()/t->N();
		if( ph < eTSDauPH ) continue;
		

		// add to array
		filtered->Add(t);
		EdbEDATrackP *t2 = FindTrack(t);
		if(t2) t2->SetTrackSearch( t2->TrackSearch()|kDaughterSearch);
		else {
			inew++;
			printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
			t->GetSegmentFirst()->PID(),
			t->GetSegmentLast()->PID(),
			t->TX(),t->TY());
			
			t2 = new EdbEDATrackP(t,eVertex);
			t2->SetTrackSearch( t2->TrackSearch()|kDaughterSearch);
			t2->SetIPUp(ip);
			eTracks->Add(t2);
		}
	}
	printf("%5d tracks are selected (new %2d tracks)\n", filtered->GetEntries(), inew);
	return filtered;
}

void EdbEDADecaySearch::TSDaughterTracks2(){
	// Filter 2segment tracks after microtrack search.
	// sevior dz cut will be applied.
	
	printf(" Daughter track search2. ");
	printf("nseg>=%d, ip=func of dz\n", eTSDauNseg2);
	
	int ntrk=eTracks->GetEntries();
	for(int i=0;i<ntrk;i++){
		EdbEDATrackP *tt = (EdbEDATrackP *) eTracks->At(i);
		EdbTrackP *t = tt->GetOriginal();
		if(t==NULL) continue;
		
		EdbSegP *s = t->GetSegmentFirst();
		
		// Nseg cut
		// IP cut
		double ip = CalcIP(s, eVertex);
		double dz = s->Z() - eVertex->Z();
		
		if( ip < GetTSDauIP2(dz) && t->N() >= eTSDauNseg2)  continue;
		
		// remove this track
		eTracks->Remove(tt);
	}
	eTracks->Sort();
	printf("%5d tracks are remained. original %d tracks\n", eTracks->GetEntries(), ntrk);
}


TObjArray * EdbEDADecaySearch::TSParentTracks(TObjArray *base){
	// Search Tracks and set eTracks. return TObjArray filled with selected tracks.
	
	printf(" Parent track search. ");
	printf("nseg>=%d, start within %d plates, ip<%.1lf w>%.1lf\n", 
			eTSParNseg, eTSParPlate, eTSParIP, eTSParPH);
	
	printf("eVertexPlatePID=%d\n", eVertexPlatePID);
	
	TObjArray *filtered = new TObjArray;
	int inew=0;
	for(int i=0;i<base->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *) base->At(i);
		if(t==NULL) continue;
		EdbSegP *s = t->GetSegmentFirst();
		
		// Nseg cut
		if( t->N() < eTSParNseg ) continue;

		// dPlate cut
		int dPlate = s->PID() - eVertexPlatePID + 1;
		if( dPlate <= -eTSParPlate || eTSParPlate < dPlate ) continue; // 0 <= dPlate <= eTSParPlate
		
		// IP cut
		double ip = CalcIP(s, eVertex);
		if( ip > eTSParIP )  continue;
		
		// PH cut
		double ph = t->Wgrains()/t->N();
		if( ph < eTSParPH ) continue;
		

		// add to array
		filtered->Add(t);
		EdbEDATrackP *t2 = FindTrack(t);
		if(t2) t2->SetTrackSearch( t2->TrackSearch()|kParentSearch);
		else {
			inew++;
			printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
				t->GetSegmentFirst()->PID(),
				t->GetSegmentLast()->PID(),
				t->TX(),t->TY());
			EdbEDATrackP *t2 = new EdbEDATrackP( t, eVertex);
			t2->SetTrackSearch( t2->TrackSearch()|kParentSearch);
			eTracks->Add(t2);
		}
	}
	
	printf("%5d tracks are selected (new %2d tracks)\n", filtered->GetEntries(), inew);
	return filtered;
}


TObjArray * EdbEDADecaySearch::TSBaseTracks(int pid){
	// Search Tracks and set eTracks. return TObjArray filled with selected segments.
	TObjArray *filtered = new TObjArray();
	printf("  pid=%2d ", pid);
	EdbPattern *pat = ePVR->GetPattern(pid);
	
	if(pat==NULL) {
		printf("No pattern is available for pid=%d\n", pid);
		return filtered;
	}

	int found=0;
	int found_as_track=0;
	for(int i=0;i<pat->N();i++){
		EdbSegP *s = pat->GetSegment(i);
		if(s==NULL) continue;
		
		// IP cut
		double ip=CalcIP(s, eVertex);
		if( ip > eBTIP )  continue;
		
		// PH cut
		if( s->W() < eBTPH ) continue;
		
		found++;
		// already found as tracks to be drawn?
		int flag=0;
		for(int j=0;j<eTracks->GetEntries();j++){
			EdbTrackP *t = (EdbTrackP *)eTracks->At(j);
			for(int k=0;k<t->N();k++){
				EdbSegP *ss = t->GetSegment(k);
				if(ss->PID()==s->PID()&&ss->ID()==s->ID()){
					flag++;
					found_as_track++;
				}
			}
		}
		if(flag) continue;
		
		// add to array
		filtered->Add(s);
		eSegments->Add(s);
		// add to array
		EdbTrackP *t = new EdbTrackP(s);
		t->SetCounters();

		EdbEDATrackP *t2 = new EdbEDATrackP(t, eVertex);
		t2->SetTrackSearch( t2->TrackSearch()|kBaseTrackSearch);
		t2->SetIPUp(ip);
		eTracks->Add(t2);

		printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
			t->GetSegmentFirst()->PID(),
			t->GetSegmentLast()->PID(),
			t->TX(),t->TY());
	}
	printf("%5d segments from pid %2d. (%d found, %d already selected as track)\n", filtered->GetEntries(), pid, found, found_as_track);
	return filtered;
}

EdbVertex *EdbEDADecaySearch::MakeFakeVertex(EdbTrackP *t, double dz){
	EdbSegP *s0 = t->GetSegmentFirst();
	
	EdbSegP s(*s0);
	s.PropagateTo(s0->Z()+dz);

	EdbVertex *v = new EdbVertex;
	v->SetID(t->ID());
	v->SetXYZ(s.X(),s.Y(),s.Z());
	
	EdbVTA *vta = new EdbVTA(t, v);
	vta->SetZpos(1);
	vta->SetFlag(2);
	vta->SetImp(EdbEDAUtil::CalcIP(&s,v));
	v->AddVTA(vta);
	return v;
}

EdbVertex * EdbEDADecaySearch::FindPrimaryVertex(){
	// Find Best Vertex.
	// select best one from pvr->eVertex
	
	printf("FindBestVertex(): \n");
	if( ePVR->Nvtx()==0 ){
		printf("No vertex is set. make a fake vertex.");
		
		EdbTrackP *tup=NULL;
		for(int i=0; i<ePVR->Ntracks(); i++){
			EdbTrackP *t = ePVR->GetTrack(i);
			if(t->N()<3) continue;
			if(EdbEDAUtil::DTRMS(t)>0.02) continue;
			if(tup==NULL) tup=t;
			if(t->Z()<tup->Z()) tup=t;
		}
		if(tup==NULL) return NULL;
		
		EdbVertex *v = MakeFakeVertex( tup, -650);
		
		return v;
	}
	
	// select most upstream vertex
	int zero=0;
	EdbVertex *v_most_upstream = ePVR->GetVertex(zero);
	for(int i=1; i<ePVR->Nvtx(); i++){
		EdbVertex *v = ePVR->GetVertex(i);
		if(v->Z()<v_most_upstream->Z()) v_most_upstream=v;
	}
	
	// gather all vertex near the most upstream vertex
	// vertices within 200 micron in Z.
	
	printf("TODO: FindPrimaryVertex... find real vertex, especially QE like kink event.\n");
	printf("TODO: use SB info.\n");
	printf("TODO: constraint for vertex Z < SB stop Z\n");
	TObjArray *vertices = new TObjArray;
	for(int i=0; i<ePVR->Nvtx(); i++){
		EdbVertex *v = ePVR->GetVertex(i);
		if(v->Z()<v_most_upstream->Z()+200) vertices->Add(v);
	}
	
	// select most high probability vertex
	EdbVertex *v_highest_prob= (EdbVertex *) vertices->At(0);
	for(int i=1; i<vertices->GetEntries(); i++){
		EdbVertex *v= (EdbVertex *) vertices->At(i);
		if(v->V()->prob()>v_highest_prob->V()->prob()) v_highest_prob = v;
	}
	
	
	printf("select upstream and high probable vertex: %d vertex\n", ePVR->Nvtx());
	for(int i=0; i<ePVR->Nvtx();i++){
		EdbVertex *v = ePVR->GetVertex(i);
		int flag = 0;
		for(int j=0; j<vertices->GetEntries(); j++) { if(v==vertices->At(j)) flag=1;}
		printf("vertex %2d, %2d tracks z=%8.1f prob=%7.5f %s %s, %s\n", i, v->N(), v->Z(), v->V()->prob(), 
		        v==v_most_upstream?"most":"    ", 
		        flag?"upstream": "         ", 
		        v==v_highest_prob?"highest prob":"");
	}
	
	delete vertices;
	return v_highest_prob;
	//return v_most_upstream;
	
}

void EdbEDADecaySearch::KinkSearch(){
	
	printf("Kink search in selected tracks\n");
	for(int i=0; i<Ntracks(); i++){
		for(int j=0; j<Ntracks(); j++){
			
			if(i==j) continue;
			
			EdbEDATrackP *t1 = GetTrack(i);
			EdbEDATrackP *t2 = GetTrack(j);
			
			EdbSegP *s1 = t1->GetSegmentLast();
			EdbSegP *s2 = t2->GetSegmentFirst();
			
			// if last segment of t1 is upstream of first segment of t2.
			if(s1->Plate() < s2->Plate()){
				
				TObjArray segs;
				segs.Add(s1);
				segs.Add(s2);
				EdbVertex *v = EdbEDAUtil::CalcVertex(&segs);
				float ip1 = EdbEDAUtil::CalcIP(s1, v);
				float ip2 = EdbEDAUtil::CalcIP(s2, v);
				
				// cut with IP and Z. (Z should between 2 segments.)
				if(ip1<20 && ip2<20 && s1->Z()<v->Z() && v->Z()<s2->Z()){
					printf("Decay vertex t1 %d t2 %d ip1 %f ip2 %f\n", t1->ID(), t2->ID(), ip1, ip2);
					EdbEDADecayVertex *dv = new EdbEDADecayVertex;
					dv->SetXYZ( v->X(), v->Y(), v->Z());
					dv->SetParent(t1);
					dv->SetDaughter(t2);
					printf("kink %d %f %f %f\n", dv->N(), dv->X(), dv->Y(), dv->Z());
					eDecayVertices->Add(dv);
				}
				
				delete v;
			}
		}
	}
}


void EdbEDADecaySearchTab::DoMicroTrackSearchButton(){
	
	EdbSegP *app = gEDA->GetSelected(0);
	if(app==NULL){
		ErrorMessage("Select at least 1 segment");
		return;
	}
	int ipl = fMTIpl->GetIntNumber();
	
	// A target track.
	EdbTrackP *t = gEDA->GetTrack(app);	 
	EdbEDATrackSet *set = gEDA->GetTrackSet(t);
	
	if(set!=gEDA->GetTrackSet("TS")){
		ErrorMessage("Currently Microtrack search is only for TrackSet = TS. stop.");
		return;
	}
	set->MicroTrackSearch(t, app, ipl);
	
	gEDA->Redraw();
}


void EdbEDADecaySearchTab::DoSearchButton(){ 

	if(gEDA->GetSelectedVertex()==NULL) {
		ErrorMessage("No vertex selected!!");
		return;
	}

//	EdbPVRec *pvr = gEDA->GetTrackSet("TS")->GetPVRec();
//	SetPVR(pvr);

	
	SetVertex(gEDA->GetSelectedVertex());
	
	// to be sure that the couples are loaded.
	// if the couples are not loaded yet, load via gEDA->GetPatternPID();
	
	if(eBT){
		for(int dPlate=-eBTPlateUp+1; dPlate<=eBTPlateDown; dPlate++){
			int pid = eVertexPlatePID + dPlate -1;
			if(pid<0||ePVR->Npatterns()-1<pid) continue;
			gEDA->GetPatternIPL(gEDA->GetIPL(pid));
		}
	}
	
	gEDA->StorePrevious();
	DoSearch();
	
	gEDA->GetVertexSet()->ClearVertices();
	gEDA->GetVertexSet()->SetVertex(eVertex);
	gEDA->GetTrackSet("TS")->ClearTracks();
	
	for(int i=0;i<Ntracks();i++) gEDA->GetTrackSet("TS")->SetTrack(GetTrack(i)->GetOriginal());
	gEDA->GetTrackSet("BT")->Clear();
	gEDA->GetTrackSet("BT")->AddSegments(eSegments);
	
	gEDA->Redraw();
}

void EdbEDADecaySearchTab::ApplyParams(){

	eTSDau      = fTSDau->GetState();
	eTSDauPH    = fTSDauPH->GetNumber();
	eTSDauNseg  = fTSDauNseg->GetIntNumber();
	
//	eMT2seg     = fMT2seg->GetState();
	eMTAll      = fMTAll->GetState();
	
	eTSDau2     = fTSDau2->GetState();
	eTSDauNseg2 = fTSDauNseg2->GetIntNumber();
	
	eSmallKink  = fSmallKink->GetState();
	
/*	eTSPar      = fTSPar->GetState();
	eTSParIP    = fTSParIP->GetNumber();
	eTSParPH    = fTSParPH->GetNumber();
	eTSParNseg  = fTSParNseg->GetIntNumber();
	eTSParPlate = fTSParPlate->GetIntNumber();
*/
	eBT         = fBT->GetState();
	eBTIP       = fBTIP->GetNumber();
	eBTPH       = fBTPH->GetNumber();
	eBTPlateUp  = fBTPlateUp->GetIntNumber();
	eBTPlateDown= fBTPlateDown->GetIntNumber();
	
	CloseParamWindow();
	DoSearchButton();
}

void EdbEDADecaySearchTab::CloseParamWindow(){
	if(eParamWindow) eParamWindow->DeleteWindow();
	eParamWindow = NULL;
}


void EdbEDADecaySearchTab::FindUpstreamVertex(){
	printf("///// Find Upstream Vertices /////\n");

	if(gEDA->GetSelectedVertex()==NULL) {
		ErrorMessage("No vertex selected!!");
		return;
	}
	SetVertex(gEDA->GetSelectedVertex());
	
	ePVR = gEDA->GetTrackSet("TS")->GetPVRec();
	if(ePVR->Nvtx()==0) {
		EdbEDAVertexTab *vtxtab = gEDA->GetVertexTab();
		TObjArray tracks(*(gEDA->GetTrackSet("TS")->GetTracks()));
		vtxtab->DoVertexing();
		gEDA->GetTrackSet("TS")->ClearTracks();
		gEDA->GetTrackSet("TS")->SetTracks(&tracks);
	}
	TObjArray *vertices = FindUpstreamVertices();
	gEDA->GetVertexSet()->AddVertices(vertices);
	gEDA->GetTrackSet("TS")->SetTracksVertices(vertices,kFALSE);
	gEDA->Redraw();
}
void EdbEDADecaySearchTab::MakeFakeVertexButton(){
	
	if(gEDA->NSelectedTracks()==0) {
		ErrorMessage("Select a track.");
		return;
	}
	for(int i=0;i<gEDA->NSelectedTracks();i++){
		EdbTrackP *t = gEDA->GetSelectedTrack(i);
		double dz = -1150;
		dz=InputNumberReal("Input dz from first segment. -1150=top surf of lead, -650=middle of lead.", dz);
		EdbVertex *v = MakeFakeVertex(t,dz);
		gEDA->GetVertexSet()->AddVertex(v);
	}
	gEDA->Redraw();
}

void EdbEDADecaySearchTab::MakeGUI(){
	SetTrackSet(gEDA->GetTrackSet("TS"));

	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGMainFrame* frame = new TGMainFrame(gClient->GetRoot());
	frame->SetWindowName("XX GUI");
	frame->SetCleanup(kDeepCleanup);
	frame->SetLayoutBroken(kTRUE);

	eTab = frame;

	int posx = 10;
	int posy = 10;
	int dx=0;

	TGTextButton *fb = new TGTextButton(frame,"Do Track Search");
	frame->AddFrame(fb);
	fb->MoveResize(posx,posy,dx=110,18);
	fb->Connect("Clicked()","EdbEDADecaySearchTab", this,"DoSearchButton()");
	fb->SetToolTipText("Do track search");

	posx+=dx+10;

	TGTextButton *eButton2 = new TGTextButton(frame,"Params");
	frame->AddFrame(eButton2);
	eButton2->MoveResize(posx,posy,dx=80,20);
	eButton2->Connect("Clicked()","EdbEDADecaySearchTab", this,"MakeParamWindow()");
	fb->SetToolTipText("Open param window for track search.");

/*
	posx+=dx+30;
	TGLabel *fLabel = new TGLabel(frame, "pl");
	frame->AddFrame(fLabel);
	fLabel->MoveResize(posx,posy,dx=15,18);
	posx += dx;

	fMTIpl = new TGNumberEntry(frame, (Double_t) 0,6,-1,(TGNumberFormat::EStyle) 5);
	frame->AddFrame(fMTIpl, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fMTIpl->MoveResize(posx,posy,dx=35,20);

	posx+=dx+10;

	fb = new TGTextButton(frame,"MT search");
	frame->AddFrame(fb);
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDADecaySearchTab", this,"DoMicroTrackSearchButton()");

*/
	posx+=dx+30;

	fb = new TGTextButton(frame,"FakeVertex");
	frame->AddFrame(fb);
	fb->MoveResize(posx,posy,dx=80,20);
	fb->Connect("Clicked()","EdbEDADecaySearchTab", this,"MakeFakeVertexButton()");
	fb->SetToolTipText("Make a fake vertex for isolated tracks.");

	posx+=dx+30;
	fb = new TGTextButton(frame,"Upstream Vertex");
	frame->AddFrame(fb);
	fb->MoveResize(posx,posy,dx=100,20);
	fb->Connect("Clicked()","EdbEDADecaySearchTab", this,"FindUpstreamVertex()");
	

	frame->MapSubwindows();
	frame->Resize();
	frame->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("DecaySearch", 2);
}

void EdbEDADecaySearchTab::MakeParamWindow(){


	  // main frame
   TGMainFrame *fMainFrame = new TGMainFrame(gClient->GetRoot(),10,300,kMainFrame | kVerticalFrame);
   fMainFrame->SetLayoutBroken(kTRUE);
   
	eParamWindow = fMainFrame;

	TGLabel *fLabel;

	int posy=10;
	int posx=10;
	int dx;

	fLabel = new TGLabel(fMainFrame,"Track Search parameters  ");
	fMainFrame->AddFrame(fLabel);
	fLabel->MoveResize(posx,posy,dx=200,20);
	posx+=dx+10;


	/////////////////////////////////////////////////////////////////////
		posy+=20;
		posx=5;
		
/*
		fTSPar = new TGCheckButton(fMainFrame,"Parent Track :");
		fTSPar->MoveResize(posx,posy,dx=110,18);
		fTSPar->SetState((EButtonState)eTSPar);
		//fTSPar->Connect("Toggled(Bool_t)","EdbEDAMainTab", this, "SetDrawTrackSets(Bool_t)");
		
		
		posx += dx+5;
		fLabel = new TGLabel(fMainFrame, "IP<");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=20,18);
		
		posx += dx;
		fTSParIP = new TGNumberEntry(fMainFrame, (Double_t) eTSParIP);
		fMainFrame->AddFrame(fTSParIP, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSParIP->MoveResize(posx,posy,dx=50,18);
		
		posx += dx+5;
		fLabel = new TGLabel(fMainFrame, "W>");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=20,18);
		
		posx += dx;
		fTSParPH = new TGNumberEntry(fMainFrame, (Double_t) eTSParPH);
		fMainFrame->AddFrame(fTSParPH, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSParPH->MoveResize(posx,posy,dx=40,18);
		
		posx += dx+5;
		
		fLabel = new TGLabel(fMainFrame, "nseg>=");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=40,18);
		
		posx += dx+5;
		fTSParNseg = new TGNumberEntry(fMainFrame, (Double_t) eTSParNseg,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fTSParNseg, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSParNseg->MoveResize(posx,posy,dx=35,18);

		posx += dx+5;
		fLabel = new TGLabel(fMainFrame, "Start w/in");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=52,18);
		
		posx += dx+5;
		fTSParPlate = new TGNumberEntry(fMainFrame, (Double_t) eTSParPlate,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fTSParPlate, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSParPlate->MoveResize(posx,posy,dx=35,18);

		posx += dx;
		fLabel = new TGLabel(fMainFrame, "pl");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=15,18);
		
*/
		//////////////////////////////////////////
		posy+=20;
		posx=5;

		fTSDau = new TGCheckButton(fMainFrame,"Daughter Search 1st");
		fTSDau->MoveResize(posx,posy,dx=150,18);
		fTSDau->SetState((EButtonState)eTSDau);

		posx += dx+10;
		
		fLabel = new TGLabel(fMainFrame, "nseg>=");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=40,18);
		
		posx += dx+5;
		fTSDauNseg = new TGNumberEntry(fMainFrame, (Double_t) eTSDauNseg,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fTSDauNseg, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSDauNseg->MoveResize(posx,posy,dx=35,18);

		posx += dx+10;
		fLabel = new TGLabel(fMainFrame, "W>");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=20,18);
		
		posx += dx;
		fTSDauPH = new TGNumberEntry(fMainFrame, (Double_t) eTSDauPH);
		fMainFrame->AddFrame(fTSDauPH, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSDauPH->MoveResize(posx,posy,dx=40,18);

		
	////////////////////////////////////////////////////////

		posy += 20;
		posx = 5;
		/*
		fMT2seg = new TGCheckButton(fMainFrame,"Microtrack for 2seg ");
		fMT2seg->MoveResize(posx,posy,dx=150,18);
		fMT2seg->SetState((EButtonState)eMT2seg);
		fMT2seg->SetToolTipText("Microtrack search for 2segment track\n"
								"1 upstream, 1 downstream");
		
		posx += dx+10;
		*/
		fMTAll = new TGCheckButton(fMainFrame,"Microtrack Search");
		fMTAll->MoveResize(posx,posy,dx=150,18);
		fMTAll->SetState((EButtonState)eMTAll);
		
		posx += dx+10;
		TGTextButton *fb = new TGTextButton(fMainFrame,"Print Condition");
		fMainFrame->AddFrame(fb);
		fb->MoveResize(posx,posy,dx=110,18);
		fb->Connect("Clicked()","EdbEDADecaySearchTab", this,"PrintRunTracking()");
		fb->SetToolTipText("Print Microtrack search condition\n"
							"EdbRunTracking can be set via gEDA->GetTrackSet(\"TS\")->SetRunTracking()\n");
	
	///////////////////////////////////////////////////////
		posy+=20;
		posx=5;

		fTSDau2 = new TGCheckButton(fMainFrame,"Daughter Search 2nd :");
		fTSDau2->MoveResize(posx,posy,dx=150,18);
		fTSDau2->SetState((EButtonState)eTSDau2);

		posx += dx+10;
		
		fLabel = new TGLabel(fMainFrame, "nseg>=");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=40,18);
		
		posx += dx+5;
		fTSDauNseg2 = new TGNumberEntry(fMainFrame, (Double_t) eTSDauNseg2,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fTSDauNseg2, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fTSDauNseg2->MoveResize(posx,posy,dx=35,18);

	///////////////////////////////////////////////////////
		posy+=20;
		posx=5;
		
		fSmallKink = new TGCheckButton(fMainFrame,"Small Kink search");
		fSmallKink->MoveResize(posx,posy,dx=150,18);
		fSmallKink->SetState((EButtonState)eSmallKink);
		
		
		
	///////////////////////////////////////////////////////
		posy += 40;
		posx = 5;
		fBT = new TGCheckButton(fMainFrame,"Basetrack");
		fBT->MoveResize(posx,posy,dx=110,18);
		fBT->SetState((EButtonState)eBT);

		posx += dx+5;
		fLabel = new TGLabel(fMainFrame, "IP<");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=20,18);
		
		posx += dx;
		fBTIP = new TGNumberEntry(fMainFrame, (Double_t) eBTIP);
		fMainFrame->AddFrame(fBTIP, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fBTIP->MoveResize(posx,posy,dx=50,18);
		
		posx += dx+5;
		fLabel = new TGLabel(fMainFrame, "W>");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=20,18);
		
		posx += dx;
		fBTPH = new TGNumberEntry(fMainFrame, (Double_t) eBTPH);
		fMainFrame->AddFrame(fBTPH, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fBTPH->MoveResize(posx,posy,dx=40,18);
		

		posx += dx+5;

		fLabel = new TGLabel(fMainFrame, "Up");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=40,18);
		
		posx += dx+5;
		fBTPlateUp = new TGNumberEntry(fMainFrame, eBTPlateUp,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fBTPlateUp, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fBTPlateUp->MoveResize(posx,posy,dx=35,18);

		posx += dx;
		fLabel = new TGLabel(fMainFrame, "pl  Down");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=52,18);

		posx += dx;
		fBTPlateDown = new TGNumberEntry(fMainFrame, eBTPlateDown,6,-1,(TGNumberFormat::EStyle) 5);
		fMainFrame->AddFrame(fBTPlateDown, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		fBTPlateDown->MoveResize(posx,posy,dx=35,18);

		posx += dx;
		fLabel = new TGLabel(fMainFrame, "pl");
		fMainFrame->AddFrame(fLabel);
		fLabel->MoveResize(posx,posy,dx=15,18);

   // main frame
   // tab widget
   TGTab *fTab = new TGTab(fMainFrame,300,280);

   // container of "Tab1"
   TGCompositeFrame *fCompositeFrame;
   fCompositeFrame = fTab->AddTab("IPCut 1st");
   fCompositeFrame->SetLayoutManager(new TGVerticalLayout(fCompositeFrame));
   fCompositeFrame->SetLayoutBroken(kTRUE);

   // embedded canvas
   TRootEmbeddedCanvas *fEmbeddedCanvas = new TRootEmbeddedCanvas(0,fCompositeFrame,300,255);
   Int_t wfEmbeddedCanvas = fEmbeddedCanvas->GetCanvasWindowId();
   TCanvas *cIPDau1 = new TCanvas("cIPDau1", 0, 0, wfEmbeddedCanvas);
   fEmbeddedCanvas->AdoptCanvas(cIPDau1);
   fCompositeFrame->AddFrame(fEmbeddedCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEmbeddedCanvas->MoveResize(0,0,300,255);
   cIPDau1->cd();
   eTSDauIPHist1->Draw();


   // container of "Tab2"
   fCompositeFrame = fTab->AddTab("IPCut 2nd");
   fCompositeFrame->SetLayoutManager(new TGVerticalLayout(fCompositeFrame));

   // embedded canvas
   fEmbeddedCanvas = new TRootEmbeddedCanvas(0,fCompositeFrame,300,255);
   wfEmbeddedCanvas = fEmbeddedCanvas->GetCanvasWindowId();
   TCanvas *cIPDau2 = new TCanvas("cIPDau2", 0, 0, wfEmbeddedCanvas);
   fEmbeddedCanvas->AdoptCanvas(cIPDau2);
   fCompositeFrame->AddFrame(fEmbeddedCanvas, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fEmbeddedCanvas->MoveResize(0,0,300,255);
   cIPDau2->cd();
   eTSDauIPHist2->Draw();

   fTab->SetTab(0);

   fTab->Resize(fTab->GetDefaultSize());
   fMainFrame->AddFrame(fTab, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTab->MoveResize(480,0,300,280);

	posy+=25;

   TGTextButton *fTextButton = new TGTextButton(fMainFrame,"Do Search");
   fMainFrame->AddFrame(fTextButton, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   fTextButton->MoveResize(200,posy,100,25);
   fTextButton->Connect("Clicked()", "EdbEDADecaySearchTab", this, "ApplyParams()");

   fMainFrame->SetMWMHints(kMWMDecorAll,
                        kMWMFuncAll,
                        kMWMInputModeless);
   fMainFrame->MapSubwindows();

   fMainFrame->Resize(fMainFrame->GetDefaultSize());
   fMainFrame->MapWindow();
   fMainFrame->Resize(800,280);
   
   gClient->WaitFor(fMainFrame);
}



void EdbEDAFeedbackEntryT::ReplaceEntry(){
	
	if(gEDA->NSelectedTracks()!=1) {
		ErrorMessage("Please select a track.");
		return;
	}
	
	EdbTrackP *t = gEDA->GetSelectedTrack();
	EdbSegP *s = gEDA->GetSelected(0);
	
	TGButton *btn = (TGButton *) gTQSender;
	Int_t id = btn->WidgetId();

	switch(id){
		
		case 10:
			eTrack=t;
			break;
		case 11:
			eTrackSeg = t;
			eSeg = s;
			break;
		case 12:
			eTrackMom = t;
			break;
		
		default :
		break;
	}
	
	UpdateGUI();
}

void EdbEDAFeedbackEntryT::HilightElement(){
	gEve->GetHighlight()->RemoveElements();
	TEveElement *el = gEDA->GetEveElement(eTrack);
	gEve->GetHighlight()->AddElement(el);
	gEve->Redraw3D();
}

void EdbEDAFeedbackEntryT::UpdateGUI(){
	
	// Main Track
	TEveElement *el = gEDA->GetEveElement(eTrack);
	eTextEntry->SetText(el->GetElementName());
	
	// First Segment
	EdbSegP *s = eSeg;
	EdbTrackP *t = eTrackSeg;
	EdbEDATrackSet *set = gEDA->GetTrackSet(eTrackSeg);
	char buf[256];
	
	sprintf(buf,"%s itrk %d pl %2d %d %s ph %2d %8.1f %8.1f %7.4f %7.4f chi2 %4.2f", 
		set->GetName(), t->ID(), s->Plate(), (int)s->ID(), s->Side()? s->Side()==1? "MT1" : "MT2" : "BT", 
		(int)s->W(), s->X(), s->Y(), s->TX(), s->TY(), s->Chi2());
//	if(s->MCEvt()>=0) sprintf( buf,"%s MCEvt %d P %7.4f PdgID %6d",buf,s->MCEvt(),s->P(), s->Flag());
	eTextEntryFS->SetText(buf);
	
	// Mom
	t=eTrackMom;
	set = gEDA->GetTrackSet(eTrackMom);
	eTextEntryMom->SetText(Form("%s trk %d pl %d -> %d %8.1f %8.1f %8.1f %7.4f %7.4f %d",
			set->GetName(), t->ID(), t->GetSegmentFirst()->Plate(), t->GetSegmentLast()->Plate(), 
			t->X(),t->Y(),t->Z(),t->TX(),t->TY(), t->Flag()));
	
}


EdbEDAFeedbackEntryT::EdbEDAFeedbackEntryT(EdbTrackP *t, TGTab * parent){
	
	if(t==NULL||parent==NULL) return;
	eCompositeFrame=NULL;
	eTextEntry=NULL;
	eTabElement=NULL;
	// container of "Tab1"
	TString name = Form("%d", t->ID());
	
	// Check if same track is already selected or not.
	TGCompositeFrame *cf = parent->GetTabContainer(name);
	if( cf!=NULL ) {// if it is already selected. stop.
		printf("Track %s is already selected\n",name.Data());
		delete this;
		return;
	}
	
	SetName ( name); // name is important to manage entries.
	eTrack = eTrackMom = eTrackSeg = t;
	eSeg = t->GetSegmentFirst();
	
	eCompositeFrame = parent->AddTab( name);
	eCompositeFrame->SetLayoutManager(new TGVerticalLayout(eCompositeFrame));
	eTabElement = parent->GetTabTab(name);
	eTabElement->SetName(name);
	eTabElement->ShowClose();
	
	// Main Track
	TGLabel *fLabel = new TGLabel(eCompositeFrame,"Main Track for ID");
	eCompositeFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,10,2));
	fLabel->MoveResize(20,20,80,20);
	
	TGTextButton *fb = new TGTextButton(eCompositeFrame,"Replace Track", 10);
	eCompositeFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fb->MoveResize(20,20,80,20);
	fb->Connect("Clicked()", "EdbEDAFeedbackEntryT", this, "ReplaceEntry()");
	
	eTextEntry = new TGTextEntry(eCompositeFrame, new TGTextBuffer(300));
	eTextEntry->SetAlignment(kTextLeft);
	eTextEntry->Resize(500,eTextEntry->GetDefaultHeight());
	eCompositeFrame->AddFrame(eTextEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	// First Segment
	fLabel = new TGLabel(eCompositeFrame,"Segment for IP calculation");
	eCompositeFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,10,2));
	fLabel->MoveResize(20,20,80,20);
	
	fb = new TGTextButton(eCompositeFrame,"Replace Track", 11);
	eCompositeFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fb->Connect("Clicked()", "EdbEDAFeedbackEntryT", this, "ReplaceEntry()");
	fb->MoveResize(20,20,80,20);
	
	eTextEntryFS = new TGTextEntry(eCompositeFrame, new TGTextBuffer(300));
	eTextEntryFS->SetAlignment(kTextLeft);
	eTextEntryFS->Resize(500,eTextEntryFS->GetDefaultHeight());
	eCompositeFrame->AddFrame(eTextEntryFS, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	// RMS Mom
	fLabel = new TGLabel(eCompositeFrame,"Main Track for ID");
	eCompositeFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,10,2));
	fLabel->MoveResize(20,20,80,20);
	
	fb = new TGTextButton(eCompositeFrame,"Replace Track", 12);
	eCompositeFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fb->Connect("Clicked()", "EdbEDAFeedbackEntryT", this, "ReplaceEntry()");
	fb->MoveResize(20,20,80,20);
	
	eTextEntryMom = new TGTextEntry(eCompositeFrame, new TGTextBuffer(300));
	eTextEntryMom->SetAlignment(kTextLeft);
	eTextEntryMom->Resize(500,eTextEntryMom->GetDefaultHeight());
	eCompositeFrame->AddFrame(eTextEntryMom, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	UpdateGUI();
	
	eCompositeFrame->Layout();
	eCompositeFrame->MapSubwindows();
	eCompositeFrame->Resize(eCompositeFrame->GetDefaultSize());
	eCompositeFrame->MapWindow();
}


void EdbEDAFeedbackEntryV::HilightElement(){
	gEve->GetHighlight()->RemoveElements();
	TEveElement *el = gEDA->GetEveElement(eVertex);
	gEve->GetHighlight()->AddElement(el);
	gEve->Redraw3D();
}

EdbEDAFeedbackEntryV::EdbEDAFeedbackEntryV(EdbVertex *v, TGTab * parent){
	
	if(v==NULL||parent==NULL) return;
	eCompositeFrame=NULL;
	eTextEntry=NULL;
	eTabElement=NULL;
	// container of "Tab1"
	TString name = Form("Vertex%d", v->ID());
	
	// Check if same track is already selected or not.
	TGCompositeFrame *cf = parent->GetTabContainer(name);
	if( cf!=NULL ) { // if it is already selected. stop.
		printf("%s is already selected\n",name.Data());
		delete this;
		return;
	}
	
	SetName(name);
	eVertex = v;
	TEveElement *el = gEDA->GetEveElement(v);
	
	eCompositeFrame = parent->AddTab( name);
	eCompositeFrame->SetLayoutManager(new TGVerticalLayout(eCompositeFrame));
	eTabElement = parent->GetTabTab(name);
	eTabElement->ShowClose();
	eTabElement->SetName(name);
	
	// Vertex
	TGLabel *fLabel = new TGLabel(eCompositeFrame,"Vertex");
	eCompositeFrame->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,10,2));
	fLabel->MoveResize(20,20,80,20);
	
	eTextEntry = new TGTextEntry(eCompositeFrame, new TGTextBuffer(200));
	eTextEntry->SetText(el->GetElementName());
	eTextEntry->SetAlignment(kTextLeft);
	eTextEntry->Resize(500,eTextEntry->GetDefaultHeight());
	eCompositeFrame->AddFrame(eTextEntry, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	
	eCompositeFrame->Layout();
	eCompositeFrame->MapSubwindows();
	eCompositeFrame->Resize(eCompositeFrame->GetDefaultSize());
	eCompositeFrame->MapWindow();
}

void EdbEDAFeedbackEditor::RemoveTrackTab(Int_t id){
	TString name = fTabTracks->GetTabTab(id)->GetName();
	fTabTracks->RemoveTab(id);
	TObject *o = fTrackEntries->FindObject(name);
	fTrackEntries->Remove(o);
	fTrackEntries->Sort();
}
void EdbEDAFeedbackEditor::RemoveVertexTab(Int_t id){
	TString name = fTabVertices->GetTabTab(id)->GetName();
	fTabVertices->RemoveTab(id);
	TObject *o = fVertexEntries->FindObject(name);
	fVertexEntries->Remove(o);
	fVertexEntries->Sort();
}
void EdbEDAFeedbackEditor::HandleButtons(Int_t id){
	printf("handle buttons\n");
	printf("%d\n", id);
}


EdbEDAFeedbackEditor::~EdbEDAFeedbackEditor(){
	delete fTabTracks;
	delete fMainFrame;
}

void EdbEDAFeedbackEditor::HilightElementT(Int_t id){
	TGTabElement *tab = fTabTracks->GetCurrentTab();
	EdbEDAFeedbackEntryT *t = (EdbEDAFeedbackEntryT *) fTrackEntries->FindObject(tab->GetName());
	if(t) t->HilightElement();
}
void EdbEDAFeedbackEditor::HilightElementV(Int_t id){
	TGTabElement *tab = fTabVertices->GetCurrentTab();
	EdbEDAFeedbackEntryV *t = (EdbEDAFeedbackEntryV *) fVertexEntries->FindObject(tab->GetName());
	if(t) t->HilightElement();
}


void EdbEDAFeedbackEditor::AddTracksButton(){
	// Add a new entry tab for the selected tracks.
	// if same id is selected more than once, ignore it.
	
	printf("%d tracks are selected\n",   gEDA->NSelectedTracks());
	if(gEDA->NSelectedTracks()==0) return;
	
	for(int i=0;i<gEDA->NSelectedTracks(); i++){
		EdbEDAFeedbackEntryT *e = new EdbEDAFeedbackEntryT(gEDA->GetSelectedTrack(i), fTabTracks);
		if(e) fTrackEntries->Add(e);
	}
	fTabTracks->Layout();
	fTabTracks->Resize(fTabTracks->GetDefaultSize());

	fMainFrame->MapSubwindows();
	
	fMainFrame->Resize(fMainFrame->GetDefaultSize());
	fMainFrame->MapWindow();
}

void EdbEDAFeedbackEditor::AddVerticesButton(){
	// Add a new entry tab for the selected Vertex.
	// if same id is selected more than once, ignore it.
	EdbEDAVertexSet *set = gEDA->GetVertexSet();
	printf("%d vertices are selected\n",   set->N());
	if(set->N()==0) return;
	
	for(int i=0;i<set->N(); i++){
		EdbEDAFeedbackEntryV *e = new EdbEDAFeedbackEntryV(set->GetVertex(i), fTabVertices);
		if(e) fVertexEntries->Add(e);
	}
	fTabVertices->Layout();
	fTabVertices->Resize(fTabVertices->GetDefaultSize());

	fMainFrame->MapSubwindows();
	
	fMainFrame->Resize(fMainFrame->GetDefaultSize());
	fMainFrame->MapWindow();
}

EdbEDAFeedbackEditor::EdbEDAFeedbackEditor(){
	
	TEveWindowSlot  *slot  = 0;
	fTrackEntries  = new TObjArray;
	fVertexEntries = new TObjArray;
	
	char name[] = "Feedback";
	
	// New Tab
	slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());

	TGTab *tabs = gEve->GetBrowser()->GetTabRight();
	TGTabElement *tab  = tabs->GetTabTab(tabs->GetNumberOfTabs()-1);
	tab->SetName(name);
	tab->ShowClose();
	
	TEveWindowPack* pack1 = slot->MakePack();
	pack1->SetElementName(name);
	pack1->SetShowTitleBar(kFALSE);
	pack1->SetHorizontal();
	
	// Embedded GL viewer.
	slot = pack1->NewSlot();
	TEveViewer *v = new TEveViewer(name);

	// 5.26.00 it works. but 5.22.00 doesn't work.
	//v->SpawnGLEmbeddedViewer((TGedEditor *) gEve->GetEditor());
	
	// but below works for both for 5.26 and 5.22
	v->SpawnGLViewer((TGedEditor *) gEve->GetEditor());

	slot->ReplaceWindow(v);
	gEve->GetViewers()->AddElement(v);
	v->AddScene(gEve->GetEventScene());
	
	
	// new Slot = our GUI main
	slot = pack1->NewSlot();   
	fMainFrame = slot->MakeFrame()->GetGUICompositeFrame();
	fMainFrame->SetWindowName("Feedback editor");
	
	
	/// Vertex
	TGTextButton *fb = new TGTextButton(fMainFrame,"Add Vertices");
	fMainFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fb->MoveResize(20,20,80,20);
	fb->Connect("Clicked()","EdbEDAFeedbackEditor", this, "AddVerticesButton()");
	
	fTabVertices = new TGTab(fMainFrame);
	fTabVertices->SetTab(0);
	fTabVertices->Resize(fTabVertices->GetDefaultSize());
	fMainFrame->AddFrame(fTabVertices, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fTabVertices->Connect("Selected(Int_t)", "EdbEDAFeedbackEditor", this, "HilightElementV(Int_t)");
	fTabVertices->Connect("CloseTab(Int_t)", "EdbEDAFeedbackEditor", this, "RemoveVertexTab(Int_t)");
	
	/// Tracks
	fb = new TGTextButton(fMainFrame,"Add Tracks");
	fMainFrame->AddFrame(fb, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fb->MoveResize(20,20,80,20);
	fb->Connect("Clicked()","EdbEDAFeedbackEditor", this, "AddTracksButton()");
	
	fTabTracks = new TGTab(fMainFrame);
	fTabTracks->SetTab(0);
	fTabTracks->Resize(fTabTracks->GetDefaultSize());
	fMainFrame->AddFrame(fTabTracks, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fTabTracks->Connect("Selected(Int_t)", "EdbEDAFeedbackEditor", this, "HilightElementT(Int_t)");
	fTabTracks->Connect("CloseTab(Int_t)", "EdbEDAFeedbackEditor", this, "RemoveTrackTab(Int_t)");
	
	
	fMainFrame->MapSubwindows();
	
	fMainFrame->Resize(fMainFrame->GetDefaultSize());
	fMainFrame->MapWindow();
	fMainFrame->Resize(490,372);
}




