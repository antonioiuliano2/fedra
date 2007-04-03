#include "TCanvas.h"
#include "TCut.h"
#include "TEventList.h"
#include "EdbTestAl.h"

ClassImp(EdbTestAl)

  using namespace TMath;

typedef struct {
  Int_t it1,it2;
  Float_t x1,y1,z1,x2,y2,z2;
  Float_t tx1,ty1,tx2,ty2;
  Float_t chi1,chi2;
} DTR;

//---------------------------------------------------------------------
EdbTestAl::EdbTestAl()
{
  eITMAX=50;  // angular step (def=50)
  eOCMAX=100; // occupancy (def=100)

  eOffset=5000;
  eBinSize=50.;//microns

  eS1=0;
  eS2=0;

  eT   = 0;
  eFile= 0;
  HD=0;
  HDF=0;
  HDF2=0;

  eMaxBin=0;
  for(int i=0; i<4; i++) {eN[i]=0; eDmin[i]=0; eDmax[i]=0; eD0[i]=0;}

}

//---------------------------------------------------------------------
EdbTestAl::~EdbTestAl()
{
  if(eT)    eT->AutoSave("SaveSelf");
  if(eFile) eFile->Close();
}

//---------------------------------------------------------------------
void EdbTestAl::HDistance(EdbPattern &p1, EdbPattern &p2)
{
  int nx=200, ny=200, no=100;
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
  
  if(!eS1) eS1=new TObjArray(1000);  else eS1->Clear();
  if(!eS2) eS2=new TObjArray(1000);  else eS2->Clear();

  for(int itx=0;itx<eITMAX;itx++) for(int ity=0;ity<eITMAX;ity++) 
    for(int i=0;i<oc1[itx*nx+ity];i++)
      for(int j=0;j<oc2[itx*nx+ity];j++)
	{
	  eS1->Add( p1.GetSegment(ind1[itx*nx*ny+ity*ny+i]) );
	  eS2->Add( p2.GetSegment(ind2[itx*nx*ny+ity*ny+j]) );
	}

  delete [] ind1;
  delete [] ind2;
  delete [] oc1;
  delete [] oc2;
}

//---------------------------------------------------------------------
int EdbTestAl::CheckMaxBin()
{
  // Output: vmax - x,y,z,phi of the highest bin

  printf("CheckMaxBin: nz = %d nphi = %d\n",eN[2],eN[3]);

  TH2F  *h2 = new TH2F("z_phi","z_phi",eN[3],eDmin[3],eDmax[3],eN[2],eDmin[2],eDmax[2]);

  float binz = (eDmax[2]-eDmin[2])/eN[2];
  float binp = (eDmax[3]-eDmin[3])/eN[3];
  float dz=0,  dphi=0, mean=0;
  float xmax=0,ymax=0;
  int   mbin=0;
  for(int iz=0; iz<eN[2]; iz++) {
    dz = eDmin[2] + iz*binz +binz/2;
    for(int ip=0; ip<eN[3]; ip++) {
      dphi = eDmin[3] + ip*binp + binp/2;
      mbin = CheckMaxBin(dz,dphi,mean,xmax,ymax);
      h2->Fill(dphi,dz,mbin);
      if(mbin>=eMaxBin) {
	eD0[0] = xmax; eD0[1]=ymax; eD0[2]=dz; eD0[3]=dphi;
	eMaxBin=mbin;
	printf("dx,dy,dz,dphi = %f %f %f %f;  maxbin/mean= %d/%f = %f\n",xmax,ymax,dz,dphi, mbin,mean,  mbin/mean);
      }
    }
  }
  h2->Draw("box");

  int mx,my,mz;
  h2->GetMaximumBin(mx,my,mz);
  float phi = h2->GetXaxis()->GetBinCenter(mx);
  float z   = h2->GetYaxis()->GetBinCenter(my);
  mbin = (int)(h2->GetMaximum());
  mean = h2->GetSum()/h2->GetNbinsX()/h2->GetNbinsY();
  printf("max(%f,%f) = %d ; mean = %f\n",phi,z,mbin,mean);

  return mbin;
}

//---------------------------------------------------------------------
int EdbTestAl::CheckMaxBin(float gdz, float phi, float &meanbin, float &xmax, float &ymax)
{
  // Output: vmax - x,y,z,phi of the highest bin

  if(!eS1) return 0;
  EdbSegP *s1=0, *s2=0;
  float x2p,y2p,dx,dy;
  TH2F  h2("H2","H2",eN[0],eDmin[0],eDmax[0],eN[1],eDmin[1],eDmax[1]);
  int n = eS1->GetEntries();
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP *)(eS1->UncheckedAt(i));
    s2 = (EdbSegP *)(eS2->UncheckedAt(i));

    x2p = s2->X()*Cos(phi)-s2->Y()*Sin(phi);
    y2p = s2->X()*Sin(phi)+s2->Y()*Cos(phi);
    // todo - transform angles?

    dx= (x2p+s2->TX()*gdz) - s1->X();
    dy= (y2p+s2->TY()*gdz) - s1->Y();
    h2.Fill(dx,dy);    
  }

  meanbin = h2.GetSum()/h2.GetNbinsX()/h2.GetNbinsY();
  int mx,my,mz;
  h2.GetMaximumBin(mx,my,mz);
  xmax = h2.GetXaxis()->GetBinCenter(mx);
  ymax = h2.GetYaxis()->GetBinCenter(my);
  return (int)(h2.GetMaximum());
}

