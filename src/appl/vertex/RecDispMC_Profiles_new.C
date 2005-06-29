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
EdbVertexRec *gEVR=0;

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

int eloss_flag = 1; // = 0 - no energy losses, 1-average energy losses, 2-Landau energy losses
int design = 0;

float momentum = 4000.; // momentum of K-meson

float seg_s[4] = {2.5,2.5,0.0025,0.0025}; //sx,sy,stx,sty,sP
float seg_zero[4] = {.0,.0,.0,.0}; //no measurement errors

int isegtest = -1;
float ShiftX = -3.*seg_s[0];
float ShiftP = 0.;
float NewSxy = 0.;

float dpp = 0.;   // average dp/P 

const unsigned long int PrintN = 100;

//float vxyz[3] = {0., 0., 0.};
float vxyz[3] = {0., 0., -1000.};   // simulated vertex position

TH1F *hp[9] = {0,0,0,0,0,0,0,0,0};
TProfile *hpp[6] = {0,0,0,0,0,0};

EdbVertex *vedb;

EdbTrackP *ti[10] = {0,0,0,0,0,0,0,0,0,0}; 

bool linfit = false;
static const int dim = brick.plates*2;
static const int dim2 = brick.plates*brick.plates*4;
double CovMS[dim2];
double *pCovMS = &CovMS[0];
float zl[dim], xl[dim];
float *pzl = &zl[0], *pxl = &xl[0];
TCanvas *cv = 0, *cv1 = 0, *cv2 = 0, *cv3 = 0, *cv4 = 0, *cv5 = 0, *cv6 = 0;
TCanvas *cvg1 = 0, *cvg2 = 0, *cvg3 = 0, *cvg4 = 0, *cvg5 = 0, *cvg6 = 0;

