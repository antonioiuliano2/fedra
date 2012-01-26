#include"EdbEDA.h"
#include<TF1.h>
extern EdbEDA *gEDA;

using namespace EdbEDAUtil;

void EdbEDAPlotTab::CheckEff(EdbPVRec *pvr, TObjArray *tracks){
	// Plot efficiencies for each plate or each angle.
	// Using the tracks in the given PVRec object.
	// Use minimum number of segment bigger equal than eEffMinSeg(default=4).
	printf("Efficiency plot\n");
	int i,j,k;

	TCanvas *c1 = CreateCanvas("Efficiency");
	c1->Divide(2,2);
	c1->cd(1);

	if(NULL==pvr) {
		EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
		pvr = set->GetPVRec();
	}
	
	if(NULL==tracks) tracks=pvr->eTracks;
	
	// Efficiency plate by plate
	int pidmax = pvr->Npatterns()-1;
	int pidmin = 0;
	
	int plmax=0, plmin=1000;
	
	for(i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;

		EdbSegP *s = pat->GetSegment(0);
		if(s==NULL) continue;
		if(s->Plate()<plmin) plmin=s->Plate();
		if(s->Plate()>plmax) plmax=s->Plate();
	}
	
	if(plmax<plmin) { int tmp=plmax; plmax=plmin; plmin=tmp;} // if order of pid and ipl were opposite.

	int npl = plmax-plmin+1;
	
//	EdbEDAAreaSet *areas = gEDA->GetAreaSet();

	TH1D *hentry = new TH1D("hentry",Form("Efficiency (#theta < %.2f rad)", eEffTmax<0.5? eEffTmax : 0.5), npl, plmin-0.5, plmax+0.5);
	TH1D *hfound = new TH1D("hfound","Found",								npl, plmin-0.5, plmax+0.5);
	TH1D *heff   = new TH1D("heff",  Form("Efficiency (#theta < %.2f rad)", eEffTmax<0.5? eEffTmax : 0.5), npl, plmin-0.5, plmax+0.5);
	
		// Efficiency for each angle
	TH1D *haentry = new TH1D("haentry","Entry and Found for each angle",eEffNbins, 0,eEffTmax);
	TH1D *hafound = new TH1D("hafound","Found"                         ,eEffNbins, 0,eEffTmax);
	TH1D *haeff   = new TH1D("haeff","Efficiency for each angle"       ,eEffNbins, 0,eEffTmax);

	
	
	EdbEDAAreaSet AreaSet;
	for(int pid=pidmin; pid<=pidmax; pid++){ // pid <= target pid
		EdbPattern *pat = pvr->GetPattern(pid);
		AreaSet.AddArea(pid, pat->Xmin(), pat->Xmax(), pat->Ymin(), pat->Ymax(), pat->Z()); // put PID instead IPL
	}
	
	
	int ntrk = tracks->GetEntries();
	for(i=0;i<ntrk;i++){
		EdbTrackP *t = (EdbTrackP *) tracks->At(i);
		if(t==NULL) continue;
		int nseg = t->N();
		if(nseg<eEffMinSeg) continue;
		double angle = sqrt(t->TX()*t->TX() + t->TY()*t->TY());
		if(angle>eEffTmax) continue;
		
		for(int pid=pidmin; pid<=pidmax; pid++){ // pid <= target pid
			EdbPattern *pat = pvr->GetPattern(pid);
			EdbEDAArea *a = AreaSet.GetAreaIPL(pid);
			if(a==NULL) continue;
			
			EdbSegP *s1, *s2, *s3, *s0,*s4;
			s1=s2=s3=s0=s4=NULL;
			for(j=0;j<nseg;j++){
				EdbSegP *s = t->GetSegment(j);
				if(pid-2==s->PID()) s0=s; // to calculate eff for the last plate
				if(pid-1==s->PID()) s1=s;
				if(pid  ==s->PID()) s2=s;
				if(pid+1==s->PID()) s3=s;
				if(pid+2==s->PID()) s4=s; // to calculate eff for the 1st plate
			}
			
			if( nseg - (s2?1:0) < eEffMinSeg) continue; // more than 4 segments except the target pid.
			EdbSegP *ss=NULL;
			if(pid==pidmin){
				if(s3==NULL||s4==NULL) continue;
				ss=s3;
			}
			else if(pid==pidmax){
				if(s0==NULL||s1==NULL) continue;
				ss=s1;
			}
			else {
				if(s1==NULL||s3==NULL) continue;  // request existence of nearest segments.	
				ss=s1;
			}
			// if the predicted point from ss is out of volume, skip.
			double z = a->Z();
			EdbSegP *s = new EdbSegP(*ss);
			s->PropagateTo(z);
			if( s->X()<a->Xmin()) continue;
			if( s->X()>a->Xmax()) continue;
			if( s->Y()<a->Ymin()) continue;
			if( s->Y()>a->Ymax()) continue;
			delete s;

		/*	
			// if the predicted point from ss is out of volume, skip.
			EdbEDAArea *a = areas->GetArea(pid);
			if(a!=NULL){
				if(a->Plate()==gEDA->GetIPL(pid)){
					double z = gEDA->GetZPID(pid);
					EdbSegP *s = new EdbSegP(*ss);
					s->PropagateTo(z);
					if( s->X()<a->Xmin()) continue;
					if( s->X()>a->Xmax()) continue;
					if( s->Y()<a->Ymin()) continue;
					if( s->Y()>a->Ymax()) continue;
					delete s;
				}
			}
		*/	
			// eff by angle
			haentry->Fill(angle);
			if(s2) hafound->Fill(angle);
			if(s2) haeff->Fill(angle);

			// eff by pid
			if(angle > (eEffTmax<0.5? eEffTmax : 0.5) ) continue;
			int ipl = pat->GetSegment(0)->Plate();
			hentry->Fill       (ipl);
			if(s2) hfound->Fill(ipl);
			if(s2) heff->Fill  (ipl);
			
		}
	}
	
	
	hentry->SetStats(0);
	hentry->SetMinimum(0);
	hentry->Draw();
	hentry->SetXTitle("Plate number");
	hentry->GetXaxis()->SetNdivisions(10);
	hfound->Draw("same");
	hfound->SetFillColor(15);
	
	c1->cd(2);

	// Efficiency plot
	// error
	double *error_arr = new double [npl+2];
	double *entry_arr = hentry->GetArray();
	double *found_arr = hfound->GetArray();
	//error_arr[1]=error_arr[npl]=0.0;
	for(i=1;i<=npl;i++){
		if(entry_arr[i]==0) {error_arr[i]=0.0; continue;}
		error_arr[i] = sqrt( (double) found_arr[i] * (entry_arr[i]-found_arr[i]) / entry_arr[i])  / entry_arr[i];
	}
	// eff mean
	double mean=0.0;
	k=0;
	for(i=1;i<=npl;i++){
		if(entry_arr[i]==0) continue;
		mean+=(double) found_arr[i]/entry_arr[i];
		k++;
	}
	mean/=k;
	// eff plot
	heff->Divide(hentry);
	heff->SetMaximum(1.02);
	heff->SetMinimum(0.0);
	heff->SetError(error_arr);
	heff->SetStats(0);
	heff->SetXTitle("Plate number");
	heff->Draw();
	TLine *l = new TLine;
	l->SetLineColor(kRed);
	l->DrawLine(plmin,mean,plmax,mean);
	TText *tx = new TText;
	tx->SetTextSize(0.045);
	tx->DrawText(plmax-0.5,mean,Form("%.2lf",mean));
	heff->Draw("same");
	
	// print text
	printf("Efficiencies for each plates\n");
	for(i=1;i<=npl;i++){
		printf("PL %2d : %5.3lf +- %5.3lf\n", plmin+i-1, (double) found_arr[i]/entry_arr[i], error_arr[i]);
	}


	c1->cd(3);
	
	haentry->SetStats(0);
	haentry->SetMinimum(0);
	haentry->Draw();
	haentry->SetXTitle("abs Angle (rad)");
	haentry->GetXaxis()->SetNdivisions(10);
	hafound->Draw("same");
	hafound->SetFillColor(15);

	c1->cd(4);

	
	// Efficiency plot
	// error 
	error_arr = new double [eEffNbins+2];
	entry_arr = haentry->GetArray();
	found_arr = hafound->GetArray();
	for(i=0;i<eEffNbins+2;i++){
		error_arr[i]=0.0;
	}
	for(i=1;i<eEffNbins+1;i++){
		if(entry_arr[i]==0) {error_arr[i]=0.0; continue;}
		error_arr[i] = sqrt( (double) found_arr[i] * (entry_arr[i]-found_arr[i]) / entry_arr[i])  / entry_arr[i];
	}
	// eff mean
	mean=0.0;
	k=0;
	printf("Efficiencies for each angle\n");
	for(i=1;i<eEffNbins+1;i++){
		if(entry_arr[i]==0) continue;
		mean+=(double) found_arr[i]/entry_arr[i];
		k++;
		printf("%.3lf < theta < %.3lf : %5.3lf +- %5.3lf\n", (i-1)*eEffTmax/eEffNbins, i*eEffTmax/eEffNbins, found_arr[i]/entry_arr[i], error_arr[i]);
	}
	mean/=k;
	// eff plot
	haeff->Divide(haentry);
	haeff->SetStats(0);
	haeff->SetMaximum(1.02);
	haeff->SetMinimum(0);
	haeff->SetError(error_arr);
	haeff->SetXTitle("Angle (rad)");
	haeff->Draw();
	l->SetLineColor(kRed);
	l->DrawLine(0,mean,eEffTmax,mean);
	tx->SetTextSize(0.045);
	tx->DrawText(eEffTmax-0.05,mean,Form("%.2lf",mean));
	haeff->Draw("same");

	c1->Update();
}










