#include "EdbLog.h"
#include "EdbDataStore.h"

#include <TSystem.h>
#include <TRandom3.h>
#include <TF1.h>
#include "EdbPVGen.h"

ClassImp(EdbDataStore);
///------------------------------------------------
EdbDataStore::EdbDataStore(){
  eRawPV.Set0();
  eSegPV.Set0();
};
///------------------------------------------------
EdbDataStore::~EdbDataStore(){ Clear();};
///------------------------------------------------
void EdbDataStore::LoadMCVertices(TObjArray* vtx){
  Log(2,"EdbDataStore::LoadMCVertices",Form("Reading %d vertices",vtx->GetEntries()));
  for(int nv=0; nv<vtx->GetEntries();++nv){
    AddVertex((EdbVertex*)vtx->At(nv));
  }
}
///------------------------------------------------
void EdbDataStore::TransferGeometry(EdbDataStore* ds){
  Log(2,"transfer geometry","");
  ds->eBrick->Clear();
  ds->eBrick->Copy(*eBrick);
}
///------------------------------------------------
void EdbDataStore::TransferTo(EdbDataStore* ds, char level,EdbSegmentCut* cut,int FromPlate, int ToPlate){
  assert(level>=0 && level<16);
  
  if(level&0x1){
    Log(2,"EdbDataStore::TransferTo","transfer MTK segments\n");
    TransferSegs(&eRawPV,&(ds->eRawPV),cut,FromPlate,ToPlate);
    Log(2,"EdbDataStore::TransferTo","done");
  }
  
  if(level&0x2){
    Log(2,"EdbDataStore::TransferTo","transfer BTK segments\n");
    TransferSegs(&eSegPV,&(ds->eSegPV),cut,FromPlate,ToPlate);
    Log(2,"EdbDataStore::TransferTo","done");
  }
  
  if(level&0x4){
    Log(2,"EdbDataStore::TransferTo","transfer Tracks\n");
    for(int nt=0;nt<eTracks.GetEntries();++nt){
      if(GetTrack(nt)->N())ds->AddTrack(GetTrack(nt));
    }
    ds->SetOwnTracks(0);
    Log(2,"EdbDataStore::TransferTo","done");
  }
  if(level&0x8){
    Log(2,"EdbDataStore::TransferTo","transfer Vertices\n");
    for(int nv=0;nv<eVTX.GetEntries();++nv)ds->AddVertex(GetVertex(nv));
    ds->SetOwnVertices(0);
    Log(2,"EdbDataStore::TransferTo","done");
  }
}
///------------------------------------------------
void EdbDataStore::TransferSegs(EdbPatternsVolume* pv0, EdbPatternsVolume* pv1,EdbSegmentCut* cut,int FromPlate,int ToPlate){
  Log(1,"EdbDataStore::TransferSegs()","Transfer segments");
  assert(pv0);
  assert(pv1);
  EdbPattern* pat0=0;
  EdbPattern* pat1=0;
  EdbSegP* seg=0;
  int npat=pv0->Npatterns();

  if(pv1->Npatterns()==0)pv0->PassProperties(*pv1);

  int nseg=0;
  float p[5];
  for(int np=0;np<npat;++np){

    pat0=pv0->GetPattern(np);
    if(pat0->Plate()<FromPlate)continue;
    if(pat0->Plate()>ToPlate)continue;
    pat1=pv1->GetPattern(np); ///find corresponding DEST pattern 
    assert(pat1);
    assert(pat1->Z()==pat0->Z());

    nseg=pat0->N();
    for(int ns=0;ns<nseg;++ns){
      seg=pat0->GetSegment(ns);
      p[0]=seg->X();
      p[1]=seg->Y();
      p[2]=seg->TX();
      p[3]=seg->TY();
      p[4]=seg->W();
      if(cut && !cut->PassCut(p))continue;
      seg=pat1->AddSegment(*seg);
      seg->SetTrack(0);
    };
//     printf("dest.pat#%d has #%d segments\n",pat1->ID(),pat1->N());
    pat1->SetID(np);
    pat1->SetSegmentsPID();
    
  }
  
}

