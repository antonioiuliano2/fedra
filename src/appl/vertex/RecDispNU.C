#include "My_Track.h"
#include "My_Header.h"

bool  use_mc_momentum = false;
bool  use_mc_mass = false;
bool  only_primary_vertex = true;
int   primary_vertex_ntracks_min = 2;
int   rec_primary_vertex_ntracks_min = 2;
bool  scanning = false;
bool  write_mc_data = false;
int   nutype = 2; // 0 - NC, 1 - CC, 2 - NC & CC
bool  smear = true, setz = true;
float back1_tetamax =  0.35; 
float back2_tetamax =  0.25; 
float back2_plim[2] = {0.5, 5.5};
float fiducial_border = 20000.;
float fiducial_border_z = 20000.;
float dpp = 0.20;   // average dp/P 
float seg_s[4]={.5, .5, 0.0015, 0.0015}; //sx,sy,stx,sty,sP
float ProbGap = 0.10; //probability to have pattern hole
float RightRatioMin = 0.5;

int maxgaps[6] = {0,3,3,6,3,6}; // all combinations

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

float AngleAcceptance = 0.8;

float ProbMinV  = 0.005;      // min vertex probability 
float ProbMinVN = 0.005;      // min vertex probability 
float ProbMinP  = 0.005;     // minimal propagate probability
float ProbMinT  = 0.005;     // minimal track probability to be used for vtx
int   nsegMin = 2;
bool  usemom=false;

//////////////////////////////////////////////////////////////////////////

extern FILE *log;
extern FILE *stdout;
extern FILE *stderr;
FILE *fmcdata = 0;
TFile *rf=0;

float vxo = 0., vyo = 0., vzo = 0.;
float vxg = 0., vyg = 0., vzg = 0.;

