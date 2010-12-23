//-- Author :  Valeri Tioukov   28-11-2008
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPositionAlignment                                                 //
//                                                                      //
// fast position alignment of 2 lists of segments                       //
// (developed for the compton alignment procedure)                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMap.h"
#include "TMath.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEventList.h"
#include "EdbLog.h"
#include "EdbPositionAlignment.h"
#include "EdbTrackFitter.h"
#include "EdbLayer.h"
#include "EdbScanCond.h"
#include "EdbPVRec.h"

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

  eSmoothKernel = "0";
  ePosTree=0;

  // linking parameters
  eS0x  = eS0y  = 2.;
  eS0tx = eS0ty = 0.01;    // sigmas at 0 angle for the chi2 calculation
  eWbaseMin     = 12;      // cut for the w of basetrack to accept it
  eChi2Max      = 3.;      // cut for the chi2 of the basetrack

}

//---------------------------------------------------------------------
EdbPositionAlignment::~EdbPositionAlignment()
{
  eComb1.Clear();
  eComb2.Clear();
  SafeDelete(eAff);
  eTracks.Delete();
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
  printf("Alignment summary: ncomb = %7d\n",eComb1.GetEntries());
  printf("%5d coinsidence in the peak with    dXY: (%6.2f %6.2f)    dz12: (%6.1f %6.1f)   at XY: (%10.1f %10.1f)\n",
	 eNpeak, eXpeak,eYpeak,eZ1peak,eZ2peak, eX0,eY0 );
}

// //---------------------------------------------------------------------
// void EdbPositionAlignment::ApplyCorrections(EdbPlateP &plate)
// {
//   // apply found corrections to the plate parameters

//   Log(3,"EdbPositionAlignment::ApplyCorrections","");
//   if(gEDBDEBUGLEVEL>2)  ePC1.PrintCorr();
//   if(gEDBDEBUGLEVEL>2)  ePC2.PrintCorr();

//   EdbLayer *l1 = plate.GetLayer(1);
//   EdbLayer *l2 = plate.GetLayer(2);

//   EdbAffine2D a1( 1,0,0,1, ePC1.eDX,   ePC1.eDY);
//   EdbAffine2D at1(1,0,0,1, ePC1.eDTX,  ePC1.eDTY);
//   l1->ApplyCorrections( ePC1.eShr,  ePC1.eDZ, a1, at1 );

//   EdbAffine2D a2( 1,0,0,1, ePC2.eDX,   ePC2.eDY);
//   EdbAffine2D at2(1,0,0,1, ePC2.eDTX,  ePC2.eDTY);
//   l2->ApplyCorrections( ePC2.eShr,  ePC2.eDZ, a2, at2 );
// }

//---------------------------------------------------------------------
bool EdbPositionAlignment::ActivatePosTree(const char *name)
{
  if(ePosTree) SafeDelete(ePosTree);
  ePosTree = new TTree(name,"postree (s1:s2)");
  EdbSegP *s1 =0, *s2 =0, *s=0;
  EdbSegCouple *cp=0;
  Float_t dz1,dz2;
  Int_t   flag;
  Float_t chi2;
  int pid1=0,pid2=0;
  float xv=0,yv=0;

  ePosTree->Branch("pid1", &pid1,"pid1/I");
  ePosTree->Branch("pid2", &pid2,"pid2/I");
  ePosTree->Branch("xv",   &xv,"xv/F");
  ePosTree->Branch("yv",   &yv,"yv/F");
  ePosTree->Branch("cp",   "EdbSegCouple",&cp,32000,99);
  ePosTree->Branch("s1.",  "EdbSegP",&s1,32000,99);
  ePosTree->Branch("s2.",  "EdbSegP",&s2,32000,99);
  ePosTree->Branch("s." ,  "EdbSegP",&s ,32000,99);

  ePosTree->Branch("dz1",  &dz1,"dz1/F");
  ePosTree->Branch("dz2",  &dz2,"dz2/F");
  ePosTree->Branch("flag", &flag,"flag/I");
  ePosTree->Branch("chi2", &chi2,"chi2/F");
  return true;
}