///------------------------------------------------
void EdbDataStore::Restore_PIDFromID(){
  ///restore patterns' "plate" and "side" from their ID
  EdbPattern* p;
  int plate, side;
  for(int np=0;np<eSegPV.Npatterns();np++){
    p=eSegPV.GetPattern(np);
    plate=p->ID();
    p->SetPID(p->ID());
    p->SetScanID(EdbID(0,plate,0,0));
    p->SetSide(0);
  }
  for(int np=0;np<eRawPV.Npatterns();np++){
    p=eRawPV.GetPattern(np);
    plate=p->ID()/2;
    p->SetPID(p->ID());
    side=1+p->ID()%2;
    p->SetScanID(EdbID(0,plate,0,0));
    p->SetSide(side);
  }
};
///------------------------------------------------
void EdbDataStore::Restore_PatFromGeom(int np0, int np1){
  ///fill patterns array from eBrick plates
  assert(eBrick);
  EdbPlateP* plt=0;
  for(int np=0;np<eBrick->Npl();++np){
    if(np<np0 || np>np1)continue;
    plt=eBrick->GetPlate(np);
    assert(plt);
    for(int side=0;side<3;++side)
      MakePattern(plt->Z()+plt->GetLayer(side)->Z(),np,side);
  }
}
///------------------------------------------------
void EdbDataStore::Restore_TrxFromVtx(){
  ///restore tracks from vertex
  ClearTracks();
  EdbVertex* v;
  for(int nv=0;nv<eVTX.GetEntries();nv++){
    v=GetVertex(nv);
    for(int nt=0;nt<v->N();nt++){
      EdbTrackP* trk=v->GetTrack(nt);
      trk->AddVTA(v->GetVTa(nt));
      if(v->GetVTa(nt)->Zpos()==1)AddTrack(trk);
    }
  }
}
///------------------------------------------------
void EdbDataStore::Restore_SegFromTrx(EdbSegmentCut* cut,int Plt0, int Plt1){
  ///restore segments from tracks
  EdbTrackP* t=0;
  EdbSegP* s=0;
  EdbPattern* p=0;
  float par[5];
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    t=GetTrack(nt);
    if(t->N()==0)continue;
    for(int ns=0;ns<t->N();++ns){
      s=t->GetSegment(ns);
      s->SetMC(t->MCEvt(),t->MCTrack());
//       s->SetFlag(1);
      if(s->Plate()<Plt0 || s->Plate()>Plt1)continue;
      if(cut){
	par[0]=s->X();
	par[1]=s->Y();
	par[2]=s->TX();
	par[3]=s->TY();
	par[4]=s->W();
	if(!cut->PassCut(par))continue;
      }
      if(gEDBDEBUGLEVEL>5){
        s->PrintNice();
        printf("plt#%d side#%d\n",s->Plate(),s->Side());
        }
      p=FindPattern(s->Plate(),s->Side());
      assert(p);
      p->AddSegment(*s);
      p->SetID(s->Plate());
      s->SetPID(p->ID());
    }
  }
}
///------------------------------------------------
EdbTrackP* EdbDataStore::FindTrack(int id){
  EdbTrackP* trk=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
//     printf("id%d/%d=%d\n",nt,eTracks.GetEntries(),trk->ID());
    if(trk->ID()==id)return trk;
  }
  return 0;
}
///------------------------------------------------
EdbVertex* EdbDataStore::FindVertex(int id){
  EdbVertex* vtx=0;
  for(int nv=0;nv<eVTX.GetEntries();++nv){
    vtx=GetVertex(nv);
    if(vtx->ID()==id)return vtx;
  }
  return 0;
}
///------------------------------------------------
EdbLayer*  EdbDataStore::FindLayer(int plate, int side){
  assert(plate>=0);
  assert(side>=0 && side<3);
  
  EdbPlateP* plt=0;
  for(int np=0;np<eBrick->Npl();++np){
    plt=eBrick->GetPlate(np);
    if(plt->ID()==plate){
      return plt->GetLayer(side);
    }
  }
  return 0;
}
///------------------------------------------------
EdbPattern* EdbDataStore::FindPattern(int plate, int side){
  Log(3,"EdbDataStore::FindPattern",Form("search plt#%d side=%d",plate,side));
  assert(plate>=0);
  assert(side>=0 && side<3);
  
  EdbPatternsVolume* pv=0;
  pv=(side==0)?&eSegPV:&eRawPV;
  EdbPattern* pat=0;
  for(int np=0;np<pv->Npatterns();++np){
    pat=pv->GetPattern(np);
    //Log(6,"pat",Form("#%d/%d plt#%d side#%d",np,pv->Npatterns(),pat->Plate(),pat->Side()));
    if(pat->Plate()==plate && pat->Side()==side)return pat;
  }
  return 0;
}
///------------------------------------------------
EdbTrackP* EdbDataStore::FindLongTrk(int nsmin){
  ///find longest track
  int nsmax=0;
  int imax=0;
  EdbTrackP* trk=0;
  for(int nt=0;nt<Nt();++nt){
    trk=GetTrack(nt);
    if(trk->PDG()!=22 && trk->P()>1.&& trk->N()>nsmax){
      imax=nt;
      nsmax=trk->N();
    }
  }
  if(nsmax<nsmin)return 0;
  return GetTrack(imax);
}
///------------------------------------------------
EdbVertex* EdbDataStore::FindPrimVtx(){
  EdbTrackP* trk=FindLongTrk(8);
  return trk->VertexS();
}
///------------------------------------------------
void EdbDataStore::AddPattern(EdbPattern* pat){
   assert(pat);
   assert(FindLayer(pat->Plate(),pat->Side()));
   if(pat->Side()==0)eSegPV.AddPattern(pat);
   else eRawPV.AddPattern(pat);
}
///------------------------------------------------
void EdbDataStore::MakePattern(double z,int plate,int side){
  Log(3,"EdbDataStore::MakePattern()",Form("plt#%d (side %d) z=%2.1f",plate,side,z));
  EdbPattern* p=new EdbPattern(0,0,z,0);
  p->SetID(plate);
  p->SetScanID(EdbID(0,plate,0,0));
  p->SetSide(side);
  p->SetPID(side?(plate*10+side):plate);
  if(side==0)eSegPV.AddPattern(p);
  else eRawPV.AddPattern(p);
   
}
///------------------------------------------------
void EdbDataStore::ClearGeom(){
  if(eBrick==0)return;
  for(int np=0;np<eBrick->Npl();++np){
    eBrick->GetPlate(np)->Clear();
    delete eBrick->GetPlate(np);
  }
  eBrick->Clear();
}
///------------------------------------------------
void EdbDataStore::ClearVTX(){
  eVTX.Clear();
}
///------------------------------------------------
void EdbDataStore::ClearTracks(bool hard){
  eTracks.Clear();
}
///------------------------------------------------
void EdbDataStore::ClearSeg(bool hard){
  EdbPatternsVolume* pv=&eSegPV;
//   int N=pv->Npatterns();
//   if(!N)return;
//   TObjArray* Sgs=0;
//   for(int k=0;k<N;k++) {
//     Sgs=pv->GetPattern(k)->GetSegments();
//     if(Sgs)Sgs->Delete();
//   }
  int N=pv->Npatterns();
  if(!N)return;
  TObjArray* Sgs=0;
  for(int k=0;k<N;k++) {
    Sgs=pv->GetPattern(k)->GetSegments();
    if(Sgs)Sgs->Delete();
//     delete pv->GetPattern(k);
  }
  if(hard){
    pv->ePatterns->SetOwner(1);
    pv->DropCell();
    pv->DropCouples();
    pv->ePatterns->Clear();
    pv->Clear();
  }
}
///------------------------------------------------
void EdbDataStore::ClearRaw(bool hard){
  EdbPatternsVolume* pv=&eRawPV;

  
  int N=pv->Npatterns();
  if(!N)return;
  TObjArray* Sgs=0;
  for(int k=0;k<N;k++) {
    Sgs=pv->GetPattern(k)->GetSegments();
    if(Sgs)Sgs->Delete();
//     delete pv->GetPattern(k);
  }
  if(hard){
    pv->ePatterns->SetOwner(1);
    pv->ePatterns->Clear();
    pv->Clear();
    pv->DropCell();
    pv->DropCouples();
  }
}
///------------------------------------------------
void EdbDataStore::PrintBrief(){
  printf("EdbDataStore. We have here:\n");
  printf(" --%d vertcies\n --%d tracks\n --%d Plates (geometry)\n",eVTX.GetEntries(),eTracks.GetEntries(),(eBrick)?eBrick->Npl():0);
  printf(" --%d BT Patterns\n --%d MT Patterns\n",eSegPV.Npatterns(),eRawPV.Npatterns());
}
///------------------------------------------------
void EdbDataStore::PrintTracks(int vlev){
  printf("EdbDataStore. We have %d tracks\n",eTracks.GetEntries());
  if(vlev<1)return;
  EdbTrackP* trk=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
    printf(" (trk #%3d) PDG=% 4d P=% 4.2f [X,Y,Z]=[% 7.2f % 7.2f % 7.2f]\t[TX,TY]=[% 2.2f % 2.2f] Nseg=%d\n",
    trk->ID(),trk->PDG(),trk->P(),trk->X(),trk->Y(),trk->Z(),trk->TX(),trk->TY(),trk->N());
    if(vlev<2)continue;
    for(int ns=0;ns<trk->N();++ns){
      EdbSegP* s=trk->GetSegment(ns);
      printf("    seg#%d  Plate=%d pid=%d flag=%d\n",s->ID(),s->Plate(),s->PID(),s->Flag());
    }
  }
}
///--------------------------------------------------
void EdbDataStore::PrintPatterns(){
  printf("EdbDataStore. We have %d BT patterns:\n",eSegPV.Npatterns());
  EdbPattern* pat=0;
  for(int np=0;np<eSegPV.Npatterns();++np){
    pat=GetSegPat(np);
    printf(" (pat #%3d) [X,Y,Z]=[% 7.2f % 7.2f % 7.2f]\t Nseg=%d\n",
	   pat->Plate(),pat->X(),pat->Y(),pat->Z(),pat->N());
  }
}
///--------------------------------------------------
void EdbDataStore::DoSmearing(EdbScanCond* cond_btk,EdbScanCond* cond_mtk){
  EdbPVGen pvg;
  if(cond_btk){
    pvg.SetVolume(&eSegPV);
    pvg.SetScanCond(cond_btk);
    pvg.SmearSegments();
  }
  if(cond_mtk){
    pvg.SetVolume(&eRawPV);
    pvg.SetScanCond(cond_mtk);
    pvg.SmearSegments();
  }
}
///--------------------------------------------------
void EdbDataStore::DoEfficiency(TF1* eff_seg,TF1* eff_mtk){
  EdbSegP* s=0;
  TRandom3 Rando;
  double eff, ran, th;
  if(eff_seg){
    for(int np=0;np<eSegPV.Npatterns();++np){
      EdbPattern* pat=GetSegPat(np);
      for(int ns=0; ns< pat->N();++ns){
        s=pat->GetSegment(ns);
        th=s->Theta();
        eff=(th<=eff_seg->GetXmax())?eff_seg->Eval(th):0;
        ran=Rando.Rndm();
        if(ran>eff)s->SetW(-s->W());
     } 
    }
  }  
  if(eff_mtk){
    for(int np=0;np<eRawPV.Npatterns();++np){
      int n1=0;
      EdbPattern* pat=GetRawPat(np);
      for(int ns=0; ns< pat->N();++ns){
        s=pat->GetSegment(ns);
        th=s->Theta();
        Log(5,"DoEfficiency",Form("mtk#%d/%d theta=%2.4f [%2.4f %2.4f]\n",ns,pat->N(),th,s->TX(),s->TY()));
        eff=(th<=eff_mtk->GetXmax())?eff_mtk->Eval(th):0;
        ran=gRandom->Rndm();
        Log(5,"DoEfficiency",Form("eff=%2.4f, ran=%2.4f => %s\n",eff,ran,eff>ran?"survive":"KILL"));
        if(ran>eff)s->SetW(-s->W());
        else ++n1;
     } 
     Log(2,"DoEfficiency",Form("Plate #%d side#%d - efficiency %d/%d\n",pat->Plate(),pat->Side(),n1,pat->N()));
    }
  }
}
///==============================================================================
///==============================================================================

