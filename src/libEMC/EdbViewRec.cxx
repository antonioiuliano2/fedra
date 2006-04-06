//-- Author :  Valeri Tioukov   6.03.2006

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewRec - one microscope view reconstruction                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH2F.h"
#include "TNtuple.h"

#include "EdbViewRec.h"
#include "EdbCluster.h"
#include "EdbFrame.h"
#include "EdbSegment.h"
#include "EdbMath.h"

ClassImp(EdbViewRec)

//____________________________________________________________________________________
EdbViewRec::EdbViewRec()
{
  SetDef();
  SetDefRec();
}

//____________________________________________________________________________________
void EdbViewRec::SetDefRec()
{
  eSx = 10.;
  eSy = 10.;
  eNx = (int)((eXmax-eXmin)/eSx)+1;
  eNy = (int)((eYmax-eYmin)/eSy)+1;
  eNcell = eNx*eNy;
  
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
      eTmask[ii] = eR[i];
      ePmask[ii] = j*eSphi[i];
      ii++;
    }
  }

  eNtot = eNphiTot*eNx*eNy;
  printf("Ntot = %ld \n",eNtot);
  ePhist.Set(eNtot);
}

//____________________________________________________________________________________
int EdbViewRec::bm(EdbView &v)
{
  using namespace TMath;

  int nfr = v.GetNframes();
  TObjArray cmatr(nfr);         //z:x:y
  for(int i=0; i<nfr; i++) 
    cmatr.Add(new TObjArray(eNcell));

  int ncycle=0;

  int ix=0,iy=0, iz=0, j=0;
  EdbCluster *cl=0;
  int ncl=v.Nclusters();

  TObjArray *arz=0;
  TObjArray *arr=0;

  printf("ncl: %d\n",ncl);

  for(int i=0; i<ncl; i++) {
    cl = v.GetCluster(i);
    ix = (int)((cl->X()-eXmin)/eSx);
    iy = (int)((cl->Y()-eYmin)/eSy);
    iz = cl->GetFrame();
    j = iy*eNx+ix;
    arz = (TObjArray*)cmatr.UncheckedAt(iz);
    if(!(arr= (TObjArray*)arz->UncheckedAt(j))) {
      arr = new TObjArray(5);
      arz->AddAt( arr,j );
    }
    arr->Add(cl);
  }

  int iz2=0;
  int n0=0,n2=0;
  TObjArray  *arz0=0, *arz2=0, *arr0=0, *arr2=0;
  EdbCluster *cl0=0,  *cl2=0;

  int neib[9]={0, 1, -1, eNx, eNx+1, eNx-1, -eNx, -eNx+1, -eNx-1};
  int jn,ict;
  float dx,dy,dz,zratio,r2,r, r2max=100;
  float xZxy,yZxy,t,phi,x1,y1,z1,x2,y2,z2,fict;

  TNtuple *ntuple = new TNtuple("ntuple","scanback tracks from oracle","x:y:t:phi:x1:y1:z1:x2:y2:z2:ict");
  
  printf("nfr: %d\n",nfr);

  float phy;
  for(int iz=0;     iz<nfr-2; iz++) {
    iz2=iz+2;
    arz0 = (TObjArray*)cmatr.UncheckedAt(iz);
    arz2 = (TObjArray*)cmatr.UncheckedAt(iz2);

    if(!arz0)       printf("!arz0 !!! \n");
    if(!arz2)       printf("!arz2 !!! \n");

    dz = v.GetFrame(iz2)->GetZ() - v.GetFrame(iz)->GetZ();
    zratio = (eZxy  - v.GetFrame(iz)->GetZ()) / dz;
    printf("dz: %f\n",dz);

    for(int j=0; j<eNcell; j++) {
      arr0 = (TObjArray*)arz0->UncheckedAt(j);
      if(!arr0)    continue;
      n0 = arr0->GetEntriesFast();

      for(int i0=0; i0<n0; i0++) {
	cl0 = (EdbCluster*)arr0->UncheckedAt(i0);

	for(int in=0; in<9; in++) {                 //neighbours
	  jn = j + neib[in];
	  if(jn<0)                  continue;
	  if(jn>=eNcell)            continue;

	  arr2 = (TObjArray*)arz2->UncheckedAt(jn);
	  if(!arr2)    continue;
	  n2 = arr2->GetEntriesFast();

	  for(int i2=0; i2<n2; i2++) {
	    cl2 = (EdbCluster*)arr2->UncheckedAt(i2);

	    dx = cl2->eX - cl0->eX;
	    dy = cl2->eY - cl0->eY;


	    r2 = (dx*dx+dy*dy);
	    if(r2>r2max)        continue;

	    xZxy = cl0->eX + dx*zratio;
	    if(xZxy<eXmin)        continue;
	    if(xZxy>eXmax)        continue;

	    yZxy = cl0->eY + dy*zratio;
	    if(yZxy<eYmin)        continue;
	    if(yZxy>eYmax)        continue;
	    
	    ncycle++;

	    phi = Pi() + ATan2(dx,dy);
	    r   = Sqrt(r2);
	    t   = ATan2(r,dz);

	    ix  = (int)((xZxy-eXmin)/eSx);
	    iy  = (int)((yZxy-eYmin)/eSy);

	    int iphi=0;
	    int it=0;
	    for(it=0; it<eNtheta-1; it++) {
	      if(r>eR[it]) iphi+=eNphi[it];
	      else break;
	    }

	    iphi += (int)(phi/eSphi[it] + 0.00001);
	    
	    ict = (iy*eNx+ix)*eNphiTot + iphi;
	    fict=ict;

	    //printf("r,t %f %f\n",r,t);

	    ePhist[ict] = ePhist[ict]+1;
	    
	    ntuple->Fill( xZxy,yZxy,t,phi,
			  cl0->eX, cl0->eY, cl0->eZ,
			  cl2->eX, cl2->eY, cl2->eZ,
			  fict );

	  }
	}
      }
    }
  }

  printf("ncycle: %d\n",ncycle);

  TNtuple *ntpix = new TNtuple("ntpix","pixels","i:pix:ix:iy:t:p");
  float p;
  int iphi;
  printf("eNtot: %d\n",eNtot);
  for (int i=0; i<eNtot; i++) {
    if(ePhist[i]<1) continue;
    iphi = i%eNphiTot;
    t = eTmask[iphi];
    p = ePmask[iphi];
    ix = i/eNphiTot;
    iy = ix/eNx;
    ix = ix%eNx;
    ntpix->Fill(i,ePhist[i],ix,iy,t,p);
  }





  //hpix->Draw();
  
  //h2->Draw("box");

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

  return ncycle;
}