TH1F *hp[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
TProfile *hpp[10] = {0,0,0,0,0,0,0,0,0,0};
TObjArray hlist, hld1, hld2, hld3, hld4, hld5, hld6, hld7, hld8;

using namespace VERTEX;

const char filename[256];
const char *INfile = &filename[0];
TFile *fmicro;
My_Track *micro = 0;
TTree *TreeMS = 0;
int Ntr = 0, Nvt = 0, MaxTrack = 0, MaxVertex = 0, Ntrg = 0, Nvtg = 0;
int Nst[500];
int PdgId[500];
int Numgent[500];
int Numgenv[500];
int nvggood = 0;
float Pt[500];
float TXt[500];
float TYt[500];
float NVt[500];
float DIRt[500];
float VTx[500], VTy[500], VTz[500], Nvtr[500], Numvtr[500][500];
float TMass[500];

EdbDataProc  *dset=0;
EdbDisplay   *ds=0;
EdbDisplay   *ds2=0;
EdbPVRec     *gAli=0;
EdbPVGen     *gener=0;
EdbPatternsVolume *vol=0;
EdbScanCond  *scan = 0;
int evnum = 0;
float z0shift = 295.;

//---------------------------------------------------------
void scan()
{
    scanning = true;
    nutype = 0;
    evnum = 0;
    bar = new TControlBar("vertical", "Event Scanning");
    bar->AddButton("Continue","scan1()", "Click Here For Analyze next event");
    bar->AddButton("End","scanend()", "Click Here For Stop Event Scanning");
    bar->AddButton("Display gen","dsvg2()", "Click Here for Display Generated Event");
    bar->AddButton("Display rec","dsv()", "Click Here For Display Reconstructed Event");
    bar->Show();
    gROOT->SaveContext();
}

//---------------------------------------------------------
void scan1()
{
    evnum++;
    if (evnum > 100)
    {
	 if (nutype == 0)
	 {
	    evnum = 1;
	    nutype = 1;
	 }
	 else
	 {
	    printf("No more events!");
	    evnum = 0;
	    nutype = 2;
	    scanning = false;
	    return;
	 }
    }
    run(evnum,evnum);
}
//---------------------------------------------------------
void scanend()
{
    scanning = false;
    nutype = 2;
    evnum = 0;
}
//---------------------------------------------------------
void run(int ib = 1, int ie = 100)
{
    char line[80], nusmtype[7], nutyp[3];
    FILE *frunlist = 0; 
    int i, j, k, l, Event, evold;
    int Event, Track, nV, Trackold, Vertexold;
    float Vt, Vx, Vy, Vz, VPx, VPy, VPz, Tx, Ty, P;

    if (Geant[11] != 3) GCodesInit();

    gSystem->Exec("mv -f fedra.lst fedra.lst.old &> /dev/null");
    gSystem->Exec("mv -f vertex.lst vertex.lst.old &> /dev/null");
    gSystem->Exec("mv -f runs.lst runs.lst.old &> /dev/null");
    for (evold = 1; evold <= 100; evold++)
    {
	sprintf(line,"ls par/01_%03d.par &>/dev/null", evold);
	if (!(gSystem->Exec(line))) break;
    }
    BookHistsV();
    if (write_mc_data) fmcdata = fopen("vertexes_and_tracks.data","w");
    for (int nut = 0; nut < 2; nut++)
    {
      if (nut == 0 && nutype == 1) continue;
      if (nut == 1 && nutype == 0) continue;
      if (nut == 0) { strcpy(nusmtype, "NC"); strcpy(nutyp, "nc"); }
      if (nut == 1) { strcpy(nusmtype, "CC"); strcpy(nutyp, "cc"); }
      if (smear == 1) strcat(nusmtype,"_NOS");
      for (i = ib; i <= ie; i++)
      {
	sprintf(line,"rm -f data &>/dev/null");
	gSystem->Exec(line);
	sprintf(line,"ln -s /mnt/nusrv3/opera/NUMU%s/numu%s.%03d data\n", nusmtype, nutyp, i);
	gSystem->Exec(line);
	frunlist = fopen("runs.lst","w");
	for (j = 58; j>0; j--)
	{

	    if (evold != i)
	    {
		sprintf(line,"mv -f par/%02d_%03d.par par/%02d_%03d.par\n", j, evold, j, i);
		gSystem->Exec(line);
	    }
	    sprintf(line,"%d   %d data/%02d_%03d.cp.root 1\n", j, i, j, i);
	    fprintf(frunlist,"%s",line);
	}
	evold = i;
	fclose(frunlist);
	
	for (j = 0; j<500; j++) Pt[j] = -100.;
	for (j = 0; j<500; j++) VTx[j] = -1000000.;
	Ntr = 0;
	Nvt = 0;
	Ntrg = 0;
	Nvtg = 0;
	for (j = 0; j<500; j++) Nst[j] = 0;
	for (j = 0; j<500; j++) Nvtr[j] = 0;
	for (j = 0; j<500; j++) Numgent[j] = -1;
	for (j = 0; j<500; j++) Numgenv[j] = -1;
	for (j = 0; j<500; j++) TMass[j] = -1;

	sprintf(filename, "/mnt/nusrv3/opera/NUMU%s/numu%s.%03d/numu%s.%03d.root", nusmtype, nutyp, i, nutyp, i);
	INfile = &filename[0];
	fmicro = new TFile(INfile);
	TreeMS = (TTree*)fmicro->Get("TreeMS");
	micro = new My_Track(TreeMS, INfile);


	int nentries_micro = Int_t(micro->fChain->GetEntriesFast());
	MaxTrack = 0;
	MaxVertex = 0;
	k = 0;
	while(k<nentries_micro){
//	    l = (micro->fChain)->LoadTree(k);
//	    if (l < 0) break;
	    micro->GetEntry(k);
	    k++;
	    Event = micro->Event;
	    Track = micro->Track;
// DEBUG
//	    if (Track != 1) continue;
	    nV = micro->nV;
	    if (nV != 1 && only_primary_vertex) continue;
	    Vt = micro->Vt;
	    Vx = micro->VTX[0];
	    Vy = micro->VTX[1];
	    Vz = micro->VTX[2];
	    VPx = micro->VPx;
	    VPy = micro->VPy;
	    VPz = micro->VPz;
	    P = TMath::Sqrt(VPx*VPx + VPy*VPy + VPz*VPz);
	    Tx = VPx/VPz;
	    Ty = VPy/VPz;
	    P = micro->P;
// DEBUG
//	    if (Track == 1)
//	    {
//		printf("Lay = %d PdgId = %d X = %f Y = %f Z = %f TX = %f TY = %f\n",
//		micro->Layer, micro->PdgId, micro->X, micro->Y, micro->Z,
//		micro->Tx, micro->Ty);
//	    }
	    if (Track >= 500) continue;
	    if (Pt[Track] == -100.)
	    {
		Ntr++;
		Pt[Track] = P;
		PdgId[Track] = micro->PdgId;
		TXt[Track] = Tx;
		TYt[Track] = Ty;
		NVt[Track] = nV;
// DEBUG
//		printf("PdgId = %i\n", PdgId[Track]);
		TMass[Track] = MassGeant(PdgId[Track]);
		if (VPz <= 0.) DIRt[Track] = -1.;
		else	       DIRt[Track] = +1.;
		Numvtr[nV][Nvtr[nV]] = Track;
		Nvtr[nV]++;
		if (Track > MaxTrack) MaxTrack = Track; 
	    }
	    if (nV >= 500) continue;
	    if (VTx[nV] == -1000000.)
	    {
		Nvt++;
		VTx[nV] = Vx;
		VTy[nV] = Vy;
		VTz[nV] = Vz;
		if (nV > MaxVertex) MaxVertex = nV; 
	    }
	    Nst[Track]++;
	}
	for (j = 0; j < MaxTrack; j++)
	{
	    Nst[j] /= 2;
// DEBUG
//	    if (Pt[j] != -100. && 0)
//	    {
//		printf(" T = %d nV = %d Nseg = %d tx = %f ty = %f P = %f\n",
//		j, NVt[j], Nst[j], TXt[j], TYt[j], Pt[j]);
//	    }
	}
// DEBUG
//	for (j = 0; j < MaxVertex && 0; j++)
//	{
//	    if (VTx[j] != -1000000.)
//	    {
//		printf(" nV = %d Ntr = %d Vx = %f Vy = %f Vz = %f\n",
//		j, Nvtr[j], VTx[j], VTy[j], VTz[j]);
//		for (int k = 0; k< Nvtr[j]; k++)
//		{
//		    printf(" %3d", Numvtr[j][k]);
//		}
//		printf("\n");
//	    }
//	}
//	continue;
	init_rec();
	delete micro;
	micro = 0;
	fmicro = 0;
      }
    }
    
    if (write_mc_data) fclose(fmcdata);
    
    for (j = 58; j>0; j--)
    {
	sprintf(line,"mv -f par/%02d_%03d.par par/%02d_%03d.par &>/dev/null\n", j, i-1, j, 1);
	gSystem->Exec(line);
    }
    if (scanning) return;
    rf = new TFile("RecDispNU.root","RECREATE","MC Vertex reconstruction");
    hlist.Write();
    rf->Close();
    delete rf;
    rf = 0;
    printf(" Shift in Z0 is %f\n", z0shift);
}

//---------------------------------------------------------
void RecDispNU()
{
  //init(data_set);
  init_rec();
}

//---------------------------------------------------------
void init_rec(char *data_set="data_set.def")
{

  Vertex *v = 0, *vc = 0;
  int Track = 0;

  if (Geant[11] != 3) GCodesInit();

  if (dset)
  {
    if (gAli->eVTX) gAli->eVTX->Delete();
    if (gAli->eTracks) gAli->eTracks->Delete();
    gAli->ResetCouples();
    int npat=gAli->Npatterns();
    TClonesArray *segs = 0;
    for(int i=0; i<npat; i++ )
	if ( segs = (gAli->GetPattern(i))->GetSegments()) segs->Delete();
    delete dset;
    dset = 0;
    gAli = 0;
  }
  if (gener)
  {
    delete gener;
    gener = 0;
  }

//  stdout = freopen("fedra.lst", "w+", stdout);
  dset=new EdbDataProc(data_set);
  dset->InitVolume(0);
  gAli = dset->PVR();

  vol=(EdbPatternsVolume*)gAli;
  gener = new EdbPVGen();
  gener->SetVolume(vol);
  gener->eVTX = new TObjArray(1000);
  scan = gAli->GetScanCond();
  gener->SetScanCond(scan);

  EdbTrackP *trg = 0;
  EdbVertex *vert = 0;
  int numv = 0, numt = 0, nv = 0, Track = 0, ntgood = 0, ntgoodv = 0, ntrgood_gen1 = 0;
  nvggood = 0;

  SmearSegments();

  for (int iv = 0; iv <= MaxVertex; iv++)
  {
	    if (VTx[iv] != -1000000.)
	    {
		vert = new EdbVertex();
		vert->SetXYZ(VTx[iv], VTy[iv], VTz[iv]-z0shift);
		ntgoodv = 0;
		for (int it = 0; it < Nvtr[iv]; it++)
		{
		    Track = Numvtr[iv][it];
		    if (Pt[Track] != -100.)
		    {
			if (Nst[Track] < 2) continue;
			ntgood++;
			trg = new EdbTrackP(); 
			Numgent[Track] = numt;
	    		trg->Set(numt++, VTx[iv], VTy[iv], TXt[Track], TYt[Track], Nst[Track], nv+1);
			trg->SetZ(VTz[iv]-z0shift);
			trg->SetP(Pt[Track]);
			if (TMass[Track] < 0.)
			    trg->SetM(0.1395700);
			else
			    trg->SetM(TMass[Track]);
			gener->AddTrack(trg);
			ntgoodv++;
			if (Nvtr[iv] < 2) break;
			if (DIRt[Track] < 0.)
			    vert->AddTrack(trg, 0);
			else
			    vert->AddTrack(trg, 1);
		    }
		}
		if (hp[24]) hp[24]->Fill((float)ntgoodv);
		if (ntgoodv < 2)
		{
		    if(trg) trg->SetFlag(0);
		    delete vert;
		    vert = 0;
		}
		else
		{
		    if (write_mc_data)
		    {
		      fprintf(fmcdata, "%d %d %f %f %f\n", iv, ntgoodv,
			      VTx[iv], VTy[iv], VTz[iv]);
		      for (int it = 0; it < Nvtr[iv]; it++)
		      {
			Track = Numvtr[iv][it];
			if (Pt[Track] != -100.)
			{
			    if (Nst[Track] < 2) continue;
			    if (TMass[Track] < 0.) TMass[Track] = 0.1395700;
			    fprintf(fmcdata, "%d %f %f %f %f\n",
				    Pdg2Geant(PdgId[Track]), TMass[Track],
				    Pt[Track], 
				    TXt[Track], TYt[Track]);
			}
		      }
		    }
		    if (iv == 1 && (ntgoodv < primary_vertex_ntracks_min)) break;
		    gener->AddVertex(vert);
		    Numgenv[nv] = iv;
		    nv++;
		    nvggood++;
  	    	    if (iv == 1)
		    {
			    ntrgood_gen1 = ntgoodv;
		    }
		}
	    }
  }
  if (!(gener->eVTX) || !(gener->eTracks) || (nvggood == 0))
  {
    if (hp[25]) hp[25]->Fill(0.);
    if (hp[26]) hp[26]->Fill(0.);
    return;
  }

  FillHistsGen();

  gAli->Link();
  printf("link ok\n");

//  stdout = freopen("vertex.lst", "w+", stdout);

  printf("Monte-Carlo Ntr = %d Nvt = %d MaxTrackNumber = %d MaxVertexNumber = %d\n", ntgood, nv, MaxTrack, MaxVertex);
  if (hp[25]) hp[25]->Fill((float)ntgood);
  if (hp[26]) hp[26]->Fill((float)nv);

  gAli->FillTracksCell();
  gAli->MakeTracks();

  int nvg = (gener->eVTX)->GetEntries();
  if (!nvg) return;
  int ntrg = (gener->eTracks)->GetEntries();
  if (!ntrg) return;

  int ntr = 0;
  if (gAli->eTracks) ntr = gAli->eTracks->GetEntries();
  if (!ntr)
  {
    if (hp[16]) hp[16]->Fill(0.);
    if (hp[19]) hp[19]->Fill(0.);
    if (hp[10]) hp[10]->Fill(0.);
    if (hp[9])  hp[9]->Fill(0.);
    if (hp[8])  hp[8]->Fill(0.);
    if (hp[23]) hp[23]->Fill(0.);
    return;
  }

  int nsegmatch = 0, itrg = 0;
  float p = 0.;
  EdbTrackP *tr = 0;
  for (int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(gAli->eTracks->At(itr));
    if (tr->Flag()<0)
    {
	continue;
    }
    trg = 0;
    if (ntrg)
    {
	itrg = tr->GetSegmentsAid(nsegmatch);
	if (itrg >= 0 && itrg < MaxTrack)
	{
	    itrg = Numgent[itrg];
	    if (itrg < 0 || itrg >= ntrg) continue;
	    trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	    p = trg->P();
	    if (dpp > 0.)
		tr->SetErrorP(p*p*dpp*dpp);
	    else
		tr->SetErrorP(p*p*0.2*0.2);
	    if (use_mc_momentum)
	    {
		p = p*(1.+dpp*gRandom->Gaus());
		if      (p < 0.05) p = 0.05;
		else if (p > 30.0) p = 30.;
		tr->SetP(p);
	    }
	    else
	    {
		p = 0.;
	    }
	    tr->SetP(p);
	    if (use_mc_mass) tr->SetM(trg->M());
	    else	     tr->SetM(0.);
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

  gAli->FitTracks(0., 0.);

  gAli->FillCell(50,50,0.015,0.015);
  gAli->PropagateTracks(55,2,ProbMinP);

  ntr = gAli->eTracks->GetEntries();
  printf("%d tracks was found\n",ntr);

  int ntrgood = 0;
  int notmatched = 0;
  int negflag = 0, ntrgood_r = 0, numveg = 0;
  int negflag1 = 0, ntrgood_r1 = 0;
  int nreject_nseg = 0, nreject_prob = 0;
  int nreject_nseg1 = 0, nreject_prob1 = 0;
  double right_ratio = 0.;
  float dx = 0.;

  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(gAli->eTracks->At(itr));
    trg = 0;
    numveg = 0;
    right_ratio = 0.;
    if (ntrg)
    {
	itrg = tr->GetSegmentsAid(nsegmatch);
	if (itrg >= 0 && itrg <= MaxTrack)
	{
	    itrg = Numgent[itrg];
	    if (itrg >= 0 && itrg < ntrg)
	    {
	      trg = (EdbTrackP*)(gener->eTracks->At(itrg));
	      right_ratio = (double)nsegmatch/tr->N();
	      if (right_ratio >= RightRatioMin)
	      {
	        numveg = trg->Flag() - 1;
		if (numveg >= 0 && numveg < 500)
		{
	    	    numveg = Numgenv[numveg];
		}
		else
		{
		    numveg = 0;
		}
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
    }
    else
    {
	notmatched++;
    }

    if (tr->Flag()<0)
    {
	negflag++;
	if (numveg == 1) negflag1++;
	continue;
    }
    if (hp[11]) hp[11]->Fill(tr->N());
    if (tr->N()<nsegMin)
    {
	nreject_nseg++;
	if (numveg == 1) nreject_nseg1++;
	continue;
    }

    dx = (tr->GetSegmentFirst())->X()-(tr->GetSegmentFFirst())->X();

    if (trg != 0 && dx != 0.)
    {
	hp[17]->Fill(tr->Prob());
    }
    if (tr->Prob()<ProbMinT)
    {
	nreject_prob++;
	if (numveg == 1) nreject_prob1++;
	continue;
    }
    if (trg)
    {
	      if (trg->Flag() == 0)        // background track
	      {
//		ntrgoodb++;
		if (right_ratio >= RightRatioMin)
		{
//		    ntrgoodb_r++;
		}
//		hp[16]->Fill(right_ratio);

	      }
	      else if (trg->Flag() <= nvg) // track at vertex
	      {
		ntrgood++;
		if (right_ratio >= RightRatioMin)
		{
		    ntrgood_r++;
		    if (numveg == 1) ntrgood_r1++;
		}
		hp[15]->Fill(right_ratio);
	      }
    }

    if (hp[12])
    {
	if (dx != 0. && tr->N() > 2) 
	    hp[12]->Fill(dx);
/*	else
	{
	    Track = (tr->GetSegmentFirst())->ID();
	    float ptx = tr->TX();
	    float pty = tr->TY();
	    float dPb = 1290.*TMath::Sqrt(1.+ptx*ptx+pty*pty); // thickness of the Pb+emulsion cell in microns
	    float teta0sq = EdbPhysics::ThetaMS2( tr->P(), tr->M(), dPb, 5810. );
	    printf("ID = %d Nseg = %f P = %f Z = %f PdgId = %d TMS2 = %f\n",
	    tr->ID(), tr->N(), tr->P(), tr->Z(), PdgId[Track], teta0sq); 
	    tr->GetSegmentFirst()->COV()->Print();
	    tr->GetSegmentFFirst()->COV()->Print();
	} */
    }

    if (trg != 0)
    {
//	DE_MC = trg->DE();
//	DE_FIT = tr->DE();
//	hp[19]->Fill(DE_FIT-DE_MC);
//	hp[23]->Fill((tr->P_MS() - trg->P())/trg->P()*100.);
    }
  }

  printf("  %6d tracks found after propagation\n", ntr-negflag);

  if (hp[16]) hp[16]->Fill((float)(ntrgood_r)/ntrg);
  if (hp[19]) hp[19]->Fill((float)(ntr-negflag)/ntrg);
  if (hp[10]) hp[10]->Fill(ntr-negflag);
  if (hp[27]) hp[27]->Fill(ntrgood_gen1 - negflag1);
  if (hp[28]) hp[28]->Fill(ntrgood_gen1 - nreject_nseg1);
  if (hp[29]) hp[29]->Fill(ntrgood_gen1 - nreject_prob1);
  if (hp[30]) hp[30]->Fill(ntrgood_gen1 - ntrgood_r1);


  bool usemom = false;

  int nvtx = gAli->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);

  int nvagood[100];
  int nvagoodm[100];
  for (int i=0; i<100; i++) nvagood[i] = 0;
  for (int i=0; i<100; i++) nvagoodm[i] = 0;
  int nvgoodm = 0, nvgood = 0;
  int ivg = 0;
 
  if (!nvtx)
  {
    if (hp[9]) hp[9]->Fill(0.);
    if (hp[23]) hp[23]->Fill(0.);
    if (hp[8]) hp[8]->Fill(0.);
    return;
  }

  int nadd = 0;
  int np = 0;
  
  nadd = gAli->ProbVertexN(ProbMinVN);

  nvtx = gAli->eVTX->GetEntries();

  for (int i=0; i<nvtx; i++)
    {
  	edbv = (EdbVertex *)((gAli->eVTX)->At(i));
	if (edbv && (edbv->Flag() != -10))
	{
	    v = edbv->V();
	    if (v)
	    {
		if (v->valid())
		{
		    if ((np = v->ntracks()) >= 2)
		    {
			    nvgood++;
			    nvagood[np]++;
			    tr = edbv->GetTrack(0);
			    int ivg0 = -1;
			    if(tr) ivg0 = tr->GetSegmentsAid(nsegmatch);
			    if (ivg0 >= 0 && ivg0 <= MaxTrack)
			    {
			      ivg0 = Numgent[ivg0];
			      if (ivg0 >= 0 && ivg0 < ntrg)
			        ivg0 = ((EdbTrackP*)(gener->eTracks->At(ivg0)))->Flag();
			      else
			        ivg0 = -1000000;
			    }
			    else
			    {
			      ivg0 = -1000000;
			    }
			    ivg = -2000000;
			    for (int j=1; j<edbv->N() && ivg0>0; j++)
			    {
				tr = edbv->GetTrack(j);
				int ivg = -2000000;
				if (tr) ivg = tr->GetSegmentsAid(nsegmatch);
				if (ivg >= 0 && ivg < MaxTrack)
				{
			    	  ivg = Numgent[ivg];
				  if (ivg >= 0 && ivg < ntrg)
			    	    ivg = ((EdbTrackP*)(gener->eTracks->At(ivg)))->Flag();
				  else
				    ivg = -2000000;
				}
				else
				    break;
				if (ivg != ivg0) break;
			    }
			    if (ivg != ivg0) continue;
			    if (ivg <= 0) continue;
			    if (ivg >  nvg) continue;
			    if (np < rec_primary_vertex_ntracks_min) continue;
			    nvagoodm[np]++;
			    nvgoodm++;
  			    edbvg = (EdbVertex *)((gener->eVTX)->At(ivg-1));
			    vxo = edbv->X();	
			    vyo = edbv->Y();	
			    vzo = edbv->Z();
			    vxg = edbvg->X();
			    vyg = edbvg->Y();
			    vzg = edbvg->Z();
			    FillHistsV(*v);
		    }
		}
	    }
	}
    }
//  if (hp[9]) hp[9]->Fill(nvgoodm[Nvtr[1]]);
  if (hp[9]) hp[9]->Fill(nvgood);
  if (nvggood)
  {
    if (hp[23]) hp[23]->Fill((float)nvgoodm/nvggood);
    if (hp[8]) hp[8]->Fill((float)nvgood/nvggood);
  }
  if (1) return;

  if (nv)
  {
    int nlv = gAli->LinkedVertexes();
    printf("%d linked vertexes found\n", nlv);
    if (nlv)
    {
      for(int i=0; i<nv; i++) 
      {
	v = (EdbVertex*)(gAli->eVTX->At(i));
	if (v->Flag() > 2)
	{
	    vc = v->GetConnectedVertex(0);
	    if (vc->ID() > v->ID())
	    {
		v->Print();
		vc->Print();
	    }
	}
      }
    }
  }
  int nn = gAli->VertexNeighboor(1000.);
  printf("%d neighbooring tracks found\n", nn);
  delete vol;
  vol = 0;
}

//---------------------------------------------------------
void init(char *data_set="data_set.def")
{
  dset=new EdbDataProc(data_set);
  dset->InitVolume(100);
  gAli = dset->PVR();
}

//---------------------------------------------------------
void dsall()
{
//  if(!gAli) init();

  TObjArray *arr   = new TObjArray();  

  gAli->ExtractDataVolumeSegAll( *arr );
 
  gStyle->SetPalette(1);
  if(!ds) 
    ds=new EdbDisplay("display-segments",-60000.,60000.,-60000., 62000., 0.,100000.);
  //ds=new EdbDisplay("display-segments",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( gAli->eTracks );
  ds->SetDrawTracks(3);
  ds->Draw();
//  delete ds;
//  ds = 0;
  delete arr;
  arr = 0;

}

//---------------------------------------------------------
void dsv( int numv = -1, int numt = -1, float binx=6, float bint=10 )
{
  if(!gAli->eVTX) return;
  if(!gAli->eTracks) return;
  if(!gAli) init();

  EdbSegP *seg=0;     // direction

  TObjArray *arrV  = new TObjArray();  // vertexes
  TObjArray *arr   = new TObjArray();  // segments
  TObjArray *arrtr = new TObjArray();  // tracks

  int ntrMin=2;
  int nvtx = gAli->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv == -1)
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }

  gAli->ExtractDataVolumeSegAll( *arr );

  int ntr = gAli->eTracks->GetEntries();
  for(int i=0; i<ntr; i++) {
      EdbTrackP *track = (EdbTrackP *)(gAli->eTracks->At(i));
      //track->Print();
      if (track->Flag() < 0) continue;
      printf("Track ID %d, Z = %f, Nseg = %d\n",
      track->ID(), track->Z(), track->N());
      if (numt < 0 || numt == i)
        arrtr->Add(track);
//      gAli->ExtractDataVolumeSeg( *track,*arr,binx,bint );
  }

  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gAli->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrV->Add(v);
  }

  gStyle->SetPalette(1);

  if(ds) delete ds;
  ds=new EdbDisplay("display-vertexes",-60000.,60000.,-60000., 62000., 0.,100000.);
//  ds=new EdbDisplay("display-vertexes",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrV );
  ds->SetDrawVertex(1);
  ds->Draw();
}
//---------------------------------------------------------
void dsvg( int numv = -1, float binx=6, float bint=10 )
{
  if(!gener->eVTX) return;

  EdbSegP *seg=0;     // direction

  TObjArray *arrV  = new TObjArray();  // vertexes
  TObjArray *arr   = new TObjArray();  // segments
  TObjArray *arrtr = new TObjArray();  // tracks

  int ntrMin=2;
  int nvtx = gener->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv == -1)
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }
  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gener->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrV->Add(v);

    gAli->ExtractDataVolumeSegAll( *arr );
    int ntr = v->N();
//    printf("ntr=%d\n",ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      //track->Print();
      printf("Vertex %d, Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      iv, track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
    }

  }

  gStyle->SetPalette(1);

  if(ds) delete ds;
  ds=new EdbDisplay("display-generated",-60000.,60000.,-60000., 62000., 0.,100000.);
