//-- Author :  Valeri Tioukov   18.03.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVRec                                                             //
//                                                                      //
// patterns volume reconstruction                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "EdbAffine.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbVertex.h"
#include "TBenchmark.h"

ClassImp(EdbScanCond)
ClassImp(EdbSegCouple)
ClassImp(EdbPatCouple)
ClassImp(EdbPVRec)
ClassImp(EdbAlignment)

int EdbSegCouple::egSortFlag=0;


//______________________________________________________________________________
EdbScanCond::EdbScanCond()
{
  SetDefault();
}

//______________________________________________________________________________
void EdbScanCond::SetDefault()
{
  eSigmaXgr=  .1;
  eSigmaYgr=  .1;
  eSigmaZgr= 3.;

  eDegrad = .6;  // means that at .4 mrad degradation is 24 % in respect to 0 angle

  eSigmaX0  = 8.;   // [microns]
  eSigmaY0  = 8.;   // [microns]
  eSigmaTX0 = .008; // [mrad]
  eSigmaTY0 = .008; // [mrad]

  ePuls0[0]     =  8;    // microtrack puls height (grains)
  ePuls0[1]     = 10;    // at 0 angle
  ePuls04[0]    =  5;    // microtrack puls height (grains)
  ePuls04[1]    =  9;    // at 0.4 rad angle

  eChi2Max  = 3.5;
  eChi2PMax = 3.5;
  eOffX=0;
  eOffY=0;

  eRadX0 = EdbPhysics::kX0_Cell;
}

//______________________________________________________________________________
void EdbScanCond::Print() const
{
  printf( "******************************************************\n");
  printf( "Scanning Conditions Parametres: %s\n", GetName() );
  printf( "Sigma Grain: \t%f \t%f \t%f \n", 
	  eSigmaXgr,eSigmaYgr,eSigmaZgr);
  printf( "Sigma0 x,y,tx,ty: \t%f \t%f \t%f \t%f \n", 
	  eSigmaX0,eSigmaY0,eSigmaTX0,eSigmaTY0);
  printf( "Angular degradation: \t%f \n", eDegrad);
  printf( "Acceptance bins:   \t%f \t%f \t%f \t%f\n",BinX(),BinY(),BinTX(),BinTY());
  printf( "Puls ramp at 0   angle: \t%f \t%f \n", ePuls0[0],  ePuls0[1]  );
  printf( "Puls ramp at 0.4 angle: \t%f \t%f \n", ePuls04[0], ePuls04[1] );
  printf( "Chi2Max: \t %f\n",Chi2Max());
  printf( "Chi2PMax:\t %f\n",Chi2PMax());
  printf( "******************************************************\n");
}

//______________________________________________________________________________
float EdbScanCond::StepX(float dz) const
{
  float sigma = TMath::Sqrt( eSigmaX0*eSigmaX0 + eSigmaTX0*dz*eSigmaTX0*dz );
  return eBinX*sigma;
}

//______________________________________________________________________________
float EdbScanCond::StepY(float dz) const
{
  float sigma = TMath::Sqrt( eSigmaY0*eSigmaY0 + eSigmaTY0*dz*eSigmaTY0*dz );
  return eBinY*sigma;
}

//______________________________________________________________________________
float EdbScanCond::Ramp(float x, float x1, float x2) const
{
  float pmin=.01;
  float pmax=1.;
  if(x2<=x1)   return 1.; //error value
  if(x<x1)     return pmin;
  if(x>x2)     return pmax;
  float p = (x-x1)/(x2-x1);
  if(p<pmin)   return pmin;
  if(p>pmax)   return pmax;
  return p;
}

//______________________________________________________________________________
float EdbScanCond::ProbSeg(float tx, float ty, float puls) const
{
  float t = TMath::Sqrt(tx*tx + ty*ty);
  return ProbSeg(t, puls);
}

