#ifndef ROOT_EdbPVRec
#define ROOT_EdbPVRec
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVRec                                                             //
//                                                                      //
// pattern volume reconstruction                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "THashList.h"
#include "EdbPattern.h"
#include "TIndexCell.h"
#include "EdbAffine.h"

class EdbAffine2D;
class TIndexCell;

//______________________________________________________________________________
class EdbScanCond : public TNamed {
  //
  //  Keep the accuracy parameters for 1 pattern/layer
  // 
 private:

  Float_t  eDegrad;    // angular degradation of parameters: S = S0*(1 + eDegrad*Ang)

  Float_t  eSigmaX0;   // [microns]  Parameters at 0 angle
  Float_t  eSigmaY0;   // [microns]  SigmaX = S0*(1+eDegrad*Ax)
  Float_t  eSigmaZ0;   // z - uncertancy
  Float_t  eSigmaTX0;  // ["rad"]
  Float_t  eSigmaTY0;  // ["rad"]

  // puls height parametrizations:

  Float_t ePuls0[2];   // signal/all is parametrised as linear in range Pmin,Pmax
  Float_t ePuls04[2];  // - at angle .4 rad

  Float_t eBinX,eBinY;        // bins [normalized to Sigma()]
  Float_t eBinTX,eBinTY;      //

  Float_t eChi2Max;           //
  Float_t eChi2PMax;          //

  Float_t eOffX, eOffY;       // maximal offsets in x and y - the accuracy of pattern 
                              // itself in respect to the upper level RS

 public:
  EdbScanCond();
  virtual ~EdbScanCond(){}

  void SetDefault();

  void SetSigma0(  float x, float y, float tx, float ty ) 
    { eSigmaX0=x;  eSigmaY0=y;  eSigmaTX0=tx;  eSigmaTY0=ty;  }
  void SetBins(float bx, float by, float btx, float bty) 
    { eBinX=bx; eBinY=by; eBinTX=btx; eBinTY=bty; }

  void SetOffset( float x, float y ) {eOffX=x; eOffY=y;}
  float OffX() const {return eOffX;}
  float OffY() const {return eOffY;}
  void SetDegrad( float d ) {eDegrad=d;}
  void SetSigmaZ0( float z ) {eSigmaZ0=z;}

  void SetPulsRamp0(  float p1, float p2 )  {ePuls0[0]=p1; ePuls0[1]=p2;}
  void SetPulsRamp04(  float p1, float p2 ) {ePuls04[0]=p1; ePuls04[1]=p2;}
  float BinX()  const {return eBinX;}
  float BinY()  const {return eBinY;}
  float BinTX() const {return eBinTX;}
  float BinTY() const {return eBinTY;}

  void  SetChi2Max(float chi2)  {eChi2Max=chi2;}
  void  SetChi2PMax(float chi2) {eChi2PMax=chi2;}
  float Chi2Max()    const {return eChi2Max;}
  float Chi2PMax()   const {return eChi2PMax;}

  float StepX(float dz)   const;
  float StepY(float dz)   const;
  float StepTX(float tx)  const { return BinTX()*SigmaTX(tx); }
  float StepTY(float ty)  const { return BinTY()*SigmaTY(ty); }

  float SigmaX(float ax)  const { return eSigmaX0*(1. + TMath::Abs(ax)*eDegrad); }
  float SigmaY(float ay)  const { return eSigmaY0*(1. + TMath::Abs(ay)*eDegrad); }
  float SigmaZ(float ax, float ay)  const 
    { return eSigmaZ0; } // TODO
  float SigmaTX(float ax) const { return eSigmaTX0*(1. + TMath::Abs(ax)*eDegrad); }
  float SigmaTY(float ay) const { return eSigmaTY0*(1. + TMath::Abs(ay)*eDegrad); }

  float Ramp(float x, float x1, float x2) const;

  float ProbSeg( float tx, float ty, float puls) const;
  float ProbSeg( float t, float puls) const; 

  void Print() const;

  ClassDef(EdbScanCond,1)  // Scanning Conditions Parameters
};

//______________________________________________________________________________
class EdbSegCouple : public TObject {

 private:
  Int_t   eID1, eID2;
  Float_t eCHI2;      // final chi2 calculation based on the linked track
  Float_t eCHI2P;     // fast chi2 estimation used for couples selection
  Int_t   eN1, eN2;         // n1,n2 - is rating of the segment (starting from 1)
  Int_t   eN1tot, eN2tot;   // total number of entries for the segment

  EdbSegP  *eS;   //!  the result of the fit

  static int egSortFlag; //! 0: chi2p only; 1: N1()*10000+N2()*10000000 + CHI2()/100

 public:
  EdbSegCouple();
  EdbSegCouple( int id1, int id2 )
    { Set0();eID1=id1;eID2=id2; eCHI2=0; eCHI2P=0; eN1=0; eN2=0; }
  ~EdbSegCouple();

  void Set0();
  void SetN1(int n1)  {eN1=n1;}
  void SetN2(int n2)  {eN2=n2;}
  void SetN1tot(int n)  {eN1tot=n;}
  void SetN2tot(int n)  {eN2tot=n;}
  void SetCHI2(float chi2)  {eCHI2=chi2;}
  void SetCHI2P(float chi2)  {eCHI2P=chi2;}

