//-- Author :  Valeri Tioukov   19-06-2009
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// EdbAlignmentV                                                          //
//                                                                        //
// General alignment of the preselected arrays (combinations) of segments //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TVector2.h"
#include "EdbLog.h"
#include "EdbAlignmentV.h"
#include "EdbLayer.h"

ClassImp(EdbAlignmentV)

using namespace TMath;

//---------------------------------------------------------------------
EdbAlignmentV::EdbAlignmentV()
{
  eXmarg = eYmarg = 0.0001;        // margins for the cell definition
  eDoubletsRate=0;
  eUseAffCorr = false;   // by default use eCorr (separated corrections)
}

//---------------------------------------------------------------------
EdbAlignmentV::~EdbAlignmentV()
{
  eS[0].Clear();
  eS[1].Clear();
  SafeDelete(eDoubletsRate);
}

//---------------------------------------------------------------------
int EdbAlignmentV::DoubletsFilterOut(bool checkview)
{
  // assumed the same pattern's segments in eS[0] and eS[1]
  int nout=0;
  int n = CheckEqualArr(eS[0], eS[1]);
  EdbSegP *s1,*s2;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eS[0].UncheckedAt(i);
    s2 = (EdbSegP*)eS[1].UncheckedAt(i);
    if(checkview) if( s2->Aid(0)==s1->Aid(0) && s2->Aid(1)==s1->Aid(1) && s2->Side()==s1->Side() )    continue;
    if( s2->W()>s1->W() ) s1->SetFlag(-10);
    else                  s2->SetFlag(-10);
    nout++;
  }
  Log(2,"DubletsFilterOut","%d segments discarded with DX,DY,DTX,DTY: (%5.1f %5.1f %5.3f %5.3f) checkview =%d", 
      nout,eDVsame[0],eDVsame[1],eDVsame[2],eDVsame[3], checkview );
  return nout;
}

//---------------------------------------------------------------------
int EdbAlignmentV::SelectIsolated()
{
  // assume the different pattern's segments in eS[0] and eS[1]
  // assume that the selection method in FillCombinations - "first s1 then s2"
  // remove all duplicated combs

  int nrem=0;
  int n = CheckEqualArr(eS[0], eS[1]);
  EdbSegP *s1=0,*s2=0;
  EdbSegP *s1ok=0,*s2ok=0;
  int   iok=-1;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eS[0].UncheckedAt(i);
    s2 = (EdbSegP*)eS[1].UncheckedAt(i);
    if(s1==s1ok) {
      eS[0].RemoveAt(i);
      eS[1].RemoveAt(i);
      nrem++;
    } 
    else if(s1ok!=0) {
      eS[0].RemoveAt(iok);
      eS[1].RemoveAt(iok);
      s1ok=0;
      nrem++;
    }
    else {  s1ok = s1, s2ok = s2; iok=i; }
  }
  
  eS[0].Compress();
  eS[1].Compress();

  Log(2,"SelectIsolated","%d couples discarded with", nrem );
  return nrem;
}

//---------------------------------------------------------------------
int EdbAlignmentV::SelectBestCouple()
{
  // assume the different pattern's segments in eS[0] and eS[1]
  // assume that the selection method in FillCombinations - "first s1 then s2"

  int nrem=0;
  int n = CheckEqualArr(eS[0], eS[1]);
  EdbSegP *s1=0,*s2=0;
  EdbSegP *s1ok=0,*s2ok=0;
  int   iok=-1;
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)eS[0].UncheckedAt(i);
    s2 = (EdbSegP*)eS[1].UncheckedAt(i);
    if(s1==s1ok) {
      if( CoupleQuality(*s1,*s2) >  CoupleQuality(*s1ok,*s2ok) ) {
	s1ok = s1, s2ok = s2;
	eS[0].RemoveAt(iok);
	eS[1].RemoveAt(iok);
	iok=i;
      }
      else { 
	eS[0].RemoveAt(i);
	eS[1].RemoveAt(i);
      }
      nrem++;
    } 
    else {  s1ok = s1, s2ok = s2; iok=i; }
  }
  
  eS[0].Compress();
  eS[1].Compress();

  Log(2,"SelectBestCouple","%d couples discarded with", nrem );
  return nrem;
}

