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

  int nentr=clust->GetEntries();
  TClonesArray *clarr = new TClonesArray("EdbCluster");

  int ncl =0;
  for(int i=0; i<nentr; i++) {
    clust->GetEntry(i);
    if( clp->GetZ()>-50.) continue;
    //if( clp->Xp()<500.) continue;
    //if( clp->Xp()>600.) continue;
    //if( clp->Yp()<500.) continue;
    //if( clp->Yp()>600.) continue;

    new((*clarr)[ncl])  
      EdbCluster( clp->Xcg(),clp->Ycg(),clp->GetFrame(),
		  clp->GetArea(), clp->Peak(), clp->GetFrame(), 0);
    ncl++;
  }
  fcl->Close();
  delete fcl;
  printf("%d clusters are readed from %s\n",ncl,file);


  SetBin(4,4,1);
  TIndexCell chains;
  VerticalChains(clarr,chains);
  InitTree("grains_tree.root");
  MakeGrainsTree(clarr,chains);
  eGrainsFile->Close();  
}

///-----------------------------------------------------------------------------
void EdbGA::CheckViewGrains()
{
  int nview = eRun->GetEntries();
  for(int i=0; i<nview; i++)  CheckViewGrains(i);
}

///-----------------------------------------------------------------------------
void EdbGA::CheckViewGrains(int vid)
{
  eVid = vid;
  TClonesArray *clusters=0;

  clusters = eRun->GetEntryClusters(vid);

  printf("%d clusters are read for view %d \n", clusters->GetEntries(), vid );

  TIndexCell chains;
  VerticalChains(clusters,chains);
  MakeGrainsTree(clusters,chains);
}

///-----------------------------------------------------------------------------
void EdbGA::VerticalChains( TClonesArray *clusters, TIndexCell &chains )
{
  //chains: "x:y:z:entry"

  Long_t v[5];
  float xb=eBinX, yb=eBinY, zb=eBinZ;    // bins

  EdbCluster *cl;

  int ncl = clusters->GetEntries();
  for( int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)(clusters->UncheckedAt(i));
    v[0] = (Long_t)(cl->X()/xb);
    v[1] = (Long_t)(cl->Y()/yb);
    v[2] = (Long_t)(cl->Z()/zb);
    v[3] = (Long_t)i;
    chains.Add(4,v);
  }

  chains.Sort();
  chains.PrintStat();
  chains.PrintPopulation(3);
}

///------------------------------------------------------------------
void EdbGA::InitTree(const char *file)
{
  eGrainsFile= new TFile(file,"RECREATE");
  eGrains= new TTree("grains","grains");
 
  int ncl=0;
  eClusters=new TClonesArray("EdbCluster");
  float x0,y0,z0;

  eGrains->Branch("vid",&eVid,"eVid/I");
  eGrains->Branch("ncl",&ncl,"ncl/I");
  eGrains->Branch("clusters",&eClusters);
  eGrains->Branch("x0",&x0,"x0/F");
  eGrains->Branch("y0",&y0,"y0/F");
  eGrains->Branch("z0",&z0,"z0/F");
}

///------------------------------------------------------------------
int EdbGA::MakeGrainsTree( TClonesArray *clust, TIndexCell &chains)
{
  int ncl=0;
  float x0,y0,z0;

  eGrains->SetBranchAddress("vid",&eVid);
  eGrains->SetBranchAddress("ncl",&ncl);
  eGrains->SetBranchAddress("clusters",&eClusters);
  eGrains->SetBranchAddress("x0",&x0);
  eGrains->SetBranchAddress("y0",&y0);
  eGrains->SetBranchAddress("z0",&z0);

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
	      GrainStat(eClusters,x0,y0,z0);
	      eGrains->Fill();
	      eClusters->Clear();
	      ngr++;
	      ncl=0;
	    }
	  }
	  new((*eClusters)[ncl])  EdbCluster( *cl );
	  ncl++;
	  zlvl=cl->Z();
	}
      }

      if( ncl>0 ) {
	GrainStat(eClusters,x0,y0,z0);
	eGrains->Fill();
	eClusters->Clear();
	ngr++;
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
  int ncl = clusters->GetEntries();
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
