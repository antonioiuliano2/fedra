//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVRec                                                             //
//                                                                      //
// patterns volume reconstruction                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbScanSet.h"
#include "EdbPlateAlignment.h"

#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "vt++/VtDistance.hh"
#include "smatrix/Functions.hh"
#include "smatrix/SVector.hh"

//ClassImp(EdbSegCouple)
ClassImp(EdbPatCouple)
ClassImp(EdbPVRec)

 int EdbSegCouple::egSortFlag=0;
// ///______________________________________________________________________________
// EdbSegCouple::EdbSegCouple() 
// { 
//   Set0();
// }
// 
// ///______________________________________________________________________________
// EdbSegCouple::~EdbSegCouple() 
// { 
//   SafeDelete(eS);
// }
// 
// ///______________________________________________________________________________
// void EdbSegCouple::Set0() 
// { 
//   eID1=0;
//   eID2=0;
//   eN1=0;
//   eN2=0;
//   eN1tot=0;
//   eN2tot=0;
//   eCHI2=0;
//   eCHI2P=0;
//   eS=0;
// }
// 
// ///______________________________________________________________________________
// void EdbSegCouple::SetSortFlag(int s) 
// { 
//   egSortFlag=s; 
// }
// 
// ///______________________________________________________________________________
// void EdbSegCouple::Print()
// {
//   printf("%f \t %f \t %d \t%d  \t\t %d \t %d \n",CHI2(),CHI2P(),ID1(),N1(),ID2(),N2() );
// }
// 
// ///______________________________________________________________________________
// int EdbSegCouple::Compare( const TObject *obj ) const
// {
//   const EdbSegCouple *seg = (EdbSegCouple *)obj;
// 
//   double f1=0, f2=0;
// 
//   if(SortFlag()==0) {
//     f1=CHI2P();
//     f2=seg->CHI2P();
//   } else {
//     f1 =      N1() +      N2()*100000  +      CHI2P()/1000.;
//     f2 = seg->N1() + seg->N2()*100000  + seg->CHI2P()/1000.;
//   }
// 
//   if (f1>f2)
//     return 1;
//   else if (f1<f2)
//     return -1;
//   else
//     return 0;
//  
// }

///==============================================================================
EdbPatCouple::EdbPatCouple()
{
  eSegCouples = new TObjArray(1000);
  eAff=0;
  eCond=0;
  ePat1=0;
  ePat2=0;
  eCHI2mode=0;
}

///______________________________________________________________________________
EdbPatCouple::~EdbPatCouple()
{
  if(eSegCouples) {
    eSegCouples->Delete();
    SafeDelete(eSegCouples);
  }
}

///______________________________________________________________________________
EdbSegCouple  *EdbPatCouple::AddSegCouple( int id1, int id2 )
{
  if(!eSegCouples)   eSegCouples = new TObjArray(1000);
  EdbSegCouple *sc = new EdbSegCouple(id1,id2);
  eSegCouples->Add(sc);
  return sc;
}

///______________________________________________________________________________
void EdbPatCouple::PrintCouples()
{
  EdbSegCouple *sc=0;
  int ncp = Ncouples();
  for(int i=0; i<ncp; i++) {
    sc = GetSegCouple(i);
    sc->Print();
  }
}

//______________________________________________________________________________
void EdbPatCouple::CalculateAffXYZ( float z, int flag )
{
  // calculate affine transformation for SELECTED couples at given z
  // if flag==0 (default) - permit patterns deformation

  if(!eAff) eAff = new EdbAffine2D();
  else eAff->Reset();

  EdbSegCouple *scp=0;
  EdbSegP *s1, *s2;
  int   ncp=Ncouples();
  
  TArrayF x1(ncp);
  TArrayF y1(ncp);
  TArrayF x2(ncp);
  TArrayF y2(ncp);

  Float_t dz1,dz2;

  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);
    s1 = Pat1()->GetSegment(scp->ID1());
    s2 = Pat2()->GetSegment(scp->ID2());
    dz1 = z - s1->Z();
    dz2 = z - s2->Z();
    x1[i] = s1->X() + dz1*s1->TX();
    y1[i] = s1->Y() + dz1*s1->TY();
    x2[i] = s2->X() + dz2*s2->TX();
    y2[i] = s2->Y() + dz2*s2->TY();
  }
  eAff->Calculate( ncp,x1.fArray,y1.fArray,x2.fArray,y2.fArray, flag);
}

//______________________________________________________________________________
void EdbPatCouple::CalculateAffXY(int flag)
{
  // calculate affine transformation for SELECTED couples at Z in the center
  float z = (Pat2()->Z()-Pat1()->Z())/2.;
  CalculateAffXYZ(z,flag);
  Pat1()->Transform(GetAff());
  GetAff()->Reset();
}

//______________________________________________________________________________
int EdbPatCouple::FindOffset0(float xmax, float ymax)
{
  Long_t vdiff[4]={0,0,0,0};
  SetOffsetsMax(xmax,ymax);
  return FindOffset( Pat1(),Pat2(),vdiff);
}

//______________________________________________________________________________
int EdbPatCouple::FindOffset01(float xmax, float ymax)
{
  Long_t vdiff[4]={0,0,1,1};
  SetOffsetsMax(xmax,ymax);
  return FindOffset( Pat1(),Pat2(),vdiff);
}

//______________________________________________________________________________
int EdbPatCouple::FindOffset1(float xmax, float ymax)
{
  Long_t vdiff[4]={1,1,1,1};
  SetOffsetsMax(xmax,ymax);
  return FindOffset( Pat1(),Pat2(),vdiff);
}

//______________________________________________________________________________
int EdbPatCouple::FindOffset( EdbPattern *pat1, EdbPattern *pat2, Long_t vdiff[4] )
{
  float dz=pat2->Z()-pat1->Z();

  float  voff[2]={0,0};
  float stepx = Cond()->StepX(dz/2);
  float stepy = Cond()->StepY(dz/2);
  int      nx = (int)( eXoffsetMax/stepx );
  int      ny = (int)( eYoffsetMax/stepy );

  if(nx==0&&ny==0) return 2;
  Log(2,"EdbPatCouple::FindOffset","( %d x %d ) with steps %8.3f %8.3f \t%f %f",
	 2*nx+1,2*ny+1,stepx,stepy,Cond()->StepTX(0),Cond()->StepTY(0) );

  FillCell_XYaXaY(pat1,Cond(),dz/2.,stepx,stepy);
  Log(3,"EdbPatCouple::FindOffset"," drop couples: %d",pat1->Cell()->DropCouples(4) );
  FillCell_XYaXaY(pat2,Cond(),-dz/2.,stepx,stepy);
  Log(3,"EdbPatCouple::FindOffset"," drop couples: %d",pat2->Cell()->DropCouples(4) );

  Long_t vshift[4] = {0,0,0,0};
  TIndexCell *c1=pat1->Cell();
  TIndexCell *c2=pat2->Cell();

  int npat0 = 0;
  int npat  = 0;
 
  for(int iy=-ny; iy<=ny; iy++ ) {
    for(int ix=-nx; ix<=nx; ix++ ) {
      vshift[0] = ix;
      vshift[1] = iy;
      c1->Shift(2,vshift);
 
      npat = c1->ComparePatterns(4,vdiff,c2);
      if(gEDBDEBUGLEVEL>1) printf("%5d",npat);

      if(npat>npat0) {
	npat0 = npat;
	voff[0] = ix*stepx;
	voff[1] = iy*stepy;
      }
 
      vshift[0] = -ix;
      vshift[1] = -iy;
      c1->Shift(2,vshift);
    }
    if(gEDBDEBUGLEVEL>1) printf("\n");
  }

  Log(2,"EdbPatCouple::FindOffset","Offset: %f %f   npat = %d", voff[0], voff[1], npat0 );

  EdbAffine2D aff;
  aff.ShiftX(voff[0]);
  aff.ShiftY(voff[1]);
  pat1->Transform(&aff);

  return npat0;
}

///______________________________________________________________________________
int EdbPatCouple::FillCHI2P()
{
  //  fast chi2 calculation used for couples selection

  EdbSegCouple *scp=0;
  float       chi2;
  int ncp = Ncouples();

  Log(3,"EdbPatCouple::FillCHI2P","eCHI2mode = %d\n",eCHI2mode);
  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);

    if(eCHI2mode==2) chi2 = Chi2Pz0(scp);
    else if(eCHI2mode==3) chi2 = Chi2KF(scp);
    else                  chi2 = Chi2A(scp);

    scp->SetCHI2P(chi2);
  }
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::FillCHI2()
{
  // final chi2 calculation based on the linked track

  EdbSegCouple *scp=0;
  float       chi2;
  int ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);
    chi2 = Chi2A(scp,0);
    scp->SetCHI2(chi2);
  }
  return Ncouples();
}

///______________________________________________________________________________
float EdbPatCouple::Chi2KF(EdbSegCouple *scp)
{
  // exact estimation of chi2 with full fitting procedure
  // Applications: up/down linking, alignment (offset search)
  //
  // All calculation are done with full corellation matrix for both segments
  // (dependency on the polar angle (phi) is taken into account)

  EdbSegP *s1 = Pat1()->GetSegment(scp->ID1());
  EdbSegP *s2 = Pat2()->GetSegment(scp->ID2());
  s1->SetErrors();
  s2->SetErrors();
  eCond->FillErrorsCov( s1->TX(), s1->TY(), s1->COV() );
  eCond->FillErrorsCov( s2->TX(), s2->TY(), s2->COV() );

  SafeDelete(scp->eS);
  scp->eS=new EdbSegP(*s2);
  float chi2 = EdbTrackFitter::Chi2Seg(scp->eS, s1);
  scp->eS->PropagateToCOV( 0.5*(s1->Z()+s2->Z()) );

  if(s1->EMULDigitArray()) 
    for(int i=0; i<s1->EMULDigitArray()->GetEntries(); i++) scp->eS->addEMULDigit(s1->EMULDigitArray()->At(i));

  return chi2;
}

///______________________________________________________________________________
float EdbPatCouple::Chi2A(EdbSegCouple *scp, int iprob)
{
  EdbSegP *s1 = Pat1()->GetSegment(scp->ID1());
  EdbSegP *s2 = Pat2()->GetSegment(scp->ID2());
  float chi2 = Chi2A(s1,s2, iprob);

  SafeDelete(scp->eS);
  EdbSegP *s = scp->eS=new EdbSegP();
  s->Set( 0,                              // id will be assigned on writing into the tree
	  (s1->X()+s2->X())/2.,
	  (s1->Y()+s2->Y())/2.,
	  (s1->X()-s2->X())/(s1->Z()-s2->Z()),
	  (s1->Y()-s2->Y())/(s1->Z()-s2->Z()),
	  s1->W()+s2->W(),0
	  );
  s->SetZ( (s2->Z()+s1->Z())/2 );
  s->SetChi2( chi2 );
  s->SetDZ( s2->Z()-s1->Z() );
  s->SetVolume( s1->Volume()+s2->Volume() );
  s->SetMC(s1->MCEvt(),s1->MCTrack());

  if(s1->EMULDigitArray()) 
    for(int i=0; i<s1->EMULDigitArray()->GetEntries(); i++) s->addEMULDigit(s1->EMULDigitArray()->At(i));
  if(s2->EMULDigitArray()) 
    for(int i=0; i<s2->EMULDigitArray()->GetEntries(); i++) s->addEMULDigit(s2->EMULDigitArray()->At(i));

  return chi2;
}

