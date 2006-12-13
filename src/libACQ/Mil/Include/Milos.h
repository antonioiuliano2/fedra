/************************************************************************/
/*
*
* Filename     :  MILOS.H
* Owner        :  Matrox Imaging dept.
* Content      :  This file contains the defines necessary to use the
*                 Matrox Imaging Library (MIL 4.0 and up) "C" user interface.
*                 under different operating systems
* Comments     :  This file has first become necessary to support the unicode
*                 strings under Windows CE.
*
* COPYRIGHT (c) Matrox Electronic Systems Ltd.
* All Rights Reserved
*************************************************************************/

#ifndef __MILOS_H
#define __MILOS_H

/************************************************************************/
/* Note that this header file might be included as a stand alone version*/
/************************************************************************/
#ifndef M_MIL_USE_OS
#define M_MIL_USE_WINDOWS       1
#endif

// Set tiff optimisation define in PPC
#if M_MIL_USE_PPC
   #ifndef M_MIL_USE_TIFF_OPTIMISATION
      #define M_MIL_USE_TIFF_OPTIMISATION M_MIL_USE_PPC
   #endif
#endif


/************************************************************************/
/* Include string.h                                                     */
/************************************************************************/
#include <string.h>

#if M_MIL_USE_PPC
   #include <ctype.h>
   #include <wchar.h>
   #include <stdarg.h>
   #ifdef __cplusplus
   extern "C"
   {
   #endif
      int  PPCFileFuncI(...);
      long PPCFileFuncL(...);
      void PPCFileFuncV(...);
      double PPCFileFuncD(...);
      char *PPCFileFuncC(char *string,...);
   #ifdef __cplusplus
   }
   #endif
   #if M_MIL_USE_ODYSSEY
      #define IM_PTYPE
   #endif
#endif

/************************************************************************/
/* STRINGS RELATED DEFINITIONS                                          */
/************************************************************************/
#if   M_MIL_USE_CE
   #define MIL_TEXT(quote)        L##quote
   #define MT(quote)              L##quote
   typedef unsigned short         MILTCHAR;     //obsolete
   typedef unsigned short *       LPMILSTR;     //obsolete
   typedef unsigned short         MIL_TEXT_CHAR;
   typedef unsigned short *       MIL_TEXT_PTR;
   typedef const unsigned short * LPCMILSTR;
   typedef long                   MIL_INTADDR;
   typedef unsigned long          MIL_UINTADDR;
#elif M_MIL_USE_WINDOWS || M_MIL_USE_PPC || M_MIL_USE_LINUX
   #if M_MIL_USE_WIN64
         #define MIL_TEXT(quote)        quote
         #define MT(quote)              quote
         typedef char                   MILTCHAR;     //obsolete
         typedef char *                 LPMILSTR;     //obsolete
         typedef char                   MIL_TEXT_CHAR;
         typedef char *                 MIL_TEXT_PTR;
         typedef const char *           LPCMILSTR;
         typedef __int64                MIL_INTADDR;
         typedef unsigned __int64       MIL_UINTADDR;
   #else
   		#define MIL_TEXT(quote)        quote
   		#define MT(quote)              quote
         typedef char                   MILTCHAR;     //obsolete
         typedef char *                 LPMILSTR;     //obsolete
         typedef char                   MIL_TEXT_CHAR;
         typedef char *                 MIL_TEXT_PTR;
         typedef const char *           LPCMILSTR;
   		typedef long                   MIL_INTADDR;
         typedef unsigned long          MIL_UINTADDR;
   #endif
#else
   #error Operating system not supported in Milos.h!
#endif

// Regular string functions' definition
#if M_MIL_USE_CE
   #define     MOs_ltoa     _ltow
   #define     MOs_itoa     _itow
   #define     MOs_ultoa    _ultow
   #define     MOs_atoi     _wtoi
   #define     MOs_atol     _wtol
   #define     MOs_strcpy   wcscpy
   #define     MOs_strncpy  wcsncpy
   #define     MOs_strcmp   wcscmp
   #define     MOs_stricmp  wcsicmp
   #define     MOs_strncmp  wcsncmp
   #define     MOs_strnicmp wcsnicmp
   #define     MOs_strcat   wcscat
   #define     MOs_strncat  wcsncat
   #define     MOs_strchr   wcschr
   #define     MOs_ANSIstrchr strchr
   #define     MOs_strlen   wcslen
   #define     MOs_ANSIstrlen strlen
   #define     MOs_sprintf  swprintf
   #define     MOs_strstr   wcsstr
   #define     MOs_sscanf   swscanf
   #define     MOs_fprintf  fwprintf
   #define     MOs_strtod   wcstod
   #define     MOs_strupr   wcsupr
   //In CE, because of the memory allocation/deallocation, wcsdup must not be used.
   #ifdef __cplusplus
   inline unsigned short * MOs_strdup(const unsigned short * wcsSource)
   {
        unsigned short * wcsCopy = (unsigned short *)malloc(sizeof(unsigned short)*(wcslen(wcsSource) + 1));
        if(wcsCopy)
            wcscpy(wcsCopy, wcsSource);
        return wcsCopy;
   }
   #else  //__cplusplus
   #define MOs_strdup(wcsSource) (wcscpy(malloc(sizeof(unsigned short)*(wcslen(wcsSource) + 1))),wcsSource)
   #endif //__cplusplus
   #define     MOs_strlwr   wcslwr
   #define     MOs_printf   wprintf
   #define     MOs_fputs    fputws
   #define     MOs_strtok   wcstok
   #define     MOs_ungetc   ungetwc
   #define     MOs_tolower  towlower
   #define     MOs_toupper  towupper
   #define     MOs_strcspn  wcscspn
   #define     MOs_asctime  _wasctime
