//-- Author :  Valeri Tioukov 1.07.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertexSpace                                                       //
//                                                                      //
// Class for vertex reconstruction                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TObjArray.h>
#include "EdbVertexSpace.h"

ClassImp(EdbVertexSpace);

//______________________________________________________________________________
EdbVertexSpace::EdbVertexSpace(float vmin[3], float vmax[3], float bin[3])
{
  for(int i=0; i<3; i++) {
    eVmin[i] = vmin[i];
    eVmax[i] = vmax[i];
    eBin[i]  = bin[i];
    eNcell[i] = (int)( (vmax[i]-vmin[i])/bin[i] );
  }

  Long_t N = eNcell[0]*eNcell[1]*eNcell[2];
  printf("create :EdbVertexSpace: %d %d %d = %ld\n",eNcell[0],eNcell[1],eNcell[2], N);
  if(N>10e9) { printf("N is too big - no space...\n"); return; }

  eCells = new TObjArray(N);
  return;
}

//______________________________________________________________________________
void EdbVertexSpace::AddTrackLine( float x1[3], float x2[3], int id)
{
  float zstep = eBin[2];
  float z = x1[2];
  int nstep = (x2[2]-x1[2])/zstep;

  for(int i=0; i<nstep; i++) {
  }
}

//______________________________________________________________________________
void EdbVertexSpace::Cycle()
{
  int ii[26];
  int jj[26];
  int kk[26];

  int ind=0;
  for(int i=-1; i<2; i++)
    for(int j=-1; j<2; j++)
      for(int k=-1; k<2; k++) {
	if( i==0&&j==0&&k==0 ) continue;
	ii[ind]=i;
	jj[ind]=j;
	kk[ind]=k;
	ind++;
      }

  if(!eCells) return;
  int n = eCells->GetSize();
  printf("cycle: %d\n",n);

  for(int i=1; i<eNcell[0]-1; i++) 
    for(int j=1; j<eNcell[1]-1; j++) 
      for(int k=1; k<eNcell[2]-1; k++) {
	ind = i*eNcell[0]+j*eNcell[1] + k;
	if( eCells->UncheckedAt(ind) )  printf("ops: %d\n",ind);

	for(int i2=0; i2<26; i2++) {
	  ind = 
	    (i+ii[i2])*eNcell[0] +
	    (j+jj[i2])*eNcell[1] +
	    (k+kk[i2]);
	  if( eCells->UncheckedAt(ind) )  printf("ops: %d\n",ind);
	}
      }
}
