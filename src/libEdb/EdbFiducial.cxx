//-- Author :  Valeri Tioukov   12.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFiducial                                                          //
//                                                                      //
// Implementation of Fiducial classes                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#ifndef WIN32
#include "TText.h"
#endif

#include <TClass.h>
#include <TMath.h>
#include "EdbFiducial.h"
#include "EdbAffine.h"
#include "EdbLog.h"

ClassImp(EdbArea)
ClassImp(EdbMark)
ClassImp(EdbMarksBox)
ClassImp(EdbMarksSet)


//______________________________________________________________________________
void EdbMark::Print(Option_t *opt) const
{
  printf("EdbMark: %d %f %f %d\n", GetID(), GetX(), GetY(), Flag() );
}
//______________________________________________________________________________
void EdbMark::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbMark.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbMark::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      EdbPoint2D::Streamer(R__b);
      R__b >> eID;
      R__b >> eX;
      R__b >> eY;
      R__b.CheckByteCount(R__s, R__c, EdbMark::IsA());
      //====end of old versions
   } else {
     EdbMark::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
EdbArea::EdbArea( int N, float stepx, float stepy, int ft, int fb, int path ) : EdbMarksBox(N)
{
  Set( N, stepx, stepy, ft, fb, path );
}

//______________________________________________________________________________
void EdbArea::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbArea.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 2) {
	EdbArea::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      EdbMarksBox::Streamer(R__b);
      R__b >> eN;
      R__b >> eStepX;
      R__b >> eStepY;
      R__b >> eFramesTop;
      R__b >> eFramesBot;
      R__b >> ePath;
      R__b.CheckByteCount(R__s, R__c, EdbArea::IsA());
      //====end of old versions
   } else {
     EdbArea::Class()->WriteBuffer(R__b,this);
   }
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
void EdbArea::Set( int N, float stepx, float stepy, int ft, int fb, int path, int nx, int ny )
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
    if(ePath==2) MakeSerpentina(eN, x, y, nx, ny);

    for(int i=0; i<eN; i++){
      AddMark(i,x[i],y[i]);
    }

    Scale(stepx,stepy);
    delete [] x;
    delete [] y;
  }
}

//______________________________________________________________________________
int   EdbArea::MakeSerpentina( int N, int *x, int *y, int nx, int ny )
{
  // convention for the starting point: (0,0)
  // first increase x then y
 
  if(nx<=0) return 0;
  if(ny<=0) return 0;
  int dx = 1;  
  if(nx<=1) dx=0;
  int i=0;
  x[i]=0;
  y[i]=0;
  for(i=1; i<N; i++) {
    if(i%nx==0) {
      dx = -1*dx;
      x[i] = x[i-1];
      y[i] = y[i-1]+1;
    } else {
      y[i] = y[i-1];
      x[i] = x[i-1]+dx;
    }
  }
  return i+1;
}

