#include "EdbLog.h"
#include "EdbDataStore.h"

#include <TSystem.h>

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
void EdbDataStore::TransferGeometry(EdbDataStore* ds){
  Log(2,"transfer geometry","");
  ds->eBrick.Clear();
  ds->eBrick.Copy(eBrick);
}
///------------------------------------------------
void EdbDataStore::TransferTo(EdbDataStore* ds, char level,EdbSegmentCut* cut){
  assert(level>=0 && level<16);
  
  if(level&0x1){
    printf("transfer MTK segments\n");
    TransferSegs(&eRawPV,&(ds->eRawPV),cut);
    printf("done\n");
  }
  
  if(level&0x2){
    printf("transfer BTK segments\n");
    TransferSegs(&eSegPV,&(ds->eSegPV),cut);
    printf("done\n");
  }
  
  if(level&0x4){
    printf("transfer Tracks\n");
    for(int nt=0;nt<eTracks.GetEntries();++nt){
      if(GetTrack(nt)->N())ds->AddTrack(GetTrack(nt));
    }
    ds->SetOwnTracks(0);
    printf("done\n");
  }
  if(level&0x8){
    printf("transfer Vertices\n");
    for(int nv=0;nv<eVTX.GetEntries();++nv)ds->AddVertex(GetVertex(nv));
    ds->SetOwnVertices(0);
    printf("done\n");
  }
}
///------------------------------------------------
void EdbDataStore::TransferSegs(EdbPatternsVolume* pv0, EdbPatternsVolume* pv1,EdbSegmentCut* cut){
  assert(pv0);
  assert(pv1);
  EdbPattern* pat0=0;
  EdbPattern* pat1=0;
  EdbSegP* seg=0;
  int npat=pv0->Npatterns();
//   printf("pv0 - %d pat\n",pv0->Npatterns());
//   printf("pv1 - %d pat\n",pv1->Npatterns());
  if(pv1->Npatterns()==0)pv0->PassProperties(*pv1);
//   printf("pv1 - %d pat (!)\n",pv1->Npatterns());
  int nseg=0;
  float p[5];
  for(int np=0;np<npat;++np){
//     printf("pat#%d of %d\n",np,npat);
    pat0=pv0->GetPattern(np);
    pat1=pv1->GetPattern(np); ///find corresponding DEST pattern 
    assert(pat1);
    assert(pat1->Z()==pat0->Z());
//     assert(pat1->Plate()==pat0->Plate());
//     assert(pat1->Side() ==pat0->Side() );
    nseg=pat0->N();
    for(int ns=0;ns<nseg;++ns){
      seg=pat0->GetSegment(ns);
      p[0]=seg->X();
      p[1]=seg->Y();
      p[2]=seg->TX();
      p[3]=seg->TY();
      p[4]=seg->W();
      if(cut && !cut->PassCut(p))continue;
      pat1->AddSegment(*seg);
    };
//     printf("dest.pat#%d has #%d segments\n",pat1->ID(),pat1->N());
  }
  
}

