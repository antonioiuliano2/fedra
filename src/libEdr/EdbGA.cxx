//-- Author :  Valeri Tioukov   31.07.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbGA                                                                //
//                                                                      //
// Grains Analysis  stuff                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//
// Usage:
// 
// root [0] EdbGA ga("/mnt/nusrv4_2/ftpuser/testFV1.grs.root",6,6,1)
// root [1] ga.CheckViewGrains(0)
// ...
// create grains_chain.root with grains tree
//

#include "TIndexCell.h"
#include "EdbCluster.h"
#include "EdbView.h"
#include "EdbGA.h"

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

  printf("1 \n");

  int ncl = clusters->GetEntries();
  for( int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)(clusters->UncheckedAt(i));
    v[0] = (Long_t)(cl->X()/xb);
    v[1] = (Long_t)(cl->Y()/yb);
    v[2] = (Long_t)(cl->Z()/zb);
    v[3] = (Long_t)i;
    chains.Add(4,v);
  }

  printf("2 \n");
  chains.Sort();
  printf("3 \n");
  chains.SetName("x:y:z:entry");
  printf("4 \n");
  chains.PrintStat();
  printf("5 \n");
  chains.PrintPopulation(3);
  printf("6 \n");
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

  for(int ix=0; ix<chains.GetEntries(); ix++)  {
    cx = chains.At(ix);
    for(int iy=0; iy<cx->GetEntries(); iy++)   {
      cy = cx->At(iy);
      zlvl = -9999999.;
      for(int iz=0; iz<cy->GetEntries(); iz++) {
	cz = cy->At(iz);
	for(int ie=0; ie<cz->GetEntries(); ie++) {
	  entry = cz->At(ie)->Value();
	  cl = (EdbCluster *)clust->UncheckedAt(entry);

	  if( cl->Z()-zlvl > 2.*eBinZ ) {
	    if( eClusters->GetEntries()>0 ) {
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

      if( eClusters->GetEntries()>0 ) {
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
