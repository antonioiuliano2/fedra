#ifndef ROOT_EdbImage
#define ROOT_EdbImage
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbImage                                                             //
//                                                                      //
// CCD Image in bytemap format                     r                    //
//                                     (2x3) :   c 012    012           //
//     convention about rows&columns:              1      345           //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObject
#include "TObject.h"
#endif

#ifndef ROOT_TArrayC
#include "TArrayC.h"
#endif

class TH2F;
class TH1F;

//______________________________________________________________________________
class EdbImage : public TObject {
 
private:

  Int_t       eColumns;          // number of columns of the matrix (1 row length)
  Int_t       eRows;             // number of rows of the matrix

  Int_t       eColors;           // maximal height of pixel (normally 256)

  TArrayC    *eBuffer;           // body of the image buffer
  Int_t       eBytes;            // number of significative bytes in the buffer

public:
  EdbImage();
  EdbImage( char *file, char *format="PGM" );
  EdbImage( int columns, int rows, char *image );
  virtual ~EdbImage();

  virtual void   Set0();
  virtual void   Print( Option_t *opt="" ) const;

  char   *GetBuffer()         const { return eBuffer->GetArray(); }
  char    Pixel(int c, int r) const { return eBuffer->At(eColumns*r + c); }
  TH2F   *GetHist2(int flip=0) const;
  TH1F   *GetHist1() const;

  Int_t   Width()  const {return eColumns;}
  Int_t   Height() const {return eRows;}
  Int_t   LoadPGM( char *file );
  Int_t   LoadRAW( char *file );
  Int_t   LoadBMP( char *file );

  ClassDef(EdbImage,1)  // CCD Image in bytemap format
};

//______________________________________________________________________________
class EdbFIRF : public TObject {

private:
   
   TArrayC     *eArr;
   Int_t       eColumns;          // number of columns of the matrix (1 row length)
   Int_t       eRows;             // number of rows of the matrix

public:
           EdbFIRF(){};
           EdbFIRF(int cols, int rows);
   virtual ~EdbFIRF();

   char   Cell(int x, int y) { return eArr->At(eColumns*y + x); }
   char  *GetBuffer()         const { return eArr->GetArray(); }
   void   SetAt(int x,int y, char a) { (eArr->GetArray())[eColumns*y + x]=a; }
   void   Reflect4(); // Copy left top corner to other quadrants with reflection
   void   Print();
   TH2F*  ApplyTo(EdbImage* img);

  ClassDef(EdbFIRF,1)  // FIR filter
};


#endif /* ROOT_EdbImage */

