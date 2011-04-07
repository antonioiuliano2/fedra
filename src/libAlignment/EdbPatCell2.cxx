#include "TH1I.h"
#include "TMath.h"
#include "EdbAffine.h"
#include "EdbLog.h"
#include "EdbPatCell2.h"

ClassImp(EdbSegCorr)
ClassImp(EdbPatCell2)

using namespace TMath;

//---------------------------------------------------------------------
void EdbSegCorr::Print()
{
  printf("EdbSegCorr (dx:dy:dz:dtx:dty:shr:phy): %f %f %f %f %f %f %f\n"
	 ,eV[0],eV[1],eV[2],eV[3],eV[4],eV[5],eV[6]);
}

///---------------------------------------------------------------------
void EdbSegCorr::ApplyCorrections(EdbSegP &s)
{
  float x=X(s), y=Y(s), tx=TX(s), ty=TY(s);
  s.SetX(x); s.SetY(y); s.SetTX(tx); s.SetTY(ty);
}

//---------------------------------------------------------------------
EdbPatCell2::EdbPatCell2()
{
  eDXlim=3; eDYlim=3; eDTXlim=0.003; eDTYlim=0.003;
  eXmarg = eYmarg = 0.0001;        // margins for the cell definition
  eDoubletsRate=0;
  ResetCorr();
}

//---------------------------------------------------------------------
void EdbPatCell2::PrintCorr()
{
  printf("EdbPatCell2 corrections: (dx,dy,dz): %7.2f %7.2f %7.2f (shr): %6.4f (dtx,dty): %6.4f %6.4f\n",
	 eDX,eDY,eDZ,eShr,eDTX,eDTY);
}

//---------------------------------------------------------------------
void EdbPatCell2::ResetCorr()
{
  eApplyCorr = false;
  ePhi = eDX = eDY = eDZ = eDTX = eDTY = 0.;
  eShr = 1;
  SafeDelete(eDoubletsRate);
}

//---------------------------------------------------------------------
void EdbPatCell2::AcceptCorrections(EdbLayer &layer)
{
  // accept corrections from the layer 
  eDX =  layer.GetAffineXY()->B1();
  eDY =  layer.GetAffineXY()->B1();
  eDZ =  layer.Zcorr();
  eDTX = layer.GetAffineTXTY()->B1();
  eDTY = layer.GetAffineTXTY()->B2();
  eShr = layer.Shr();
}

//---------------------------------------------------------------------
void EdbPatCell2::ApplyCorrections(EdbLayer &layer)
{
  // propagate corrections to layer 
  EdbAffine2D a2( 1,0,0,1, eDX,   eDY);
  EdbAffine2D at2(1,0,0,1, eDTX,  eDTY);
  layer.ApplyCorrections( eShr,  eDZ, a2, at2 );
  Log(3,"EdbPatCell2::ApplyCorrections","");
  if(gEDBDEBUGLEVEL>2)  PrintCorr();

}

//---------------------------------------------------------------------
void EdbPatCell2::ApplyCorrections(EdbPattern &pat)
{
  // apply corrections to the segments of a given pat
  pat.TransformShr(eShr);
  EdbAffine2D aff(1,0,0,1, eDX, eDY);
  pat.Transform(&aff);
  EdbAffine2D affa(1,0,0,1, eDTX, eDTY);
  pat.TransformA(&affa);
}

//---------------------------------------------------------------------
void EdbPatCell2::InitPat(EdbPattern &pat, int nx, int ny)
{
  float min[2] = {pat.Xmin()-eXmarg, pat.Ymin()-eYmarg };
  float max[2] = {pat.Xmax()+eXmarg, pat.Ymax()+eYmarg };
  int   n[2]   = {nx,ny};
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell));
  Log(2, "EdbPatCell2::InitPat", "maxcell = %d\n",maxcell);
  InitCell(maxcell,n,min,max);
}