//---------------------------------------------------------------------
Int_t EdbPositionAlignment::WritePosTree()
{
  if(!ePosTree)    return 0;
  ePosTree->SetAlias("x1","s1.eX+dz1*s1.eTX");
  ePosTree->SetAlias("x2","s2.eX+dz2*s2.eTX");
  ePosTree->SetAlias("y1","s1.eY+dz1*s1.eTY");
  ePosTree->SetAlias("y2","s2.eY+dz2*s2.eTY");

  ePosTree->SetAlias( "tx1c" , Form("s1.eTX/(%f)+(%f)",ePC1.eShr,ePC1.eDTX) );
  ePosTree->SetAlias( "ty1c" , Form("s1.eTY/(%f)+(%f)",ePC1.eShr,ePC1.eDTY) );
  ePosTree->SetAlias( "tx2c" , Form("s2.eTX/(%f)+(%f)",ePC2.eShr,ePC2.eDTX) );
  ePosTree->SetAlias( "ty2c" , Form("s2.eTY/(%f)+(%f)",ePC2.eShr,ePC2.eDTY) );

  ePosTree->SetAlias("x1c" ,  Form("(%f) + s1.eX + tx1c*(%f)",ePC1.eDX,ePC1.eDZ) );
  ePosTree->SetAlias("y1c" ,  Form("(%f) + s1.eY + ty1c*(%f)",ePC1.eDY,ePC1.eDZ) );
  ePosTree->SetAlias("x2c" ,  Form("(%f) + s2.eX + tx2c*(%f)",ePC2.eDX,ePC2.eDZ) );
  ePosTree->SetAlias("y2c" ,  Form("(%f) + s2.eY + ty2c*(%f)",ePC2.eDY,ePC2.eDZ) );

  ePosTree->SetAlias("dx" , "x2c-x1c" );
  ePosTree->SetAlias("dy" , "y2c-y1c" );
  ePosTree->SetAlias("dtx" , "tx2c-tx1c" );
  ePosTree->SetAlias("dty" , "ty2c-ty1c" );

  ePosTree->SetAlias("dr" , "sqrt(dx*dx+dy*dy)" );
  ePosTree->SetAlias("dt" , "sqrt(dtx*dtx+dty*dty)" );

  ePosTree->Write();
  SafeDelete(ePosTree);
  return 1;
}


/*
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
*/

//---------------------------------------------------------------------
//int EdbPositionAlignment::FillPosTree(TObjArray &arr1,TObjArray &arr2, float dz1, float dz2, int flag)
//{

  //TObjArray arrtr;
  //RankCouples(arr1,arr2,arrtr);

//---------------------------------------------------------------------
int EdbPositionAlignment::FillPosTree(float dz1, float dz2, int flag)
{
  if(!ePosTree)    return 0;
  EdbSegP      *sb1 = new EdbSegP();
  EdbSegP      *sb2 = new EdbSegP();
  EdbSegP      *sb  = new EdbSegP();
  EdbSegCouple *cp  = new EdbSegCouple();

  ePosTree->SetBranchAddress("cp"  , &cp  );
  ePosTree->SetBranchAddress("s1." , &sb1);
  ePosTree->SetBranchAddress("s2." , &sb2);
  ePosTree->SetBranchAddress("s."  , &sb);
  ePosTree->SetBranchAddress("dz1" , &dz1);
  ePosTree->SetBranchAddress("dz2" , &dz2);
  ePosTree->SetBranchAddress("flag", &flag);
  Float_t chi2;
  ePosTree->SetBranchAddress("chi2", &chi2);

  EdbSegCouple *cpt=0;
  EdbSegP *s1, *s2;
  EdbTrackP *t;
  int nsaved = 0;

  int n   = eComb1.GetEntries();
  int nt  = eTracks.GetEntries();
  int ncp = eSegCouples.GetEntries();

  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eComb1.UncheckedAt(i);
    s2 = (EdbSegP*)eComb2.UncheckedAt(i);
    if(nt==n) {
      t = (EdbTrackP*)eTracks.UncheckedAt(i);
      if(t->Chi2()>eChi2Max) continue;
      sb->Copy( *((EdbSegP*)t) );
    }
    if(ncp==n) {
      cpt = (EdbSegCouple*)eSegCouples.UncheckedAt(i);
      *cp = *cpt;
    }
    sb1->Copy(*s1);
    sb2->Copy(*s2);    
    ePosTree->Fill();
    nsaved++;
  }
  Log(2,"FillPosTree","%s %d couples saved with chi2 <= %f", ePosTree->GetName(), nsaved, eChi2Max);
  return n;
}

//---------------------------------------------------------------------
int EdbPositionAlignment::ReadPosTree(TTree *tree, EdbPattern *p, EdbPattern *p1, EdbPattern *p2, TEventList *lst)
{
  if(!tree)    return 0;
  EdbSegP *sb1 = 0;//new EdbSegP();
  EdbSegP *sb2 = 0;//new EdbSegP();
  EdbSegP *sb  = 0;//new EdbSegP();
  Float_t dz1,dz2;
  Int_t flag;
  tree->SetBranchAddress("s1." , &sb1);
  tree->SetBranchAddress("s2." , &sb2);
  tree->SetBranchAddress("s."  , &sb);
  tree->SetBranchAddress("dz1" , &dz1);
  tree->SetBranchAddress("dz2" , &dz2);
  tree->SetBranchAddress("flag", &flag);
  Float_t chi2;
  tree->SetBranchAddress("chi2", &chi2);

  int     n  = tree->GetEntries();
  if(lst) n = lst->GetN();

  for(int i=0; i<n; i++) {
    if(lst) tree->GetEntry(lst->GetEntry(i));
    else    tree->GetEntry(i);
    if(p)  p->AddSegment(  *sb  );
    if(p1) p1->AddSegment( *sb1 );
    if(p2) p2->AddSegment( *sb2 );
  }
  Log(2,"ReadPosTree","%d basetracks read from %s", n,tree->GetName());
  return n;
}