#elif (M_MIL_USE_WINDOWS || M_MIL_USE_PPC)
   #define     MOs_ltoa     _ltoa
   #define     MOs_itoa     _itoa
   #define     MOs_ultoa    _ultoa
   #define     MOs_atoi     atoi
   #define     MOs_atol     atol
   #define     MOs_strcpy   strcpy
   #define     MOs_strncpy  strncpy
   #define     MOs_strcmp   strcmp
   #define     MOs_stricmp  stricmp
   #define     MOs_strncmp  strncmp
   #define     MOs_strnicmp strnicmp
   #define     MOs_strcat   strcat
   #define     MOs_strncat  strncat
   #define     MOs_strchr   strchr
   #define     MOs_ANSIstrchr strchr
   #if M_MIL_USE_WIN64
      #define MOs_strlen       (long)(strlen) //size_t is an __int64 under Win64,
   #else
      #define     MOs_strlen   strlen
   #endif
   #if M_MIL_USE_WIN64
      #define     MOs_ANSIstrlen    (long)(strlen) //size_t is an __int64 under Win64,
   #else
      #define     MOs_ANSIstrlen    strlen
   #endif
   #if M_MIL_USE_PPC
      #define     MOs_fprintf  PPCFileFuncI
      #define     MOs_printf   PPCFileFuncI
   #else
      #define     MOs_fprintf  fprintf
      #define     MOs_printf   printf
   #endif
   #define     MOs_sprintf  sprintf
   #define     MOs_strstr   strstr
   #define     MOs_sscanf   sscanf
   #define     MOs_strtod   strtod
   #define     MOs_strupr   strupr
#if M_MIL_USE_WINDOWS
   #define     MOs_strdup   _strdup
#else
   #define     MOs_strdup   strdup
#endif
   #define     MOs_strlwr   strlwr
   #define     MOs_strtok   strtok
   #define     MOs_ungetc   ungetc
   #define     MOs_tolower  tolower
   #define     MOs_tcscpy   _tcscpy
   #define     MOs_tcslen   _tcslen
   #define     MOs_tcsncpy  _tcsncpy
   #define     MOs_toupper  toupper
   #define     MOs_strcspn  strcspn
   #define     MOs_asctime  asctime
#elif M_MIL_USE_LINUX
   #define     MOs_ltoa         // use sprintf instead
   #define     MOs_itoa         // use sprintf instead
   #define     MOs_ultoa        // use sprintf instead
   #define     MOs_atoi     atoi
   #define     MOs_atol     atol
   #define     MOs_strcpy   strcpy
   #define     MOs_strncpy  strncpy
   #define     MOs_strcmp   strcmp
   #define     MOs_stricmp  strcasecmp
   #define     MOs_strncmp  strncmp
   #define     MOs_strnicmp strncasecmp
   #define     MOs_strcat   strcat
   #define     MOs_strncat  strncat
   #define     MOs_strchr   strchr
   #define     MOs_ANSIstrchr strchr
   #define     MOs_strlen   strlen
   #define     MOs_ANSIstrlen    strlen
   #define     MOs_fprintf  fprintf
   #define     MOs_printf   printf
   #define     MOs_sprintf  sprintf
   #define     MOs_strstr   strstr
   #define     MOs_sscanf   sscanf
   #define     MOs_strtod   strtod
   #define     MOs_strupr   strupr
   #define     MOs_strdup   strdup
   #define     MOs_strlwr   strlwr
   #define     MOs_strtok   strtok
   #define     MOs_ungetc   ungetc
   #define     MOs_tolower  tolower
   #define     MOs_tcscpy
   #define     MOs_tcslen
   #define     MOs_tcsncpy
   #define     MOs_toupper  toupper
   #define     MOs_strcspn  strcspn
   #define     MOs_asctime  asctime