//______________________________________________________________________________
float EdbScanCond::ProbSeg(float t, float puls) const 
{
  // use linear puls ramp approximation
  float pa1 = ePuls0[0] - t/.4*(ePuls0[0]-ePuls04[0]);
  float pa2 = ePuls0[1] - t/.4*(ePuls0[1]-ePuls04[1]);
  return Ramp(puls,pa1,pa2);
}

///______________________________________________________________________________
EdbSegCouple::EdbSegCouple() 
{ 
  Set0();
}

///______________________________________________________________________________
EdbSegCouple::~EdbSegCouple() 
{ 
  if(eS) delete eS;
}

///______________________________________________________________________________
void EdbSegCouple::Set0() 
{ 
  eID1=0;
  eID2=0;
  eN1=0;
  eN2=0;
  eN1tot=0;
  eN2tot=0;
  eCHI2=0;
  eCHI2P=0;
  eS=0;
}

///______________________________________________________________________________
void EdbSegCouple::SetSortFlag(int s) 
{ 
  egSortFlag=s; 
}

///______________________________________________________________________________
void EdbSegCouple::Print()
{
  printf("%f \t %f \t %d \t%d  \t\t %d \t %d \n",CHI2(),CHI2P(),ID1(),N1(),ID2(),N2() );
}

///______________________________________________________________________________
int EdbSegCouple::Compare( const TObject *obj ) const
{
  const EdbSegCouple *seg = (EdbSegCouple *)obj;

  double f1=0, f2=0;

  if(SortFlag()==0) {
    f1=CHI2P();
    f2=seg->CHI2P();
  } else {
    f1 =      N1() +      N2()*100000  +      CHI2P()/1000.;
    f2 = seg->N1() + seg->N2()*100000  + seg->CHI2P()/1000.;
  }

  if (f1>f2)
    return 1;
  else if (f1<f2)
    return -1;
  else
    return 0;
 
}

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
  if(eSegCouples) delete eSegCouples;
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
void EdbPatCouple::CalculateAffXYZ( float z )
{
  // calculate affine transformation for SELECTED couples at given z

  if(!eAff) eAff = new EdbAffine2D();
  else eAff->Reset();

  EdbSegCouple *scp=0;
  EdbSegP *s1, *s2;
  int   ncp=Ncouples();
  
  Float_t *x1 = new Float_t[ncp];
  Float_t *y1 = new Float_t[ncp];
  Float_t *x2 = new Float_t[ncp];
  Float_t *y2 = new Float_t[ncp];

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
  eAff->Calculate( ncp,x1,y1,x2,y2 );

  delete[] x1;
  delete[] y1;
  delete[] x2;
  delete[] y2;
}

//______________________________________________________________________________
void EdbPatCouple::CalculateAffXY()
{
  // calculate affine transformation for SELECTED couples at Z in the center
  float z = (Pat2()->Z()-Pat1()->Z())/2.;
  CalculateAffXYZ(z);
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
  printf("FindOffset( %d x %d ) with steps %8.3f %8.3f \t%f %f\n",
	 2*nx+1,2*ny+1,stepx,stepy,Cond()->StepTX(0),Cond()->StepTY(0) );

  FillCell_XYaXaY(pat1,Cond(),dz/2.,stepx,stepy);
  printf( " drop couples: %d \n",pat1->Cell()->DropCouples(4) );
  FillCell_XYaXaY(pat2,Cond(),-dz/2.,stepx,stepy);
  printf( " drop couples: %d \n",pat2->Cell()->DropCouples(4) );

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
      printf(" %5d", npat);

      if(npat>npat0) {
	npat0 = npat;
	voff[0] = ix*stepx;
	voff[1] = iy*stepy;
      }
 
      vshift[0] = -ix;
      vshift[1] = -iy;
      c1->Shift(2,vshift);
    }
    printf("\n");
  }

  printf("\nOffset: %f %f   npat = %d \n\n", voff[0], voff[1], npat0 );

  EdbAffine2D *aff  = new EdbAffine2D();
  aff->ShiftX(voff[0]);
  aff->ShiftY(voff[1]);
  pat1->Transform(aff);

  return npat0;
}