ClassImp(EdbDSRec);
///------------------------------------------------
EdbDSRec::EdbDSRec(){
  ///init Vertexing:
  eVRec.eEdbTracks=&eTracks;
  eVRec.eDZmax=5000;
  eVRec.eProbMin=0.001;
  eVRec.eImpMax=50;
  eVRec.eUseMom=0;
  eVRec.eUseSegPar=1;
  eVRec.eQualityMode=0;
  ///init momentum estimator:
  eMomEst.SetParPMS_Mag();
  eMomEst.eAlg = 0;
  eMomEst.eMinEntr = 3;
}

///------------------------------------------------
void EdbDSRec::Clear(bool hard){
  EdbDataStore::Clear(hard);
  eVRec.Reset();
}
///------------------------------------------------

int EdbDSRec::DoTracking(bool use_btk, int p0, int p1){
  ///init tracker:
  EdbTrackAssembler tracker;
  tracker.eCond=use_btk?eCond_b:eCond_m;
//   .SetSigma0(3,3,0.005,0.005);
  tracker.eDTmax=0.07;
  tracker.eDRmax=45.;
//   tracker.eDZGapMax=10000;
  tracker.InitTrZMap(  2400, eBrick->Xmin(), eBrick->Xmax(),   2000, eBrick->Ymin(), eBrick->Ymax(),   30 );
  ///doing tracking using microtracks or basetracks
  int nsegmin=use_btk?2:3;
  SetOwnTracks(1);
  EdbPatternsVolume* pv=use_btk?(&eSegPV):(&eRawPV);
  int npl=pv->Npatterns();
  EdbPattern *pat=0;
  EdbLayer *plate=0;
  for(int ipass=0; ipass<2; ipass++) {
    printf("\n\n*************** ipass=%d ************\n",ipass);
    tracker.eCollisionsRate=0;
    for(int i=0; i<npl; i++) {
      if(i<p0 || i>p1)continue;
      pat=pv->GetPattern(i);
      if(pat->N()==0)continue;
      Log(3,Form("EdbDSRec::DoTracking(%d,%d,%d)",use_btk,p0,p1),Form("plate=%d at Z=%4.1f side=%d [%d segm]\n",pat->Plate(),pat->Z(),pat->Side(),pat->N()));
      plate = eBrick->GetPlate(pat->Plate());
//       printf("pattern z = (%2.4f vs %2.4f)\n", pat->Z(),plate->Z());
      pat->SetSegmentsZ();
      pat->SetSegmentsPID();
      pat->Transform(    plate->GetAffineXY()   );
      pat->TransformShr( plate->Shr() );
      pat->TransformA(   plate->GetAffineTXTY() );
      pat->SetSegmentsPlate(pat->Plate());
//       printf("flag0=%d\n",pat->GetSegment(0)->Flag());
      
      //       pat=ds_rec->eRawPV.GetPattern(i);
      
//       printf("pat #%d has %d segments\n",i,pat->N());
      if(i>0) tracker.ExtrapolateTracksToZ(pat->Z());
      tracker.FillTrZMap();
      tracker.AddPattern(*pat);
//       printf("ok\n");
    }
  }

  int Ntr=0;
  int ntr = tracker.Tracks().GetEntries();
  for( int i=0; i<ntr; i++ )     {
    EdbTrackP *t = (EdbTrackP*)(tracker.Tracks().At(i));
    int nseg=t->N();
//     printf("trk %d/%d: nseg=%d (min %d)\n",i,ntr,nseg);
    if(nseg>=nsegmin) {
      for(int j=0; j<nseg; j++) {
	EdbSegP *s = t->GetSegment(j);
	s->SetErrors();
	tracker.eCond.FillErrorsCov(s->TX(),s->TY(),s->COV());
      }
      t->SetP(1.);
      t->FitTrackKFS(0);
      //fit.FitTrackLine(*t);
      tracker.RecalculateSegmentsProb(*t);
      t->SetCounters();
      AddTrack(new EdbTrackP(*t));
      
      Ntr++;
    }
//     else
    //if(t->N()>2)   t->PrintNice();
  }
  return Ntr;
}