///______________________________________________________________________________
float EdbPatCouple::Chi2A( EdbSegP *s1, EdbSegP *s2, int iprob  )
{
  // fast estimation of chi2 in the special case when the position 
  // errors of segments are negligible in respect to angular errors: 
  // sigmaXY/dz << sigmaTXY
  // application: up/down linking, alignment (offset search)
  //
  // All calculation are done in the track plane which remove the 
  // dependency of the polar angle (phi)

  TVector3 v1,v2,v;
  v1.SetXYZ( s1->TX(), s1->TY() , -1. );
  v2.SetXYZ( s2->TX(), s2->TY() , -1. );
  v.SetXYZ(  -( s2->X() - s1->X() ),
	     -( s2->Y() - s1->Y() ),
	     -( s2->Z() - s1->Z() ) );

  float phi = v.Phi();
  v.RotateZ(  -phi );
  v1.RotateZ( -phi );
  v2.RotateZ( -phi );

  float dz  = v.Z();
  float tx  = v.X()/dz;
  float ty  = v.Y()/dz;
  float stx   = eCond->SigmaTX(tx);
  float sty   = eCond->SigmaTY(ty);

  float prob1=1., prob2=1.;
  if(iprob) {
    prob1 = eCond->ProbSeg(tx,ty,s1->W());
    prob2 = eCond->ProbSeg(tx,ty,s2->W());
  }
  float dtx1 = (v1.X()-tx)*(v1.X()-tx)/stx/stx/prob1;
  float dty1 = (v1.Y()-ty)*(v1.Y()-ty)/sty/sty/prob1;
  float dtx2 = (v2.X()-tx)*(v2.X()-tx)/stx/stx/prob2;
  float dty2 = (v2.Y()-ty)*(v2.Y()-ty)/sty/sty/prob2;

  float chi2a=TMath::Sqrt(dtx1+dty1+dtx2+dty2)/2.;
  return chi2a;
}

///______________________________________________________________________________
float EdbPatCouple::Chi2Pz0(EdbSegCouple *scp)
{
  float sx=.5, sy=.5, sz=3., dz=44.;
  float sa;
  float a1,a2;
  float tx,ty;

  TVector3 v1,v2,v;

  EdbSegP *s1 = Pat1()->GetSegment(scp->ID1());
  EdbSegP *s2 = Pat2()->GetSegment(scp->ID2());

  tx = (s1->TX()+s2->TX())/2.;
  ty = (s1->TY()+s2->TY())/2.;

  v1.SetXYZ( s1->TX()/sx, s1->TY()/sy , 1./sz );
  v2.SetXYZ( s2->TX()/sx, s2->TY()/sy , 1./sz );

  v = v1+v2;
  v *= .5;

  a1 = v.Angle(v1);
  a2 = v.Angle(v2);

  sa = sz/dz*TMath::Cos(v.Theta());

  float chi2 =  TMath::Sqrt( (a1*a1 + a2*a2)/2. ) / 
    eCond->ProbSeg( tx, ty, (s1->W()+s2->W())/2. ) / sa;

  SafeDelete(scp->eS);
  EdbSegP *s = scp->eS=new EdbSegP();
  s->Set( 0,                              // id will be assigned on writing into the tree
	  (s1->X()+s2->X())/2.,
	  (s1->Y()+s2->Y())/2.,
	  tx,
	  ty,
	  s1->W()+s2->W(),0
	  );
  s->SetZ( (s2->Z()+s1->Z())/2 );
  s->SetDZ( (s2->DZ()+s1->DZ())/2. );
  s->SetChi2( chi2 );
  s->SetVolume( s1->Volume()+s2->Volume() );
  s->SetMC(s1->MCEvt(),s1->MCTrack());

  return chi2;
}

///______________________________________________________________________________
int EdbPatCouple::CutCHI2P(float chi2max)
{
  TObjArray *sCouples = new TObjArray();
  EdbSegCouple *sc = 0;
  int ncp = Ncouples();

  if(gEDBDEBUGLEVEL>1) printf("CutCHI2P (%4.1f):  %d -> ", chi2max,ncp );
 
  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if(sc->CHI2P()<=chi2max)      sCouples->Add(sc);
    else SafeDelete(sc);
  }
  SafeDelete(eSegCouples);
  eSegCouples=sCouples;
  if(gEDBDEBUGLEVEL>1) printf("%d\n", Ncouples() );
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::SelectIsolated()
{
  TObjArray *sCouples = new TObjArray();
  EdbSegCouple *sc = 0;
  int ncp = Ncouples();
  if(gEDBDEBUGLEVEL>1) printf("SelectIsolated:  %d -> ", ncp );

  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if( sc->N1tot()>1 || sc->N2tot()>1 ) {SafeDelete(sc);}
    else sCouples->Add(sc);
  }
  SafeDelete(eSegCouples);
  eSegCouples=sCouples;
  if(gEDBDEBUGLEVEL>1) printf(" %d \n", Ncouples() );
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::SortByCHI2P()
{
  int   npat=0;

  EdbSegCouple::SetSortFlag(0);    // sort by CHI2P
  eSegCouples->UnSort();
  eSegCouples->Sort();

  int np1 = ePat1->N();
  int np2 = ePat2->N();
  TArrayI found1(np1);
  TArrayI found2(np2);
  int i;
  for(i=0; i<np1; i++) found1[i]=0;
  for(i=0; i<np2; i++) found2[i]=0;

  EdbSegCouple *sc = 0;

  int ncp = Ncouples();
  for( i=0; i<ncp; i++ ) {
    sc = GetSegCouple(i);
    found1[sc->ID1()]++;
    found2[sc->ID2()]++;
    sc->SetN1( found1[sc->ID1()] );
    sc->SetN2( found2[sc->ID2()] );
    npat++;
  }

  ncp = Ncouples();
  for( i=0; i<ncp; i++ ) {
    sc = GetSegCouple(i);
    sc->SetN1tot( found1[sc->ID1()] );
    sc->SetN2tot( found2[sc->ID2()] );
  }

  EdbSegCouple::SetSortFlag(1);      // sort by (numbers + CHI2P)
  eSegCouples->UnSort();
  eSegCouples->Sort();

  return npat;
}

///______________________________________________________________________________
int EdbPatCouple::Align(int alignFlag)

{
  int npat =0;

  SetZlink( (Pat1()->Z()+Pat2()->Z())/2. );  // link at central z

  Log(2,"/nEdbPatCouple::Align","patterns: %d (%d)  and  %d (%d) at Zlink = %f",
	 Pat1()->ID(), Pat1()->N(), Pat2()->ID(),Pat2()->N(), Zlink() );

  npat = FindOffset01(eXoffsetMax,eYoffsetMax);   if(npat<2) return npat;

  Long_t vdiff[4]={0,0,0,0};
  int nitr=5;

  FillCell_XYaXaY(Cond(), Zlink(), 2 );    Pat2()->Cell()->DropCouples(4);

  for(int i=0; i<nitr; i++) {
    FillCell_XYaXaY(Cond(), Zlink(), 1 );   Pat1()->Cell()->DropCouples(4);
    npat = DiffPat(Pat1(),Pat2(),vdiff);

    CalculateAffXYZ(Zlink(), alignFlag);

    Pat1()->Transform(GetAff());
    if( Pat1()->DiffAff(GetAff()) < Cond()->SigmaX(0)/20. ) break;  // stop iterations
  }

  vdiff[0]=vdiff[1]=vdiff[2]=vdiff[3]=1;

  npat = DiffPat( Pat1(), Pat2(), vdiff );
  FillCHI2P();
  SortByCHI2P();
  CutCHI2P(1.5);
  SelectIsolated();
  npat = Ncouples();                     // the selected couples for the final transformation
  CalculateAffXYZ(Zlink(),alignFlag);
  Pat1()->Transform(GetAff());
  Pat1()->SetNAff(npat);
  EdbAffine2D affA;
  affA.Set( GetAff()->A11(), GetAff()->A12(), GetAff()->A21(), GetAff()->A22(),0,0 );

  //psel1->CalculateAXAY(psel2,affA);
  Pat1()->TransformA(&affA);
  GetAff()->Reset();


  //psel2->CalculateAXAY(psel1,affA);
  //pat2->TransformA(affA);

  return npat;
}

///______________________________________________________________________________
int EdbPatCouple::LinkSlow( float chi2max )
{
  // to link segments of already aligned patterns
  // slow - because check all couples

  int npat =0;
  EdbPattern const *pat1=Pat1();   if(!pat1) return npat;
  EdbPattern const *pat2=Pat2();   if(!pat2) return npat;

  ClearSegCouples();

  Log(2,"EdbPatCouple::LinkSlow","patterns: %d (%d)  and  %d (%d) \n",
	   pat1->ID(), pat1->N(), pat2->ID(),pat2->N());

  EdbSegCouple   *c;
  float chi2;

  int n1,n2;
  n1 = pat1->N();
  for( int i1=0; i1<n1; i1++ ) {
    n2 = pat2->N();
    for( int i2=0; i2<n2; i2++ ) {
      chi2 = Chi2A( pat1->GetSegment(i1), pat2->GetSegment(i2) );
      if( chi2 > chi2max )        continue;
      c=AddSegCouple(i1,i2);
      c->SetCHI2P(chi2);
      npat++;
    }
  }
  return npat;
}

///______________________________________________________________________________
int EdbPatCouple::CheckSegmentsDuplication(EdbPattern *pat)
{
  TIndexCell   *cell=pat->Cell();
  int n1 = cell->GetEntries();
  TIndexCell   *c1,*c2,*c3,*c4;
  EdbSegP *s1=0,*s2=0;
  for( int i1=0; i1<n1; i1++) {
    c1 = cell->At(i1);
    int n2 = c1->GetEntries();
    for( int i2=0; i2<n2; i2++) {
      c2 = c1->At(i2);
      int n3 = c2->GetEntries();
      for( int i3=0; i3<n3; i3++) {
	c3 = c2->At(i3);
	int n4 = c3->GetEntries();
	for( int i4=0; i4<n4; i4++) {
	  c4 = c3->At(i4);
	  int N=c4->N();
	  if(N>1) {
	    for(int j1=0; j1<N-1; j1++) {
	      s1=pat->GetSegment(c4->At(j1)->Value());
	      for(int j2=j1+1; j2<N; j2++) {
		s2=pat->GetSegment(c4->At(j2)->Value());
		
		if(gDIFF) gDIFF->Fill( s1->X(),s1->Y(),s1->TX(),s1->TY(),s1->W(),
				       s2->X(),s2->Y(),s2->TX(),s2->TY(),s2->W(),
				       s1->Z(),
				       s1->Aid(0), s1->Aid(1), s2->Aid(0), s2->Aid(1)
				       );
		if( s1->Flag()<0 ) continue;
		if( s2->Flag()<0 ) continue;
		if( s1->Aid(0) == s2->Aid(0)&& s1->Aid(1) == s2->Aid(1) && s1->Side()==s2->Side() ) continue;
		if( TMath::Abs(s2->X()-s1->X()) > 3.) continue;
		if( TMath::Abs(s2->Y()-s1->Y()) > 3.) continue;
		s2->W()>s1->W() ? s1->SetFlag(-1):s2->SetFlag(-1);
	      }
	    }
	  }
	}
      }
    }
  }
  return 0;
}

///______________________________________________________________________________
int EdbPatCouple::LinkFast()
{
  // to link segments of already aligned patterns
  // fast - because check couples by cells

  if(Pat1()->N()<1) return 0;
  if(Pat2()->N()<1) return 0;
  int npat =0;
  SetZlink( (Pat2()->Z()+Pat1()->Z())/2. );

  Log(2,"EdbPatCouple::LinkFast"," %3d (%7d)  and  %3d (%7d) at Z = %13.3f",
	   Pat1()->ID(), Pat1()->N(), Pat2()->ID(),Pat2()->N(), Zlink() );

  FillCell_XYaXaY(Cond(), Zlink() );

  //CheckSegmentsDuplication(Pat1());
  //CheckSegmentsDuplication(Pat2());

  Long_t vdiff[4]={1,1,1,1};

  npat = DiffPat( Pat1(), Pat2(), vdiff );

  FillCHI2P();
  return npat;
}

//______________________________________________________________________________
int EdbPatCouple::DiffPat( EdbPattern *pat1, EdbPattern *pat2, 
			   Long_t vdiff[4] )
{
  if(!pat1) return 0;
  if(!pat2) return 0;
  TIndexCell   *c1=pat1->Cell();
  TIndexCell   *c2=pat2->Cell();
  int ncouples = 0; 

  ncouples = DiffPatCell(c1,c2,vdiff);

  return ncouples;
}