///______________________________________________________________________________
int EdbPatCouple::FillCHI2P()
{
  //  fast chi2 calculation used for couples selection

  EdbSegCouple *scp=0;
  float       chi2;
  int ncp = Ncouples();

  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);

    if     (eCHI2mode==1) chi2 = Chi2Pn(scp);
    else if(eCHI2mode==2) chi2 = Chi2Pz0(scp);
    else if(eCHI2mode==3) chi2 = Chi2KF(scp);
    else                  chi2 = Chi2A(scp);

    scp->SetCHI2P(chi2);
  }
  return Ncouples();
}

//______________________________________________________________________________
/* TODO
bool EdbPatCouple::IsCompatible( EdbSegP *s1, EdbSegP *s2, EdbScanCond *cond ) const
{
  // return true if segments are compatible with acceptance defined in cond 

  float dtx=s2->TX()-s1->TX();
  float stx = cond->SigmaTX( TMath::Max( TMath::Abs(s2->TX()),TMath::Abs(s1->TX())) );
  if( dtx > BinTX()*stx )    return false;
  float dty=TY()-s.TY();
  if( dty*dty > STY()*nsigt*nsigt )    return false;
  float dz=s.Z()-Z();
  float dx=X()+TX()*dz-s.X();
  if( dx*dx > SX()*nsigx*nsigx )       return false;
  float dy=Y()+TY()*dz-s.Y();
  if( dy*dy > SY()*nsigx*nsigx )       return false;
  return true;
}
*/

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
  // exact estimation of chi2 with Kalman filtering procedure
  // application: up/down linking, alignment (offset search)
  //
  // All calculation are done with full corellation matrix for both segments
  // (dependency on the polar angle (phi))

  EdbSegP *s1 = Pat1()->GetSegment(scp->ID1());
  EdbSegP *s2 = Pat2()->GetSegment(scp->ID2());

  float tx,sx,sy,stx,sty;

  tx = TMath::Sqrt(s1->TX()*s1->TX()+s1->TY()*s1->TY());
  sx    = eCond->SigmaX(tx);
  sy    = eCond->SigmaY(0);
  stx   = eCond->SigmaTX(tx);
  sty   = eCond->SigmaTY(0);
  float sx2 = sx*sx, sy2 = sy*sy, sz2 = 0., stx2 = stx*stx, sty2= sty*sty;
  s1->SetErrorsCOV(sx2, sy2, sz2, stx2, sty2, 0.);

  tx = TMath::Sqrt(s2->TX()*s2->TX()+s2->TY()*s2->TY());
  sx    = eCond->SigmaX(tx);
  stx   = eCond->SigmaTX(tx);
  sx2 = sx*sx; stx2 = stx*stx;
  s2->SetErrorsCOV(sx2, sy2, sz2, stx2, sty2, 0.);

  if(scp->eS) delete (scp->eS);
  scp->eS=new EdbSegP(*s2);
  return EdbVertexRec::Chi2Seg(scp->eS, s1);
}

///______________________________________________________________________________
float EdbPatCouple::Chi2A(EdbSegCouple *scp, int iprob)
{
  EdbSegP *s1 = Pat1()->GetSegment(scp->ID1());
  EdbSegP *s2 = Pat2()->GetSegment(scp->ID2());
  return Chi2A(s1,s2, iprob);
}

