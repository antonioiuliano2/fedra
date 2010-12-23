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
#include "EdbRunAccess.h"
#include "EdbPositionAlignment.h"
#include "EdbAlignmentMap.h"
#include "EdbCell2.h"

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
    eMapTree->Branch("peakbin",&peakbin,"peakbin/F");   // peak bin
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
  eDensityMax    = eEnv->GetValue("comptonmap.DensityMax" ,    1.);

  for(int i=0; i<ePat1->N(); i++) ePat1->GetSegment(i)->SetFlag(0);
  for(int i=0; i<ePat2->N(); i++) ePat2->GetSegment(i)->SetFlag(0);
  eYcell = eXcell = eEnv->GetValue("comptonmap.zonesize" , 9000);

  eGlobal.eXcell  = 200;                    // set approximate bin size in microns
  eGlobal.eYcell  = 200;
  eGlobal.eDXmin  =  eGlobal.eDYmin  =  eEnv->GetValue("comptonmap.DoubletsDX" ,    5.);     // acceptance to remove doublets
  eGlobal.eDTXmin =  eGlobal.eDTYmin =  eEnv->GetValue("comptonmap.DoubletsDTX" ,   0.005);
  eGlobal.FillArrays(*ePat1, *ePat2);

  TH2F *hpat1 = eGlobal.ePC1.DrawH2("hpat1");
  TH2F *hpat2 = eGlobal.ePC2.DrawH2("hpat2");
  hpat1->Write("hpat1");
  hpat2->Write("hpat2");

  TH1I *spectr1 = eGlobal.ePC1.DrawSpectrum("spectr1");
  spectr1->Write("spectrum1");
  TH1I *spectr2 = eGlobal.ePC2.DrawSpectrum("spectr2");
  spectr2->Write("spectrum2");

  //eGlobal.DoubletsFilterOut(true);

  eGlobal.ActivatePosTree("doublets");
  eGlobal.DoubletsFilterOut(false);
  eGlobal.WritePosTree();

  eGlobal.SpotsFilterOut(30);               // remove overoccupated cells

  if(gEDBDEBUGLEVEL>1) {
    eGlobal.ePC1.PrintStat();
    eGlobal.ePC2.PrintStat();
    EdbH2 *pat1sel = eGlobal.ePC1.FillSelectedH2();
    EdbH2 *pat2sel = eGlobal.ePC2.FillSelectedH2();
    hpat1 = pat1sel->DrawH2("hpat1sel");    hpat1->Write("hpat1sel");
    hpat2 = pat2sel->DrawH2("hpat2sel");    hpat2->Write("hpat2sel");
  }

  EdbPositionAlignment local;
  local.eYcell   = local.eXcell   = eEnv->GetValue("comptonmap.Xcell" , 100);                    // set approximate bin size in microns
  local.eDTXmax  =  local.eDTYmax = eEnv->GetValue("comptonmap.DTmax" , 0.15);  // acceptance for coincidence selections
  local.eNZ1step = eEnv->GetValue("comptonmap.NZ1step" , 100); 
  local.eZ1from  = eEnv->GetValue("comptonmap.Z1from" , -150); 
  local.eZ1to    = eEnv->GetValue("comptonmap.Z1to" ,     50); 
  local.eNZ2step = eEnv->GetValue("comptonmap.NZ2step" , 100); 
  local.eZ2from  = eEnv->GetValue("comptonmap.Z2from" ,  -50); 
  local.eZ2to    = eEnv->GetValue("comptonmap.Z2to" ,    150); 
  //local.eNZ1step =  local.eNZ2step = 15;     // set big step in z for the global rough alignment
  local.eBinY = local.eBinX  = eEnv->GetValue("comptonmap.BinXY" ,    10);  // bin size for the diff hist
  local.eRpeak = eEnv->GetValue("comptonmap.Rpeak" ,    4.);  // position peak acceptance
  float  testzone = eEnv->GetValue("comptonmap.TestZone" ,   30000);
  //eEnv->Print();

  float center[2] = { (eGlobal.ePC1.Xmax()+eGlobal.ePC1.Xmin())/2, (eGlobal.ePC1.Ymax()+eGlobal.ePC1.Ymin())/2};
  float    min[2] = { Max( eGlobal.ePC1.Xmin(), center[0]-testzone/2),
		      Max( eGlobal.ePC1.Ymin(), center[1]-testzone/2) };
  float    max[2] = { Min( eGlobal.ePC1.Xmax(), center[0]+testzone/2),
		      Min( eGlobal.ePC1.Ymax(), center[1]+testzone/2) };
  Log(1,"Global alignment","in limits: X(%f %f), Y(%f %f) with max dens: %f ",min[0],max[0],min[1],max[1],eDensityMax);

  TObjArray arr1(50000), arr2(50000);
  eGlobal.SelectZone(min,max,arr1,arr2, eDensityMax);
  float areaGlobal = (max[0]-min[0])*(max[1]-min[1]);

  printf("******* Global alignment: n1 = %d n2 = %d *******\n",arr1.GetEntriesFast(), arr2.GetEntriesFast());
  local.FillArrays(arr1, arr2, min, max);
  local.FillCombinations();
  local.eSmoothKernel = "k5a";
  float normKernel    =  25;
  local.Align();                         // global peak selection
  //local.eAff->Print();
  TH2F *hdxy  = local.eHpeak.DrawH2("global_hdxy");
  TH2F *hdz12 = local.eHDZ.DrawH2("global_hdz12");
  hdxy->Write("global_hdxy");
  hdz12->Write("global_hdz12");

  local.PrintSummary();
  eGV1.SetXYZ(local.eXpeak,local.eYpeak,local.eZ1peak);
  eGV2.SetXYZ(           0,           0,local.eZ2peak);
  EdbPeak2 p2d(local.eHpeak);
  p2d.ProbPeaks(2);          // prob for 2 most significant peaks
  float  peakVolume = p2d.EstimatePeakVolumeSafe(0) / normKernel;
  printf("******* End of Global alignment *******\n\n");

  // do global correction:
  ePat1->ProjectTo(local.eZ1peak);
  ePat2->ProjectTo(local.eZ2peak);
  ePat1->Transform(local.eAff);
  ePat1->SetZ(0); ePat1->SetSegmentsZ();
  ePat2->SetZ(0); ePat2->SetSegmentsZ();

  // define the best zone size using the results of the global alignment
  float  requestPeakVolume = eEnv->GetValue("comptonmap.PeakVolume" ,    150.);
  float  zonesize = Sqrt( areaGlobal * requestPeakVolume/peakVolume );
  Log(1,"EdbAlignmentMap::AlignMap",
	  "zonesize selected as %10.2f  considering global peakVolume = %10.2f and requested zonePeakVolume as %10.2f", 
	  zonesize, peakVolume, requestPeakVolume);

  // fill initial map
  min[0] =  eGlobal.ePC1.Xmin();  min[1] =  eGlobal.ePC1.Ymin();
  max[0] =  eGlobal.ePC1.Xmax();  max[1] =  eGlobal.ePC1.Ymax();
  //int     n[2] = { (int)((max[0]-min[0])/eXcell+1), (int)((max[1]-min[1])/eYcell+1) };
  int     n[2] = { (int)((max[0]-min[0])/zonesize+1), (int)((max[1]-min[1])/zonesize+1) };
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
  //local.eXcell = local.eYcell   = 100;

  eMap.PrintStat();

  int izone = 0;
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
      local.eXcell = local.eYcell   = eEnv->GetValue("comptonmap.Xcell" , 100);
      local.FillArrays(arr1, arr2, min, max);
      printf("align with n1 = %d n2 = %d\n",arr1.GetEntriesFast(), arr2.GetEntriesFast());
      local.FillCombinations();
      local.Align();                                      // local peak selection

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

  float peakbin=0, mean3=0, mean=0;
  if(pol.eHpeak.Integral()>0) {
    EdbPeak2 p2d(pol.eHpeak);
    p2d.ProbPeaks(2);          // prob for 2 most significant peaks
    peakbin = p2d.ePeak[0];
    mean3   = p2d.eMean3[0];
    mean    = p2d.eMean[0];
  }

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

  pol.WritePosTree();

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
  fprintf(f,"%d %d %f %f   %f %f\n",eNx,eNy,eMap.Xbin(),eMap.Ybin(), eGV1.X(), eGV1.Y() );
  int n = eMapTree->GetEntries();
  for(int i=0; i<n; i++) {
    eMapTree->GetEntry(i);
    fprintf(f,"%5d %13.1f %13.1f %7.2f %7.2f   %7.2f %7.2f %5d\n", 
	    IZONE,X,Y,DX+eGV1.X(),DY+eGV1.Y(),DZ1,DZ2,N);
  }
  fclose(f);
}

