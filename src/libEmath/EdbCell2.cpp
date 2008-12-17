//-- Author :  Valeri Tioukov   28-11-2008
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCell2                                                             //
//                                                                      //
// 2-dim collection of objects                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbCell2.h"
#include "EdbLog.h"

ClassImp(EdbH2)
ClassImp(EdbCell2)
ClassImp(EdbPeak2)

  using namespace TMath;

//____________________________________________________________________________
EdbH2::EdbH2()
{
  for(int i=0; i<2; i++) eN[i]=0;
  eNcell=0;
  eNC=0;
}

//____________________________________________________________________________
EdbH2::EdbH2( const EdbH2 &h )
{
  eNC = 0;
  Copy(h);
}

//____________________________________________________________________________
void EdbH2::Copy( const EdbH2 &h )
{
  Delete();
  for(int i=0; i<2; i++)     { 
    eN[i]=h.eN[i];
    eMin[i]=h.eMin[i];
    eMax[i]=h.eMax[i];
    eBin[i]=h.eBin[i];
  }
  eNcell=h.eNcell;
  if(h.eNC) {
    eNC = new Int_t[eNcell];
    memcpy( eNC, h.eNC, sizeof(Int_t)*eNcell );
  }
}

//____________________________________________________________________________
EdbH2::~EdbH2()
{ 
  Delete();
}

//____________________________________________________________________________________
void EdbH2::Delete()
{
  if(eNC)    { delete [] eNC;    eNC = 0;  }
}

//____________________________________________________________________________
int EdbH2::InitH2(int n[2], float min[2], float max[2])
{ 
  for(int i=0; i<2; i++) {
    if(n[i]<1) return 0;
    eN[i]=n[i];
    eMin[i] = min[i];
    eMax[i] = max[i];
    eBin[i] = (eMax[i]-eMin[i])/eN[i];
    //    if(eBin[i]<0.00000001) return 0;
  }
  eNcell = eN[0]*eN[1];
  eNC = new Int_t[eNcell];
  CleanCells();
  return eNcell;
}

//____________________________________________________________________________________
void EdbH2::CleanCells()
{
  if(eNC) memset(eNC,'\0',eNcell*sizeof(Int_t));
}

//____________________________________________________________________________
int EdbH2::Fill(float x, float y, int n)
{ 
  int j = Jcell(x,y);
  if(j<0)              return 0;
  eNC[j] += n;
  return n;
}

//____________________________________________________________________________________
int EdbH2::DiscardHighCells(int nmax)
{
  int ic=0;
  for(int i=0; i<eNcell; i++)  
    if(eNC[i]>nmax) { eNC[i]=0; ic++; }
  return ic;
}

//____________________________________________________________________________________
TH1I *EdbH2::DrawSpectrum(const char *name)
{
  int imax = MaxBin();
  TH1I *h = new TH1I(name,"EdbH2 spectrum plot", imax,0,imax);
  for(int i=0; i<eNcell; i++)    h->Fill( eNC[i] );
  return h;
}

//____________________________________________________________________________________
TH2F *EdbH2::DrawH2(const char *name)
{
  TH2F *h = new TH2F(name, "EdbH2 2D plot", eN[0],eMin[0],eMax[0],eN[1],eMin[1],eMax[1]);
  for(int i=0; i<eN[0]; i++)
    for(int j=0; j<eN[1]; j++)
      h->Fill( X(i), Y(j), eNC[Jcell(i,j)] );
  return h;
}

//____________________________________________________________________________________
void EdbH2::PrintStat()
{
  printf("X:%5d cells in range (%10.2f %10.2f) with bin %10.2f \n", eN[0],eMin[0],eMax[0],eBin[0]);
  printf("Y:%5d cells in range (%10.2f %10.2f) with bin %10.2f \n", eN[1],eMin[1],eMax[1],eBin[1]);
  if(!eNC)  return;
  int nmax=0, ntot=0;
  int nmin=kMaxInt;
  for(int i=0; i<eNcell; i++) {
    if( eNC[i]>nmax ) nmax=eNC[i];
    if( eNC[i]<nmin ) nmin=eNC[i];
    ntot += eNC[i];
  }
  printf("%d objects in %d cells filled as: (%d:%d) with mean= %f\n", 
	 ntot, eNcell, nmin, nmax, 1.*ntot/eNcell);
}

//____________________________________________________________________________________
Long_t EdbH2::Integral()
{
  Long_t ntot=0;
  for(int i=0; i<eNcell; i++) ntot+=eNC[i];
  return ntot;
}

//____________________________________________________________________________________
Long_t EdbH2::Integral(int iv[2], int ir[2])
{
  Long_t ntot=0, nbin=0;
  for(int ix=iv[0]-ir[0]; ix<=iv[0]+ir[0]; ix++) 
    for(int iy=iv[1]-ir[1]; iy<=iv[1]+ir[1]; iy++) 
      {ntot+=Bin(ix,iy); nbin++;}
  return ntot;
}