//---------------------------------------------------------------------
float EdbAlignmentV::CoupleQuality( EdbSegP &s1, EdbSegP &s2 )
{
  // TODO
  return s1.W()+s2.W();
}

//---------------------------------------------------------------------
int EdbAlignmentV::FillCombinations()
{
  float dxMax = 2*Max( ePC[0].Xbin(), ePC[1].Xbin() );
  float dyMax = 2*Max( ePC[0].Ybin(), ePC[1].Ybin() );
  return FillCombinations( eDVsame, dxMax, dyMax, 1);
}

//---------------------------------------------------------------------
int EdbAlignmentV::FillCombinations(float dv[4], float dxMax, float dyMax, bool doFill)
{
  // input:  dv = coinsidence condition (dx,dy,dtx,dty)
  //         dxMax, dyMax - the area around each segment for the combinations 
  //         selection, it can be selected bigger then coinsidence condition due to 
  //         corrections displacements
  // 
  // the cells must be already filled

  eS[0].Clear();
  eS[1].Clear();
  int ir2[2] = { int((dxMax-0.0001)/ePC[0].Xbin())+1, int((dyMax-0.0001)/ePC[0].Ybin())+1 };

  int nout=0;

  TObjArray arr1(10000);
  TObjArray arr2(100);
  float v[2];
  EdbSegP *s1,*s2;

  int ncomb;                      // combinations rate counter
  int n1 = ePC[0].SelectObjects(arr1);
  for(int i=0; i<n1; i++) {
    ncomb=0;
    s1 = (EdbSegP*)arr1.UncheckedAt(i);
    if(s1->Flag()==-10)                                        continue;
    arr2.Clear();
    v[0] = X( 0, *s1);
    v[1] = Y( 0, *s1);
    int n2 = ePC[1].SelectObjectsC(v,ir2,arr2);

    if(n2<1)                                                    continue;
    float tx1 = TX(0, *s1);
    float ty1 = TY(0, *s1);
    for(int i2=0; i2<n2; i2++) {
      s2 = (EdbSegP*)arr2.UncheckedAt(i2);
      if(s2->Flag()==-10)                                       continue;
      if( s2==s1 )                                              continue;
      if( Abs(X(1, *s2) - v[0]) > dv[0] )                 continue;
      if( Abs(Y(1, *s2) - v[1]) > dv[1] )                 continue;
      if( Abs(TX(1, *s2)- tx1)  > dv[2] )                 continue;
      if( Abs(TY(1, *s2)- ty1)  > dv[3] )                 continue;

      if(doFill) {
	eS[0].Add(s1);
	eS[1].Add(s2);
      }
      ncomb++;
    }
    nout+=ncomb;
    if(eDoubletsRate) eDoubletsRate->Fill(ncomb);
  }

  arr1.Clear();
  arr2.Clear();

  Log(3,"FillCombinations","%d selected with the acceptance: %7.2f %7.2f (%d,%d) and tolerance (%7.2f%7.2f %7.4f %7.4f)",
      nout, dxMax,dyMax, ir2[0], ir2[1], dv[0], dv[1], dv[2], dv[3] );
  return nout;
}

