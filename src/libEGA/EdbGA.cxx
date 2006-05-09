//-- Author :  Valeri Tioukov   31.07.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbGA                                                                //
//                                                                      //
// Grains Analysis  module                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//
// Usage:
// 
// root [0] EdbGA ga("/mnt/nusrv4_2/ftpuser/testFV1.grs.root",6,6,1)
// root [1] ga.CheckViewGrains()
// ...
// create grains_chain.root with grains tree
//

#include "TIndexCell.h"
#include "TEventList.h"

#include "EdbCluster.h"
#include "EdbView.h"
#include "EdbGA.h"
#include "EdbIP.h"

ClassImp(EdbGA)

///-----------------------------------------------------------------------------
EdbGA::EdbGA(char *fname, float bx, float by, float bz) 
{
  SetRun(fname);  
  SetBin(bx,by,bz);
  InitTree();
}

///-----------------------------------------------------------------------------
EdbGA::~EdbGA()
{
  if(eGrainsFile) { 
    eGrainsFile->Write(); 
    eGrainsFile->Purge(); 
    eGrainsFile->Close(); 
  }
  if(eRun)   delete eRun;  
}

///-----------------------------------------------------------------------------
void EdbGA::GetClustPFile(const char *file)
{
  TFile *fcl = new TFile(file);
  TTree *clust = (TTree *)fcl->Get("clust");
  EdbClustP *clp = 0;
  clust->SetBranchAddress("cl",&clp);

  int nentr = (int)(clust->GetEntries());
  TClonesArray *clarr = new TClonesArray("EdbCluster");

  int ncl =0;
  for(int i=0; i<nentr; i++) {
    clust->GetEntry(i);
    //if( clp->GetZ()>-50.) continue;
    //if( clp->Xp()<500.) continue;
    //if( clp->Xp()>600.) continue;
    //if( clp->Yp()<500.) continue;
    //if( clp->Yp()>600.) continue;

    new((*clarr)[ncl])  
      EdbCluster( clp->Xcg(),clp->Ycg(),clp->Z(),
		  clp->GetArea(), clp->Peak(), clp->GetFrame(), 0);
    ncl++;
  }
  fcl->Close();
  delete fcl;
  printf("%d clusters are readed from %s\n",ncl,file);


  SetBin(4,4,1);
  TIndexCell chains;
  printf("chains\n");
  VerticalChains(clarr,chains);
//    for(int i=0; i<10; i++) {
//      printf("chainsA\n");
//      VerticalChainsA(clarr);
//    }
  printf("\n tree: \n\n");
  InitTree("grains_tree.root");
  MakeGrainsTree(clarr,chains);
  eGrainsFile->Close();  
}

///-----------------------------------------------------------------------------
void EdbGA::CheckViewGrains(const char* options)
{
  int nview = eRun->GetEntries();
  for(int i=0; i< nview ; i++)  CheckViewGrains(i,options);
}

///-----------------------------------------------------------------------------
void EdbGA::CheckViewGrains(int vid, const char* options)
{
  eVid = vid;
  TClonesArray *clusters=0;

  clusters = eRun->GetEntryClusters(vid);

  printf("%d clusters are read for view %d \n", clusters->GetEntriesFast(), vid );

  TIndexCell chains;
  VerticalChains(clusters,chains);
  MakeGrainsTree(clusters,chains,options);
}

///-----------------------------------------------------------------------------
void EdbGA::VerticalChainsA( TClonesArray *clusters )
{
  float eDX = 1301, eDY=1025;
  eBinX=4; eBinY=4;
  const int nx = (int)(eDX/eBinX);
  const int ny = (int)(eDY/eBinY);
  const int nz = 50; 

  int *v = new int[nx*nx*ny];

  int ix,iy,iz,iv;

  EdbCluster *cl;

  for(iz=0; iz<nz; iz++)
    for(iy=0; iy<ny; iy++)
      for(ix=0; ix<nx; ix++) {
	iv = ix + nx*iy + nx*ny*iz;
	v[iv] = -1;
      }
  
  int ncl = clusters->GetEntriesFast();
  for( int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)(clusters->UncheckedAt(i));
    ix = (int)(cl->X()/eBinX);
    iy = (int)(cl->Y()/eBinY);
    iz = (int)(cl->GetFrame());

    iv = ix + nx*iy + nx*ny*iz;
    v[iv] = i;
  }

  delete[] v;
}