///------------------------------------------------

int EdbDSRec::DoTracking0(bool use_btk,int p0, int p1){
  float  momentum = 0.3;
  float  mass     = 0.139;  // particle mass
  float  ProbMinP = 0.001;   // minimal probability to accept segment on propagation
  int    nsegmin  = 1;      // minimal number of segments to propagate this track
  int    ngapmax  = 3;      // maximal gap for propagation
  
  EdbPVRec gAli;
  EdbPatternsVolume* pv=use_btk?&eSegPV:&eRawPV;
  
  ///transfer patterns
  EdbPattern* pat,*pat1;
  for(int np=0; np<pv->Npatterns(); ++np){
    pat=pv->GetPattern(np);
    if(pat->Plate()<p0 || pat->Plate()>p1)continue;
    pat1=(EdbPattern*)pat->Clone();
    pat1->SetSegmentsZ();
    pat1->SetID(np);
    pat1->SetSegmentsPID();
    Log(1,"EdbDSRec",Form("add pattern %d with %d segments",pat1->PID(),pat1->N()));
    gAli.AddPattern(pat1);
  }
  
  gAli.SetScanCond(use_btk?&eCond_b:&eCond_m);
  gAli.SetCouplesPeriodic(0,1);
  int ntr = EdbDataProc::LinkTracksWithFlag( &gAli, momentum, ProbMinP, nsegmin, ngapmax, 0 );
  gAli.FitTracks( momentum, mass );
  Log(2,"EdbDSRec::DoTracking0",Form("ntr = %d\n",ntr));
  
  ///propagate
  EdbTrackFitter tf;
  
  EdbTrackP *tr=0;
  ntr = gAli.eTracks->GetEntries();
  
  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(gAli.eTracks->At(i));
    tr->SetID(i);
    tr->SetSegmentsTrack();
    tr->SetFlag(0);
  }
  
  int nadd = 0;
  int nseg=0;
  int Ntr=0;
