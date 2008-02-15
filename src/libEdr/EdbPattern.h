#ifndef ROOT_EdbPattern
#define ROOT_EdbPattern
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPattern                                                           //
//                                                                      //
// pattern of seglemts                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TSortedList.h"
#include "TArrayL.h"
#include "TMatrixD.h"
#include "EdbVirtual.h"
#include "TClonesArray.h"
#include "TRefArray.h"

class EdbAffine2D;
class TIndexCell;
class EdbVTA;
class EdbVertex;

//______________________________________________________________________________
class EdbSegP : public TObject, public EdbTrack2D {
 
 private:
  Int_t      ePID;            // mother pattern ID
  Int_t      eID;             // segment id (unique in plate)
  Int_t      eVid[2];         // [0]-view entry in the input tree, [1]-segment entry in the view
  Int_t      eAid[2];         // [0]-AreaID, [1]-ViewID
  Int_t      eFlag;
  Int_t      eTrack;          // id of the track (-1) if no track
  Float_t    eX , eY, eZ;     // coordinates
  Float_t    eTX, eTY;        // direction tangents
  Float_t    eSZ;             // square of the Z-error
  Float_t    eChi2;           // chi-square 
  Float_t    eProb;           // probability
  Float_t    eW;              // weight
  Float_t    eVolume;         // segment volume
  Float_t    eDZ;             // the length of segment along z-axis
  Float_t    eDZem;           // the length of segment along z-axis in the emulsion
  Float_t    eP;              // momentum of the particle
  Int_t      eMCTrack;        // MC track number
  Int_t      eMCEvt;          // MC event number
  TRefArray *eEMULDigitArray; //! AM+AC 27/07/07

 protected: 

  TMatrixD   *eCOV;            // covariance matrix of the parameters (x,y,tx,ty,p)

 public:

  EdbSegP() {
    eEMULDigitArray =0;
    Set0();
  }
  EdbSegP(int id, float x, float y, float tx, float ty, float w=0, int flag=0);
  EdbSegP(const EdbSegP &s) {  
    eEMULDigitArray = 0;
    Set0(); Copy(s); 
  }

  virtual ~EdbSegP() { 
    SafeDelete(eCOV);
    SafeDelete(eEMULDigitArray);
  }

  static void LinkMT(const EdbSegP* s1,const EdbSegP* s2, EdbSegP* s);
  void    PropagateTo( float z );
  void    PropagateToCOV( float z );
  void    MergeTo( EdbSegP &s );
  Float_t ProbLink( EdbSegP &s1, EdbSegP &s2 );
  bool    IsCompatible(EdbSegP &s, float nsigx, float nsigt) const;

  void    addEMULDigit(TObject* a) {
    if(!eEMULDigitArray) eEMULDigitArray = new TRefArray();
    eEMULDigitArray->Add(a);
  }

  TRefArray* EMULDigitArray() const  { return eEMULDigitArray;}

  void    Set0();
  void    Copy(const EdbSegP &s);
  void    Clear() { eCOV->Clear(); }
  void    Set(int id, float x, float y, float tx, float ty, float w, int flag)
             { eID=id; eX=x; eY=y; eTX=tx; eTY=ty; eW=w; eFlag=flag; }
  void    SetErrors() {SetErrors( 1.,1.,0.,.0001,.0001,1.);}
  void    SetErrors( float sx2, float sy2, float sz2, float stx2, float sty2, float sp2=1. );
  void    SetErrorsCOV( float sx2, float sy2, float sz2, float stx2, float sty2, float sp2=1. );
  
  void    SetErrorP( float sp2 ) {
    if(!eCOV) eCOV = new TMatrixD(5,5);
    (*eCOV)(4,4) = (double)sp2;
  }
  
  void    SetCOV( TMatrixD &cov) { 
    if(!(&cov)) return;
    if(eCOV) eCOV->Copy(cov);
    else eCOV = new TMatrixD(cov);
  }

  void SetCOV( double *array, int dim=5) { 
    if(!array) return;
    if(!eCOV)  eCOV = new TMatrixD(5,5);
    for(int k=0; k<dim; k++) 
      for(int l=0; l<dim; l++) (*eCOV)(k,l) = array[k*dim + l];
  }