//---------------------------------------------------------------------
void EdbAlignmentV::OptimiseVar1(int side, int ivar,  EdbH2 *hdxy, EdbH2 *hdtxy)
{
  // use already preselected "couples" - useful to test small variations of parameters
  if(!SideOK(side)) return;

  EdbH1 &h = eH[side][ivar];
  if( h.N() < 1 ) {
    Log(1,"OptimiseVar1","ERROR: var %d of side %d have %d steps - skipped", ivar,side, h.N() );
    return;
  }

  h.CleanCells();

  int   npk0=0;
  float dx0=0,dy0=0;  //the position of the highest peak in xy
  float var0=0;      //the position of the highest peak on var
  for(int i=0; i<h.N(); i++) 
    {
      float var= h.X(i);
      eCorr[side].SetV( ivar, var );
      
      int npk = Ncoins(eDVsame, hdxy); 
      float dx=0, dy=0;
      if(hdxy){
	EdbPeak2 pk2(*hdxy);
	npk = pk2.FindPeak( dx, dy );
      }
      h.SetBin( i, npk );
      if( npk > npk0 ) { npk0=npk; dx0 = dx; dy0=dy; var0 = var; }
      Log(4,"OptimiseVar1"," %d %f %d (%f %f)", i, h.X(i), h.Bin(i), dx,dy );
    }

  eCorr[side].AddV(0,dx0);      // set x-y corrections if any
  eCorr[side].AddV(1,dy0);
  eCorr[side].SetV(ivar,var0);

  Log(3,"OptimiseVar1"," side:var (%d:%d): found %d at var=%f  xy:(%f %f)", side, ivar, npk0, var0, dx0,dy0 );
}


//---------------------------------------------------------------------
void EdbAlignmentV::OptimiseVar2( int side1, int ivar1, int side2, int ivar2, 
				  EdbH2 &h12,  EdbH2 *hdxy, EdbH2 *hdtxy )
{
  // use already preselected "couples" - useful to test small variations of parameters
  // especially after Hdistance preselection
  // Input:    2 variables defoned as side1/ivar1 and side1/ivar1
  // Output:   h12 - filled selections histo
  // Optional: hdxy, hdtxy - 2d- histograms defined outside and used for the peak selection
  if(!SideOK(side1)) return;
  if(!SideOK(side2)) return;
  EdbH1 &h1 = eH[side1][ivar1];
  EdbH1 &h2 = eH[side2][ivar2];
  if( h1.N() < 1 || h2.N() < 1) {
    Log(1,"OptimiseVar2","ERROR: var %d of side %d have %d steps - skipped", ivar1,side1, h1.N() );
    Log(1,"OptimiseVar2","ERROR: var %d of side %d have %d steps - skipped", ivar2,side2, h2.N() );
    return;
  }
  Log(3,"OptimiseVar2","side/var: %d/%d and %d/%d with  %d x %d = %d attempts", 
      side1, ivar1, side2, ivar2, h1.N(), h2.N(), h1.N()*h2.N() );

    //h12.Delete();
  int    n12[2]  = { h1.N(), h2.N() };
  float  min[2]  = { h1.Xmin(), h2.Xmin() };
  float  max[2]  = { h1.Xmax(), h2.Xmax() };
  h12.InitH2(n12, min, max);

  h1.CleanCells();
  h2.CleanCells();

  int   npk0=0;
  float dx0=0,dy0=0;  //the position of the highest peak

  for(int i1=0; i1<h1.N(); i1++)
    {
      eCorr[side1].SetV( ivar1, h1.X(i1) );
      for(int i2=0; i2<h2.N(); i2++) 
	{
	  eCorr[side2].SetV( ivar2, h2.X(i2) );
	  int nc = Ncoins(eDVsame, hdxy);
	  if(!hdxy) h12.SetBin( i1, i2, nc );
	  else {
	    EdbPeak2 pk2(*hdxy);
	    float  dx, dy;
	    int npk = pk2.FindPeak( dx, dy );
	    //int npk = pk2.FindPeak9( dx, dy );
	    if( npk > npk0 ) { npk0=npk; dx0 = dx; dy0=dy; }
	    h12.SetBin( i1, i2, npk );
	  }
	}
    }

  eCorr[side1].AddV(0,dx0);      // set x-y corrections
  eCorr[side1].AddV(1,dy0);
}

