#include "TMath.h"
#include "EdbLog.h"
#include "EdbPatCell2.h"

ClassImp(EdbPatCell2)

using namespace TMath;

//---------------------------------------------------------------------
EdbPatCell2::EdbPatCell2()
{
  eDXmin=3; eDYmin=3; eDTXmin=0.003; eDTYmin=0.003;
}

//---------------------------------------------------------------------
void EdbPatCell2::InitPat(EdbPattern &pat, int nx, int ny)
{
  float marg=0.000001;
  float min[2] = {pat.Xmin()-marg, pat.Ymin()-marg };
  float max[2] = {pat.Xmax()+marg, pat.Ymax()+marg };
  int   n[2]   = {nx,ny};
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell));
  printf("maxcell = %d\n",maxcell);
  InitCell(maxcell,n,min,max);
}

//---------------------------------------------------------------------
void EdbPatCell2::InitPatBin(EdbPattern &pat, float binx, float biny)
{
  float marg=0.000001;
  float min[2] = {pat.Xmin()-marg, pat.Ymin()-marg };
  float max[2] = {pat.Xmax()+marg, pat.Ymax()+marg };
  int   n[2]   = { (int)((max[0]-min[0])/binx+1), (int)((max[1]-min[1])/biny+1) };
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell)); 
  printf("maxcell = %d\n",maxcell);
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
    AddObject( s->X(), s->Y(), (TObject*)s );
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
    AddObject( s->X(), s->Y(), (TObject*)s );
  }
  return eNcell;
}

//---------------------------------------------------------------------
int EdbPatCell2::DoubletsFilterOut()
{
  // assign flag = -10 for the duplicated segments
  int nout=0;
  TObjArray arr1(100);
  TObjArray arr2(100);
  int iv[2], ir1[2] = {0,0}, ir2[2]={1,1};
  EdbSegP *s1,*s2;
  for(int ix=0; ix<NX(); ix++)
    for(int iy=0; iy<NY(); iy++) {
      iv[0] = ix; iv[1] = iy;
      arr1.Clear();
      arr2.Clear();
      SelectObjectsC(iv,ir1,arr1);
      SelectObjectsC(iv,ir2,arr2);
      for(int i1=0; i1<arr1.GetEntriesFast(); i1++) {
 	s1 = (EdbSegP*)arr1.UncheckedAt(i1);
	if(s1->Flag()==-10) continue;
 	for(int i2=0; i2<arr2.GetEntriesFast(); i2++) {
 	  s2 = (EdbSegP*)arr2.UncheckedAt(i2);
	  if(s2->Flag()==-10) continue;
 	  if( s2==s1 )                                              continue;
 	  if( Abs(s2->X()-s1->X())   > eDXmin )                     continue;
	  if( Abs(s2->Y()-s1->Y())   > eDYmin )                     continue;
	  if( Abs(s2->TX()-s1->TX()) > eDTXmin )                    continue;
	  if( Abs(s2->TY()-s1->TY()) > eDTYmin )                    continue;
	  if( s2->Aid(0)==s1->Aid(0) && s2->Aid(1)==s1->Aid(1) )    continue;
	  if( s2->W()>s1->W() ) s1->SetFlag(-10);
	  else                  s2->SetFlag(-10);
	  nout++;
	}
      }
    }

  Log(1,"DoubletsFilterOut","%d segments discarded",nout);
  return nout;
}

//------------------------------------------------------------------------------------
int EdbPatCell2::FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arrC1, TObjArray &arrC2)
{
  int min[2] = {  0 ,  0 };
  int max[2] = {NX(),NY()};
  return FillCombinations(cell, ir2, arrC1, arrC2, min, max);
}

//------------------------------------------------------------------------------------
int EdbPatCell2::FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arrC1, TObjArray &arrC2, float fmin[2], float fmax[2])
{
  int min[2] = {  Max(0,IX(fmin[0]))      ,  Max(0,IY(fmin[1]))      };
  int max[2] = {  Min(NX(),IX(fmax[0])+1) ,  Min(NX(),IY(fmax[1])+1) };
  if( max[0]<=min[0] ) return 0;
  if( max[1]<=min[1] ) return 0;
  return FillCombinations(cell, ir2, arrC1, arrC2, min, max);
}

//------------------------------------------------------------------------------------
int EdbPatCell2::FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arrC1, TObjArray &arrC2, int min[2], int max[2])
{
  int nout=0;

  TObjArray arr1(100);
  TObjArray arr2(100);
  int iv[2], ir1[2] = {0,0};
  float v[2];
  EdbSegP *s1,*s2;
  for(int ix=min[0]; ix<max[0]; ix++)
    for(int iy=min[1]; iy<max[1]; iy++) {
      iv[0] = ix; iv[1] = iy;
      arr1.Clear();
      arr2.Clear();
      SelectObjectsC(iv,ir1,arr1);
      v[0] = X(iv[0]);
      v[1] = Y(iv[1]);
      cell.SelectObjectsC(v,ir2,arr2);
      //cell.SelectObjectsC(iv,ir2,arr2);
      for(int i1=0; i1<arr1.GetEntriesFast(); i1++) {
 	s1 = (EdbSegP*)arr1.UncheckedAt(i1);
	if(s1->Flag()==-10) continue;
 	for(int i2=0; i2<arr2.GetEntriesFast(); i2++) {
 	  s2 = (EdbSegP*)arr2.UncheckedAt(i2);
	  if(s2->Flag()==-10) continue;
 	  if( s2==s1 )                                              continue;
 	  if( Abs(s2->X()-s1->X())   > eDXmin )                     continue;
	  if( Abs(s2->Y()-s1->Y())   > eDYmin )                     continue;
	  if( Abs(s2->TX()-s1->TX()) > eDTXmin )                    continue;
	  if( Abs(s2->TY()-s1->TY()) > eDTYmin )                    continue;

	  arrC1.Add(s1);
	  arrC2.Add(s2);

	  nout++;
	}
      }
    }

  Log(2,"FindCombinations","%d couples found in limits: min:(%d %d) max:(%d %d)",nout,min[0],min[1],max[0],max[1]);
  return nout;
}
