//-- Author :  Valeri Tioukov   28-11-2008

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbAlignmentMap                                                      //
//                                                                      //
// to create the compton alignment map                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TMath.h>
#include <TFile.h>
#include <TGraph2D.h>
#include "EdbLog.h"
#include "EdbPositionAlignment.h"
#include "EdbAlignmentMap.h"

#include "TCell2.h"

ClassImp(EdbAlignmentMap)

using namespace TMath;

//---------------------------------------------------------------------
EdbAlignmentMap::EdbAlignmentMap(const char *file, const char *mode)
{
  eEnv          = 0;
  ePat1 = ePat2 = 0;
  eOutputFile   = 0;
  eMapTree      = 0;
  eNx=eNy=0;
  eGraphDX=eGraphDY=eGraphDZ1=eGraphDZ2=0;
  InitFile(file, mode);
}

//---------------------------------------------------------------------
EdbAlignmentMap::~EdbAlignmentMap()
{
  SaveAll();
  if(eOutputFile) {
    eOutputFile->Close();
    SafeDelete(eOutputFile);
  }
}

//---------------------------------------------------------------------
int EdbAlignmentMap::InitFile(const char *file, const char *mode)
{
  Log(2,"EdbAlignmentMap::InitFile","%s for %s",file,mode);
  if(file)  eOutputFile = new TFile(file,mode);

  if( eOutputFile && (strstr("READ",mode) || strstr("UPDATE",mode)) ) {
    eMapTree = (TTree*)eOutputFile->Get("maptree");
    TVector3 *v = (TVector3*)eOutputFile->Get("GV1");
    eGV1 = *v;
    v = (TVector3*)eOutputFile->Get("GV2");
    eGV2 = *v;
    EdbCell2 *c = (EdbCell2*)eOutputFile->Get("Map");
    eMap.Copy(*c);
    eMap.PrintStat();
  }
  else {
    eMapTree  = new TTree("maptree","maptree (izone:x0:y0:dx:dy:dz1:dz2:npeak:Hdxy)");
    Int_t   izone, peak, peakbin;
    Float_t mean3,mean;
    Float_t x0,y0,dx,dy,dz1,dz2;
    TH2F *hdxy=0, *hdz12=0;
    eMapTree->Branch("izone",  &izone,"izone/I");       // zone id
    eMapTree->Branch("x0",     &x0,"x0/F");             // zone position
    eMapTree->Branch("y0",     &y0,"y0/F");
    eMapTree->Branch("peak",   &peak,"peak/I");         // total number of the found coinsidence
    eMapTree->Branch("peakbin",&peakbin,"peakbin/I");   // peak bin
    eMapTree->Branch("mean3",  &mean3,"mean3/F");       // mean in 3x3 neigbour around the peak bin
    eMapTree->Branch("mean",   &mean,"mean/F");         // mean out of the peak zone
    eMapTree->Branch("dx",     &dx,"dx/F");             // found offsets
    eMapTree->Branch("dy",     &dy,"dy/F");
    eMapTree->Branch("dz1",    &dz1,"dz1/F");
    eMapTree->Branch("dz2",    &dz2,"dz2/F");
    eMapTree->Branch("Hdxy", "TH2F", &hdxy, 128000,0);
    eMapTree->Branch("Hdz12","TH2F", &hdz12,128000,0);
  }
  return 1;
}

//---------------------------------------------------------------------
void EdbAlignmentMap::SaveAll()
{
  if(!eOutputFile)               return;
  if(!eOutputFile->IsWritable()) return;
  if(eMapTree)  eMapTree->Write();
  if(eEnv)      eEnv->Write();
  eGV1.Write("GV1");
  eGV2.Write("GV2");
  eMap.Write("Map");
  eOutputFile->Save();
  eOutputFile->Purge();
}