///-----------------------------------------------------------------------------
void EdbGA::VerticalChains( TClonesArray *clusters, TIndexCell &chains )
{
  //chains: "x:y:z:entry"

  Long_t v[5];
  float xb=eBinX, yb=eBinY, zb=eBinZ;    // bins

  EdbCluster *cl;

  int ncl = clusters->GetEntriesFast();
  for( int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)(clusters->UncheckedAt(i));
    v[0] = (Long_t)(cl->X()/xb);
    v[1] = (Long_t)(cl->Y()/yb);
    v[2] = (Long_t)(cl->Z()/zb);
    v[3] = (Long_t)i;
    chains.Add(4,v);
  }

  chains.Sort();
  //chains.PrintStat();
  //chains.PrintPopulation(3);
}

///------------------------------------------------------------------
void EdbGA::InitTree(const char *file)
{
  eGrainsFile= new TFile(file,"RECREATE");
  eGrains= new TTree("grains","grains");
 
  Int_t ncl=0,fmin,fmax;
  eClusters=new TClonesArray("EdbCluster");
  Float_t x0,y0,z0,vol,amin,amax, zmin, zmax; 

  eGrains->Branch("vid",&eVid,"eVid/I");
  eGrains->Branch("ncl",&ncl,"ncl/I");
  eGrains->Branch("clusters",&eClusters);
  eGrains->Branch("x0",&x0,"x0/F");
  eGrains->Branch("y0",&y0,"y0/F");
  eGrains->Branch("z0",&z0,"z0/F");
  eGrains->Branch("vol",&vol,"vol/F");
  eGrains->Branch("amin",&amin,"amin/F");
  eGrains->Branch("amax",&amax,"amax/F");
  eGrains->Branch("zmin",&zmin,"zmin/F");
  eGrains->Branch("zmax",&zmax,"zmax/F");
  eGrains->Branch("fmin",&fmin,"fmin/I");
  eGrains->Branch("fmax",&fmax,"fmax/I");
}

