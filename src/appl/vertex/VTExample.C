///-----------------------------------------------------------------------
using namespace VERTEX;

EdbDataProc  *dset=0;
EdbPVRec     *gAli=0;

int   maxgaps[6] = {0,3,3,6,3,6}; // all combinations
float AngleAcceptance = 1.0;  // max difference between tracks slopes (2-track vtx)
float ProbMinV  = 0.01;      // min vertex probability 
float ProbMinP  = 0.01;     // minimal propagate probability
float ProbMinT  = 0.01;     // minimal track probability to be used for vtx
int   nsegMin = 3;
bool  usemom = false;

//---------------------------------------------------------
void VTExample( char *data_set="july2003_data_set.def" )
{
  dset=new EdbDataProc(data_set);
  dset->InitVolume(0);
  gAli = dset->PVR();

  gAli->Link();

  gAli->FillTracksCell(); 
  gAli->MakeTracks();
  if (!gAli->eTracks) return;

  int ntr = gAli->eTracks->GetEntries();
  EdbTrackP *tr = 0;
  float p = 0.;
  // loop on tracks for momentum error setting
  for(int i=0; i<ntr; i++)
  {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    p  = 4.;  // use 20% error for 4 GeV/c
    tr->SetErrorP(0.2*0.2*p*p);
  }
  // fit all tracks assume momentum 4 GeV/c and pion mass
  gAli->FitTracks(4.);

  gAli->FillCell(50,50,0.015,0.015);
  // merge consistient tracks
  gAli->PropagateTracks(29,2,ProbMinP);
  // find 2-tracks vertexes
  int nvtx = gAli->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);
  printf("%d 2-track vertexes was found\n",nvtx);

  if(nvtx != 0) {
    // merge 2-tracks vertexes into N-tracks (if common track exist)
    int nadd = gAli->ProbVertexN(ProbMinV);
    printf("%d vertexes with number of tracks >2 was found\n",nadd);
  }

  if (!(gAli->eVTX)) return;
  EdbVertex *v = 0;
  EdbVertex *vc = 0;
  Vertex *vt = 0;
  int nv = gAli->eVTX->GetEntries();
  int nvg = 0;
  float xv,yv,zv,xve,yve,zve,probv, rmsdist,rmsang,chindfv;
  int ndfv, ntrv;
  for(int i=0; i<nv; i++) 
  {
    v = (EdbVertex*)(gAli->eVTX->At(i));
    if (v->Flag() != -10) // skip "broken" vertexes
    {
	    nvg++;
	    xv = v->VX(); // X vertex coordinate
	    yv = v->VY(); // Y vertex coordinate
	    zv = v->VZ(); // Z vertex coordinate
	    vt = v->V();  // Vt++ vertex pointer
	    xve = vt->vxerr(); // error on x-coordinate
	    yve = vt->vyerr(); // error on y-coordinate
	    zve = vt->vzerr(); // error on z-coordinate
	    chindfv = vt->ndf() > 0 ? vt->chi2()/vt->ndf() : 0;
	    probv = vt->prob(); // vertex chi square probability
	    rmsdist = vt->rmsDistAngle(); // RMS of track-vertex distances
	    rmsang = vt->angle(); // RMS of angles between tracks
	    ntrv = vt->ntracks(); // number of tracks in vertex
	    v->Print();
    }
  }
  printf("Total %d vertexes was found\n",nvg);

  if (nvg)
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

  int nn = gAli->VertexNeighboor(1000.,2);
  printf("%d neighbooring tracks found\n", nn);
}
///-----------------------------------------------------------------------
