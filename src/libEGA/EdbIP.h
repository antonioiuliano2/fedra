#ifndef ROOT_EdbIP
#define ROOT_EdbIP

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbIP                                                                //
//                                                                      //
// Image processing classes                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "TArrayF.h"
#include "EdbCluster.h"

class EdbImage;
class EdbFrame;
class TTree;

//______________________________________________________________________________
class EdbFIRF : public TNamed {

private:
   
   TArrayF     *eArr;
   Int_t       eColumns;          // number of columns of the matrix (1 row length)
   Int_t       eRows;             // number of rows of the matrix

   static const Float_t eg3x3A[];   //!
   static const Float_t egHP1[];    //!
   static const Float_t egHP2[];    //!
   static const Float_t egHP3[];    //!
   static const Float_t eg5x5A[];   //!
   static const Float_t eg5x5B[];   //!
   static const Float_t eg5x5C[];   //!
   static const Float_t eg6x6A[];   //!

public:
           EdbFIRF(){};
           EdbFIRF(int cols, int rows);
	   EdbFIRF( const char *firf );
   virtual ~EdbFIRF();

   float  Cell(int x, int y) { return eArr->At(eColumns*y + x); }
   float  *GetBuffer()         const { return eArr->GetArray(); }
   void   SetArr(int n, float *arr){}
   void   SetAt(int x,int y, float a) { (eArr->GetArray())[eColumns*y + x]=a; }
   void   Reflect4(); // Copy left top corner to other quadrants with reflection
   void   Print();
   static void   PrintList();
   TH2F*  ApplyTo(EdbImage* img);

  ClassDef(EdbFIRF,1)  // FIR filter
};

//______________________________________________________________________________
class EdbClustP : public EdbCluster {

 private:

  Float_t  eXcg, eYcg;  // center of gravity with gray level
  Float_t  ePeak;       // peak height (above threshold)
  Float_t  eXp,  eYp;   // peak position

 public:
  EdbClustP(){};

  void SetCG(   float x, float y ) {eXcg=x; eYcg=y;}
  void SetPeak( float x, float y, float peak ) {eXp=x; eYp=y; ePeak=peak;}
  float Xcg()  const {return eXcg;}
  float Ycg()  const {return eYcg;}
  float Peak() const {return ePeak;}
  float Xp()   const {return eXp;}
  float Yp()   const {return eYp;}
  void AddPixel( float ic, float ir, float pix );
  void Reset();
  //void Print();

  ClassDef(EdbClustP,1)  // cluster reconstruction
};

//______________________________________________________________________________
class EdbIP : public TObject {

 private:
  EdbFIRF  *eFIR;
  Float_t   eThr;    // threshold

 public:
  EdbIP();

  TTree *InitTree();
  void SetFIR(EdbFIRF *fir) {eFIR = fir;}
  void SetThr(Float_t thr)  {eThr = thr;}

  static int   Peak8 ( TH2F *h, float thr );
  static int   Peak12( TH2F *h, float thr );
  int          CutBG( EdbFrame *frame );
  int          Clusterize( EdbFrame *frame, TTree *tree );
  static int   Clusterize( TH2F *h, float thr, TTree *tree, float z, int ifr );
  static float BurnPix( TH2F *h, int ic, int ir, float thr, EdbClustP &cl );

  ClassDef(EdbIP,1)  // Image Processing
};

#endif /* ROOT_EdbIP */
