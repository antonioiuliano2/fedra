#include "snprintf.h"
#include "TSystem.h"
#include "TDatime.h"
#include "EdbLog.h"

Int_t gEDBDEBUGLEVEL = 2;
FILE *gEDBLOGFILE=0;

//______________________________________________________________________________
void Log(int level, const char *rout, const char *fmt, ...)
{
// Print message to the logfile and to stdout.

  if(level>gEDBDEBUGLEVEL) return;

  static Int_t buf_size = 2048;
  static char *buf = 0;

  va_list ap;
  va_start(ap, fmt);

 again:
  if (!buf)
    buf = new char[buf_size];

  Int_t n = vsnprintf(buf, buf_size, fmt, ap);
  // old vsnprintf's return -1 if string is truncated new ones return
  // total number of characters that would have been written
  if (n == -1 || n >= buf_size) {
    if (n == -1)
      buf_size *= 2;
    else
      buf_size = n+1;
    delete [] buf;
    buf = 0;
    va_end(ap);
    va_start(ap, fmt);
    goto again;
  }
  va_end(ap);

  
  fprintf(stdout, "%-16s: ", rout);
  fprintf(stdout, "%s\n", buf);
  if(gEDBDEBUGLEVEL>2) return;     // do not print to file
  TDatime t;
  if(gEDBLOGFILE) {
    fprintf(gEDBLOGFILE, "%s> ", t.AsSQLString());
    fprintf(gEDBLOGFILE, "%-16s: ", rout);
    fprintf(gEDBLOGFILE, "%s\n", buf);
  }
}
