//-- Author :  Valeri Tioukov   12.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFiducial                                                          //
//                                                                      //
// Implementation of Fiducial classes                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include  <stdio.h>
#ifndef ROOT_TMath
#include "TMath.h"
#endif
 
#ifndef WIN32
#include "TText.h"
#endif

#ifndef ROOT_EdbFiducial
#include "EdbFiducial.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif

//#ifndef ROOT_EdbBasic
//#include "EdbBasic.h"
//#endif


ClassImp(EdbMark)
ClassImp(EdbMarksBox)
ClassImp(EdbMarksSet)

ClassImp(EdbArea)

//______________________________________________________________________________
EdbArea::EdbArea( int N, float stepx, float stepy, int ft, int fb, int path ) : EdbMarksBox(N)
{
  Set( N, stepx, stepy, ft, fb, path );
}


//______________________________________________________________________________
void EdbArea::Print( Option_t opt ) const
{
  printf("EdbArea: \t %d %f %f %d %d \n", eN, eStepX, eStepY, eFramesTop, eFramesBot );
  //EdbMarksBox::Print();
}

//______________________________________________________________________________
int  EdbArea::ReadParameters( const char *str )
{
  int  npar = 0;

  int   n, ft, fb, path;
  float stepx,stepy;

  npar = sscanf(str,"%d %f %f %d %d %d", &n, &stepx, &stepy, &ft, &fb, &path );

  Set( n, stepx, stepy, ft, fb, path);

  return npar;
}


//______________________________________________________________________________
void EdbArea::Set( int N, float stepx, float stepy, int ft, int fb, int path )
{
  eN = N;
  eFramesTop = ft;
  eFramesBot = fb;
  eStepX     = stepx;
  eStepY     = stepy;
  ePath      = path;

  if( GetMarks() ) GetMarks()->Clear();

  if(ePath) {
    int *x = new int[eN];
    int *y = new int[eN];
    
    if(ePath==1) MakeSpiralPath(eN, x, y);

    for(int i=0; i<eN; i++){
      AddMark(i,x[i],y[i]);
    }

    Scale(stepx,stepy);
    delete [] x;
    delete [] y;
  }
}

//______________________________________________________________________________
void   EdbArea::MakeSpiralPath( int N, int *x, int *y )
{
  //convention: n=0 - central point
 
  int step[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };   // loop directions
 
  int a = 0;
  int b = 0;
  int k = 0;

  for( int i=0; i<N; i++ ) {
    
    x[i] = a;
    y[i] = b;

    if( TMath::Abs(a) == TMath::Abs(b) ) {
      k = ++k%4;
      if( a>=0 && b<=0 ) {
	a += 1;
	continue;
      }
    }

    a += step[k][0];
    b += step[k][1];
  }
}

//______________________________________________________________________________
void   EdbArea::SetCentre( float x, float y )
{
  float dx = x - At(0)->X();
  float dy = y - At(0)->Y();
  EdbAffine2D aff(1,0,0,1,dx,dy);
  Transform(&aff);
}

//______________________________________________________________________________
void   EdbArea::Scale( float dx, float dy )
{
  EdbAffine2D aff(dx,0,0,dy,0,0);
  Transform(&aff);
}

//______________________________________________________________________________
EdbMarksSet::EdbMarksSet()
{
  eAbsolute     = new EdbMarksBox();
  eStage        = new EdbMarksBox();
}

//______________________________________________________________________________
EdbMarksSet::~EdbMarksSet()
{
  if(eAbsolute)      delete  eAbsolute;
  if(eStage)         delete  eStage;
}

//______________________________________________________________________________
void  EdbMarksSet:: DrawA( Option_t *opt ) const
{ 
  if(eAbsolute) eAbsolute->Draw( 23, 4, 1. );
}

//______________________________________________________________________________
void  EdbMarksSet:: DrawS( Option_t *opt ) const
{ 
  if(eStage) eStage->Draw( 22, 3, 1. );
}

//______________________________________________________________________________
Int_t  EdbMarksSet::GetN()    const 
{ 
  return TMath::Min( GetNa(), GetNs() );
}

//______________________________________________________________________________
Int_t  EdbMarksSet::GetNa()    const
{ 
  if(eAbsolute) return eAbsolute->GetN(); else return 0;
}

//______________________________________________________________________________
Int_t  EdbMarksSet::GetNs()    const
{ 
  if(eStage)    return eStage->GetN();    else return 0;
}

//______________________________________________________________________________
EdbAffine2D  *EdbMarksSet::Abs2Stage() const
{ 
  EdbAffine2D *aff = new EdbAffine2D();
  aff->Calculate( (EdbPointsBox2D *)GetAbsolute(), (EdbPointsBox2D *)GetStage() );
  return aff;
}

//______________________________________________________________________________
EdbAffine2D  *EdbMarksSet::Stage2Abs() const
{
  EdbAffine2D *aff = new EdbAffine2D();
  aff->Calculate( (EdbPointsBox2D *)GetStage(), (EdbPointsBox2D *)GetAbsolute() );
  return aff;
}

//______________________________________________________________________________
Int_t  EdbMarksSet::ReadDB( char *file )
{ 
  return ReadDollar( file, eStage );
}

