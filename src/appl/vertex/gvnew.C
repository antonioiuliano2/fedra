//------------------------------------------------------------------
// 
// Usage example:
//
// root [0] .L gv.C
// root [1] gv(300); // generate and reconstruct 300 K->3Pi events
//
//------------------------------------------------------------------

using namespace VERTEX;

EdbPVRec    *ali=0;
EdbPVGen    *gener = 0;
EdbScanCond *scanCond=0;

int   nve=10, nback1=100, nback2=100;
float back1_tetamax =  0.35; 
float back2_tetamax =  0.25; 
float back2_plim[2] = {0.5, 5.5};
float fiducial_border = 10000.;
float vxyz[3] = {0., 0., 0.};   // simulated vertex position

int local_coordinates = 1;
int ideal_tracks = 0;
int neutral_primary = 1;
int rec_type = 2;
int vert_type = 1;
int npi = 3;
int nsegMin = 4;
float pMin = .1;
int eloss_flag = 1; // =0 - no energy losses, 1-average energy losses, 2-Landau energy losses

float momentumK=30.; // momentum of K-meson

float dpp = 0.15;   // average dp/P 
float seg_s[4]={.5, .5, 0.0015, 0.0015}; //sx,sy,stx,sty,sP
float seg_zero[4]={.0,.0,.0,.0}; //no measurement errors
float ProbGap = 0.10;

int maxgaps[6] = {1,2,1,1,1,1};
float AngleAcceptance = 0.4;

float ProbMinV = 0.00;
float ProbMinP = 0.05;
float ProbMinT = 0.00;

const unsigned long int PrintN = 100;


TObjArray *genVtx;  //vertex array

