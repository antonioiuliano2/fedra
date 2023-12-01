//-- Author :  Valeri Tioukov   22.06.2011

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewMatch - nearby views matching - the primary purpose is distortion corrections
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TBox.h"
#include "TArrow.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TGraph2D.h"
#include "TMath.h"
#include "EdbLog.h"
#include "EdbCluster.h"
#include "EdbSegment.h"
#include "EdbViewMatch.h"

using namespace TMath;

ClassImp(EdbClMatch)
ClassImp(EdbViewMatch)

//____________________________________________________________________________________
EdbViewMatch::EdbViewMatch()
{
  eNClMin      = 20;
  eR2CenterMax = 15.;
  eRmax        =  1.;
  eOutputFile  =  0;

  eCl.SetClass("EdbClMatch",10); eCl.ExpandCreateFast(100000000);
  eGr.SetClass("EdbSegment",10); eGr.Expand(10000000);

  eXpix  = 0; //0.30625;
  eYpix  = 0; //0.30714;
  eNXpix = 0; //1280;
  eNYpix = 0; //1024;
  
  eCorrectionMatrixStepX = 10.; //microns - will be tuned in setpar
  eCorrectionMatrixStepY = 10.; //microns - will be tuned in setpar
  
  eAreaMin  =0;  eAreaMax  =90000000;
  eVolumeMin=0;  eVolumeMax=90000000;

}

//____________________________________________________________________________________
void EdbViewMatch::SetPar(TEnv &env)
{
  eNClMin      = env.GetValue("viewdist.NClMin" , 20);
  eR2CenterMax = env.GetValue("viewdist.R2CenterMax" , 15.);
  eRmax        = env.GetValue("viewdist.Rmax" ,  1.);

  eXpix  = env.GetValue("viewdist.Xpix" , 0.); // 0.30625);
  eYpix  = env.GetValue("viewdist.Ypix" , 0.); //0.30714);
  eNXpix = env.GetValue("viewdist.NXpix", 0);  //1280);
  eNYpix = env.GetValue("viewdist.NYpix", 0);  //1024);

  eCorrectionMatrixStepX = env.GetValue("viewdist.MatrixStepX", 10);
  eCorrectionMatrixStepY = env.GetValue("viewdist.MatrixStepY", 10);
  
  eDumpGr = env.GetValue("viewdist.DumpGr", 0);
  
  sscanf( env.GetValue("viewdist.ClusterAreaLimits", "0 90000000"), "%f %f", &eAreaMin, &eAreaMax);
  sscanf( env.GetValue("viewdist.ClusterVolumeLimits", "0 90000000"), "%f %f", &eVolumeMin, &eVolumeMax);
  
  printf("\n----------------------- Processing Parameters ---------------------------\n");
  printf("eNClMin\t %d\n",eNClMin);
  printf("eR2CenterMax\t %6.2f\n",eR2CenterMax);
  printf("eRmax\t %f6.2\n",eRmax);
  printf("Pixel:  %9.7f x %9.7f \n", eXpix, eYpix );
  printf("Matrix: %d x %d pixels, \t  %15.7f x %15.7f microns", eNXpix, eNYpix, eXpix*eNXpix, eYpix*eNYpix);
  printf("Clusters Area limits: %7.0f  %7.0f \n",  eAreaMin, eAreaMax );
  printf("Clusters Volume limits: %7.0f  %7.0f \n",  eVolumeMin, eVolumeMax );
  printf("-------------------------------------------------------------------------\n\n");
  
}

//____________________________________________________________________________________
void EdbViewMatch::InitCorrMap()
{
  // matrix of the corrections in each entry is TArrayD with 2 entries:(dx,dy)
  float  mi[2] = { -eNXpix*Abs(eXpix)/2., -eNYpix*Abs(eYpix)/2. };
  float  ma[2] = {  eNXpix*Abs(eXpix)/2.,  eNYpix*Abs(eYpix)/2. };
  int    n[2] = { int((ma[0]-mi[0])/eCorrectionMatrixStepX), int((ma[1]-mi[1])/eCorrectionMatrixStepY) };
  
  eCorrectionMatrixStepX = (ma[0]-mi[0])/n[0];
  eCorrectionMatrixStepY = (ma[1]-mi[1])/n[1];
  n[0] = int((ma[0]-mi[0]+1.)/eCorrectionMatrixStepX);
  n[1] = int((ma[1]-mi[1]+1.)/eCorrectionMatrixStepY);
  
  eCorrMap.InitCell(20, n, mi, ma);
  eCorrMap.PrintStat();
  int nc=eCorrMap.Ncell();
  for( int i=0; i<nc; i++ ) {
    TArrayD *a = new TArrayD(2);
    eCorrMap.AddObject(i, (TObject*)a );
    eCorrMap.SetBin(i,0);
  }
  
  eCorrMap0.InitCell(20, n, mi, ma);
  nc=eCorrMap0.Ncell();
  for( int i=0; i<nc; i++ ) {
    TArrayD *a = new TArrayD(2);
    eCorrMap0.AddObject(i, (TObject*)a );
    eCorrMap0.SetBin(i,0);
  }  

}

