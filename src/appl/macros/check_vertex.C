//----------------------------------------------------------------------------
//
//  Usage: $ root -l 
//         root[1] .L check_vertex.C
//         root[2] trseg(16)  // process the event 16 (simulation of cause!)
//
//  Usage: $ root -l 
//         root[1] .L check_vertex.C
//         root[2] trvol()  // do all vertex reconstruction starting from the linked_tracks.root
//
//----------------------------------------------------------------------------

EdbDataProc  *dproc=0;
EdbPVRec     *gAli=0;
EdbVertexRec *gEVR=0;
EdbDisplay   *ds=0;

namespace TRACKING_PAR
{
  float  momentum = 4.;     // GeV
  float  mass     = 0.139;  // particle mass
  float  ProbMinP = 0.05;   // minimal probability to accept segment on propagation
  int    nsegmin  = 2;      // minimal number of segments to propagate this track
  int    ngapmax  = 3;      // maximal gap for propagation
}
namespace VERTEX_PAR
{
  float DZmax      = 3000.;  // maximum z-gap in the track-vertex group
  float ProbMinV   = 0.001;  // minimum acceptable probability for chi2-distance between tracks
  float ImpMax     = 40.;    // maximal acceptable impact parameter [microns] (for preliminary check)
  bool  UseMom     = false;  // use or not track momentum for vertex calculations
  bool  UseSegPar  = false;  // use only the nearest measured segments for vertex fit (as Neuchatel)
  int   QualityMode= 0;      // vertex quality estimation method (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average track-vertex distance)
}

//---------------------------------------------------------------------
void check_vertex()
{
  trseg(16);    // reconstruct event starting from basetracks
  //trvol();    // reconstruct vertexes starting from linked_tracks.root
}

//---------------------------------------------------------------------
void trseg( int event=40, const char *def="pions_data_set.def" )
{
  // this function read basetracks for a given simulated event and do the 
  // tracking and vertex reconstruction

  FILE *f = fopen("default.par","w");
  fprintf(f,"INCLUDE opera_emulsion.par\n");
  fprintf(f,"RCUT 0 abs(s.eTX)<1.\&\&abs(s.eTY)<1.\&\&s.eMCEvt==%d\n",event);
  fclose(f);

  init(def, 0);                                //read all basetracks (option 0)
  do_tracking();
  gAli->FillCell(30,30,0.009,0.009);
  do_vertex();
  sd();                   // draw all segments
  vd();                   // draw reconstructed vertex
}

//---------------------------------------------------------------------
void trvol( const char *def="pions_data_set.def", const char *rcut = "nseg>1" )
{
  // this function read volume tracks and do the vertex reconstruction

  init(def, 100 ,rcut);                      // read tracks (option 100)
  gAli->FillCell(30,30,0.009,0.009);
  do_propagation();
  do_vertex();
  vd(3);   // draw reconstructed vertex with >=3 tracks

  //dproc->MakeTracksTree(gAli,"linked_tracks_p.root");
}

//---------------------------------------------------------------------
void init( const char *def, int iopt,  const char *rcut="1" )
{
  dproc = new EdbDataProc(def);
  dproc->InitVolume(iopt, rcut);
  gAli = dproc->PVR();
}

//---------------------------------------------------------------------
void do_tracking()
{
  using namespace TRACKING_PAR;
  gAli->SetCouplesPeriodic(0,1);

  int ntr = dproc->LinkTracksWithFlag( gAli, momentum, ProbMinP, nsegmin, ngapmax, 0 );

  gAli->FitTracks( momentum, mass );
}

//---------------------------------------------------------------------
void do_propagation()
{
  // example of additional propagation and other tracking operations if necessary

  EdbTrackP *tr=0;
  int ntr = gAli->eTracks->GetEntries();
  printf("ntr = %d\n",ntr);

  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    tr->SetID(i);
    tr->SetSegmentsTrack();
  }

  int nadd = 0;
  int nseg=0;
  
  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    //tr = tr->SetErrorP(0.2*0.2*4.*4.);
    nseg = tr->N();
    tr->SetP(tr->P_MS());
    if(tr->Flag()<0) continue;
    nadd += gAli->PropagateTrack( *tr, true, 0.05, 3, 0 );
    if(tr->Flag()<0) printf("%d flag=%d\n",i,tr->Flag());
    if(tr->N() != nseg) printf("%d nseg=%d (%d) \t p = %f\n",i,tr->N(),nseg,tr->P());
  }
  printf("nadd = %d\n",nadd);
}

