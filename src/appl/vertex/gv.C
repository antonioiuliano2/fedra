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

int eloss_flag = 2; // =0 - no energy losses, 1-average energy losses, 2-Landau energy losses

float momentumK=30.; // momentum of K-meson

float seg_s[4]={1.,1.,0.0015,0.0015}; //sx,sy,stx,sty,sP

float dpp = 0.2;   // average dp/P 

const unsigned long int PrintN = 100;

//float vxyz[3] = {0., 0., 0.};
float vxyz[3] = {0., 0., 13000.};   // simulated vertex position

TObjArray *genVtx;  //vertex array

TH1F *hp[9] = {0,0,0,0,0,0,0,0,0};

EdbVertex *vedb;

double mK=.4937;  // mass of K-meson

//-------------------------------------------------------------------
TGenPhaseSpace *K3Pi(float p=3.)
{
  // K->3Pi decay phase space

  double e=TMath::Sqrt(p*p+mK*mK);
  TLorentzVector K(0,0,p,e);
  Double_t masses[3] = { 0.139, 0.139, 0.139 } ;
  TGenPhaseSpace *psp = new TGenPhaseSpace();
  psp->SetDecay(K, 3, masses);
  return psp;
}

TGenPhaseSpace *pDecay= K3Pi(momentumK);

///------------------------------------------------------------
void gv(int n=1)
{
  // main steering routine for K->3Pi vertex generation&reconstruction test

  timespec_t t0, t;
  double dt;
  BookHistsV();

  ali      = new EdbPVRec();
  EdbPatternsVolume *vol=(EdbPatternsVolume*)ali;

  makeVolumeOPERAn(brick,vol);

  ali->eVTX = new TObjArray();

  TTimeStamp  ts=TTimeStamp();
  t0=ts.GetTimeSpec();
  for(int i=1; i<=n; i++) {
    g1(1);
    if (i)
    if (!(i%PrintN))
    {
	ts=TTimeStamp();
	t=ts.GetTimeSpec();
	dt=(t.tv_sec-t0.tv_sec)*1000000000.+(t.tv_nsec-t0.tv_nsec);
	printf("%d events generated in %.4f sec (%.1f msec per event)\n",
	i,dt/1000000000.,dt/(double)i/1000000.);
    }
  }

  DrawHistsV();
  //delete ali;
  //ali = 0;
}


///------------------------------------------------------------
void g1(int nv=1)
{
  //generation of 1 event

  gen_v(brick, nv);

  for(int i=0; i<nv; i++) {
    vedb = (EdbVertex *)(ali->eVTX->At(i));
//    vedb->Print();
    rec_v(*vedb);
//    disp_v();
    //draw_v(*vedb,1.);
  }

  ali->eVTX->Clear();
  ali->eTracks->Clear();
  ali->Clear();

  delete vedb;
  vedb = 0;
}

///------------------------------------------------------------
EdbDisplay *ds=0;

void disp_v()
{
  // display of 1 event

  if(!ali) return;
  ali->FillCell(50,50,0.015,0.015);

  TObjArray *arr   = new TObjArray();
  TObjArray *arrtr = new TObjArray();
  float binx=10, bint=10;

  EdbTrackP *track;
  for(int i=0; i<ali->eTracks->GetEntries(); i++) {
    track = (EdbTrackP*)(ali->eTracks->At(i));
    arrtr->Add(track);
    ali->ExtractDataVolumeSeg( *track,*arr,binx,bint );
  }


  gStyle->SetPalette(1);
  if(!ds)
    ds=new EdbDisplay("display-t",-14000.,14000.,-14000.,14000.,0.,80000.);
  
  ds->SetArrSegP( arr );
  ds->SetArrTr( arrtr );
  ds->Draw();
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
  float x=vedb.ePoint.X();
  float y=vedb.ePoint.Y();
  float z=vedb.ePoint.Z();

  Double_t    weight = pDecay->Generate();

  int nt = pDecay->GetNt();
  for(int ip=0; ip<nt; ip++) {
    TLorentzVector *pi = pDecay->GetDecay(ip);

    tr[ip] = new EdbTrackP();
    double tx = pi->Px()/pi->Pz();
    double ty = pi->Py()/pi->Pz();
    tr[ip]->Set(ip, x, y, (float)tx, (float)ty,1,0);
    tr[ip]->SetZ(z);
    tr[ip]->SetP(pi->P());
    tr[ip]->SetM(pi->M());
    vedb.AddTrack(tr[ip]);
    ali->AddTrack(tr[ip]);
  }


  tr[nt] = new EdbTrackP();
  tr[nt]->Set(ip, x, y, 0., 0.,1,0);
  tr[nt]->SetZ(0);
  tr[nt]->SetP(momentumK);
  tr[nt]->SetM(mK);
  ali->AddTrack(tr[nt]);

  // generation of basetrack for primary K-meson

  float zlim[2];
  zlim[0] =-1000.;
  zlim[1] =12000.;
  EdbVertexRec::TrackMC( *((EdbPatternsVolume*)ali),zlim, brick.lim, seg_s, *(tr[nt]), eloss_flag );
  tr[nt]->FitTrackKFS(false);
  
}