//  ds=new EdbDisplay("display-generated",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrV );
  ds->SetDrawVertex(1);
  ds->Draw();
}
//---------------------------------------------------------
void dsvg2( int numv = -1, float binx=6, float bint=10 )
{
  if(!gener->eVTX) return;

  EdbSegP *seg=0;     // direction

  TObjArray *arrV  = new TObjArray();  // vertexes
  TObjArray *arr   = new TObjArray();  // segments
  TObjArray *arrtr = new TObjArray();  // tracks

  int ntrMin=2;
  int nvtx = gener->eVTX->GetEntries();
  EdbVertex *v = 0;
  int iv0,iv1;
  if (numv == -1)
  {
    iv0 = 0;
    iv1 = nvtx;
  }
  else
  {
    iv0 = numv;
    iv1 = numv+1;
  }
  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gener->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrV->Add(v);

    gAli->ExtractDataVolumeSegAll( *arr );
    int ntr = v->N();
//    printf("ntr=%d\n",ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      //track->Print();
      printf("Vertex %d, Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      iv, track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
    }

  }

  gStyle->SetPalette(1);

  if(ds2) delete ds2;
  ds2=new EdbDisplay("display-generated-2",-60000.,60000.,-60000., 62000., 0.,100000.);
//  ds=new EdbDisplay("display-generated-2",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds2->SetArrSegP( arr );
  ds2->SetArrTr( arrtr );
  ds2->SetArrV( arrV );
  ds2->SetDrawVertex(1);
  ds2->Draw();
}

