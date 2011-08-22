#include"EdbDecaySearch.h"

ClassImp(EdbTrackDS)
ClassImp(EdbDecaySearch)
ClassImp(EdbDecayVertex)

void EdbTrackDS::Print(){
	printf("%6d %4d %2d %2d %8.1lf %8.1lf %8.1lf %7.4lf %7.4lf ",
		ID(), Flag(),  GetSegmentFirst()->Plate(), N(),
		X(), Y(), Z(), TX(), TY());
	printf("%2d %2d %2d %2d %6.1lf %4d ",
		 SegmentFirst()->Plate(),
		Particle(), CSTrack(), Darkness(), IPUp(), Classification());
	printf("%c%c%c %s\n",
		TrackSearch()&EdbDecaySearch::kParentSearch       ? 'P' : '-',
		TrackSearch()&EdbDecaySearch::kBaseTrackSearch    ? 'B' : '-',
		TrackSearch()&EdbDecaySearch::kDaughterSearch     ? 'D' : '-',
		Comment());
}
void EdbTrackDS::SetFlagsAuto(){
	// check if the track is a scanback track
	EdbTrackP *tsb = NULL; //gEDA?gEDA->CheckScanback(this):NULL;
	if(tsb) SetCSTrack(tsb->ID());

	int idmuon     = -1; //gEDA?gEDA->IdMuon():-1;
	if (CSTrack()==idmuon&&idmuon!=-1) {
		SetParticle(1); // 1: muon
		printf("      Track: %d is muon\n", ID());
	}
	double p, pmin, pmax;
	CalcP(this, p, pmin, pmax, 0); // pmin = pmin, pmax = pmax
	SetP(p,pmin,pmax);
}



EdbDecayVertex::EdbDecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbTrackDS *parent, EdbTrackDS *daughter1, EdbTrackDS *daughter2, EdbTrackDS *daughter3, EdbTrackDS *daughter4, EdbTrackDS *daughter5){
	
	
	TObjArray daughters;
	if( daughter1 ) daughters.Add(daughter1);
	if( daughter2 ) daughters.Add(daughter2);
	if( daughter3 ) daughters.Add(daughter3);
	if( daughter4 ) daughters.Add(daughter4);
	if( daughter5 ) daughters.Add(daughter5);
	
	EdbDecayVertex( v2ry, v1ry, parent, &daughters);
	
}
	
	
	
EdbDecayVertex::EdbDecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbTrackDS *parent, TObjArray *daughters) {
	EdbDecayVertex();
	if(v2ry) EdbVertex::SetXYZ( v2ry->X(), v2ry->Y(), v2ry->Z());
	ePrimaryVertex = v1ry;
	
	if(parent) SetParent( parent);
	if(daughters) SetDaughters( daughters);
	
}

EdbDecayVertex::~EdbDecayVertex(){
	if(eDaughters) delete eDaughters;
}

void EdbDecayVertex::AddTrack( EdbTrackDS *t){
	EdbVTA *vta = new EdbVTA(t->GetOriginal(), (EdbVertex *)this);
	vta->SetZpos( t->Z()>this->Z() ? 1 : 0);
	vta->SetFlag(2);
	vta->SetImp( CalcIP( t, (EdbVertex *) this));
	AddVTA(vta);
}
	
void EdbDecayVertex::SetParent( EdbTrackDS *parent){
	eParent = parent;
	AddTrack( eParent);
}

void EdbDecayVertex::SetDaughter( EdbTrackDS *daughter){
	if(eDaughters==NULL) eDaughters=new TObjArray;
	if(!eDaughters->FindObject(daughter)) {
		eDaughters->Add(daughter);
		AddTrack(daughter);
	}
}

void EdbDecayVertex::SetDaughters( TObjArray *daughters){
	for(int i=0; i<daughters->GetEntries(); i++){
		SetDaughter( (EdbTrackDS *) daughters->At(i));
	}
}


ClassImp(EdbSmallKink)


int EdbSmallKink::IPL1(){ return S1()->Plate();}
int EdbSmallKink::IPL2(){ return S2()->Plate();}