/*
///______________________________________________________________________________
float EdbPatCouple::Chi2Aold(EdbSegP *s1, EdbSegP *s2, int iprob)
{
  // fast estimation of chi2 in the special case when the position 
  // errors of segments are negligible in respect to angular errors: 
  // sigmaXY/dz << sigmaTXY
  // application: up/down linking, alignment (offset search)
  //
  // badness: generate combinatorical peaks in 4 quadrants
  //
  float dz  = s2->Z() - s1->Z();
  float tx  = (s2->X() - s1->X())/dz;
  float ty  = (s2->Y() - s1->Y())/dz;
  float stx   = eCond->SigmaTX(tx);
  float sty   = eCond->SigmaTY(ty);
  float prob1=1., prob2=1.;
  if(iprob) {
    prob1 = eCond->ProbSeg(tx,ty,s1->W());
    prob2 = eCond->ProbSeg(tx,ty,s2->W());
  }
  float dtx1 = (s1->TX()-tx)*(s1->TX()-tx)/stx/stx/prob1;
  float dty1 = (s1->TY()-ty)*(s1->TY()-ty)/sty/sty/prob1;
  float dtx2 = (s2->TX()-tx)*(s2->TX()-tx)/stx/stx/prob2;
  float dty2 = (s2->TY()-ty)*(s2->TY()-ty)/sty/sty/prob2;
  return TMath::Sqrt(dtx1+dty1+dtx2+dty2)/2.;
}
*/

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

  //printf("dz,tx,ty,stx,sty:  %f\t %f %f\t %f %f\n",dz,tx,ty,stx,sty); 
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

/*
///______________________________________________________________________________
float EdbPatCouple::Chi2full( EdbSegP *s1, EdbSegP *s2, int iprob  )
{
  // full estimation of chi2  
  //
  // All calculation are done in the track plane which remove the 
  // dependancy of the polar angle (phi)

  float tx,ty;

  tx = (s2->X() - s1->X())/(s2->Z() - s1->Z());
  ty = (s2->Y() - s1->Y())/(s2->Z() - s1->Z());

  TVector3 v1,v2,v;  // values
  TVector3 s1,s2,s;  // sigmas
  s1.SetXYZ( 


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
  tx  = v.X()/dz;
  ty  = v.Y()/dz;
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
  return TMath::Sqrt(dtx1+dty1+dtx2+dty2)/2.;
}

*/

///______________________________________________________________________________
float EdbPatCouple::Chi2Pn( EdbSegCouple *scp )
{
  //use grain parameters for segment errors estimation

  EdbSegP *s1=0, *s2=0;
  float sx=.5, sy=.5, sz=3., dz=44.;
  float sa;
  float a1,a2;
  float tx,ty;

  TVector3 v1,v2,v;

  s1 = Pat1()->GetSegment(scp->ID1());
  s2 = Pat2()->GetSegment(scp->ID2());

  tx = (s2->X() - s1->X())/(s2->Z() - s1->Z());
  ty = (s2->Y() - s1->Y())/(s2->Z() - s1->Z());

  v1.SetXYZ( s1->TX()/sx, s1->TY()/sy , 1./sz );
  v2.SetXYZ( s2->TX()/sx, s2->TY()/sy , 1./sz );
  v.SetXYZ(  -( s2->X() - s1->X() )/sx , 
	     -( s2->Y() - s1->Y() )/sy , 
	     -( s2->Z() - s1->Z() )/sz   );
  a1 = v.Angle(v1);
  a2 = v.Angle(v2);
  sa = sz/dz*TMath::Cos(v.Theta());
  
  return TMath::Sqrt( (a1*a1 + a2*a2)/2. ) / 
    eCond->ProbSeg( tx, ty, (s1->W()+s2->W())/2. ) / sa;
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

  return TMath::Sqrt( (a1*a1 + a2*a2)/2. ) / 
    eCond->ProbSeg( tx, ty, (s1->W()+s2->W())/2. ) / sa;
}