//---------------------------------------------------------------------
void EdbPositionAlignment::SaveAsTree(EdbPattern &pat, const char *name)
{
  TTree *tree = new TTree(name,"pattern tree");
  EdbSegP *s=new EdbSegP();
  tree->Branch("s" ,  "EdbSegP",&s ,32000,99);
  int n = pat.N(), nsaved=0;
  for(int i=0; i<n; i++) {
    s->Copy( *(pat.GetSegment(i)) );
    tree->Fill();
    nsaved++;
  }
  tree->Write();
  SafeDelete(tree);
  Log(2,"SaveAsTree","%d segments saved", nsaved);
}

//---------------------------------------------------------------------
void EdbPositionAlignment::RankCouples()
{
  eTracks.Clear();
  RankCouples(eComb1, eComb2, eTracks);
}

//---------------------------------------------------------------------
void EdbPositionAlignment::RankCouples0()
{
  // construct basetracks only using the position information

  eTracks.Clear();
  int n = eComb1.GetEntries();
  Log(3,"RankCouples0","%d" ,n);
  EdbSegP  *s1, *s2,  *s;
  float dz;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eComb1.At(i);
    s2 = (EdbSegP*)eComb2.At(i);
    dz = s2->Z()-s1->Z();
    s = new EdbSegP( i,
		     0.5*(s1->X() + s2->X()),
		     0.5*(s1->Y() + s2->Y()),
		     (s2->X() - s1->X())/dz,
		     (s2->Y() - s1->Y())/dz,
		     s1->W() + s2->W()
		     );
    s->SetZ(0.5*(s1->Z() + s2->Z()));
    eTracks.Add( s );
  }

  Log(2,"RankCouples0","%d tracks ok", eTracks.GetEntriesFast() );

}

//---------------------------------------------------------------------
void EdbPositionAlignment::RankCouples( TObjArray &arr1,TObjArray &arr2, TObjArray &arrtr )
{
  int n = arr1.GetEntries();
  Log(3,"RankCouples","%d" ,n);

  EdbSegP   *s1, *s2;
  EdbTrackP *t;
  EdbTrackFitter tf;
  EdbScanCond cond1,cond2;
  cond1.SetSigma0(1.,1., 0.010,0.010);  cond1.SetDegrad(4); cond1.SetPulsRamp0(6,9); cond1.SetPulsRamp04(6,9);
  cond2.SetSigma0(1.,1., 0.010,0.010);  cond2.SetDegrad(4); cond2.SetPulsRamp0(6,9); cond2.SetPulsRamp04(6,9);
  EdbSegP seg;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)arr1.At(i);
    s2 = (EdbSegP*)arr2.At(i);
    seg.Copy(*s1);   // to set correctly vid, aid, etc
    //tf.Chi2SegM( *s1, *s2, seg, cond1, cond2);
    tf.Chi2ASeg( *s1, *s2, seg, cond1, cond2);
    //seg.SetChi2( tf.Chi2ACP( *s1, *s2, cond1) );   //TODO test!!

    t = new EdbTrackP(seg);
    t->AddSegment(s1);
    t->AddSegment(s2);
    //    s2->SetMC(s.MCEvt(),s.MCTrack());
    arrtr.Add( t );
  }

  RankCouplesFast(arrtr);
  Log(2,"RankCouples","%d tracks ok", arrtr.GetEntriesFast() );
}

//---------------------------------------------------------------------
void EdbPositionAlignment::RankCouplesFast( TObjArray &arrtr )
{
  int ntr = arrtr.GetEntries();
  Log(1,"RankCouplesFast","%d tracks", ntr );
  TArrayF chi2arr(ntr);
  TArrayI ind(ntr);
  eSegCouples.Delete();
  eSegCouples.Expand(ntr);
  EdbTrackP *t=0;
  for(int i=0; i<ntr; i++) {
    t = ((EdbTrackP*)arrtr.At(i));
    chi2arr[i]= t->Chi2();
    t->GetSegment(0)->SetFlag(0);   //use flag as a counter
    t->GetSegment(1)->SetFlag(0);
  }
  Sort(ntr,chi2arr.GetArray(),ind.GetArray(),0);
  
  EdbSegP *s0=0, *s1=0;
  for(int i=0; i<ntr; i++) {
    t = ((EdbTrackP*)arrtr.At(ind[i]));
    s0 = t->GetSegment(0);
    s1 = t->GetSegment(1);
    s0->SetFlag( s0->Flag()+1 );
    s1->SetFlag( s1->Flag()+1 );
    EdbSegCouple *sc = new EdbSegCouple();
    sc->SetN1(s0->Flag());
    sc->SetN2(s1->Flag());
    eSegCouples.Add(sc);
  }

  for(int i=0; i<ntr; i++) {
    t = ((EdbTrackP*)arrtr.At(ind[i]));
    s0 = t->GetSegment(0);
    s1 = t->GetSegment(1);
    EdbSegCouple *sc = (EdbSegCouple *)eSegCouples.UncheckedAt(i);
    sc->SetCHI2P( t->Chi2() );
    sc->SetN1tot(s0->Flag());
    sc->SetN2tot(s1->Flag());
    s0->SetFlag(0);
    s1->SetFlag(0);
  }

}

