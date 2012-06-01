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

  eCl.SetClass("EdbClMatch",20000000);
  eGr.SetClass("EdbSegment",10000000);

  eXpix  = 0.30625;
  eYpix  = 0.30714;
  eNXpix = 1280;
  eNYpix = 1024;
  
  eCorrectionMatrixStepX = 10.; //microns - will be tuned in setpar
  eCorrectionMatrixStepY = 10.; //microns - will be tuned in setpar

}

//____________________________________________________________________________________
void EdbViewMatch::SetPar(TEnv &env)
{
  eNClMin      = env.GetValue("viewdist.NClMin" , 20);
  eR2CenterMax = env.GetValue("viewdist.R2CenterMax" , 15.);
  eRmax        = env.GetValue("viewdist.Rmax" ,  1.);

  eXpix  = env.GetValue("viewdist.Xpix" , 0.30625);
  eYpix  = env.GetValue("viewdist.Ypix" , 0.30714);
  eNXpix = env.GetValue("viewdist.NXpix", 1280);
  eNYpix = env.GetValue("viewdist.NYpix", 1024);
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

}

//____________________________________________________________________________________
void EdbViewMatch::InitGMap()
{
  float mi[2] = {-200,-160}, ma[2]={600,500}; 
  int    n[2] = { int((ma[0]-mi[0])/10), int((ma[1]-mi[1])/10) };
  eGMap.InitCell(20, n, mi, ma);
}

//____________________________________________________________________________________
void EdbViewMatch::AddCluster( float x,float y,float z, float xv,float yv, int view, int frame )
{
  //  EdbClMatch *c = new EdbClMatch(x,y,z, xv,yv, view,frame );
  //   eCl.Add(c);
  int ind = eCl.GetEntriesFast();
  EdbClMatch *c = new(eCl[ind]) EdbClMatch(x,y,z, xv,yv, view,frame );
  
  float v[2]={ x+xv,  y+yv };
  int  ir[2]={1,1};
  TObjArray arr;
  EdbSegment *s0 = 0;
  int n = eGMap.SelectObjectsC( v, ir, arr);
  if(n) { 
   float r, rmin=2.*eRmax;
    for(int i=0; i<n; i++) {
      EdbSegment *s = (EdbSegment *)arr.At(i);
      r = Sqrt( (s->GetX0()-v[0])*(s->GetX0()-v[0]) + (s->GetY0()-v[1])*(s->GetY0()-v[1]) );
      if(r<eRmax) if(r<rmin) { rmin=r; s0 = s; }
    }
  }
  if(s0) { s0->AddElement(c); s0->SetX0(v[0]); s0->SetY0(v[1]); s0->SetZ0(z); }
  else  {
    int inds = eGr.GetEntriesFast();
    EdbSegment *s = new(eGr[inds]) EdbSegment(v[0],v[1],z,0,0);
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
    if(nc<eNClMin)        { s->GetElements()->Clear(); continue; }
    float rmin=10000., r;
    for( int ic=0; ic<nc; ic++ ) {
      EdbClMatch *c = (EdbClMatch *)(s->GetElements()->UncheckedAt(ic));
      r = Sqrt( c->eX*c->eX + c->eY*c->eY );                                     // distance to view center
      if(r<rmin) { rmin=r; s->SetX0(c->eXv+c->eX); s->SetY0(c->eYv+c->eY); }
    }
    if(rmin>eR2CenterMax) s->GetElements()->Clear();
 }
}

//____________________________________________________________________________________
void EdbViewMatch::CalculateCorr()
{
  //TFile f("dist.root","RECREATE");
  //TTree tree("dist","dist");
  //tree.AddBranch("x",&x,");
  
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

  Double_t meanx=0, meany=0, wtot=0;
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
    wtot  += w;
  }
  
  meanx /= wtot;
  meany /= wtot;
  printf("\nmeanx = %g    meany = %g  wtot = %f\n", meanx, meany, wtot);
  
  cc->Write("corr_map");
  gROOT->SetBatch(batch);

}

//____________________________________________________________________________________
void EdbViewMatch::GenerateCorrectionMatrix(const char *addfile)
{
  int nentries = eCorrMap.Ncell();
  Double_t *vx   = new Double_t[nentries];
  Double_t *vy   = new Double_t[nentries];
  Double_t *vdx  = new Double_t[nentries];
  Double_t *vdy  = new Double_t[nentries];

  
  for(int i=0; i<nentries; i++) {
    int w = eCorrMap.Bin(i);    if(!w) continue;
    TArrayD *arr = (TArrayD*)eCorrMap.GetObject(i,0);
    //     if(!arr) break;
    vx[i]   = eCorrMap.Xj(i);
    vy[i]   = eCorrMap.Yj(i);

    vdx[i]  = arr->At(0);
    vdy[i]  = arr->At(1);
    //printf("%d %f %f    %f  %f\n", i, vx[i], vy[i], vdx[i], vdy[i] );
  }

  TGraph2D *gdx  = new TGraph2D("graphDX","graphDX",nentries,vx,vy,vdx);
  TGraph2D *gdy  = new TGraph2D("graphDY","graphDY",nentries,vx,vy,vdy);
  
  TH2F hdx("hdx","CorrectionMatrix dX",eNXpix, -eXpix*eNXpix/2., eXpix*eNXpix/2., eNYpix, -eYpix*eNYpix/2., eYpix*eNYpix/2.  );
  TH2F hdy("hdy","CorrectionMatrix dY",eNXpix, -eXpix*eNXpix/2., eXpix*eNXpix/2., eNYpix, -eYpix*eNYpix/2., eYpix*eNYpix/2.  );
  
  FILE *fadd=0;
  if(addfile) fadd = fopen(addfile,"r");
  char str[256];
  
   if(fadd) fgets(str, 256, fadd);    //skip first line
   int iadd, jadd;
   float xadd, yadd, dxadd, dyadd;
  
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
      
      if(fadd) { 
        if(fgets(str, 256, fadd)==NULL) Log(1,"GenerateCorrectionMatrix","ERROR: addfile is not correct");
        if( sscanf(str, "%d %d %f %f %f %f", &iadd, &jadd, &xadd, &yadd, &dxadd, &dyadd) != 6 )  {
          Log(1,"GenerateCorrectionMatrix","ERROR: addfile is not correct");
          break;
        }
        dx += dxadd;
        dy += dyadd;
      }
      fprintf(fmatr,"%5d %5d %12.6f %12.6f  %11.6f %11.6f\n", i, j, x,y,dx,dy);
 
      hdx.Fill(x,y,dx);
      hdy.Fill(x,y,dy);
    }
  }
  fclose(fmatr);
  if(fadd) fclose(fadd);
  
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
  int ncl = eCl.GetEntries();
  int ngr = eGr.GetEntries();
  int icgr=0,  iccl=0;
  for(int i=0; i<ngr; i++) {
    int nc = ((EdbSegment*)eGr.At(i))->GetNelements();
    if(nc>=eNClMin) { icgr++;  iccl+=nc; }
    
    //if(nc>500)   printf("grain %d  with  %d  cl\n", i, nc);
  }
  printf("\n%d clusters processed\n",ncl);
  printf("\n%d grains found\n",ngr);
  printf("\n%d grains used (ncl > %d and closer then %f to center) \n",icgr, eNClMin, eR2CenterMax );
  printf("\n%d clusters used in good grains\n",iccl);
  printf("Matrix definition: %d %d  %f %f\n", eNXpix, eNYpix, eXpix, eYpix);
}
