#ifndef ROOT_EdbRunParent
#define ROOT_EdbRunParent

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunParent                                                         //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbRunTracking.h"

//______________________________________________________________________________
class EdbRunParent : public EdbRunTracking {

 private:

  Float_t   eIPmax; // max ip for parent slection

 public:
  EdbRunParent();
  ~EdbRunParent();

  int FindParentCandidates( EdbSegP &s, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 );

  int FindComplimentsIP( EdbSegP &s, EdbPattern &pat, TObjArray &found );

  ClassDef(EdbRunParent,1)  // to look for parent bt/mt in raw data
};

#endif /* ROOT_EdbRunParent */

