//-- Author :  Valeri Tioukov   16.03.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVGen - Patterns Volume Generator                                 //
//                                                                      //
//  collection of agorithms                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TRandom.h"
#include "EdbPVGen.h"

ClassImp(EdbPVGen)

//______________________________________________________________________________________
void EdbPVGen::GenerateBeam( int n, float xx[4], float sxx[4], float lim[4], float z0, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  Float_t x0,y0,x,y,z,tx,ty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = new EdbSegP();

  int j=0;
  while(j<n) {

    x0 = xx[0] + sxx[0] * gRandom->Gaus();
    y0 = xx[1] + sxx[1] * gRandom->Gaus();
    if(x0<lim[0])                      continue; 
    if(y0<lim[1])                      continue; 
    if(x0>lim[2])                      continue; 
    if(y0>lim[3])                      continue; 
    j++;
    tx = xx[2] + sxx[2] * gRandom->Gaus();
    ty = xx[3] + sxx[3] * gRandom->Gaus();      // starting point (at z0)

    for (int i=0; i<pv->Npatterns(); i++ ) {

      pat = pv->GetPattern(i);
      z = pat->Z();

      x = x0 + tx*(z-z0);
      y = y0 + ty*(z-z0);

      seg->Set(j, x, y, tx, ty,1,0);
      seg->SetZ(z);
      seg->SetFlag(flag);
      pat->AddSegment( *seg );

    }
  }
}

//______________________________________________________________________________________
void EdbPVGen::GenerateBackground( int n, float xmin[4], float xmax[4], int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  Float_t x,y,z,tx,ty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = new EdbSegP();

  for( int j=0; j<n; j++ ) {

    for (int i=0; i<pv->Npatterns(); i++ ) {

      pat = pv->GetPattern(i);
      z = pat->Z();

      x  = xmin[0] + (xmax[0]-xmin[0]) * gRandom->Rndm();
      y  = xmin[1] + (xmax[1]-xmin[1]) * gRandom->Rndm();
      tx = xmin[2] + (xmax[2]-xmin[2]) * gRandom->Rndm();
      ty = xmin[3] + (xmax[3]-xmin[3]) * gRandom->Rndm();

      seg->Set(j, x, y, tx, ty,1,0);
      seg->SetZ(z);
      seg->SetFlag(flag);
      pat->AddSegment( *seg );

    }
  }
}

//______________________________________________________________________________
void EdbPVGen::GeneratePulsPoisson( float mean, float a, 
				    float wmin, float wmax, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;
  float w;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);
      if(flag>0)  if( seg->Flag() != flag)  continue;

    REGEN:
      w = a * gRandom->Poisson(mean);
      if(wmin>0) if(w<wmin) goto REGEN;
      if(wmax>0) if(w>wmax) w = wmax;

      seg->SetW( w );
    }
  }
}

//_________________________________________________________________________________________
void EdbPVGen::GeneratePulsGaus( float amp, float mean, float sigma, 
				 float wmin, float wmax, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;
  float w;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);
      if(flag>0)  if( seg->Flag() != flag)  continue;
      
    REGEN:
      w = (int)(amp * gRandom->Gaus(mean,sigma));
      if(wmin>0) if(w<wmin) goto REGEN;
      if(wmax>0) if(w>wmax) w = wmax;

      seg->SetW( w );
    }
  }
}

//______________________________________________________________________________
void EdbPVGen::SmearSegments()
{
  // smearing with parameters defined by ScanCond

  EdbPatternsVolume *pv = GetVolume();
  EdbScanCond *scan = eScanCond;

  Float_t x,y,z,tx,ty;
  Float_t sx,sy,sz,stx,sty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);

      sx  = scan->SigmaX(seg->TX());
      sy  = scan->SigmaY(seg->TY());
      sz  = 0.;
      stx = scan->SigmaTX(seg->TX());
      sty = scan->SigmaTY(seg->TY());

      x  = gRandom->Gaus(seg->X(),   sx);
      y  = gRandom->Gaus(seg->Y(),   sy);
      z  = gRandom->Gaus(seg->Z(),   sz);
      tx = gRandom->Gaus(seg->TX(), stx);
      ty = gRandom->Gaus(seg->TY(), sty);

      seg->Set( seg->ID(), x,y,tx,ty,seg->W(),seg->Flag());
      seg->SetZ( z );
      //seg->SetErrors( sx,sy,sz,stx,sty );

    }
  }
}

//______________________________________________________________________________
void EdbPVGen::SmearPatterns(float shift, float rot)
{
  EdbPatternsVolume *pv = GetVolume();
  EdbAffine2D a;

  for( int i=0; i<pv->Npatterns()-1; i++ ) {

    a.Rotate( gRandom->Gaus(0, rot)   );
    a.ShiftX( gRandom->Gaus(0, shift) );
    a.ShiftY( gRandom->Gaus(0, shift) );

    pv->GetPattern(i)->Transform(&a);
  }

}
