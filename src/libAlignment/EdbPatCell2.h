#ifndef ROOT_EdbPatCell2
#define ROOT_EdbPatCell2

#include "EdbPattern.h"
#include "EdbCell2.h"

//-------------------------------------------------------------------------------------------------
class EdbPatCell2 : public EdbCell2
{
public:
  float eDXmin,  eDYmin;        // min position difference for the doublets cutout
  float eDTXmin, eDTYmin;       // min angular  difference for the doublets cutout

public:
  EdbPatCell2();
  virtual ~EdbPatCell2(){}

  void  InitPat(EdbPattern &pat, int nx, int ny);   // define limits to fit the pattern
  void  InitPatBin(EdbPattern &pat, float binx, float biny);   // define limits to fit the pattern

  int   FillCell(EdbPattern &pat);                   // limits should be already defined
  int   FillCell(TObjArray  &pat);                   // limits should be already defined
  int   DoubletsFilterOut();
  int   FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arr1, TObjArray &arr2);
  int   FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arr1, TObjArray &arr2, float min[2], float max[2]);
  int   FillCombinations(EdbPatCell2 &cell, int ir2[2], TObjArray &arr1, TObjArray &arr2, int   min[2], int   max[2]);

  ClassDef(EdbPatCell2,1)  // helper class to analyse the single pattern
};

#endif /* ROOT_EdbPatCell2 */