///-----------------------------------------------------------------------
void BookHistsV()
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_Dx","Vertex X error",100,-50.,50.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_Dy","Vertex Y error",100,-50.,50.);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_Dz","Vertex Z error",100,-100.,100.);
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_Sx","Vertex X sigma",100,0.,25.);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_Sy","Vertex Y sigma",100,0.,25.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_Sz","Vertex Z sigma",100,0.,50.);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",25,0.,5.);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",26,0.,1.04);
//  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Mass","Vertex Mass error",50,-1.000,+1.000);
  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Vpercg","Ratio reconstructed/generated vertexes",150, 0.,3.0);
  char title[128];
  sprintf(title,"Number of reconstructed vertexs");
  if (!hp[9]) hp[9] = new TH1F("Hist_Vt_Nvert",title,20,0.,20.);
  sprintf(title,"Number of reconstructed tracks");
  if (!hp[10]) hp[10] = new TH1F("Hist_Vt_Ntrack",title,100,0.,100.);
  if (!hp[11]) hp[11] = new TH1F("Hist_Vt_Nsegs","Number of track segments",60,0.,60.);
  if (!hp[12]) hp[12] = new TH1F("Hist_Vt_Dtrack","Track DeltaX, microns",100,-1.,+1.);
  if (!hp[13]) hp[13] = new TH1F("Hist_Vt_NsegsG","Number of generated track segments",60,0.,60.);
