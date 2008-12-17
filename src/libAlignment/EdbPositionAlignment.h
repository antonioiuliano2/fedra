#ifndef ROOT_EdbPositionAlignment
#define ROOT_EdbPositionAlignment

#include <TNtuple.h>
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbPatCell2.h"

//-------------------------------------------------------------------------------------------------
class EdbPositionAlignment : public TObject
{
public:
  // input parameters:
  float   eX0, eY0;              // coordinates of the center of the zone (for the ePeakNT only)

  Float_t eZ1from,eZ1to;        // limits in Z for the peak search (ePat1)
  Float_t eZ2from,eZ2to;        // limits in Z for the peak search (ePat2)
  Int_t   eNZ1step, eNZ2step;   // number of steps for the peak search

  Float_t eXcell, eYcell;       // cell size (for example 50 microns)
  Float_t eDTXmax,eDTYmax;      // max angular acceptance (ex: 0.15) for the coinsidences
  Float_t eBinX, eBinY;         // bin size for the differential hist (for example 5 microns)

  Float_t eDXmin,  eDYmin;        // min position difference for the dublets cutout
  Float_t eDTXmin, eDTYmin;       // min angular  difference for the dublets cutout

  // service objects:
  EdbPatCell2 ePC1, ePC2;        // cells with the pointers to segments
  TObjArray   eComb1, eComb2;    // array with the selected combinations of segments

  // results of the alignment:
  Float_t eZ1peak, eZ2peak;     // peak position in Z
  Float_t eXpeak,eYpeak;        // peak position in X,Y
  Int_t   eNpeak;               // number of found combinations
  EdbAffine2D *eAff;            // the found affine transformation (when applied to pattern1 gives pattern2 )
  EdbH2       eHpeak;           // histogram used for peak selection
  EdbH2       eHDZ;             // histogram used for  Z-selection

  TTree   *ePosTree;            // optional: tree with s1:s2 for the selected combinations
                                     
public:
  EdbPositionAlignment();
  virtual ~EdbPositionAlignment();

  bool  ActivatePosTree();
  int   FillPosTree(TObjArray &arr1,TObjArray &arr2, float dz1, float dz2, int flag);
  int   FillPosTree(EdbPattern &p1, EdbPattern &p2, float dz1, float dz2, int flag);
  void  PrintStat();
  void  PrintSummary();

  int   Align();

  int   SelectZone( float min[2], float max[2], TObjArray &arr1, TObjArray &arr2, float maxDens );
  int   SelectZoneSide( EdbPatCell2 &pc, float min[2], float max[2], TObjArray &arr, int nmax=kMaxInt );

  int   FillArrays(EdbPattern &p1 ,  EdbPattern &p2);
  int   FillArrays(TObjArray &arr1,  TObjArray &arr2, float xymin[2], float xymax[2]);
  int   SpotsFilterOut(int nmax);
  int   DoubletsFilterOut();
  int   FillCombinations();
  int   FillCombinations(float min[2], float max[2]);

  void  PositionPlot(float dz1, float dz2, EdbH2 &hd);
  void  Zselection(EdbH2 &hd);

  float Xcorr(EdbSegP &s, float dz) {return s.X() + dz*s.TX();}
  float Ycorr(EdbSegP &s, float dz) {return s.Y() + dz*s.TY();}
  float Xcorr(EdbSegP &s, float dz, float dx) {return dx + s.X() + dz*s.TX();}
  float Ycorr(EdbSegP &s, float dz, float dy) {return dy + s.Y() + dz*s.TY();}

  int SelectPeak(EdbPattern &p1,EdbPattern &p2, float dx, float dy, float dz1, float dz2 ); 
  int SelectPeak(EdbPattern &p1,EdbPattern &p2, float dx=10, float dy=10 )  
    { return SelectPeak(p1,p2,dx,dy,eZ1peak,eZ2peak); } 

  void ResetPeak() { eZ1peak=eZ2peak=eXpeak=eYpeak=0; eNpeak=0; }

  ClassDef(EdbPositionAlignment,1)  // new alignment class developed mainly for compton search
};
#endif /* ROOT_EdbPositionAlignment */
