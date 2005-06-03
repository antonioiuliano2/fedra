//----------------------------------------------
// usage: root RecDispEX.C
// dsv(54)  - display vertex 54
//----------------------------------------------


using namespace VERTEX;

EdbDataProc  *dset=0;
EdbDisplay   *ds=0;
EdbPVRec     *gAli=0;
EdbPVRec     *ali=0;

int maxgaps[6] = {0,3,3,6,3,6}; // all combinations
float AngleAcceptance = 1.0;   // maximal difference between tracks slopes
float ProbMinV  = 0.01;      // min vertex probability 
float ProbMinP  = 0.01;     // minimal propagate probability
float ProbMinT  = 0.01;     // minimal track probability to be used for vtx
int   nsegMin = 3;           // minimal track segments number to be used for vtx
bool  usemom = false;

TH1F *hp[23] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
TObjArray hlist, hld1, hld2, hld3, hld4;

void RecDispEX()
{
  init();
}

//---------------------------------------------------------
void init( char *file="ali.root" )
{
  TFile *f = new TFile(file);
  gAli = (EdbPVRec*)(f->FindKey("ali")->ReadObj());
  // hists booking
  BookHistsV();
  // link microtracks
  gAli->Link();

  gAli->FillTracksCell(); 
  // make track candidates
  gAli->MakeTracks();
  if (!gAli->eTracks) return;

  int ntr = gAli->eTracks->GetEntries();
  // set track momentum error - 20% for 4 GeV/c
  for(int i=0; i<ntr; i++) 
    ((EdbTrackP*)(gAli->eTracks->At(i)))->SetErrorP(0.2*0.2*4.*4.);
  // tracks fit assuming momentum 4 GeV/c and pion mass
  gAli->FitTracks(4.);

  gAli->FillCell(50,50,0.015,0.015);

  // merge consistient tracks
  gAli->PropagateTracks(29,2,ProbMinP);

  if (!(gAli->eTracks)) return;
  ntr = gAli->eTracks->GetEntries();
  // fill hist with number of tracks
  if (hp[10]) hp[10]->Fill(ntr);
  EdbTrackP *tr = 0;
  float ang = 0.;
  float tx  = 0.;
  float ty  = 0.;
  // tracks loop
  for(int i=0; i<ntr; i++) 
  {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    if (tr->Flag() != -10) // skip "broken" tracks
    {
        // fill hist with number of segments
	if (hp[11]) hp[11]->Fill(tr->N());
        // fill hist with difference between measured and reconstructed coordinate
	if (hp[12]) hp[12]->Fill(tr->GetSegment(0)->X()-tr->GetSegmentF(0)->X());
        // fill hist with track probability
	if (hp[17]) hp[17]->Fill(tr->Prob());
	tx = tr->TX();
	ty = tr->TY();
	ang = TMath::ACos(1./(1.+tx*tx+ty*ty))*1000.;
        // fill hist with track Teta angle
	if (hp[20]) hp[20]->Fill(ang);
        // fill hist with track momentum
	if (hp[21]) hp[21]->Fill(tr->P());
    }
  }
  // find 2-track vertexes
  int nvtx = gAli->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);
  printf("%d 2-track vertexes was found\n",nvtx);

  if(nvtx != 0) {
    // merge 2-tracks vertexes into N-tracks
    int nadd = gAli->ProbVertexN(ProbMinV);
    printf("%d vertexes with number of tracks >2 was found\n",nadd);
  }

  if (!(gAli->eVTX)) return;
  EdbVertex *v = 0;
  EdbVertex *vc = 0;
  Vertex *vt = 0;
  int nv = gAli->eVTX->GetEntries();
  int nvg = 0;
  for(int i=0; i<nv; i++) 
  {
    v = (EdbVertex*)(gAli->eVTX->At(i));
    if (v->Flag() != -10) // skip "used" vertexes
    {
	    nvg++;
	    vt = v->V();
	    if (vt) FillHistsV(*vt);
	    if (v->N() > 2) v->Print();
    }
  }
  // fill hist with number of vertexes (exclude "used" ones)
  if (hp[9]) hp[9]->Fill(nvg);

  if (nv)
  {
    // set corresponding flag value for "linked" vertexes
    int nlv = gAli->LinkedVertexes();
    printf("%d linked vertexes found\n", nlv);
    if (nlv)
    {
      for(int i=0; i<nv; i++) 
      {
	v = (EdbVertex*)(gAli->eVTX->At(i));
	if (v->Flag() > 2) // select "linked" vertexes only
	{
	    vc = v->GetConnectedVertex(0);
	    if (vc->ID() > v->ID())
	    {
//		v->Print();
//		vc->Print();
	    }
	}
      }
    }
  }
  // fill vertexes neighborhood lists with tracks and segments
  int nn = gAli->VertexNeighboor(1000., 2);
  printf("%d neighbooring tracks found\n", nn);
  // store results
  //TFile fo("alirec.root","RECREATE");
  //gAli->Write("alirec");
  //fo.Close();
}

