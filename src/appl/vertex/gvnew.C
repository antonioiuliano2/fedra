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

float back1_tetamax =  0.35; 
float back2_tetamax =  0.25; 
float back2_plim[2] = {0.5, 5.5};
float fiducial_border = 10000.;
float fiducial_border_z = 10000.;
float vxyz[3] = {0., 0., 0.};   // simulated vertex position

int local_coordinates = 1;
int ideal_tracks = 0;
int neutral_primary = 1;
int rec_type = 2;
int vert_type = 1;
int npi = 4;
int nuse = 4;
int nsegMin = 4;
float pMin = .1;
int eloss_flag = 1; // =0 - no energy losses, 1-average energy losses, 2-Landau energy losses

float momentumK=20.; // momentum of K-meson

float dpp = 0.015;   // average dp/P 
float seg_s[4]={.5, .5, 0.0015, 0.0015}; //sx,sy,stx,sty,sP
float seg_zero[4]={.0,.0,.0,.0}; //no measurement errors
float ProbGap = 0.10;
float RightRatioMin = 0.5;

//int maxgaps[6] = {1,2,1,1,1,1};
int maxgaps[6] = {2,3,2,2,2,2};
float AngleAcceptance = 0.4;

float ProbMinV  = 0.01;
float ProbMinV3 = 0.01;
float ProbMinV4 = 0.01;
float ProbMinP  = 0.01;
float ProbMinT  = 0.01;

const unsigned long int PrintN = 100;


TObjArray *genVtx;  //vertex array

TH1F *hp[22] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

EdbVertex *vedb;

int ntall = 0;
int numt = 0;
int numev = 0;

