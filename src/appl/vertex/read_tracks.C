using namespace MATRIX;
using namespace VERTEX;


//----------------------------------------------------------------------------
void read_tracks()
{
  EdbVertex v;
  v.Test();
}

//----------------------------------------------------------------------------
void test_tracks(const char *file_name="linked_tracks.root")
{
  EdbTrackP *track  = new EdbTrackP(8);
  TTree *tracks = init_tracks(file_name, track);

  //test_ms();

  
  int nentr= tracks->GetEntries();
  //int nentr= 10;

  int nseg=1;
  for(int i=0; i<nentr; i++) {
    tracks->GetEntry(i);

    if(track->N()>nseg) {
      printf("track %d  has %d segments\n", i, track->N());
      track->COV().Print();
      nseg++;
    }
  }
  
}



//----------------------------------------------------------------------------
void test_ms(TCut cut="nseg==10")
{
  TCanvas *c1=new TCanvas();
  c1->Divide(2,2);
  c1->cd(1);
  tracks->Draw("(t.eY+t.eTY*(s.eZ-t.eZ))-s.eY:s.eZ-t.eZ",cut);
  c1->cd(2);
  tracks->Draw("(t.eX+t.eTX*(s.eZ-t.eZ))-s.eX:s.eZ-t.eZ",cut);
  c1->cd(3);
  tracks->Draw("(t.eY+t.eTY*(s.eZ-t.eZ))-s.eY:(t.eX+t.eTX*(s.eZ-t.eZ))-s.eX:s.eZ-t.eZ",cut);
  c1->cd(4);
  tracks->Draw("s.eTY:s.eTX:s.eZ","nseg>10&&abs(s.eTY-0.02)<0.04&&abs(s.eTX+0.06)<0.04");
}

//----------------------------------------------------------------------------
void test_fit(const char *file_name="linked_tracks.root")
{
  EdbTrackP *track  = new EdbTrackP(8);
  TTree     *tracks = init_tracks(file_name, track);

  float sx=5., sy=5., sz=0., stx=0.006, sty=0.006, sp=0.1;
  float p=1.;

  //int nentr= tracks1->GetEntries();
  int nentr= 10;
  
  for(int i=0; i<nentr; i++) {
    tracks->GetEntry(i);

    printf("track %d  has %d segments\n", i, track->N());

    for(int j=0; j<track->N(); j++) 
      (track->GetSegment(j))->SetErrors(sx*sx,sy*sy,sz*sz,stx*stx,sty*sty,sp*sp);

    track->FitTrackKF();
    track->Print();
  }

}

//----------------------------------------------------------------------------
TTree *init_tracks(const char *file_name, EdbTrackP *track)
{
  char *tree_name="tracks";
  TFile *f = new TFile(file_name);
  if (f)  tracks = (TTree*)f->Get(tree_name);
  if(!tracks) return 0;
  f->cd();

  int nseg,trid,npl,n0;
  float xv,yv;
  EdbSegP *tr = (EdbSegP*)track;
  TClonesArray *segments=track->S()->GetSegments();

  //tracks->SetBranchAddress("trid",&trid);
  //tracks->SetBranchAddress("nseg",&nseg);
  //tracks->SetBranchAddress("npl",&npl);
  //tracks->SetBranchAddress("n0",&n0);
  //tracks->SetBranchAddress("xv",&xv);
  //tracks->SetBranchAddress("yv",&yv);
  tracks->SetBranchAddress("t",&tr);
  tracks->SetBranchAddress("s",&segments);
  return tracks;
}