#else
   #error Operating system not supported in Milos.h!
#endif

/**********************************************************************/
/* int64 related defines                                              */
/**********************************************************************/
#if M_MIL_USE_WINDOWS
   // Windows defines
   typedef                __int64   MILINT64;      //obsolete
   typedef const          __int64   MILCINT64;     //obsolete
   typedef       unsigned __int64   MILUINT64;     //obsolete
   typedef const unsigned __int64   MILCUINT64;    //obsolete
   typedef       unsigned short     MILWCHAR;

   typedef                __int64   MIL_INT64;
   typedef const          __int64   MIL_CINT64;
   typedef       unsigned __int64   MIL_UINT64;
   typedef const unsigned __int64   MIL_CUINT64;
   typedef       unsigned short     MIL_WCHAR;

#elif M_MIL_USE_PPC || M_MIL_USE_LINUX
   // PPC defines
   typedef                long long MILINT64;      //obsolete
   typedef const          long long MILCINT64;     //obsolete
   typedef       unsigned long long MILUINT64;     //obsolete
   typedef const unsigned long long MILCUINT64;    //obsolete
   typedef       unsigned short     MILWCHAR;

   typedef                long long MIL_INT64;
   typedef const          long long MIL_CINT64;
   typedef       unsigned long long MIL_UINT64;
   typedef const unsigned long long MIL_CUINT64;
   typedef       unsigned short     MIL_WCHAR;


#else
   #error "Compilation model not supported"
#endif

/************************************************************************/
/* FILES RELATED DEFINITIONS                                            */
/************************************************************************/

#include <stdio.h>   /* required for definition of file */

#if M_MIL_USE_WINDOWS || !M_MIL_USE_TIFF_OPTIMISATION
   typedef FILE* MIL_FILE;
#elif M_MIL_USE_PPC
   class CFileMemExchange;

   struct FilePtr
      {
      CFileMemExchange* FileStream;
      FILE* DirectFilePtr;
      };

   typedef FilePtr* MIL_FILE;
#endif
typedef unsigned int        MIL_SIZE_T;

#if M_MIL_USE_CE
   #include "mil.h"
   #define     MOs_hypot   _hypot

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif
   MFTYPE32 MIL_TEXT_PTR  MFTYPE MOs_getenv(LPCMILSTR VarName);
   MFTYPE32 int           MFTYPE MOs_putenv(LPCMILSTR VarName);
/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

   #define     MOs_fopen    _wfopen
   #define     MOs_fclose   fclose
   #define     MOs_fwrite   fwrite
   #define     MOs_fread    fread
   #define     MOs_fseek    fseek
   #define     MOs_ftell    ftell
   #define     MOs_feof     feof
   #define     MOs_ferror   ferror
   #define     MOs_fflush   fflush
   #define     MOs_fgetc    fgetc
   #define     MOs_fgets    fgetws
   #define     MOs_clearerr clearerr
   #define     MOs_remove(n)  DeleteFile(n) ? 0 : 1
   #define     MOs_rename(n,m)   MoveFile(n,m) ? 0 : -1
#elif (M_MIL_USE_WINDOWS) || (!M_MIL_USE_TIFF_OPTIMISATION)
   #define     MOs_fopen    fopen
   #define     MOs_fclose   fclose
   #define     MOs_fwrite   fwrite
   #define     MOs_fread    fread
   #define     MOs_fseek    fseek
   #define     MOs_ftell    ftell
   #define     MOs_feof     feof
   #define     MOs_ferror   ferror
   #define     MOs_fflush   fflush
   #define     MOs_fgetc    fgetc
   #define     MOs_fgets    fgets
   #define     MOs_clearerr clearerr
   #define     MOs_remove   remove
   #define     MOs_rename   rename
   #define     MOs_hypot   hypot
   #define     MOs_getenv  getenv
   #define     MOs_putenv  putenv
   #define     MOs_fputs    fputs
