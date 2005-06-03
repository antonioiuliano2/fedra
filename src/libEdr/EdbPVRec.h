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
#include "EdbLayer.h"

class EdbAffine2D;
class TIndexCell;
class EdbVertex;
class EdbVTA;
class TIndex2;

//______________________________________________________________________________
class EdbSegCouple : public TObject {

 private:
  Int_t   eID1, eID2;
  Float_t eCHI2;      // final chi2 calculation based on the linked track
  Float_t eCHI2P;     // fast chi2 estimation used for couples selection
  Int_t   eN1, eN2;         // n1,n2 - is rating of the segment (starting from 1)
  Int_t   eN1tot, eN2tot;   // total number of entries for the segment

 public:
  EdbSegP  *eS;   //!  the result of the fit
 private:
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
  int     Ncouples() const { if(eSegCouples) return eSegCouples->GetEntriesFast(); 
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
  int          CHI2mode() const { return eCHI2mode; }
  float        Chi2Pz0(EdbSegCouple *scp);
  float        Chi2Pn(EdbSegCouple *scp);
  float        Chi2A(EdbSegCouple *scp, int iprob=1);
  float        Chi2A(EdbSegP *s1, EdbSegP *s2, int iprob=1);
  float        Chi2KF(EdbSegCouple *scp);

  int          FillCHI2();
  int          FillCHI2P();

  int          SelectIsolated();
  int          CutCHI2P(float chimax);
  int          SortByCHI2P();
  void         PrintCouples();
  
  int    Align(int alignFlag);
  //int    AlignAff(EdbPattern *pat1, EdbPattern *pat2, Long_t vdiff[4]);
  int    FindOffset0(float xmax, float ymax);
  int    FindOffset01(float xmax, float ymax);
  int    FindOffset1(float xmax, float ymax);
  int    FindOffset( EdbPattern *pat1, EdbPattern *pat2, Long_t vdiff[4]);
  void   CalculateAffXY(int alignFlag);
  void   CalculateAffXYZ(float z, int alignFlag);

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
  TObjArray   *eVTX;          // array of vertex
  TList        eVTA;          // vertex-track associations

 public:
  EdbPVRec();
  ~EdbPVRec();

  void    FillTracksStartEnd(TIndexCell &starts, TIndexCell &ends,
			     float zFrom, float zTo, float zBin,
			     float ProbMinT, int Nsegmin);

  int     ProbVertex(int maxgap[6], float dA, float ProbMin,
		     float ProbMinT=0.05, int Nsegmin=3, bool usemom=true);
  int     ProbVertex(TIndexCell &list1, TIndexCell &list2,
		     int BinDifMin,     int BinDifMax,
		     float dA,          float ProbMin,   float zBin, bool usemom);
  int	  ProbVertexN(float ProbMin);
  int	  LinkedVertexes();
  int	  VertexNeighboor(float RadMax = 1000., int Dpat = 1, float ImpMax = 1000000.);
  int	  VertexNeighboor(EdbVertex *v, float RadMax = 1000., int Dpat = 1, float ImpMax = 1000000.);
  int	  SelVertNeighboor( EdbVertex *v, int seltype, float RadMax, int Dpat, TObjArray *ao);

  void    FillCell( float stepx,  float stepy, float steptx,float stepty);
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

  int  Ncouples() const { if(ePatCouples) return ePatCouples->GetEntriesFast(); 
                          else return 0; }

  EdbPatCouple  *GetCouple(int i) const { if(ePatCouples) 
                               return (EdbPatCouple  *)(ePatCouples->At(i));
                               else return 0; }

  TIndexCell *GetTracksCell() const { return eTracksCell; }

  float Chi2Fast(EdbSegP &s1, EdbSegP &s2);
  int   MergeTracks(int maxgap=2);
  int   MergeTracks1(int maxgap);
  int   MakeTracks(int nsegments=2);
  void  FitTracks(float p=10., float mass=0.139, TObjArray *gener = 0,
		  int design=0);

  int    LinkSlow();
  int    Link();
  int    Align(int alignFlag);
  //  int    AlignA();
  //  int    LinkTracks();
  void   FillTracksCellFast();
  void   AddCouplesToTracks(EdbPatCouple *pc, TIndex2 &itracks );
  void   AddCouplesToTracksM(EdbPatCouple *pc, TIndex2 &itracks );
  void   FillTracksCell2();
  void   FillTracksCell1();
  void   FillTracksCell();
  int    MakeHoles(int ort);
  int    InsertHole( const EdbSegP *s1, const EdbSegP *s2, int pid );
  int    CollectSegment(TIndexCell *ct, TIndexCell *cross);
  int    CollectSegment1(TIndexCell *ct, THashList *cross);
  int    SelectLongTracks(int nsegments);

  int    MakeSummaryTracks();
  int    FineCorrXY(int ipat, EdbAffine2D &aff, int flag);
  int    FineCorrTXTY(int ipat, EdbAffine2D &aff);
  int    FineCorrZ(int ipat, float &dz);
  int    FineCorrShr(int ipat, float &shr);

  int    FineCorrF(int ipat, EdbAffine2D &aff,  EdbAffine2D &afft );

  void SetOffsetsMax(float ox, float oy);
  void SetSegmentsErrors();
  void SetSegmentErrors( EdbSegP &seg );
  void SetSegmentProbability( EdbSegP &seg );

  void AddTrack(EdbTrackP *track) {
    if(!eTracks) eTracks = new TObjArray();
    eTracks->Add(track);
  }

  void AddVertex(EdbVertex *vtx) {
    if(!eVTX) eVTX = new TObjArray();
    eVTX->Add((TObject*)vtx);
  }

  void AddVTA(EdbVTA *vta) {
    eVTA.Add((TObject*)vta);
  }

  int ExtractDataVolumeSeg( EdbTrackP &tr, TObjArray &arr, 
			    float binx, float bint );
  int ExtractDataVolumeSegAll( TObjArray &arr );

  int PropagateTracks(int nplmax, int nplmin, float probMin = 0.05,
		      int ngapMax = 3, int design = 0 );
  int PropagateTrack( EdbTrackP &tr, bool followZ, float probMin = 0.05,
		      int ngapMax = 3, int design = 0 );

  void ClearPropagation(int design = 0);

  int MakePatternsFromTracks();

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
