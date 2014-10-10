//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbView                                                              //
//                                                                      //
// Implementation of scanning View class                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TSystem.h>
#include <TRandom.h>
#include <TMath.h>
#include <TH3.h>
#include <TPolyLine3D.h>

#include "EdbCluster.h"
#include "EdbFrame.h"
#include "EdbView.h"
#include "EdbSegment.h"
#include "EdbRun.h"
#include "EdbLog.h"

ClassImp(EdbView)
ClassImp(EdbViewHeader)

//______________________________________________________________________________
EdbView::EdbView()
{
  eHeader   = new EdbViewHeader();
  eClusters = new TClonesArray("EdbCluster",  5000);
  eSegments = new TClonesArray("EdbSegment",  5000);
  eTracks   = new TClonesArray("EdbTrack",     200);
  eFrames   = new TClonesArray("EdbFrame",      32);

  eLastSystemTime = gSystem->Now();
}

//______________________________________________________________________________
EdbView::~EdbView()
{
  //  Clear();

  if(eClusters)  { eClusters->Delete(); SafeDelete(eClusters); }
  if(eSegments)  { eSegments->Delete(); SafeDelete(eSegments); }
  if(eTracks)    { eTracks->Delete();   SafeDelete(eTracks);   }
  if(eHeader)    SafeDelete(eHeader);
  if(eFrames)    { eFrames->Delete();   SafeDelete(eFrames);   }
}