//______________________________________________________________________________
int EdbPatCouple::DiffPatCell( TIndexCell *cel1, TIndexCell *cel2, 
			       Long_t vdiff[4] )
{
  // Input: 2 cells as "x:y:ax:ay:entry" where entry is local id 
  //        vdiff - vector of differences
  //
  // Output: 1 patterns as: "entry1:entry2" 
  //
  // Action: all entries from both patterns satisfied to current vdiff for 
  //         each cell are selected. Entries could be taken more than once

  // TODO: move this algorithm to TIndexCell (n-dim)

  int ncouples = 0;
  int npat = 0;
  ClearSegCouples();

  TIndexCell   *c1[4],*c2[4];

  Long_t v[2]={0,0};
  Long_t val0[6]={0,0,0,0,0,0};
  Long_t val[5]={0,0,0,0,0};
  int    vind[5]={-1,-1,-1,-1,-1};
  
  //EdbSegP *s1,*s2;

  int ncel1, nc10, nc11, nc12, nc13, nc23; 

  ncel1 = cel1->GetEntries();
  for( vind[0]=0; vind[0]<ncel1; vind[0]++) {                              //x1
    c1[0] = cel1->At(vind[0]);
    val0[0] = c1[0]->Value();
    for( val[0] = val0[0]-vdiff[0]; val[0]<=val0[0]+vdiff[0]; val[0]++ ) {              //x2
      c2[0] = cel2->Find(val[0]);
      if(!c2[0])                      continue;

      nc10 = c1[0]->GetEntries();
      for( vind[1]=0; vind[1]<nc10; vind[1]++) {                         //y1
	c1[1] = c1[0]->At(vind[1]);
	val0[1] = c1[1]->Value();
	for( val[1] = val0[1]-vdiff[1]; val[1]<=val0[1]+vdiff[1]; val[1]++ ) {          //y2
	  c2[1] = c2[0]->Find(val[1]);
	  if(!c2[1])                 continue;

	  nc11 = c1[1]->GetEntries();
	  for( vind[2]=0; vind[2]<nc11; vind[2]++) {                     //ax1
	    c1[2] = c1[1]->At(vind[2]);
	    val0[2] = c1[2]->Value();
	    for( val[2] = val0[2]-vdiff[2]; val[2]<=val0[2]+vdiff[2]; val[2]++ ) {      //ax2
	      c2[2] = c2[1]->Find(val[2]);
	      if(!c2[2])                      continue;
	      
	      nc12 = c1[2]->GetEntries();
	      for( vind[3]=0; vind[3]<nc12; vind[3]++) {                 //ay1
		c1[3] = c1[2]->At(vind[3]);
		val0[3] = c1[3]->Value();
		for( val[3] = val0[3]-vdiff[3]; val[3]<=val0[3]+vdiff[3]; val[3]++ ) {  //ay2
		  c2[3] = c2[2]->Find(val[3]);
		  if(!c2[3])                 continue;

		  npat++;

		  nc13 = c1[3]->GetEntries();
		  for(int ie1=0; ie1<nc13; ie1++) {
		    nc23 = c2[3]->GetEntries();
		    for(int ie2=0; ie2<nc23; ie2++) {

		      ncouples++;

		      v[0] = c1[3]->At(ie1)->Value();
		      v[1] = c2[3]->At(ie2)->Value();

//  		      s1 = Pat1()->GetSegment(v[0]);
//  		      s2 = Pat2()->GetSegment(v[1]);
//  		      if(IsCompatible(s1,s2,eCond)        //TODO

		      AddSegCouple((int)v[0],(int)v[1]);

		    }
		  }

		}
	      }
	    }
	  }
	}
      }
    }
  }

  return npat;
}

//______________________________________________________________________________
void EdbPatCouple::FillCell_XYaXaY( EdbScanCond *cond, float zlink, int id )
{
  // fill cells  according to Sigma(angle) functions at z=zlink

  float dz1 = zlink - Pat1()->Z();
  float dz2 = zlink - Pat2()->Z();
  float dz  = TMath::Max( TMath::Abs(dz1), TMath::Abs(dz2) );
  float stepx = cond->StepX(dz);
  float stepy = cond->StepY(dz);

  if(id==0||id==1) FillCell_XYaXaY( Pat1(), cond, dz1, stepx, stepy );
  if(id==0||id==2) FillCell_XYaXaY( Pat2(), cond, dz2, stepx, stepy );
}

//______________________________________________________________________________
void EdbPatCouple::FillCell_XYaXaY( EdbPattern *pat, EdbScanCond *cond, float dz, 
				    float stepx, float stepy )
{
  // fill cells  according to Sigma(angle) functions
  // here cells size do not depends on dz!

  TIndexCell *cell = pat->Cell();
  if(cell) cell->Drop();
  pat->SetStep(stepx,stepy,0,0);

  float x,y,tx,ty;
  Long_t  val[5];  // x,y,ax,ay,i
  EdbSegP *p;
  int npat = pat->N();
  for(int i=0; i<npat; i++ ) {
    p = pat->GetSegment(i);
    if( p->Track() >-1 )      continue;       //to check side effects!!!
    tx = p->TX();
    ty = p->TY();
    x  = p->X() + tx*dz;
    y  = p->Y() + ty*dz;
    val[0]= (Long_t)(x / stepx  );
    val[1]= (Long_t)(y / stepy  );
    val[2]= (Long_t)(tx/ cond->StepTX(tx) );
    val[3]= (Long_t)(ty/ cond->StepTY(ty) );
    val[4]= (Long_t)(i);
    cell->Add(5,val);
  }
  cell->Sort();
  //cell->PrintStat();
  //cell->SetName("x:y:tx:ty:entry");
}

///=============================================================================
EdbPVRec::EdbPVRec()
{
  ePatCouples = new TObjArray();
  eTracks = 0;
  eVTX    = 0;
  eScanCond = 0;
  eChi2Max=999.;
  eVdiff[0]=eVdiff[1]=eVdiff[2]=eVdiff[3]=0;
  eTracksCell = new TIndexCell();
  gROOT->GetListOfSpecials()->Add(this);
}

///______________________________________________________________________________
EdbPVRec::~EdbPVRec()
{
  
  if(ePatCouples) {
    ePatCouples->Delete();
    SafeDelete(ePatCouples);
  }
  if(eTracks) {
    eTracks->Delete();
    SafeDelete(eTracks);
  }
//  if(eVTX)    {
//    eVTX->Delete();
//    SafeDelete(eVTX);
//  }
  SafeDelete(eTracksCell);
  if (gROOT->GetListOfSpecials()->FindObject(this))
    {
      gROOT->GetListOfSpecials()->Remove(this);
    }
}

///______________________________________________________________________________
EdbTrackP* EdbPVRec::FindTrack(int id) const
{
  for(int i=0; i<Ntracks(); i++) 
    if(GetTrack(i)->ID()==id) return GetTrack(i);
  return 0;
}

///______________________________________________________________________________
EdbSegP* EdbPVRec::FindSegment(int PlateID, int SegmentID) const
{

  for(int i=0; i<Npatterns(); ++i) {
		EdbPattern* pat = GetPattern(i);
		if (pat->PID() != PlateID) continue;
		for(int j=0; j<pat->N(); ++j) {
			EdbSegP* seg = pat->GetSegment(j);
			if (seg->ID() == SegmentID) return seg;
		}
	}
  return 0;
}

///______________________________________________________________________________
EdbPatCouple *EdbPVRec::AddCouple(int id1, int id2)
{
  EdbPatCouple *c = new EdbPatCouple();
  c->SetID(id1,id2);
  ePatCouples->Add(c);
  return c;
}

//______________________________________________________________________________
void EdbPVRec::SetOffsetsMax(float ox, float oy)
{
  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    GetCouple(i)->SetOffsetsMax(ox,oy);
  }
}

//______________________________________________________________________________
void EdbPVRec::SetCouples()
{
  // using already setted ID's of couples attach the patterns 

  EdbPatCouple *pc = 0;
  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->SetPat1( GetPattern(pc->ID1()) );
    pc->SetPat2( GetPattern(pc->ID2()) );

    pc->SetCHI2mode(GetScanCond()->Chi2Mode());
  }
}

//______________________________________________________________________________
void EdbPVRec::ResetCouples()
{
  EdbPatCouple *pc = 0;
  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->ClearSegCouples();
    pc->SetPat1( GetPattern(pc->ID1()) );
    pc->SetPat2( GetPattern(pc->ID2()) );
  }
}

//______________________________________________________________________________
void EdbPVRec::ResetTracks()
{
  for(int i=0; i<Npatterns(); i++ ) {
    EdbPattern *p =  GetPattern(i);
    if(!p) continue;
    for(int j=0; j<p->N(); j++ ) p->GetSegment(j)->SetTrack(-1);
  }
  SafeDelete(eTracks);
  SafeDelete(eTracksCell);
}

//______________________________________________________________________________
void EdbPVRec::SetCouplesAll()
{
  // form couples array for all available patterns ID's
  SetCouplesPeriodic(0,1);
}

//______________________________________________________________________________
void EdbPVRec::SetCouplesPeriodic(int istart, int iperiod)
{
  // istart:   start from pattern
  // iperiod:  distance between patterns

  DeleteCouples();
  EdbPatCouple *pc = 0;
  int npat=Npatterns();

  if( istart<0 || istart>=npat-iperiod ) return;

  int ifirst=0; 
  for(int i=0; i<npat; i++ ) {if(GetPattern(i)) ifirst=i; break;}
  for(int i=ifirst; i<npat; i++ ) 
    if(GetPattern(i)) GetPattern(i)->SetSegmentsPID();
  if(ifirst<istart) istart=ifirst;
  for(int i=istart; i<npat-iperiod; i+=iperiod ) {
    pc = new EdbPatCouple();
    pc->SetID(i,i+iperiod);
    pc->SetCond(eScanCond);
    pc->SetOffset(0,0,0,0);
    pc->SetSigma(1,1,.003,.003);       //TODO: organise this sigmas
    AddCouple(pc);
  }
  SetCouples();
}

//______________________________________________________________________________
void EdbPVRec::SetCouplesExclude(TArrayI &exclude)
{
  // by Alessandra Pastore
  // TArray exclude[npat]={0,1,1,0,...,0} where
  //           0 = plate to be excluded from the volume pattern; 
  //           1 = plate to be included into the volume pattern

  DeleteCouples();
  EdbPatCouple *pc = 0;
  int npat = Npatterns();

  for(int i=0; i<npat; i++ ) GetPattern(i)->SetSegmentsPID();
  for(int i=0; i<npat-1; i++) {
    if(exclude[i]==0)    continue;
    for(int j=i+1; j<npat; j++) {
      if(exclude[j]==0) continue;

      pc = new EdbPatCouple();
      pc->SetID(i,j);
      pc->SetCond(eScanCond);
      pc->SetOffset(0,0,0,0);
      pc->SetSigma(1,1,.003,.003);
      AddCouple(pc);
      break;
    }
  }

  SetCouples();
}

//______________________________________________________________________________
void EdbPVRec::FillCell(float stepx, float stepy, float steptx, float stepty)
{
  int npat=Npatterns();
  for(int i=0; i<npat; i++ ) GetPattern(i)->FillCell(stepx,stepy,steptx,stepty);
}

//______________________________________________________________________________
void EdbPVRec::SetSegmentErrors( EdbSegP &seg )
{
  // segment errors are depends on the scanning conditions and segment
  // parameters (angles). 
  // Puls height is represents segments probability (not accuracy) and should 
  // be taken into account separately

  float sx  = GetScanCond()->SigmaX( seg.TX() );
  float sy  = GetScanCond()->SigmaY( seg.TY() );
  float sz  = GetScanCond()->SigmaZ( seg.TX(), seg.TY() );
  float stx = GetScanCond()->SigmaTX( seg.TX() );
  float sty = GetScanCond()->SigmaTY( seg.TY() );

  seg.SetErrors(sx*sx,sy*sy,sz*sz,stx*stx,sty*sty);
}

//______________________________________________________________________________
void EdbPVRec::SetSegmentProbability( EdbSegP &seg )
{
  // segment probability is depends on:
  // puls height
  // signal puls distribution
  // noise  puls distribution
  // angles

  seg.SetProbability( GetScanCond()->ProbSeg(seg.TX(), seg.TY(), seg.W()) );

}

//______________________________________________________________________________
void EdbPVRec::SetSegmentsErrors()
{
  if(!GetScanCond()) {
    Log(1,"EdbPVRec::SetSegmentsErrors","ERROR: ScanCond didn't set!!!");
    return;
  }
  EdbPattern *pat;
  EdbSegP    *seg;
  int npat =Npatterns();
  int nseg=0;
  for(int i=0; i<npat; i++ ) {
    pat = GetPattern(i);
    if(!pat) continue;
    nseg = pat->N();
    for(int j=0; j<nseg; j++ ) {
      seg = pat->GetSegment(j);      
      SetSegmentErrors( *seg );
      SetSegmentProbability( *seg );
    }
  }
}