//---------------------------------------------------------------------
int EdbTestAl::FillTree( float gdz )
{
  if(!eS1) return 0;

  eFile = new TFile("alt.root","RECREATE");
  eT    = new TTree("T","Alignment tree");
  float dx,dy;
  static DTR dtr;
  eT->Branch("dtr",&dtr,"it1/I:it2/I:x1/F:y1/F:z1/F:x2/F:y2/F:z2/F:tx1/F:ty1/F:tx2/F:ty2/F:chi1/F:chi2/F");
  EdbSegP *s1,*s2;
  int steps=int(2.*eOffset/eBinSize);
  if(HD) HD->Reset(); 
  else   HD=new TH2F("HD","HD",steps,-eOffset,eOffset,steps,-eOffset,eOffset);
  if(HDF) HDF->Reset(); 
  else    HDF=new TH2F("HDF","HDF",100,-eBinSize,eBinSize,100,-eBinSize,eBinSize);
  HDF2=new TH2F("HDF2","HDF2",100,-3*eBinSize,3*eBinSize,100,-3*eBinSize,3*eBinSize);

  int n = eS1->GetEntries();
  for(int i=0; i<n; i++) {
    s1 = (EdbSegP *)(eS1->UncheckedAt(i));
    s2 = (EdbSegP *)(eS2->UncheckedAt(i));
    dtr.x1=s1->X();
    dtr.y1=s1->Y();
    dtr.z1=s1->Z();
    dtr.x2=s2->X();
    dtr.y2=s2->Y();
    dtr.z2=s2->Z();
    dtr.chi1=s1->Chi2();
    dtr.chi2=s2->Chi2();
    
    dtr.tx1=s1->TX();
    dtr.ty1=s1->TY();
    dtr.tx2=s2->TX();
    dtr.ty2=s2->TY();
    
    dx= (dtr.x2+s2->TX()*gdz) - dtr.x1;
    dy= (dtr.y2+s2->TY()*gdz) - dtr.y1;
    eT->Fill();
    HD->Fill(dx,dy);
    HDF->Fill(dx,dy);
    HDF2->Fill(dx,dy);
  }
  HD->Write();
  HDF->Write();
  HDF2->Write();
  eT->Write();
  return n;
}

// //---------------------------------------------------------------------
// int EdbTestAl::MakeTrans(EdbAffine2D &aff, float dz, float dx, float dy)
// {
//   if(!eS1) return 0;
//   int n = eS1->GetEntries();
//   printf("MakeTrans: n=%d\n",n);
//   TArrayF x1(n);
//   TArrayF x2(n);
//   TArrayF y1(n);
//   TArrayF y2(n);
//   int ngood=0;
//   for(int i=0; i<n; i++) {
//     s1 = (EdbSegP *)(eS1->UncheckedAt(i));
//     s2 = (EdbSegP *)(eS2->UncheckedAt(i));
//     if( Abs(s2->X()-s1->X()) > dx) continue;
//     if( Abs(s2->Y()-s1->Y()) > dy) continue;
//     x1[i] = s1->X();
    
//   }
// }

//---------------------------------------------------------------------
int EdbTestAl::MakeTrans(EdbAffine2D &aff, float dz, const char *ccut)
{
  static DTR dtr;
  eT->SetBranchAddress("dtr",&dtr);

  printf("MakeTrans: propagate the first pattern to %f before...\n",dz);
  char str[64];
  eT->SetAlias("dtx","tx2-tx1");
  eT->SetAlias("dty","ty2-ty1");
  sprintf(str,"x2-(%f*tx1+x1)",dz);  eT->SetAlias("dx",str);
  sprintf(str,"y2-(%f*ty1+y1)",dz);  eT->SetAlias("dy",str);

  TCanvas *c = new TCanvas("alcan","alignment check");
  c->Divide(2,2);
  
  TCut cut=ccut;

  c->cd(1); eT->Draw("dy:dx",cut);
  c->cd(2); eT->Draw("dy:x2",cut);
  c->cd(4); eT->Draw("dx:y2",cut);

  TEventList *lst =0;
  eT->Draw(">>lst", cut );
  lst = (TEventList*)gDirectory->GetList()->FindObject("lst");
  int nlst =lst->GetN();
  int entr=0;

  TArrayF x1(nlst);
  TArrayF x2(nlst);
  TArrayF y1(nlst);
  TArrayF y2(nlst);

  for(int i=0; i<nlst; i++ ) {
    entr = lst->GetEntry(i);
    eT->GetEntry(entr);
    x1[i]=dtr.x1;
    y1[i]=dtr.y1;
    x2[i]=dtr.x2;
    y2[i]=dtr.y2;
  }

  aff.Calculate(nlst,x1.GetArray(),y1.GetArray(),x2.GetArray(),y2.GetArray());
  return nlst;
}