///______________________________________________________________________________
int EdbPatCouple::CutCHI2P(float chi2max)
{
  TObjArray *sCouples = new TObjArray();
  EdbSegCouple *sc = 0;
  int ncp = Ncouples();
  printf("CutCHI2P (%4.1f):  %d -> ", chi2max,ncp );
 
  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if(sc->CHI2P()<=chi2max)      sCouples->Add(sc);
    else{ delete sc;  sc=0; }
  }
  delete eSegCouples;
  eSegCouples=sCouples;
  printf(" %d \n", Ncouples() );
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::SelectIsolated()
{
  TObjArray *sCouples = new TObjArray();
  EdbSegCouple *sc = 0;
  int ncp = Ncouples();
  printf("SelectIsolated:  %d -> ", ncp );

  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if( sc->N1tot()>1 || sc->N2tot()>1 )   { delete sc;  sc=0; }
    else sCouples->Add(sc);    
  }
  delete eSegCouples;
  eSegCouples=sCouples;
  printf(" %d \n", Ncouples() );
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
  int *found1 = new int[np1];
  int *found2 = new int[np2];
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
int EdbPatCouple::Align()

{
  int npat =0;

  SetZlink( (Pat1()->Z()+Pat2()->Z())/2. );  // link at central z

  printf("\n************************************************************\n");
  printf("align patterns: %d (%d)  and  %d (%d) at Zlink = %f\n",
	 Pat1()->ID(), Pat1()->N(), Pat2()->ID(),Pat2()->N(), Zlink() );

  npat = FindOffset01(eXoffsetMax,eYoffsetMax);   if(npat<2) return npat;

  Long_t vdiff[4]={0,0,0,0};
  int nitr=5;

  FillCell_XYaXaY(Cond(), Zlink(), 2 );    Pat2()->Cell()->DropCouples(4);

  for(int i=0; i<nitr; i++) {
    FillCell_XYaXaY(Cond(), Zlink(), 1 );   Pat1()->Cell()->DropCouples(4);
    npat = DiffPat(Pat1(),Pat2(),vdiff);

    CalculateAffXYZ(Zlink());

    Pat1()->Transform(GetAff());
    if( Pat1()->DiffAff(GetAff()) < Cond()->SigmaX(0)/20. ) break;  // stop iterations
  }

  vdiff[0]=vdiff[1]=vdiff[2]=vdiff[3]=1;

  npat = DiffPat( Pat1(), Pat2(), vdiff );
  FillCHI2P();
  SortByCHI2P();
  CutCHI2P(1.5);
  SelectIsolated();
  CalculateAffXYZ(Zlink());
  Pat1()->Transform(GetAff());
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

  printf("Couple: link (slow) patterns: %d (%d)  and  %d (%d) \n",
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
int EdbPatCouple::LinkFast()
{
  // to link segments of already aligned patterns
  // fast - because check couples by cells

  int npat =0;

  SetZlink( (Pat2()->Z()+Pat1()->Z())/2. );

  printf("Couple: link patterns: %d (%d)  and  %d (%d) at Z = %f\n",
	 Pat1()->ID(), Pat1()->N(), Pat2()->ID(),Pat2()->N(), Zlink() );

  FillCell_XYaXaY(Cond(), Zlink() );

  Long_t vdiff[4]={1,1,1,1};

  npat = DiffPat( Pat1(), Pat2(), vdiff );

  FillCHI2P();
  //  FillCHI2();

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
  //cell->SetName("x:y:tx:ty:entry");
}

///=============================================================================
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
}

///______________________________________________________________________________
EdbPVRec::~EdbPVRec()
{
  if(ePatCouples)     delete ePatCouples;
  if(eTracks)         delete eTracks;
  if(eVTX)            delete eVTX;
  if(eTracksCell)     delete eTracksCell;
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
  EdbPatCouple *pc = 0;
  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->SetPat1( GetPattern(pc->ID1()) );
    pc->SetPat2( GetPattern(pc->ID2()) );

    if( TMath::Abs(pc->Pat2()->Z() - pc->Pat1()->Z()) < 40. )  pc->SetCHI2mode(2); 
    //pc->SetCHI2mode(3);  /// debug
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
void EdbPVRec::SetCouplesAll()
{
  EdbPatCouple *pc = 0;
  int npat=Npatterns();
  for(int i=0; i<npat; i++ ) GetPattern(i)->SetSegmentsPID();
  for(int i=0; i<npat-1; i++ ) {
    pc = new EdbPatCouple();
    pc->SetID(i,i+1);
    pc->SetCond(eScanCond);
    pc->SetOffset(0,0,0,0);
    pc->SetSigma(1,1,.003,.003);       //TODO: organise this sigmas
    AddCouple(pc);
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
  EdbPattern *pat;
  EdbSegP    *seg;

  int npat, nseg;
  npat =Npatterns();
  for(int i=0; i<npat; i++ ) {
    pat = GetPattern(i);
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
  printf(" EdbPVRec::LinkSlow: npat= %d \n",npat);
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
  //printf(" EdbPVRec (LinkFast): npat= %d \n",npat);
  return npat;
}

//______________________________________________________________________________
int EdbPVRec::LinkTracks()
{
  SetCouples();

  EdbPatCouple *pc = 0;

  int ncp=Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->LinkFast();
    pc->CutCHI2P(1.5);
    pc->SortByCHI2P();
    pc->SelectIsolated();
  }
  FillTracksCell();
  SelectLongTracks( Npatterns());

  AlignA();

  //Align();
  return 1;
}

//______________________________________________________________________________
int EdbPVRec::AlignA()
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

  int ncp = Ncouples();
  for(i=0; i<ncp; i++ )   GetCouple(i)->CalculateAffXY();

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
int EdbPVRec::Align()
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
    pc->Align();
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

  printf("fill tracks cell... \n");

  Long_t vid1,vid2;

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
  printf(" ok\n");
  tracksCell->PrintPopulation(1);
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
    printf("cross: %d %d\n",iv,ncpp);
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
  //cross->PrintPopulation(1);

  TIndexCell *ct=0;
  int ncross = cross->GetSize();
  printf("ncross = %d\n",ncross);
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
    printf("cross: %d %d\n",iv,ncpp);
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
  cross->PrintPopulation(1);

  TIndexCell *ct=0;
  int ncross = cross->N(1);
  printf("ncross = %d\n",ncross);
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
  tracks->PrintPopulation(1);
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
    printf("\n%d tracks are merged!\n",  m1);
    if(m1==0)  break;
    merged += m1;
  }

  printf("\nTotal: %d tracks are merged!\n",  merged);

  return merged;
}

//______________________________________________________________________________
int EdbPVRec::MergeTracks1(int maxgap)
{
  int imerged=0;
  float X0 = GetScanCond()->RadX0();

  EdbTrackP *tr =0;

  int npat = Npatterns();
  int ntr  = eTracks->GetEntriesFast();
  int nseg;

  Long_t  v[2];
  TIndexCell starts,ends;              // "ist:entry"   "iend:entry"

  for(int itr=0; itr<ntr; itr++)   {
    tr = (EdbTrackP*)(eTracks->At(itr));
    tr->SetFlag(0);
    nseg = tr->N();
    if(nseg>=npat-1)                              continue;

    if( tr->GetSegment(0)->PID()      > 1      )  {        // tracks with missing starts
      v[0] = tr->GetSegment(0)->PID();
      v[1] = itr;
      starts.Add(2,v);
    }
    if( tr->GetSegment(nseg-1)->PID() < npat-2 )  {        // tracks with missing ends
      v[0] = tr->GetSegment(nseg-1)->PID();
      v[1] = itr;
      ends.Add(2,v);
    }
  }

  starts.Sort();
  ends.Sort();

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
void EdbPVRec::FitTracks(float p, float mass)
{
  // measurement errors: TODO

  float X0 = GetScanCond()->RadX0();

  EdbTrackP *tr =0;
  //EdbSegP   *seg=0;

  int ntr = eTracks->GetEntriesFast();
  int nseg;
  for(int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(eTracks->At(itr));

    nseg = tr->N();

    //for(int iseg=0; iseg<nseg; iseg++) {
    //  seg = tr->GetSegment(iseg);
      //sx2 = GetScanCond()->SigmaX( seg->TX() );   sx2*=sx2;
      //sy2 = GetScanCond()->SigmaY( seg->TY() );   sy2*=sy2;
      //stx2 = GetScanCond()->SigmaTX( seg->TX() ); stx2*=stx2;
      //sty2 = GetScanCond()->SigmaTY( seg->TY() ); stx2*=stx2;
      //seg->SetErrors(sx2,sy2,sz2,stx2,sty2,sp2);
    //}

    tr->SetP(p);
    tr->SetM(mass);
    tr->FitTrackKFS(true,X0);
  }

}

//______________________________________________________________________________
int EdbPVRec::MakeTracks(int nsegments)
{
  // extract from index_table tracks longer then nsegments
  // and form tracks array

  if(eTracks) delete eTracks;
  eTracks  = new TObjArray();
  printf("make tracks...\n");

  int         nseg, ntr=0, n0;
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

    n0=0;
    for(int is=0; is<nseg; is++) {
      vid = ct->At(is)->Value();
      seg = GetSegment(vid);
      //seg->SetPID( Pid(vid) );   //TODO check!!!
      if(seg->Flag()<0) n0++;
      track->AddSegment(seg);
    }
    track->SetN0(n0);
    track->SetID(it);
    eTracks->Add(track);
    ntr++;
  }

  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr;
}

//______________________________________________________________________________
int EdbPVRec::MakeTracksTree()
{
  if(!eTracks) return 0;

  char *file="linked_tracks.root";
  printf("\nwrite tracks into %s ...\t ",file);
  TFile fil(file,"RECREATE");
  TTree *tracks= new TTree("tracks","tracks");

  EdbTrackP    *track = new EdbTrackP(8);
  EdbSegP      *tr = (EdbSegP*)track;
  TClonesArray *segments  = new TClonesArray("EdbSegP");
  TClonesArray *segmentsf = new TClonesArray("EdbSegP");

  int   nseg,trid,npl,n0;
  float xv=X();
  float yv=Y();
 
  tracks->Branch("trid",&trid,"trid/I");
  tracks->Branch("nseg",&nseg,"nseg/I");
  tracks->Branch("npl",&npl,"npl/I");
  tracks->Branch("n0",&n0,"n0/I");
  tracks->Branch("xv",&xv,"xv/F");
  tracks->Branch("yv",&yv,"yv/F");
  tracks->Branch("t.","EdbSegP",&tr,32000,99);
  tracks->Branch("s", &segments);
  tracks->Branch("sf",&segmentsf);

  int ntr = eTracks->GetEntriesFast();
  for(int itr=0; itr<ntr; itr++) {
    track = (EdbTrackP*)(eTracks->At(itr));

    trid = track->ID();
    nseg = track->N();
    npl  = track->Npl();
    n0   = track->N0();

    tr = (EdbSegP*)track;

    segments->Clear("C");
    segmentsf->Clear("C");
    int nseg = track->N();
    EdbSegP *s=0,*sf=0;
    for(int is=0; is<nseg; is++) {
      s = track->GetSegment(is);
      if(s) new((*segments)[is])  EdbSegP( *s );
      sf = track->GetSegmentF(is);
      if(sf) new((*segmentsf)[is])  EdbSegP( *sf );
    }

    tracks->Fill();
    track->Clear();
  }

  tracks->Write();
  fil.Close();
  printf("%d tracks are written \n",ntr);
  return ntr; 
}

//______________________________________________________________________________
int EdbPVRec::FineCorrXY(int ipat, EdbAffine2D &aff)
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
  aff.Calculate( itr,x1,y1,x,y );
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
    //    printf("track chi2 = %f\n",track->CHI2());
  }
  return ntr;
}