void EdbSmallKink::Draw(Option_t *option){
	/*
	if(gEDA==NULL) return;
	TEvePointSet *ps = new TEvePointSet();
	ps->SetMarkerStyle(21);
	ps->SetNextPoint(eS1->X(), eS1->Y(), eS1->Z()*gEDA->GetScaleZ());
	ps->SetMarkerColor(kGreen);
	gEve->AddElement(ps);
	*/
}





double EdbDecaySearch::GetTSDauIP1(double dz){
	TH1F *h = eTSDauIPHist1;
	
	if(dz<h->GetXaxis()->GetXmin()) return h->GetBinContent(1);
	if(dz>h->GetXaxis()->GetXmax()) return h->GetBinContent(h->GetNbinsX());
	for(int i=1;i<h->GetNbinsX();i++){
		if(dz>=h->GetBinLowEdge(i)&&dz<h->GetBinLowEdge(i)+h->GetBinWidth(i)) return h->GetBinContent(i);
	}
	return 0.0;
}

double EdbDecaySearch::GetTSDauIP2(double dz){
	TH1F *h = eTSDauIPHist2;
	
	if(dz<h->GetXaxis()->GetXmin()) return h->GetBinContent(1);
	if(dz>h->GetXaxis()->GetXmax()) return h->GetBinContent(h->GetNbinsX());
	for(int i=1;i<h->GetNbinsX();i++){
		if(dz>=h->GetBinLowEdge(i)&&dz<h->GetBinLowEdge(i)+h->GetBinWidth(i)) return h->GetBinContent(i);
	}
	return 0.0;
}
	
void EdbDecaySearch::SmallKinkSearch(){
	for(int i=0;i<Ntracks();i++){
		EdbTrackDS *t = GetTrack(i);
		TObjArray *kinks=CheckInTrackKink(t);
		/*
		// Draw plots
		if(kinks->GetEntries()!=0) {
			if(gEDA==NULL) continue;
			EdbEDAPlotTab *p = gEDA->GetPlotTab();
			if(p!=NULL) p->CheckKink(t);
		}
		*/
	}
}

TObjArray *EdbDecaySearch::CheckInTrackKink(EdbTrackP *trk){
	// Search kink. return TObjArray of EdbSmallKink
	
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
	//if(gEDA) vtxpl=gEDA->GetIPLZ(eVertex->Z());
	//else {
		vtxpl=0; // TODO
		printf("VERTEX plate is not set!!!!! \n");
	//}
	
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
		if(s1->Plate()>vtxpl+eSmallKinkNpl) continue;
		
		// calculate kink angle in transverse and longitudinal projection.
		double dtt, dtl;
		CalcDTTransLongi(s1,s2, &dtt, &dtl);
		double dt = sqrt(dtt*dtt+dtl*dtl);
		
		// calculate position difference from 1st segment.
		double dxt, dxl;
		CalcDXTransLongi(t->GetSegmentFirst(),s2, &dxt, &dxl);
		
		if( fabs(dtt)>rmst*eSmallKinkRmin || fabs(dtl)>rmsl*eSmallKinkRmin ) {
			// if there is kink.
			// if kink angle is bigger than 5 times delta-theta rms in one of the 2 projection.

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
			EdbSmallKink *kink = new EdbSmallKink(v, t, s1, s2, dtt, dtl, dxt, dxl, ndau, p, pmin, pmax, Pt, rmst, rmsl);
			
			// if(gEDA) gEDA->AddVertex(v);
			kinks->Add(kink); // for this track
			eKinks->Add(kink); // for all tracks.
			
			{
				// Make Decay vertex
				// this is a temporary solution.
				EdbDecayVertex *vdecay = new EdbDecayVertex;
				vdecay->SetXYZ( v->X(), v->Y(), v->Z());
				vdecay->SetPrimaryVertex(eVertex);
				
				EdbTrackP *tp = new EdbTrackP(*t);
				tp->Clear();
				for(int i=0; i<=j; i++) tp->AddSegment( t->GetSegment(i));
				tp->SetCounters();
				vdecay->SetParent( new EdbTrackDS(tp, eVertex));
				
				EdbTrackP *td = new EdbTrackP(*t);
				td->Clear();
				for(int i=j+1; i<t->N(); i++) td->AddSegment( t->GetSegment(i));
				td->SetCounters();
				vdecay->SetDaughter( new EdbTrackDS(td, eVertex));
				vdecay->SetType( EdbDecayVertex::kSmallKink | EdbDecayVertex::kLong);
				eDecayVertices->Add(vdecay);
			}
			
			
			printf("Kink candidate. itrk %d plate %d - %d kink angle %.4lf P %.3lf Pt %.3lf ", 
				t->ID(), s1->PID(), s2->PID(), dt, p, Pt);
			printf("(Trans, Longi) = ( %.4lf, %.4lf ) thr ( %.4lf, %.4lf )\n",
				dtt, dtl, rmst*3, rmsl*3);
			// gEDA->AddDrawObject(kink);
		}
	}
	
	printf("%d kink candidates are found.\n", kinks->GetEntries());
	
	return kinks;
}