//______________________________________________________________________________
int EdbPVRec::LinkSlow()
{
  // link tracks in aligned volume
  int npat=0;
  SetCouples();

  EdbPatCouple *pc = 0;

  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->LinkSlow( Chi2Max() );
    npat = pc->SortByCHI2P();
  }
  Log(2,"EdbPVRec::LinkSlow"," npat= %d",npat);
  return npat;
}

//______________________________________________________________________________
int EdbPVRec::Link()
{
  // link tracks in aligned volume
  int npat=0;
  SetCouples();

  EdbPatCouple *pc = 0;

  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->LinkFast();
    pc->CutCHI2P(eChi2Max);
    pc->SortByCHI2P();
    npat += pc->Ncouples();
  }
  Log(3,"EdbPVRec::Link","total number of linked segments = %d",npat);
  return npat;
}

//______________________________________________________________________________
int EdbPVRec::Align(int alignFlag)
{
  if(alignFlag<10)  return AlignOld(alignFlag);
  else {
    EdbScanSet sci, sca;
    for(int i=0; i<Npatterns(); i++) {
      EdbPlateP *pl = new EdbPlateP();
      EdbPattern *p = GetPattern(i);
      pl->SetID( p->PID() );
      pl->SetZlayer( p->Z(), p->Z(), p->Z() );
      sci.eB.AddPlate(pl);
    }
    sci.MakePIDList();
    sca.Copy(sci);
    return AlignPlates( sci, sca );
  }
}

//______________________________________________________________________________
int EdbPVRec::AlignPlates( EdbScanSet &sc, EdbScanSet &sca, const char *reportdir)
{
  int npat = Npatterns();
  if(npat<2) return 0;
  for(int i=0; i<npat-1; i++) {
    EdbPattern *p1 = GetPattern(i);
    EdbPattern *p2 = GetPattern(i+1);
    EdbPlateP *plate1 = sc.GetPlate(p1->PID());
    EdbPlateP *plate2 = sc.GetPlate(p2->PID());
    Log(1,"AlignPlates","\n align %d -> %d", p1->PID(), p2->PID() );
    EdbPlateP *pc = new EdbPlateP(); // "couple" to be used for the alignment
    pc->GetLayer(1)->SetID(plate1->ID());
    pc->GetLayer(2)->SetID(plate2->ID());
    float DZ = plate2->Z()-plate1->Z();

    gROOT->SetBatch();
    EdbPlateAlignment av;

    if(reportdir)
      av.InitOutputFile( Form("%6d/%2.2d_%2.2d.al.root",sc.eB.ID(),p1->PID(),p2->PID()) );

    av.Align( *p1, *p2 , DZ );
    p1->SetNAff( av.eNcoins );

    if(reportdir)
      av.CloseOutputFile();

    if( av.eStatus ) {
      pc->GetLayer(1)->CopyCorr( av.eCorrL[0] );
      pc->GetLayer(2)->CopyCorr( av.eCorrL[1] );
      pc->GetLayer(1)->SetZlayer( -(pc->GetLayer(1)->Zcorr()), 0, 0);
    } else pc->GetLayer(1)->SetZlayer( -DZ, 0, 0 );

    sca.ePC.Add(pc);
  }
  sca.eB.SetID(sca.eB.ID());
  sca.AssembleBrickFromPC();
  sca.SetAsReferencePlate( sc.eB.GetPlate(npat-1)->ID() );   // last plate is the reference one

  for(int i=0; i<npat; i++) {
    EdbPattern *p = GetPattern(i);
    p->Transform( sca.eB.GetPlate(i)->GetAffineXY() );
    p->SetZ( sca.eB.GetPlate(i)->Z() );
    p->SetSegmentsZ();
    p->SetSegmentsDZ(300);
  }

  return npat;
}

//______________________________________________________________________________
int EdbPVRec::AlignOld(int alignFlag)
{
  // align patterns in volume
  int npat=Npatterns();

  TObjArray aKeep(Npatterns());  
  EdbAffine2D *a0;

  int i;
  for(i=0; i<npat; i++ ) {
    a0 = new EdbAffine2D();
    GetPattern(i)->GetKeep(*a0);
    aKeep[i]=a0;
  }


  SetCouples();
  EdbPatCouple *pc = 0;
  int ncp = Ncouples();
  for(i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->Align(alignFlag);
  }


  EdbAffine2D a;
  npat = Npatterns();
  for(i=npat-1; i>0; i-- ) {
    GetPattern(i)->GetKeep(a);
    a0=(EdbAffine2D *)(aKeep.At(i));
    a0->Invert();
    a0->Transform(&a);
    GetPattern(i-1)->Transform(a0);
  }

  return npat;
}

//______________________________________________________________________________
void EdbPVRec::FillTracksCell()
{
  // TODO: speed-up this algorithm

  // fill tracks cell "vid1:vid2"
  // second segment is considered as leading one

  Log(2,"\nEdbPVRec::FillTracksCell","...");

  Long_t vid1,vid2;

  if(!eTracksCell) eTracksCell = new TIndexCell();
  TIndexCell *tracksCell = eTracksCell;  // "vid1:vid2"
  TIndexCell *cc=0;

  if(tracksCell) tracksCell->Drop();

  EdbPatCouple *pc = 0;
  EdbSegCouple *sc = 0;

  int ncp = Ncouples();
  int ncpp;
  for(int iv=0; iv<ncp; iv++ ) {
    pc = GetCouple(iv);
    ncpp = pc->Ncouples();
    for(int ip=0; ip<ncpp; ip++) {
      sc = pc->GetSegCouple(ip);

      vid1 = Vid( pc->ID1(),sc->ID1() );
      vid2 = Vid( pc->ID2(),sc->ID2() );

      cc=tracksCell->FindAdd(vid1);
      if(!cc->N(1)) cc->Add(vid1);
      cc->Add(vid2);
      cc->SetValue(vid2);
    }
  }
  tracksCell->Sort();
  if(gEDBDEBUGLEVEL>1) tracksCell->PrintPopulation(1);
}

//______________________________________________________________________________
void EdbPVRec::FillTracksCellFast()
{
  // fill tracks cell "vid1:vid2"
  // second segment is considered as leading one

  int ncp = Ncouples();
  Log(2,"EdbPVRec::FillTracksCellFast","build tracks from couples for ncp=%d ...", ncp);
  if(ncp<1)     return;

  SafeDelete(eTracks);
  eTracks  = new TObjArray();

  EdbPatCouple *pc = 0;
  TIndex2 itracks(0);

  for( int iv=0; iv<ncp; iv++ ) {
    pc = GetCouple(iv);
    AddCouplesToTracksM( pc , itracks );
  }


  //TODO: split&analyse tracks

  int nsegmax=100, nfound=0;
  TArrayI segtab(nsegmax);
  segtab.Reset();
  int nseg=0;
  int ntr = eTracks->GetEntries();
  for( int i=0; i<ntr; i++ ) {
    nseg=((EdbTrackP*)(eTracks->At(i)))->N();
    if(nseg>nsegmax-1) nseg=nsegmax;
    if(nseg>nfound) nfound=nseg;
    segtab[nseg] =  segtab[nseg]+1;
  }

  for(int i=0; i<nfound+1; i++) 
    Log(2,"EdbPVRec::FillTracksCellFast","%d \t tracks with  %d segments",segtab[i],i);

  Log(2,"EdbPVRec::FillTracksCellFast","%d tracks are found", ntr);
}

//_________________________________________________________________________
void EdbPVRec::AddCouplesToTracks(EdbPatCouple *pc, TIndex2 &itracks )
{
  EdbTrackP  *track = 0;

  EdbSegCouple *sc = 0;
  int ncpp = pc->Ncouples();

  Double_t *w    = new Double_t[ncpp];

  int ind=-1;
  int nind=itracks.GetSize();

  for(int ip=0; ip<ncpp; ip++) {
    sc = pc->GetSegCouple(ip);
    track=0; 

    if(nind)    ind = itracks.FindIndex( sc->ID1() );
    else        ind = -1;

    if(ind>-1)  track = (EdbTrackP*)(eTracks->At(ind));
    if(!track)  {
      track = new EdbTrackP();
      track->AddSegment( GetPattern(pc->ID1())->GetSegment(sc->ID1()) );
      AddTrack(track);
      ind = eTracks->GetLast();
    }
    track->AddSegment( GetPattern(pc->ID2())->GetSegment(sc->ID2()) );

    w[ip] = TIndex2::BuildValue( sc->ID2(), ind );
  }
  itracks.Set(0);
  itracks.BuildIndex(ncpp,w);
  delete[] w;
}

//_________________________________________________________________________
void EdbPVRec::AddCouplesToTracksM(EdbPatCouple *pc, TIndex2 &itracks )
{
  // merge segments into clusters

  EdbTrackP  *track = 0;

  EdbSegCouple *sc = 0;
  int ncpp = pc->Ncouples();

  Double_t *w    = new Double_t[ncpp];

  int ind=-1;
  int nind=itracks.GetSize();

  //make starting tracks: I10
  if(nind==0) {
    for(int ip=0;  ip<ncpp; ip++) {
      sc = pc->GetSegCouple(ip);
      w[ip] = TIndex2::BuildValue( sc->ID1(), ip );
    }
    itracks.Set(0);
    itracks.BuildIndex(ncpp,w);

    int maj0=-10, maj=0;
    for(int i=0; i<ncpp; i++) {
      maj = itracks.Major(i);
      if( maj0<maj ) {
	track = new EdbTrackP();
	AddTrack( track );
	track->SetID(eTracks->GetLast());
	maj0=maj;
      }
      itracks.SetMinor( i, eTracks->GetLast() );
    }
  }

  // make I12

  for(int ip=0; ip<ncpp; ip++) {
    sc = pc->GetSegCouple(ip);
    track=0; 

    ind = itracks.FindIndex( sc->ID1() );

    if(ind>-1)  track = (EdbTrackP*)(eTracks->At(ind));
    if(!track)  {
      track = new EdbTrackP();
      track->AddSegment( GetPattern(pc->ID1())->GetSegment(sc->ID1()) );
      AddTrack(track);
      ind = eTracks->GetLast();
      track->SetID(ind);
    }
    track->AddSegment( GetPattern(pc->ID2())->GetSegment(sc->ID2()) );

    w[ip] = TIndex2::BuildValue( sc->ID2(), ind );
  }
  itracks.Set(0);
  itracks.BuildIndex(ncpp,w);
  delete[] w;

  //merge tracks

  int maj0=-10, maj=0, id0=-1, id=-1;
  for(int i=0; i<ncpp; i++) {
    maj = itracks.Major(i);
    id  = itracks.Minor(i);
    track = (EdbTrackP*)(eTracks->At(id));
    if( maj0!=maj ) id0=id;
    else {
      track->SetID(id0);
      itracks.SetMinor(i, id0 );
    }
    maj0=maj;
  }
}

//______________________________________________________________________________
void EdbPVRec::FillTracksCell1()
{
  // TODO: speed-up this algorithm

  // fill tracks cell "vid1:vid2"
  // second segment is considered as leading one
  Long_t vid1,vid2;

  TIndexCell *tracks = eTracksCell;  // "vid1:vid2"
  TIndexCell *cc=0;
  TIndexCell *cv1=0;
  TIndexCell *cv2=0;

  THashList  *cross = new THashList();
  //  TIndexCell *cross = new TIndexCell();

  if(tracks) tracks->Drop();

  EdbPatCouple *pc = 0;
  EdbSegCouple *sc = 0;

  int ncp = Ncouples();
  int ncpp;
  for(int iv=0; iv<ncp; iv++ ) {
    pc = GetCouple(iv);
    ncpp = pc->Ncouples();
    Log(2,"EdbPVRec::FillTracksCell1","cross: %d %d\n",iv,ncpp);
    for(int ip=0; ip<ncpp; ip++) {
      sc = pc->GetSegCouple(ip);

      vid1 = Vid( pc->ID1(),sc->ID1() );
      vid2 = Vid( pc->ID2(),sc->ID2() );
      cv1 = new TIndexCell(vid1);
      cv2 = new TIndexCell(vid2);
      if( !(cc=(TIndexCell*)cross->FindObject(cv1)) )  { cross->Add(cv1); cc=cv1; }
      cc->FindAdd(vid2);
      if( !(cc=(TIndexCell*)cross->FindObject(cv2)) )  { cross->Add(cv2); cc=cv2; }
      cc->FindAdd(vid1);
    }
  }

  TIndexCell *ct=0;
  int ncross = cross->GetSize();
  Log(2,"EdbPVRec::FillTracksCell1","ncross = %d",ncross);
  int ncc=0;

  for(int i=0; i<ncross; i++) {
    cc = (TIndexCell*)(cross->At(i));
    ncc = cc->N(1);
    if(!ncc)     continue;

    ct = tracks->FindAdd(cc->Value());
    ct->FindAdd(cc->Value());

    CollectSegment1(ct,cross);
  }

  tracks->Sort();
  tracks->PrintPopulation(1);
}