//-------------------------------------------------------------------
void run(int Nstat = 20000)
{
    // p = 1., 2., 4., 6., 1000.
    // n = 2   3   5  10   20
    FILE *f = 0;
    int ip1=0, ip2=1, in1=2, in2=3;
    float sigxt[5][5];
    float sigtxt[5][5];
    float sigxs[5][5];
    float sigtxs[5][5];
    float rmsxt[5][5];
    float rmstxt[5][5];
    float rmsxs[5][5];
    float rmstxs[5][5];
    float meaxt[5][5];
    float meatxt[5][5];
    float meaxs[5][5];
    float meatxs[5][5];
    float errxt[5][5];
    float errtxt[5][5];
    int hst[5][5];
    float mom[5], ns[5];
    for (int i=ip1; i<ip2; i++)
    {
	mom[i] = i*2;
	if (i == 0) mom[i] = .1;
	if (i == 4) mom[i] = 1000.;
	momentum = mom[i];
	for (int j=in1; j<in2; j++)
	{
	    if (j == 0) brick.plates = 2;
	    if (j == 1) brick.plates = 3;
	    if (j == 2) brick.plates = 5;
	    if (j == 3) brick.plates = 10;
	    if (j == 4) brick.plates = 20;
	    printf("--------------------------\n  P = %6.1f N = %2d\n",
	    mom[i], brick.plates); 
	    ClearHistsV();
	    gv(Nstat);
	    cv->cd(1);
	    hp[0]->Fit("gaus","q");
	    rmsxt[i][j] = hp[0]->GetRMS(1);
	    meaxt[i][j] = hp[0]->GetFunction("gaus")->GetParameter(1);
	    errxt[i][j] = hp[4]->GetMean(1);
	    sigxt[i][j] = hp[0]->GetFunction("gaus")->GetParameter(2);
	    cv->cd(2);
	    hp[1]->Fit("gaus","q");
	    rmstxt[i][j] = hp[1]->GetRMS(1);
	    meatxt[i][j] = hp[1]->GetFunction("gaus")->GetParameter(1);
	    errtxt[i][j] = hp[5]->GetMean(1);
	    sigtxt[i][j] = hp[1]->GetFunction("gaus")->GetParameter(2);
	    cv->cd(3);
	    hp[2]->Fit("gaus","q");
	    rmsxs[i][j] = hp[2]->GetRMS(1);
	    meaxs[i][j] = hp[2]->GetFunction("gaus")->GetParameter(1);
	    sigxs[i][j] = hp[2]->GetFunction("gaus")->GetParameter(2);
	    cv->cd(4);
	    hp[3]->Fit("gaus","q");
	    rmstxs[i][j] = hp[3]->GetRMS(1);
	    meatxs[i][j] = hp[3]->GetFunction("gaus")->GetParameter(1);
	    sigtxs[i][j] = hp[3]->GetFunction("gaus")->GetParameter(2);
	    hst[i][j] = hp[0]->GetEntries();
	    ns[j] = brick.plates;
	}
    }
    f = fopen("result.out","w");
    for (int i=ip1; i<ip2; i++)
    {
	for (int j=in1; j<in2; j++)
	{
	    printf("**** P = %3.1f N = %2d Nev = %4d\n",
	    mom[i], ns[j], hst[i][j]);
	    fprintf(f, "**** P = %3.1f N = %2d Nev = %4d\n",
	    mom[i], ns[j], hst[i][j]);
	    printf("SX_tr = %6.3f STX_tr = %10.7f SX_s = %6.3f STX_s = %8.5f\n",
	    sigxt[i][j], sigtxt[i][j], sigxs[i][j], sigtxs[i][j]);
	    fprintf(f, "SX_tr = %6.3f STX_tr = %10.7f SX_s = %6.3f STX_s = %8.5f\n",
	    sigxt[i][j], sigtxt[i][j], sigxs[i][j], sigtxs[i][j]);
	    printf("RX_tr = %6.3f RTX_tr = %10.7f RX_s = %6.3f RTX_s = %8.5f\n",
	    rmsxt[i][j], rmstxt[i][j], rmsxs[i][j], rmstxs[i][j]);
	    fprintf(f, "RX_tr = %6.3f RTX_tr = %10.7f RX_s = %6.3f RTX_s = %8.5f\n",
	    rmsxt[i][j], rmstxt[i][j], rmsxs[i][j], rmstxs[i][j]);
	    printf("MX_tr = %6.3f MTX_tr = %10.7f MX_s = %6.3f MTX_s = %8.5f\n",
	    meaxt[i][j], meatxt[i][j], meaxs[i][j], meatxs[i][j]);
	    fprintf(f, "MX_tr = %6.3f MTX_tr = %10.7f MX_s = %6.3f MTX_s = %8.5f\n",
	    meaxt[i][j], meatxt[i][j], meaxs[i][j], meatxs[i][j]);
	    printf("EX_tr = %6.3f ETX_tr = %10.7f\n",
	    errxt[i][j], errtxt[i][j]);
	    fprintf(f, "EX_tr = %6.3f ETX_tr = %10.7f\n",
	    errxt[i][j], errtxt[i][j]);
	}
    }
    fclose(f);
    if (linfit)
    {
	f = fopen("linfit.out","w");
	printf(" Track fitting accuracy vs number of segments and momentum\n");
	printf("        (Global matrix method\n\n");
	fprintf(f, " Track fitting accuracy vs number of segments and momentum\n");
	fprintf(f, "        (Global matrix method)\n\n");
    }
    else
    {
	f = fopen("kalman.out","w");
	printf(" Track fitting accuracy vs number of segments and momentum\n");
	printf("        (Kalman filtering method)\n\n");
	fprintf(f, " Track fitting accuracy vs number of segments and momentum\n");
	fprintf(f, "        (Kalman filtering method)\n\n");
    }
    printf("\n*************** Sigma X (microns) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** Sigma X (microns) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.3f    ",sigxt[i][j]);
	    fprintf(f, "%7.3f    ",sigxt[i][j]);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    printf("\n*************** RMS X (microns) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** RMS X (microns) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.3f    ",rmsxt[i][j]);
	    fprintf(f, "%7.3f    ",rmsxt[i][j]);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    printf("\n*************** ERR X (microns) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** ERR X (microns) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.3f    ",errxt[i][j]);
	    fprintf(f, "%7.3f    ",errxt[i][j]);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    printf("\n*************** Sigma TX (mrad) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** Sigma TX (mrad) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.2f    ",sigtxt[i][j]*1000.);
	    fprintf(f, "%7.2f    ",sigtxt[i][j]*1000.);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    printf("\n*************** RMS TX (mrad) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** RMS TX (mrad) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.2f    ",rmstxt[i][j]*1000.);
	    fprintf(f, "%7.2f    ",rmstxt[i][j]*1000.);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    printf("\n*************** ERR TX (mrad) **********\n");
    printf("                          Nplates\n");
    printf("   P (GeV/c)    ");
    fprintf(f, "\n*************** ERR TX (mrad) **********\n");
    fprintf(f, "                          Nplates\n");
    fprintf(f, "   P (GeV/c)    ");
    for (int j=in1; j<in2; j++)
    {
	printf("%2d         ", ns[j]);
	fprintf(f, "%2d         ", ns[j]);
    }
    printf("\n");
    fprintf(f, "\n");
    for (int i=ip1; i<ip2; i++)
    {
	printf("   %6.1f    ", mom[i]);
	fprintf(f, "   %6.1f    ", mom[i]);
	for (int j=in1; j<in2; j++)
	{
	    printf("%7.2f    ",errtxt[i][j]*1000.);
	    fprintf(f, "%7.2f    ",errtxt[i][j]*1000.);
	}
	printf("\n");
	fprintf(f, "\n");
    }
    fclose(f);
}

