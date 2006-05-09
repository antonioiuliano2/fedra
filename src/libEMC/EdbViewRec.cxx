//-- Author :  Valeri Tioukov   6.03.2006

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewRec - one microscope view reconstruction                      //
//              "hedgehog" tracking                                     //
//////////////////////////////////////////////////////////////////////////
#include "TH2F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TArrayL.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "EdbViewRec.h"
#include "EdbCluster.h"
#include "EdbFrame.h"
#include "EdbSegment.h"
#include "EdbMath.h"

ClassImp(EdbViewCell)
ClassImp(EdbViewRec)

  using namespace TMath;

//____________________________________________________________________________________
EdbViewCell::EdbViewCell()
{
  eNcell=0;
  eCells=0;
}

//____________________________________________________________________________________
void EdbViewCell::CalcN()
{
  eNx = (int)((eXmax-eXmin)/eSx)+1;
  eNy = (int)((eYmax-eYmin)/eSy)+1;
  eNcellXY = eNx*eNy;
  eNcell = eNfr*eNcellXY;
}

//____________________________________________________________________________________
int EdbViewCell::FillCell(EdbView &v)
{
  eNcl=v.Nclusters();
  if(eNcl<1) return 0;

  if(eNcell>0) CleanCell();
  CalcN();
  eCells = new TObjArray*[eNcell];
  for(int i=0; i<eNcell; i++) eCells[i]=0;
  eNeib[0]=0; 
  eNeib[1]=1;      eNeib[2]=-1;      eNeib[3]=eNx;    eNeib[4]=-eNx; 
  eNeib[5]=eNx+1;  eNeib[6]=eNx-1;   eNeib[7]=-eNx+1; eNeib[8]=-eNx-1;

  //printf("Fill %d Cells with %d clusters...\n",eNcl,eNcell);
  EdbCluster *c=0;
  int j=0;
  for(int i=0; i<eNcl; i++) {
    c = v.GetCluster(i);
    if( c->eX<eXmin || c->eX>eXmax || c->eY<eYmin || c->eY>eYmax) continue;
    j = Jcell(c->eFrame, c->eX, c->eY);
    if(!eCells[j]) eCells[j] = new TObjArray();
    eCells[j]->Add(c);
  }
  return eNcl;
}

//____________________________________________________________________________________
void EdbViewCell::CleanCell()
{
  if(eNcell>0) {
    if(eCells) {
      for(int i=0; i<eNcell; i++) {
	if(eCells[i]) delete (TObjArray*)(eCells[i]);
      }
      delete[] eCells;
      eCells=0;
    }
    eNcell=0;
  }
  return;
}

//____________________________________________________________________________________
void EdbViewCell::Print()
{
  printf("View Cell Limits      : %f %f %f %f\n", eXmin,eXmax,eYmin,eYmax);
  printf("View Cell Sx,Sy       : %f %f\n", eSx,eSy);
  printf("View Cell Nx,Ny,Nfr   : %d %d %d\n", eNx,eNy,eNfr);
  printf("View Cell Ntot        : %d\n", eNcell );
  printf("View Cell Ncl         : %d\n", eNcl );
}

//____________________________________________________________________________________
EdbViewRec::EdbViewRec()
{
  SetDef();
  SetDefRec();
}