void EdbDecaySearch::PrintTracks(){
	printf("ibrick trkid  flag pl ns x        y        z        tx      ty      pl pa cs dk ip1ry class PBDLU comment\n");
	for(int i=0;i<Ntracks();i++){
		EdbTrackDS *t = GetTrack(i);
		t->Print();
	}
}

void EdbDecaySearch::MTSearch2seg(TObjArray *tracks){
	// Microtrack search for 2 segment track.
	// if eSet is not set, do nothing.
	
	// if eSet==NULL or Preparation is failed, do nothing.
	//int ret = eSet ? eSet->PrepareScanSetForMT() : -1; 
	int ret = -1; 
	if(ret==-1) {
		printf("ScanSet error. stop.\n");
		return;
	}
	
//	for(int i=0;i<tracks->GetEntries();i++){
//		EdbTrackP *t = ((EdbTrackDS *) tracks->At(i))->GetOriginal();
//		if(t->N()==2) eSet->MicroTrackSearch(t);
//	}
}
void EdbDecaySearch::MTSearchAll(TObjArray *tracks){
	// Microtrack search for all tracks.
	// if eSet is not set, do nothing.
	
	// if eSet==NULL or Preparation is failed, do nothing.
	//int ret = eSet ? eSet->PrepareScanSetForMT() : -1; 
	int ret = -1;
	if(ret==-1) {
		printf("ScanSet error. stop.\n");
		return;
	}
	
//	for(int i=0;i<tracks->GetEntries();i++){
//		gEve->SetStatusLine(Form("MicroTrackSearch %d/%d", i+1, tracks->GetEntries()));
//		EdbTrackP *t = ((EdbTrackDS *) tracks->At(i))->GetOriginal();
//		eSet->MicroTrackSearch(t);
//	}

}

TObjArray * EdbDecaySearch::DoSearch(){
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
		EdbTrackP *t = ((EdbTrackDS *) eTracks->At(i))->GetOriginal();
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
	for(int i=0;i<eTracks->GetEntries();i++) hmt[((EdbTrackDS *) eTracks->At(i))->GetOriginal()->N()>=10?10:((EdbTrackDS *) eTracks->At(i))->GetOriginal()->N()]++;
	
	// Daughter cut 2nd.
	if(eTSDau2){
		TObjArray tmp = *eTracks;
		TSDaughterTracks2();
	}
	for(int i=0;i<eTracks->GetEntries();i++) hdau2[((EdbTrackDS *) eTracks->At(i))->N()>=10?10:((EdbTrackDS *) eTracks->At(i))->GetOriginal()->N()]++;

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
	
	
	if( eKinkSearch) KinkSearch();
	if( eShortDecay) ShortDecaySearch();
	
	return eTracks;
}


void EdbDecaySearch::SetVertex (EdbVertex *v) { 
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

TObjArray *EdbDecaySearch::FindUpstreamVertices(){
	
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


TObjArray * EdbDecaySearch::TSDaughterTracks(TObjArray *base){
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
		EdbTrackDS *t2 = FindTrack(t);
		if(t2) t2->SetTrackSearch( t2->TrackSearch()|kDaughterSearch);
		else {
			inew++;
			printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
			t->GetSegmentFirst()->PID(),
			t->GetSegmentLast()->PID(),
			t->TX(),t->TY());
			
			t2 = new EdbTrackDS(t,eVertex);
			t2->SetTrackSearch( t2->TrackSearch()|kDaughterSearch);
			eTracks->Add(t2);
		}
	}
	printf("%5d tracks are selected (new %2d tracks)\n", filtered->GetEntries(), inew);
	return filtered;
}