//______________________________________________________________________________
int EdbPVRec::SelectLongTracks(int nsegments)
{
  int ntr=0;
  if(!eTracksCell) return ntr;
  if(nsegments<2) return ntr;
  if(eTracks) delete eTracks;
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

  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr; 
}

///______________________________________________________________________________
int EdbPVRec::PropagateTracks(int nplmax, int nplmin)
{
  //  extrapolate incomplete tracks and update them with new segments
  //
  //  input: nplmax - the maximal length of the track to be continued
  //  input: nplmin - the minimal length of the track to be continued

  TIndexCell cn;  //"npl:prob:entry"
  Long_t v[3];

  int nseg=0;
  EdbTrackP *tr=0;
  for(int i=0; i<eTracks->GetEntriesFast(); i++) {
    tr = (EdbTrackP*)(eTracks->At(i));
    tr->SetID(i);
    tr->SetFlag(0);
    tr->SetSegmentsTrack();
    v[0]= -(tr->Npl());
    v[1]= (Long_t)(tr->Prob()*100);
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

	if(tr->Flag()==-10) continue; 

	//printf("\n propagate track: %d with %d segments ",tr->ID(),tr->N());

	nseg = PropagateTrack(*tr, true);
  	nsegTot += nseg;
	//printf("\t %d after true ",tr->N());

	if(tr->Npl()>nplmax)   continue;
	if(tr->Flag()==-10)    continue;

  	nseg = PropagateTrack(*tr, false);
  	nsegTot += nseg;
	//printf("\t %d after false \n",tr->N());

      }
    }
  }
  return nsegTot;
}