//-------------------------------------------------------------------
void gv(int n=1)
{
  // main steering routine for K->3Pi vertex generation&reconstruction test

  timespec_t t0, t;
  double dt;

  BookHistsV();
  if (linfit) CalcCovMS();

  ali      = new EdbPVRec();
  EdbPatternsVolume *vol=(EdbPatternsVolume*)ali;

  makeVolumeOPERAn(brick,vol);

  scanCond = new EdbScanCond();

  scanCond->SetSigma0( seg_s[0], seg_s[1], seg_s[2], seg_s[3] );  // sigma0 "x,y,tx,ty" at 0 angle
  scanCond->SetDegrad( 0.1 );               // sigma(tx) = sigma0*(1+degrad*tx)
  scanCond->SetBins(8,8,4,4);             // bins in [sigma] for checks
  scanCond->SetPulsRamp0(  1.,1. );         // in range (Pmin:Pmax) Signal/All is nearly linear
  scanCond->SetPulsRamp04( 1.,1. );         //
  scanCond->SetChi2Max(2.7);
  scanCond->SetChi2PMax(2.7);
  scanCond->SetRadX0(brick.X0);
  scanCond->SetName("Prototype_OPERA_basetrack");

  ali->SetScanCond(scanCond);
  ali->eVTX = new TObjArray();

  gener = new EdbPVGen();
  gener->SetVolume(vol);
  gener->eVTX = new TObjArray(1000);
  gener->SetScanCond(scanCond);

  gEVR = new EdbVertexRec();
  if (!(ali->eTracks)) ali->eTracks = new TObjArray(100);
  gEVR->eEdbTracks =  ali->eTracks;
  if (!(ali->eVTX))   ali->eVTX = new TObjArray(100);
  gEVR->eVTX       =  ali->eVTX;
  gEVR->ePVR       =  ali;

  gEVR->eZbin       = 100.;      // default is 100. microns
  gEVR->eAbin       = 0.01;      // default is 0.01 rad
  gEVR->eDZmax      = 3000.;     // default is 3000. microns
  gEVR->eProbMin    = 0.01;      // default 0.01, i.e 1%
  gEVR->eImpMax     = 25.;       // default is 25. microns
  gEVR->eUseSegPar  = false;     // default is FALSE - use fitted track parameters


  TTimeStamp  ts=TTimeStamp();
  t0=ts.GetTimeSpec();
  for(int i=1; i<=n; i++) {
    g1(1);
    if (i)
    {
	if (!(i%PrintN))
	{
	    ts=TTimeStamp();
	    t=ts.GetTimeSpec();
	    dt=(t.tv_sec-t0.tv_sec)*1000000000.+(t.tv_nsec-t0.tv_nsec);
	    printf("%d events generated in %.4f sec (%.1f msec per event)\n",
	    i,dt/1000000000.,dt/(double)i/1000000.);
	}
    }
  }
  DrawHistsV();
  delete ali;
  ali = 0;
  delete gener;
  gener = 0;
}


///------------------------------------------------------------
void g1(int nv=1)
{
  //generation of 1 event

  gen_v(brick, nv);

  for(int i=0; i<nv; i++) {
    vedb = (EdbVertex *)(ali->eVTX->At(i));
    rec_v(*vedb);
  }

  ali->eVTX->Clear();
  ali->eTracks->Clear();
  ali->Clear();

  delete vedb;
  vedb = 0;
}

///------------------------------------------------------------
void gen_v(BRICK &b, int nv )
{
  // event generation

  for(int i=0; i<nv; i++) {
    vedb = new EdbVertex();
    vedb->SetXYZ(vxyz[0],vxyz[1], vxyz[2]);
    ali->eVTX->Add(vedb);
    gen_tracks_v( *vedb);
    gen_segs_v(b, *vedb);
  }

}

///-------------------------------------------------------------------
void makeVolumeOPERAn( BRICK &b, EdbPatternsVolume *v )
{
  // Volume geometry setting

  EdbPattern *pat =0;
  for(int i=0; i<b.plates; i++) {
    pat = new EdbPattern(0,0,b.z0+b.dz*i);
    v->AddPattern(pat);
  }
}

//-------------------------------------------------------------------
void gen_tracks_v(EdbVertex &vedb)
{
  // generation of track parameters according to phase space

  EdbTrackP *tr[10]; 
  float x = 0.;
  float y = 0.;
  float z = 0.;
  float tx = 0.;
  float ty = 0.;

  for(int ip=0; ip<1; ip++) {
    tr[ip] = new EdbTrackP();
    tr[ip]->Set(ip, x, y, tx, ty, 1, 0);
    tr[ip]->SetZ(z);
    tr[ip]->SetP(momentum);
    tr[ip]->SetM(0.139);
    gEVR->AddTrack(vedb, tr[ip], 0);
    ali->AddTrack(tr[ip]);
  }

}

//-------------------------------------------------------------------
void gen_segs_v(BRICK &b, EdbVertex &vedb)
{
  // generation of basetracks for secondary tracks

  unsigned int seed = 0;
  float zlim[2];
  zlim[0] = -1000.;  
  zlim[1] = 130000.;
  EdbTrackP *tr;

  int nt = vedb.N();
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    tr->SetFlag(1000+ip);
    ti[ip] = new EdbTrackP();
    ti[ip]->Set(tr->ID(),tr->X(),tr->Y(),tr->TX(),tr->TY(),tr->W(),tr->Flag());
    ti[ip]->SetZ(tr->Z());
    ti[ip]->SetP(tr->P());
    ti[ip]->SetM(tr->M());
    seed = gRandom->GetSeed();
    gener->TrackMC( zlim, b.lim, *tr, eloss_flag, 0.);
    gRandom->SetSeed(seed);
    scanCond->SetSigma0(       0.,       0.,       0.,       0. );  // sigma0 "x,y,tx,ty" at 0 angle
    gener->TrackMC( zlim, b.lim, *ti[ip], eloss_flag, 0.);
    scanCond->SetSigma0( seg_s[0], seg_s[1], seg_s[2], seg_s[3] );  // sigma0 "x,y,tx,ty" at 0 angle
  }
}