//---------------------------------------------------------------------
void EdbAlignmentMap::AlignMap()
{
  gEDBDEBUGLEVEL = eEnv->GetValue("comptonmap.EdbDebugLevel" ,    1);
  eDensityMax    = eEnv->GetValue("comptonmap.DensityMax" ,    1.);

  for(int i=0; i<ePat1->N(); i++) ePat1->GetSegment(i)->SetFlag(0);
  for(int i=0; i<ePat2->N(); i++) ePat2->GetSegment(i)->SetFlag(0);
  eYcell = eXcell = eEnv->GetValue("comptonmap.zonesize" , 9000);

  eGlobal.eXcell  = 200;                    // set approximate bin size in microns
  eGlobal.eYcell  = 200;
  eGlobal.eDXmin  =  eGlobal.eDYmin  = 5;     // acceptance to remove doublets
  eGlobal.eDTXmin =  eGlobal.eDTYmin = 0.005;
  eGlobal.FillArrays(*ePat1, *ePat2);

  TH2F *hpat1 = eGlobal.ePC1.DrawH2("hpat1");
  TH2F *hpat2 = eGlobal.ePC2.DrawH2("hpat2");
  hpat1->Write("hpat1");
  hpat2->Write("hpat2");

  eGlobal.DoubletsFilterOut();
  eGlobal.SpotsFilterOut(30);               // remove overoccupated cells

  if(gEDBDEBUGLEVEL>1) {
    eGlobal.ePC1.PrintStat();
    eGlobal.ePC2.PrintStat();
  }

  EdbPositionAlignment local;
  local.eXcell   = 200;                    // set approximate bin size in microns
  local.eYcell   = 200;
  local.eDTXmax  =  local.eDTYmax = 0.15;  // acceptance for coincidence selections
  local.eNZ1step = eEnv->GetValue("comptonmap.NZ1step" , 100); 
  local.eZ1from  = eEnv->GetValue("comptonmap.Z1from" , -150); 
  local.eZ1to    = eEnv->GetValue("comptonmap.Z1to" ,     50); 
  local.eNZ2step = eEnv->GetValue("comptonmap.NZ2step" , 100); 
  local.eZ2from  = eEnv->GetValue("comptonmap.Z2from" ,  -50); 
  local.eZ2to    = eEnv->GetValue("comptonmap.Z2to" ,    150); 
  local.eBinY = local.eBinX  = eEnv->GetValue("comptonmap.BinXY" ,    10);  // bin size for the diff hist
  local.eNZ1step =  local.eNZ2step = 15;     // set big step in z for the global rough alignment
  float  testzone = eEnv->GetValue("comptonmap.TestZone" ,   30000);
  //eEnv->Print();

  float center[2] = { (eGlobal.ePC1.Xmax()+eGlobal.ePC1.Xmin())/2, (eGlobal.ePC1.Ymax()+eGlobal.ePC1.Ymin())/2};
  float    min[2] = { Max( eGlobal.ePC1.Xmin(), center[0]-testzone/2),
		      Max( eGlobal.ePC1.Ymin(), center[1]-testzone/2) };
  float    max[2] = { Min( eGlobal.ePC1.Xmax(), center[0]+testzone/2),
		      Min( eGlobal.ePC1.Ymax(), center[1]+testzone/2) };

  Log(2,"test alignment","in limits: X(%f %f), Y(%f %f) with max dens: %f ",min[0],max[0],min[1],max[1],eDensityMax);

  TObjArray arr1(10000), arr2(10000);
  eGlobal.SelectZone(min,max,arr1,arr2, eDensityMax);
  printf("n1 = %d n2 = %d\n",arr1.GetEntriesFast(), arr2.GetEntriesFast());

  local.FillArrays(arr1, arr2, min, max);
  local.FillCombinations(min,max);
  local.Align();                         // global peak selection
  local.eAff->Print();
  local.PrintSummary();
  eGV1.SetXYZ(local.eXpeak,local.eYpeak,local.eZ1peak);
  eGV2.SetXYZ(           0,           0,local.eZ2peak);

  // do global correction:
  ePat1->ProjectTo(local.eZ1peak);
  ePat2->ProjectTo(local.eZ2peak);
  ePat1->Transform(local.eAff);
  ePat1->SetZ(0); ePat1->SetSegmentsZ();
  ePat2->SetZ(0); ePat2->SetSegmentsZ();

  // fill initial map
  min[0] =  eGlobal.ePC1.Xmin();  min[1] =  eGlobal.ePC1.Ymin();
  max[0] =  eGlobal.ePC1.Xmax();  max[1] =  eGlobal.ePC1.Ymax();
  int     n[2] = { (int)((max[0]-min[0])/eXcell+1), (int)((max[1]-min[1])/eYcell+1) };
  eNx = n[0];
  eNy = n[1];
  eEnv->SetValue("comptonmap.eNx",eNx);
  eEnv->SetValue("comptonmap.eNy",eNy);
  eMap.InitCell(1,n,min,max);
  local.ResetPeak();

  local.eNZ1step =   25;
  local.eZ1from  =  -50;
  local.eZ1to    =   50;
  local.eNZ2step =   25;
  local.eZ2from  =  -50;
  local.eZ2to    =   50;
  local.eBinY = local.eBinX  = 5;
  local.eXcell = local.eYcell   = 100;

  eMap.PrintStat();

  //gEDBDEBUGLEVEL=0;
  int izone = 0;
  TVector3 dv1,dv2;
  for(int iy=0; iy<eMap.NY(); iy++) 
    for(int ix=0; ix<eMap.NX(); ix++) {
      min[0] = eMap.X(ix) - eMap.Xbin()/2;
      min[1] = eMap.Y(iy) - eMap.Ybin()/2;
      max[0] = eMap.X(ix) + eMap.Xbin()/2;
      max[1] = eMap.Y(iy) + eMap.Ybin()/2;
      local.eX0 = eMap.X(ix);
      local.eY0 = eMap.Y(iy);
      printf("\n(%d:%d)\n", ix,iy);
      arr1.Clear();
      arr2.Clear();
      eGlobal.SelectZone(min,max,arr1,arr2, eDensityMax);
      local.FillArrays(arr1, arr2, min, max);
      local.FillCombinations(min,max);
      printf("align with n1 = %d n2 = %d\n",arr1.GetEntriesFast(), arr2.GetEntriesFast());
      local.Align();                                      // local peak selection

      dv1.SetXYZ(local.eXpeak, local.eYpeak, local.eZ1peak);
      dv2.SetXYZ(local.eXpeak, local.eYpeak, local.eZ2peak);

      FillMapTree( local, izone );
      local.PrintSummary();
      local.ResetPeak();
      izone++;
    }
}