  TMatrixD &COV() const {return *eCOV;}
  void    SetSZ( float sz )             { eSZ=sz; }
  void    SetZ( float z )               { eZ=z; }
  void    SetDZ( float dz )             { eDZ=dz; }
  void    SetDZem( float dz )           { eDZem=dz; }
  void    SetID( int id )               { eID=id; }
  void    SetPID( int pid )             { ePID=pid; }
  void    SetFlag( int flag )           { eFlag=flag; }
  void    SetTrack( int trid )          { eTrack=trid; }
  void    SetW( float w )               { eW=w; }
  void    SetP( float p )               { eP=p; }
  void    SetProb( float prob )         { eProb=prob; }
  void    SetChi2( float chi2 )         { eChi2=chi2; }
  void    SetVolume( float w )          { eVolume=w; }
  void    SetVid(int vid, int sid)      { eVid[0]=vid; eVid[1]=sid; }
  void    SetAid(int a,   int v)        { eAid[0]=a; eAid[1]=v; }
  void    SetProbability( float p )     { eProb=p; }
  void    SetMC( int mEvt, int mTrack ) { eMCEvt=mEvt; eMCTrack=mTrack; }

  Int_t   MCEvt()   const {return eMCEvt;}
  Int_t   MCTrack() const {return eMCTrack;}
  Int_t   ID()      const {return eID;}
  Int_t   PID()     const {return ePID;}
  Int_t   Flag()    const {return eFlag;}
  Int_t   Track()   const {return eTrack;}
  Float_t W()       const {return eW;}
  Float_t P()       const {return eP;}
  Float_t Z()       const {return eZ;}
  Float_t DZ()      const {return eDZ;}
  Float_t DZem()    const {return eDZem;}
  Float_t Prob()    const {return eProb;}
  Float_t Chi2()    const {return eChi2;}
  Float_t Volume()  const {return eVolume;}

  Float_t SX()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(0,0); }
  Float_t SY()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(1,1); }
  Float_t STX() const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(2,2); }
  Float_t STY() const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(3,3); }
  Float_t SP()  const  { if(!eCOV) return 0; return (Float_t)(*eCOV)(4,4); }
  Float_t SZ()  const  { return eSZ; }
  Int_t   Vid(int i) const {return (i < 0 || i > 1) ? -1 : eVid[i];}
  Int_t   Aid(int i) const {return (i < 0 || i > 1) ? -1 : eAid[i];} 

  // mandatory virtual functions:
  Float_t X()        const {return eX;}
  Float_t Y()        const {return eY;}
  Float_t TX()       const {return eTX;}
  Float_t TY()       const {return eTY;}
  void    SetX(  float x )  { eX=x; }
  void    SetY(  float y )  { eY=y; }
  void    SetTX( float tx ) { eTX=tx; }
  void    SetTY( float ty ) { eTY=ty; }
 
  //other functions
  Float_t Phi()      const {return TMath::ATan2(eTY,eTX);}
  Float_t Theta()    const {return TMath::Sqrt(eTY*eTY+eTX*eTX);}
 
  void    Print( Option_t *opt="") const;

  Bool_t  IsEqual(const TObject *obj) const;
  Bool_t  IsSortable() const { return kTRUE; }
  Int_t   Compare(const TObject *obj) const;

  ClassDef(EdbSegP,16)  // segment
};

//______________________________________________________________________________
class EdbSegmentsBox : public TObject, public EdbPointsBox2D {
 
 private:
 
  Float_t eX,eY,eZ;         // central point

  TClonesArray *eSegments;  // collection of segments (EdbSegP)

  Float_t eDZkeep;           // eDZkeep = eZ - Zoriginal (before any projections)

  //  void (*PF_COUPLE)(EdbSegP *seg1,EdbSegP *seg2); //pointer to the function

  //protected:
 public:
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
 
  EdbSegP   *AddSegment(int i, EdbSegP &s);
  EdbSegP   *AddSegment(EdbSegP &s);
  EdbSegP   *AddSegment(EdbSegP &s1, EdbSegP &s2); 
  EdbSegP   *AddSegment(int id, float x, float y, float tx, float ty, 
		    float w=0, int flag=0);
  Int_t  GetN()    const 
    {if(eSegments) return eSegments->GetEntriesFast(); else return 0;}
  EdbSegP      *GetSegment(int i) const { return (EdbSegP*)eSegments->At(i); }
  EdbSegP      *GetSegmentLast()  const { if(GetN()) return (EdbSegP*)eSegments->Last(); else return 0; }
  TClonesArray *GetSegments()     const { return eSegments; }
  void          *GetSegmentsAddr()  { return &eSegments; }

  void SetSegmentsZ();
  // mandatory virtual functions:
  void       SetX(float x) { eX=x; }
  void       SetY(float y) { eY=y; }