double DE_MC, DE;
///------------------------------------------------------------
void rec_v(EdbVertex &vedb)
{

  // tracks and vertex reconstruction (KF fitting only without track finding)

  int nsegMin = 2;
  int ns = 0, ns2 = 0, is = 0;
  float p, de;
  EdbTrackP *tr;
  float par[2], dpar[2][2], chi2l = 0.;
  float *ppar = &par[0], *pdpar = &dpar[0][0]; 
  float dx = 0., dtx = 0., sx = 0., stx = 0.;

  int nt = vedb.N();
  //printf("\n nt = %d\n\n",nt);
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    //    tr->Print();
    if (tr->N() < nsegMin) break;
    if (tr->P() < 0.1) break;

    p = tr->P();
    tr->SetErrorP(p*p*dpp*dpp);
    DE_MC = tr->DE();

    if (dpp > 0.)
    {
	p = p*(1.+dpp*gRandom->Gaus());
	if (p < 0.01) p = 0.01;
    }
    else
    {
	p = p*(1.+ShiftP);
	if (p < 0.01) p = 0.01;
    }
    tr->SetP(p);
    if (linfit)
    {
	if ((ns = tr->N()) != brick.plates) continue;
	ns2 = 2*ns;
	for (int i = 0; i<ns2; i++)
	{
	    is = i/2;
	    zl[i] = tr->GetSegment(is)->Z();
	    if (i%2) zl[i] = zl[i] + 150.;
	    else     zl[i] = zl[i] - 150.;
	    xl[i] = tr->GetSegment(is)->X();
	    if (i%2) xl[i] = xl[i] + 150.*tr->GetSegment(is)->TX();
	    else     xl[i] = xl[i] - 150.*tr->GetSegment(is)->TX();
	}
	if (EdbMath::LinFitCOV(ns2, pzl, pxl, pCovMS, ppar, pdpar, &chi2l))
	{
	    hp[0]->Fill(par[0]-ti[ip]->X());
	    hp[1]->Fill(par[1]-ti[ip]->TX());
	    sx = (float)TMath::Sqrt(dpar[0][0]);
	    stx = (float)TMath::Sqrt(dpar[1][1]);
	    hp[4]->Fill(sx);
	    hp[5]->Fill(stx);
	    hp[6]->Fill(TMath::Prob((double)chi2l, ns2-2));
	}
	hp[2]->Fill((tr->GetSegmentFirst())->X()-ti[ip]->X());
	hp[3]->Fill((tr->GetSegmentFirst())->TX()-ti[ip]->TX());
	continue;
    }

    EdbSegP *s, *sf, *sg;
    float sx,sy;
    
    if(tr->N() == brick.plates)
    {
	for (int is = 0; is < brick.plates; is++)
	{
	    s  = tr->GetSegment(is);
	    if (is == isegtest)
	    {
		if (ShiftX < 0.)
		{
		    sg = ti[ip]->GetSegment(is);
		    s->SetX(sg->X() + (1.-2.*gRandom->Rndm())*ShiftX);
		}
		else if (ShiftX > 0.)
		{
		    s->SetX(s->X()+ShiftX);
		}
	    }
	    if (NewSxy != 0.)
	    {
		sx = NewSxy*NewSxy;
		sy = NewSxy*NewSxy;
		s->SetErrors(sx,sy,s->SZ(),s->STX(),s->STY());
	    }
	}
    }
    
    tr->FitTrackKFS(true, brick.X0, design);

    hp[0]->Fill((tr->GetSegmentFFirst())->X()-ti[ip]->X());
    hp[1]->Fill((tr->GetSegmentFFirst())->TX()-ti[ip]->TX());
    hp[2]->Fill((tr->GetSegmentFirst())->X()-ti[ip]->X());
    hp[3]->Fill((tr->GetSegmentFirst())->TX()-ti[ip]->TX());
    hp[6]->Fill(tr->Prob());
    hp[7]->Fill(tr->Chi2());

    DE = tr->DE();
    
    TMatrixD cov(5,5);

    if(tr->NF() == brick.plates)
    {
	for (int is = 0; is < brick.plates; is++)
	{
	    s  = tr->GetSegment(is);
	    sf = tr->GetSegmentF(is);
	    dx = sf->X() - s->X();
	    if (ti[ip])
	    {
		dx = sf->X() - (ti[ip]->GetSegment(is))->X();
		dtx = sf->TX() - (ti[ip]->GetSegment(is))->TX();
	    }
	    cov = (sf->COV());
	    sx = (float)TMath::Sqrt(cov(0,0));
	    stx = (float)TMath::Sqrt(cov(2,2));
	    hpp[0]->Fill((float)is,dx);
	    hpp[1]->Fill((float)is,sx);

	    dx = sf->X() - s->X();
	    dtx = sf->TX() - s->TX();
	    if (seg_s[0] > sx)  hpp[2]->Fill((float)is,dx/TMath::Sqrt(seg_s[0]*seg_s[0] - sx*sx));
	    if (seg_s[2] > stx) hpp[3]->Fill((float)is,dtx/TMath::Sqrt(seg_s[2]*seg_s[2] - stx*stx));

	    hpp[4]->Fill((float)is,sf->Chi2());
	    hpp[5]->Fill((float)is,sf->Prob());

	    if (is == 0)
	    {
		hp[4]->Fill(sx);
		hp[5]->Fill(stx);
	    }
	}
    }
  }
}

