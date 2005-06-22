#ifndef ROOT_EdbTraceBack
#define ROOT_EdbTraceBack

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbTraceBack                                                         //
//                                                                      //
// define the counters logic here                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObject.h"
#include "EdbPattern.h"

//______________________________________________________________________________
class EdbTraceBack : public TObject {

 public:
  
  static void SetBaseTrackVid(  EdbSegP &s, int plate,  int piece,  int entry );
  static void FromBaseTrackVid( EdbSegP &s, int &plate, int &piece, int &entry );

  ClassDef(EdbTraceBack,1)  // define the counters logic here
};

#endif /* ROOT_EdbTraceBack */
