//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbView                                                              //
//                                                                      //
// Implementation of scanning View class                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "TRandom.h"
#include "TMath.h"
#include "TH3.h"
#include "TPolyLine3D.h"

#ifndef ROOT_EdbCluster
#include "EdbCluster.h"
#endif

#ifndef ROOT_EdbFrame
#include "EdbFrame.h"
#endif

#ifndef ROOT_EdbView
#include "EdbView.h"
#endif

#ifndef ROOT_EdbSegment
#include "EdbSegment.h"
#endif

#ifndef ROOT_EdbRun
#include "EdbRun.h"
#endif

ClassImp(EdbView)
ClassImp(EdbViewHeader)


//______________________________________________________________________________
EdbView::EdbView()
{
  eHeader   = new EdbViewHeader();
  eClusters = new TClonesArray("EdbCluster", 20000);
  eSegments = new TClonesArray("EdbSegment",  5000);
  eTracks   = new TClonesArray("EdbTrack",    2000);
  eFrames   = new TClonesArray("EdbFrame",      32);

  eLastSystemTime = gSystem->Now();
}

//______________________________________________________________________________
EdbView::~EdbView()
{
  Clear();

  if(eClusters)  delete eClusters;
  if(eSegments)  delete eSegments;
  if(eTracks)    delete eTracks;
  if(eHeader)    delete eHeader;
  if(eFrames)    delete eFrames; 
}

//______________________________________________________________________________
void EdbView::Clear()
{
  if(eClusters) { eClusters->Clear(); }
  if(eSegments) { eSegments->Clear(); }
  if(eTracks)   { eTracks->Clear();   }
  if(eFrames)   { eFrames->Clear();   }
}


//______________________________________________________________________________
void EdbView::Transform( EdbAffine2D *aff )
{
  if(eClusters)   
    for( int i=0; i<Nclusters(); i++ )   ((EdbPoint2D*)(GetCluster(i)))->Transform(aff);
  if(eSegments) 
    for( int i=0; i<Nsegments(); i++ )   GetSegment(i)->Transform(aff);
  if(eTracks)   
    for( int i=0; i<Ntracks(); i++ )     GetTrack(i)->Transform(aff);
}

//______________________________________________________________________________
int EdbView::ReadView( char *fname )
{
  FILE *fp = fopen(fname,"r");
  fclose(fp);
  return 0;
}

//______________________________________________________________________________
void EdbView::Print( Option_t *opt ) const
{
  printf("\nEdbView: ");
  if(eClusters) printf(  "\t %d clusters\n", eClusters->GetLast()+1);
  if(eSegments) printf("\t\t %d segments\n", eSegments->GetLast()+1);
  if(eTracks)   printf("\t\t %d tracks  \n", eTracks->GetLast()+1);
  if(eFrames)   printf("\t\t %d frames  \n", eFrames->GetLast()+1);
  eHeader->Print();
}

//______________________________________________________________________________
void EdbView::PrintClusters( Option_t *opt ) const
{
  EdbCluster     *cluster  =0;

  int ncl = eClusters->GetLast()+1;
  for( int icl=0; icl<ncl; icl++) {
    cluster = (EdbCluster*)(eClusters->UncheckedAt(icl));
    cluster->Print();
  }  
}

//______________________________________________________________________________
void EdbView::DeleteClustersFog()
{
  EdbCluster     *cluster  =0;

  int ncl = Nclusters();
  for( int icl=0; icl<ncl; icl++) {
    cluster = (EdbCluster*)(eClusters->UncheckedAt(icl));
    if( cluster->GetSegment() < 0 )     eClusters->Remove(cluster);
  }

  eClusters->Compress();
}

//______________________________________________________________________________
void EdbView::AddCluster( EdbCluster *c )
{
  int i = eClusters->GetLast()+1;
  new((*eClusters)[i++])  EdbCluster( *c );
}

//______________________________________________________________________________
void EdbView::AddCluster( float x,  float y,  float z,  
			  float a,  float v, int f, int s, int seg )
{
  int i = eClusters->GetLast()+1;
  new((*eClusters)[i++])  EdbCluster( x,y,z, a,v,f,s,seg );
}

