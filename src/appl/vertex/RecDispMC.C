//------------------------------------------------------------------
// 
// Usage example:
//
// root [0] .L RecDispMC.C
// root [1] gv(1,200,5,0.2);   // gv(nVol,nVtx,BG,Cosm)
//                             // generate nVol volumes with nVtx vertexes,
//                             // with BG/mm^2/plate uncorellated basetracks
//                             // with Cosm/mm^2 cosmic tracks per brick
//
// root [2] d()     - draw plots
// root [3] dsv()   - draw vertexes
// root [4] dst()   - draw tracks
// root [5] dsall() - draw everything
//
//  volume dimensions are defined by BRICK structure
//  The "fiducial volume" is used for the vertex, cosmics and BG generation
//  it's defined by fiducial_border and fiducial_border_z parameters
//------------------------------------------------------------------

bool  mc_make_tracks  = true;	     // build tracks from MC information
bool  use_mc_momentum = true;        // use or not MC track momentum
bool  use_mc_mass = false;           // use MC particle mass - or pion mass
int   primary_vertex_ntracks_min = 2;     // min generated vertex multiplicity
int   rec_primary_vertex_ntracks_min = 2; // min reconstructed vertex multiplicity
bool  read_from_file = false;         // read events from file
bool  regenerate_vertex_xyz = true;  // vertex coordinates regeneration
				     // (when readevents from file)
bool  select_neighborhood = false;   // call vertexes neighborhood procedure
float back1_tetamax =  0.35;         // angular range for BG segments
float back2_tetamax =  0.25;         // angular range for cosmics
float back2_plim[2] = {1., 1.};    // momentum range for cosmics

float fiducial_border = 50000.;      // Working brick area is:
float fiducial_border_z = 12000.;    // xmin = brick.xmin + fiducial_border
				     // xmax = brick.xmax - fiducial_border
				     // ymin = brick.ymin + fiducial_border
				     // ymax = brick.ymax - fiducial_border
				     // zmin = fiducial_border_z
				     // zmax = brick.zmax - fiducial_border_z
				     // note: tracks orinated from this
				     // fiducial volume not limited by this
				     // volume, but total brick volume
int neutral_primary = 1;  // 1 if neutral primary (phase space generation)
int npi = 4;   // number of secondary pions (phase space generation)
int nuse = 2;  // number of "visible" secondary pions (phase space generation)
float momentumK=15.; // momentum of K-meson (or compound object)
//double mK=.4937;   // mass of K-meson
double mK=5.6;     // mass of compound  object similar to neutrino events kinematics
double mPi=.139;   // mass of Pi-meson
double mP=0.938;   // mass of Proton

float dpp = 0.20;   // average dp/P 
float seg_s[4]={1.2, 1.2, 0.0015, 0.0015}; //sx,sy,stx,sty
float seg_zero[4]={.0,.0,.0,.0}; //no measurement errors
float ProbGap = 0.10; //probability to have pattern hole

float RightRatioMin = 0.5; // minimal contamination of "right" segments in
			   // reconstructed track
int nsegMin = 2;  // minimal number of track segments for using track in vertexing

int maxgaps[6] = {0,3,3,6,3,6};  // track edges criteria cuts (2-track vertexes)
// -maxgap[0] <= PID[start] - PID[end]   <= maxgap[1] for starts-ends pairs
// -maxgap[2] <= PID[start] - PID[start] <= maxgap[2] for starts-starts pairs
// -maxgap[4] <= PID[end]   - PID[end]   <= maxgap[4] for ends-ends pairs
// if maxgap[0]<0 - ignore start-end pairs
// if maxgap[2]<0 - ignore start-start pairs
// if maxgap[4]<0 - ignore end-end pairs
// End-Start     z-vertex limits
//  	    zvmin = Min(Z-End ,Z-Start)
//	    zvmax = Max(Z-End ,Z-Start) + zBin
//	    zvmin < z-vertex < zvmax
// Start-Start   z-vertex limits
//  	    zvmax = Min(Z-Start1 ,Z-Start2) + zBin
//	    zvmin = zvmax - (maxgap[3]*zBin)
//	    zvmin < z-vertex < zvmax
// End-End       z-vertex limits
//  	    zvmin = Max(Z-End1 ,Z-End2)
//	    zvmax = zvmin + (maxgap[5]*zBin)
//	    zvmin < z-vertex < zvmax

float AngleAcceptance = 2.0;   // maximal difference between track slopes

float ProbMinV  = 0.01; //minimal chi2-probability for 2-tracks vertexes
float ProbMinVN = 0.01; //minimal chi2-probability for N-tracks vertexes
float ProbMinP  = 0.01; //minimal chi2-probability for tracks merging 
float ProbMinT  = 0.01; //minimal track chi2-probability for using in vertexing

int eloss_flag = 0; // =0 - no energy losses, 1-average energy losses, 2-Landau energy losses
int design = +0; // -1 - take into account energy losses in Kalman filter,
		 //      assuming particle direction opposit Z-axis
		 // +1 - take into account energy losses in Kalman filter,
		 //      assuming particle direction along Z-axis
		 //  0 - don't take into account energy losses in Kalman filter
struct BRICK {
  int   plates;                       // plates to be simulated
  float z0;                           // start from here
  float dz;                           // thickness of ene plate (Emu+Pb)
  float lim[4];                       // xmin,ymin,xmax,ymax
  float X0;			      // material radiation length
};

BRICK brick;

brick.plates =      58;
brick.z0     =      0.;
brick.dz     =   1290.;
brick.lim[0] = -60000.;
brick.lim[1] = -60000.;
brick.lim[2] =  60000.;
brick.lim[3] =  60000.;
brick.X0     =   5810.;


//---------------------------------------------------------------------------
// Fixed section (almost)

using namespace VERTEX;

const unsigned long int PrintN = 100; // printing frequency (events)

EdbPVRec    *ali=0;
EdbPVGen    *gener = 0;
EdbScanCond *scanCond=0;

// Segments, tracks and vertexes arrays (for display)

TObjArray *arrs   = new TObjArray();  
TObjArray *arrtr   = new TObjArray();  
TObjArray *arrv   = new TObjArray();  
TObjArray *arrs2   = new TObjArray();  
TObjArray *arrtr2   = new TObjArray();  
TObjArray *arrv2   = new TObjArray();  

// histograms pointers