//____________________________________________________________________________________
void EdbViewMatch::InitGMap()
{
  float bin=5;
  float  mi[2] = { -eNXpix*Abs(eXpix)/2-bin, -eNYpix*Abs(eYpix)/2-bin };
  float  ma[2] = {  1.5*eNXpix*Abs(eXpix)+bin,  1.5*eNYpix*Abs(eYpix)+bin };
  int    n[2] = { int((ma[0]-mi[0])/bin), int((ma[1]-mi[1])/bin) };
  eGMap.InitCell(250, n, mi, ma);
}

//____________________________________________________________________________________
void EdbViewMatch::AddCluster( EdbClMatch *c)
{
  float v[2]={ c->eX+c->eXv,  c->eY+c->eYv };
  int  ir[2]={1,1};
  TObjArray arr;
  EdbSegment *s0 = 0;
  int n = eGMap.SelectObjectsC( v, ir, arr);
  if(n>100) printf("n=%d\n",n);
  if(n) { 
   float r, rmin=2.*eRmax;
    for(int i=0; i<n; i++) {
      EdbSegment *s = (EdbSegment *)arr.At(i);
      r = Sqrt( (s->GetX0()-v[0])*(s->GetX0()-v[0]) + (s->GetY0()-v[1])*(s->GetY0()-v[1]) );
      if(r<eRmax) if(r<rmin) { rmin=r; s0 = s; }
    }
  }
  if(s0) { s0->AddElement(c); s0->SetX0(v[0]); s0->SetY0(v[1]); s0->SetZ0(c->eZ); }
  else  {
    int inds = eGr.GetEntriesFast();
    EdbSegment *s = new(eGr[inds]) EdbSegment(v[0],v[1],c->eZ,0,0);
    s->AddElement(c);
    eGMap.AddObject(v, s);
  }

}

//____________________________________________________________________________________
void EdbViewMatch::CalculateGrRef()
{
  // take as grain reference position of the most central cluster
  
  int ngr = eGr.GetEntries();
  for(int i=0; i<ngr; i++) {
    EdbSegment *s = ((EdbSegment*)eGr.UncheckedAt(i));
    int nc = s->GetNelements();
    float rmin=10000., r;
    for( int ic=0; ic<nc; ic++ ) {
      EdbClMatch *c = (EdbClMatch *)(s->GetElements()->UncheckedAt(ic));
      r = Sqrt( c->eX*c->eX + c->eY*c->eY );                                     // distance to view center
      if(r<rmin) { rmin=r; s->SetX0(c->eXv+c->eX); s->SetY0(c->eYv+c->eY); s->SetDz(r); }
    }
 }
}

//____________________________________________________________________________________
void EdbViewMatch::CutGrRef()
{
  // remove marginal grains from calculation
  int ngr = eGr.GetEntries();
  for(int i=0; i<ngr; i++) {
    EdbSegment *s = ((EdbSegment*)eGr.UncheckedAt(i));
    int nc = s->GetNelements();
    if     (nc<eNClMin)              s->GetElements()->Clear();
    else if(s->GetDz()>eR2CenterMax) s->GetElements()->Clear();
  }
}

//____________________________________________________________________________________
TNtuple *EdbViewMatch::DumpGr(const char *name)
{
  TNtuple *nt = new TNtuple( name,"grains dump","ig:n:xg:yg:rmin:nc:ic:xc:yc:xcv:ycv:dx:dy:w");
  int ngr = eGr.GetEntries();
  for(int ig=0; ig<ngr; ig++) {
    EdbSegment *s = ((EdbSegment*)eGr.UncheckedAt(ig));
    int nc = s->GetNelements();                                  if(!nc) continue;
    float dx=0, dy=0;
    for( int ic=0; ic<nc; ic++ ) {
      EdbClMatch *c = (EdbClMatch *)(s->GetElements()->UncheckedAt(ic));
      int j = eCorrMap.Jcell(c->eX,c->eY);
      int w = eCorrMap.Bin(j);
      if(w) {
	TArrayD *dxy = (TArrayD*)( eCorrMap.GetObject(j, 0 ) );
	dx = (*dxy)[0];
	dy = (*dxy)[1];
      } else {dx=dy=0;}
      nt->Fill( ig, s->GetNelements(), s->GetX0(), s->GetY0(), s->GetDz(), nc, ic, c->eX, c->eY, c->eXv, c->eYv, dx,dy,w);
    }
  }
  return nt;
}

