EdbDataProc  *dset=0;
EdbDisplay   *ds=0;
EdbPVRec     *gAli=0;
EdbPVRec     *ali=0;


int maxgaps[6] = {0,2,2,6,2,6}; // all combinations

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

float AngleAcceptance = 0.4;

float ProbMinV  = 0.01;      // min vertex probability 
float ProbMinP  = 0.01;     // minimal propagate probability
float ProbMinT  = 0.01;     // minimal track probability to be used for vtx
int   nsegMin=4;
bool  usemom=false;

void disp()
{
  //init(data_set);
  init_rec();
}

//---------------------------------------------------------
void init_rec(char *data_set="data_set.def")
{
  dset=new EdbDataProc(data_set);
  dset->InitVolume(0);
  gAli = dset->PVR();

  gAli->Link();
  printf("link ok\n");

  gAli->FillTracksCell();
  gAli->MakeTracks();

  if (!gAli->eTracks) return;

  int ntr = gAli->eTracks->GetEntries();
  for(int i=0; i<ntr; i++) 
    ((EdbTrackP*)(gAli->eTracks->At(i)))->SetErrorP(0.1);

  //gAli->FitTracks(-1., -1., gener->eTracks);
  gAli->FitTracks(4.);
  gAli->FillCell(50,50,0.015,0.015);
  gAli->PropagateTracks(55,2,ProbMinP);

//  int ntr = gAli->eTracks->GetEntries();
//  EdbTrackP *tr = 0;
//  for(int i=0; i<ntr; i++) 
//  {
//    tr = (EdbTrackP *)(gAli->eTracks->At(i));
//    tr->SetErrorP(0.1);
//    if (tr->ID() == 10 ||tr->ID() == 16 || tr->ID() == 21 )
//    {
//	tr->SetFlag(-10);
//    }
//  }
  
  int nvtx = gAli->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);
  printf("%d 2-track vertexes was found\n",nvtx);

  EdbVertex *v  = 0;
  EdbVertex *vc = 0;
//  for(int iv=0; iv<nvtx; iv++) 
//  {
//    v = (EdbVertex*)(gAli->eVTX->At(iv));
//    v->Print();
//  }
  if(nvtx != 0) {
    int nadd = gAli->ProbVertexN(ProbMinV);
    printf("%d vertexes with number of tracks >2 was found\n",nadd);
  }

  if (!(gAli->eVTX)) return;
  int nv = gAli->eVTX->GetEntries();
  for(int i=0; i<nv; i++) 
  {
    v = (EdbVertex*)(gAli->eVTX->At(i));
    if (v->Flag() != -10)
    {
	v->Print();
    }
  }
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
  if(!gAli) init();

  TObjArray *arr   = new TObjArray();  

  EdbTrackP *track = (EdbTrackP*)(gAli->eTracks->At(0));

  gAli->ExtractDataVolumeSegAll( *arr );
 
  gStyle->SetPalette(1);
  if(!ds) 
    //ds=new EdbDisplay("display-t",-60000.,60000.,-60000., 62000., 0.,100000.);
    ds=new EdbDisplay("display-t",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( gAli->eTracks );
  ds->SetDrawTracks(3);
  ds->Draw();
}







//---------------------------------------------------------
void dsv( int numv = -1, float binx=6, float bint=10 )
{
  if(!gAli->eVTX) return;
  if(!gAli) init();

  EdbSegP *seg=0;     // direction

  TObjArray *arrV  = new TObjArray();  // vertexes
  TObjArray *arr   = new TObjArray();  // segments
  TObjArray *arrtr = new TObjArray();  // tracks

  int ntrMin=3;
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
  for(int iv=iv0; iv<iv1; iv++)  {   
    v = (EdbVertex *)(gAli->eVTX->At(iv));
    if(!v)            continue;
    if(v->N()<ntrMin) continue;

    arrV->Add(v);

    int ntr = v->N();
//    printf("ntr=%d\n",ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      //track->Print();
      printf("Vertex %d, Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      iv, track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
      gAli->ExtractDataVolumeSeg( *track,*arr,binx,bint );
    }

  }

  gStyle->SetPalette(1);
  if(!ds)
    ds=new EdbDisplay("display-t",26000.,30000.,-56000.,-52000.,40000.,100000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrV );
  ds->SetDrawVertex(1);
  ds->Draw();
}