//---------------------------------------------------------------------
bool EdbAlignmentV::SideOK(int side)
{
  if(side==0||side==1) return 1; 
  else { 
    Log(1,"SideOK","ERROR: invalid side index %d",side); 
    return 0;
  }
}

//---------------------------------------------------------------------
Int_t EdbAlignmentV::CheckEqualArr(TObjArray &arr1, TObjArray &arr2)
{
  int n1 = arr1.GetEntries();
  int n2 = arr2.GetEntries();
  int n = Min(n1,n2);
  if(n1!=n2) Log(1,"CheckEqualArr","WARNING: arrays are not equal: %d %d, take first %d segs", n1,n2,n );
  return n;
}

//---------------------------------------------------------------------
Float_t EdbAlignmentV::FineCorrZ(TObjArray &arr1, TObjArray &arr2)
{
  double dzz=0, dzx=0, dzy=0;
  int n = CheckEqualArr(arr1,arr2);
  int icx=0, icy=0;
  for(int i=0; i<n; i++) {
    EdbSegP *s1 = (EdbSegP*)arr1.UncheckedAt(i);
    EdbSegP *s2 = (EdbSegP*)arr2.UncheckedAt(i);
    float tx1 =  TX(0, *s1), ty1 =  TY(0, *s1);
    float x1  =  X(0,*s1),  y1 =  Y(0,*s1);
    float x2  =  X(1,*s2),  y2 =  Y(1,*s2);

    if(tx1>0.1) {
      dzx += (x2-x1)/tx1;
      icx++;
    }
    if(ty1>0.1) {
      dzy += (y2-y1)/ty1;
      icy++;
    }
  }

  if(icx+icy>3)   dzz = (dzx+dzy)/(icx+icy);

  Log(3,"FineCorrZ","dzz =%f with icx=%d and icy=%d (of %d) segments",
      (float)dzz, icx,icy, n );

  return (float)dzz;
}

//---------------------------------------------------------------------
void EdbAlignmentV::CorrToCoG(int side, EdbPattern &p)
{
  int n=p.N();
  Double_t x0=0, y0=0;
  for(int i=0; i<n; i++) {
    EdbSegP *s = p.GetSegment(i);
    x0 += X(side, *s);
    y0 += Y(side, *s);
  }
  x0/=n;   y0/=n;
  eCorr[0].AddV(0, -x0);
  eCorr[0].AddV(1, -y0);
  eCorr[1].AddV(0, -x0);
  eCorr[1].AddV(1, -y0);
}

//---------------------------------------------------------------------
void EdbAlignmentV::CorrToCoG(int side, TObjArray &p)
{
  int n=p.GetEntries();
  Double_t x0=0, y0=0;
  for(int i=0; i<n; i++) {
    EdbSegP *s = (EdbSegP*)p.UncheckedAt(i);
    x0 += X(side,*s);
    y0 += Y(side,*s);
  }
  x0/=n;   y0/=n;
  eCorr[0].AddV(0, -x0);
  eCorr[0].AddV(1, -y0);
  eCorr[1].AddV(0, -x0);
  eCorr[1].AddV(1, -y0);
}

//---------------------------------------------------------------------
Float_t EdbAlignmentV::FineCorrPhi(TObjArray &arr1, TObjArray &arr2)
{
  // for the correct answer the O,O should be in the CoG of the second pattern
  int n = CheckEqualArr(arr1,arr2);
  if(n<3) return 0;
  double x1=0, y1=0, x2=0,y2=0;
  Double_t R=0, r=0, dPhi=0;
  for(int i=0; i<n; i++) {
    EdbSegP *s1 = (EdbSegP*)arr1.UncheckedAt(i);
    EdbSegP *s2 = (EdbSegP*)arr2.UncheckedAt(i);
    x1 =  X(0,*s1);
    y1 =  Y(0,*s1);
    x2 =  X(1,*s2);
    y2 =  Y(1,*s2);
    r = Sqrt(x1*x1+y1*y1);
    dPhi += (ATan2(-y2,-x2) - ATan2(-y1,-x1)) * r;
    R += r;
  }
  dPhi /= R;
  Log(3,"FineCorrPhi","dPhi =%f with %d segments",      (float)dPhi, n );
  return (float)dPhi;
}