//____________________________________________________________________________________
void EdbViewRec::SetDefRec()
{
  eClMinA = 1;     // by default accept all clusters
  eClMaxA = 10000; // by default accept almoust all clusters
  
  eNtheta = 7;
  eR.Set(eNtheta);
  eSphi.Set(eNtheta);
  eNphi.Set(eNtheta);

  eR[0] = .5;                //microns
  eR[1] =  1.;
  eR[2] =  2.;
  eR[3] =  4.;
  eR[4] =  6.;
  eR[5] =  8.;
  eR[6] =  10.;
  
  eSphi[0]= TMath::TwoPi();
  for(int i=1; i<eNtheta; i++) eSphi[i] = eR[0]/(eR[i]+eR[i-1]);

  for(int i=0; i<eNtheta; i++) 
    eNphi[i] = (int)((TMath::TwoPi() + 0.1*eSphi[i])/eSphi[i]);

  // reset Sphi for having the exact equal sectors
  for(int i=0; i<eNtheta; i++) eSphi[i] = TMath::TwoPi()/eNphi[i];

  eNphiTot = (int)(eNphi.GetSum());
  printf("eNphiTot = %d \n",eNphiTot);

  for(int i=0; i<eNtheta; i++)
    printf("eNphi[%d] = %d eSphi=%f \n",i,eNphi[i], eSphi[i] );

  eTmask.Set(eNphiTot);
  ePmask.Set(eNphiTot);
  int ii=0;
  for(int i=0; i<eNtheta; i++) {
    for(int j=0; j<eNphi[i]; j++) {
      eTmask[ii] = eR[i]/5.87;           // TODO!!!!!!
      ePmask[ii] = j*eSphi[i];
      ii++;
    }
  }

  eVC.SetLimits(eXmin,eXmax,eYmin,eYmax);
  eVC.SetS(10,10);
  eVC.CalcN();
  eNtot = eNphiTot*eVC.eNx*eVC.eNy;
  printf("Ntot = %ld \n",eNtot);

  ePhist.Set(eNtot);
  eSeedThres = 5;    // default threshold for the seed value
  eSeedLim   = 48;   // limit for the seed value
  eNseedMax  = 100;  // limit for the good seeds (segments) to be analysed (used in CheckSeedThres)
  eC = new EdbCluster*[eSeedLim*eNtot];
  eStep     = 1;
  eStepFrom = 2;
  eStepTo   = 3;

  ePulsThres = 5;    //default threshold for the segment puls value

  eNgr=0;
  eNgrMax=100000;
  eG = new TClonesArray("EdbSegment",eNgrMax);
  eView=0;
  ePS = 0;   //segments
}

//____________________________________________________________________________________
bool  EdbViewRec::SetView(EdbView *v)
{
  if(!v) return false;
  eView=v;

  if     ( eView->GetNframesTop()==0 && eView->GetNframesBot()>0 ) {  //bottom side
    eZmin = eView->GetZ1();
    eZmax = eView->GetZ2();
  }
  else if( eView->GetNframesBot()==0 && eView->GetNframesTop()>0 ) {  //top side
    eZmin = eView->GetZ3();
    eZmax = eView->GetZ4();
  } else return false;

  eZxy = 0.5*(eZmin+eZmax);
  return true;
}

//____________________________________________________________________________________
int EdbViewRec::FillViewCells()
{
  eVC.SetNfr( eView->GetNframes() );
  int n = eVC.FillCell( *eView );
  //eVC.Print();
  return n;
}

//____________________________________________________________________________________
void EdbViewRec::ResetClustersSeg()
{
  // set sl.eSegment to the default value (-1) - important only for simulation

  int ncl=eView->Nclusters();
  if(ncl>0)
    for(int i=0; i<ncl; i++) eView->GetCluster(i)->eSegment=-1;
}