//______________________________________________________________________________
Int_t  EdbMarksSet::ReadDA( char *file )
{ 
  int     mod,  pln,   prg,  plt,  pos;
  float   sftx, sfty,  gap,  rot;
  float   pnm,  eml,   bas,  shr0,  shr1;
  int     n;
  float   x0,y0,z0, x1,y1,z1;

  int nmarks=0;
  int ncols=0;

  FILE *fp = fopen( file,"r");
  char            buffer[256]="";

  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%d %d %d %d %d",  &mod, &pln, &prg, &plt, &pos   );
  if( ncols < 4 ) 
    { printf("ERROR: EdbMarksSet::ReadDA: wrong format\n");    return 0; }
  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%f %f %f %f",     &sftx, &sfty,  &gap,  &rot     );
  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%f %f %f %f %f",  &pnm, &eml, &bas, &shr0, &shr1 );
  if( ncols < 4 ) 
    { printf("ERROR: EdbMarksSet::ReadDA: wrong format\n");    return 0; }

  for(int i=0; i<10000; i++) {
    if (fgets (buffer, 256, fp) == NULL) break;
    ncols = sscanf( buffer,"%d %f %f %f %f %f %f",  
		     &n, &x0, &y0, &z0, &x1, &y1, &z1 );
    if( ncols < 7 )                                        break;
    eAbsolute->AddMark(n,x0,y0);
    eStage->AddMark(n,x1,y1);
    nmarks++;
  }

  printf("%d marks are read from file %s\n", nmarks, file);
  return nmarks;
}

//______________________________________________________________________________
Int_t  EdbMarksSet::ReadDollar( char *file, EdbMarksBox *mbox )
{ 
  //$B - Fiducial marks (Chorus coordinates): -> NFID
  //
  //    mod pln prd plt   pos
  //     2   5   11  -1                   <- plate line
  //    sftx  sfty  gap  rot
  //     0.    0.    0.   0.              <- position correction line
  //    pnm  eml   bas  shr0  shr1
  //     0.  100.  850.  2.               <- emulsion thikness line
  //     n     x         y     z           
  //     10 -339667. -209560.  0.         <- X-ray marks line
  //     12 -39704.  -209630.  0.
  //     13 -339646. -59477.   0.
  //     15 -39746.  -59533.   0.

  int     mod,  pln,   prg,  plt,  pos;
  float   sftx, sfty,  gap,  rot;
  float   pnm,  eml,   bas,  shr0,  shr1;
  int     n;
  float   x,y,z;

  int nmarks=0;
  int ncols=0;

  FILE *fp = fopen( file,"r");
  char            buffer[256]="";

  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%d %d %d %d %d",  &mod, &pln, &prg, &plt, &pos   );
  if( ncols < 4 ) 
    { printf("ERROR: EdbMarksSet::ReadDA: wrong format\n");    return 0; }
  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%f %f %f %f",     &sftx, &sfty,  &gap,  &rot     );
  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf( buffer,"%f %f %f %f %f",  &pnm, &eml, &bas, &shr0, &shr1 );
  if( ncols < 4 ) 
    { printf("ERROR: EdbMarksSet::ReadDA: wrong format\n");    return 0; }

  for(int i=0; i<10000; i++) {
    if (fgets (buffer, 256, fp) == NULL)                  break;
    ncols = sscanf( buffer,"%d %f %f %f",  &n, &x, &y, &z );
    if( ncols < 3 )                                       break;
    mbox->AddMark(n,x,y);
    nmarks++;
  }

  printf("%d marks are read from file %s\n", nmarks, file);
  return nmarks;
}

//______________________________________________________________________________
void EdbMarksSet::Print(Option_t *opt) const
{
  if(eAbsolute)      eAbsolute->Print();
  if(eStage)         eStage->Print();
}

//______________________________________________________________________________
EdbMarksBox::EdbMarksBox()
{
  eMarks = new TClonesArray("EdbMark",4);
}

//______________________________________________________________________________
EdbMarksBox::EdbMarksBox( int n )
{
  eMarks = new TClonesArray("EdbMark",n);
}

//______________________________________________________________________________
EdbMarksBox::~EdbMarksBox( )
{
  if(eMarks) delete eMarks;
}

//______________________________________________________________________________
void  EdbMarksBox::Draw(int style, int   col, float size, float tsiz )
{
  DrawPoints( style, col, size );

  if(tsiz<0)                       return;

  char lable[4];

#ifndef WIN32
  for( int i=0; i<N(); i++ ) {
    sprintf(lable,"%d",  GetMark(i)->GetID() );
    TText *t = new TText( At(i)->X(),  At(i)->Y(), lable );
    t->Draw();
    t->SetTextSize(tsiz);
  }
#endif
}

//______________________________________________________________________________
void EdbMarksBox::Print(Option_t *opt) const
{
  printf("EdbMarksBox: %d fiducials\n", GetN() );
  for(int i=0; i<GetN(); i++) GetMark(i)->Print();
}

//______________________________________________________________________________
Int_t EdbMarksBox::GetN()    const 
{ 
  if(eMarks) return eMarks->GetLast()+1; else return 0;
}

//______________________________________________________________________________
EdbMark      *EdbMarksBox::GetMark(int i) const 
{ 
  if(eMarks) return (EdbMark*) eMarks->At(i); 
  else       return 0;
}

//______________________________________________________________________________
void EdbMarksBox::AddMark(int id, float x, float y)
{
  new((*eMarks)[GetN()])  EdbMark( id,x,y );
}

//______________________________________________________________________________
void EdbMark::Print(Option_t *opt) const
{
  printf("EdbMark: %d %f %f\n", GetID(), GetX(), GetY() );
}