//____________________________________________________________________________________
void EdbViewMatch::CalculateCorr()
{
  int ngr = eGr.GetEntries();
  Log(2,"EdbViewMatch::CalculateCorr","%d grains used",ngr);
  for(int i=0; i<ngr; i++) {
    EdbSegment *s = ((EdbSegment*)eGr.UncheckedAt(i));
    int nc = s->GetNelements();                                  if(!nc) continue;
    float x0 = s->GetX0(), y0= s->GetY0();
    for( int ic=0; ic<nc; ic++ ) {
      EdbClMatch *c = (EdbClMatch *)(s->GetElements()->UncheckedAt(ic));
      float dx = c->eX+c->eXv - x0;
      float dy = c->eY+c->eYv - y0;
      TArrayD *dxy = (TArrayD*)(eCorrMap.GetObject(c->eX, c->eY, 0));
      if(!dxy) { Log(1,"EdbViewMatch::CalculateCorr","WARNING: null dxy:  ic=%d i=%d   cx,cy: %f %f",ic,i, c->eX, c->eY);       continue; }
      (*dxy)[0] += dx;
      (*dxy)[1] += dy;
      eCorrMap.Fill(c->eX, c->eY);
    }
  }
  
  int nc=eCorrMap.Ncell();
  Log(2,"EdbViewMatch::CalculateCorr","%d cells in the map",nc);
  for( int i=0; i<nc; i++ ) {
    TArrayD *dxy = (TArrayD*)( eCorrMap.GetObject(i, 0 ) );    if(!dxy) continue;
    int w = eCorrMap.Bin(i);                                   if(!w) continue;
    (*dxy)[0] /= w;
    (*dxy)[1] /= w;
  }

}

//____________________________________________________________________________________
void EdbViewMatch::DrawCorrMap()
{
  bool batch = gROOT->IsBatch();
  if(eOutputFile) {
    Log(2,"DrawCorrMap","Save to file %s", eOutputFile->GetName());
    gROOT->SetBatch();
  }
  
  TCanvas *cc = new TCanvas("cc","Corrections map",1200,900);
      
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  float margin=10;
  double minXborder = eCorrMap.Xmin() - margin;
  double maxXborder = eCorrMap.Xmax() + margin;
  double minYborder = eCorrMap.Ymin() - margin;
  double maxYborder = eCorrMap.Ymax() + margin;

  TH2F *hh = new TH2F("hh","Corrections map",100,minXborder,maxXborder,100,minYborder,maxYborder);
  hh->GetXaxis()->SetTitle("X (#mum)");
  hh->GetYaxis()->SetTitle("Y (#mum)");
  hh->Draw();

  TBox *plate = new TBox(eCorrMap.Xmin(),eCorrMap.Ymin(),eCorrMap.Xmax(),eCorrMap.Ymax());
  plate->SetFillColor(16);
  plate->SetFillStyle(3001);
  plate->Draw();

  Double_t meanx=0, meany=0, meanr=0, wtot=0;
  float scale = 15.;
  int nc=eCorrMap.Ncell();
  for( int i=0; i<nc; i++ ) {
    int w = eCorrMap.Bin(i);    if(!w) continue;
    TArrayD *dxy = (TArrayD*)( eCorrMap.GetObject(i, 0 ) );
    float x = eCorrMap.Xj(i);
    float y = eCorrMap.Yj(i);
    float dx = (*dxy)[0];
    float dy = (*dxy)[1];

    TArrow *arrow = new TArrow(x,y,x+scale*dx,y+scale*dy,0.01);
    arrow->SetLineWidth(1);
    arrow->Draw();
    
    meanx += dx;
    meany += dy;
    meanr += sqrt(dx*dx+dy*dy);
    wtot  += w;
  }
  
  meanx /= wtot;
  meany /= wtot;
  meanr /= wtot;
  printf("\nmeanR = %g   meanx = %g   meany = %g  wtot = %f\n", meanr, meanx, meany, wtot);

  eGMap.DrawH2("eGMap","entries/bin")->Write();
  eCorrMap.DrawH2("eCorrMap","entries/bin")->Write();
  cc->Write("corr_map");
  gROOT->SetBatch(batch);
}