//---------------------------------------------------------------------
Int_t EdbAlignmentV::FindCorrDiff( float dvsame[4], int side, int nlim )
{
  //find diffs with the default settings and correct requested side
  if(!SideOK(side)) return 0;
  float dvf[4];
  int n = FindDiff(eS[0], eS[1], dvsame, dvf );
  if(n<nlim) return n;
  eCorr[side].AddV(0,dvf[0]);
  eCorr[side].AddV(1,dvf[1]);
  //eCorr[side].AddV(3,dvf[2]);
  //eCorr[side].AddV(4,dvf[3]);
  Log(3,"FindCorrDiff","correct side %d  with %11.7f %11.7f %10.7f %10.7f using %5d coinsidences",
      side, dvf[0] ,dvf[1] ,dvf[2] ,dvf[3],n);
  return n;
}

//---------------------------------------------------------------------
Int_t EdbAlignmentV::FindDiff(TObjArray &arr1, TObjArray &arr2, float dvlim[4], float dvfound[4] )
{
  int n = CheckEqualArr(arr1,arr2);
  int ic=0;
  EdbSegP *s1,*s2;
  Double_t sdx=0, sdy=0, sdtx=0, sdty=0;
  float dvf[4];
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP*)arr1.UncheckedAt(i);
    s2 = (EdbSegP*)arr2.UncheckedAt(i);
    if( ValidCoinsidence(*s1, *s2, dvlim, dvf) ) {
      sdx   += dvf[0];
      sdy   += dvf[1];
      sdtx  += dvf[2];
      sdty  += dvf[3];
      ic++;
    }
  }
  dvfound[0]  = sdx/ic;
  dvfound[1]  = sdy/ic;
  dvfound[2]  = sdtx/ic;
  dvfound[3]  = sdty/ic;
  return ic;
}


//---------------------------------------------------------------------
Bool_t EdbAlignmentV::ValidCoinsidence(EdbSegP &s1, EdbSegP &s2, float dv[4], float dfound[4] )
{
  if(&s1==&s2)                                                                return 0;
  if(s1.Flag()==-10)                                                          return 0;
  if(s2.Flag()==-10)                                                          return 0;
  dfound[0] = X(1,s2)  - X(0,s1);  if(Abs(dfound[0]) > dv[0])   return 0;
  dfound[1] = Y(1,s2)  - Y(0,s1);  if(Abs(dfound[1]) > dv[1])   return 0;
  dfound[2] = TX(1,s2) - TX(0,s1); if(Abs(dfound[2]) > dv[2])   return 0;
  dfound[3] = TY(1,s2) - TY(0,s1); if(Abs(dfound[3]) > dv[3])   return 0;
  return 1;
}

//---------------------------------------------------------------------
Int_t EdbAlignmentV::Ncoins(float dv[4], EdbH2 *hdxy, EdbH2 *hdtxy, TObjArray *sel1, TObjArray *sel2 )
{
  // calculate all coinsidences inside dv:(dx,dy, dtx,dty)
  // fill position and angular 2-d plots if requested
  // fill arrays with selected segments sel1, sel2 if requested

  int ic = 0; // counter
  int n = CheckEqualArr(eS[0],eS[1]);

  if(hdxy)   hdxy->CleanCells();
  if(hdtxy)  hdtxy->CleanCells();

  float dfound[4];
  for(int i=0; i<n; i++)
    {
      EdbSegP *s1 = (EdbSegP*)eS[0].UncheckedAt(i);
      EdbSegP *s2 = (EdbSegP*)eS[1].UncheckedAt(i);

      if( !ValidCoinsidence(*s1, *s2, dv, dfound) )    continue;

      ic++;
      if(sel1)  sel1->Add(s1);
      if(sel2)  sel2->Add(s2);
      if(hdxy)  hdxy->Fill(dfound[0],dfound[1]);
      if(hdtxy) hdtxy->Fill(dfound[2],dfound[3]);
    }
  return ic;
}