//______________________________________________________________________________
void EdbView::AddSegment( EdbSegment *s )
{
  int i = eSegments->GetLast()+1;
  new((*eSegments)[i++])  EdbSegment( *s );
}

//______________________________________________________________________________
void EdbView::AddTrack( EdbTrack *t )
{
  int i = eTracks->GetLast()+1;
  new((*eTracks)[i++])  EdbTrack( *t );
}

//______________________________________________________________________________
void EdbView::AddFrame( int id, float z, int ncl, int npix )
{
  int i = eFrames->GetLast()+1;
  new((*eFrames)[i++])  EdbFrame( id,z,ncl,npix );
}

//______________________________________________________________________________
void EdbView::AddFrame( EdbFrame *frame )
{
  int i = eFrames->GetLast()+1;
  new((*eFrames)[i++])  EdbFrame( *frame );

  printf("EdbView::AddFrame: \n");
  frame->Print();
}

//______________________________________________________________________________
TList *EdbView::GetClustersFrame( int frame ) const
{
  TList *list = new TList();

  EdbCluster *cluster=0;
  int ncl = eClusters->GetLast()+1;

  for(int i=0; i<ncl; i++){
    cluster = (EdbCluster*)(eClusters->UncheckedAt(i));
    if( cluster->GetFrame() == frame )  list->Add(cluster);
  }
  return list;
}

//______________________________________________________________________________
TPolyMarker3D *EdbView::DrawClustersFog(Option_t *opt) const
{
  EdbCluster *cluster=0;
  int ncl = eClusters->GetLast()+1;

  TPolyMarker3D *marks = new TPolyMarker3D(ncl,1);

  for(int i=0; i<ncl; i++){
    cluster = (EdbCluster*)(eClusters->At(i));
    if( cluster->GetSegment() < 0 ) 
      marks->SetNextPoint( cluster->GetX(), cluster->GetY(), cluster->GetZ() );
  }
  return marks;
}

//______________________________________________________________________________
TPolyMarker3D *EdbView::DrawClustersSegments(Option_t *opt) const
{
  EdbCluster *cluster=0;
  int ncl = eClusters->GetLast()+1;

  TPolyMarker3D *marks = new TPolyMarker3D(ncl,1);

  for(int i=0; i<ncl; i++){
    cluster = (EdbCluster*)(eClusters->At(i));
    if( cluster->GetSegment() >= 0 ) 
      marks->SetNextPoint( cluster->GetX(), cluster->GetY(), cluster->GetZ() );
  }
  return marks;
}

//______________________________________________________________________________
TObjArray *EdbView::GetSegmentsClusters() const
{
  int nseg = eSegments->GetLast()+1;
  TObjArray *segments = new TObjArray(nseg);
  
  EdbSegment *seg1=0, *seg2=0;
  
  for(int iseg=0; iseg<nseg; iseg++) {
    seg1 = (EdbSegment*)eSegments->At(iseg);
    seg2 = new EdbSegment(*seg1);
    segments->Add(seg2);
  }

  EdbCluster *cl1=0, *cl2=0;

  int ncl = eClusters->GetLast()+1;
  int segID=-1;

  for(int i=0; i<ncl; i++) {
    cl1 = (EdbCluster*)eClusters->At(i);
    segID = cl1->GetSegment();
    if( segID < 0)      continue;
    if( segID > nseg)   continue;
    cl2 = new EdbCluster(*cl1);
    seg1 = (EdbSegment*)segments->At(segID);
    seg1->AddElement(cl2);
  }
    
  return segments;
}

