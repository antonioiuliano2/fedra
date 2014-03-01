#ifndef ROOT_EdbCouplesTree
#define ROOT_EdbCouplesTree

#include "TTree.h"
#include "TCut.h"
#include "EdbSegCouple.h"
#include "EdbAffine.h"
#include "EdbMask.h"

class TIndex2;
class EdbPattern;

//-------------------------------------------------------------------------------------------------
class EdbCouplesTree : public TObject
{
public:
  // branches:
  EdbSegP      *eS1,*eS2,*eS;     // segments to be written
  EdbSegCouple *eCP;              // couples object
  Float_t       eXv, eYv;         // relative center
  Int_t         ePid1, ePid2;

  TTree *eTree;                   // couples tree

  EdbMask     *eEraseMask;                // id's (entries) of segments to be erased when read couples tree

  //Correction parameters
  TCut         eCut;                      // cut to be applied on read
  bool         eApplyCorrections;         // if true - apply corrections stored in ePlate when do GetEntry
  EdbPlateP    ePlate;

public:
  EdbCouplesTree();
  virtual ~EdbCouplesTree();

  bool   InitCouplesTree(const char *name="couples", const char *fname=0, Option_t *mode="READ");
  TEventList *InitCutList();
  void   Close();
  Int_t  Fill(EdbSegP *s1, EdbSegP *s2, EdbSegP *s=0, EdbSegCouple *cp=0, float xv=0, float yv=0, int pid1=0, int pid2=0 );
  Int_t  Fill();
  bool   WriteTree();
  int    GetEntry(int i);
  void   ApplyCorrections();
  int    GetCPData( EdbPattern *pat, EdbPattern *p1=0, EdbPattern *p2=0, TIndex2 *trseg=0 );
  int    GetCPData( TObjArray &cparr );
  void   Print();

  ClassDef(EdbCouplesTree,1)  // service class for couples tree access
};
#endif /* ROOT_EdbCouplesTree */
