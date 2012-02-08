#ifndef ROOT_EdbCorrectionMapper
#define ROOT_EdbCorrectionMapper

#include "EdbCell2.h"
#include "EdbSegP.h"
#include "EdbAlignmentV.h"
class TCanvas;

//-------------------------------------------------------------------------------------------------
class EdbCorrectionBin : public TObject
{
 public:
  
  EdbAlignmentV eAl;
  
  EdbLayer    eLayer;
  
 public:
  EdbCorrectionBin();
  virtual ~EdbCorrectionBin();

  virtual void AddSegCouple(EdbSegP *s1, EdbSegP *s2) { eAl.AddSegCouple(s1,s2); }
  
  virtual void CalculateCorrections();
  void         AddBin(EdbCorrectionBin &bin);
  
  ClassDef(EdbCorrectionBin,1)  // corrections bin
};

//-------------------------------------------------------------------------------------------------
class EdbCorrectionMapper : public EdbCorrectionBin
{
 public:
  EdbCell2         eMapAl;            // working object with local alignments
  EdbCorrectionMap eMap;              // result: corrections map
  
  EdbH2 eHdxy;          // dxdy
  EdbH2 eHdtxy;         // dtxdty
  EdbH1 eHdz;           // dz
  EdbH1 eHshr;          // 
   
  EdbH2 eHdty_ty;      //
  
  EdbH2 eHxy1  , eHxy2;
  EdbH2 eHtxty1, eHtxty2;
  
  EdbID eID1,eID2;
  float eZ1, eZ2;
  
  int eNcpMin;            // min number of coincidences to accept correction
  
 public:
  EdbCorrectionMapper();
  virtual ~EdbCorrectionMapper();
  
  int               InitMap(int nx, float minx, float maxx, int ny, float miny, float maxy );
  EdbCorrectionBin *GetBin(int i) { return (EdbCorrectionBin*)(eMapAl.GetObject( i, 0)); }
  EdbCorrectionBin *GetBin(float x, float y) { return (EdbCorrectionBin*)(eMapAl.GetObject(x, y, 0)); }
  
  void              CalculateCorrections();
  void              MakeCorrectionsTable();
  void              UpdateLayerWithLocalCorr( EdbLayer &la );
  void              AddSegCouple(EdbSegP *s1, EdbSegP *s2);
  void              Fill(EdbSegP &s1, EdbSegP &s2);
  void              Write(const char *suffix = "");
  TCanvas          *DrawSum(const char *suffix);
  TCanvas          *DrawMap(EdbCorrectionMap &map, const char *suffix = "");
  void              SetCorrDZ();
  EdbH2            *MapDZ();
  EdbH2            *MapDV(int ivar);

  void              SmoothCorrections();

  ClassDef(EdbCorrectionMapper,1)  // to calculate correction map
};

#endif /* ROOT_EdbCorrectionMapper */