//-------------------------------------------------------------------
void gen_segs_v(BRICK &b, EdbVertex &vedb)
{
  // generation of basetracks for secondary tracks

  float zlim[2];
  zlim[0] =-1000.;  
  zlim[1] =130000.;
  EdbTrackP *tr;

  int nt = vedb.N();
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    tr->SetFlag(1000+ip);

    EdbVertexRec::TrackMC( *((EdbPatternsVolume*)ali), zlim, b.lim, seg_s, *tr, eloss_flag);
  }


}

///------------------------------------------------------------
void rec_v(EdbVertex &vedb)
{
  // tracks and vertex reconstruction (KF fitting only without track finding)

  int nsegMin=5;
  float p;
  EdbTrackP *tr;

  Vertex *v = new Vertex();
  Track *t[10]={0,0,0,0,0,0,0,0,0,0};
  int nt = vedb.N();
  //printf("\n nt = %d\n\n",nt);
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    //    tr->Print();
    if (tr->N() < nsegMin) break;
    if (tr->P() < 0.1) break;

    p = tr->P();
    tr->SetErrorP(p*p*dpp*dpp);

    p = p*(1.+dpp*gRandom->Gaus());
    if (p < 0.01) p = 0.01;

    tr->SetP(p);
    tr->FitTrackKFS(false);

    t[ip] = new Track();
    if (!EdbVertexRec::Edb2Vt( *tr, *t[ip] )) break;
    t[ip]->rm(tr->M());            //?
    v->push_back(*t[ip]);           //?
  }

  if (v->ntracks() == 3)       // fit vertex
  {
    v->use_kalman(true);
    
    v->use_momentum(true);

    if (v->findVertexVt()) {
	
      for(int ip=0; ip<nt; ip++) {
	tr = vedb.GetTrack(ip);
	//	tr->SetX( (float)(v->vx()) );
	//tr->SetY( (float)(v->vy()) );
	//tr->SetZ( (float)(v->vz()) );
      }

	FillHistsV(*v);
    }
  }



  for(int ip=0; ip<nt; ip++) if (t[ip]) {delete t[ip]; t[ip]=0;}
  delete v;
  v =0;
}

///------------------------------------------------------------
void draw_v(EdbVertex &vedb, float zoom=1.)
{
  // some plots

  TH1F *hseg[4];
  hseg[0] =  new TH1F("Hist_seg_dx" ,"dx" ,100,-5.,5.);
  hseg[1] =  new TH1F("Hist_seg_dy" ,"dy" ,100,-5.,5.);
  hseg[2] =  new TH1F("Hist_seg_dtx","dtx",100,-5.,5.);
  hseg[3] =  new TH1F("Hist_seg_dty","dty",100,-5.,5.);

  TCanvas *cv = new TCanvas();
  float xmin=brick.lim[0]/zoom;
  float ymin=brick.lim[1]/zoom;
  float xmax=brick.lim[2]/zoom;
  float ymax=brick.lim[3]/zoom;
  float zmin=(brick.z0-brick.dz)/zoom;
  float zmax=(brick.z0+brick.dz*brick.plates)/zoom;
  TH3F *hist = new TH3F("hist","title",100,xmin,xmax,100,ymin,ymax,100,zmin,zmax);
  hist->Draw();
  TPolyLine3D *line=0;

  EdbSegP *seg=0;
  EdbSegP *segf=0;
  int nt = vedb->N();
  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);

    tr->SetDZ(3000);
    line = seg_line( (EdbSegP*)tr );
    line->SetLineColor(kGreen);
    line->SetLineWidth(2);
    line->Draw();


    int nseg=tr->N();
    for(int is=0; is<nseg; is++) {
      seg = tr->GetSegment(is);
      seg->SetDZ(300);
      line = seg_line( seg );
      line->SetLineColor(kRed);
      line->SetLineWidth(2);
      line->Draw();

      segf = tr->GetSegmentF(is);
      segf->SetDZ(1300.);
      line = seg_line( segf );
      line->SetLineColor(kBlue);
      line->SetLineWidth(1);
      line->Draw();

      hseg[0]->Fill(seg->X()-segf->X());
      hseg[1]->Fill(seg->Y()-segf->Y());
      hseg[2]->Fill(1000.*(seg->TX()-segf->TX()));
      hseg[3]->Fill(1000.*(seg->TY()-segf->TY()));
    }


  }

  TCanvas *ch = new TCanvas();
  ch->Divide(2,2);
  for(int i=0; i<4; i++) {
    ch->cd(i+1);
    hseg[i]->Draw();
  }


  float pms=0;

  TGraphErrors *gr=0;
  TCanvas *ctr = new TCanvas();
  ctr->Divide(2,2);

  for(int ip=0; ip<nt; ip++) {
    tr = vedb.GetTrack(ip);
    ctr->cd(ip+1);
    pms=EdbVertexRec::P_MS(*tr,brick.X0,true);
    printf("p = %f \t pms = %f \t m = %f\n", tr->P(), pms, tr->M() );
  }


}

