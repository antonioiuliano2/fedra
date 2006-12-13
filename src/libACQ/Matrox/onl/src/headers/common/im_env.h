/*******************************************************************
 *
 * COPYRIGHT (c) 2000 Matrox Electronic Systems Ltd.
 * All Rights Reserved
 *
 *******************************************************************/

#ifndef __IM_ENV_H__
#define __IM_ENV_H__


#if !defined(_IM_LOCAL_CODE) && defined(SHOW_INCLUDE_FILE)
#pragma message ("#include "__FILE__)
#endif

// Need auto-config ?
#if (defined(__IMAPI_H__) || defined(__IMAGE2_H__)) && !defined(_IM_HOST_OS_NT) && !defined(_IM_LOCAL_CODE)
// Auto-config compiler settings because no known compile mode setting defines

#define PROD_NAME             "ODYSSEY"


#if defined(__GNUC__) && defined(__i386__)

#ifndef _IM_COMP_GNUC
#define _IM_COMP_GNUC
#endif

#ifndef _IM_HOST_X86_CODE
#define _IM_HOST_X86_CODE
#endif

#if !defined(I_UNIX)
#define I_UNIX
#endif

#define _IM_HOST_CODE

#if defined(__LINUX__)

#ifndef _IM_HOST_OS_LINUX
#define _IM_HOST_OS_LINUX
#endif

#elif defined(__NTO__)

#ifndef _IM_HOST_OS_NTO
#define _IM_HOST_OS_NTO
#endif

#endif

#elif defined(_MSC_VER)

#define _IM_COMP_MICROSOFT
#define _IM_HOST_X86_CODE
#define _IM_HOST_OS_NT
#define _IM_HOST_CODE
#if !defined(_X86_)
#define _X86_
#endif
#if !defined(NT)
#define NT
#endif
#if !defined(WIN)
#define WIN
#endif
#if !defined(WIN32)
#define WIN32
#endif

#elif defined(__MWERKS__)

#define _IM_COMP_PPC
#define _IM_LOCAL_CODE

#endif   // End compiler choice

#endif   // End auto-config


#if !defined(__IMAPI_H__) && !defined(__IMAGE2_H__)
/* In some case im_env.h is included alone.
 * Neither by imapi.h and image2.h.
 * Be sure to define the proper packing in that case.
 * push & pop are not required for this special case.
 */

#if defined(_IM_COMP_WATCOM) || defined(_IM_COMP_GNUC) || defined(_IM_COMP_MICROSOFT)
#pragma pack( 1 )
#endif

#endif

#ifndef INLINE_FUNCTION
#if defined(_IM_COMP_MICROSOFT)
#define INLINE_FUNCTION                __inline

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

#if defined(_IM_COMP_MIL_PA)
#undef double
#define double __int64
#endif

#if defined(_IM_COMP_MIL_PA_NO_INLINE)
#undef INLINE_FUNCTION
#define INLINE_FUNCTION
#endif

#elif defined(_IM_COMP_WATCOM)
#define INLINE_FUNCTION                inline

#elif defined(_IM_LOCAL_CODE) && defined(__HIGHC__)
#define INLINE_FUNCTION                _Inline
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;

#elif defined(_IM_LOCAL_CODE) && defined(__MWERKS__)
#define INLINE_FUNCTION                inline
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;

#elif defined(_IM_COMP_GNUC)

#if defined(__NTO__)
#define INLINE_FUNCTION                inline
#elif defined(_IM_HOST_OS_LINUX_DRIVER)
#define INLINE_FUNCTION                static inline
#else
#define INLINE_FUNCTION                __inline
#endif

//typedef long long LONGLONG;
//typedef unsigned long long ULONGLONG;

#else
#define INLINE_FUNCTION                inline
#endif
#endif


#ifndef ASM_FUNCTION
#if defined(__MWERKS__)
#define ASM_FUNCTION                   asm
#define FORCE_DATA_IN_TEXT_SECTION     __declspec(section ".text") extern
#define INTERRUPT_FUNCTION

#elif defined(__HIGHC__)
#define ASM_FUNCTION
/* #define FORCE_DATA_IN_TEXT_SECTION     __declspec(section ".text") extern */
#define INTERRUPT_FUNCTION             _Interrupt
#define FORCE_DATA_IN_TEXT_SECTION     extern
#ifdef __ALTIVEC
#undef __ALTIVEC
#endif
#include <ppc/asm.h>

#else
#define ASM_FUNCTION
#define FORCE_DATA_IN_TEXT_SECTION     extern
#define INTERRUPT_FUNCTION

#endif
#endif

/* NT OS */
#if defined(_IM_HOST_OS_NT)
#include <im_nt.h>

/* PPC OS */
#elif defined(_IM_LOCAL_CODE)
#include <im_ppc.h>

/* LINUX OS */
#elif defined(_IM_HOST_OS_LINUX)
#include <im_linux.h>

/* NEUTRINO OS */
#elif defined(_IM_HOST_OS_NTO)
#include <im_nto.h>

/* Error Condition */
#else
#error IM_WARNING: Operating system and/or processor type not specified (Refer to image2.h header for compiler switch settings!)
#endif


#if   defined(I_UNIX)
#define DIRECTORY_SEPARATOR      "/"
#define PATHSEPARATOR            ":"
#else
#define DIRECTORY_SEPARATOR      "\\"
#define PATHSEPARATOR            ";"
#endif

#endif /* __IM_ENV_H__ */