//______________________________________________________________________________
void EdbPVRec::FillTracksCell2()
{
  // TODO: speed-up this algorithm

  // fill tracks cell "vid1:vid2"
  // second segment is considered as leading one
  Long_t vid1,vid2;

  TIndexCell *tracks = eTracksCell;  // "vid1:vid2"
  TIndexCell *cc=0;
  TIndexCell *cross = new TIndexCell();

  if(tracks) tracks->Drop();

  EdbPatCouple *pc = 0;
  EdbSegCouple *sc = 0;

  int ncp = Ncouples();
  int ncpp;
  for(int iv=0; iv<ncp; iv++ ) {
    pc = GetCouple(iv);
    ncpp = pc->Ncouples();
    Log(2,"EdbPVRec::FillTracksCell2","cross: %d %d\n",iv,ncpp);
    for(int ip=0; ip<ncpp; ip++) {
      sc = pc->GetSegCouple(ip);

      vid1 = Vid( pc->ID1(),sc->ID1() );
      vid2 = Vid( pc->ID2(),sc->ID2() );

      cc=cross->FindAdd(vid1);
      //      cc->FindAdd(vid1);
      cc->FindAdd(vid2);

      cc=cross->FindAdd(vid2);
      cc->FindAdd(vid1);
      //      cc->FindAdd(vid2);

    }
  }
  cross->Sort();
  if(gEDBDEBUGLEVEL>1) cross->PrintPopulation(1);

  TIndexCell *ct=0;
  int ncross = cross->N(1);
  Log(2,"EdbPVRec::FillTracksCell2","ncross = %d",ncross);
  int ncc=0;

  for(int i=0; i<ncross; i++) {
    cc = cross->At(i);
    ncc = cc->N(1); 
    if(!ncc)     continue;

    ct = tracks->FindAdd(cc->Value());
    ct->FindAdd(cc->Value());

    CollectSegment(ct,cross);
  }

  tracks->Sort();
  if(gEDBDEBUGLEVEL>1) tracks->PrintPopulation(1);
}

//______________________________________________________________________________
int EdbPVRec::CollectSegment( TIndexCell *ct, TIndexCell *cross)
{
  TIndexCell *cc =0;
  int ncc =0;
  int nct = ct->N(1);
  int flag=0;
  for(int j=0; j<nct; j++) {
    cc  =  cross->Find( ct->At(j)->Value() );
    ncc = cc->N(1);
    if(!ncc) continue;
    flag++;
    for(int icc=0; icc<ncc; icc++) {
      ct->FindAdd(cc->At(icc)->Value());
    }
    cc->List()->Delete();
    if(!CollectSegment(ct,cross)) return 0;
  }
  return flag;
}

//______________________________________________________________________________
int EdbPVRec::CollectSegment1( TIndexCell *ct, THashList *cross)
{
  TIndexCell *cc =0;
  int ncc =0;
  int nct = ct->N(1);
  int flag=0;
  for(int j=0; j<nct; j++) {
    cc  =  (TIndexCell*)(cross->FindObject( ct->At(j) ));
    ncc = cc->N(1);
    if(!ncc) continue;
    flag++;
    for(int icc=0; icc<ncc; icc++) {
      ct->FindAdd(cc->At(icc)->Value());
    }
    cc->List()->Delete();
    if(!CollectSegment1(ct,cross)) return 0;
  }
  return flag;
}

//______________________________________________________________________________
int EdbPVRec::MakeHoles(int ort)
{
  // holes attached only from the one side corresponding to ort
  // only tracks with nseg>= abs(ort) are extrapolated

  TIndexCell *ct;
  Long_t     vn=0,v0=0,v1=0,v2=0;
  int        nholes=0;

  int id=0,pid=0;
  EdbSegP   *s1=0, *s2=0;
  int n=0;

  int ntc=eTracksCell->N(1);
  for(int it=0; it<ntc; it++) {

    ct = eTracksCell->At(it);
    n = ct->N(1);
    if( n >= Npatterns() )       continue;  // too many   segments
    if( n < TMath::Abs(ort) )    continue;  // not enough segments
    vn = ct->At(n-1)->Value();
    v0 = ct->At(0)->Value();
    if( Pid(vn)-Pid(v0) < n-1 )  continue;  // track is not isolated

    if(ort<0)    {              // attach at the beginnning of the track
      if( Pid(v0) < 1 )                continue;
      v2=v0;
      v1 = ct->At(1)->Value();
      pid =  Pid(v0)-1;
    } else if(ort>0) {          // attach at the end of the track
      if( Pid(vn) > Npatterns()-2 )    continue;
      v2=vn;
      v1 = ct->At(n-2)->Value();
      pid =  Pid(vn)+1;
    }

    s1 = GetPattern( Pid(v1) )->GetSegment( Sid(v1) );
    s2 = GetPattern( Pid(v2) )->GetSegment( Sid(v2) );

    id = InsertHole( s1,s2, pid); 
    ct->Add( Vid( pid, id) );
    nholes++;
  }
  return nholes;
}

//______________________________________________________________________________
int EdbPVRec::InsertHole( const EdbSegP *s1, const EdbSegP *s2, int pid )
{
  EdbSegP s;
  EdbSegP::LinkMT(s1,s2,&s);
  s.SetFlag(-1);
  s.SetW( (s1->W()+s2->W())/2 );
  EdbPattern *pat = GetPattern(pid);
  //s.SetProbability( pat->Cond()->ProbSeg( s.TX(),s.TY(),s.W() );
  s.PropagateTo(pat->Z());
  s.SetDZ(s1->DZ());
  pat->AddSegment(s);
  return pat->N()-1;
}

//______________________________________________________________________________
int EdbPVRec::MergeTracks(int maxgap)
{
  int merged=0,m1=0;

  for(int i=0; i<10; i++) {
    m1 = MergeTracks1(maxgap);
    Log(2,"EdbPVRec::MergeTracks","%d tracks are merged!",  m1);
    if(m1==0)  break;
    merged += m1;
  }

  Log(2,"EdbPVRec::MergeTracks","Total: %d tracks are merged!",  merged);

  return merged;
}

//______________________________________________________________________________
int EdbPVRec::MergeTracks1(int maxgap)
{
  int imerged=0;

  /*
  float X0 = GetScanCond()->RadX0();

  int npat = Npatterns();

  TIndexCell starts,ends;              // "ist:entry"   "iend:entry"
  FillTracksStartEnd( starts, ends, 1, npat-2 );

  TIndexCell *ce=0, *cs=0;
  int         itre,itrs, iend;
  EdbTrackP  *tre, *trs;
  EdbSegP    *s1, *s2;

  float tx,ty,dz,dx,dy;
  float stx=0.015, sty=0.015, sx=6., sy=6.;

  for(int ie=0; ie<ends.GetEntries(); ie++)   {
    ce = ends.At(ie);
    iend = ce->Value();

    for(int igap=0; igap<maxgap+1; igap++) {
      cs = starts.Find(iend+igap);
      if(!cs) continue;

      for(int iee=0; iee<ce->GetEntries(); iee++) {
        itre = ce->At(iee)->Value();
        tre  = (EdbTrackP*)((*eTracks)[itre]);
        if(!tre)             continue;
        if(tre->Flag()==-10) continue;
        s1 = tre->GetSegmentF(tre->N()-1);

        for(int iss=0; iss<cs->GetEntries(); iss++) {
          itrs = cs->At(iss)->Value();
          trs  = (EdbTrackP*)((*eTracks)[itrs]);
          if(!trs)             continue;
          if(trs->Flag()==-10) continue;
          s2 = trs->GetSegmentF(0);

          if( TMath::Abs(s2->TX()-s1->TX()) > stx )                 continue;
          if( TMath::Abs(s2->TY()-s1->TY()) > sty )                 continue;
          dz = s2->Z()-s1->Z();
          tx = (s2->TX() + s1->TX())/2.;
          dx =  (s2->X() - tx*dz/2.) - (s1->X() + tx*dz/2.);
          if(TMath::Abs(dx) > sx+stx*TMath::Abs(dz)/2. )            continue;
          ty = (s2->TY() + s1->TY())/2.;
          dy =  (s2->Y() - ty*dz/2.) - (s1->Y() + ty*dz/2.);
          if( TMath::Abs(dy)> sy+sty*TMath::Abs(dz)/2. )            continue;

          //if(tre->N()+trs->N()>npat)
          printf("%f \t%f %f \t%f %f %d  %d \n",
                 dz , dx,dy,
                 s2->TX()-s1->TX(), s2->TY()-s1->TY(),
                 tre->N(),trs->N() );

          printf( "prob = %15.13f\n", EdbVertexRec::ProbeSeg(s1,s2, 5810., tre->M()) );

          tre->AddTrack(*trs);
          tre->FitTrackKFS(true,X0);
          tre->SetFlag(-10);
          tre->SetN0( tre->N0() + trs->N0() + igap-1 );
          tre->SetNpl( tre->Npl() + trs->Npl() + igap-1 );

          (*eTracks)[itrs]=0;    delete trs; trs = 0;

          imerged++;
          break;

        }
      }
    }
  }

  eTracks->Compress();

  */
  return imerged;
}

//______________________________________________________________________________
float EdbPVRec::Chi2Fast(EdbSegP &s1, EdbSegP &s2)
{
  float kSZ = 1., kSX=.5, kSY=.5; // pattern to pattern errors: TODO

  float tx0,ty0, stx2,sty2; //mean angle
  float w1,w2;
  float tx, ty;
  float dz = s2.Z()-s1.Z();
  float chi2=0;

  w1  = 1./s1.SX();  w2  = 1./s2.SX();
  tx0 = (w1*s1.TX() + w2*s2.TX())/(w1+w2);
  w1 += w2;
  if( TMath::Abs(dz)>kSZ ) {
    tx   = (s2.X()-s1.X())/dz;
    w2 = 1./((s1.SX() + s2.SX() + kSX*kSX)/dz/dz);
    tx0 = (tx0*w1 + tx*w2)/(w1+w2);
    w1 += w2;
  }
  stx2 = 1./w1;

  w1  = 1./s1.SY();  w2  = 1./s2.SY();
  ty0 = (w1*s1.TY() + w2*s2.TY())/(w1+w2);
  w1 += w2;
  if( TMath::Abs(dz)>kSZ ) {  
    ty   = (s2.Y()-s1.Y())/dz;
    w2 = 1./((s1.SY() + s2.SY() + kSY*kSY)/dz/dz);
    ty0 = (ty0*w1 + ty*w2)/(w1+w2);
    w1 += w2;
  }
  sty2 = 1./w1;

  float dx =  (s2.X() - tx0*dz/2.) - (s1.X() + tx0*dz/2.);
  float dy =  (s2.Y() - ty0*dz/2.) - (s1.Y() + ty0*dz/2.);
  float sx2 = s1.SX()+s2.SX() + stx2*dz*dz;
  float sy2 = s1.SY()+s2.SY() + sty2*dz*dz;

  chi2  = (s2.TX()-s1.TX())*(s2.TX()-s1.TX())/(s1.SX()+s2.SX());
  chi2 += (s2.TY()-s1.TY())*(s2.TY()-s1.TY())/(s1.SY()+s2.SY());
  chi2 += dx*dx/sx2 + dy*dy/sy2;

  chi2 /=4.;
  if(chi2<100) printf("%f \t%f %f \t%f %f \tchi2=%f\n",
		     dz , dx,dy, s2.TX()-s1.TX(), s2.TY()-s1.TY(),chi2 );
  return chi2;
}