/////////////////////////////////////////////////////////////////////////////
void EdbEDAPlotTab::CheckPHDAngle(EdbPVRec *pvr){
	int i,j;

	TCanvas *c1 = CreateCanvas("PH-dAngle");

	if(NULL==pvr) {
		EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
		pvr = set->GetPVRec();
	}

	// Efficiency plate by plate
	
	int plmax=0, plmin=1000;
	
	for(i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;
		EdbSegP *s = pat->GetSegment(0);
		if(s==NULL) continue;
		if(s->Plate()<plmin) plmin=s->Plate();
		if(s->Plate()>plmax) plmax=s->Plate();
	}

	if(plmax<plmin) { int tmp=plmax; plmax=plmin; plmin=tmp;} // if order of pid and ipl were opposite.
	int npl = plmax-plmin+1;
	
	int nsegcut = gEDA->GetMainTab()->GetNsegCut();
	
	double *dangle=new double[2*npl];
	
	TNtuple *nt = new TNtuple ("ntqualities","TNtuple for quality plots","nseg:ph:rms");
	for(i=0; i<pvr->Ntracks(); i++){
		EdbTrackP *t = pvr->GetTrack(i);
		double ph = t->Wgrains()/t->N();
		int nseg=t->N();

		for(j=0;j<nseg-1;j++){
			EdbSegP *s1 = t->GetSegment(j);
			EdbSegP *s2 = t->GetSegment(j+1);
			dangle[2*j]   = s1->TX() - s2->TX();
			dangle[2*j+1] = s1->TY() - s2->TY();
		}
		double rms = DTRMS(t);
		
		nt->Fill(nseg,ph,rms);
	}
	
	TH1D *hphall   = new TH1D("hphall",   "PH mean = #SigmaPH/nseg",  20, 12.5,32.5);
	hphall->SetXTitle("PH mean");
	nt->Draw("ph >>hphall");
	TH1D *hphnseg2 = new TH1D("hphnseg2", "PH, Nseg=2",  20, 12.5,32.5);
	hphnseg2->SetLineColor(2);
	nt->Draw("ph >>hphnseg2","nseg==2");
	TH1D *hphnsegcut = new TH1D("hphnsegcut", Form("PH, Nseg>=%d", nsegcut), 20, 12.5,32.5);
	hphnsegcut->SetLineColor(kBlue);
	nt->Draw("ph >>hphnsegcut",Form("nseg>=%d", nsegcut));
	
	TH1D *hdtrmsall = new TH1D("hdtrmsall", "Angular deviation", 50, 0, 0.1);
	hdtrmsall->SetXTitle("deviation RMS (rad)");
	nt->Draw("rms >>hdtrmsall");
	TH1D *hdtrmsnseg2 = new TH1D("hdtrmsnseg2", "Angular deviation, Nseg==2", 50, 0, 0.1);
	hdtrmsnseg2->SetLineColor(2);
	nt->Draw("rms >>hdtrmsnseg2","nseg==2");
	TH1D *hdtrmsnsegcut = new TH1D("hdtrmsnsegcut", Form("Angular deviation, Nseg>=%d", nsegcut), 50, 0, 0.1);
	hdtrmsnsegcut->SetLineColor(kBlue);
	nt->Draw("rms >>hdtrmsnsegcut",Form("nseg>=%d", nsegcut));
	
	TH2I *hphrmsall = new TH2I("hphrmsall","PH-#delta#theta^{RMS}", 20, 12.5,32.5, 50, 0, 0.1);
	nt->Draw("rms:ph >>hphrmsall");
	hphrmsall->SetXTitle("PH mean");
	hphrmsall->SetYTitle("#delta#theta^{RMS} rad");

	TH2I *hphrmscut = new TH2I("hphrmscut",Form("PH-#delta#theta^{RMS}, nseg>=%d", nsegcut), 20, 12.5,32.5, 50, 0, 0.1);
	nt->Draw("rms:ph >>hphrmscut",Form("nseg>=%d",nsegcut));
	hphrmscut->SetXTitle("PH mean");
	hphrmscut->SetYTitle("#delta#theta^{RMS} rad");
	
	c1->Clear();
	c1->Divide(2,2);
	c1->cd(1);
	hphall->Draw();
	hphnsegcut->Draw("same");
	hphnseg2->Draw("same");

	TLegend *leg = new TLegend(0.5,0.85, 0.75, 0.99);
	leg->AddEntry(hphall, "PH, all", "f");
	leg->AddEntry(hphnseg2, "PH, nseg==2", "f");
	leg->AddEntry(hphnsegcut, Form("PH, nseg>=%d", nsegcut), "f");
	leg->Draw();
	
	c1->cd(2);
	hdtrmsall->Draw();
	hdtrmsnsegcut->Draw("same");
	hdtrmsnseg2->Draw("same");
	leg->Draw();



	c1->cd(3);

	double a = gEDA->GetMainTab()->GetPHDTRMS();
	double b = gEDA->GetMainTab()->GetPHCut();
	TF1 *func = new TF1("fphdtrms", Form("%lf*(x-%lf)", a,b), 12.5, 32.5);
	hphrmsall->Draw("col");
	func->Draw("same");
	TPaveText *pave = new TPaveText(0.5,0.88, 0.75, 0.99,"brNDC");
	int nselected=nt->GetEntries(Form("rms<%lf*(ph-%lf)", a, b));
	int nrejected=nt->GetEntries(Form("rms>=%lf*(ph-%lf)", a, b));
	pave->AddText(Form("%5d selected",nselected));
	pave->AddText(Form("%5d rejected",nrejected));
	pave->Draw();
	
	
	c1->cd(4);
	hphrmscut->Draw("col");
	func->Draw("same");
	pave = new TPaveText(0.5,0.88, 0.75, 0.99,"brNDC");
	nselected=nt->GetEntries(Form("nseg>=%d&&rms<%lf*(ph-%lf)", nsegcut, a, b));
	nrejected=nt->GetEntries(Form("nseg>=%d&&rms>=%lf*(ph-%lf)", nsegcut, a, b));
	pave->AddText(Form("%5d selected",nselected));
	pave->AddText(Form("%5d rejected",nrejected));
	pave->Draw();


}

