//-- Author :  Valeri Tioukov   22/12/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanProc                                                          //
//                                                                      //
// scanned data processing library                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "Varargs.h"
#include "TSystem.h"
#include "EdbLog.h"
#include "EdbTrackFitter.h"
#include "EdbScanProc.h"
#include "EdbTestAl.h"

using namespace TMath;

ClassImp(EdbScanProc)
//----------------------------------------------------------------
EdbScanProc::EdbScanProc()
{
}

//----------------------------------------------------------------
void EdbScanProc::Print()
{
  printf("ProcDirClient: %s\n",eProcDirClient.Data());
}

//----------------------------------------------------------------
bool EdbScanProc:: AddAFFtoScanSet(EdbScanSet &sc, int b1, int p1, int s1, int e1,int b2, int p2, int s2, int e2)
{
  int id1[4]={b1,p1,s1,e1};
  int id2[4]={b2,p2,s2,e2};
  return AddAFFtoScanSet(sc,id1,id2);
}

//----------------------------------------------------------------
bool EdbScanProc:: AddAFFtoScanSet(EdbScanSet &sc, EdbID eid1, EdbID eid2)
{
  int id1[4]; eid1.Get(id1);
  int id2[4]; eid2.Get(id2);
  return AddAFFtoScanSet(sc,id1,id2);
}

//----------------------------------------------------------------
bool EdbScanProc:: AddAFFtoScanSet(EdbScanSet &sc, int id1[4], int id2[4])
{
  // read affine tranformation from file, form "plate" and add it into EdbScanSet:ePC

  EdbDataPiece piece;  
  TString parfile;
  MakeAffName(parfile,id1,id2);
  piece.eFileNamePar = parfile;
  if(piece.TakePiecePar() < 0) return false;
  float       dz = piece.GetLayer(0)->Z();
  EdbAffine2D *a = piece.GetLayer(0)->GetAffineXY();
  if(gEDBDEBUGLEVEL>2) a->Print();

  EdbPlateP *p = new EdbPlateP();   // 0-base, 1-up, 2-down

  p->GetLayer(1)->SetID(id1[1]);
  p->GetLayer(1)->SetZlayer(-dz,-dz,-dz);
  p->GetLayer(1)->SetAffXY(a->A11(),a->A12(),a->A21(),a->A22(),a->B1(),a->B2());

  p->GetLayer(2)->SetID(id2[1]);
  p->GetLayer(2)->SetZlayer(0,0,0);
  p->GetLayer(2)->SetAffXY(1,0,0,1,0,0);

  sc.ePC.Add(p);
  return true;
}

//----------------------------------------------------------------
int EdbScanProc::AssembleScanSet(EdbScanSet &sc)
{
  if (sc.eIDS.GetSize() == 1) {  // add nominal plate
    EdbPlateP *plate = new EdbPlateP();
    Float_t dz0=214,dz1=45,dz2=45;  //TODO!
    plate->SetZlayer(z, z - dz0/2 + dz1, z+dz0/2+dz2);                
    plate->GetLayer(0)->SetZlayer(0,-dz0/2,dz0/2);       // internal plate coord
    plate->GetLayer(2)->SetZlayer(-dz0/2,-dz0/2-dz2,-dz0/2);
    plate->GetLayer(1)->SetZlayer( dz0/2, dz0/2, dz0/2+dz1);
    sc.eB.AddPlate(plate);
    return 1;
  }
  // make couples in a given order

  EdbID *id1=0, *id2=0;
  for( int i=1; i<sc.eIDS.GetEntries(); i++ ) {
    id1 = (EdbID *)(sc.eIDS.At(i-1));
    id2 = (EdbID *)(sc.eIDS.At(i));
    AddAFFtoScanSet(sc, *id1, *id2);
  }
  return sc.AssembleBrickFromPC();
}

//----------------------------------------------------------------
int EdbScanProc::ReadFoundSegment(EdbID id,  EdbSegP &s, int flag)
{
  EdbPattern pat;
  ReadFound(pat, id, flag);
  EdbSegP *ss = pat.FindSegment(s.ID());
  if(!ss) return 0;
  s.Copy(*ss);
  return 1;
}

//----------------------------------------------------------------
int EdbScanProc::ReadFoundTrack(EdbScanSet &sc,  EdbTrackP &track, int flag)
{
  // read track.ID() from pieces in sc.IDS .found.root and apply transformations from sc

  EdbPlateP  *plate;
  int count=0;
  int n = sc.eIDS.GetEntries();
  EdbPattern pat;
  pat.AddSegment(track.ID(), 0,0,0,0);
  EdbSegP *s = pat.GetSegment(0);

  for(int i=0; i<n; i++) {
    EdbID *id    = (EdbID *)(sc.eIDS.At(i));
    if(ReadFoundSegment(*id,*s)<=0) continue;
    count++;
    plate = sc.GetPlate(id->ePlate);
    pat.Transform(    plate->GetAffineXY()   );
    pat.TransformA(   plate->GetAffineTXTY() );
    pat.TransformShr( plate->Shr() );
    s->SetZ(plate->Z());
    s->SetDZ(-214);                            //TODO!!!
    s->SetPID(id->ePlate);
    track.AddSegment( new EdbSegP(*s) );
  }

  return count;
}

//----------------------------------------------------------------
int EdbScanProc::ReadFoundTracks(EdbScanSet &sc,  EdbPVRec &ali, int flag)
{
  // read all tracks  from found.root listed in sc.IDS and apply transformations from sc.eB
  // return the total number of segments added

  EdbPlateP  *plate;
  int n = sc.eIDS.GetEntries();
  int count=0;

  for(int i=0; i<n; i++) {
    EdbID *id  = sc.GetID(i);
    EdbPattern pat;
    ReadFound(pat, *id, flag);
    plate = sc.GetPlate(id->ePlate);
    pat.SetPID(id->ePlate);
    pat.SetSegmentsPID();
    pat.Transform(    plate->GetAffineXY()   );
    pat.TransformA(   plate->GetAffineTXTY() );
    pat.TransformShr( plate->Shr() );
    pat.SetZ(plate->Z());
    pat.SetSegmentsZ();
    //    s->SetDZ(-214);                            //TODO!!!
    
    for(int j=0; j<pat.N(); j++) {
      EdbSegP *s = pat.GetSegment(j);
      EdbTrackP *track = ali.FindTrack(s->ID());
      if(track)  track->AddSegment( new EdbSegP(*s) );
      else       ali.AddTrack( new EdbTrackP(new EdbSegP(*s)) );
    }
  }

  return count;
}

//----------------------------------------------------------------
int EdbScanProc::ReadScanSetCP(EdbScanSet &sc,  EdbPVRec &ali, TCut c, bool do_erase)
{
  // read data from scanset sc with cut c and fill ali
  // sc.eIDS is used as an id list
  // sc.eB   is used to get the brick geometry and affine transformations - must be filled before

  int cnt=0;
  int n = sc.eIDS.GetEntries();    // number of pieces to get
  EdbID      *id;
  EdbPlateP  *plate;
  EdbPattern *pat;
  for(int i=0; i<n; i++) {
    id    = (EdbID *)(sc.eIDS.At(i));
    plate = sc.GetPlate(id->ePlate);
    pat   = new EdbPattern();
    cnt += ReadPatCPnopar( *pat, *id, c, do_erase);

    pat->SetPID(id->ePlate);
    for(int j=0; j<pat->N(); j++) pat->GetSegment(j)->SetVid( id->ePlate, pat->GetSegment(j)->Vid(1) );
    pat->SetZ(plate->Z());
    pat->SetSegmentsZ();
    pat->Transform(    plate->GetAffineXY()   );
    pat->TransformA(   plate->GetAffineTXTY() );
    pat->TransformShr( plate->Shr() );
    ali.AddPattern( pat );
  }

  ali.SetPatternsID();
  for(int i=0; i<n; i++)  ali.GetPattern(i)->SetSegmentsPID();  // PID of the segment must be ID of the pattern!

  ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(ali.GetScanCond()->Chi2PMax());
  ali.SetOffsetsMax(ali.GetScanCond()->OffX(),ali.GetScanCond()->OffY());

  return cnt;
}

//----------------------------------------------------------------
bool EdbScanProc::MakeAFFSet(EdbScanSet &sc)
{
  if(sc.eIDS.GetEntries()<2) return 0;
  EdbID *id1,*id2;
  for(int i=0; i<sc.eIDS.GetEntries()-1; i++) {
    id1 = (EdbID *)(sc.eIDS.At(i));
    id2 = (EdbID *)(sc.eIDS.At(i+1));
    EdbAffine2D aff;
    sc.GetAffP2P(id1->ePlate,id2->ePlate, aff);
    float dz = sc.GetDZP2P(id1->ePlate,id2->ePlate);
    TString str;
    MakeAffName(str,*id1,*id2);
  
    char card[128];
    sprintf(card,"ZLAYER 0 %f 0 0",dz);
    LogPrint(id1->eBrick,2,"MakeAFFSet","%s as %s", str.Data(),card);
    AddParLine(str.Data(),card);
    sprintf(card,"AFFXY 0 %f %f %f %f %f %f", aff.A11(), aff.A12(), aff.A21(), aff.A22(), aff.B1(), aff.B2() );
    LogPrint(id1->eBrick,2,"MakeAFFSet","%s as %s", str.Data(),card);
    AddParLine(str.Data(),card);
  }
  return 1;
}

