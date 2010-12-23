#ifndef ROOT_EdbPositionAlignment
#define ROOT_EdbPositionAlignment

//#include <TNtuple.h>
//#include <TString.h>
#include "EdbBrick.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbCell1.h"
#include "EdbPatCell2.h"

//-------------------------------------------------------------------------------------------------
class EdbPositionAlignment : public TObject
{
public:
  // service objects:
  EdbPatCell2 ePC1, ePC2;        // cells with the pointers to segments
  TObjArray   eComb1, eComb2;    // array with the selected combinations of segments
  TObjArray   eTracks;           // tracks created with segments of eComb1, eComb2
  TObjArray   eSegCouples;       // segment couples objects to fill couples format tree
  EdbPatCell2 ePC;               // cells with the pointers to tracks

  // input parameters:
  float   eX0, eY0;              // coordinates of the center of the zone (for the ePeakNT only)

  Float_t eXcell, eYcell;        // cell size (for example 50 microns)
  Float_t eDTXmax,eDTYmax;       // max angular acceptance (ex: 0.15) for the coinsidences
  Float_t eBinX, eBinY;          // bin size for the differential hist (for example 5 microns)

  Float_t eDXmin,  eDYmin;       // min position difference for the dublets cutout
  Float_t eDTXmin, eDTYmin;      // min angular  difference for the dublets cutout
  Float_t eRpeak;                // coordinate peak acceptance

  // z-selection block: 
  Float_t eZ1from,eZ1to;         // limits in Z for the peak search (ePat1)
  Float_t eZ2from,eZ2to;         // limits in Z for the peak search (ePat2)
  Int_t   eNZ1step, eNZ2step;    // number of steps for the z-selection
 
 // results of the z-selection:
  Float_t eZ1peak, eZ2peak;     // peak position in Z
  Float_t eXpeak,eYpeak;        // peak position in X,Y
  Int_t   eNpeak;               // number of found combinations
  EdbAffine2D *eAff;            // the found affine transformation (when applied to pattern1 gives pattern2 )
  EdbH2       eHpeak;           // histogram used for peak selection
  EdbH2       eHDZ;             // histogram used for  Z-selection

  TString     eSmoothKernel;    // used to smooth histograms

  TTree   *ePosTree;            // optional: tree with s1:s2 for the selected combinations

  // linking:
  Float_t eS0x,eS0y,eS0tx,eS0ty;    // sigmas at 0 angle for the chi2 calculation
  Float_t eWbaseMin;                // cut for the w of basetrack to accept it
  Float_t eChi2Max;                 // cut for the chi2 of the basetrack
  
  // shrinkage-selection block: 
  Float_t eShr1from,eShr1to;        // limits in Shr for the peak search (ePat1)
  Float_t eShr2from,eShr2to;        // limits in Shr for the peak search (ePat2)
  Int_t   eNShr1step, eNShr2step;   // number of steps for the Shr-selection
  // results of the shrinkage selection:
  EdbH2       eHShr1;             // histogram used for the shrinkage-selection
  EdbH2       eHShr2;             // histogram used for the shrinkage-selection


  Bool_t      eDoRot;             // if true - perform the rotation selection
  EdbH1       eRot;               // definition of the rotation steps
                                     
public:
  EdbPositionAlignment();
  virtual ~EdbPositionAlignment();

  bool  ActivatePosTree(const char *name="postree");
  int   WritePosTree();
  int   ReadPosTree(TTree *tree,  EdbPattern *p=0, EdbPattern *p1=0, EdbPattern *p2=0, TEventList *lst=0 );
  int   FillPosTree( float dz1, float dz2, int flag); //{ return FillPosTree(eComb1,eComb2,dz1,dz2,flag); }
  //int   FillPosTree(TObjArray &arr1,TObjArray &arr2, float dz1, float dz2, int flag);
  //int   FillPosTree(EdbPattern &p1, EdbPattern &p2, float dz1, float dz2, int flag);
  void  PrintStat();
  void  PrintSummary();

  int   Align();

