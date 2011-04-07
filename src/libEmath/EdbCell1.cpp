//-- Author :  Valeri Tioukov   28-11-2008
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCell1                                                             //
//                                                                      //
// 2-dim collection of objects                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TDirectory.h"
#include "TMath.h"
#include "EdbCell1.h"
#include "EdbLog.h"

ClassImp(EdbH1)
ClassImp(EdbCell1)
  //ClassImp(EdbPeak1)

  using namespace TMath;

//____________________________________________________________________________
EdbH1::EdbH1()
{
  Set0();
}
//____________________________________________________________________________
void EdbH1::Set0()
{
  eN=0;
  eNcell=0;
  eNC=0;
}

//____________________________________________________________________________
EdbH1::EdbH1( const EdbH1 &h )
{
  eNC = 0;
  Copy(h);
}

//____________________________________________________________________________
void EdbH1::Copy( const EdbH1 &h )
{
  Delete();
  eN=h.eN;
  eMin=h.eMin;
  eMax=h.eMax;
  eBin=h.eBin;
  eNcell=h.eNcell;
  if(h.eNC) {
    eNC = new Int_t[eNcell];
    memcpy( eNC, h.eNC, sizeof(Int_t)*eNcell );
  }
}

//____________________________________________________________________________
EdbH1::~EdbH1()
{ 
  Delete();
}

//____________________________________________________________________________________
void EdbH1::Delete()
{
  if(eNC)    { delete [] eNC;    eNC = 0;  }
}

//____________________________________________________________________________
int EdbH1::InitH1(int n, float min, float max)
{ 
  if(n<1) return 0;
  eN=n;
  eMin = min;
  eMax = max;
  eBin = (eMax-eMin)/eN;
  eNcell = eN;
  eNC = new Int_t[eNcell];
  CleanCells();
  return eNcell;
}

//____________________________________________________________________________________
void EdbH1::CleanCells()
{
  if(eNC) memset(eNC,'\0',eNcell*sizeof(Int_t));
}

//____________________________________________________________________________
int EdbH1::Fill(float x, int n)
{ 
  int j = Jcell(x);
  if(j<0)              return 0;
  eNC[j] += n;
  return n;
}

//____________________________________________________________________________________
int EdbH1::DiscardHighCells(int nmax)
{
  int ic=0;
  for(int i=0; i<eNcell; i++)  
    if(eNC[i]>nmax) { eNC[i]=0; ic++; }
  return ic;
}

//____________________________________________________________________________________
TH1I *EdbH1::DrawSpectrum(const char *name)
{
  int imax = MaxBin();
  TH1I *h = new TH1I(name,"EdbH1 spectrum plot", imax,0,imax);
  for(int i=0; i<eNcell; i++)    h->Fill( eNC[i] );
  return h;
}

//____________________________________________________________________________________
TH1F *EdbH1::DrawH1(const char *name, const char *title)
{
  TObject *obj=0;
  if((obj=gDirectory->FindObject(name))) delete obj;
  TH1F *h = new TH1F(name, title, eN,eMin,eMax);
  for(int i=0; i<eN; i++)    h->Fill( X(i), eNC[Jcell(i)] );
  return h;
}

//____________________________________________________________________________________
void EdbH1::PrintStat()
{
  printf("X:%5d cells in range (%10.2f %10.2f) with bin %10.2f \n", eN,eMin,eMax,eBin);
  if(!eNC)  return;
  int nmax=0, ntot=0;
  int nmin=kMaxInt;
  for(int i=0; i<eNcell; i++) {
    if( eNC[i]>nmax ) nmax=eNC[i];
    if( eNC[i]<nmin ) nmin=eNC[i];
    ntot += eNC[i];
  }
  printf("%d entries in %d bins in the range: (%d:%d) with mean occupancy= %f\n", 
	 ntot, eNcell, nmin, nmax, 1.*ntot/eNcell);
}
//____________________________________________________________________________________
void EdbH1::Print()
{
  PrintStat();
  for(int i=0; i<N(); i++) 
    printf("%d (%f) = %d\n", i, X(i), Bin(i) );
}


//____________________________________________________________________________________
Long_t EdbH1::Integral()
{
  Long_t ntot=0;
  for(int i=0; i<eNcell; i++) ntot+=eNC[i];
  return ntot;
}