//----------------------------------------------------------------
int EdbScanProc::AlignSet(EdbScanSet &sc, int npre, int nfull, const char *opt )
{
  if(sc.eIDS.GetEntries()<2) return 0;
  int n=0;
  EdbID *id1,*id2;
  for(int i=0; i<sc.eIDS.GetEntries()-1; i++) {
    id1 = (EdbID *)(sc.eIDS.At(i));
    id2 = (EdbID *)(sc.eIDS.At(i+1));
    int id14[4]; id1->Get(id14);
    int id24[4]; id2->Get(id24);
    n += AlignAll(id14, id24, npre, nfull, opt);
  }
  return n;
}


//----------------------------------------------------------------
int EdbScanProc::LinkSet(EdbScanSet &sc, int npre, int nfull, int correct_ang)
{
  int n=0;
  EdbID *id;
  for(int i=0; i<sc.eIDS.GetEntries(); i++) {
    id = (EdbID *)(sc.eIDS.At(i));
    n += LinkRunAll(*id,npre,nfull,correct_ang);
  }
  return n;
}

//----------------------------------------------------------------
void EdbScanProc::CheckFiles( EdbScanSet &sc, const char *suffix )
{
  EdbID *id;
  for(int i=0; i<sc.eIDS.GetEntries(); i++)  {
    id = (EdbID*)sc.eIDS.At(i);
    TString str;
    MakeFileName(str,*id,suffix);
    gSystem->Exec(Form("ls -l %s",str.Data()));
  }
}

//----------------------------------------------------------------
EdbMask *EdbScanProc::ReadEraseMask(EdbID id)
{
  TString str;
  MakeFileName(str,id,"er.root");
  TFile f(str.Data());
  if(!f.IsOpen()) return 0;

  EdbMask *m = 0;
  f.GetObject("mask",m);
  f.Close();
  return m;
}

//----------------------------------------------------------------
int EdbScanProc::ReadPatCPnopar(EdbPattern &pat, EdbID id, TCut cut, bool do_erase)
{
  TString cpfile;
  MakeFileName(cpfile,id,"cp.root");
  EdbDataPiece piece;
  piece.eFileNameCP  = cpfile;
  piece.AddRCut(0,cut);
  if(!piece.InitCouplesTree("READ")) return 0;

  if(do_erase) piece.eEraseMask = ReadEraseMask(id);
  return piece.GetCPData_new( &pat,0,0,0 );
}

//----------------------------------------------------------------
void EdbScanProc::PrepareVolumesPred( int idIN[4], EdbPattern &points,
				      int before, int after, int pmin, int pmax, EdbScanSet *sc )
{
  // create prediction patterns for the stopping points in the segments of the pattern "points"
  // should be correctly defined  (eID ePID eX eY eSX eSY)

  TIndexCell cell;
  Long_t  v[2];  // pl,id
  EdbSegP *s=0;
  for(int i=0; i<points.N(); i++) {
    s = points.GetSegment(i);
    for(int ip=Max(s->PID()-before,pmin); ip<=Min(s->PID()+after,pmax); ip++) {
      v[0] = (Long_t)(ip);
      v[1] = (Long_t)(i);
      cell.Add(2,v);
    }
  }

  cell.Sort();
  int count=0;
  int plate;
  for(int ip=0; ip<cell.GetEntries(); ip++) {
    plate = cell.At(ip)->Value();                             // current prediction plate
    EdbPattern pat;
    for(int iv=0; iv<cell.At(ip)->GetEntries(); iv++) {
      s = points.GetSegment(cell.At(ip)->At(iv)->Value());    // s.PID() - stopping plate
      pat.AddSegment(*s);
      pat.GetSegment(iv)->SetPID(plate);
      if(sc) {
	EdbAffine2D aff;
	if(sc->GetAffP2P(s->PID(), plate, aff) ) pat.GetSegment(iv)->Transform(&aff);
      }
    }
    int id[4]={idIN[0],plate,idIN[2],idIN[3]};
    WritePred(pat,id);
    count += pat.N();
  }
  
  LogPrint(idIN[0],2,"PrepareVolumesPred","%d.%d.%d.%d: for %d  volumes generated %d predictions with settings: before=%d after=%d pmin=%d pmax=%d\n",
	   idIN[0],idIN[1],idIN[2],idIN[3],points.N(), count, before,after,pmin,pmax );
}

//----------------------------------------------------------------
bool EdbScanProc::FlashRawDir(EdbScanClient &scan, int id[4])
{
  // move all rwc and rwd files from the raw scanning directory into the new subdir

  char str[256];
  TDatime dt;
  sprintf(str,"%s/rw_%u",scan.eRawDirClient.Data(),dt.Get());
  LogPrint(id[0],2,"FlashRawDir","%d.%d.%d.%d: move all into %s", id[0],id[1],id[2],id[3],str);
  if(!gSystem->OpenDirectory(str))   
    if( gSystem->MakeDirectory(str) == -1) 
      {
	LogPrint(id[0],2,"FlashRawDir","WARNING! %d.%d.%d.%d: FAILED creating directory %s", 
		 id[0],id[1],id[2],id[3],str);
	return false;
      }
  char str2[256];

#ifdef WIN32
  sprintf(str2,"ren %s/raw.* %s",scan.eRawDirClient.Data(),str);
#else
  sprintf(str2,"mv %s/raw.* %s",scan.eRawDirClient.Data(),str);
#endif

  gSystem->Exec(str2);
  return true;
}

//----------------------------------------------------------------
int EdbScanProc::LoadPlate(EdbScanClient &scan, int id[4], int attempts)
{
  int status=0;
  FlashRawDir(scan,id);
  TString map;
  if(!GetMap(id[0],map)) {
    LogPrint(id[0],1,"LoadPlate","ERROR: map file does not exist! Stop here. *** %d.%d.%d  status = %d ***",
	     id[0],id[1],id[2],status);
    return status;
  }
  status= scan.LoadPlate(id[0],id[1],map.Data(),attempts);
  LogPrint(id[0],1,"LoadPlate","******************** %d.%d.%d  status = %d ************************************",
	   id[0],id[1],id[2],status);
  return status;
}

//------------------------------------------------------------------------------------------
int EdbScanProc::RemoveDublets( EdbPattern &pin, EdbPattern &pout, int brick )
{
  // input:  pin  - predictions pattern
  // output: pout - predictions pattern with dublets removed
  if(pin.N()<2)     return 0;
  float r,rt;
  float rmin = 0.1;    // [micron] TODO - pass as parameter?
  float rtmin= 0.0001; // [rad]    TODO - pass as parameter?
  OptimizeScanPath(pin, pout,brick);
  EdbSegP *s=0,*s1=0;
  int n= pout.N();
  for(int i=n-1; i>0; i--) {
    s  = pout.GetSegment(i);
    s1 = pout.GetSegment(i-1);
    r  = Sqrt((s->X()-s1->X())*(s->X()-s1->X()) + (s->Y()-s1->Y())*(s->Y()-s1->Y()));
    if( r>rmin   )   continue;
    rt = Sqrt((s->TX()-s1->TX())*(s->TX()-s1->TX()) + (s->TY()-s1->TY())*(s->TY()-s1->TY()));
    if( rt>rtmin )   continue;
    pout.GetSegments()->RemoveAt(i);
  }
  pout.GetSegments()->Compress();
  LogPrint(brick,2,"RemoveDublets","%d segments before -> %d segments after: %d dublets removed", n, pout.N(), n-pout.N());
  return n-pout.N();
}

//------------------------------------------------------------------------------------------
void EdbScanProc::OptimizeScanPath(EdbPattern &pin, EdbPattern &pout, int brick)
{
  // input:  pin  - predictions pattern
  // output: pout - predictions pattern with optimized path (should be empty at the beginning)

  int n = pin.N();
  if(pout.N()) pout.GetSegments()->Delete();
  if(n>2) {
    EdbSegP *s;
    TIndexCell cell;
    float xmin = pin.Xmin()-0.000001, xmax = pin.Xmax()+0.000001;
    float ymin = pin.Ymin()-0.000001, ymax = pin.Ymax()+0.000001;
    float eps  = Sqrt(3.*(xmax-xmin)*(ymax-ymin)/n);
    float binx = (xmax-xmin)/((int)((xmax-xmin)/eps));
    Long_t  v[3];  // x,y,i
    for(int i=0; i<n; i++ ) {
      s = pin.GetSegment(i);
      v[0] = (Long_t)((s->X()-xmin)/binx);
      v[1] = (Long_t)((s->Y()-ymin));
      if(v[0]%2 != 0) v[1] = -v[1];         // serpentina along y
      v[2] = i;
      cell.Add(3,v);
    }
    cell.Sort();
    for(int ix=0; ix<cell.GetEntries(); ix++)
      for(int iy=0; iy<cell.At(ix)->GetEntries(); iy++)
	for(int ii=0; ii<cell.At(ix)->At(iy)->GetEntries(); ii++) {
	  s = pin.GetSegment(cell.At(ix)->At(iy)->At(ii)->Value());
	  pout.AddSegment(*s);
	}
  }
  if( pout.N()>0 )
    if( pin.SummaryPath() > pout.SummaryPath() ) {          // good optimization
      LogPrint(brick,2,"OptimizeScanPath","with %d  predictions: gain in path[mm] before/after = %.1f/%.1f = %.1f",
	       n, pin.SummaryPath()/1000,pout.SummaryPath()/1000, pin.SummaryPath()/pout.SummaryPath());
      return;
    }
  if(pout.N()) pout.GetSegments()->Delete();
  for(int i=0; i<n; i++) pout.AddSegment(*(pin.GetSegment(i)));
}

