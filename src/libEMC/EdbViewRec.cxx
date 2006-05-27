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
void EdbViewCell::Init()
{
  eNx = (int)((eXmax-eXmin)/eSx)+1;
  eNy = (int)((eYmax-eYmin)/eSy)+1;
  eNcellXY = eNx*eNy;
  eNcell = eNfr*eNcellXY;
}

//____________________________________________________________________________________
int EdbViewCell::FillCell(TClonesArray &v, int flag)
{
  // if flag ==0 (default) use all clusters
  // else use only clusters with eSegment==eFlag (-10 - grains)

  eNcl=v.GetLast()+1;
  if(eNcl<1) return 0;

  if(eNcell>0) CleanCell();
  Init();
  eCells = new TObjArray*[eNcell];
  for(int i=0; i<eNcell; i++) eCells[i]=0;
  eNeib[0]=0; 
  eNeib[1]=1;      eNeib[2]=-1;      eNeib[3]=eNx;    eNeib[4]=-eNx; 
  eNeib[5]=eNx+1;  eNeib[6]=eNx-1;   eNeib[7]=-eNx+1; eNeib[8]=-eNx-1;

  //printf("Fill %d Cells with %d clusters...\n",eNcl,eNcell);
  EdbCluster *c=0;
  int j=0;
  for(int i=0; i<eNcl; i++) {
    c = (EdbCluster*)v.UncheckedAt(i);
    if(flag) if(c->eSegment!=flag)                                continue;
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
  SetPrimary();
}

//____________________________________________________________________________________
void EdbViewRec::SetPrimary()
{
	// define default primary values used later for the calculations in Init() function
	// before Init them can be modified by correspondent setters

  eClMinA = 1;     // by default accept all clusters
  eClMaxA = 10000; // by default accept almoust all clusters

  eVC.SetLimits(eXmin,eXmax,eYmin,eYmax);
  eVC.SetS(10,10);                         // cell size

  eDZmin = 5.87;   // TODO!!!
  enT = 7;         // number of Theta divisions!
  
  eThetaLim  = 1.5;     // max theta angle
  enSeedsLim = 100000;  // limit for the total number of seeds
  eSeedLim   = 48;      // limit for the seed value
  eNseedMax0 = 10;      // starting limit for the good seeds (segments) to be analysed (used in CheckSeedThres)/theta
  eStep     = 1;
  eStepFrom = 2;
  eStepTo   = 3;
  eRmax     = 11.;      // limit on 3-dim distance between points [microns]
  eSeedThres0   = 5;   // limit for the seed value
  eFact=1.;            // occupancy correction factor

  ePulsThres = 4;    //default threshold for the segment puls value

  eNgr=0;
  eNgrMax=100000;           // limit for the grains number
  eAddGrainsToView = false; // use a dedicated array for eG
  eNclGrMin = 1;
  eNclGrMax = 6;

// set to 0 all pointers 
  eView=0;
  eG  = 0;          // grains
  eGrainsTree = 0;
}

//____________________________________________________________________________________
bool EdbViewRec::Init()
{
	// allocate memory here, return true if ok
	// all settings must be done before!

  printf("\nInit reconstructor:\n\n");

  if( eSeedThres.GetSize() != enT ) {     // set default threshold for the seed value
    eSeedThres.Set(enT);
    for(int i=0; i<enT; i++) eSeedThres[i]=eSeedThres0;
  }
  if( eNseedMax.GetSize() != enT ) {     // set default threshold for the seed value
    eNseedMax.Set(enT);
    for(int i=0; i<enT; i++) eNseedMax[i]=eNseedMax0;
  }

  enP.Set(enT);        // number of Phi divisions [enT]
  eTheta.Set(enT);     // limits of theta divisions [enT]
  eR.Set(enT);         // limits of theta divisions [enT]
  esP.Set(enT);        // number of Phi divisions [enT]
  esY.Set(enT);        // step   of Y divisions [enT]
  esX.Set(enT);        // step   of X divisions [enT]

  printf("enT \t= %d\n",enT);
  float st=0, sp=0, t0=0, dt = 0.;
  for(int it=0; it<enT; it++) {
    st = 1.8*SThetaGr( t0+dt , 0., eDZmin , eGrainSX, eGrainSY, eGrainSZ);
    dt = st/2.;
    eTheta[it] = t0 + st;
    t0 = eTheta[it];
    eR[it] = eDZmin*Tan(eTheta[it]);
    sp = Min( TwoPi(), 2.*SPhiGr( eTheta[it]-st , 0., eDZmin , eGrainSX, eGrainSY, eGrainSZ) );
    enP[it] = (int)(TwoPi()/sp + 0.001);
    esP[it] = TwoPi()/enP[it];            // reset Sphi for having the exactly equal sectors
    
    esX[it] = st/Cos(eTheta[it])*20. * 2.;
    esY[it] = eTheta[0]/Cos(eTheta[0])*20. * 2.;
  }

  enPtot = (int)(enP.GetSum());     // total number of phi divisions
  enY.Set(enPtot);                  // number of Y divisions [enPtot]
  printf("enPtot \t= %d\n",enPtot);

  eDmax = Sqrt( (eYmax-eYmin)*(eYmax-eYmin) + (eXmax-eXmin)*(eXmax-eXmin) );
  eFact = (eDmax*eDmax) / ((eYmax-eYmin)*(eXmax-eXmin));

  int kp=0;
  for(int it=0; it<enT; it++) {
    for(int ip=0; ip<enP[it]; ip++) {
      enY[kp] = (int)(eDmax/esY[it] + 1);
      kp++;
    }
  }
  enYtot = (int)(enY.GetSum());  // total number of Y divisions
  enX.Set(enYtot);               // number of X divisions [enYtot]
  printf("enYtot \t= %d\n",enYtot);

  kp=0;
  int ky=0;
  for(int it=0; it<enT; it++) {
    for(int ip=0; ip<enP[it]; ip++) {
      for(int iy=0; iy<enY[kp]; iy++) {
	enX[ky] = (int)(eDmax/esX[it] + 1);
	ky++;
      }
      kp++;
    }
  }
  enXtot = (int)(enX.GetSum());    // total number of X divisions
  printf("enXtot \t= %d\n",enXtot);
  printf("eDmax \t= %f  eFact = %f \n",eDmax, eFact);

  ehX = new Short_t[enXtot];		//- phase histogram
  epY = new Short_t*[enYtot];		//- pointers to the first x[iy]
  epP = new Short_t**[enPtot];		//- pointers to the first y[ip]
  epT = new Short_t***[enT];		//- pointers to the first phi[it]

  epS = new EdbCluster**[enXtot];	    //- pointers to seeds lists (in epC)
  epC = new EdbCluster*[enSeedsLim*eSeedLim];  //- pointers to clusters

  Short_t *px = ehX;
  epY[0] = px;
  for(int iy=1; iy<enYtot; iy++) {	px += enX[iy-1]; epY[iy]=px; }

  Short_t **py = epY;
  epP[0] = py;
  for(int ip=1; ip<enPtot; ip++) {	py += enY[ip-1]; epP[ip]=py; }

  Short_t ***pp = epP;
  epT[0] = &(epP[0]);
  for(int it=1; it<enT; it++)    {	pp += enP[it-1]; epT[it]=pp; }

  for(int it=0; it<enT; it++)
    printf("%d theta = %5.4f  (r= %4.2f)\t esP = %f x %d \t sx = %f sy = %f \tseedthres = %d \tNseedsmax = %d\n",
	   it,      eTheta[it], eR[it],
	   esP[it], enP[it], 
	   esX[it], esY[it],
	   eSeedThres[it],
	   eNseedMax[it]);
  
  /*  
  printf("\ncheck index:\n");
  kp=0;
  for(int it=0; it<enT; it++)
    for(int ip=0; ip<enP[it]; ip++) {
      for(int iy=0; iy<enY[ip]; iy++) {
	printf("ind[ %d %d %d ] = %d\n",it,ip,iy, epT[it][ip][iy]-ehX);
      }
    }
  */

  eVC.Init();

  if(!eAddGrainsToView) 
    if(eNgrMax>0) eG = new TClonesArray("EdbSegment",eNgrMax);

  return true;
}

//____________________________________________________________________________________
void EdbViewRec::InitR()
{
  eR[0] = .5;                //microns
  eR[1] =  1.;
  eR[2] =  2.;
  eR[3] =  4.;
  eR[4] =  6.;
  eR[5] =  8.;
  eR[6] =  10.;
}

//---------------------------------------------------------------------------------
float EdbViewRec::SThetaGr(float theta, float phi, float dz, float sx, float sy, float sz)
{
	// cone aperture theta angle for grain of size sx,sy,sz  visible at theta,phi,dz
	return ATan( Sqrt( sz*Sin(theta)*sz*Sin(theta) +
		sx*Sin(phi)*sx*Sin(phi) + sy*Cos(phi)*sy*Cos(phi) )  /
		(dz/Cos(theta)) );
}

//---------------------------------------------------------------------------------
float EdbViewRec::SPhiGr(float theta, float phi, float dz, float sx, float sy, float sz)
{
	// cone aperture phi angle for grain of size sx,sy,sz  visible at theta,phi,dz
	float rz = Abs(dz*Tan(theta));
	float rg = Sqrt( sx*Sin(phi)*sx*Sin(phi) + sy*Cos(phi)*sy*Cos(phi) );
	if(rz<rg) return TwoPi();
	return   ATan(rg/rz);
}

//____________________________________________________________________________________
bool  EdbViewRec::SetView(EdbView *v)
{
  if(!v)   return false;
  eView = v;
  eCL   = v->GetClusters();
  if(!eCL) return false;

  if     ( eView->GetNframesTop()==0 && eView->GetNframesBot()>0 ) {  //bottom side
    eZmin = eView->GetZ1();
    eZmax = eView->GetZ2();
    eZxy     = eZmax;        // base point
  }
  else if( eView->GetNframesBot()==0 && eView->GetNframesTop()>0 ) {  //top side
    eZmin = eView->GetZ3();
    eZmax = eView->GetZ4();
    eZxy     = eZmin;        // base point
  } else return false;
  eZcenter = 0.5*(eZmin+eZmax);
  if(eAddGrainsToView) eG = eView->GetSegments();
  return true;
}

//____________________________________________________________________________________
int EdbViewRec::FillViewCells(int flag)
{
  // if flag ==0 (default) use all clusters
  // else use only clusters with eSegment==eFlag (-10 - grains)

  eVC.SetNfr( eView->GetNframes() );
  int n = eVC.FillCell( *(eCL), flag );
  //eVC.Print();
  return n;
}

//____________________________________________________________________________________
void EdbViewRec::ResetClustersSeg()
{
  // set sl.eSegment to the default value (-1) - important only for simulation

  int ncl=eView->Nclusters();
  if(ncl>0)
    for(int i=0; i<ncl; i++) ((EdbCluster*)eCL->UncheckedAt(i))->eSegment=-1;
}

//____________________________________________________________________________________
int EdbViewRec::FindGrains(int option)
{
  // if option = 0 (default) grains reconstructed as a segments and added to eG  array
  // if option = 1           grains reconstructed as a clusters and added to eCL array

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
    cl0 = (EdbCluster*)eCL->UncheckedAt(i);
    if( cl0->eSegment<0 ) cl0->eSegment = eNgr++;
    if(eNgr>eNgrMax) {printf("ERROR: too many grains: %d - stop tracking!!\n",eNgr); eNgr--; return -1; }
    
    if(!eG->UncheckedAt(cl0->eSegment))  new((*eG)[cl0->eSegment]) EdbSegment();
    ((EdbSegment*)eG->UncheckedAt(cl0->eSegment))->AddElement(cl0);
  }
  printf("ncl: %d   ngr: %d ( %3.1f %% )\n", eVC.eNcl,eNgr, 100.*eNgr/eVC.eNcl );

  EdbSegment *s=0;
  EdbCluster c;
  for(int i=0; i<eNgr; i++) {
    s = (EdbSegment*)eG->UncheckedAt(i);
    s->SetID(i);
    if( s->GetNelements() < eNclGrMin) s->UnSetIDE();  // release clusters
    else     {
      if(option==1)  {
	FitSegmentToCl(*s,c, 1);
	c.eSegment = -10;  // to distinguish grains from clusters
	new((*eCL)[eCL->GetLast()+1])  EdbCluster(c);
	s->UnSetIDE();
      }
      else           FitSegment(*s,1);
    }
    s->SetPuls(s->GetNelements());
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
int EdbViewRec::FitSegment(EdbSegment &s, int wkey)
{
  // if wkey = 0 - equal weights (default)
  //         = 1 - weighted with cluster area

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
      if(wkey==1) W[i] = c1->eArea;
      else        W[i] = 1.;
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

//____________________________________________________________________________________
int EdbViewRec::FitSegmentToCl(EdbSegment &s, EdbCluster &c, int wkey)
{
  // Special function to calculate the grain parameters and fill the output cluster c
  // if wkey = 0 - equal weights (default)
  //         = 1 - weighted with cluster area

  EdbCluster *c1=0;
  TObjArray *arr = s.GetElements();
  if(!arr) return -1;
  int ncl = arr->GetEntriesFast();
  if(!ncl) return 0;

  if(ncl==1) {
    c1 = (EdbCluster*)arr->UncheckedAt(0);
    c.Set( c1->eX, c1->eY, c1->eZ, c1->eArea, c1->eVolume, c1->eFrame, c1->eSide, c1->eSegment );
    return ncl;
  }
  else {
    double SX=0,SY=0,SZ=0;
    float  SA=0,SV=0,SF=0,SW=0;
    float  w;
    for(int i=0; i<ncl; i++) {
      c1 = (EdbCluster*)arr->UncheckedAt(i);
      if(wkey==1) w = c1->eArea;
      else        w = 1.;
      SX += w*c1->eX;
      SY += w*c1->eY;
      SZ += w*c1->eZ;
      SF += w*c1->eFrame;
      SW += w;
      SA +=   c1->eArea;
      SV +=   c1->eVolume;
    }
    c.Set( (float)(SX/SW), (float)(SY/SW), (float)(SZ/SW), SA, SV, Nint(SF/SW), c1->eSide, c1->eSegment );
  }
  return ncl;
}

///______________________________________________________________________________
float EdbViewRec::CalculateSegmentChi2( EdbSegment &s, float sx, float sy, float sz )
{
	//assumed that clusters are attached to segments
  TObjArray *clusters = s.GetElements();
  if(!clusters) return 0;
  int ncl = clusters->GetLast()+1;
  if(ncl<=0)     return 0;

  // segment line parametrized as 2 points
  float xyz1[3] = { s.GetX0() /sx, 
		    s.GetY0() /sy, 
		    s.GetZ0() /sz };
  float xyz2[3] = { (s.GetX0() + s.GetDz()*s.GetTx()) /sx,
		    (s.GetY0() + s.GetDz()*s.GetTy()) /sy,
		    (s.GetZ0() + s.GetDz())           /sz };

  bool        inside=true;
  double      d, chi2=0;
  EdbCluster  *cl=0;
  float       xyz[3];
  for(int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)clusters->At(i);
    xyz[0] = cl->GetX()/sx;
    xyz[1] = cl->GetY()/sy;
    xyz[2] = cl->GetZ()/sz;
    d = EdbMath::DistancePointLine3(xyz,xyz1,xyz2, inside);
    chi2 += d*d;
  }
  return TMath::Sqrt(chi2/ncl);
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

  TFile *f;
  f = new TFile(file,"RECREATE");
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

//____________________________________________________________________________________
int EdbViewRec::SelectSegments()
{
  int good = 0;
  eISC.Delete();

  EdbCluster **pps;
  int jmax;
  Short_t *p0,*pnext, *plast = ehX+enXtot;
  for(int it=0; it<enT; it++) {
    p0    = epT[it][0][0];
    if(it==enT-1) pnext = plast;
    else          pnext = epT[it+1][0][0];
    while(p0<pnext) {
      jmax = *p0;
      if( jmax>=eSeedThres[it] ) {
	//	  printf("it,jmax,thres %d %d %d \n",it,jmax, eSeedThres[it]);
	int ict = p0-ehX;
	pps = epS[ict];
	jmax = (jmax<eSeedLim)?jmax:eSeedLim;
	for(int j=0; j<jmax; j++) {
	  eISC.FindAdd(ict)->FindAdd( eCL->IndexOf( *(pps) ) );
	  pps++;
	}
	good++;
      }
      p0++;
    }
  }

//   eISC.SetName("iseg:icl");
//   eISC.Print("");

  printf("%d seed segments selected\n", good);
  return good;
}

//____________________________________________________________________________________
int EdbViewRec::SelectSegments2()
{
  int good  = 0;
  int seeds = 0;

  EdbSegment *s=0;
  EdbCluster *c=0;
  EdbCluster **pps;
  int jmax;
  Short_t *p0,*pnext, *plast = ehX+enXtot;
  for(int it=0; it<enT; it++) {
    p0    = epT[it][0][0];
    if(it==enT-1) pnext = plast;
    else          pnext = epT[it+1][0][0];
    while(p0<pnext) {
      jmax = *p0;
      if( jmax>=eSeedThres[it] ) {
	int ict = p0-ehX;
	pps = epS[ict];
	jmax = (jmax<eSeedLim)?jmax:eSeedLim;
	seeds++;
	s = new EdbSegment();
	for(int j=0; j<jmax; j++) {
	  c = *pps;
	  if( !(s->GetElements()) ) s->AddElement(c);
	  else if( !(s->GetElements()->FindObject(c)) ) s->AddElement(c);
	  pps++;
	}
	if( GoodSegment(*s) )  {
	  s->SetIDE(good++);
	  eView->AddSegment(s);
	}
	else delete s;
      }
      p0++;
    }
  }

  printf("%d ( %d ) segments (seeds) are selected\n", good,seeds);
  return good;
}

//____________________________________________________________________________________
bool EdbViewRec::GoodSegment(EdbSegment &s)
{
  // TODO: 
  // - probability  to be a straight line  (chi2)
  // - probability  to be a grain (big&long)
  // - probability  to be a casual coincidence of more grains

  if(!s.GetElements())            return false;
  s.SetPuls(s.GetNelements());
  if( s.GetPuls() < ePulsThres )  return false;
  FitSegment(s);
  if( Sqrt(s.GetTx()*s.GetTx()+s.GetTy()*s.GetTy()) > eThetaLim )     return false;
  s.SetSigma( Sqrt(s.GetSigmaX()*s.GetSigmaX() + s.GetSigmaY()*s.GetSigmaY()),
	       CalculateSegmentChi2(s, eGrainSX, eGrainSY, eGrainSZ) );
  if( s.GetSigmaY() > 3.5 )       return false;

  return true;
}

//____________________________________________________________________________________
int  EdbViewRec::RefillSegment(EdbSegment &s)
{
  // attach to the segment all clusters(grains) close to him
  // assumed the segment is fitted

  float dz0 = s.GetZ0() - eZmin;
  float dz1 = eZmax - s.GetZ0();
  float v[3];
  // segment line in a normalized space:
  float start[3]= { (s.GetX0()-dz0*s.GetTx())/eGrainSX, 
		    (s.GetY0()-dz0*s.GetTy())/eGrainSY, 
		    eZmin/eGrainSZ };
  float end[3]  = { (s.GetX0()+dz1*s.GetTx())/eGrainSX, 
		    (s.GetY0()+dz1*s.GetTy())/eGrainSY, 
		    eZmax/eGrainSZ };

  float sx = Sqrt( eGrainSZ*eGrainSZ*s.GetTx()*s.GetTx() + eGrainSX*eGrainSX );
  float sy = Sqrt( eGrainSZ*eGrainSZ*s.GetTy()*s.GetTy() + eGrainSY*eGrainSY );

  TObjArray  *cla=0;
  TObjArray  *ela=s.GetElements();
  ela->Clear();
  EdbCluster *c=0;
  float x0,y0,x,y,dz;
  int   n1, lx, ly, lx0,ly0;
  for(int i=0; i<eVC.eNfr; i++) {
    dz = eView->GetFrame(i)->GetZ() - s.GetZ0();
    x0  = s.GetX0() + s.GetTx()*dz;
    y0  = s.GetY0() + s.GetTy()*dz;
    lx0 = eVC.IXcell(x0);
    ly0 = eVC.IYcell(y0);
    for(int ix=-1; ix<2; ix++) 
      for(int iy=-1; iy<2; iy++) {
	x = x0 + ix*sx;
	if(x<eVC.eXmin||x>eVC.eXmax) continue;
	y = y0 + iy*sy;
	if(y<eVC.eYmin||y>eVC.eYmax) continue;
	lx = eVC.IXcell(x);
	ly = eVC.IYcell(y);
	if(ix!=0&&iy!=0) 
	  if( lx == lx0 && ly == ly0 ) continue;
	cla = eVC.GetCell(i,lx,ly);
	if(!cla) continue;
	n1 = cla->GetEntriesFast();
	for(int i1=0; i1<n1; i1++) {
	  c = (EdbCluster*)cla->UncheckedAt(i1);
	  v[0] = c->eX/eGrainSX;
	  v[1] = c->eY/eGrainSY;
	  v[2] = c->eZ/eGrainSZ;
	  if( EdbMath::DistancePointLine3(v,start,end, true) < 2. )
	    if( !(ela->FindObject(c)) )  ela->Add( c );
	}
      }
  }
  int nn = ela->GetEntriesFast();
  s.SetPuls(nn);
  return nn;
}

//____________________________________________________________________________________
int EdbViewRec::RefitSegments()
{
  int good=0;
  int nseg = eView->Nsegments();
  EdbSegment *s;
  for(int i=0; i<nseg; i++) {
    s = eView->GetSegment(i);
    if(s->GetPuls()<1)  continue;
    s->UnSetIDE();
    RefillSegment(*s);
    if( !GoodSegment(*s) ) {  
      s->UnSetIDE(); 
      s->SetPuls(0); 
      continue; 
    }
    s->SetIDE(i);
    good++;
  }

  printf("%d segments after refitting\n", good);
  return good;
}
//____________________________________________________________________________________
int EdbViewRec::MergeSegments2()
{
  int nseg = eView->Nsegments();
  if(nseg<2) return nseg;

  int nmerge=0;
  TObject *c;
  EdbSegment *s;
  EdbSegment *s1,*s2;
  for(int i=0; i<nseg-1; i++) {
    s1 = eView->GetSegment(i);
    for(int j=i+1; j<nseg; j++) {
      s2 = eView->GetSegment(j);
      if( s1->GetPuls() < 1 )                     continue;
      if( s2->GetPuls() < 1 )                     continue;
      if( Abs(s1->GetX0() - s2->GetX0()) > 40.)   continue;
      if( Abs(s1->GetY0() - s2->GetY0()) > 40.)   continue;
      if( Abs(s1->GetTx() - s2->GetTx()) > 0.12)  continue;
      if( Abs(s1->GetTy() - s2->GetTy()) > 0.12)  continue;
      if( Chi2Seg( *s1,*s2 ) > 3. )               continue;

      s = new EdbSegment();
      s->Copy(*s1);
      for(int ii=0; ii<s2->GetNelements(); ii++) {
	c = s2->GetElements()->UncheckedAt(ii);
	if( !(s->GetElements()->FindObject(c)) )  s->AddElement( c );
      }
      if( GoodSegment(*s) ) {
	s->SetID(s1->GetID());
	s1->Copy(*s);
	s2->UnSetIDE();
	s2->SetPuls(0);
	nmerge++;
      }
      delete s;
    }
  }

  int good=0;
  nseg = eView->Nsegments();
  for(int i=0; i<nseg; i++) {
    s = eView->GetSegment(i);
    if(s->GetPuls()<1)  continue;
    s->SetIDE(i);
    good++;
  }

  printf("%d segments after %d merging\n", good, nmerge);
  return good;
}

//____________________________________________________________________________________
float EdbViewRec::Chi2Seg(EdbSegment &s1, EdbSegment &s2)
{
  return 1.;
}

//____________________________________________________________________________________
int EdbViewRec::MergeSegments()
{
  int nseg = eISC.GetEntries();
  if(nseg<1) return 0;
  
  eISC.Sort();
  TIndexCell *is1=0, *is2=0;
  for(int i1 = nseg-1; i1>=0; i1--) {
    is1 = eISC.At(i1);     //segment 1
    for(int i2 = 0; i2<i1; i2++) {
      is2 = eISC.At(i2);     //segment 2

      for(int j2=0; j2<is2->N(); j2++) 
	if( is1->Find( is2->At(j2)->Value() ) )   { //has common clusters
	  for(int j1=0; j1<is1->N(); j1++) 
	    is2->FindAdd( is1->At(j1)->Value() );
	  eISC.Drop(i1);
	  goto NEXT_S1;
	}
    }
  NEXT_S1:
    continue;
  }
  eISC.Purge(2);
  eISC.Sort();
//   isc.SetName("iseg:icl");
//   isc.Print("");
  nseg =  eISC.GetEntries();
  printf("%d segments after merging\n", nseg);
  return nseg;
}

//____________________________________________________________________________________
int EdbViewRec::AddSegmentsToView()
{
  int good=0;
  int nseg = eISC.GetEntries();
  if(nseg<1) return 0;

  float tlim2 = eThetaLim*eThetaLim;
  for(int i = 0; i<nseg; i++)  eISC.At(i)->SetValue(eISC.At(i)->N());
  eISC.Sort();

  EdbCluster *c=0;
  EdbSegment *s=0;
  int ncl;
  for(int i = 0; i<nseg; i++) {
    ncl = eISC.At(i)->N();
    if(ncl>1) {
      s = new EdbSegment();
      for(int j=0; j<ncl; j++ ) {
	//printf("icl = %d \n", isc.At(i)->At(j)->Value());
	c = (EdbCluster*)eCL->UncheckedAt( eISC.At(i)->At(j)->Value() );
	s->AddElement(c);
      }
      s->SetPuls(s->GetNelements());
      FitSegment(*s);
      if( (s->GetTx()*s->GetTx()+s->GetTy()*s->GetTy()) > tlim2)   continue;
      s->SetSigma( Sqrt(s->GetSigmaX()*s->GetSigmaX() + s->GetSigmaY()*s->GetSigmaY()),
		   CalculateSegmentChi2(*s, eGrainSX, eGrainSY, eGrainSZ) );
      if( s->GetSigmaY()>10 )    continue;
      s->SetX0( s->GetX0()+ (eZxy-s->GetZ0())*s->GetTx());
      if(s->GetX0()<eXmin) continue;
      if(s->GetX0()>eXmax) continue;
      s->SetY0( s->GetY0()+ (eZxy-s->GetZ0())*s->GetTy());
      if(s->GetY0()<eYmin) continue;
      if(s->GetY0()>eYmax) continue;
      s->SetZ0( eZxy );
      s->SetIDE(good++);
      eView->AddSegment(s);
    }
  }
  
  return good;
}

//____________________________________________________________________________________
int EdbViewRec::CheckSeedThres()
{
  Short_t *p0,*pnext, *plast = ehX+enXtot;
  Int_t nfill;
  Int_t ntot  = 0;
  
  for(int i=0; i<enT; i++) {
    eSeedThres[i] = eSeedThres0;
  }
  
  for(int it=0; it<enT; it++) {
    TArrayL spt(256);   //phase occupancy histogram
    p0    = epT[it][0][0];
    if(it==enT-1) pnext = plast;
    else          pnext = epT[it+1][0][0];
    ntot = (int)((pnext-p0)/eFact);
    nfill=0;
    while(p0<pnext) {
      spt[*p0]++;
      if(*p0>0)     nfill++;
      p0++;
    }
    printf("\nit = %3d   occup: %d / %d = %f %% \n",it, nfill, ntot, 100.*nfill/ntot );

	int thres=0, sum=0, sumthres=0;
	for (int i=255; i>0; i--)   {
		if(spt[i]<1) continue;
		printf("%3d %ld\n",i,spt[i]);
	    if(!thres) sumthres=sum;
		sum += spt[i];
		if(!thres&&sum>eNseedMax[it]) thres = i+1;
	}
	if(thres>eSeedThres[it]) eSeedThres[it]=thres;
	printf("sumthres(%d) = %d   eSeedThres[%d]= %d\n\n",thres,sumthres, it, eSeedThres[it]);
  }


  //TArrayL sp(256);   //phase occupancy histogram
  //for (int i=0; i<enXtot; i++) 
    //if(ehX[i]>0)      sp[ehX[i]]++;
  //printf("\n total occup: %d / %d = %f %% \n", (int)(sp.GetSum()), enXtot, 100.*eFact*sp.GetSum()/enXtot );

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
  //return thres;
  return 0;
}

//____________________________________________________________________________________
int EdbViewRec::FindSeeds()
{
  
  if(eVC.eNcl<1) return 0;
  if(eVC.eNfr<2) return 0;
  printf("\nFindSeed with %d clusters, %d frames \t eZcenter[%6.1f:%6.1f] = %6.1f   eZxy = %6.1f\n",
	 eVC.eNcl,eVC.eNfr,eZmin,eZmax,eZcenter,eZxy);

  bzero(ehX,enXtot*sizeof(Short_t));
  //memset(ehX,'\0',enXtot*sizeof(Short_t));

  TObjArray  *arr0=0, *arr1=0;
  EdbCluster *cl0=0,  *cl1=0;
  int   iz1=0, jn=0;
  int   n0=0,n1=0;
  long ncycle=0, ict=0, nseeds=0;
  float r2max=eVC.eSx*eVC.eSx;
  float r2d3max = eRmax*eRmax;
  float dx,dy,dz,r2, r2d3;
  float r,phi,t;
  float zratio,xZxy,yZxy;
  int   it,ip,ix,iy;
  float x0,x1,y0,y1, phirot;
  EdbCluster **pps;

  for(int istep=eStepFrom; istep<eStepTo; istep+=eStep) {
    for(int iz=0;     iz<eVC.eNfr-istep; iz++) {
      iz1=iz+istep;
      
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
	      ncycle++;
	      
	      x0 =  cl0->eX;
	      x1 =  cl1->eX;
	      y0 =  cl0->eY;
	      y1 =  cl1->eY;
	      dx = x1 - x0;
	      dy = y1 - y0;
	      dz = cl1->eZ - cl0->eZ;
	      r2 = dx*dx+dy*dy;
	      r2d3 = r2+dz*dz;
	      if(r2>r2max)                 continue;
	      if(r2d3>r2d3max)             continue;
	      r = Sqrt(r2);
	      t   = ATan2(r,Abs(dz));
	      if(t>eTheta[enT-1])          continue;
	      phi = ATan2(dy,dx);      // defined in [ -pi : pi ]
	      
	      for(it=0; it<enT-1; it++) if(t<eTheta[it]) break;
	      ip = (int)((Pi()+phi-0.00001)/esP[it]+0.00001);

	      if(it>0)   {         //rotation:
		phirot = -((ip+0.5)*esP[it] - Pi());               // rotation angle is the same for all entries of the same phi-bin
		x0 = cl0->eX*Cos(phirot) - cl0->eY*Sin(phirot);
		y0 = cl0->eX*Sin(phirot) + cl0->eY*Cos(phirot);
		x1 = cl1->eX*Cos(phirot) - cl1->eY*Sin(phirot);
		y1 = cl1->eX*Sin(phirot) + cl1->eY*Cos(phirot);
		dx = x1-x0;
		dy = y1-y0;
	      }

	      //printf("dx,dy,dz: %f %f %f\n",dx,dy,dz);

	      zratio = (eZcenter  - cl0->eZ)/dz;
	      xZxy = eDmax/2. + x0 + dx*zratio;
	      if(xZxy<0)        continue;
	      if(xZxy>eDmax)    continue;
	      
	      yZxy = eDmax/2. + y0 + dy*zratio;
	      if(yZxy<0)        continue;
	      if(yZxy>eDmax)    continue;

	      iy = (int)(yZxy/esY[it]);
	      ix = (int)(xZxy/esX[it]);
	      
	      //printf("ict[ %d  %d  %d %d ]\n",it,ip,iy,ix);
	      ict = epT[it][ip][iy] + ix -ehX;
	      //printf("%d\n",ict);
	      
	      if(ict<0||ict>=enXtot) {
		printf("ict[ %d  %d  %d ] = %ld\n",it,ip,iy,ict);
		printf("esX,     esY: %f %f\n",esX[it], esY[it]);
		printf("eYmin, eYmax: %f %f\n",eYmin,eYmax );
		printf("eXmin, eXmax: %f %f\n",eXmin,eXmax );
		printf("yZxy,   xZxy: %f %f\n",yZxy,xZxy);
		return 0;
	      }
	      
	      if(ehX[ict]<1) {
		epS[ict] = epC + nseeds*eSeedLim;
		nseeds++;
		if( nseeds >= enSeedsLim)  {
		  printf("ERROR: too many seeds: %ld, abandon tracking! \n",nseeds);
		  return 0;
		}
	      }
	      pps = epS[ict] + ehX[ict];

	      if(ehX[ict]<eSeedLim) *pps = cl0;
	      pps++;
	      //printf("ict = %d  bin = %d\n",ict, ehX[ict]);
	      ehX[ict]++;
	      if(ehX[ict]<eSeedLim) *pps=cl1;
	      ehX[ict]++;

	    }
	  }
	}
      }
    }
  }
  printf("occupancy: %ld / %d * %4.3f = %5.2f %% \n\n", nseeds,enXtot, eFact, 100.*eFact*nseeds/enXtot);
  return nseeds;
}