//____________________________________________________________________________________
void EdbViewMatch::GenerateCorrectionMatrix(bool addmap)
{
  int nentries = eCorrMap.Ncell();
  Double_t *vx   = new Double_t[nentries];
  Double_t *vy   = new Double_t[nentries];
  Double_t *vdx  = new Double_t[nentries];
  Double_t *vdy  = new Double_t[nentries];

  for(int i=0; i<nentries; i++) {
    int w = eCorrMap.Bin(i);    if(!w) continue;
    TArrayD *arr = (TArrayD*)eCorrMap.GetObject(i,0);
    if(!arr) { Log(1,"EdbViewMatch::GenerateCorrectionMatrix","ERROR: missed bin");  break; }
    vx[i]   = eCorrMap.Xj(i);
    vy[i]   = eCorrMap.Yj(i);
    vdx[i]  = arr->At(0);
    vdy[i]  = arr->At(1);
  }

  TGraph2D *gdx  = new TGraph2D("graphDX","graphDX",nentries,vx,vy,vdx);
  TGraph2D *gdy  = new TGraph2D("graphDY","graphDY",nentries,vx,vy,vdy);
  
  TH2F hdx("hdx","CorrectionMatrix dX",eNXpix, -eXpix*eNXpix/2., eXpix*eNXpix/2., eNYpix, -eYpix*eNYpix/2., eYpix*eNYpix/2.  );
  TH2F hdy("hdy","CorrectionMatrix dY",eNXpix, -eXpix*eNXpix/2., eXpix*eNXpix/2., eNYpix, -eYpix*eNYpix/2., eYpix*eNYpix/2.  );
  
  FILE *fmatr = fopen("correction_matrix.txt","w");
  fprintf(fmatr,"%d %d  %f %f\n", eNXpix, eNYpix, eXpix, eYpix);
  
  for(int i=0; i<eNXpix; i++) {
    for(int j=0; j<eNYpix; j++) {
      float x = (i - 0.5*(eNXpix-1))*eXpix;
      float y = (j - 0.5*(eNYpix-1))*eYpix;
      //float dx = gdx->Interpolate(x,y);
      //float dy = gdy->Interpolate(x,y);
      float dx = ((TArrayD *)eCorrMap.GetObject(x,y,0))->At(0);
      float dy = ((TArrayD *)eCorrMap.GetObject(x,y,0))->At(1);
      
      if(addmap) {
	TArrayD *dxy0 = (TArrayD *)eCorrMap0.GetObject(x,y,0);
	dx += (*dxy0)[0];
	dy += (*dxy0)[1];
      }
      fprintf(fmatr,"%5d %5d %12.6f %12.6f  %11.6f %11.6f\n", i, j, x,y,dx,dy);
 
      hdx.Fill(x,y,dx);
      hdy.Fill(x,y,dy);
    }
  }
  fclose(fmatr);
  
  bool batch = gROOT->IsBatch();
  if(eOutputFile) {
    Log(2,"GenerateCorrectionMatrix","Save to file %s", eOutputFile->GetName());
    gROOT->SetBatch();
  }

  //TCanvas c("c","CorrectionMatrix", 700, 1200 );
  //c.Divide(1,2);
  //c.cd(1);  hdx.Draw("colz");
  //c.cd(2);  hdy.Draw("colz");
  //c.Write();
  hdx.Write("hdx");
  hdy.Write("hdy");
  gdx->Write("gdx");
  gdy->Write("gdy");
  gROOT->SetBatch(batch);
  
}

//____________________________________________________________________________________
void EdbViewMatch::Print()
{
  printf("\n-----------------------------------------------------------------------------------------------------\n");
  int ncl = eCl.GetEntries();
  int ngr = eGr.GetEntries();
  int icgr=0,  iccl=0;
  for(int i=0; i<ngr; i++) {
    int nc = ((EdbSegment*)eGr.At(i))->GetNelements();
    if(nc>=eNClMin) { icgr++;  iccl+=nc; }
  }
  printf("\n%d grains found\n",ngr);
  printf("\n%d grains selected with ncl > %d and closer then %.2f to view center \n",icgr, eNClMin, eR2CenterMax );
  printf("\n%d clusters used in selected grains, mean: %d clusters/grain \n",iccl, (int)(iccl/icgr) );
  printf("Matrix definition: %d %d  %f %f view size: %.2f %.2f\n", eNXpix, eNYpix, eXpix, eYpix, eNXpix*eXpix, eNYpix*eYpix );

  eGMap.PrintStat();
  eCorrMap.PrintStat();
  printf("-----------------------------------------------------------------------------------------------------\n");
}