//---------------------------------------------------------------------
void EdbAlignmentV::HDistance(EdbPattern &p1, EdbPattern &p2, float dxMax, float dyMax)
{
  Log(3,"HDistance","preselection for patterns with %d and %d segments",p1.N(), p2.N());

  int eITMAX=51;  // angular normalization (def=50) (the step is ~1./eITMAX)
  int eOCMAX=100; // occupancy (def=100)
  int nx=201, ny=201, no=eOCMAX;
 
  Long_t *ind1 = new Long_t[nx*ny*no];
  Long_t *ind2 = new Long_t[nx*ny*no];
  Int_t  *oc1  = new Int_t[nx*ny];
  Int_t  *oc2  = new Int_t[nx*ny];
  memset(ind1,0,nx*ny*no*sizeof(Long_t));
  memset(ind2,0,nx*ny*no*sizeof(Long_t));
  memset( oc1,0,nx*ny*sizeof(Int_t));
  memset( oc2,0,nx*ny*sizeof(Int_t));

  int itx,ity;  

  for(int i=0;i<eITMAX;i++) for(int j=0;j<eITMAX;j++) { oc1[i*nx+j]=0; oc2[i*nx+j]=0;} 
  for(int i=0;i<p1.N();i++){
    itx=int( (p1.GetSegment(i)->TX()+1.)*eITMAX/2. );
    ity=int( (p1.GetSegment(i)->TY()+1.)*eITMAX/2. );
    if(itx<eITMAX)
      if(ity<eITMAX)
	if(itx>=0)
	  if(ity>=0)
	    if(oc1[itx*nx+ity]<eOCMAX-1) 
	      { ind1[itx*nx*ny+ity*ny+oc1[itx*nx+ity]]=i; (oc1[itx*nx+ity])++;}
  }
  for(int i=0;i<p2.N();i++){
    itx=int( (p2.GetSegment(i)->TX()+1.)*eITMAX/2 );
    ity=int( (p2.GetSegment(i)->TY()+1.)*eITMAX/2 );
    if(itx<eITMAX)
      if(ity<eITMAX)
	if(itx>=0)
	  if(ity>=0)
	    if(oc2[itx*nx+ity]<eOCMAX-1) 
	      { ind2[itx*nx*ny+ity*ny+oc2[itx*nx+ity]]=i; (oc2[itx*nx+ity])++;}
  }
  
  eS[0].Clear();
  eS[1].Clear();

  EdbSegP *s1,*s2;
  for(int itx=0;itx<eITMAX;itx++) for(int ity=0;ity<eITMAX;ity++) 
    for(int i=0;i<oc1[itx*nx+ity];i++) {
      s1 = p1.GetSegment(ind1[itx*nx*ny+ity*ny+i]);
      for(int j=0;j<oc2[itx*nx+ity];j++)
	{
	  s2 = p2.GetSegment(ind2[itx*nx*ny+ity*ny+j]);
	  if(Abs(s2->eX-s1->eX)>dxMax) continue;
	  if(Abs(s2->eY-s1->eY)>dyMax) continue;
	  eS[0].Add( s1 );
	  eS[1].Add( s2 );
	}
    }
  delete [] ind1;
  delete [] ind2;
  delete [] oc1;
  delete [] oc2;
  Log(3,"HDistance","preselection for patterns with %d and %d segments: %d comb",p1.N(), p2.N(), eS[0].GetEntries());
}