//double mK=.4937;  // mass of K-meson
double mK=2.;
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
int charges[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

FILE *f=0;
TFile *rf=0;
TObjArray hlist, hld1, hld2, hld3;

int  nvg_tot = 0;
int  ntrg_tot = 0;
int  ntr_tot = 0;
int  ntrgood_tot = 0;
int  ntrgoodb_tot = 0;
int  ntrgood_r_tot = 0;
int  ntrgoodb_r_tot = 0;
int  nv2good_tot = 0;
int  nv2goodm_tot = 0;
int  nv3good_tot = 0;
int  nv3goodm_tot = 0;
int  nv4good_tot = 0;
int  nv4goodm_tot = 0;
int  nv5good_tot = 0;
int  nv5goodm_tot = 0;

//-------------------------------------------------------------------
TGenPhaseSpace *K3Pi(float p=3.)
{
  // K->3Pi decay phase space

  double e=TMath::Sqrt(p*p+mK*mK);
  TLorentzVector K(0,0,p,e);
  Double_t masses[20];
  for (int i=0; i<20; i++)
  {
    masses[i] = 0;
    if (i < npi) masses[i] = mPi;
    charges[i] = 0;
    if (i < nuse) charges[i] = 1;
  }
  charges[npi] = 1;
  if (neutral_primary) charges[npi] = 0;
  TGenPhaseSpace *psp = new TGenPhaseSpace();
  psp->SetDecay(K, npi, masses);
  return psp;
}

TGenPhaseSpace *pDecay = 0;

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
void gv(int n=1, int nve=100, int nback1=0, int nback2=0)
{
  // main steering routine for K->3Pi vertex generation&reconstruction test

  if (rec_type > 0 && vert_type == 0 && nve > 1)
  {
    printf("Wrong combination of conditions - fixed vertex position\n");
    printf("with multiple vertexes in volume\n");
    return;
  }
  if ( npi < 2 || npi > 20 || nuse > npi)
  {
    printf("Wrong combination of kinematic conditions:\n");
    printf(" %d secondary pions, tracking for %d pions \n", npi, nuse);
    return;
  }
  timespec_t t0, t;
  double dt;
  char outfile[128];

  sprintf(outfile,"%db_%dK%dpi_%dub_%dtb", n, nve, nuse, nback1, nback2);
  f = fopen(strcat(outfile,".out"),"w");
  fprintf(f,"Monte-Carlo for %d brick(s) with %d K-%dpi decays, kaon momentum %.1f GeV/c\n",
	     n, nve, nuse, momentumK);
  if (nback1)
    fprintf(f,"%d uncorrelated segments per pattern, acceptance %.2f rad\n",
	        nback1, back1_tetamax);
  else
    fprintf(f,"No uncorrelated segments background\n");
  if (nback1)
    fprintf(f,"%d background tracks per volume, acceptance %.2f rad, momentum %.2f-%.2f GeV/c\n",
	        nback2, back2_tetamax, back2_plim[0], back2_plim[1]);
  else
    fprintf(f,"No stright tracks  background\n");
  fprintf(f,"%d plates (thickness %4.1f mm) per brick, working area %.1fx%.1f mm2\n",
	     brick.plates, brick.dz/1000., (brick.lim[2]-brick.lim[0])/1000.,
	     (brick.lim[3]-brick.lim[1])/1000.);
  fprintf(f,"Basetrack measurement errors:\n  %6.1f microns (X) , %6.1f microns (Y)\n",
	     seg_s[0], seg_s[1]);
  fprintf(f,"  %6.4f mrad    (TX), %6.4f mrad    (TY)\n",
	     seg_s[2], seg_s[3]);
  fprintf(f,"Track momentum relative error %d%%\n",
	     (int)(dpp*100.));
  if (eloss_flag == 0)
    fprintf(f,"No energy losses simulation during particle tracking.\n");
  else if (eloss_flag == 1)
    fprintf(f,"Take into account average energy losses.\n");
  else if (eloss_flag == 2)
    fprintf(f,"Simulate energy losses according Landau distribution.\n");
  fprintf(f,"Track segment registration un-efficiency %d%%\n",
	     (int)(ProbGap*100.));
  fprintf(f,"Minimal Chi2 probability for segment attachment %6.4f\n",
	     ProbMinP);
  fprintf(f,"Minimal number of track segments for vertexing %d\n",
	     nsegMin);
  fprintf(f,"Minimal Chi2 track  probability for vertexing %6.4f\n",
	     ProbMinT);
  fprintf(f,"Minimal Chi2 vertex probability for vertex registration %6.4f\n",
	     ProbMinV);
  fprintf(f,"-----------------------------------------------------------\n");
  ntall = nve*(nuse+1-neutral_primary)+nback2;

  nvg_tot = 0;
  ntrg_tot = 0;
  ntr_tot = 0;
  ntrgood_tot = 0;
  ntrgoodb_tot = 0;
  ntrgood_r_tot = 0;
  ntrgoodb_r_tot = 0;
  nv2good_tot = 0;
  nv2goodm_tot = 0;
  nv3good_tot = 0;
  nv3goodm_tot = 0;
  nv4good_tot = 0;
  nv4goodm_tot = 0;
  nv5good_tot = 0;
  nv5goodm_tot = 0;

  hlist.Clear();
  hld1.Clear();
  hld2.Clear();
  hld3.Clear();
  BookHistsV(nve);

  if (ali)
  {
    delete ali;
    ali = 0;
  }
  if (gener)
  {
    delete gener;
    gener = 0;
  }
  if (pDecay)
  {
    delete pDecay;
    pDecay = 0;
  }

  pDecay   = K3Pi(momentumK);
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

  TTimeStamp  ts; // = new TTimeStamp();
  t0=ts.GetTimeSpec();
  numev = 0;
  for(int i=1; i<=n; i++) {
    if (n>1) fprintf(f,"Brick %d\n",i);
    g1(nve,nback1,nback2);
    numev++;
    if (!((numev*nve)%PrintN))
    {
	TTimeStamp ts;
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
    fprintf(f,"-----------------------------------------------------------\n");
  }

//  DrawHistsV();
  int nprong = nuse + 1 - neutral_primary;
  int ngv    = nve*n;
  int ntrgv  = nprong*ngv;
  int ntrgb  = nback2*n;
  float tr_eff = (float)ntr_tot/(ntrgv+ntrgb);
  printf("Track finding efficiency %6.1f\n",
	     tr_eff*100.);
  fprintf(f,"Track finding efficiency %6.1f\n",
	     tr_eff*100.);
  if (ntrgv)
  {
    float trgood_eff = (float)ntrgood_tot/ntrgv;
    printf("Vertex Track finding efficiency %6.1f\n",
	     trgood_eff*100.);
    fprintf(f,"Vertex Track finding efficiency %6.1f\n",
	     trgood_eff*100.);
  }
  if (ntrgb)
  {
    float trgoodb_eff = (float)ntrgoodb_tot/ntrgb;
    printf("Background Track finding efficiency %6.1f\n",
	     trgoodb_eff*100.);
    fprintf(f,"Background Track finding efficiency %6.1f\n",
	     trgoodb_eff*100.);
  }
  if (ntrgv)
  {
    float trgood_r_eff = (float)ntrgood_r_tot/ntrgv;
    printf("Vertex Track finding efficiency %6.1f (right segs >= %.1f%%)\n",
	     trgood_r_eff*100., RightRatioMin*100.);
    fprintf(f,"Vertex Track finding efficiency %6.1f (right segs >= %.1f%%)\n",
	     trgood_r_eff*100., RightRatioMin*100.);
  }
  if (ntrgb)
  {
    float trgoodb_r_eff = (float)ntrgoodb_r_tot/ntrgb;
    printf("Background Track finding efficiency %6.1f (right segs >= %.1f%%)\n",
	     trgoodb_r_eff*100., RightRatioMin*100.);
    fprintf(f,"Background Track finding efficiency %6.1f (right segs >= %.1f%%)\n",
	     trgoodb_r_eff*100., RightRatioMin*100.);
  }
  int n2gv = ngv;
  if (nprong == 3) n2gv = 3*ngv;
  if (nprong == 4) n2gv = 6*ngv;
  if (nprong == 5) n2gv = 10*ngv;
  if (n2gv)
  {
    float v2good_eff = (float)nv2good_tot/n2gv;
    printf("2-Track vertexes finding efficiency %6.1f\n",
	     v2good_eff*100.);
    fprintf(f,"2-Track vertexes finding efficiency %6.1f\n",
	     v2good_eff*100.);
    float v2goodm_eff = (float)nv2goodm_tot/n2gv;
    printf("Right 2-Track vertexes finding efficiency %6.1f\n",
	     v2goodm_eff*100.);
    fprintf(f,"Right 2-Track vertexes finding efficiency %6.1f\n",
	     v2goodm_eff*100.);
  }
  if (nprong == 3 && ngv)
  {
    float v3good_eff = (float)nv3good_tot/ngv;
    printf("3-Track vertexes finding efficiency %6.1f\n",
	     v3good_eff*100.);
    fprintf(f,"3-Track vertexes finding efficiency %6.1f\n",
	     v3good_eff*100.);
    float v3goodm_eff = (float)nv3goodm_tot/ngv;
    printf("Right 3-Track vertexes finding efficiency %6.1f\n",
	     v3goodm_eff*100.);
    fprintf(f,"Right 3-Track vertexes finding efficiency %6.1f\n",
	     v3goodm_eff*100.);
  }
  if (nprong == 4 && ngv)
  {
    float v4good_eff = (float)nv4good_tot/ngv;
    printf("4-Track vertexes finding efficiency %6.1f\n",
	     v4good_eff*100.);
    fprintf(f,"4-Track vertexes finding efficiency %6.1f\n",
	     v4good_eff*100.);
    float v4goodm_eff = (float)nv4goodm_tot/ngv;
    printf("Right 4-Track vertexes finding efficiency %6.1f\n",
	     v4goodm_eff*100.);
    fprintf(f,"Right 4-Track vertexes finding efficiency %6.1f\n",
	     v4goodm_eff*100.);
  }
  if (nprong == 5 && ngv)
  {
    float v5good_eff = (float)nv5good_tot/ngv;
    printf("5-Track vertexes finding efficiency %6.1f\n",
	     v5good_eff*100.);
    fprintf(f,"5-Track vertexes finding efficiency %6.1f\n",
	     v5good_eff*100.);
    float v5goodm_eff = (float)nv5goodm_tot/ngv;
    printf("Right 5-Track vertexes finding efficiency %6.1f\n",
	     v4goodm_eff*100.);
    fprintf(f,"Right 5-Track vertexes finding efficiency %6.1f\n",
	     v5goodm_eff*100.);
  }
  fclose(f);
  f = 0;
  sprintf(outfile,"%db_%dK%dpi_%dub_%dtb", n, nve, nuse, nback1, nback2);
  rf = new TFile(strcat(outfile,".root"),"RECREATE","MC Vertex reconstruction");
  hlist.Write();
  rf->Close();
  delete rf;
  rf = 0;
}


///------------------------------------------------------------
void g1(int nv=1, int nb1=0, int nb2=0)
{
  //generation of 1 pattern volume
  float vlim[4], vzlim[2];
  
  if (gener)
  {
    if (gener->eVTX) gener->eVTX->Delete();
    if (gener->eTracks) gener->eTracks->Delete();
    gener->eVTX = new TObjArray(1000);
  }
  if (ali)
  {
    if (ali->eVTX) ali->eVTX->Delete();
    if (ali->eTracks) ali->eTracks->Delete();
    ali->ResetCouples();
    int npat=ali->Npatterns();
    TClonesArray *segs = 0;
    for(int i=0; i<npat; i++ )
	if ( segs = (ali->GetPattern(i))->GetSegments()) segs->Delete();
    ali->eVTX = new TObjArray(1000);
  }

  vlim[0] = brick.lim[0] + fiducial_border;
  vlim[1] = brick.lim[1] + fiducial_border;
  vlim[2] = brick.lim[2] - fiducial_border;
  vlim[3] = brick.lim[3] - fiducial_border;
  vzlim[0] = fiducial_border_z;
  vzlim[1] = brick.plates*brick.dz - fiducial_border_z;

  if (nv) 
	gener->GeneratePhaseSpaceEvents( nv, pDecay,       vzlim,
				         vlim,             seg_s,
				         ProbGap,          eloss_flag,
				         charges);
  if (rec_type >= 2 && nb1) 
	gener->GenerateUncorrelatedSegments(nb1, brick.lim, seg_s,
					    back1_tetamax, -30 );
  if (rec_type >= 1 && nb2)
	gener->GenerateBackgroundTracks(nb2, brick.lim, back2_plim,
				        seg_s,     back2_tetamax,
				        ProbGap,   eloss_flag );
  FillHistsGen();


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
    
	hp[19]->Fill(DE_FIT-DE_MC);
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
  fprintf(f," %6d generated vertexes\n", nvg);
  fprintf(f," %6d generated tracks\n", ntr);

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
    
    hp[19]->Fill(DE_FIT-DE_MC);
  }

  printf(" %6d tracks found after track selection\n", ntrgood);
  printf("        ( %4d tracks with nseg < %d )\n", ntr_nmin, nsegMin);
  printf("        ( %4d tracks with p    < %6.3f GeV/c)\n", ntr_pmin, pMin);
  printf("        ( %4d tracks with prob < %6.3f )\n", ntr_prob, ProbMinT);
  fprintf(f," %6d tracks found after track selection\n", ntrgood);
  fprintf(f,"        ( %4d tracks with nseg < %d )\n", ntr_nmin, nsegMin);
  fprintf(f,"        ( %4d tracks with p    < %6.3f GeV/c)\n", ntr_pmin, pMin);
  fprintf(f,"        ( %4d tracks with prob < %6.3f )\n", ntr_prob, ProbMinT);

  bool usemom = false;
  if ( neutral_primary && (nuse == npi) ) usemom = true; 
  int nvtx = ali->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);

  printf(" %d vertexes found\n", nvtx);

  int nadd = 0;
  int nprong = nuse + 1 - neutral_primary;
  
  if ( nprong > 2 ) nadd = ali->ProbVertex3(ProbMinV3);

  int nvgood = 0;
  int nvgoodm = 0;
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
		    if (v->ntracks() == nprong)
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
			    nvgoodm++;
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
  printf(" %d(%d) real vertexes found (%d matched to MC)\n", nvgood, nadd, nvgoodm);
  fprintf(f," %d(%d) real vertexes found (%d matched to MC)\n", nvgood, nadd, nvgoodm);

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


  int nvg = 0;
  if (gener->eVTX) nvg = gener->eVTX->GetEntries();
  int ntrg = 0;
  if (gener->eTracks) ntrg = gener->eTracks->GetEntries();

  printf("%6d generated vertexes\n", nvg);
  printf("%6d generated tracks\n", ntrg);
  fprintf(f,"%6d generated vertexes\n", nvg);
  fprintf(f,"%6d generated tracks\n", ntrg);

  nvg_tot += nvg;
  ntrg_tot += ntrg;

  ali->Link();
  printf("link ok\n");
  ali->FillTracksCell();

  ali->MakeTracks();

  int itrg = 0;
  int nsegmatch = 0;
  int ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();

  for (int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(ali->eTracks->At(itr));
    if (tr->Flag()<0)
    {
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
	}
    }
  }

  fprintf(f," %6d tracks found after track making\n", ntr);

  ali->FitTracks(-1., -1., gener->eTracks);
  ali->FillCell(50,50,0.015,0.015);
  ali->PropagateTracks(brick.plates-1,2,ProbMinP);

  ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();

  int ntrgood = 0;
  int ntrgood_r = 0;
  int negflag = 0;
  int notmatched = 0;
  int nreject_prob = 0;
  int nreject_nseg = 0;
  int ntrgoodb = 0;
  int ntrgoodb_r = 0;
  double right_ratio = 0.;

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
		right_ratio = (double)nsegmatch/tr->N();
		if (right_ratio >= RightRatioMin)
		{
		    ntrgoodb_r++;
		}
		hp[16]->Fill(right_ratio);

	    }
	    else if (trg->Flag() <= nvg) // track at vertex
	    {
		ntrgood++;
		right_ratio = (double)nsegmatch/tr->N();
		if (right_ratio >= RightRatioMin)
		{
		    ntrgood_r++;
		}
		hp[15]->Fill(right_ratio);
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
	hp[19]->Fill(DE_FIT-DE_MC);
	hp[17]->Fill(tr->Prob());
    }
  }

  printf("  %6d tracks found after propagation\n", ntr-negflag);
  printf("  %6d matched tracks found (at vertexes), %6d with right segs >= %.0f%%\n", ntrgood, ntrgood_r, RightRatioMin*100.);
  printf("  %6d matched tracks found (background ), %6d with right segs >= %.0f%%\n", ntrgoodb, ntrgoodb_r, RightRatioMin*100.);