//____________________________________________________________________________________
Long_t EdbH1::Integral(int iv, int ir)
{
  Long_t ntot=0, nbin=0;
  for(int ix=iv-ir; ix<=iv+ir; ix++) 
      {ntot+=Bin(ix); nbin++;}
  return ntot;
}

//____________________________________________________________________________
int EdbH1::MaxBin()
{
  int peak=0;
  for(int i=0; i<eNcell; i++)  if(eNC[i]>peak)  peak =eNC[i];
  return peak;
}



/*

//____________________________________________________________________________
void EdbPeak1::InitPeaks( int npeaks )
{
  eNpeaks=0;
  ePeak.Set(npeaks);
  eMean3.Set(npeaks);
  eMean.Set(npeaks);
  eNorm = 1.;  // default norm factor;
}

//____________________________________________________________________________
void EdbPeak1::Print()
{
  printf("%d peaks:\n", eNpeaks);
  for(int i=0; i<eNpeaks; i++) 
	  printf("%d :  %f %f %f \n", i, ePeak[i], eMean3[i], eMean[i]);
}


//____________________________________________________________________________
float EdbPeak1::ProbPeak()
{
  int   iv[2];
  FindPeak(iv);
  int   ir[2]={1,1};              // 3x3 neigbouring
  return ProbPeak(iv,ir);
}

//____________________________________________________________________________
int EdbPeak1::ProbPeaks( int npeaks )
{
  int ir[2]={1,1};
  return ProbPeaks(npeaks,ir);
}

//____________________________________________________________________________
int EdbPeak1::ProbPeaks( int npeaks, int ir[2])
{
  int   iv[2], ic=0;
  for(int i=0; i<npeaks; i++) {
    FindPeak(iv);
    ProbPeak(iv,ir);
    WipePeak(iv,ir);
    ic++;
  }
  return ic;
}

//____________________________________________________________________________
float EdbPeak1::ProbPeak(int iv[2], int ir[2])
{
  float prob=0;
  int   npeak      = Bin(iv);
  int   nbinPeak   = (2*ir[0]+1)*(2*ir[1]+1);
  float meanNeib   = 1.*(Integral(iv,ir) - npeak)/(nbinPeak-1);
  float meanNoPeak = 1.*(Integral() - Integral(iv,ir))/(Ncell()-nbinPeak);
  printf("ProbPeak found at (%3d %3d): %4d  %6.3f  %6.3f \n", iv[0],iv[1],npeak, meanNeib,meanNoPeak);
  ePeak[eNpeaks]  = npeak;
  eMean3[eNpeaks] = meanNeib;
  eMean[eNpeaks]  = meanNoPeak;
  eNpeaks++;
  return prob;
}

//____________________________________________________________________________
int EdbPeak1::WipePeak(int iv[2], int ir[2])
{
  int mean = (int)Mean();
  int nbin=0;
  for(int ix=iv[0]-ir[0]; ix<=iv[0]+ir[0]; ix++) 
    for(int iy=iv[1]-ir[1]; iy<=iv[1]+ir[1]; iy++) 
      if(Jcell(ix,iy)>-1)
	{eNC[Jcell(ix,iy)] = mean; nbin++;}
  return nbin;
}

//____________________________________________________________________________
int EdbPeak1::FindPeak(int iv[2])
{
  int peak=0;
  for(int ix=0; ix<eN[0]; ix++)
    for(int iy=0; iy<eN[1]; iy++) {
      int j = Jcell(ix,iy);
      if(eNC[j]>peak) {
        peak =eNC[j];
        iv[0] = ix;
        iv[1] = iy;
      }
    }
  Log(3,"EdbPeak1::FindPeak","of %d at %d %d", peak, iv[0],iv[1] );
  return peak;
}

//____________________________________________________________________________
int EdbPeak1::FindPeak(float v[2])
{
  int iv[2];
  int peak=FindPeak(iv);
  v[0] = X(iv[0]);
  v[1] = Y(iv[1]);
  return peak;
}

//____________________________________________________________________________
int EdbPeak1::FindPeak(float &x, float &y)
{
  int iv[2];
  int peak=FindPeak(iv);
  x = X(iv[0]);
  y = Y(iv[1]);
  return peak;
}

//____________________________________________________________________________
float EdbPeak1::EstimatePeakVolumeSafe(int ipeak)
{
  // add to the background one standard deviation for the safety
  if(ipeak>(eNpeaks-1)||ipeak<0)  return 0;
  return ePeak[ipeak] + 8*eMean3[ipeak] - 9*(eMean[ipeak] + Sqrt(eMean[ipeak]));
}

//____________________________________________________________________________
float EdbPeak1::EstimatePeakVolume(int ipeak)
{
  if(ipeak>(eNpeaks-1)||ipeak<0)  return 0;
  return ePeak[ipeak] + 8*eMean3[ipeak] - 9*eMean[ipeak];
}

//____________________________________________________________________________
float EdbPeak1::Xmean()
{
  Double_t mean=0, sum=0;
  for(int ix=0; ix<eN[0]; ix++) {
    float x = X(ix);
    for(int iy=0; iy<eN[1]; iy++) {
      int j = Jcell(ix,iy);
      mean += x*eNC[j];
      sum  += eNC[j];
    }
  }
  mean /= sum;
  return mean;
}

//____________________________________________________________________________
float EdbPeak1::Ymean()
{
  Double_t mean=0, sum=0;
  for(int iy=0; iy<eN[1]; iy++) {
    float y = Y(iy);
    for(int ix=0; ix<eN[0]; ix++) {
      int j = Jcell(ix,iy);
      mean += y*eNC[j];
      sum  += eNC[j];
    }
  }
  mean /= sum;
  return mean;
}

//____________________________________________________________________________
float EdbPeak1::FindGlobalPeak(float &x, float &y, float ratio)
{
  // to find the center of wide multibin peak select ratio*Nbin of highest bins and calculate CoG
  int n = Ncell();
  TArrayI ind(n);
  Sort(n, eNC , ind.GetArray(), 0);  // sort encreasing order
  Int_t nwipe = Min( (Int_t)((1.-ratio)*n), n-1 );
  double mean=0;
  for(int i=0; i<nwipe; i++) {
    mean += eNC[ind[i]];
    eNC[ind[i]] = 0;
  }
  mean /= nwipe;
  x = Xmean();
  y = Ymean();
  float peakvolume = Integral() - (n-nwipe)*mean;
  Log(3,"EdbPeak1::FindGlobalPeak","of volume %8.2f at %10.2f %10.2f using %d bins", 
      peakvolume,x,y, n-nwipe );
  return peakvolume;
}

//______________________________________________________________________________
float EdbPeak1::Smooth(Option_t *option)
{
   // Smooth bin contents of this 2-d histogram using kernel algorithms
   // similar to the ones used in the raster graphics community.
   // Bin contents in the active range are replaced by their smooth values.
   //
   // This code is copied from root_5.22 mainly to not depend on the newest root versions
   // Adopted to fedra by VT
   // to speedup processing do all calculations as integer, so the histogram do not normalised at output
   // return value - norm-factor of the kernel

   if(NX()<1||NY()<1)  return 1.;

   Int_t k5a[5][5] =  { { 0, 0, 1, 0, 0 }, 
			{ 0, 2, 2, 2, 0 }, 
			{ 1, 2, 5, 2, 1 }, 
			{ 0, 2, 2, 2, 0 }, 
			{ 0, 0, 1, 0, 0 } };   // norm = 25
   Int_t k5b[5][5] =  { { 0, 1, 2, 1, 0 },
			{ 1, 2, 4, 2, 1 },
			{ 2, 4, 8, 4, 2 },
			{ 1, 2, 4, 2, 1 },
			{ 0, 1, 2, 1, 0 } };
   Int_t k3a[3][3] =  { { 0, 1, 0 },
			{ 1, 2, 1 },
			{ 0, 1, 0 } };      // norm = 6
   Int_t k3b[3][3] =  { { 1, 2, 1 },        //gauss smoothing
			{ 2, 4, 2 },
			{ 1, 2, 1 } };      // norm = 16

   Int_t ksize_x, ksize_y;
   TString opt = option;
   opt.ToLower();
   Int_t *kernel = &k5a[0][0];
   if (opt.Contains("k5a")) {
     kernel = &k5a[0][0];
     ksize_x = ksize_y = 5;
   }
   else if (opt.Contains("k5b")) {
     kernel = &k5b[0][0];
     ksize_x = ksize_y = 5;
   }
   else if (opt.Contains("k3a")) {
      kernel = &k3a[0][0];
      ksize_x = ksize_y = 3;
   } 
   else if (opt.Contains("k3b")) {
      kernel = &k3b[0][0];
      ksize_x = ksize_y = 3;
   } 
   else return 1.;

   // Kernel tail sizes (kernel sizes must be odd for this to work!) 
   Int_t x_push = (ksize_x-1)/2;
   Int_t y_push = (ksize_y-1)/2; 

   Int_t norm=0;
   for(int i=0; i<ksize_x; i++)
     for(int j=0; j<ksize_y; j++) norm += kernel[i*ksize_y +j];

   int nx = NX(), ny= NY();

   EdbH1 buf( *((EdbH1*)this) );

   Long_t content;
   int bin=0, k=0;

   for (int i=0; i<nx; i++){             // input hist
     for (int j=0; j<=ny; j++) { 
       
       content = 0;
       for (int n=0; n<ksize_x; n++) {          // kernel
	 for (int m=0; m<ksize_y; m++) { 
	   Int_t xb = i+(n-x_push);
	   Int_t yb = j+(m-y_push); 
	   bin = buf.Bin(xb,yb);                      // out of dimensions - return 0;
	   if (!bin) continue;
	   k = kernel[n*ksize_y +m];
	   if (!k) continue;
	   content += k*bin;
	 }
       }
       //SetBin( i, j, Nint(1.*content/norm) );
       SetBin( i, j, content );
     } 
   } 

   eNorm = (float)norm; 
   return eNorm;
}

*/