TH1F *hp[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

EdbVertex *vedb;

int ntall = 0;
int numt = 0;
int numev = 0;

double mK=.4937;  // mass of K-meson
double mPi=.139;  // mass of K-meson

EdbTrackP *trp = 0;

struct BRICK {
  int   plates;                       // plates to be simulated
  float z0;                           // start from here
  float dz;                           // 
  float lim[4];                       // xmin,ymin,xmax,ymax
  float X0; 
};

BRICK brick;

brick.plates =      56;
brick.z0     =      0.;
brick.dz     =   1300.;
brick.lim[0] = -50000.;
brick.lim[1] = -50000.;
brick.lim[2] =  50000.;
brick.lim[3] =  50000.;
brick.X0     =   5810.;

float vxo = 0., vyo = 0., vzo = 0.;
float vxg = 0., vyg = 0., vzg = 0.;
double DE_MC, DE_FIT;

//-------------------------------------------------------------------
TGenPhaseSpace *K3Pi(float p=3.)
{
  // K->3Pi decay phase space

  double e=TMath::Sqrt(p*p+mK*mK);
  TLorentzVector K(0,0,p,e);
  Double_t masses[3] = { mPi, mPi, mPi } ;
  TGenPhaseSpace *psp = new TGenPhaseSpace();
  psp->SetDecay(K, npi, masses);
  return psp;
}

TGenPhaseSpace *pDecay= K3Pi(momentumK);

///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack( EdbScanCond *cond )
{
  cond->SetSigma0( .5, .5,.0015,.0015 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 0.1 );               // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(8,8,4,4);             // bins in [sigma] for checks
  cond->SetPulsRamp0(  1.,1. );         // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 1.,1. );         //
  cond->SetChi2Max(2.7);
  cond->SetChi2PMax(2.7);
  cond->SetRadX0(5810.);

  cond->SetName("Prototype_OPERA_basetrack");
}

///------------------------------------------------------------
void gv(int n=1)
{
  // main steering routine for K->3Pi vertex generation&reconstruction test

  if (rec_type > 0 && vert_type == 0 && nve > 1)
  {
    printf("Wrong combination of conditions - fixed vertex position\n");
    printf("with multiple vertexes in volume\n");
  }
  timespec_t t0, t;
  double dt;
  ntall = nve*(npi+1-neutral_primary)+nback2;
  BookHistsV();

  ali      = new EdbPVRec();
  EdbPatternsVolume *vol=(EdbPatternsVolume*)ali;
  makeVolumeOPERAn(brick,vol);

  ali->eVTX = new TObjArray(1000);

  scanCond = new EdbScanCond();
  Set_Prototype_OPERA_basetrack(scanCond);

  ali->SetScanCond(scanCond);

  ali->SetPatternsID();
  ali->SetCouplesAll();
  ali->SetChi2Max(scanCond->Chi2PMax());
  //ali->SetOffsetsMax(cond->OffX(),cond->OffY());

  gener = new EdbPVGen();
  gener->SetVolume(vol);
  gener->eVTX = new TObjArray(1000);
  gener->SetScanCond(scanCond);

  TTimeStamp  ts=TTimeStamp();
  t0=ts.GetTimeSpec();
  numev = 0;
  for(int i=1; i<=n; i++) {
    g1(nve,nback1,nback2);
    numev++;
    if (!((numev*nve)%PrintN))
    {
	ts=TTimeStamp();
	t=ts.GetTimeSpec();
	dt=(t.tv_sec-t0.tv_sec)*1000000000.+(t.tv_nsec-t0.tv_nsec);
	if (nve>0&&nback2<=0)
	    printf("%d events generated in %.4f sec (%.1f msec per event)\n",
	    numev*nve,dt/1000000000.,dt/(double)(numev*nve)/1000000.);
	else if (nve>0&&nback2>0)
	    printf("%d events and %d background tracks generated in %.4f sec (%.1f msec per event)\n",
	    numev*nve,numev*nback2, dt/1000000000.,dt/(double)(numev*nve)/1000000.);
	else if (nve<=0&&nback2>0)
	    printf("%d background tracks generated in %.4f sec (%.1f msec per event)\n",
	    numev*nback2, dt/1000000000.,dt/(double)(numev*nback2)/1000000.);
    }
  }

  DrawHistsV();
  delete ali;
  ali = 0;
  delete gener;
  gener = 0;
}


///------------------------------------------------------------
void g1(int nv=1, int nb1=0, int nb2=0)
{
  //generation of 1 pattern volume
  float vlim[4], vzlim[2];
  
  vlim[0] = brick.lim[0] + fiducial_border;
  vlim[1] = brick.lim[1] + fiducial_border;
  vlim[2] = brick.lim[2] - fiducial_border;
  vlim[3] = brick.lim[3] - fiducial_border;
  vzlim[0] = fiducial_border;
  vzlim[1] = brick.plates*brick.dz - fiducial_border;

  if (nv) 
	gener->GeneratePhaseSpaceEvents( nv, pDecay,           vzlim,
				         vlim,             seg_s,
				         ProbGap,          eloss_flag,
				         !neutral_primary);
  if (rec_type >= 2 && nb1) 
	gener->GenerateUncorrelatedSegments(nb1, brick.lim, seg_s,
					    back1_tetamax, -30 );
  if (rec_type >= 1 && nb2)
	gener->GenerateBackgroundTracks(nb2, brick.lim, back2_plim,
				        seg_s,     back2_tetamax,
				        ProbGap,   eloss_flag );

  if (rec_type == 0)
  {
	rec_v(nv);
  }
  else if(rec_type == 1)
  {
	rec_vfind();
  }
  else
  {
	rec_all();
  }

  if (gener->eVTX) gener->eVTX->Clear();
  if (gener->eTracks) gener->eTracks->Clear();
  gener->Clear();
  if (ali->eTracks) ali->eTracks->Clear();
  if (ali->eVTX) ali->eVTX->Clear();
  ali->Clear();

}

///------------------------------------------------------------
void makeVolumeOPERAn( BRICK &b, EdbPatternsVolume *v )
{
  // Volume geometry setting

  EdbPattern *pat =0;
  for(int i=0; i<b.plates; i++) {
    pat = new EdbPattern(0,0,b.z0+b.dz*i);
    v->AddPattern(pat);
  }
}
///------------------------------------------------------------
void rec_v(int nv)
{
  // tracks and vertex reconstruction (KF fitting only without track finding)

  float p;
  float xg, yg, zg, txg, tyg, pg;
  EdbTrackP *tr, *trg;
  EdbVertex *vedb = 0;
  EdbVertex *vedbg = 0;
  Vertex *v;

  if (hp[9]) hp[9]->Fill(nv);

  int ntre = 0;
  for(int i=0; i<nv; i++) {
    vedbg = (EdbVertex *)(gener->eVTX->At(i));
    vedb = new EdbVertex();
    int nt = vedbg->N();
    for(int ip=0; ip<nt; ip++) {
	tr = vedbg->GetTrack(ip);
	if (hp[11]) hp[11]->Fill(tr->N());
	if (tr->P() == momentumK) continue;
	if (tr->N() < nsegMin) break;
	if (tr->P() < 0.1) break;

	p = tr->P();
	tr->SetErrorP(p*p*dpp*dpp);
	DE_MC = tr->DE();

	p = p*(1.+dpp*gRandom->Gaus());
	if (p < 0.01) p = 0.01;

	if (ideal_tracks)
	{
	    xg = tr->X();
	    yg = tr->Y();
	    zg = tr->Z();
	    txg = tr->TX();
	    tyg = tr->TY();
	    pg  = tr->P();
	}
	tr->SetP(p);
	tr->FitTrackKFS(false);
	if (ideal_tracks)
	{
	    tr->Set(tr->ID(), xg, yg, txg, tyg, tr->W(), tr->Flag());
	    tr->SetZ(zg);
	    tr->SetP(pg);
	}
	
	ali->AddTrack(tr);
	vedb->AddTrack(tr, vedbg->Zpos(ip), ProbMinV );
	if (ip == 1)
	{
	    if ( vedb->MakeV() != 2)
	    {
		printf(" Wrong vertex making!\n");
		break;
	    }
	    vedb->V()->findVertexVt();
	}
	
	if (hp[12]) hp[12]->Fill(tr->GetSegmentFirst()->X()-tr->GetSegmentFFirst()->X());

	DE_FIT = tr->DE();
    
	hp[13]->Fill(DE_FIT-DE_MC);
    }

    v = vedb->V();

    if (v)
    {
      if (v->valid())
      {
	if (v->ntracks() == npi)       // fit vertex
	{
	    if ( local_coordinates )
	    {
		vxo = vedb->X();	
		vyo = vedb->Y();	
		vzo = vedb->Z();
		vxg = vedbg->X();
		vyg = vedbg->Y();
		vzg = vedbg->Z();
	    }	
	    FillHistsV(*v);
	}
      }
    }
    ntre += vedb->N();
  }
  if (hp[10]) hp[10]->Fill(ntre);
}
///------------------------------------------------------------
void rec_vfind()
{
  // tracks and vertex reconstruction (vertex finding and fitting, without track finding)

  float xg, yg, zg, txg, tyg, pg;
  int zpos;
  int nvg = gener->eVTX->GetEntries();

  Vertex *v;
  EdbVertex *edbv, *edbv1, *edbv2, *edbvg
  EdbTrackP *tr = 0;
  EdbSegP *s = 0;
  float p, trprob;
  int ntr = 0;
  if (gener->eTracks) ntr = gener->eTracks->GetEntries();
  if (hp[10]) hp[10]->Fill(ntr);
  printf(" %6d generated vertexes\n", nvg);
  printf(" %6d generated tracks\n", ntr);

  int ntr_nmin = 0;
  int ntr_pmin = 0;
  int ntr_prob = 0;
  int ntrgood = 0;

  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(gener->eTracks->At(itr));

    if (hp[11]) hp[11]->Fill(tr->N());
//    printf(" tr # %d, N = %d P = %f\n", itr, tr->N(), tr->P());
    if (tr->N() < nsegMin)
    {
	ntr_nmin++;
	continue;
    }
    if (tr->P() < pMin)
    {
	ntr_pmin++;
	continue;
    }

    p = tr->P();
    tr->SetErrorP(p*p*dpp*dpp);
    DE_MC = tr->DE();

    p = p*(1.+dpp*gRandom->Gaus());
    if (p < 0.01) p = 0.01;

    if (ideal_tracks)
	{
	    xg = tr->X();
	    yg = tr->Y();
	    zg = tr->Z();
	    txg = tr->TX();
	    tyg = tr->TY();
	    pg  = tr->P();
	}

    tr->SetP(p);
    tr->FitTrackKFS(false);
    if (hp[12]) hp[12]->Fill(tr->GetSegmentFirst()->X()-tr->GetSegmentFFirst()->X());
    DE_FIT = tr->DE();

    if (ideal_tracks)
	{
	    tr->Set(tr->ID(), xg, yg, txg, tyg, tr->W(), tr->Flag());
	    tr->SetZ(zg);
	    tr->SetP(pg);
	}
    
    trprob = tr->Prob();

    hp[17]->Fill(trprob);

    if (trprob < ProbMinT)
    {
	ntr_prob++;
	continue;
    }
    ali->AddTrack(tr);
    ntrgood++;
    
    hp[13]->Fill(DE_FIT-DE_MC);
  }

  printf(" %6d tracks found after track selection\n", ntrgood);
  printf("        ( %4d tracks with nseg < %d )\n", ntr_nmin, nsegMin);
  printf("        ( %4d tracks with p    < %6.3f GeV/c)\n", ntr_pmin, pMin);
  printf("        ( %4d tracks with prob < %6.3f )\n", ntr_prob, ProbMinT);

  int nvtx = ali->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin);

  printf(" %d vertexes found\n", nvtx);

  int nadd = ali->ProbVertex3(ProbMinV);

  int nvgood = 0;
  int ivg = 0;
  for (int i=0; i<nvtx; i++)
    {
  	edbv = (EdbVertex *)((ali->eVTX)->At(i));
	if (edbv)
	{
	    v = edbv->V();
	    if (v)
	    {
		if (v->valid())
		{
		    if (v->ntracks() == 3)
		    {
			nvgood++;
			if ( local_coordinates )
			{
			    int ivg0 = (edbv->GetTrack(0)->Flag());
			    ivg = ivg0 + 1;
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				ivg = (edbv->GetTrack(j)->Flag());
				if (ivg != ivg0) break;
			    }
			    if (ivg != ivg0) continue;
  			    edbvg = (EdbVertex *)((gener->eVTX)->At(ivg-1));
			    vxo = edbv->X();	
			    vyo = edbv->Y();	
			    vzo = edbv->Z();
			    vxg = edbvg->X();
			    vyg = edbvg->Y();
			    vzg = edbvg->Z();
			}	
  	    		FillHistsV(*v);
		    }
		}
	    }
	}
    }
  
  if (hp[9]) hp[9]->Fill(nvgood);
  printf(" %d real vertexes found\n", nvgood);

}


