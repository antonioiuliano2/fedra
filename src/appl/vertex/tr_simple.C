using namespace VERTEX;

EdbDataProc  *dproc=0;
EdbPVRec     *gAli=0;
EdbVertexRec *gEVR=0;

float ProbMinV  = 0.01;	// min vertex probability 
float ProbMinP  = 0.01;	// minimal propagate probability
float momentum  = 1.;	// assumed tracks momenta, GeV/c
float dpp       = .5;	// assumed average track momentum error, dp/p
bool  UseSegPar = false;// use measured segment parameters
bool  ReFit     = false;// re-fit tracks from linked_tracks.root
			// (for example, with another momenta)
bool  ReProp    = false;// additional propagations of tracks from linked_tracks.root
			// (may be, some improvement?)
			// (this operation can lead to join parts of track)

//---------------------------------------------------------------------
void tr_simple( const char *rcut = "nseg>2&&t.eFlag>-1" ) // track selection criteria
{
  dproc = new EdbDataProc("Carbonium_data_set.def");
  dproc->InitVolume(100, rcut);

  gAli = dproc->PVR();

  int ntr = gAli->eTracks->GetEntries();
  printf("\n%d tracks are in the list\n",ntr);

  EdbTrackP *tra = 0;
  int ntrgood = 0;
  for(int i=0; i<ntr; i++)
  {
	tra = ((EdbTrackP*)(gAli->eTracks->At(i)));
	if (tra->Flag() >= 0) ntrgood++;
  }
  printf("\n%d good tracks are in the list\n",ntrgood);

  if (ReFit)   // re-fit tracks
  {
    for(int i=0; i<ntr; i++)
    {
	tra = ((EdbTrackP*)(gAli->eTracks->At(i)));
	if (tra->Flag() < 0) continue;	 
	tra->SetErrorP(dpp*dpp*momentum*momentum);
	tra->GetSegmentFirst()->SetErrorP(dpp*dpp*momentum*momentum);
	tra->GetSegmentLast()->SetErrorP(dpp*dpp*momentum*momentum);
    }
    gAli->FitTracks(momentum,0.139);
  }

  gAli->FillCell(50,50,0.015,0.015);

  if (ReProp)   // apply additional propagation
  {
    gAli->PropagateTracks(55,2,ProbMinP);
    ntrgood = 0;
    for(int i=0; i<ntr; i++)
    {
	tra = ((EdbTrackP*)(gAli->eTracks->At(i)));
	if (tra->Flag() >= 0) ntrgood++;
    }
    printf("\n%d good tracks are in the list after additional propagation\n",
	    ntrgood);
  }

  // tracks loop
  ntr = gAli->eTracks->GetEntries();
  ntrgood = 0;
  int nseg = 0;
  EdbSegP *seg = 0;
  for(int i=0; i<ntr; i++) 
  {
    tra = (EdbTrackP*)(gAli->eTracks->At(i));
    if (!tra) continue;
    if (tra->Flag() != -10) // skip "broken" tracks
    {
	if (tra->N() < 10) continue; // skip "short" tracks
				     // you can put here additional cuts 

	// you can put here track analysis
        // number of segments is tra->N()
        // track probability is tra->Prob()
	// etc
	ntrgood++;
	nseg = tra->N();  // number of basetracks (segments) in this track
	for (int j=0; j<nseg; j++) // j = 0      - segment with minimal Z 
	{			   //   = nseg-1 - segment with maximal Z
	    seg = tra->GetSegment(j);
	    if (!seg) continue;
	    // you can put here basetracks analysis
	}
    }
  }
  printf("%d good tracks was found\n",ntrgood);

  gEVR = new EdbVertexRec();
  if (!(gAli->eTracks)) gAli->eTracks = new TObjArray(100);
  gEVR->eEdbTracks = gAli->eTracks;
  if (!(gAli->eVTX)) gAli->eVTX = new TObjArray(100);
  gEVR->eVTX       = gAli->eVTX;
  gEVR->ePVR       = gAli;

  gEVR->eDZmax      = 3000.;     // default is 3000. microns
  gEVR->eProbMin    = ProbMinV;  // minimal vertex probability,
			         // default 0.01, i.e. 1%
  gEVR->eImpMax     = 55.;       // maximal impact parameter-distance between
				 // tracks, for 2-tracks vertices, default is 25. microns
  gEVR->eUseSegPar  = UseSegPar; // default is FALSE - use fitted track parameters,
				 // not measured values for nearest basetrack
  gEVR->eQualityMode= 0;         // 0 - prob/(sx**2+sy**2), 1 = 1./aver_impact

  int nvtx = gEVR->FindVertex(); // find 2-prongs vertices
  printf("%d 2-track vertexes was found\n",nvtx);

  if(nvtx != 0) {
 
    int nadd = gEVR->ProbVertexN();  // find N-prongs vertices by joining 2-prongs
    printf("%d vertexes with number of tracks >2 was found\n",nadd);
    printf("----------------------------------------------\n");

    EdbVertex *v = 0;
    Vertex *vt = 0;
    int ntv = 0;
    for(int i=0; i<nvtx; i++)
    {
	v = (EdbVertex*)(gEVR->eVTX->At(i));
	if (!v) continue;
	if (v->Flag() < 0) continue;  // skip used 2-tracks vertices
	ntv=v->N();                   // number of prongs (tracks) at vertex
	// v->VX() is vertex X coordinate
	// v->VY() is vertex Y coordinate
	// v->VZ() is vertex Z coordinate
	vt = v->V();
	// vt->ndf() is number of degree of freedom
	// vt->chi2() is vertex chi2
	// vt->prob() is vertex probability
	// vt->rmsDistAngle() is average distance from vertex to tracks
	// vt->angle() is average angle between tracks

	for(int j=0; j<ntv; j++) 
	{
	    tra = v->GetTrack(j);
	    if (!tra) continue;
	    // you can put here analysis of vertex tracks

	    nseg = tra->N();  // number of basetracks (segments) in this track
	    for (int k=0; k<nseg; k++) 
	    {
		seg = tra->GetSegment(k);
		if (!seg) continue;
		// you can put here basetracks analysis for vertex tracks
	    }
	}
    }
  }
}