//----------------------------------------------------------------
int EdbScanProc::ConvertAreas(EdbScanClient &scan, int id[4], int flag, const char *opt)
{
  // can be called separately in case of missed conversion
  EdbPattern pred;
  ReadPred(pred, id, flag);
  EdbRun *run = InitRun(id);
  if(!run) return 0;
  int scanned = scan.ConvertAreas(id,pred,*run,opt);
  LogPrint(id[0],1,"ConvertAreas","%d.%d.%d.%d  with %d predictions with flag %d; %d views stored", 
	   id[0],id[1],id[2],id[3],pred.N(),flag,run->GetEntries());
  run->Close();
  delete run;
  return scanned;
}

//----------------------------------------------------------------
int EdbScanProc::ScanAreas(EdbScanClient &scan, int id[4], int flag, const char *opt)
{
  EdbPattern pred;
  ReadPred(pred, id, flag);
  LogPrint(id[0],1,"ScanAreas","%d.%d.%d.%d  with %d predictions with flag %d", 
	   id[0],id[1],id[2],id[3],pred.N(),flag);
  EdbPattern predopt;
  OptimizeScanPath(pred,predopt,id[0]);
  EdbRun *run = InitRun(id);
  if(!run) return 0;
  int scanned = scan.ScanAreas(id,predopt,*run,opt);
  LogPrint(id[0],1,"ScanAreas","%d.%d.%d.%d  %d predictions scanned; run with %d views stored", 
	   id[0],id[1],id[2],id[3],scanned, run->GetEntries() );
  run->Close();
  delete run;
  return scanned;
}

//----------------------------------------------------------------
int EdbScanProc::ScanAreas(EdbScanClient &scan, EdbPattern &pred, int id[4], const char *opt) // NEW!!!
{
  LogPrint(id[0],1,"ScanAreas","%d.%d.%d.%d  with %d predictions", id[0],id[1],id[2],id[3],pred.N());
  EdbPattern predopt;
  OptimizeScanPath(pred,predopt,id[0]);
  EdbRun *run = InitRun(id);
  if(!run) return 0;
  int scanned = scan.ScanAreas(id,predopt,*run,opt);
  LogPrint(id[0],1,"ScanAreas","%d.%d.%d.%d  %d predictions scanned; run with %d views stored", id[0],id[1],id[2],id[3],scanned, run->GetEntries() );
  run->Close();
  delete run;
  return scanned;
}

//----------------------------------------------------------------
int EdbScanProc::CopyFile(int id1[4], int id2[4], const char *suffix, bool overwrite)
{
  // copy piece file from id1 to id2
  TString name1, name2;
  MakeFileName(name1,id1,suffix);
  MakeFileName(name2,id2,suffix);
  int status = gSystem->CopyFile(name1.Data(), name2.Data(), overwrite);
  LogPrint(id1[0],2,"CopyFile","status=%d from %s to %s", status, name1.Data(),name2.Data() );
  //sleep(10);
  if( gSystem->AccessPathName(name2, kReadPermission) ) return 0; //can not access file!
  return 1;
}

//----------------------------------------------------------------
int EdbScanProc::RemoveFile(EdbID id, const char *suffix)
{
  // remove file
  TString name;
  MakeFileName(name,id,suffix);
  char str[256];
  sprintf(str,"%s/file_to_remove",eProcDirClient.Data());
  int status = gSystem->Rename(name.Data(), str);
  LogPrint(id.eBrick,2,"RemoveFile","status=%d from %s to %s", status, name.Data(), str );
  return status;
}

//----------------------------------------------------------------
int EdbScanProc::CopyAFFPar(int id1c[4], int id2c[4], int id1p[4], int id2p[4], bool overwrite)
{
  // copy AFF/xc_yc.par to AFF/xp_yp.par
  TString name1, name2;
  MakeAffName(name1,id1c,id2c);
  MakeAffName(name2,id1p,id2p);
  LogPrint(id1c[0],2,"CopyAFFPar","from %s to %s", name1.Data(),name2.Data());
  return gSystem->CopyFile(name1.Data(), name2.Data(), overwrite);
}

//----------------------------------------------------------------
bool EdbScanProc::SetAFF0(int id1[4], int id2[4])
{
  TString str;
  MakeAffName(str,id1,id2);
  char card[64];
  sprintf(card,"AFFXY 0 1. 0. 0. 1. 0. 0.");
  LogPrint(id1[0],2,"SetAFF0","%s as %s", str.Data(),card);
  return AddParLine(str.Data(),card);
}

//----------------------------------------------------------------
bool EdbScanProc::SetAFFDZ(int id1[4], int id2[4], float dz)
{
  TString str;
  MakeAffName(str,id1,id2);
  char card[64];
  sprintf(card,"ZLAYER 0 %f 0 0",dz);
  LogPrint(id1[0],2,"SetAFFDZ","%s as %s", str.Data(),card);
  return AddParLine(str.Data(),card);
}

//----------------------------------------------------------------
int EdbScanProc::LinkRunAll(int id[4], int npre, int nfull, int correct_ang )
{
  LogPrint(id[0],1,"LinkRunAll","%d.%d.%d.%d  %d prelinking + %d fullinking", id[0],id[1],id[2],id[3],npre,nfull);
  int nc=0;
  if(npre>0) {
    MakeInPar(id,"prelinking");    // make input par file (x.x.x.x.in.par) for the current ID including the prelinking par file
    for(int i=0; i<npre; i++) {
      nc = LinkRun(id,0);          // will be done (pre)linking and updated x.x.x.x.par file
      if(correct_ang)  CorrectAngles(id);
    }
  }
  if(nfull>0) {
    MakeInPar(id,"fulllinking");   // make input par file including the fulllinking par file
    for(int i=0; i<nfull; i++)
      nc = LinkRun(id,1);         // will be done (full)linking and DO NOT updated x.x.x.x.par file
  }
  LogPrint(id[0],1,"LinkRunAll","%d couples stored", nc);
  return nc;
}

//----------------------------------------------------------------
bool EdbScanProc::ProjectFound(int id1[4],int id2[4])
{
  //take xp.xp.xp.xp.found.root and produce yp.yp.yp.yp.pred.root using the AFF/xp_yp.par
  LogPrint(id1[0],2,"ProjectFound","from %d.%d.%d.%d to %d.%d.%d.%d", id1[0],id1[1],id1[2],id1[3],id2[0],id2[1],id2[2],id2[3]);
  EdbPattern pat;
  ReadFound(pat,id1);
  //pat.SetZ(0);
  //pat.SetSegmentsZ();
  for(int i=0; i<pat.N(); i++) {
    pat.GetSegment(i)->SetErrors(50., 50., 0., .1, .1);  // area to be scanned for this prediction
  }
  ApplyAffZ(pat,id1,id2);
  WritePred(pat,id2);
  WritePatTXT(pat,id2,"man.pred.txt");
  return true;
}

//----------------------------------------------------------------
bool EdbScanProc::CorrectPredWithFound(int id1[4],int id2[4], const char *opt, int patmin)
{
  // take p1.found.root, apply AFF/p1_p2.par, read p2.found.root, align and update AFF/p1_p2.par

  EdbPattern p1;   ReadFound(p1,id1);
  EdbPattern p2;   ReadFound(p2,id2);

  if(p1.N()<2||p2.N()<2) {
    LogPrint(id1[0],1,"CorrectPredWithFound","ERROR: correction is impossible - too small pattern: %d", p1.N(), p2.N());
    return false;
  }
  else   if(p1.N()<patmin || p2.N()<patmin) 
    LogPrint(id1[0],1,"CorrectPredWithFound","WARNING: unreliable correction - pattern is too small: %d < %d", p1.N(), p2.N(),patmin);
  
  EdbAffine2D aff;  float dz; 
  if(!GetAffZ(aff, dz, id1,id2))  return false;
  p1.Transform(&aff);
  p1.SetZ(-dz);   p1.SetSegmentsZ();
  p2.SetZ(0);     p2.SetSegmentsZ();

  MakeInPar(id2,"fullalignment");
  EdbDataPiece piece2;
  InitPiece(piece2, id2);

  EdbPVRec ali;
  ali.AddPattern(&p1);
  ali.AddPattern(&p2);
  EdbScanCond *cond = piece2.GetCond(0);
  cond->SetChi2Mode(3);
  ali.SetScanCond( cond );
  ali.SetPatternsID();
  ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(cond->Chi2PMax());
  ali.SetOffsetsMax(cond->OffX(),cond->OffY());

  ali.Align(2);
  //ali.Align(0);
  int nal = ali.GetCouple(0)->Ncouples();

  if(nal<patmin) {
    LogPrint(id1[0],1,"CorrectAffWithPred","WARNING: pattern is too small: %d < %d: do not update par file!", nal, patmin);
    return false;
  }

  TString parfileOUT;
  MakeAffName(parfileOUT,id1,id2);
  piece2.eFileNamePar = parfileOUT;
  ali.GetPattern(0)->GetKeep(aff);
  piece2.UpdateAffPar(0,aff);

  if( strstr(opt,"-z")) {
    EdbDataProc proc;
    proc.LinkTracksWithFlag( &ali, 10., 0.05, 2, 3, 0 );
    printf("befire corr z: z1=%f z2=%f \n",ali.GetPattern(0)->Z(), ali.GetPattern(1)->Z());
    ali.FineCorrZnew();
    printf("after corr z: z1=%f z2=%f \n",ali.GetPattern(0)->Z(), ali.GetPattern(1)->Z());
    piece2.UpdateZPar(0,-ali.GetPattern(0)->Z());
  }
 
  LogPrint(id1[0],1,"CorrectPredWithFound","from %d.%d.%d.%d to %d.%d.%d.%d: used %d (out of %d predictions and %d found) for correction", 
	   id1[0],id1[1],id1[2],id1[3],id2[0],id2[1],id2[2],id2[3],  nal, p1.N(), p2.N() );
  return true;
}