///-----------------------------------------------------------------------
void BookHistsV()
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_DtrackX","Track Delta X, microns",100,-20.,+20.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_DtrackTX","Track Delta TX, rad",100,-0.020,+0.020);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_DsegX","Segment Delta X, microns",100,-20.,+20.);
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_DsegTX","Segment Delta TX, rad",100,-0.050,+0.050);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_StrackX","Track Sigma X, microns",100,0.,+10.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_StrackTX","Track Sigma TX, rad",100,0.,+0.010);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Prob","Chi Square Probability",101,0.,1.01);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Chi2","Chi Square",100,0.,20.0);
  if (!hpp[0]) hpp[0] = new TProfile("Hist_DX_Prof","Delta X vs Z",brick.plates,0.,brick.plates,-1000.,+1000.,"s");
  if (!hpp[1]) hpp[1] = new TProfile("Hist_SX_Prof","Sigma X vs Z",brick.plates,0.,brick.plates,-1000.,+1000.," ");
  if (!hpp[2]) hpp[2] = new TProfile("Hist_DXP_Prof","Pull quantities X vs Z",brick.plates,0.,brick.plates,-5.,+5.,"s");
  if (!hpp[3]) hpp[3] = new TProfile("Hist_DTXP_Prof","Pull quantities TX vs Z",brick.plates,0.,brick.plates,-5.,+5.,"s");
  if (!hpp[4]) hpp[4] = new TProfile("Hist_Chi2_Prof","Chi2 vs Z",brick.plates,0.,brick.plates, 0.,+20.,"s");
  if (!hpp[5]) hpp[5] = new TProfile("Hist_Prob_Prof","Prob vs Z",brick.plates,0.,brick.plates, 0.,+1.04,"s");
  hpp[0]->SetStats(kFALSE);
  hpp[1]->SetStats(kFALSE);
  hpp[2]->SetStats(kFALSE);
  hpp[3]->SetStats(kFALSE);
  hpp[4]->SetStats(kFALSE);
  hpp[5]->SetStats(kFALSE);
}