//______________________________________________________________________________
void EdbPVRec::FitTracks(float p, float mass, TObjArray *gener, int design)
{
  // measurement errors: TODO
  // TODO: move gener logic out from EdbPVRec; i voobshe polnyi bardak v etoi funkzii

  float X0 =  GetScanCond()->RadX0();
  float pms = 0.;
  int nsegmatch = 0;

  int ntr = eTracks->GetEntries();

  if (p > 0. && mass > 0.)
	printf("fit %d tracks assuming p = %f, mass = %f and X0 = %f ...\n",
	 ntr,p,mass,X0);
  else if ( p <  0. && mass > 0.)
	printf("fit %d tracks assuming MC momentum, mass = %f and X0 = %f ...\n",
	 ntr,mass,X0);
  else if ( p == 0. && mass > 0.)
	printf("fit %d tracks with MS momentum, mass = %f and X0 = %f ...\n",
	 ntr,mass,X0);
  else if ( p > 0. && mass < 0.)
	printf("fit %d tracks assuming p = %f, MC mass and X0 = %f ...\n",
	 ntr,p,X0);
  else if ( p <  0. && mass < 0.)
	printf("fit %d tracks assuming MC momentum, MC mass and X0 = %f ...\n",
	 ntr,X0);
  else if ( p == 0. && mass < 0.)
	printf("fit %d tracks with MS momentum, MC mass and X0 = %f ...\n",
	 ntr,X0);
  else if ( p > 0. && mass == 0.)
	printf("fit %d tracks assuming p = %f, pre-defined mass and X0 = %f ...\n",
	 ntr,p,X0);
  else if ( p <  0. && mass == 0.)
	printf("fit %d tracks assuming MC momentum, pre-defined mass and X0 = %f ...\n",
	 ntr,X0);
  else if ( p == 0. && mass == 0.)
	printf("fit %d tracks with MS momentum, pre-defined mass and X0 = %f ...\n",
	 ntr,X0);


  EdbMomentumEstimator tf;
  tf.eX0 = X0;
  tf.eM  = mass;
  EdbTrackP *tr = 0, *trg = 0;
  int nseg, itrg;

  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(eTracks->At(itr));

    tr->ClearF();

    nseg = tr->N();

    if(p>0)    tr->SetP(p);
    else if(p<0 && gener)
    {
	if ((itrg = tr->GetSegmentsMCTrack(nsegmatch)) >= 0)
	{
	    trg = (EdbTrackP*)(gener->At(itrg));
	    if (trg)
	    {
    		tr->SetP(trg->P());
	    }
	}
    }
    else if (p == 0.)
    {
	if (tr->P() == 0.) 
	{
	    pms = tf.P_MS(*tr);
	    if      (pms < 0.05) pms = 0.05;
	    else if (pms > 30.0) pms = 30.;
	    tr->SetP(pms);
	}
    }
    else tr->SetP(4.);

    if(mass>0) tr->SetM(mass);
    else if(mass<0 && gener)
    {
	if ((itrg = tr->GetSegmentsMCTrack(nsegmatch)) >= 0)
	{
	    trg = (EdbTrackP*)(gener->At(itrg));
	    if (trg)
	    {
    		tr->SetM(trg->M());
	    }
	}
    }
    else if (tr->M() == 0.) tr->SetM(0.139);
    tr->FitTrackKFS(false,X0,design);
  }

}

//______________________________________________________________________________
int EdbPVRec::MakeTracks(int nsegments, int flag)
{
  // extract from index_table tracks longer then nsegments
  // and form tracks array
  // assign the flag to the newly created tracks
  // return the number of created tracks

  int ntr0 = 0;
  if( eTracks ) ntr0 = eTracks->GetEntries();
  else   eTracks  = new TObjArray();

  Log(2,"\nEdbPVRec::MakeTracks","ntr0=%d...",ntr0);

  int         nseg, ntr=0;
  Long_t      vid=0;
  EdbSegP    *seg=0;
  EdbTrackP  *track = 0;
  TIndexCell *ct=0;

  int ntc=eTracksCell->GetEntries();
  for(int it=0; it<ntc; it++) {

    ct = eTracksCell->At(it);
    nseg = ct->GetEntries();
    if( nseg < nsegments )             continue;
    track = new EdbTrackP(nseg);

    track->SetNpl( Pid(ct->At(nseg-1)->Value()) - Pid(ct->At(0)->Value()) +1 );

    for(int is=0; is<nseg; is++) {
      vid = ct->At(is)->Value();
      seg = GetSegment(vid);
      track->AddSegment(seg);
    }
    eTracks->Add(track);
    ntr++;
  }

  for(int i=ntr0; i<ntr0+ntr; i++) {
    track = GetTrack(i);
    track->SetID(i);
    track->SetFlag(flag);
    track->SetCounters();
  }

  Log(2,"EdbPVRec::MakeTracks","%d tracks with >= %d segments are selected",ntr, nsegments);
  return ntr;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrF(int ipat, EdbAffine2D &aff,  EdbAffine2D &afft )
{
  if(!eTracks) return 0;

  float probMax=.01; // TODO

  int   ntr = eTracks->GetEntriesFast();
  TArrayF x(ntr) , y(ntr), x1(ntr), y1(ntr);
  TArrayF tx(ntr),ty(ntr),tx1(ntr),ty1(ntr);

  int itr=0;
  int nseg=0;
  EdbTrackP *track=0;
  EdbSegP   *s=0, *sf=0;

  for(int i=0; i<ntr; i++) {

    track = (EdbTrackP*)eTracks->At(i);

    Log(2,"EdbPVRec::FineCorrF","track->CHI2() = %f %f   prob = %f",
	   track->CHI2(),track->CHI2F(),track->Prob() );
    if(track->Prob()<probMax) continue;

    sf=0;
    nseg = track->N();
    for(int j=0; j<nseg; j++) {
      s = track->GetSegment(j);
      if( s->PID()==ipat ) {
	sf = track->GetSegmentF(j);
	break;
      }
    }

    if(!sf)  continue;

    x1[itr]  = s->X();
    y1[itr]  = s->Y();
    tx1[itr] = s->TX();
    ty1[itr] = s->TY();
    x[itr]   = sf->X();
    y[itr]   = sf->Y();
    tx[itr]  = sf->TX();
    ty[itr]  = sf->TY();
    itr++;
  }

  aff.CalculateTurn( itr, x1.GetArray(),y1.GetArray(),x.GetArray(),y.GetArray() );
  GetPattern(ipat)->Transform(&aff);

  afft.CalculateTurn( itr,tx1.GetArray(),ty1.GetArray(),tx.GetArray(),ty.GetArray() );
  GetPattern(ipat)->TransformA(&afft);
  return itr;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrXY(int ipat, EdbAffine2D &aff, int flag)
{
  if(!eTracks) return 0;
  int   ntr = eTracks->GetEntriesFast();
  float *x = new float[ntr];
  float *y = new float[ntr];
  float *x1 = new float[ntr];
  float *y1 = new float[ntr];
  int   itr=0;
  EdbTrackP *track=0;
  EdbSegP   *seg=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    if(track->CHI2()>1.) continue;
    seg = track->GetSegment(ipat);
    x1[itr] = seg->X();
    y1[itr] = seg->Y();
    x[itr]  = track->X() + track->TX()*(seg->Z() - track->Z());
    y[itr]  = track->Y() + track->TY()*(seg->Z() - track->Z());
    itr++;
  }
  aff.Calculate( itr,x1,y1,x,y,flag );
  GetPattern(ipat)->Transform(&aff);
  delete[] x;
  delete[] y;
  delete[] x1;
  delete[] y1;
  return itr;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrTXTY(int ipat, EdbAffine2D &aff)
{
  if(!eTracks) return 0;
  int   ntr = eTracks->GetEntriesFast();
  float *tx  = new float[ntr];
  float *ty  = new float[ntr];
  float *tx1 = new float[ntr];
  float *ty1 = new float[ntr];
  int   itr=0;
  EdbTrackP *track=0;
  EdbSegP   *seg=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    if(track->CHI2()>1.) continue;
    seg = track->GetSegment(ipat);
    tx1[itr] = seg->TX();
    ty1[itr] = seg->TY();
    tx[itr]  = track->TX();
    ty[itr]  = track->TY();
    itr++;
  }
  aff.CalculateTurn( itr,tx1,ty1,tx,ty );
  GetPattern(ipat)->TransformA(&aff);
  delete[] tx;
  delete[] ty;
  delete[] tx1;
  delete[] ty1;
  return itr;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrZ(int ipat, float &dz)
{
  if(!eTracks) return 0;
  int   ntr = eTracks->GetEntriesFast();
  float  tx1,ty1;
  double t1;
  double t;
  double dzz=0;
  int   itr=0;
  EdbTrackP *track=0;
  EdbSegP   *seg1=0;
  EdbSegP   *seg2=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    if(track->CHI2()>1.2) continue;
    t  = TMath::Sqrt( track->TX()*track->TX() + track->TY()*track->TY() );
    if(t<.1)             continue;
    if(t>.5)             continue;
    seg1 = track->GetSegment(ipat);
    seg2 = track->GetSegment(ipat+1);
    tx1 = (seg2->X()-seg1->X())/(seg2->Z()-seg1->Z());
    ty1 = (seg2->Y()-seg1->Y())/(seg2->Z()-seg1->Z());
    t1 = TMath::Sqrt(tx1*tx1+ty1*ty1);
    dzz += t1/t;
    itr++;
  }
  dzz /= itr;    // dzz is the ratio of "position angle"/"track angle"
  dz = (GetPattern(ipat+1)->Z()-GetPattern(ipat)->Z())*dzz;

  return itr;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrZnew()
{
  // calculate z-correction for all patterns based on linked tracks of any length
  // gaps to be ignored...

  int  nMin = 5; // minimal statistics for doing correction
  if(!eTracks) return 0;
  int   ntr  = eTracks->GetEntriesFast();

  if(ntr < nMin) { Log(1,"EdbPVRec::FineCorrZnew"," ntr<%d : nothing to do!", nMin ); return 0; }

  int   npat = Npatterns();
  float  tx1,ty1;
  double t1;
  double t;
  TArrayD dzz(npat);
  TArrayI itr(npat);
  TArrayF znew(npat);

  EdbTrackP *track=0;
  EdbSegP   *seg1=0;
  EdbSegP   *seg2=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    if(track->CHI2()>1.2) continue;
    t  = TMath::Sqrt( track->TX()*track->TX() + track->TY()*track->TY() );
    if(t<.1)              continue;
    if(t>.5)              continue;

    int nseg = track->N();
    for(int j=0; j<nseg-1; j++ )      {
      seg1 = track->GetSegment(j);
      seg2 = track->GetSegment(j+1);
      if(TMath::Abs(seg1->PID()-seg2->PID()) != 1)       continue;     // gap skipped
      int ipat = TMath::Min( seg1->PID(), seg2->PID() );
      tx1 = (seg2->X()-seg1->X())/(seg2->Z()-seg1->Z());
      ty1 = (seg2->Y()-seg1->Y())/(seg2->Z()-seg1->Z());
      t1 = TMath::Sqrt(tx1*tx1+ty1*ty1);
      dzz[ipat] = dzz[ipat] + t1/t;
      itr[ipat] = itr[ipat]+1;
    }
  }

  znew[npat-1] = GetPattern(npat-1)->Z();
  for(int i=npat-2; i>=0; i-- )  {
    if(itr[i]<nMin) dzz[i] = 1;        // 
    else  dzz[i] = dzz[i]/itr[i];      // dzz is the ratio of "position angle"/"track angle"
    float dz = (GetPattern(i)->Z()-GetPattern(i+1)->Z())*dzz[i];
    znew[i] = znew[i+1] + dz;
    Log(2,"EdbPVRec::FineCorrZnew","Ajust Z of pat: %3d  by %4d tracks: dz =%7.3f  Zold, Znew: %12.3f %12.3f   diff = %7.3f", 
	   i, itr[i], dz, GetPattern(i)->Z(), znew[i], znew[i]-GetPattern(i)->Z() );
  }

  for(int i=0; i<npat-1; i++ )  {
    GetPattern(i)->SetZ(znew[i]);
    GetPattern(i)->SetSegmentsZ();
  }

  return 0;
}

//______________________________________________________________________________
int EdbPVRec::FineCorrShr(int ipat, float &shr)
{
  if(!eTracks) return 0;
  int   ntr = eTracks->GetEntriesFast();
  double t1;
  double t;
  double dzz=0;
  int   itr=0;
  EdbTrackP *track=0;
  EdbSegP   *seg=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    if(track->CHI2()>1.2) continue;
    t  = TMath::Sqrt( track->TX()*track->TX() + track->TY()*track->TY() );
    if(t<.1)             continue;
    if(t>.45)             continue;
    seg = track->GetSegment(ipat);
    t1 = TMath::Sqrt( seg->TX()* seg->TX() +  seg->TY()* seg->TY() );
    dzz += t1/t;
    itr++;
  }
  dzz /= itr;    // dzz is the ratio of "segment angle"/"track angle"
  shr = dzz;

  return itr;
}


