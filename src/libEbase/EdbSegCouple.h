#ifndef ROOT_EdbSegCouple
#define ROOT_EdbSegCouple
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegCouple                                                         //
//                                                                      //
// service class for couples handling                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbSegP.h"

//______________________________________________________________________________
class EdbSegCouple : public TObject {

 private:
  Int_t   eID1, eID2;
  Float_t eCHI2;      // final chi2 calculation based on the linked track
  Float_t eCHI2P;     // fast chi2 estimation used for couples selection
  Int_t   eN1, eN2;         // n1,n2 - is rating of the segment (starting from 1)
  Int_t   eN1tot, eN2tot;   // total number of entries for the segment

 public:
  EdbSegP  *eS;    //!  the result of the fit
  EdbSegP  *eS1;   //!  pointers - useful when all segments are in memory
  EdbSegP  *eS2;   //!
  bool     eIsOwner; //!
  
 private:
  static int egSortFlag; //! 0: chi2p only; 1: N1()*10000+N2()*10000000 + CHI2()/100

 public:
  EdbSegCouple();
  EdbSegCouple(const EdbSegCouple &sc );
  EdbSegCouple( int id1, int id2 )
    { Set0();eID1=id1;eID2=id2; eCHI2=0; eCHI2P=0; eN1=0; eN2=0; }
  ~EdbSegCouple();

  void Set0();
  void SetN1(int n1)  {eN1=n1;}
  void SetN2(int n2)  {eN2=n2;}
  void SetN1tot(int n)  {eN1tot=n;}
  void SetN2tot(int n)  {eN2tot=n;}
  void SetCHI2(float chi2)  {eCHI2=chi2;}
  void SetCHI2P(float chi2)  {eCHI2P=chi2;}

  void Print();

  static void SetSortFlag(int s=0);
  static int     SortFlag() {return egSortFlag;}

  int  ID1()     const {return eID1;}
  int  ID2()     const {return eID2;}
  int   N1()     const {return eN1;}
  int   N2()     const {return eN2;}
  int   N1tot()  const {return eN1tot;}
  int   N2tot()  const {return eN2tot;}
  float CHI2()   const {return eCHI2;}
  float CHI2P()  const {return eCHI2P;}

  //ULong_t Hash() const { return fValue; }
  Bool_t  IsEqual(const TObject *obj) const
    { return
	(ID1()==((EdbSegCouple*)obj)->ID1())&&
	(ID2()==((EdbSegCouple*)obj)->ID2()); }
  Bool_t  IsSortable() const { return kTRUE; }
  Int_t   Compare(const TObject *obj) const;

  ClassDef(EdbSegCouple,1)  // couple of segments
};
#endif /* ROOT_EdbSegCouple */