//____________________________________________________________________________
EdbCell1::EdbCell1()
{
  eCellLim=0;
  epO=0;
  epC=0;
}

//____________________________________________________________________________
EdbCell1::~EdbCell1()
{ 
  Delete();
}

//____________________________________________________________________________________
void  EdbCell1::PrintStat()
{
  printf("EdbCell1 limit = %4d\n",eCellLim);
  ((EdbH1*)this)->PrintStat();
}

//____________________________________________________________________________________
void EdbCell1::Delete()
{
  if(epC)    { delete [] epC;    epC = 0;  }
  if(epO)    { delete [] epO;    epO = 0;  }
}

//____________________________________________________________________________
int EdbCell1::InitCell(int maxpercell, int n, float min, float max)
{
  InitH1(n, min, max);
  Log(3,"EdbCell1::InitCell","reserve space for maxpercell(%d) * eNcell(%d) = %d objects", 
      maxpercell, eNcell, maxpercell*eNcell);
  eCellLim = maxpercell;
  epO     = new TObject*[eNcell*eCellLim];    //- pointers to objects
  epC     = new TObject**[eNcell];            //- pointers to cells
  TObject **po = epO;
  epC[0]=po;
  for(int i=1; i<eNcell; i++) {po+=eCellLim; epC[i]=po;}
  return eNcell;
}