//______________________________________________________________________________
int   EdbArea::MakeSpiralPath( int N, int *x, int *y)
{
  //convention: n=0 - central point
 
  int step[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };   // loop directions
 
  int a = 0;
  int b = 0;
  int k = 0;

  int i;
  for( i=0; i<N; i++ ) {
    
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
  return i+1;
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
  eXmin=eXmax=eYmin=eYmax=0;  eBrick=0;
  eAbsolute     = new EdbMarksBox();
  eStage        = new EdbMarksBox();
}

//______________________________________________________________________________
EdbMarksSet::EdbMarksSet( EdbMarksSet &ms )
{
  if(ms.GetAbsolute()) eAbsolute = new EdbMarksBox( *(ms.GetAbsolute()) );
  else                 eAbsolute     = new EdbMarksBox();
  if(ms.GetStage())    eStage    = new EdbMarksBox( *(ms.GetStage()) );
  else                 eStage        = new EdbMarksBox();
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
Int_t  EdbMarksSet::ChangeMapStringSpacer( TString &str, char oldspacer, char newspacer)
{
  // Changes the spacer inside the map string with a new character
  for (int is=0;is<str.Length();is++) 
    if(str[is]==oldspacer) str[is]=newspacer;

  return(1);
}

//______________________________________________________________________________
Int_t  EdbMarksSet::ReadMap( char *file, char spacer)
{
  // Reads map file and adds informations to eAbsolute
  Int_t nmarks = 0;
  Int_t mark, flag;
  Float_t x, y;

  FILE *fp = fopen( file,"r");

  TString str_header = "mapext:_%ld_%*d_%*d_%*d;_%d_%f_%f_%f_%f";
  TString str_mark = ";_%d_%f_%f_%*f_%*f_%*d_%*d_%d";

  ChangeMapStringSpacer(str_header,'_',spacer);
  ChangeMapStringSpacer(str_mark,'_',spacer);

  if(!fp)
    { Log(1,"EdbMarksSet::ReadMap","ERROR: can not open file %s\n", file); return 0; }

  if ( fscanf(fp, str_header.Data(), &eBrick,&nmarks,&eXmin,&eYmin,&eXmax,&eYmax) != 6 )
    { Log(1,"EdbMarksSet::ReadMap","ERROR: file %s is empty or wrong format\n", file); return 0; }

  GetAbsolute()->GetMarks()->Clear("C");

  for (int imarks=0;imarks<nmarks;imarks++)
    {
      if (fscanf(fp, str_mark.Data(), &mark, &x, &y, &flag) != 4)
	{ Log(1,"EdbMarksSet::ReadMap","ERROR: file %s is empty or wrong format\n", file); return 0; }
      GetAbsolute()->AddMark(mark,x,y,flag);
    }

  fclose(fp);

  Log(3,"EdbMarksSet::ReadMap","%d marks are read from file %s\n", nmarks, file);
  return nmarks;
}

//______________________________________________________________________________
Int_t  EdbMarksSet::WriteMap( char *file, char spacer)
{
  // Create a map string starting from the informations stored in this class
  // and in the EdbMarksBox object eAbsolute

  FILE *fp = fopen( file,"w");

  TString str_header = "mapext:_%ld_1_0_0;_%d_%.4f_%.4f_%.4f_%.4f";
  TString str_mark = ";_%d_%0.4f_%0.4f_%0.4f_%0.4f_1_1_%d";

  ChangeMapStringSpacer(str_header,'_',spacer);
  ChangeMapStringSpacer(str_mark,'_',spacer);

  if(!fp)
    { Log(1,"EdbMarksSet::ReadMap","ERROR: can not open file %s\n", file); return 0; }
  
  int nmarks = GetAbsolute()->GetN();
  
  fprintf(fp, str_header.Data(), eBrick,nmarks,eXmin,eYmin,eXmax,eYmax);
  
  for (int imarks=0;imarks<nmarks;imarks++)
    {
      EdbMark *mark = GetAbsolute()->GetMark(imarks);
      fprintf(fp, str_mark.Data(), mark->GetID(),mark->GetX(),mark->GetY(),mark->GetX(),mark->GetY(),mark->Flag());
    }
  
  fclose(fp);

  Log(3,"EdbMarksSet::WriteMap","%d marks are written as string map in the file %s\n", nmarks, file);
  return nmarks;
}

//______________________________________________________________________________
void EdbMarksSet::Print(Option_t *opt) const
{
  printf("EdbMarksSet: eBrick = %ld\n", eBrick);
  printf("EdbMarksSet: eXmin, eYmin, eXmax, eYmax = %f %f %f %f\n", eXmin, eYmin, eXmax, eYmax);
  printf("EdbMarksSet: Absolute mark set:\n");
  if(eAbsolute)      eAbsolute->Print();
  printf("EdbMarksSet: Stage mark set:\n");
  if(eStage)         eStage->Print();
}

//______________________________________________________________________________
void EdbMarksSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbMarksSet.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbMarksSet::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      TObject::Streamer(R__b);
      R__b >> eAbsolute;
      R__b >> eStage;
      R__b.CheckByteCount(R__s, R__c, EdbMarksSet::IsA());
      //====end of old versions
   } else {
     EdbMarksSet::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
EdbMarksBox::EdbMarksBox()
{
  eMarks = new TClonesArray("EdbMark",4);
}

//______________________________________________________________________________
EdbMarksBox::EdbMarksBox( const EdbMarksBox &mb ) : EdbPointsBox2D( mb )
{
  if(mb.GetMarks()) eMarks = new TClonesArray( *(mb.GetMarks()) );
  else              eMarks = new TClonesArray("EdbMark",4);
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
void EdbMarksBox::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbMarksBox.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbMarksBox::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution  (version 1)
      TObject::Streamer(R__b);
      EdbPointsBox2D::Streamer(R__b);
      eMarks->Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, EdbMarksBox::IsA());
      //====end of old versions
   } else {
     EdbMarksBox::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void  EdbMarksBox::Draw(int style, int   col, float size, float tsiz )
{
  DrawPoints( style, col, size );

  if(tsiz<0)                       return;

  char lable[4];

#ifndef WIN32
  int n=N();
  for( int i=0; i<n; i++ ) {
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
  int n=GetN();
  for(int i=0; i<n; i++) GetMark(i)->Print();
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
void EdbMarksBox::AddMark(int id, float x, float y, int flag)
{
  new((*eMarks)[GetN()])  EdbMark( id,x,y,flag );
}