//----------------------------------------------------------------
bool EdbScanProc::CorrectAffWithPred(int id1[4],int id2[4], const char *opt, int patmin)
{
  // take p1.found.root, apply AFF/p1_p2.par, align to p2 and update AFF/p1_p2.par
  EdbPattern pat;
  ReadFound(pat,id1);
  if(pat.N()<patmin)
    LogPrint(id1[0],1,"CorrectAffWithPred","WARNING: unreliable correction - pattern is too small: %d < %d", pat.N(),patmin);
  else if(pat.N()<2) {
    LogPrint(id1[0],1,"CorrectAffWithPred","ERROR: correction is impossible - too small pattern: %d", pat.N());
    return false;
  }

  EdbAffine2D aff;
  float dz; 
  if(!GetAffZ(aff, dz, id1,id2))  return false;
  pat.Transform(&aff);
  pat.SetZ(-dz);
  pat.SetSegmentsZ();

  MakeInPar(id2,"fullalignment");
  EdbPattern p2;
  EdbDataPiece piece2;
  InitPiece(piece2, id2);
  ReadPiece(piece2, p2);
  p2.SetZ(0);
  p2.SetSegmentsZ();

  EdbPVRec ali;
  ali.AddPattern(&pat);
  ali.AddPattern(&p2);
  EdbScanCond *cond = piece2.GetCond(0);
  cond->SetChi2Mode(3);
  ali.SetScanCond( cond );
  ali.SetPatternsID();
  ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(cond->Chi2PMax());
  ali.SetOffsetsMax(cond->OffX(),cond->OffY());

  ali.Align(2);
  //ali.Align(0);
  int nal = ali.GetCouple(0)->Ncouples();

  if(nal<patmin) {
    LogPrint(id1[0],1,"CorrectAffWithPred","WARNING: pattern is too small: %d < %d: do not update par file!", nal, patmin);
    return false;
  }

  TString parfileOUT;
  MakeAffName(parfileOUT,id1,id2);
  piece2.eFileNamePar = parfileOUT;
  ali.GetPattern(0)->GetKeep(aff);
  piece2.UpdateAffPar(0,aff);
  if( strcmp(opt,"-z") >=0 ) {
    ali.FineCorrZnew();
    piece2.UpdateZPar(0,-ali.GetPattern(0)->Z());
  }
 
  LogPrint(id1[0],1,"CorrectAffWithPred","from %d.%d.%d.%d to %d.%d.%d.%d: used %d (out of %d predictions) for correction", 
	   id1[0],id1[1],id1[2],id1[3],id2[0],id2[1],id2[2],id2[3],  nal, pat.N() );
  return true;
}

//----------------------------------------------------------------
int EdbScanProc::FindPredictions(int id[4], int flag, int maxholes)
{
  //find predictions of yp.yp.yp.yp.pred.root in yp.yp.yp.yp.cp.root and produce yp.yp.yp.yp.found.root
  // flag: -1 - all predictions
  //        0 - only found in the previous plate (no holes)
  //        1 - 1 holes before
  //        2 - 2 holes before
  //        3 - 3 holes before

  int nfound=0;
  EdbPattern pred, found;
  ReadPred(pred,id, flag);
  nfound =  FindPredictions(pred, id,found, maxholes);
  WriteFound(found,id);

  //EdbTestAl ta;
  //ta.HDistance(pred,found);
  //ta.FillTree(0);
  //EdbAffine2D aff;
  //ta.MakeTrans(aff,0,"(dx*dx+dy*dy)>0.00001&&abs(dx)<100&&abs(dy)<100");

  return nfound;
}

//----------------------------------------------------------------
int EdbScanProc::TestAl(int id1[4], int id2[4])
{
  MakeInPar(id1,"prealignment");
  MakeInPar(id2,"prealignment");
  TString name;
  MakeFileName(name,id1,"in.par");
  TString parfileOUT;
  MakeAffName(parfileOUT,id1,id2);
  parfileOUT.Prepend("INCLUDE ");
  AddParLine(name.Data(),	parfileOUT.Data());
  
  EdbPattern p1,p2;

  ReadPatCP(p2,id2);
  p2.SetZ(0);
  p2.SetSegmentsZ();

  ReadPatCP(p1,id1);
  EdbAffine2D aff1;
  float dz;
  GetAffZ(aff1,dz,id1,id2);
  p1.SetZ(-dz);
  p1.SetSegmentsZ();

  return TestAl(p1,p2);
}

//----------------------------------------------------------------
int EdbScanProc::TestAl(EdbPattern &p1, EdbPattern &p2)
{
  EdbTestAl ta;
  ta.HDistance(p1,p2);

  float bin[4]={20,20, 25,0.001};                  // default values for normal alignment (expected dz=1300)
  ta.eDmin[0]=-5000; ta.eDmin[1]=-5000; ta.eDmin[2]=  1200; ta.eDmin[3]=-0.015;
  ta.eDmax[0]= 5000; ta.eDmax[1]= 5000; ta.eDmax[2]=  1500; ta.eDmax[3]= 0.015;

  FILE *f = fopen("testal.par","r");
  if(f) {
    for(int i=0; i<4; i++) {
      float min=0,max=0,b=0;
      if(!(fscanf(f,"%f %f %f",&min,&max,&b )==3))  {Log(1,"EdbScanProc::TestAl","ERROR: read from testal.par"); return 0;}
      else { 
	ta.eDmin[i]=min; ta.eDmax[i]=max;  bin[i]=b; 
      }
    }
    fclose(f);
  }
  for(int i=0; i<4; i++) ta.eN[i] = (Int_t)((ta.eDmax[i]-ta.eDmin[i]-bin[i]/2.)/bin[i])+1;
  for(int i=0; i<4; i++) ta.eDmax[i] = ta.eDmin[i]+bin[i]*ta.eN[i];
  for(int i=0; i<4; i++) printf("%d \t%f %f %f %d\n",i, ta.eDmin[i],ta.eDmax[i],bin[i],ta.eN[i]);

  ta.CheckMaxBin();
  EdbAffine2D aff;
  aff.Rotate(-ta.eD0[3]);
  aff.ShiftX(ta.eD0[0]);
  aff.ShiftY(ta.eD0[1]);
  if(gEDBDEBUGLEVEL>1) aff.Print();
  //ta.FillTree( piece2.GetLayer(0)->Z()-piece1.GetLayer(0)->Z() );
  return 0;
}
  
//----------------------------------------------------------------
int EdbScanProc::AlignAll(int id1[4], int id2[4], int npre, int nfull, const char *opt)
{
  int nal=0;
  if(npre>0) {
    MakeInPar(id1,"prealignment");
    MakeInPar(id2,"prealignment");
    for(int i=0; i<npre; i++)
      nal=Align(id1,id2,"");	// find affine transformation from id1 to id2 and update par of id1
  }
  if(nfull>0) {
    MakeInPar(id1,"fullalignment");
    MakeInPar(id2,"fullalignment");
    for(int i=0; i<nfull; i++)
      nal=Align(id1,id2,opt);	// find affine transformation from id1 to id2 and update par of id1
  }
  LogPrint(id1[0],1,"AlignAll","%d.%d.%d.%d to %d.%d.%d.%d with %d pre and %d full.  The final pattern is: %d", 
	   id1[0],id1[1],id1[2],id1[3],id2[0],id2[1],id2[2],id2[3],npre,nfull,nal);
  return nal;
}

//----------------------------------------------------------------
int EdbScanProc::WritePatTXT(EdbPattern &pred, int id[4], const char *suffix, int flag)
{
  // write ascii predictions file as .../bXXXXXX/pYYY/a.a.a.a.suffix
  //        man      - for manual check by sysal
  //                 - overvise complete format
  EdbSegP *s=0;
  TString str;
  MakeFileName(str,id,suffix);
  FILE *f = fopen(str.Data(),"w");
  if(!f) { LogPrint(id[0],1,"WritePatTXT","ERROR! can not open file %s", str.Data()); return 0; }
  for(int i=0; i<pred.N(); i++) {
    s = pred.GetSegment(i);
    if(flag>-1) if(flag!=s->Flag())  continue;
    if( strcmp(suffix,"man") >=0 )
      fprintf(f,"%8d %11.2f %11.2f %8.4f %8.4f %d\n",
	      s->ID(),s->X(),s->Y(),s->TX(),s->TY(),s->Flag());
    else 
      fprintf(f,"%8d %11.2f %11.2f %8.4f %8.4f %9.2f %9.2f %8.4f %8.4f %d\n",
	      s->ID(),s->X(),s->Y(),s->TX(),s->TY(),s->SX(),s->SY(),s->STX(),s->STY(),s->Flag());
  }
  fclose(f);
  LogPrint(id[0],2,"WritePatTXT","%s with %d predictions with flag: %d", str.Data(),pred.N(), flag);
  return pred.N();
}