//____________________________________________________________________________
bool EdbCell1::AddObject(float x, TObject *obj)
{ 
  int j = Jcell(x);
  return AddObject(j, obj);
}

//____________________________________________________________________________
bool EdbCell1::AddObject(int j, TObject *obj)
{ 
  if(j>=eNcell)        return 0;
  if(j<0)              return 0;
  if(eNC[j]>=eCellLim) return 0;
  if(!obj)             return 0;
  epC[j][eNC[j]] = obj;
  eNC[j]++;
  return 1;
}

//____________________________________________________________________________
int EdbCell1::SelectObjectsC(int vcent, int vdiff, TObjArray &arr)
{
  int min = vcent-vdiff;
  int max = vcent+vdiff;
  return SelectObjects(min,max, arr);
}

//____________________________________________________________________________
int EdbCell1::SelectObjects(float min, float max, TObjArray &arr)
{
  return SelectObjects( IX(min), IX(max), arr);
}

//____________________________________________________________________________
int EdbCell1::SelectObjects(int min, int max, TObjArray &arr)
{
  int nobj  = 0;
  int jcell = 0;
  for(int ix = Max(0,min); ix<=Min(eN-1,max); ix++)
    {
      jcell = Jcell(ix);
      if(eNC[jcell]<1)      continue;
      for(int k=0; k<eNC[jcell]; k++)   { arr.Add( epC[jcell][k] );  nobj++; }
    }
  return nobj;
}

//____________________________________________________________________________
int EdbCell1::SelectObjects(TObjArray &arr)
{
  // select all objects
  int nobj  = 0;
  for(int i = 0; i<eNcell; i++)
    {
      if(eNC[i]<1)      continue;
      for(int k=0; k<eNC[i]; k++)   { arr.Add( epC[i][k] );  nobj++; }
    }
  return nobj;
}


