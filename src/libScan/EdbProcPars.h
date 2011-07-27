#ifndef ROOT_EdbProcPars
#define ROOT_EdbProcPars
#include "TEnv.h"

class EdbProcPars : public TObject
{
public: 
  EdbProcPars(){}
  virtual ~EdbProcPars(){}
  
  static void SetLinkDef(TEnv  &cenv);
  static void SetAlignDef(TEnv &cenv);
  static void SetTrackDef(TEnv &cenv);
  
  ClassDef(EdbProcPars,1)  // default reconstruction parameters handling
};
#endif /* ROOT_EdbProcPars */
