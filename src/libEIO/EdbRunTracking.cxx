//-- Author :  Valeri Tioukov   15.11.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunTracking                                                       //
//                                                                      //
// ??????????????????                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "Riostream.h"
#include "TSystem.h"
#include "EdbMath.h"
#include "EdbSegment.h"
#include "EdbCluster.h"
#include "EdbRunAccess.h"
#include "EdbRunTracking.h"
#include "EdbPVRec.h"
#include "EdbLog.h"
#include "EdbTrackFitter.h"
#ifndef __CINT__
#include "libDataConversion.h"
#endif

using namespace TMath;

ClassImp(EdbRunTracking)

///_________________________________________________________________________
EdbRunTracking::~EdbRunTracking()
{

}

///_________________________________________________________________________
void EdbRunTracking::Set0()
{
  eCondMT.SetDefault();
  eCondBT.SetDefault();
  eDeltaTheta=0.;
  eDeltaR=0;
  ePulsMinMT=0.;
  eChi2MaxMT=0.;
  ePulsMinBT=0.;
  eChi2MaxBT=0.;
}


//----------------------------------------------------------------------------------------
int EdbRunTracking::UpdateFlag(int flag, int status)
{
  int bth = flag/10000;
  int mth = (flag/100)%100;
  int tb  = flag%10;

  switch (status) {
  case -1: bth++; mth++;       break;
  case  0: bth=0; mth=0; tb=0; break;
  case  1: bth++; mth=0; tb=1; break;
  case  2: bth++; mth=0; tb=2; break;
  }

  return( bth*10000+mth*100+tb );
}


//----------------------------------------------------------------------------------------
int EdbRunTracking::GetHoles(int flag)
{
  return(flag/10000);
}

//----------------------------------------------------------------------------------------
int EdbRunTracking::FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr )
{
  // return found sorted by increasing chi2

  int nfound=0;
  int maxcand=chiarr.GetSize();
  TArrayF   chi2arr(maxcand);
  TObjArray arr(maxcand);
  TArrayI   ind(maxcand);

  int nseg = pat.FindCompliments(s,arr,30,200);  // acceptance (prelim): s.SX()*30; s.STX*200
  printf("\nnseg = %d\n",nseg);
  if(nseg>maxcand)  {
    printf("Warning!: Too many segments %d, accept only the first %d \n", nseg, maxcand);
    nseg = maxcand;
  }
  if(nseg<=0) return 0;

  EdbSegP *s2=0;
  for(int j=0; j<nseg; j++) {
    s2 = (EdbSegP *)arr.At(j);
    EdbSegP s3;
    s3.Copy(s);
    chi2arr[j] = EdbTrackFitter::Chi2Seg(&s3, s2);
  }
  TMath::Sort(nseg,chi2arr.GetArray(),ind.GetArray(),0);
  printf("pred = %f %f %f %f\n",s.X(),s.Y(),s.TX(),s.TY());
  for(int j=0; j<nseg; j++) {
    s2 = (EdbSegP *)arr.At(ind[j]);
    printf("j = %d, ind = %d, chi2 = %f    %f\n",j,ind[j],chi2arr[ind[j]],chi2max);
    printf("Tx = %f %f %f %f\n",s2->X(),s2->Y(),s2->TX(),s2->TY());
    if(chi2arr[ind[j]] > chi2max ) break;
    chiarr[j] = chi2arr[ind[j]];
    s2->SetMC(s.MCEvt(),s.MCTrack());
    found.Add(s2);
    nfound++;
  }

  printf("nfound = %d\n",nfound);
  return nfound;
}