///------------------------------------------------------------
void rec_all()
{
  // tracks and vertex reconstruction (track finding and fitting, vertex finding and fitting)

  float xg, yg, zg, txg, tyg, pg;
  int zpos;
  Vertex *v;
  EdbVertex *edbv, *edbv1, *edbv2, *edbvg
  EdbTrackP *tr = 0;
  EdbTrackP *trg = 0;
  EdbSegP *s = 0;
  float p;

  int itrg = 0;
  int ntrgood = 0;
  int nsegmatch = 0;

  int nvg = 0;
  if (gener->eVTX) nvg = gener->eVTX->GetEntries();
  int ntrg = 0;
  if (gener->eTracks) ntrg = gener->eTracks->GetEntries();
  printf(" %6d generated vertexes\n", nvg);
  printf(" %6d generated tracks\n", ntrg);

  ali->Link();
  printf("link ok\n");
  ali->FillTracksCell();

  ali->MakeTracks();

  int ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();
  int negflag = 0;
  int notmatched = 0;

  for (int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(ali->eTracks->At(itr));
    if (tr->Flag()<0)
    {
	negflag++;
	continue;
    }
    trg = 0;
    if (ntrg)
    {
	itrg = tr->GetSegmentsFlag(nsegmatch);
	if (itrg >= 0 && itrg < ntrg)
	{
	    trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	    p = trg->P();
	    tr->SetErrorP(p*p*dpp*dpp);
	    p = p*(1.+dpp*gRandom->Gaus());
	    tr->SetP(p);
	    if (trg->Flag() == 0)        // background track
	    {
	    }
	    else if (trg->Flag() <= nvg) // track at vertex
	    {
	    }
	}
	else
	{
	    notmatched++;
	}
    }
  }

  printf(" %6d tracks found after track making\n", ntr);
//  printf("        ( %4d tracks with negative flag)\n", negflag);
//  printf("        ( %4d tracks not matched with generated ones)\n", notmatched);

  ali->FitTracks(-1., -1., gener->eTracks);
  ali->FillCell(50,50,0.015,0.015);
  ali->PropagateTracks(brick.plates-1,2,ProbMinP);

  ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();

  negflag = 0;
  notmatched = 0;
  int nreject_prob = 0;
  int nreject_nseg = 0;
  int ntrgoodb = 0;

  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(ali->eTracks->At(itr));
    if (tr->Flag()<0)
    {
	negflag++;
	continue;
    }
    if (hp[11]) hp[11]->Fill(tr->N());
    if (tr->N()<nsegMin)
    {
	nreject_nseg++;
	continue;
    }
    if (tr->Prob()<ProbMinT)
    {
	nreject_prob++;
    }
    trg = 0;
    if (ntrg)
    {
	itrg = tr->GetSegmentsFlag(nsegmatch);
	if (itrg >= 0 && itrg < ntrg)
	{
	    trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	    if (trg->Flag() == 0)        // background track
	    {
		ntrgoodb++;
		hp[16]->Fill((double)nsegmatch/tr->N());
	    }
	    else if (trg->Flag() <= nvg) // track at vertex
	    {
		ntrgood++;
		hp[15]->Fill((double)nsegmatch/tr->N());
	    }
	}
	else
	{
	    notmatched++;
	}
    }
    else
    {
	notmatched++;
    }

//    printf(" tr # %d, N = %d P = %f\n", itr, tr->N(), tr->P());
//    if (tr->N() < nsegMin) continue;
//    if (tr->P() < 0.1) continue;


    if (hp[12]) hp[12]->Fill(tr->GetSegmentFirst()->X()-tr->GetSegmentFFirst()->X());

    if (trg != 0)
    {
	DE_MC = trg->DE();
	DE_FIT = tr->DE();
	hp[13]->Fill(DE_FIT-DE_MC);
	hp[17]->Fill(tr->Prob());
    }
  }

  printf(" %6d tracks found after propagation\n", ntr);
  printf(" %6d matched tracks found (at vertexes)\n", ntrgood);
  printf(" %6d matched tracks found (background )\n", ntrgoodb);
  printf("        ( %4d tracks with negative flag)\n", negflag);
  printf("        ( %4d tracks not matched with generated ones)\n", notmatched);
  printf("        ( %4d tracks with nseg < %d )\n", nreject_nseg, nsegMin);
  printf("        ( %4d tracks with prob < %6.3f )\n", nreject_prob, ProbMinT);

  if (hp[10]) hp[10]->Fill(ntr-negflag);

  int nvtx = ali->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin);

  printf(" %d vertexes found\n", nvtx);

  int nadd = ali->ProbVertex3(ProbMinV);

  int nvgood = 0;
  int ivg = 0;
  for (int i=0; i<nvtx; i++)
    {
  	edbv = (EdbVertex *)((ali->eVTX)->At(i));
	if (edbv)
	{
	    v = edbv->V();
	    if (v)
	    {
		if (v->valid())
		{
		    if (v->ntracks() == 3)
		    {
			nvgood++;
			if ( local_coordinates )
			{
			    int ivg0 = (edbv->GetTrack(0)->GetSegmentsFlag(nsegmatch));
			    if (ivg0 >= 0)
			        ivg0 = ((EdbTrackP*)(gener->eTracks->At(ivg0)))->Flag();
			    ivg = ivg0 + 1;
//			    printf("Vertex %4d, ivg = %d\n", i, ivg0);
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				int ivg = (edbv->GetTrack(j)->GetSegmentsFlag(nsegmatch));
				if (ivg >= 0)
			    	    ivg = ((EdbTrackP*)(gener->eTracks->At(ivg)))->Flag();
				else
				    break;
				if (ivg != ivg0) break;
//				printf("             ivg = %d\n", ivg);
			    }
			    if (ivg != ivg0) continue;
  			    edbvg = (EdbVertex *)((gener->eVTX)->At(ivg-1));
			    vxo = edbv->X();	
			    vyo = edbv->Y();	
			    vzo = edbv->Z();
			    vxg = edbvg->X();
			    vyg = edbvg->Y();
			    vzg = edbvg->Z();
			}	
  	    		FillHistsV(*v);
		    }
		}
	    }
	}
    }
  
  if (hp[9]) hp[9]->Fill(nvgood);
  printf(" %d real vertexes found\n", nvgood);

}