//  printf("        ( %4d tracks with negative flag)\n", negflag);
  printf("         ( %4d tracks not matched with generated ones)\n", notmatched);
  printf("         ( %4d tracks with nseg < %d )\n", nreject_nseg, nsegMin);
  printf("         ( %4d tracks with prob < %6.3f )\n", nreject_prob, ProbMinT);
  fprintf(f,"  %6d tracks found after propagation\n", ntr-negflag);
  fprintf(f,"  %6d matched tracks found (at vertexes), %6d with right segs >= %.0f%%\n", ntrgood, ntrgood_r, RightRatioMin*100.);
  fprintf(f,"  %6d matched tracks found (background ), %6d with right segs >= %.0f%%\n", ntrgoodb, ntrgoodb_r, RightRatioMin*100.);
//  fprintf(f,"        ( %4d tracks with negative flag)\n", negflag);
  fprintf(f,"         ( %4d tracks not matched with generated ones)\n", notmatched);
  fprintf(f,"         ( %4d tracks with nseg < %d )\n", nreject_nseg, nsegMin);
  fprintf(f,"         ( %4d tracks with prob < %6.3f )\n", nreject_prob, ProbMinT);

  ntr_tot += ntr-negflag;
  ntrgood_tot += ntrgood;
  ntrgoodb_tot += ntrgoodb;
  ntrgood_r_tot += ntrgood_r;
  ntrgoodb_r_tot += ntrgoodb_r;



  if (hp[10]) hp[10]->Fill(ntr-negflag);

  bool usemom = false;
  if ( neutral_primary && (nuse == npi) ) usemom = true; 

  int nvtx = ali->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);

  int nvgood = 0;
  int nvgoodm = 0;
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
		    if (v->ntracks() == 2)
		    {
			nvgood++;
			if ( local_coordinates )
			{
			    int ivg0 = (edbv->GetTrack(0)->GetSegmentsFlag(nsegmatch));
			    if (ivg0 >= 0)
			        ivg0 = ((EdbTrackP*)(gener->eTracks->At(ivg0)))->Flag();
			    ivg = ivg0 + 1;
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				int ivg = (edbv->GetTrack(j)->GetSegmentsFlag(nsegmatch));
				if (ivg >= 0)
			    	    ivg = ((EdbTrackP*)(gener->eTracks->At(ivg)))->Flag();
				else
				    break;
				if (ivg != ivg0) break;
			    }
			    if (ivg != ivg0) continue;
			    nvgoodm++;
			}	
		    }
		}
	    }
	}
    }
  printf("%d 2-track vertexes found (%d matched to MC)\n", nvgood, nvgoodm);
  fprintf(f,"%d 2-track vertexes found (%d matched to MC)\n", nvgood, nvgoodm);

  nv2good_tot += nvgood;
  nv2goodm_tot += nvgoodm;

  int nadd = 0;
  int nprong = nuse + 1 - neutral_primary;
  
  if ( nprong == 3 ) nadd = ali->ProbVertex3(ProbMinV3);
  if ( nprong == 4 ) nadd = ali->ProbVertex4(ProbMinV4);

  nvgood = 0;
  nvgoodm = 0;
  ivg = 0;
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
		    if (v->ntracks() == nprong)
		    {
			nvgood++;
			if ( local_coordinates )
			{
			    tr = edbv->GetTrack(0);
			    int ivg0 = -1;
			    if(tr) ivg0 = tr->GetSegmentsFlag(nsegmatch);
			    if (ivg0 >= 0)
			        ivg0 = ((EdbTrackP*)(gener->eTracks->At(ivg0)))->Flag();
			    ivg = ivg0 + 1;
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				tr = edbv->GetTrack(j);
				int ivg = -1;
				if(tr) ivg = tr->GetSegmentsFlag(nsegmatch);
				if (ivg >= 0)
			    	    ivg = ((EdbTrackP*)(gener->eTracks->At(ivg)))->Flag();
				else
				    break;
				if (ivg != ivg0) break;
			    }
			    if (ivg != ivg0) continue;
			    nvgoodm++;
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
  if (nprong == 3)
  {
    printf("%d 3-tracks vertexes found (%d matched to MC)\n", nvgood, nvgoodm);
    fprintf(f,"%d 3-tracks vertexes found (%d matched to MC)\n", nvgood, nvgoodm);
    nv3good_tot += nvgood;
    nv3goodm_tot += nvgoodm;
  }
  if (nprong == 4)
  {
    printf("%d 4-tracks vertexes found (%d matched to MC)\n", nvgood, nvgoodm);
    fprintf(f,"%d 4-tracks vertexes found (%d matched to MC)\n", nvgood, nvgoodm);
    nv4good_tot += nvgood;
    nv4goodm_tot += nvgoodm;
  }
}