//---------------------------------------------------------------------
int EdbAlignmentMap::ApplyMap(EdbPattern &pat)
{
  // applied to "pat1" gives "pat2"

  Log(1,"ApplyMap","to pattern with %d segments",pat.N());
  if(!eGraphDX) return 0;
  if(!eGraphDY) return 0;
  EdbSegP *s;
  int n = pat.N();
  for(int i=0; i<n; i++) {
    s = pat.GetSegment(i);
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


//-------------------------------------------------------------------------------
int  EdbAlignmentMap::Link()
{
  return Link( *eEnv, *ePat1, *ePat2, *ePlate);
}

//-------------------------------------------------------------------------------
int  EdbAlignmentMap::Link(TEnv &cenv, EdbPattern &p1all, EdbPattern &p2all, EdbPlateP &plate)
{
  //--------------------------
  // to be implemented:
  // - select the subpatterns for shrinkage correction
  // - do shrinkage correction
  // - apply the results for subpatterns
  // - check dz
  // - apply the results for full patterns
  // - link at the fixed dz
  //--------------------------

  eOutputFile->cd();

  int npeak=0;

  //CheckPattern(p1all, "1");
  //CheckPattern(p2all, "2");

  float DZ       = plate.GetLayer(2)->Z() - plate.GetLayer(1)->Z();

  bool doCorr    = cenv.GetValue("fedra.link.DoCorr",   false);
  bool doShr     = cenv.GetValue("fedra.link.DoShr",    false);
  
  //atest.FillArrays(p1sel, p2sel);     // test linking with the selected segments
  //atest.DoubletsFilterOut(true);
  if(doCorr) {
    float wmin = cenv.GetValue("fedra.link.shr.Wmin", 9. );
    float wmax = cenv.GetValue("fedra.link.shr.Wmax", 100. );
    float tmin = cenv.GetValue("fedra.link.shr.Tmin", 0.01 );
    float tmax = cenv.GetValue("fedra.link.shr.Tmax", 0.5 );
    
    EdbPattern p1sel,p2sel;
    SelectSampleForShrinkageCorr(p1all,p1sel, wmin, wmax, tmin, tmax);
    SelectSampleForShrinkageCorr(p2all,p2sel, wmin, wmax, tmin, tmax);

    float dx1 = p1sel.Xmax() -  p1sel.Xmin();
    float dy1 = p1sel.Ymax() -  p1sel.Ymin();
    float cell1 = Sqrt( dx1*dy1/p1sel.N() );
    float dx2 = p2sel.Xmax() -  p2sel.Xmin();
    float dy2 = p2sel.Ymax() -  p2sel.Ymin();
    float cell2 = Sqrt( dx2*dy2/p2sel.N() );
    float cell = Max( (float)Min(cell1,cell2), (float)(cenv.GetValue("fedra.link.CellXY", 25.)) );

    EdbPositionAlignment atest;
    atest.eXcell   = atest.eYcell   = cell;
    atest.eBinX    = atest.eBinY    = cenv.GetValue("fedra.link.BinXY",    4. );
    atest.eDTXmax  = atest.eDTYmax  = cenv.GetValue("fedra.link.DTMax",    0.02 );

    atest.FindCorrections( p1sel, p2sel, DZ, doShr);
    TH2F *hshr1 = atest.eHShr1.DrawH2("hshr1");
    TH2F *hshr2 = atest.eHShr2.DrawH2("hshr2");
    hshr1->Write("hshr1");     SafeDelete(hshr1);
    hshr2->Write("hshr2");     SafeDelete(hshr2);


    atest.ePC1.ApplyCorrections(p1all);
    atest.ePC2.ApplyCorrections(p2all);
    atest.ePC1.ApplyCorrections( *(plate.GetLayer(1)) );
    atest.ePC2.ApplyCorrections( *(plate.GetLayer(2)) );
    plate.Write("plate");


//     Log(1,"Link","check corrections:");
//     EdbPositionAlignment atest2;
//     atest2.eXcell   = atest2.eYcell   = cenv.GetValue("fedra.link.CellXY",  10 );
//     atest2.eBinX    = atest2.eBinY    = cenv.GetValue("fedra.link.BinXY",    4 );
//     atest2.eDTXmax  = atest2.eDTYmax  = cenv.GetValue("fedra.link.DTMax", 0.02 );
//     atest.ePC1.ApplyCorrections(p1sel);
//     atest.ePC2.ApplyCorrections(p2sel);
//     atest2.FindCorrections(p1sel, p2sel, DZ, false);
  
  }

  //return 1;

  // --------- end of test linking -----------


  EdbPositionAlignment aall;
  aall.eXcell   = aall.eYcell   = cenv.GetValue("fedra.link.CellXY",     10. );
  aall.eBinX    = aall.eBinY    = cenv.GetValue("comptonmap.link.BinXY",  4. );
  aall.eDTXmax  = aall.eDTYmax  = cenv.GetValue("comptonmap.link.DTMax",  0.02 );
  aall.eChi2Max                 = cenv.GetValue("fedra.link.Chi2Max",     3.  );

  aall.ePC1.eDZ =  DZ/2;
  aall.ePC2.eDZ = -DZ/2;
  aall.ePC1.eApplyCorr = aall.ePC2.eApplyCorr = true;

  aall.FillArrays(p1all, p2all);

  TH2F *hpat1 = aall.ePC1.DrawH2("hpat1");
  TH2F *hpat2 = aall.ePC2.DrawH2("hpat2");
  hpat1->Write("hpat1");      SafeDelete(hpat1);
  hpat2->Write("hpat2");      SafeDelete(hpat2);
  TH1I *spectr1 = aall.ePC1.DrawSpectrum("spectrum1");
  spectr1->Write("spectrum1");  SafeDelete(spectr1);
  TH1I *spectr2 = aall.ePC2.DrawSpectrum("spectrum2");
  spectr2->Write("spectrum2");  SafeDelete(spectr2);

  aall.DoubletsFilterOut(true);
  aall.ePC1.eDoubletsRate = new TH1I("DoubletsRate","DoubletsRate",100,-0.5,99.5);

  float dpos = cenv.GetValue("fedra.link.AccPos",    15. );
  float dang = cenv.GetValue("fedra.link.AccAng",    0.1 );
  aall.FillCombinations(aall.ePC1, aall.ePC2, dpos, dpos, dang, dang );
  aall.RankCouples();
  aall.ePC1.eDoubletsRate->Write();  SafeDelete(aall.ePC1.eDoubletsRate);

  int nfill=0;
  if(aall.ActivatePosTree("couples"))  nfill = aall.FillPosTree(DZ/2, -DZ/2, 10);
  FillMapTree(aall,10);

  return npeak;
}

//-------------------------------------------------------------------------------
int  EdbAlignmentMap::CheckDZbase(TEnv &cenv, EdbPattern &p1all, EdbPattern &p2all)
{
  // test "alignment" to check dz
  
  float wmin = cenv.GetValue("fedra.link.shr.Wmin", 9 );
  float wmax = cenv.GetValue("fedra.link.shr.Wmax", 100 );
  float tmin = cenv.GetValue("fedra.link.shr.Tmin", 0.01 );
  float tmax = cenv.GetValue("fedra.link.shr.Tmax", 0.5 );

  EdbPattern p1sel,p2sel;
  SelectSampleForShrinkageCorr(p1all,p1sel, wmin, wmax, tmin, tmax);
  SelectSampleForShrinkageCorr(p2all,p2sel, wmin, wmax, tmin, tmax);

  EdbPositionAlignment atest;
  //atest.eXcell   = atest.eYcell   = cenv.GetValue("fedra.link.CellXY",  10 );
  atest.eXcell   = atest.eYcell   = cenv.GetValue("fedra.link.CellXY",  100 );    // enlarge
  atest.eBinX    = atest.eBinY    = cenv.GetValue("fedra.link.BinXY",    4 );
  atest.eDTXmax  = atest.eDTYmax  = cenv.GetValue("fedra.link.DTMax", 0.02 );

  atest.eNZ1step =   35;
  atest.eZ1from  =  -350;
  atest.eZ1to    =   350;
  atest.eNZ2step =   35;
  atest.eZ2from  =  -350;
  atest.eZ2to    =   350;


  float DZbase = cenv.GetValue("fedra.link.DZbase", 214.);
  //p1sel.SetZ(0);       p1sel.SetSegmentsZ();
  //p2sel.SetZ(DZbase);  p2sel.SetSegmentsZ();
  p1sel.ProjectTo(DZbase/2.);
  p2sel.ProjectTo(-DZbase/2.);

  atest.SaveAsTree(p1sel,"p1sel");
  atest.SaveAsTree(p2sel,"p2sel");

  atest.FillArrays(p1sel,p2sel);
  printf( "align with n1 = %d n2 = %d\n",p1sel.N(), p2sel.N() );
  atest.FillCombinations();
  atest.Align();                                      // local peak selection

  FillMapTree( atest, 0 );

  return 0;
}

//-------------------------------------------------------------------------------
int  EdbAlignmentMap::SelectSampleForShrinkageCorr(EdbPattern &p, EdbPattern &plnk, float wmin, float wmax, float tmin, float tmax )
{
  // to do: analyse patterns before and select the best sample
  int nsel=0;

  int n = p.N();
  EdbSegP *s;
  for(int i=0; i<n; i++) {
    s = p.GetSegment(i);
    if( s->W()<wmin )       continue;
    if( s->Theta() < tmin ) continue;
    if( s->Theta() > tmax ) continue;
    if( s->Flag() == -10  ) continue;
    if( s->W()    > wmax  ) continue;
    plnk.AddSegment(*s);
  }
  nsel = plnk.N();
  Log(2,"SelectSampleForShrinkageCorr","%d of %d segments selected",nsel,n);
  return nsel;
}

//-------------------------------------------------------------------------------
void  EdbAlignmentMap::CheckPattern(EdbPattern &p, const char *suffix )
{
  // produce control plots for this pattern

  TH1F *hw = new TH1F( Form("w_%s",suffix), "W", 100, 0, 100);
  TH2F *htxty = new TH2F( Form("txty_%s",suffix), "W", 
			  100, -1.5, 1.5, 100, -1.5,1.5 );

  int n = p.N();
  EdbSegP *s;
  for(int i=0; i<n; i++) {
    s = p.GetSegment(i);
    hw->Fill( s->W() );
    htxty->Fill( s->TX(), s->TY());
  }

  hw->Write();
  htxty->Write();

  Log(2,"CheckPattern","with %d segments",n);
}


//------------------------------------------------------------------------
void  EdbAlignmentMap::get_run_patterns(const char *runfile, TEnv *cenv, EdbPattern &p1, EdbPattern &p2)
{
  // read patterns from the raw files in the root format

  Log(3," EdbAlignmentMap::get_run_patterns","from file: %s",runfile);

  float x           = cenv->GetValue("comptonmap.x0"   , 40000.);
  float y           = cenv->GetValue("comptonmap.y0"   , 40000.);
  float size        = cenv->GetValue("comptonmap.size" , 500000.);
  float pulsmin     = cenv->GetValue("comptonmap.pulsmin" , 8);
  int   side1       = cenv->GetValue("comptonmap.side1" , 1);
  int   side2       = cenv->GetValue("comptonmap.side2" , 2);
  gEDBDEBUGLEVEL    = cenv->GetValue("comptonmap.EdbDebugLevel",    2);

  //cenv->Print();

  EdbRunAccess ra;
  ra.InitRun(runfile);

  ra.eAFID = cenv->GetValue("fedra.link.AFID"   , 1);

  float min[5] = {-500,-500,-1.,-1., pulsmin};
  float max[5] = { 500, 500, 1., 1., 50};
  ra.AddSegmentCut(side1, 1, min, max);
  ra.AddSegmentCut(side2, 1, min, max);

  EdbSegP s(0, x, y, 0,0);

  int n1= ra.GetPatternXY(s, side1, p1, size);
  int n2= ra.GetPatternXY(s, side2, p2, size);

  p1.SetID(1);
  p2.SetID(2);

  Log(2," EdbAlignmentMap::get_run_patterns","read segments: n1=%d at z=%f   and  n2=%d at z=%f",
      n1,ra.GetLayer(1)->Z(),n2,ra.GetLayer(2)->Z());
}

//-------------------------------------------------------------------------------
void  EdbAlignmentMap::Link( const char *file, EdbPlateP &plate )
{
  Log(3," EdbAlignmentMap::Link","file %s",file);
  if(gEDBDEBUGLEVEL>2) plate.PrintPlateLayout();

  EdbPattern p1, p2;
  get_run_patterns( file , eEnv, p1, p2);
  p1.SetZ( plate.GetLayer(1)->Z() );  p1.SetSegmentsZ();
  p2.SetZ( plate.GetLayer(2)->Z() );  p2.SetSegmentsZ();

  Log(2," EdbAlignmentMap::Link","%d at %f  and  %d at %f",p1.N(),p1.Z(),p2.N(),p2.Z() );

  eOutputFile->cd();
  ePat1  = &p1;
  ePat2  = &p2;
  ePlate = &plate;
  Link();
  SaveAll();
 }

//--------------------------------------------------------------
void   EdbAlignmentMap::GetPostreeAsPat(EdbPattern &pat, const char *filename)
{
  TFile f(filename);
  TTree *tree = (TTree*)(f.Get("couples"));
  int n = tree->GetEntries();
  if(!n)  return;
  EdbSegP *s = new EdbSegP();
  tree->SetBranchAddress("s.",&s);
  for(int i=0; i<n; i++) {
    tree->GetEntry(i);
    pat.AddSegment(*s);
  }
  Log(2,"GetPostreeAsPat","%d segments read from file %s",n, filename);
  f.Close();
}


//-------------------------------------------------------------------------------
int  EdbAlignmentMap::CheckXY(TEnv &cenv, EdbPattern &p1all, EdbPattern &p2all)
{
  // test "alignment" to check DX and DY
 
  float wmin = cenv.GetValue("fedra.align.Wmin", 16 );
  float wmax = cenv.GetValue("fedra.align.Wmax", 100 );
  float tmin = cenv.GetValue("fedra.align.Tmin", 0.03 );
  float tmax = cenv.GetValue("fedra.align.Tmax", 0.5 );

  EdbPattern p1sel,p2sel;
  SelectSampleForShrinkageCorr(p1all,p1sel, wmin, wmax, tmin, tmax);
  SelectSampleForShrinkageCorr(p2all,p2sel, wmin, wmax, tmin, tmax);

  EdbPositionAlignment atest;
  atest.eXcell   = atest.eYcell   = cenv.GetValue("fedra.align.CellXY",  100 );    // enlarge
  atest.eBinX    = atest.eBinY    = cenv.GetValue("fedra.align.BinXY",    4 );
  atest.eDTXmax  = atest.eDTYmax  = cenv.GetValue("fedra.align.DTMax",  0.01 );

  float DZ = cenv.GetValue("fedra.align.DZ", 1300);

  EdbH2 hxy;
  float xmin = -2000, ymin = -2000, xmax = 2000, ymax = 2000; 

  //for(int i=0; i<=11; i++) {
  //float dz = DZ+(i-5)*500;
    atest.WideSearchXY(p1sel, p2sel, hxy, DZ, xmin, xmax, ymin, ymax);
    hxy.DrawH2("hxy")->Write(Form("hxy"));
    //}

  return 0;
}