///-----------------------------------------------------------------------
Double_t roadp(Double_t *x, Double_t *par)
{
//    double s = seg_s[0];
    double s = TMath::Sqrt(seg_s[0]*seg_s[0]+seg_s[2]*seg_s[2]*150.*150.);
    double n = brick.plates*2;
    double x0 = (float)brick.plates/2.;
    double sx = 0.;
    double xs = 0.;
    double xx = *x - x0;
    for (int i=0; i<n; i++)
    {
	xs = i/2+0.5;
	if (i%2) xs = xs + 0.15/1.3;
	else     xs = xs - 0.15/1.3;
	sx += (xs-x0)*(xs-x0);
    }
    Double_t r2 = 2.*(1./n+xx*xx/sx);
    Double_t r = 0.;
    if (r2 > 0.) r = s*TMath::Sqrt(r2);
    return r;
}
///-----------------------------------------------------------------------
void DrawHistsV()
{
  char title[80];
  if (!cv)
  {
    cv = new TCanvas("cv", "Track parameters", 500, 0, 600, 750);
    cv->Divide(2,3);
  }
  else
  {
    cv->Clear();
    cv->Divide(2,3);
  }
  for(int i=0; i<6; i++) {
    cv->cd(i+1);
    if (hp[i]) hp[i]->Draw();
  }
  cv->Update();
  if (!linfit)
  {
//	double xr = 7.;
//	double pr = 0.;
//	printf("roadp(%4.1f) = %f\n", xr, roadp(&xr, &pr));
    TF1 *f = new TF1("roadp", roadp, 0., (double)brick.plates, 0);
//    f->SetParameters(&pr);
    f->SetMinimum(0.);
    if (!cv1)
    {
	cv1 = new TCanvas("cv1", "Error road", 0, 400, 500, 350);
//	cv1->Divide(1,2);
    }
    else
    {
	cv1->Clear();
//	cv1->Divide(1,2);
    }
    cv1->cd();
    sprintf(title, "Track momentum %d GeV/c", (int)momentum);
    hpp[0]->SetTitle(title);
    hpp[0]->SetXTitle("Track segment number");
    hpp[0]->SetYTitle("Coordinate accuracy, microns");
    hpp[0]->SetLineColor(2);
    hpp[0]->Draw();
    hpp[1]->SetLineColor(3);
    hpp[1]->Draw("HISTCSAME");
    f->SetLineColor(4);
    f->Draw("LSAME");

    if (!cv3)
    {
	cv3 = new TCanvas("cv3", "Chi2 vs Segment", 0, 400, 500, 350);
//	cv3->Divide(1,2);
    }
    else
    {
	cv3->Clear();
//	cv3->Divide(1,2);
    }
    cv3->cd();
    sprintf(title, "Track momentum %d GeV/c", (int)momentum);
    hpp[4]->SetTitle(title);
    hpp[4]->SetXTitle("Track segment number");
    hpp[4]->SetYTitle("Fitted Segment Chi2");
    hpp[4]->SetLineColor(2);
    hpp[4]->Draw();

    if (!cv6)
    {
	cv6 = new TCanvas("cv6", "Prob vs Segment", 0, 400, 500, 350);
//	cv6->Divide(1,2);
    }
    else
    {
	cv6->Clear();
//	cv6->Divide(1,2);
    }
    
    cv6->cd();
    sprintf(title, "Track momentum %d GeV/c", (int)momentum);
    hpp[5]->SetTitle(title);
    hpp[5]->SetXTitle("Track segment number");
    hpp[5]->SetYTitle("Fitted Segment Prob");
    hpp[5]->SetLineColor(2);
    hpp[5]->Draw();

    if (!cv4)
    {
	cv4 = new TCanvas("cv4", "Pull quantities", 0, 400, 500, 350);
	cv4->Divide(1,2);
    }
    else
    {
	cv4->Clear();
	cv4->Divide(1,2);
    }
    cv4->cd(1);
    sprintf(title, "Coordinate pull quantity, track momentum %d GeV/c", (int)momentum);
    hpp[2]->SetTitle(title);
    hpp[2]->SetXTitle("Track segment number");
    hpp[2]->SetYTitle("Pull quantity mean and sigma");
    hpp[2]->SetLineColor(2);
    hpp[2]->SetMinimum(-1.5);
    hpp[2]->SetMaximum(+1.5);
    hpp[2]->Draw();
    cv4->cd(2);
    sprintf(title, "Slope pull quantity, track momentum %d GeV/c", (int)momentum);
    hpp[3]->SetTitle(title);
    hpp[3]->SetXTitle("Track segment number");
    hpp[3]->SetYTitle("Pull quantity mean and sigma");
    hpp[3]->SetLineColor(2);
    hpp[3]->SetMinimum(-1.5);
    hpp[3]->SetMaximum(+1.5);
    hpp[3]->Draw();
  }
  if (!cv2)
  {
    cv2 = new TCanvas("cv2", "Probability", 0, 400, 500, 350);
    cv2->cd();
    if (hp[6]) hp[6]->Draw();
  }
  else
  {
    cv2->Clear();
    cv2->cd();
    if (hp[6]) hp[6]->Draw();
  }
//  cv2->Update();

  if (!cv5)
  {
    cv5 = new TCanvas("cv5", "Chi2", 0, 400, 500, 350);
    cv5->cd();
    if (hp[7]) hp[7]->Draw();
  }
  else
  {
    cv5->Clear();
    cv5->cd();
    if (hp[7]) hp[7]->Draw();
  }
//  cv2->Update();
}
///-----------------------------------------------------------------------
void ClearHistsV()
{
  for(int i=0; i<7; i++) {
//    if (hp[i]) hp[i]->Clear();
    if (hp[i]) hp[i]->Reset();
  }
  for(int i=0; i<4; i++) {
//    if (hpp[i]) hpp[i]->Clear();
    if (hpp[i]) hpp[i]->Reset();
  }
}
///-----------------------------------------------------------------------
void CalcCovMS()
{
    int i, j, l;
    int n = brick.plates;
    int m = n*2;
    int plusi = 0, plusj = 0, emul = 0;
    Double_t xi, xj, dx, dxi, dxj, xg1, xg2;
    Double_t sx2 = seg_s[0]*seg_s[0]+seg_s[2]*seg_s[2]*150.*150.;
    Double_t sxc = seg_s[0]*seg_s[0]-seg_s[2]*seg_s[2]*150.*150.;
    Double_t stx2 = seg_s[2]*seg_s[2];
    Double_t cov = 0., len = 0., X = 0.;
    Double_t beta = momentum/TMath::Sqrt(momentum*momentum+0.139*0.139);
    Double_t h = 0.0141/(momentum*beta);
    h = h*h;
    Double_t H = 0.;

    for (i = 0; i<m*m; i++)
    {
	CovMS[i] = 0.;
    }

    for (i = 0; i<m; i++)
    {
	plusi = i%2;
	xi = brick.z0 + brick.dz*(i/2);
	if (plusi) xi = xi + 150.;
	else	   xi = xi - 150.;
	for (j = 0; j<m; j++)
	{
	    plusj = j%2;
	    xj = brick.z0 + brick.dz*(j/2);
	    if (plusj) xj = xj + 150.;
	    else       xj = xj - 150.;
	    cov = 0.;
	    xg1 = 0.;
	    xg2 = 0.;
	    for (l = 0; l<m; l++)
	    {
		emul = 1 - l%2;
		if (emul)
		{
		    len = 300.;
		    X = 286000.;
		    H = h/X;
		}
		else
		{
		    len = 1000.;
		    X =   5600.;
		    H = h/X;
		}
		xg2 = xg1 + len;
		if (xi <= xg1 ) break;
		if (xj <= xg1 ) break;
		if (xi > xg1 && xi <= xg2 && xj > xg1 && xj <=xg2)
		{
		    dx = xi - xg1;
		    cov += 0.5*H*dx*dx*(xj - xg1 - 0.33333*dx);
		}
		else if (xi > xg1 && xi <= xg2 && xj > xg2)
		{
		    dx = xi - xg1;
		    cov += 0.5*H*dx*dx*(len - 0.33333*dx + (xj - xg2));
		}
		else if (xj > xg1 && xj <= xg2 && xi > xg2)
		{
		    dx = xj - xg1;
		    cov += 0.5*H*dx*dx*(len - 0.33333*dx + (xi - xg2));
		}
		else if (xi >= xg2 && xj >= xg2)
		{
		    dxi = xi - xg2;
		    dxj = xj - xg2;
		    cov += H*len*(0.33333*len*len + 0.5*len*(dxi+dxj) + dxi*dxj);
		}
		xg1 = xg2;
	    }
	    if (i == j)
	    {
		cov += sx2;
	    }
	    else if ( TMath::Abs(xi-xj) <= 305. )
	    {
		cov += sxc;
	    }
	    CovMS[m*i+j] = cov;
	}
    }
}
///-----------------------------------------------------------------------
float Ysigx[5][5] = {
      {0.502,      0.489,      0.461,      0.458,      0.486},    
      {0.442,      0.469,      0.453,      0.470,      0.469},    
      {0.463,      0.447,      0.463,      0.468,      0.459},    
      {0.479,      0.452,      0.453,      0.480,      0.442},    
      {0.450,      0.451,      0.384,      0.290,      0.223}    
};
float Yrmsx[5][5] = {
      {0.506,      0.505,      0.474,      0.480,      0.495},    
      {0.457,      0.469,      0.460,      0.491,      0.482},    
      {0.477,      0.452,      0.472,      0.466,      0.458},    
      {0.477,      0.462,      0.463,      0.480,      0.452},    
      {0.454,      0.450,      0.385,      0.289,      0.227}    
};
float Yerrx[5][5] = {
      {0.492,      0.492,      0.492,      0.492,      0.492},    
      {0.484,      0.482,      0.482,      0.482,      0.482},    
      {0.477,      0.470,      0.470,      0.470,      0.470},    
      {0.475,      0.463,      0.462,      0.462,      0.462},    
      {0.473,      0.444,      0.383,      0.294,      0.224}    
};
float Yerrxlin[5][5] = {
      {0.487,      0.487,      0.487,      0.487,      0.487},    
      {0.479,      0.477,      0.477,      0.477,      0.477},    
      {0.475,      0.467,      0.467,      0.467,      0.467},    
      {0.474,      0.461,      0.459,      0.459,      0.459},    
      {0.473,      0.444,      0.383,      0.294,      0.223}    
};
float Ysigtx[5][5] = {
      { 1.38,       1.41,       1.30,       1.41,       1.33},    
      { 1.12,       1.15,       1.15,       1.15,       1.16},    
      { 0.83,       0.85,       0.87,       0.84,       0.87},    
      { 0.71,       0.68,       0.69,       0.68,       0.71},    
      { 0.49,       0.27,       0.13,       0.11,       0.11}    
};
float Yrmstx[5][5] = {
      { 1.39,       1.41,       1.32,       1.43,       1.36},    
      { 1.14,       1.17,       1.19,       1.17,       1.17},    
      { 0.90,       0.85,       0.88,       0.86,       0.86},    
      { 0.72,       0.68,       0.69,       0.69,       0.71},    
      { 0.48,       0.26,       0.12,       0.07,       0.04}    
};
float Yerrtx[5][5] = {
      { 1.37,       1.37,       1.37,       1.37,       1.37},    
      { 1.16,       1.16,       1.16,       1.16,       1.16},    
      { 0.87,       0.87,       0.87,       0.87,       0.87},    
      { 0.72,       0.70,       0.70,       0.70,       0.70},    
      { 0.48,       0.26,       0.12,       0.07,       0.04}    
};