void EdbDecaySearch::TSDaughterTracks2(){
	// Filter 2segment tracks after microtrack search.
	// sevior dz cut will be applied.
	
	printf(" Daughter track search2. ");
	printf("nseg>=%d, ip=func of dz\n", eTSDauNseg2);
	
	int ntrk=eTracks->GetEntries();
	for(int i=0;i<ntrk;i++){
		EdbTrackDS *tt = (EdbTrackDS *) eTracks->At(i);
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


TObjArray * EdbDecaySearch::TSParentTracks(TObjArray *base){
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
		EdbTrackDS *t2 = FindTrack(t);
		if(t2) t2->SetTrackSearch( t2->TrackSearch()|kParentSearch);
		else {
			inew++;
			printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
				t->GetSegmentFirst()->PID(),
				t->GetSegmentLast()->PID(),
				t->TX(),t->TY());
			EdbTrackDS *t2 = new EdbTrackDS( t, eVertex);
			t2->SetTrackSearch( t2->TrackSearch()|kParentSearch);
			eTracks->Add(t2);
		}
	}
	
	printf("%5d tracks are selected (new %2d tracks)\n", filtered->GetEntries(), inew);
	return filtered;
}


TObjArray * EdbDecaySearch::TSBaseTracks(int pid){
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

		EdbTrackDS *t2 = new EdbTrackDS(t, eVertex);
		t2->SetTrackSearch( t2->TrackSearch()|kBaseTrackSearch);
		eTracks->Add(t2);

		printf("   Track id=%6d ip=%5.1lf pid%02d->%02d %7.4lf %7.4lf\n", t->ID(), ip, 
			t->GetSegmentFirst()->PID(),
			t->GetSegmentLast()->PID(),
			t->TX(),t->TY());
	}
	printf("%5d segments from pid %2d. (%d found, %d already selected as track)\n", filtered->GetEntries(), pid, found, found_as_track);
	return filtered;
}

