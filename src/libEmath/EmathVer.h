#ifndef ROOT_EmathVer
#define ROOT_EmathVer
#include "Riostream.h"
#include "EdbLog.h"
class  EmathVer{
 public:
  EmathVer() {Log(2,"libEmath", "\tloaded...");}
};
EmathVer gInstance_EmathVer;

#endif /* EmathVer */
