//-- Author :  Valeri Tioukov   6.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbImage                                                             //
//                                                                      //
// Implementation of Image class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TH2.h"

#ifndef ROOT_EdbImage
#include "EdbImage.h"
#endif
#include "Riostream.h"
//#include <fstream>   //created problems under Windows
//using namespace std;
 
ClassImp(EdbImage)
ClassImp(EdbFIRF)

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
  else if    ( !strcmp(format,"BMP") )  counter = LoadBMP(file);

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
  printf("Image: %d x %d \t :  %d bytes\n", eColumns, eRows, eBytes);
}

//______________________________________________________________________________
TH1F *EdbImage::GetHist1() const
{
  TH1F *hist = new TH1F("pix","Pixels distribution", 
			eColors, 0, eColors);
  int i=0;
  unsigned char *buf = (unsigned char*)(eBuffer->GetArray());

  for (int icl=0; icl<eColumns; icl++) {
    for (int ir=0; ir<eRows; ir++) {
      i = ir*eColumns + icl;
      hist->Fill( (int)(buf[i]) );
    }
  }

  return hist;
}

//______________________________________________________________________________
TH2F *EdbImage::GetHist2(int flip) const
{
  TH2F *hist = new TH2F("ccd","CCD image", 
			eColumns, 0, eColumns, 
			eRows, 0, eRows);
  int i=0;
  unsigned char *buf = (unsigned char*)(eBuffer->GetArray());

  int pix;

  for (int icl=0; icl<eColumns; icl++) {
    for (int ir=0; ir<eRows; ir++) {
      i = ir*eColumns + icl;
      pix = (int)(buf[i]);
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
 */
  return 0;
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

//____________________________________________________________________________________
Int_t EdbImage::LoadBMP( char *file )
{
/*
  Loads a BMP format file...
  Author: Igor Kreslo
*/
typedef struct {
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */
} HEADER;

typedef struct {
   unsigned int size;               /* Header size in bytes      */
   int width,height;                /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   unsigned int compression;        /* Compression type          */
   unsigned int imagesize;          /* Image size in bytes       */
   int xresolution,yresolution;     /* Pixels per meter          */
   unsigned int ncolours;           /* Number of colours         */
   unsigned int importantcolours;   /* Important colours         */
} INFOHEADER;

HEADER hdr;
INFOHEADER ihdr;

 char* ptr;
 fstream raw(file,ios::in);
 ptr=(char*)&hdr;
 raw.read(ptr,14);
 ptr=(char*)&ihdr;
 raw.read(ptr,40);
 if(ihdr.bits!=8) { printf("Error: %d bits/pixel bitmap is not supported!\n",ihdr.bits); return -1;}
 int N=ihdr.width*ihdr.height;
 char* byte= new char[N];
 raw.read(byte,N);
 raw.close();
 eBytes=N;
 eColumns=ihdr.width;
 eRows=ihdr.height;
 eColors=256;

 eBuffer->Adopt(eBytes,byte);

 printf("EdbImage::LoadBMP: file %s  is readed...Image %d x %d pixels.\n", file,eColumns,eRows);

 return N;
}

//====================================================================================
const Float_t EdbFIRF::eg3x3A[]  = {
  1,  1, 1,   
  1, -8, 1,  
  1,  1, 1  };
const Float_t EdbFIRF::egHP1[] = {  
  1,  1, 1,   
  1, -9, 1,  
  1,  1, 1  };
const Float_t EdbFIRF::egHP2[] = {  
  0,  1, 0,   
  1, -5, 1,  
  0,  1, 0  };
const Float_t EdbFIRF::egHP3[] = { 
  -1,  2, -1, 
   2, -5,  2, 
  -1,  2, -1  };
const Float_t EdbFIRF::eg5x5A[] = { 
  4,  4,  4,  4,  4,
  4, -7, -7, -7,  4,
  4, -7, -7, -7,  4,
  4, -7, -7, -7,  4,
  4,  4,  4,  4,  4  };
const Float_t EdbFIRF::eg5x5B[] = { 
  4,  4,  4,  4,  4,
  4, -7, -7, -7,  4,
  4, -7, -8, -7,  4,
  4, -7, -7, -7,  4,
  4,  4,  4,  4,  4  };
const Float_t EdbFIRF::eg6x6A[] = { 
  4,  4,  4,  4,  4,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4, -5, -5, -5, -5,  4,
  4,  4,  4,  4,  4,  4  };


//____________________________________________________________________________________
EdbFIRF::EdbFIRF(int cols, int rows)
{
 eArr = new TArrayF();
 eArr->Adopt(cols*rows, new float[cols*rows]);
 eColumns=cols;
 eRows=rows;
 eArr->Reset();
}

//===============================================================================
EdbFIRF::EdbFIRF( const char *firf )
{
  SetName(firf);
  eArr = new TArrayF();
  if( !strcmp(firf,"HP1") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP1);
  } else if( !strcmp(firf,"HP2") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP2);
  } else if( !strcmp(firf,"HP3") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,egHP3);
  } else if( !strcmp(firf,"3x3A") ) {
    eColumns=3;
    eRows=3;
    eArr->Set(eColumns*eRows,eg3x3A);
  } else if( !strcmp(firf,"5x5A") ) {
    eColumns=5;
    eRows=5;
    eArr->Set(eColumns*eRows,eg5x5A);
  } else if( !strcmp(firf,"5x5B") ) {
    eColumns=5;
    eRows=5;
    eArr->Set(eColumns*eRows,eg5x5B);
  } else if( !strcmp(firf,"6x6A") ) {
    eColumns=6;
    eRows=6;
    eArr->Set(eColumns*eRows,eg6x6A);
  }
  Print();
}