//  if (!hp[19]) hp[19] = new TH1F("Hist_Vt_DE","DE(MC)-DE",100,-0.25,+0.25);
  if (!hp[19]) hp[19] = new TH1F("Hist_Vt_Tperc","Ratio reconstructed/generated tracks",250, 0.,5.0);
  if (!hp[14]) hp[14] = new TH1F("Hist_Vt_Dist","RMS track-vertex distance",100, 0.,50.);
  if (!hp[15]) hp[15] = new TH1F("Hist_Vt_Match","Right segments fraction (tracks at vertexes)",110, 0.,1.1);
//  if (!hp[16]) hp[16] = new TH1F("Hist_Vt_Matchb","Right segments fraction (backgound tracks)",110, 0.,1.1);
  if (!hp[16]) hp[16] = new TH1F("Hist_Vt_Tpercg","Ratio reconstructed(matched)/generated tracks",250, 0.,5.0);
  if (!hp[17]) hp[17] = new TH1F("Hist_Vt_Trprob","Track probability",110, 0.,1.1);
  if (!hp[18]) hp[18] = new TH1F("Hist_Vt_AngleV","RMS track-track angle, mrad", 100, 0.,1000.);
  if (!hp[20]) hp[20] = new TH1F("Hist_Vt_Angle","Track angle, mrad", 100, 0.,2000.);
  if (!hp[21]) hp[21] = new TH1F("Hist_Vt_Momen","Track momentum, GeV/c", 50, 0.,5.);
  if (!hp[22]) hp[22] = new TH1F("Hist_Vt_Ntracks","Number of tracks at vertex (rec)", 20, 0.,20.);