  //  void CalculateS // TODO
  void Print();

  static void SetSortFlag(int s=0);
  static int     SortFlag() {return egSortFlag;}

  int  ID1()     const {return eID1;}
  int  ID2()     const {return eID2;}
  int   N1()     const {return eN1;}
  int   N2()     const {return eN2;}
  int   N1tot()  const {return eN1tot;}
  int   N2tot()  const {return eN2tot;}
  float CHI2()   const {return eCHI2;}
  float CHI2P()  const {return eCHI2P;}

  //ULong_t Hash() const { return fValue; }
  Bool_t  IsEqual(const TObject *obj) const
    { return
	(ID1()==((EdbSegCouple*)obj)->ID1())&&
	(ID2()==((EdbSegCouple*)obj)->ID2()); }
  Bool_t  IsSortable() const { return kTRUE; }
  Int_t   Compare(const TObject *obj) const;

  ClassDef(EdbSegCouple,1)  // couple of segments
};

//______________________________________________________________________________
class EdbPatCouple : public TObject {
 
 private:

  Int_t  eID[2];           // id-s of patterns in volume

  int eCoupleType;         // 1 - up/down link; 0 - plate-to-plate

  EdbPattern *ePat1;
  EdbPattern *ePat2;

  Float_t eOffset[4];      // maximal allowed offset before alignment
                           // "x:y:tx:ty"
  Float_t eSigma[4];       // expected pat<->pat sigma after alignment

  EdbAffine2D *eAff;        // affine transformation as: pat2 = pat1->Transform(aff)

  TObjArray *eSegCouples;   //
  //  TClonesArray *eSegCouples;   //
  
  EdbScanCond *eCond;

  float eZlink;            // at this z patterns should be linked (aligned)

  float eXoffsetMax;
  float eYoffsetMax;

  float eChi2Max; 

  int eCHI2mode;           // algorithm used for chi2 calculation

 public:
  EdbPatCouple();
  ~EdbPatCouple();

  void SetID(int id1, int id2) { eID[0]=id1; eID[1]=id2; }
  void SetOffsetsMax(float ox, float oy)
    { eXoffsetMax=ox; eYoffsetMax=oy; }
  void SetOffset( float o1,float o2, float o3, float o4 ) 
    { eOffset[0]=o1; eOffset[1]=o2; eOffset[2]=o3; eOffset[3]=o4;  }
  void SetSigma( float s1,float s2, float s3, float s4 ) 
    { eSigma[0]=s1; eSigma[1]=s2; eSigma[2]=s3; eSigma[3]=s4; }
  void SetZlink(float z) {eZlink=z;}

  void SetPat1(EdbPattern *pat1) { ePat1=pat1; }
  void SetPat2(EdbPattern *pat2) { ePat2=pat2; }

  void SetCond(EdbScanCond *cond) { eCond=cond; }

  EdbAffine2D  *GetAff()  {return eAff;}
  EdbScanCond  *Cond()    {return eCond;}

  float   Zlink()    const {return eZlink;}
  int     Ncouples() const { if(eSegCouples) return eSegCouples->GetEntries(); 
                             else return 0; }
  void  ClearSegCouples() { eSegCouples->Clear(); }


  EdbSegCouple   *AddSegCouple(int id1, int id2);
  EdbSegCouple   *GetSegCouple(int i) const 
    { return (EdbSegCouple *)(eSegCouples->At(i)); }
  void   RemoveSegCouple(EdbSegCouple *sc) { 
    //eSegCouples->Remove(sc); 
    if(sc) delete sc; sc=0;
  }

  EdbPattern     *Pat1()  { return ePat1; }
  EdbPattern     *Pat2()  { return ePat2; }



  int          LinkSlow( float chi2max );
  int          LinkFast();
  int          DiffPat( EdbPattern *pat1, EdbPattern *pat2, 
			Long_t vdiff[4]);

  int          DiffPatCell( TIndexCell *cel1, TIndexCell *cel2, 
			    Long_t vdiff[4]);

  void         SetCHI2mode(int m) { eCHI2mode=m; }
  float        Chi2Pz0(EdbSegCouple *scp);
  float        Chi2Pn(EdbSegCouple *scp);
  float        Chi2A(EdbSegCouple *scp, int iprob=1);
  float        Chi2A(EdbSegP *s1, EdbSegP *s2, int iprob=1);
  int          FillCHI2();
  int          FillCHI2P();

  int          SelectIsolated();
  int          CutCHI2P(float chimax);
  int          SortByCHI2P();
  void         PrintCouples();
  
  int    Align();
  //int    AlignAff(EdbPattern *pat1, EdbPattern *pat2, Long_t vdiff[4]);
  int    FindOffset0(float xmax, float ymax);
  int    FindOffset01(float xmax, float ymax);
  int    FindOffset1(float xmax, float ymax);
  int    FindOffset( EdbPattern *pat1, EdbPattern *pat2, Long_t vdiff[4]);
  void   CalculateAffXY();
  void   CalculateAffXYZ(float z);