//______________________________________________________________________________
EdbFIRF::~EdbFIRF()
{
  if(eArr) { delete eArr; eColumns=0; eRows=0; }
}

//______________________________________________________________________________
void    EdbFIRF::Reflect4()
{
 int X=eColumns;
 int Y=eRows;
 int HX=(int)((eColumns+1)/2);
 int HY=(int)((eRows+1)/2);
 for(int i=0;i<HY;i++){
   for(int j=0;j<HX;j++){
    SetAt(X-j-1,i,Cell(j,i));
    SetAt(X-j-1,Y-i-1,Cell(j,i));
    SetAt(j,Y-i-1,Cell(j,i));
   }
 }
}

//______________________________________________________________________________
void EdbFIRF::Print()
{
  printf("FIR filter: %s\n",GetName());
 float sum=0;
 for(int i=0;i<eRows;i++){
   for(int j=0;j<eColumns;j++){
     printf("%3.1f\t",Cell(j,i));
     sum+=Cell(j,i);
   }
   printf("\n");
 }
 printf("\nSum = %f\n",sum);
}

//______________________________________________________________________________
void EdbFIRF::PrintList()
{
  printf("known 3x3 filtres: \n\t3x3A \n\tHP1 \n\tHP2 \n\tHP3 \n");
  printf("known 5x5 filtres: \n\t5x5A \n\t5x5B \n");
  printf("known 6x6 filtres: \n\t6x6A \n");
}

//__________________________________________________________________________________
TH2F* EdbFIRF::ApplyTo(EdbImage* img)
{
  int x0=eColumns/2+1;
  int y0=eRows/2+1;
  int x1=img->Width()-x0-1;
  int y1=img->Height()-y0-1; //margins
  Float_t S;

  TH2F* buf=new TH2F("img","Filtered image",
		     img->Width(),0,img->Width(),img->Height(),0,img->Height());

  for(int y=y0;y<y1;y++) for(int x=x0;x<x1;x++){
   S=0;
   for(int yf=0;yf<eRows;yf++) for(int xf=0;xf<eColumns;xf++){
     S+=img->Pixel(x+xf-eColumns/2,y+yf-eRows/2)*Cell(xf,yf);
   }
   buf->Fill(x,y,S);
  }

  return buf;
}