  Float_t   X()       const {return eX;}
  Float_t   Y()       const {return eY;}
  Float_t   Z()       const {return eZ;}
  float     DZ()      const {return eDZkeep;}
  Int_t     N()       const {return GetN();}
  EdbPoint *At(int i) const {return (EdbPoint*)GetSegment(i);}
 
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

  TSortedList *eS;     // array of segments
  TSortedList *eSF;    // array of fitted segments
  Int_t        eNpl;   // number of plates passed throw
  Int_t        eN0;    // number of holes (if any)
  Float_t      eM;     // invariant mass of the particle
  Float_t      eDE;    // total energy loss of the particle between first and last segments
  Int_t        ePDG;   // particle ID from PDG
  EdbVTA      *eVTAS;  //! vertex track start is attached to
  EdbVTA      *eVTAE;  //! vertex track end is attached to

 public:

  EdbTrackP(int nseg=0);
  EdbTrackP(EdbSegP *seg, float m);
  EdbTrackP(EdbTrackP &track) : EdbSegP( *((EdbSegP *)&track) ) {Copy(track);}
  virtual ~EdbTrackP();

  void     Set0();

  void  AddVTA(EdbVTA *vta);
  void  ClearVTA(EdbVTA *vta);

  EdbVTA *VTAS() const { return eVTAS;}
  EdbVTA *VTAE() const { return eVTAE;}
  EdbVertex  *VertexS();
  EdbVertex  *VertexE();

  void    SetPDG( int pdg )  { ePDG=pdg; }
  Int_t   PDG()      const {return ePDG;}

  void    SetM( float m )  { eM=m; }
  Float_t M()      const {return eM;}

  void    SetCounters() { SetNpl(); SetN0(); }

  void    SetN0( int n0 )  { eN0 = n0; }
  void    SetN0()          { eN0 = eNpl-N(); }
  Int_t   N0()      const  {return eN0;}

  void    SetDE( float de )  { eDE=de; }
  Float_t DE()      const  {return eDE;}

  void    SetNpl( int npl )  { eNpl=npl; }
  void    SetNpl()  
    { if(eS) eNpl = 1+TMath::Abs(GetSegment(0)->PID() - GetSegment(N()-1)->PID()); }
  Int_t   Npl()      const   {return eNpl;}

  //  TList *S()  const { return eS; }
  //  TList *SF() const { return eSF; }

  //int      N() const  { if(eS)  return eS->GetEntries(); else return 0; } //TODO fast

  int      N()  const  { return (eS) ?  eS->GetSize()  : 0; }
  int      NF() const  { return (eSF)?  eSF->GetSize() : 0; }

  float    Wgrains() const;
  int	   GetSegmentsFlag( int &nseg ) const;
  int	   GetSegmentsAid( int &nseg ) const;
  int	   GetSegmentsMCTrack( int &nseg ) const;

  EdbSegP *GetSegmentFirst()  const {return (eS) ? (EdbSegP*)(eS->First()) : 0;}
  EdbSegP *GetSegmentLast()   const {return (eS) ? (EdbSegP*)(eS->Last()) : 0;}

  EdbSegP *GetSegmentFFirst() const {return (eSF) ? (EdbSegP*)(eSF->First()) : 0;}
  EdbSegP *GetSegmentFLast()  const {return (eSF) ? (EdbSegP*)(eSF->Last()) : 0;}

  EdbSegP *GetSegment(int i)  const {return (eS) ? (EdbSegP*)(eS->At(i)) : 0; }
  EdbSegP *GetSegmentF(int i) const {return (eSF) ? (EdbSegP*)(eSF->At(i)) : 0;}

  EdbSegP *TrackZmin(bool usesegpar=false) const { if(usesegpar || (!eSF)) return GetSegmentFirst(); else return GetSegmentFFirst(); }
  EdbSegP *TrackZmax(bool usesegpar=false) const { if(usesegpar || (!eSF)) return GetSegmentLast();  else return GetSegmentFLast(); }

  float Zmax()   const;
  float Zmin()   const;
  int   Dir()    const { if(DZ()<0) return -1; else return 1; }
  const EdbSegP  *TrackStart() const;
  const EdbSegP  *TrackEnd()   const;
  float Zstart() const { return TrackStart()->Z(); }
  float Zend()   const { return TrackEnd()->Z(); }
  void  AddTrack(const EdbTrackP &tr);

