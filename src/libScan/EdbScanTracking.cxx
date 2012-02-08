//-- Author :  Valeri Tioukov   19/02/2011
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanTracking                                                      //
//                                                                      //
// To handle tracking in the scanset                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbLog.h"
#include "EdbDataSet.h"
#include "EdbScanTracking.h"
#include "EdbAlignmentV.h"
#include "EdbTrackFitter.h"

using namespace TMath;

ClassImp(EdbTrackAssembler)
ClassImp(EdbScanTracking)

//--------------------------------------------------------------------------------------
EdbTrackAssembler::~EdbTrackAssembler()
{
}

//--------------------------------------------------------------------------------------
EdbTrackAssembler::EdbTrackAssembler()
{
  eMapMarg = 50.; // [microns]
  eZ = 0;
  eCellN=10;    //mean n/cell
  eDTmax=0.07;
  eDRmax=45.;
  eDZGapMax = 5000;
  eCollisionsRate=0;
  
  // for basetracks:
  eCond.SetDefault();
  eCond.SetSigma0( 4, 4, 0.005, 0.005 );
  eCond.SetPulsRamp0(14., 21.);
  eCond.SetPulsRamp04(14., 21.);
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::DoubletsFilterOut(EdbPattern &p)
{
  EdbAlignmentV adup;
  adup.eDVsame[0]=adup.eDVsame[1]=10.;
  adup.eDVsame[2]=adup.eDVsame[3]=0.08;
  adup.FillGuessCell(p,p,1.);
  adup.FillCombinations();
  adup.DoubletsFilterOut(0);   // assign flag -10 to the duplicated segments
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::AddPattern(EdbPattern &p)
{
  int ntrBefore=eTracks.GetEntries();
  if(ntrBefore>0) ExtrapolateTracksToZ(p.Z());
  
  //DoubletsFilterOut();
  int nseg = p.N();
  for(int j=0; j<nseg; j++) {
    EdbSegP *s = p.GetSegment(j);
    if(s->Flag()==-10)   continue;
    if( !AddSegment( *(p.GetSegment(j)) ) )
         AddSegmentAsTrack( *(p.GetSegment(j)) );
  }
  int ntrAfter = eTracks.GetEntries();
  int totSegTr=0;
  for(int i=0; i<ntrAfter; i++) totSegTr += ((EdbTrackP*)(eTracks.At(i)))->N();
  Log(2,"EdbTrackAssembler::AddPattern","with z=%10.2f   %d segments %d tracks before %d - after; collisionsrate: %d;  segments recorded: %d  totSegTr: %d",
    p.Z(),nseg, ntrBefore, ntrAfter, eCollisionsRate, eSegments.N(), totSegTr );
}

//--------------------------------------------------------------------------------------
EdbTrackP *EdbTrackAssembler::AddSegment(EdbSegP &s)
{
  TObjArray trsel;
  float v[2] = { s.X(), s.Y() };
  //int   ir[2]= { eDRmax+50, eDRmax+50 };
  int nsel =  eTrZMap.SelectObjectsC( v, eDRmax+50 , trsel ); 
  if(!nsel) { 
    //printf("nsel=%d\n",nsel); s.PrintNice(); 
    return 0;  }
  //printf("nsel=%d\n",nsel);
  float prob, probmax = 0.0001;
  EdbSegP *ssbest = 0;
  for(int i=0; i<nsel; i++) {
    EdbSegP *ss = (EdbSegP*)(trsel.At(i));
    //printf("\nbefore: ");    s.PrintNice();
    prob = ProbSeg( *ss, s );
    //printf("prob probmax:  %f %f\n",  prob, probmax);
    if( prob > probmax ) { ssbest = ss; probmax=prob; }
  }
  if(!ssbest)  return 0;
  //printf("probmax = %10.7f \n", probmax);
  s.SetProb(probmax);
  EdbTrackP *t = (EdbTrackP*)(ssbest);
  EdbSegP *sz = t->GetSegmentWithClosestZ( t->Z(), 45. );
  //if(sz) printf("%f %f \n", s.Prob(),sz->Prob());
  if(!sz) t->AddSegment( eSegments.AddSegment(s) );
  else {
    if( !SameSegment(s,*sz) ) {
      if( s.Prob() > sz->Prob() )  t->SubstituteSegment( sz ,  eSegments.AddSegment(s) );
    //printf("%f %f \n", s.Prob(),sz->Prob());
    //s.PrintNice();
      eCollisionsRate++;
    }
  }
  return t;
}

//--------------------------------------------------------------------------------------
bool EdbTrackAssembler::SameSegment( EdbSegP &s1, EdbSegP &s2 )
{
  if( Abs( s1.X() - s2.X() )   <0.0001 &&
      Abs( s1.Y() - s2.Y() )   <0.0001 &&
      Abs( s1.TX()- s2.TX() )  <0.0001 &&
      Abs( s1.TY()- s2.TY() )  <0.0001 &&
      Abs( s1.W() - s2.W() )   <0.0001  )    return true;
  return false;
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::RecalculateSegmentsProb( EdbTrackP &tr )
{
  // assumed that track is fitted: reset the segmets probabilities
  int n=tr.N();
  for(int i=0; i<n; i++) 
    tr.GetSegment(i)->SetProb( ProbSeg( *(tr.GetSegmentF(i)),  *(tr.GetSegment(i)) ) ); 
}

//--------------------------------------------------------------------------------------
float EdbTrackAssembler::ProbSeg( EdbSegP &s1, EdbSegP &s2 )
{
 // return the probability that the second segment can belong to track defined by s1
  float dtx = s1.TX() - s2.TX();
  if( Abs( dtx ) > eDTmax )    return 0;
  float dty = s1.TY() - s2.TY();
  if( Abs( dty ) > eDTmax )    return 0;
  double dt2 = dtx*dtx +  dty*dty;
  if(dt2>eDTmax*eDTmax)        return 0;

  float dz = s2.Z()-s1.Z();
  float dx = s2.X() - (s1.X() + dz*s1.TX());
  if( Abs( dx ) > eDRmax )     return 0;
  float dy = s2.Y() - (s1.Y() + dz*s1.TY());
  if( Abs( dy ) > eDRmax )     return 0;
  double dr2 = dx*dx +  dy*dy;
  if(dr2>eDRmax*eDRmax)        return 0;

  EdbSegP s;
  float chi2 = eFitter.Chi2SegM( s1, s2, s, eCond, eCond );
  
  float prob = (float)TMath::Prob( chi2*chi2, 4 );
  
  prob *= eCond.ProbSeg( s2.Theta(), s2.W() );            // the proability component depending on the grains number
  prob *= (float)TMath::Prob( s2.Chi2()*s2.Chi2(), 4 );   // the proability component depending on the segment strength
  
  return prob;
}

//--------------------------------------------------------------------------------------
EdbTrackP *EdbTrackAssembler::AddSegmentAsTrack(EdbSegP &s)
{
  if(s.W()<16    )  return 0;
  if(s.Chi2()>2.5)  return 0;
  EdbTrackP *t = new EdbTrackP( eSegments.AddSegment(s), 0.139);    // EdbTrackAssembler is owner of segments 
  eTracks.Add(t);
  //EdbSegP ss;
  //t->MakePredictionTo(eZ,ss);
  //eTrZ.AddSegment(ss);
  return t;
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::ExtrapolateTracksToZ(float z)
{
  eTrZ.Clear();
  eTrZMap.CleanCells();
  
  eZ=z;
  int n=eTracks.GetEntries();
  for(int i=0; i<n; i++) {
    EdbTrackP *t = (EdbTrackP*)(eTracks.At(i));
    
    if( !AcceptDZGap(*t, z) )   continue;
    
    t->MakePredictionTo(eZ,*t);                        // extrapolation of tracks itself
    //((EdbSegP *)t)->PrintNice();
    eTrZ.Add(t);
  }
  
  //InitTrZMap();
  
  FillTrZMap();
  if(gEDBDEBUGLEVEL>2) eTrZMap.PrintStat();
}

//--------------------------------------------------------------------------------------
bool EdbTrackAssembler::AcceptDZGap(EdbTrackP &t, float z)
{
  float z1 = t.GetSegmentFirst()->Z();
  float z2 = t.GetSegmentLast()->Z();
  if(Min( Abs(z1-z), Abs(z2-z)) > eDZGapMax ) return false;
  return true;
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::FillTrZMap()
{
  int n=eTrZ.GetEntries();
  Log(2,"EdbTrackAssembler::FillTrZMap", "with %d tracks",n);
  for(int i=0; i<n; i++) {
    EdbSegP *s = (EdbSegP*)(eTrZ.At(i));
    eTrZMap.AddObject( s->X(), s->Y(), s );
  }
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::InitTrZMap( int nx, float xmi, float xma, 
                                    int ny, float ymi, float yma,  int ncell)
{
  float  mi[2] = {  xmi, ymi };
  float  ma[2] = {  xma, yma };
  int     n[2] = { nx, ny };
  eTrZMap.InitCell(ncell, n, mi, ma);
}

//--------------------------------------------------------------------------------------
void EdbTrackAssembler::InitTrZMap()
{
/*  float  mi[2] = {  eTrZ.Xmin()-eMapMarg, eTrZ.Ymin()-eMapMarg };
  float  ma[2] = {  eTrZ.Xmax()+eMapMarg, eTrZ.Ymax()+eMapMarg };
  float  dens = eTrZ.N()/( (ma[0]-mi[0])*(ma[1]-mi[1]));
  float  step=10000;
  if(dens>0.00000001) step = Sqrt( eCellN/dens );
  int    n[2] = { int((ma[0]-mi[0])/step)+1, int((ma[1]-mi[1])/step)+1 };
  float stepX = (ma[0]-mi[0])/n[0];
  float stepY = (ma[1]-mi[1])/n[1];
  n[0] = int((ma[0]-mi[0]+1.)/stepX);
  n[1] = int((ma[1]-mi[1]+1.)/stepY);
  eTrZMap.InitCell(3*eCellN, n, mi, ma);*/
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void EdbScanTracking::TrackSetBT(EdbID idset, TEnv &env)
{

// read scanset object
 EdbScanSet *ss = eSproc->ReadScanSet(idset);
  if(!ss) { Log(1,"EdbScanTracking::TrackSetBT",
    "Error! set for %s do not found",idset.AsString()); return; }

  int npl = ss->eIDS.GetEntries();
  if(npl<2) { Log(1,"EdbScanTracking::TrackSetBT", "Warning! npl<2 : %d stop tracking!",npl); return; }
 
// create and init tracking object
  EdbTrackAssembler etra;
  
  etra.eCond.SetSigma0(         env.GetValue("fedra.track.Sigma0"         , "3 3 0.005 0.005") );
  etra.eDTmax                 = env.GetValue("fedra.track.DTmax"          ,   0.07 );
  etra.eDRmax                 = env.GetValue("fedra.track.DRmax"          ,  45. );
  bool        do_erase        = env.GetValue("fedra.track.erase"          ,  false );
  const char  *cut            = env.GetValue("fedra.readCPcut"            , "1");
  bool        do_misalign     = env.GetValue("fedra.track.do_misalign"    ,    0 );
  int         npass           = env.GetValue("fedra.track.npass"          ,    2 );
  float       misalign_offset = env.GetValue("fedra.track.misalign_offset",  500. );
  bool        do_local_corr   = env.GetValue("fedra.track.do_local_corr"  ,    1 );

  etra.InitTrZMap(  2400, 0, 120000,   2000, 0, 100000,   30 );

  EdbPattern p;
  
  EdbAffine2D misalign[60];
  if(do_misalign) {
    for(int i=0; i<60; i++) {
      misalign[i].ShiftX(     i%3 * misalign_offset );
      misalign[i].ShiftY( (i+2)%3 * misalign_offset );
    }
  }

// read segments and use them for tracking
  for(int ipass=0; ipass<npass; ipass++) {
    printf("\n\n*************** ipass=%d ************\n",ipass);
    etra.eCollisionsRate=0;
   for(int i=0; i<npl; i++) {
    EdbID *id = ss->GetID(i);
    
    EdbPlateP *plate = ss->GetPlate(id->ePlate);
    
    EdbPattern p;
    eSproc->ReadPatCPnopar(p,*id, cut, do_erase);
    p.SetZ(plate->Z());
    p.SetSegmentsZ();
    p.SetID(i);
    p.SetPID(i);
    p.SetSegmentsPID();
    //plate->Print();
    p.Transform(    plate->GetAffineXY()   );
    p.TransformShr( plate->Shr() );
    p.TransformA(   plate->GetAffineTXTY() );
    p.SetSegmentsPlate(id->ePlate);
    //printf("pattern with z: %f\n", p.Z());
    
    if(do_local_corr) {
        int nseg = p.N();
        for(int j=0; j<nseg; j++) {
          EdbSegP *s = p.GetSegment(j);
          plate->CorrectSegLocal(*s);
        }
    }
    
    
    if(do_misalign) {
      p.Transform(&misalign[i]);
      Log(2,"EdbScanTracking::TrackSetBT","apply misalignment of %f",misalign_offset);
      //misalign[i].Print();
    }

    etra.AddPattern(p);
   }
  }

// save tracks
 
  TObjArray selectedTracks;
  
  EdbTrackFitter fit;
  
  int ntr = etra.Tracks().GetEntries();
  for( int i=0; i<ntr; i++ )     {
    EdbTrackP *t = (EdbTrackP*)(etra.Tracks().At(i));
    int nseg=t->N();
    if(nseg>1) {
      for(int j=0; j<nseg; j++) {
	EdbSegP *s = t->GetSegment(j);
	s->SetErrors();
	etra.eCond.FillErrorsCov(s->TX(),s->TY(),s->COV());
      }
       t->SetP(1.);
       t->FitTrackKFS(0,10000);
       //fit.FitTrackLine(*t);
       etra.RecalculateSegmentsProb(*t);
       t->SetCounters();
       selectedTracks.Add(t);
     }
     //if(t->N()>2)   t->PrintNice();
  }
  
  EdbDataProc::MakeTracksTree( selectedTracks, 0., 0., Form("b%s.trk.root", idset.AsString()) );

}
