//-- Author :  Valeri Tioukov   28-11-2008
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPositionAlignment                                                 //
//                                                                      //
// fast position alignment of 2 lists of segments                       //
// (developed for the compton alignment procedure)                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEventList.h"
#include "EdbLog.h"
#include "EdbPositionAlignment.h"

ClassImp(EdbPositionAlignment)

using namespace TMath;

//---------------------------------------------------------------------
EdbPositionAlignment::EdbPositionAlignment()
{
  eXcell=eYcell=50;           // bin size (for example 50 microns)
  eDTXmax=eDTYmax=0.15;       // max angular acceptance (ex: 0.15) for the coincidence
  eDXmin=eDYmin=5;            // min position difference for the doublets cutout
  eDTXmin=eDTYmin=0.005;      // min angular  difference for the doublets cutout

  eX0=eY0=0;

  eZ1from=eZ1to=eZ1peak=0;
  eZ2from=eZ2to=eZ2peak=0;
  eNZ1step=eNZ2step=1;
  eNpeak = 0;
  eAff=0;

  ePosTree=0;
}

//---------------------------------------------------------------------
EdbPositionAlignment::~EdbPositionAlignment()
{
  eComb1.Clear();
  eComb2.Clear();
  SafeDelete(eAff);
}

//---------------------------------------------------------------------
void EdbPositionAlignment::PrintStat()
{
  ePC1.PrintStat();
  ePC2.PrintStat();
  printf("Z variate as:  pat1: %d (%6.1f %6.1f)  pat2: %d (%6.1f %6.1f) \n", 
	 eNZ1step,eZ1from,eZ1to,  eNZ2step,eZ2from,eZ2to); 
}

//---------------------------------------------------------------------
void EdbPositionAlignment::PrintSummary()
{
  printf("Alignment summary: ncomb = %d\n",eComb1.GetEntries());
  printf("%d coinsidence in the peak with    dXY: (%6.2f %6.2f)    dz12: (%6.1f %6.1f)   at XY: (%10.1f %10.1f)\n",
	 eNpeak, eXpeak,eYpeak,eZ1peak,eZ2peak, eX0,eY0 );
}