//  if (!hp[23]) hp[23] = new TH1F("Hist_Vt_Pmulsca","Relative Momentum error", 50, -250.,250.);
  if (!hp[23]) hp[23] = new TH1F("Hist_Vt_Vpercmg","Ratio reconstructed(matched)/generated vertexes",100, 0.,5.0);
  if (!hp[24]) hp[24] = new TH1F("Hist_Vt_NtracksMCTV","Number of tracks at vertex (MC)", 20, 0.,20.);
  if (!hp[25]) hp[25] = new TH1F("Hist_Vt_NtracksMCT","Number of generated tracks", 100, 0.,100.);
  if (!hp[26]) hp[26] = new TH1F("Hist_Vt_NtracksMCV","Number of generated vertexes", 20, 0.,20.);
  if (!hp[27]) hp[27] = new TH1F("Hist_Vt_Nneg","Number of primary vertex tracks (not broken ones)", 20, 0.,20.);
  if (!hp[28]) hp[28] = new TH1F("Hist_Vt_Nnsg","Number of primary vertex tracks (not short ones)", 20, 0.,20.);
  if (!hp[29]) hp[29] = new TH1F("Hist_Vt_Npro","Number of primary vertex tracks (high probability)", 20, 0.,20.);
  if (!hp[30]) hp[30] = new TH1F("Hist_Vt_Noko","Number of rejected primary vertex tracks", 20, 0.,20.);
  if (!hp[31]) hp[31] = new TH1F("Hist_Vt_DZpat","DZ for ajunced patterns", 300, 0.,3000.);
  if (!hpp[0]) hpp[0] = new TProfile("Hist_Vt_Zpat","Segments Z vs pattern number", 60, -1.,59., -1000000., +1000000, "s");

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
  hld3.Add(hp[26]);
  hld3.Add(hp[25]);

  hld4.Add(hp[22]);
  hld4.Add(hp[23]);
  hld4.Add(hp[24]);
  hld4.Add(hp[14]);

  hld5.Add(hp[27]);
  hld5.Add(hp[28]);
  hld5.Add(hp[29]);
  hld5.Add(hp[30]);

  hld6.Add(hpp[0]);
  hld6.Add(hp[31]);

  for (int i=0; i<32; i++)
    if (hp[i]) hlist.Add(hp[i]);
}