///-----------------------------------------------------------------------
void BookHistsV(int nve)
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_Dx","Vertex X error",100,-5.,5.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_Dy","Vertex Y error",100,-5.,5.);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_Dz","Vertex Z error",50,-250.,250.);
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_Sx","Vertex X sigma",100,0.,10.);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_Sy","Vertex Y sigma",100,0.,10.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_Sz","Vertex Z sigma",100,0.,500.);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",25,0.,5.);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",26,0.,1.04);
  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Mass","Vertex Mass error",50,-1.000,+1.000);
  char title[128];
  sprintf(title,"Number of reconstructed vertexs (%d generated)",nve);
  if (!hp[9]) hp[9] = new TH1F("Hist_Vt_Nvert",title,nve*2,0.,(float)(nve*2));
  sprintf(title,"Number of reconstructed tracks (%d generated)",ntall);
  if (!hp[10]) hp[10] = new TH1F("Hist_Vt_Ntrack",title,ntall*2,0.,(float)(ntall*2));
  if (!hp[11]) hp[11] = new TH1F("Hist_Vt_Nsegs","Number of track segments",brick.plates+1,0.,(float)(brick.plates+1));
  if (!hp[12]) hp[12] = new TH1F("Hist_Vt_Dtrack","Track DeltaX, microns",100,-5.,+5.);
  if (!hp[13]) hp[13] = new TH1F("Hist_Vt_NsegsG","Number of generated track segments",brick.plates+1,0.,(float)(brick.plates+1));
  if (!hp[19]) hp[19] = new TH1F("Hist_Vt_DE","DE(MC)-DE",100,-0.25,+0.25);
  if (!hp[14]) hp[14] = new TH1F("Hist_Vt_Dist","RMS track-vertex distance",100, 0.,50.);
  if (!hp[15]) hp[15] = new TH1F("Hist_Vt_Match","Right segments fraction (tracks at vertexes)",110, 0.,1.1);
  if (!hp[16]) hp[16] = new TH1F("Hist_Vt_Matchb","Right segments fraction (backgound tracks) ",110, 0.,1.1);
  if (!hp[17]) hp[17] = new TH1F("Hist_Vt_Trprob","Track probability",110, 0.,1.1);
  if (!hp[18]) hp[18] = new TH1F("Hist_Vt_AngleV","RMS track-track angle, mrad", 100, 0.,1000.);
  if (!hp[20]) hp[20] = new TH1F("Hist_Vt_Angle","Track angle, mrad", 100, 0.,1000.);
  if (!hp[21]) hp[21] = new TH1F("Hist_Vt_Momen","Track momentum, GeV/c", 100, 0.,20.);

  hld1.Add(hp[0]);
  hld1.Add(hp[1]);
  hld1.Add(hp[2]);
  hld1.Add(hp[3]);
  hld1.Add(hp[4]);
  hld1.Add(hp[5]);
  hld1.Add(hp[6]);
  hld1.Add(hp[7]);
  hld1.Add(hp[8]);

  hld2.Add(hp[11]);
  hld2.Add(hp[15]);
  hld2.Add(hp[12]);
  hld2.Add(hp[13]);
  hld2.Add(hp[16]);
  hld2.Add(hp[19]);
  hld2.Add(hp[17]);
  hld2.Add(hp[20]);
  hld2.Add(hp[21]);

  hld3.Add(hp[9]);
  hld3.Add(hp[10]);
  hld3.Add(hp[14]);
  hld3.Add(hp[18]);

  for (int i=0; i<22; i++)
    if (hp[i]) hlist.Add(hp[i]);
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
  if ( (nuse == npi) && neutral_primary )
  {
    double rmass = v.mass(smass);
    hp[8]->Fill(rmass-mK);
  }
  hp[14]->Fill(v.rmsDistAngle());
  hp[18]->Fill(v.angle()*1000.);
}
///-----------------------------------------------------------------------
void FillHistsGen()
{
  // tracks and vertex reconstruction (KF fitting only without track finding)

  double xg, yg, zg, txg, tyg, pg, ang;
  EdbTrackP *tr;
  EdbVertex *vedbg = 0;

  int nv = 0;
  if(gener->eVTX) nv = gener->eVTX->GetEntries();
  for(int i=0; i<nv; i++) {
    vedbg = (EdbVertex *)(gener->eVTX->At(i));
    int nt = vedbg->N();
    for(int ip=0; ip<nt; ip++) {
	tr = vedbg->GetTrack(ip);
        xg = tr->X();
	yg = tr->Y();
        zg = tr->Z();
        txg = tr->TX();
	tyg = tr->TY();
        pg  = tr->P();
	ang = TMath::ACos(1./(1.+txg*txg+tyg*tyg))*1000.;
	if (hp[20]) hp[20]->Fill(ang);
	if (hp[21]) hp[21]->Fill(pg);
    }
  }
  int nt = 0;
  if (gener->eTracks) nt = gener->eTracks->GetEntries();
  for(int i=0; i<nt; i++) {
	tr = (EdbTrackP *)(gener->eTracks->At(i));
        xg = tr->X();
	yg = tr->Y();
        zg = tr->Z();
        txg = tr->TX();
	tyg = tr->TY();
        pg  = tr->P();
	ang = TMath::ACos(1./(1.+txg*txg+tyg*tyg))*180./TMath::Pi();
	if (hp[13]) hp[13]->Fill(tr->N());
  }
}