  int   SelectZone( float min[2], float max[2], TObjArray &arr1, TObjArray &arr2, float maxDens );
  int   SelectZoneSide( EdbPatCell2 &pc, float min[2], float max[2], TObjArray &arr, int nmax=kMaxInt );
  static int   SelectBestComptons( TObjArray &a,  TObjArray &arr, int nmax);

  int   FillArrays(EdbPattern &p1 ,  EdbPattern &p2);
  int   FillArrays(TObjArray &arr1,  TObjArray &arr2, float xymin[2], float xymax[2] );
  int   SpotsFilterOut(int nmax);
  int   DoubletsFilterOut(bool checkview=true);
  int   DoubletsFilterOutSide(EdbPatCell2 &pc, bool checkview=true);
  int   FillCombinations();
  int   FillCombinations(EdbPatCell2 &pc1, EdbPatCell2 &pc2, float dx, float dy, float dtx, float dty);

  int   SelectNarrowPeakDXDY(float dr, EdbH2 &hxy);
  void  PositionPlotA(EdbH2 &hd, float DR=0, TObjArray *arr1=0, TObjArray *arr2=0);
  void  FindDiff(TObjArray &arr1, TObjArray &arr2, EdbPatCell2 &pc1, EdbPatCell2 &pc2, 
		 float &dx, float &dy, float &dtx, float &dty );
  void  FindDiff12(float &dx, float &dy, float &dtx, float &dty );
  void  FindDiff10(float &dx, float &dy, float &dtx, float &dty );
  void  FindDiff20(float &dx, float &dy, float &dtx, float &dty );
  void  AlignWithTracks(int nang=3, int npos=0);
  void  FindCorrections(EdbPattern &pat1, EdbPattern &pat2, float DZ, bool doShrink);

  void  PositionPlot(float dz1, float dz2, EdbH2 &hd);
  void  PositionPlot(EdbH2 &hd);
  void  Zselection(EdbH2 &hd);

  void  ShrinkageSelection(float dzbase);
  int   ShrinkageSelectionSide(EdbPatCell2 &pc1, EdbPatCell2 &pc2, EdbH2 &hshr, int nstep, float deltaShr);
  int   Link();
  //int   Link(EdbPattern &p1, EdbPattern &p2);

  float Xcorr(EdbSegP &s, float dz) {return s.X() + dz*s.TX();}
  float Ycorr(EdbSegP &s, float dz) {return s.Y() + dz*s.TY();}
  float Xcorr(EdbSegP &s, float dz, float dx) {return dx + s.X() + dz*s.TX();}
  float Ycorr(EdbSegP &s, float dz, float dy) {return dy + s.Y() + dz*s.TY();}

  int SelectPeak(EdbPattern &p1,EdbPattern &p2, float dx, float dy, float dz1, float dz2 ); 
  int SelectPeak(EdbPattern &p1,EdbPattern &p2, float dx=10, float dy=10 )  
    { return SelectPeak(p1,p2,dx,dy,eZ1peak,eZ2peak); } 

  void ResetPeak() { eZ1peak=eZ2peak=eXpeak=eYpeak=0; eNpeak=0; }

  void RankCouples0();
  void RankCouples();
  void RankCouples( TObjArray &arr1,TObjArray &arr2, TObjArray &arrtr );
  void RankCouplesFast( TObjArray &arrtr );

  void ResetPositionCorr() { ePC1.eDX = ePC1.eDY = ePC2.eDX = ePC2.eDY = 0;}
  void ResetAngularCorr()  { ePC1.eDTX = ePC1.eDTY = ePC2.eDTX = ePC2.eDTY = 0;}

  //  void ApplyCorrections(EdbPlateP &plate);

  int  WideSearchXY(EdbPattern &pat1, EdbPattern &pat2, EdbH2 &hxy, float dz, float xmin, float xmax, float ymin, float ymax);

  void SaveAsTree(EdbPattern &pat, const char *name);


  ClassDef(EdbPositionAlignment,1)  // new alignment class developed mainly for compton search
};
#endif /* ROOT_EdbPositionAlignment */