//____________________________________________________________________________________
int EdbViewRec::FindGrains()
{

  if(eVC.eNcl<1) return 0;
  if(eVC.eNfr<2) return 0;
  //printf("FindGrains with %d clusters, %d frames:\n\n",eVC.eNcl,eVC.eNfr);
  if(eNgr) {eG->Delete(); eNgr=0;}

  TObjArray  *arr0=0, *arr1=0;
  EdbCluster *cl0=0,  *cl1=0;
  float dx,dy;
  int   iz1=0, jn=0;
  int   n0=0,n1=0;

  for(int iz=0;     iz<eVC.eNfr-1; iz++) {
    iz1=iz+1;
    for(int j=0;     j<eVC.eNcellXY; j++) {
      arr0 = eVC.GetCell(iz,j);
      if(!arr0)    continue;
      n0 = arr0->GetEntriesFast();
      
      for(int i0=0; i0<n0; i0++) {
	cl0 = (EdbCluster*)arr0->UncheckedAt(i0);
	
	for(int in=0; in<9; in++) {                 //neighbours
	  jn = j + eVC.Jneib(in);
	  if(jn<0)                  continue;
	  if(jn>=eVC.eNcellXY)      continue;
	  arr1 = eVC.GetCell(iz1,jn);
	  if(!arr1)    continue;
	  n1 = arr1->GetEntriesFast();
	  for(int i1=0; i1<n1; i1++) {
	    cl1 = (EdbCluster*)arr1->UncheckedAt(i1);
	    
	    dx = cl1->eX - cl0->eX;
	    if( Abs(dx) > eGrainSX ) continue;
	    dy = cl1->eY - cl0->eY;
	    if( Abs(dy) > eGrainSY ) continue;
	    
	    if( cl0->eSegment < 0 )  cl0->eSegment = eNgr++;
	    cl1->eSegment = cl0->eSegment;
	  }
	}
      }
    }
  }

  for(int i=0; i<eVC.eNcl; i++) {
    cl0 = eView->GetCluster(i);
    if( cl0->eSegment<0 ) cl0->eSegment = eNgr++;
    if(eNgr>eNgrMax) {printf("ERROR: too many grains: %d - stop tracking!!\n",eNgr); eNgr--; return -1; }
    
    if(!eG->UncheckedAt(cl0->eSegment))  new((*eG)[cl0->eSegment]) EdbSegment();
    ((EdbSegment*)eG->UncheckedAt(cl0->eSegment))->AddElement(cl0);
  }
  printf("ncl: %d   ngr: %d ( %3.1f %% )\n", eVC.eNcl,eNgr, 100.*eNgr/eVC.eNcl );

  EdbSegment *s=0;
  for(int i=0; i<eNgr; i++) {
    s = (EdbSegment*)eG->UncheckedAt(i);
    FitSegment(*s);
    s->SetID(i);
  }
  return eNgr;
}

//____________________________________________________________________________________
int EdbViewRec::CheckFramesShift()
{
  if( eNgr<1 )  return 0;
  int nfr = eView->GetNframes();
  if( nfr<2 )  return 0;

  TArrayD     Xshift(nfr),Yshift(nfr),Zshift(nfr);
  TArrayI     Nshift(nfr);
  EdbSegment  *s=0;
  EdbCluster  *cl0=0,*cl1=0;
  TObjArray   *arr=0;
  int         ncl=0;
  int         ifr=0;

  //TNtuple *fshift = new TNtuple("fshift","frames shift","x0:y0:z0:x1:y1:z1:ifr:ncl:gr");
  for(int i=0; i<eNgr; i++) {
    s = (EdbSegment*)(eG->UncheckedAt(i));
    arr = s->GetElements();
    ncl = arr->GetEntriesFast();
    if(ncl<2) continue;
    for(int j=1; j<ncl; j++) {
      cl0 = (EdbCluster*)s->GetElements()->UncheckedAt(j-1);
      cl1 = (EdbCluster*)s->GetElements()->UncheckedAt(j);
      ifr = cl1->eFrame;
      Nshift[ifr]++;
      Xshift[ifr] += (cl1->eX - cl0->eX);
      Yshift[ifr] += (cl1->eY - cl0->eY);
      Zshift[ifr] += (cl1->eZ - cl0->eZ);
      //		fshift->Fill( cl0->eX,cl0->eY,cl0->eZ, cl1->eX,cl1->eY,cl1->eZ,1.*cl1->eFrame,1.*ncl, 1.*cl0->eSegment );
    }
  }
  
  for(int i=1; i<nfr; i++) {
    Xshift[i] /= Nshift[i];
    Yshift[i] /= Nshift[i];
    Zshift[i] /= Nshift[i];
    printf("Frame shift(%2d): %+6.4f %+6.4f %f\t by %d shadows\n",i, Xshift[i],Yshift[i],Zshift[i],Nshift[i] );
  }
  
  printf("------------------------------------------------------\n");
  printf("Total shift    : %+4.2f   %+4.2f \t by %d shadows; dz=%6.2f\n", 
	 Xshift.GetSum(),Yshift.GetSum(),(int)Nshift.GetSum(),Zshift.GetSum() );
  printf("Mean  angle    : %+4.3f   %+4.3f \n\n",
	 Xshift.GetSum()/Zshift.GetSum(),
	 Yshift.GetSum()/Zshift.GetSum() );

  return (int)Nshift.GetSum();
}