///------------------------------------------------------------
TPolyLine3D *seg_line(EdbSegP *seg)
{
  TPolyLine3D *line = new TPolyLine3D(2);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  float dz = seg->DZ();
  line->SetPoint(1,
		 seg->X() + seg->TX()*dz,
		 seg->Y() + seg->TY()*dz,
		 seg->Z()+            dz );

  return line;
}

///------------------------------------------------------------
TPolyMarker3D *seg_mark(EdbSegP *seg)
{
  TPolyMarker3D *line = new TPolyMarker3D(2,1);
  line->SetPoint(0, seg->X(), seg->Y(), seg->Z() );
  float dz = seg->DZ();
  line->SetPoint(1,
		 seg->X() + seg->TX()*dz,
		 seg->Y() + seg->TY()*dz,
		 seg->Z()+            dz );

  return line;
}

///-----------------------------------------------------------------------
void BookHistsV()
{
  if (!hp[0]) hp[0] = new TH1F("Hist_Vt_Dx","Vertex X error",100,-5.,5.);
  if (!hp[1]) hp[1] = new TH1F("Hist_Vt_Dy","Vertex Y error",100,-5.,5.);
  if (!hp[2]) hp[2] = new TH1F("Hist_Vt_Dz","Vertex Z error",50,-500.,500.);
  if (!hp[3]) hp[3] = new TH1F("Hist_Vt_Sx","Vertex X sigma",100,0.,1.);
  if (!hp[4]) hp[4] = new TH1F("Hist_Vt_Sy","Vertex Y sigma",100,0.,1.);
  if (!hp[5]) hp[5] = new TH1F("Hist_Vt_Sz","Vertex Z sigma",100,0.,200.);
  if (!hp[6]) hp[6] = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",25,0.,5.);
  if (!hp[7]) hp[7] = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",25,0.,1.);
  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Mass","Vertex Mass",50,-0.200,+0.200);
//  if (!hp[8]) hp[8] = new TH1F("Hist_Vt_Ntra","Vertex Ntracks",20,0.,20.);
}

double smass = 0.139;

///-----------------------------------------------------------------------
void FillHistsV(Vertex &v)
{
  hp[0]->Fill(v.vx() - (double)vxyz[0]);
  hp[1]->Fill(v.vy() - (double)vxyz[1]);
  hp[2]->Fill(v.vz() - (double)vxyz[2]);
  hp[3]->Fill(v.vxerr());
  hp[4]->Fill(v.vyerr());
  hp[5]->Fill(v.vzerr());
  hp[6]->Fill(v.ndf() > 0 ? v.chi2()/v.ndf() : 0);
  hp[7]->Fill(v.prob());
  double rmass = v.mass(smass);
  hp[8]->Fill(rmass-mK);
//  hp[8]->Fill(v.ntracks());
}

///-----------------------------------------------------------------------
void DrawHistsV()
{
  TCanvas *cv = new TCanvas();
  cv->Divide(3,3);
  for(int i=0; i<9; i++) {
    cv->cd(i+1);
    if (hp[i]) hp[i]->Draw();
  }
}
void ClearHistsV()
{
  for(int i=0; i<9; i++) {
    if (hp[i]) hp[i]->Clear();
  }
}
