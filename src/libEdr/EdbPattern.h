#ifndef ROOT_EdbPattern
#define ROOT_EdbPattern
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPattern                                                           //
//                                                                      //
// pattern of seglemts                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TTree.h"
#include "TObjArray.h"
#include "TArrayL.h"
#include "TMatrixD.h"
#include "EdbVirtual.h"

class EdbAffine2D;
class TIndexCell;


//______________________________________________________________________________
class EdbSegP : public TObject, public EdbTrack2D {
 
 private:
  Int_t      ePID;             // plate id
  Int_t      eID;              // segment id (unique in plate)
  Int_t      eVid[2];          // [0]-view entry in the input tree, [1]-segment entry in the view
  Int_t      eAid[2];          // [0]-AreaID, [1]-ViewID
  Int_t      eFlag;

  Float_t    eX , eY, eZ;      // coordinates
  Float_t    eTX, eTY;         // direction tangents

  Float_t    eSZ;              // square of the Z-erorr

  Float_t    eChi2;            // chi-square 
  Float_t    eProb;            // probability
  Float_t    eW;               // weight
  Float_t    eVolume;          // segment volume
  Float_t    eDZ;              // the length of segment along z-axis
  Float_t    eDZem;            // the length of segment along z-axis in the emulsion
  Float_t    eP;               // momentum of the particle

 protected: 
  TMatrixD   *eCOV;            // covariant matrix of the parameters (x,y,tx,ty,p)

 public:
  EdbSegP();
  EdbSegP(int id, float x, float y, float tx, float ty, float w=0, int flag=0)
    { Set(id,x,y,tx,ty,w,flag); eCOV=0; eZ=0; }
  virtual ~EdbSegP() { if(eCOV){delete eCOV; eCOV=0;} }
  EdbSegP(EdbSegP &s) { eCOV=0; Copy(s); }

  static void LinkMT(const EdbSegP* s1,const EdbSegP* s2, EdbSegP* s);
  void PropagateTo( float z );
  void MergeTo( EdbSegP &s );
  Float_t    ProbLink( EdbSegP &s1, EdbSegP &s2 );

  void    Copy(const EdbSegP &s);

  void    Set(int id, float x, float y, float tx, float ty, float w=0, int flag=0) 
    { eID=id; eX=x; eY=y; eTX=tx; eTY=ty; eW=w; eFlag=flag; }

  
  void    SetErrors( float sx2, float sy2, float sz2, float stx2, float sty2, float sp2=0 )
    { if(!eCOV) eCOV = new TMatrixD(5,5);
      else eCOV->Clear();
      (*eCOV)(0,0) = (double)sx2; 
      (*eCOV)(1,1) = (double)sy2; 
      (*eCOV)(2,2) = (double)stx2;
      (*eCOV)(3,3) = (double)sty2;
      (*eCOV)(4,4) = (double)sp2;
      eSZ = sz2;
    }
  
  void    SetErrorP( float sp2 )
    { if(!eCOV) eCOV = new TMatrixD(5,5);
      (*eCOV)(4,4) = (double)sp2;
    }
  

  void SetCOV( TMatrixD &cov) 
    { 
      if(!(&cov)) return;
      if(eCOV) eCOV->Copy(cov);
      else eCOV = new TMatrixD(cov);
    }

  void SetCOV( double *array, int dim=5) 
    { 
      if(!array) return;
      if(!eCOV)  eCOV = new TMatrixD(5,5);
      for(int k=0; k<dim; k++) 
	for(int l=0; l<dim; l++) (*eCOV)(k,l) = array[k*dim + l];
    }

  TMatrixD &COV() const {return *eCOV;}
  void     SetSZ( float sz )   { eSZ=sz; }

  void     SetZ( float z )   { eZ=z; }
  void     SetDZ( float dz )   { eDZ=dz; }
  void     SetDZem( float dz )   { eDZem=dz; }
  void     SetID( int id )   { eID=id; }
  void     SetPID( int pid ) { ePID=pid; }
  void     SetFlag( int flag ) { eFlag=flag; }
  void     SetW( float w )  { eW=w; }
  void     SetP( float p )  { eP=p; }
  void     SetProb( float prob )  { eProb=prob; }
  void     SetChi2( float chi2 )  { eChi2=chi2; }
  void     SetVolume( float w )  { eVolume=w; }
  void     SetVid(int vid, int sid) { eVid[0]=vid; eVid[1]=sid; }
  void     SetAid(int a,   int v)   { eAid[0]=a; eAid[1]=v; }

  void     SetProbability( float p ) { eProb=p; }

