//-- Author :  Valeri Tioukov   16.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  EdbPrediction                                                       //
//                                                                      //
//  Predictions a-la $c                                                 //
//                                                                      //
//  $c data representation:
//  2 coordinates called as "Y" and "Z" for compatibility with CHORUS coord system,
//  but correspondent functions are X() and Y() to be 1-st and 2-d coord in base
//  classes
//
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_EdbFiducial
#include "EdbFiducial.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif

#ifndef ROOT_EdbPrediction
#include "EdbPrediction.h"
#endif

//#ifndef ROOT_EdbBasic
//#include "EdbBasic.h"
//#endif

ClassImp(EdbPredictionDC)
ClassImp(EdbPredictionsBox)

//_____________________________________________________________________________
void EdbPredictionDC::WriteDC( FILE *file ) const
{
  fprintf( file,"%i %i %i %f %f %f %f \n",
	   GetID(), GetEvent(), GetFlag(), TX(), TY(), X(),Y() );
}

//_____________________________________________________________________________
void EdbPredictionDC::Print( Option_t *opt ) const
{
  printf("EdbPredictionDC: %i %i %i %f %f %f %f \n",
	 GetID(), GetEvent(), GetFlag(), TX(), TY(), X(),Y() );
}


//______________________________________________________________________________
EdbPredictionsBox::EdbPredictionsBox()
{
  ePredictions = new TClonesArray("EdbPredictionDC",1000);
}

//______________________________________________________________________________
EdbPredictionsBox::EdbPredictionsBox(const EdbPredictionsBox &pb) : EdbPointsBox2D(pb)
{
  if(pb.GetPredictions()) ePredictions = new TClonesArray(*pb.GetPredictions());
  else ePredictions = new TClonesArray("EdbPredictionDC",1000);
}

//______________________________________________________________________________
EdbPredictionsBox::EdbPredictionsBox(int n)
{
  ePredictions = new TClonesArray("EdbPredictionDC",n);
}

//______________________________________________________________________________
EdbPredictionsBox::~EdbPredictionsBox()
{
  if(ePredictions) delete ePredictions;
}

//______________________________________________________________________________
Int_t  EdbPredictionsBox::GetN()    const
{ 
  if(ePredictions) return ePredictions->GetLast()+1; else return 0;
}

//______________________________________________________________________________
void     EdbPredictionsBox::WriteDC( char *file ) const
{
  FILE *fp = fopen( file,"w");
  if( N()>0 ) for(int i=0; i<N(); i++ ) GetPrediction(i)->WriteDC(fp);
  fclose(fp);
}

//______________________________________________________________________________
void     EdbPredictionsBox::WriteDC1( char *file ) const
{
  // temporary routine to renumber the tracks

  EdbPredictionDC *p = 0;
  FILE *fp = fopen( file,"w");

  if( N()>0 ) for(int i=0; i<N(); i++ ) {

    p = GetPrediction(i);

    fprintf( fp,"%i %i %i %f %f %f %f \n",
	     i, 
	     p->GetEvent(), 
	     p->GetFlag(), 
	     p->TX(), 
	     p->TY(), 
	     p->X(),
	     p->Y() 
	     );

  }
  fclose(fp);
}

//______________________________________________________________________________
TNtuple     *EdbPredictionsBox::MakeNtuple( char *name ) const
{
  TNtuple *nt = new TNtuple( name, "$c predictions", "id:event:flag:ty:tz:y:z" );
  EdbPredictionDC *pdc=0;

  if( N()>0 ) {
    for(int i=0; i<N(); i++ ) {
      pdc = GetPrediction(i);
      nt->Fill( pdc->GetID(),
		pdc->GetEvent(),
		pdc->GetFlag(),
		pdc->GetTy(),
		pdc->GetTz(),
		pdc->GetYp(),
		pdc->GetZp() );
    }
  }
  return nt;
}

//______________________________________________________________________________
void     EdbPredictionsBox::Print( Option_t *opt) const
{
  printf("EdbPredictionsBox: %d elements\n", N() );
  if( N()>0 ) for(int i=0; i<N(); i++ ) GetPrediction(i)->Print();
}

//______________________________________________________________________________
void     EdbPredictionsBox::AddPrediction( int id, int event, int flag, 
					 float ty,  float tz,  float y,  float z )
{
  new((*ePredictions)[GetN()])  
    EdbPredictionDC( id, event, flag, 
		     ty,  tz,  y,  z );
}

//______________________________________________________________________________
void     EdbPredictionsBox::Generate( int n )
{
  for( int i=0; i<n; i++ ) {

    AddPrediction( i, i, 0, 0, 0, 1000*i, 1000*i );

  }
}

//______________________________________________________________________________
Int_t     EdbPredictionsBox::ReadDC( char* file )
{
  FILE *fp = fopen( file,"r");
  char            buffer[256]="";

  int ncols=0;

  // read header
  int      module, stack, period, plate;
  float    a1, a2;
  if (fgets (buffer, 256, fp) == NULL) return 0;
  ncols = sscanf(buffer,"%d %d %d %d %f %f",
              &module, &stack, &period, &plate, &a1, &a2);

  // read tracks
  int     ntr = 0;
  int     id, ev, flag;
  float   ty, tz, y, z;
  for( int i=0; i<100000; i++ ) {
    if (fgets (buffer, 256, fp) == NULL) break;
    ncols = sscanf(buffer,"%d %d %d %f %f %f %f",
		    &id, &ev, &flag, &ty, &tz, &y, &z);
    if (ncols != 7 && ncols>0) {
      printf("*** EmuDCTrack::Read  Error: ncolomns =%d  != 7", ncols );
      ncols = -1;
      break;
    }
    if(ncols<=0) break;
    AddPrediction( id, ev, flag, ty, tz, y, z );
    ntr++;
  }
  fclose(fp);

  printf("%d tracks are readed from file %s\n", ntr, file );
  return ntr;
}

//______________________________________________________________________________
Int_t     EdbPredictionsBox::ReadDC0( char* file )
{
  // read dc file with no header

  FILE *fp = fopen( file,"r");
  char            buffer[256]="";

  int ncols=0;

  // read header
//    int      module, stack, period, plate;
//    float    a1, a2;
//    if (fgets (buffer, 256, fp) == NULL) return 0;
//    ncols = sscanf(buffer,"%d %d %d %d %f %f",
//                &module, &stack, &period, &plate, &a1, &a2);

  // read tracks
  int     ntr = 0;
  int     id, ev, flag;
  float   ty, tz, y, z;
  for( int i=0; i<100000; i++ ) {
    if (fgets (buffer, 256, fp) == NULL) break;
    ncols = sscanf(buffer,"%d %d %d %f %f %f %f",
		    &id, &ev, &flag, &ty, &tz, &y, &z);
    if (ncols != 7 && ncols>0) {
      printf("*** EmuDCTrack::Read  Error: ncolomns =%d  != 7", ncols );
      ncols = -1;
      break;
    }
    if(ncols<=0) break;
    AddPrediction( id, ev, flag, ty, tz, y, z );
    ntr++;
  }
  fclose(fp);

  printf("%d tracks are readed from file %s\n", ntr, file );
  return ntr;
}

//______________________________________________________________________________
EdbPredictionDC      *EdbPredictionsBox::GetPrediction(int i) const
{
  if(ePredictions) return (EdbPredictionDC*) ePredictions->At(i); 
  else       return 0;
}