//---------------------------------------------------------------------
void EdbPatCell2::InitPatBin(EdbPattern &pat, float binx, float biny)
{
  float min[2] = {pat.Xmin()-eXmarg, pat.Ymin()-eYmarg };
  float max[2] = {pat.Xmax()+eXmarg, pat.Ymax()+eYmarg };
  int   n[2]   = { (int)((max[0]-min[0])/binx+1), (int)((max[1]-min[1])/biny+1) };
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell)); 
  Log(2, "EdbPatCell2::InitPatBin", "maxcell = %d\n",maxcell);
  InitCell(maxcell,n,min,max);
}

//---------------------------------------------------------------------
int EdbPatCell2::FillCell(EdbPattern &pat)
{
  // assume that the cell is already initialized
  EdbSegP *s=0;
  int n = pat.N();
  for(int i=0; i<n; i++) {
    s = pat.GetSegment(i);
    AddObject( Xs(*s), Ys(*s), (TObject*)s );
  }
  return eNcell;
}

//---------------------------------------------------------------------
int EdbPatCell2::FillCell(TObjArray &pat)
{
  // assume that the cell is already initialized
  EdbSegP *s=0;
  int n = pat.GetEntriesFast();
  for(int i=0; i<n; i++) {
    s = (EdbSegP*)pat.UncheckedAt(i);
    AddObject( Xs(*s), Ys(*s), (TObject*)s );
  }
  return eNcell;
}

//------------------------------------------------------------------------------------
int EdbPatCell2::FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arrC1, TObjArray &arrC2, bool doFill)
{
  int nout=0;

  TObjArray arr1(10000);
  TObjArray arr2(100);
  float v[2];
  EdbSegP *s1,*s2;

  int ncomb;                      // douplications rate counter
  int n1 = SelectObjects(arr1);
  for(int i=0; i<n1; i++) {
    ncomb=0;
    s1 = (EdbSegP*)arr1.UncheckedAt(i);
    if(s1->Flag()==-10)              continue;
    arr2.Clear();
    v[0] = Xs(*s1);
    v[1] = Ys(*s1);
    int n2 = cell.SelectObjectsC(v,ir2,arr2);
    if(n2<1)                         continue;
    float tx1 = TXs(*s1);
    float ty1 = TYs(*s1);
    for(int i2=0; i2<n2; i2++) {
      s2 = (EdbSegP*)arr2.UncheckedAt(i2);
      if(s2->Flag()==-10)                                    continue;
      if( s2==s1 )                                           continue;
      if( Abs(cell.Xs(*s2) - v[0]) > eDXlim )                continue;
      if( Abs(cell.Ys(*s2) - v[1]) > eDYlim )                continue;
      if( Abs(cell.TXs(*s2)- tx1)  > eDTXlim )               continue;
      if( Abs(cell.TYs(*s2)- ty1)  > eDTYlim )               continue;

      if(doFill) {
	arrC1.Add(s1);
	arrC2.Add(s2);
      }
      ncomb++;
    }
    nout+=ncomb;
    if(eDoubletsRate) eDoubletsRate->Fill(ncomb);
  }
  
  Log(3,"EdbPatCell2::FillCombinations","%7d couples found with the acceptance:(dx,dy): %6.2f %6.2f (dtx,dty): %6.4f %6.4f",
      nout,eDXlim,eDYlim,eDTXlim,eDTYlim );
  if(gEDBDEBUGLEVEL>2&&eApplyCorr)             PrintCorr();
  if(gEDBDEBUGLEVEL>2&&cell.eApplyCorr)   cell.PrintCorr();
  return nout;
}

//------------------------------------------------------------------------------------
EdbH2 *EdbPatCell2::FillSelectedH2()
{
  EdbH2 *h = new EdbH2();
  h->InitH2(eN, eMin, eMax);
  EdbSegP *s;
  for(int i=0; i<eNcell; i++)
    for(int j=0; j<eNC[i]; j++) {
      s = (EdbSegP*)(GetObject(i,j));
      if(s->Flag()==-10) continue;
      h->AddBin(i,1);
    }
  return h;
}