//____________________________________________________________________________________
int EdbViewRec::FitSegment(EdbSegment &s)
{
  EdbCluster *c1=0;
  TObjArray *arr = s.GetElements();
  if(!arr) return -1;
  int ncl = arr->GetEntriesFast();
  if(!ncl) return 0;

  if(ncl==1) {
    c1 = (EdbCluster*)arr->UncheckedAt(0);
    s.Set( c1->X(), c1->Y(), c1->Z(), 0.,0., 0., s.GetSide(), ncl, s.GetID() );
    return ncl;
  }
  else {
    float *X = new float[ncl];
    float *Y = new float[ncl];
    float *Z = new float[ncl];
    float *W = new float[ncl];
    for(int i=0; i<ncl; i++) {
      c1 = (EdbCluster*)arr->UncheckedAt(i);
      W[i] = c1->eArea;
      X[i] = c1->eX;
      Y[i] = c1->eY;
      Z[i] = c1->eZ;
    }
    float zmin=Z[0],zmax=Z[0];
    for(int i=1; i<ncl; i++) { 
      if(zmin>Z[i]) zmin=Z[i];
      if(zmax<Z[i]) zmax=Z[i];
    }
    float x0,y0,z0,tx,ty,ex,ey;
    EdbMath::LFIT3( X, Y, Z, W, ncl, 
		    x0, y0, z0, tx, ty, ex, ey );
    
    s.Set( x0,y0,z0, tx,ty, zmax-zmin, s.GetSide(), ncl, s.GetID());
    s.SetSigma(ex,ey);
    delete[] X;
    delete[] Y;
    delete[] Z;
    delete[] W;
  }
  return ncl;
}

///------------------------------------------------------------------
namespace GRAINS_TREE
{
  TClonesArray *eClust;
  EdbSegment   *eSeg;
  Int_t        eViewID;
  Int_t        eAreaID;
  Float_t      eXview;
  Float_t      eYview;
}

///------------------------------------------------------------------
void EdbViewRec::InitGrainsTree(const char *file)
{
  using namespace GRAINS_TREE;

  TFile *f = new TFile(file,"RECREATE");
  eGrainsTree = new TTree("grains","grains");
  eClust = new TClonesArray("EdbCluster");
  eSeg   = 0;
  eGrainsTree->Branch("view",&eViewID,"view/I");
  eGrainsTree->Branch("area",&eAreaID,"area/I");
  eGrainsTree->Branch("xv",&eXview,"xv");
  eGrainsTree->Branch("yv",&eYview,"xv");
  eGrainsTree->Branch("c",&eClust);
  eGrainsTree->BranchOld("s","EdbSegment",&eSeg);
}

///------------------------------------------------------------------
int EdbViewRec::FillGrainsTree()
{
  using namespace GRAINS_TREE;
  if( eNgr<1 ) return 0;

  eViewID = eView->GetViewID();
  eAreaID = eView->GetAreaID();
  eXview = eView->GetXview();
  eYview = eView->GetYview();
  EdbCluster *cl=0;
  int ncl=0;
  for(int i=0; i<eNgr; i++) {
    eSeg = (EdbSegment*)(eG->UncheckedAt(i));
    //printf("id=%d\n",s->GetID());
    ncl = eSeg->GetElements()->GetEntriesFast();
    if(ncl>1) {
      for(int j=0; j<ncl; j++) {
	cl = (EdbCluster*)eSeg->GetElements()->UncheckedAt(j);
	new((*eClust)[j])  EdbCluster( *cl );
      }
      eGrainsTree->Fill();
    }
    eClust->Clear();
  }
  eGrainsTree->AutoSave();
  return eNgr;
}

