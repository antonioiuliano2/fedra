//-- Author :  Valeri Tioukov, Igor Kreslo   01.11.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbIP                                                                //
//                                                                      //
// Implementation of Image Processing classes                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "EdbImage.h"
#include "EdbFrame.h"
#include "EdbIP.h"

ClassImp(EdbFIRF)
ClassImp(EdbClustP)
ClassImp(EdbIP)

//====================================================================================
const Float_t EdbFIRF::eg3x3A[]  = {
  1,  1, 1,   
  1, -8, 1,  
  1,  1, 1  };
const Float_t EdbFIRF::egHP1[] = {  
  1,  1, 1,   
  1, -9, 1,  
  1,  1, 1  };
const Float_t EdbFIRF::egHP2[] = {  
  0,  1, 0,   
  1, -5, 1,  
  0,  1, 0  };
const Float_t EdbFIRF::egHP3[] = { 
  -1,  2, -1, 
   2, -5,  2, 
  -1,  2, -1  };
const Float_t EdbFIRF::eg5x5A[] = { 
  4,  4,  4,  4,  4,
  4, -7, -7, -7,  4,
  4, -7, -7, -7,  4,
  4, -7, -7, -7,  4,
  4,  4,  4,  4,  4  };
const Float_t EdbFIRF::eg5x5B[] = { 
  4,  4,  4,  4,  4,
  4, -7, -7, -7,  4,
  4, -7, -8, -7,  4,
  4, -7, -7, -7,  4,
  4,  4,  4,  4,  4  };
const Float_t EdbFIRF::eg5x5C[] = { 
  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,
  1,  1, -24, 1,  1,
  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1  };
const Float_t EdbFIRF::eg6x6A[] = { 
  4,  4,  4,  4,  4,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4,  4,  4,  4,  4,  4  };


//____________________________________________________________________________________
EdbFIRF::EdbFIRF(int cols, int rows)
{
 eArr = new TArrayF();
 eArr->Adopt(cols*rows, new float[cols*rows]);
 eColumns=cols;
 eRows=rows;
 eArr->Reset();
}

//===============================================================================
EdbFIRF::EdbFIRF( const char *firf )
{
  SetName(firf);
  eArr = new TArrayF();
  if( !strcmp(firf,"HP1") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP1);
  } else if( !strcmp(firf,"HP2") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP2);
  } else if( !strcmp(firf,"HP3") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP3);
  } else if( !strcmp(firf,"3x3A") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,eg3x3A);
  } else if( !strcmp(firf,"5x5A") ) {
    eColumns=5;
    eRows=5;
    eArr->Set(eColumns*eRows,eg5x5A);
  } else if( !strcmp(firf,"5x5B") ) {
    eColumns=5;
    eRows=5;
    eArr->Set(eColumns*eRows,eg5x5B);
  } else if( !strcmp(firf,"5x5C") ) {
    eColumns=5;
    eRows=5;
    eArr->Set(eColumns*eRows,eg5x5C);
  } else if( !strcmp(firf,"6x6A") ) {
    eColumns=6;
    eRows=6;
    eArr->Set(eColumns*eRows,eg6x6A);
  }
  Print();
}

//______________________________________________________________________________
EdbFIRF::~EdbFIRF()
{
  if(eArr) { delete eArr; eColumns=0; eRows=0; }
}

//______________________________________________________________________________
void    EdbFIRF::Reflect4()
{
 int X=eColumns;
 int Y=eRows;
 int HX=(int)((eColumns+1)/2);
 int HY=(int)((eRows+1)/2);
 for(int i=0;i<HY;i++){
   for(int j=0;j<HX;j++){
    SetAt(X-j-1,i,Cell(j,i));
    SetAt(X-j-1,Y-i-1,Cell(j,i));
    SetAt(j,Y-i-1,Cell(j,i));
   }
 }
}

//______________________________________________________________________________
void EdbFIRF::Print()
{
  printf("FIR filter: %s\n",GetName());
 float sum=0;
 for(int i=0;i<eRows;i++){
   for(int j=0;j<eColumns;j++){
     printf("%3.1f\t",Cell(j,i));
     sum+=Cell(j,i);
   }
   printf("\n");
 }
 printf("\nSum = %f\n",sum);
}