//---------------------------------------------------------------------
bool EdbPositionAlignment::ActivatePosTree()
{
  if(ePosTree) SafeDelete(ePosTree);
  ePosTree = new TTree("postree","postree (s1:s2)");
  EdbSegP *s1 =0, *s2 =0;
  Float_t dz1,dz2;
  Int_t flag;
  ePosTree->Branch("s1.",  "EdbSegP",&s1,32000,99);
  ePosTree->Branch("s2.",  "EdbSegP",&s2,32000,99);
  ePosTree->Branch("dz1",  &dz1,"dz1/F");
  ePosTree->Branch("dz2",  &dz2,"dz2/F");
  ePosTree->Branch("flag", &flag,"flag/I");
  return true;
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillPosTree(EdbPattern &p1, EdbPattern &p2, float dz1, float dz2, int flag)
{
  if(!ePosTree)    return 0;
  int n = p1.N();
  TObjArray arr1(n),arr2(n);
  for(int i=0; i<n; i++) {
    arr1.Add( p1.GetSegment(i) );
    arr2.Add( p2.GetSegment(i) );
  }
  return FillPosTree(arr1,arr2, dz1, dz2, flag);
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillPosTree(TObjArray &arr1,TObjArray &arr2, float dz1, float dz2, int flag)
{
  if(!ePosTree)    return 0;
  EdbSegP *sb1 = new EdbSegP();
  EdbSegP *sb2 = new EdbSegP();
  ePosTree->SetBranchAddress("s1." , &sb1);
  ePosTree->SetBranchAddress("s2." , &sb2);
  ePosTree->SetBranchAddress("dz1" , &dz1);
  ePosTree->SetBranchAddress("dz2" , &dz2);
  ePosTree->SetBranchAddress("flag", &flag);
  
  EdbSegP *s1, *s2;
  int n = arr1.GetEntries();
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)arr1.At(i);
    s2 = (EdbSegP*)arr2.At(i);
    sb1->Copy(*s1);
    sb2->Copy(*s2);
    //  sb1->SetZ(sb1->Z()+dz1); sb1->SetX(x1); sb1->SetY(y1);
    //  sb2->SetZ(sb2->Z()+dz2); sb2->SetX(x2); sb2->SetY(y2);
    ePosTree->Fill();
  }
  return n;
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillArrays(EdbPattern &p1, EdbPattern &p2)
{
  ePC1.Reset();
  ePC2.Reset();

  float marg = 0.0001;
  float min1[2] = { p1.Xmin()-marg, p1.Ymin()-marg };
  float max1[2] = { p1.Xmax()+marg, p1.Ymax()+marg };
  int     n1[2] = { (int)((max1[0]-min1[0])/eXcell+1), (int)((max1[1]-min1[1])/eYcell+1) };
  eXcell = (max1[0]-min1[0])/n1[0];
  eYcell = (max1[1]-min1[1])/n1[1];
  float min2[2] = { min1[0]-eXcell, min1[1]-eYcell };
  float max2[2] = { max1[0]+eXcell, max1[1]+eYcell };
  int   n2[2]   = { n1[0]+2     , n1[1]+2      };

  if( n1[0]<1 || n1[1]<1 )           return 0;
  int meancell1 = (int)(p1.N()/n1[0]/n1[1]);
  int meancell2 = (int)(p2.N()/n2[0]/n2[1]);
  int maxcell   = (int)Max(meancell1,meancell2)+10;
  maxcell += (int)(5*Sqrt(maxcell));                         // define safe value for max/cell limit

  ePC1.InitCell(maxcell, n1, min1, max1);
  ePC2.InitCell(maxcell, n2, min2, max2);

  int nc1 = ePC1.FillCell(p1);
  int nc2 = ePC2.FillCell(p2);

  Log(3,"FillArrays","%d and %d cells filled", nc1,nc2);
  return nc1;
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillArrays(TObjArray &arr1, TObjArray &arr2, float xymin[2], float xymax[2] )
{
  ePC1.Reset();
  ePC2.Reset();

  float marg = 0.0001;
  float min1[2] = { xymin[0]-marg, xymin[1]-marg };
  float max1[2] = { xymax[0]+marg, xymax[1]+marg };
  int     n1[2] = { (int)((max1[0]-min1[0])/eXcell+1), (int)((max1[1]-min1[1])/eYcell+1) };
  eXcell = (max1[0]-min1[0])/n1[0];
  eYcell = (max1[1]-min1[1])/n1[1];
  float min2[2] = { min1[0]-eXcell, min1[1]-eYcell };
  float max2[2] = { max1[0]+eXcell, max1[1]+eYcell };
  int   n2[2]   = { n1[0]+2     , n1[1]+2      };

  if( n1[0]<1 || n1[1]<1 )           return 0;
  int entr1 = arr1.GetEntriesFast();
  int entr2 = arr2.GetEntriesFast();
  int meancell1 = (int)(entr1/n1[0]/n1[1]);
  int meancell2 = (int)(entr2/n2[0]/n2[1]);
  int maxcell   = (int)Max(meancell1,meancell2)+10;
  maxcell += (int)(5*Sqrt(maxcell));                         // define safe value for max/cell limit

  ePC1.InitCell(maxcell, n1, min1, max1);
  ePC2.InitCell(maxcell, n2, min2, max2);

  int nc1 = ePC1.FillCell(arr1);
  int nc2 = ePC2.FillCell(arr2);

  Log(3,"FillArrays","%d and %d cells filled", nc1,nc2);
  return nc1;
}

//---------------------------------------------------------------------
int    EdbPositionAlignment::SelectZone( float min[2], float max[2], TObjArray &arr1, TObjArray &arr2, float maxDens)
{
  // density unit is N/100/100 microns
  int ic=0;
  int nmax = (int)(maxDens*(max[0]-min[0])*(max[1]-min[1]) /100/100);
  ic += SelectZoneSide(ePC1, min, max, arr1, nmax);
  float min2[2] = {min[0]-eXcell, min[1]-eYcell };
  float max2[2] = {max[0]+eXcell, max[1]+eYcell };
  nmax = (int)(maxDens*(max2[0]-min2[0])*(max2[1]-min2[1]) /100/100);
  ic += SelectZoneSide(ePC2, min2, max2, arr2, nmax);
  return ic;
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::SelectZoneSide( EdbPatCell2 &pc, float min[2], float max[2], TObjArray &arr, int nmax)
{
  int ic=0;
  TObjArray a(arr.GetSize());
  pc.SelectObjects(min,max,a);
  int n = a.GetEntriesFast();

  EdbSegP *s;

  if(n>nmax)     {
    TArrayF   comptonity(n);   // shoud be defined as: the bigger - the better
    TArrayI   ind(n);
    for(int i=0; i<n; i++) {
      s = (EdbSegP*)a.At(i);
      if(s->Flag()<0) comptonity[i] =0;
      else            comptonity[i] = s->W()*100 - s->Theta();  // proportional to ngrains and low theta
    }
    Sort(n,comptonity.GetArray(),ind.GetArray(),1);  // sort in decreasing order
    for(int i=0; i<nmax; i++) {
      s = (EdbSegP*)a.At(ind[i]);
      arr.Add(s);  ic++;
    }
  }
  else {
    for(int i=0; i<n; i++) {
      s = (EdbSegP*)a.UncheckedAt(i);
      if(s->Flag()>-1) { arr.Add(s);    ic++; }
    }
  }

  return ic;
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillCombinations()
{
  float min[2] = {ePC1.Xmin(), ePC1.Ymin()};
  float max[2] = {ePC1.Xmax(), ePC1.Ymax()};
  return FillCombinations(min,max);
}


//---------------------------------------------------------------------
int EdbPositionAlignment::FillCombinations(float min[2], float max[2])
{
  // the cells must be already filled

  ePC1.eDXmin  = eXcell;   ePC1.eDYmin  = eYcell;
  ePC1.eDTXmin = eDTXmax;  ePC1.eDTYmin = eDTYmax;
  eComb1.Clear();
  eComb2.Clear();
  int ir[2] = {1,1};
  int ncomb = ePC1.FillCombinations(ePC2, ir, eComb1, eComb2, min, max);
  Log(2,"FillCombinations","%d combinations selected", ncomb);
  return ncomb;
}

//---------------------------------------------------------------------
void  EdbPositionAlignment::PositionPlot( float dz1, float dz2, EdbH2 &hd)
{
  // dz1,dz2 offsets in Z, 
  // output: hd - differential hist, should be already defined

  int n1 = eComb1.GetEntries();
  EdbSegP *s1,*s2;
  float x1,x2,y1,y2;
  for(int i=0; i<n1; i++) {
    s1 = (EdbSegP*)eComb1.At(i);
    s2 = (EdbSegP*)eComb2.At(i);
    x1 = Xcorr(*s1,dz1);
    y1 = Ycorr(*s1,dz1);
    x2 = Xcorr(*s2,dz2);
    y2 = Ycorr(*s2,dz2);
    hd.Fill(x2-x1,y2-y1);
  }
}

//---------------------------------------------------------------------
void  EdbPositionAlignment::Zselection(EdbH2 &hd)
{
  if(eNZ1step<1 || eNZ2step<1) {
    Log(1,"Zselection","ERROR: eNZ1step = %d, eNZ2step = %d",eNZ1step,eNZ2step); 
    return; 
  }

  float maxMax=0, meanMax=0, meanMean=0, meanbin=0;
  int   maxbin=0,ic=0;

  int      nz[2]  = {eNZ1step,eNZ2step};
  float  minz[2]  = {Min(eZ1from,eZ1to), Min(eZ2from,eZ2to)};
  float  maxz[2]  = {Max(eZ1from,eZ1to), Max(eZ2from,eZ2to)};
  EdbH2  hz12;
  hz12.InitH2(nz, minz, maxz);
  //hz12.PrintStat();

  float dz1,dz2;
  for(int i1=0; i1<nz[0]; i1++)
    {
      for(int i2=0; i2<nz[1]; i2++)
	{
	  dz1 = hz12.X(i1);
	  dz2 = hz12.Y(i2);
	  //printf("dz1,dz2= %f %f\n",dz1,dz2);
	  hd.CleanCells();
	  PositionPlot(dz1,dz2,hd);
	  maxbin = hd.MaxBin();
	  if(maxbin>maxMax)  {                // new peak
	    maxMax=maxbin;
	    eZ1peak=dz1;
	    eZ2peak=dz2;
	    eHpeak.Copy(hd);
	  }

	  hz12.Fill(dz1,dz2,maxbin);

	  meanbin   =  hd.Mean();
	  meanMax  += maxbin;
	  meanMean += meanbin;
	  ic++;
	  Log(3,"Zselection","max: %5d mean: %8.4f at dZ(%7.2f %7.2f)", maxbin,meanbin, dz1, dz2);
	}
    }
  meanMax  /= ic;
  meanMean /= ic;

  eHDZ.Copy(hz12);

  float vpeak[2];
  EdbPeak2 p2d(eHpeak);
  p2d.FindPeak(vpeak);
  eXpeak=vpeak[0];
  eYpeak=vpeak[1];

  Log(2,"Zselection","maxMax: %8.2f meanMax: %8.4f  meanMean: %8.4f at dZ(%7.2f %7.2f)  XY(%7.2f %7.2f)",  
      maxMax, meanMax, meanMean, eZ1peak, eZ2peak, eXpeak,eYpeak);
}


//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::Align()
{
  // find the best alignment of the given zone

  int n[2] = { (int)(2*(eXcell/eBinX)+1), (int)(2*(eYcell/eBinY)+1) };
  float min[2] = {-eXcell,-eYcell};
  float max[2] = { eXcell, eYcell};
  EdbH2 hd;           // define the differential hist for the peaks search
  hd.InitH2(n, min, max);
  Zselection(hd);     // roughly define eZ1peak, eZ2peak, eXpeak,eYpeak
 
  FillPosTree( eComb1, eComb2, eZ1peak, eZ2peak, 0);

  EdbPattern p1, p2;
  int npeak = SelectPeak(p1,p2, eBinX,  eBinY );

  FillPosTree( p1, p2, 0, 0, 1);
  
  return npeak;
}

//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::SelectPeak(EdbPattern &p1,EdbPattern &p2, 
				     float dxMax, float dyMax, float dz1, float dz2)
{
  // Select the couples formed the peak
  // assumed that the peak eXpeak, eYpeak is approximately found
  // Input: dz1,dz2 - for projection
  //        dxMax, dYmax - for selection
  // Output p1,p2 with found segments

  int n1 = eComb1.GetEntries();

  TH2F *hpeak = new TH2F("hpeak","Selected peak",100,-dxMax+eXpeak,dxMax+eXpeak,100,-dyMax+eYpeak,dyMax+eYpeak);
  int ic=0;
  EdbSegP *s1,*s2;
  EdbSegP s;
  float dx,dy;
  for(int i=0; i<n1; i++) {
    s1 = (EdbSegP*)eComb1.At(i);
    s2 = (EdbSegP*)eComb2.At(i);
    dx = Xcorr(*s2,dz2) - Xcorr(*s1,dz1);
    dy = Ycorr(*s2,dz2) - Ycorr(*s1,dz1);
    if(Abs(dx-eXpeak)>dxMax) continue;
    if(Abs(dy-eYpeak)>dyMax) continue;
    s.Copy(*s1);    s.SetX(Xcorr(*s1,dz1));    s.SetY(Ycorr(*s1,dz1)); s.SetZ(0);   p1.AddSegment(s);
    s.Copy(*s2);    s.SetX(Xcorr(*s2,dz2));    s.SetY(Ycorr(*s2,dz2)); s.SetZ(0);   p2.AddSegment(s);
    hpeak->Fill(dx,dy);
    ic++;
  }
  eNpeak = ic;
  eXpeak = hpeak->GetMean(1);
  eYpeak = hpeak->GetMean(2);
  Log(2,"SelectPeak","%d coincidences selected at XY:(%f %f)  with RMS:(%f %f)  (%5.2f %5.2f)",
      ic, hpeak->GetMean(1), hpeak->GetMean(2), hpeak->GetRMS(1), hpeak->GetRMS(2),
      hpeak->GetRMS(1)/(2*dxMax/Sqrt(12)), hpeak->GetRMS(2)/(2*dyMax/Sqrt(12)) );

  if(!eAff) eAff = new EdbAffine2D();
  eAff->ShiftX(hpeak->GetMean(1));
  eAff->ShiftY(hpeak->GetMean(2));

  SafeDelete(hpeak);

  return ic;
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::DoubletsFilterOut()
{
  // assign flag = -10 for the duplicated segments
  ePC1.eDXmin  = eDXmin;   ePC1.eDYmin  = eDYmin;
  ePC1.eDTXmin = eDTXmin;  ePC1.eDTYmin = eDTYmin;
  int nout1 = ePC1.DoubletsFilterOut();
  ePC2.eDXmin  = eDXmin;   ePC2.eDYmin  = eDYmin;
  ePC2.eDTXmin = eDTXmin;  ePC2.eDTYmin = eDTYmin;
  int nout2 = ePC2.DoubletsFilterOut();
  Log(3,"DubletsFilterOut","%d and %d segments discarded", nout1, nout2);
  return nout1+nout2;
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::SpotsFilterOut(int nmax)
{
  // discard cells with nenries > nmax
  int nout1 = ePC1.DiscardHighCells(nmax);
  int nout2 = ePC2.DiscardHighCells(nmax);
  Log(3,"SpotsFilterOut","%d and %d cells discarded", nout1, nout2);
  return nout1+nout2;
}