///-----------------------------------------------------------------------
void BookHistsV()
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_Dx","Vertex X error",100,-5.,5.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_Dy","Vertex Y error",100,-5.,5.);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_Dz","Vertex Z error",50,-500.,500.);
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_Sx","Vertex X sigma",100,0.,10.);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_Sy","Vertex Y sigma",100,0.,10.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_Sz","Vertex Z sigma",100,0.,1000.);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",25,0.,5.);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",26,0.,1.04);
  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Mass","Vertex Mass error",50,-1.000,+1.000);
  char title[128];
  sprintf(title,"Number of reconstructed vertexs (%d generated)",nve);
  if (!hp[9]) hp[9] = new TH1F("Hist_Vt_Nvert",title,nve*2,0.,(float)(nve*2));
  sprintf(title,"Number of reconstructed tracks (%d generated)",ntall);
  if (!hp[10]) hp[10] = new TH1F("Hist_Vt_Ntrack",title,ntall*2,0.,(float)(ntall*2));
  if (!hp[11]) hp[11] = new TH1F("Hist_Vt_Nsegs","Number of track segments",brick.plates+1,0.,(float)(brick.plates+1));
  if (!hp[12]) hp[12] = new TH1F("Hist_Vt_Dtrack","Track DeltaX",100,-5.,+5.);
  if (!hp[13]) hp[13] = new TH1F("Hist_Vt_DE","DE(MC)-DE",100,-0.5,+0.5);
  if (!hp[14]) hp[14] = new TH1F("Hist_Vt_Dist","RMS track-vertex distance",100, 0.,50.);
  if (!hp[15]) hp[15] = new TH1F("Hist_Vt_Match","Right segments fraction (tracks at vertexes)",110, 0.,1.1);
  if (!hp[16]) hp[16] = new TH1F("Hist_Vt_Matchb","Right segments fraction (backgound tracks) ",110, 0.,1.1);
  if (!hp[17]) hp[17] = new TH1F("Hist_Vt_Trprob","Track probability",110, 0.,1.1);