//----------------------------------------------------------------------------------------
int EdbRunTracking::FindCandidates( EdbSegP &spred, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 )
{
  // Find microtracks and basetracks for the prediction segment "spred"
  // Selection criteria:
  //  1) select all preliminary microtracks with (puls >= ePreliminaryPulsMinMT) (6) and (chi2 < ePreliminaryChi2MaxMT) for both side
  //  2) select all basetracks with preliminary microtracks, with (chi2_bt < eChi2MaxBT)
  //  3) if no basetrack, apply cut on microtracks: (puls >= ePulsMinMT) (10) and (chi2 < eChi2MaxMT)

  // Input:
  //   spred - track prediction
  // Output: 
  //   fnds1 - microtracks having puls>=ePulsMinMT and chi2<eChi2MaxMT in the top side of the emulsion plate
  //   fnds2 - microtracks having puls>=ePulsMinMT and chi2<eChi2MaxMT in the bottom side of the emulsion plate
  //   fndbt - basetracks built from the preliminary microtracks and (chi2_bt < eChi2MaxBT)

  ClearCuts();

  spred.SetZ(107.);                                      // TODO!
  spred.SetErrors();
  float xmin[5]={-500, -500, spred.TX()-eDeltaTheta, spred.TY()-eDeltaTheta, ePreliminaryPulsMinMT };         //TODO!!
  float xmax[5]={ 500,  500, spred.TX()+eDeltaTheta, spred.TY()+eDeltaTheta, 50 };

  eCondBT.FillErrorsCov( spred.TX(), spred.TY(), spred.COV() );

  EdbPVRec aview; //with 2 patterns of preselected microtracks
  aview.AddPattern( new EdbPattern(0,0,214));  // TODO! sequence??
  aview.AddPattern( new EdbPattern(0,0,0)  );

  for(int side=1; side<=2; side++) {
    Log(3,"FindCandidates","side = %d\n",side);
    EdbPattern pat;
    AddSegmentCut(side,1,xmin,xmax);
    GetPatternXY( spred, side,  pat );

    for(int i=0; i<pat.N(); i++) {
      EdbSegP *s = pat.GetSegment(i);
      s->SetErrors();
      eCondMT.FillErrorsCov( spred.TX(), spred.TY(), s->COV() );
    }
    pat.FillCell(10,10,0.01,0.01);  //divide view on this cells
      
    TArrayF chi2arr(1000);  //TODO!
    TObjArray found;
    
    FindCompliments( spred, pat, found, ePreliminaryChi2MaxMT, chi2arr );
    for(int j=0; j<found.GetEntries(); j++) {
      EdbSegP *s = (EdbSegP *)(found.At(j));
      s->SetChi2(chi2arr[j]);                        // TODO -???
      aview.GetPattern(side-1)->AddSegment(*s);
    }
  }

  // filling fnds1

  Log(3,"FindCandidates","%d %d\n",aview.GetPattern(0)->N(),aview.GetPattern(1)->N());

  for(int is=0; is<aview.GetPattern(0)->N(); is++) {
    EdbSegP *s = aview.GetPattern(0)->GetSegment(is);
    if ( s->W()>=ePulsMinMT && s->Chi2()<eChi2MaxMT ) fnds1.AddSegment(*s);
  }

  // filling fnds2

  for(int is=0; is<aview.GetPattern(1)->N(); is++) {
    EdbSegP *s = aview.GetPattern(1)->GetSegment(is);
    if ( s->W()>=ePulsMinMT && s->Chi2()<eChi2MaxMT ) fnds2.AddSegment(*s);
  }

  // filling fndbt
  
  EdbPattern bt;

  for(int is1=0; is1<aview.GetPattern(0)->N(); is1++) {
    for(int is2=0; is2<aview.GetPattern(1)->N(); is2++) {

      EdbSegP *s1 = aview.GetPattern(0)->GetSegment(is1);
      EdbSegP *s2 = aview.GetPattern(1)->GetSegment(is2);

      float dx1=s1->X()-(spred.X()+spred.TX()*(s1->Z()-spred.Z()));
      float dy1=s1->Y()-(spred.Y()+spred.TY()*(s1->Z()-spred.Z()));
      float dx2=s2->X()-(spred.X()+spred.TX()*(s2->Z()-spred.Z()));
      float dy2=s2->Y()-(spred.Y()+spred.TY()*(s2->Z()-spred.Z()));
      float r = Sqrt( (dx1-dx2)*(dx1-dx2) + (dy1-dy2)*(dy1-dy2) ); 

      if(r<eDeltaR) {  // has good BT
	EdbSegP s3;
	s3.Copy(spred);
	s3.SetX( 0.5*(s1->X() + s2->X()) );
	s3.SetY( 0.5*(s1->Y() + s2->Y()) );
	s3.SetZ( 0.5*(s1->Z() + s2->Z()) );
	s3.SetTX( (s2->X() - s1->X()) / (s2->Z() - s1->Z()) );
	s3.SetTY( (s2->Y() - s1->Y()) / (s2->Z() - s1->Z()) );
	s3.SetFlag(0);

	float chi = EdbTrackFitter::Chi2Seg(&s3, &spred);
	if(chi<eChi2MaxBT) {
	  s3.SetChi2(chi);
	  bt.AddSegment(s3);
	}
      }
    }
  }

  TArrayF chi2arr(bt.N());
  for (int i=0;i<bt.N();i++) chi2arr[i]=bt.GetSegment(i)->Chi2();
  TArrayI   ind(bt.N());
  TMath::Sort(bt.N(),chi2arr.GetArray(),ind.GetArray(),false);
  for(int i=0; i<bt.N(); i++) {
    EdbSegP *s = bt.GetSegment(ind[i]);
    fndbt.AddSegment(*s);
  }

  Log(3,"FindCandidates","Found %d basetrack and %d+%d microtrack candidates\n",fndbt.N(),fnds1.N(),fnds2.N());

  return 1; //TODO!
}


//----------------------------------------------------------------------------------------
// int EdbRunTracking::FindCandidateMT( EdbSegP &spred, EdbSegP &fnd )
// {
//   // Select best microtrack matching with a track prediction
//   // Input: 
//   //   spred - track prediction
//   // Output:
//   //   fnd - found microtrack
//   // Return:
//   //   0 - no microtrack found
//   //   1 - best microtrack is found in top side
//   //   2 - best microtrack is found in bottom side