//   nadd=gAli.PropagateTracks(0,57,0.001,3,0);
  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(gAli.eTracks->At(i));
 
    float p=momentum;
    
    tr->SetErrorP(0.2*0.2*p*p);
    
    nseg = tr->N();
    tr->SetP(p);
    printf("pid=%d\n",tr->GetSegmentFirst()->PID());
    if(tr->Flag()<0) continue;
    //     tr->PrintNice();
    nadd += gAli.PropagateTrack( *tr, true, 0.001, 3, 0 );
    if(tr->Flag()<0) printf("%d flag=%d\n",i,tr->Flag());
    //if(tr->N() != nseg) printf("%d nseg=%d (%d) \t p = %f\n",i,tr->N(),nseg,tr->P());
    ///add track here
    AddTrack((EdbTrackP*)tr->Clone());
    Ntr++;
  }
  printf("nadd = %d\n",nadd);
      
  return Ntr;
}
///------------------------------------------------
int EdbDSRec::DoVertexing(){
  SetOwnVertices(1);
  // performing vertexing
  if(gEDBDEBUGLEVEL>=1) printf("%d tracks for vertexing\n",  eVRec.eEdbTracks->GetEntries() );
  int nvtx = eVRec.FindVertex();
  if(gEDBDEBUGLEVEL>=1) printf("%d 2-track vertexes was found\n",nvtx);
  if(nvtx == 0) return 0;
  eVRec.ProbVertexN();
  for(int nv=0;nv<eVRec.Nvtx();nv++){
    EdbVertex* v=eVRec.GetVertex(nv);
    if(v->Flag()<0)delete v;
    else eVTX.Add(v);
  }
  return eVRec.Nvtx();
}
///------------------------------------------------
int EdbDSRec::DoMomEst(){
  
  float p,p0,p1;
  float tl,tt;
  EdbTrackP* trk=0;
  EdbTrackP* clon=0;
  int Nest=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
    clon=(EdbTrackP*)trk->Clone();
    Log(1,"EdbDSRec::DoMomEst",Form("Mom Est: track #%d (%dseg)",nt,trk->N()));
//     if(trk->N()<5){trk->SetP(0); printf("track #%d (%dseg) - skip!\n",nt,trk->N());continue;}
    p=eMomEst.PMSang(*clon);
//     if(eMomEst.eStatus==-1){printf("track #%d (%dseg) - skip!\n",nt,trk->N());continue;}
    printf("track #%d (%dseg) mom %f - status %d\n",nt,trk->N(),p,eMomEst.eStatus);
    p0=eMomEst.ePmin;
    p1=eMomEst.ePmax;
/*    tl=eMomEst.eGX->GetY()[0];
    tt=eMomEst.eGY->GetY()[0]; */
//     printf("track #%d (%dseg) mom %f [%2.2f <> %2.2f]\n",nt,trk->N(),p,p0,p1);
//     printf("dTL=%2.4g, dTT=%2.4g\n",tl,tt);
//     if(p<0 && (tl>0.015 || tt>0.015)){
//       printf("BAD!\n");
//       trk->SetP(-p);
//       continue;
//     }
//     if(p<0)p=0;
    trk->SetP(p);
    trk->SetPerr(p0,p1);
    delete clon;
  }
  return Nest;
}
///------------------------------------------------
int EdbDSRec::DoFindBlkSeg(EdbVertex* v,int w0, double ImpMax, double RMax, int Dpat){
  int nn=0;
  v->ClearNeighborhood();
  EdbVTA* vta=0;
  printf("start\n");
  EdbPattern* pat=0;
  EdbSegP* seg=0;
  double dzmax=Dpat*1300;
  double dx,dy,dz;
  double imp=0;
  for(int np=0;np<eSegPV.Npatterns();++np){
    pat=eSegPV.GetPattern(np);
    dz=fabs(pat->Z()-v->Z());
    if(dz<0 || dz>dzmax)continue;
    for(int ns=0;ns<pat->N();++ns){
      seg=pat->GetSegment(ns);
      if(seg->Track()!=-1)continue; ///segment assigned to track
      if(seg->W()<w0)continue; ///not black enough
      imp=v->DistSeg(seg);
      printf("imp =%2.1f\n",imp);
      if (imp > ImpMax) continue;
      dx=seg->X()-v->X();
      dy=seg->Y()-v->Y();
      vta = new EdbVTA((EdbTrackP *)seg, v);
      vta->SetZpos(1);
      vta->SetFlag(1);
      vta->SetImp(imp);
      vta->SetDist(sqrt(dx*dx+dy*dy+dz*dz));
      v->AddVTA(vta);
      ++nn;
    }
  }
  return nn;
}
///------------------------------------------------
int EdbDSRec::DoDecaySearch(){
  return 0;
}