#elif M_MIL_USE_PPC
   // Prototypes
   MIL_FILE    MStream_fopen(LPCMILSTR FileName, LPCMILSTR Mode);
   int         MStream_fclose(MIL_FILE File);
   MIL_SIZE_T  MStream_fwrite( const void *Buffer, MIL_SIZE_T Size, MIL_SIZE_T Count, MIL_FILE File);
   MIL_SIZE_T  MStream_fread( void *Buffer, MIL_SIZE_T Size, MIL_SIZE_T Count, MIL_FILE File);
   int         MStream_fseek(MIL_FILE File, long Offset, int Origin);
   long        MStream_ftell(MIL_FILE File);
   int         MStream_feof(MIL_FILE File);
   int         MStream_ferror(MIL_FILE File);
   int         MStream_fflush(MIL_FILE File);
   int         MStream_fgetc(MIL_FILE File);
   char*       MStream_fgets(char* String, int N, MIL_FILE File);
   void        MStream_clearerr(MIL_FILE File);
   int         MStream_fputs(const char* String, MIL_FILE File);

   #define     MOs_fopen    MStream_fopen
   #define     MOs_fclose   MStream_fclose
   #define     MOs_fwrite   MStream_fwrite
   #define     MOs_fread    MStream_fread
   #define     MOs_fseek    MStream_fseek
   #define     MOs_ftell    MStream_ftell
   #define     MOs_feof     MStream_feof
   #define     MOs_ferror   MStream_ferror
   #define     MOs_fflush   MStream_fflush
   #define     MOs_fgetc    MStream_fgetc
   #define     MOs_fgets    MStream_fgets
   #define     MOs_clearerr MStream_clearerr
   #define     MOs_remove   PPCFileFuncV
   #define     MOs_rename   PPCFileFuncV
   #define     MOs_fputs    MStream_fputs

   #define     MOS_GENERIC_READ   (0x80000000L)
   #define     MOS_GENERIC_WRITE  (0x40000000L)

   #define     MOS_CREATE_NEW                 1
   #define     MOS_CREATE_ALWAYS              2
   #define     MOS_OPEN_EXISTING              3
   #define     MOS_OPEN_ALWAYS                4
   #define     MOS_TRUNCATE_EXISTING          5
#elif M_MIL_USE_LINUX
   #define     MOs_fopen    fopen
   #define     MOs_fclose   fclose
   #define     MOs_fwrite   fwrite
   #define     MOs_fread    fread
   #define     MOs_fseek    fseek
   #define     MOs_ftell    ftell
   #define     MOs_feof     feof
   #define     MOs_ferror   ferror
   #define     MOs_fflush   fflush
   #define     MOs_fgetc    fgetc
   #define     MOs_fgets    fgets
   #define     MOs_clearerr clearerr
   #define     MOs_remove   remove
   #define     MOs_rename   rename
   #define     MOs_hypot    hypot
   #define     MOs_getenv   getenv
   #define     MOs_putenv   putenv
   #define     MOs_fputs    fputs
#else
   #error Operating system not supported in Milos.h!
#endif

// Host System Buffers RAM limit of pointer
/*----------------------------------------------------------------------*/
/* Max far access! Will return the number of bytes that can be accessed */
/* of the passed pointer. Then, we can safely increment this pointer in */
/* far mode until this limit. After, we need to increment in huge!      */
/*----------------------------------------------------------------------*/
/* This is for the 32 bits RAM limit of pointer */
#if (M_MIL_USE_WINDOWS || M_MIL_USE_PPC || M_MIL_USE_LINUX)
 #define M_RAM_HOST_LIMIT                0x40000000L  // 1073741824 (1G)

 /* This version of macro will take the 32 bits offset and substract it  */
 /* from the maximum in 32 bits: 1G!                                     */
 /*   Note 1G is for  a) bit 31 reserved for sign in long result         */
 /*                   b) bit 30 reserved for substract result always > 0 */
 #define M_MaxFarAccess(p) ( M_RAM_HOST_LIMIT - ((long)(p)&0x3FFFFFFFL) )
#else
   #error Operating system not supported in Milos.h!
#endif

// Host System Buffers defines and tile limits
#define M_COPY_DIRECT_FLAG_HOST        !NULL
#define M_RAM_TILE_SIZE_HOST(x)        ((long)(M_RAM_HOST_LIMIT-16L)/(sizeof(x)))
#define M_RAM_TILE_SIZE_X_HOST(x)      M_RAM_TILE_SIZE_HOST(x)
#define M_RAM_TILE_SIZE_Y_HOST(x)      M_RAM_TILE_SIZE_HOST(x)
#define M_HOST_RAM_TILE_SIZE_HOST(x)   M_RAM_TILE_SIZE_HOST(x)
#define M_HOST_RAM_TILE_SIZE_X_HOST(x) M_RAM_TILE_SIZE_HOST(x)
#define M_HOST_RAM_TILE_SIZE_Y_HOST(x) M_RAM_TILE_SIZE_HOST(x)
#define M_HOST_RAM_TILE_REST_HOST      M_RESTRICT_NONE


/************************************************************************/
/* C++ EXCEPTIONS SUPPORT                                               */
/************************************************************************/

#if (M_MIL_USE_WINDOWS) && !(M_MIL_USE_CE) && !(M_MIL_USE_PPC)
   #define MOs_try                        try
   #define MOs_catch(x)                   catch(x)
   //TBA MOs_throw
#else
   #define MOs_try                        if (true)            // Execute try block
   #define MOs_catch(x)                   else if (false)      // Do not execute catch block
   //TBA MOs_throw
#endif


#endif /* #ifndef __MILOS_H */