//----------------------------------------------------------------
int EdbScanProc::ReadPatTXT(EdbPattern &pred, int id[4], const char *suffix, int flag0)
{
  // read ascii predictions file as .../bXXXXXX/pYYY/a.a.a.a.suffix
  //        man      - for manual check by sysal
  //                 - overvise complete format
  EdbSegP s;
  int   ids=0,flag=0;
  float x=0,y=0,tx=0,ty=0,sx=0,sy=0,stx=0,sty=0;
  TString str;
  MakeFileName(str,id,suffix);
  FILE *f = fopen(str.Data(),"r");
  if(!f) { LogPrint(id[0],1,"ReadPatTXT","ERROR! can not open file %s", str.Data()); return 0; }
  char  buffer[256]="";
  int ic=0;
  for( int i=0; i<100000; i++ ) {
    if (fgets (buffer, 256, f) == NULL) break;
    if( strcmp(suffix,"man") >=0 ) {
      if( sscanf(buffer,"%d %f %f %f %f %d", 
		 &ids,&x,&y,&tx,&ty,&flag) !=6) break;
      if(flag0>-1) if(flag0 != flag) continue;
      s.Set(ids,x,y,tx,ty,50.,flag);
      s.SetErrors(50,50,0.,0.6,0.6);
    } else {
      if( sscanf(buffer,"%d %f %f %f %f %f %f %f %f %d", 
		 &ids,&x,&y,&tx,&ty,&sx,&sy,&stx,&sty,&flag) !=10) break;
      if(flag0>-1) if(flag0 != flag) continue;
      s.Set(ids,x,y,tx,ty,50.,flag);
      s.SetErrors(sx,sy,0.,stx,sty);
    }
    pred.AddSegment(s);		ic++;
  }
  fclose(f);
  LogPrint(id[0],2,"ReadPatTXT","%s with %d predictions with flag: %d", str.Data(),pred.N(),flag0);
  return ic;
}

//----------------------------------------------------------------
int EdbScanProc::WritePatRoot(EdbPattern &pred, int id[4], const char *suffix, int flag)
{
  // write root predictions file as .../bXXXXXX/pYYY/a.a.a.a.suffix
  int n = pred.N();
  TString str;
  MakeFileName(str,id,suffix);
  TFile f(str.Data(),"RECREATE");
  if(flag<0) pred.Write("pat");
  else {
    EdbPattern pat;
    pat.SetID(pred.ID());
    pat.SetPID(pred.PID());
    pat.SetX(pred.X());
    pat.SetY(pred.Y());
    pat.SetZ(pred.Z());
    for(int i=0; i<pred.N(); i++) 
      if(pred.GetSegment(i)->Flag()==flag) pat.AddSegment(*(pred.GetSegment(i)));
    n = pat.N();
    pat.Write("pat");
  }
  f.Close();
  LogPrint(id[0],2,"WritePatRoot","%s with %d predictions with flag: %d", str.Data(),n,flag);
  return n;
}

//----------------------------------------------------------------
int EdbScanProc::ReadPatRoot(EdbPattern &pred, int id[4], const char *suffix, int flag)
{
  // read root predictions file as .../bXXXXXX/pYYY/a.a.a.a.suffix
  int n=0;
  TString str;
  MakeFileName(str,id,suffix);
  TFile f(str.Data());
  EdbPattern *p=0;
  f.GetObject("pat",p);
  if(!p) { f.Close(); return 0;}
  for(int i=0; i<p->N(); i++) {
    if(flag>-1) if(flag != p->GetSegment(i)->Flag() ) continue;
    pred.AddSegment(*(p->GetSegment(i))); n++;
  }
  f.Close();
  p->Delete();
  LogPrint(id[0],2,"ReadPatRoot","%s with %d predictions with flag: %d", str.Data(),n,flag);
  return n;
}

//----------------------------------------------------------------
bool EdbScanProc::CheckProcDir(int id[4], bool create)
{
  //return true if dir ../bXXXXXX/pXXX exist, if create==true (default) create it if necessary 
  char str[256];
  void *dirp=0; // pointer to the directory
  sprintf(str,"%s/b%6.6d", eProcDirClient.Data(),id[0]);
  dirp = gSystem->OpenDirectory(str);
  if(!dirp) {
    if(create) gSystem->MakeDirectory(str);
    else return false;
  } else  { gSystem->FreeDirectory(dirp); dirp=0;}

  sprintf(str,"%s/b%6.6d/p%3.3d", eProcDirClient.Data(),id[0],id[1]);
  dirp = gSystem->OpenDirectory(str);
  if(!dirp) {
    if(create) gSystem->MakeDirectory(str);
    else return false;
  } else  { gSystem->FreeDirectory(dirp); dirp=0;}
  dirp = gSystem->OpenDirectory(str);
  if(!dirp) return false;
  gSystem->FreeDirectory(dirp);
  dirp=0;
  return true;
}

//----------------------------------------------------------------
void EdbScanProc::MakeFileName(TString &s, int ID[4], const char *suffix)
{
  //make full file pathname for id and add suffix
  char str[256];
  sprintf(str,"%s/b%6.6d/p%3.3d/%d.%d.%d.%d.%s",
	  eProcDirClient.Data(),ID[0], ID[1], ID[0], ID[1], ID[2], ID[3],suffix);
  s=str;
}

//----------------------------------------------------------------
void EdbScanProc::MakeAffName(TString &s, int id1[4], int id2[4], const char *suffix)
{
  //make affine file name as ../bXXXXXX/AFF/a.a.a.a_b.b.b.b.aff.par
  char str[256];
  sprintf(str,"%s/b%6.6d/AFF/%d.%d.%d.%d_%d.%d.%d.%d.%s",
	  eProcDirClient.Data(), id1[0],
	  id1[0], id1[1], id1[2], id1[3],
	  id2[0], id2[1], id2[2], id2[3],
	  suffix);
  s=str;
}

//----------------------------------------------------------------
bool EdbScanProc::GetMap(int brick, TString &map)
{
  // get map string from the map file of this brick : .../bXXXXXX/bXXXXXX.map
  char str[256];
  sprintf(str,"%s/b%6.6d/b%6.6d.map", eProcDirClient.Data(),brick,brick);
  LogPrint(brick,1,"GetMap"," from file: %s\n",str);
  FILE *f = fopen(str,"r");
  if(!f)     {  LogPrint(brick,1,"GetMap","no map file: %s !!!\n",str); return false; }
  else if (fgets (str, 256, f) == NULL) 
    {  LogPrint(brick,1,"GetMap","error reading map file: %s !!!\n",str); return false; }
  LogPrint(brick,1,"GetMap","%s\n",str);
  map=str;
  fclose(f);
  return true;
}

//----------------------------------------------------------------
EdbRun *EdbScanProc::InitRun(int id[4])
{
  // create new run file as eProcDirClient/bXXXXXX/pYYY/x.y.s.p.raw.root
  if(!CheckProcDir(id)) return 0;
  TString str;
  MakeFileName(str,id,"raw.root");   // the file will have the requested name 

  if( !gSystem->AccessPathName(str.Data(), kFileExists) ) {   // if the file with the same name exists it will be saved as *root.xxx.save
    TString str2;
    for(int ic=0; ic<1000; ic++) {
      str2 = str; str2+="."; str2+=ic; str2+=".save";
      if( !gSystem->AccessPathName(str2.Data(), kFileExists) ) continue;
      else                                                     break;
    }
    gSystem->CopyFile(str.Data(), str2.Data());
    LogPrint(id[0], 3,"EdbScanProc::InitRun"," %s\n",str2.Data());
  }
  LogPrint(id[0], 3,"EdbScanProc::InitRun"," %s\n",str.Data());
  return new EdbRun(str.Data(),"RECREATE");
}

//-------------------------------------------------------------------
bool EdbScanProc::AddParLine(const char *file, const char *line)
{
  // add string to par file
  FILE *f = fopen(file,"a+");
  if(!f) return false;
  fprintf(f,"%s\n",line);
  fclose(f);
  return true;
}

//-------------------------------------------------------------------
bool EdbScanProc::MakeInPar(int id[4], const char *option)
{
  // prepare x.x.x.x.in.par file for the process defined in the option string
  TString name;
  MakeFileName(name,id,"in.par");
  FILE *f = fopen(name.Data(),"w");
  if(!f) {
    LogPrint(id[0],2,"MakeInPar","ERROR! can't open file: %s",name.Data() );
    return false;
  }
  fprintf(f,"INCLUDE %s/parset/opera_emulsion.par\n",eProcDirClient.Data());
  fprintf(f,"INCLUDE %s/parset/%s.par\n",eProcDirClient.Data(),option);
  TString nm;
  MakeFileName(nm,id,"par");
  fprintf(f,"INCLUDE %s\n",nm.Data());
  fclose(f);
  return true;
}

//-------------------------------------------------------------------
int EdbScanProc::CorrectAngles(int id[4])
{
  EdbDataPiece piece;
  InitPiece(piece,id);
  TString parfileOUT;
  MakeFileName(parfileOUT,id,"par");
  piece.eFileNamePar = parfileOUT;
  return piece.CorrectAngles();
}

//-------------------------------------------------------------------
int EdbScanProc::LinkRun(int id[4], int noUpdate)
{
  // the x.x.x.x.in.par file must be prepared before
  EdbDataPiece piece;
  InitPiece(piece,id);
  EdbDataProc  proc;
  proc.SetNoUpdate(noUpdate);
  TString parfileOUT;
  MakeFileName(parfileOUT,id,"par");
  piece.eFileNamePar = parfileOUT;
  return proc.Link(piece);
}

//----------------------------------------------------------------------------------------
int EdbScanProc::FindPredictionsRaw(int idp[4], int idr[4])
{
  // find raw microtracks for the predictions of idp in raw data of idr
  // Input:  idp.pred.root, idr.raw.root
  // Output: idp.found.root, idp.found.raw.txt

  EdbPattern pred;
  EdbPattern found;
  ReadPred(pred,idp);
  EdbRunAccess ra;
  InitRunAccess(ra,idr);

  EdbScanCond condBT;
  EdbScanCond condMT;
  SetDefaultCondBT(condBT);
  SetDefaultCondMT(condMT);
  float delta_theta = 0.1;
  float puls_min    = 7;
  float chi2max     = 1.6;
  int nfound = FindPredictionsRaw(pred,found,ra, condBT,condMT, delta_theta, puls_min, chi2max);

  return nfound;
}

