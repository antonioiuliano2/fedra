/*******************************************************************
 *
 * COPYRIGHT (c) 2000 Matrox Electronic Systems Ltd.
 * All Rights Reserved
 *
 *******************************************************************/

#ifndef __IMAPI_H__
#define __IMAPI_H__

#if !defined(_IM_LOCAL_CODE) && defined(SHOW_INCLUDE_FILE)
#pragma message ("#include "__FILE__)
#endif

#include <im_env.h>     /* Odyssey Operating System and Processor environment configuration header */

#if defined(_IM_HOST_OS_NT) && defined(_IM_COMP_MICROSOFT)
#pragma pack( push, enter_imapi_h)
#pragma pack( 1 )
#endif

#include <imapitef.h>   /* Odyssey API Type Definitions   */
#include <imapidef.h>   /* Odyssey API Defines            */
#include <imapipro.h>   /* Odyssey API Prototypes         */


#if defined(_IM_HOST_OS_NT) && defined(_IM_COMP_MICROSOFT)
#pragma pack( pop, enter_imapi_h)
#elif defined(_IM_HOST_OS_LINUX) || defined(_IM_HOST_OS_NTO)
#pragma pack()
#endif

#endif /* __IMAPI_H__ */
