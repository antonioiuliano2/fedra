//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCluster                                                           //
//                                                                      //
// Implementation of cluster class                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TPolyMarker3D
#include "TPolyMarker3D.h"
#endif

#ifndef ROOT_EdbCluster
#include "EdbCluster.h"
#endif

ClassImp(EdbCluster)
ClassImp(EdbClustersBox)

//______________________________________________________________________________
void EdbCluster::Print( Option_t *opt ) const
{
  printf("Cluster: %f %f %f \t %f %f \t %d %d %d\n", 
	 eX, eY, eZ, eArea, eVolume, eFrame, eSide, eSegment);
}

//______________________________________________________________________________
void EdbCluster::Draw( Option_t *opt ) const
{
  TPolyMarker3D *m = new TPolyMarker3D(1,1);
  m->Draw();
}

//______________________________________________________________________________
EdbClustersBox::EdbClustersBox( )
{
  eClusters = new TClonesArray("EdbCluster");
}

//______________________________________________________________________________
EdbClustersBox::EdbClustersBox( int n )
{
  eClusters = new TClonesArray("EdbCluster",n);
}

//______________________________________________________________________________
EdbClustersBox::~EdbClustersBox( )
{
  if(eClusters)    delete eClusters;
}

//______________________________________________________________________________
void EdbClustersBox::AddCluster( EdbCluster *c )
{
  int i = eClusters->GetLast()+1;
  new((*eClusters)[i++])  EdbCluster( *c );
}
 
//______________________________________________________________________________
void EdbClustersBox::AddCluster( float x,  float y,  float z,
                          float a,  float v, int f, int s, int seg )
{
  int i = eClusters->GetLast()+1;
  new((*eClusters)[i++])  EdbCluster( x,y,z, a,v,f,s,seg );
}

//______________________________________________________________________________
Int_t    EdbClustersBox::GetN()    const
{
  if(eClusters) return eClusters->GetLast()+1; else return 0;
}
 
//______________________________________________________________________________
EdbCluster  *EdbClustersBox::GetCluster(int i) const
{
  if(eClusters) return (EdbCluster*) eClusters->At(i);
  else       return 0;
}