//____________________________________________________________________________________
void EdbViewMatch::MakeDistortionMap( const char *fname, TEnv &cenv, const char *usefile, const char *addfile )
{
  EdbRun run(fname);
  SetPar(cenv);
  InitGMap();
  InitCorrMap();
  bool do_add=0;
  if(usefile) {
    ReadMatrix2Map(usefile, eCorrMap0);
    do_add=1;
  }

  int      n = run.GetEntries();
  EdbView *v = run.GetView();
  run.GetEntry(0,1);
  float dx0=0;
  float dy0=0;
  int count=0;
  for(int i=0; i<n; i++) {
    run.GetEntry(i,1,1);
    int ncl   = v->Nclusters();
    float xv  = v->GetXview();
    float yv  = v->GetYview();
    int view  = v->GetViewID();
    printf("%6d ", ncl);
    for(int ic=0; ic<ncl; ic++) {
      EdbCluster *c = v->GetCluster(ic);
      if(c->GetArea()>eAreaMin&&c->GetArea()<eAreaMax) {
	if(usefile)
	{
	  TArrayD *dxy0 = (TArrayD *)eCorrMap0.GetObject(c->eX,c->eY,0);
	  if(dxy0){ dx0 = (*dxy0)[0]; dy0 = (*dxy0)[1]; }
	  else dx0=dy0=0;
	}
	EdbClMatch *cm = (EdbClMatch*)(eCl[count++]);
	cm->eX=c->eX-dx0; cm->eY=c->eY-dy0; cm->eZ=c->eZ; 
	cm->eXv=xv; cm->eYv=yv;
	cm->eView=view; cm->eFrame=c->GetFrame();
        AddCluster(cm);
      }
    }
  }
  printf("\nread %d clusters from %s\n",count,fname);

  CalculateGrRef();
  
  eOutputFile = new TFile("map.root","RECREATE");
  //if(eDumpGr) { TNtuple *ntgr = DumpGr("gr"); ntgr->Write(); }
  CutGrRef();
  CalculateCorr();
  DrawCorrMap();
  GenerateCorrectionMatrix(do_add);
  if(eDumpGr) { TNtuple *ntgr = DumpGr("grcut"); ntgr->Write(); }
  eOutputFile->Close();
  Print();
  
}

//____________________________________________________________________________________
void EdbViewMatch::ReadMatrix2Map(const char *file, EdbCell2 &map)
{
  Log(1,"ReadMatrix2Map","%s ",file);
  FILE *f = fopen(file,"r");
  if(!f) { Log(1,"ReadMatrix2Map","file %s not found!",file); return; }
  char str[256]; 
  fgets(str, 256, f);
  int nxpix, nypix;
  float xpix, ypix;
  sscanf(str,"%d %d  %f %f", &nxpix, &nypix, &xpix, &ypix);
  if( nxpix!=eNXpix||nypix!=eNYpix||abs(xpix-eXpix)>0.000001||abs(ypix-eYpix)>0.000001 ) 
  { Log(1,"ReadMatrix2Map","ERROR: matrix definition is different!",file); 
    printf("%d %d  %f %f\n", nxpix, nypix, xpix, ypix);
    return; 
  }
  
  int i0,j0;
  float x0,y0,dx0,dy0;
  for(int i=0; i<eNXpix; i++) {
    for(int j=0; j<eNYpix; j++) {
      if(fgets(str, 256, f)==NULL) Log(1,"GenerateCorrectionMatrix","ERROR: input file is not correct");
      if( sscanf(str, "%d %d %f %f %f %f", &i0, &j0, &x0, &y0, &dx0, &dy0) == 6 )  
      {
	TArrayD *dxy = (TArrayD *)eCorrMap0.GetObject(x0,y0,0);
	(*dxy)[0]+=dx0;
	(*dxy)[1]+=dy0;
	eCorrMap0.Fill(x0, y0);
      }
    }    
  }
  fclose(f);
  
  //eCorrMap0.PrintStat();
  
  int nc=eCorrMap0.Ncell();
  for( int i=0; i<nc; i++ ) {
    TArrayD *dxy = (TArrayD*)( eCorrMap0.GetObject(i, 0 ) );    if(!dxy) continue;
    int w = eCorrMap0.Bin(i);                                   if(!w) continue;
    (*dxy)[0] /= w;
    (*dxy)[1] /= w;
  }
  
}