//____________________________________________________________________________
int EdbH2::MaxBin()
{
  int peak=0;
  for(int i=0; i<eNcell; i++)  if(eNC[i]>peak)  peak =eNC[i];
  return peak;
}


//____________________________________________________________________________
void EdbPeak2::InitPeaks( int npeaks )
{
  eNpeaks=0;
  ePeak.Set(npeaks);
  eMean3.Set(npeaks);
  eMean.Set(npeaks);
}


//____________________________________________________________________________
float EdbPeak2::ProbPeak()
{
  int   iv[2];
  FindPeak(iv);
  int   ir[2]={1,1};              // 3x3 neigbouring
  return ProbPeak(iv,ir);
}

//____________________________________________________________________________
int EdbPeak2::ProbPeaks( int npeaks )
{
  int ir[2]={1,1};
  return ProbPeaks(npeaks,ir);
}

//____________________________________________________________________________
int EdbPeak2::ProbPeaks( int npeaks, int ir[2])
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
float EdbPeak2::ProbPeak(int iv[2], int ir[2])
{
  float prob=0;
  int   npeak      = Bin(iv);
  int   nbinPeak   = (2*ir[0]+1)*(2*ir[1]+1);
  float meanNeib   = 1.*(Integral(iv,ir) - npeak)/(nbinPeak-1);
  float meanNoPeak = 1.*(Integral() - Integral(iv,ir))/(Ncell()-nbinPeak);
  printf("ProbPeak found at (%d %d):  %d  %6.3f  %6.3f \n", iv[0],iv[1],npeak, meanNeib,meanNoPeak);
  ePeak[eNpeaks]  = npeak;
  eMean3[eNpeaks] = meanNeib;
  eMean[eNpeaks]  = meanNoPeak;
  eNpeaks++;
  return prob;
}

//____________________________________________________________________________
int EdbPeak2::WipePeak(int iv[2], int ir[2])
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
int EdbPeak2::FindPeak(int iv[2])
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
  return peak;
}

//____________________________________________________________________________
int EdbPeak2::FindPeak(float v[2])
{
  int iv[2];
  int peak=FindPeak(iv);
  v[0] = X(iv[0]);
  v[1] = Y(iv[1]);
  return peak;
}

//____________________________________________________________________________
EdbCell2::EdbCell2()
{
  eCellLim=0;
  epO=0;
  epC=0;
}

//____________________________________________________________________________
EdbCell2::~EdbCell2()
{ 
  Delete();
}

//____________________________________________________________________________________
void EdbCell2::Delete()
{
  if(epC)    { delete [] epC;    epC = 0;  }
  if(epO)    { delete [] epO;    epO = 0;  }
}


//____________________________________________________________________________
int EdbCell2::InitCell(int maxpercell, int n[2], float min[2], float max[2])
{
  InitH2(n, min, max);
  eCellLim = maxpercell;
  epO     = new TObject*[eNcell*eCellLim];    //- pointers to objects
  epC     = new TObject**[eNcell];            //- pointers to cells
  TObject **po = epO;
  epC[0]=po;
  for(int i=1; i<eNcell; i++) {po+=eCellLim; epC[i]=po;}
  return eNcell;
}

//____________________________________________________________________________
bool EdbCell2::AddObject(int ix, int iy, TObject *obj)
{ 
  int j = Jcell(ix,iy);
  return AddObject(j, obj);
}

//____________________________________________________________________________
bool EdbCell2::AddObject(float x, float y, TObject *obj)
{ 
  int j = Jcell(x,y);
  return AddObject(j, obj);
}

//____________________________________________________________________________
bool EdbCell2::AddObject(int j, TObject *obj)
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
int EdbCell2::SelectObjectsC(int vcent[2], int vdiff[2], TObjArray &arr)
{
  int min[2] = {vcent[0]-vdiff[0],vcent[1]-vdiff[1]};
  int max[2] = {vcent[0]+vdiff[0],vcent[1]+vdiff[1]};
  return SelectObjects(min,max, arr);
}

//____________________________________________________________________________
int EdbCell2::SelectObjects(float min[2], float max[2], TObjArray &arr)
{
  int mi[2] = { IX(min[0]), IY(min[1]) };
  int ma[2] = { IX(max[0]), IY(max[1]) };
  return SelectObjects(mi,ma, arr);
}

//____________________________________________________________________________
int EdbCell2::SelectObjects(int min[2], int max[2], TObjArray &arr)
{
  int nobj  = 0;
  int jcell = 0;
  for(int ix = Max(0,min[0]); ix<=Min(eN[0]-1,max[0]); ix++)
    for(int iy = Max(0,min[1]); iy<=Min(eN[1]-1,max[1]); iy++) 
      {
	jcell = Jcell(ix,iy);
	if(eNC[jcell]<1)      continue;
	for(int k=0; k<eNC[jcell]; k++)   { arr.Add( epC[jcell][k] );  nobj++; }
      }
  //Log(3,"SelectObjects","%d objects selecte in limits min(%d %d) max(%d %d)",nobj,min[0],min[1],max[0],max[1]);
  return nobj;
}