///-----------------------------------------------------------------------
void DrawHistsV()
{
  TCanvas *cv1 = new TCanvas("Vertex_reconstruction_1","MC Vertex reconstruction", 760, 760);
  int n = hld1.GetEntries();
  if (n < 2)
  {
  }
  else if (n < 3)
  {
    cv1->Divide(1,2);    
  }
  else if (n < 4)
  {
    cv1->Divide(1,3);    
  }
  else if (n < 5)
  {
    cv1->Divide(2,2);    
  }
  else if (n < 6)
  {
    cv1->Divide(2,3);    
  }
  else if (n < 7)
  {
    cv1->Divide(2,3);    
  }
  else if (n < 8)
  {
    cv1->Divide(2,4);    
  }
  else if (n < 9)
  {
    cv1->Divide(2,4);    
  }
  else if (n < 10)
  {
    cv1->Divide(3,3);    
  }
  else if (n < 11)
  {
    cv1->Divide(2,5);    
  }
  else
  {
    cv1->Divide(3,5);    
  }
  for(int i=0; i<n; i++) {
    cv1->cd(i+1);
    if (hld1.At(i)) hld1.At(i)->Draw();
  }

  TCanvas *cv2 = new TCanvas("Vertex_reconstruction_2","Vertex reconstruction (tracks hists)", 600, 760);
  int n = hld2.GetEntries();
  if (n < 2)
  {
  }
  else if (n < 3)
  {
    cv2->Divide(1,2);    
  }
  else if (n < 4)
  {
    cv2->Divide(1,3);    
  }
  else if (n < 5)
  {
    cv2->Divide(2,2);    
  }
  else if (n < 6)
  {
    cv2->Divide(2,3);    
  }
  else if (n < 7)
  {
    cv2->Divide(2,3);    
  }
  else if (n < 8)
  {
    cv2->Divide(2,4);    
  }
  else if (n < 9)
  {
    cv2->Divide(2,4);    
  }
  else if (n < 10)
  {
    cv2->Divide(3,3);    
  }
  else if (n < 11)
  {
    cv2->Divide(2,5);    
  }
  else
  {
    cv2->Divide(3,5);    
  }
  for(int i=0; i<n; i++) {
    cv2->cd(i+1);
    if (hld2.At(i)) hld2.At(i)->Draw();
  }

  TCanvas *cv3 = new TCanvas("Vertex_reconstruction_3","Vertex reconstruction (eff hists)", 600, 760);
  n = hld3.GetEntries();
  if (n < 2)
  {
  }
  else if (n < 3)
  {
    cv3->Divide(1,2);    
  }
  else if (n < 4)
  {
    cv3->Divide(1,3);    
  }
  else if (n < 5)
  {
    cv3->Divide(2,2);    
  }
  else if (n < 6)
  {
    cv3->Divide(2,3);    
  }
  else if (n < 7)
  {
    cv3->Divide(2,3);    
  }
  else if (n < 8)
  {
    cv3->Divide(2,4);    
  }
  else if (n < 9)
  {
    cv3->Divide(2,4);    
  }
  else if (n < 10)
  {
    cv3->Divide(3,3);    
  }
  else if (n < 11)
  {
    cv3->Divide(2,5);    
  }
  else
  {
    cv3->Divide(3,5);    
  }
  for(int i=0; i<n; i++) {
    cv3->cd(i+1);
    if (hld3.At(i)) hld3.At(i)->Draw();
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
///------------------------------------------------------------
void d() { DrawHistsV();}
