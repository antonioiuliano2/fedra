//-- Author :  Valeri Tioukov   18.03.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVRec                                                             //
//                                                                      //
// patterns volume reconstruction                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TIndexCell.h"
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

///______________________________________________________________________________
///______________________________________________________________________________
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
EdbSegCouple   *EdbPatCouple::AddSegCouple( int id1, int id2 )
{
  EdbSegCouple *sc = new EdbSegCouple(id1,id2);
  if(!eSegCouples)   eSegCouples = new TObjArray(1000);
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
  float x1[ncp];
  float y1[ncp];
  float x2[ncp];
  float y2[ncp];
  float dz1,dz2;

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
  //printf("Chi2 filled:  %d \n",Ncouples());
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::CutCHI2P(float chi2max)
{
  TObjArray *sCouples = new TObjArray();
  EdbSegCouple *sc = 0;
  printf("CutCHI2P (%4.1f):  %d -> ", chi2max,Ncouples() );
  int ncp = Ncouples();
 
  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if(sc->CHI2P()<=chi2max)      sCouples->Add(sc);
    else {
      delete sc;
      sc=0;
    }
  }
  //eSegCouples->Clear();
  delete eSegCouples;
  eSegCouples=sCouples;
  //eSegCouples->Compress();
  printf(" %d \n", Ncouples() );
  return Ncouples();
}

