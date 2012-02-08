#ifndef ROOT_EdbViewMatch
#define ROOT_EdbViewMatch
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewMatch - nearby views matching by clusters&grains - the primary purpose is distortions correction  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "TFile.h"
#include "TEnv.h"
#include "EdbCell2.h"


//______________________________________________________________________________
class EdbClMatch : public TObject {
public:
  Float_t  eX,eY,eZ;   // cluster coords in view RS
  Float_t  eXv,eYv;   // view coords
  Int_t    eView;
  Int_t    eFrame;
  
  EdbClMatch(){};
  EdbClMatch(float x,float y,float z, float xv,float yv, int view, int frame)
  { eX=x; eY=y; eZ=z; eXv=xv; eYv=yv; eView=view; eFrame=frame; }
  virtual ~EdbClMatch(){}
  
  ClassDef(EdbClMatch,1)  // service structure for views matching
};

//______________________________________________________________________________
class EdbViewMatch : public TObject {
private:
  int        eNClMin;      // minimal number of clusters inside grain to be used for corrections
  float      eR2CenterMax; // the maximal distance to the matrix center for the reference cluster
  float      eRmax;        // acceptance for clusters matching
  
  TClonesArray  eCl;       // array of EdbClMatch objects
  TClonesArray  eGr;       // array of EdbSegment objects (cluster "clouds" in this case)
  EdbCell2   eGMap;        // map of grains (EdbSegments)
  EdbCell2   eCorrMap;     // map of corrections
  
  float eXpix,  eYpix;
  int   eNXpix, eNYpix;

  float  eCorrectionMatrixStepX;
  float  eCorrectionMatrixStepY;
  
public:
  TFile *eOutputFile;

public:
  EdbViewMatch();
  virtual ~EdbViewMatch(){}

  void InitGMap();
  void InitCorrMap();
  
  void AddCluster( float x,float y,float z, float xv,float yv, int view, int frame );
  
  void CalculateGrRef();
  void CalculateCorr();
  void DrawCorrMap();
  void GenerateCorrectionMatrix(const char *addfile);
  void SetPar(TEnv &env);
  void SetPixelSize(float xpix, float ypix) { eXpix=xpix; eYpix=ypix; }
  
  void Print();
  
  ClassDef(EdbViewMatch,1)  // 
};

#endif /* ROOT_EdbViewMatch */