//---------------------------------------------------------------------
void do_vertex()
{
  using namespace VERTEX_PAR;

  //gAli->FitTracks(4.,0.139 );
  gEVR = new EdbVertexRec();
  gEVR->eEdbTracks = gAli->eTracks;
  gEVR->eVTX       = gAli->eVTX;

  gEVR->eDZmax=DZmax;
  gEVR->eProbMin=ProbMinV;
  gEVR->eImpMax=ImpMax;
  gEVR->eUseMom=UseMom;
  gEVR->eUseSegPar=UseSegPar;
  gEVR->eQualityMode=QualityMode;

  printf("%d tracks vor vertexing\n",  gEVR->eEdbTracks->GetEntries() );
  int nvtx = gEVR->FindVertex();
  printf("%d 2-track vertexes was found\n",nvtx);

  if(nvtx == 0) return;
  int nadd =  gEVR->ProbVertexN();
}

//---------------------------------------------------------------------
void td()
{
  // draw all tracks

  TObjArray *trarr=gAli->eTracks;
  gStyle->SetPalette(1);
  const char *dsname="display-t";
  ds = EdbDisplay::EdbDisplayExist(dsname);
  if(!ds)  ds=new EdbDisplay(dsname,-50000.,50000.,-50000.,50000.,-4000.,80000.);
  
  ds->SetDrawTracks(4);
  ds->SetArrTr( trarr );
  printf("%d tracks to display\n", trarr->GetEntries() );
  ds->Draw();
}

//---------------------------------------------------------------------
void sd()
{
  // draw all tracks and segments (basetracks)

  TObjArray *trarr = gAli->eTracks;
  TObjArray *sarr  = new TObjArray();

  EdbSegP *s=0;
  for(int i=0; i<gAli->Npatterns(); i++) {
    EdbPattern *pat = gAli->GetPattern(i);
    for(int j=0; j<pat->N(); j++) {
      s = pat->GetSegment(j);
      if(s->Track()<0)                  // exclude segments already attached to tracks
	sarr->Add(s);
    }
  }

  printf("%d tracks to display\n",   trarr->GetEntries() );
  printf("%d segments to display\n", sarr->GetEntries()  );

  gStyle->SetPalette(1);
  const char *dsname="display-s";
  ds = EdbDisplay::EdbDisplayExist(dsname);
  if(!ds)  ds=new EdbDisplay(dsname,-50000.,50000.,-50000.,50000.,-4000.,80000.);
  
  ds->SetDrawTracks(4);
  ds->SetArrSegP( sarr );
  ds->SetArrTr( trarr );
  ds->Draw();
}

//---------------------------------------------------------------------
void vd( int trmin=2, float amin=.0)
{
  // draw vertexes with multiplicity>=trmin, and aperture >= amin

  TObjArray *varr = new TObjArray();
  TObjArray *tarr = new TObjArray();

  EdbVertex *v=0;
  EdbTrackP *t=0;

  int nv = gEVR->Nvtx();
  printf("nv=%d\n",nv);
  if(nv<1) return;

  for(int i=0; i<nv; i++) {
    v = (EdbVertex *)(gEVR->eVTX->At(i));
    if(v->Flag()<0)         continue;
    if( v->N()<trmin )                       continue;
    if( v->N()<3 && v->MaxAperture()<amin )  continue;

    varr->Add(v);
    for(int j=0; j<v->N(); j++) tarr->Add( v->GetTrack(j) );
  }

  gStyle->SetPalette(1);

  const char *dsname="display-v";
  ds = EdbDisplay::EdbDisplayExist(dsname);
  if(!ds)  ds=new EdbDisplay(dsname,-50000.,50000.,-50000.,50000.,-4000., 80000.);
  
  ds->SetArrTr( tarr );
  printf("%d tracks to display\n", tarr->GetEntries() );
  ds->SetArrV( varr );
  printf("%d vertex to display\n", varr->GetEntries() );
  ds->SetDrawTracks(4);
  ds->SetDrawVertex(1);
  ds->Draw();
}