///------------------------------------------------
void EdbDataStore::RestoreFromID(){
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
void EdbDataStore::RestoreTracks(){
  ///restore tracks from vertex
  ClearTracks();
  EdbVertex* v;
  for(int nv=0;nv<eVTX.GetEntries();nv++){
    v=(EdbVertex*)GetVertex(nv);
    for(int nt=0;nt<v->N();nt++){
      EdbTrackP* trk=v->GetTrack(nt);
      trk->AddVTA(v->GetVTa(nt));
      if(v->GetVTa(nt)->Zpos()==1)AddTrack(trk);
    }
  }
}
///------------------------------------------------
EdbTrackP* EdbDataStore::FindTrack(int id){
  EdbTrackP* trk=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
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
  for(int np=0;np<eBrick.Npl();++np){
    plt=eBrick.GetPlate(np);
    if(plt->ID()==plate){
      return plt->GetLayer(side);
    }
  }
  return 0;
}
///------------------------------------------------
EdbPattern* EdbDataStore::FindPattern(int plate, int side){
  assert(plate>=0);
  assert(side>=0 && side<3);
  
  EdbPatternsVolume* pv=0;
  pv=(side==0)?&eSegPV:&eRawPV;
  EdbPattern* pat=0;
  for(int np=0;np<pv->Npatterns();++np){
    pat=pv->GetPattern(np);
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
void EdbDataStore::ClearGeom(){
  for(int np=0;np<eBrick.Npl();++np){
    eBrick.GetPlate(np)->Clear();
    delete eBrick.GetPlate(np);
  }
  eBrick.Clear();
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
  pv->ePatterns->SetOwner(1);
  pv->DropCell();
  pv->DropCouples();
  int N=pv->Npatterns();
  if(!N)return;
  TObjArray* Sgs=0;
  for(int k=0;k<N;k++) {
    Sgs=pv->GetPattern(k)->GetSegments();
    if(Sgs)Sgs->Delete();
//     delete pv->GetPattern(k);
  }
  if(hard)pv->ePatterns->Clear();
  pv->Clear();
}
///------------------------------------------------
void EdbDataStore::ClearRaw(bool hard){
  EdbPatternsVolume* pv=&eRawPV;

  pv->ePatterns->SetOwner(1);
  pv->DropCell();
  pv->DropCouples();
  int N=pv->Npatterns();
  if(!N)return;
  TObjArray* Sgs=0;
  for(int k=0;k<N;k++) {
    Sgs=pv->GetPattern(k)->GetSegments();
    if(Sgs)Sgs->Delete();
//     delete pv->GetPattern(k);
  }
  if(hard)pv->ePatterns->Clear();
  pv->Clear();
}
///------------------------------------------------
void EdbDataStore::PrintBrief(){
  printf("EdbDataStore. We have here:\n");
  printf(" --%d vertcies\n --%d tracks\n --%d Plates (geometry)\n",eVTX.GetEntries(),eTracks.GetEntries(),eBrick.Npl());
  printf(" --%d BT Patterns\n --%d MT Patterns\n",eSegPV.Npatterns(),eRawPV.Npatterns());
}
///------------------------------------------------
void EdbDataStore::PrintTracks(){
  printf("EdbDataStore. We have %d tracks:\n",eTracks.GetEntries());
  EdbTrackP* trk=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
    printf(" (trk #%3d) PDG=% 4d P=% 4.2f [X,Y,Z]=[% 7.2f % 7.2f % 7.2f]\t[TX,TY]=[% 2.2f % 2.2f] Nseg=%d\n",
    trk->ID(),trk->PDG(),trk->P(),trk->X(),trk->Y(),trk->Z(),trk->TX(),trk->TY(),trk->N());
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
///==============================================================================
///==============================================================================

ClassImp(EdbDSRec);
///------------------------------------------------
EdbDSRec::EdbDSRec(){
  ///init tracker:
  eTracker.eCond.SetSigma0(3,3,0.005,0.005);
  eTracker.eDTmax=0.07;
  eTracker.eDRmax=45.;
  eTracker.InitTrZMap(  2400, 0, 120000,   2000, 0, 100000,   30 );
  ///init Vertexing:
  eVRec.eEdbTracks=&eTracks;
  ///init momentum estimator:
  eMomEst.SetParPMS_Mag();
  eMomEst.eAlg = 0;
  eMomEst.eMinEntr = 3;
}

///------------------------------------------------
void EdbDSRec::Clear(){
  EdbDataStore::Clear();
  eVRec.Reset();
  printf("DSRec cleared:\n");
  PrintBrief();
}
///------------------------------------------------

int EdbDSRec::DoTracking(bool use_btk){
  
  ///doing tracking using microtracks or basetracks
  int nsegmin=use_btk?2:3;
  SetOwnTracks(1);
  EdbPatternsVolume* pv=use_btk?(&eSegPV):(&eRawPV);
  int npl=pv->Npatterns();
  EdbPattern *pat=0;
  EdbLayer *plate=0;
  for(int ipass=0; ipass<2; ipass++) {
    printf("\n\n*************** ipass=%d ************\n",ipass);
    eTracker.eCollisionsRate=0;
    for(int i=0; i<npl; i++) {
      pat=pv->GetPattern(i);
//       printf("plate=%d side=%d\n",pat->Plate(),pat->Side());
      plate = eBrick.GetPlate(pat->Plate());
//       printf("pattern z = (%2.4f vs %2.4f)\n", pat->Z(),plate->Z());
      pat->SetSegmentsZ();
      pat->SetSegmentsPID();
      pat->Transform(    plate->GetAffineXY()   );
      pat->TransformShr( plate->Shr() );
      pat->TransformA(   plate->GetAffineTXTY() );
      pat->SetSegmentsPlate(pat->Plate());
      
      //       pat=ds_rec->eRawPV.GetPattern(i);
      
      printf("pat #%d has %d segments\n",i,pat->N());
      eTracker.AddPattern(*pat);
    }
  }

  int Ntr=0;
  int ntr = eTracker.Tracks().GetEntries();
  for( int i=0; i<ntr; i++ )     {
    EdbTrackP *t = (EdbTrackP*)(eTracker.Tracks().At(i));
    int nseg=t->N();
    if(nseg>=nsegmin) {
      for(int j=0; j<nseg; j++) {
	EdbSegP *s = t->GetSegment(j);
	s->SetErrors();
	eTracker.eCond.FillErrorsCov(s->TX(),s->TY(),s->COV());
      }
      t->SetP(1.);
      t->FitTrackKFS(0);
      //fit.FitTrackLine(*t);
      eTracker.RecalculateSegmentsProb(*t);
      t->SetCounters();
      AddTrack(t);
      
      Ntr++;
    }
//     else
    //if(t->N()>2)   t->PrintNice();
  }
  return Ntr;
}

///------------------------------------------------
int EdbDSRec::DoVertexing(){
  SetOwnVertices(1);
  // performing vertexing
  if(gEDBDEBUGLEVEL>1) printf("%d tracks vor vertexing\n",  eVRec.eEdbTracks->GetEntries() );
  int nvtx = eVRec.FindVertex();
  if(gEDBDEBUGLEVEL>1) printf("%d 2-track vertexes was found\n",nvtx);
  if(nvtx == 0) return 0;
  eVRec.ProbVertexN();
  for(int nv=0;nv<eVRec.Nvtx();nv++){
    eVTX.Add(eVRec.GetVertex(nv));
  }
  return eVRec.Nvtx();
}
///------------------------------------------------
int EdbDSRec::DoMomEst(){
  
  float p,p0,p1;
  float tl,tt;
  EdbTrackP* trk=0;
  int Nest=0;
  for(int nt=0;nt<eTracks.GetEntries();++nt){
    trk=GetTrack(nt);
    if(trk->N()<8){trk->SetFlag(-1); printf("track #%d (%dseg) - skip!\n",nt,trk->N());continue;}
    p=eMomEst.PMS(*trk);
//     if(eMomEst.eStatus==-1){printf("track #%d (%dseg) - skip!\n",nt,trk->N());continue;}
    p0=eMomEst.ePmin;
    p1=eMomEst.ePmax;
    tl=eMomEst.eGX->GetY()[0];
    tt=eMomEst.eGY->GetY()[0];
//   tf->DrawPlots();
    printf("track #%d (%dseg) mom %f [%2.2f <> %2.2f]\n",nt,trk->N(),p,p0,p1);
    printf("dTL=%2.4g, dTT=%2.4g\n",tl,tt);
    if(p<0 && (tl>0.015 || tt>0.015)){
      printf("BAD!\n");
      trk->SetFlag(-2);
      continue;
    }
    trk->SetP(p);
    trk->SetPerr(p0,p1);
  }
  return Nest;
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
void EdbDataStore::SavePlateToRaw(int PID,const char* dir,int id){
    ///Convert this event to RAW data format!
//     char* fname=Form("%s/p%03d/%d.%d.%d.%d.raw.root",dir,PID+1,id,PID+1,0,0);
    char* fname=Form("%s/%d.%d.%d.%d.raw.root",dir,id,PID+1,100,10);
    EdbRun run(fname,"RECREATE");
    printf("created run\n");
    printf("added view\n");
    EdbSegP* seg=0;
    EdbPattern* ptop=FindPattern(PID,2);
    printf("cycle through %d TOP segs\n",ptop->N());
    run.GetView()->SetNframes(1,0);
    for(int ns=0;ns<ptop->N();++ns){
      seg=ptop->GetSegment(ns);
      run.GetView()->AddSegment(seg->X(),seg->Y(),seg->Z(),seg->TX(),seg->TY(),seg->DZ(),1,(int)(seg->W()),seg->ID());
    }
    run.AddView();
    run.GetView()->Clear();
    EdbPattern* pbot=FindPattern(PID,1);
    printf("cycle through %d BOT segs\n",pbot->N());
    run.GetView()->SetNframes(0,1);
    for(int ns=0;ns<pbot->N();++ns){
      seg=pbot->GetSegment(ns);
      run.GetView()->AddSegment(seg->X(),seg->Y(),seg->Z(),seg->TX(),seg->TY(),seg->DZ(),2,(int)(seg->W()),seg->ID());
    }
    run.AddView();
    printf("close\n");
    run.PrintBranchesStatus();
    run.Save();
    run.Close();
    //   EdbPattern* pbot=FindPattern(PID,2);
    
//   TFile* fout=new TFile(Form("%s/run%d.%d.root",dir,id,PID),"recreate");
//   EdbRunHeader *hdrR=new EdbRunHeader(id);
//   hdrR->Write();
// 
//   TTree* Views=new TTree("Views","Views");
//   EdbViewHeader *hdrV=new EdbViewHeader();
//   Views->Branch("headers","EdbViewHeader",&hdrV);
//   
//   EdbPattern* ptop=FindPattern(PID,1);
//   EdbPattern* pbot=FindPattern(PID,2);
//   TClonesArray segarr("EdbSegment",ptop->N());
//   Views->Branch("segments", &segarr);
// 
//   EdbSegP* seg;
//   printf("saving %d TOP segments...\n",ptop->N());
//   hdrV->SetCoordXY(ptop->X(),ptop->Y());
//   hdrV->SetNframes(1,0);
//   hdrV->SetViewID(1);
//   hdrV->SetNsegments(ptop->N());
//   for(int ns=0;ns<ptop->N();++ns){
//     seg=ptop->GetSegment(ns);
//     new(segarr[ns]) EdbSegment(seg->X(),seg->Y(),seg->Z(),seg->TX(),seg->TY(),seg->DZ(),
// 				 1,seg->W(),seg->ID());
//   }
//   Views->Fill();
//   segarr.Clear("C");
//   segarr.Expand(pbot->N());
//     
//   printf("saving %d BOT segments...\n",pbot->N());
//   hdrV->SetCoordXY(pbot->X(),pbot->Y());
//   hdrV->SetNframes(0,1);
//   hdrV->SetViewID(2);
//   hdrV->SetNsegments(pbot->N());
//   for(int ns=0;ns<pbot->N();++ns){
//     seg=pbot->GetSegment(ns);
//     new(segarr[ns]) EdbSegment(seg->X(),seg->Y(),seg->Z(),seg->TX(),seg->TY(),seg->DZ(),
// 				 2,seg->W(),seg->ID());
//   }
//   Views->Fill();
//   
//   printf("Done!\n");
//   Views->Write();
//   segarr.Clear("C");
//   fout->Close();
    
};
///--------------------------------------------------
void EdbDataStore::SaveToRaw(char* dir,int id){
  TString dir1=Form("%s/b%06d",dir,id);
  if(gSystem->AccessPathName(dir1.Data())){
    printf("create dir \"%s\"\n",dir1.Data()); 
    gSystem->mkdir(dir1.Data());
  }
  TString dir2=dir1;
  for(int i=0;i<Nplt();i++){
    dir2=Form("%s/p%03d",dir1.Data(),i+1);
    if(gSystem->AccessPathName(dir2.Data())){
      printf("... create dir \"%s\"\n",dir2.Data()); 
      gSystem->mkdir(dir2.Data());
    }
    SavePlateToRaw(i,dir2.Data(),id);
  }
}
