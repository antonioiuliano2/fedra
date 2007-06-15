//-- Author :  Valeri Tioukov   22/12/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanProc                                                          //
//                                                                      //
// scanned data processing library                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "snprintf.h"
#include "TDatime.h"
#include "TSystem.h"
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
  a->Print();

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
  
  LogPrint(idIN[0],"PrepareVolumesPred","%d.%d.%d.%d: for %d  volumes generated %d predictions with settings: before=%d after=%d pmin=%d pmax=%d\n",
	   idIN[0],idIN[1],idIN[2],idIN[3],points.N(), count, before,after,pmin,pmax );
}

//----------------------------------------------------------------
bool EdbScanProc::FlashRawDir(EdbScanClient &scan, int id[4])
{
  // move all rwc and rwd files from the raw scanning directory into the new subdir

  // todo: Linux only yet!

  char str[256];
  TDatime dt;
  sprintf(str,"%s/rw_%u",scan.eRawDirClient.Data(),dt.Get());
  LogPrint(id[0],"FlashRawDir","%d.%d.%d.%d: move all into %s", id[0],id[1],id[2],id[3],str);
  if(!gSystem->OpenDirectory(str))   
    if( gSystem->MakeDirectory(str) == -1) 
      {
	LogPrint(id[0],"FlashRawDir","WARNING! %d.%d.%d.%d: FAILED creating directory %s", 
		 id[0],id[1],id[2],id[3],str);
	return false;
      }
  char str2[256];
  sprintf(str2,"mv %s/raw.* %s",scan.eRawDirClient.Data(),str);
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
    LogPrint(id[0],"LoadPlate","ERROR: map file does not exist! Stop here. *** %d.%d.%d  status = %d ***",
	     id[0],id[1],id[2],status);
    return status;
  }
  status= scan.LoadPlate(id[0],id[1],map.Data(),attempts);
  LogPrint(id[0],"LoadPlate","******************** %d.%d.%d  status = %d ************************************",
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
  LogPrint(brick,"RemoveDublets","%d segments before -> %d segments after: %d dublets removed", n, pout.N(), n-pout.N());
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
      LogPrint(brick,"OptimizeScanPath","with %d  predictions: gain in path[mm] before/after = %.1f/%.1f = %.1f",
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
  LogPrint(id[0],"ConvertAreas","%d.%d.%d.%d  with %d predictions with flag %d; %d views stored", 
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
  LogPrint(id[0],"ScanAreas","%d.%d.%d.%d  with %d predictions with flag %d", id[0],id[1],id[2],id[3],pred.N(),flag);
  EdbPattern predopt;
  OptimizeScanPath(pred,predopt,id[0]);
  EdbRun *run = InitRun(id);
  if(!run) return 0;
  int scanned = scan.ScanAreas(id,predopt,*run,opt);
  LogPrint(id[0],"ScanAreas","%d.%d.%d.%d  %d predictions scanned; run with %d views stored", id[0],id[1],id[2],id[3],scanned, run->GetEntries() );
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
  LogPrint(id1[0],"CopyFile","from %s to %s", name1.Data(),name2.Data() );
  return gSystem->CopyFile(name1.Data(), name2.Data(), overwrite);
}

//----------------------------------------------------------------
int EdbScanProc::CopyAFFPar(int id1c[4], int id2c[4], int id1p[4], int id2p[4], bool overwrite)
{
  // copy AFF/xc_yc.par to AFF/xp_yp.par
  TString name1, name2;
  MakeAffName(name1,id1c,id2c);
  MakeAffName(name2,id1p,id2p);
  LogPrint(id1c[0],"CopyAFFPar","from %s to %s", name1.Data(),name2.Data());
  return gSystem->CopyFile(name1.Data(), name2.Data(), overwrite);
}

//----------------------------------------------------------------
bool EdbScanProc::SetAFF0(int id1[4], int id2[4])
{
  TString str;
  MakeAffName(str,id1,id2);
  char card[64];
  sprintf(card,"AFFXY 0 1. 0. 0. 1. 0. 0.");
  LogPrint(id1[0],"SetAFF0","%s as %s", str.Data(),card);
  return AddParLine(str.Data(),card);
}

//----------------------------------------------------------------
bool EdbScanProc::SetAFFDZ(int id1[4], int id2[4], float dz)
{
  TString str;
  MakeAffName(str,id1,id2);
  char card[64];
  sprintf(card,"ZLAYER 0 %f 0 0",dz);
  LogPrint(id1[0],"SetAFFDZ","%s as %s", str.Data(),card);
  return AddParLine(str.Data(),card);
}

//----------------------------------------------------------------
int EdbScanProc::LinkRunAll(int id[4], int npre, int nfull )
{
  LogPrint(id[0],"LinkRunAll","%d.%d.%d.%d  %d prelinking + %d fullinking", id[0],id[1],id[2],id[3],npre,nfull);
  int nc=0;
  if(npre>0) {
    MakeInPar(id,"prelinking");    // make input par file (x.x.x.x.in.par) for the current ID including the prelinking par file
    for(int i=0; i<npre; i++)
      nc = LinkRun(id,0);        // will be done (pre)linking and updated x.x.x.x.par file
  }
  if(nfull>0) {
    MakeInPar(id,"fulllinking");   // make input par file including the fulllinking par file
    for(int i=0; i<nfull; i++)
      nc = LinkRun(id,1);         // will be done (full)linking and DO NOT updated x.x.x.x.par file
  }
  LogPrint(id[0],"LinkRunAll","%d couples stored", nc);
  return nc;
}

//----------------------------------------------------------------
bool EdbScanProc::ProjectFound(int id1[4],int id2[4])
{
  //take xp.xp.xp.xp.found.root and produce yp.yp.yp.yp.pred.root using the AFF/xp_yp.par
  LogPrint(id1[0],"ProjectFound","from %d.%d.%d.%d to %d.%d.%d.%d", id1[0],id1[1],id1[2],id1[3],id2[0],id2[1],id2[2],id2[3]);
  EdbPattern pat;
  ReadFound(pat,id1);
  //pat.SetZ(0);
  //pat.SetSegmentsZ();
  for(int i=0; i<pat.N(); i++) 
    pat.GetSegment(i)->SetErrors(50., 50., 0., .1, .1);  // area to be scanned for this prediction
  ApplyAffZ(pat,id1,id2);
  WritePred(pat,id2);
  WritePatTXT(pat,id2,"man.pred.txt");
  return true;
}

//----------------------------------------------------------------
bool EdbScanProc::CorrectAffWithPred(int id1[4],int id2[4], const char *opt, int patmin)
{
  // take p1.found.root, apply AFF/p1_p2.par, align to p2 and update AFF/p1_p2.par
  EdbPattern pat;
  ReadFound(pat,id1);
  if(pat.N()<patmin)
    LogPrint(id1[0],"CorrectAffWithPred","WARNING: unreliable correction - pattern is too small: %d < %d", pat.N(),patmin);
  else if(pat.N()<2) {
    LogPrint(id1[0],"CorrectAffWithPred","ERROR: correction is impossible - too small pattern: %d", pat.N());
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
    LogPrint(id1[0],"CorrectAffWithPred","WARNING: pattern is too small: %d < %d: do not update par file!", nal, patmin);
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
 
  LogPrint(id1[0],"CorrectAffWithPred","from %d.%d.%d.%d to %d.%d.%d.%d: used %d (out of %d predictions) for correction", 
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

  /*
  EdbDataPiece piece1,piece2;
  InitPiece(piece1, id1);
  piece1.GetLayer(0)->SetZlayer(-1*piece1.GetLayer(0)->Z(), 0,0);
  ReadPiece(piece1, p1);
  p1.SetZ(piece1.GetLayer(0)->Z());
  p1.SetSegmentsZ();

  InitPiece(piece2, id2);
  piece2.GetLayer(0)->SetZlayer(0, 0,0);
  ReadPiece(piece2, p2);
  p2.SetZ(piece2.GetLayer(0)->Z());
  p2.SetSegmentsZ();
  */

  //printf("EdbScanProc::Align: Z1 = %f z2 = %f\n",piece1.GetLayer(0)->Z(),piece2.GetLayer(0)->Z());
  EdbTestAl ta;
  ta.HDistance(p1,p2);
  //ta.FillTree( -5000 );
  //ta.FillTree( 0 );

  float bin[4]={20,20,100,0.001};
  ta.eDmin[0]=-5000; ta.eDmin[1]=-5000; ta.eDmin[2]=  1200; ta.eDmin[3]=-0.015;
  ta.eDmax[0]= 5000; ta.eDmax[1]= 5000; ta.eDmax[2]=  1500; ta.eDmax[3]= 0.015;

  FILE *f = fopen("testal.par","r");
  if(f) {
    for(int i=0; i<4; i++) {
      float min=0,max=0,b=0;
      if(!(fscanf(f,"%f %f %f",&min,&max,&b )==3))  {printf("ERROR: read from testal.par"); return 0;}
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
  aff.Print();
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
  LogPrint(id1[0],"AlignAll","%d.%d.%d.%d to %d.%d.%d.%d with %d pre and %d full.  The final pattern is: %d", 
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
  LogPrint(id[0],"WritePatTXT","%s with %d predictions with flag: %d", str.Data(),pred.N(), flag);
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
  LogPrint(id[0],"ReadPatTXT","%s with %d predictions with flag: %d", str.Data(),pred.N(),flag0);
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
  LogPrint(id[0],"WritePatRoot","%s with %d predictions with flag: %d", str.Data(),n,flag);
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
  LogPrint(id[0],"ReadPatRoot","%s with %d predictions with flag: %d", str.Data(),n,flag);
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
  printf("get map from file: %s\n",str);
  FILE *f = fopen(str,"r");
  if(!f)     { printf("no map file: %s !!!\n",str); return false; }
  else if (fgets (str, 256, f) == NULL) 
    { printf("error reading map file: %s !!!\n",str); return false; }
  printf("%s\n",str);
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
    printf("EdbScanProc::InitRun: %s\n",str2.Data());
  }
  printf("EdbScanProc::InitRun: %s\n",str.Data());
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
    LogPrint(id[0],"MakeInPar","ERROR! can't open file: %s",name.Data() );
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

//-------------------------------------------------------------------
//bool EdbScanProc::FindPredictionsRaw(EdbPattern &pred, EdbPattern &found, EdbRunAccess &ra)/
//{
//}

//-------------------------------------------------------------------
bool EdbScanProc::InitRunAccess(EdbRunAccess &ra, int id[4])
{
  // initialize the EdbRunAccess object useful for the raw data handling
  EdbDataPiece p;
  if(!InitPiece(p,id)) return false;
  p.Print();
  ra.eAFID = p.eAFID;
  if( !ra.InitRun(p.GetRunFile(0)) ) return false;
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
int EdbScanProc::ReadPatCP(EdbPattern &pat, int id[4])
{
  // read CP file ("base" segments) applying all cuts and transformations from x.x.x.x.in.par
  // the Z of the pat and all segments will be z of layer 0 defined in the par file(s)
  EdbDataPiece piece;
  InitPiece(piece, id);
  piece.GetLayer(0)->SetZlayer(piece.GetLayer(0)->Z(), 0,0);
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
      found.GetSegmentLast()->SetFlag(s->Flag()+1); // flag is the number of missed plates
      found.GetSegmentLast()->SetID(s->ID());
    }
  }
  fclose(f);
  fclose(fmt);

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

  LogPrint(id[0],"FindPredictions","%d.%d.%d.%d:  %d out of %d predictions are found (%d-zero, %d-single, %d-multy),  maxholes=%d", 
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
  
  ali.Align(2);
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
  if( strcmp(option,"-z") >=0 ) {
    ali.FineCorrZnew();
    piece1.UpdateZPar(0,-ali.GetPattern(0)->Z());
  }
  
  return npat;
 }

//______________________________________________________________________________
void EdbScanProc::LogPrint(int brick, const char *rout, const char *fmt, ...)
{
// Print message to the logfile and to stdout.
 
  static Int_t buf_size = 2048;
  static char *buf = 0;

  va_list ap;
  va_start(ap, fmt);
  
 again:
  if (!buf)
    buf = new char[buf_size];
  
  Int_t n = vsnprintf(buf, buf_size, fmt, ap);
  // old vsnprintf's return -1 if string is truncated new ones return
  // total number of characters that would have been written
  if (n == -1 || n >= buf_size) {
    if (n == -1)
      buf_size *= 2;
    else
      buf_size = n+1;
    delete [] buf;
    buf = 0;
    va_end(ap);
    va_start(ap, fmt);
    goto again;
  }
  va_end(ap);
  
  TDatime t;
  fprintf(stdout, "%-12s: ", rout);
  fprintf(stdout, "%s\n", buf);
  char str[256];
  sprintf(str,"%s/b%6.6d/b%6.6d.log", eProcDirClient.Data(), brick,brick);
  FILE *f = fopen(str,"a");
  if(f) {
    fprintf(f, "%s> ", t.AsSQLString());
    fprintf(f, "%-16s: ", rout);
    fprintf(f, "%s\n", buf);
    fclose(f);
  }
}
