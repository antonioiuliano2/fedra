/******************************************************************************/
/*
*
*    Filename:  ODYSSEY.H
*    Owner   :  Matrox Imaging dept.
*    Rev     :  $Revision: 1.1 $
*    Content :  This file contains the new defines that are needed by the user
*               to use the MIL current library with the GENESIS.
*
*    COPYRIGHT (c) Matrox Electronic Systems Ltd.
*    All Rights Reserved
*/
/******************************************************************************/

#ifndef __MIL_H
#include <mil.h>
#endif

#ifndef __ODYSSEY_H
#define __ODYSSEY_H

//==============================================================================
// PCI device information
//==============================================================================
#define M_PCI_VENDOR_ID             0x00 // (16 lsb)
#define M_PCI_DEVICE_ID             0x00 // (16 msb)
#define M_PCI_COMMAND               0x01 // (16 lsb)
#define M_PCI_STATUS                0x01 // (16 msb)
#define M_PCI_REVISION_ID           0x02 // (byte 0)
#define M_PCI_CLASS_CODE            0x02 // (byte 1,2,3)
#define M_PCI_LATENCY_TIMER         0x03 // (byte 1)
#define M_PCI_BASE_ADRS0            0x04 // 
#define M_PCI_BASE_ADRS1            0x05 // 
#define M_PCI_INT_LINE              0x0F // (byte 0)
#define M_PCI_INT_PIN               0x0F // (byte 1)

#define M_RGB888_ATIMACH64     0xE5E5E500L /* Internal use only */
#define M_RGB888_NORMALVGA     0x39393900L /* Internal use only */

#if M_MIL_USE_WINDOWS
   #ifndef _IM_HOST_OS_NT
      #define _IM_HOST_OS_NT
      #define _IM_HOST_CODE
   #endif
   #ifndef _IM_HOST_CODE
      #define _IM_HOST_CODE
   #endif
   #ifdef _MSC_VER
      #ifndef _IM_COMP_MICROSOFT
         #define _IM_COMP_MICROSOFT
      #endif
   #endif
   #ifdef __TURBOC__
      #ifndef _IM_COMP_BC
         #define _IM_COMP_BC
      #endif
   #endif
#endif

#endif