///______________________________________________________________________________
int EdbPVRec::PropagateTrack( EdbTrackP &tr, bool followZ )
{
  float binx=10, bint=10;
  float X0 = GetScanCond()->RadX0();
  float probMin = 0.05;

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
  int nseg =0, nsegTot=0;

  int ntr = eTracks->GetEntriesFast();

  int ngap =0;
  int ngapMax=3; //TODO - as parameter

  //printf("pstart, pend, step: %d %d %d \t ntr =%d\n",pstart,pend,step,ntr);

  for(int i=pstart+step; i!=pend+step; i+=step ) {
    pat = GetPattern(i);
    if(!pat)                   continue;
    ss.PropagateTo(pat->Z());
    
    arr.Clear();
    nseg = pat->FindCompliments(ss,arr,binx,bint);

    float probmax=0, prob=0;
    segmax=0;
    if(nseg==1)   {                           // TODO add segment owner logic
      probmax=1.;
      segmax = (EdbSegP*)(arr.At(0));
    } else if(nseg>1) {
      for(int is=0; is<nseg; is++ ) {
	seg = (EdbSegP*)(arr.At(is));
	prob = EdbVertexRec::ProbeSeg( &tr, seg, X0 );
	if( prob>probmax ) { probmax=prob; segmax=seg; }
      }
    }
    if(!segmax) {ngap++; continue;}

    int trind= segmax->Track();

    if( trind >= 0 && trind<ntr ) {
      EdbTrackP *ttt = ((EdbTrackP*)eTracks->At(trind));
      if( ttt->N() > tr.N() ) {
	ngap++; continue;
	//tr.SetFlag(-10);
	//tr.SetSegmentsTrack(-1);
	//return 0;
      } else {
	ttt->SetFlag(-10);
	//ttt->SetSegmentsTrack(-1);
      }
    }

    if(probmax>probMin) {
      if( EdbVertexRec::AttachSeg( tr, segmax , X0, probMin, probmax )) {

	segmax->SetTrack(tr.ID());
	tr.MakeSelector(ss,followZ);
	nsegTot++;
	ngap =0;
	int fl = tr.Flag()+1;
	tr.SetFlag(fl);
      }
    }

    if(ngap>ngapMax) break;
  }

  tr.SetNpl();
  tr.FitTrackKFS(followZ,X0);           // TODO remove refit?
  tr.SetSegmentsTrack();



  return nsegTot;
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
    //ss.PropagateTo(pat->Z());

    nseg += pat->FindCompliments(ss,arr,binx,bint);
  }
  printf("%d segments are selected\n",nseg);
  return nseg;
}