//---------------------------------------------------------------------
void  EdbAlignmentV::InitPatCellN(EdbCell2 &cell, EdbPattern &pat, int nx, int ny)
{
  // note: no correction (do we need tis function?)
  float min[2] = {pat.Xmin()-eXmarg, pat.Ymin()-eYmarg };
  float max[2] = {pat.Xmax()+eXmarg, pat.Ymax()+eYmarg };
  int   n[2]   = {nx,ny};
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell));
  Log(2, "InitPatCellN", "maxcell = %d\n",maxcell);
  cell.InitCell(maxcell,n,min,max);
}

//---------------------------------------------------------------------
void EdbAlignmentV::InitPatCellBin( int side, EdbPattern &pat, float binx, float biny)
{
  if(!SideOK(side)) return;

  EdbSegP s1min(0,pat.Xmin(), pat.Ymin(), 0,0 );
  EdbSegP s1max(0,pat.Xmax(), pat.Ymax(), 0,0 );
  float min[2] = { X(side,s1min)-eXmarg, X(side,s1min)-eYmarg };
  float max[2] = { X(side,s1max)+eXmarg, X(side,s1max)+eYmarg };

  int   n[2]   = { (int)((max[0]-min[0])/binx+1), (int)((max[1]-min[1])/biny+1) };
  int maxcell = pat.N()/n[0]/n[1]+10;
  maxcell += (int)(5*Sqrt(maxcell));
  Log(2, "InitPatCellBin", "maxcell = %d\n",maxcell);
  ePC[side].InitCell(maxcell,n,min,max);
}

//---------------------------------------------------------------------
void  EdbAlignmentV::FillGuessCell( EdbPattern &p1, EdbPattern &p2, float binOK, float offsetMax)
{
  // binOK - is the mean number of entries requested per bin

  int np1 = p1.N(), np2 = p2.N();

  EdbSegP s1min(0,p1.Xmin(), p1.Ymin(), 0,0 );
  EdbSegP s1max(0,p1.Xmax(), p1.Ymax(), 0,0 );
  EdbSegP s2min(0,p2.Xmin(), p2.Ymin(), 0,0 );
  EdbSegP s2max(0,p2.Xmax(), p2.Ymax(), 0,0 );
  float xmin1=X(0,s1min), ymin1=Y(0,s1min), xmax1=X(0,s1max), ymax1=Y(0,s1max);
  float xmin2=X(1,s2min), ymin2=Y(1,s2min), xmax2=X(1,s2max), ymax2=Y(1,s2max);

  if(Abs(xmin2-xmin1)>offsetMax)                                                // cut useless patterns margins
    if(xmin2>xmin1) xmin1=xmin2-offsetMax; 
    else  xmin2=xmin1-offsetMax;  
  if(Abs(xmax2-xmax1)>offsetMax) 
    if(xmax2>xmax1) xmax2=xmax1+offsetMax; 
    else  xmax1=xmax2+offsetMax;

  if(Abs(ymin2-ymin1)>offsetMax)                                                // cut useless patterns margins
    if(ymin2>ymin1) ymin1=ymin2-offsetMax; 
    else  ymin2=ymin1-offsetMax;  
  if(Abs(ymax2-ymax1)>offsetMax) 
    if(ymax2>ymax1) ymax2=ymax1+offsetMax; 
    else  ymax1=ymax2+offsetMax;
  

  float s1 = (xmax1-xmin1)*(ymax1-ymin1);
  float xbin1 = Sqrt( s1/(np1/binOK)  );
  float s2 = (xmax2-xmin2)*(ymax2-ymin2);
  float xbin2 = Sqrt( s2/(np2/binOK)  );
  float xbin = Min(xbin1,xbin2);

  float min[2] = { Min(xmin1,xmin2)-eXmarg, Min(ymin1,ymin2)-eYmarg };
  float max[2] = { Max(xmax1,xmax2)+eXmarg, Max(ymax1,ymax2)+eYmarg };
  int   n[2]   = { (int)((max[0]-min[0])/xbin+1), (int)((max[1]-min[1])/xbin+1) };
 
  int maxcell1 = np1/n[0]/n[1]+10;   maxcell1 += (int)(5*Sqrt(maxcell1));
  ePC[0].InitCell(maxcell1,n,min,max);

  int maxcell2 = np2/n[0]/n[1]+10;    maxcell2 += (int)(5*Sqrt(maxcell2));
  ePC[1].InitCell(maxcell2,n,min,max);

  FillCell( 0, p1 );
  FillCell( 1, p2 );

  if( Log(3, "FillGuessCell", "with patterns of %d %d  statistics:",np1,np2) ) {
    ePC[0].PrintStat();
    ePC[1].PrintStat();
  }
}