//---------------------------------------------------------------------
int EdbAlignmentMap::FillMapTree( EdbPositionAlignment &pol, int izone )
{
  if(!eMapTree) return 0;

  EdbPeak2 p2d(pol.eHpeak);
  p2d.ProbPeaks(2);          // prob for 2 most significant peaks
  int   peakbin = p2d.ePeak[0];
  float mean3   = p2d.eMean3[0];
  float mean    = p2d.eMean[0];

  TH2F *hdxy  = pol.eHpeak.DrawH2("hdxy");
  TH2F *hdz12 = pol.eHDZ.DrawH2("hdz12");
  
  eMapTree->SetBranchAddress("izone",   &izone);
  eMapTree->SetBranchAddress("x0",      &pol.eX0);
  eMapTree->SetBranchAddress("y0",      &pol.eY0);
  eMapTree->SetBranchAddress("peak",    &pol.eNpeak);
  eMapTree->SetBranchAddress("peakbin", &peakbin);
  eMapTree->SetBranchAddress("mean3",   &mean3);
  eMapTree->SetBranchAddress("mean",    &mean);
  eMapTree->SetBranchAddress("dx",      &pol.eXpeak);
  eMapTree->SetBranchAddress("dy",      &pol.eYpeak);
  eMapTree->SetBranchAddress("dz1",     &pol.eZ1peak);
  eMapTree->SetBranchAddress("dz2",     &pol.eZ2peak);
  eMapTree->SetBranchAddress("Hdxy",    &hdxy);
  eMapTree->SetBranchAddress("Hdz12",   &hdz12);
  eMapTree->Fill();

  SafeDelete(hdxy);
  SafeDelete(hdz12);

  if(pol.ePosTree) {
    //pol.ePosTree->SetAlias("dz1",Form("(%f)",pol.eZ1peak));
    //pol.ePosTree->SetAlias("dz2",Form("(%f)",pol.eZ2peak));
    pol.ePosTree->SetAlias("x1","s1.eX+dz1*s1.eTX");
    pol.ePosTree->SetAlias("x2","s2.eX+dz2*s2.eTX");
    pol.ePosTree->SetAlias("y1","s1.eY+dz1*s1.eTY");
    pol.ePosTree->SetAlias("y2","s2.eY+dz2*s2.eTY");
    pol.ePosTree->Write();
  }
  return 1;
}