//______________________________________________________________________________
int EdbPVRec::MakeSummaryTracks()
{
  if(!eTracks) return 0;
  int ntr = eTracks->GetEntriesFast();
  EdbTrackP *track=0;
  for(int i=0; i<ntr; i++) {
    track = (EdbTrackP*)eTracks->At(i);
    track->FitTrack();
  }
  return ntr;
}


//______________________________________________________________________________
int EdbPVRec::SelectLongTracks(int nsegments)
{
  int ntr=0;
  if(!eTracksCell) return ntr;
  if(nsegments<2) return ntr;
  SafeDelete(eTracks);
  eTracks = new TObjArray();

  EdbTrackP  *track=0;
  EdbSegP    *seg=0;

  ResetCouples();
  EdbPatCouple *pc=0;
  TIndexCell   *ct=0;
  Long_t vid1=0,vid2=0;
  
  int ntc, nct; 

  ntc = eTracksCell->GetEntries();
  for(int it=0; it<ntc; it++) {

    ct = eTracksCell->At(it);
    if( ct->N() < nsegments )     continue;
    track = new EdbTrackP();
    track->SetID( ct->Value() );

    nct = ct->GetEntries();
    int is;
    for(is=0; is<nct-1; is++) {
      vid1 = ct->At(is)->Value();
      vid2 = ct->At(is+1)->Value();
      pc = GetCouple(Pid(vid1));              //TODO: depends on cp sequence
      pc->AddSegCouple( Sid(vid1), Sid(vid2) );
    }

    for(is=0; is<nct; is++) {
      vid1 = ct->At(is)->Value();
      seg = GetPattern(Pid(vid1))->GetSegment(Sid(vid1));
      //seg->SetPID(Pid(vid1));
      track->AddSegment(seg);
    }

    ntr++;
    eTracks->Add(track);
  }

  Log(2,"EdbPVRec::SelectLongTracks","%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr; 
}

///______________________________________________________________________________
int EdbPVRec::CombTracks( int nplmin, int ngapMax, float probMin )
{
  // eliminate crossing&overlapping tracks with multiple segments usage
  // discard tracks with probability < probMin

  int ntr = eTracks->GetEntries();
  Log(3,"EdbPVRec::CombTracks","Comb %d tracks, longer then %d; ngaps <= %d ..."
	 ,ntr,nplmin, ngapMax );

  int nseg=0, nsegtot=0;

  // *** sort tracks by quality

  TIndexCell cn;  //"npl:prob:entry"
  Long_t v[3];

  EdbTrackP *tr=0;
  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(eTracks->At(i));
    tr->SetID(i);
    tr->SetCounters();
    nsegtot += tr->SetSegmentsTrack(-1);
    v[0]= -(tr->Npl());
    v[1]= (Long_t)((1.-tr->Prob())*100);
    v[2]= i;
    cn.Add(3,v);
  }
  cn.Sort();

  Log(3,"EdbPVRec::CombTracks","%d tracks with %d segments for processing...",ntr,nsegtot);

  // *** set track ID for segments attached to

  TIndexCell *cp=0, *c=0;
  int nn=cn.GetEntries();
  for(int i=nn-1; i>=0; i--) {
    cp = cn.At(i);                              // tracks with fixed npl
    int np = cp->GetEntries();
    for(int ip=np-1; ip>=0; ip--) {
      c = cp->At(ip);                           // tracks with fixed Npl & Prob
      int nt = c->GetEntries();
      for(int it=0; it<nt; it++) {
	tr = (EdbTrackP*)(eTracks->At( c->At(it)->Value() ) );
	tr->SetSegmentsTrack();
      }
    }
  }

  // discard bad tracks with flag -10

  cp=0; 
  c=0;
  nn=cn.GetEntries();
  for(int i=0; i<nn; i++) {
    cp = cn.At(i);                              // tracks with fixed npl

    int np = cp->GetEntries();
    for(int ip=0; ip<np; ip++) {
      c = cp->At(ip);                           // tracks with fixed Npl & Prob

      int nt = c->GetEntries();
      for(int it=0; it<nt; it++) {
	
	tr = (EdbTrackP*)(eTracks->At( c->At(it)->Value() ) );

  	if(tr->RemoveAliasSegments()>0){
  	  if(tr->N()<nplmin)             tr->SetFlag(-10);
  	  if(tr->CheckMaxGap()>ngapMax)  tr->SetFlag(-10);
  	}

      }
    }
  }

  // discard tracks with low probability
//   for(int i=0; i<ntr; i++) {
//     tr = GetTrack(i);
//     if(tr->Prob() < probMin) tr->SetFlag(-10);
//   }

  // release the segments and eliminate bad tracks from the tracks array

  int trind;
  EdbSegP *seg=0;
  for(int i=0; i<ntr; i++) {
    tr = GetTrack(i);
    if(tr->Flag() != -10) continue;
    nseg = tr->N();
    for (int iseg=0; iseg<nseg; iseg++) {
      seg = tr->GetSegment(iseg);
      trind = seg->Track();
      if ( trind  < 0 )   continue;                                // segment is already free
      if ( GetTrack(trind)->Flag() == -10 ) seg->SetTrack(-1);     // release segment
    }
  }

  for(int i=ntr-1; i>-1; i--) {
    tr = GetTrack(i);
    if(tr->Flag() != -10) continue;
    eTracks->RemoveAt(i);
    SafeDelete(tr);
  }
  eTracks->Compress();

  nsegtot = 0;

  ntr = eTracks->GetEntries();
  for(int i=0; i<ntr; i++) {
    tr = GetTrack(i);
    tr->SetID(i);
    nsegtot += tr->SetSegmentsTrack();
  }

  Log(3,"EdbPVRec::CombTracks","%d tracks with %d segments remaining",ntr,nsegtot);
  return ntr;
}

///______________________________________________________________________________
void EdbPVRec::SetSegmentsTracks()
{
  EdbTrackP *tr=0;
  int ntr = Ntracks();
  for(int i=0; i<ntr; i++) {
    tr = GetTrack(i);
    tr->SetID(i);
    if(tr->Flag()<0) continue;
    tr->SetSegmentsTrack();
  }
}

///______________________________________________________________________________
int EdbPVRec::PropagateTracks(int nplmax, int nplmin, float probMin,
			      int ngapMax, int design)
{
  //  extrapolate incomplete tracks and update them with new segments
  //
  //  input: nplmax - the maximal length of the track to be continued
  //  input: nplmin - the minimal length of the track to be continued

//   if (eVTX)
//   {
//     eVTX->Delete();
//     eVTX->Clear();
//   }

  //ClearPropagation(design);

  int ntr = CombTracks(nplmin, ngapMax);  // clean-up from all tracking defects TODO remove this call from here? 

  Log(2,"EdbPVRec::PropagateTracks"," %d tracks, selecting in range [%d : %d] plates, ngaps <= %d ..."
	 ,ntr,nplmin,nplmax, ngapMax );

  if(ntr<1) return 0;

  TIndexCell cn;  //"npl:prob:entry"
  Long_t v[3];
  int nseg=0;
  EdbTrackP *tr=0;
  for(int i=0; i<ntr; i++) {
    tr = GetTrack(i);
    v[0]= -(tr->Npl());
    v[1]= (Long_t)((1.-tr->Prob())*100);
    v[2]= i;
    cn.Add(3,v);
  }
  cn.Sort();

  int nsegTot=0;

  TIndexCell *cp=0, *c=0;
  int nn=cn.GetEntries();

  for(int i=0; i<nn; i++) {
    cp = cn.At(i);                              // tracks with fixed npl
    if( -(cp->Value()) > nplmax )    continue;
    if( -(cp->Value()) < nplmin )    continue;

    int np = cp->GetEntries();
    for(int ip=0; ip<np; ip++) {
      c = cp->At(ip);                           // tracks with fixed Npl & Prob

      int nt = c->GetEntries();
      for(int it=0; it<nt; it++) {
	
	tr = (EdbTrackP*)(eTracks->At( c->At(it)->Value() ) );

	if(!tr) { 
	  Log(1,"EdbPVRec::PropagateTracks","ERROR: bad track(%d) pointer!!",i);
	  continue;
	}

	if(tr->Flag()==-10) continue;

	nseg = PropagateTrack(*tr, true, probMin, ngapMax, design);
  	nsegTot += nseg;

	if(tr->Npl()>nplmax)   continue;
	if(tr->Flag()==-10)    continue;

  	nseg = PropagateTrack(*tr, false, probMin, ngapMax, design);
  	nsegTot += nseg;

      }
    }
  }

  ntr = CombTracks(nplmin, ngapMax);  // clean-up all tracking defects

  Log(2,"EdbPVRec::PropagateTracks","%d segments are attached after propagation",nsegTot);
  return nsegTot;
}

///______________________________________________________________________________
int EdbPVRec::PropagateTrack( EdbTrackP &tr, bool followZ, float probMin,
			      int ngapMax,   int design )
{
  float binx=10, bint=10;
  float X0 = GetScanCond()->RadX0();

  EdbSegP ss; // the "selector" segment
  int step = tr.MakeSelector(ss,followZ);     // step in pid

  int pstart = ss.PID(), pend=0;
  if     (step>0) pend = Npatterns()-1;
  else if(step<0) pend = 0;
  else return 0;

  TObjArray arr;
  EdbSegP *seg=0;
  EdbSegP *segmax=0;

  EdbPattern *pat  = 0;
  int   nseg =0, nsegTot=0;
  int   ntr = eTracks->GetEntriesFast();
  int   ngap =0, trind=0;
  float probmax=0, prob=0;
  EdbTrackP *ttt = 0;

  for(int i=pstart+step; i!=pend+step; i+=step ) {
    pat = GetPattern(i);
    if(!pat)                     goto GAP;
    ss.PropagateTo(pat->Z());
    
    arr.Clear();
    nseg = pat->FindCompliments(ss,arr,binx,bint);
    if(!nseg)                   goto GAP;

    probmax=prob=0;
    segmax=0;
    for(int is=0; is<nseg; is++ ) {
      seg = (EdbSegP*)(arr.At(is));
      prob = ProbeSeg( &tr, seg, X0 );
      if( prob>probmax ) { probmax=prob; segmax=seg; }
    }
    if(!segmax)                  goto GAP;
    if(probmax<probMin)          goto GAP;

    trind= segmax->Track();
    if(trind==tr.ID()) {
      Log(1,"EdbPVRec::PropagateTrack","TRACK LOOP: %d %d",trind, tr.ID());
      goto GAP;
    }
    ttt=0;
    if( trind >= 0 && trind<ntr )    {
      ttt = ((EdbTrackP*)eTracks->At(trind));
      if(!ttt)  { Log(1,"EdbPVRec::PropagateTrack","BAD TRACK POINTER: %d\n", trind); goto GAP;};
      
      if(ttt->VertexS() || ttt->VertexE()) goto GAP;

      if(!(tr.VertexS()) && !(tr.VertexE()))
      {
        if(ttt->Flag()>=0) {
	    if( ttt->N() > tr.N() )                                goto GAP;
	    else if( segmax->Z() > (ttt->TrackZmin()->Z()+300.) && 
		     segmax->Z() < (ttt->TrackZmax()->Z()-300.) )  goto GAP; // do not attach in-middle segments
        }
      }
    }

    if( !AttachSeg( tr, segmax , X0, probMin, probmax ))          goto GAP;

    if(ttt) ttt->SetFlag(-10);

    segmax->SetTrack(tr.ID());
    tr.MakeSelector(ss,followZ);
    nsegTot++;
    ngap =0;
    //    tr.SetFlag(tr.Flag()+1);            // to check side effects!

  GAP:
    if(++ngap>ngapMax) break;
  }

  tr.SetNpl();
  tr.SetN0();
  tr.FitTrackKFS(followZ,X0,design);
  tr.SetSegmentsTrack();

  return nsegTot;
}

