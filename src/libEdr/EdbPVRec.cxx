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
}

//______________________________________________________________________________
void EdbScanCond::Print() const
{
  printf( "******************************************************\n");
  printf( "Scanning Conditions Parametres: %s\n", GetName() );
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
  int ncp = eSegCouples->GetEntries();
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
  EdbSegCouple *scp=0;
  float       chi2;
  int ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);
    chi2 = (Pat1()->GetSegment(scp->ID1()))->Chi2Aprob( *(Pat2()->GetSegment(scp->ID2())) );
    scp->SetCHI2P(chi2);
  }
  //printf("Chi2P filled:  %d \n",Ncouples());
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::FillCHI2n()
{
  EdbSegP *s1=0, *s2=0;
  float sx=.5, sy=.5, sz=3., dz=44.;
  float sa;
  float chi2, a1,a2;
  float tx,ty;

  TVector3 v1,v2,v;

  EdbSegCouple *scp=0;
  int ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);
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

    chi2 = TMath::Sqrt( (a1*a1 + a2*a2)/2. ) / 
      eCond->ProbSeg( tx, ty, (s1->W()+s2->W())/2. ) / sa;
    scp->SetCHI2(chi2);
  }

  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::FillCHI2()
{
  EdbSegCouple *scp=0;
  float       chi2;
  int ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
    scp=GetSegCouple(i);
    chi2 = (Pat1()->GetSegment(scp->ID1()))->Chi2A( *(Pat2()->GetSegment(scp->ID2())) );
    scp->SetCHI2(chi2);
  }
  return Ncouples();
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
int EdbPatCouple::SortByCHI2()
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
    if( Pat1()->DiffAff(GetAff()) < Cond()->SigmaX(0)/10. ) break;  // stop iterations
  }

  vdiff[0]=vdiff[1]=vdiff[2]=vdiff[3]=1;

  npat = DiffPat( Pat1(), Pat2(), vdiff );
  FillCHI2P();
  SortByCHI2();
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
      chi2 = (pat1->GetSegment(i1))->Chi2Aprob( *(pat2->GetSegment(i2)) );
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
  FillCHI2();

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
  //printf("npat = %d   ncouples = %d\n", npat,ncouples);
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

//______________________________________________________________________________
double EdbPatCouple::Chi2z( EdbSegP &s1,  EdbSegP &s2 ) const
{
  EdbSegP s(s1);
  s += s2;         // mean segment

  double d1 = s.Chi2(s1);
  double d2 = s.Chi2(s2);

  return TMath::Sqrt( (d1*d1 + d2*d2)/2. );
}


///=============================================================================
///=============================================================================