//---------------------------------------------------------------------
void EdbAlignmentMap::SaveMap( const char *file )
{
  if(!eMapTree)   return;
  FILE *f = fopen(file,"w");
  if(!f)          return;
  int   N,IZONE;
  float X, Y, DX, DY, DZ1, DZ2;
  eMapTree->ResetBranchAddresses();
  eMapTree->SetBranchAddress("izone",&IZONE);
  eMapTree->SetBranchAddress("peak" ,&N);
  eMapTree->SetBranchAddress("x0"   ,&X);
  eMapTree->SetBranchAddress("y0"   ,&Y);
  eMapTree->SetBranchAddress("dx"   ,&DX);
  eMapTree->SetBranchAddress("dy"   ,&DY);
  eMapTree->SetBranchAddress("dz1"  ,&DZ1);
  eMapTree->SetBranchAddress("dz2"  ,&DZ2);
  fprintf(f,"%d %d %f %f   %f %f\n",eNx,eNy,eXcell,eYcell, eGV1.X(), eGV1.Y() );
  int n = eMapTree->GetEntries();
  for(int i=0; i<n; i++) {
    eMapTree->GetEntry(i);
    fprintf(f,"%5d %13.1f %13.1f %7.2f %7.2f   %7.2f %7.2f %5d\n", 
	    IZONE,X,Y,DX,DY,DZ1,DZ2,N);
  }
  fclose(f);
}

//---------------------------------------------------------------------
int EdbAlignmentMap::ApplyMap(EdbPattern &pat1, EdbPattern &pat2)
{
  Log(1,"ApplyMap","to patterns with %d and %d segments",pat1.N(),pat2.N());
  if(!eGraphDX) return 0;
  if(!eGraphDY) return 0;
  EdbSegP *s;
  int n = pat1.N();
  for(int i=0; i<n; i++) {
    s = pat1.GetSegment(i);
    float dx = eGraphDX->Interpolate(s->X(),s->Y());
    float dy = eGraphDY->Interpolate(s->X(),s->Y());
    //float dz = eGraphDZ1->Interpolate(s->X(),s->Y());
    float dz = 0;
    s->SetZ( dz + s->Z());
    s->SetX( dx + (s->X() + s->TX()*dz));
    s->SetY( dy + (s->Y() + s->TY()*dz));
  }

  /*
  n = pat2.N();
  for(int i=0; i<n; i++) {
    s = pat2.GetSegment(i);
    float dz = eGraphDZ2->Interpolate(s->X(),s->Y());
    float dx=0, dy=0;
    s->SetZ( dz + s->Z());
    s->SetX( dx + (s->X() + s->TX()*dz));
    s->SetY( dy + (s->Y() + s->TY()*dz));
  }
  */
  Log(1,"ApplyMap","%d segments corrected",n);

  return n;
}

