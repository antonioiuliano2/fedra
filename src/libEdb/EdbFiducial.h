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
  Int_t      eFlag;            // reserved for information top/bottom/lateral/damaged/etc

 public: 
  EdbMark(){}
  EdbMark(int id, float x, float y, int flag=0): eID(id), eX(x), eY(y), eFlag(flag) {}
  virtual ~EdbMark(){}

  //members access functions
  Int_t   GetID()   const { return eID; }
  Float_t GetX()    const { return eX;  }
  Float_t GetY()    const { return eY;  }
  void    Set(int id, float x, float y) { eID=id; eX=x; eY=y; }
  void    SetFlag(int flag) { eFlag=flag; }
  Int_t   Flag() const { return eFlag; }

  // mandatory virtual functions:
  Float_t    X() const  { return GetX(); }
  Float_t    Y() const  { return GetY(); }
  void    SetX( float x ) { eX=x; }
  void    SetY( float y ) { eY=y; }

 //other functions

  void       Print( Option_t *opt="") const;

  ClassDef(EdbMark,2)  // fiducial mark
};

//______________________________________________________________________________
class EdbMarksBox : public TObject, public EdbPointsBox2D {

 private:

  TClonesArray *eMarks;  // collection of fiducial marks

 public: 
  EdbMarksBox();
  EdbMarksBox( const EdbMarksBox &mb );
  EdbMarksBox( int n );
  virtual ~EdbMarksBox();


  //members access functions
  void          AddMark(int id, float x, float y, int flag=0);
  Int_t         GetN()         const;
  EdbMark      *GetMark(int i) const;
  TClonesArray *GetMarks()     const { return eMarks; }

  // mandatory virtual functions:
  Int_t         N()       const { return GetN(); }
  EdbPoint     *At(int i) const      { return (EdbPoint*)GetMark(i); }

  void  Draw(int style=23, int   col=4, float size=1., float tsiz=.03 );

  // other functions
  void       Print( Option_t *opt="") const;

  ClassDef(EdbMarksBox,2)  // collection of fiducial marks
};


//______________________________________________________________________________
class EdbMarksSet : public TObject {

 private:
  
  EdbMarksBox   *eAbsolute;       // fiducial marks in absolute coord ($b)
  EdbMarksBox   *eStage;          // fiducial marks in stage    coord ($a)

 public:
  Float_t  eXmin,  eXmax;
  Float_t  eYmin,  eYmax;
  Long_t   eBrick;

 public: 
  EdbMarksSet();
  EdbMarksSet( EdbMarksSet &ms );
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

  Int_t           ChangeMapStringSpacer( TString &str, char oldspacer, char newspacer);
  Int_t           ReadMap( char *file, char spacer='_', char shape='S');
  Int_t           WriteMap( char *file, char spacer='_', char shape='S', int plate=1);
  Int_t           ReadMap2( char *file, char spacer='_', char shape='S');
  Int_t           WriteMap2( char *file, char spacer='_', char shape='S', int plate=1);

  void            DrawA( Option_t *opt="") const;
  void            DrawS( Option_t *opt="") const;

  void            Print( Option_t *opt="") const;

  ClassDef(EdbMarksSet,2)  // $b and $a
};

//______________________________________________________________________________
class EdbArea : public EdbMarksBox {

 private:
  
  Int_t     eN;            // number of views in the area
  
  Float_t   eStepX;        // distance between zones
  Float_t   eStepY;

  Int_t     eFramesTop;    // number of frames to scan
  Int_t     eFramesBot;

  Int_t     ePath;         // path to be used (0-nopath, 1-spiral, 2-serpentine, ...)

 public:
  EdbArea( int n=1, float stepx=1, float stepy=1, int ft=0, int fb=0, int path=0 );
  virtual ~EdbArea(){}

  void   Set( int n, float stepx, float stepy, int ft, int fb, int path=1, int nx=0, int ny=0 );

  int   MakeSpiralPath( int n , int *x, int *y );
  int   MakeSerpentina( int n , int *x, int *y, int nx, int ny );

  void   SetCentre( float x, float y );
  void   Scale( float dx, float dy );

  Int_t        GetN()              const   { return eN; }
  Float_t      GetStepX()          const   { return eStepX; }
  Float_t      GetStepY()          const   { return eStepY; }
  Int_t        GetNframesTop()     const   { return eFramesTop; }
  Int_t        GetNframesBot()     const   { return eFramesBot; }
  Int_t        GetPath()           const   { return ePath; }

  Int_t      Nviews()        { return N(); }

  Float_t    Xview( int iv ) { return At(iv)->X(); }
  Float_t    Yview( int iv ) { return At(iv)->Y(); }

  int  ReadParameters( const char *str );
  void Print(Option_t opt=0) const;

  ClassDef(EdbArea,3)  // Area structure definition
};

#endif /* ROOT_EdbFiducial */