float Xn[5] = {2., 3., 5., 10., 20.};
float Xnlin[5] = {2.2, 3.2, 5.2, 10.2, 20.2};
void graphs()
{
    char legt[64];
    int imom[5] = {1, 2, 4, 6, 1000};
    TGraph *gsigx[5];
    TGraph *grmsx[5];
    TGraph *gerrx[5];
    TGraph *gerrxlin[5];
    TGraph *gsigtx[5];
    TGraph *grmstx[5];
    TGraph *gerrtx[5];
    TLegend *lx = new TLegend(0.15,0.15,0.4,0.4);
    TLegend *ltx = new TLegend(0.6,0.16,0.85,0.41);
    for (int i=0; i<5; i++)
    {
	gsigx[i] = new TGraph(5, &Xn[0], &Ysigx[i][0]);
	gsigx[i]->SetMinimum(0.);
	gsigx[i]->SetMaximum(0.6);
	gsigx[i]->SetTitle("Track coordinate sigma");
	gsigx[i]->SetMarkerStyle(20);
	gsigx[i]->SetMarkerColor(i+2);
	sprintf(legt, "%4d GeV/c", imom[i]);
	lx->AddEntry(gsigx[i], legt, "p");
	ltx->AddEntry(gsigx[i], legt, "p");
	gsigx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	gsigx[i]->GetYaxis()->SetTitle("Sigma, microns");
	gsigx[i]->SetLineColor(i+2);

	grmsx[i] = new TGraph(5, &Xn[0], &Yrmsx[i][0]);
	grmsx[i]->SetMinimum(0.);
	grmsx[i]->SetMaximum(0.6);
	grmsx[i]->SetTitle("Track coordinate RMS");
	grmsx[i]->SetMarkerStyle(20);
	grmsx[i]->SetMarkerColor(i+2);
	grmsx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	grmsx[i]->GetYaxis()->SetTitle("RMS, microns");
	grmsx[i]->SetLineColor(i+2);

	gerrx[i] = new TGraph(5, &Xn[0], &Yerrx[i][0]);
	gerrx[i]->SetMinimum(0.);
	gerrx[i]->SetMaximum(0.6);
	gerrx[i]->SetTitle("Track coordinate error");
	gerrx[i]->SetMarkerStyle(20);
	gerrx[i]->SetMarkerColor(i+2);
	gerrx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	gerrx[i]->GetYaxis()->SetTitle("Error, microns");
	gerrx[i]->SetLineColor(i+2);

	gerrxlin[i] = new TGraph(5, &Xnlin[0], &Yerrxlin[i][0]);
	gerrxlin[i]->SetMinimum(0.);
	gerrxlin[i]->SetMaximum(0.6);
	gerrxlin[i]->SetTitle("Track coordinate sigma");
	gerrxlin[i]->SetMarkerStyle(22);
	gerrxlin[i]->SetMarkerColor(1);
	gerrxlin[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	gerrxlin[i]->GetYaxis()->SetTitle("Sigma, microns");

	gsigtx[i] = new TGraph(5, &Xn[0], &Ysigtx[i][0]);
	gsigtx[i]->SetMinimum(0.);
	gsigtx[i]->SetMaximum(1.6);
	gsigtx[i]->SetTitle("Track slope sigma");
	gsigtx[i]->SetMarkerStyle(20);
	gsigtx[i]->SetMarkerColor(i+2);
	gsigtx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	gsigtx[i]->GetYaxis()->SetTitle("Sigma, mrad");
	gsigtx[i]->SetLineColor(i+2);

	grmstx[i] = new TGraph(5, &Xn[0], &Yrmstx[i][0]);
	grmstx[i]->SetMinimum(0.);
	grmstx[i]->SetMaximum(1.6);
	grmstx[i]->SetTitle("Track slope RMS");
	grmstx[i]->SetMarkerStyle(20);
	grmstx[i]->SetMarkerColor(i+2);
	grmstx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	grmstx[i]->GetYaxis()->SetTitle("RMS, mrad");
	grmstx[i]->SetLineColor(i+2);

	gerrtx[i] = new TGraph(5, &Xn[0], &Yerrtx[i][0]);
	gerrtx[i]->SetMinimum(0.);
	gerrtx[i]->SetMaximum(1.6);
	gerrtx[i]->SetTitle("Track slope error");
	gerrtx[i]->SetMarkerStyle(20);
	gerrtx[i]->SetMarkerColor(i+2);
	gerrtx[i]->GetXaxis()->SetTitle("Track lenght (in segments)");
	gerrtx[i]->GetYaxis()->SetTitle("Error, mrad");
	gerrtx[i]->SetLineColor(i+2);
    }
    sprintf(legt, "Global fit");
    lx->AddEntry(gerrxlin[0], legt, "p");
    if (!cvg1 && 0)
    {
	cvg1 = new TCanvas("cvg1", "Track coordinate sigma, microns", 0, 400, 500, 350);
    }
    else
    {
	if (cvg1) cvg1->Clear();
    }
    if (cvg1) cvg1->cd(1);
    for (int i=0; i<5 && cvg1; i++)
    {
	if (i == 0)
	{
	  gsigx[i]->Draw("CPA");
	  lx->Draw();
	}
	else
	  gsigx[i]->Draw("CPSAME");
    }
    if (!cvg2 && 0)
    {
	cvg2 = new TCanvas("cvg2", "Track coordinate RMS, microns", 0, 400, 500, 350);
    }
    else
    {
	if (cvg2) cvg2->Clear();
    }
    if (cvg2) cvg2->cd(1);
    for (int i=0; i<5 && cvg2; i++)
    {
	if (i == 0)
	{
	  grmsx[i]->Draw("CPA");
	  lx->Draw();
	}
	else
	  grmsx[i]->Draw("CPSAME");
    }
    if (!cvg3 && 1)
    {
	cvg3 = new TCanvas("cvg3", "Track oordinate error, microns", 0, 400, 500, 600);
    }
    else
    {
	if (cvg3) cvg3->Clear();
    }
    if (cvg3) cvg3->cd(1);
    for (int i=0; i<5 && cvg3; i++)
    {
	if (i == 0)
	{
	  gerrx[i]->Draw("CPA");
	  gerrxlin[i]->Draw("PSAME");
	  lx->Draw();
	}
	else
	{
	  gerrx[i]->Draw("CPSAME");
	  gerrxlin[i]->Draw("PSAME");
	}
    }

    if (!cvg4 && 0)
    {
	cvg4 = new TCanvas("cvg4", "Track slope sigma, mrad", 0, 400, 500, 350);
    }
    else
    {
	if (cvg4) cvg4->Clear();
    }
    if (cvg4) cvg4->cd(1);
    for (int i=0; i<5 && cvg4; i++)
    {
	if (i == 0)
	{
	  gsigtx[i]->Draw("CPA");
	  lxt->Draw();
	}
	else
	  gsigtx[i]->Draw("CPSAME");
    }

    if (!cvg5 && 0)
    {
	cvg5 = new TCanvas("cvg5", "Track slope RMS, mrad", 0, 400, 500, 350);
    }
    else
    {
	if (cvg5) cvg5->Clear();
    }
    if (cvg5) cvg5->cd(1);
    for (int i=0; i<5 && cvg5; i++)
    {
	if (i == 0)
	{
	 grmstx[i]->Draw("CPA");
	 ltx->Draw();
	}
	else
	 grmstx[i]->Draw("CPSAME");
    }

    if (!cvg6 && 1)
    {
	cvg6 = new TCanvas("cvg6", "Track slope error, mrad", 0, 400, 500, 350);
    }
    else
    {
	if (cvg6) cvg6->Clear();
    }
    if (cvg6) cvg6->cd(1);
    for (int i=0; i<5 && cvg6; i++)
    {
	if (i == 0)
	{
	  gerrtx[i]->Draw("CPA");
	  ltx->Draw();
	}
	else
	  gerrtx[i]->Draw("CPSAME");
    }
}