//---------------------------------------------------------
void init_set(char *data_set)
{
  dset=new EdbDataProc(data_set);
  dset->InitVolume(0);
  gAli = dset->PVR();

  TFile f("ali.root","RECREATE");
  gAli->Write("ali");
  f.Close();
}

//---------------------------------------------------------
void ds(int iseg=0, float binx=20, float bint=10, int ntr=1)
{
  if(!gAli) init();
  if(ali) delete ali;

  EdbSegP *seg=0;     // direction

  TObjArray *arr   = new TObjArray();
  TObjArray *arrtr = new TObjArray();

  for(int i=0; i<ntr; i++) {
    EdbTrackP *track = (EdbTrackP*)(gAli->eTracks->At(iseg+i));
    track->SetSegmentsTrack();

    arrtr->Add(track);
    gAli->ExtractDataVolumeSeg( *track,*arr,binx,bint );
  }

    gStyle->SetPalette(1);
    const char *title = "display-segments";
    if(!EdbDisplay::EdbDisplayExist(title)) 
      ds=new EdbDisplay(title,-15000.,15000.,-15000.,15000.,-4000.,40000.);

    ds->SetArrSegP( arr );
    ds->SetArrTr( arrtr );
    ds->Draw();
}
//---------------------------------------------------------
void dsv( int numv = -1, int ntrMin=2, float binx=6, float bint=10 )
{
  if(!gAli->eVTX) return;
  if(!gAli) init();

  EdbSegP *seg=0;     // direction

  TObjArray *arrV  = new TObjArray();  // vertexes
  TObjArray *arr   = new TObjArray();  // segments
  TObjArray *arrtr = new TObjArray();  // tracks

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
    printf("Vertex %d, multiplicity %d\n", iv, ntr);
    for(int i=0; i<ntr; i++) {
      EdbTrackP *track = v->GetTrack(i);
      printf("     Track ID %d, Z = %f, Nseg = %d, Zpos = %d\n",
      track->ID(), track->Z(), track->N(), v->Zpos(i));
      arrtr->Add(track);
      gAli->ExtractDataVolumeSeg( *track,*arr,binx,bint );
    }

  }

  gStyle->SetPalette(1);
  const char *title = "display-vertexes";
  if(!EdbDisplay::EdbDisplayExist(title)) 
    ds=new EdbDisplay(title,-15000.,15000.,-15000.,15000.,-4000.,40000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrV );
  ds->SetDrawVertex(1);
  ds->SetDrawTrack(4);
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
///------------------------------------------------------------
void analyze( char *file="alirec.root" )
{
  TFile *f = new TFile(file);
  gAli = (EdbPVRec*)(f->FindKey("alirec")->ReadObj());

  BookHistsV();

  if (!gAli->eTracks) return;

  printf("\n%d tracks are in the list\n",gAli->eTracks->GetEntriesFast());

  ntr = gAli->eTracks->GetEntries();
  if (hp[10]) hp[10]->Fill(ntr);
  EdbTrackP *tr = 0;
  float ang = 0.;
  float tx  = 0.;
  float ty  = 0.;
  for(int i=0; i<ntr; i++) 
  {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    if (tr->Flag() != -10)
    {
	if (hp[11]) hp[11]->Fill(tr->N());
	if (hp[12]) hp[12]->Fill(tr->GetSegment(0)->X()-tr->GetSegmentF(0)->X());
	if (hp[17]) hp[17]->Fill(tr->Prob());
	tx = tr->TX();
	ty = tr->TY();
	ang = TMath::ACos(1./(1.+tx*tx+ty*ty))*1000.;
	if (hp[20]) hp[20]->Fill(ang);
	if (hp[21]) hp[21]->Fill(tr->P());
    }
  }

  if (!(gAli->eVTX)) return;
  EdbVertex *v = 0;
  EdbVertex *vc = 0;
  Vertex *vt = 0;
  int nv = gAli->eVTX->GetEntries();
  int nvg = 0;
  for(int i=0; i<nv; i++) 
  {
    v = (EdbVertex*)(gAli->eVTX->At(i));
    if (v->Flag() != -10)
    {
	    nvg++;
	    vt = v->V();
	    if (vt) FillHistsV(*vt);
	    if (v->N() > 2) v->Print();
    }
  }
  
  printf("%d vertexes found\n", nvg);

  if (hp[9]) hp[9]->Fill(nvg);

  int nlv = 0;
  int nn = 0;
  if (nv)
  {

      for(int i=0; i<nv; i++) 
      {
	v = (EdbVertex*)(gAli->eVTX->At(i));
	if (v->Flag() > 2)
	{
	    vc = v->GetConnectedVertex(0);
	    if (vc->ID() > v->ID())
	    {
//		v->Print();
//		vc->Print();
		nlv++;
	    }
	}
	nn += v->Nn();
      }
  }
  
  printf("%d linked vertexes found\n", nlv);

  printf("%d neighbooring tracks and segments found\n", nn);

}