EdbVertex *EdbDecaySearch::MakeFakeVertex(EdbTrackP *t, double dz){
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

EdbVertex * EdbDecaySearch::FindPrimaryVertex(){
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
	printf("memo: currently mostly 2 track vertex is selected.");
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

void EdbDecaySearch::KinkSearch(){
	printf("------------------------------\n");
	printf("Kink search in selected tracks\n");
	for(int i=0; i<Ntracks(); i++){
		for(int j=0; j<Ntracks(); j++){
			
			if(i==j) continue;
			
			EdbTrackDS *t1 = GetTrack(i);
			EdbTrackDS *t2 = GetTrack(j);
			
			EdbSegP *s1 = t1->GetSegmentLast();
			EdbSegP *s2 = t2->GetSegmentFirst();
			
			if(s1->Plate() >= s2->Plate()) continue;
			// if last segment of t1 is upstream of first segment of t2.
			
			// ignore kink angle < 20 mrad.
			float kinkangle = sqrt((s1->TX()-s2->TX())*(s1->TX()-s2->TX())+(s1->TY()-s2->TY())*(s1->TY()-s2->TY()));
			if( kinkangle<0.02) continue;
			
			// remove very low momentum
			double rmss, rmst, rmsl;
			DTRMSTL(t2, &rmss, &rmst, &rmsl);
			if( rmst>0.015 && rmsl>0.015) continue;
			
			TObjArray segs;
			segs.Add(s1);
			segs.Add(s2);
			EdbVertex *v = EdbEDAUtil::CalcVertex(&segs);
			float ip1 = EdbEDAUtil::CalcIP(s1, v);
			float ip2 = EdbEDAUtil::CalcIP(s2, v);
			
			// cut with IP and Z. (Z should between 2 segments.)
			if(ip1<20 && ip2<20 && s1->Z()-500<v->Z() && v->Z()<s2->Z()+500){
				printf("Decay vertex t1 %d t2 %d ip1 %f ip2 %f\n", t1->ID(), t2->ID(), ip1, ip2);
				EdbDecayVertex *dv = new EdbDecayVertex;
				dv->SetXYZ( v->X(), v->Y(), v->Z());
				dv->SetParent(t1);
				dv->SetDaughter(t2);
				dv->SetType( EdbDecayVertex::kLong);
				printf("kink found, %d %f %f %f\n", dv->N(), dv->X(), dv->Y(), dv->Z());
				eDecayVertices->Add(dv);
			}
			
			delete v;
		}
	}
}

float EdbDecaySearch::GetIPCut(EdbVertex *v, EdbSegP *s){
	// calculate IP cut for given 1ry vertex and segment.
	// if segment is BT, Z of segment is assumed Z=s->Z()-150;
	// otherwise, use s->Z();
	
	float Z = s->Side()==0? s->Z()-150 : s->Z();
	float dZ = fabs( Z-v->Z());
	return 5.0+0.01*dZ;
}
	
	

void EdbDecaySearch::ShortDecaySearch(){
	printf(" --------------------------\n");
	printf("// Short decay search    //\n");
	printf("-------------------------- \n");
	printf("Using tracks which start from vertex plate and Nseg>=3\n");
	printf("TODO: Microtrack search!!\n");
	
	// select tracks in vertex plate
	TObjArray tracks;
	for(int i=0; i<Ntracks(); i++){
		EdbTrackDS *t = GetTrack(i);
		EdbSegP *s1 = t->GetSegmentFirst();
		
		// Nseg cut
		if(t->N()<=2) continue;
		
		// use only tracks which start from vertex plate.
		if(s1->PID()!=eVertexPlatePID) continue;
		
		tracks.Add(t);
	}
	printf("Tracks selection for short decay : %d -> %d\n", Ntracks(), tracks.GetEntries());
	
	if(tracks.GetEntries()==0) return;
	
	// Calculate vertex with all tracks.  First time.
	TObjArray segs;
	for(int i=0; i<tracks.GetEntries(); i++) segs.Add( ((EdbTrackDS *)tracks.At(i))->GetSegmentFirst());
	EdbVertex *v = CalcVertex(&segs);
	float ipcut = GetIPCut(v, (EdbSegP *)segs.At(0));
	printf("Vertex with all tracks: %.1f %.1f %.1f\n", v->X(), v->Y(), v->Z());
	printf("dZ=%.1f microns -> IP cut %.1f\n", ((EdbSegP *)segs.At(0))->Z()-v->Z(), ipcut);
	
	int flag=0;
	// check if short decay candidate exists or not.
	
	if(tracks.GetEntries()==2){ // 2 track case.
		EdbTrackDS *t1 = (EdbTrackDS *) tracks.At(0);
		EdbSegP *s1 = t1->GetSegmentFirst();
		EdbTrackDS *t2 = (EdbTrackDS *) tracks.At(1);
		EdbSegP *s2 = t2->GetSegmentFirst();
		float ip = CalcDmin(s1,s2);
		printf("track %5d x track %5d, dmin %4.1f %s\n", t1->ID(), t2->ID(), ip, ip>ipcut?">ipcut":"");
		if(ip>ipcut) flag=2;
	}
	else { // more tracks case
		float ipmean=0.0;
		for(int i=0; i<tracks.GetEntries(); i++){
			EdbTrackDS *t = (EdbTrackDS *) tracks.At(i);
			EdbSegP *s = t->GetSegmentFirst();
			float ip = CalcIP( s, v);
			
			printf("track %5d (%7.4f, %7.4f) ip %4.1f %s\n", t->ID(), t->TX(), t->TY(), ip, ip>ipcut?">ipcut":"");
			ipmean +=ip;
			if(ip>ipcut) flag++;
		}
		ipmean /= tracks.GetEntries();
		printf("IP mean %.1lf microns with %d tracks\n", ipmean, tracks.GetEntries());
	}
	if(flag==0) {
		printf("No short decay. stop.\n");
		return;
	}
	printf("%d short decay candidates\n", flag);
	
	printf("Vertex refinement by removing one or more tracks.\n");
	
	while(flag&&tracks.GetEntries()!=2){
		// loop to make vertex removing one track.
		int imax=-1;
		float ipmax = 0;
		for(int i=0; i<tracks.GetEntries(); i++){
			EdbTrackDS *t = (EdbTrackDS *) tracks.At(i);
			EdbSegP *s = t->GetSegmentFirst();
			
			TObjArray segs2 = segs;
			segs2.Remove( s);
			segs2.Sort();
			
			EdbVertex *v2 = CalcVertex(&segs2);
			float ip = CalcIP(s, v2);
			
			if(ip>ipmax) {
				imax=i;
				ipmax=ip;
			}
		}
		
		// if ipmax>ipcut.
		if(ipmax>ipcut) {
			EdbTrackDS *tsd = (EdbTrackDS *) tracks.At(imax);
			
			// Calculate new vertex position.
			tracks.Remove(tsd);
			tracks.Sort();
			segs.Clear();
			for(int i=0; i<tracks.GetEntries(); i++) segs.Add( ((EdbTrackDS *)tracks.At(i))->GetSegmentFirst());
			EdbVertex *v2 = CalcVertex(&segs);
			
			// Set as Primary vertex.
			eVertex = v2;
			
			// remove very low momentum
			double rmss, rmst, rmsl;
			DTRMSTL(tsd, &rmss, &rmst, &rmsl);
			if( rmst>0.015 && rmsl>0.015) continue;
			
			// Make Short Decay vertex
			EdbVertex *vsd = MakeFakeVertex( tsd, -150);
			EdbDecayVertex *vdecay = new EdbDecayVertex;
			vdecay->SetXYZ( vsd->X(), vsd->Y(), vsd->Z());
			vdecay->SetPrimaryVertex(v2);
			vdecay->SetDaughter( tsd);
			vdecay->SetType( EdbDecayVertex::kShort);

			eDecayVertices->Add(vdecay);
			
			
			printf("Short decay of track %d is found. ip = %5.1f.  \n", tsd->ID(), ipmax);
			
			// Check if no other tracks has ip>ipcut
			flag=0;
			if(tracks.GetEntries()==2){
				EdbTrackDS *t1 = (EdbTrackDS *) tracks.At(0);
				EdbSegP *s1 = t1->GetSegmentFirst();
				EdbTrackDS *t2 = (EdbTrackDS *) tracks.At(1);
				EdbSegP *s2 = t2->GetSegmentFirst();
				float ip = CalcDmin(s1,s2);
				printf("track %5d x track %5d, dmin %4.1f %s\n", t1->ID(), t2->ID(), ip, ip>ipcut?">ipcut":"");
				if(ip>ipcut) flag=2;
			}
			else {
				float ipmean=0.0;
				for(int i=0; i<segs.GetEntries(); i++){
					EdbSegP *s = (EdbSegP *) segs.At(i);
					float ip = CalcIP(s, v2);
					if(ip>ipcut) flag++;
					ipmean += ip;
				}
				ipmean /= segs.GetEntries();
				printf("IP mean %.1lf microns with %d tracks, removing track %d\n",  ipmean, tracks.GetEntries(), tsd->ID());
			}
		} else flag=0;
		
	}
	
	
	if( flag && tracks.GetEntries()==2){
		// Short decay with only 1 partner track. (without vertex)
		for(int i=0; i<2; i++){
			EdbTrackDS *t1 = (EdbTrackDS *) tracks.At(i);
			EdbSegP *s1 = t1->GetSegmentFirst();
			EdbTrackDS *t2 = (EdbTrackDS *) tracks.At(!i);
			EdbSegP *s2 = t2->GetSegmentFirst();
			
			// remove very low momentum
			double rmss, rmst, rmsl;
			DTRMSTL(t1, &rmss, &rmst, &rmsl);
			if( rmst>0.015 && rmsl>0.015) continue;
			
			EdbVertex *vsd = MakeFakeVertex( t1, -150);
			EdbDecayVertex *vdecay = new EdbDecayVertex;
			vdecay->SetXYZ( vsd->X(), vsd->Y(), vsd->Z());
			vdecay->SetDaughter( t1);
			vdecay->SetPartner( t2);
			vdecay->SetType(EdbDecayVertex::kShort);
			eDecayVertices->Add(vdecay);
			
		}
	}
	
	for(int i=0; i<NDecayVertices(); i++){
		EdbDecayVertex *v = GetDecayVertex(i);
		if(v->GetPrimaryVertex()) v->SetPrimaryVertex(eVertex);
	}
}