//  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Ntra","Vertex Ntracks",20,0.,20.);
}

double smass = 0.139;

///-----------------------------------------------------------------------
void FillHistsV(Vertex &v)
{
  hp[0]->Fill(v.vx() + ((double)vxo - (double)vxg));
  hp[1]->Fill(v.vy() + ((double)vyo - (double)vyg));
  hp[2]->Fill(v.vz() + ((double)vzo - (double)vzg));
  hp[3]->Fill(v.vxerr());
  hp[4]->Fill(v.vyerr());
  hp[5]->Fill(v.vzerr());
  hp[6]->Fill(v.ndf() > 0 ? v.chi2()/v.ndf() : 0);
  hp[7]->Fill(v.prob());
  double rmass = v.mass(smass);
  hp[8]->Fill(rmass-mK);
  hp[14]->Fill(v.rmsDistAngle());
//  hp[8]->Fill(v.ntracks());
}
///-----------------------------------------------------------------------
void DrawHistsV()
{
  TCanvas *cv = new TCanvas();
  cv->Divide(3,3);
  for(int i=0; (i<9)&&hp[i]; i++) {
    cv->cd(i+1);
    if (hp[i]) hp[i]->Draw();
  }
  TCanvas *cv1 = new TCanvas();
  cv1->Divide(3,3);
  for(int i=9; hp[i]; i++) {
    cv1->cd(i+1-9);
    if (hp[i]) hp[i]->Draw();
  }
}
///------------------------------------------------------------
void ClearHistsV()
{
  for(int i=0; hp[i]; i++) {
    hp[i]->Clear();
  }
}
///------------------------------------------------------------