///------------------------------------------------------------------
int EdbGA::MakeGrainsTree(TClonesArray *clust, TIndexCell &chains, const char* option)
{

  // process the options
  int min_ncl=1;  // minimum number of clusters of each grains 
  if (! sscanf(option,"MIN_NCL") ) sscanf(option,"MIN_NCL=%d",&min_ncl);

  // set branch addresses
  Int_t ncl=0,fmin, fmax;
  Float_t x0,y0,z0,vol,amin,amax, zmin, zmax;
  eGrains->SetBranchAddress("vid",&eVid);
  eGrains->SetBranchAddress("ncl",&ncl);
  eGrains->SetBranchAddress("clusters",&eClusters);
  eGrains->SetBranchAddress("x0",&x0);
  eGrains->SetBranchAddress("y0",&y0);
  eGrains->SetBranchAddress("z0",&z0);
  eGrains->SetBranchAddress("vol",&vol);
  eGrains->SetBranchAddress("amin",&amin);
  eGrains->SetBranchAddress("amax",&amax);
  eGrains->SetBranchAddress("zmin",&zmin);
  eGrains->SetBranchAddress("zmax",&zmax);
  eGrains->SetBranchAddress("fmin",&fmin);
  eGrains->SetBranchAddress("fmax",&fmax);

  int ngr=0;
  int entry=0;

  TIndexCell *cx;
  TIndexCell *cy;
  TIndexCell *cz;

  float zlvl;
  EdbCluster *cl;

  int nix,niy,niz,nie;

  nix = chains.GetEntries();
  for(int ix=0; ix<nix; ix++)  {
    cx = chains.At(ix);
    niy = cx->GetEntries();
    for(int iy=0; iy<niy; iy++)   {
      cy = cx->At(iy);
      zlvl = -9999999.;
      niz = cy->GetEntries();
      for(int iz=0; iz<niz; iz++) {
        cz = cy->At(iz);
        nie = cz->GetEntries();
        for(int ie=0; ie<nie; ie++) {
          entry = cz->At(ie)->Value();
          cl = (EdbCluster *)clust->UncheckedAt(entry);

          if( cl->Z()-zlvl > 2.*eBinZ ) {
            if( ncl>0 ) {
              if(ncl>=min_ncl) {
	              //GrainStat(eClusters,x0,y0,z0);
                GrainStat2(eClusters,x0,y0,z0,vol,amin,amax, zmin, zmax, fmin, fmax);
	              eGrains->Fill();
	              ngr++;
              }
	            eClusters->Clear();
	            ncl=0;
	            }
          }
		      new((*eClusters)[ncl])  EdbCluster( *cl );
		      ncl++;
		      zlvl=cl->Z();
		    }
      }

      if( ncl>0 ) {
          if(ncl>=min_ncl) {
	          //GrainStat(eClusters,x0,y0,z0);
            GrainStat2(eClusters,x0,y0,z0,vol,amin,amax, zmin, zmax, fmin, fmax);
	          eGrains->Fill();
	          ngr++;
        }
        eClusters->Clear();
        ncl=0;
      }
    }
  }

  eGrains->Write();
  printf("%d grains are selected\n",ngr);

  return ngr;
  }

  ///------------------------------------------------------------------------
  void EdbGA::GrainStat(TClonesArray *clusters, float &x0, float &y0, float &z0)
  {
  double xs=0,ys=0,zs=0; 
  EdbCluster *cl=0;
  int ncl = clusters->GetEntriesFast();
  for( int i=0; i<ncl; i++ ) {
    cl = (EdbCluster *)clusters->At(i);
    xs += cl->X();
    ys += cl->Y();
    zs += cl->Z();
  }
  x0 = xs/ncl;
  y0 = ys/ncl;
  z0 = zs/ncl;
  }
  ///------------------------------------------------------------------------
  int EdbGA::GrainStat2(TClonesArray *clusters, float &x0, float &y0, float &z0,
                        float &vol, float &amin, float &amax, float &zmin,
                        float &zmax, int &fmin, int &fmax)
  {
  float x,y,z,area,xs=0,ys=0,zs=0,areasum=0; 
  int frame; 
  EdbCluster *cl=0;
  amin  = 10000000;
  amax  =  0;
  zmin  =  1.e30; 
  zmax  = -1.e30;
  fmin  = 10000000; 
  fmax  =  0;

  int ncl = clusters->GetEntriesFast();
  for( int i=0; i<ncl; i++ ) 
  {
    cl = (EdbCluster *) clusters->At(i);
    x = cl->X();  
    y = cl->Y(); 
	  z = cl->Z(); 
	  frame = cl->GetFrame() ; 
	  area  = cl->GetArea()  ;
    if(area < amin) amin = area ;
    if(area > amax) amax = area ;
    if(frame   < fmin   ) { fmin = frame ; zmax = z ;}
    if(frame   > fmax   ) { fmax = frame ; zmin = z ;}
  /*    xs += x*area;
    ys += y*area;
    zs += z*area;
  */    xs += x;
      ys += y;
      zs += z;
    areasum += area;
  }
  /*  x0 = xs/areasum;
  y0 = ys/areasum;
  z0 = zs/areasum;
  */  x0 = xs/ncl;
  y0 = ys/ncl;
  z0 = zs/ncl;

  float zlen = zmax - zmin;
  if(zlen && ncl>1) vol = areasum*zlen/(ncl-1) ;    // should be (areasum/ncl) * (zlen*ncl/(ncl-1))
  else vol = -999;

  return 0;
}
///------------------------------------------------------------------------
void EdbGA::SelectGrains(TCut c1, const char* outfile)
{
  SelectGrains(c1.GetTitle(), outfile);
}
///------------------------------------------------------------------------
void EdbGA::SelectGrains(const char* selection, const char* outfile)
{
// save only the selected grains

   // set branch addresses
   Int_t ncl,fmin, fmax;
   Float_t x0,y0,z0,vol,amin,amax, zmin, zmax;
   eGrains->SetBranchAddress("vid",&eVid);
   eGrains->SetBranchAddress("ncl",&ncl);
   eGrains->SetBranchAddress("clusters",&eClusters);
   eGrains->SetBranchAddress("x0",&x0);
   eGrains->SetBranchAddress("y0",&y0);
   eGrains->SetBranchAddress("z0",&z0);
   eGrains->SetBranchAddress("vol",&vol);
   eGrains->SetBranchAddress("amin",&amin);
   eGrains->SetBranchAddress("amax",&amax);
   eGrains->SetBranchAddress("zmin",&zmin);
   eGrains->SetBranchAddress("zmax",&zmax);
   eGrains->SetBranchAddress("fmin",&fmin);
   eGrains->SetBranchAddress("fmax",&fmax);

   //select the event list
   printf("Total: %d grains\n", (int)eGrains->GetEntries());
   eGrains->Draw(">>lst",selection);
   TEventList* lst = (TEventList*) gDirectory->Get("lst");

   // create a new tree
   TFile* nfile = new TFile(outfile,"recreate");
   TTree* tree2 = (TTree*) eGrains->CloneTree(0);
   tree2->CopyAddresses(eGrains);

   int nentries = lst->GetN(); printf("Select %d grains",nentries);  printf(" with \"%s\"\n", selection);
   printf("Create the new tree...     ");
   for(int i=0;i<nentries;i++)
   {
      eGrains->GetEntry(lst->GetEntry(i));
      tree2->Fill();
      if((i%5000)==0) printf("\b\b\b\b%3d%%",(int)((double)i/double(nentries)*100.)) ;
   }
   printf("\b\b\b\b100%%\n");
   tree2->Write("grains",TObject::kOverwrite);
   nfile->Close();
}