//---------------------------------------------------------------------
void EdbAlignmentMap::ExtractMapFromTree()
{
  // assume that in eMap is defined the map structure
  int     n[2] = {eMap.NX()+2, eMap.NY()+2};       // define map with margins of one bin
  float min[2] = {eMap.Xmin()-eMap.Xbin(), eMap.Ymin()-eMap.Ybin()};
  float max[2] = {eMap.Xmax()+eMap.Xbin(), eMap.Ymax()+eMap.Ybin()};

  EdbCell2 map;
  map.InitCell(1,n,min,max);    // in each cell: TarrayF of (x,y, dx,dy,dz1,dz2, n)
  
  int   N,IZONE;
  float X, Y, DX, DY, DZ1, DZ2;
  eMapTree->ResetBranchAddresses();
  eMapTree->SetBranchAddress("izone",&IZONE);
  eMapTree->SetBranchAddress("peak" ,&N);
  eMapTree->SetBranchAddress("x0"   ,&X);
  eMapTree->SetBranchAddress("y0"   ,&Y);
  eMapTree->SetBranchAddress("dx"   ,&DX);
  eMapTree->SetBranchAddress("dy"   ,&DY);
  eMapTree->SetBranchAddress("dz1"  ,&DZ1);
  eMapTree->SetBranchAddress("dz2"  ,&DZ2);
  int nentr = eMapTree->GetEntries();
  for(int i=0; i<nentr; i++) {
    eMapTree->GetEntry(i);
    TArrayF *arr = new TArrayF(7);
    arr->AddAt( X           , 0);
    arr->AddAt( Y           , 1);
    arr->AddAt( DX +eGV1.X(), 2);
    arr->AddAt( DY +eGV1.Y(), 3);
    arr->AddAt( DZ1+eGV1.Z(), 4);
    arr->AddAt( DZ2         , 5);
    arr->AddAt( N           , 6);
    map.AddObject(X,Y,(TObject*)arr);
  }

  int ix0,iy0;
  for(int ix=0; ix<map.NX(); ix++)
    for(int iy=0; iy<map.NY(); iy++) {
      ix0 = ix; iy0 = iy;
      if(ix==0)           ix0=1;
      if(ix==map.NX()-1 ) ix0=map.NX()-2;
      if(iy==0)           iy0=1;
      if(iy==map.NY()-1 ) iy0=map.NY()-2;
      if( ix0!=ix || iy0!=iy ) {
	TArrayF *arr0 = (TArrayF*)map.GetObject(ix0,iy0,0);
	TArrayF *arr = new TArrayF(*arr0);
	arr->AddAt( map.X(ix), 0 );
	arr->AddAt( map.Y(iy), 1 );
	map.AddObject(ix,iy,(TObject*)arr);
      }
    }

  Log(2, "EdbAlignmentMap::ExtractMapFromTree","with %d modes", map.Ncell() );
  if(gEDBDEBUGLEVEL>=2)  map.PrintStat();

  int nentries = map.Ncell();
  Double_t *vx   = new Double_t[nentries];
  Double_t *vy   = new Double_t[nentries];
  Double_t *vdx  = new Double_t[nentries];
  Double_t *vdy  = new Double_t[nentries];
  Double_t *vdz1 = new Double_t[nentries];
  Double_t *vdz2 = new Double_t[nentries];

  for(int i=0; i<nentries; i++) {
    TArrayF *arr = (TArrayF*)map.GetObject(i,0);
    if(!arr) break;
    vx[i]   = arr->At(0);
    vy[i]   = arr->At(1);
    vdx[i]  = arr->At(2);
    vdy[i]  = arr->At(3);
    vdz1[i] = arr->At(4);
    vdz2[i] = arr->At(5);
    //printf("%d %f %f %f %f\n",i,vx[i],vy[i],vdx[i], vdy[i]);
  }

  eGraphDX  = new TGraph2D("graphDX","graphDX",nentries,vx,vy,vdx);
  eGraphDY  = new TGraph2D("graphDY","graphDY",nentries,vx,vy,vdy);
  eGraphDZ1 = new TGraph2D("graphDZ1","graphDZ1",nentries,vx,vy,vdz1);
  eGraphDZ2 = new TGraph2D("graphDZ2","graphDZ2",nentries,vx,vy,vdz2);
  //eGraphDZ = new TGraph2D("graphDZ","graphDZ",nentries,vx,vy,vdz);

  if(eOutputFile) {
    if(eOutputFile->IsWritable()) {
      eGraphDX->Write( "graphDX");
      eGraphDY->Write( "graphDY");
      eGraphDZ1->Write("graphDZ1");
      eGraphDZ2->Write("graphDZ2");
      eOutputFile->Purge();
    }
  }

}
