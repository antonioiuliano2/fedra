#ifndef ROOT_AcqTrackReconstructor
#define ROOT_AcqTrackReconstructor
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqTrackReconstructor                                                      //
//                                                                      //
// Interface to Microtracks reconstructor							        //
//								                                        //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "EdbView.h"

//______________________________________________________________________________
class AcqTrackReconstructor : public TObject {

 
 public:
  AcqTrackReconstructor();
  virtual ~AcqTrackReconstructor();
  int RecRating(EdbView* View); // reconstruction based on rating propagation
  int RecNetscan(EdbView* View); // reconstruction a la Netscan (slow)
  int RecSySal(EdbView* View); // reconstruction a la SySal 
  int RecValeri(EdbView* View); //reconstruction a'la V. Tiukov

   ClassDef(AcqTrackReconstructor,1)  
};



#endif /* ROOT_AcqTrackReconstructor */
