#ifndef ROOT_EdbCombGen
#define ROOT_EdbCombGen

#include <vector>
#include <iostream>
#include <algorithm>
#include <bitset>

using namespace std;

#include "TObject.h"
#include "TObjArray.h"

//______________________________________________________________________________
class EdbCombGen : public TObject {

 private:
  TObjArray    eElements;           // objects to be combinated
  vector<bool> eBits;               // bits array
  Int_t        eSize;               // total number of items
  Int_t        eItems;              // number of items to be selected
  Bool_t       eNext;

 public:
  EdbCombGen() {}
  EdbCombGen( TObjArray &elements, // Original items
              int nitems=-1);// :    // Number of items to choose
              //eElements(elements), eNext(true);
  ~EdbCombGen() {}

   bool NextCombination(TObjArray &selected, TObjArray &regected);

  ClassDef(EdbCombGen,1)  // objects combinations generator
};

#endif /* ROOT_EdbCombGen */
