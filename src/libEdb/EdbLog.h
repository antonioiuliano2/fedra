#ifndef ROOT_EdbLog
#define ROOT_EdbLog

#include "TError.h"
#include "TNtuple.h"

R__EXTERN Int_t   gEDBDEBUGLEVEL;
R__EXTERN FILE   *gEDBLOGFILE;

R__EXTERN TNtuple   *gDIFF;

/* enum EDebugLevel { */
/*    kDebNone         = 0, */
/*    kDebMin          = 1, */
/*    kDebNormal       = 2, */
/*    kDebHigh         = 3, */
/*    kDebAll          = 4 */
/* }; */

extern void Log0(Int_t level, const char *location, const char *msgfmt, va_list va);
extern void Log(Int_t level, const char *location, const char *msgfmt, ...);

#endif /* ROOT_EdbLog */