  Int_t    ID()     const {return eID;}
  Int_t    PID()    const {return ePID;}
  Int_t    Flag()   const {return eFlag;}
  Float_t  W()      const {return eW;}
  Float_t  P()      const {return eP;}
  Float_t  Z()      const {return eZ;}
  Float_t  DZ()     const {return eDZ;}
  Float_t  DZem()   const {return eDZem;}
  Float_t  Prob()   const {return eProb;}
  Float_t  Chi2()   const {return eChi2;}
  Float_t  Volume() const {return eVolume;}

  Float_t    SX()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(0,0); }
  Float_t    SY()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(1,1); }
  Float_t    STX() const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(2,2); }
  Float_t    STY() const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(3,3); }
  Float_t    SP()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(4,4); }
  Float_t    SZ()  const  { return eSZ; }
  Int_t      Vid(int i) const 
    {if(i<0) return -1; if(i>1) return -1; return eVid[i];}
  Int_t      Aid(int i) const 
    {if(i<0) return -1; if(i>1) return -1; return eAid[i];}

  // mandatory virtual functions:
  Float_t    X()  const  { return eX; }
  Float_t    Y()  const  { return eY; }
  Float_t    TX() const  { return eTX; }
  Float_t    TY() const  { return eTY; }
  void    SetX(  float x )   { eX=x; }
  void    SetY(  float y )   { eY=y; }
  void    SetTX( float tx ) { eTX=tx; }
  void    SetTY( float ty ) { eTY=ty; }
 
 //other functions
 
  void       Print( Option_t *opt="") const;
 
  ClassDef(EdbSegP,12)  // segment
};

//______________________________________________________________________________

class EdbSegmentsBox : public TObject, public EdbPointsBox2D {
 
 private:
 
  Float_t eX,eY,eZ;         // central point

  TClonesArray *eSegments;  // collection of segments (EdbSegP)

  Float_t eDZkeep;           // eDZkeep = eZ - Zoriginal (before any projections)

  //  void (*PF_COUPLE)(EdbSegP *seg1,EdbSegP *seg2); //pointer to the function

 protected:
  void       SetZ(float z) { eZ=z; }

 public:
  EdbSegmentsBox(int nseg=0);
  EdbSegmentsBox(EdbSegmentsBox &box)
    {
      eX=box.X();
      eY=box.Y();
      eZ=box.Z();
      int n=box.N();
      eSegments = new TClonesArray("EdbSegP",n);
      for(int i=0; i<n; i++) 
	AddSegment( *(box.GetSegment(i)) );
    }

  EdbSegmentsBox(float x0, float y0, float z0, int nseg=0);
  virtual ~EdbSegmentsBox();
 
  void   AddSegment(EdbSegP &s);
  void   AddSegment(EdbSegP &s1, EdbSegP &s2); 
  void   AddSegment(int id, float x, float y, float tx, float ty, 
		    float w=0, int flag=0);
  Int_t  GetN()    const 
    {if(eSegments) return eSegments->GetLast()+1; else return 0;}
  EdbSegP   *GetSegment(int i) const { return (EdbSegP*)eSegments->At(i); }
  //EdbSegP   *GetLastSegment() { return (EdbSegP*)eSegments->GetLast(); }
  TClonesArray *GetSegments()     const { return eSegments; }
  void          *GetSegmentsAddr()  { return &eSegments; }

  void SetSegmentsZ();
  // mandatory virtual functions:
  void       SetX(float x) { eX=x; }
  void       SetY(float y) { eY=y; }

  Float_t       X()       const { return eX; }
  Float_t       Y()       const { return eY; }
  Float_t       Z()       const { return eZ; }
  float        DZ()       const {return eDZkeep;}
  Int_t         N()       const { return GetN(); }
  EdbPoint     *At(int i)       { return (EdbPoint*)GetSegment(i); }
 
  // other finctions
  void   Print( Option_t *opt="") const;
 
  void Set0();
  void Reset();
  void ProjectTo(const float z);

  int CalculateXY(  EdbSegmentsBox *p , EdbAffine2D *aff  );
  int CalculateAXAY(EdbSegmentsBox *p , EdbAffine2D *affA );

  void TransformA(    const EdbAffine2D *affA );
  void TransformARot( const EdbAffine2D *affA );
  void TransformShr(  const float shr );

  float DiffAff( EdbAffine2D *aff );
  float Diff( EdbSegmentsBox &p );

  ClassDef(EdbSegmentsBox,1)  // collection of segments
};

//______________________________________________________________________________

class EdbTrackP : public EdbSegP {
 
 private:
  EdbSegP  eSegZmin;
  EdbSegP  eSegZmax;

