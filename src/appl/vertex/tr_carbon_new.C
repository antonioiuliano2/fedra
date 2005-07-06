using namespace VERTEX;

EdbDataProc  *dproc=0;
EdbPVRec     *gAli=0;
EdbDisplay   *ds=0;
EdbVertexRec *gEVR=0;

float ProbMinV  = 0.01;	// min vertex probability 
float ProbMinP  = 0.01;	// minimal propagate probability
int   nsegMin   = 3;
int   MINMULT   = 3;	// minimum event multiplicity
bool  usemom    = false;
float momentum  = 1.;	// assumed tracks momenta, GeV/c
float dpp       = .5;	// assumed average track momentum error, dp/p
bool  UseSegPar = false;// use measured segment parameters
bool  ReFit     = false;	// re-fit tracks from linked_tracks.root
			// (for example, with another momenta)
bool  ReProp    = false;	// additional propagations of tracks from linked_tracks.root
			// (may be, some improvement?)
			// (this operation can lead to join parts of track)

TH1F *hp[23] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
TObjArray hlist, hld1, hld2, hld3, hld4;

//---------------------------------------------------------------------
void tr_carbon_new( const char *rcut = "nseg>2&&t.eFlag>-1" )
{
  BookHistsV();

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

  if (ReFit)
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

  if (ReProp)
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

  if (hp[10]) hp[10]->Fill(ntr);
  float ang = 0.;
  float tx  = 0.;
  float ty  = 0.;
  // tracks loop
  ntr = gAli->eTracks->GetEntries();
  int nt = 0;
  for(int i=0; i<ntr; i++) 
  {
    tra = (EdbTrackP*)(gAli->eTracks->At(i));
    if (!tra) continue;
//    tra->Print();
    if (tra->Flag() != -10 && tra->N() >= 2) // skip "broken" tracks
    {
        // fill hist with number of segments
	if (hp[11]) hp[11]->Fill(tra->N());
        // fill hist with difference between measured and reconstructed coordinate
	if (hp[12]) hp[12]->Fill((tra->GetSegment(0))->X()-(tra->GetSegmentF(0))->X());
        // fill hist with track probability
	if (hp[17]) hp[17]->Fill(tra->Prob());
	tx = tra->TX();
	ty = tra->TY();
	ang = TMath::ACos(1./(1.+tx*tx+ty*ty))*1000.;
        // fill hist with track Teta angle
	if (hp[20]) hp[20]->Fill(ang);
        // fill hist with track momentum
	if (hp[21]) hp[21]->Fill(tra->P());
	nt++;
    }
  }
  printf("%d good tracks was found\n",nt);

  gEVR = new EdbVertexRec();
  if (!(gAli->eTracks)) gAli->eTracks = new TObjArray(100);
  gEVR->eEdbTracks = gAli->eTracks;
  if (!(gAli->eVTX)) gAli->eVTX = new TObjArray(100);
  gEVR->eVTX       = gAli->eVTX;
  gEVR->ePVR       = gAli;

  gEVR->eZbin       = 100.;      // default is 100. microns
  gEVR->eAbin       = 0.01;      // default is 0.01 rad
  gEVR->eDZmax      = 3000.;     // default is 3000. microns
  gEVR->eProbMin    = ProbMinV;  // default 0.01, i.e 1%
  gEVR->eImpMax     = 55.;       // default is 25. microns
  gEVR->eUseSegPar  = UseSegPar; // default is FALSE - use fitted track parameters
  gEVR->eQualityMode= 0;         // 0 - prob/(sx**2+sy**2), 1 = 1./aver_impact

  int nvtx = gEVR->FindVertex();
  printf("%d 2-track vertexes was found\n",nvtx);

  if(nvtx != 0) {
    int nadd = gEVR->ProbVertexN();
    printf("%d vertexes with number of tracks >2 was found\n",nadd);
    printf("----------------------------------------------\n");
    EdbVertex *v = 0;
    Vertex *vt = 0;
    for(int i=0; i<nvtx; i++)
    {
	v = (EdbVertex*)(gEVR->eVTX->At(i));
	if (!v) continue;
//	if (i<10) v->Print();
	if (v->Flag() < 0) continue;
	vt = v->V();
	if (vt) FillHistsV(*vt);
    }
  }
//  vd();
}

//---------------------------------------------------------------------
void td()
{
  TObjArray *trarr=dset->PVR()->eTracks;
  gStyle->SetPalette(1);

  const char *title = "display-tracks";
  if(!(ds=EdbDisplay::EdbDisplayExist(title))) 
    ds=new EdbDisplay(title,-50000.,50000.,-50000., 50000., -4000.,80000.);

  ds->SetVerRec(gEVR);
  ds->SetDrawTracks(4);
  ds->SetArrTr( trarr );
  ds->Draw();
}