//________________________________________________________________________
double EdbPVRec::ProbeSeg(const EdbTrackP *tr1, EdbTrackP *tr2, const float X0)
{
  return ProbeSeg( (EdbSegP*)tr1, (EdbSegP*)tr2, X0, tr1->M() );
}

//________________________________________________________________________
double EdbPVRec::ProbeSeg(const EdbTrackP *tr, EdbSegP *s,
				   const float X0)
{
  return ProbeSeg( (EdbSegP*)tr, s, X0, tr->M() );
}


using namespace MATRIX;
using namespace VERTEX;

//________________________________________________________________________
double EdbPVRec::ProbeSeg(const EdbSegP *tr, EdbSegP *s,
			      const float X0, const float ma)
{
  // Return value:        Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept couples with Prob >= ProbMin then ProbMin is the 
  //                      probability to reject the good couple
  //
  // The mass and momentum of the tr are used for multiple scattering estimation

  float  ds;        // distance in the media for MS estimation
  double teta0sq;
  double dz;

  VtVector par( (double)(tr->X()), 
		(double)(tr->Y()),  
		(double)(tr->TX()), 
		(double)(tr->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (tr->COV())(k,l);

  Double_t chi2=0.; 


  VtSymMatrix dms(4);   // multiple scattering matrix (depends on P,m)
  dms.clear();

  dz = s->Z()-tr->Z();
  ds = dz*TMath::Sqrt(1.+par(2)*par(2)+par(3)*par(3)); // thickness of media in microns
  teta0sq = EdbPhysics::ThetaMS2( tr->P(), ma, ds, X0 );

  dms(0,0) = teta0sq*dz*dz/3.;
  dms(1,1) = dms(0,0);
  dms(2,2) = teta0sq;
  dms(3,3) = dms(2,2);
  dms(2,0) = teta0sq*dz/2.;
  dms(3,1) = dms(2,0);
  dms(0,2) = dms(2,0);
  dms(1,3) = dms(2,0);

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();

  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;

  VtVector parpred(4);            // prediction from seg0 to seg
  parpred = pred*par;
  
  VtSymMatrix covpred(4);         // covariance matrix for prediction
  covpred = pred*(cov*pred.T())+dms;

  VtSymMatrix dmeas(4);           // original covariance  matrix for seg2
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) dmeas(k,l) = (s->COV())(k,l);
  
  covpred = covpred.dsinv();
  dmeas   = dmeas.dsinv();
  cov = covpred + dmeas;
  cov = cov.dsinv();
  
  VtVector meas( (double)(s->X()), 
		 (double)(s->Y()),  
		 (double)(s->TX()), 
		 (double)(s->TY()) );

  par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

  chi2 = (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

  return TMath::Prob(chi2,4);
}

//________________________________________________________________________
bool EdbPVRec::AttachSeg( EdbTrackP& tr, EdbSegP *s,
			      const float X0, const float ProbMinP, float &prob )
{
  // Return value:        Prob: is Chi2 probability (area of the tail of Chi2-distribution)
  //                      If we accept couples with Prob >= ProbMinP then ProbMinP is the 
  //                      probability to reject the good couple
  //
  // The mass and momentum of the tr are used for multiple scattering estimation

  float  ds;        // distance in the media for MS estimation
  double teta0sq;
  double dz;

  const EdbSegP *snear=0;
  if( TMath::Abs( s->Z() - tr.TrackZmin()->Z() ) < 
      TMath::Abs( s->Z() - tr.TrackZmax()->Z() ) )  snear = tr.TrackZmin();
  else snear = tr.TrackZmax();

  VtVector par( (double)(snear->X()), 
		(double)(snear->Y()),  
		(double)(snear->TX()), 
		(double)(snear->TY()) );

  VtSymMatrix cov(4);             // covariance matrix for seg0 (measurements errors)
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) cov(k,l) = (snear->COV())(k,l);

  Double_t chi2=0.; 


  VtSymMatrix dms(4);   // multiple scattering matrix (depends on P,m)
  dms.clear();

  dz = s->Z()-snear->Z();
  ds = dz*TMath::Sqrt(1.+par(2)*par(2)+par(3)*par(3)); // thickness of media in microns
  teta0sq = EdbPhysics::ThetaMS2( tr.P(), tr.M(), ds, X0 );

  dms(0,0) = teta0sq*dz*dz/3.;
  dms(1,1) = dms(0,0);
  dms(2,2) = teta0sq;
  dms(3,3) = dms(2,2);
  dms(2,0) = teta0sq*dz/2.;
  dms(3,1) = dms(2,0);
  dms(0,2) = dms(2,0);
  dms(1,3) = dms(2,0);

  VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
  pred.clear();

  pred(0,0) = 1.;
  pred(1,1) = 1.;
  pred(2,2) = 1.;
  pred(3,3) = 1.;
  pred(0,2) = dz;
  pred(1,3) = dz;

  VtVector parpred(4);            // prediction from seg0 to seg
  parpred = pred*par;
  
  VtSymMatrix covpred(4);         // covariance matrix for prediction
  covpred = pred*(cov*pred.T())+dms;

  VtSymMatrix dmeas(4);           // original covariance  matrix for seg2
  for(int k=0; k<4; k++) 
    for(int l=0; l<4; l++) dmeas(k,l) = (s->COV())(k,l);
  
  covpred = covpred.dsinv();
  dmeas   = dmeas.dsinv();
  cov = covpred + dmeas;
  cov = cov.dsinv();
  
  VtVector meas( (double)(s->X()), 
		 (double)(s->Y()),  
		 (double)(s->TX()), 
		 (double)(s->TY()) );

  par = cov*(covpred*parpred + dmeas*meas);   // new parameters for seg

  chi2 = (par-parpred)*(covpred*(par-parpred)) + (par-meas)*(dmeas*(par-meas));

  prob = (float)TMath::Prob(chi2,4);

  if (prob >= ProbMinP)
  {
    EdbSegP *sfnew = new EdbSegP( tr.ID(),
				  (float)par(0),(float)par(1),(float)par(2),(float)par(3),
				  tr.W()+1.,tr.Flag() );

    sfnew->SetCOV( cov.array(), 4 );
    sfnew->SetChi2((float)chi2);
    sfnew->SetProb(prob);
    sfnew->SetZ(s->Z());

    tr.AddSegment(s);
    tr.AddSegmentF(sfnew);

    return true;
  }
  return false;
}

///______________________________________________________________________________
int EdbPVRec::ExtractDataVolumeSeg( EdbTrackP &tr, TObjArray &arr, 
				    float binx, float bint )
{
  int npat = Npatterns();

  EdbSegP ss; // the "selector" segment 

  tr.MakeSelector(ss);

  EdbPattern *pat  = 0;
  int nseg =0;

  for(int i=0; i<npat; i++) {
    pat = GetPattern(i);
    if(!pat)                   continue;
    
    ss.PropagateTo(pat->Z());

    nseg += pat->FindCompliments(ss,arr,binx,bint);
  }
  Log(2,"EdbPVRec::ExtractDataVolumeSeg","%d segments are selected\n",nseg);
  return nseg;
}

///______________________________________________________________________________
int EdbPVRec::ExtractDataVolumeSegAll( TObjArray &arr )
{
  int npat = Npatterns();

  EdbPattern *pat  = 0;
  int nseg =0;

  for(int i=0; i<npat; i++) {
    pat = GetPattern(i);
    if(!pat)                   continue;

    for(int j=0; j<pat->N(); j++) {
      arr.Add(pat->GetSegment(j));
      nseg++;
    }
  }
  Log(2,"EdbPVRec::ExtractDataVolumeSegAll","%d segments are selected\n",nseg);
  return nseg;
}

///______________________________________________________________________________
EdbPattern  *EdbPVRec::GetPatternByPID(int pid)
{
  EdbPattern *p=0;
  for(int i=0; i<Npatterns(); i++) {
    p = GetPattern(i);
    if(p) if(p->PID()==pid) return p;
  }
  return 0;
}

///______________________________________________________________________________
Int_t  EdbPVRec::NSeg()
{
  EdbPattern *p=0;
	Int_t nseg=0;
  for(int i=0; i<Npatterns(); i++) {
    p = GetPattern(i);
    nseg+=p->N();
  }
  return nseg;
}

///______________________________________________________________________________
EdbSegP *EdbPVRec::AddSegment(EdbSegP &s)
{
  // add new segment to this 
  // create and insert new pattern if needed
  // plate, side and Z for the segment should be correctly defined
  // Note: slow function - use for additional segments only as "manual check" etc

  EdbPattern *p  = GetPatternByPlate( s.Plate(), s.Side() );
  if(!p) {
    Log(1,"EdbPVRec::AddSegment",
	"WARNING: The pattern for plate/side %d/%d  does not found, add new one with z = %f ", 
	s.Plate(), s.Side(), s.Z() );
    p = new EdbPattern( 0., 0., s.Z() );
    p->SetID(s.PID());
    p->SetScanID( s.ScanID() );
    p->SetSide( s.Side() );
    InsertPattern(p, eDescendingZ);
  }
  return p->AddSegment(s);
}

///______________________________________________________________________________
int  EdbPVRec::AddSegments(EdbPVRec &ali)
{
  // copy the segments from ali and add them to this
  int nseg=0;
  for(int i=0; i<ali.Npatterns(); i++) {
    EdbPattern *pa = GetPattern(i);
    for(int j=0; j<pa->N(); j++) {
      AddSegment(  *(pa->GetSegment(j)) );
      nseg++;
    }
  }
  Log(2,"EdbPVRec::AddSegments","%d new segments are inserted",nseg);
  return nseg;
}

///______________________________________________________________________________
int  EdbPVRec::AddSegments(EdbTrackP &track)
{
  // copy the segments from track and add them to this
  int nseg=0;
  for(int i=0; i<track.N(); i++) {
    AddSegment(  *(track.GetSegment(i)) );
    nseg++;
  }
  Log(2,"EdbPVRec::AddSegments","%d new segments are inserted for using track %d",nseg, track.ID() );
  return nseg;
}

///______________________________________________________________________________
void  EdbPVRec::SetScanIDPatSeg(EdbID id)
{
  // set eBrick,eMajor and eMinor for all segments of all volume patterns
  // leave ePlate as is
  for(int i=0; i<Npatterns(); i++) {
    EdbPattern *p = GetPattern(i);
    for(int j=0; j<p->N(); j++) {
      EdbSegP *s = p->GetSegment(j);
      id.ePlate = s->Plate();
      s->SetScanID(id);
    }
  }
}

///______________________________________________________________________________
void  EdbPVRec::SetScanIDTrackSeg(EdbID id)
{
  // set eBrick,eMajor and eMinor for all segments of all volume patterns
  // leave ePlate as is
  for(int i=0; i<Ntracks(); i++) {
    EdbTrackP *t = GetTrack(i);
    for(int j=0; j<t->N(); j++) {
      EdbSegP *s = t->GetSegment(j);
      id.ePlate = s->Plate();
      s->SetScanID(id);
    }
  }
}


///______________________________________________________________________________
void  EdbPVRec::PrintSummary()
{
  // Print Summary Information of relevant data of this object.
  // Useful for debugging purposes.
  
  cout <<"EdbPVRec::PrintSummary()   Print Summary Information of relevant data of this object." << endl;
  cout <<"EdbPVRec::PrintSummary()   Useful for debugging purposes:" << endl;
  
  for(int i=0; i<Npatterns(); i++) {
    EdbPattern *p = GetPattern(i);
    cout <<"-------------- Pattern Nr = " << i << " at Z-Position= " << p->Z()  << " with "<< p->N() << " entries. ---- Print First and Last Segment:";
    if (p->N()<1) {
      cout << " Sorry, but no segments in pattern. " << endl;
    }
    else {
      cout << endl;
      EdbSegP *s = p->GetSegment(0);
      s->PrintNice();
      if (p->N()==1) continue;
      s=p->GetSegment(p->N()-1);
      s->PrintNice();
    }
  }

  return;
}