//----------------------------------------------------------------------------------------
int EdbScanProc::FindPredictionsRaw( EdbPattern &pred, EdbPattern &fnd, EdbRunAccess &ra, 
				     EdbScanCond &condBT, EdbScanCond &condMT, 
				     float delta_theta, float puls_min, float puls_mt, float chi2max, FILE *out )
{
  // find raw microtracks for the predictions "pred" in run "ra"
  // Input:  pred,ra
  // Output: fnd - basetracks with position taken from the best microtrack (if any) and the angle of the predicted basetrack

  Log(2,"FindPredictionsRaw"," search for %d predictions \n", pred.N());

  TFile ftree("micro.root","RECREATE");
  EdbSegP      *s_b    = new EdbSegP();
  EdbSegP      *sf_b  = new EdbSegP();
  TClonesArray *pat1_b = new TClonesArray("EdbSegP");
  TClonesArray *pat2_b = new TClonesArray("EdbSegP");
  TTree micro("micro","micro");
  micro.Branch("s.","EdbSegP",&s_b,32000,99);
  micro.Branch("sf.","EdbSegP",&sf_b,32000,99);
  micro.Branch("s1.",&pat1_b,32000,99);
  micro.Branch("s2.",&pat2_b,32000,99);

  if(!out) out = fopen("micro.txt","w");
  if(!out) return 0;
  if(gEDBDEBUGLEVEL>2) ra.Print();
  
  for(int i=0; i<pred.N(); i++) {
    ra.ClearCuts();
    EdbSegP s;
    s.Copy( *(pred.GetSegment(i)) );
    s.SetZ(107.);                                      // TODO!
    s.SetErrors();
    float xmin[5]={-500, -500, s.TX()-delta_theta, s.TY()-delta_theta,  puls_min };         //TODO!!
    float xmax[5]={ 500,  500, s.TX()+delta_theta, s.TY()+delta_theta,  50       };
    condBT.FillErrorsCov( s.TX(), s.TY(), s.COV() );
    fprintf(out,"\n%8.8d   %11.2f %11.2f %7.4f %7.4f\n",s.ID(),s.X(),s.Y(),s.TX(),s.TY());

    EdbPVRec aview; //with 2 patterns of preselected microtracks
    aview.AddPattern( new EdbPattern(0,0,214));  // TODO! sequence??
    aview.AddPattern( new EdbPattern(0,0,0)  );

    for(int side=1; side<=2; side++) {
      Log(3,"FindPredictionsRaw","side = %d\n",side);
      EdbPattern pat;
      ra.AddSegmentCut(side,1,xmin,xmax);
      ra.GetPatternXY( s, side,  pat );

      for(int i=0; i<pat.N(); i++) {
	EdbSegP *ss = pat.GetSegment(i);
	ss->SetErrors();
	condMT.FillErrorsCov( s.TX(), s.TY(), ss->COV() );
      }
      pat.FillCell(10,10,0.01,0.01);  //divide view on this cells
      
      TArrayF   chi2arr(1000);  //TODO!
      TObjArray found;
      int nf= FindCompliments(s,pat,found, chi2max, chi2arr);
      for(int j=0; j<found.GetEntries(); j++) {
	EdbSegP *s2 = (EdbSegP *)(found.At(j));
	s2->SetChi2(chi2arr[j]);                        // TODO -???
	aview.GetPattern(side-1)->AddSegment(*s2);
	float offx=s2->X()-(s.X()+s.TX()*(s2->Z()-s.Z()));
	float offy=s2->Y()-(s.Y()+s.TY()*(s2->Z()-s.Z()));
	fprintf(out,"s%1d(%2d)%4d %11.2f %11.2f %7.4f %7.4f %6.1f %7.2f %7.2f %7.4f %7.4f %6.3f %3.0f\n",
		side,nf,s2->ID(),s2->X(),s2->Y(),s2->TX(),s2->TY(),s2->Z(),
		offx,offy,
		s2->TX()-s.TX(),s2->TY()-s.TY(),chi2arr[j],s2->W());
      }
    }

    EdbSegP sfmt;      // container for the found best microtrack passed the cuts
    sfmt.SetChi2(10.*chi2max);

    float rlim   = 20;        // TODO
    float chi, chimin = 1.5;  // TODO  for bt selection
    EdbSegP *s1b=0, *s2b=0;   // the best bt
    EdbSegP s3;

    for(int is1=0; is1<aview.GetPattern(0)->N(); is1++) {
      EdbSegP *s1 = aview.GetPattern(0)->GetSegment(is1);
      if( sfmt.Chi2() > s1->Chi2() && s1->W() >= puls_mt ) {sfmt.Copy(*s1); sfmt.SetFlag(1);}
    }
    for(int is2=0; is2<aview.GetPattern(1)->N(); is2++) {
      EdbSegP *s2 = aview.GetPattern(1)->GetSegment(is2);
      if( sfmt.Chi2() > s2->Chi2() && s2->W() >= puls_mt ) {sfmt.Copy(*s2); sfmt.SetFlag(2);}
    }
    
    for(int is1=0; is1<aview.GetPattern(0)->N(); is1++) {
      for(int is2=0; is2<aview.GetPattern(1)->N(); is2++) {
	EdbSegP *s1 = aview.GetPattern(0)->GetSegment(is1);
	EdbSegP *s2 = aview.GetPattern(1)->GetSegment(is2);

	float dx1=s1->X()-(s.X()+s.TX()*(s1->Z()-s.Z()));
	float dy1=s1->Y()-(s.Y()+s.TY()*(s1->Z()-s.Z()));
	float dx2=s2->X()-(s.X()+s.TX()*(s2->Z()-s.Z()));
	float dy2=s2->Y()-(s.Y()+s.TY()*(s2->Z()-s.Z()));
	float r = Sqrt( (dx1-dx2)*(dx1-dx2) + (dy1-dy2)*(dy1-dy2) ); 
	fprintf(out,"r = %7.2f  ",r);
	if(r<rlim) {  // has good BT
	  s3.Copy(s);
	  s3.SetX( 0.5*(s1->X() + s2->X()) );
	  s3.SetY( 0.5*(s1->Y() + s2->Y()) );
	  s3.SetZ( 0.5*(s1->Z() + s2->Z()) );
	  s3.SetTX( (s2->X() - s1->X()) / (s2->Z() - s1->Z()) );
	  s3.SetTY( (s2->Y() - s1->Y()) / (s2->Z() - s1->Z()) );
	  s3.SetFlag(0);

	  //s3.Print();
	  //s.Print();
	  chi = EdbTrackFitter::Chi2Seg(&s3, &s);
	  printf("chi = %7.4f\n",chi);
	  fprintf(out,"chi = %7.4f\n",chi);
	  if(chi<chimin) {                           //select the best basetrack
	    chimin = chi;
	    s1b = s1;
	    s2b = s2;
	  }
	}
      }
    }

    EdbSegP sf;                // container for the found track

    int bth, mth, tb; // basetrack holes, mt-holes, top/bottom;

    sf.Copy(s);
    if(s1b&&s2b) {
      sf.SetX( 0.5*(s1b->X() + s2b->X()) );
      sf.SetY( 0.5*(s1b->Y() + s2b->Y()) );
      sf.SetZ( 0.5*(s1b->Z() + s2b->Z()) );
      sf.SetTX( (s2b->X() - s1b->X()) / (s2b->Z() - s1b->Z()) );
      sf.SetTY( (s2b->Y() - s1b->Y()) / (s2b->Z() - s1b->Z()) );
      sf.SetFlag(0);                                              // if bt found : bth=0, mth=0, tb=0
      sf.SetChi2(chimin);
    } else if(sfmt.Chi2()<chi2max) {  // found good microtrack
      //float zmean = ra.GetLayer(0)->Z();
      float zmean = s.Z();
      printf("Zmt = %f  zmean = %f   Zs = %f \n",sfmt.Z(),zmean,s.Z());
      sf.SetX( sfmt.X() + s.TX()*(zmean-sfmt.Z()) );
      sf.SetY( sfmt.Y() + s.TY()*(zmean-sfmt.Z()) );
      sf.SetZ(zmean);
      bth = s.Flag()/10000;  bth++;
      mth = 0;
      tb = sfmt.Flag();
      sf.SetFlag(bth*10000+mth*100+tb);                   // if mt found : bth++, mth=0, tb=1/2
    } else {
      bth = s.Flag()/10000;      bth++;
      mth = (s.Flag()/100)%100;  mth++;
      tb  =  s.Flag()%10;
      sf.SetFlag(bth*10000+mth*100+tb);         // hole: if not found: bth++, mth++, tb= keep last value
    }

    s_b->Copy(s);
    sf_b->Copy(sf);
    pat1_b = aview.GetPattern(0)->GetSegments();
    pat2_b = aview.GetPattern(1)->GetSegments();
    micro.SetBranchAddress("s1."  , &pat1_b );
    micro.SetBranchAddress("s2."  , &pat2_b );
    micro.Fill();

    fnd.AddSegment(sf);
  }
  
  //ftree.cd();
  micro.Write();
  ftree.Close();
  fclose(out);
  return 1; //TODO!
}

