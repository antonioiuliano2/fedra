#ifndef ROOT_EdbCouplesTree
#define ROOT_EdbCouplesTree

#include "TTree.h"
#include "EdbSegCouple.h"
#include "EdbAffine.h"

//-------------------------------------------------------------------------------------------------
class EdbCouplesTree : public TObject
{
public:
  // branches:
  EdbSegP      *eS1,*eS2,*eS;     // segments to be written
  EdbSegCouple *eCP;              // couples object
  Float_t       eXv, eYv;         // relative center
  Int_t         ePid1, ePid2; 
   //Float_t       eDZ1, eDZ2;       // z-corrections
  //Int_t         eFlag; 
  //Float_t       eChi2;            // ??? required?

  TTree *eTree;                   // couples tree

public:
  EdbCouplesTree();
  virtual ~EdbCouplesTree(){}

  bool  InitCouplesTree(const char *name="couples", const char *fname=0, Option_t *mode="READ");
  Int_t Fill(EdbSegP *s1, EdbSegP *s2, EdbSegP *s=0, EdbSegCouple *cp=0, float xv=0, float yv=0, int pid1=0, int pid2=0 );
  Int_t Fill();
  bool  WriteTree();
  int   GetEntry(int i) {return eTree->GetEntry(i);}
  void  Print();

  ClassDef(EdbCouplesTree,1)  // service class for couples tree access
};
#endif /* ROOT_EdbCouplesTree */