TH1F *hp[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

EdbVertex *vedb;

int ntall = 0;      // estimated number of possible reconstructed tracks
int numbricks = 0;  // number of generated bricks

float vxyz[3] = {0., 0., 0.};                 // simulated vertex position
float vxo = 0., vyo = 0., vzo = 0.;
float vxg = 0., vyg = 0., vzg = 0.;
double DE_MC, DE_FIT;
int charges[101];

FILE *f=0;
FILE *fmcdata=0;
TFile *rf=0;

TObjArray hlist, hld1, hld2, hld3, hld4;

int  neg_tot = 0;   // total number of generated events
int  nvg_tot = 0;   // total number of generated vertexes
int  nvgm_tot[100];   // total number of generated vertexes for different
			// multiplicity
int  ntrg_tot = 0;  // total number of generated tracks (events + backg)
int  ntrgv_tot = 0;  // total number of generated tracks (events only)
int  ntrgb     = 0;  // total number of generated tracks (cosmic only)

int  negood_tot = 0;   // total number of reconstructed events (primary vtx found)
int  ntr_tot = 0;   // total number of reconstructed tracks (w/o broken)
int  ntrgood_tot = 0; // total number of reconstructed event tracks (after cuts),
		      //  matched to MC
int  ntrgoodb_tot = 0; // total number of reconstructed backg tracks (after cuts),
		       // matched to MC
int  ntrgood_r_tot = 0; // total number of reconstructed event tracks (after cuts),
		      //  matched to MC, good segments contamination > RightRatioMin
int  ntrgoodb_r_tot = 0; // total number of reconstructed backg tracks (after cuts),
		       // matched to MC, good segments contamination > RightRatioMin
int  nvgood_tot[100];   // total number of reconstructed vertexes for different
			// multiplicity
int  nvgoodm_tot[100];   // total number of reconstructed vertexes for different
			// multiplicity, matched to MC
int  nvgoodmg_tot[100];   // total number of reconstructed vertexes for different
			// generated multiplicity, matched to MC
int  nvgoodmt_tot = 0;   // total number of reconstructed vertexes, matched to MC
int  n2gv = 0;  // number ofgenerated 2-tracks combinations (vertexes)
int  nprongmax = 0;  // maximal reconstructed vertex maultiplicity
int  nprongmaxg = 0;  // maximal generated vertex maultiplicity
int  evcount = 0; // event count for reading from file
bool fmc1steof = true;

TGenPhaseSpace *pDecay = 0;

//-------------------------------------------------------------------
TGenPhaseSpace *K3Pi(float p=3.)
{
  // K->3Pi decay phase space

  double e=TMath::Sqrt(p*p+mK*mK);
  TLorentzVector K(0,0,p,e);
  Double_t masses[100];
  for (int i=0; i<100; i++)
  {
    masses[i] = 0;
    if (i < npi) masses[i] = mPi;
    if (i == 0) masses[i] = mP;
    charges[i] = 0;
    if (i < nuse) charges[i] = 1;
  }
  charges[npi] = 1;
  if (neutral_primary) charges[npi] = 0;
  TGenPhaseSpace *psp = new TGenPhaseSpace();
  psp->SetDecay(K, npi, masses);
  return psp;
}
///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack( EdbScanCond *cond )
{
  // sigma0 "x, y, tx, ty" at zero angle
  cond->SetSigma0( seg_s[0], seg_s[1], seg_s[2], seg_s[3] );
  cond->SetDegrad( 0.2 );          // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(20, 20, 10, 10);     // bins in [sigma] for checks
  cond->SetPulsRamp0(  15., 15. );   // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 15., 15. );
  cond->SetChi2Max( 3.5 );
  cond->SetChi2PMax( 3.5 );
  cond->SetRadX0( brick.X0 );

  cond->SetName("Prototype_OPERA_basetrack");
}
///------------------------------------------------------------
void gv(int n=1, int nve=100, float back1=0., float back2=0.)
{
  // main steering routine for vertex generation & reconstruction test
  char outfile[128];

  if ( !read_from_file && (npi < 2 || npi > 100 || nuse > npi) )
  {
    printf("Wrong combination of kinematic conditions:\n");
    printf(" %d secondary pions, tracking for %d pions \n", npi, nuse);
    return;
  }

  timespec_t t0, t;
  double dt;

  if (read_from_file)
  {
    fmcdata = fopen("vertexes_and_tracks.data","r");
    rewind(fmcdata);
    ntall = 10*nve;
  }
  else
  {
    float area = (brick.lim[2]-brick.lim[0]-2.*fiducial_border)*
		 (brick.lim[3]-brick.lim[1]-2.*fiducial_border)/1000000.;
    ntall = nve*(nuse+1-neutral_primary)+back2*area;
  }

  // log MC conditions on stdout and logfile
  print_conditions(n, nve, back1, back2);

  // clear variables and objects
  clear_all();

  // histogram booking
  BookHistsV(nve);

  // Phase Space Generator initialization
  pDecay   = K3Pi(momentumK);
  // Create PVRec object
  ali      = new EdbPVRec();
  // Get Patterns Volume pointer
  EdbPatternsVolume *vol=(EdbPatternsVolume*)ali;
  // Make patterns
  makeVolumeOPERAn(brick,vol);
  // Create Vertexes TObjArray in PVRec
  ali->eVTX = new TObjArray(1000);
  // Create Scan Conditions object
  scanCond = new EdbScanCond();
  // Set Scan conditions variables
  Set_Prototype_OPERA_basetrack(scanCond);
  // Assign Scan Conditions to PVRec
  ali->SetScanCond(scanCond);

  ali->SetPatternsID();
  ali->SetCouplesAll();
  ali->SetChi2Max(scanCond->Chi2PMax());

  // Create PVGen object
  gener = new EdbPVGen();
  // Use the same volume as PVRec
  gener->SetVolume(vol);
  // Create Vertexes TObjArray in PVGen
  gener->eVTX = new TObjArray(1000);
  // Assign Scan Conditions to PVGen (the same as for PVRec)
  gener->SetScanCond(scanCond);

  TTimeStamp  ts; // Time Stamp for printing
  t0=ts.GetTimeSpec();

  int breakflag = 0;
  int print_at_end = 0;
  int nprong = nuse + 1 - neutral_primary;

  for(int i=1; i<=n; i++) {
    numbricks++;
    // generate one brick with:
    //		nve   events,
    //		back1 uncorrelated segments per mm^2 per plate,
    //		back2 cosmic tracks per mm^2,
    // return true if EOF conditions (when readevents from file)
    if (g1(nve,back1,back2)) breakflag = 1;
    // print time and statistic
    if ((!(nvg_tot%PrintN)) || breakflag || (i == n))
    {
	TTimeStamp ts;
	t=ts.GetTimeSpec();
	dt=(t.tv_sec-t0.tv_sec)*1000000000.+(t.tv_nsec-t0.tv_nsec);
	if ( !breakflag && (i != n) )
	{ 
	  if (nve>0&&back2<=0.)
	    printf("%d events generated in %.4f sec (%.1f msec per event)\n",
	    neg_tot,dt/1000000000.,dt/(double)(neg_tot)/1000000.);
	  else if (nve>0&&back2>0.)
	    printf("%d events and %d cosmic tracks generated in %.4f sec (%.1f msec per event)\n",
	    neg_tot,ntrgb, dt/1000000000.,dt/(double)(neg_tot)/1000000.);
	  else if (nve<=0&&back2>0.)
	    printf("%d cosmic tracks generated in %.4f sec (%.1f msec per track)\n",
	    ntrgb, dt/1000000000.,dt/(double)(ntrgb)/1000000.);
	}
	else
	{
	    print_at_end = 1;
	}
    }
    if (breakflag) break;
  }
  if (read_from_file)
  {
    fclose(fmcdata);
  }
  else
  {
    ntrgv_tot  = nprong*nvg_tot;
    nprongmaxg = nprong;
  }
  
  // log MC results on stdout and logfile
  print_results();
  // Store histogramms on ROOT file
  sprintf(outfile,"%db_%dK%dpi_%dub_%3.1ftb", n, nve, nuse, (int)back1, back2);
  rf = new TFile(strcat(outfile,".root"),"RECREATE","MC Vertex reconstruction");
  hlist.Write();
  rf->Close();
  delete rf;
  rf = 0;
  // print time and statistic
  if ( print_at_end )
  { 
    if (nve>0&&back2<=0.)
	printf("%d events generated in %.4f sec (%.1f msec per event)\n",
	neg_tot,dt/1000000000.,dt/(double)(neg_tot)/1000000.);
    else if (nve>0&&back2>0.)
	printf("%d events and %d cosmic tracks generated in %.4f sec (%.1f msec per event)\n",
	neg_tot,ntrgb, dt/1000000000.,dt/(double)(neg_tot)/1000000.);
    else if (nve<=0&&back2>0.)
	printf("%d cosmic tracks generated in %.4f sec (%.1f msec per track)\n",
	ntrgb, dt/1000000000.,dt/(double)(ntrgb)/1000000.);
  }
}
///------------------------------------------------------------
void print_conditions(int n, int nve, float back1, float back2)
{
  char outfile[128];

  sprintf(outfile,"%db_%dK%dpi_%dub_%3.1ftb", n, nve, nuse, (int)back1, back2);

  f = fopen(strcat(outfile,".out"),"w");

  if (read_from_file)
    fprintf(f,"Monte-Carlo for %d brick(s) with %d events from file vertexes_and_tracks.data\n", n, nve);
  else
    fprintf(f,"Monte-Carlo for %d brick(s) with %d K-%dpi decays, kaon momentum %.1f GeV/c\n",
	     n, nve, nuse, momentumK);
  if (back1)
    fprintf(f,"%d uncorrelated segments per mm^2 per pattern, acceptance %.2f rad\n",
	        (int)back1, back1_tetamax);
  else
    fprintf(f,"No uncorrelated segments background\n");
  if (back2)
    fprintf(f,"%3.1f cosmic tracks per volume, acceptance %.2f rad, momentum %.2f-%.2f GeV/c\n",
	        back2, back2_tetamax, back2_plim[0], back2_plim[1]);
  else
    fprintf(f,"No cosmic tracks  background\n");
  fprintf(f,"%3d plates (thickness %5.2f mm) per brick, total area %.1fx%.1f mm2\n",
	     brick.plates, brick.dz/1000., (brick.lim[2]-brick.lim[0])/1000.,
	     (brick.lim[3]-brick.lim[1])/1000.);
  fprintf(f,"                                           work  area %.1fx%.1f mm2\n",
	     (brick.lim[2]-brick.lim[0]-2.*fiducial_border)/1000.,
	     (brick.lim[3]-brick.lim[1]-2.*fiducial_border)/1000.);
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
  return;
}
///------------------------------------------------------------
void print_results()
{
  float tr_eff = (float)ntr_tot/(ntrgv_tot+ntrgb);
  printf("-----------------------------------------------------------\n");
  fprintf(f,"-----------------------------------------------------------\n");
  printf("Total  tracks finding efficiency %6.1f%%\n",
	     tr_eff*100.);
  fprintf(f,"Toatal  tracks finding efficiency %6.1f%%\n",
	     tr_eff*100.);
  if (ntrgv_tot)
  {
    float trgood_eff = (float)ntrgood_tot/ntrgv_tot;
    float trgood_r_eff = (float)ntrgood_r_tot/ntrgv_tot;
    printf("Event  tracks finding efficiency %6.1f%% ( %6.1f%% with right segs >= %.1f%%)\n",
	     trgood_eff*100., trgood_r_eff*100., RightRatioMin*100.);
    fprintf(f, "Event  tracks finding efficiency %6.1f%% ( %6.1f%% with right segs >= %.1f%%)\n",
	     trgood_eff*100., trgood_r_eff*100., RightRatioMin*100.);
  }
  if (ntrgb)
  {
    float trgoodb_eff = (float)ntrgoodb_tot/ntrgb;
    float trgoodb_r_eff = (float)ntrgoodb_r_tot/ntrgb;
    printf("Cosmic tracks finding efficiency %6.1f%% ( %6.1f%% with right segs >= %.1f%%)\n",
	     trgoodb_eff*100., trgoodb_r_eff*100., RightRatioMin*100.);
    fprintf(f, "Cosmic tracks finding efficiency %6.1f%% ( %6.1f%% with right segs >= %.1f%%)\n",
	     trgoodb_eff*100., trgoodb_r_eff*100., RightRatioMin*100.);
  }
  printf("------------------------------------------------------------\n");
  fprintf(f, "------------------------------------------------------------\n");
  if (!read_from_file)
  {
    int nprong = nuse + 1 - neutral_primary;
    n2gv = (nprong*(nprong-1)/2)*nvg_tot;
  }
  printf("Maximal generated vertex multiplicity %d tracks\n", nprongmaxg);
  fprintf(f, "Maximal generated vertex multiplicity %d tracks\n", nprongmaxg);
  printf("Maximal reconstructed vertex multiplicity %d tracks\n", nprongmax);
  fprintf(f, "Maximal reconstructed vertex multiplicity %d tracks\n", nprongmax);
  int nvgmsum = 0;
  int nvgoodmsum = 0;
  int nvgoodmgsum = 0;
  printf("------------------------------------------------------------\n");
  fprintf(f, "------------------------------------------------------------\n");
  for (int i=2; (i<=nprongmaxg) && nvg_tot; i++)
  {
    nvgmsum += nvgm_tot[i];
    nvgoodmsum += nvgoodm_tot[i];
    nvgoodmgsum += nvgoodmg_tot[i];
    if (nvgood_tot[i] <= 0) continue;
    float vgood_eff = (float)nvgood_tot[i]/nvg_tot;
    float vgooda_eff = 0.;
    if (nvgm_tot[i]) vgooda_eff = (float)nvgood_tot[i]/nvgm_tot[i];
    if (i>2) printf("-----\n");
    printf("Total %d-track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgood_eff*100., vgooda_eff*100.);
    fprintf(f,"Total %d-Track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgood_eff*100., vgooda_eff*100.);
    float vgoodm_eff = (float)nvgoodm_tot[i]/nvg_tot;
    vgooda_eff = 0.;
    if (nvgm_tot[i]) vgooda_eff = (float)nvgoodm_tot[i]/nvgm_tot[i];
    printf("Match %d-Track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgoodm_eff*100., vgooda_eff*100.);
    fprintf(f,"Match %d-Track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgoodm_eff*100., vgooda_eff*100.);
    float vgoodmg_eff = (float)nvgoodmg_tot[i]/nvg_tot;
    vgooda_eff = 0.;
    if (nvgm_tot[i]) vgooda_eff = (float)nvgoodmg_tot[i]/nvgm_tot[i];
    printf("Right %d-Track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgoodmg_eff*100., vgooda_eff*100.);
    fprintf(f,"Right %d-Track vertexes finding efficiency %6.1f%% (%6.1f%%)\n",
	     i, vgoodmg_eff*100., vgooda_eff*100.);
  }
  printf("------------------------------------------------------------\n");
  fprintf(f, "------------------------------------------------------------\n");
//  printf("nvg_tot = %d, nvgmsum = %d, nvgoodmsum = %d, nvgoodmt_tot = %d, nvgoodmgsum = %d\n",
//	  nvg_tot, nvgmsum, nvgoodmsum, nvgoodmt_tot, nvgoodmgsum);
  if (neg_tot)
  {
    printf("Primary vertex finding efficiency %6.1f%% (%6.1f%%)\n",
	    (float)negood_tot/neg_tot*100., (float)nvgoodmgsum/neg_tot*100.);
    fprintf(f, "Primary vertex finding efficiency %6.1f%% (%6.1f%%)\n",
	    (float)negood_tot/neg_tot*100., (float)nvgoodmgsum/neg_tot*100.);
  }
  printf("------------------------------------------------------------\n");
  fprintf(f, "------------------------------------------------------------\n");
  fclose(f);
  f = 0;
  return;
}
///------------------------------------------------------------
void clear_all()
{
  nvg_tot = 0;
  ntrg_tot = 0;
  ntrgb = 0;
  ntr_tot = 0;
  ntrgood_tot = 0;
  ntrgoodb_tot = 0;
  ntrgood_r_tot = 0;
  ntrgoodb_r_tot = 0;
  nvgoodmt_tot = 0;
  numbricks = 0;
  ntrgv_tot = 0;
  nprongmax = 0;
  nprongmaxg = 0;
  evcount = 0;
  for (int i=0; i<100; i++)
  {
    nvgm_tot[i] = 0;
    nvgood_tot[i] = 0;
    nvgoodm_tot[i] = 0;
    nvgoodmg_tot[i] = 0;
  }

  hlist.Clear();
  hld1.Clear();
  hld2.Clear();
  hld3.Clear();
  hld4.Clear();

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
  return;
}
///------------------------------------------------------------
int read_events_from_file(int ne, float *fractbkg, float vlim[4], float vzlim[2])
{
    int retval = 0;
    float VTx;
    float VTy;
    float VTz;
    float zlimt[2];
    EdbVertex *vedb = 0;
    EdbTrackP *tr = 0; 
    int numt = 0, ntrv = 0, indv, gcode, nitem;
    float TMass, Pt, TXt, TYt;
    int iiv = 0;
    while (iiv == 0)
    {
     for (int iv = 0; iv < ne; iv++)
     {
      // read vertex line: vert number, multiplicity, coordinates
      nitem = fscanf(fmcdata, "%d %d %f %f %f\n", &indv, &ntrv, &VTx, &VTy, &VTz);
      if (nitem <= 0 && fmc1steof && evcount == 0)
      {
        fmc1steof = false;
        rewind(fmcdata);
        // re-read vertex line: vert number, multiplicity, coordinates
        nitem = fscanf(fmcdata, "%d %d %f %f %f\n", &indv, &ntrv, &VTx, &VTy, &VTz);
        if (nitem <= 0)
        {
	    retval = 1;
	    *fractbkg = (float)evcount/ne;
	    iiv = -1;
	    break;
        }
      }
      else if ( nitem <= 0 )
      {
        retval = 1;
	*fractbkg = (float)evcount/ne;
	iiv = -1;
	break;
      }
      if (ntrv < primary_vertex_ntracks_min)
      {
        // skip event with low multiplicity
        for (int it = 0; it < ntrv; it++)
        {
	    fscanf(fmcdata, "%d %f %f %f %f\n", &gcode, &TMass, &Pt, &TXt, &TYt);
	}
	continue;
      }
      if (regenerate_vertex_xyz)
      {
        // generate vertex position into fiducial volume
	VTx = vlim[0] + (vlim[2] - vlim[0])*gRandom->Rndm();
	VTy = vlim[1] + (vlim[3] - vlim[1])*gRandom->Rndm();
	VTz = vzlim[0] + (vzlim[1] - vzlim[0])*gRandom->Rndm();
      }
      evcount++;
      vedb = new EdbVertex();
      vedb->SetXYZ(VTx, VTy, VTz);
      int ntrvreal = 0;
      for (int it = 0; it < ntrv; it++)
      {
    	    // read track parameters: Geant_code, mass, momentum, slopes 
	    fscanf(fmcdata, "%d %f %f %f %f\n", &gcode, &TMass, &Pt, &TXt, &TYt);
	    tr = new EdbTrackP();
	    tr->Set(numt++, VTx, VTy, TXt, TYt, 1, iiv+1);
	    tr->SetZ(VTz);
	    tr->SetP(Pt);
	    tr->SetM(TMath::Abs(TMass));
	    if (TMass >= 0.)
	    {
		zlimt[0] = VTz;
		zlimt[1] = 100000.;
	    }
	    else
	    {
		zlimt[0] = VTz;
		zlimt[1] = -1000.;
	    }
	    // generate segments
	    gener->TrackMC( zlimt, brick.lim, *tr, eloss_flag, ProbGap);
	    // reject tracks with one segment only
	    if (tr->N() >= 2)
	    {
		gener->AddTrack(tr);
		if (TMass < 0.)
		    vedb->AddTrack(tr, 0);
		else
		    vedb->AddTrack(tr, 1);
		ntrvreal++;
		ntrgv_tot++;
	    }
	    else
	    {
		if (tr->N())
		{
		    tr->GetSegment(0)->SetFlag(-1);
		}
		delete tr;
		tr = 0;
	    }
      }
      if (ntrvreal == 0)
      {
	// reject vertex with no "good" tracks
        delete vedb;
	vedb = 0;
      }
      else if (ntrvreal == 1)
      {
	// reject vertex with no one track only
        delete vedb;
	vedb = 0;
	if (indv == 1)
	{
	    // primary vertex
	    if (tr)
	    {
		ntrgv_tot--;
		gener->eTracks->Remove(tr);
		delete tr;
		tr = 0;
	    }
	}
	if (tr)
	{
	    tr->SetFlag(0);
	    ntrgv_tot--;
	}
      }
      else
      {
        // accept vertex
	vedb->SetFlag(indv);
        gener->AddVertex(vedb);
        iiv++;
	if (indv == 1) neg_tot++;
        n2gv += (ntrvreal*(ntrvreal-1)/2);
	nvgm_tot[ntrvreal]++;
        if (ntrvreal > nprongmaxg) nprongmaxg = ntrvreal;
      }
     }
     if (iiv < 0) break;
    }
    return retval;
}
///------------------------------------------------------------
int g1(int ne=1, float b1=0., float b2=0.)
{
  // generation of 1 pattern volume
  float vlim[4], vzlim[2];
  // delete previous generated tracks and vertexes
  if (gener)
  {
    if (gener->eVTX) gener->eVTX->Delete();
    if (gener->eTracks) gener->eTracks->Delete();
    gener->eVTX = new TObjArray(1000);
  }
  // delete previous reconstructed tracks and vertexes, generated segments
  if (ali)
  {
    ali->eVTA.Clear();
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
  // working area
  float area = (vlim[2]-vlim[0])*(vlim[3]-vlim[1])/1000000.;
  // number of background segments and tracks
  int nb1 = b1*area;
  int nb2 = b2*area;
  vzlim[0] = fiducial_border_z;
  vzlim[1] = brick.plates*brick.dz - fiducial_border_z;

  float fractbkg = 1.;
  int retval = 0;
// generate events (vertexes)
  if (read_from_file && ne)
  {
	retval = read_events_from_file(ne, &fractbkg, vlim, vzlim);
  }
  else if (ne)
  { 
	gener->GeneratePhaseSpaceEvents( ne, pDecay,       vzlim,    vlim,
				         brick.lim,        ProbGap,  eloss_flag,
					 charges);
	neg_tot+=ne;
	nvgm_tot[nuse + 1 - neutral_primary] += ne;
  }
  int nb1f, nb2f;
// generate uncorrelated segments
  if ((nb1f = (int)(nb1*fractbkg))) 
	gener->GenerateUncorrelatedSegments(nb1f, brick.lim, 
					    back1_tetamax, 0 );
// generate cosmictracks
  if ((nb2f = (int)(nb2*fractbkg)))
	gener->GenerateBackgroundTracks(nb2f,       vlim,      brick.lim,
					back2_plim, back2_tetamax,
				        ProbGap,    eloss_flag );
  ntrgb  += nb2f;
// fill hists with generated track parameters
  FillHistsGen();
// reconstruction procedure
  rec_all();

  return retval;
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
void rec_all()
{
  // tracks and vertex reconstruction (track finding and fitting, vertex finding and fitting)

  int numrect[10000];
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

  nvg_tot += nvg;
  ntrg_tot += ntrg;

  // link microtracks
  ali->Link();
  ali->FillTracksCell();

  printf("------------------------------------------------------------\n");
  printf("%d generated vertexes\n", nvg);
  printf("%d generated tracks\n", ntrg);
  fprintf(f,"  %6d generated vertexes\n", nvg);
  fprintf(f,"  %6d generated tracks\n", ntrg);

  // make track candidates
  if (mc_make_tracks)
  {
    ali->eTracks = new TObjArray();
    gener->MakeTracksMC(2,ali->eTracks);
  }
  else
    ali->MakeTracks();

  int itrg = 0;
  int nsegmatch = 0;
  int ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();
  // loop for tracks momentum and momentum error setting
  for (int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(ali->eTracks->At(itr));
    trg = 0;
    if (ntrg)
    {
	itrg = tr->GetSegmentsFlag(nsegmatch);
	if (itrg >= 0 && itrg < ntrg)
	{
	    trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	    p = trg->P();
	    if (dpp != 0.) tr->SetErrorP(p*p*dpp*dpp);
	    else           tr->SetErrorP(p*p*0.2*0.2);
	    if (use_mc_momentum)
	    {
		p = p*(1.+dpp*gRandom->Gaus());
		if (p < 0.050) p = 0.050;
	    }
	    else
	    {
		p = 0.;
	    }
	    tr->SetP(p);
	}
    }
  }

  fprintf(f,"  %6d tracks found after track making\n", ntr);
  
  float mpar = -1.;
  if (use_mc_mass)
    mpar = -1.;
  else
    mpar = 0.;
  // fit tracks
  ali->FitTracks(0., mpar, gener->eTracks, design);
  ali->FillCell(50,50,0.015,0.015);

  // merge tracks
  ali->PropagateTracks(brick.plates-1,2,ProbMinP);

  ntr = 0;
  if (ali->eTracks) ntr = ali->eTracks->GetEntries();

  // clear some counters
  int ntrgood = 0;
  int ntrgood_r = 0;
  int negflag = 0;
  int notmatched = 0;
  int nreject_prob = 0;
  int nreject_nseg = 0;
  int ntrgoodb = 0;
  int ntrgoodb_r = 0;
  double right_ratio = 0.;
  float pro = 0.;

  // loop on tracks for count statistic and hists filling

  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(ali->eTracks->At(itr));
    if (tr->Flag()<0)
    {
	// count "broken" tracks (at track merging procedure)
	negflag++;
	continue;
    }
    if (hp[11]) hp[11]->Fill(tr->N());
    if (tr->N()<nsegMin)
    {
	// count too short tracks
	nreject_nseg++;
	continue;
    }
    trg = 0;
    if (ntrg)
    {
	// find corresponding generated track
	itrg = tr->GetSegmentsFlag(nsegmatch);
	if (itrg >= 0 && itrg < ntrg)
	{
	    trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	}
    }
    if (trg && ((pro = tr->Prob()) < 0.9999))
    {
	// fill probability hist (reject "one-segment" tracks (too high MS)
	hp[17]->Fill(pro);
    }
    if (tr->Prob()<ProbMinT)
    {
//	ang = TMath::ACos(1./(1.+tr->TX()*tr->TX()+tr->TY()*tr->TY()))*1000.;
//	printf("Track ID = %d, N = %d Prob = %f X = %f Y = %f Z = %f\n      TX = %f TY = %f P = %f M = %f\n",
//	       tr->ID(), tr->N(), tr->Prob(), tr->X(), tr->Y(), tr->Z(), 
//	       tr->TX(), tr->TY(), tr->P(), tr->M()); 
//	hp[19]->Fill(ang);
	// count low probability tracks
	nreject_prob++;
	continue;
    }
    if (ntrg)
    {
	if (trg)
	{
	    if (trg->Flag() == 0)        // background track
	    {
		// count tracks matched to MC (cosmic)
		ntrgoodb++;
		right_ratio = (double)nsegmatch/tr->N();
		if (right_ratio >= RightRatioMin)
		{
		    // count tracks matched to MC with high "right" segments
		    // contamination (cosmic tracks)
		    ntrgoodb_r++;
		}
		hp[16]->Fill(right_ratio);

	    }
	    else if (trg->Flag() <= nvg) // track at vertex
	    {
		// count tracks matched to MC (tracks at vertexes)
		ntrgood++;
		right_ratio = (double)nsegmatch/tr->N();
		if (right_ratio >= RightRatioMin)
		{
		    // count tracks matched to MC with high "right" segments
		    // contamination (tracks at vertexes)
		    ntrgood_r++;
		}
		hp[15]->Fill(right_ratio);
	    }
	}
	else
	{
	    // count tracks not matched to MC
	    notmatched++;
	}
    }
    else
    {
	// count tracks not matched to MC
	notmatched++;
    }

//    printf(" tr # %d, N = %d P = %f\n", itr, tr->N(), tr->P());
//    if (tr->N() < nsegMin) continue;
//    if (tr->P() < 0.1) continue;


    // fill hist with difference between measured and fitted track coordinate
    if (hp[12]) hp[12]->Fill(tr->GetSegmentFirst()->X()-tr->GetSegmentFFirst()->X());

    if (trg != 0)
    {
	DE_MC = trg->DE();
	DE_FIT = tr->DE();
	// fill hist with difference between estimated and
	// generated  track energy losses
	hp[19]->Fill(DE_FIT-DE_MC);
	// fill hist with relative difference between MS-estimated and
	// generated  track momentum
//	hp[23]->Fill((tr->P_MS(brick.X0,trg->M(),true) - trg->P())/trg->P()*100.);
	hp[23]->Fill((1./tr->P_MS(brick.X0,trg->M(),false) - 1./trg->P())*trg->P()*100.);
    }
  }

  printf("  %6d tracks found after propagation\n", ntr-negflag);
  printf("  %6d matched tracks found (at vertexes), %6d with right segs >= %.0f%%\n", ntrgood, ntrgood_r, RightRatioMin*100.);
  printf("  %6d matched tracks found (cosmic     ), %6d with right segs >= %.0f%%\n", ntrgoodb, ntrgoodb_r, RightRatioMin*100.);
  printf("         ( %4d tracks not matched with generated ones)\n", notmatched);
  printf("         ( %4d tracks with nseg < %d )\n", nreject_nseg, nsegMin);
  printf("         ( %4d tracks with prob < %6.3f )\n", nreject_prob, ProbMinT);
  fprintf(f,"  %6d tracks found after propagation\n", ntr-negflag);
  fprintf(f,"  %6d matched tracks found (at vertexes), %6d with right segs >= %.0f%%\n", ntrgood, ntrgood_r, RightRatioMin*100.);
  fprintf(f,"  %6d matched tracks found (cosmic     ), %6d with right segs >= %.0f%%\n", ntrgoodb, ntrgoodb_r, RightRatioMin*100.);
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
  if ( neutral_primary && (nuse == npi) && !read_from_file ) usemom = true; 

  // build 2-tracks vertexes
  int nvtx = ali->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);

  // clearsome counters
  int nvgood[100];
  int nvgoodm[100];
  for (int i=0; i<100; i++) nvgood[i] = 0;
  for (int i=0; i<100; i++) nvgoodm[i] = 0;
  for (int i=0; i<10000; i++) numrect[i] = 0;

  int nadd = 0, npg = 0;
  int nprong = nuse + 1 - neutral_primary;
  
  // build N-tracks vertexes
  if (nprong > 2 || read_from_file) nadd = ali->ProbVertexN(ProbMinVN);

  // loop on found vertexes, search of corresponding generated vertex,
  // count some statistics, fill hists
  int ivg = 0, nvgoodt = 0, nvgoodmt = 0, np = 0, ntold = 0;
  for (int i=0; i<nvtx; i++)
    {
  	edbv = (EdbVertex *)((ali->eVTX)->At(i));
	if (edbv)
	{
	    if (edbv->Flag() == -10) continue;
	    v = edbv->V();
	    if (v)
	    {
		if (v->valid())
		{
		    if ((np = v->ntracks()) >= 2)
		    {
			    // reconstructed vertex with "np" tracks
			    nvgood[np]++;
			    nvgoodt++;
    			    if (np > nprongmax) nprongmax = np;
			    tr = edbv->GetTrack(0);
			    // search corresponding generated vertex for 1st track
			    int ivg0 = -1000000;
			    if(tr) ivg0 = tr->GetSegmentsFlag(nsegmatch);
			    if (ivg0 >= 0 && ivg0 < ntrg)
			        ivg0 = ((EdbTrackP*)(gener->eTracks->At(ivg0)))->Flag();
			    else
				ivg0 = -1000000;
			    ivg = -2000000;
			    // check that all track has the same generated vertex
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				tr = edbv->GetTrack(j);
				int ivg = -2000000;
				if(tr) ivg = tr->GetSegmentsFlag(nsegmatch);
				if (ivg >= 0 && ivg < ntrg)
			    	    ivg = ((EdbTrackP*)(gener->eTracks->At(ivg)))->Flag();
				else
				{
				    ivg = -2000000;
				    break;
				}
				if (ivg != ivg0) break;
			    }
			    if (ivg != ivg0) continue;
			    if (ivg <= 0) continue;
			    if (ivg > nvg) continue;
			    // corresponding generated vertex found!
			    if (np < rec_primary_vertex_ntracks_min) continue;
  			    edbvg = (EdbVertex *)((gener->eVTX)->At(ivg-1));
			    npg = edbvg->N();
			    ntold = numrect[ivg];
			    // count reconstructed vertex only one time for
			    // the same generated vertex
			    if (ntold == 0)
			    {
				nvgoodmg_tot[npg]++;
				numrect[ivg] = npg;
			    }
			    nvgoodmt++;
			    nvgoodmt_tot++;
			    nvgoodm[np]++;
			    // count primary vertexes (i.e. events)
			    if (edbvg->Flag() == 1) negood_tot++;
			    vxo = edbv->VX();	
			    vyo = edbv->VY();	
			    vzo = edbv->VZ();
			    vxg = edbvg->X();
			    vyg = edbvg->Y();
			    vzg = edbvg->Z();
  	    		    FillHistsV(*edbv, *v);
		    }
		}
	    }
	}
    }
  if (hp[9]) hp[9]->Fill(nvgoodmt);
  for (int i=2; i<=nprongmax; i++)
  {
    if (nvgood[i] <= 0) continue;
    printf("%6d %d-tracks vertexes found (%d matched to MC)\n", nvgood[i], i, nvgoodm[i]);
    fprintf(f,"%6d %d-tracks vertexes found (%d matched to MC)\n", nvgood[i], i, nvgoodm[i]);
    nvgood_tot[i] += nvgood[i];
    nvgoodm_tot[i] += nvgoodm[i];
  }
  if (nvtx)
  {
    int nlv = ali->LinkedVertexes();
    printf("------------------------------------------------------------\n");
    printf("%d linked vertexes found\n", nlv);
    if (nlv)
    {
      EdbVertex *vl = 0;
      EdbVertex *vc = 0;
      for(int i=0; i<nvtx; i++) 
      {
	vl = (EdbVertex*)(ali->eVTX->At(i));
	if (!vl) continue;
	if (vl->Flag()) > 2)
	{
	    vc = vl->GetConnectedVertex(0);
	    if (!vc) continue;
	    if (vc->ID() > vl->ID())
	    {
//		vl->Print();
//		vc->Print();
	    }
	}
      }
    }
    if (select_neighborhood)
    {
	int nn = ali->VertexNeighboor(2500., 4, 100000.);
	printf("------------------------------------------------------------\n");
	printf("%d neighbooring tracks and segments found\n", nn);
    }
  }
}

///-----------------------------------------------------------------------
void BookHistsV(int nve)
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_Dx","Vertex X error",100,-50.,50.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_Dy","Vertex Y error",100,-50.,50.);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_Dz","Vertex Z error",100,-100.,100.);
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
//  if (!hp[19]) hp[19] = new TH1F("Hist_Vt_DE","Momentum for low prob tracks",100,0.,2000.);
  if (!hp[14]) hp[14] = new TH1F("Hist_Vt_Dist","RMS track-vertex distance",100, 0.,50.);
  if (!hp[15]) hp[15] = new TH1F("Hist_Vt_Match","Right segments fraction (tracks at vertexes)",110, 0.,1.1);
  if (!hp[16]) hp[16] = new TH1F("Hist_Vt_Matchb","Right segments fraction (backgound tracks) ",110, 0.,1.1);
  if (!hp[17]) hp[17] = new TH1F("Hist_Vt_Trprob","Track probability",110, 0.,1.1);
  if (!hp[18]) hp[18] = new TH1F("Hist_Vt_AngleV","RMS track-track angle, mrad", 100, 0.,1000.);
  if (!hp[20]) hp[20] = new TH1F("Hist_Vt_Angle","Track angle, mrad", 100, 0.,2000.);
  if (!hp[21]) hp[21] = new TH1F("Hist_Vt_Momen","Track momentum, GeV/c", 50, 0.,5.);
  if (!hp[22]) hp[22] = new TH1F("Hist_Vt_Ntracks","Number of tracks at vertex", 20, 0.,20.);
  if (!hp[23]) hp[23] = new TH1F("Hist_Vt_Pmulsca","Relative Momentum error", 50, -100.,100.);

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

  hld4.Add(hp[22]);
  hld4.Add(hp[23]);

  for (int i=0; i<24; i++)
    if (hp[i]) hlist.Add(hp[i]);
}
///-----------------------------------------------------------------------
void FillHistsV(EdbVertex &edbv, Vertex &v)
{
  double smass = 0.13957;
  // fill hists with differences between reconstructed and generated position
  hp[0]->Fill((double)vxo - (double)vxg);
  hp[1]->Fill((double)vyo - (double)vyg);
  hp[2]->Fill((double)vzo - (double)vzg);
  // fill hists with vertex coordinate errors
  hp[3]->Fill(v.vxerr());
  hp[4]->Fill(v.vyerr());
  hp[5]->Fill(v.vzerr());
  // fill hist with chi2/ndf
  hp[6]->Fill(v.ndf() > 0 ? v.chi2()/v.ndf() : 0);
  // fill hist with vertex chi2 probability
  hp[7]->Fill(v.prob());
  if ( (nuse == npi) && neutral_primary && !read_from_file)
  {
    double rmass = v.mass(smass);
    hp[8]->Fill(rmass-mK);
  }
  // calculate average track-vertex distance
  double rmsdist = v.rmsDistAngle();
//  hp[14]->Fill(rmsdist);
  // fill hist with average track-track angle
  hp[18]->Fill(v.angle()*1000.);
  // fill histy with vertex multiplicity
  hp[22]->Fill(v.ntracks());
  // fill hist with impact parameters
  int n = edbv.N();
  for (int j=0; j<n; j++)
    if (hp[14]) hp[14]->Fill(edbv.Impact(j));
}
///-----------------------------------------------------------------------
void FillHistsGen()
{
  // tracks and vertex reconstruction (KF fitting only without track finding)

  double xg, yg, zg, txg, tyg, pg, ang;
  EdbTrackP *tr;
  EdbVertex *vedbg = 0;

  int nv = 0;
  if (!gener) return;
  if(gener->eVTX) nv = gener->eVTX->GetEntries();
  // loop on generated vertexes
  for(int i=0; i<nv; i++) {
    vedbg = (EdbVertex *)(gener->eVTX->At(i));
    if (!vedbg) continue;
    int nt = vedbg->N();
    // loop on vertex tracks
    for(int ip=0; ip<nt; ip++) {
	tr = vedbg->GetTrack(ip);
	if (!tr) continue;
        xg = tr->X();
	yg = tr->Y();
        zg = tr->Z();
        txg = tr->TX();
	tyg = tr->TY();
        pg  = tr->P();
	// calculate teta angle (mrad)
	ang = TMath::ACos(1./(1.+txg*txg+tyg*tyg))*1000.;
	if (hp[20]) hp[20]->Fill(ang);
	if (hp[21]) hp[21]->Fill(pg);
    }
  }
  int nt = 0;
  if (gener->eTracks) nt = gener->eTracks->GetEntries();
  // loop on all generated tracks
  for(int i=0; i<nt; i++) {
	tr = (EdbTrackP *)(gener->eTracks->At(i));
	if (!tr) continue;
        xg = tr->X();
	yg = tr->Y();
        zg = tr->Z();
        txg = tr->TX();
	tyg = tr->TY();
        pg  = tr->P();
	ang = TMath::ACos(1./(1.+txg*txg+tyg*tyg))*180./TMath::Pi();
	// fill hist with number of generated segments
	if (hp[13]) hp[13]->Fill(tr->N());
  }
}

///-----------------------------------------------------------------------
void DrawHistsV()
{
  char title[128];

  sprintf(title,"Number of reconstructed vertexs (%d generated)",nvg_tot);
  if (hp[9]) hp[9]->SetTitle(title);
  sprintf(title,"Number of reconstructed tracks (%d generated)",ntrg_tot);
  if (hp[10]) hp[10]->SetTitle(title);

  TCanvas *cv1 = new TCanvas("Vertex_reconstruction_1","MC Vertex reconstruction", 760, 760);
  DrawHlist(cv1, hld1);

  TCanvas *cv2 = new TCanvas("Vertex_reconstruction_2","Vertex reconstruction (tracks hists)", 600, 760);
  DrawHlist(cv2, hld2);

  TCanvas *cv3 = new TCanvas("Vertex_reconstruction_3","Vertex reconstruction (eff hists)", 600, 760);
  DrawHlist(cv3, hld3);

  TCanvas *cv4 = new TCanvas("Vertex_reconstruction_4","Vertex reconstruction (ntracks)", 600, 760);
  DrawHlist(cv4, hld4);
}
///------------------------------------------------------------
void DrawHlist(TCanvas *c, TObjArray h)
{
  int n = h.GetEntries();
  if (n < 2)
  {
  }
  else if (n < 3)
  {
    c->Divide(1,2);    
  }
  else if (n < 4)
  {
    c->Divide(1,3);    
  }
  else if (n < 5)
  {
    c->Divide(2,2);    
  }
  else if (n < 6)
  {
    c->Divide(2,3);    
  }
  else if (n < 7)
  {
    c->Divide(2,3);    
  }
  else if (n < 8)
  {
    c->Divide(2,4);    
  }
  else if (n < 9)
  {
    c->Divide(2,4);    
  }
  else if (n < 10)
  {
    c->Divide(3,3);    
  }
  else if (n < 11)
  {
    c->Divide(2,5);    
  }
  else
  {
    c->Divide(3,5);    
  }
  for(int i=0; i<n; i++) {
    c->cd(i+1);
    if (h.At(i)) h.At(i)->Draw();
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
void d() { DrawHistsV();}

//---------------------------------------------------------
EdbDisplay *ds=0;
EdbDisplay *ds2=0;
///------------------------------------------------------------
void dsall()
{

  if(ds) delete ds; 
  arrs->Clear();  

  ali->ExtractDataVolumeSegAll( *arrs );
 
  gStyle->SetPalette(1);

  ds=new EdbDisplay("display-segments",-60000.,60000.,-60000., 60000., 0.,80000.);
  
  ds->SetArrSegP( arrs );
  if (ali->eTracks) ds->SetArrTr( ali->eTracks );
  if (ali->eVTX) ds->SetArrV( ali->eVTX );
  ds->SetDrawTracks(3);
  ds->Draw();

}
///------------------------------------------------------------
void dst( int numt = -1 )
{

  if(ds) delete ds; 
  arrs->Clear();  
  arrtr->Clear();  

  if (ali->eTracks)
  {
    int ntr = ali->eTracks->GetEntries();
    if (!ntr) return;
  }
  else return;

  int it0,it1;
  if (numt == -1)
  {
    it0 = 0;
    it1 = ntr;
  }
  else
  {
    it0 = numt;
    it1 = numt+1;
  }

  //if (numt < 0) ali->ExtractDataVolumeSegAll( *arrs );
 
  gStyle->SetPalette(1);
  ds=new EdbDisplay("display-tracks",-60000.,60000.,-60000., 60000., 0.,80000.);
  
  EdbTrackP *track = 0;
  for(int i=it0; i<it1; i++) {
      track = (EdbTrackP *)(ali->eTracks->At(i));
      if(track->Flag()<0)           continue;
      if (track->Prob() < ProbMinT) continue;
      arrtr->Add(track);
  }
  ds->SetArrSegP( arrs );
  ds->SetArrTr(arrtr);
  ds->SetDrawTracks(4);
  ds->Draw();

}

//---------------------------------------------------------
void dsv( int numv = -1, int ntrMin=0, float binx=6, float bint=10 )
{
  if(!ali->eVTX) return;
  if(!ali->eTracks) return;

  arrs->Clear();  
  arrtr->Clear();  
  arrv->Clear();  
  //if(ds) delete ds; 

  int nvtx = ali->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv <0 )
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }

  if (numv == -10) ali->ExtractDataVolumeSegAll( *arrs );

  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(ali->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;
    if(v->Flag() == -10) continue;
    arrv->Add(v);

    int ntr = v->N();
    printf("Vertex %d, number of tracks %d\n", iv, ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      printf("       Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
    }

    if (numv == -1) continue;

    int nntr = v->Nn();
    for(int i=0; i<nntr; i++) {
      if (v->GetVTn(i)->Flag() == 1)
      {
        EdbSegP *seg = (EdbSegP *)(v->GetVTn(i)->GetTrack());
	if (!seg) continue;
	arrs->Add(seg);
        printf("       Neighboor Segment ID %d, Z = %f\n",
               seg->ID(), seg->Z());
      }
      else if (v->GetVTn(i)->Flag() == 0)
      {
        EdbTrackP *track = (EdbTrackP *)v->GetVTn(i)->GetTrack();
	if (!track) continue;
	arrtr->Add(track);
        printf("       Neighboor Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
               track->ID(), track->Z(), track->N(), v->GetVTn(i)->Zpos());
      }
    }
  }

  gStyle->SetPalette(1);

  if(!ds) 
    ds=new EdbDisplay("display-vertexes",-60000.,60000.,-60000., 60000., 0.,80000.);
  
  ds->SetArrSegP( arrs );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrv );
  if (numv == -10)
    ds->SetDrawTracks(3);
  else
    ds->SetDrawTracks(4);
  ds->SetDrawVertex(1);
  ds->Draw();

}
//---------------------------------------------------------
void dsvg( int numv = -1, float binx=6, float bint=10 )
{
  if(!gener->eVTX) return;

  if(ds) delete ds; 
  arrs->Clear();  
  arrtr->Clear();  
  arrv->Clear();  

  EdbSegP *seg=0;     // direction

  int ntrMin=2;
  int nvtx = gener->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv < 0)
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }
  
  if (numv == -10) ali->ExtractDataVolumeSegAll( *arrs );
  
  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gener->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrv->Add(v);

    int ntr = v->N();
    printf("Vertex %d, number of tracks %d\n", iv, ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      printf("       Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
    }

  }

  gStyle->SetPalette(1);

  ds=new EdbDisplay("display-generated-vertexes",-60000.,60000.,-60000., 60000., 0.,80000.);
  
  ds->SetArrSegP( arrs );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrv );
  if (numv == -10)
    ds->SetDrawTracks(3);
  else
    ds->SetDrawTracks(4);
  ds->SetDrawVertex(1);
  ds->Draw();
}
//---------------------------------------------------------
void dsvg2( int numv = -1, float binx=6, float bint=10 )
{
  if(!gener->eVTX) return;

  if(ds2) delete ds2; 
  arrs2->Clear();  
  arrtr2->Clear();  
  arrv2->Clear();  

  EdbSegP *seg=0;     // direction

  int ntrMin=2;
  int nvtx = gener->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv < 0)
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }
  if (numv == -10) ali->ExtractDataVolumeSegAll( *arrs2 );
  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gener->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrv2->Add(v);

    int ntr = v->N();
    printf("Vertex %d, number of tracks %d\n", iv, ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      printf("       Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr2->Add(track);
    }
  }

  gStyle->SetPalette(1);

  ds2=new EdbDisplay("display-generated-vertexes-2",-60000.,60000.,-60000., 60000., 0.,80000.);
  
  ds2->SetArrSegP( arrs2 );
  ds2->SetArrTr( arrtr2 );
  ds2->SetArrV( arrv2 );
  if (numv == -10)
    ds->SetDrawTracks(3);
  else
    ds->SetDrawTracks(4);
  ds2->SetDrawVertex(1);
  ds2->Draw();
}