//----------------------------------------------------------------------------------------
int EdbScanProc::FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr )
{
  // return found sorted by increasing chi2

  int nfound=0;
  int maxcand=chiarr.GetSize();
  TArrayF   chi2arr(maxcand);
  TObjArray arr(maxcand);
  TArrayI   ind(maxcand);
  
  int nseg = pat.FindCompliments(s,arr,30,200);  // acceptance (prelim): s.SX()*30; s.STX*200
  printf("\nnseg = %d\n",nseg);
  if(nseg>maxcand)  {
    printf("Warning!: Too many segments %d, accept only the first %d \n", nseg, maxcand);
    nseg = maxcand;
  }
  if(nseg<=0) return 0;

  EdbSegP *s2=0;
  for(int j=0; j<nseg; j++) {
    s2 = (EdbSegP *)arr.At(j);
    EdbSegP s3;
    s3.Copy(s);
    chi2arr[j] = EdbTrackFitter::Chi2Seg(&s3, s2);
  }
  TMath::Sort(nseg,chi2arr.GetArray(),ind.GetArray(),0);
  for(int j=0; j<nseg; j++) {
    s2 = (EdbSegP *)arr.At(ind[j]);
    if(chi2arr[ind[j]] > chi2max ) break;
    chiarr[j] = chi2arr[ind[j]];
    s2->SetMC(s.MCEvt(),s.MCTrack());
    found.Add(s2);
    nfound++;
  }

  printf("nfound = %d\n",nfound);
  return nfound;
}

//----------------------------------------------------------------------------------------
void EdbScanProc::SetDefaultCondBT(EdbScanCond &cond)
{
  cond.SetSigma0( 10., 10., 0.007, 0.007 );   // sigma0 "x, y, tx, ty" at zero angle
  cond.SetDegrad( 5. );                       // sigma(tx) = sigma0*(1+degrad*tx)
  cond.SetBins(0, 0, 0, 0); //???                  // bins in [sigma] for checks
  cond.SetPulsRamp0(  5., 5. );               // in range (Pmin:Pmax) Signal/All is nearly linear
  cond.SetPulsRamp04( 5., 5. );
  cond.SetChi2Max( 6.5 );
  cond.SetChi2PMax( 6.5 );
  cond.SetRadX0( 5810. );
  cond.SetName("OPERA_basetrack");
}

//----------------------------------------------------------------------------------------
void EdbScanProc::SetDefaultCondMT(EdbScanCond &cond)
{
  cond.SetSigma0( 1., 1., 0.025, 0.025 );   // sigma0 "x, y, tx, ty" at zero angle
  cond.SetDegrad( 5. );                       // sigma(tx) = sigma0*(1+degrad*tx)
  cond.SetBins(0, 0, 0, 0);  //???                 // bins in [sigma] for checks
  cond.SetPulsRamp0(  5., 5. );               // in range (Pmin:Pmax) Signal/All is nearly linear
  cond.SetPulsRamp04( 5., 5. );
  cond.SetChi2Max( 6.5 );
  cond.SetChi2PMax( 6.5 );
  cond.SetRadX0( 5810. );
  cond.SetName("OPERA_microtrack");
}

//-------------------------------------------------------------------
bool EdbScanProc::InitRunAccess(EdbRunAccess &ra, int id[4], bool do_update)
{
  // initialize the EdbRunAccess object useful for the raw data handling
  EdbDataPiece p;
  if(!InitPiece(p,id)) return false;
  if(gEDBDEBUGLEVEL>2) p.Print();
  ra.eAFID = p.eAFID;
  LogPrint(id[0],2,"InitRunAccess","open file %s",p.GetRunFile(0));
  if( !ra.InitRun(p.GetRunFile(0), do_update) ) {
    LogPrint(id[0],1,"InitRunAccess","ERROR open file %s !!!",p.GetRunFile(0));
    return false;
  }
  ra.GetLayer(1)->SetZlayer( p.GetLayer(1)->Z(),p.GetLayer(1)->Zmin(),p.GetLayer(1)->Zmax());
  ra.GetLayer(2)->SetZlayer( p.GetLayer(2)->Z(),p.GetLayer(2)->Zmin(),p.GetLayer(2)->Zmax());
  ra.GetLayer(1)->SetShrinkage( p.GetLayer(1)->Shr());
  ra.GetLayer(2)->SetShrinkage( p.GetLayer(2)->Shr());
  return true;
}

//-------------------------------------------------------------------
bool EdbScanProc::InitPiece(EdbDataPiece &piece, int id[4])
{
  // set raw, cp and par for the piece according to id
  TString runfile, cpfile, parfile;
  MakeFileName(runfile,id,"raw.root");
  MakeFileName(cpfile,id,"cp.root");
  MakeFileName(parfile,id,"in.par");
  piece.AddRunFile(runfile);
  piece.eFileNameCP  = cpfile;
  piece.eFileNamePar = parfile;
  if(piece.TakePiecePar()<0) return false;
  return true;
}

//-------------------------------------------------------------------
int EdbScanProc::ReadPatCP(EdbPattern &pat, int id[4], TCut cut)
{
  // read CP file ("base" segments) applying all cuts and transformations from x.x.x.x.in.par
  // the Z of the pat and all segments will be z of layer 0 defined in the par file(s)
  EdbDataPiece piece;
  InitPiece(piece, id);
  piece.GetLayer(0)->SetZlayer(piece.GetLayer(0)->Z(), 0,0);
  piece.AddRCut(0,cut);
  int n = ReadPiece(piece, pat);
  pat.SetZ(piece.GetLayer(0)->Z());
  pat.SetSegmentsZ();
  return n;
}

//-------------------------------------------------------------------
int EdbScanProc::ReadPiece(EdbDataPiece &piece, EdbPattern &pat)
{
  //assuming that piece was initialised correctly
  if(!piece.InitCouplesTree("READ")) return 0;
  piece.GetCPData_new( &pat,0,0,0 );
  pat.SetSegmentsZ();
  pat.Transform(    piece.GetLayer(0)->GetAffineXY()   );
  pat.TransformA(   piece.GetLayer(0)->GetAffineTXTY() );
  pat.TransformShr( piece.GetLayer(0)->Shr()  );
  return 1;
}