//______________________________________________________________________________
void EdbView::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbView.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 2) {
	EdbView::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      TObject::Streamer(R__b);
      R__b >> eHeader;
      eClusters->Streamer(R__b);
      eSegments->Streamer(R__b);
      eTracks->Streamer(R__b);
      eFrames->Streamer(R__b);
      R__b >> eLastSystemTime;
      R__b.CheckByteCount(R__s, R__c, EdbView::IsA());
      //====end of old versions
   } else {
     EdbView::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbView::Clear()
{
  if(eClusters) { eClusters->Clear(); }
  if(eSegments) { 
    TObjArray *e=0;
    for(int i=0; i<Nsegments(); i++) {
      e=((EdbSegment*)eSegments->At(i))->GetElements();
      if( e ) e->Clear();
    }
    eSegments->Clear(); 
  }
  if(eTracks)   { eTracks->Clear();   }
  if(eFrames)   { eFrames->Clear();   }
}


//______________________________________________________________________________
void EdbView::Transform( const EdbAffine2D *aff )
{
  if(eClusters) {
    int ncl=Nclusters();
    for( int i=0; i<ncl; i++ )   ((EdbPoint2D*)(GetCluster(i)))->Transform(aff);
  }
  if(eSegments) {
    int nseg=Nsegments();
    for( int i=0; i<nseg; i++ )   GetSegment(i)->Transform(aff);
  }
  if(eTracks) {
    int ntr=Ntracks();
    for( int i=0; i<ntr; i++ )     GetTrack(i)->Transform(aff);
  }
}

//______________________________________________________________________________
void EdbView::Scale( float zscale )
{
  // scale all z coordinates and angles to the given factor
  EdbCluster *c=0;
  EdbSegment *s=0;
  EdbTrack   *t=0;
  if(eClusters) {
    int ncl=Nclusters();
    for( int i=0; i<ncl; i++ ) {  
      c = GetCluster(i); 
      c->SetZ( c->GetZ()*zscale ); 
    }
  }
  if(eSegments) {
    int nseg=Nsegments();
    for( int i=0; i<nseg; i++ ) { 
      s = GetSegment(i); 
      s->SetZ0( s->GetZ0()*zscale ); 
      s->SetTx( s->GetTx()/zscale ); 
      s->SetTy( s->GetTy()/zscale ); 
    }
  }
  if(eTracks) {
    int ntr=Ntracks();
    for( int i=0; i<ntr; i++ ) {
      t = GetTrack(i);
      t->SetZ0( t->GetZ0()*zscale ); 
      t->SetTx( t->GetTx()/zscale ); 
      t->SetTy( t->GetTy()/zscale );
    }
  }
}

//______________________________________________________________________________
void EdbView::Shift( float zshift )
{
  // shift all z coordinates to the given offset
  EdbCluster *c=0;
  EdbSegment *s=0;
  EdbTrack   *t=0;
  if(eClusters) {
    int ncl=Nclusters();
    for( int i=0; i<ncl; i++ ) {  
      c = GetCluster(i); 
      c->SetZ( c->GetZ()+zshift ); 
    }
  }
  if(eSegments) {
    int nseg=Nsegments();
    for( int i=0; i<nseg; i++ ) { 
      s = GetSegment(i); 
      s->SetZ0( s->GetZ0()+zshift ); 
    }
  }
  if(eTracks) {
    int ntr=Ntracks();
    for( int i=0; i<ntr; i++ ) {
      t = GetTrack(i);
      t->SetZ0( t->GetZ0()+zshift ); 
    }
  }
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
float EdbView::ZFrameMin() const
{
  float zmin=GetFrame(0)->GetZ();
  for(int i=1; i<GetNframes(); i++) 
    {
      float z = GetFrame(i)->GetZ();
      if(z<zmin) zmin=z;
    }
  return zmin;
}

//______________________________________________________________________________
float EdbView::ZFrameMax() const
{
  float zmax=GetFrame(0)->GetZ();
  for(int i=1; i<GetNframes(); i++) 
    {
      float z = GetFrame(i)->GetZ();
      if(z>zmax) zmax=z;
    }
  return zmax;
}

//______________________________________________________________________________
EdbSegment* EdbView::AddSegment( EdbSegment &s )
{
  // return directly pointer to the segment
  int i = eSegments->GetLast()+1;
  return new((*eSegments)[i++])  EdbSegment( s );
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

  //printf("EdbView::AddFrame: \n");
  //frame->Print();
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
int EdbView::AttachSegmentsToTracks()
{
  // Assumed that:
  // tracks.eID                       - number of segments in a track
  // Segments are written consecutively in the order they appear in microtracks (i.e. m0c0,m0c1,m0c2,m1c0,m1c1,m2c0,m2c1...)

  int nseg = eSegments->GetLast()+1; if(nseg<1) return 0;
  int ntr = eTracks->GetLast()+1;     if(ntr<1) return 0;

  int icnt=0;
  for(int itr=0; itr<ntr; itr++)
  {
    EdbTrack *t = (EdbTrack*)(eTracks->UncheckedAt(itr));
    if(!t) continue;
    int nst = t->GetID(); if(nst<1) continue;
    if( t->GetElements() ) t->GetElements()->Clear();
    for(int ist=0; ist<nst; ist++)
    {
      if(icnt >= nseg) break;
      EdbSegment *s = (EdbSegment*)(eSegments->UncheckedAt(icnt++));
      if(s) t->AddElement(s);
    }
  }
  Log(3,"EdbView::AttachSegmentsToTracks", "ntr = %d nseg = %d icnt = %d",ntr,nseg,icnt);
  return icnt;
}

//______________________________________________________________________________
int EdbView::AttachClustersToSegments()
{
  int nca=-1;
  nca = AttachClustersToSegmentsFast();
  if(nca<0)  {
    nca = AttachClustersToSegmentsSlow();
    printf("AttachClustersToSegments: segments do not sorted - use slow algorithm!\n");
  }
  return nca;
}

//______________________________________________________________________________
int EdbView::AttachClustersToSegmentsFast()
{
  // assume that cluster->GetSegment() == segment entry :

  EdbCluster *cl=0;
  int ncl = eClusters->GetLast()+1;
  EdbSegment *seg=0;
  int nseg = eSegments->GetLast()+1;

  // TODO: why elements not cleared???

//    for(int is=0; is<nseg; is++){
//      seg = (EdbSegment*)(eSegments->UncheckedAt(is));
//      if(seg->GetNelements()>0) seg->GetElements()->Clear();
//    }

  int iseg;
  int nca=0;
  for(int i=0; i<ncl; i++){
    cl = (EdbCluster*)(eClusters->UncheckedAt(i));
    iseg=cl->GetSegment();
    if( iseg < 0 )      continue;
    if( iseg > nseg-1 ) return -1;
    seg = (EdbSegment*)(eSegments->At(iseg));
    seg->AddElement(cl);
    nca++;
    if(seg->GetNelements()>seg->GetPuls()) {
      printf("AttachClustersToSegmentsFast: ncl(%d) > puls(%d)    %d\n", 
	     seg->GetNelements(),seg->GetPuls(), seg->GetID());
      printf("Area: %d View: %d  Segment: %d, i,iseg = %d %d\n",GetAreaID(), GetViewID(), seg->GetID(), i,iseg);
    }

  }
  return nca;
}

//______________________________________________________________________________
int EdbView::AttachClustersToSegmentsSlow()
{
  // if segments do not ordered :

  EdbCluster *cl=0;
  int ncl = eClusters->GetLast()+1;

  EdbSegment *seg=0;
  int nseg = eSegments->GetLast()+1;
  int id,nca=0;
  for(int is=0; is<nseg; is++){
    seg = (EdbSegment*)(eSegments->UncheckedAt(is));
    if(seg->GetNelements()>0) seg->GetElements()->Clear();
    id = seg->GetID();
    for(int i=0; i<ncl; i++){
      cl = (EdbCluster*)(eClusters->UncheckedAt(i));
      if( cl->GetSegment() != id ) continue;
      seg->AddElement(cl);
      nca++;
    }
  }
  return nca;
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
  float xmin=-200, xmax=200;
  float ymin=-150, ymax=150;
  //float xmin=GetXview()-500, xmax=xmin+1000.;
  //float ymin=GetYview()-500, ymax=ymin+1000.;

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
    marksSeg->SetMarkerStyle(21);
    marksSeg->SetMarkerSize(.3);
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
EdbViewHeader::EdbViewHeader()
{
  Set0();
}

//______________________________________________________________________________
EdbViewHeader::~EdbViewHeader()
{
  //if(eZlevels) delete eZlevels;
}

//______________________________________________________________________________
void EdbViewHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbViewHeader.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 4) {
	EdbViewHeader::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      R__b >> eViewID;
      R__b >> eAreaID;
      R__b >> eXview;
      R__b >> eYview;
      R__b >> eZ1;
      R__b >> eZ2;
      R__b >> eZ3;
      R__b >> eZ4;
      R__b >> eNframesTop;
      R__b >> eNframesBot;
      R__b >> eTime;
      R__b >> eNclusters;
      R__b >> eNsegments;
      eAff.Streamer(R__b);
      R__b >> eCol;
      R__b >> eRow;
      R__b >> eStatus;
      R__b >> eEvent;
      R__b >> eTrack;
      R__b.CheckByteCount(R__s, R__c, EdbViewHeader::IsA());
      //====end of old versions
   } else {
     EdbViewHeader::Class()->WriteBuffer(R__b,this);
   }
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
  eCol=0;
  eRow=0;
  eStatus=0;
  eEvent=-999;
  eTrack=-999;
}

//______________________________________________________________________________
void EdbViewHeader::Print() const
{
  printf("ViewHeader:\t %d %d \t %f %f \t col:row = %d:%d\n", eViewID, eAreaID, eXview, eYview, eCol, eRow);
  printf("Nframes:   \t %d/%d \t Z(1/2/3/4): %f %f %f %f\n",
		eNframesTop,eNframesBot, eZ1,eZ2,eZ3,eZ4);
  printf("Ncl = %d \t Nseg = %d\n",eNclusters, eNsegments);
  printf("View time: \t %d  \t Scan status = %d \n", eTime, eStatus );
}