//---------------------------------------------------------------------
void vd( int trmin=3, int trmax=100, float amin=.015, float amax=2.)
{
  TObjArray *varr = new TObjArray();
  TObjArray *tarr = new TObjArray();

  EdbVertex *v=0;
  EdbTrackP *t=0;

  int nv = gEVR->eVTX->GetEntries();
  for(int i=0; i<nv; i++) {
    v = (EdbVertex *)(gEVR->eVTX->At(i));

    if( v->N()<trmin )                       continue;
    if( v->N()>trmax )                      continue;
    if( v->Flag()<0  )                      continue;
    if( v->N()<3 && v->MaxAperture()<amin )  continue;//
    if( v->N()<3 && v->MaxAperture()>amax )  continue;

    varr->Add(v);
    for(int j=0; j<v->N(); j++) tarr->Add( v->GetTrack(j) );
  }

  gStyle->SetPalette(1);
  const char *title = "display-vertices";
  if(!(ds=EdbDisplay::EdbDisplayExist(title))) 
//    ds=new EdbDisplay(title,-30000.,2000., -2000.,42000.,-80000.,80000.);
    ds=new EdbDisplay(title,60000.,70000., 45000.,50000., 0., 110000.);
  
  ds->SetVerRec(gEVR);
  ds->SetArrTr( tarr );
  ds->SetArrV( varr );
  ds->SetDrawTracks(4);
  ds->SetDrawVertex(1);
  ds->Draw();
}
///-----------------------------------------------------------------------
void BookHistsV()
{
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_Sx","Vertex X sigma",100,0.,10.);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_Sy","Vertex Y sigma",100,0.,10.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_Sz","Vertex Z sigma",100,0.,500.);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",25,0.,5.);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",26,0.,1.04);
  if (!hp[9]) hp[9] = new TH1F("Hist_Vt_Nvert","Number of reconstructed vertexs",100,0.,100.);
  if (!hp[22]) hp[22] = new TH1F("Hist_Vt_Ntracks","Number of tracks at vertex", 20, 0.,20.);
  if (!hp[14]) hp[14] = new TH1F("Hist_Vt_Dist","RMS track-vertex distance",100, 0.,50.);
  if (!hp[18]) hp[18] = new TH1F("Hist_Vt_AngleV","RMS track-track angle, mrad", 100, 0.,1000.);

  if (!hp[10]) hp[10] = new TH1F("Hist_Vt_Ntrack","Number of reconstructed tracks",100,0.,10000.);
  if (!hp[11]) hp[11] = new TH1F("Hist_Vt_Nsegs","Number of track segments",60,0.,60.);
  if (!hp[12]) hp[12] = new TH1F("Hist_Vt_Dtrack","Track DeltaX, microns",100,-5.,+5.);
  if (!hp[17]) hp[17] = new TH1F("Hist_Vt_Trprob","Track probability",110, 0.,1.1);
  if (!hp[20]) hp[20] = new TH1F("Hist_Vt_Angle","Track angle, mrad", 100, 0.,1000.);
  if (!hp[21]) hp[21] = new TH1F("Hist_Vt_Momen","Track momentum, GeV/c", 100, 0.,20.);

  hld1.Add(hp[3]);
  hld1.Add(hp[4]);
  hld1.Add(hp[5]);
  hld1.Add(hp[6]);
  hld1.Add(hp[7]);
  hld1.Add(hp[9]);
  hld1.Add(hp[22]);
  hld1.Add(hp[14]);
  hld1.Add(hp[18]);

  hld2.Add(hp[10]);
  hld2.Add(hp[11]);
  hld2.Add(hp[12]);
  hld2.Add(hp[17]);
  hld2.Add(hp[20]);
  hld2.Add(hp[21]);

  for (int i=0; i<22; i++)
    if (hp[i]) hlist.Add(hp[i]);
}

///-----------------------------------------------------------------------
void FillHistsV(Vertex &v)
{
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
void DrawHistsV()
{

  int n = hld1.GetEntries();
  if (n)
  {
    TCanvas *cv1 = new TCanvas("Vertex_reconstruction_1","MC Vertex reconstruction", 760, 760);
    DrawHlist(cv1, hld1);
  }

  n = hld2.GetEntries();
  if (n)
  {
    TCanvas *cv2 = new TCanvas("Vertex_reconstruction_2","Vertex reconstruction (tracks hists)", 600, 760);
    DrawHlist(cv2, hld2);
  }

  n = hld3.GetEntries();
  if (n)
  {
    TCanvas *cv3 = new TCanvas("Vertex_reconstruction_3","Vertex reconstruction (eff hists)", 600, 760);
    DrawHlist(cv3, hld3);
  }

  n = hld4.GetEntries();
  if (n)
  {
    TCanvas *cv4 = new TCanvas("Vertex_reconstruction_4","Vertex reconstruction (ntracks)", 600, 760);
    DrawHlist(cv4, hld4);
  }
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
  for(int i=0; i<22; i++) {
    if (hp[i]) hp[i]->Clear();
  }
}
