using namespace MATRIX;
using namespace VERTEX;

TTree *init_tracks(const char *file_name, EdbTrackP *track)
{
  char *tree_name="tracks";
  TFile *f = new TFile(file_name);
  if (f)  tracks = (TTree*)f->Get(tree_name);
  if(!tracks) return;
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

void read_tracks(const char *file_name="linked_tracks.root")
{
  EdbTrackP *track1  = new EdbTrackP(8);
  TTree     *tracks1 = init_tracks(file_name, track1);

  EdbTrackP *track2  = new EdbTrackP(8);
  TTree     *tracks2 = init_tracks(file_name, track2);

  float sx=5., sy=5., sz=0., stx=0.006, sty=0.006, sp=0.1;
  float p=1.;

  float x1,y1,z1,tx1,ty1;
  float x2,y2,z2,tx2,ty2;


  //int nentr= tracks1->GetEntries();
  int nentr= 10;
  
  for(int i1=0; i1<nentr; i1++) {
    tracks1->GetEntry(i1);
    track1->SetErrors(sx*sx,sy*sy,sz*sz,stx*stx,sty*sty,sp*sp);

    x1 = track1->X();
    y1 = track1->Y();
    z1 = track1->Z();
    tx1 = track1->TX();
    ty1 = track1->TY();

    VtVector p1( x1,  y1,  z1,  tx1,  ty1, 1.);
    CMatrix  c1(track1->COV());
    t1 = new Track(p1,c1);

    for(int i2=0; i2<nentr; i2++) {
      tracks2->GetEntry(i2);
      track2->SetErrors(sx*sx,sy*sy,sz*sz,stx*stx,sty*sty,sp*sp);


      x2 = track2->X();
      y2 = track2->Y();
      z2 = track2->Z();
      tx2 = track2->TX();
      ty2 = track2->TY();

      VtVector p2( x2,  y2,  z2,  tx2,  ty2, 1.);
      CMatrix  c2(track2->COV());
      t2 = new Track(p2,c2);

      v = new Vertex(*t1, *t2);
      v->findVertexVt();
      if (v->valid())  	{
	cout << "Vertex XYZ is "
             << v->vx()
             << ", "
             << v->vy()
             << ", "
             << v->vz()
             << ", Chi2 is "
             << v->chi2()
             << ", Ntracks is "
             << v->ntracks() << endl;  
      }
      delete t2; t2=0;
      delete v;  v=0;
    }
    delete t1; t1=0;
  }

}