//---------------------------------------------------------------------
void  EdbAlignmentV::FillCell( int side, EdbPattern &pat )
{
  // assume that the cell is already initialized
  if(!SideOK(side)) return;
  EdbSegP *s=0;
  int n = pat.N();
  for(int i=0; i<n; i++) {
    s = pat.GetSegment(i);
    ePC[side].AddObject( X(side,*s), Y(side,*s), (TObject*)s );
  }
}

//---------------------------------------------------------------------
void  EdbAlignmentV::FillCell( int side, TObjArray &pat)
{
  // assume that the cell is already initialized
  if(!SideOK(side)) return;
  EdbSegP *s=0;
  int n = pat.GetEntriesFast();
  for(int i=0; i<n; i++) {
    s = (EdbSegP*)pat.UncheckedAt(i);
    ePC[side].AddObject( X(side,*s), Y(side,*s), (TObject*)s );
  }
}

//---------------------------------------------------------------------
Int_t  EdbAlignmentV::CalculateAffXY(TObjArray &arr1, TObjArray &arr2, EdbAffine2D &aff)
{
  int n = CheckEqualArr(arr1,arr2);
  TArrayF x1(n), x2(n), y1(n), y2(n);
  for(int i=0; i<n; i++ ) {
    EdbSegP *s1 = (EdbSegP*)arr1.UncheckedAt(i);
    EdbSegP *s2 = (EdbSegP*)arr2.UncheckedAt(i);
    x1[i] = X(0,*s1);    y1[i] = Y(0,*s1);
    x2[i] = X(1,*s2);    y2[i] = Y(1,*s2);
  }
  aff.Calculate(n,x1.GetArray(),y1.GetArray(),x2.GetArray(),y2.GetArray(),0);
  return n;
}

//---------------------------------------------------------------------
Int_t  EdbAlignmentV::CalculateAffTXTY(TObjArray &arr1, TObjArray &arr2, EdbAffine2D &aff)
{
  int n = CheckEqualArr(arr1,arr2);
  TArrayF x1(n), x2(n), y1(n), y2(n);
  for(int i=0; i<n; i++ ) {
    EdbSegP *s1 = (EdbSegP*)arr1.UncheckedAt(i);
    EdbSegP *s2 = (EdbSegP*)arr2.UncheckedAt(i);
    x1[i] = TX(0, *s1);    y1[i] = TY(0, *s1);
    x2[i] = TX(1, *s2);    y2[i] = TY(1, *s2);
  }
  aff.Calculate(n,x1.GetArray(),y1.GetArray(),x2.GetArray(),y2.GetArray(),0);
  return n;
}

//---------------------------------------------------------------------
void EdbAlignmentV::Corr2Aff(EdbLayer &layer)
{
  EdbAffine2D &a0 = *(layer.GetAffineXY());
  a0.Reset();
  a0.ShiftX(  eCorr[0].V(0) );  a0.ShiftY( eCorr[0].V(1) );
  a0.Rotate(  eCorr[0].V(6) );
  a0.ShiftX( -eCorr[1].V(0) );  a0.ShiftY( -eCorr[1].V(1) );
  layer.SetShrinkage( eCorr[0].V(5) );
  layer.SetZcorr(     eCorr[0].V(2) );
}