//---------------------------------------------------------------------------------
int EdbViewRec::SeekBySeed(EdbSegment &s, float t, float p, int j)
{
// perform the carefull segment reconstruction starting from the seed;
// assuming that eView is available and eCell is filled

  printf("SeekBySeed: phi=%f theta = %f \n", p,t);
  TObjArray av;  // 3-vectors
  TVector3 *v;
  TObjArray *arr=0;
  EdbCluster *c;
  int n,jn, ntot=0;
  for(int iz=0; iz<eVC.eNfr; iz++) {
    for(int in=0; in<9; in++) {                 //neighbours
      jn = j + eVC.Jneib(in);
      if(jn<0)                  continue;
      if(jn>=eVC.eNcellXY)      continue;

      arr = eVC.GetCell(iz,jn);
      if(!arr)    continue;
      n = arr->GetEntriesFast();
      for(int i=0; i<n; i++) {
	c = (EdbCluster*)arr->UncheckedAt(i);
	v = new TVector3(c->eX,c->eY,c->eZ);
	av.Add(v);
	c->eSegment = s.GetID();
	ntot++;
      }
    }
  }

  float ax= eGrainSX*Sin(p);
  float ay= eGrainSY*Cos(p);
  float sy = Sqrt(ax*ax+ay*ay);
  float sx = eGrainSZ*Sin(t);
  ax = eGrainSX*Cos(p);
  ay = eGrainSY*Sin(p);
  sx = Sqrt(sx*sx + ax*ax + ay*ay);
  sx *=4.;
  sy *=3.;
  printf("sx = %f sy = %f\n",sx,sy);

  TIndexCell ics;
  Long_t val[3];               //[x:y:i]
  for(int i=0; i<ntot; i++) {
    v = (TVector3*)av.At(i);
    v->RotateZ(-p);
    v->RotateY(-t);
    val[0] = (Long_t)(v->X()/sx);
    val[1] = (Long_t)(v->Y()/sy);
    val[2] = i;
    ics.Add(3,val);
  }
  ics.Sort();

  int maxn=0;
  const TIndexCell *icmax=0;
  for(int i0=0; i0<ics.GetEntries(); i0++) 
    for(int i1=0; i1<ics.At(i0)->GetEntries(); i1++) {
      printf("%d %d %d\n",ics.At(i0)->Value(),ics.At(i0)->At(i1)->Value(),ics.At(i0)->At(i1)->N());
      if( maxn < ics.At(i0)->At(i1)->N()) {
	icmax = ics.At(i0)->At(i1);
	maxn = icmax->N();
      }
    }

  if(maxn<4)   return maxn;
  //printf("maxn = %d\n", maxn);
  //ics.PrintStat();
  //icmax->Print("");

  return maxn;
}

//____________________________________________________________________________________
int EdbViewRec::SelectSegments()
{
  int good = 0;

  //printf("select segments with SeeedThres >= %d\n",eSeedThres);

  TIndexCell isc;             // segments: "iseg:icl"
  int jmax;
  for(int i=0; i<eNtot; i++) {
    if( ePhist[i] < eSeedThres )   continue;
    jmax = Min(eSeedLim,ePhist[i]);
    for(int j=0; j<jmax; j++) 
      isc.FindAdd(i)->FindAdd( eView->GetClusters()->IndexOf(eC[i*eSeedLim+j]) );
  }
//   isc.SetName("iseg:icl");
//   isc.Sort();
//   isc.Print("");

  int nseg = isc.GetEntries();
  if(nseg<1) return 0;

  TIndexCell *is1=0, *is2=0;
  for(int i1 = nseg-1; i1>=0; i1--) {
    is1 = isc.At(i1);     //segment 1
    for(int i2 = 0; i2<i1; i2++) {
      is2 = isc.At(i2);     //segment 2

      for(int j2=0; j2<is2->N(); j2++) 
	if( is1->Find( is2->At(j2)->Value() ) )   { //has common clusters
	  for(int j1=0; j1<is1->N(); j1++) 
	    is2->FindAdd( is1->At(j1)->Value() );
	  isc.Drop(i1);
	  goto NEXT_S1;
	}
    }
  NEXT_S1:
    continue;
  }
  isc.Purge(2);
  isc.Sort();
//   isc.SetName("iseg:icl");
//   isc.Print("");


  EdbCluster *c=0;
  EdbSegment *s=0;
  for(int i = 0; i<isc.GetEntries(); i++) 
    if(isc.At(i)->N()>1) {
      s = new EdbSegment();
      for(int j=0; j<isc.At(i)->N(); j++ ) {
	//printf("icl = %d \n", isc.At(i)->At(j)->Value());
	c = eView->GetCluster( isc.At(i)->At(j)->Value() );
	s->AddElement(c);
      }
      s->SetPuls(s->GetNelements());
      s->SetIDE(good++);
      FitSegment(*s);
      eView->AddSegment(s);
      s->Print();
    }
  
  //eView->Print();


  /*

  EdbSegment *sbase=0;
  for(int i=0; i<nseg; i++) {                      // join short to long
    s = (EdbSegment*)ePS->At(i);
    if(s->GetPuls()<1) continue;
    ncl = s->GetNelements();
    for(int j=0; j<ncl; j++) {                     // check for the alias clusters
      c = (EdbCluster*)s->GetElements()->At(j);
      if(c->eSegment != s->GetID())
	if(ePS->At(c->eSegment)) 
	  sbase = (EdbSegment*)ePS->At(c->eSegment);
    }

    if(sbase)  {                                   // join s to sbase
      for(int j=0; j<ncl; j++) {
	c = (EdbCluster*)s->GetElements()->At(j);
	sbase->AddElement(c);
      }
      s->SetPuls(0);
      sbase->SetPuls(s->GetNelements());
      sbase->SetIDE();
      sbase->Print();
    }
  }

  for(int i=0; i<nseg; i++) {
    s = (EdbSegment*)ePS->At(i);
    if(s->GetPuls()>0)  {
      FitSegment(*s);
      good++;
      s->Print();
    }
  }
  */

  return good;
}

