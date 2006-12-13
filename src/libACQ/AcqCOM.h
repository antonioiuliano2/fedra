#ifndef ROOT_AcqCOM
#define ROOT_AcqCOM
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqCOM                                                      //
//                                                                      //
// Interface to serial port							        //
//								                                        //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"

//______________________________________________________________________________
class AcqCOM : public TObject {

 
 public:
  AcqCOM();
  int Open(int port, int baud);
  void Close();
  int SendString(char * string, int N);
  int RecvString(char * string, int N); //nonblocking call!, returns number of received chars
  void Flush();
  void CleanAll();
  virtual ~AcqCOM();
   ClassDef(AcqCOM,1)  
};



#endif /* ROOT_AcqCOM */