  void FillCell_XYaXaY( EdbScanCond *cond, float zlink, int id=0 );
  void FillCell_XYaXaY( EdbPattern *pat, EdbScanCond *cond, float dz, 
		   float stepx, float stepy );

  int   ID1() const { return eID[0]; }
  int   ID2() const { return eID[1]; }

  float OffsetX()  const { return eOffset[0]; }
  float OffsetY()  const { return eOffset[1]; }
  float OffsetTX() const { return eOffset[2]; }
  float OffsetTY() const { return eOffset[3]; }

  float SigmaX()   const { return eSigma[0]; }
  float SigmaY()   const { return eSigma[1]; }
  float SigmaTX()  const { return eSigma[2]; }
  float SigmaTY()  const { return eSigma[3]; }

  ClassDef(EdbPatCouple,1)  // couple of patterns to be aligned
};

//______________________________________________________________________________
class EdbPVRec : public EdbPatternsVolume {
 
 private:

  TObjArray   *ePatCouples;    // patterns couples

  float   eChi2Max;
  Long_t  eVdiff[4];          // how many ajenced bins should be taken into account
  EdbScanCond *eScanCond;     // scanning conditions (sigma, puls, etc)

  TIndexCell  *eTracksCell;
 public:
  TObjArray   *eTracks;

 public:
  EdbPVRec();
  ~EdbPVRec();

  void    SetScanCond(EdbScanCond *scan) { eScanCond=scan; }
  EdbScanCond const *GetScanCond() { return eScanCond; }

  void SetBinsCheck(int nx, int ny, int ntx, int nty) 
    { eVdiff[0]=nx; eVdiff[1]=ny; eVdiff[2]=ntx; eVdiff[3]=nty; }
  EdbPatCouple *AddCouple(int id1, int id2);
  void AddCouple(EdbPatCouple *c) { ePatCouples->Add(c); }
  void SetCouples();
  void ResetCouples();
  void SetCouplesAll();

  float Chi2Max( ) const { return eChi2Max; }
  void SetChi2Max(float chi)
    { eChi2Max=chi; }

  int  Ncouples() const { if(ePatCouples) return ePatCouples->GetEntries(); 
                          else return 0; }

  EdbPatCouple  *GetCouple(int i) const { if(ePatCouples) 
                               return (EdbPatCouple  *)(ePatCouples->At(i));
                               else return 0; }

  TIndexCell *GetTracksCell() const { return eTracksCell; }
  int   MakeTracksTree();

  int    LinkSlow();
  int    Link();
  int    Align();
  int    AlignA();
  int    LinkTracks();
  void   FillTracksCell2();
  void   FillTracksCell1();
  void   FillTracksCell();
  int    MakeHoles(int ort);
  int    InsertHole( const EdbSegP *s1, const EdbSegP *s2, int pid );
  int    CollectSegment(TIndexCell *ct, TIndexCell *cross);
  int    CollectSegment1(TIndexCell *ct, THashList *cross);
  int    SelectLongTracks(int nsegments);

  int    MakeSummaryTracks();
  int    FineCorrXY(int ipat, EdbAffine2D &aff);
  int    FineCorrTXTY(int ipat, EdbAffine2D &aff);
  int    FineCorrZ(int ipat, float &dz);
  int    FineCorrShr(int ipat, float &shr);

  void SetOffsetsMax(float ox, float oy);
  void SetSegmentsErrors();
  void SetSegmentErrors( EdbSegP &seg );
  void SetSegmentProbability( EdbSegP &seg );


  /*

  float Chi2( EdbSegP *s1,  EdbSegP *s2 ) const;
  float Chi2_( EdbSegP *s1,  EdbSegP *s2 ) const;
  float Chi2Z0( EdbSegP *s1,  EdbSegP *s2 ) const;

  void VerifyParameters();
  void PrintParameters();

  int    AlignFast();
  int    AlignFast(EdbPatternsVolume &pvol);
  int    Align();
  int    Align(int pass);

  void   RemakeTracksCell();

  void   FillTracksCell(  EdbPattern *pat1, EdbPattern *pat2,
			  int e1,int e2 );

  int  ExtractSelectedTracks( EdbPatternsVolume *pvol=0, int nseg=2 );

  void ExtractSelectedPatterns( EdbPattern *pat1, 
				EdbPattern *pat2, 
				TIndexCell *csel,
				EdbPattern *psel1, 
				EdbPattern *psel2 );



  TIndexCell *TracksCell() const { return eTracksCell; }


  */


  ClassDef(EdbPVRec,1)  // Patterns Volume reconstructor
};
 
//______________________________________________________________________________
class EdbAlignment : public TObject {
 
 private:

  EdbScanCond *eScanCond;   // segments (scanning) properties

  EdbPVRec    *ePV;     // patterns volume to be processed

 public:
  EdbAlignment(){}
  ~EdbAlignment(){}

  ClassDef(EdbAlignment,1)  // alignment class
};
 
//______________________________________________________________________________
#endif /* ROOT_EdbPVRec */
