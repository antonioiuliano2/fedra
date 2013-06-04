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
#include "EdbScanCond.h"
#include "EdbSegCouple.h"

class EdbAffine2D;
class TIndexCell;
class EdbVertex;
class EdbVTA;
class TIndex2;
class EdbScanSet;

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
  void  ClearSegCouples() { if(eSegCouples) eSegCouples->Clear(); }

  EdbSegCouple   *AddSegCouple(int id1, int id2);
  EdbSegCouple   *GetSegCouple(int i) const 
    { return (EdbSegCouple *)(eSegCouples->At(i)); }
  void   RemoveSegCouple(EdbSegCouple *sc) { SafeDelete(sc); }

  EdbPattern     *Pat1()  { return ePat1; }
  EdbPattern     *Pat2()  { return ePat2; }



  int          LinkSlow( float chi2max );
  int          LinkFast();
  int          CheckSegmentsDuplication(EdbPattern *pat);
  int          DiffPat( EdbPattern *pat1, EdbPattern *pat2, 
			Long_t vdiff[4]);

  int          DiffPatCell( TIndexCell *cel1, TIndexCell *cel2, 
			    Long_t vdiff[4]);

  void         SetCHI2mode(int m) { eCHI2mode=m; }
  int          CHI2mode() const { return eCHI2mode; }
  float        Chi2Pz0(EdbSegCouple *scp);
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

 public:
  EdbPVRec();
  ~EdbPVRec();

  void DummyCycle();
  
  void  FillCell( float stepx,  float stepy, float steptx,float stepty);
  void  SetScanCond(EdbScanCond *scan) { eScanCond=scan; }
  EdbScanCond const *GetScanCond() { return eScanCond; }

  void  SetBinsCheck(int nx, int ny, int ntx, int nty) 
    { eVdiff[0]=nx; eVdiff[1]=ny; eVdiff[2]=ntx; eVdiff[3]=nty; }
  EdbPatCouple *AddCouple(int id1, int id2);
  void  AddCouple(EdbPatCouple *c) { ePatCouples->Add(c); }
  void  SetCouples();
  void  ResetCouples();
  void  DeleteCouples() { if(ePatCouples) ePatCouples->Delete(); }
  void  SetCouplesAll();
  void  SetCouplesPeriodic(int istart, int iperiod);
  void  SetCouplesExclude(TArrayI &exclude);

  float Chi2Max( ) const { return eChi2Max; }
  void  SetChi2Max(float chi) { eChi2Max=chi; }

  Int_t Ncouples() const {return ePatCouples?ePatCouples->GetEntriesFast():0;}

  EdbPatCouple  *GetCouple(int i) const { if(ePatCouples) 
                               return (EdbPatCouple  *)(ePatCouples->At(i));
                               else return 0; }

  TIndexCell *GetTracksCell() const { return eTracksCell; }

  void  ResetTracks();
  float Chi2Fast(EdbSegP &s1, EdbSegP &s2);
  int   MergeTracks(int maxgap=2);
  int   MergeTracks1(int maxgap);
  int   MakeTracks(int nsegments=2, int flag=0);
  void  FitTracks(float p=10., float mass=0.139, TObjArray *gener = 0,
		  int design=0);
  Int_t Ntracks() const {return eTracks ? eTracks->GetEntriesFast() : 0;}

  int    LinkSlow();
  int    Link();
  int    Align(int alignFlag);
  int    AlignOld(int alignFlag);
  int    AlignPlates( EdbScanSet &sci, EdbScanSet &sca, const char *reportdir=0);

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
  int    FineCorrZnew();
  int    FineCorrShr(int ipat, float &shr);

  int    FineCorrF(int ipat, EdbAffine2D &aff,  EdbAffine2D &afft );

  void SetSegmentsTracks();

  void SetOffsetsMax(float ox, float oy);
  void SetSegmentsErrors();
  void SetSegmentErrors( EdbSegP &seg );
  void SetSegmentProbability( EdbSegP &seg );

  EdbTrackP* GetTrack(int i) const {return eTracks ? (EdbTrackP*)(eTracks->At(i)) : 0;}
  EdbTrackP* FindTrack(int id) const;
	
	EdbSegP* FindSegment(int PlateID, int SegmentID) const;

  void AddTrack(EdbTrackP *track) {
    if (!eTracks) eTracks = new TObjArray();
    eTracks->Add(track);
  }

  void AddVertex(EdbVertex *vtx) {
    if(!eVTX) eVTX = new TObjArray();
    eVTX->Add((TObject*)vtx);
  }
  Int_t      Nvtx()       const  {return eVTX ? eVTX->GetEntriesFast() : 0;}
  EdbVertex *GetVertex(Int_t &i) {return eVTX ? (EdbVertex*)eVTX->At(i) : 0;}
  
  Int_t      NSeg();
	  

  int ExtractDataVolumeSeg( EdbTrackP &tr, TObjArray &arr, 
			    float binx, float bint );
  int ExtractDataVolumeSegAll( TObjArray &arr );

  int CombTracks( int nplmin, int ngapMax, float probMin=0.01 );

  int PropagateTracks(int nplmax, int nplmin, float probMin = 0.05,
		      int ngapMax = 3, int design = 0 );
  int PropagateTrack( EdbTrackP &tr, bool followZ, float probMin = 0.05,
		      int ngapMax = 3, int design = 0 );

  static double ProbeSeg( const EdbTrackP *s1, EdbTrackP *s2, 
			  const float X0=5810. );
  static double ProbeSeg( const EdbTrackP *s1, EdbSegP *s2, 
			  const float X0=5810. );
  static double ProbeSeg( const EdbSegP *s1, EdbSegP *s2, 
			  const float X0=5810., const float mass=0.1396 );

  static bool AttachSeg(  EdbTrackP& tr, EdbSegP *s,
			  const float X0, const float ProbMin, float &prob );

  EdbPattern  *GetPatternByPID(int pid);
  int          AddSegments(EdbPVRec &ali);
  int          AddSegments(EdbTrackP &track);
  EdbSegP     *AddSegment(EdbSegP &s);
  void         SetScanIDPatSeg(EdbID id);
  void         SetScanIDTrackSeg(EdbID id);
  
  float        Xmin();
  float        Xmax();
  float        Ymin();
  float        Ymax();
  //float        Zmin();
  //float        Zmax();
  
  void 		PrintSummary();

  ClassDef(EdbPVRec,1)  // Patterns Volume reconstructor
};
#endif /* ROOT_EdbPVRec */