/*
//---------------------------------------------------------------------
int EdbPositionAlignment::LinkSegments(TObjArray &arr1,TObjArray &arr2, TObjArray &arr)
{
  EdbTrackFitter tf;
  EdbScanCond cond1,cond2;
  cond1.SetSigma0(1,1,3,3); cond1.SetDegrad(4);
  cond2.SetSigma0(1,1,3,3); cond2.SetDegrad(4);
  EdbSegP s;

  arr.Clear();
  int n = arr1.GetEntries();
  for(int i=0; i<n; i++)
}
*/

//---------------------------------------------------------------------
int EdbPositionAlignment::FillArrays(EdbPattern &p1, EdbPattern &p2)
{
  ePC1.Reset();
  ePC2.Reset();

  float marg = eXcell/2.;
  float min1[2] = { Min(p1.Xmin(),p2.Xmin())-marg, Min(p1.Ymin(),p2.Ymin())-marg };
  float max1[2] = { Max(p1.Xmax(),p2.Xmax())+marg, Max(p1.Ymax(),p2.Ymax())+marg };
  int     n1[2] = { (int)((max1[0]-min1[0])/eXcell+1), (int)((max1[1]-min1[1])/eYcell+1) };
  //eXcell = Max((max1[0]-min1[0])/n1[0],mincell);
  //eYcell = Max((max1[1]-min1[1])/n1[1],mincell);
  float min2[2] = { min1[0]-eXcell, min1[1]-eYcell };
  float max2[2] = { max1[0]+eXcell, max1[1]+eYcell };
  int   n2[2]   = { n1[0]+2     , n1[1]+2      };

  if( n1[0]<1 || n1[1]<1 )           return 0;
  int meancell1 = (int)(p1.N()/n1[0]/n1[1]);
  int meancell2 = (int)(p2.N()/n2[0]/n2[1]);
  int maxcell1  = (int)(10 + meancell1 + 5*Sqrt(meancell1));
  int maxcell2  = (int)(10 + meancell2 + 5*Sqrt(meancell2));

  ePC1.InitCell(maxcell1, n1, min1, max1);
  ePC2.InitCell(maxcell2, n2, min2, max2);

  int nc1 = ePC1.FillCell(p1);
  int nc2 = ePC2.FillCell(p2);

  Log(3,"FillArrays","filled objects/cells: %d / %d  and  %d / %d  in cells of (%6.1f x %6.1f)", 
      p1.N(), nc1, p2.N(), nc2, eXcell, eYcell);
  return nc1+nc2;
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
  TObjArray a(arr.GetSize());
  pc.SelectObjects(min,max,a);
  return SelectBestComptons(a,arr,nmax);
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::SelectBestComptons( TObjArray &a,  TObjArray &arr, int nmax)
{
  int ic=0;
  int n = a.GetEntriesFast();
  EdbSegP *s;
  if(n>nmax)     {
    TArrayF   comptonity(n);   // should be defined as: the bigger - the better
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
  return FillCombinations(ePC1, ePC2, eXcell, eYcell, eDTXmax, eDTYmax);
}

//---------------------------------------------------------------------
int EdbPositionAlignment::FillCombinations(EdbPatCell2 &pc1, EdbPatCell2 &pc2, float dx, float dy, float dtx, float dty)
{
  // the cells must be already filled
  pc1.eDXlim  = dx;   pc1.eDYlim  = dy;
  pc1.eDTXlim = dtx;  pc1.eDTYlim = dty;
  eComb1.Clear();
  eComb2.Clear();
  int ir[2] = { int((dx-0.0001)/eXcell)+1, int((dy-0.0001)/eYcell)+1 };
  int ncomb = pc1.FillCombinations(pc2, ir, eComb1, eComb2);
  Log(3,"FillCombinations","%d selected with the acceptance: %7.2f %7.2f ( %d x %4.1f, %d x %4.1f )   and %7.4f %7.4f ", 
      ncomb, dx,dy, ir[0], eXcell, ir[1], eYcell, dtx, dty );
  return ncomb;
}

//---------------------------------------------------------------------
void  EdbPositionAlignment::PositionPlotA( EdbH2 &hd, float DR, TObjArray *arr1, TObjArray *arr2)
{
  // dz1,dz2 offsets in Z should be defined in ePC1, ePC2
  // output: hd - differential hist, should be already defined
  // 
  // fill arr1 and arr2 if requested, using dr

  hd.CleanCells();
  int n1 = eComb1.GetEntries();
  EdbSegP *s1,*s2;
  bool fillarr=false;
  if(arr1&&arr2&&DR>0)  fillarr=true;
  float dx, dy, dr;
  for(int i=0; i<n1; i++) {
    s1 = (EdbSegP*)eComb1.UncheckedAt(i);
    s2 = (EdbSegP*)eComb2.UncheckedAt(i);
    dx = ePC2.Xs(*s2)-ePC1.Xs(*s1);
    dy = ePC2.Ys(*s2)-ePC1.Ys(*s1);
    hd.Fill( dx, dy );
    if(fillarr) {
      dr = Sqrt(dx*dx+dy*dy);
      if(dr>DR)  continue;
      arr1->Add(s1);
      arr2->Add(s2);
    }
  }
}

//---------------------------------------------------------------------
void  EdbPositionAlignment::PositionPlot( float dz1, float dz2, EdbH2 &hd)
{
  // dz1,dz2 offsets in Z, 
  // output: hd - differential hist, should be already defined

  int n1 = eComb1.GetEntries();
  EdbSegP *s1,*s2;
  for(int i=0; i<n1; i++) {
    s1 = (EdbSegP*)eComb1.UncheckedAt(i);
    s2 = (EdbSegP*)eComb2.UncheckedAt(i);
    hd.Fill( Xcorr(*s2,dz2)-Xcorr(*s1,dz1), Ycorr(*s2,dz2)-Ycorr(*s1,dz1) );
  }
}

//---------------------------------------------------------------------
void  EdbPositionAlignment::PositionPlot(EdbH2 &hd)
{
  // output: hd - differential hist, should be already defined
  // use correction defined in ePC1, ePC2 if requested

  int n1 = eComb1.GetEntries();
  EdbSegP *s1,*s2;
  for(int i=0; i<n1; i++) {
    s1 = (EdbSegP*)eComb1.UncheckedAt(i);
    s2 = (EdbSegP*)eComb2.UncheckedAt(i);
    hd.Fill( ePC2.Xs(*s2) - ePC1.Xs(*s1), 
	     ePC2.Ys(*s2) - ePC1.Ys(*s1) );
  }
}

/*
//---------------------------------------------------------------------
void  EdbPositionAlignment::FindXYcorrection(EdbPattern &p1, EdbPattern &p2, float dz1, float dz2)
{
  
  PositionPlot( float dz1, float dz2, EdbH2 &hd)
}
*/

//---------------------------------------------------------------------
void  EdbPositionAlignment::Zselection(EdbH2 &hd)
{
  if(eNZ1step<1 || eNZ2step<1) {
    Log(1,"Zselection","ERROR: eNZ1step = %d, eNZ2step = %d",eNZ1step,eNZ2step); 
    return; 
  }

  float maxMax=0, meanMax=0, meanMean=0, meanbin=0;
  int   maxbin=0,ic=0;
  //float maxPeak = 0;

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
	  hd.CleanCells();
	  PositionPlot(dz1,dz2,hd);

	  EdbPeak2 p2di(hd);
	  p2di.Smooth(eSmoothKernel);
	  //maxbin = hd.MaxBin();
          maxbin = p2di.MaxBin();
	  if(maxbin>maxMax) 	    maxMax=maxbin;
	  hz12.Fill(dz1,dz2,maxbin);
	  //meanbin   =  hd.Mean();
	  meanbin   =  p2di.Mean();
	  meanMax  += maxbin;
	  meanMean += meanbin;
	  ic++;
	  Log(3,"Zselection","max: %5d mean: %8.4f at dZ(%7.2f %7.2f)", maxbin,meanbin, dz1, dz2);
	}
    }
  meanMax  /= ic;
  meanMean /= ic;

  EdbPeak2 p2z(hz12);
  p2z.Smooth("k5a");
  eHDZ.Copy(*((EdbH2*)(&p2z)));
  p2z.FindGlobalPeak(eZ1peak, eZ2peak, 0.05);

  hd.CleanCells();
  PositionPlot(eZ1peak,eZ2peak,hd);

  EdbPeak2 p2d(hd);
  p2d.Smooth(eSmoothKernel);
  eHpeak.Copy(*((EdbH2*)(&p2d)));
  p2d.FindPeak(eXpeak, eYpeak);

  //  eHpeak.Copy(hd);
  //float vpeak[2];
  //EdbPeak2 p2d(eHpeak);
  //p2d.FindPeak(vpeak);
  //eXpeak=vpeak[0];
  //eYpeak=vpeak[1];

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
 
  if(ePosTree) FillPosTree( eZ1peak, eZ2peak, 0);   // flag=0

  EdbPattern p1, p2;
  int npeak = 0;
  npeak = SelectPeak(p1,p2, eRpeak, eRpeak );
  p1.Clear(); p2.Clear();
  npeak = SelectPeak(p1,p2, eRpeak, eRpeak );
  //if(ePosTree) FillPosTree( p1, p2, 0, 0, 1);    // flag=1
  
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
  Log(2,"SelectPeak","%4d coincidences selected at XY:(%8.2f %8.2f) +-%6.2f   with RMS:(%5.2f %5.2f) Uniformity:(%5.2f %5.2f)",
      ic,  hpeak->GetMean(1), hpeak->GetMean(2), dxMax, hpeak->GetRMS(1), hpeak->GetRMS(2),
      hpeak->GetRMS(1)/(2*dxMax/Sqrt(12)), hpeak->GetRMS(2)/(2*dyMax/Sqrt(12)) );

  SafeDelete(eAff); 
  eAff = new EdbAffine2D();
  eAff->ShiftX(hpeak->GetMean(1));
  eAff->ShiftY(hpeak->GetMean(2));

  SafeDelete(hpeak);

  return ic;
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::DoubletsFilterOut(bool checkview)
{
  return DoubletsFilterOutSide(ePC1,checkview) + DoubletsFilterOutSide(ePC2,checkview);
}

//---------------------------------------------------------------------
int  EdbPositionAlignment::DoubletsFilterOutSide(EdbPatCell2 &pc, bool checkview)
{
  // assign flag = -10 for the duplicated segments
  // if(checkview) - do not remove close segments if them are in the same view

  int nout =0;
  int n = FillCombinations(pc, pc, eDXmin, eDYmin, eDTXmin, eDTYmin);
  EdbSegP *s1,*s2;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eComb1.UncheckedAt(i);
    s2 = (EdbSegP*)eComb2.UncheckedAt(i);
    if(checkview) if( s2->Aid(0)==s1->Aid(0) && s2->Aid(1)==s1->Aid(1) &&  s2->Side()==s1->Side() )    continue;
    if( s2->W()>s1->W() ) s1->SetFlag(-10);
    else                  s2->SetFlag(-10);
    nout++;
  }

  if(ePosTree) FillPosTree( 0., 0., -10);

  Log(2,"DubletsFilterOut","%d segments discarded with DX,DY,DTX,DTY: (%5.1f %5.1f %5.3f %5.3f) checkview =%d", 
      nout,eDXmin, eDYmin, eDTXmin, eDTYmin, checkview );
  return nout;
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

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::ShrinkageSelection(float dzbase)
{
  // select the shrinkage
  ePC1.eShr=ePC2.eShr=1;

  ePC1.eDZ = dzbase;  ePC2.eDZ =   0;
  ShrinkageSelectionSide( *(&ePC1), *(&ePC2), eHShr1, 10, 0.5);
  ePC2.eDZ = -dzbase;  ePC1.eDZ =    0;
  ShrinkageSelectionSide( *(&ePC2), *(&ePC1), eHShr2, 10, 0.5);

  ePC1.eDZ = dzbase;  ePC2.eDZ =   0;
  ShrinkageSelectionSide( *(&ePC1), *(&ePC2), eHShr1, 50, 0.5);
  ePC2.eDZ = -dzbase;  ePC1.eDZ =    0;
  ShrinkageSelectionSide( *(&ePC2), *(&ePC1), eHShr2, 50, 0.5);

  Log(2,"ShrinkageSelection","found  %7.3f %7.3f", ePC1.eShr,  ePC2.eShr );
}

//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::ShrinkageSelectionSide(EdbPatCell2 &pc1, EdbPatCell2 &pc2, EdbH2 &hshr, int nstep, float deltaShr)
{
  // select the shrinkage for one side

  int   npeak=0;

  int      n[2]  = { nstep, 1 };
  float  min[2]  = {pc1.eShr-deltaShr, pc2.eShr-0.001};
  float  max[2]  = {pc1.eShr+deltaShr, pc2.eShr+0.001};
  EdbH2  h12;

  h12.InitH2(n, min, max);

  //pc1.eDXlim  = eXcell;   pc1.eDYlim  = eYcell;
  //pc1.eDTXlim = 0.1;      pc1.eDTYlim = 0.1;
  //int ir[2] = {1,1};

  pc1.eDXlim  = pc1.eDYlim  = 15;
  pc1.eDTXlim = 0.1;      pc1.eDTYlim = 0.1;
  int ir[2] = {0,0};

  pc1.eApplyCorr = pc2.eApplyCorr = true;

  eComb1.Clear();
  eComb2.Clear();

  float x1,x2;
  for(int i1=0; i1<n[0]; i1++)
    {
      for(int i2=0; i2<n[1]; i2++)
	{
	  x1 = pc1.eShr = h12.X(i1);
	  x2 = pc2.eShr = h12.Y(i2);
	  int ncomb = pc1.FillCombinations(pc2, ir, eComb1, eComb2, 0);
 	  h12.Fill(x1,x2,ncomb);
	  Log(3,"ShrinkageSelection:","ncomb = %6d at (%7.2f %7.2f)", ncomb, x1, x2);
	}
    }

  if(gEDBDEBUGLEVEL>2)  h12.PrintStat();

  hshr.Copy(h12);

  float vpeak[2];
  EdbPeak2 p2d(h12);
  npeak = p2d.FindPeak(vpeak);
  pc1.eShr = vpeak[0];

  Log(3,"ShrSelection","maxcomb: %6d (%7.2f %7.2f)",  npeak,vpeak[0],vpeak[1]);
  return npeak;
}

//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::SelectNarrowPeakDXDY(float dr, EdbH2 &hxy )
{
  // assuming that the combinations are found
  // make the position plot and find the maximum,
  // select only the combinations belongs to the maximum cutting out the tails

  float vpeak[2]={0,0};
  PositionPlotA(hxy);
  EdbPeak2 p2d(hxy);
  p2d.FindPeak(vpeak);
  ePC1.eDX +=  vpeak[0];
  ePC1.eDY +=  vpeak[1];

  for(int i=0; i<6; i++) {
    TObjArray comb1,comb2;
    PositionPlotA(hxy, dr, &comb1, &comb2);
    float dx, dy, dtx, dty; 
    FindDiff(comb1,comb2,ePC1,ePC2, dx, dy, dtx, dty);
    Log(3,"SelectNarrowPeakDXDY", "diff: %f %f %f %f with %d   eDX,eDY = %f %f", dx, dy, dtx, dty, comb1.GetEntries(), ePC1.eDX,ePC1.eDY);
    ePC1.eDX += dx;
    ePC1.eDY += dy;
  }

  TObjArray comb1,comb2;
  PositionPlotA(hxy, dr, &comb1, &comb2);
  hxy.DrawH2("dxdy")->Write("dxdy");

  if(ActivatePosTree("postreeDXDYwide"))  { FillPosTree(0, 0, 9); WritePosTree(); }

  eComb1.Clear();  eComb2.Clear();
  int nsel = comb1.GetEntries();
  for(int i=0; i<nsel; i++) {
    eComb1.Add(comb1.UncheckedAt(i));
    eComb2.Add(comb2.UncheckedAt(i));
  }

  Log(2,"SelectNarrowPeakDXDY", "peak of %d found at %f %f with dr = %f", eComb1.GetEntries(), ePC1.eDX,ePC1.eDY, dr);
  return comb1.GetEntries();
}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::AlignWithTracks(int nang, int npos)
{
  // assuming that the combinations are found and tracks are reconstructed
  // correct DTX,DTY of the sides in respect of the tracks

  for(int i=0; i<nang; i++) {
    float dx, dy, dtx, dty; 
    FindDiff10(dx, dy, dtx, dty);
    Log(3,"AlignWithTracks", "%d angle side 1 diff: %f %f %f %f with %d ", i,dx, dy, dtx, dty, eComb1.GetEntries());
    ePC1.eDTX += dtx;
    ePC1.eDTY += dty;
    FindDiff20(dx, dy, dtx, dty);
    Log(3,"AlignWithTracks", "%d angle side 2 diff: %f %f %f %f with %d ", i,dx, dy, dtx, dty, eComb2.GetEntries());
    ePC2.eDTX += dtx;
    ePC2.eDTY += dty;
  }

  for(int i=0; i<npos; i++) {
    float dx, dy, dtx, dty; 
    FindDiff12(dx, dy, dtx, dty);
    Log(3,"AlignWithTracks", "%d pos side 1 diff: %f %f %f %f with %d ", i, dx, dy, dtx, dty, eComb1.GetEntries());
    ePC1.eDX += dx/2.;
    ePC1.eDY += dy/2.;
    ePC2.eDX -= dx/2.;
    ePC2.eDY -= dy/2.;
  }

}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::FindDiff12(float &dx, float &dy, float &dtx, float &dty )
{
  FindDiff(eComb1, eComb2, ePC1, ePC2, dx,dy,dtx,dty);
}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::FindDiff10(float &dx, float &dy, float &dtx, float &dty )
{
  FindDiff(eComb1, eTracks, ePC1, ePC, dx,dy,dtx,dty);
}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::FindDiff20(float &dx, float &dy, float &dtx, float &dty )
{
  FindDiff(eComb2, eTracks, ePC2, ePC, dx,dy,dtx,dty);
}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::FindDiff(TObjArray &arr1, TObjArray &arr2, EdbPatCell2 &pc1, EdbPatCell2 &pc2, 
				    float &dx, float &dy, float &dtx, float &dty )
{
  Log(3,"FindDiff","pc1: %f %f %f %f %f %f %f",pc1.eDX, pc1.eDY, pc1.eDZ, pc1.eDTX, pc1.eDTY, pc1.ePhi, pc1.eShr);
  Log(3,"FindDiff","pc2: %f %f %f %f %f %f %f",pc2.eDX, pc2.eDY, pc2.eDZ, pc2.eDTX, pc2.eDTY, pc2.ePhi, pc2.eShr);

  int n = arr1.GetEntries();
  EdbSegP *s1,*s2;
  Double_t sdx=0, sdy=0, sdtx=0, sdty=0;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)arr1.UncheckedAt(i);
    s2 = (EdbSegP*)arr2.UncheckedAt(i);
    sdx  += pc2.Xs(*s2)-pc1.Xs(*s1);
    sdy  += pc2.Ys(*s2)-pc1.Ys(*s1);
    sdtx += pc2.TXs(*s2)-pc1.TXs(*s1);
    sdty += pc2.TYs(*s2)-pc1.TYs(*s1);
  }
  if(Abs(sdtx)>10)     Log(1,"FindDiff","%f %f %f %f",sdx,sdy,sdtx,sdty);
  dx  = sdx/n;
  dy  = sdy/n;
  dtx = sdtx/n;
  dty = sdty/n;
}