//______________________________________________________________________________
void EdbFIRF::PrintList()
{
  printf("known 3x3 filtres: \n\t3x3A \n\tHP1 \n\tHP2 \n\tHP3 \n");
  printf("known 5x5 filtres: \n\t5x5A \n\t5x5B \n\t5x5C \n");
  printf("known 6x6 filtres: \n\t6x6A \n");
}

//__________________________________________________________________________________
TH2F* EdbFIRF::ApplyTo(EdbImage* img)
{
  int x0=eColumns/2+1;
  int y0=eRows/2+1;
  int x1=img->Width()-x0-1;
  int y1=img->Height()-y0-1; //margins
  Float_t S;

  TH2F* buf=new TH2F("img","Filtered image",
		     img->Width(),0,img->Width(),img->Height(),0,img->Height());

  for(int y=y0;y<y1;y++) for(int x=x0;x<x1;x++){
   S=0;
   for(int yf=0;yf<eRows;yf++) for(int xf=0;xf<eColumns;xf++){
     S+=img->Pixel(x+xf-eColumns/2,y+yf-eRows/2)*Cell(xf,yf);
   }
   buf->Fill(x,y,S);
  }

  return buf;
}

//====================================================================================
void  EdbClustP::AddPixel( float x, float y, float pix )
{
  float a   = GetArea()+1;
  float v   = GetVolume()+pix;
  float xc  = (X()*GetArea() + x) / a;    // geometrical center
  float yc  = (Y()*GetArea() + y) / a;
  float xcg = (Xcg()*GetVolume() + x*pix) / v;   // center of gravity
  float ycg = (Ycg()*GetVolume() + y*pix) / v;

  SetX(xc);  SetY(yc);  SetArea(a); 
  SetCG(xcg,ycg);       SetVolume(v);
  if( pix > Peak() ) SetPeak(x,y,pix);
}

//-----------------------------------------------------------------------------------
void  EdbClustP::Reset()
{
  Set0();
  SetCG(0,0);
  SetPeak(0,0,0);
}

//====================================================================================
EdbIP::EdbIP()
{
  eFIR=0;
  eThr=0;
}


//-----------------------------------------------------------------------------------
int  EdbIP::Peak8( TH2F *h, float thr )
{
  int xn[8] = { 1, -1, 0,  0, 1,  1, -1, -1 }; //pixel 3x3 suburbs
  int yn[8] = { 0,  0, 1, -1, 1, -1,  1, -1 };

  int nc = h->GetNbinsX();
  int nr = h->GetNbinsY();
  float pix,pp;

  printf("hist: %d %d\n",nc,nr);
  int npeaks=0;
  int ic,ir,in;           // declare here to fix marazmatic eror messages of VC++
  for(ic=2; ic<nc-3; ic++) {
    for(ir=2; ir<nr-3; ir++) {

      pix = (float)(h->GetBinContent(ic,ir)); 
      if( pix < thr)   goto NEXTPIX;

      for(in=0; in<8; in++) {
	pp = (float)(h->GetBinContent( ic+xn[in], ir+yn[in] ));
      	if( pix <= pp ) goto NEXTPIX;
      }
      printf("peak8(%d,%d) =  %f\n",ic,ir,pix); 
      npeaks++;

    NEXTPIX:
      continue;
    }
  }

  return npeaks;
}

//=============================================================================
int  EdbIP::Peak12( TH2F *h, float thr )
{
  int xn[12] = { 1, -1, 0,  0, 1,  1, -1, -1, 2, 0,  0, -2 }; //pixel 12 suburbs
  int yn[12] = { 0,  0, 1, -1, 1, -1,  1, -1, 0, 2, -2,  0 };

  int nc = h->GetNbinsX();
  int nr = h->GetNbinsY();
  float pix,pp;

  float s4, s8;
  printf("hist: %d %d\n",nc,nr);

  int npeaks=0;
  for(int ic=2; ic<nc-3; ic++) {
    for(int ir=2; ir<nr-3; ir++) {

      pix = h->GetBinContent(ic,ir); 
      
      if( pix < thr) goto NEXTPIX;
      int in;

      s4=0;
      for(in=0; in<4; in++) {
	pp = h->GetBinContent( ic+xn[in], ir+yn[in] );
      	if( pix < pp ) goto NEXTPIX;
	s4 += pp;
      }

      s8=0;
      for(in=4; in<12; in++) {
	pp = h->GetBinContent( ic+xn[in], ir+yn[in] );
      	if( pix < pp ) goto NEXTPIX;
	s8 += pp;
      }
      s4 /= 4.;
      s8 /= 8.;
      if( s4  < s8+5 ) goto NEXTPIX;
      if( pix < s4+5 ) goto NEXTPIX;

      npeaks++;
      printf("peak12(%d,%d) =  %f \t %f\t %f\n",ic,ir,pix, s4,s8);
      h->SetBinContent(ic,ir,0,5000);

    NEXTPIX:
      continue;
    }
  }

  return npeaks;
}

