//-- Author :  Valeri Tioukov   6.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbImage                                                             //
//                                                                      //
// Implementation of Image class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <fstream.h> 

#ifndef ROOT_EdbImage
#include "EdbImage.h"
#endif
 
ClassImp(EdbImage)

//______________________________________________________________________________
EdbImage::EdbImage()
{
  Set0();
}

//______________________________________________________________________________
EdbImage::EdbImage(char *file, char *format)
{
  Set0();
  eBuffer  = new TArrayC();

  Int_t counter = 0;

  if         ( !strcmp(format,"PGM") )  counter = LoadPGM(file);
  else if    ( !strcmp(format,"RAW") )  counter = LoadRAW(file);

  if(counter != eBytes) 
    printf("ERROR:  EdbImage::EdbImage: counter != eBytes \n");
}

//______________________________________________________________________________
EdbImage::~EdbImage()
{
  if(eBuffer) delete eBuffer;
  Set0();
}

//______________________________________________________________________________
void EdbImage::Set0()
{
  eRows    = 0;
  eColumns = 0;
  eBuffer  = 0;
  eBytes  = 0;
}

//______________________________________________________________________________
EdbImage::EdbImage( int columns, int rows, char *image )
{
  eColumns = columns;
  eRows    = rows;
  eBytes  = rows*columns;

  int save = 320;                        // 32 bytes safety margin

  int s = eBytes + save;
  char *buffer = new char[s];

  eBuffer = new TArrayC();

  int size = (int)strlen(image);     printf("strlen(image) = %d\n",size);

  strncpy(buffer,image,eBytes);

  eBuffer->Adopt(s,buffer);
}

//______________________________________________________________________________
void EdbImage::Print( Option_t *opt ) const
{
  printf("Image: %d x %d \t :  %d bytes\n", eRows, eColumns, eBytes);
}

//______________________________________________________________________________
TH1S *EdbImage::GetHist1() const
{
  TH1S *hist = new TH1S("pix","Pixels distribution", 
			eColors, 0, eColors);
  int i=0;
  unsigned char *buf = (unsigned char*)(eBuffer->GetArray());

  for (int icl=0; icl<eColumns; icl++) {
    for (int ir=0; ir<eRows; ir++) {
      i = ir*eColumns + icl;
      hist->Fill( (short)(buf[i]) );
    }
  }

  return hist;
}

//______________________________________________________________________________
TH2S *EdbImage::GetHist2(int flip) const
{
  TH2S *hist = new TH2S("ccd","CCD image", 
			eColumns, 0, eColumns, 
			eRows, 0, eRows);
  int i=0;
  unsigned char *buf = (unsigned char*)(eBuffer->GetArray());

  int pix;

  for (int icl=0; icl<eColumns; icl++) {
    for (int ir=0; ir<eRows; ir++) {
      i = ir*eColumns + icl;
      pix = (short)(buf[i]);
      if(flip) pix = eColors-pix;
      hist->Fill( icl, ir, pix );
    }
  }

  return hist;
}

//____________________________________________________________________________
Int_t EdbImage::LoadRAW( char *file )
{
/*
  Loads a .raw format file...
  Author: Alfredo Cocco
*/

  eColumns = 512;
  eRows    = 512;

  fstream raw(file, ios::in);
  char byte[262144];
  raw.read(byte,262144);
  raw.close();

  eBytes = 262144;
  int     save = 32;     //safety margins in bytes

  //  int index;
  //  int k = 0;
 
  //  for(int i=0; i<size; i++) {
  //    for(int j=0; j<size; j++) {
  //    index = j + i * size;
  //    buffer[index] = (int) byte[k++];
  //  }
  // }

  eBuffer->Adopt(eBytes+save,byte);
 
  return eBytes;
}

//____________________________________________________________________________
Int_t EdbImage::LoadPGM( char *file )
{
/*
  Loads a PGM format file...
  Author: Rosario Peluso
*/
  int      colors, pix, i, j, p2, p5=0;
  char     Id[10], Buffer[BUFSIZ];
  FILE    *in = fopen( file, "rb" );
  int     NRows, NCols; 
  int     save = 32;     //safety margins in bytes

  fgets( Id, 10, in );
  if( (p2 = strncmp(Id, "P2", 2)) && (p5 = strncmp(Id, "P5", 2)) )
    {
      printf("LoadPGM: Error Id\n");      return 0;
    }
 
  while( (fgets(Buffer, BUFSIZ, in) != NULL) && (*Buffer == '#') )
    ;
 
  if( sscanf(Buffer, "%d %d", &NRows, &NCols) != 2 )
    { printf("LoadPGM: Error NRows, &NCols\n");  return 0; }

  eRows    = NRows;
  eColumns = NCols;
  eBytes   = eRows*eColumns;
  
  if( fscanf(in, "%d\n", &colors) != 1 )
    { printf("LoadPGM: Error\n");  return 0; }

  eColors = colors;
 
  if( colors != 255 )
      printf("LoadPGM: Warning: colors = %d \n", colors);
 
  char *Pixel = new char[NRows * NCols + save];
 
  int counter = 0;
  if( !p2 )
    {
      for( j = 0; j < NRows; j++ )
	for( i = 0; i < NCols; i++ )
	  {
	    if( fscanf(in, "%d", &pix) != 1 )   printf("LoadPGM: Error\n");
	    Pixel[j * NCols + i] = pix;
	    counter++;
	  }
    }
  else if( !p5 && ((counter = fread((void *) Pixel, 1, NRows * NCols, in)) != eBytes) )
    {
      printf( "LoadPGM: Error loading binary file\n" );
      return( 0 );
    }
    
  fclose( in );

  eBuffer->Adopt(eBytes+save,Pixel);
 
  printf("EdbImage::LoadPGM: file %s  is readed...\n", file);
 
  return counter;
}