//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::Link()
{
  //FillArrays(p1, p2);
  eComb1.Clear();
  eComb2.Clear();
  ePC1.eDXlim  = 2*eXcell;   ePC1.eDYlim  = 2*eYcell;
  ePC1.eDTXlim = 0.12;       ePC1.eDTYlim = 0.12;
  int ir[2] = {2,2};
  int ncomb = ePC1.FillCombinations(ePC2, ir, eComb1, eComb2);
  //RankCouples();

  Log(1,"WARNING: deprecated function: EdbPositionAlignment::Link"," %8d combinations found",       ncomb );
  return ncomb;
}

//----------------------------------------------------------------------------------------------
void EdbPositionAlignment::FindCorrections(EdbPattern &pat1, EdbPattern &pat2, float DZ, bool doShrink)
{
  // find corrections assuming the patterns already closely aligned (linking case)
  // DZ is the distance between patterns(fixed)

  FillArrays(pat1, pat2);     // fill at nominal quote - important for doublets and for shrinkage check
  //DoubletsFilterOut(true);
  if(doShrink)   ShrinkageSelection(DZ);

  ePC1.eDZ =  DZ/2;
  ePC2.eDZ = -DZ/2;
  ePC1.eApplyCorr = ePC2.eApplyCorr = true;
  FillArrays(pat1, pat2);                      // fill at the same quote - to increase the coinsidences
  FillCombinations();
  //Link();

  int    nxy[2]  = {51,51};                    // TODO
  float  min[2]  = {-2*eXcell, -2*eXcell };
  float  max[2]  = { 2*eXcell,  2*eXcell };
  EdbH2  hxy;
  hxy.InitH2(nxy, min, max);

  SelectNarrowPeakDXDY(10., hxy);   // only peak couples remaining in eComb1/2
  RankCouples0();                   // construct tracks (without correction)

  if(ActivatePosTree("postreeDXDY"))  { FillPosTree(0, 0, 9); WritePosTree(); }

  ResetPositionCorr();
  ResetAngularCorr();


  AlignWithTracks(3,0);      // only angular correction of segments to keep original basetrack angle 

  if(ActivatePosTree("postreeDXDYalign"))  { FillPosTree(0, 0, 8); WritePosTree(); }

  Log(3,"FindCorrections","side1 (shr,dx,dy,dtx,dty): %f   %f %f   %f %f", ePC1.eShr, ePC1.eDX, ePC1.eDY, ePC1.eDTX, ePC1.eDTY);
  Log(3,"FindCorrections","side2 (shr,dx,dy,dtx,dty): %f   %f %f   %f %f", ePC2.eShr, ePC2.eDX, ePC2.eDY, ePC2.eDTX, ePC2.eDTY);
}