//____________________________________________________________________________________
int EdbViewRec::CheckSeedThres()
{
  TArrayL sp(256);   //phase occupancy histogram
  for (int i=0; i<eNtot; i++) 
    if(ePhist[i]>0)
      sp[ePhist[i]]++;
  
  int thres=0, sum=0, sumthres=0;
  for (int i=255; i>0; i--) {
    if(sp[i]<1) continue;
    printf("%3d %ld\n",i,sp[i]);
    if(!thres) sumthres=sum;
    sum += sp[i];
    if(!thres&&sum>eNseedMax) thres = i+1;
  }
  if(thres>eSeedThres) eSeedThres=thres;
  printf("sumthres(%d) = %d   eSeedThres= %d\n\n",thres,sumthres, eSeedThres);

  /*
  TNtuple *ntpix = new TNtuple("ntpix","pixels","i:pix:ix:iy:t:p");
  float p,t;
  int iphi,ix,iy;
  int jxy=0;
  for (int i=0; i<eNtot; i++) {
    if(ePhist[i]<eSeedThres) continue;
    iphi = i%eNphiTot;
    t = eTmask[iphi];
    p = ePmask[iphi];
    jxy = i/eNphiTot;
    iy  = jxy/eVC.eNx;
    ix  = jxy%eVC.eNx;
    ntpix->Fill(i,ePhist[i],ix,iy,t,p);
  }
  */
  return thres;
}