void EdbEDAPlotTab::CheckAlignment(EdbPVRec *pvr){
	int i,j;

	TCanvas *c1 = CreateCanvas("Alignment");
	c1->Divide(2,2);
	c1->cd(1);

	if(NULL==pvr) {
		EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
		pvr = set->GetPVRec();
	}

	// Efficiency plate by plate
	int pidmax = pvr->Npatterns()-1;
	int pidmin = 0;
	
	int plmax=0, plmin=1000;
	
	for(i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;
		EdbSegP *s = pat->GetSegment(0);
		if(s==NULL) continue;
		if(s->Plate()<plmin) plmin=s->Plate();
		if(s->Plate()>plmax) plmax=s->Plate();
	}

	if(plmax<plmin) { int tmp=plmax; plmax=plmin; plmin=tmp;} // if order of pid and ipl were opposite.

	int npl = plmax-plmin+1;

	int pid;
	
	int nsegcut = gEDA->GetMainTab()->GetNsegCut();

	
	TNtuple *nt = new TNtuple("ntali", "alignment", "itrk:ipl1:ipl2:ax:ay:x:y:dx:dy:dax:day");
	
	for(i=0;i<pvr->Ntracks();i++){
		EdbTrackP *t = pvr->GetTrack(i);
		if(t==NULL) continue;
		if(npl>2&&t->N()<3) continue;
		if(t->N()<nsegcut) continue;
		//EdbSegP *s1st = t->GetSegmentFirst();
		//EdbSegP *slst = t->GetSegmentLast();
		
		for(pid=pidmin; pid<pidmax; pid++){
			EdbSegP *s1=0,*s2=0;
			for(j=0;j<t->N();j++){
				EdbSegP *s = t->GetSegment(j);
				if(pid==s->PID())   s1=s;
				if(pid+1==s->PID()) s2=s;
			}
			if(s1&&s2) {
				double dz = (s2->Z()-s1->Z())/2;
				double dx = s1->X()+s1->TX()*dz - ( s2->X()-s2->TX()*dz );
				double dy = s1->Y()+s1->TY()*dz - ( s2->Y()-s2->TY()*dz );
				double dax = s1->TX() - s2->TX();
				double day = s1->TY() - s2->TY();
				nt->Fill(t->ID(), s1->Plate(), s2->Plate(), t->TX(), t->TY(), s1->X(), s1->Y(),dx,dy,dax,day);
			}
		}
	}
	
	TProfile *profx  = new TProfile("profalix",   "#deltax Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1,"s");
	TProfile *profx2 = new TProfile("profalix2",  "#deltax Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1);
	TProfile *profy  = new TProfile("profaliy",   "#deltay Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1,"s");
	TProfile *profy2 = new TProfile("profaliy2",  "#deltax Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1);
	TProfile *profax = new TProfile("profaliax",  "#delta#thetax Profile, bar = Error(black) RMS(gray)", npl+1, plmin-1, plmax+1,"s");
	TProfile *profax2= new TProfile("profaliax2", "#deltax Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1);
	TProfile *profay = new TProfile("profaliay",  "#delta#thetay Profile, bar = Error(black) RMS(gray)", npl+1, plmin-1, plmax+1,"s");
	TProfile *profay2= new TProfile("profaliay2", "#deltax Profile, bar = Error(black) RMS(gray)",       npl+1, plmin-1, plmax+1);
	
	c1->cd(1);
	nt->Draw("dx:(ipl1+ipl2)/2 >>profalix2");
	nt->Draw("dx:(ipl1+ipl2)/2 >>profalix");
	profx->SetMinimum(-15);
	profx->SetMaximum(15);
	profx->SetLineColor(kGray);
	profx->SetXTitle("Plate number");
	profx->SetYTitle("#mum");
	profx2->Draw("same");
	
	c1->cd(2);
	nt->Draw("dy:(ipl1+ipl2)/2 >>profaliy2");
	nt->Draw("dy:(ipl1+ipl2)/2 >>profaliy");
	profy->SetMinimum(-15);
	profy->SetMaximum(15);
	profy->SetLineColor(kGray);
	profy->SetXTitle("Plate number");
	profy->SetYTitle("#mum");
	profy2->Draw("same");

	c1->cd(3);
	nt->Draw("dax:(ipl1+ipl2)/2 >>profaliax2");
	nt->Draw("dax:(ipl1+ipl2)/2 >>profaliax");
	profax->SetMinimum(-0.020);
	profax->SetMaximum(0.020);
	profax->SetLineColor(kGray);
	profax->SetXTitle("Plate number");
	profax->SetYTitle("rad");
	profax2->Draw("same");

	c1->cd(4);
	nt->Draw("day:(ipl1+ipl2)/2 >>profaliay2");
	nt->Draw("day:(ipl1+ipl2)/2 >>profaliay");
	profay->SetMinimum(-0.020);
	profay->SetMaximum(0.020);
	profay->SetLineColor(kGray);
	profay->SetXTitle("Plate number");
	profay->SetYTitle("rad");
	profay2->Draw("same");
	
	
	printf("Alignments. nseg>=%d\n", nsegcut);
	printf("%5s %6s %6s ", "pl","dxcent","dxrms");
	printf("%6s %6s ", "dycent","dyrms");
	printf("%8s %8s ", "daxcent","daxrms");
	printf("%8s %8s \n", "daycent","dayrms");
	for(i=2;i<npl+1;i++){
		printf("%5.1lf ", profx->GetBinCenter(i));
		printf("%6.2lf %6.2lf ", profx->GetBinContent(i), profx->GetBinError(i));
		printf("%6.2lf %6.2lf ", profy->GetBinContent(i), profy->GetBinError(i));
		printf("%8.5lf %8.5lf ", profax->GetBinContent(i), profax->GetBinError(i));
		printf("%8.5lf %8.5lf \n", profay->GetBinContent(i), profay->GetBinError(i));
	}
	
}


void EdbEDAPlotTab::CheckOverview(EdbPVRec *pvr){
	if(NULL==pvr) {
		EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
		pvr = set->GetPVRec();
	}

	// Efficiency plate by plate
	
	int plmax=0, plmin=1000;
	
	for(int i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;
		EdbSegP *s = pat->GetSegment(0);
		if(s==NULL) continue;
		if(s->Plate()<plmin) plmin=s->Plate();
		if(s->Plate()>plmax) plmax=s->Plate();
	}

	if(plmax<plmin) { int tmp=plmax; plmax=plmin; plmin=tmp;} // if order of pid and ipl were opposite.

	int npl = plmax-plmin+1;

	int nsegcut = gEDA->GetMainTab()->GetNsegCut();

	// definition of histgrams
	TH2I *hangle = new TH2I("hangle",Form("Angular distribution nseg>=%d",nsegcut), 60,-0.6,0.6, 60,-0.6,0.6);
	hangle->SetXTitle("#thetax [rad]");
	hangle->SetYTitle("#thetay [rad]");
	
	TH1I *hnseg = new TH1I("hnseg","Nseg", npl, 0.5, npl+0.5);
	hnseg->SetXTitle("N segments");

	TH1I *hph = new TH1I("hph",Form("PH, nseg>=%d",nsegcut), 20, 12.5,32.5);
	hph->SetXTitle("PH");

	TH1I *hphgt4 = new TH1I("hphgt4","PH, nseg>=4", 20, 12.5,32.5);
	hphgt4->SetXTitle("PH, nseg>=4");
	hphgt4->SetLineColor(kBlue);

	TH1I *hphmeangt4 = new TH1I("hphmeangt4","PH, nseg>=4", 20, 12.5,32.5);
	hphmeangt4->SetXTitle("PH mean, nseg>=4");
	hphmeangt4->SetLineColor(kBlue);

	TH1I *h1stpl = new TH1I("h1stpl",Form("1st plate, nseg>=%d",nsegcut), npl, plmin-0.5, plmax+0.5);
	h1stpl->SetLineColor(kBlue);
	h1stpl->SetXTitle("First plate");

	TH1I *hlastpl = new TH1I("hlastpl",Form("last plate, nseg>=%d",nsegcut), npl, plmin-0.5, plmax+0.5);
	hlastpl->SetXTitle("Last plate");

	for(int i=0;i<pvr->Ntracks();i++){
		EdbTrackP *t = pvr->GetTrack(i);
		if(t==NULL) continue;
		
		double ph = t->Wgrains()/t->N();

		int nseg = t->N();
		hnseg->Fill(nseg);
		if(nseg>=nsegcut){
			hangle->Fill(t->TX(),t->TY());
			h1stpl->Fill((t->GetSegmentFirst())->Plate());
			hlastpl->Fill((t->GetSegmentLast())->Plate());
		}
		if(nseg>=4) hphmeangt4->Fill(ph);
		
		for(int j=0;j<t->N();j++){
			if(nseg>=nsegcut)hph->Fill(t->GetSegment(j)->W());
			if(t->N()>=4) hphgt4->Fill(t->GetSegment(j)->W());
		}
	}
	
	// drawing
	TCanvas *c1=CreateCanvas("Overview");
	c1->Divide(2,2);
	
	c1->cd(1);
	hangle->Draw("colz");
	
	c1->cd(2);
	hnseg->Draw();
	
	c1->cd(3);

	int max=(int)hph->GetMaximum();
	if( max < hphgt4->GetMaximum()) max= (int)hphgt4->GetMaximum();
	hph->SetMaximum(max*1.1);
	hph->Draw();
	hphgt4->Draw("same");
	hphmeangt4->SetFillColor(kYellow);
	hphmeangt4->Draw("same");
	TLegend *leg = new TLegend(0.12,0.70, 0.40, 0.88);
	leg->AddEntry(hph, Form("PH, nseg>=%d",nsegcut), "l");
	leg->AddEntry(hphgt4, "PH, nseg#geq4", "l");
	leg->AddEntry(hphmeangt4, "PH mean, nseg#geq4", "lf");
	
	leg->Draw();
	
	c1->cd(4);
	hlastpl->SetTitle("Start and End plate");
	hlastpl->Draw();
	h1stpl->Draw("same");
	leg = new TLegend(0.55,0.6, 0.75, 0.70);
	leg->AddEntry(h1stpl, "First plate", "l");
	leg->AddEntry(hlastpl, "Last plate", "l");
	leg->Draw();
	
	c1->Update();
}

void EdbEDAPlotTab::CheckTracks(){
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	if(selected_tracks->GetEntries()==0) return;

	for(int i=0;i<selected_tracks->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *) selected_tracks->At(i);
		if(t==NULL) continue;
		CheckSingleTrack(t);
	}
}

void EdbEDAPlotTab::CheckSingleTrack(EdbTrackP *t){
	
	printf("CheckSingleTrack : Make a plot for a track.\n");
	// copy of EdbTrackP
	EdbTrackP *t2 = new EdbTrackP;
	t2->Copy(*t);
	// first and last segment
	EdbSegP *s1st = t->GetSegmentFirst();
	EdbSegP *slast= t->GetSegmentLast();
	
	// momentum
	double p,pmin,pmax;
	CalcP(t,p,pmin,pmax);
	// trajectries
	TNtuple *nt = new TNtuple(Form("nt%d",t->ID()),Form("Track %d",t->ID()),"eTrack:eID:eW:ipl:eX:eY:eZ:eTX:eTY:dX:dY:eSide");
	double W[60];
	for(int i=0;i<t->N();i++){
		EdbSegP *s = t->GetSegment(i);
		W[i] = s->W();
		t2->PropagateTo(s->Z());
		double dx = s->X()-t2->X();
		double dy = s->Y()-t2->Y();
		
		nt->Fill(t->ID(),s->ID(),s->W(),s->Plate(), s->X(),s->Y(),s->Z(),s->TX(),s->TY(),dx,dy, s->Side());
		
	}
	double Wmean = TMath::Mean(t->N(),W);
	double Wrms = TMath::RMS(t->N(),W);
	
	// cosmic eff and PH calculation.
	double wcr=0.0, effcr=0.0;
	int wcrcnt=0, effcrcnt=0;
	
	EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
	EdbPVRec *pvr = set->GetPVRec();
	
	if(pvr!=NULL){
		printf("Calculate the reference efficiency from TS (cosmic rays).");

		int plmax=0, plmin=1000;
		for(int i=0;i<pvr->Npatterns();i++){
			EdbPattern *pat = pvr->GetPattern(i);
			if(pat==NULL) continue;
			EdbSegP *s = pat->GetSegment(0);
			if(s==NULL) continue;
			if(s->Plate()<plmin) plmin=s->Plate();
			if(s->Plate()>plmax) plmax=s->Plate();
		}
		
		int trklencut = abs(plmax-plmin);
		trklencut/=2;
		trklencut= (int) (trklencut*(1-sqrt(t->TX()*t->TX()+t->TY()*t->TY())));
		if(trklencut<8) trklencut=8;
		if(trklencut>20) trklencut=20;
		printf(" Track length cut for cosmic ray = %d, dtheta<%.3lfrad\n", trklencut,eDTReference);
		for(int i=0;i<set->NBase();i++){
			EdbTrackP *tc = set->GetTrackBase(i);
			if(tc==NULL) continue;
			// track length cut
			int iplF = tc->GetSegmentFirst()->Plate();
			int iplL = tc->GetSegmentLast()->Plate();
			int trklen = iplL-iplF+1;
			if(trklen<trklencut) continue;
			
			// angular cut
			if( fabs(t->TX()-tc->TX())>eDTReference) continue;
			if( fabs(t->TY()-tc->TY())>eDTReference) continue;
			
			// efficiency
			printf(" %2d->%2d, %2d/%2d = %.2lf\n", iplF, iplL, tc->N()-2, trklen-2, (double) (tc->N()-2)/(trklen-2));
			effcr += (double) (tc->N()-2)/(trklen-2);
			effcrcnt++;
			
			// W
			for(int j=0;j<tc->N();j++){
				EdbSegP *s = tc->GetSegment(j);
				wcr+=s->W();
				wcrcnt++;
			}
			
		}
		wcr   = wcrcnt==0   ? 0.0 : wcr/wcrcnt;
		effcr = effcrcnt==0 ? 0.0 : effcr/effcrcnt;
	}
	
	EdbTrackP *tsb = gEDA->CheckScanback(t);
	int sbid = tsb?tsb->ID():-1;
	int muonid = gEDA->IdMuon();
	
	CreateCanvas(Form("%d", t->ID()));
		
	TPaveText *pt = new TPaveText(0.05,0.87,0.9,0.98,"br");
	pt->SetTextSize(0.03);
	pt->SetTextAlign(12);
	int trklen = abs(slast->Plate() - s1st->Plate())+1;
	
	pt->AddText(Form("trk %4d pl %2d -> %2d %8.1lf %8.1lf %8.1lf %7.4lf %7.4lf",
				t->ID(), t->GetSegmentFirst()->Plate(), 
				t->GetSegmentLast()->Plate(),
				t->X(),t->Y(),t->Z(),t->TX(),t->TY()));
				
	int nMT = 0;
	for(int i=0;i<t->N();i++) if(t->GetSegment(i)->Side()!=0) nMT++;
	pt->AddText(Form("length = %d, nseg = %d (n_{MT}=%d), eff = %.3lf, PHmean = %.1lf #pm %.1lf (CR %.3lf, %.1lf)", 
		trklen, t->N(), nMT,
		t->N()==2?0.0: (double)(t->N()-2)/(trklen-2), Wmean, t->N()>1 ? Wrms/sqrt((double)(t->N()-1)) : Wrms, effcr, wcr));
	
	
	pt->AddText(Form("P = %.2lf - %.2lf +%.2lf GeV/c (90%%CL)", p, pmin, pmax));
	pt->Draw();
	

	if(sbid>0){
		TPaveText *pt = new TPaveText(0.82,0.87,0.95,0.98,"br");
		pt->SetTextSize(0.035);
		pt->SetTextAlign(12);
		pt->AddText(Form("SBid = %d", sbid));
		if(sbid==muonid) pt->AddText(Form("Muon"));
		pt->Draw();
	}
	
	TPad *c1_x  = new TPad("c1_x",  "Trajectories", 0.01,0.01,0.99,0.85);
	c1_x->Draw();

	c1_x->cd();
	//c1_x->Divide(3,3);
	c1_x->Divide(3,2);

	nt->SetMarkerStyle(20);
	nt->SetMarkerSize(0.4);

	int ic=1;

	c1_x->cd(ic++);
	nt->Draw("eX:ipl");
	nt->GetHistogram()->SetYTitle("X #mum");
	nt->SetMarkerColor(kRed);
	nt->Draw("eX:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("eX:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);

	TLine *l = new TLine();
	t2->PropagateTo(s1st->Z());
	double x1 = s1st->Plate();
	double y1 = t2->X();
	t2->PropagateTo(slast->Z());
	double x2 = slast->Plate();
	double y2 = t2->X();
	l->DrawLine(x1,y1,x2,y2);

	c1_x->cd(ic++);
	nt->Draw("dX:ipl");
	nt->GetHistogram()->SetYTitle("dX #mum");
	nt->SetMarkerColor(kRed);
	nt->Draw("dX:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("dX:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);

	c1_x->cd(ic++);
	nt->Draw("eTX:ipl");
	nt->GetHistogram()->SetYTitle("TX rad");
	nt->SetMarkerColor(kRed);
	nt->Draw("eTX:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("eTX:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);
	
	c1_x->cd(ic++);
	nt->Draw("eY:ipl");
	nt->GetHistogram()->SetYTitle("Y #mum");
	nt->SetMarkerColor(kRed);
	nt->Draw("eY:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("eY:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);


	t2->PropagateTo(s1st->Z());
	x1 = s1st->Plate();
	y1 = t2->Y();
	t2->PropagateTo(slast->Z());
	x2 = slast->Plate();
	y2 = t2->Y();
	l->DrawLine(x1,y1,x2,y2);

	c1_x->cd(ic++);
	nt->Draw("dY:ipl");
	nt->GetHistogram()->SetYTitle("dY #mum");
	nt->SetMarkerColor(kRed);
	nt->Draw("dY:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("dY:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);

	c1_x->cd(ic++);
	nt->Draw("eTY:ipl");
	nt->GetHistogram()->SetYTitle("TY rad");
	nt->SetMarkerColor(kRed);
	nt->Draw("eTY:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("eTY:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);
	/*
	c1_x->cd(ic++);
	
	nt->Draw("eW:ipl");
	nt->GetHistogram()->SetYTitle("Pulse Height");
//	nt->GetHistogram()->Fit("pol1");
	nt->GetHistogram()->SetMinimum(0);
	nt->GetHistogram()->SetMaximum(0);
	nt->SetMarkerColor(kRed);
	nt->Draw("eW:ipl","eSide==1", "same");
	nt->SetMarkerColor(kBlue);
	nt->Draw("eW:ipl","eSide==2", "same");
	nt->SetMarkerColor(kBlack);
	l->DrawLine(s1st->Plate(), wcr, slast->Plate(), wcr);
	*/
}

void EdbEDAPlotTab::CheckKinkTracks(){
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	if(selected_tracks->GetEntries()==0) return;

	for(int i=0;i<selected_tracks->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *) selected_tracks->At(i);
		if(t==NULL) continue;
		CheckKink(t);
	}
	
	
}


TObjArray * EdbEDAPlotTab::CheckKink(EdbTrackP *trk){
	// Search kink and make a plot.
	
	if(trk->N()<2) {
		printf("Track %d nseg=%d, too short for kink search\n", trk->ID(), trk->N());
		return NULL;
	}
	
	char ntname[20];
	sprintf(ntname,"ntKinkDT%d", trk->ID());
	TNtuple *tree = (TNtuple *) gROOT->FindObject(ntname);
	if(tree) tree->Delete();
	
	tree = new TNtuple(ntname, "InTrack Decay Search","itrk:ipl1:ipl2:P:dtt:dtl:dt:Pt:rmst:rmsl:rms:dxt:dxl");
	
	TObjArray *kinks = new TObjArray;
	
	EdbTrackP *t = CleanTrack(trk); // Remove fake segment. this may be deleted later.
	
	if(t->Npl()<t->N()){
		// in this case P estimation doesn't work.
		// this can happen if a track is formed from 2 different data set.
		// (PID definitions are defferent amond 2 data set.)
		// put absolute plate number as PID. (temporary solution)
		for(int i=0;i<t->N();i++) t->GetSegment(i)->SetPID(t->GetSegment(i)->Plate());
	}
	
	// minimum value of rms (angular resolution).
	double angle = sqrt(t->TX()*t->TX()+t->TY()*t->TY());
	double resT  = 0.0015*sqrt(2.0);
	double resL  = 0.0015*(1+angle*5)*sqrt(2.0);
	
	// loop over delta-theta
	int n=t->N();
	for(int j=0;j<n-1;j++){
		// kink angle calculation
		EdbSegP *s1 = t->GetSegment(j);
		EdbSegP *s2 = t->GetSegment(j+1);
		
		int ipl1 = s1->Plate();
		int ipl2 = s2->Plate();
		if(ipl2>=57) continue;
		
		// calculate RMS removing this kink angle
		double rms,rmst,rmsl;
		DTRMSTLGiven1Kink(t, j, &rms, &rmst, &rmsl); // DTRMS for each projection.
		
		// set minimum value for RMS by angular resolution.
		rmst = rmst>resT ? rmst : resT;
		rmsl = rmsl>resL ? rmsl : resL;
		
		// calculate kink angle in transverse and longitudinal projection.
		double dtt, dtl;
		CalcDTTransLongi(s1,s2, &dtt, &dtl);
		double dt = sqrt(dtt*dtt+dtl*dtl);
		double dxt, dxl;
		CalcDXTransLongi(t->GetSegmentFirst(),s2, &dxt, &dxl);
		
		// momentum calculation using downstream from s2.
		double p,pmin,pmax;
		CalcPPartial(t,s2,t->GetSegmentLast(), p, pmin, pmax, kFALSE);
		
		// calculate Pt at the kink, using downstream momentum.
		double Pt = p>0 ? p*dt : -1.;
		
		// fill the tree.
		tree->Fill( t->ID(), ipl1, ipl2, p, dtt, dtl, dt, Pt, rmst, rmsl, rms, dxt, dxl);
		
		// if there is kink.
		// if kink angle is bigger than 5 times delta-theta rms in one of the 2 projection.
		if( fabs(dtt)>rmst*5 || fabs(dtl)>rmsl*5 ) {
			// put the data in a struct.
			// calculate vertex point with 2 segments.
			TObjArray segs;
			segs.Add(s1);
			segs.Add(s2);
			EdbVertex *v = CalcVertex(&segs);
			gEDA->AddVertex(v);
			
			int ndau = n-j-1;
			// put the data in a struct.
			EdbEDASmallKink *kink = new EdbEDASmallKink(v, t, s1, s2, dtt, dtl, dxt, dxl, ndau, p, pmin, pmax, Pt, rmst, rmsl);
			kinks->Add(kink);
			
			printf("Kink candidate. itrk %d plate %d - %d kink angle %.4lf P %.3lf Pt %.3lf\n", 
				t->ID(), ipl1, ipl2, dt, p, Pt);
			printf(" (Transverse, Longitudinal) = ( %.4lf, %.4lf ) threshold ( %.4lf, %.4lf ), R = %.2f %.2f\n",
				dtt, dtl, rmst*5, rmsl*5, dtt/rmst, dtl/rmsl);
			printf(" rms=%.4lf rms_transvers=%.4lf rms_longitudinal=%.4lf\n", rms, rmst, rmsl);
			printf(" kinkpoint %.1f %.1f %.1f\n", v->X(), v->Y(), v->Z());
				
			//gEDA->AddDrawObject(kink);
		}
	}
	
	printf("%d kink candidates are found.\n", kinks->GetEntries());
	
	
	tree->SetMarkerStyle(20);
	TText *text = new TText();
	text->SetTextSize(0.035);
	text->SetTextAngle(20);

	double iplmin = tree->GetMinimum("ipl1");
	double iplmax = tree->GetMaximum("ipl2")+1;
	
	// calculate dtmax for the graph hight.
	double dt6[6];
	dt6[0] = fabs(tree->GetMaximum("dtt"));
	dt6[1] = fabs(tree->GetMinimum("dtt"));
	dt6[2] = fabs(tree->GetMaximum("dtl"));
	dt6[3] = fabs(tree->GetMinimum("dtl"));
	dt6[4] = tree->GetMaximum("rmst") * 5;
	dt6[5] = tree->GetMaximum("rmsl") * 5;
	double dtmax = TMath::MaxElement(6,dt6) * 1.15 * 1e3;
	
	double rmst = tree->GetMinimum("rmst");
	double rmsl = tree->GetMinimum("rmsl");

	TText *textrms = new TText();
	textrms->SetTextSize(0.035);
	

	TCanvas *c1 = CreateCanvas(Form("K%d%s", trk->ID(), kinks->GetEntries()? "!":""));
	c1->Divide(2,2);

	c1->cd(1);

	TString hname = Form("hrmst%d",trk->ID());
	TH1F *h1 = (TH1F *) gROOT->FindObject(hname);
	if(h1) h1->Delete();
	h1 = new TH1F(hname,Form("#delta#theta^{RMS}Transverse itrk=%d",trk->ID()), (int)(iplmax-iplmin+1), iplmin-0.5, iplmax+0.5);
	h1->SetLineColor(kGray);
	h1->SetFillColor(kGray);
	tree->Draw("ipl2 >>"+hname,"rmst*1e3");

	hname+="_5";
	TH1F *h15 = (TH1F *) gROOT->FindObject(hname);
	if(h15) h15->Delete();
	h15 = new TH1F(hname,Form("#delta#theta^{RMS}Transverse itrk=%d",trk->ID()), (int)(iplmax-iplmin+1), iplmin-0.5, iplmax+0.5);
	tree->Draw("ipl2 >>"+hname,"rmst*1e3*5");
	h15->SetLineColor(kCyan-10);
	h15->SetFillColor(kCyan-10);
	h15->SetXTitle("Plate number");
	h15->SetYTitle("Kink angle (mrad)");
	h15->SetMaximum(dtmax);
	h15->SetStats(0);
	
	h15->Draw();
	h1->Draw("same");
	tree->Draw("abs(dtt)*1e3:ipl2","","same");
	
	TLine *l = new TLine();
	double xmin = h1->GetXaxis()->GetXmin();
	double xmax = h1->GetXaxis()->GetXmax();
	l->DrawLine(xmin, rmst*1e3, xmax, rmst*1e3);
	l->SetLineColor(kCyan);
	l->DrawLine(xmin, rmst*5*1e3, xmax, rmst*5*1e3);
	
	textrms->DrawText(xmin+0.8*(xmax-xmin), rmst*1e3, Form("%.1lfmrad",rmst*1e3));
	textrms->DrawText(xmin+0.8*(xmax-xmin), 5*rmst*1e3, Form("%.1lfmrad",5*rmst*1e3));
	
	if(kinks->GetEntries()){
		for(int i=0;i<kinks->GetEntries();i++){
			EdbEDASmallKink *kink = (EdbEDASmallKink *) kinks->At(i);
			text->DrawText(kink->IPL2()+0.5, fabs(kink->DTT())*1e3, Form("%.1lfmrad pl%d-%d pt%.3lf", fabs(kink->DTT()*1e3), kink->IPL1(), kink->IPL2(), kink->PT()));
		}
	}

	c1->cd(2);

	hname = Form("hrmsl%d",trk->ID());
	h1 = (TH1F *) gROOT->FindObject(hname);
	if(h1) h1->Delete();
	h1 = new TH1F(hname,Form("#delta#theta^{RMS}Transverse itrk=%d",trk->ID()), (int)(iplmax-iplmin+1), iplmin-0.5, iplmax+0.5);
	h1->SetLineColor(kGray);
	h1->SetFillColor(kGray);
	tree->Draw("ipl2 >>"+hname,"rmsl*1e3");

	hname+="_5";
	h15 = (TH1F *) gROOT->FindObject(hname);
	if(h15) h15->Delete();
	h15 = new TH1F(hname,Form("#delta#theta^{RMS}Transverse itrk=%d",trk->ID()), (int)(iplmax-iplmin+1), iplmin-0.5, iplmax+0.5);
	tree->Draw("ipl2 >>"+hname,"rmsl*1e3*5");
	h15->SetLineColor(kCyan-10);
	h15->SetFillColor(kCyan-10);
	h15->SetXTitle("Plate number");
	h15->SetYTitle("Kink angle (mrad)");
	h15->SetMaximum(dtmax);
	h15->SetStats(0);
	
	h15->Draw();
	h1->Draw("same");
	tree->Draw("abs(dtl)*1e3:ipl2","","same");

	l = new TLine();
	l->DrawLine(xmin, rmsl*1e3, xmax, rmsl*1e3);
	l->SetLineColor(kCyan);
	l->DrawLine(xmin, rmsl*5*1e3, xmax, rmsl*5*1e3);
	
	textrms->DrawText(xmin+0.8*(xmax-xmin), rmsl*1e3, Form("%.1lfmrad",rmsl*1e3));
	textrms->DrawText(xmin+0.8*(xmax-xmin), 5*rmsl*1e3, Form("%.1lfmrad",5*rmsl*1e3));
	
	if(kinks->GetEntries()){
		for(int i=0;i<kinks->GetEntries();i++){
			EdbEDASmallKink *kink = (EdbEDASmallKink *) kinks->At(i);
			text->DrawText(kink->IPL2()+0.5, fabs(kink->DTL())*1e3, Form("%.1lfmrad pl%d-%d pt%.3lf", fabs(kink->DTL()*1e3), kink->IPL1(), kink->IPL2(), kink->PT()));
			
		}
	}
	
	
	c1->cd(3);
	TH2F *h = (TH2F *) gROOT->FindObject(Form("hdxt%d",trk->ID()));
	if(h) h->Delete();
	double dxmin = tree->GetMinimum("dxt");
	double dxmax = tree->GetMaximum("dxt");
	h = new TH2F(Form("hdxt%d",trk->ID()),Form("#deltax Transverse itrk=%d", trk->ID()), 10, iplmin, iplmax, 10, dxmin,dxmax);
	h->SetXTitle("Plate number");
	h->SetYTitle("#deltax (#mum)");
	h->SetStats(0);
	h->Draw();
	tree->Draw("dxt:ipl2","","same");
	if(kinks->GetEntries()){
		for(int i=0;i<kinks->GetEntries();i++){
			EdbEDASmallKink *kink = (EdbEDASmallKink *) kinks->At(i);
			text->DrawText(kink->IPL2()+0.5, kink->DXT(), Form("%.1lfmrad pl%d-%d pt%.3lf", fabs(kink->DTT()*1e3), kink->IPL1(), kink->IPL2(), kink->PT()));
		}
	}


	c1->cd(4);
	h = (TH2F *) gROOT->FindObject(Form("hdxl%d",trk->ID()));
	if(h) h->Delete();
	dxmin = tree->GetMinimum("dxl");
	dxmax = tree->GetMaximum("dxl");
	h = new TH2F(Form("hdxl%d",trk->ID()),Form("#deltax Longitudinal itrk=%d", trk->ID()), 10, iplmin, iplmax, 10, dxmin,dxmax);
	h->SetXTitle("Plate number");
	h->SetYTitle("#deltax (#mum)");
	h->SetStats(0);
	h->Draw();
	tree->Draw("dxl:ipl2","","same");
	
	if(kinks->GetEntries()){
		for(int i=0;i<kinks->GetEntries();i++){
			EdbEDASmallKink *kink = (EdbEDASmallKink *) kinks->At(i);
			text->DrawText(kink->IPL2()+0.5, kink->DXL(), Form("%.1lfmrad pl%d-%d pt%.3lf", fabs(kink->DTL()*1e3), kink->IPL1(), kink->IPL2(), kink->PT()));
		}
	}
	
	return kinks;
}

void EdbEDAPlotTab::SetMomAlg(){
	int alg = EdbEDAUtil::InputNumberInteger(
		"Set Momentum methos.\n"
		"0 = Angular method (default)\n"
		"3 = Coordinate method\n"
		, eTF.eAlg);
	
	eTF.eAlg = alg;
}

void EdbEDAPlotTab::MomPlot(){
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	if(selected_tracks->GetEntries()==0) return;
	
	double angular_resolution = eMomAngleRes->GetNumber();
	eTF.eDT0  = angular_resolution;
	eTF.eDTx0 = angular_resolution;
	eTF.eDTy0 = angular_resolution;
	
	for(int i=0;i<selected_tracks->GetEntries();i++){
		EdbTrackP *t0 = (EdbTrackP *) selected_tracks->At(i);
		if(t0==NULL) continue;
		EdbTrackP *t=CleanTrack(t0);
		
		if(t->Npl()<t->N()){
			// in this case P estimation doesn't work.
			// this can happen if a track is formed from 2 different data set.
			// (PID definitions are defferent amond 2 data set.)
			// put absolute plate number as PID. (temporary solution)
			for(int i=0;i<t->N();i++) t->GetSegment(i)->SetPID(t->GetSegment(i)->Plate());
		}
		
		for(int i=0;i<t->N();i++) t->GetSegment(i)->SetPID(t->GetSegment(i)->Plate());
		t->SetCounters();
		
		if(t->N()<=2) continue;
		eTF.eMinEntr=3; 
		
		printf("Track ID: %i ; TX=%1.4f  ,  TY=%1.4f  ,  Nb of BT= %i\n",t->ID(),t->TX(),t->TY(),t->N());

		eTF.PMS(*t);      
		
		float p, pmin, pmax;
		if (sqrt(t->TX()*t->TX()+t->TY()*t->TY())<=0.2)  // use P3D
		{
			printf("   PT =%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] \n", eTF.ePy, eTF.ePYmin, eTF.ePYmax);	   
			printf(" ->P3D=%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] \n", eTF.eP, eTF.ePmin, eTF.ePmax);
			p=eTF.eP;
			pmin=eTF.ePmin;
			pmax=eTF.ePmax;
		}
		if (sqrt(t->TX()*t->TX()+t->TY()*t->TY())>0.2)  // use PT
		{
			printf(" ->PT =%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] \n", eTF.ePy, eTF.ePYmin, eTF.ePYmax);	   
			printf("   P3D=%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] \n", eTF.eP, eTF.ePmin, eTF.ePmax);
			p=eTF.ePy;
			pmin=eTF.ePYmin;
			pmax=eTF.ePYmax;
		}
		TCanvas *c1 = CreateCanvas(Form("P%d", t->ID()));

		eTF.DrawPlots(c1);
	}
}

void EdbEDAPlotTab::MakeGUI(){
	if(gEve==NULL) return;
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(TRootBrowser::kBottom);

	TGHorizontalFrame* fMainFrame = new TGHorizontalFrame(gClient->GetRoot());
	fMainFrame->SetWindowName("XX GUI");
	fMainFrame->SetCleanup(kDeepCleanup);

	//TGLabel *fLabel;
	TGTextButton *fb;
	int posy=10;
	int posx=10;
	int dx=50;

	TGGroupFrame *fGroup;
	
	///////////////////////////////////////////////////////////////////
	fGroup = new TGGroupFrame(fMainFrame,"TS");
	fGroup->SetLayoutBroken(kTRUE);
		posy=18;
		posx=10;
		fb = new TGTextButton(fGroup,"Overview");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckOverview()");
		fb->SetToolTipText("Basic plots, nsegcut on Main tab is valid.");
		posx+=dx+10;
		fb = new TGTextButton(fGroup,"Efficiency");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckEff()");
		fb->SetToolTipText("Efficiency for TS. Use all tracks with more than 4 segments except the plate to be evaluated.");

		posy+=23;
		posx=10;
		fb = new TGTextButton(fGroup,"PH - dAngle");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckPHDAngle()");

		posx+=dx+10;
		fb = new TGTextButton(fGroup,"Alignment");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckAlignment()");
		fb->SetToolTipText("Position, Angle displacement as profile hist (RMS). Nsegcut on Main tab is valid.");
		
	fMainFrame->AddFrame(fGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fGroup->Resize(200,75);

	////////////////////////////////////////////////////////////////////////
	fGroup = new TGGroupFrame(fMainFrame,"Track");
	fGroup->SetLayoutBroken(kTRUE);

		posy=18;
		posx=10;

		fb = new TGTextButton(fGroup,"Track");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckTracks()");

		posx+=dx+10;
		fb = new TGTextButton(fGroup,"Kink");
		fb->MoveResize(posx,posy,dx=80,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"CheckKinkTracks()");
		fb->SetToolTipText("Search Kink");

		posy+=23;
		posx=10;

		fb = new TGTextButton(fGroup,"Mom Plot");
		fb->MoveResize(posx,posy,dx=70,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"MomPlot()");
		fb->SetToolTipText("Make Momentum plots. \nAngular resolution indicated in the right number entry will be used. default=0.0021");
		
		posx+=dx+5;
		fb = new TGTextButton(fGroup,"Alg");
		fb->MoveResize(posx,posy,dx=30,20);
		fb->Connect("Clicked()","EdbEDAPlotTab",this,"SetMomAlg()");
		fb->SetToolTipText("Select angular method or coordinate method\nfor momentum computation.");

		posx+=dx+5;
		eMomAngleRes = new TGNumberEntry(fGroup, eTF.eDT0, 11,-1,TGNumberFormat::kNESRealFour);
		eMomAngleRes->MoveResize(posx,posy,dx=60,20);
		eMomAngleRes->GetNumberEntry()->SetToolTipText("Angular resolution. default=0.0021\n"
			"This will be applied only for angular method.");
	
	fMainFrame->AddFrame(fGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
	fGroup->Resize(240,75);

//	posx+=dx+10;
	
	fMainFrame->MapSubwindows();
	fMainFrame->Resize();
	fMainFrame->MapWindow();

	browser->StopEmbedding();
	browser->SetTabTitle("Plots", 2);
}