double smass = 0.1395700;

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
  hp[14]->Fill(v.rmsDistAngle());
  hp[18]->Fill(v.angle()*1000.);
  hp[22]->Fill(v.ntracks());
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
	if (hp[13]) hp[13]->Fill(tr->W());
  }
}

///-----------------------------------------------------------------------
void DrawHistsV()
{
  TCanvas *cv1 = new TCanvas("Vertex_reconstruction_1","MC Vertex reconstruction", 760, 760);
  DrawHlist(cv1, hld1);

  TCanvas *cv2 = new TCanvas("Vertex_reconstruction_2","Vertex reconstruction (tracks hists)", 600, 760);
  DrawHlist(cv2, hld2);

  TCanvas *cv3 = new TCanvas("Vertex_reconstruction_3","Vertex reconstruction (eff hists)", 600, 760);
  DrawHlist(cv3, hld3);

  TCanvas *cv4 = new TCanvas("Vertex_reconstruction_4","Vertex reconstruction (ntracks)", 600, 760);
  DrawHlist(cv4, hld4);

  TCanvas *cv5 = new TCanvas("Vertex_reconstruction_5","Vertex reconstruction (1st vertex tracks)", 600, 760);
  DrawHlist(cv5, hld5);

  TCanvas *cv6 = new TCanvas("Vertex_reconstruction_6","Vertex reconstruction (1st pattern Z)", 600, 760);
  DrawHlist(cv6, hld6);
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
void d() { DrawHistsV();}
///------------------------------------------------------------
void ClearHistsV()
{
  for(int i=0; hp[i]; i++) {
    hp[i]->Clear();
  }
}
//______________________________________________________________________________

#define MAXCODES 10000
int Geant[2*MAXCODES];
//______________________________________________________________________________

void GCodesInit() {

  for(int i=0;i<MAXCODES*2;i++) Geant[i]=0;

  Geant[22]=1;               // photon
  Geant[MAXCODES+11]=2;      // e+
  Geant[11]=3;               // e-
  Geant[12]=4;               // e-neutrino (NB: flavour undefined by Geant)
  Geant[14]=4;               // mu-neutrino (NB: flavour undefined by Geant)
  Geant[16]=4;               // tau-neutrino (NB: flavour undefined by Geant)
  Geant[MAXCODES+13]=5;      // mu+
  Geant[13]=6;               // mu-
  Geant[111]=7;              // pi0 
  Geant[211]=8;              // pi+
  Geant[MAXCODES+211]=9;     // pi-
  Geant[130]=10;             // K long
  Geant[321]=11;             // K+
  Geant[MAXCODES+321]=12;    // K-
  Geant[2112]=13;            // n
  Geant[2212]=14;            // p
  Geant[MAXCODES+2212]=15;   // anti-proton
  Geant[310]=16;             // K short
  Geant[221]=17;             // eta
  Geant[3122]=18;            // Lambda
  Geant[3222]=19;            // Sigma+
  Geant[3212]=20;            // Sigma0
  Geant[3112]=21;            // Sigma-
  Geant[3322]=22;            // Xi0
  Geant[3212]=23;            // Xi-
  Geant[3334]=24;            // Omega- (PB) ???diff from babar
  Geant[MAXCODES+2112]=25;   // anti-neutron
  Geant[MAXCODES+3122]=26;   // anti-Lambda
  Geant[MAXCODES+3222]=27;   // Sigma-
  Geant[MAXCODES+3212]=28;   // Sigma0
  Geant[MAXCODES+3112]=29;   // Sigma+ (PB)*/  
  Geant[MAXCODES+3322]=30;   // Xi0
  Geant[MAXCODES+3312]=31;   // Xi+
  Geant[MAXCODES+3334]=32;   // Omega+ (PB) ???diff from babar
  Geant[MAXCODES+15]=33;     // tau+
  Geant[15]=34;              // tau-
  Geant[411]=35;             // D+
  Geant[MAXCODES+411]=36;    // D-
  Geant[421]=37;             // D0
  Geant[MAXCODES+421]=38;    // D0
  Geant[431]=39;             // Ds+
  Geant[MAXCODES+431]=40;    // anti Ds-
  Geant[4122]=41;            // Lamba_c+
  Geant[24]=42;              // W+
  Geant[MAXCODES+24]=43;     // W-
  Geant[23]=44;              // Z
  Geant[0]=45;               // deuteron
  Geant[0]=46;               // triton
  Geant[0]=47;               // alpha
  Geant[0]=48;               // G nu ? PDG ID 0 is undefined
}
//______________________________________________________________________________

int Pdg2Geant(int val) {

  if (abs(val)>MAXCODES) return(-1);

  if (val>=0 && (val < MAXCODES)) return( Geant[val] );
  else if (val < 0 && (-val < MAXCODES)) return( Geant[MAXCODES-val] );
  else return 8;
}
//______________________________________________________________________________

double MassGeant(int val) {
double masses[50] = {
    0.,
    0.00051099906,
    0.00051099906,
    0.,
    0.105658389,
    0.105658389,
    0.1349764,
    0.1395700,
    0.1395700,
    0.497672,
    0.493677,
    0.493677,
    0.93956563,
    0.93827231,
    0.93827231,
    0.497672,
    0.54745 ,
    1.115684,
    1.18937 ,
    1.19255 ,
    1.197436,
    1.3149  ,
    1.32132 ,
    1.67245 ,
    0.93956563,
    1.115684,
    1.18937 ,
    1.19255 ,
    1.197436,
    1.3149  ,
    1.32132 ,
    1.67245 ,
    1.7771  ,
    1.7771  ,
    1.8694  ,
    1.8694  ,
    1.8646  ,
    1.8646  ,
    1.9685  ,
    1.9685  ,
    2.2851  ,
    80.220  ,
    80.220  ,
    91.187  ,
    1.875613,
    2.80925 ,
    3.727417,
    0.      ,
    2.80923 ,
    0.};

  if (abs(val) >= MAXCODES) return(0.13957);
  int gc = 0;
  if (val>=0) gc = Geant[val];
  else gc = Geant[MAXCODES-val];
  if(gc > 0)
    return masses[gc-1];
  else
    return(0.13957);
}
//______________________________________________________________________________
void SmearSegments()
{
  // smearing with parameters defined by ScanCond

  Float_t x,y,z,tx,ty;
  Float_t sx,sy,sz,stx,sty,sumz, sumzold;

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;
  EdbTrackP  *tr  = 0;
  int trgind = 0, Track = 0;
  int n = vol->Npatterns();

  sumzold = -1.;
  for( int i=0; i<n; i++ ) {
    pat = vol->GetPattern(i);

    sumz = 0.;
    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);

      Track = seg->Aid(1);
      trgind  = -1;
      if (Track < 500) trgind = Numgent[Track];
      if (gener && trgind >= 0)
      {
        if (gener->eTracks)
        {
	    if (trgind < gener->eTracks->GetEntries())
	    {
		tr = (EdbTrackP *)gener->eTracks->At(trgind);
		if (tr) tr->AddSegment(seg);
	    }
        }
      }
      if (smear)
      {
        sx  = scan->SigmaX(seg->TX());
        sy  = scan->SigmaY(seg->TY());
        sz  = 0.;
        stx = scan->SigmaTX(seg->TX());
        sty = scan->SigmaTY(seg->TY());

        x  = gRandom->Gaus(seg->X(),   sx);
        y  = gRandom->Gaus(seg->Y(),   sy);
        z  = gRandom->Gaus(seg->Z(),   sz);
        tx = gRandom->Gaus(seg->TX(), stx);
        ty = gRandom->Gaus(seg->TY(), sty);

        seg->Set( seg->ID(), x, y, tx, ty, seg->W(), seg->Flag());
        seg->SetErrorsCOV( sx*sx, sy*sy, sz*sz, stx*stx, sty*sty );
      }
      if (setz)
      {
	z = seg->Z();
        sumz += z;
        hpp[0]->Fill(i, z);
	if (i && sumzold != -1.) hp[31]->Fill(sumzold - z);
//	if (i && sumzold != -1.) hp[31]->Fill(z - sumzold);
        seg->SetZ( (n - i - 1)*1290. );
//        seg->SetZ( i*1290. );
      }
    }
    if (pat->N()) sumzold = sumz/pat->N();
    else sumzold = -1.;
//    if ((i == (n-1)) && setz && (sumzold != -1.)) z0shift = sumzold;
  }
}