//______________________________________________________________________________
void EdbView::Draw(Option_t *option)
{
  float xmin=0, xmax=512;
  float ymin=0, ymax=512;
  //float xmin=GetXview()-500, xmax=xmin+1000.;
  //float ymin=GetYview()-500, ymax=ymin+1000.;

  //float xmin=Xmin(), xmax=Xmax();
  //float ymin=Ymin(), ymax=Ymax();
  float zmin = TMath::Min( GetZ1(), GetZ4() );
  float zmax = TMath::Max( GetZ1(), GetZ4() );

  TH3F *hist = new TH3F("hist","title",1,xmin,xmax,1,ymin,ymax,1,zmin,zmax);
  //TH3F *hist = new TH3F("hist","title",1,0,512,1,0,512,1,zmin,zmax);

  hist->Draw();

  if(eClusters) {
    TPolyMarker3D *marksFog = DrawClustersFog();
    marksFog->SetMarkerColor(kBlue);
    marksFog->SetMarkerStyle(21);
    marksFog->SetMarkerSize(.3);
    marksFog->Draw();

    TPolyMarker3D *marksSeg = DrawClustersSegments();
    marksSeg->SetMarkerColor(kRed);
    marksSeg->SetMarkerStyle(20);
    marksSeg->SetMarkerSize(.4);
    marksSeg->Draw();
  }

  if(eTracks) {
    // Draw tracks ....

    EdbTrack   *tr = 0;
    int ntr = Ntracks();
    
    for(int itr=0; itr<ntr; itr++) {
      tr = (EdbTrack*)eTracks->At(itr);
      
      TPolyLine3D *line = new TPolyLine3D(2);
      line->SetPoint(0, tr->GetX0(), tr->GetY0(), tr->GetZ0() );
      line->SetPoint(1, 
		     tr->GetX0() + tr->GetTx()*tr->GetDz(),
		     tr->GetY0() + tr->GetTy()*tr->GetDz(),
		     tr->GetZ0()+              tr->GetDz() );

      line->SetLineColor(kGreen);
      line->SetLineWidth(10);
      line->Draw();
    }
  }

  if(eSegments) {
    // Draw segments ....

    EdbSegment   *seg = 0;
    int nseg =  Nsegments();

    for(int iseg=0; iseg<nseg; iseg++) {
      seg = (EdbSegment*)eSegments->At(iseg);
      
      TPolyLine3D *line = new TPolyLine3D(2);
      line->SetPoint(0, seg->GetX0(), seg->GetY0(), seg->GetZ0() );
      line->SetPoint(1, 
		     seg->GetX0() + seg->GetTx()*seg->GetDz(),
		     seg->GetY0() + seg->GetTy()*seg->GetDz(),
		     seg->GetZ0()+               seg->GetDz() );
      line->Draw();
    }
  }

}

//______________________________________________________________________________
float EdbView::Xmax() const
{
  // View coordinates are in pixels:
  if(gRUN)     return  Xmin() + gRUN->GetHeader()->CameraWidth();
  else         return  512;
}

//______________________________________________________________________________
float EdbView::Ymax() const
{
  // View coordinates are in pixels:
  if(gRUN)     return  Ymin() + gRUN->GetHeader()->CameraHeight();
  else         return  512;
}

//______________________________________________________________________________
void EdbView::GenerateClustersTrack( EdbTrack *track, int n0, float sigma)
{
  EdbSegment segment;

  int  count = Nsegments();
  float x0 = track->GetX0();
  float y0 = track->GetY0();
  float z1 = GetZ1();
  float z2 = GetZ2();
  float z3 = GetZ3();
  float z4 = GetZ4();
  float tx = track->GetTx();
  float ty = track->GetTy();

  segment.Set( x0,y0,  z1, tx, ty, z2-z1, 0, -1, count );
  AddSegment(&segment);
  GenerateClustersSegment( GetSegment(Nsegments()-1), 25, .3 );

  x0 = x0 + (z3-z1)*tx;
  y0 = y0 + (z3-z1)*ty;
  count = Nsegments();

  segment.Set( x0,y0,  z3, tx, ty, z4-z3, 1, -1, count );
  AddSegment(&segment);
  GenerateClustersSegment( GetSegment(Nsegments()-1), 25, .3 );
}

