//-- Author :  Valeri Tioukov   15.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBasic                                                             //
//                                                                      //
// collection of general purpose algorithms                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdarg.h>

//__________________________________________________________________________
int Sfscanf(FILE *fp, char *mes, ...)
  // to provide fortran-like reading by strings
{
  char            buffer[256]="";
  int             ncols;
  va_list         marker;
 
  if (fgets (buffer, 256, fp) == NULL)
    return -1;
  else{
    va_start(marker,mes);
    ncols = vsscanf( buffer, mes, marker );
    va_end(marker);
  }
  return ncols;
}