//____________________________________________________________________________________
int EdbViewRec::FindSeed()
{
  
  if(eVC.eNcl<1) return 0;
  if(eVC.eNfr<2) return 0;
  printf("\nFindSeed with %d clusters, %d frames \t eZxy[%6.1f:%6.1f] = %6.1f\n",
	 eVC.eNcl,eVC.eNfr,eZmin,eZmax,eZxy);

  bzero(ePhist.GetArray(),eNtot*sizeof(Short_t));

  TObjArray  *arr0=0, *arr1=0;
  EdbCluster *cl0=0,  *cl1=0;
  int   iz1=0, jn=0;
  int   n0=0,n1=0;
  int ncycle=0, ict=0, nfilled=0;
  float r2max=eVC.eSx*eVC.eSx;
  float dx,dy,dz,r2;
  float r,phi,t;
  float zratio,xZxy,yZxy;

  for(int istep=eStepFrom; istep<eStepTo; istep+=eStep) {
    for(int iz=0;     iz<eVC.eNfr-istep; iz++) {
      iz1=iz+istep;
      
      dz = eView->GetFrame(iz1)->GetZ() - eView->GetFrame(iz)->GetZ();
      zratio = (eZxy  - eView->GetFrame(iz)->GetZ()) / dz;
      //printf("dz: %f\n",dz);

      for(int j=0;     j<eVC.eNcellXY; j++) {
	arr0 = eVC.GetCell(iz,j);
	if(!arr0)    continue;
	n0 = arr0->GetEntriesFast();
	
	for(int i0=0; i0<n0; i0++) {
	  cl0 = (EdbCluster*)arr0->UncheckedAt(i0);
	  
	  for(int in=0; in<9; in++) {                 //neighbours
	    jn = j + eVC.Jneib(in);
	    if(jn<0)                  continue;
	    if(jn>=eVC.eNcellXY)      continue;
	    arr1 = eVC.GetCell(iz1,jn);
	    if(!arr1)    continue;
	    n1 = arr1->GetEntriesFast();
	    for(int i1=0; i1<n1; i1++) {
	      cl1 = (EdbCluster*)arr1->UncheckedAt(i1);
	      
	      dx = cl1->eX - cl0->eX;
	      dy = cl1->eY - cl0->eY;
	      r2 = (dx*dx+dy*dy);
	      if(r2>r2max)        continue;

	      xZxy = cl0->eX + dx*zratio;
	      if(xZxy<eXmin)        continue;
	      if(xZxy>eXmax)        continue;
	      
	      yZxy = cl0->eY + dy*zratio;
	      if(yZxy<eYmin)        continue;
	      if(yZxy>eYmax)        continue;
	      
	      ncycle++;
	      
	      phi = Pi() + ATan2(dy,dx);
	      r   = Sqrt(r2);
	      t   = ATan2(r,dz);
	      
	      int iphi=0;
	      int it=0;
	      for(it=0; it<eNtheta-1; it++) {
		if(r>eR[it]) iphi+=eNphi[it];
		else break;
	      }
	      
	      iphi += (int)(phi/eSphi[it] + 0.00001);
	      
	      ict = eVC.JcellXY(xZxy,yZxy)*eNphiTot + iphi;
	      
	      if(ePhist[ict]<1) nfilled++;
	      if(ePhist[ict]<eSeedLim) eC[ict*eSeedLim+ePhist[ict]]=cl0;
	      //printf("ict = %d  ePhist = %d  ind=%d\n",ict, ePhist[ict], ict*eSeedLim+ePhist[ict]);
	      ePhist[ict]++;
	      if(ePhist[ict]<eSeedLim) eC[ict*eSeedLim+ePhist[ict]]=cl1;
	      ePhist[ict]++;

	    }
	  }
	}
      }
    }
  }
  printf("occupancy: %d / %d = %5.2f %% \n\n", nfilled,eNtot, 100.*nfilled/eNtot);
  return ncycle;
}

/*
  Double_t *w = new Double_t[ncl];
  Short_t *sw = (Short_t*)w;

  for(int i=0; i<ncl; i++) {
    cl = v.GetCluster(i);
    sw[4*i+1] = cl->GetFrame();
    sw[4*i+2] = (Short_t)((cl->eX-eXmin));
    sw[4*i+3] = (Short_t)((cl->eY-eYmin));

    //w[i] = iz*100000+ix+iy/100000;
  }

  TIndex2 iclu(0);
  iclu.BuildIndex(ncl,w);
*/

  //TClonesArray *clone = v.GetClusters();
  //clone->Sort();


//   for(int j=0; j<eNcell; j++) {

//     ix = j%eNx;
//     iy = j/eNx;
    
//     if(!cmatr.UncheckedAt(j))  	h2->Fill( i,j,cmatr.At(j)->GetEntries() );

//     ((TObjArray*)cmatr.At(j))->Add(cl);
//   }

//   long r2;
//   for(int j=0; j<1000; j++)
//     for(int i=0; i<eNcell; i++) {
//       ix = i/eNx;
//       iy = i%eNx;
//       r2 = ix*ix+iy*iy;
//       //if(r2>100) continue;
//       if( cmatr.At(i) ) ncycle++;
//     }