EdbDisplay *ds=0;

void disp_v()
{
  // display of 1 event

  if(!ali) return;
  ali->FillCell(50,50,0.015,0.015);

  TObjArray *arr   = new TObjArray();
  TObjArray *arrtr = new TObjArray();
  float binx=10, bint=10;

  EdbTrackP *track;
  int ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)(ali->eTracks->At(i));
    arrtr->Add(track);
    ali->ExtractDataVolumeSeg( *track,*arr,binx,bint );
  }


  gStyle->SetPalette(1);
  if(!ds)
    ds=new EdbDisplay("display-t",-14000.,14000.,-14000.,14000.,0.,80000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->Draw();
}

///-------------------------------------------------------------------
void draw_v(EdbVertex &vedb, float zoom=1.)
{
  // some plots

  TH1F *hseg[4];
  hseg[0] =  new TH1F("Hist_seg_dx" ,"dx" ,100,-5.,5.);
  hseg[1] =  new TH1F("Hist_seg_dy" ,"dy" ,100,-5.,5.);
  hseg[2] =  new TH1F("Hist_seg_dtx","dtx",100,-5.,5.);
  hseg[3] =  new TH1F("Hist_seg_dty","dty",100,-5.,5.);

  TCanvas *cv = new TCanvas();
  float xmin=brick.lim[0]/zoom;
  float ymin=brick.lim[1]/zoom;
  float xmax=brick.lim[2]/zoom;
  float ymax=brick.lim[3]/zoom;
  float zmin=(brick.z0-brick.dz)/zoom;
  float zmax=(brick.z0+brick.dz*brick.plates)/zoom;
  TH3F *hist = new TH3F("hist","title",100,xmin,xmax,100,ymin,ymax,100,zmin,zmax);
  hist->Draw();
  TPolyLine3D *line=0;

  EdbSegP *seg=0;
  EdbSegP *segf=0;
  int nt = vedb->N();
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);

    tr->SetDZ(3000);
    line = seg_line( (EdbSegP*)tr );
    line->SetLineColor(kGreen);
    line->SetLineWidth(2);
    line->Draw();


    int nseg=tr->N();
    for(int is=0; is<nseg; is++) {
      seg = tr->GetSegment(is);
      seg->SetDZ(300);
      line = seg_line( seg );
      line->SetLineColor(kRed);
      line->SetLineWidth(2);
      line->Draw();

      segf = tr->GetSegmentF(is);
      segf->SetDZ(1300.);
      line = seg_line( segf );
      line->SetLineColor(kBlue);
      line->SetLineWidth(1);
      line->Draw();

      hseg[0]->Fill(seg->X()-segf->X());
      hseg[1]->Fill(seg->Y()-segf->Y());
      hseg[2]->Fill(1000.*(seg->TX()-segf->TX()));
      hseg[3]->Fill(1000.*(seg->TY()-segf->TY()));
    }


  }

  TCanvas *ch = new TCanvas();
  ch->Divide(2,2);
  for(int i=0; i<4; i++) {
    ch->cd(i+1);
    hseg[i]->Draw();
  }


  float pms=0;

  TGraphErrors *gr=0;
  TCanvas *ctr = new TCanvas();
  ctr->Divide(2,2);

  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    ctr->cd(ip+1);
    pms=EdbVertexRec::P_MS(*tr,brick.X0,true);
    printf("p = %f \t pms = %f \t m = %f\n", tr->P(), pms, tr->M() );
  }


}

///------------------------------------------------------------
TPolyLine3D *seg_line(EdbSegP *seg)
{
  TPolyLine3D *line = new TPolyLine3D(2);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  float dz = seg->DZ();
  line->SetPoint(1,
		 seg->X() + seg->TX()*dz,
		 seg->Y() + seg->TY()*dz,
		 seg->Z()+            dz );

  return line;
}

///------------------------------------------------------------
TPolyMarker3D *seg_mark(EdbSegP *seg)
{
  TPolyMarker3D *line = new TPolyMarker3D(2,1);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  float dz = seg->DZ();
  line->SetPoint(1,
		 seg->X() + seg->TX()*dz,
		 seg->Y() + seg->TY()*dz,
		 seg->Z()+            dz );

  return line;
}