//-------------------------------------------------------------------
int EdbScanProc::FindPredictions(EdbPattern &pred, int id[4], EdbPattern &found, int maxholes)
{
  // find predictions pred in couples tree of id and prepare pattern "found"
  // assumed that pred are transformed and projected into the coord system of id
  // Input:   pred - pattern with predictions
  //            id - the data piece to be processed
  //      maxholes - the maximum number of holes (missed segments) for doing extrapolation
  // Output: found - pattern with found tracks
  //         x.x.x.x.found.txt summary file with all candidats

  EdbPVRec ali;
  EdbPattern *pat=0;
  EdbDataPiece piece;

  // predicted:
  pat = new EdbPattern(pred.X(),pred.Y(),0,100);
  for(int i=0; i<pred.N(); i++) pat->AddSegment(*(pred.GetSegment(i)));
  pat->SetPID(0);
  pat->SetSegmentsZ();      // z=0  (the same)
  ali.AddPattern(pat);

  // scanned:
  InitPiece(piece, id);
  EdbPattern *patbt = new EdbPattern(0.,0., 0,100 );
  EdbPattern *pat1  = new EdbPattern(0.,0., 0,100 );
  EdbPattern *pat2  = new EdbPattern(0.,0., 0,100 );

  if(!piece.InitCouplesTree("READ")) return 0;
  piece.GetCPData_new( patbt,pat1,pat2,0 );
  patbt->SetSegmentsZ();
  patbt->Transform(    piece.GetLayer(0)->GetAffineXY()   );
  patbt->TransformA(   piece.GetLayer(0)->GetAffineTXTY() );
  patbt->TransformShr( piece.GetLayer(0)->Shr()  );
  pat1->SetSegmentsZ();
  pat1->Transform(    piece.GetLayer(0)->GetAffineXY()   );
  pat1->TransformA(   piece.GetLayer(0)->GetAffineTXTY() );
  pat2->SetSegmentsZ();
  pat2->Transform(    piece.GetLayer(0)->GetAffineXY()   );
  pat2->TransformA(   piece.GetLayer(0)->GetAffineTXTY() );

  //ReadPiece(piece, *pat);
  patbt->SetSegmentsZ();      // z=0 (the same)
  patbt->SetPID(1);
  ali.AddPattern(patbt);

  EdbScanCond *cond = piece.GetCond(0);
  cond->SetChi2Mode(3);
  ali.SetScanCond( cond );
  ali.SetPatternsID();
  //ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(cond->Chi2PMax());

  EdbSegP *s=0;
  for(int ip=0; ip<ali.Npatterns(); ip++)
    for(int i=0; i<ali.GetPattern(ip)->N(); i++) {
      s = ali.GetPattern(ip)->GetSegment(i);
      s->SetErrors();
      cond->FillErrorsCov( s->TX(), s->TY(), s->COV() );
    }

  TString str;
  MakeFileName(str,id,"found.txt");
  FILE *f = fopen(str.Data(),"w");

  TString strmt;
  MakeFileName(strmt,id,"found.mt.txt");
  FILE *fmt = fopen(strmt.Data(),"w");

  int maxcand=100;
  TArrayF chiarr(maxcand);
  TArrayI ind(maxcand);
  TArrayI count(maxcand);
  TArrayI cnsel(maxcand);

  pat = ali.GetPattern(1);
  pat->FillCell(20,20,0.01,0.01);
  int nseg=0;
  TObjArray arr;
  EdbSegP *s2=0;
  EdbSegP s3;
  for(int i=0; i<pred.N(); i++) {
    s = ali.GetPattern(0)->GetSegment(i);
    arr.Clear();
    nseg = pat->FindCompliments(*s,arr,cond->BinX(),cond->BinTX());
    if(nseg>maxcand)       continue;
    count[nseg]++;
    int nsel=0;
    if(nseg>=0) {
      for(int j=0; j<nseg; j++) {
	s2 = (EdbSegP *)arr.At(j);
	s3.Copy(*s2);
	chiarr[j] = EdbTrackFitter::Chi2Seg(&s3, s);
      }
      TMath::Sort(nseg,chiarr.GetArray(),ind.GetArray(),0);
      for(int j=0; j<nseg; j++) {
	s2 = (EdbSegP *)arr.At(ind[j]);
	if(chiarr[ind[j]] > cond->Chi2PMax() ) break;
	nsel=j+1;
	s2->SetMC(s->MCEvt(),s->MCTrack());
      }

      fprintf(f,"\n%8.8d %11.2f %11.2f %8.4f %8.4f %d\n",
	      s->ID(),s->X(),s->Y(),s->TX(),s->TY(), nsel);
      fprintf(fmt,"\n%8.8d %11.2f %11.2f %8.4f %8.4f %d\n",
	      s->ID(),s->X(),s->Y(),s->TX(),s->TY(), nsel);
      for(int j=0; j<nsel; j++) {
	s2 = (EdbSegP *)arr.At(ind[j]);
	fprintf(f,"%8d %11.2f %11.2f %8.4f %8.4f %6.2f %3.0f\n",
		j+1,s2->X(),s2->Y(),s2->TX(),s2->TY(),chiarr[ind[j]],s2->W());
	fprintf(fmt,"%8d %11.2f %11.2f %8.4f %8.4f %6.2f %3.0f\n",
		j+1,s2->X(),s2->Y(),s2->TX(),s2->TY(),chiarr[ind[j]],s2->W());
	int imt=-1;
	if(patbt->GetSegments()->FindObject(s2)) imt = patbt->GetSegments()->IndexOf(s2);
	if(imt>0) {
	  EdbSegP *smt = pat1->GetSegment(imt);
	  fprintf(fmt,"s1:%5d %11.2f %11.2f %8.4f %8.4f %3.0f\n",
		  j+1,smt->X(),smt->Y(),smt->TX(),smt->TY(),smt->W());
	  smt = pat2->GetSegment(imt);
	  fprintf(fmt,"s2:%5d %11.2f %11.2f %8.4f %8.4f %3.0f\n",
		  j+1,smt->X(),smt->Y(),smt->TX(),smt->TY(),smt->W());
	}
      }

    }
    cnsel[nsel]++;
    if(nsel>0) {
      found.AddSegment(*((EdbSegP *)arr.At(ind[0])));   // add the best segment
      found.GetSegmentLast()->SetFlag(0);               // reset flag if found good candidate
      found.GetSegmentLast()->SetID(s->ID());
    }
    else if(s->Flag()<maxholes) {
      found.AddSegment(*(s));                       // add itself in case of hole
      found.GetSegmentLast()->SetFlag(s->Flag()+1); // flag is the number of missed plates // OLD FLAG DEFINITION
      found.GetSegmentLast()->SetID(s->ID());
    }
  }
  fclose(f);
  fclose(fmt);
  delete pat1; pat1=0;
  delete pat2; pat2=0;

  printf("Total: %d predictions, %d basetracks in scanned pattern\n",pred.N(), pat->N() );
  int sum=0;
  printf("Before chi2 cut: \n" );
  for(int i=0; i<maxcand; i++) 
    if(count[i]>0) {
      printf("count(%5d)= %5d\n",i, count[i] );
      sum+=count[i];
    }
  printf("sum = %d\n",sum );
  sum=0;
  printf("After chi2 cut: \n" );
  for(int i=0; i<maxcand; i++) 
    if(cnsel[i]>0) {
      printf("cnsel(%5d)= %5d\n",i, cnsel[i] );
      sum+=cnsel[i];
    }
  printf("sum = %d\n",sum );

  LogPrint(id[0],1,"FindPredictions","%d.%d.%d.%d:  %d out of %d predictions are found (%d-zero, %d-single, %d-multy),  maxholes=%d", 
	   id[0],id[1],id[2],id[3],sum-cnsel[0],pred.N(),cnsel[0],cnsel[1], sum-cnsel[0]-cnsel[1], maxholes);

  return sum-cnsel[0];
}

//-------------------------------------------------------------------
bool  EdbScanProc::GetAffZ(EdbAffine2D &aff, float &dz, int id1[4],int id2[4])
{
  // read affine transformations and deltaZ from x.x.x.x_y.y.y.y.aff.par
  TString parfile;
  MakeAffName(parfile,id1,id2);
  EdbDataPiece piece;
  piece.eFileNamePar = parfile;
  piece.TakePiecePar();
  EdbAffine2D *a = piece.GetLayer(0)->GetAffineXY();
  if(!a) return false;
  aff.Set( a->A11(),a->A12(),a->A21(),a->A22(),a->B1(),a->B2() );
  dz = piece.GetLayer(0)->Z();
  return true;
}

//-------------------------------------------------------------------
bool  EdbScanProc::ApplyAffZ(EdbPattern &pat,int id1[4],int id2[4])
{
  // read affine transformations and deltaZ from x.x.x.x_y.y.y.y.aff.par and apply it to pat
  EdbAffine2D aff;
  float       dz;
  if( !GetAffZ( aff, dz, id1, id2) ) return false;
  printf("ApplyAffZ: dz = %f\n",dz);
  aff.Print();
  pat.ProjectTo(dz);
  pat.Transform(&aff);
  return true;
}

//-------------------------------------------------------------------
int EdbScanProc::Align(int id1[4], int id2[4], const char *option)
{
  // Align 2 patterns, assumed that already exists file x.x.x.x_y.y.y.y.aff.par with deltaZ inside.
  // Convension about Z(setted while process): the z of id2 is 0, the z of id1 is (-deltaZ) where
  // deltaZ readed from aff.par file in a way that pattern of id1 projected 
  // to deltaZ correspond to pattern of id2

  int npat=0;
  TString name;
  MakeFileName(name,id1,"in.par");
  TString parfileOUT;
  MakeAffName(parfileOUT,id1,id2);
  parfileOUT.Prepend("INCLUDE ");
  AddParLine(name.Data(),	parfileOUT.Data());
  
  EdbPVRec ali;
  EdbPattern *pat=0;
  EdbDataPiece piece1,piece2;
  
  InitPiece(piece1, id1);
  piece1.GetLayer(0)->SetZlayer(-1*piece1.GetLayer(0)->Z(), 0,0);
  pat = new EdbPattern(0.,0., piece1.GetLayer(0)->Z(),100 );
  ReadPiece(piece1, *pat);
  pat->SetPID(0);
  pat->SetSegmentsZ();
  ali.AddPattern(pat);
  
  InitPiece(piece2, id2);
  piece2.GetLayer(0)->SetZlayer(0, 0,0);
  pat = new EdbPattern(0.,0., piece2.GetLayer(0)->Z(),100 );
  ReadPiece(piece2, *pat);
  pat->SetPID(1);
  pat->SetSegmentsZ();
  ali.AddPattern(pat);

  printf("EdbScanProc::Align: Z1 = %f z2 = %f\n",piece1.GetLayer(0)->Z(),piece2.GetLayer(0)->Z());
  
  EdbScanCond *cond = piece1.GetCond(0);
  cond->SetChi2Mode(3);
  ali.SetScanCond( cond );
  ali.SetPatternsID();
  ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(cond->Chi2PMax());
  ali.SetOffsetsMax(cond->OffX(),cond->OffY());
  
  if( strstr( option,"-a")> 0 && strstr( option,"-a2")==0 )
    ali.Align(0);                                              //GS: 25-07-07
  else
    ali.Align(2); 
//ali.Align(2);
  ali.PrintAff();
  npat = ali.GetCouple(0)->Ncouples();

  MakeAffName(parfileOUT,id1,id2);
  piece1.eFileNamePar = parfileOUT;
  EdbAffine2D  aff;
  ali.GetPattern(0)->GetKeep(aff);
  piece1.UpdateAffPar(0,aff);
  
  TString  cpfile;
  MakeFileName(cpfile,id1,"al.cp.root");
  EdbDataProc proc;
  proc.LinkTracksWithFlag( &ali, 10., 0.05, 2, 3, 0 );
  TTree *cptree=EdbDataPiece::InitCouplesTree(cpfile.Data(),"RECREATE");
  proc.FillCouplesTree(cptree, &ali,0);
  proc.CloseCouplesTree(cptree);
  
  //ali.FitTracks( 10, 0.139 );         // is important to call it before MakeTracksTree!
  //MakeFileName(cpfile,id1,"al.tr.root");
  //proc.MakeTracksTree(&ali,cpfile.Data());
  if( strstr(option,"-z") ) {
    ali.FineCorrZnew();
    piece1.UpdateZPar(0,-ali.GetPattern(0)->Z());
  }
  
  return npat;
 }

//______________________________________________________________________________
void EdbScanProc::LogPrint(int brick, int level, const char *location, const char *va_(fmt), ...)
{
// Print message to the logfile and to stdout.
  if(gEDBLOGFILE) {
    printf("WARNING in LogPrint! logfile seems to be opened. Trying to close it...\n");
    fclose(gEDBLOGFILE);
    gEDBLOGFILE=0;
  }
  char str[512];
  sprintf(str,"%s/b%6.6d/b%6.6d.log", eProcDirClient.Data(), brick,brick);
  gEDBLOGFILE = fopen(str,"a");
  if(!gEDBLOGFILE) printf("ERROR in LogPrint! can not open logfile: %s\n",str);
  
  va_list ap;
  va_start(ap,va_(fmt));
  Log0(level, location, va_(fmt), ap);
  va_end(ap);

  if(gEDBLOGFILE) fclose(gEDBLOGFILE);
  gEDBLOGFILE=0;
}
