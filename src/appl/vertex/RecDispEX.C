
using namespace VERTEX;

EdbDataProc  *dset=0;
EdbDisplay   *ds=0;
EdbPVRec     *gAli=0;
EdbPVRec     *ali=0;

int maxgaps[6] = {0,2,2,6,2,6}; // all combinations
float AngleAcceptance = 0.4;
float ProbMinV  = 0.001;      // min vertex probability 
float ProbMinP  = 0.001;     // minimal propagate probability
float ProbMinT  = 0.001;     // minimal track probability to be used for vtx
int   nsegMin=4;
bool  usemom=false;

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

  BookHistsV();

  gAli->Link();
  printf("link ok\n");

  gAli->FillTracksCell(); 
  gAli->MakeTracks();
  if (!gAli->eTracks) return;

  int ntr = gAli->eTracks->GetEntries();
  for(int i=0; i<ntr; i++) 
    ((EdbTrackP*)(gAli->eTracks->At(i)))->SetErrorP(0.1);

  gAli->FitTracks(4.);
  printf("\n%d tracks are in the list\n",gAli->eTracks->GetEntriesFast());

  gAli->FillCell(50,50,0.015,0.015);
  gAli->PropagateTracks(29,2,ProbMinP);

  if (!(gAli->eTracks)) return;
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

  int nvtx = gAli->ProbVertex(maxgaps, AngleAcceptance, ProbMinV, ProbMinT, nsegMin, usemom);
  printf("%d 2-track vertexes was found\n",nvtx);


  if(nvtx != 0) {
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
    if (v->Flag() != -10)
    {
	    nvg++;
	    vt = v->V();
	    if (vt) FillHistsV(*vt);
	    v->Print();
    }
  }

  if (hp[9]) hp[9]->Fill(nvg);

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

  TFile f("alirec.root","RECREATE");
  gAli->Write("alirec");
  f.Close();
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
    if(!ds) 
      ds=new EdbDisplay("display-t",-15000.,15000.,-15000.,15000.,-4000.,40000.);

    ds->SetArrSegP( arr );
    ds->SetArrTr( arrtr );
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
    ds=new EdbDisplay("display-t",-15000.,15000.,-15000.,15000.,-4000.,40000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->SetArrV( arrV );
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

double smass = 0.139;

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
  if (!n) return;
  TCanvas *cv1 = new TCanvas("Vertex_reconstruction_1","MC Vertex reconstruction", 760, 760);
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

  int n = hld2.GetEntries();
  if (!n) return;
  TCanvas *cv2 = new TCanvas("Vertex_reconstruction_2","Vertex reconstruction (tracks hists)", 600, 760);
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

  n = hld3.GetEntries();
  if (!n) return;
  TCanvas *cv3 = new TCanvas("Vertex_reconstruction_3","Vertex reconstruction (eff hists)", 600, 760);
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

  n = hld4.GetEntries();
  if (!n) return;
  TCanvas *cv4 = new TCanvas("Vertex_reconstruction_4","Vertex reconstruction (ntracks)", 600, 760);
  if (n < 2)
  {
  }
  else if (n < 3)
  {
    cv4->Divide(1,2);    
  }
  else if (n < 4)
  {
    cv4->Divide(1,3);    
  }
  else if (n < 5)
  {
    cv4->Divide(2,2);    
  }
  else if (n < 6)
  {
    cv4->Divide(2,3);    
  }
  else if (n < 7)
  {
    cv4->Divide(2,3);    
  }
  else if (n < 8)
  {
    cv4->Divide(2,4);    
  }
  else if (n < 9)
  {
    cv4->Divide(2,4);    
  }
  else if (n < 10)
  {
    cv4->Divide(3,3);    
  }
  else if (n < 11)
  {
    cv4->Divide(2,5);    
  }
  else
  {
    cv4->Divide(3,5);    
  }
  for(int i=0; i<n; i++) {
    cv4->cd(i+1);
    if (hld4.At(i)) hld4.At(i)->Draw();
  }
}
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
	    v->Print();
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
		v->Print();
		vc->Print();
		nlv++;
	    }
	}
	nn += v->Nn();
      }
  }
  
  printf("%d linked vertexes found\n", nlv);

  printf("%d neighbooring tracks and segments found\n", nn);

}