///______________________________________________________________________________
int EdbPatCouple::SelectIsolated()
{
  EdbSegCouple *sc = 0;
  int ncp = Ncouples();
  printf("SelectIsolated:  %d -> ", ncp );
  for( int i=ncp-1; i>=0; i-- ) {
    sc = GetSegCouple(i);
    if( sc->N1tot()>1 || sc->N2tot()>1 )   RemoveSegCouple(sc);
  }
  eSegCouples->Compress();
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
  for(int i=0; i<np1; i++) found1[i]=0;
  for(int i=0; i<np2; i++) found2[i]=0;

  EdbSegCouple *sc = 0;

  int ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
    sc = GetSegCouple(i);
    found1[sc->ID1()]++;
    found2[sc->ID2()]++;
    sc->SetN1( found1[sc->ID1()] );
    sc->SetN2( found2[sc->ID2()] );
    npat++;
  }

  ncp = Ncouples();
  for( int i=0; i<ncp; i++ ) {
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
  //printf("diffpat: %d \n",npat);

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

		      AddSegCouple(v[0],v[1]);

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
  TIndexCell *tracksCell = eTracksCell;
  FillTracksCell();
  SelectLongTracks( Npatterns(), tracksCell );

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
  for(int i=0; i<npat; i++ ) {
    a0 = new EdbAffine2D();
    GetPattern(i)->GetKeep(*a0);
    aKeep[i]=a0;
  }

  int ncp = Ncouples();
  for(int i=0; i<ncp; i++ )   GetCouple(i)->CalculateAffXY();

  EdbAffine2D a;
  npat = Npatterns();
  for(int i=npat-1; i>0; i-- ) {
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

  for(int i=0; i<npat; i++ ) {
    a0 = new EdbAffine2D();
    GetPattern(i)->GetKeep(*a0);
    aKeep[i]=a0;
  }


  SetCouples();
  EdbPatCouple *pc = 0;
  int ncp = Ncouples();
  for(int i=0; i<ncp; i++ ) {
    pc = GetCouple(i);
    pc->Align();
  }


  EdbAffine2D a;
  npat = Npatterns();
  for(int i=npat-1; i>0; i-- ) {
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
 
  TFile fil("linked_tracks.root","RECREATE");
  TTree *tracks= new TTree("tracks","tracks");

  TClonesArray *segments=new TClonesArray("EdbSegP",100);

  tracks->Branch("trid",&trid,"trid/I");
  tracks->Branch("nseg",&nseg,"nseg/I");
  tracks->Branch("npl",&npl,"npl/I");
  tracks->Branch("n0",&n0,"n0/I");
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
    //    tracks->SetBranchAddress("s",&segments);
    //    tracks->SetBranchAddress("t.",&tr);
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
int EdbPVRec::SelectLongTracks(int nsegments, TIndexCell *tracksCell)
{
  int ntr=0;
  if(!tracksCell) return ntr;
  if(nsegments<2) return ntr;
  //if(eTracks) delete eTracks;
  //eTracks = new TObjArray();

  //EdbTrackP  *track=0;
  //EdbSegP    *seg=0;

  ResetCouples();
  EdbPatCouple *pc=0;
  TIndexCell *ct;
  Long_t vid1=0,vid2=0;
  
  int ntc, nct; 

  ntc = tracksCell->GetEntries();
  for(int it=0; it<ntc; it++) {

    ct = tracksCell->At(it);
    if( ct->N() < nsegments )     continue;
    //track = new EdbTrackP();
    //track->SetID( ct->Value() );

    nct = ct->GetEntries()-1;
    for(int is=0; is<nct; is++) {

      vid1 = ct->At(is)->Value();
      vid2 = ct->At(is+1)->Value();
      pc = GetCouple(Pid(vid1));              //TODO: depends on cp sequence
      pc->AddSegCouple( Sid(vid1), Sid(vid2) );
      
      //seg = GetPattern(pid)->GetSegment(segid);
      //seg->SetPID(pid);
      //track->AddSegment(*seg);
    }

    ntr++;
    //eTracks->Add(track);
  }

  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr; 
}









/*


//______________________________________________________________________________
int EdbPatternsVolume::AlignFast(EdbPatternsVolume &pvol)
{
  // TODO: in this function we follow only connected tracks 

  int pass = 0;
  int nseg = 0;
  if(pvol.Npatterns()<2) return nseg;
  
  TObjArray aKeep(pvol.Npatterns());
  EdbAffine2D *a0;

  for(int i=0; i<pvol.Npatterns(); i++ ) {
    a0 = new EdbAffine2D();
    pvol.GetPattern(i)->GetKeep(*a0);
    aKeep[i]=a0;
  }

  for(int i=1; i<Npatterns()-1; i++ ) {
    EdbPattern *psel1 = new EdbPattern();
    EdbPattern *psel2 = new EdbPattern();
    nseg += Align(pvol.GetPattern(i),pvol.GetPattern(i+1), pass,
		  psel1,psel2 );
    //pvol.SubstitutePattern(i,psel1);
    delete psel1;
    delete psel2;
  }

  EdbAffine2D a;
  for(int i=pvol.Npatterns()-1; i>0; i-- ) {
    pvol.GetPattern(i)->GetKeep(a);
    a0=(EdbAffine2D *)(aKeep.At(i));
    a0->Invert();
    a0->Transform(&a);
    pvol.GetPattern(i-1)->Transform(a0);
  }

  return nseg;
}

//______________________________________________________________________________
int EdbPatternsVolume::Align()
{
  int ntr = 0;
  if(Npatterns()<2)  return ntr;

  VerifyParameters();    // check parameters, if missed - set default
  SetPatternsID();

  SetBinsCheck(0,0,0,0);
  Align(0);
  PrintAff();

  float ks =3.;
  SetOffsetsMax(SigmaX(0),SigmaY(0));
  SetBins( ks,ks,ks,ks );
  SetBinsCheck(1,1,1,1);
  Align(1);
  PrintAff();
  //Link(1);

  EdbPatternsVolume pvol;
  PassProperties(pvol);
  ExtractSelectedTracks( &pvol, Npatterns() );

  pvol.SetOffsetsMax(SigmaX(0),SigmaY(0));
  pvol.SetBins( ks,ks,ks,ks );
  pvol.SetBinsCheck(1,1,1,1);
  
  pvol.Align(2);
  pvol.PrintAff();

//    ks = 1.1;
//    pvol.SetBins(ks*eSigmaX,ks*eSigmaY,ks*eSigmaTX,ks*eSigmaTY);
//    pvol.Align(3);
//    pvol.PrintAff();

//    EdbPatternsVolume pvol2;
//    pvol.PassProperties(pvol2);
//    //pvol.SelectTracks(&pvol2, pvol.Npatterns(), 3);

//    //SelectTracks( 0, 2, 1 );

  return ntr;
}

//______________________________________________________________________________
int EdbPatternsVolume::Align(int pass)
{
  int nseg = 0;

  //eTracksCell->Drop();

  TObjArray aKeep(Npatterns());
  EdbAffine2D *a0;

  for(int i=0; i<Npatterns(); i++ ) {
    a0 = new EdbAffine2D();
    GetPattern(i)->GetKeep(*a0);
    aKeep[i]=a0;
  }

  for(int i=0; i<Npatterns()-1; i++ ) {
    EdbPattern *psel1 = new EdbPattern();
    EdbPattern *psel2 = new EdbPattern();
    nseg += Align(GetPattern(i),GetPattern(i+1), pass,
		  psel1,psel2);

    delete psel1;
    delete psel2;
  }

  EdbAffine2D a;
  for(int i=Npatterns()-1; i>0; i-- ) {
    GetPattern(i)->GetKeep(a);
    a0=(EdbAffine2D *)(aKeep.At(i));
    a0->Invert();
    a0->Transform(&a);
    GetPattern(i-1)->Transform(a0);
  }

  return nseg;
}
//______________________________________________________________________________
int EdbPatternsVolume::AlignFast()
{
  // TODO: the idea is to use for next couple only connected segments

  int ntr = 0;
  if(Npatterns()<2)  return ntr;

  VerifyParameters();    // check parameters, if missed - set default
  SetPatternsID();

  EdbPatternsVolume pvol(*this);

  SetBinsCheck(0,0,1,1);
  AlignFast( pvol );
  PrintAff();

  return ntr;
}


//______________________________________________________________________________
float EdbPatternsVolume::Chi2_( EdbSegP *s1,  EdbSegP *s2 ) const
{
  double dz = s2->Z() - s1->Z();

  if (dz == 0 ) return Chi2Z0(s1,s2);

  return 1.- s1->ProbLink(*s1,*s2);
}
 //______________________________________________________________________________
float EdbPatternsVolume::Chi2Z0( EdbSegP *s1,  EdbSegP *s2 ) const
{
  //assumed that segments has the same z

  double dx = (s2->X() - s1->X());
  double dy = (s2->Y() - s1->Y());

  double tx = (s2->TX()+s1->TX())/2;
  double ty = (s2->TY()+s1->TY())/2;

  double chi21 = 
    TMath::Sqrt( ( dx*dx /SigmaX(tx)/SigmaX(tx) +
		   dy*dy /SigmaY(ty)/SigmaY(ty) )/2. );
  double chi22 = 
    TMath::Sqrt( ( (s2->TX()-s1->TX())*(s2->TX()-s1->TX()) /SigmaTX(tx)/SigmaTX(tx) +
		   (s2->TY()-s1->TY())*(s2->TY()-s1->TY()) /SigmaTY(ty)/SigmaTY(ty) )/2. );

  return (float)((chi21+chi22)/2.);
}


//______________________________________________________________________________
void EdbPatternsVolume::VerifyParameters()
{
  float meanDist = 50.;   // mean average distance between tracks

  if(eXoffsetMax<=0.) eXoffsetMax = meanDist;
  if(eYoffsetMax<=0.) eYoffsetMax = meanDist;

  float ks=3.;

  if(eBinX<=0.)  eBinX  = ks;
  if(eBinY<=0.)  eBinY  = ks;
  if(eBinTX<=0.) eBinTX = ks;
  if(eBinTY<=0.) eBinTY = ks;

  PrintParameters();
}

//______________________________________________________________________________
void EdbPatternsVolume::PrintParameters()
{
  printf("Alignment parameters: \n");
  printf("MaxOffsets: %f %f \n", eXoffsetMax,eYoffsetMax);
  printf("Bins:       %f %f %f %f \n", eBinX, eBinY, eBinTX, eBinTY );
  printf("Vdiff:      %ld %ld %ld %ld \n", eVdiff[0],eVdiff[1],eVdiff[2],eVdiff[3]);
  eScan.Print();
}

//____________________________________________________________________________________
int EdbPatternsVolume::ExtractSelectedTracks( EdbPatternsVolume *pvol, int nsegments )
{
  // extract the linked tracks from the eTracksCell "trid:segid" and put them 
  // to the eTracks and to the output volume "pvol" if defined

  int ntr=0;
  if(!eTracksCell) return ntr;
  //eTracksCell->Sort();
  //eTracksCell->PrintPopulation(1);

  eTracks->Delete();
  EdbTrackP *track=0;

  int       pid, segid;
  EdbSegP   *seg=0;

  TIndexCellIter trItr(eTracksCell,1);
  const TIndexCell *ctr=0;
  while ( (ctr=trItr.Next()) ) {

    if( ctr->N() < nsegments )     continue;

    track = new EdbTrackP();
    track->SetID( ctr->Value() );

    TIndexCellIter segItr(ctr,1);
    const TIndexCell *cseg=0;
    while ( (cseg=segItr.Next()) ) {

      segid = cseg->Value();
      pid = segid/1000000;
      segid -= pid*1000000;

      seg = GetPattern(pid)->GetSegment(segid);

      if(pvol)  pvol->GetPattern(pid)->AddSegment( *seg );

      seg->SetPID(pid);
      track->AddSegment(*seg);
    }
    ntr++;
    eTracks->Add(track);

  }

  printf("selected %d tracks \n", ntr );
  return ntr;
}


//______________________________________________________________________________
void EdbPatternsVolume::ExtractSelectedPatterns( EdbPattern *pat1, 
						 EdbPattern *pat2, 
						 TIndexCell *csel,
						 EdbPattern *psel1, 
						 EdbPattern *psel2 )
{

  TIndexCell *c1 = 0;
  Long_t e1,e2;

  for( int i1=0; i1<csel->N(1); i1++ ) {
    c1 = csel->At(i1);
    e1 = c1->Value();

    for( int i2=0; i2<c1->N(); i2++ ) {
      e2 = c1->At(i2)->Value();

      psel1->AddSegment( *(pat1->GetSegment(e1)));
      psel2->AddSegment( *(pat2->GetSegment(e2)));
    }
  }

}


//______________________________________________________________________________
void EdbPatternsVolume::RemakeTracksCell()
{
  if(eTracksCell) delete eTracksCell; 
  eTracksCell = new TIndexCell();
}
 


*/