EdbPVRec::EdbPVRec()
{
  ePatCouples = new TObjArray();
  eTracks = 0;
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
    //printf("SetCouples: %d(%d) %d %d \n",i,Ncouples(), pc->ID1(),pc->ID2());
    pc->SetPat1( GetPattern(pc->ID1()) );
    pc->SetPat2( GetPattern(pc->ID2()) );
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
void EdbPVRec::SetSegmentErrors( EdbSegP &seg )
{
  // segment errors are depends on the scanning conditions and segment
  // parameters (angles). 
  // Puls height is represents segments probablility (not accuracy) and should 
  // be taken into account separately

  seg.SetErrors(
		GetScanCond()->SigmaX( seg.TX() ),
		GetScanCond()->SigmaY( seg.TY() ),
		GetScanCond()->SigmaZ( seg.TX(), seg.TY() ),
		GetScanCond()->SigmaTX( seg.TX() ),
		GetScanCond()->SigmaTY( seg.TY() ) 
		);

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
    npat = pc->SortByCHI2();
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
    pc->SortByCHI2();
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
    pc->SortByCHI2();
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
int EdbPVRec::InsertHoles()
{
  TIndexCell *ct;
  Long_t     vn=0,v0=0,v=0;
  int        nholes=0;

  int id=0,pid=0;
  EdbSegP   *s1=0, *s2=0;
  int n=0;
  int ntc=eTracksCell->N(1);
  for(int it=0; it<ntc; it++) {

    ct = eTracksCell->At(it);
    n = ct->N(1);
    if( n >= Npatterns() )       continue;  // too many segments
    vn = ct->At(n-1)->Value();
    v0 = ct->At(0)->Value();
    if( Pid(vn)-Pid(v0) < n-1 )  continue;  // track is not isolated

    if( Pid(v0) > 0 )  {
      v = ct->At(1)->Value();
      s1 = GetPattern( Pid(v0) )->GetSegment( Sid(v0) );
      s2 = GetPattern( Pid(v)  )->GetSegment( Sid(v) );
      pid =  Pid(v0)-1;
      id = InsertHole( s2,s1, pid); 
      ct->Add( Vid( pid, id) );
      nholes++;
    }
    if( Pid(vn) < Npatterns()-1 )  {
      v = ct->At(n-2)->Value();
      s1 = GetPattern( Pid(v)  )->GetSegment( Sid(v)  );
      s2 = GetPattern( Pid(vn) )->GetSegment( Sid(vn) );
      pid = Pid(vn)+1;
      id = InsertHole( s1,s2, pid );
      ct->Add( Vid( pid, id) );
      nholes++;
    }
  }
  return nholes;
}

//______________________________________________________________________________
int EdbPVRec::InsertHole( const EdbSegP *s1, const EdbSegP *s2, int pid )
{
  EdbSegP s;
  EdbSegP::LinkMT(s1,s2,&s);
  s.SetFlag(-1);
  EdbPattern *pat = GetPattern(pid);
  s.PropagateTo(pat->Z());
  pat->AddSegment(s);
  return pat->N()-1;
}

//______________________________________________________________________________
int EdbPVRec::MakeHoles()
{
  int nholes =0;
  FillTracksCell1();
  nholes = InsertHoles();
  eTracksCell->Sort();
  printf( "nholes= %d\n" , nholes );
  return nholes;
}

//______________________________________________________________________________
int EdbPVRec::MakeTracksTree()
{

  EdbSegP *seg;
  EdbSegP *s0=0;
  EdbSegP *tr = new EdbSegP();
  int nseg,trid,npl,n0;
  float xv=X();
  float yv=Y();
 
  TFile fil("linked_tracks.root","RECREATE");
  TTree *tracks= new TTree("tracks","tracks");

  TClonesArray *segments=new TClonesArray("EdbSegP",100);

  tracks->Branch("trid",&trid,"trid/I");
  tracks->Branch("nseg",&nseg,"nseg/I");
  tracks->Branch("npl",&npl,"npl/I");
  tracks->Branch("n0",&n0,"n0/I");
  tracks->Branch("xv",&xv,"xv/F");
  tracks->Branch("yv",&yv,"yv/F");
  tracks->Branch("t","EdbSegP",&tr,32000,99);
  tracks->Branch("s",&segments);

  int ntr=0;
  int nsegments = 2;
  if(!eTracksCell) return ntr;
  if(nsegments<2) return ntr;

  EdbPattern *pat=0;
  TIndexCell *ct;
  Long_t vid=0;
  
  int ntc=eTracksCell->GetEntries();
  for(int it=0; it<ntc; it++) {

    ct = eTracksCell->At(it);
    if( ct->N() < nsegments )     continue;

    trid = ct->At(0)->Value();
    nseg = ct->GetEntries();
    npl = Pid(ct->At(nseg-1)->Value()) - Pid(ct->At(0)->Value()) +1;

    n0=0;
    for(int is=0; is<nseg; is++) {
      vid = ct->At(is)->Value();
      pat = GetPattern( Pid(vid) );
      seg = pat->GetSegment( Sid(vid) );
      seg->SetPID( Pid(vid) );
      if(seg->Flag()<0) n0++;

      if(is==0)  tr = new EdbSegP(*seg);
      else       {
	s0 = new EdbSegP(*tr);
	EdbSegP::LinkMT(s0,seg,tr);
	delete s0;
      }
      new((*segments)[is])  EdbSegP( *seg );
    }
    tracks->Fill();
    segments->Clear();
    ntr++;
  }
  tracks->Write();
  fil.Close();
  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr; 
}


//______________________________________________________________________________
int EdbPVRec::FineCorrXY(int ipat, EdbAffine2D &aff)
{
  if(!eTracks) return 0;
  int   ntr = eTracks->GetEntries();
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
  int   ntr = eTracks->GetEntries();
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
  int   ntr = eTracks->GetEntries();
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
  int   ntr = eTracks->GetEntries();
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
  int ntr = eTracks->GetEntries();
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
      seg->SetPID(Pid(vid1));
      track->AddSegment(*seg);
    }

    ntr++;
    eTracks->Add(track);
  }

  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr; 
}