//------------------------------------------------------------------------------------
TTree  *EdbIP::InitTree()
{
  TFile *f    = new TFile("clusters.root","RECREATE");
  f->cd();
  TTree *tree = new TTree("clust","clusters tree");  

  EdbClustP *cl = new EdbClustP();
  tree->Branch("cl","EdbClustP",&cl,32000,99);

  return tree;
  //  f.Close();
}

//------------------------------------------------------------------------------------
int  EdbIP::CutBG( EdbFrame *frame )
{
  int npix = 0;
  TH2F *h = eFIR->ApplyTo(frame->GetImage());
  int nc = h->GetNbinsX();
  int nr = h->GetNbinsY();
  float pix;
  int ic,ir;
  for(ic=0; ic<nc; ic++) {
    for(ir=0; ir<nr; ir++) {
      pix = (float)(h->GetBinContent(ic,ir));
      if(pix<=eThr) h->SetBinContent(ic,ir,0);
      else h->SetBinContent(ic,ir,pix-eThr);
    }
  }

  float binmax = h->GetMaximum();
  printf("binmax = %f\n",binmax);
  unsigned char *buf = (unsigned char*)(frame->GetImage()->GetBuffer());

  for(ic=0; ic<nc; ic++) {
    for(ir=0; ir<nr; ir++) {
      pix = (float)(h->GetBinContent(ic,ir));
      if(pix>0)   {
	buf[nc*ir+ic] = (unsigned char)(255.*pix/binmax);
	npix++;
      }
      else buf[nc*ir+ic] =0;
    }
  }
  
  delete h;
  return npix;
}


//------------------------------------------------------------------------------------
int  EdbIP::Clusterize( EdbFrame *frame, TTree *tree )
{
  TH2F *img = eFIR->ApplyTo(frame->GetImage());
  float z = frame->GetZ();
  int   ifr = frame->GetID();
  int ncl = Clusterize( img, eThr, tree, z, ifr );
  delete img;
  return ncl;
}

//------------------------------------------------------------------------------------
int  EdbIP::Clusterize( TH2F *h, float thr, TTree *tree, float z, int ifr )
{
  // c++ remake of my good old "Fire in steppe ;-)"

  int nc = h->GetNbinsX();
  int nr = h->GetNbinsY();
  float pix;
  float wcl;
  int   ncl=0;
  printf("hist: %d %d\n",nc,nr);

  tree->GetDirectory()->cd();
  EdbClustP *cl = 0;
  tree->SetBranchAddress("cl",&cl);
  cl = new EdbClustP();

  for(int ic=2; ic<nc-3; ic++) {
    for(int ir=2; ir<nr-3; ir++) {

      pix = (float)(h->GetBinContent(ic,ir));
      if( pix <= thr)   continue;

      cl->Reset();
      wcl = BurnPix( h, ic,ir, thr, *cl);
      cl->SetZ(z);
      cl->SetFrame(ifr);
      tree->Fill();
      ncl++;
    }
  }

  tree->AutoSave();

  return ncl;
}

//------------------------------------------------------------------------------------
float EdbIP::BurnPix( TH2F *h, int ic, int ir, float thr, EdbClustP &cl )
{
  float pix=h->GetBinContent(ic,ir);
  if( pix<thr )   return 0;

  cl.AddPixel(ic-.5,ir-.5,pix-thr);

  h->SetBinContent(ic,ir,0,-pix);
  pix = 1;
  
  int xn[4] = { 1, -1, 0,  0 };    //pixel closest suburbs
  int yn[4] = { 0,  0, 1, -1 };
  float pp;
  int i,j;

  for(int in=0; in<4; in++) {
    i = ic+xn[in];
    j = ir+yn[in];
    pp = (float)( h->GetBinContent(i,j) );
    if( pp < thr ) continue;
    pix += BurnPix(h,i,j, thr, cl);
  }
  return pix;
}