  EdbSegmentsBox  *eS;    //! array of segments
  TArrayL         *eVid;  //! volume-wide segments id's

 public:
  EdbTrackP(int nseg=0);
  EdbTrackP(EdbTrackP &track) : EdbSegP( *((EdbSegP *)&track) )
    {
      eS = new EdbSegmentsBox(*(track.S()));
      eSegZmin.Copy(*track.TrackZmin());
      eSegZmax.Copy(*track.TrackZmax());
    }
  virtual ~EdbTrackP();

  EdbSegmentsBox *S() const { return eS; }

  const EdbSegP  *TrackZmin() const {return &eSegZmin;}
  const EdbSegP  *TrackZmax() const {return &eSegZmax;}

  void     AddSegment(EdbSegP &s)  
    { 
      if(!eS) eS = new EdbSegmentsBox();
      eS->AddSegment(s);
    }

  EdbSegP *GetSegment(int i) const {if(eS) return eS->GetSegment(i); else return 0; }
  int      N() const  {if(eS)  return eS->N(); else return 0; }
  void     Reset()    { if(eS) eS->Reset(); }

  void Copy(const EdbTrackP &tr);
  void FitTrack();
  int FitTrackKF( float mass, bool zmax=false );
  int FitTrackKFS( float mass, bool zmax=false );

  static double ThetaPb2( float p, float dPath, float mass);

  float CHI2();

  TArrayL  *GetVid() const { return eVid; }
  void SetVid(int n) {if(eVid) delete eVid; eVid = new TArrayL(n);}

  void Clear() {if(eS) eS->Reset();}

  void Print() 
    { 
      printf("EdbTrackP with %d segments:\n", N());
      ((EdbSegP*)this)->Print(); 
      if(eS) eS->Print(); 
    }

  ClassDef(EdbTrackP,4)  // track consists of segments
};

//______________________________________________________________________________

class EdbPattern : public EdbSegmentsBox {
 
 private:
  Int_t       eID;          // pattern id in the volume
  TIndexCell *eCell;        //! associated with eSegments

  Float_t eSigma[4];          // accuracy in comparison to neibour pattern

 public:
  EdbPattern();
  EdbPattern(float x0, float y0, float z0);
  virtual ~EdbPattern();
 
  TIndexCell *Cell() const {return eCell;}
  void Set0();
  void SetID(int id) {eID=id;}
  void SetSigma(float sx,float sy,float stx,float sty) 
    { eSigma[0]=sx; eSigma[1]=sy; eSigma[2]=stx; eSigma[3]=sty; }
  void Reset();

  int  ID() const {return eID;}

  ClassDef(EdbPattern,1)  // pattern of segments
};

//______________________________________________________________________________
class EdbPatternsVolume : public TObject {
 
 private:
 
  Float_t    eX,eY,eZ;       // central point

 public:

  TObjArray  *ePatterns;      // collection of patterns

  TIndexCell *eTracksCell;    //! "vidt:vids" - connected segments cell
  TIndexCell *ePatternsCell;  //! "pid:id1:chi2:id2" - all found couples

  TObjArray *eTracks;         //! array of tracks

  EdbPatternsVolume   *eSV;  // volume for selected stuff

 public:
  EdbPatternsVolume();
  EdbPatternsVolume(EdbPatternsVolume &pvol);
  virtual ~EdbPatternsVolume();

  void Set0();
  void SetPatternsID();

  void Transform( const EdbAffine2D *aff );
  void Shift(float x, float y);
  void Centralize();
  void Centralize(float xc, float yc);
  void SetXYZ( float x, float y, float z ) { eX=x; eY=y; eZ=z; }

  Float_t X() const {return eX;}
  Float_t Y() const {return eY;}
  Float_t Z() const {return eZ;}
  Int_t   Npatterns() const { if(ePatterns) 
                        return ePatterns->GetEntries();
                        else return 0; }

  void PassProperties(EdbPatternsVolume &pvol);

  void         AddPattern( EdbPattern *pat );
  EdbPattern  *GetPattern( int id ) const;


  void DropCell();

  void   PrintStat( Option_t *opt="") const;
  void   PrintStat( EdbPattern &pat ) const;
  void   PrintAff() const;
  int    DropCouples();

  Long_t Vid(int pid, int sid) { return pid*1000000+sid; }
  Int_t  Pid(Long_t vid) { return vid/1000000; }
  Int_t  Sid(Long_t vid) { return vid%1000000; }

  ClassDef(EdbPatternsVolume,1)  // patterns nostri
};

#endif /* ROOT_EdbPattern */
