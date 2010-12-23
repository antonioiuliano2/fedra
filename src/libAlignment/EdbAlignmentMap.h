#ifndef ROOT_EdbAlignmentMap
#define ROOT_EdbAlignmentMap

#include <TH2F.h>
#include <TGraph2D.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TEnv.h>
#include "EdbBrick.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbPositionAlignment.h"

//-------------------------------------------------------------------------------------------------
class EdbAlignmentMap : public TObject
{
 public:
  TEnv       *eEnv;             // environment used to pass the parameters
  EdbPattern *ePat1;            // big patterns to be splitted and aligned piece by piece
  EdbPattern *ePat2;
  float eDensityMax;           // the max segments density for patterns selection (in N/100/100 microns)

  EdbPlateP *ePlate; // 

  EdbPositionAlignment  eGlobal; // service object for the zones selection

  TVector3 eGV1,eGV2;           // global offsets found by the test alignment (dx:dy:dz)

  float eXcell, eYcell;         // approximate zones size (for example 10000 microns)
  int   eNx, eNy;               // number of divisions calculated using eXcell, eYcell

  EdbCell2   eMap;              // in each cell TArratF with (x,y, dx,dy,dz1,dz2, n)
  
  TGraph2D *eGraphDX;
  TGraph2D *eGraphDY;
  TGraph2D *eGraphDZ1;
  TGraph2D *eGraphDZ2;

  TFile   *eOutputFile;
  TTree   *eMapTree;

 public:
  EdbAlignmentMap(const char *file=0, const char *mode=0);
  virtual ~EdbAlignmentMap();

  int   InitFile(const char *file=0, const char *mode=0);
  void  AlignMap();
  void  SaveAll();
  int   FillMapTree( EdbPositionAlignment &pol, int izone );
  void  SaveMap( const char *file );
  void  ExtractMapFromTree();

  int   ApplyMap(EdbPattern &pat);

  void  CheckPattern(EdbPattern &p, const char *suffix);
  int   CheckDZbase(TEnv &cenv, EdbPattern &p1, EdbPattern &p2);

  int   SelectSampleForShrinkageCorr(EdbPattern &p, EdbPattern &psel, float wmin, float wmax, float tmin, float tmax  );
  int   Link();
  int   Link(TEnv &cenv, EdbPattern &p1, EdbPattern &p2, EdbPlateP &plate);

  void   Link( const char *file, EdbPlateP &plate );
  void  get_run_patterns(const char *runfile, TEnv *cenv, EdbPattern &p1, EdbPattern &p2);
  static void  GetPostreeAsPat(EdbPattern &pat, const char *filename);

  int   CheckXY(TEnv &cenv, EdbPattern &p1all, EdbPattern &p2all);

  ClassDef(EdbAlignmentMap,1)  // 2-d alignment map finder
};

#endif /* ROOT_EdbAlignmentMap */