  void  AddSegment(EdbSegP *s)
  { 
    if(!eS) eS = new TSortedList();
    eS->Add(s);
  }
  void  RemoveSegment(EdbSegP *s)
  { 
    if(!eS) return;
    eS->Remove(s);
    s->SetTrack(-1);
    SetCounters();
  }
  void  AddSegmentF(EdbSegP *s)  
  { 
    if(!eSF) 
      {
        eSF = new TSortedList();
	eSF->SetOwner();
      }
    eSF->Add(s);
  }

  int   RemoveAliasSegments();
  int   CheckMaxGap();
  int   CheckAliasSegments();
  int   SetSegmentsTrack(int id) {for(int i=0; i<N(); i++) GetSegment(i)->SetTrack(id); return N();}
  int   SetSegmentsTrack() {return SetSegmentsTrack(ID());}
  int   FitTrackKFS( bool zmax=false, float X0=5810., int design = 0 );
  int   MakeSelector( EdbSegP &ss, bool followZ=true );
  float CHI2();
  float CHI2F();
  void  FitTrack();
  void  Copy(const EdbTrackP &tr);
  void  Clear()  { if(eS)  eS->Clear(); if(eSF) eSF->Clear(); }
  void  ClearF() { if(eSF) eSF->Clear(); }
  void  Print();
  void  PrintNice();

  ClassDef(EdbTrackP,6)  // track consists of segments
};

//______________________________________________________________________________
class EdbPattern : public EdbSegmentsBox {
 
 private:

  Int_t       eID;             // pattern id in the volume
  Int_t       ePID;            // correspond to the piece ID
  TIndexCell *eCell;           //! associated with eSegments
  Float_t     eStepX ,eStepY;  // bin size for the index cell 
  Float_t     eStepTX,eStepTY; // bin size for the index cell 
  Float_t     eSigma[4];       // accuracy in comparison to neibour pattern
  Int_t       eFlag;           // pattern flag
  Int_t       eNAff;           // number of segments selected for affine calculation

 public:

  EdbPattern();
  EdbPattern( EdbPattern &p );
  EdbPattern(float x0, float y0, float z0, int n=0 );
  virtual ~EdbPattern();
 
  void  Set0();
  void  SetSigma(float sx,float sy,float stx,float sty) 
                {eSigma[0]=sx; eSigma[1]=sy; eSigma[2]=stx; eSigma[3]=sty;}
  void  SetStep(float stepx, float stepy, float steptx, float stepty ) 
                {eStepX=stepx; eStepY=stepy; eStepTX=steptx; eStepTY=stepty;}
  void  Reset();
  float SummaryPath();
  void  FillCell( float stepx, float stepy, float steptx, float stepty );
  int   FindCompliments(EdbSegP &s, TObjArray &arr, float nsig, float nsigt);
  void  SetSegmentsPID();
  EdbPattern *ExtractSubPattern(float min[5], float max[5]);

  void  SetID(int id)   {eID=id;}
  void  SetPID(int pid) {ePID=pid;}
  void  SetNAff(int n)  {eNAff=n;}

  Int_t       NAff()   const {return eNAff;}
  float       StepX()  const {return eStepX;}
  float       StepY()  const {return eStepY;}
  float       StepTX() const {return eStepTX;}
  float       StepTY() const {return eStepTY;}
  int         ID()     const {return eID;}
  int         PID()    const {return ePID;}
  TIndexCell *Cell()   const {return eCell;}



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
                        return ePatterns->GetEntriesFast();
                        else return 0; }

  void PassProperties(EdbPatternsVolume &pvol);

  void         AddPattern( EdbPattern *pat );
  void         AddPatternAt( EdbPattern *pat, int id );
  EdbPattern  *GetPattern( int id ) const;

  EdbPattern  *NextPattern(float z, int dir) const;

  void DropCell();

  void   PrintStat( Option_t *opt="") const;
  void   PrintStat( EdbPattern &pat ) const;
  void   PrintAff() const;
  int    DropCouples();

  Long_t Vid(int pid, int sid) const { return pid*1000000+sid; }
  Int_t  Pid(Long_t vid)       const { return vid/1000000; }
  Int_t  Sid(Long_t vid)       const { return vid%1000000; }

  EdbSegP *GetSegment(Long_t vid) const 
    {return GetPattern(Pid(vid))->GetSegment( Sid(vid) );}

  int FindComplimentsVol(EdbSegP &s, TObjArray &arr, float nsig, float nsigt, int dpat);

  void   Print() const;

  ClassDef(EdbPatternsVolume,1)  // patterns nostri
};

#endif /* ROOT_EdbPattern */
