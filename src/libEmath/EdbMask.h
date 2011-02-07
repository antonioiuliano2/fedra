#ifndef ROOT_EdbMask
#define ROOT_EdbMask

#include "TObject.h"
#include "TArrayC.h"

//______________________________________________________________________________
class EdbMask : public TObject {

 private:
  TArrayC    eBuffer;           // body of the mask

 public:
  EdbMask() {}
  EdbMask(int n) {eBuffer.Set(n);}
  ~EdbMask() {}

  int  At(int i) { if(i<0||i>=eBuffer.fN) return 0; else return (int)(eBuffer[i]); }
  void SetAt(int i, int val) { eBuffer.AddAt((Char_t)val, i);  }

  ClassDef(EdbMask,1)  // mask service class
};

#endif /* ROOT_EdbMask */