//----------------------------------------------------------------------------------------------
int EdbPositionAlignment::WideSearchXY(EdbPattern &pat1, EdbPattern &pat2, EdbH2 &hxy, float dz, float xmin, float xmax, float ymin, float ymax)
{
  // wide prealignment (in case of the basetracks)

  int   npeak=0;

  ePC1.eDZ =  dz/2; 
  ePC2.eDZ = -dz/2;
  ePC1.eApplyCorr = ePC.eApplyCorr = true;
  FillArrays(pat1, pat2);     // fill at central quote
  DoubletsFilterOut(false);

  int      n[2]  = { (int)((xmax-xmin)/eXcell) +1 , (int)((ymax-ymin)/eYcell) +1 };
  float  min[2]  = { -((int)(n[0]/2)+0.5)*eXcell , -((int)(n[1]/2)+0.5)*eYcell };
  float  max[2]  = {  ((int)(n[0]/2)+0.5)*eXcell ,  ((int)(n[1]/2)+0.5)*eYcell };
  EdbH2  h12;
  h12.InitH2(n, min, max);

  ePC1.eDXlim  = eXcell;   ePC1.eDYlim  = eYcell;
  ePC1.eDTXlim = 0.01;     ePC1.eDTYlim = 0.01;

  int ir[2] = {1,1};
  float x1,x2;
  for(int i1=0; i1<n[0]; i1++)
    {
      for(int i2=0; i2<n[1]; i2++)
	{
	  x1 = ePC1.eDX = h12.X(i1);
	  x2 = ePC1.eDY = h12.Y(i2);
	  eComb1.Clear();
	  eComb2.Clear();
	  int ncomb = ePC1.FillCombinations(ePC2, ir, eComb1, eComb2);
 	  h12.Fill(x1,x2,ncomb);
	  Log(3,"WideSearchXY:","ncomb = %6d at (%7.2f %7.2f)", ncomb, x1, x2);
	}
    }

  h12.PrintStat();
  hxy.Copy(h12);

  float vpeak[2];
  EdbPeak2 p2d(h12);
  npeak = p2d.FindPeak(vpeak);
  ePC1.eDX = vpeak[0];
  ePC1.eDY = vpeak[1];
  eComb1.Clear();
  eComb2.Clear();
  int ncomb = ePC1.FillCombinations(ePC2, ir, eComb1, eComb2);
  Log(3,"WideSearchXY","ncomb = %d",ncomb);

  int    nxy[2]  = {21,21};                    // TODO
  float  minxy[2]  = {-1000, -1000 };
  float  maxxy[2]  = { 1000,  1000 };
  EdbH2  hxyn;
  hxyn.InitH2(nxy, minxy, maxxy);
  SelectNarrowPeakDXDY(1000, hxyn);

  Log(2,"WideSearchXY","PC1: %f %f %f %f PC2:%f %f %f %f ", ePC1.eDX, ePC1.eDY, ePC1.eDZ,ePC1.eShr, ePC2.eDX, ePC2.eDY, ePC2.eDZ, ePC2.eShr);

  hxyn.DrawH2("hxyn")->Write("hxyn");

  ActivatePosTree("align");
  FillPosTree(dz/2, -dz/2, 0);
  WritePosTree();

  Log(2,"WideSearchXY","maxcomb: %6d (%7.2f %7.2f)",  npeak,vpeak[0],vpeak[1]);
  return npeak;
}