///------------------------------------------------
void EdbDSRec::FillECovTrks(){
    EdbTrackP* trk=0;
    TObjArray trx1;
    for(int nt=0;nt<Nt();nt++){
      trk=GetTrack(nt);
      for(int ns=0;ns<trk->N();ns++) FillECovSeg(trk->GetSegment(ns));
      trk->FitTrackKFS(0);
    }
}
///------------------------------------------------
void EdbDSRec::FillECovSeg(EdbSegP* seg,EdbScanCond* cnd){
  assert(seg);
  seg->SetErrors();
  seg->SetErrorP(1.);
  if(cnd==0) ///if cnd=0 - decide which to use
    cnd=(seg->DZ()>100)?&eCond_b:&eCond_m;
  cnd->FillErrorsCov(seg->TX(),seg->TY(),seg->COV());
}
//-------------------------------------------------------------------------------
void EdbDSRec::FillECovPV(EdbPatternsVolume* pv,EdbScanCond* cnd){
  assert(pv);
  EdbPattern* pat=0;
  EdbSegP* seg;
  for(int i=0;i<pv->Npatterns();i++){
    pat=pv->GetPattern(i);
    pat->SetSegmentsPID();
    for(int j=0;j<pat->GetN();j++){
      seg=pat->GetSegment(j);
      seg->SetID(j);
      FillECovSeg(seg);
    }
  }
}
///------------------------------------------------
void EdbDSRec::FillErrorsCOV(){
  FillECovPV(&eRawPV,&eCond_m);
  FillECovPV(&eSegPV,&eCond_b);
  FillECovTrks();
}
///--------------------------------------------------
void EdbDataStore::SavePlateToRaw(const char* fname,int PID,Option_t* option){
    ///Convert this event to RAW data format!    
    EdbSegP* seg=0;
    EdbSegment* seg1=0;
    EdbPattern* ptop=FindPattern(PID,2);
    EdbPattern* pbot=FindPattern(PID,1);
    if(ptop->N()==0 && pbot->N()==0)return;
    
    ///Get plate geometry:
    EdbPlateP* p=eBrick->GetPlate(PID);
    ///--------------------------
    
    EdbRun run(fname,option);
    Log(1,"EdbDataStore::SavePlateToRaw",Form("Open file \"%s\" opt=\"%s\"",fname,option));
    Log(2,"EdbDataStore::SavePlateToRaw","created run");
    Log(2,"EdbDataStore::SavePlateToRaw",Form("cycle through %d TOP segs",ptop->N()));
    int Vid=0;
    const int NsegPerView=1000; ///maximum segments per view. When this is reached - create a new view.
    int Ns=0;
    run.GetView()->SetNframes(1,0);
    run.GetView()->GetHeader()->SetViewID(++Vid);
    for(int ns=0;ns<ptop->N();++ns){
      if(Ns==NsegPerView){
	Ns=0;
	run.AddView();
	run.GetView()->Clear();
	run.GetView()->GetHeader()->SetViewID(++Vid);
      }
      seg=ptop->GetSegment(ns);
      if(seg->W()<0)continue;
      seg1=run.GetView()->AddSegment(p->Xp(*seg),p->Yp(*seg),seg->Z(),p->TXp(*seg),p->TYp(*seg),seg->DZ(),0,(int)(seg->W()),seg->ID());
      seg1->SetSigma(seg->P(),1);
      ++Ns;
    }
    run.AddView();
    run.GetView()->Clear();
 
    Log(2,"EdbDataStore::SavePlateToRaw",Form("cycle through %d BOT segs",pbot->N()));
    run.GetView()->SetNframes(0,1);
    run.GetView()->GetHeader()->SetViewID(++Vid);
    Ns=0;
    for(int ns=0;ns<pbot->N();++ns){
      if(Ns==NsegPerView){
	Ns=0;
	run.AddView();
	run.GetView()->Clear();
	run.GetView()->GetHeader()->SetViewID(++Vid);
      }
      seg=pbot->GetSegment(ns);
      if(seg->W()<0)continue;
      seg1=run.GetView()->AddSegment(p->Xp(*seg),p->Yp(*seg),seg->Z(),p->TXp(*seg),p->TYp(*seg),seg->DZ(),1,(int)(seg->W()),seg->ID());      
      seg1->SetSigma(seg->P(),1);
      ++Ns;
    }
    run.AddView();
    run.GetView()->Clear();
    Log(2,"EdbDataStore::SavePlateToRaw","Close plate");
    run.Save();
    run.Close();    
};
///--------------------------------------------------
void EdbDataStore::SaveToRaw(char* dir,int id,Option_t* option){
/// Save the raw data to FEDRA brick structure
  EdbScanProc sp;
  EdbID Eid(id,0,1,10);
  
  sp.eProcDirClient=dir;
  sp.CheckBrickDir(Eid);
  sp.CheckAFFDir(id);

  EdbID PL=Eid;
  EdbID PL0=Eid;
  EdbPlateP* p=0;

  float z,z0;
  for(int i=0;i<eBrick->Npl();i++){
    p=eBrick->GetPlate(i);
    z=p->Z();
    PL.ePlate=i+1;
    sp.CheckPlateDir(PL);
    TString fnm;
    sp.MakeFileName(fnm,PL,"raw.root",true); ///generate plate filename
    SavePlateToRaw(fnm.Data(),i,option); ///save plate.
    if(i>0){
      ///save affine tranformations
      sp.MakeAffName(fnm,PL,PL0);
      FILE* afF=fopen(fnm.Data(),"w"); ///save affine parameters
      fprintf(afF,"ZLAYER 0   %4f 0 0\n",z0-z);
      fclose(afF);
    }
    PL0=PL;
    z0=z;
  }
}
