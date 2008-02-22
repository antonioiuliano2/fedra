//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCluster                                                           //
//                                                                      //
// Implementation of cluster class                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TPolyMarker3D.h>
#include <TClass.h>
#include "EdbCluster.h"

ClassImp(EdbCluster)
ClassImp(EdbClustersBox)

//______________________________________________________________________________
void EdbCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbCluster.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbCluster::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      EdbPoint3D::Streamer(R__b);
      R__b >> eX;
      R__b >> eY;
      R__b >> eZ;
      R__b >> eArea;
      R__b >> eVolume;
      R__b >> eFrame;
      R__b >> eSide;
      R__b >> eSegment;
      R__b.CheckByteCount(R__s, R__c, EdbCluster::IsA());
      //====end of old versions
   } else {
     EdbCluster::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbCluster::Set0()
{
  SetX(0); SetY(0); SetZ(0);
  eArea=0; eVolume=0; 
  eFrame=0; eSide=0; eSegment=0;
}

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
void EdbClustersBox::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbClustersBox.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbClustersBox::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      EdbPointsBox3D::Streamer(R__b);
      eClusters->Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, EdbClustersBox::IsA());
      //====end of old versions
   } else {
     EdbClustersBox::Class()->WriteBuffer(R__b,this);
   }
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