//______________________________________________________________________________
void EdbView::GenerateClustersSegment( EdbSegment *segment, int n0, float sigma)
{
  //    n0    - mean number of grains/100 microns
  //    sigma - XY dispersion in microns

  float xmin = 0;
  float ymin = 0;
  float xmax = 512;
  float ymax = 512;

  float  length0  = 100./n0;    // mean flight length [microns]

  float x0 = segment->GetX0();
  float y0 = segment->GetY0();
  float z0 = segment->GetZ0();
  float dz = segment->GetDz();
  float x=0,y=0,z=0;
  float path=0;
  int area=0, volume=0, frame=-1;
  int side=segment->GetSide();
  int seg=segment->GetID();

  float tx = segment->GetTx();
  float ty = segment->GetTy();

  length0 = length0/sqrt( 1 + tx*tx + ty*ty );

  int ncl=0;

  while( 1 ) {
    path += gRandom->Exp(length0); 
    if( path>dz)   break;
    z = z0+path;
    x = x0 + tx*path + gRandom->Gaus(0,sigma);
    y = y0 + ty*path + gRandom->Gaus(0,sigma);

    if( x < xmin )   continue;
    if( x > xmax )   continue;
    if( y < ymin )   continue;
    if( y > ymax )   continue;

    volume = gRandom->Poisson(50);
    AddCluster(x,y,z, area, volume, frame, side, seg );
    ncl++;
  }
  segment->SetPuls(ncl);
}

//______________________________________________________________________________
void EdbView::GenerateClustersFog( float density )
{
  float  x1=0, x2=512;
  float  y1=0, y2=512;

  float x=0,y=0,z=0; 
  int   area=0, volume=0, frame=0, side=0;

  float z1 = GetZ1();
  float z2 = GetZ2();
  float z3 = GetZ3();
  float z4 = GetZ4();

  float activeVolume = TMath::Abs((x2-x1)*(y2-y1)*( z2-z1 + z4-z3 ));
  int   nClusters    = (int)( density*activeVolume/(10*10*10) );

  int   ft = GetNframesTop();
  int   fb = GetNframesBot();
  int   nf = ft+fb;

  for( int i=0; i<nClusters; i++ ){

    frame = gRandom->Integer( nf );
    if( frame < ft )   {
      side = 0;
      z    = z1 + (z2-z1)/ft * frame;
    }
    else {
      side = 1;
      z    = z3 + (z4-z3)/fb * (frame-ft);
    }
  
    x      = x1 + gRandom->Rndm()*(x2-x1);
    y      = y1 + gRandom->Rndm()*(y2-y1);
    area   = (int)gRandom->Exp(5);
    volume = gRandom->Poisson(50);

    AddCluster(x,y,z, area,volume, frame, side);
  }
}

//______________________________________________________________________________
void EdbView::GenerateFrames( int n )
{
  if(eFrames) eFrames->Clear();

  EdbFrame *frame=0;

  for( int ifr=0; ifr<n; ifr++) {
    frame = new EdbFrame( ifr,2,3,"123456" );
    AddFrame(frame);
  }
}

//______________________________________________________________________________
//______________________________________________________________________________
EdbViewHeader::EdbViewHeader()
{
  Set0();
}

//______________________________________________________________________________
EdbViewHeader::~EdbViewHeader()
{
  if(eZlevels) delete eZlevels;
}

//______________________________________________________________________________
void EdbViewHeader::Set0()
{
  eViewID  = 0; 
  eAreaID  = 0; 
  eXview   = 0;
  eYview   = 0;
  eZ1   = 0;
  eZ2   = 0;
  eZ3   = 0;
  eZ4   = 0;
  eNframesTop = 0;
  eNframesBot = 0;

  eTime=0;
  eNclusters=0;
  eNsegments=0;

  eZlevels=0;
}

//______________________________________________________________________________
void EdbViewHeader::Print() const
{
  printf("ViewHeader:\t %d %d \t %f %f \n", eViewID, eAreaID, eXview, eYview);
  printf("View time: \t %d \n", eTime );
  if(eZlevels) {
    printf("Zlevels(%d): \t", eZlevels->GetSize() );
    for(int i=0; i<eZlevels->GetSize(); i++)       printf(" %f ", eZlevels->At(i) );
    printf("\n");
  }
}
