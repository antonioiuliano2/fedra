#ifndef ROOT_EdbFiducial
#define ROOT_EdbFiducial
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFiducial                                                          //
//                                                                      //
// fiducial marks                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"

#ifndef ROOT_EdbVirtual
#include "EdbVirtual.h"
#endif

class EdbAffine2D;

//______________________________________________________________________________
class EdbMark : public TObject, public EdbPoint2D {

 private:
  Int_t      eID;              // fiducial mark id
  Float_t    eX, eY;           // coordinates

 public: 
  EdbMark(){}
  EdbMark(int id, float x, float y): eID(id), eX(x), eY(y) {}
  virtual ~EdbMark(){}

  //members access functions
  Int_t   GetID()   const { return eID; }
  Float_t GetX()    const { return eX;  }
  Float_t GetY()    const { return eY;  }
  void    Set(int id, float x, float y) { eID=id; eX=x; eY=y; }

  // mandatory virtual functions:
  Float_t    X() const  { return GetX(); }
  Float_t    Y() const  { return GetY(); }
  void    SetX( float x ) { eX=x; }
  void    SetY( float y ) { eY=y; }

 //other functions

  void       Print( Option_t *opt="") const;

  ClassDef(EdbMark,1)  // fiducial mark
};

//______________________________________________________________________________
class EdbMarksBox : public TObject, public EdbPointsBox2D {

 private:

  TClonesArray *eMarks;  // collection of fiducial marks

 public: 
  EdbMarksBox();
  EdbMarksBox( int n );
  virtual ~EdbMarksBox();


  //members access functions
  void          AddMark(int id, float x, float y);
  Int_t         GetN()         const;
  EdbMark      *GetMark(int i) const;
  TClonesArray *GetMarks()     const { return eMarks; }

  // mandatory virtual functions:
  Int_t         N()       const { return GetN(); }
  EdbPoint     *At(int i)       { return (EdbPoint*)GetMark(i); }

  void  Draw(int style=23, int   col=4, float size=1., float tsiz=.03 );

  // other finctions
  void       Print( Option_t *opt="") const;

  ClassDef(EdbMarksBox,1)  // collection of fiducial marks
};


//______________________________________________________________________________
class EdbMarksSet : public TObject {

 private:
  
  EdbMarksBox   *eAbsolute;       // fiducial marks in absolute coord ($b)
  EdbMarksBox   *eStage;          // fiducial marks in stage    coord ($a)

 public: 
  EdbMarksSet();
  virtual ~EdbMarksSet();

  Int_t           GetN()     const;
  Int_t           GetNa()    const;
  Int_t           GetNs()    const;

  EdbMarksBox    *GetAbsolute()  const { return eAbsolute; }
  EdbMarksBox    *GetStage()     const { return eStage;    }

  EdbAffine2D    *Abs2Stage()    const;
  EdbAffine2D    *Stage2Abs()    const;

  Int_t           ReadDA( char *file );
  Int_t           ReadDB( char *file );
  Int_t           ReadDollar( char *file, EdbMarksBox *mbox );

  void            DrawA( Option_t *opt="") const;
  void            DrawS( Option_t *opt="") const;

  void            Print( Option_t *opt="") const;

  ClassDef(EdbMarksSet,1)  // $b and $a
};

//______________________________________________________________________________
class EdbArea : public EdbMarksBox {

 private:
  
  Int_t     eN;            // number of views in the area
  
  Float_t   eStepX;        // distance between zones
  Float_t   eStepY;

  Int_t     eFramesTop;    // number of frames to scan
  Int_t     eFramesBot;

 public:
  EdbArea( int n=1, float stepx=1, float stepy=1, int ft=0, int fb=0 );
  virtual ~EdbArea(){}

  void   MakeSpiralPath( int n , int *x, int *y );

  void   Set( int n, float stepx, float stepy, int ft, int fb );

  void   SetCentre( float x, float y );
  void   Scale( float dx, float dy );

  Int_t        GetN()             const   { return eN; }
  Float_t      GetStepX()         const   { return eStepX; }
  Float_t      GetStepY()         const   { return eStepY; }
  Int_t        GetNframesTop()     const   { return eFramesTop; }
  Int_t        GetNframesBot()     const   { return eFramesBot; }

  Int_t      Nviews()        { return N(); }

  Float_t    Xview( int iv ) { return At(iv)->X(); }
  Float_t    Yview( int iv ) { return At(iv)->Y(); }

  int  ReadParameters( const char *str );
  void Print(Option_t opt=0) const;

  ClassDef(EdbArea,1)  // Area to scan
};

#endif /* ROOT_EdbFiducial */