//   EdbPattern fndbt,fnds1,fnds2;
//   FindCandidates( spred, fndbt, fnds1, fnds2 );

//   int side=0;

//   if (fnds1.N()>0) {
//     fnd.SetChi2(fnds1.GetSegment(0)->Chi2()); 
//     side=1;
//   } else if (fnds2.N()>0) {
//     fnd.SetChi2(fnds2.GetSegment(0)->Chi2());
//     side=2;
//   } else return(0);

//   for (int i=0;i<fnds1.N();i++) {
//     EdbSegP *s = fnds1.GetSegment(i);
//     if( fnd.Chi2() > s->Chi2() ) { fnd.Copy(*s); side=1; }
//   }

//   for (int i=0;i<fnds2.N();i++) {
//     EdbSegP *s = fnds2.GetSegment(i);
//     if( fnd.Chi2() > s->Chi2() ) { fnd.Copy(*s); side=2; }
//   }

//   return(side);
// }


//----------------------------------------------------------------------------------------
int EdbRunTracking::FindCandidateMT( EdbPattern &fnds1, EdbPattern &fnds2, EdbSegP &fnd )
{
  int side=0;

  fnd.SetChi2(10000.);

  if (fnds1.N()>0) {
    EdbSegP *s = fnds1.GetSegment(0);
    fnd.Copy(*s); 
    side=1;
  }
  if (fnds2.N()>0) {
    EdbSegP *s = fnds2.GetSegment(0);
    if (s->Chi2()<fnd.Chi2()) {
      fnd.Copy(*s);
      side=2;
    }
  }

  return(side);
}


//----------------------------------------------------------------------------------------
int EdbRunTracking::FindCandidateBT(EdbPattern &fndbt, EdbSegP &fnd )
{
  if (fndbt.N()>0) {
    EdbSegP *s = fndbt.GetSegment(0);
    fnd.Copy(*s);
    return(1);
  }
  else return(0);
}


int EdbRunTracking::FindPrediction( EdbSegP &spred, EdbSegP &fnd, EdbSegP &snewpred )
{
  // Select the best (micro or base) track matching with the prediction
  // and prepare for a new search
  // Input:
  //   spred - track prediction
  // Output:
  //   fnd - found track
  //   snewpred - fnd modified in order to be suitable as a prediction for a new search:
  //       - if a basetrack is found, it contains fnd with flag equal to 0
  //       - if a microtrack is found, it contains slopes from the prediction and positions 
  //         from an extrapolation of the found microtrack. The flag is updated as explained below
  //       - if nothing is found, it contains the prediction. The flag is updated as explained below
  // Return:
  //   -1: no track found
  //    0: basetrack found
  //    1: microtrack top found
  //    2: microtrack bottom found

  EdbPattern fndbt,fnds1,fnds2;
  FindCandidates( spred, fndbt, fnds1, fnds2 );

  if ( FindCandidateBT(fndbt,fnd) > 0 ) {
      snewpred.Copy(fnd);
      snewpred.SetFlag(0);                              // if bt found : bth=0, mth=0, tb=0
      Log(1,"FindPrediction","Found a basetrack among %d basetrack candidates\n",fndbt.N());
      return(0);
  }

  float zmean;

  switch ( FindCandidateMT(fnds1,fnds2,fnd) ) {

  case 0:
    snewpred.Copy(spred);
    snewpred.SetFlag(UpdateFlag(spred.Flag(),-1));         // hole: if not found: bth++, mth++, tb= keep last value
    Log(1,"FindPrediction","Found nothing among %d basetrack and %d+%d microtrack candidates\n",fndbt.N(),fnds1.N(),fnds2.N());
    return(-1);
    
  case 1:
    snewpred.Copy(spred);
    zmean = spred.Z();
    snewpred.SetX( fnd.X() + spred.TX()*(zmean-fnd.Z()) );
    snewpred.SetY( fnd.Y() + spred.TY()*(zmean-fnd.Z()) );
    snewpred.SetZ(zmean);
    snewpred.SetFlag(UpdateFlag(spred.Flag(),1));          // if mt found : bth++, mth=0, tb=1
    Log(1,"FindPrediction","Found a microtrack in top side among and %d+%d microtrack candidates\n",fnds1.N(),fnds2.N());
    return(1);
    
  case 2:
    snewpred.Copy(spred);
    zmean = spred.Z();
    snewpred.SetX( fnd.X() + spred.TX()*(zmean-fnd.Z()) );
    snewpred.SetY( fnd.Y() + spred.TY()*(zmean-fnd.Z()) );
    snewpred.SetZ(zmean);
    snewpred.SetFlag(UpdateFlag(spred.Flag(),2));         // if mt found : bth++, mth=0, tb=2
    Log(1,"FindPrediction","Found a microtrack in bottom side among and %d+%d microtrack candidates\n",fnds1.N(),fnds2.N());
    return(2);

  }

  return(-1);
}
