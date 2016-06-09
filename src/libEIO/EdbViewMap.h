#ifndef ROOT_EdbViewMap
#define ROOT_EdbViewMap

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewMap                                                           //
//                                                                      //
// views positions correction using alignment data                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbRun.h"
#include "TIndexCell.h"
#include "EdbCell2.h"
#include "EdbLayer.h"
#include "TVector2.h"
#include "TH2D.h"

//______________________________________________________________________________
#define make_bit(_i_) (1 << _i_)

inline bool test_bit(int bit_,int flags_)
{
  return (flags_ & bit_) != 0;
};
inline bool test_ith_bit(int i_,int flags_)
{
  return test_bit(make_bit(i_),flags_);
};

//______________________________________________________________________________
struct AApv        //side-by-side alignment
{
  AlignmentParView *apv;
  float quality;   // aligment quality coefficient (from 0 to 1)
};

//______________________________________________________________________________
struct AView   // view and neighbouring
{
  EdbViewHeader    *header;
  int               nalv;
  AApv             *alv[16]; // all alignments with this header
  bool              isCorrected;
  EdbAffine2D       aff;
  float             quality; // view quality coefficient (from 0 to 1)
};

//______________________________________________________________________________
struct AArea
{
  int   ilast;    // index of the last filled slot
  int   nview;    // length of the view array
  AView **view;
};

//______________________________________________________________________________
struct ASide
{
  AArea **area;
  int    narea;
};

//______________________________________________________________________________
struct ARun
{
  ASide side[2];
};

//______________________________________________________________________________
class EdbViewMap : public TObject {

  public:
    Int_t              eNcp; // number of objects in eALcp
    AlignmentParView **eALcp; //! read tree with alignment parameters here
    
    Int_t              eNvh;  // number of headers
    TObjArray          eViewHeaders;  // read headers for all views here
    
    Int_t              eNpvh;  // number of headers
    TObjArray          ePinViewHeaders;  // read headers for all pinned views here
    
    ARun eAR;     // structures with pointers to the above containers
    
    TTree *eAHT;  //aligned headers tree
    
    Int_t  eAlgorithm;   // algorithm used for correction: 0-bestNeigbour, 1-allNeigbours, 2,3 - hysteresis corr

    Int_t  ePinPointID;   // pin point ID to be used for this alignment pinID=(1000 + 2*pointID+side)

    TH2D *eHX0odd;         // odd lines x-offset  side=0
    TH2D *eHX0even;        // even lines x-offset
    TH2D *eHY0odd;         // odd lines x-offset
    TH2D *eHY0even;        // even lines x-offset
    TH2D *eHX1odd;         // odd lines x-offset   side=1
    TH2D *eHX1even;        // even lines x-offset
    TH2D *eHY1odd;         // odd lines x-offset
    TH2D *eHY1even;        // even lines x-offset

  public:
    EdbViewMap();
    virtual ~EdbViewMap();
    
    void ReadViewsHeaders(const char *file, TCut &cut);
    void ReadPinViewsHeaders(const char *file);
    void ReadViewAlign(const char *file);
    void MakeAHTnocorr();
    void CorrectLines( TTree &tree );
    void SaveLinesCorrToRun(const char *fin);
    
    void AddInverseAlign();
    EdbViewHeader *GetViewHeader( AlignmentParView &apv, int fs );
    EdbViewHeader *GetHeader(int i) { return (EdbViewHeader*)(eViewHeaders.At(i)); }
    EdbViewHeader *GetPinHeader(int i) { return (EdbViewHeader*)(ePinViewHeaders.At(i)); }
    int  ViewSide(const EdbViewHeader &vh) const { return vh.GetNframesTop()==0?0:1; }
    
    void InitAR();
    void FillAR();
    AView *GetAView(EdbViewHeader &h);
    AView *GetAView(int side, int area, int view);
    AArea *GetAArea(int side, int area) { return eAR.side[side].area[area]; }
    void CheckViewFrameAlignQuality(const char *file);
    void FillALcp();
    
    void DoCorrection();
    
    int    PinViewID(int pinpoint, int side) {return -(1000+2*pinpoint+side); }
    int    PinPoint(int pinviewid) {return (-1000-pinviewid)/2; }
    void   DoCorrection1( AArea &area );
    //void   CorrectPinned( AArea &area );
    AView *SelectBestNotCorrected( AArea &area);
    float  ViewCorrectability(AView &v);
    float  ViewCorrectability1(AView &v);
    float  ViewCorrectabilityAll(AView &v);
    void   DoCorrectionBestNeib( AView &v );
    void   DoCorrectionAllNeib( AView &v );

    float Quality( AApv &apv );
    void  Transform( AView &v, const AlignmentParView &apv );
    void  CorrectToStage();
    void FillAHT( AlignmentParView *apv, EdbViewHeader *vn, EdbViewHeader *vc, EdbAffine2D *aff, float w, TTree *&aht, const char *name );
    
    void SaveCorrToRun(const char *fin);
    void ConvertRun(const char *fin, const char *fout);
    void CheckView(int side, int area, int idview);
    void CheckView(AView *v);
    
    static bool IsApplied(   const AlignmentParFrame &apf) {return test_ith_bit(0,apf.flag);}
    static bool IsFound(     const AlignmentParFrame &apf) {return test_ith_bit(1,apf.flag);}
    static bool IsRecovered( const AlignmentParFrame &apf) {return test_ith_bit(2,apf.flag);}

    static bool IsApplied(   const AlignmentParView &apv) {return test_ith_bit(0,apv.flag);}
    static bool IsPin(       const AlignmentParView &apv) {return test_ith_bit(1,apv.flag);}
    static bool IsFound(     const AlignmentParView &apv) {return test_ith_bit(2,apv.flag);}
    
    bool IsBug0(AView &v1, AView &v2);

    void CorrectCols(TTree &tree);
    void SaveColsCorrToRun(const char *fin);
    void t(const char *fin, const char *fout, int algorithm, const char *cut="1");

    ClassDef(EdbViewMap,1)  // views map
};

#endif /* ROOT_EdbViewMap */

