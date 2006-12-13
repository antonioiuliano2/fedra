/************************************************************************/
/*
*
* Filename     :  MIL.H
* Owner        :  Matrox Imaging dept.
* Rev          :  $Revision: 1.1 $
* Content      :  This file contains the defines necessary to use the
*                 Matrox Imaging Library "C" user interface.
*
* Comments     :  Some defines may be here but not yet
*                 implemented in the library.
*
* COPYRIGHT (c) Matrox Electronic Systems Ltd.
* All Rights Reserved
*************************************************************************/

#ifndef __MIL_H
#define __MIL_H

//TBR MIL 8.0
#ifndef M_MIL_75_COMPATIBLE
#define M_MIL_READY_FOR_MIL80 1
#define M_MIL_USE_64_BIT_ATTRIBUTES 1
#else
#define M_MIL_READY_FOR_MIL80 0
#define M_MIL_USE_64_BIT_ATTRIBUTES 0
#endif


/************************************************************************/
/* DEFAULT SETUP FLAG */
/************************************************************************/

/* include set-up instructions (if not specified) */
#include <milsetup.h>

/* include mil memory allocator when needed */
#ifdef M_MIL_USE_MEMORY_ALLOCATOR
#include <milmem.h>
#endif

/************************************************************************/
/* Support for old defines name                                         */
/************************************************************************/
#ifndef OldDefinesSupport
   #define OldDefinesSupport     1
#endif


/************************************************************************/
/* MIL HOST CURRENT VERSION  (Inquired by MappInquire)                  */
/************************************************************************/
#define M_MIL_CURRENT_VERSION          7.80
#define M_MIL_CURRENT_INT_VERSION    0x0780
#define M_MIL_CURRENT_ASCII_VERSION  "7.80"


/************************************************************************/
/* MIL buffer identifier type                                           */
/************************************************************************/
typedef long MIL_ID;

/************************************************************************/
/* MIL main                                                             */
/************************************************************************/
#if M_MIL_USE_CE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define main(x) *UnUsedPtr; int WINAPI WinMain(HINSTANCE hInstance,\
                     HINSTANCE hPrevInstance,\
                     LPTSTR     lpCmdLine,\
                     int       nCmdShow)
#endif


/************************************************************************/
/* General default parameters (may be bit encoded)                      */
/************************************************************************/
#define M_NULL                                           0x00000000L

#if M_MIL_USE_LINUX
   #define MAKE_INT64(I)        (I ## LL)
#else
   #define MAKE_INT64(I)        (I)
#endif

#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
   #define M_PSEUDO_ID                                   0x80000000L    //TBR MIL 8.0
   #define M_IS_PSEUDO_ID(X)                             (X&M_PSEUDO_ID)
   #define M_OVERSCAN_DISABLE                            0x40000000L
#else
   #define M_PSEUDO_ID                                   0x00000000L    //TBR MIL 8.0
   #define M_IS_PSEUDO_ID(X)                             (X >= M_ID_OFFSET_OF_DEFAULT_KERNEL)
   #define M_OVERSCAN_DISABLE                            (M_ID_OFFSET_OF_DEFAULT_KERNEL >> 1L)
#endif

#define M_FALSE                                       0L
#define M_NO                                          0L
#define M_OFF                                         0L
#define M_IN_PROGRESS                                 0L
#define M_FINISHED                                    1L
#define M_TRUE                                        1L
#define M_YES                                         1L
#define M_ON                                          1L
#define M_WAIT                                        1L
#define M_CREATE                                      M_YES
#define M_FREE                                        0x00020000
#define M_DEFAULT                                     0x10000000L
#define M_QUIET                                       0x08000000L
#define M_INSFLAG                                     0x04000000L //TBR MIL 8.0
#define M_SPECIAL                                     0x02000000L
#define M_VALID                                       0x00000001L
#define M_INVALID                                     -1L
#define M_CLEAR                                       0x00000001L
#define M_NO_CLEAR                                    0x00000002L
#define M_LUT_OFFSET                                  0x80000000L
#define M_ENABLE                                      -9997L
#define M_DISABLE                                     -9999L
#define M_LOCAL                                       0x01000000
#define M_REMOTE                                      0x00000000
#define M_UNINITIALIZED                               0x0badbad0

#define M_EXTENDED                                    0x80000000L            // remove
#define M_PERSISTENT                                  0x00000100L

#define M_ALL                                         0x40000000L
#define M_ANY                                         0x11000000L
#define M_INVALID_MDID                                0x40000000L
#define M_ALWAYS                                               0L  // defined in Milblob.h

/************************************************************************/
/* Board generic information.                                           */
/************************************************************************/
#define M_RS170                                       0x00000001L
#define M_NTSC                                        0x00000002L
#define M_CCIR                                        0x00000003L
#define M_PAL                                         0x00000004L
#define M_NTSC_RGB                                    0x00000005L
#define M_PAL_RGB                                     0x00000006L
#define M_SECAM_RGB                                   0x00000007L
#define M_SECAM                                       0x00000008L
/* Reserve next 2 values                       from   0x00000009L*/
/*                                             to     0x0000000AL*/


/************************************************************************/
/* Buffer ID offset for defaults                                        */
/************************************************************************/
#define M_UNSIGNED                                    0x00000000L
#define M_SIGNED                                      0x80000000L
#define M_FLOAT                                      (0x40000000L | M_SIGNED)
#define M_DOUBLE                                     (0x20000000L | M_SIGNED)
#define M_SIZE_BIT_MASK                               0x000000FFL
#define M_TYPE_MASK                                   0xFFFFFF00L

/************************************************************************/
/* MMX related                                                          */
/************************************************************************/
#define M_READ_PREFETCH_EXTRA_BYTES                   32

// Kept for backward compatibility.
#define M_MMX_EXTRA_BYTES                             M_READ_PREFETCH_EXTRA_BYTES

// Default values for memory allocators. Can be overridden at run-
// time by any OS-specific version of the MIL memory allocator
#define M_MEMORY_ALLOC_PRE_PADDING_VALUE             M_READ_PREFETCH_EXTRA_BYTES
#define M_MEMORY_ALLOC_POST_PADDING_VALUE            M_READ_PREFETCH_EXTRA_BYTES
#define M_MEMORY_ALLOC_PITCH_BYTE_VALUE              1
#define M_MEMORY_ALLOC_ALIGNMENT_VALUE               128

/************************************************************************/
/* Non cachable related                                                 */
/************************************************************************/
#define M_NO_ALIGNMENT                               -1
#define M_4K_ALIGNMENT                                4096


/************************************************************************/
/* Multi thread                                                         */
/************************************************************************/
#define M_MULTI_THREAD                                0x00000001L
#define M_HOST_THREAD                                 0x00000002L
#define M_MIL_THREAD                                  0x00000004L
#define M_STATE                                       0x00000008L
#define M_SIGNALED                                    0x00000010L
#define M_NOT_SIGNALED                                0x00000020L
#define M_THREAD_SELECT                               0x00000040L
#define M_THREAD_DETACH                               0x00000080L
#define M_AUTO_RESET                                  0x00002000L
#define M_MANUAL_RESET                                0x00004000L

#define M_IDLE                                                 1L
#define M_LOWEST                                               6L
#define M_BELOW_NORMAL                                         7L
#ifndef M_MIL_75_COMPATIBLE
#define M_NORMAL                                               8L
#else
#define M_NORMAL                                               1L
#endif //M_MIL_75_COMPATIBLE
#define M_ABOVE_NORMAL                                         9L
#define M_HIGHEST                                             10L
#define M_TIME_CRITICAL                                       15L

#define M_THREAD_PRIORITY                             1019L
#define M_EVENT_ALLOC                                 1700L
#define M_EVENT_FREE                                  1701L
#define M_EVENT_SEND                                  1702L
#define M_EVENT_CONTROL                               1703L
#define M_EVENT_SYNCHRONIZE                           1704L
#ifdef  M_MIL_75_COMPATIBLE                                                 //TBR MIL 8.0
#define M_EVENT_WAIT                                  0x00000100L       //TBR MIL 8.0
#define M_EVENT_STATE                                 0x00000200L       //TBR MIL 8.0
#define M_EVENT_SET                                   0x00000400L       //TBR MIL 8.0
#else
#define M_EVENT_WAIT                                  1705L
#define M_EVENT_STATE                                 1706L
#define M_EVENT_SET                                   1707L
#endif
#define M_EVENT_TIMEOUT                               1708L
#define M_THREAD_ALLOC                                1800L
#define M_THREAD_FREE                                 1801L
#define M_THREAD_WAIT                                 1802L
#define M_THREAD_CONTROL                              1803L
#define M_THREAD_MODE                                 1804L
#define M_THREAD_IO_MODE                              1805L
#define M_THREAD_CREATE                               1806L //TBR MIL 8.0
#define M_THREAD_SELECTABLE_ALLOC                     1807L
#define M_THREAD_END_WAIT                             1808L
#define M_THREAD_START                                1809L


/************************************************************************/
/* General Inquire/Control ...                                          */
/************************************************************************/
#define M_OWNER_APPLICATION                           1000L
#define M_OWNER_SYSTEM                                1001L
#define M_SIZE_X                                      1002L
#define M_SIZE_Y                                      1003L
#define M_SIZE_Z                                      1004L
#define M_SIZE_BAND                                   1005L
#define M_SIZE_BAND_LUT                               1006L
#define M_SIZE_BIT                                    1007L
#define M_TYPE                                        1008L
#define M_NUMBER                                      1009L
#define M_FORMAT                                      1010L
#define M_FORMAT_SIZE                                 1011L
#define M_INIT_FLAG                                   1012L
#define M_ATTRIBUTE                                   1013L
#define M_SIGN                                        1014L
#define M_LUT_ID                                      1015L
#define M_NATIVE_ID                                   1016L
#define M_NATIVE_CONTROL_ID                           1017L
#define M_COLOR_MODE                                  1018L
#define M_THREAD_PRIORITY                             1019L
#define M_NEED_UPDATE                                 1020L
#define M_SURFACE                                     1021L
//1022L was use by M_WINDOW_DDRAW_SURFACE, that is now in oldDefine section.
#define M_OWNER_SYSTEM_TYPE                           1023L
#define M_DISP_NATIVE_ID                              1024L
#define M_SATURATION_SIZE_BIT                         1035L
#define M_SATURATION_TYPE                             1036L
#define M_LOCATION                                    1037L
#define M_OBJECT_VALID                                1038L

/************************************************************************/
/* MsysAlloc()                                                          */
/************************************************************************/
/* System type */
#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
#define M_DEFAULT_HOST                                0x00FF0001L
#else
#define M_DEFAULT_HOST                                0x10000001L       //TBR MIL 8.0
#endif

#ifdef  M_MIL_75_COMPATIBLE                                                 //TBR MIL 8.0
#define M_INTERNAL_DEFAULT_HOST                       M_DEFAULT_HOST    //TBR MIL 8.0
#else                                                                   //TBR MIL 8.0
#define M_INTERNAL_DEFAULT_HOST                       0x00FF0003L
#endif                                                                  //TBR MIL 8.0

#define M_SYSTEM_HOST_TYPE                             9L
#define M_SYSTEM_VGA_TYPE                             13L
#define M_SYSTEM_METEOR_TYPE                          15L
#define M_SYSTEM_PULSAR_TYPE                          16L
#define M_SYSTEM_GENESIS_TYPE                         17L
#define M_SYSTEM_CORONA_TYPE                          18L
#define M_SYSTEM_METEOR_II_TYPE                       20L
#define M_SYSTEM_METEOR_II_DIG_TYPE                   21L
#define M_SYSTEM_1394_TYPE                            22L
#define M_SYSTEM_METEOR_II_1394_TYPE                  M_SYSTEM_1394_TYPE
#define M_SYSTEM_MMX_TYPE                             23L
#define M_SYSTEM_SSE_TYPE                             24L
#define M_SYSTEM_ORION_TYPE                           25L
#define M_SYSTEM_SSE2_TYPE                            26L
#define M_SYSTEM_AMD_3DNOW_TYPE                       27L
#define M_SYSTEM_AMD_MMX_EXT_TYPE                     28L
#define M_SYSTEM_AMD_3DNOW_EXT_TYPE                   29L
#define M_SYSTEM_CORONA_II_TYPE                       30L
#define M_SYSTEM_REMOTE_HOST_TYPE                     31L
#define M_SYSTEM_METEOR_II_CL_TYPE                    32L
#define M_SYSTEM_ALTIVEC_TYPE                         33L
#define M_SYSTEM_CRONOS_TYPE                          34L
#define M_SYSTEM_ODYSSEY_TYPE                         35L
#define M_SYSTEM_HELIOS_TYPE                          36L
#define M_SYSTEM_IRIS_TYPE                            37L
#define M_SYSTEM_CRONOSPLUS_TYPE                      38L
#define M_SYSTEM_MORPHIS_TYPE                         39L
#define M_SYSTEM_ODYSSEY_NATIVE_TYPE                  40L


/* MsysAlloc() flags  */
#define M_COMPLETE                                    0x00000000L
#define M_PARTIAL                                     0x00000001L
#define M_WINDOWS                                     0x00000002L
#define M_DISP_WAIT_SELECT                            0x00000004L
#define M_DISP_TEXT_SAVE                              0x00000008L
#define M_USE_DMA_FOR_PROC_BUF                        0x00000010L
#define M_USE_DMA_FOR_DISP_BUF                        0x00000020L
#define M_USE_DMA_FOR_GRAB_BUF                        0x00000040L
#define M_PRE_ALLOC_DMA_MEM                           0x00000080L
//#define M_DMA_MEM_MASK                                0x0000FF00L
#define M_NO_INTERRUPT                                0x00010000L
#define M_NO_FIELD_START_INTERRUPT                    0x00020000L
#define M_DISP_NO_WAIT_SELECT                         0x00040000L
#define M_NO_DDRAW                                    0x00080000L
#define M_EXTERNAL_CLK_TTL                            0x00100000L
#define M_SYSTEM_METEOR_II_CL_FLAG                    0x00400000L
#define M_ACCELERATOR_DISABLE                         0x00400000L
#define M_EXTERNAL_CLK_422                            0x00200000L
#define M_DDRAW                                       0x40000000L
#define M_SYSTEM_METEOR_II_FLAG                       0x01000000L
#define M_REGISTER_EDITOR                             0x02000000L
#define M_NEW_SYSALLOC                                0x04000000L
#define M_USE_COMMAND_DECODER                         0x00000100L
#define M_SYS_MD_ID                                   0x08000000L
#define M_DEFAULT                                     0x10000000L
#define M_INTERNAL_ALLOCATION                         ((BUFATTRTYPE)MAKE_INT64(0x0000000080000000))
#define M_STAR_GATE                                   0x40000000L
#define M_SYSTEM_CORONA_II_FLAG                       0x20000000L

#define M_DMA_BLOCK_SIZE                           256L

#define M_DMA_BUF_SIZE(Val)  (((((Val+255L)/256L) << 8) & M_DMA_MEM_MASK) | M_PRE_ALLOC_DMA_MEM)
#define M_USE_DMA            (M_USE_DMA_FOR_PROC_BUF | M_USE_DMA_FOR_DISP_BUF | M_USE_DMA_FOR_GRAB_BUF)


/************************************************************************/
/* SysAlloc() in Mil Interpreter                                        */
/************************************************************************/

//TBR MIL 8.0 This section is only used by the interpreter and could be removed in MIL 8.0
#define M_SYSTEM_HOST_PTR          (M_SYSTEM_HOST_TYPE            +  50L)
#define M_SYSTEM_VGA_PTR           (M_SYSTEM_VGA_TYPE             +  50L)
#define M_SYSTEM_METEOR_PTR        (M_SYSTEM_METEOR_TYPE          +  50L)
#define M_SYSTEM_PULSAR_PTR        (M_SYSTEM_PULSAR_TYPE          +  50L)
#define M_SYSTEM_GENESIS_PTR       (M_SYSTEM_GENESIS_TYPE         +  50L)
#define M_SYSTEM_CORONA_PTR        (M_SYSTEM_CORONA_TYPE          +  50L)
#define M_SYSTEM_ORION_PTR         (M_SYSTEM_ORION_TYPE           +  50L)
#define M_SYSTEM_METEOR_II_PTR     (M_SYSTEM_METEOR_II_TYPE       +  50L)
#define M_SYSTEM_METEOR_II_DIG_PTR (M_SYSTEM_METEOR_II_DIG_TYPE   +  50L)
#define M_SYSTEM_METEOR_II_CL_PTR  (M_SYSTEM_METEOR_II_CL_TYPE    +  50L)
#define M_SYSTEM_CRONOS_PTR        (M_SYSTEM_CRONOS_TYPE          +  50L)
#define M_SYSTEM_HELIOS_PTR        (M_SYSTEM_HELIOS_TYPE          +  50L)
#define M_SYSTEM_IRIS_PTR          (M_SYSTEM_IRIS_TYPE            +  50L)
#define M_SYSTEM_CRONOSPLUS_PTR    (M_SYSTEM_CRONOSPLUS_TYPE      +  50L)

/************************************************************************/
/* MsysInquire() / MsysControl() Types                                  */
/************************************************************************/
#define M_OWNER_APPLICATION                           1000L
#define M_OWNER_SYSTEM                                1001L
#define M_EXECUTE_BUFFER_HOOKS                        1002L
#define M_TYPE                                        1008L
#define M_NUMBER                                      1009L
#define M_FORMAT                                      1010L
#define M_FORMAT_SIZE                                 1011L
#define M_INIT_FLAG                                   1012L
#define M_ATTRIBUTE                                   1013L
#define M_NATIVE_ID                                   1016L
#define M_NATIVE_CONTROL_ID                           1017L
#define M_LOGICAL_TO_PHYSICAL_ADDRESS                 1018L
#define M_MAP_VIDEO_MEMORY                            1019L

#define M_MIL_ID_INTERNAL                             1025L

#define M_THREAD_MODE                                 1804L

#define M_SYSTEM_TYPE                                 2000L
#define M_SYSTEM_TYPE_PTR                             2001L
//2002L was use by M_DISPLAY_NUM, that is now in the Display reserved area.
//2003L was use by M_DISPLAY_TYPE, that is now in the Display reserved area.
#define M_DIGITIZER_NUM                               2004L
#define M_DIGITIZER_TYPE                              2005L
#define M_PROCESSOR_NUM                               2006L
//2007L was use by M_PROCESSOR_TYPE, that is now in the processor specific section.
#define M_PROCESSING_SYSTEM                           2008L
#define M_PROCESSING_SYSTEM_TYPE                      2009L
#define M_TUNER_NUM                                   2010L
#define M_TUNER_TYPE                                  2011L
#define M_RGB_MODULE_NUM                              2012L
#define M_RGB_MODULE_TYPE                             2013L
#define M_BOARD_TYPE                                  2014L
#define M_BOARD_REVISION                              2015L
#define M_DISPLAY_LIST                                2016L
#define M_WIN_MODE                                    2017L
//2018L was use by M_DUAL_SCREEN_MODE, that is now in the Display reserved area.
//2019L was use by M_UNDERLAY_SURFACE_AVAILABLE, that is now in the Display reserved area.
#define M_MAX_TILE_SIZE                               2021L
#define M_MAX_TILE_SIZE_X                             2022L
#define M_MAX_TILE_SIZE_Y                             2023L
#define M_LOW_LEVEL_SYSTEM_ID                         2024L
#define M_NATIVE_THREAD_ID                            2026L
#define M_NATIVE_MODE_ENTER                           2027L
#define M_NATIVE_MODE_LEAVE                           2028L
#define M_PHYSICAL_ADDRESS_UNDERLAY                   2029L
//2030L was use by M_PHYSICAL_ADDRESS_VGA, that is now in the Display reserved area.
#define M_PSEUDO_LIVE_GRAB_ON_MGA                     2031L
#define M_PSEUDO_LIVE_GRAB                            2032L
#define M_FORCE_PSEUDO_IN_NON_UNDERLAY_DISPLAYS       2033L
#define M_LIVE_GRAB                                   2034L
#define M_LIVE_GRAB_WHEN_DISPLAY_DOES_NOT_MATCH       2035L
#define M_LIVE_GRAB_TRACK                             2036L
#define M_LIVE_GRAB_MOVE_UPDATE                       2037L
#define M_LIVE_GRAB_END_TRIGGER                       2038L
#define M_STOP_LIVE_GRAB_WHEN_MENU                    2040L
#define M_STOP_LIVE_GRAB_WHEN_INACTIVE                2041L
#define M_STOP_LIVE_GRAB_WHEN_DISABLED                2042L
#define M_GRAB_BY_DISPLAY_CAPTURE                     2043L
#define M_ALLOC_BUF_RGB888_AS_RGB555                  2044L
#define M_RGB555_BUFFER_ALLOCATION                    2045L
#define M_LAST_GRAB_IN_TRUE_BUFFER                    2046L
#define M_NO_GRAB_WHEN_NO_INPUT_SIGNAL                2047L
#define M_PCI_LATENCY                                 2048L
#define M_FAST_PCI_TO_MEM                             2049L
#define M_DCF_SUPPORTED                               2050L
#define M_DMA_ENABLE                                  2051L
#define M_DMA_DISABLE                                 2052L
#define M_DIB_ONLY                                    2053L
#define M_DIB_OR_DDRAW                                2054L
#define M_FLIP_ONLY                                   2055L
//2057L was use by M_PRIMARY_DDRAW_SURFACE_MEM_PTR, that is now in the Display reserved area.
//2058L was use by M_PRIMARY_DDRAW_SURFACE_PITCH_BYTE, that is now in the Display reserved area.
//2059L was use by M_PRIMARY_DDRAW_SURFACE_SIZE_X, that is now in the Display reserved area.
//2060L was use by M_PRIMARY_DDRAW_SURFACE_SIZE_Y, that is now in the Display reserved area.
//2061L was use by M_PRIMARY_DDRAW_SURFACE_SIZE_BITS, that is now in the Display reserved area.
#define M_INTERNAL_FORMAT_SIZE                        2062L
#define M_INTERNAL_FORMAT_ENUMERATION                 2063L
#define M_INTERNAL_FORMAT_CHECK                       2064L
//2065L was use by M_DDRAW_AVAILABLE, that is now in the Display reserved area.
#define M_BOARD_CODE                                  2066L
#define M_LIVE_GRAB_DDRAW                             2067L
#define M_THREAD_CONTEXT_PTR                          2068L
#define M_PSEUDO_LIVE_GRAB_NB_FRAMES                  2069L
#define M_PSEUDO_LIVE_GRAB_NB_FIELDS                  2070L
#define M_DISPLAY_DOUBLE_BUFFERING                    2071L
#define M_PSEUDO_LIVE_GRAB_TIME                       2072L
#define M_PCI_BRIDGE_LATENCY                          2073L
#define M_PSEUDO_LIVE_GRAB_WITH_DDRAW                 2074L
#define M_MULTI_DISP_IN_UNDERLAY                      2075L
#define M_MULTI_DISP_FOR_GRAB                         2076L
#define M_TIMEOUT                                     2077L
#define M_AUTO_FLIP_FOR_TRUE_COLOR                    2078L
#define M_PCI_BRIDGE_HOST_WRITE_POSTING               2079L
#define M_FAST_MEM_TO_VGA                             2080L
#define M_ERROR_ASYNCHRONOUS_LOG                      2081L
#define M_LIVE_GRAB_WHEN_NOT_VISIBLE                  2082L
//TBM
//This is keep for backward compatibility with MIL 6.1. After MIL 7.0 release, this define will
//have to be rename M_USE_MMX_OLD and the MsysInquire and MsysControl functions will have to be
//modified at all levels to reflect this modification. The flag M_USE_MMX_NEW will be rename to
//M_USE_MMX.
#define M_USE_MMX_OLD                                 2083L
#define M_OVERLAPPED_STRUC                            2085L
#define M_PHYSICAL_ADDRESS_VIA                        2086L
#define M_PCI_MGA_ID                                  2087L
#define M_PCI_VIA_ID                                  2088L
#define M_PCI_BRIDGE_ID                               2089L
#define M_NATIVE_SYSTEM_NUMBER                        2090L
#define M_NATIVE_NODE_NUMBER                          2091L

#define M_CUSTOMER_PRODUCT_ID                         2093L
#define M_UNDERLAY_MEM_SIZE                           2094L
#define M_OVERLAY_MEM_SIZE                            2095L
#define M_VIA_MEM_SIZE                                2096L
#define M_SYSTEM_NAME                                 2097L
#define M_SERIAL_NUMBER                               2098L
#define M_ERROR_CLEAR                                 2099L
#define M_SYS_DEBUG_BUFFER_SIZE                       2100L  // internal use only
#define M_SYS_DEBUG_BUFFER_ID                         2101L  // internal use only
#define M_USE_NOA                                     2102L
#define M_USE_ACCELERATOR                             M_USE_NOA
#define M_COMPRESSION_MODULE_PRESENT                  2104L
#define M_DIGITAL_MODULE_PRESENT                      2105L
#define M_INTERRUPT_NEW_TECHNIQUE                     2106L
#define M_INTERRUPT_NEW_TECHNIQUE_STAT                2107L
#define M_INTERRUPT_NEW_TECHNIQUE_QUEUE               2108L
#define M_INTERRUPT_MASTER_SWITCH                     2109L
#define M_BUS_MASTER_COPY_TO_HOST                     2110L
#define M_BUS_MASTER_COPY_FROM_HOST                   2111L
#define M_MEMORY_OFF_SCREEN_AVAILABLE                 2112L
#define M_BUFCOPY_SUPPORTED                           2113L
#define M_COMPRESSION_SUPPORTED                       2114L

#define M_UART_BUFFER_INIT                            2115L  // internal use only
#define M_UART_BUFFER_FREE                            2116L  // internal use only
#define M_CLL_UART_PARITY                             2117L  // internal use only
#define M_CLL_UART_IRQ_CALL                           2118L  // internal use only
#define M_SET_DUAL_SCREEN_MODE                        2119L  // internal use only
#define M_RESET_GRAB_INFO                             2120L  // internal use only
#define M_COMPRESSION_BOARD_TYPE                      2121L
#define M_COMPRESSION_BOARD_RESET                     2122L
#define M_COMPACT_PCI_PRESENT                         2123L
#define M_TEST_CACHE_COHERENCE                        2124L
#define M_HARDWARE_COMPRESSION                        2130L
#define M_HARDWARE_DECOMPRESSION                      2131L
#define M_UART_PRESENT                                2132L
#define M_PC104_PRESENT                               2133L
//TBM
//This is keep for backward compatibility with MIL 6.1. After MIL 7.0 release, this define will
//have to be rename M_USE_SSE_OLD and the MsysInquire and MsysControl functions will have to be
//modified at all levels to reflect this modification. The flag M_USE_SSE_NEW will be rename to
//M_USE_SSE.
#define M_USE_SSE_OLD                                 2134L
//2135L was use by M_USE_DDRAW_UNDERLAY, that is now in the AppInquire display reserved area.
//2136L was use by M_DDRAW_UNDERLAY_SUPPORTED, that is now in the Display reserved area.
#define M_DDRAW_UNDERLAY_FORMAT                       2137L
#define M_ENCODER_PRESENT                             2138L
#define M_COMPRESSION_INTERNAL_BUFFER_SIZE            2139L
//2140L was use by M_PRIMARY_DDRAW_SURFACE_FORMAT, that is now in the Display reserved area.
#define M_LIVE_GRAB_NO_TEARING                        2141L
//2153L was use by M_DDRAW_UNDERLAY_FORMAT_SUPPORTED, that is now in the Display reserved area.
#define M_BIOS_REVISION                               2154L
#define M_FPGA_REVISION                               2155L
#define M_FIRMWARE_REVISION                           M_FPGA_REVISION
#define M_INTERNAL_CAMERA_STRUCT_PTR                  2156L
#define M_INTERNAL_CAMERA_STRUCT_SIZE                 2157L
#define M_INTERNAL_DRV_DEBUG                          2158L  // Internal use only
#define M_SUPPORTED_INTERFACE                         2159L
//2160L was use by M_USE_SSE2, that is now in the processor specific section.
//2161L was use by M_USE_AMD_3DNOW, that is now in the processor specific section.
//2162L was use by M_USE_AMD_MMX_EXTENSION, that is now in the processor specific section.
//2163L was use by M_USE_AMD_3DNOW_EXTENSION, that is now in the processor specific section.
//2164L was use by M_CPU_VENDOR_NAME, that is now in the processor specific section.
//2165L was use by M_CPU_FAMILY_NUMBER, that is now in the processor specific section.
//2166L was use by M_CPU_MODEL_NUMBER, that is now in the processor specific section.
//2167L was use by M_CPU_STEPPING_NUMBER, that is now in the processor specific section.
//2168L was use by M_CPU_SERIAL_NUMBER, that is now in the processor specific section.
#define M_SYSTEM_NUM                                  2169L
#define M_ASSOCIATED_VGA_SYSTEM_ID                    2170L  // internal use only
#define M_ASSOCIATED_VGA_DEVICE                       2171L
#define M_SYSTEM_DESCRIPTOR                           2172L
#define M_SYSTEM_DESCRIPTOR_SIZE                      2173L
#define M_SERIAL_NUMBER_SIZE                          2174L
#define M_BOARD_SUB_REVISION                          2200L
#define M_USER_BIT_CHANGE                             2244L
#define M_USER_BIT_CHANGE_HANDLER_PTR                 2245L
/* Reserve next 19 values                       from  2246L*/
/*                                             to     2264L*/
#define M_USER_BIT_CHANGE_HANDLER_USER_PTR            2266L
/* Reserve next 19 values                       from  2267L*/
/*                                             to     2285L*/
#define M_1394_FORCE_BUS_RESET                        2286L
#define M_1394_GENERATION_COUNT                       2287L
//2288L was use by M_PHYSICAL_SIZE_VGA, that is now in the Display reserved area.
#define M_CURRENTLY_USED_CAMERA                       2289L
#define M_DIGITIZER_MODE                              2290L
#define M_MSG_BUFFER_SIZE                             2291L
#define M_MSG_BUFFER_PHYSICAL_ADDRESS                 2292L
#define M_MSG_LOG_MODE                                2293L
#define M_DEFAULT_PITCH_BYTE                          2294L
#define M_SYS_USE_WINDOWED_CLASS                      2295L
#define M_SYSTEM_LICENSE_MODULES                      2296L
//2297L was used by M_DUAL_HEAD, which is now a display specific app section (M_APP_DISPLAY_START)
//2298L was used by M_MULTI_MONITOR, which is now a display specific app section (M_APP_DISPLAY_START)
#define M_SUBSYSTEM_ID                                2299L // internal use only
#define M_USE_DDRAW                                   2300L
#define M_PREFERRED_DISPLAY_SYSTEM                    2301L
#define M_USAGE_METER                                 2302L
#define M_GRAB_USAGE                                  2303L
#define M_ACCELERATOR_USAGE                           2304L
#define M_DMA_USAGE                                   2305L
#define M_UTILITY_USAGE                               2306L
#define M_MAX_UNIT_IDLE_TIME                          2307L
/* Reserve for futur usage meter                      2308L */
/* Reserve for futur usage meter                      2309L */
#define M_BUFFER_DEFAULT_LOCATION                     2310L
#define M_ALLOCATION_OVERSCAN_SIZE                    2311L
#define M_FAST_OVERSCAN_BUFFER                        2312L

#define M_NATIVE_CLUSTER_NUMBER                       M_NATIVE_SYSTEM_NUMBER
#define M_CPU_SPEED                                   2320L
#define M_ACCELERATOR_SPEED                           2321L
#define M_ACCELERATOR_PRESENT                         2322L
#define M_MEMORY_SIZE                                 2323L
#define M_MEMORY_FREE                                 2324L
#define M_FAST_MEMORY_FREE                            2325L
#define M_SHARED_MEMORY_FREE                          2326L
#define M_HEAP_SIZE                                   2327L
#define M_HEAP_FREE                                   2328L
#define M_SIZE_X_CREATE_RESTRICTION                   2329L
#define M_SHARED_MEMORY_SIZE                          2330L
#define M_DIGITIZER_NUM_ALLOCATED                     2331L
/* Reserve next 4 values                       from   3232L*/
/*                                             to     3235L*/
/* Next 4 values are used to specify device number of the digitizer.*/
#define M_SERIAL_DEVICE_PORT_NUMBERS                  2336L

#define M_FIRMWARE_UPDATE                             2340

#define M_DDRAW_OVERLAY_SURFACE_USED                  3200L
/* Reserve next 16 values                      from   3201L*/
/*                                             to     3216L*/
/* Next 16 values are used to specify the index of the VGA.*/


#define M_USER_BIT_INQUIRE_START                      4363L
#define M_USER_BIT                                    4087L
/* Reserve next 31 values                      from   4088L*/
/*                                             to     4118L*/
/* Reserve next 4 values                       from   4120L*/
/*                                             to     4123L*/

#define M_USER_BIT_COUNT                              4364L
#define M_USER_BIT_OPTOMODULE                         4365L
#define M_USER_BIT_INQUIRE_END                        4366L
#define M_FORCE_PITCH_BYTE_MULTIPLE                   4367L
#define M_ODYSSEY_BEHAVIOUR                           4368L

//EEPROM-related defines
#define M_EEPROM_SIZE                                 4369L
#define M_EEPROM_L_OFFSET                             4370L
#define M_EEPROM_L_SIZE                               4371L

#define M_BOARD_TYPE_STRING                           4372L
#define M_BOARD_TYPE_STRING_SIZE                      4373L

#define M_SUPPORT_START_NO                            4374L
/*Reserve values between M_SUPPORT_START_NO
                          and M_SUPPORT_END_NO*/
#define M_DISPLAY_SUPPORT                             4374L
#define M_MULTITHREAD_SUPPORT                         4375L
#define M_ASYNCHRONOUS_CALL_SUPPORT                   4376L
#define M_SUPPORT_END_NO                              4439L

#define M_SUPPORT_START_YES                           4440L
/*Reserve values between M_SUPPORT_START_NO and M_SUPPORT_END_NO*/
#define M_SUPPORT_END_YES                             4500L

// Display reserved area
#define M_SYS_DISPLAY_START                           4501L
#define M_IS_IN_DISPLAY_SYS_RANGE(X)     ((X>=M_SYS_DISPLAY_START) && (X<=M_SYS_DISPLAY_END))
#define M_MAIN_DDRAW_OBJECT                           M_SYS_DISPLAY_START+0L     //4501
#define M_DIRECTX_OBJECT                              M_SYS_DISPLAY_START+1L     //4502
#define M_DDRAW_AVAILABLE                             M_SYS_DISPLAY_START+2L     //4503
#define M_DDRAW_UNDERLAY_FORMAT_SUPPORTED             M_SYS_DISPLAY_START+3L     //4504
#define M_DDRAW_UNDERLAY_SUPPORTED                    M_SYS_DISPLAY_START+4L     //4505
#define M_DDRAW_UNDERLAY_ZOOM_MAX                     M_SYS_DISPLAY_START+5L     //4506
#define M_DDRAW_UNDERLAY_ZOOM_MIN                     M_SYS_DISPLAY_START+6L     //4507
#define M_PHYSICAL_ADDRESS_VGA                        M_SYS_DISPLAY_START+7L     //4508
#define M_PHYSICAL_SIZE_VGA                           M_SYS_DISPLAY_START+8L     //4509
#define M_PRIMARY_DDRAW_SURFACE_FORMAT                M_SYS_DISPLAY_START+9L     //4510
#define M_PRIMARY_DDRAW_SURFACE_SIZE_BITS             M_SYS_DISPLAY_START+10L    //4511
#define M_PRIMARY_DDRAW_SURFACE_SIZE_X                M_SYS_DISPLAY_START+11L    //4512
#define M_PRIMARY_DDRAW_SURFACE_SIZE_Y                M_SYS_DISPLAY_START+12L    //4513
#define M_PRIMARY_DDRAW_SURFACE_PITCH_BYTE            M_SYS_DISPLAY_START+13L    //4514
#define M_PRIMARY_DDRAW_SURFACE_MEM_PTR               M_SYS_DISPLAY_START+14L    //4515
#define M_PRIMARY_DDRAW_SURFACE                       M_SYS_DISPLAY_START+15L    //4516
#define M_UNDERLAY_SURFACE_AVAILABLE                  M_SYS_DISPLAY_START+16L    //4517
#define M_DUAL_SCREEN_MODE                            M_SYS_DISPLAY_START+17L    //4518
#define M_DISPLAY_OUTPUT_NUM                          M_SYS_DISPLAY_START+18L    //4519
#define M_WINDOWED_DISPLAY_AVAILABLE                  M_SYS_DISPLAY_START+19L    //4520
#define M_EXTERN_DISPLAY_AVAILABLE                    M_SYS_DISPLAY_START+20L    //4521
#define M_EXTERN_DISPLAY_OBJECT                       M_SYS_DISPLAY_START+21L    //4522
#define M_EXTERN_DISPLAY_API_MODULE                   M_SYS_DISPLAY_START+22L    //4523
#define M_EXTERN_DISPLAY_SUPPORTED_FORMAT             M_SYS_DISPLAY_START+23L    //4524
#define M_VSYNCH_DIRECTX_MAX_WAIT                     M_SYS_DISPLAY_START+24L    //4525
#define M_MEMORY_MANAGER                              M_SYS_DISPLAY_START+25L    //4526
#define M_DDRAW_LIVE_VIDEO_FORMAT_SUPPORTED           M_SYS_DISPLAY_START+26L    //4527
#define M_FIRST_VGA_DIRECTX_OBJECT                    M_SYS_DISPLAY_START+27L    //4528
#define M_UNDERLAY_FORMAT_SUPPORTED                   M_SYS_DISPLAY_START+28L    //4529
#define M_UNDERLAY_SUPPORTED                          M_SYS_DISPLAY_START+29L    //4530
#define M_UNDERLAY_ZOOM_MAX                           M_SYS_DISPLAY_START+30L    //4531
#define M_UNDERLAY_ZOOM_MIN                           M_SYS_DISPLAY_START+31L    //4532
#define M_IS_A_PARHELIA                               M_SYS_DISPLAY_START+32L    //4533
#define M_UNDERLAY_ZOOM_X_MAX                         M_SYS_DISPLAY_START+33L    //4534
#define M_UNDERLAY_ZOOM_X_MIN                         M_SYS_DISPLAY_START+34L    //4535
#define M_UNDERLAY_ZOOM_Y_MAX                         M_SYS_DISPLAY_START+35L    //4536
#define M_UNDERLAY_ZOOM_Y_MIN                         M_SYS_DISPLAY_START+36L    //4537
#define M_UNDERLAY_LIVE_ZOOM_X_MAX                    M_SYS_DISPLAY_START+37L    //4538
#define M_UNDERLAY_LIVE_ZOOM_X_MIN                    M_SYS_DISPLAY_START+38L    //4539
#define M_UNDERLAY_LIVE_ZOOM_Y_MAX                    M_SYS_DISPLAY_START+39L    //4540
#define M_UNDERLAY_LIVE_ZOOM_Y_MIN                    M_SYS_DISPLAY_START+40L    //4541
#define M_MTX0_TRANSFER_SUPPORTED                     M_SYS_DISPLAY_START+41L    //4542
#define M_MTX0_CAPS                                   M_SYS_DISPLAY_START+42L    //4543
#define M_IS_IN_10_BITS_MODE                          M_SYS_DISPLAY_START+43L    //4544
#define M_IS_IN_10_BITS_COLOR_MODE                    M_SYS_DISPLAY_START+44L    //4545
#define M_IS_IN_10_BITS_GRAYSCALE_MODE                M_SYS_DISPLAY_START+45L    //4546
#define M_CAN_OFFSET_IN_UNDERLAY_LIVE                 M_SYS_DISPLAY_START+46L    //4547
#define M_DISPLAY_NUM                                 M_SYS_DISPLAY_START+47L    //4548
////////////// MAXIMUM //////////////////////////////////////////////////+99L    //4600
/*Reserve values between M_SYS_DISPLAY_START and M_SYS_DISPLAY_END*/
#define M_SYS_DISPLAY_END                             4600L
#define M_PERFORMANCE_LOGS                            4601L
#define M_INTERRUPT_LATENCY                           4602L
#define M_DEBUG_BUFFER_TO_FILE                        M_SYS_DEBUG_BUFFER_ID

#define M_APP_INQUIRE_SYS_START                       5000L
/*Reserve values betwen 5000 and 5999 for AppInquires.
Needey for remote systems                                 */
#define M_APP_INQUIRE_SYS_END                         5999L

#define M_CACHE_FLUSH                                 8192L


///////////////////////////////////////////////////////////////////////////////////////////////////
//Processor specific and optimization MsysInquire/MsysControl type.
//The range between 0x00002800 to 0x00002FFF (10240L to 12287).
///////////////////////////////////////////////////////////////////////////////////////////////////
#define M_CPU_FLAG_RANGE                              0x00002800L
#define M_CPU_FLAG_MASK                               0xFFFFF800L

#define M_PROCESSOR_TYPE                              0x00002800L
#define M_INTEL_X86                                   0x00002801L
#define M_INTEL_IA64                                  0x00002802L
#define M_PPC_7400                                    0x00002803L
//Reserved for future processor type                  0x00002804L
//                                                 to 0x0000281FL
#define M_UNKNOWN_TYPE                                0x00002820L

#define M_USE_FULL_OPTIMIZATION                       0x00002821L
#define M_USE_SIMD                                    0x00002821L

//Intel x86 specific
#define M_USE_MMX                                     0x00002822L
#define M_USE_MMX_NEW                                 0x00002822L
#define M_USE_SSE_INT                                 0x00002823L
#define M_USE_SSE                                     0x00002824L
#define M_USE_SSE_NEW                                 0x00002824L
#define M_USE_SSE2_INT                                0x00002825L
#define M_USE_SSE2                                    0x00002826L
//Reserved for future Intel instruction set expension 0x00002827L
//                                                 to 0x00002830L
#define M_USE_AMD_3DNOW                               0x00002831L
#define M_USE_AMD_MMX_EXTENSION                       0x00002832L
#define M_USE_AMD_3DNOW_EXTENSION                     0x00002833L
//Reserved for future AMD instruction set expension   0x00002834L
//                                                 to 0x00002840L
#define M_CPU_VENDOR_NAME                             0x00002841L
#define M_CPU_FAMILY_NUMBER                           0x00002842L
#define M_CPU_MODEL_NUMBER                            0x00002843L
#define M_CPU_STEPPING_NUMBER                         0x00002844L
#define M_CPU_SERIAL_NUMBER                           0x00002845L
//Reseved for future CPU specific inquire             0x00002846L
//                                                 to 0x00002860L
#define M_USE_ALTIVEC                                 0x00002861L
//Reserved for future PPC instruction set expension   0x00002862L
//                                                 to 0x00002870L

#define M_CPU_SUPPORT_RDTSC                           0x00002871L
//Reserved for future processor specific flags        0x00002872L
//                                                 to 0x00002FFFL

//Auxiliary IO defines 4Sight
#define M_USER_BIT                                    4087L
/* Reserve next 31 values                      from   4088L*/
/*                                             to     4118L*/
#define M_USER_BIT_PRESENT                            0x40000000L
#define M_USER_BIT_MODE                               0x80000000L
#define M_USER_BIT_VALUE                              0xA0000000L
#define M_USER_BIT_INTERRUPT_MODE                     0xC0000000L
#define M_USER_BIT_INTERRUPT_STATE                    0xE0000000L
#define M_USER_BIT_MASK                               0xE0000000L
#define M_USER_BIT_BIT_MASK_ENABLED                   0x10000000L
#define M_USER_BIT_INVALID_BIT_MASK                   0x0FFFFFFFL
#define M_USER_BIT_ALL                                0xFFFFF
#define M_BIT_MASK(mask)   ( ((long) mask <= M_USER_BIT_ALL) ? (M_USER_BIT_BIT_MASK_ENABLED | ((long) mask)) : (M_USER_BIT_BIT_MASK_ENABLED | M_USER_BIT_INVALID_BIT_MASK) )
#define M_USER_BIT_CHANGE                             2244L
#define M_USER_BIT_CHANGE_HANDLER_PTR                 2245L
/* Reserve next 19 values                       from  2246L*/
/*                                             to     2264L*/
#define M_USER_BIT_CHANGE_HANDLER_USER_PTR            2266L
/* Reserve next 19 values                       from  2267L*/
/*                                             to     2285L*/
#define M_AGP_TO_PCI_TRANSFER                         2286L
#define M_TRANSFER_PRIORITY                           2287L
#define M_THREAD_CANCEL                               2288L
#define M_THREAD_HALT                                 2289L
#define M_NUMBER_OF_GRAB_BLOCKS                       2290L
/* Reserve next 4 values                        from  2291L*/
/*                                              to    2294L*/
#define M_GRAB_BLOCK_FACTOR                           2295L
/* Reserve next 4 values                        from  2296L*/
/*                                              to    2299L*/

// To override compensation on default VGA system
#define M_NO_DEFAULT_VGA_COMPENSATION                 0x01000000L
#define M_IS_NO_DEFAULT_VGA_COMPENSATION(f)           (((f)&M_NO_DEFAULT_VGA_COMPENSATION)&&(((f)&(0xFFFF0000))==M_NO_DEFAULT_VGA_COMPENSATION))

// Maximum string size for M_SYSTEM_NAME inquire value
#define M_SYSTEM_NAME_SIZE                            32L

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
//#define M_LIVE_VIDEO                                  M_LIVE_GRAB   --> M_LIVE_VIDEO: name reused for buffer attribute
#define M_LAST_GRAB_IN_ACTUAL_BUFFER                  M_LAST_GRAB_IN_TRUE_BUFFER
#define M_SWITCH_TO_PSEUDO_WHEN_OVERLAPPED            M_PSEUDO_LIVE_GRAB
#define M_FORCE_PSEUDO_IN_NON_PULSAR_DISPLAYS         M_FORCE_PSEUDO_IN_NON_UNDERLAY_DISPLAYS
#define M_SYS_TYPE                                    M_SYSTEM_TYPE
#define M_SYS_TYPE_PTR                                M_SYSTEM_TYPE_PTR
#define M_SYS_NUMBER                                  M_NUMBER
#define M_SYS_INIT_FLAG                               M_INIT_FLAG
#define M_SYS_DISPLAY_NUM                             M_DISPLAY_NUM
#define M_SYS_DISPLAY_TYPE                            M_DISPLAY_TYPE
#define M_SYS_DIGITIZER_NUM                           M_DIGITIZER_NUM
#define M_SYS_DIGITIZER_TYPE                          M_DIGITIZER_TYPE
#define M_SYS_PROCESSOR_NUM                           M_PROCESSOR_NUM
#define M_SYS_BOARD_TYPE                              M_BOARD_TYPE
#define M_SYS_BOARD_REVISION                          M_BOARD_REVISION
#define M_SYS_TUNER_NUM                               M_TUNER_NUM
#define M_SYS_TUNER_TYPE                              M_TUNER_TYPE
#define M_SYS_RGB_MODULE_NUM                          M_RGB_MODULE_NUM
#define M_SYS_RGB_MODULE_TYPE                         M_RGB_MODULE_TYPE
#define M_SYS_DISPLAY_LIST                            M_DISPLAY_LIST
#define M_SYS_DUAL_SCREEN_MODE                        M_DUAL_SCREEN_MODE
#define M_SYS_UNDERLAY_SURFACE_AVAILABLE              M_UNDERLAY_SURFACE_AVAILABLE
#define M_SYS_UNDERLAY_SURFACE_PHYSICAL_ADDRESS       M_PHYSICAL_ADDRESS_UNDERLAY
#define M_SYS_WIN_MODE                                M_WIN_MODE
#define M_SYS_MAX_TILE_SIZE                           M_MAX_TILE_SIZE
#define M_SYS_MAX_TILE_SIZE_X                         M_MAX_TILE_SIZE_X
#define M_SYS_MAX_TILE_SIZE_Y                         M_MAX_TILE_SIZE_Y
#define M_ON_BOARD_MEM_ADRS                           M_PHYSICAL_ADDRESS_UNDERLAY
#define M_ON_BOARD_VGA_ADRS                           M_PHYSICAL_ADDRESS_VGA
#define MMX_EXTRA_BYTES                               M_MMX_EXTRA_BYTES
#define M_PSEUDO_LIVE_GRAB_WHEN_OVERLAPPED            M_PSEUDO_LIVE_GRAB
#define M_LOCATION_WORK_HOST                          M_SYSTEM_HOST_TYPE
#define M_DMA_POOL_FREE                               M_NON_PAGED_MEMORY_FREE
#define M_DMA_POOL_USED                               M_NON_PAGED_MEMORY_USED
#define M_DMA_POOL_SIZE                               M_NON_PAGED_MEMORY_SIZE
#define M_BOTH_FIELD                                  M_BOTH_FIELDS


#endif // !!! MAP FOR OLD DEFINES

/************************************************************************/
/* MsysInquire() / MsysControl() Values                                 */
/************************************************************************/

#define M_FORCE                                       -9998L
#define M_L1                                              1L

/************************************************************************/
/* MsysHookFunction()                                                   */
/************************************************************************/
#define M_1394_BUS_RESET                              2287L
#define M_USER_BIT_CHANGE                             2244L
#define M_USER_BIT_CHANGE_HANDLER_PTR                 2245L
/* Reserve next 19 values                       from  2246L*/
/*                                             to     2264L*/
#define M_USER_BIT_CHANGE_HANDLER_USER_PTR            2266L
/* Reserve next 19 values                       from  2267L*/
/*                                             to     2285L*/


/************************************************************************/
/* MsysGetHookInfo()                                                    */
/************************************************************************/

#define M_USER_BIT                                    4087L
/* Reserve next 31 values                      from   4088L*/
/*                                             to     4118L*/
#define M_USER_BIT_FIELD                              4181L
#define M_USER_BIT_CHANGE                             2244L
#define M_USER_BIT_CHANGE_HANDLER_PTR                 2245L
/* Reserve next 19 values                       from  2246L*/
/*                                             to     2264L*/
#define M_USER_BIT_CHANGE_HANDLER_USER_PTR            2266L
/* Reserve next 19 values                       from  2267L*/
/*                                             to     2285L*/


/************************************************************************/
/* MsysConfigAccess()                                                   */
/************************************************************************/
#define M_PCI_CONFIGURATION_SPACE                     0L
#define M_DETECT_PCI_DEVICE                           1L
#define M_1394_CONFIGURATION_SPACE                    2L
#define M_RESERVED_FOR_INTERNAL_USE_SYS_CONFIG        3L

#define M_DEFAULT                                     0x10000000L
#define M_READ                                        1
#define M_WRITE                                       2
#define M_BIG_ENDIAN                                  0x00000100L
#define M_LITTLE_ENDIAN                               0x00000200L


#define M_PCI_VENDOR_ID                               0x00 // (16 lsb)
#define M_PCI_DEVICE_ID                               0x00 // (16 msb)
#define M_PCI_COMMAND                                 0x01 // (16 lsb)
#define M_PCI_STATUS                                  0x01 // (16 msb)
#define M_PCI_REVISION_ID                             0x02 // (byte 0)
#define M_PCI_CLASS_CODE                              0x02 // (byte 1,2,3)
#define M_PCI_LATENCY_TIMER                           0x03 // (byte 1)
#define M_PCI_BASE_ADRS0                              0x04 //
#define M_PCI_BASE_ADRS1                              0x05 //
#define M_PCI_INT_LINE                                0x0F // (byte 0)
#define M_PCI_INT_PIN                                 0x0F // (byte 1)

//Auxiliary IO values 4Sight
#define M_OFF                                         0L       // Must be zero
#define M_ON                                          1L
#define M_INPUT                                       2L
#define M_OUTPUT                                      3L
#define M_EDGE_RISING                                 12L
#define M_EDGE_FALLING                                13L
#define M_DISABLE                                     -9999L
#define M_ENABLE                                      -9997L

/************************************************************************/
/* MsysControlThread                                                    */
/************************************************************************/
// Control Values
#define M_SIGNALED                                    0x00000010L
#define M_NOT_SIGNALED                                0x00000020L
#define M_AUTO_RESET                                  0x00002000L
#define M_MANUAL_RESET                                0x00004000L
#define M_DEFAULT                                     0x10000000L

#define M_THREAD_CURRENT                              0x01000000L
#define M_THREAD_ALL                                  0x02000000L
#define M_QUEUED                                      0x04000000L    //To be ORed with M_THREAD_COMMANDS_ABORT

#define M_IDLE                                                 1L
#define M_LOWEST                                               6L
#define M_BELOW_NORMAL                                         7L
#ifndef M_MIL_75_COMPATIBLE
#define M_NORMAL                                               8L
#else
#define M_NORMAL                                               1L
#endif //M_MIL_75_COMPATIBLE
#define M_ABOVE_NORMAL                                         9L
#define M_HIGHEST                                             10L
#define M_TIME_CRITICAL                                       15L

// Control Types
#define M_INQUIRE                                     0x80000000

#define M_NATIVE_ID                                   1016L
#define M_THREAD_PRIORITY                             1019L
#define M_EVENT_ALLOC                                 1700L
#define M_EVENT_FREE                                  1701L
#define M_EVENT_SYNCHRONIZE                           1704L
#ifdef  M_MIL_75_COMPATIBLE                                                 //TBR MIL 8.0
#define M_EVENT_WAIT                                  0x00000100L       //TBR MIL 8.0
#define M_EVENT_STATE                                 0x00000200L       //TBR MIL 8.0
#define M_EVENT_SET                                   0x00000400L       //TBR MIL 8.0
#else
#define M_EVENT_WAIT                                  1705L
#define M_EVENT_STATE                                 1706L
#define M_EVENT_SET                                   1707L
#endif
#define M_EVENT_TIMEOUT                               1708L
#define M_EVENT_CREATE                                1710L
#define M_EVENT_CREATE_ON_NATIVE                      1711L
#define M_THREAD_WAIT                                 1802L
#define M_THREAD_MODE                                 1804L
#define M_THREAD_COMMANDS_ABORT                       1810L
#define M_THREAD_TIME_SLICE                           1811L
#define M_ACCELERATOR                                 1813L
#define M_BUS_MASTER_COPY_MODE                        1815L



/************************************************************************/
/* MdispAlloc() for Windowed system                                     */
/************************************************************************/
// Maximum number of display that can be managed
#define M_MAX_VGA_DEVICE                              16L

/* DispNum */
#define M_DEV0                                        0L
#define M_DEV1                                        1L
#define M_DEV2                                        2L
#define M_DEV3                                        3L
#define M_DEV4                                        4L
#define M_DEV5                                        5L
#define M_DEV6                                        6L
#define M_DEV7                                        7L
#define M_DEV8                                        8L
#define M_DEV9                                        9L
#define M_DEV10                                       10L
#define M_DEV11                                       11L
#define M_DEV12                                       12L
#define M_DEV13                                       13L
#define M_DEV14                                       14L
#define M_DEV15                                       15L
#define M_DEV16                                       16L
#define M_DEV17                                       17L
#define M_DEV18                                       18L
#define M_DEV19                                       19L
#define M_DEV20                                       20L
#define M_DEV21                                       21L
#define M_DEV22                                       22L
#define M_DEV23                                       23L
#define M_DEV24                                       24L
#define M_DEV25                                       25L
#define M_DEV26                                       26L
#define M_DEV27                                       27L
#define M_DEV28                                       28L
#define M_DEV29                                       29L
#define M_DEV30                                       30L
#define M_DEV31                                       31L
#define M_DEV32                                       32L
#define M_DEV33                                       33L
#define M_DEV34                                       34L
#define M_DEV35                                       35L
#define M_DEV36                                       36L
#define M_DEV37                                       37L
#define M_DEV38                                       38L
#define M_DEV39                                       39L
#define M_DEV40                                       40L
#define M_DEV41                                       41L
#define M_DEV42                                       42L
#define M_DEV43                                       43L
#define M_DEV44                                       44L
#define M_DEV45                                       45L
#define M_DEV46                                       46L
#define M_DEV47                                       47L
#define M_DEV48                                       48L
#define M_DEV49                                       49L
#define M_DEV50                                       50L
#define M_DEV51                                       51L
#define M_DEV52                                       52L
#define M_DEV53                                       53L
#define M_DEV54                                       54L
#define M_DEV55                                       55L
#define M_DEV56                                       56L
#define M_DEV57                                       57L
#define M_DEV58                                       58L
#define M_DEV59                                       59L
#define M_DEV60                                       60L
#define M_DEV61                                       61L
#define M_DEV62                                       62L
#define M_DEV63                                       63L
#define M_NODE0                                       0x00010000L
#define M_NODE1                                       0x00020000L
#define M_NODE2                                       0x00040000L
#define M_NODE3                                       0x00080000L
#define M_NODE4                                       0x00100000L
#define M_NODE5                                       0x00200000L
#define M_NODE6                                       0x00400000L
#define M_NODE7                                       0x00800000L
#define M_NODE8                                       0x01000000L
#define M_NODE9                                       0x02000000L
#define M_NODE10                                      0x04000000L
#define M_NODE11                                      0x08000000L
#define M_NODE12                                      0x10000000L
#define M_NODE13                                      0x20000000L
#define M_NODE14                                      0x40000000L
#define M_NODE15                                      0x80000000L
#define M_NODE_BITS                                   (M_NODE0|M_NODE1|M_NODE2|M_NODE3|M_NODE4|M_NODE5|   \
                                                       M_NODE6|M_NODE7|M_NODE8|M_NODE9|M_NODE10|M_NODE11| \
                                                       M_NODE12|M_NODE13|M_NODE14|M_NODE15)

/* Init Flag */

#define M_USE_MEMORY_VCF                              0x00000010L // Old Display define
#define M_USE_EXTERNAL_CLOCK                          0x00000020L
#define M_USE_INTERNAL_CLOCK                          0x00000040L
#define M_OVR                                         0x00000080L
#define M_OVERLAY                                     M_OVR
#define M_DDRAW_UNDERLAY                              0x00000002L
#define M_DDRAW_UND                                   M_DDRAW_UNDERLAY
#define M_ZOOM_ENHANCED                               0x00000000L // Old display define
//#define M_NEW_DISP_TYPE                             0x00000100L
//#define M_NEW_DISP_TYPE                             0x00000200L
#define M_ZOOM_BASIC                                  0x00000400L // Old display define
#define M_DISPLAY_8_BASIC                             0x00000000L // Old display define
#define M_DISPLAY_8_ENHANCED                          0x00000800L // Old display define
#define M_DISPLAY_24_ENHANCED                         0x00000000L // Old display define
#define M_DISPLAY_24_BASIC                            0x00001000L // Old display define
#define M_DISPLAY_24_WINDOWS                          0x00002000L // Old display define
#define M_DUAL_SCREEN                                 0x00004000L
#define M_VIDEO_ENCODER                               0x00008000L
#define M_AUXILIARY                                   0x00010000L
#define M_COLORTABLE_INDEX                            0x00020000L // Old display define
#define M_EXTERNAL                                    0x00040000L
#define M_SINGLE_SCREEN                               0x00080000L // Old display define
#define M_PALETTE_NOCOLLAPSE                          0x00100000L // Old display define
#define M_IN_BUFFER_DISPLAY                           0x00200000L
#define M_DESKTOP_DISPLAY                             0x00400000L
#define M_UND                                         0x00800000L // Old display define
#define M_UNDERLAY                                    M_UND       // Old display define
#define M_WINDOWED                                    0x01000000L
#define M_NON_WINDOWED                                0x02000000L // Old display define
#define M_BASE_DRIVER                                 0x04000000L // Old display define
#define M_MIL_WINDOW                                  0x08000000L
#define M_USER_WINDOW                                 0x10000000L
#define M_GDI_OVERLAY                                 0x20000000L
//#define M_NEW_DISP_TYPE                             0x40000000L
#define M_INTERNAL_ALLOCATION                         ((BUFATTRTYPE)MAKE_INT64(0x0000000080000000))
#define M_DISPLAY_ENHANCED                            (M_DISPLAY_8_ENHANCED + M_DISPLAY_24_ENHANCED) // Old display define
#define M_DISPLAY_BASIC                               (M_DISPLAY_8_BASIC    + M_DISPLAY_24_BASIC   ) // Old display define
#define M_DISPLAY_WINDOWS                             (M_DISPLAY_8_BASIC    + M_DISPLAY_24_WINDOWS ) // Old display define
#define M_PALETTE_COLLAPSE                            0x00000000L // Old display define
#define M_WINDOW_MAXIMIZE                             0x00000000L // Old display define
#define M_WINDOW_NO_MENUBAR                           0x00000000L // Old display define
#define M_WINDOW_NO_TITLEBAR                          0x00000000L // Old display define
#define M_WINDOW_NO_KEY                               0x00000000L // Old display define
#define M_WINDOW_USE_FORMAT                           0x00000000L // Old display define
#define M_WINDOW_NO_SYSBUTTON                         0x00000000L // Old display define
#define M_WINDOW_NO_MINBUTTON                         0x00000000L // Old display define
#define M_WINDOW_NO_MAXBUTTON                         0x00000000L // Old display define
#define M_COLORTABLE_RGB                              0x00000000L // Old display define

/************************************************************************/
/* MdispInquire() / MdispControl() Types                                */
/************************************************************************/
#define M_OWNER_APPLICATION                           1000L
#define M_OWNER_SYSTEM                                1001L
#define M_SIZE_X                                      1002L
#define M_SIZE_Y                                      1003L
#define M_SIZE_Z                                      1004L
#define M_SIZE_BAND                                   1005L
#define M_SIZE_BAND_LUT                               1006L
#define M_SIZE_BIT                                    1007L
#define M_TYPE                                        1008L
#define M_NUMBER                                      1009L
#define M_FORMAT                                      1010L
#define M_FORMAT_SIZE                                 1011L
#define M_INIT_FLAG                                   1012L
#define M_ATTRIBUTE                                   1013L
#define M_SIGN                                        1014L
#define M_LUT_ID                                      1015L
#define M_NATIVE_ID                                   1016L
#define M_NATIVE_CONTROL_ID                           1017L
#define M_COLOR_MODE                                  1018L
#define M_THREAD_PRIORITY                             1019L
#define M_NEED_UPDATE                                 1020L
#define M_SURFACE                                     1021L
//1022L was use by M_WINDOW_DDRAW_SURFACE, that is now in oldDefine section.
#define M_OWNER_SYSTEM_TYPE                           1023L
#define M_DISP_NATIVE_ID                              1024L
#define M_ENCODER_START                               1025L
#define M_ENCODER                                     M_ENCODER_START
#define M_ENCODER_MODE                                1026L
#define M_ENCODER_TYPE                                1027L
#define M_ENCODER_SYNC_SOURCE                         1028L
#define M_ENCODER_SYNC                                M_ENCODER_SYNC_SOURCE
#define M_ENCODER_PEDESTAL                            1029L
#define M_ENCODER_FILTER                              1030L
#define M_ENCODER_RGB_SYNC                            1031L
#define M_ENCODER_INFO_MODE                           1032L
#define M_ENCODER_RESET                               1033L   //1034 reseved
#define M_ENCODER_SELECT_FIELD_ODD                    1037L
#define M_ENCODER_SELECT_FIELD_EVEN                   1038L
#define M_ENCODER_CONTRAST                            1039L
#define M_ENCODER_BRIGHTNESS                          1040L
#define M_ENCODER_SATURATION                          1041L
#define M_ENCODER_HUE                                 1042L
#define M_ENCODER_OVR_SHOW                            1043L
#define M_ENCODER_OVR_LUT                             1044L
#define M_ENCODER_OVR_BUF_ID                          1045L

#define M_ENCODER_STATE                               M_ENCODER
#define M_SYNC_TYPE                                   M_ENCODER_RGB_SYNC
#define M_ENCODER_SYNC_LOCK                           M_ENCODER_SYNC_SOURCE
#define M_FILTER                                      1046L
#define M_ENCODER_SYNC_FIELDS                         1047L
#define M_ENCODER_CLOCK                               1048L

#define M_UNDERLAY_PAN_Y                              1049L
#define M_UNDERLAY_PAN_X                              1050L

/* Reserve next values endoder use             from   1049L*/
/*                                             to     1100L*/
#define M_ENCODER_END                                 1100L

#define M_PAN_X                                       3000L
#define M_PAN_Y                                       3001L
#define M_ZOOM_X                                      3002L
#define M_ZOOM_Y                                      3003L
#define M_HARDWARE_PAN                                3004L
#define M_HARDWARE_ZOOM                               3005L
#define M_SELECTED                                    3006L
#define M_KEY_MODE                                    3007L
#define M_KEY_CONDITION                               3008L
#define M_KEY_MASK                                    3009L
#define M_TRANSPARENT_COLOR                           0x01000060L
#define M_KEY_SUPPORTED                               3011L
#define M_VGA_BUF_ID                                  3012L
#define M_WINDOW_BUF_WRITE                            3013L
#define M_WINDOW_BUF_ID                               3014L
#define M_OVERLAY_ID                                  3015L
#define M_OVERLAY                                     M_OVR // 0x00000080
#define M_WINDOW_OVR_DISP_ID                          3017L
#define M_INTERPOLATION_MODE                          3018L
#define M_HOOK_OFFSET                                 3019L
#define M_FRAME_START_HANDLER_PTR                     3020L
#define M_FRAME_START_HANDLER_USER_PTR                3021L
#define M_OVERLAY_LUT                                 3022L
#define M_OVERLAY_SHOW                                3023L
#define M_WINDOW_DISPLAY_SETTINGS                     3024L
#define M_WINDOW_OVR_LUT_REMAP                        3025L
#define M_WINDOW_AUTO_ACTIVATION_FOR_DDRAW            3026L
#define M_DISPLAY_16_TO_8                             3027L
#define M_DISPLAY_16_TO_8_SHIFT                       3028L
#define M_DISPLAY_MODE                                3029L
#define M_WINDOW_OVR_FLICKER                          3031L
#define M_PIXEL_FORMAT                                3032L
#define M_LUT_SUPPORTED                               3033L


#define M_WINDOW_ZOOM                                 3051L
#define M_WINDOW_RESIZE                               3052L
#define M_WINDOW_OVERLAP                              3053L
#define M_WINDOW_SCROLLBAR                            3054L
// 3055L was used by M_WINDOW_UPDATE  now in old defines
#define M_WINDOW_PROTECT_AREA                         3056L
#define M_WINDOW_TITLE_BAR                            3057L
#define M_WINDOW_MENU_BAR                             3058L
#define M_WINDOW_TITLE_BAR_CHANGE                     3059L
#define M_WINDOW_MENU_BAR_CHANGE                      3060L
#define M_WINDOW_MOVE                                 3061L
#define M_WINDOW_SYSBUTTON                            3062L
#define M_WINDOW_MINBUTTON                            3063L
#define M_WINDOW_MAXBUTTON                            3064L
#define M_WINDOW_COLOR                                3065L
#define M_WINDOW_COLOR_CHANGE                         3066L
#define M_PALETTE_WINDOWS                             3068L
#define M_WINDOW_PALETTE_NOCOLLAPSE                   3069L
#define M_WINDOW_PALETTE_BACKGROUND                   3070L
#define M_WINDOW_PALETTE_AUTO                         3071L
#define M_WINDOW_RANGE                                3072L  //0x0C00
#define M_WINDOW_UPDATE_AUTO_ON_CONTROL               3073L
// 3074L was used by M_WINDOW_UPDATE_WITH_SEND_MESSAGE  now in old defines
#define M_WINDOW_SNAP_X                               3075L
#define M_WINDOW_SNAP_Y                               3076L
#define M_WINDOW_UPDATE_REGION                        3077L
#define M_WINDOW_UPDATE_ONLY_INVALID_BORDER           3078L
#define M_WINDOW_UPDATE_KEEP_PALETTE_ALIVE            3079L
#define M_WINDOW_UPDATE_ADD_BEGINPAINT                3080L
#define M_WINDOW_UPDATE_ON_PAINT                      3081L
#define M_WINDOW_UPDATE_MANUAL                        3082L
#define M_WINDOW_PAINT                                3083L
#define M_WINDOW_ACTIVATE_DELAY                       3084L
#define M_WINDOW_CLIP_IN_CLIENT                       3085L
#define M_WINDOW_SYNC_SELECT                          3087L
#define M_WINDOW_INITIAL_POSITION_X                   3088L
#define M_WINDOW_INITIAL_POSITION_Y                   3089L
#define M_WINDOW_BENCHMARK_IN_DEBUG                   3090L
#define M_WINDOW_ERASE_BACKGROUND                     3091L
#define M_WINDOW_OVR_BUFFER_ALIVE                     3092L
#define M_WINDOW_OVR_BUFFER_PTR                       3093L
#define M_WINDOW_OVR_FLICKER_FREE_ALIVE               3094L
#define M_WINDOW_OVR_FLICKER_FREE_PTR                 3095L
#define M_WINDOW_OVR_DESTRUCTIVE                      3096L
#define M_WINDOW_OVR_KEYER_PTR                        3097L
#define M_WINDOW_MANUAL_OVR_ADD                       3098L
#define M_WINDOW_MANUAL_FLICKER_COPY                  3099L
#define M_WINDOW_MANUAL_OVR_ADD_FLICKER_COPY          3100L
#define M_WINDOW_USE_SUBCLASS_TRACKING                3101L
#define M_WINDOW_USE_SYSTEMHOOK_TRACKING              3102L
#define M_WINDOW_ATTRIBUTE_FOR_OVERLAY                3103L
#define M_WINDOW_ATTRIBUTE_FOR_FLICKER                3104L
#define M_WINDOW_MASK_FOR_OVERLAY_VERIFICATION        3105L
#define M_WINDOW_MASK_FOR_FLICKER_VERIFICATION        3106L
#define M_DESKTOP_CHANGE                              3107L
#define M_WINDOW_HOOK_BLOCKING_SERIALIZATION          3108L
#define M_WINDOW_ATTRIBUTE_FOR_BUFFER                 3109L
#define M_WINDOW_HANDLE                               3110L
#define M_WINDOW_OFFSET_X                             3111L
#define M_WINDOW_OFFSET_Y                             3112L
#define M_WINDOW_SIZE_X                               3113L
#define M_WINDOW_SIZE_Y                               3114L
#define M_WINDOW_PAN_X                                3115L
#define M_WINDOW_PAN_Y                                3116L
#define M_WINDOW_ZOOM_X                               3117L
#define M_WINDOW_ZOOM_Y                               3118L
#define M_WINDOW_TITLE_NAME                           3119L
#define M_HOOK_MODIFIED_DIB_PTR                       3120L
#define M_WINDOW_USE_SYSTEMHOOK_TRACKING_ACTIVE       3121L
/* Reserve next 2 values                       from   3121L*/
/*                                             to     3122L*/
#define M_HOOK_MODIFIED_DIB_USER_PTR                  3123L
/* Reserve next 2 values                       from   3124L*/
/*                                             to     3125L*/
#define M_HOOK_MODIFIED_WINDOW_PTR                    3126L
/* Reserve next 2 values                       from   3127L*/
/*                                             to     3128L*/
#define M_HOOK_MODIFIED_WINDOW_USER_PTR               3129L
/* Reserve next 2 values                       from   3130L*/
/*                                             to     3131L*/
#define M_HOOK_MESSAGE_LOOP_PTR                       3132L
/* Reserve next 2 values                       from   3133L*/
/*                                             to     3134L*/
#define M_HOOK_MESSAGE_LOOP_USER_PTR                  3135L
/* Reserve next 2 values                       from   3136L*/
/*                                             to     3137L*/
#define M_WINDOW_APPFRAME_HANDLE                      3138L
#define M_WINDOW_MDICLIENT_HANDLE                     3139L
#define M_WINDOW_MDIFRAME_HANDLE                      3140L
#define M_WINDOW_VISIBLE_OFFSET_X                     3141L
#define M_WINDOW_VISIBLE_OFFSET_Y                     3142L
#define M_WINDOW_VISIBLE_SIZE_X                       3145L
#define M_WINDOW_VISIBLE_SIZE_Y                       3146L
#define M_WINDOW_DIB_HANDLE                           3147L
#define M_WINDOW_DISPLAY_DIB_HANDLE                   3148L
#define M_WINDOW_ACTIVE                               3149L
#define M_WINDOW_ENABLE                               3150L
#define M_PALETTE_HANDLE                              3151L
#define M_WINDOW_THREAD_HANDLE                        3152L
#define M_WINDOW_THREAD_ID                            3153L
//3154L was use by M_WINDOW_DIB_HEADER, that is now in oldDefine section.
#define M_WINDOW_KEYBOARD_USE                         3155L
#define M_WINDOW_CLIP_LIST_SIZE                       3156L
#define M_WINDOW_CLIP_LIST                            3157L
#define M_WINDOW_CLIP_LIST_ACCESS                     3158L
#define M_FRAME_START_TRIGGER_MODE                    3159L
#define M_FRAME_START_TRIGGER                         3160L
#define M_WINDOW_DIB                                  3161L
#define M_WINDOW_MAP_BUFFER                           3162L
#define M_WINDOW_UPDATE_EXCLUDE_RECTANGLE             3164L
#define M_WINDOW_SYNC_UPDATE                          3165L
#define M_WINDOW_TITLE_NAME_SIZE                      3166L
#define M_WINDOW_DRIVER_SIZE_BIT                      3167L
#define M_WINDOW_SYNC_UPDATE_WHEN_HOOK_BLOCKED        3168L
#define M_WINDOW_CLIP_LIST_BLOCKING_SERIALIZATION     3169L
#define M_DESKTOP_LOCK_TIMEOUT                        3170L
#define M_WINDOW_PALETTE_MESSAGES                     3171L
#define M_WINDOW_PAINT_MESSAGES                       3172L
#define M_WINDOW_COMMAND_PROMPT_FULL_DRAG             3173L
#define M_WINDOW_DISPLAY_MODE                         3174L
#define M_WINDOW_REFRESH_AT_DESELECT                  3175L
#define M_WINDOW_BLIT_WITH_DDRAW                      3180L
#define M_DESKTOP_CURRENT_SCAN_LINE                   3181L
#define M_VIEW_MODE                                   3182L
#define M_VIEW_BIT_SHIFT                              3183L
#define M_DDRAW_UNDERLAY_SET_POSITION                 3184L
#define M_DDRAW_UPDATE_OVERLAY                        3185L
#define M_WINDOW_DDRAW_DDCAPS                         3186L
#define M_FLIP_DDRAW_SURFACE                          3187L
#define M_NO_TEARING                                  3188L
#define M_DEFAULT_PALETTE                             3189L
#define M_ZOOM_MAX_X                                  3190L
#define M_ZOOM_MIN_X                                  3191L
#define M_ZOOM_MAX_Y                                  3192L
#define M_ZOOM_MIN_Y                                  3193L
#define M_WINDOW_PAINT_DURING_WM_QUERYNEWPALETTE      3194L
//3195L was use by M_DDRAW_UNDERLAY_ZOOM_MAX, that is now in the Display reserved area.
//3196L was use by M_DDRAW_UNDERLAY_ZOOM_MIN, that is now in the Display reserved area.
#define M_DUAL_ZOOM                                   3197L
#define M_DUAL_ZOOM_SUPPORTED                         3198L
#define M_UPDATE                                      3199L
#define M_WINDOW_MOVING_STATE                         3200L
#define M_WINDOW_OVR_FLICKER_BUF_ID                   3201L
#define M_WINDOW_VISIBLE                              3202L
#define M_SCALE_X                                     3203L
#define M_SCALE_Y                                     3204L
#define M_WINDOW_SUBCLASSING_RESISTANT                3205L // internal use only
#define M_AUXILIARY_BUF_ID                            3206L // internal use only
#define M_AUXILIARY_KEEP_DISPLAY_ALIVE                3207L

#define M_GRAB_BUFFERS                                5095L // internal use only

#define M_SCALE                                 0x00008010L

// MdispControl(M_REMAP_TYPE)
#define M_MULTI_BYTES                                    1L
#define M_AUTO_SCALE                                     2L
#define M_BIT_SHIFT                                      3L
#define M_LOGARITHMIC                                    4L
#define M_PIXEL_VALUES                                   5L
#define M_TRANSPARENT                                    0x01000059L
#define M_MED_SERIES                                     M_TRANSPARENT

#define M_DISP_CONTROL_DIRECT_START                   10000L
#define M_NO_TEARING_SKIP_MODE                        (M_DISP_CONTROL_DIRECT_START+1L)    // 10001
#define M_NO_SKIP                                     (M_DISP_CONTROL_DIRECT_START+2L)    // 10002
#define M_SKIP_OLDEST                                 (M_DISP_CONTROL_DIRECT_START+3L)    // 10003
#define M_SKIP_NEWEST                                 (M_DISP_CONTROL_DIRECT_START+4L)    // 10004
#define M_DISPLAY_SCHEME                              (M_DISP_CONTROL_DIRECT_START+5L)    // 10005
#define M_OVERLAY_CLEAR                               (M_DISP_CONTROL_DIRECT_START+6L)    // 10006
#define M_FORCE_MONO_OVR                              (M_DISP_CONTROL_DIRECT_START+7L)    // 10007
#define M_ZOOM_FACTOR_X                               (M_DISP_CONTROL_DIRECT_START+8L)    // 10008
#define M_ZOOM_FACTOR_Y                               (M_DISP_CONTROL_DIRECT_START+9L)    // 10009
#define M_PAN_OFFSET_X                                (M_DISP_CONTROL_DIRECT_START+10L)   // 10010
#define M_PAN_OFFSET_Y                                (M_DISP_CONTROL_DIRECT_START+11L)   // 10011
#define M_VGA_INDEX                                   (M_DISP_CONTROL_DIRECT_START+12L)   // 10012
#define M_GDI_COMPATIBLE_OVERLAY                      (M_DISP_CONTROL_DIRECT_START+13L)   // 10013
#define M_HSCROLL_VISIBLE                             (M_DISP_CONTROL_DIRECT_START+14L)   // 10014
#define M_VSCROLL_VISIBLE                             (M_DISP_CONTROL_DIRECT_START+15L)   // 10015
#define M_SRC_UPDATE_REGION_OFFSET_X                  (M_DISP_CONTROL_DIRECT_START+16L)   // 10016
#define M_SRC_UPDATE_REGION_OFFSET_Y                  (M_DISP_CONTROL_DIRECT_START+17L)   // 10017
#define M_NEAREST_COLOR                               (M_DISP_CONTROL_DIRECT_START+18L)   // 10018
#define M_CENTER_DISPLAY                              (M_DISP_CONTROL_DIRECT_START+19L)   // 10019

#define M_VGA_DEVICE_SYSTEM_ID                        (M_DISP_CONTROL_DIRECT_START+100L)  // 10100
#define M_DISPLAY_SURFACE                             (M_DISP_CONTROL_DIRECT_START+101L)  // 10101
#define M_DDRAW_UNDERLAY_ID                           (M_DISP_CONTROL_DIRECT_START+102L)  // 10102
#define M_USER_OVERLAY_ID                             (M_DISP_CONTROL_DIRECT_START+106L)  // 10106
#define M_DISPLAY_RESOLUTION                          (M_DISP_CONTROL_DIRECT_START+107L)  // 10107
#define M_DEFAULT_DISPLAY_RESOLUTION                  (M_DISP_CONTROL_DIRECT_START+108L)  // 10108
#define M_SAFE_MODE                                   (M_DISP_CONTROL_DIRECT_START+109L)  // 10109
#define M_USE_ON_BOARD_BUFFER                         (M_DISP_CONTROL_DIRECT_START+110L)  // 10110
#define M_SELECT_USER_BUFFER_DIRECLTY_ON_DISPLAY      (M_DISP_CONTROL_DIRECT_START+111L)  // 10111
#define M_DISPLAY_FORMAT                              (M_DISP_CONTROL_DIRECT_START+112L)  // 10112
#define M_DISPLAY_FORMAT_SUPPORTED                    (M_DISP_CONTROL_DIRECT_START+113L)  // 10113
#define M_DISPLAY_TYPE                                (M_DISP_CONTROL_DIRECT_START+114L)  // 10114
#define M_USE_DDRAW_UNDERLAY_SURFACE                  (M_DISP_CONTROL_DIRECT_START+115L)  // 10115
#define M_USE_DDRAW_UNDERLAY_IN_DUALHEAD              (M_DISP_CONTROL_DIRECT_START+116L)  // 10116
#define M_DISPLAY_SCHEME_ON_TITLEBAR                  (M_DISP_CONTROL_DIRECT_START+117L)  // 10117
#define M_UPDATE_SYNCHRONIZATION                      (M_DISP_CONTROL_DIRECT_START+118L)  // 10118
#define M_UNDERLAY_ALWAYS_ON_TOP                      (M_DISP_CONTROL_DIRECT_START+119L)  // 10119
#define M_COMPLEX_BUFFER                              (M_DISP_CONTROL_DIRECT_START+120L)  // 10120
#define M_USER_WINDOW_HANDLE                          (M_DISP_CONTROL_DIRECT_START+121L)  // 10121
#define M_USE_HOST_OVERLAY                            (M_DISP_CONTROL_DIRECT_START+122L)  // 10122
#define M_SELECTED_LINK_COPY                          (M_DISP_CONTROL_DIRECT_START+123L)  // 10123
#define M_HOOK_ON_ERROR                               (M_DISP_CONTROL_DIRECT_START+124L)  // 10124
#define M_USE_LIVE_DDRAW_UNDERLAY                     (M_DISP_CONTROL_DIRECT_START+125L)  // 10125
#define M_UPDATE_WITH_PAINT_MESSAGE                   (M_DISP_CONTROL_DIRECT_START+126L)  // 10126
#define M_WINDOW_ZOOM_FACTOR_X                        (M_DISP_CONTROL_DIRECT_START+127L)  // 10127
#define M_WINDOW_ZOOM_FACTOR_Y                        (M_DISP_CONTROL_DIRECT_START+128L)  // 10128
#define M_REAL_ZOOM_FACTOR_X                          (M_DISP_CONTROL_DIRECT_START+129L)  // 10129
#define M_REAL_ZOOM_FACTOR_Y                          (M_DISP_CONTROL_DIRECT_START+130L)  // 10130
#define M_DISPLAY_SCHEME_COUNT                        (M_DISP_CONTROL_DIRECT_START+131L)  // 10131
#define M_USE_DOUBLE_BUFFERING_SCHEME                 (M_DISP_CONTROL_DIRECT_START+132L)  // 10132
#define M_UPDATE_STATE                                (M_DISP_CONTROL_DIRECT_START+133L)  // 10133
#define M_FORCE_DISPLAY_SCHEME_CHANGE                 (M_DISP_CONTROL_DIRECT_START+134L)  // 10134
#define M_DISPLAY_DUMP                                (M_DISP_CONTROL_DIRECT_START+135L)  // 10135
#define M_BUFFER_TO_RECYCLE                           (M_DISP_CONTROL_DIRECT_START+136L)  // 10136
#define M_DDRAW_UNDERLAY_SHOW                         (M_DISP_CONTROL_DIRECT_START+137L)  // 10137
#define M_DISP_CONTROL_DIRECT_STOP                    11000L

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_KEY_COLOR                                   M_TRANSPARENT_COLOR
#define M_WINDOW_OVR_WRITE                            M_OVERLAY
#define M_WINDOW_OVR_BUF_ID                           M_OVERLAY_ID
#define M_WINDOW_OVR_SHOW                             M_OVERLAY_SHOW
#define M_WINDOW_OVR_LUT                              M_OVERLAY_LUT
#define M_WINDOW_UPDATE                               M_UPDATE

#define M_VGA_PIXEL_FORMAT                            M_PIXEL_FORMAT

#define M_DISP_LUT                                    M_LUT_ID
#define M_DISP_NUMBER                                 M_NUMBER
#define M_DISP_FORMAT                                 M_FORMAT
#define M_DISP_INIT_FLAG                              M_INIT_FLAG
#define M_DISP_PAN_X                                  M_PAN_X
#define M_DISP_PAN_Y                                  M_PAN_Y
#define M_DISP_ZOOM_X                                 M_ZOOM_X
#define M_DISP_ZOOM_Y                                 M_ZOOM_Y
#define M_DISP_HARDWARE_PAN                           M_HARDWARE_PAN
#define M_DISP_HARDWARE_ZOOM                          M_HARDWARE_ZOOM
#define M_DISP_KEY_MODE                               M_KEY_MODE
#define M_DISP_KEY_CONDITION                          M_KEY_CONDITION
#define M_DISP_KEY_MASK                               M_KEY_MASK
#define M_DISP_KEY_COLOR                              M_KEY_COLOR
#define M_DISP_16_TO_8                                M_DISPLAY_16_TO_8
#define M_DISP_16_TO_8_SHIFT                          M_DISPLAY_16_TO_8_SHIFT
#define M_DISP_MODE                                   M_DISPLAY_MODE
#define M_DISP_THREAD_PRIORITY                        M_THREAD_PRIORITY
#define M_DISP_INTERPOLATION_MODE                     M_INTERPOLATION_MODE
#define M_DISP_HOOK_OFFSET                            M_HOOK_OFFSET
#define M_DISP_VGA_BUF_ID                             M_VGA_BUF_ID
#define M_DISP_OVR_WRITE                              M_WINDOW_OVR_WRITE
#define M_DISP_OVR_BUF_ID                             M_WINDOW_OVR_BUF_ID
#define M_DISP_BUF_WRITE                              M_WINDOW_BUF_WRITE
#define M_DISP_BUF_ID                                 M_WINDOW_BUF_ID
#define M_DISP_WINDOW_OVR_BUF_ID                      M_WINDOW_OVR_BUF_ID
#define M_DISP_WINDOW_OVR_WRITE                       M_WINDOW_OVR_WRITE
#define M_DISP_VGA_DISPLAY_ID                         M_WINDOW_OVR_DISP_ID
#define M_DISP_KEY_SUPPORTED                          M_KEY_SUPPORTED

#define M_DISP_WINDOW_ZOOM                            M_WINDOW_ZOOM
#define M_DISP_WINDOW_RESIZE                          M_WINDOW_RESIZE
#define M_DISP_WINDOW_OVERLAP                         M_WINDOW_OVERLAP
#define M_DISP_WINDOW_SCROLLBAR                       M_WINDOW_SCROLLBAR
#define M_DISP_WINDOW_UPDATE                          M_WINDOW_UPDATE
#define M_DISP_WINDOW_PROTECT_AREA                    M_WINDOW_PROTECT_AREA
#define M_DISP_WINDOW_TITLE_BAR                       M_WINDOW_TITLE_BAR
#define M_DISP_WINDOW_MENU_BAR                        M_WINDOW_MENU_BAR
#define M_DISP_WINDOW_TITLE_BAR_CHANGE                M_WINDOW_TITLE_BAR_CHANGE
#define M_DISP_WINDOW_MENU_BAR_CHANGE                 M_WINDOW_MENU_BAR_CHANGE
#define M_DISP_WINDOW_MOVE                            M_WINDOW_MOVE
#define M_DISP_WINDOW_SYSBUTTON                       M_WINDOW_SYSBUTTON
#define M_DISP_WINDOW_MINBUTTON                       M_WINDOW_MINBUTTON
#define M_DISP_WINDOW_MAXBUTTON                       M_WINDOW_MAXBUTTON
#define M_DISP_WINDOW_COLOR                           M_WINDOW_COLOR
#define M_DISP_WINDOW_COLOR_CHANGE                    M_WINDOW_COLOR_CHANGE
#define M_DISP_WINDOW_PALETTE_NOCOLLAPSE              M_WINDOW_PALETTE_NOCOLLAPSE
#define M_DISP_WINDOW_PALETTE_BACKGROUND              M_WINDOW_PALETTE_BACKGROUND
#define M_DISP_WINDOW_PALETTE_AUTO                    M_WINDOW_PALETTE_AUTO
#define M_DISP_WINDOW_ERASE_BACKGROUND                M_WINDOW_ERASE_BACKGROUND
#define M_DISP_WINDOW_AUTO_UPDATE                     M_WINDOW_UPDATE_AUTO_ON_CONTROL
#define M_DISP_WINDOW_UPDATE_WITH_MESSAGE             M_WINDOW_UPDATE_WITH_SEND_MESSAGE
#define M_DISP_WINDOW_SNAP_X                          M_WINDOW_SNAP_X
#define M_DISP_WINDOW_SNAP_Y                          M_WINDOW_SNAP_Y
#define M_DISP_WINDOW_UPDATE_REGION                   M_WINDOW_UPDATE_REGION
#define M_DISP_WINDOW_UPDATE_ONLY_INVALID_BORDER      M_WINDOW_UPDATE_ONLY_INVALID_BORDER
#define M_DISP_WINDOW_UPDATE_KEEP_PALETTE_ALIVE       M_WINDOW_UPDATE_KEEP_PALETTE_ALIVE
#define M_DISP_WINDOW_MANUAL_UPDATE                   M_WINDOW_UPDATE_MANUAL
#define M_DISP_WINDOW_PAINT                           M_WINDOW_PAINT
#define M_DISP_WINDOW_ACTIVATE_DELAY                  M_WINDOW_ACTIVATE_DELAY
#define M_DISP_WINDOW_CLIP_IN_CLIENT                  M_WINDOW_CLIP_IN_CLIENT
#define M_DISP_WINDOW_SYNC_SELECT                     M_WINDOW_SYNC_SELECT
#define M_DISP_WINDOW_INITIAL_POSITION_X              M_WINDOW_INITIAL_POSITION_X
#define M_DISP_WINDOW_INITIAL_POSITION_Y              M_WINDOW_INITIAL_POSITION_Y
#define M_DISP_WINDOW_BENCHMARK_IN_DEBUG              M_WINDOW_BENCHMARK_IN_DEBUG
#define M_DISP_WINDOW_RANGE                           M_WINDOW_RANGE
#define M_DISP_WINDOW_OVR_BUFFER_ALIVE                M_WINDOW_OVR_BUFFER_ALIVE
#define M_DISP_WINDOW_OVR_BUFFER_PTR                  M_WINDOW_OVR_BUFFER_PTR
#define M_DISP_WINDOW_OVR_FLICKER_FREE_ALIVE          M_WINDOW_OVR_FLICKER_FREE_ALIVE
#define M_DISP_WINDOW_OVR_FLICKER_FREE_PTR            M_WINDOW_OVR_FLICKER_FREE_PTR
#define M_DISP_WINDOW_OVR_DESTRUCTIVE                 M_WINDOW_OVR_DESTRUCTIVE
#define M_DISP_WINDOW_OVR_KEYER_PTR                   M_WINDOW_OVR_KEYER_PTR
#define M_DISP_WINDOW_MANUAL_OVR_ADD                  M_WINDOW_MANUAL_OVR_ADD
#define M_DISP_WINDOW_MANUAL_FLICKER_COPY             M_WINDOW_MANUAL_FLICKER_COPY
#define M_DISP_WINDOW_MANUAL_OVR_ADD_FLICKER_COPY     M_WINDOW_MANUAL_OVR_ADD_FLICKER_COPY
#define M_DISP_WINDOW_USE_SUBCLASS_TRACKING           M_WINDOW_USE_SUBCLASS_TRACKING
#define M_DISP_WINDOW_USE_SYSTEMHOOK_TRACKING         M_WINDOW_USE_SYSTEMHOOK_TRACKING
#define M_DISP_WINDOW_ATTRIBUTE_FOR_OVERLAY           M_WINDOW_ATTRIBUTE_FOR_OVERLAY
#define M_DISP_WINDOW_ATTRIBUTE_FOR_FLICKER           M_WINDOW_ATTRIBUTE_FOR_FLICKER
#define M_DISP_WINDOW_MASK_FOR_OVERLAY_VERIFICATION   M_WINDOW_MASK_FOR_OVERLAY_VERIFICATION
#define M_DISP_WINDOW_MASK_FOR_FLICKER_VERIFICATION   M_WINDOW_MASK_FOR_FLICKER_VERIFICATION
#define M_DISP_DESKTOP_CHANGE                         M_DESKTOP_CHANGE

#define M_DISP_WINDOW_HANDLE                          M_WINDOW_HANDLE
#define M_DISP_WINDOW_OFFSET_X                        M_WINDOW_OFFSET_X
#define M_DISP_WINDOW_OFFSET_Y                        M_WINDOW_OFFSET_Y
#define M_DISP_WINDOW_SIZE_X                          M_WINDOW_SIZE_X
#define M_DISP_WINDOW_SIZE_Y                          M_WINDOW_SIZE_Y
#define M_DISP_WINDOW_PAN_X                           M_WINDOW_PAN_X
#define M_DISP_WINDOW_PAN_Y                           M_WINDOW_PAN_Y
#define M_DISP_WINDOW_ZOOM_X                          M_WINDOW_ZOOM_X
#define M_DISP_WINDOW_ZOOM_Y                          M_WINDOW_ZOOM_Y
#define M_DISP_WINDOW_TITLE_NAME                      M_WINDOW_TITLE_NAME
#define M_DISP_HOOK_MODIFIED_DIB_PTR                  M_HOOK_MODIFIED_DIB_PTR
#define M_DISP_HOOK_MODIFIED_DIB_USER_PTR             M_HOOK_MODIFIED_DIB_USER_PTR
#define M_DISP_HOOK_MODIFIED_WINDOW_PTR               M_HOOK_MODIFIED_WINDOW_PTR
#define M_DISP_HOOK_MODIFIED_WINDOW_USER_PTR          M_HOOK_MODIFIED_WINDOW_USER_PTR
#define M_DISP_HOOK_MESSAGE_LOOP_PTR                  M_HOOK_MESSAGE_LOOP_PTR
#define M_DISP_HOOK_MESSAGE_LOOP_USER_PTR             M_HOOK_MESSAGE_LOOP_USER_PTR
#define M_DISP_WINDOW_APPFRAME_HANDLE                 M_WINDOW_APPFRAME_HANDLE
#define M_DISP_WINDOW_MDICLIENT_HANDLE                M_WINDOW_MDICLIENT_HANDLE
#define M_DISP_WINDOW_MDIFRAME_HANDLE                 M_WINDOW_MDIFRAME_HANDLE
#define M_DISP_VISIBLE_OFFSET_X                       M_WINDOW_VISIBLE_OFFSET_X
#define M_DISP_VISIBLE_OFFSET_Y                       M_WINDOW_VISIBLE_OFFSET_Y
#define M_DISP_VISIBLE_SIZE_X                         M_WINDOW_VISIBLE_SIZE_X
#define M_DISP_VISIBLE_SIZE_Y                         M_WINDOW_VISIBLE_SIZE_Y
#define M_VISIBLE_OFFSET_X                            M_WINDOW_VISIBLE_OFFSET_X
#define M_VISIBLE_OFFSET_Y                            M_WINDOW_VISIBLE_OFFSET_Y
#define M_VISIBLE_SIZE_X                              M_WINDOW_VISIBLE_SIZE_X
#define M_VISIBLE_SIZE_Y                              M_WINDOW_VISIBLE_SIZE_Y
#define M_DISP_WINDOW_DIB_HANDLE                      M_WINDOW_DIB_HANDLE
#define M_DISP_WINDOW_DISPLAY_DIB_HANDLE              M_WINDOW_DISPLAY_DIB_HANDLE
#define M_DISP_WINDOW_ACTIVE                          M_WINDOW_ACTIVE
#define M_DISP_WINDOW_ENABLE                          M_WINDOW_ENABLE
#define M_DISP_PALETTE_HANDLE                         M_PALETTE_HANDLE
#define M_DISP_WINDOW_THREAD_HANDLE                   M_WINDOW_THREAD_HANDLE
#define M_DISP_WINDOW_THREAD_ID                       M_WINDOW_THREAD_ID
#define M_DISP_WINDOW_DIB                             M_WINDOW_DIB
#define M_DISP_WINDOW_CLIP_LIST_SIZE                  M_WINDOW_CLIP_LIST_SIZE
#define M_DISP_WINDOW_CLIP_LIST                       M_WINDOW_CLIP_LIST
#define M_DISP_WINDOW_CLIP_LIST_ACCESS                M_WINDOW_CLIP_LIST_ACCESS

#define M_DISP_WINDOW_CHANGE_TITLE_BAR                M_DISP_WINDOW_TITLE_BAR_CHANGE
#define M_DISP_WINDOW_CHANGE_MENU_BAR                 M_DISP_WINDOW_MENU_BAR_CHANGE
#define M_DISP_WINDOW_CHANGE_COLOR                    M_DISP_WINDOW_COLOR_CHANGE
#define M_DISP_WINDOW_DO_PAINT                        M_DISP_WINDOW_PAINT
#define M_DISP_SELECT                                 M_SELECTED
#define M_AUTOMATIC                                   M_DEFAULT
#define M_WINDOW_DIB_HEADER                           M_BITMAPINFO
#define M_WINDOW_UPDATE_WITH_SEND_MESSAGE             M_UPDATE_WITH_PAINT_MESSAGE
#endif


/************************************************************************/
/* MdispControl() / MdispInquire() Values                               */
/************************************************************************/

#define M_FULL_SIZE                      0L
#define M_NORMAL_SIZE                    1L

#define M_BENCHMARK_IN_DEBUG_ON          0x00000001L
#define M_BENCHMARK_IN_DEBUG_OFF         0
#define M_BENCHMARK_IN_DEBUG_TRACE       0x00000002L
#define M_BENCHMARK_IN_DEBUG_NOTRACE     0
#define M_BENCHMARK_IN_DEBUG_ALLSIZE     0x00000004L
#define M_BENCHMARK_IN_DEBUG_CSTSIZE     0

#define M_DISPLAY_SCAN_LINE_START        0x00000000L
#define M_DISPLAY_SCAN_LINE_END          0xFFFFFFFFL

#define M_INFINITE                       0xFFFFFFFFL
#define M_SLAVE                          0L
#define M_MASTER                         1L

#define M_FILTER_LOW_PASS_TYPE_A         1L
#define M_FILTER_NOTCH                   2L
#define M_FILTER_EXTENDED                3L
#define M_FILTER_LOW_PASS_TYPE_B         4L
#define M_BYPASS                         5L
#define M_LOW_PASS_0                     M_FILTER_LOW_PASS_TYPE_A
#define M_LOW_PASS_1                     M_FILTER_LOW_PASS_TYPE_B
#define M_LOW_PASS_2                     M_FILTER_EXTENDED
#define M_NOTCH                          M_FILTER_NOTCH

#define M_TRANSPARENT_COLOR              0x01000060L

/************************************************************************/
/* MdispLut()                                                           */
/************************************************************************/
#define M_PSEUDO                                      (M_LUT_OFFSET + 8L)


/************************************************************************/
/* MdispHook()                                                          */
/************************************************************************/
#define M_ASYNC_HOOK                   0x08000000L

/* Defines for hook to modification to bitmap and window */
#define M_NOT_MODIFIED                0 /* No changed at all              */
#define M_MODIFIED_LUT                1 /* Disp lut is changed            */
#define M_MODIFIED_DIB                2 /* Disp buffer data is changed    */
#define M_MODIFIED_ZOOM               3 /* Disp is zoomed                 */
#define M_MODIFIED_PAN                4 /* Disp is panned                 */
#define M_MODIFIED_DIB_CREATION       5 /* Disp receives a new buffer ID  */
#define M_MODIFIED_DIB_DESTRUCTION    6 /* Disp receives a buffer ID 0    */
#define M_MODIFIED_WINDOW_CREATION    7 /* Wnd is created                 */
#define M_MODIFIED_WINDOW_DESTRUCTION 8 /* Wnd is destroyed               */
#define M_MODIFIED_WINDOW_LOCATION    9 /* Wnd size is changed            */
#define M_MODIFIED_WINDOW_OVERLAP    11 /* Wnd overlap is changed         */
#define M_MODIFIED_WINDOW_ICONIZED   12 /* Wnd is changed to iconic state */
#define M_MODIFIED_WINDOW_ZOOM       13 /* Wnd is zoomed                  */
#define M_MODIFIED_WINDOW_PAN        14 /* Wnd is panned                  */
#define M_MODIFIED_WINDOW_MENU       15 /* Wnd menu pulled-down           */
#define M_MODIFIED_WINDOW_PAINT      16 /* Wnd is painted with image      */
#define M_MODIFIED_WINDOW_ACTIVE     17 /* Wnd activation state changed   */
#define M_MODIFIED_WINDOW_ENABLE     18 /* Wnd enable state changed       */
#define M_MODIFIED_WINDOW_CLIP_LIST  19 /* Wnd clip list changed          */

/* M_MODIFIED_WINDOW_MENU modification hook defines */
#define M_MODIFIED_SYS_MENU            0x00100L
#define M_MODIFIED_APP_MENU            0x00200L
#define M_MODIFIED_USER_APP_MENU       0x10000L
#define M_MODIFIED_RESTORE_MENUITEM    0x00001L
#define M_MODIFIED_MOVE_MENUITEM       0x00002L
#define M_MODIFIED_SIZE_MENUITEM       0x00003L
#define M_MODIFIED_MINIMIZE_MENUITEM   0x00004L
#define M_MODIFIED_MAXIMIZE_MENUITEM   0x00005L
#define M_MODIFIED_CLOSE_MENUITEM      0x00006L
#define M_MODIFIED_TASKLIST_MENUITEM   0x00007L
#define M_MODIFIED_MENUBAR_MENUITEM    0x00008L
#define M_MODIFIED_TITLEOFF_MENUITEM   0x00009L
#define M_MODIFIED_ZOOMIN_MENUITEM     0x0000AL
#define M_MODIFIED_ZOOMOUT_MENUITEM    0x0000BL
#define M_MODIFIED_NOZOOM_MENUITEM     0x0000CL

/* M_MODIFIED_WINDOW_ACTIVE modification hook defines */
/* M_MODIFIED_WINDOW_ENABLE modification hook defines */
#define M_MODIFIED_STATE_FROM_WINDOW   0
#define M_MODIFIED_STATE_FROM_PARENT   0x00000010L
#define M_MODIFIED_OFF                 0
#define M_MODIFIED_ON                  0x00000001L

/* M_MODIFIED_WINDOW_CLIP_LIST modification hook defines */
/* M_MODIFIED_WINDOW_CLIP_LIST modification hook defines */
#define M_MODIFIED_ACCESS_RECTANGULAR_OFF 0
#define M_MODIFIED_ACCESS_RECTANGULAR_ON  0x00000001L
#define M_MODIFIED_ACCESS_OFF             0
#define M_MODIFIED_ACCESS_ON              0x00000002L
#define M_MODIFIED_ACCESS_COMMAND_PROMPT  0x00000004L

/* Type of event to hook for a display */
#define M_HOOK_AFTER             0x10000000L          /* For hook after modification      */
#define M_HOOK_BEFORE            0x20000000L          /* For hook before modification     */
#define M_HOOK_MODIFIED_DIB      1L                   /* For buffer bitmap modification   */
#define M_HOOK_MODIFIED_WINDOW   2L                   /* For disp window modification     */
#define M_HOOK_MESSAGE_LOOP      4L                   /* For disp window modification     */
#define M_FRAME_START            9L                   /* For disp frame start             */
#define M_PRE_SCHEME_CHANGE      10L                  /* For hook before the display scheme change (new event for MIL8.0) */
#define M_POST_SCHEME_CHANGE     11L                  /* For hook after the display scheme change (new event for MIL8.0)  */
#define M_BASIC_DISPLAY_EVENT    (12L | M_ASYNC_HOOK) /* For hook when disp window is displayed in another VGA,or between, or unsupported zoom or more (new MIL8) */
#define M_SURFACE_LOST           (14L | M_ASYNC_HOOK) /* For hook when DirectX surface is lost (new event for MIL8.0)     */
/************************************************************************/
/* MdispOverlayKey()                                                    */
/************************************************************************/
#define M_KEY_ON_COLOR                                1L
#define M_KEY_OFF                                     2L
#define M_KEY_ALWAYS                                  3L


/************************************************************************/
/* MdigAlloc() defines                                                  */
/************************************************************************/
#define M_DIGITIZER_COLOR                             0x00000001L
#define M_DIGITIZER_MONO                              0x00000002L
#define M_USE_MEMORY_DCF                              0x00000010L
#define M_DCF_REALLOC                                 0x00000020L
#define M_DISPLAY_DIGITIZER                           0x00000040L
#define M_NO_INPUT_PRESENT                            0x00000080L
#define M_NO_FIELD_CHECK                              0x00000100L
#define M_SHARED_BANDWIDTH                            0x00000200L
#define M_DIG_CREATE                                  0x00000400L
#define M_EXCLUSIVE_BANDWIDTH                         M_DEFAULT
#ifdef  M_MIL_75_COMPATIBLE                               //TBR MIL 8.0
#define M_FAST                                        0x00040000L
#else
#define M_FAST                                        0x00002000L
#endif
#define M_RESTORE_DIG_LUT                             0x00080000L

/************************************************************************/
/* MdigInquire() / MdigControl() Types                                  */
/************************************************************************/
#define M_NO_INPUT_PRESENT                            0x00000080L

#define M_OWNER_APPLICATION                           1000L
#define M_OWNER_SYSTEM                                1001L
#define M_SIZE_X                                      1002L
#define M_SIZE_Y                                      1003L
#define M_SIZE_Z                                      1004L
#define M_SIZE_BAND                                   1005L
#define M_SIZE_BAND_LUT                               1006L
#define M_SIZE_BIT                                    1007L
#define M_TYPE                                        1008L
#define M_NUMBER                                      1009L
#define M_FORMAT                                      1010L
#define M_FORMAT_SIZE                                 1011L
#define M_INIT_FLAG                                   1012L
#define M_ATTRIBUTE                                   1013L
#define M_SIGN                                        1014L
#define M_LUT_ID                                      1015L
#define M_NATIVE_ID                                   1016L
#define M_NATIVE_CONTROL_ID                           1017L
#define M_COLOR_MODE                                  1018L
#define M_THREAD_PRIORITY                             1019L
#define M_OWNER_SYSTEM_TYPE                           1023L
#define M_LOW_LEVEL_API_OBJECT                        1024L
#define M_SERIAL_NUMBER                               2098L
#define M_SERIAL_NUMBER_0                             2099L
#define M_SERIAL_NUMBER_1                             2100L

#define M_CHANNEL                                     4000L
/* Reserve next 1 bits                         from  (4000L | 0x00800000L)*/
#define M_CHANNEL_NUM                                 4001L
#define M_BASE_BLACK_REF                              4002L
#define M_BLACK_REF                                   4003L
/* Reserve next 8 bits                         from  (4003L | 0x00000000L)*/
/*                                                   (4003L | 0x10000000L)*/
/*                                                   (4003L | 0x20000000L)*/
/*                                                   (4003L | 0x40000000L)*/
/*                                                   (4003L | 0x80000000L)*/
/*                                                   (4003L | 0x01000000L)*/
/*                                                   (4003L | 0x02000000L)*/
/*                                                   (4003L | 0x04000000L)*/
/*                                             to    (4003L | 0x08000000L)*/
#define M_WHITE_REF                                   4005L
/* Reserve next 8 bits                         from  (4005L | 0x00000000L)*/
/*                                                   (4005L | 0x10000000L)*/
/*                                                   (4005L | 0x20000000L)*/
/*                                                   (4005L | 0x40000000L)*/
/*                                                   (4005L | 0x80000000L)*/
/*                                                   (4005L | 0x01000000L)*/
/*                                                   (4005L | 0x02000000L)*/
/*                                                   (4005L | 0x04000000L)*/
/*                                             to    (4005L | 0x08000000L)*/
#define M_HUE_REF                                     4006L
#define M_SATURATION_REF                              4007L
#define M_BRIGHTNESS_REF                              4008L
#define M_CONTRAST_REF                                4009L
#define M_GRAB_SCALE                                  4010L
#define M_GRAB_SCALE_X                                4011L
#define M_GRAB_SCALE_Y                                4012L
#define M_GRAB_SUBSAMPLE                              4013L
#define M_GRAB_SUBSAMPLE_X                            4014L
#define M_GRAB_SUBSAMPLE_Y                            4015L
#define M_GRAB_MODE                                   4016L
#define M_GRAB_FRAME_NUM                              4017L
#define M_GRAB_FIELD_NUM                              4018L
#define M_GRAB_INPUT_GAIN                             4019L    // dependent of M_LUMINANCE and M_CHROMINANCE
#define M_INPUT_MODE                                  4020L
#define M_SCAN_MODE                                   4021L
#define M_SOURCE_SIZE_X                               4022L
#define M_SOURCE_SIZE_Y                               4023L
#define M_SOURCE_OFFSET_X                             4024L
#define M_SOURCE_OFFSET_Y                             4025L
#define M_INTERNAL_SOURCE_SIZE_X                      4026L
#define M_INTERNAL_SOURCE_SIZE_Y                      4027L
#define M_INTERNAL_SOURCE_OFFSET_X                    4028L
#define M_INTERNAL_SOURCE_OFFSET_Y                    4029L
#define M_GRAB_END_HANDLER_PTR                        4030L
#define M_GRAB_END_HANDLER_USER_PTR                   4032L
#define M_GRAB_START_HANDLER_PTR                      4033L
#define M_GRAB_START_HANDLER_USER_PTR                 4035L
#define M_GRAB_FIELD_END_HANDLER_PTR                  4036L
#define M_GRAB_FIELD_END_HANDLER_USER_PTR             4037L
#define M_GRAB_FIELD_END_ODD_HANDLER_PTR              4038L
#define M_GRAB_FIELD_END_ODD_HANDLER_USER_PTR         4039L
#define M_GRAB_FIELD_END_EVEN_HANDLER_PTR             4040L
#define M_GRAB_FIELD_END_EVEN_HANDLER_USER_PTR        4041L
#define M_GRAB_FRAME_END_HANDLER_PTR                  4042L
#define M_GRAB_FRAME_END_HANDLER_USER_PTR             4043L
#define M_GRAB_FRAME_START_HANDLER_PTR                4044L
#define M_GRAB_FRAME_START_HANDLER_USER_PTR           4045L
#define M_FIELD_START_HANDLER_PTR                     4046L
#define M_FIELD_START_HANDLER_USER_PTR                4047L
#define M_FIELD_START_ODD_HANDLER_PTR                 4048L
#define M_FIELD_START_ODD_HANDLER_USER_PTR            4049L
#define M_FIELD_START_EVEN_HANDLER_PTR                4050L
#define M_FIELD_START_EVEN_HANDLER_USER_PTR           4051L
#define M_SCALING_Y_AVAILABLE                         4052L
#define M_GRAB_TRIGGER_SOURCE                         4053L
#define M_GRAB_TRIGGER_MODE                           4054L
#define M_NATIVE_CAMERA_ID                            4060L
#define M_VCR_INPUT_TYPE                              4061L
#define M_CLIP_SRC_SUPPORTED                          4062L
#define M_CLIP_DST_SUPPORTED                          4063L
#define M_HOOK_FUNCTION_SUPPORTED                     4064L
#define M_GRAB_WINDOW_RANGE_SUPPORTED                 4065L
#define M_GRAB_SCALE_X_SUPPORTED                      4066L
#define M_GRAB_SCALE_Y_SUPPORTED                      4067L
#define M_GRAB_8_BITS_SUPPORTED                       4068L
#define M_GRAB_15_BITS_SUPPORTED                      4069L
#define M_GRAB_32_BITS_SUPPORTED                      4070L
#define M_GRAB_EXTRA_LINE                             4071L
#define M_GRAB_ABORT                                  4072L
#define M_GRAB_DESTRUCTIVE_IN_PROGRESS                4073L
#define M_GRAB_START_MODE                             4074L
#define M_GRAB_WINDOW_RANGE                           4075L
#define M_INPUT_SIGNAL_PRESENT                        4078L
#define M_INPUT_SIGNAL_SOURCE                         4079L
#define M_FIELD_START_THREAD_ID                       4080L
#define M_GRAB_FIELD_END_ODD_THREAD_ID                4081L
#define M_GRAB_FIELD_END_EVEN_THREAD_ID               4082L
#define M_FIELD_START_THREAD_HANDLE                   4083L
#define M_GRAB_FIELD_END_ODD_THREAD_HANDLE            4084L
#define M_GRAB_FIELD_END_EVEN_THREAD_HANDLE           4085L
#define M_FORMAT_UPDATE                               4086L
#define M_USER_BIT                                    4087L
/* Reserve next 31 values                      from   4088L*/
/*                                             to     4118L*/
#define M_GRAB_FAIL_CHECK                             4120L
#define M_GRAB_FAIL_STATUS                            4121L
#define M_GRAB_FAIL_RETRY_NUMBER                      4122L
#define M_GRAB_ON_ONE_LINE                            4123L
#define M_GRAB_WRITE_FORMAT                           4124L
#define M_GRAB_LUT_PALETTE                            4125L
#define M_GRAB_HALT_ON_NEXT_FIELD                     4126L
#define M_GRAB_TIMEOUT                                4127L
#define M_GRAB_IN_PROGRESS                            4128L
#define M_FIELD_START_HOOK_WHEN_GRAB_ONLY             4129L
#define M_SOUND_VOLUME_REF                            4130L
#define M_SOUND_VOLUME_RIGHT_REF                      4131L
#define M_SOUND_VOLUME_LEFT_REF                       4132L
#define M_SOUND_TYPE_REF                              4133L
#define M_SOUND_TYPE_STATUS                           4134L
#define M_SOUND_BASS_REF                              4135L
#define M_SOUND_TREBLE_REF                            4136L
#define M_EXTERNAL_CHROMINANCE                        4137L
#define M_TUNER_FREQUENCY                             4138L
#define M_TUNER_STANDARD                              4139L
#define M_CLOCK_NOT_ALWAYS_VALID                      4140L
#define M_GRAB_LINESCAN_MODE                          4141L
#define M_GRAB_PERIOD                                 4142L
#define M_OVERRIDE_ROUTER                             4143L
#define M_GRAB_EXPOSURE                               4150L
/* Reserve next 8 values                       from   4151L*/
/*                                             to     4158L*/
#define M_GRAB_EXPOSURE_SOURCE                        4160L
/* Reserve next 8 values                       from   4161L*/
/*                                             to     4168L*/
#define M_GRAB_EXPOSURE_MODE                          4170L
/* Reserve next 8 values                       from   4171L*/
/*                                             to     4178L*/
#define M_GRAB_EXPOSURE_TIME                          4180L
/* Reserve next 8 values                       from   4181L*/
/*                                             to     4188L*/
#define M_GRAB_EXPOSURE_TIME_DELAY                    4190L
/* Reserve next 8 values                       from   4191L*/
/*                                             to     4198L*/
#define M_GRAB_TRIGGER                                4200L
/* Reserve next 8 values                       from   4201L*/
/*                                             to     4208L*/
#define M_GRAB_EXPOSURE_BYPASS                        4210L
#define M_DCF_REALLOC_HANDLER_PTR                     4211L
#define M_DCF_REALLOC_HANDLER_USER_PTR                4212L
#define M_USER_IN_FORMAT                              4213L
#define M_USER_OUT_FORMAT                             4214L
#define M_GRAB_RESTRICTION_CHECK                      4215L
#define M_LAST_GRAB_BUFFER                            4216L
#define M_NATIVE_LAST_GRAB_OSB_ID                     4217L
#define M_SYNCHRONIZE_ON_STARTED                      4218L
#define M_GRAB_WAIT                                   4219L
#define M_GRAB_FIELD_START_HANDLER_PTR                4220L
#define M_GRAB_FIELD_START_HANDLER_USER_PTR           4221L
#define M_GRAB_FIELD_START_ODD_HANDLER_PTR            4222L
#define M_GRAB_FIELD_START_ODD_HANDLER_USER_PTR       4223L
#define M_GRAB_FIELD_START_EVEN_HANDLER_PTR           4224L
#define M_GRAB_FIELD_START_EVEN_HANDLER_USER_PTR      4225L
#define M_GRAB_16_BITS_SUPPORTED                      4226L
#define M_GRAB_24_BITS_SUPPORTED                      4227L
#define M_INPUT_SIGNAL_HSYNC_LOCK                     4228L
#define M_GRAB_BLOCK_SIZE                             4229L
#define M_GRAB_DIRECTION_X                            4230L
#define M_GRAB_DIRECTION_Y                            4231L
#define M_GRAB_FREQ_MAX                               4232L
#define M_GRAB_THREAD_HANDLE                          4233L
#define M_GRAB_THREAD_ID                              4234L
#define M_GRAB_START_THREAD_HANDLE                    4235L
#define M_GRAB_START_THREAD_ID                        4236L
#define M_GRAB_END_THREAD_HANDLE                      4237L
#define M_GRAB_END_THREAD_ID                          4238L
#define M_GRAB_PATH_OVERRIDE                          4239L
#define M_GRAB_PATH_OVERRIDE_DCF                      4240L
#define M_GRAB_PATH_PCI                               4241L
#define M_GRAB_AUTOMATIC_INPUT_GAIN                   4242L
#define M_GRAB_DISPLAY_SCALER_QUALITY                 4243L
#define M_GRAB_VALID                                  4244L
#define M_GRAB_FIELD_START_THREAD_HANDLE              4248L
#define M_GRAB_FIELD_START_THREAD_ID                  4249L
#define M_GRAB_EXPOSURE_TRIGGER_MODE                  4250L
/* Reserve next 8 values                       from   4251L*/
/*                                             to     4258L*/
#define M_GRAB_FIELD_END_THREAD_HANDLE                4259L
#define M_GRAB_FIELD_END_THREAD_ID                    4260L
#define M_GRAB_FRAME_END_THREAD_HANDLE                4261L
#define M_GRAB_FRAME_END_THREAD_ID                    4262L
#define M_SET_VCOUNT                                  4263L
#define M_JPEG_ENCODING_MODE                          4264L
#define M_JPEG_DECODING_MODE                          4265L
#define M_JPEG_MARKERS_ENABLE                         4266L
#define M_JPEG_NUMBER_OF_SCAN_ENCODED                 4267L
#define M_JPEG_INPUT_COLOR_MODE                       4268L
#define M_JPEG_OUTPUT_COLOR_MODE                      4269L
#define M_JPEG_IMAGE_FORMAT                           4270L
#define M_JPEG_NAX                                    4271L
#define M_JPEG_NAY                                    4272L
#define M_JPEG_PAX                                    4273L
#define M_JPEG_PAY                                    4274L
#define M_JPEG_AUTO_START_MODE                        4275L
#define M_JPEG_COMPRESS_MODE                          4276L
#define M_JPEG_HW_PROG_MODE                           4277L
#define M_HOOK_MASTER_THREAD_HANDLE                   4278L
#define M_HOOK_MASTER_THREAD_ID                       4279L
#define M_GRAB_PATH_RR                                4280L
#define M_PIXCLK_JPEG_SIGNAL_PRESENT                  4281L
#define M_GRAB_VM                                     4282L
#define M_GARBAGE_UV_ADDRESS                          4283L
#define M_SET_EXPOSURE_ENABLE                         4290L
/* Reserve next 8 values                       from   4291L*/
/*                                             to     4298L*/
#define M_INPUT_SIGNAL_COLOR_LOCK                     4399L

#define M_JPEG_FIRST_FIELD_SIZEBYTE                   4301L
#define M_JPEG_SECOND_FIELD_SIZEBYTE                  4302L
#define M_JPEG_OFFSET_X                               4303L
#define M_JPEG_OFFSET_Y                               4304L
#define M_CURRENT_GRAB_BUFFER_HOST_ADDRESS            4305L
#define M_CURRENT_GRAB_BUFFER_PHYSICAL_ADDRESS        4306L
#define M_RESET_CONTINUOUS_GRABBER                    4307L
#define M_TIMER_INTERRUPT                             4308L
#define M_TIMER_INTERRUPT_LENGHT                      4309L
#define M_TIMER_SET_INTERRUPT                         4310L
#define M_GRAB_SAMPLING_POSITION                      4311L
#define M_JPEG_DMA_TRANSFER                           4312L
#define M_GRAB_FRAME_START_THREAD_HANDLE              4313L
#define M_GRAB_FRAME_START_THREAD_ID                  4314L
#define M_INTERNAL_GRAB_BUFFERS_NB                    4315L
#define M_INTERNAL_GRAB_BUFFERS_ID                    4316L
#define M_INTERNAL_GRAB_BUFFERS_FORMAT                4317L
#define M_KS0127_REV_ID                               4318L
#define M_DECODER_REV_ID                              M_KS0127_REV_ID
#define M_GRAB_SCALE_MODE                             4319L
#define M_GRAB_SCALE_MODE_X                           4320L
#define M_GRAB_SCALE_MODE_Y                           4321L
#define M_START_THREAD_HANDLE                         4322L
#define M_START_THREAD_ID                             4323L
#define M_SOURCE_COMPENSATION                         4324L
#define M_GRAB_COLOR                                  4325L
#define M_SYNCHRONIZE_CHANNEL                         4326L
#define M_ODD_EVEN_CORRECTION                         4327L
#define M_POLLING_FOR_END_OF_XFER                     4328L
#define M_FORCE_PSEUDO_LIVE_GRAB                      4329L
/* Free to use                                 from   4330*/
/*                                             to     4339*/
#define M_GRAB_EXPOSURE_CLOCK_SOURCE                  4340L
/* Reserve next 8 values                       from   4341L*/
/*                                             to     4348L*/
#define M_GRAB_EXPOSURE_CLOCK_FREQUENCY               4350L
/* Reserve next 8 values                       from   4351L*/
/*                                             to     4358L*/
#define M_HOOK_FOR_PSEUDO_LIVE_GRAB                   4360L
#define M_YIELD_FOR_END_OF_XFER                       4361L
#define M_GRAB_SCALE_QUALITY                          4362L
#define M_USER_BIT_INQUIRE_START                      4363L
#define M_USER_BIT                                    4087L
/* Reserve next 31 values                      from   4088L*/
/*                                             to     4118L*/
#define M_USER_BIT_COUNT                              4364L
#define M_USER_BIT_OPTOMODULE                         4365L
#define M_USER_BIT_INQUIRE_END                        4366L

#define M_GRAB_LINE_HANDLER_PTR                       4367L
#define M_GRAB_LINE_HANDLER_USER_PTR                  4368L
#define M_GRAB_QUEUE_SIZE                             4369L
#define M_SECONDARY_GRAB_BUFFER                       4370L
#define M_GRAB_FRAME_END_TIME_STAMP_BUFFER            4371L
#define M_GRAB_BUF_MODIFIED                           4372L
#define M_RGB_PIXEL_SWAP                              4373L
#define M_PREFERRED_BUFFER_FORMAT                     4374L
#define M_GRAB_IN_USER_BUFFER                         4375L
#define M_SYNCHRONIZE_GRAB_WITH_DISPLAY_REFRESH       4376L
#define M_LAST_GRAB_BUFFER_INDEX                      4377L



// Map of UART defines
#define M_UART_PARITY                                 5001L
#define M_UART_STOP_BITS                              5002L
#define M_UART_DATA_LENGTH                            5003L
#define M_UART_SPEED                                  5004L
#define M_UART_WRITE_CHAR                             5005L
#define M_UART_READ_CHAR                              5006L
#define M_UART_DATA_PENDING                           5007L
#define M_UART_WRITE_STRING                           5008L
#define M_UART_WRITE_STRING_LENGTH                    5009L
#define M_UART_READ_STRING                            5010L
#define M_UART_READ_STRING_LENGTH                     5011L
#define M_UART_READ_STRING_MAXIMUM_LENGTH             5012L
#define M_UART_STRING_DELIMITER                       5013L
#define M_UART_TIMEOUT                                5014L
#define M_UART_ERROR_MESSAGE                          5015L // internal use only
#define M_UART_OUTPUT                                 5016L

#define M_UART_THREAD_HANDLE                          5017L
#define M_UART_THREAD_ID                              5018L
#define M_UART_DATA_RECEIVED_HANDLER_PTR              5019L
#define M_UART_DATA_RECEIVED_HANDLER_USER_PTR         5020L

/* defines for 1394*/
#define M_MIN_VALUE                                      2L
#define M_MAX_VALUE                                      1L

#define M_BRIGHTNESS                                  5100L/* Reserve all values between 5100 and 5159*/
#define M_AUTO_EXPOSURE                               5104L
#define M_SHARPNESS                                   5108L
#define M_WHITE_BALANCE                               5112L
#define M_HUE                                   0x00008000L
#define M_SATURATION                                M_GREEN
#define M_GAMMA                                       5116L
#define M_SHUTTER                                     5120L
#define M_GAIN                                        5124L
#define M_IRIS                                        5128L
#define M_FOCUS                                       5132L
#define M_TEMPERATURE                                 5136L
#define M_TRIGGER                                     5140L
#define M_ZOOM                                        5144L
#define M_PAN                                         5148L
#define M_TILT                                        5152L
#define M_OPTICAL_FILTER                              5156L
#define M_CAPTURE_SIZE                                5160L
#define M_CAPTURE_QUALITY                             5164L
#define M_WHITE_BALANCE_U                             5168L
#define M_WHITE_BALANCE_V                             5172L
#define M_TARGET_TEMPERATURE                          5176L
/* Reserve next 3 values                       from   5177L*/
/*                                             to     5179L*/
#define M_FORMAT_SUPPORTED                            5180L
#define M_FORMAT_SUPPORTED_NUM                        5181L
#define M_FORMAT_SUPPORTED_LENGTH                     5182L
#define M_TRIGGER_SUPPORTED                           5183L
#define M_INPUT_LOW_PASS_FILTER                       5184L
#define M_PIXCLK_FREQ                                 5185L
#define M_INPUT_FILTER                                5186L
#define M_DIGITIZER_CONTROL_LOG                       5187L
#define M_TARGET_BUFFER_UPDATE                        5188L
#define M_DUMP_REGISTERS                              5189L

/* Reserve 3 values for M_SATURATION           from   8192*/
/*                                             to     8195*/

/* defines for camera link*/
#define M_CAMERALINK_CC1_SOURCE                       5200L
#define M_CAMERALINK_CC2_SOURCE                       5201L
#define M_CAMERALINK_CC3_SOURCE                       5202L
#define M_CAMERALINK_CC4_SOURCE                       5203L

/* defines for channel switch lock mechanism*/
#define M_CAMERA_LOCK                                 5300L
#define M_CAMERA_COLOR_LOCK                           5301L
#define M_CAMERA_LOCK_SENSITIVITY                     5302L
#define M_CAMERA_UNLOCK_SENSITIVITY                   5303L
#define M_CAMERA_LOCKED                               5304L
#define M_CAMERA_COLOR_LOCKED                         5305L
#define M_CAMERA_LOCK_MODE                            5306L

#define M_CAMERA_LOCK_HANDLER_PTR                     5307L
#define M_CAMERA_LOCK_HANDLER_USER_PTR                5308L
#define M_CAMERA_LOCK_THREAD_ID                       5309L
#define M_CAMERA_LOCK_THREAD_HANDLE                   5310L

/* defines for camera detect */
#define M_CAMERA_PRESENT_HANDLER_PTR                  5311L
#define M_CAMERA_PRESENT_HANDLER_USER_PTR             5312L
#define M_CAMERA_PRESENT_THREAD_ID                    5313L
#define M_CAMERA_PRESENT_THREAD_HANDLE                5314L
#define M_CAMERA_PRESENT                              5315L
#define M_CAMERA_PRESENT_SENSITIVITY                  5316L

#define M_WRITE_GRAB_VALIDATION_TAG                   5317L

#define M_INFO_FORMAT                                 5318L
#define M_HV_RESET                                    5319L
#define M_FORCE_INTERNAL_GRAB_BUFFER                  5320L
#define M_INITIALIZATION_DELAY                        5321L
#define M_SELECTED_FRAME_RATE                         5322L
#define M_VENDOR                                      5323L
#define M_VENDOR_LENGTH                               5324L
#define M_MODEL_LENGTH                                5325L
#define M_BAYER_CONVERSION                            5326L
#define M_GRAB_PROCESSING                             5327L
#define M_BAYER_COEFFICIENTS_ID                       5328L
#define M_ACQUISITION_PATH                            5329L

#define M_GRAB_LL_SEQ_CONTEXT                         5330L
/* Reserve next value                                 5331L*/

/* Reserve next value                                 5332 */


#define M_HOOK_EVENT_BACKDOOR_INTERNAL                0x4444L // 17476
#define M_HOOK_CANCEL_EVENT_BACKDOOR_INTERNAL         0x4464L // 17508

/* Reserve 3 values for M_HUE                  from   32768*/
/*                                             to     32771*/
#define M_MODEL                                      0X10000L   //65536


#define M_START_REGISTER_ADDRESS                   0xF0000000
/*Reserve the values between 0xF000000 and 0xF0FFFFFF for 1394*/
#define M_END_REGISTER_ADDRESS                     0xF0FFFFFF

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_DIG_TYPE                                    M_TYPE
#define M_DIG_NUMBER                                  M_NUMBER
#define M_DIG_FORMAT                                  M_FORMAT
#define M_DIG_INIT_FLAG                               M_INIT_FLAG
#define M_DIG_CHANNEL_NUM                             M_CHANNEL_NUM
#define M_DIG_LUT                                     M_LUT_ID
#define M_DIG_REF_BLACK                               M_BLACK_REF
#define M_DIG_REF_WHITE                               M_WHITE_REF
#define M_DIG_REF_HUE                                 M_HUE_REF
#define M_DIG_REF_SATURATION                          M_SATURATION_REF
#define M_DIG_REF_BRIGHTNESS                          M_BRIGHTNESS_REF
#define M_DIG_REF_CONTRAST                            M_CONTRAST_REF
#define M_DIG_BLACK_REF                               M_BLACK_REF
#define M_DIG_WHITE_REF                               M_WHITE_REF
#define M_DIG_HUE_REF                                 M_HUE_REF
#define M_DIG_SATURATION_REF                          M_SATURATION_REF
#define M_DIG_BRIGHTNESS_REF                          M_BRIGHTNESS_REF
#define M_DIG_CONTRAST_REF                            M_CONTRAST_REF
#define M_DIG_INPUT_MODE                              M_INPUT_MODE
#define M_DIG_GRAB_SCALE                              M_GRAB_SCALE
#define M_DIG_GRAB_SCALE_X                            M_GRAB_SCALE_X
#define M_DIG_GRAB_SCALE_Y                            M_GRAB_SCALE_Y
#define M_DIG_GRAB_SUBSAMPLE                          M_GRAB_SUBSAMPLE
#define M_DIG_GRAB_SUBSAMPLE_X                        M_GRAB_SUBSAMPLE_X
#define M_DIG_GRAB_SUBSAMPLE_Y                        M_GRAB_SUBSAMPLE_Y
#define M_DIG_GRAB_MODE                               M_GRAB_MODE
#define M_DIG_GRAB_FRAME_NUM                          M_GRAB_FRAME_NUM
#define M_DIG_GRAB_FIELD_NUM                          M_GRAB_FIELD_NUM
#define M_DIG_SOURCE_SIZE_X                           M_SOURCE_SIZE_X
#define M_DIG_SOURCE_SIZE_Y                           M_SOURCE_SIZE_Y
#define M_DIG_SOURCE_OFFSET_X                         M_SOURCE_OFFSET_X
#define M_DIG_SOURCE_OFFSET_Y                         M_SOURCE_OFFSET_Y
#define M_DIG_USER_BIT                                M_USER_BIT
#define M_DIG_INPUT_SIGNAL_PRESENT                    M_INPUT_SIGNAL_PRESENT
#define M_DIG_INPUT_SIGNAL_SOURCE                     M_INPUT_SIGNAL_SOURCE
#define M_DIG_SOUND_VOLUME_REF                        M_SOUND_VOLUME_REF
#define M_DIG_SOUND_VOLUME_RIGHT_REF                  M_SOUND_VOLUME_RIGHT_REF
#define M_DIG_SOUND_VOLUME_LEFT_REF                   M_SOUND_VOLUME_LEFT_REF
#define M_DIG_SOUND_TYPE_REF                          M_SOUND_TYPE_REF
#define M_DIG_SOUND_BASS_REF                          M_SOUND_BASS_REF
#define M_DIG_SOUND_TREBLE_REF                        M_SOUND_TREBLE_REF
#define M_DIG_FORMAT_UPDATE                           M_FORMAT_UPDATE
#define M_DIG_CLIP_SRC_SUPPORTED                      M_CLIP_SRC_SUPPORTED
#define M_DIG_CLIP_DST_SUPPORTED                      M_CLIP_DST_SUPPORTED
#define M_DIG_HOOK_FUNCTION_SUPPORTED                 M_HOOK_FUNCTION_SUPPORTED
#define M_GRAB_INTERLACED_MODE                        M_SCAN_MODE
#define M_GRAB_THREAD_PRIORITY                        M_THREAD_PRIORITY
#define M_HOOK_PRIORITY                               M_THREAD_PRIORITY
#define M_GRAB_WINDOWS_RANGE                          M_GRAB_WINDOW_RANGE
#define M_GRAB_WINDOWS_RANGE_SUPPORTED                M_GRAB_WINDOW_RANGE_SUPPORTED

#define M_HW_TRIGGER                                  M_HARDWARE_PORT0
#define M_SW_TRIGGER                                  M_SOFTWARE
#define M_VSYNC_TRIGGER                               M_VSYNC
#define M_HSYNC_TRIGGER                               M_HSYNC
#define M_DIG_CHANNEL                                 M_CHANNEL

#endif

// List of defines for CLLjpegControl
//==============================================================================
#define M_LOSSY                                          1L
#define M_AUTO_BIT_RATE_CONTROL                          2L
#define M_STATISTICAL                                    3L
#define M_BIT_RATE_CONTROL                               4L
#define M_TABLES_ONLY                                    5L
#define M_TABLES_PRELOAD                                 6L
#define M_LOSSLESS                                       7L
#define M_FAST_PREVIEW                                   8L

#define M_APP                                         0x00000001L
#define M_COM                                         0x00000002L
#define M_DRI                                         0x00000004L
#define M_DQT                                         0x00000008L
#define M_DHT                                         0x00000010L
#define M_DNL                                         0x00000020L
#define M_DQTI                                        0x00000040L
#define M_DHTI                                        0x00000080L

#define M_NSCN_1                                         0L
#define M_NSCN_2                                         1L
#define M_NSCN_3                                         2L
#define M_NSCN_4                                         3L
#define M_NSCN_5                                         4L
#define M_NSCN_6                                         5L
#define M_NSCN_7                                         6L
#define M_NSCN_8                                         7L

/* Inquire Values */
#define M_DIGITAL                                     0L
#define M_ANALOG                                      1L
#define M_INTERLACE                                   0L
#define M_PROGRESSIVE                                 1L
#define M_LINESCAN                                    2L
#define M_ACTIVATE                                    1L

/* Inquire Values for the color mode */
#define M_MONOCHROME                                  0L
#define M_COMPOSITE                                   1L
#define M_RGB                                         8L
#define M_YUV                                         4L
#define M_EXTERNAL_CHROMINANCE                        4137L
#define M_MONO8_VIA_RGB                               M_MONO32
#define M_SEPARATE                                    0x8000L

/************************************************************************/
/* Board types                                                          */
/************************************************************************/
#define M_BOARD_TYPE_MASK                             0x000000FFL

// Pulsar board type
#define M_PULSAR_XXX                                  19L
#define M_PULSAR                                      20L
#define M_PULSAR_WITH_RS422                           21L
#define M_PULSAR_RS422_JIG                            22L

// Module type
#define M_DIGITAL_MODULE                              0x0100L
#define M_LVDS_MODULE                                 M_RS422_MODULE
#define M_COMPRESSION_MODULE                          0x0200L
#define M_DAC_MODULE                                  0x0400L
#define M_XCL                                         0x0800L
#define M_XA                                          0x1000L
#define M_XPRO                                        0x2000L
#define M_ACQUISITION_MODULE                          0x4000L
#define M_AUXIO_MODULE                                0x8000L

// Grab Module type
#define M_QHA                                         0x00100000L
#define M_SFCL                                        0x00200000L
#define M_DBCL                                        0x00400000L
#define M_QD                                          0x00800000L
#define M_DMA                                         0x00010000L


// Corona board type
#define M_CORONA                                      20L
#define M_CORONA_LC                                   21L
#define M_CORONA_VIA                                  22L
#define M_CORONA_RR                                   23L
#define M_METEOR_II_MC                                24L
#define M_CORONA_II                                   25L
#define M_METEOR_II                                   27L

#define M_METEOR_II_STD                               M_METEOR_II
#define M_CORONA_WITH_DIG_MODULE                      (M_CORONA|M_DIGITAL_MODULE)
#define M_CORONA_II_WITH_DIG_MODULE                   (M_CORONA_II|M_DIGITAL_MODULE)
#define M_METEOR_II_WITH_COMPRESSION_MODULE           (M_METEOR_II|M_COMPRESSION_MODULE)
#define M_METEOR_II_STD_WITH_COMPRESSION_MODULE       (M_METEOR_II_STD|M_COMPRESSION_MODULE)
#define M_METEOR_II_MC_WITH_COMPRESSION_MODULE        (M_METEOR_II_MC|M_COMPRESSION_MODULE)

// Meteor board type
#define M_METEOR                                      20L
#define M_METEOR_TV                                   21L
#define M_METEOR_RGB                                  22L
#define M_METEOR_PRO                                  23L
#define M_METEOR_TV_PRO                               24L
#define M_METEOR_RGB_PRO                              25L

// Genesis board type
#define M_GENESIS                                     30L
#define M_GENESIS_PRO                                 31L
#define M_GENESIS_LC                                  32L
#define M_METEOR_II_DIG                               33L
#define M_GENESIS_PLUS                                34L
#define M_METEOR_II_CL                                35L
#define M_METEOR_II_DIG_WITH_MJPEG_MODULE             (M_METEOR_II_DIG|M_COMPRESSION_MODULE)
#define M_GENESIS_WITH_DAC_MODULE                     (M_GENESIS|M_DAC_MODULE)
#define M_GENESIS_LC_WITH_DAC_MODULE                  (M_GENESIS_LC|M_DAC_MODULE)
#define M_ODYSSEY                                     36L
#define M_ODYSSEY_NATIVE                              37L

// MJPEG/JPEG module board type (internal use only)
#define M_MJPEG                                       1L
#define M_JPEG                                        5L
#define M_DEVICE_MJPEG                                M_MJPEG // with interlaced/progressive compression/decompression
#define M_DEVICE_JPEG                                 M_JPEG // with interlaced/progressive compression/decompression
                                                             // and progressive compression from interlaced timings
// Orion board type
#define M_ORION                                       40L
#define M_ORION_4SIGHT_II_STD                         41L
#define M_ORION_4SIGHT_II_RGB                         42L
#define M_ORION_4SIGHT_II_DUAL                        43L

// 1394 board type
#define M_METEOR_II_1394                              50L
#define M_NON_MATROX_OHCI_BOARD                       51L

//Cronos board type
#define M_CRONOS                                      60L

//Cronos Plusboard type
#define M_CRONOSPLUS                                 70L

//Helios board type
#define M_HELIOS                                      80L

//Iris board type
#define M_IRIS                                        5128L


// Morphis Board type
#define M_MORPHIS                                     90L

//Host board type
#define M_HOST                                        100L

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_MJPEG_MODULE                                M_COMPRESSION_MODULE
#define M_RS422_MODULE                                M_DIGITAL_MODULE
#define M_METEOR_II_WITH_MJPEG_MODULE                 M_METEOR_II_WITH_COMPRESSION_MODULE
#define M_METEOR_II_STD_WITH_MJPEG_MODULE             M_METEOR_II_STD_WITH_COMPRESSION_MODULE
#define M_METEOR_II_MC_WITH_MJPEG_MODULE              M_METEOR_II_MC_WITH_COMPRESSION_MODULE
#endif

/************************************************************************/
/* MdigControl() / MdigInquire() Values                                 */
/************************************************************************/
#define M_WEIGHTED                                    2L
#define M_CONTINUOUS                                 -1L

#define M_TIMER1                                      1L
#define M_TIMER2                                      2L
#define M_TIMER3                                      3L
#define M_TIMER4                                      4L
#define M_TIMER5                                      5L
#define M_TIMER6                                      6L
#define M_TIMER7                                      7L
#define M_TIMER8                                      8L

#define M_LOW                                         1L
#define M_HIGH                                        3L

#define M_ARM_CONTINUOUS                              9L
#define M_ARM_MONOSHOT                                10L
#define M_ARM_RESET                                   11L
#define M_EDGE_RISING                                 12L
#define M_EDGE_FALLING                                13L
#define M_LEVEL_LOW                                   14L
#define M_LEVEL_HIGH                                  15L
#define M_HARDWARE_PORT0                              16L
#define M_HARDWARE_PORT1                              17L
#define M_HARDWARE_PORT_CAMERA                        18L
#define M_START_EXPOSURE                              19L
#define M_SOFTWARE                                    20L
#define M_USER_DEFINED                                21L
#define M_HSYNC                                       22L
#define M_VSYNC                                       23L
#define M_HARDWARE_PORT2                              24L
#define M_HARDWARE_PORT3                              25L
#define M_SYSCLK                                      26L
#define M_PIXCLK                                      27L
#define M_URGENT                                      28L
#define M_NOW                                         29L
#define M_FRAME                                       30L
#define M_HARDWARE_PORT4                              31L
#define M_HARDWARE_PORT5                              32L
#define M_HARDWARE_PORT6                              33L
#define M_HARDWARE_PORT7                              34L
#define M_HARDWARE_PORT8                              35L
#define M_HARDWARE_PORT9                              36L
#define M_HARDWARE_PORT10                             37L
#define M_HARDWARE_PORT11                             38L
#define M_HARDWARE_PORT12                             39L
#define M_HARDWARE_PORT13                             40L
#define M_HARDWARE_PORT14                             41L
#define M_HARDWARE_PORT15                             42L

#define M_FILL_DESTINATION                            -1
#define M_FILL_DISPLAY                                -2
#define M_SYNCHRONOUS                                 1L
#define M_ASYNCHRONOUS                                2L
#define M_ASYNCHRONOUS_QUEUED                         3L

#define M_LUT_PALETTE0                                0L
#define M_LUT_PALETTE1                                1L
#define M_LUT_PALETTE2                                2L
#define M_LUT_PALETTE3                                3L
#define M_LUT_PALETTE4                                4L
#define M_LUT_PALETTE5                                5L
#define M_LUT_PALETTE6                                6L
#define M_LUT_PALETTE7                                7L

#define M_GAIN0                                       10L
#define M_GAIN1                                       11L
#define M_GAIN2                                       12L
#define M_GAIN3                                       13L
#define M_GAIN4                                       14L

#define M_TTL                                         1L
#define M_RS422                                       2L
#define M_LVDS                                        M_RS422

#define M_ENABLE                                      -9997L
#define M_FINAL_GRAB                                  -9998L
#define M_DISABLE                                     -9999L

// M_CAMERA_LOCK
#define M_ENABLE                                      -9997L
#ifdef  M_MIL_75_COMPATIBLE                           //TBR MIL 8.0
#define M_FAST                                   0x00040000L
#else
#define M_FAST                                   0x00002000L
#endif

// Uart parity values
#define M_ODD                                         1L
#define M_EVEN                                        2L

#define M_RS232                                       1L
#define M_RS485                                       2L


/************************************************************************/
/* MdigControl(): Grab VM stream ID                                     */
/************************************************************************/
#define M_VM_STREAM_ID_0                              0L
#define M_VM_STREAM_ID_1                              1L
#define M_VM_STREAM_ID_2                              2L
#define M_VM_STREAM_ID_3                              3L
#define M_VM_STREAM_ID_4                              4L
#define M_VM_STREAM_ID_5                              5L
#define M_VM_STREAM_ID_6                              6L
#define M_VM_STREAM_ID_7                              7L
#define M_VM_STREAM_ID_8                              8L
#define M_VM_STREAM_ID_9                              9L
#define M_VM_STREAM_ID_10                            10L
#define M_VM_STREAM_ID_11                            11L
#define M_VM_STREAM_ID_12                            12L
#define M_VM_STREAM_ID_13                            13L
#define M_VM_STREAM_ID_14                            14L


/************************************************************************/
/* MdigChannel()                                                        */
/************************************************************************/
#define M_TUNER_CHANNEL                               0x00100000L
/* Reserve next 126 values (M_TUNER_CHANNEL +   0L)   0x00100000L*/
/*                         (M_TUNER_CHANNEL + 126L)   0x0010007EL*/
#define M_TUNER_BAND                                  0x00200000L
/* Reserve next 2   values (M_TUNER_BAND | M_REGULAR) 0x00220000L*/
/*                         (M_TUNER_BAND | M_CABLE  ) 0x00220001L*/
#define M_DEFAULT                                     0x10000000L
#define M_CH0                                         0x20000000L
#define M_CH1                                         0x40000000L
#define M_CH2                                         0x80000000L
#define M_CH3                                         0x01000000L
#define M_CH4                                         0x02000000L
#define M_CH5                                         0x04000000L
#define M_CH6                                         0x08000000L
#define M_CH7                                         0x00200000L
#define M_CH8                                         0x00010000L
#define M_CH9                                         0x00020000L
#define M_CH10                                        0x00040000L
#define M_CH11                                        0x00080000L
#define M_CH12                                        0x00001000L
#define M_CH13                                        0x00002000L
#define M_CH14                                        0x00004000L
#define M_CH15                                        0x00008000L
#define M_CH16                                        0x00000100L
#define M_CH17                                        0x00000200L
#define M_CH18                                        0x00000400L
#define M_CH19                                        0x00000800L
#define M_CH20                                        0x00000010L
#define M_CH21                                        0x00000020L
#define M_CH22                                        0x00000040L
#define M_CH23                                        0x00000080L
#define M_SYNC                                        0x00400000L
#define M_SIGNAL                                      0x00800000L
#define M_AC                                          0x00000004L
#define M_DC                                          0x00000002L
#define M_CHANNEL_COLOR                               0x00000001L
#define M_RGB                                         8L
#define M_YC                                          9L
#define M_RCA                                         M_CH0
#define M_ALL_CHANNEL                                 (M_CH0|M_CH1|M_CH2|M_CH3|M_CH4|M_CH5|M_CH6|M_CH7|M_CH8|M_CH9|M_CH10|M_CH11)
#define M_ALL_CHANNELS                                M_ALL_CHANNEL
#define M_REGULAR                                     0x00020000L
#define M_CABLE                                       0x00020001L

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_DIG_TUNER_CHANNEL                           M_TUNER_CHANNEL
#define M_DIG_TUNER_BAND                              M_TUNER_BAND
#endif

/************************************************************************/
/* MdigLut()                                                            */
/************************************************************************/
#define M_DEFAULT_LUT                                 (M_PSEUDO_ID+1)

/************************************************************************/
/* MdigReference()                                                      */
/************************************************************************/
#define M_BLACK                                       0L
#define M_WHITE                                       1L
#define M_STEREO                                      0L
#define M_MONO                                        1L

/* See the Inquire for the M_CHx values */
#define M_CH0_REF                                     M_CH0
#define M_CH1_REF                                     M_CH1
#define M_CH2_REF                                     M_CH2
#define M_CH3_REF                                     M_CH3
#define M_CH4_REF                                     M_CH4
#define M_CH5_REF                                     M_CH5
#define M_CH6_REF                                     M_CH6
#define M_CH7_REF                                     M_CH7
#define M_ALL_REF                                     (M_CH0_REF | M_CH1_REF | M_CH2_REF | M_CH3_REF | M_CH4_REF | M_CH5_REF | M_CH6_REF | M_CH7_REF)
#define M_VOLTAGE                                     0x00001000L
#define M_MIN_LEVEL                                   0L
#define M_MAX_LEVEL                                   255L

/************************************************************************/
/* MdigFocus()                                                          */
/************************************************************************/
#define M_NO_FILTER        0x00080000L
#define M_NO_SUBSAMPLING   0x00100000L
#define M_SMART_SCAN       0x00200000L
#define M_SCAN_ALL         0x00400000L
#define M_BISECTION        0x00800000L
#define M_REFOCUS          0x01000000L
#define M_EVALUATE         0x02000000L
#define M_CHANGE           0x00000063L
#define M_ON_FOCUS         0x00000064L

/************************************************************************/
/* MdigGrabWait()                                                       */
/************************************************************************/
#define M_GRAB_NEXT_FRAME                             1L
#define M_GRAB_NEXT_FIELD                             2L
#define M_GRAB_END                                    3L


/************************************************************************/
/* MdigHookFunction()                                                   */
/************************************************************************/
#define M_FIRST                                       0x10000000L
#define M_LAST                                        0x20000000L
#define M_UNHOOK                                      0x04000000L

#define M_GRAB_END                                    3L
#define M_GRAB_START                                  4L
#define M_GRAB_FRAME_END                              5L
#define M_GRAB_FIELD_END_ODD                          6L
#define M_GRAB_FIELD_END_EVEN                         7L
#define M_GRAB_FIELD_END                              8L
#define M_FRAME_START                                 9L
#define M_FIELD_START                                 20L
#define M_FIELD_START_ODD                             21L
#define M_FIELD_START_EVEN                            22L
#define M_GRAB_FRAME_START                            13L
#define M_GRAB_FIELD_START                            14L
#define M_GRAB_FIELD_START_ODD                        15L
#define M_GRAB_FIELD_START_EVEN                       16L
#define M_UART_DATA_RECEIVED                          17L
#define M_GRAB_LINE                                   0x00100000L
#define M_DCF_REALLOC                                 0x00000020L
#define M_CAMERA_LOCK                                 5300L
#define M_CAMERA_PRESENT                              5315L

/************************************************************************/
/* MgenLutFunction()                                                    */
/************************************************************************/
#define M_LOG                                         0x00000000L
#define M_EXP                                         0x00000001L
#define M_SIN                                         0x00000002L
#define M_COS                                         0x00000003L
#define M_TAN                                         0x00000004L
#define M_QUAD                                        0x00000005L

/************************************************************************/
/* MgenWarpParameter()                                                  */
/************************************************************************/
/* 8 bits reserved for number of fractional bits added to M_FIXED_POINT */
#define M_WARP_MATRIX                                 0x00100000L
#define M_WARP_POLYNOMIAL                             0x00200000L
#define M_WARP_LUT                                    0x00400000L
#define M_WARP_4_CORNER                               0x00800000L
#define M_WARP_4_CORNER_REVERSE                       0x01000000L
/* Optional controls */
#define M_FIXED_POINT                                 0x00004000L
#define M_OVERSCAN_ENABLE                             0x00008000L

#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
#define M_OVERSCAN_DISABLE                            0x40000000L
#else
#define M_OVERSCAN_DISABLE                            (M_ID_OFFSET_OF_DEFAULT_KERNEL >> 1L)
#endif

#ifdef  M_MIL_75_COMPATIBLE                               //TBR MIL 8.0
#define M_FAST                                        0x00040000L
#else
#define M_FAST                                        0x00002000L
#endif
#define M_VERY_FAST                                   0x00010000L
#define M_REGULAR                                     0x00020000L
/* Transforms */
#define M_RESIZE                                      1L
#define M_ROTATE                                      2L
#define M_SHEAR_X                                     3L
#define M_SHEAR_Y                                     4L
#define M_TRANSLATE                                   5L
#define M_SCALE                                 0x00008010L

/************************************************************************/
/* MbufBayer() defines                                                  */
/************************************************************************/
#define M_BAYER_MASK                                  224L
#define M_BAYER_GB                                    32L
#define M_BAYER_BG                                    64L
#define M_BAYER_RG                                    96L
#define M_BAYER_GR                                    128L
#define M_BAYER_GB_OLD                                7L
#define M_BAYER_BG_OLD                                8L
#define M_BAYER_RG_OLD                                9L
#define M_BAYER_GR_OLD                                10L
#define M_WHITE_BALANCE_CALCULATE                     16L
#define M_BAYER_BW_COMPAT                             256L
#define M_BAYER_NORMALIZE                             512L
#define M_BAYER_REVERSE                               M_REVERSE
#define M_BAYER_OVERSCAN_ENABLE                       M_OVERSCAN_ENABLE
#define M_BAYER_OVERSCAN_DISABLE                      M_OVERSCAN_DISABLE

/************************************************************************/
/* MimGetResult()                                                       */
/************************************************************************/
#define M_VALUE                                       0L
#define M_POSITION_X                                  0x00003400L
#define M_POSITION_Y                                  0x00004400L
#define M_NB_EVENT                                    5L

/************************************************************************/
/* MimInquire()                                                         */
/************************************************************************/
#define M_RESULT_SIZE                                 0L
#define M_RESULT_TYPE                                 1L

/************************************************************************/
/* MimFindExtreme()                                                     */
/************************************************************************/
#define M_MAX_VALUE                                   1L
#define M_MIN_VALUE                                   2L
#define M_MIN_ABS_VALUE                               4L
#define M_MAX_ABS_VALUE                               5L

/************************************************************************/
/* MimStat()                                                            */
/************************************************************************/
#define M_MIN_ABS                                     0x00200000L
#define M_MAX_ABS                                     0x00800000L
#define M_SUM_ABS                                     0x00000008L
#define M_SUM_SQUARED                                 0x00000400L
#define M_SUM                                         0x20000000L  //Already defined in milocr.h
#define M_MEAN                                        0x08000000L  //Already defined in milmeas.h
#define M_STANDARD_DEVIATION                          0x10000000L  //Already defined in milmeas.h

/************************************************************************/
/* MimArith()                                                           */
/************************************************************************/
#define M_CONSTANT                                    0x8000L
#define M_FIXED_POINT                                 0x00004000L
#define M_ADD                                         0x0000L
#define M_ADD_CONST                                   (M_ADD | M_CONSTANT)
#define M_SUB                                         0x0001L
#define M_SUB_CONST                                   (M_SUB | M_CONSTANT)
#define M_NEG_SUB                                     0x000AL
#define M_CONST_SUB                                   (M_NEG_SUB | M_CONSTANT)
#define M_SUB_ABS                                     0x0011L
#define M_MIN                                         0x02000000L
#define M_MIN_CONST                                   (M_MIN | M_CONSTANT)
#define M_MAX                                         0x04000000L
#define M_MAX_CONST                                   (M_MAX | M_CONSTANT)
#define M_OR                                          0x0016L
#define M_OR_CONST                                    (M_OR  | M_CONSTANT)
#define M_AND                                         0x0017L
#define M_AND_CONST                                   (M_AND | M_CONSTANT)
#define M_XOR                                         0x0018L
#define M_XOR_CONST                                   (M_XOR | M_CONSTANT)
#define M_NOR                                         0x0019L
#define M_NOR_CONST                                   (M_NOR | M_CONSTANT)
#define M_NAND                                        0x001AL
#define M_NAND_CONST                                  (M_NAND | M_CONSTANT)
#define M_XNOR                                        0x001BL
#define M_XNOR_CONST                                  (M_XNOR | M_CONSTANT)
#define M_NOT                                         0x0014L
#define M_NEG                                         0x0023L
#define M_ABS                                         0x000CL
#define M_PASS                                        0x0002L
#define M_CONST_PASS                                  (M_PASS | M_CONSTANT)
#define M_MULT                                        0x0100L
#define M_MULT_CONST                                  (M_MULT | M_CONSTANT)
#define M_DIV                                         0x0101L
#define M_DIV_CONST                                   (M_DIV | M_CONSTANT)
#define M_INV_DIV                                     0x0102L
#define M_CONST_DIV                                   (M_INV_DIV | M_CONSTANT)


/************************************************************************/
/* MimArithMultiple()                                                   */
/************************************************************************/
#define M_OFFSET_GAIN                                 0x0000L
#define M_WEIGHTED_AVERAGE                            0x0001L
#define M_MULTIPLY_ACCUMULATE_1                       0x0002L
#define M_MULTIPLY_ACCUMULATE_2                       0x0004L

/************************************************************************/
/* MimFlip()                                                            */
/************************************************************************/
#define M_FLIP_VERTICAL                               1L
#define M_FLIP_HORIZONTAL                             2L

/************************************************************************/
/* MimBinarize(), MimClip(), MimLocateEvent(), MimStat()                */
/* Some of these defines are also define in milutil.h                   */
/************************************************************************/
#define M_IN_RANGE                                    1L
#define M_OUT_RANGE                                   2L
#define M_EQUAL                                       3L
#define M_NOT_EQUAL                                   4L
#define M_GREATER                                     5L
#define M_LESS                                        6L
#define M_GREATER_OR_EQUAL                            7L
#define M_LESS_OR_EQUAL                               8L
#define M_MASK                                        9L

/************************************************************************/
/* MimConvolve()                                                        */
/************************************************************************/
#define M_ID_OFFSET_OF_DEFAULT_KERNEL                 (0x00100000L|M_PSEUDO_ID)
#define M_ID_OFFSET_OF_EDGE_DETECTVAR                 (0x00100100L|M_PSEUDO_ID)
#define M_SMOOTH                                      (M_ID_OFFSET_OF_DEFAULT_KERNEL +  0L)
#define M_LAPLACIAN_EDGE                              (M_ID_OFFSET_OF_DEFAULT_KERNEL +  1L)
#define M_LAPLACIAN_EDGE2                             (M_ID_OFFSET_OF_DEFAULT_KERNEL +  2L)
#define M_SHARPEN                                     (M_ID_OFFSET_OF_DEFAULT_KERNEL +  3L)
#define M_SHARPEN2                                    (M_ID_OFFSET_OF_DEFAULT_KERNEL +  4L)
#define M_HORIZ_EDGE                                  (M_ID_OFFSET_OF_DEFAULT_KERNEL +  5L)
#define M_VERT_EDGE                                   (M_ID_OFFSET_OF_DEFAULT_KERNEL +  6L)
#define M_EDGE_DETECT                                 (M_ID_OFFSET_OF_DEFAULT_KERNEL +  7L)
#define M_EDGE_DETECT2                                (M_ID_OFFSET_OF_DEFAULT_KERNEL +  8L)
#define M_EDGE_DETECTVAR(Val)                         (M_ID_OFFSET_OF_EDGE_DETECTVAR + Val)
#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
#define M_OVERSCAN_DISABLE                            0x40000000L
#else
#define M_OVERSCAN_DISABLE                            (M_ID_OFFSET_OF_DEFAULT_KERNEL >> 1L)
#endif

/************************************************************************/
/* MimEdgeDetect()                                                      */
/************************************************************************/
#define M_SOBEL                                       M_EDGE_DETECT
#define M_PREWITT                                     M_EDGE_DETECT2
#define M_NOT_WRITE_ANGLE                             1L
#define M_NOT_WRITE_INT                               2L
#define M_FAST_ANGLE                                  4L
#define M_FAST_GRADIENT                               8L
#define M_FAST_EDGE_DETECT                            (M_FAST_ANGLE + M_FAST_GRADIENT)
#define M_REGULAR_ANGLE                               16L
#define M_REGULAR_GRADIENT                            64L
#define M_REGULAR_EDGE_DETECT                         (M_REGULAR_ANGLE + M_REGULAR_GRADIENT)
#define M_USE_RESERVED_ANGLE_VALUE                    128L


/************************************************************************/
/* MimRank()                                                            */
/************************************************************************/
#define M_MEDIAN                                      0x10000L
#define M_3X3_RECT                                    (M_ID_OFFSET_OF_DEFAULT_KERNEL + 20L)
#define M_3X3_CROSS                                   (M_ID_OFFSET_OF_DEFAULT_KERNEL + 21L)

/************************************************************************/
/* MimMorphic(), ...                                                    */
/************************************************************************/
#define M_ERODE                                       1L
#define M_DILATE                                      2L
#define M_THIN                                        3L
#define M_THICK                                       4L
#define M_HIT_OR_MISS                                 5L
#define M_MATCH                                       6L
#define M_HIT_THRESHOLD                              16L

/************************************************************************/
/* MimThin()                                                            */
/************************************************************************/
#define M_TO_SKELETON                                 -1L


/************************************************************************/
/* MimThick()                                                           */
/************************************************************************/
#define M_TO_IDEMPOTENCE                              M_TO_SKELETON

/************************************************************************/
/* MimDistance()                                                        */
/************************************************************************/
#define M_CHAMFER_3_4                                 0x01
#define M_CITY_BLOCK                                  0x02
#define M_CHESSBOARD                                  0x04
#define M_FORWARD                                     0x01
#define M_BACKWARD                                    0x02
#define M_OVERSCAN_TO_DO                              0x04
#define M_BOTH                                        0x07

/************************************************************************/
/* MimWatershed()                                                        */
/************************************************************************/
#define M_WATERSHED                                   0x0001
#define M_BASIN                                       0x0002
#define M_MINIMA_FILL                                 0x0004
#define M_MAXIMA_FILL                                 0x0008

#define M_4_CONNECTED                                 0x00000010L
#define M_8_CONNECTED                                 0x00000020L
#define M_REGULAR                                     0x00020000L
#define M_STRAIGHT_WATERSHED                          0x0080
#define M_SKIP_LAST_LEVEL                             0x0100
#define M_FILL_SOURCE                                 0x0200
#define M_LABELLED_MARKER                             0x0400

#define M_NEW_METHOD                                  0x0800

/************************************************************************/
/* MimProject()                                                         */
/************************************************************************/
#define M_0_DEGREE                                      0.0
#define M_90_DEGREE                                    90.0
#define M_180_DEGREE                                  180.0
#define M_270_DEGREE                                  270.0

/************************************************************************/
/* MimResize(), MimTranslate() and MimRotate()                          */
/************************************************************************/
#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
#define M_OVERSCAN_DISABLE                            0x40000000L
#else
#define M_OVERSCAN_DISABLE                            (M_ID_OFFSET_OF_DEFAULT_KERNEL >> 1L)
#endif
#define M_OVERSCAN_ENABLE                             0x00008000L
#define M_INTERPOLATE                                 0x00000004L
#define M_BILINEAR                                    0x00000008L
#define M_BICUBIC                                     0x00000010L
#define M_AVERAGE                                     0x00000020L
#define M_NEAREST_NEIGHBOR                            0x00000040L
#define M_OVERSCAN_CLEAR                              0x00000080L
#define M_FIT_ALL_ANGLE                               0x00000100L
#define M_BINARY                                      0x00001000L
#ifdef  M_MIL_75_COMPATIBLE                               //TBR MIL 8.0
#define M_FAST                                        0x00040000L
#else
#define M_FAST                                        0x00002000L
#endif
#define M_REGULAR                                     0x00020000L


/************************************************************************/
/* MimResize                                                            */
/************************************************************************/
#define M_FILL_DESTINATION                            -1 // DO NOT MODIFY, WITHOUT CODE MODIFICATION


/************************************************************************/
/* MimHistogramEqualize()                                               */
/************************************************************************/
#define M_UNIFORM                                     1
#define M_EXPONENTIAL                                 2
#define M_RAYLEIGH                                    3
#define M_HYPER_CUBE_ROOT                             4
#define M_HYPER_LOG                                   5


/************************************************************************/
/* MimConvert()                                                         */
/************************************************************************/
#define M_RGB_TO_HLS                                  1L
#define M_RGB_TO_L                                    2L
#define M_HLS_TO_RGB                                  3L
#define M_L_TO_RGB                                    4L
#define M_RGB_TO_Y                                    5L
#define M_RGB_TO_H                                    6L

#define M_RGB_TO_YUV16                                101L     // Same as Mildev.h
#define M_RGB_TO_YCRCB                                7L
#define M_RGB_TO_YCLIP                                8L
#define M_YUV16_TO_RGB                                301L     // Same as Mildev.h
#define M_YCRCB_TO_RGB                                9L

// If the conversion type is set to M_DEFAULT, the conversion will be
//  choosen according to the type of buffers passed to the function.
#define M_DEFAULT                                    0x10000000L

// These can be added to the conversion type to speed-up the
//   process ( M_FAST ) or have a better precision ( M_REGULAR ).
//   Not all conversion are affected by this flag.
#define M_REGULAR                                     0x00020000L
#ifdef  M_MIL_75_COMPATIBLE                               //TBR MIL 8.0
#define M_FAST                                        0x00040000L
#else
#define M_FAST                                        0x00002000L
#endif

/************************************************************************/
/* MimWarp()                                                            */
/************************************************************************/
/* 8 bits reserved for number of fractional bits */
#define M_WARP_MATRIX                                 0x00100000L
#define M_WARP_POLYNOMIAL                             0x00200000L
#define M_WARP_LUT                                    0x00400000L
#define M_OVERSCAN_ENABLE                             0x00008000L

#if  M_MIL_READY_FOR_MIL80                                              //TBR MIL 8.0
#define M_OVERSCAN_DISABLE                            0x40000000L
#else
#define M_OVERSCAN_DISABLE                            (M_ID_OFFSET_OF_DEFAULT_KERNEL >> 1L)
#endif

#define M_OVERSCAN_CLEAR                              0x00000080L
#define M_FIXED_POINT                                 0x00004000L

/************************************************************************/
/* MimTransform()                                                       */
/************************************************************************/
#define M_FFT                                         1L
#define M_DCT8x8                                      2L
#define M_DCT                                         3L
#define M_FFT_OLD                                     4L

#define M_FORWARD                                     0x01
#define M_REVERSE                                     4L
#define M_NORMALIZED                                  0x00000002L
#define M_NORMALIZE                                   M_NORMALIZED
#define M_1D_ROWS                                     0x00000010L
#define M_1D_COLUMNS                                  0x00000020L
#define M_REAL                                        0x00000040L
#define M_CENTER                                      0x00000080L
#define M_MAGNITUDE                                   0x00000100L
#define M_PHASE                                       0x00000200L
#define M_LOG_SCALE                                   0x00000400L


/************************************************************************/
/* MimPolarTransform()                                                  */
/************************************************************************/
#define M_RECTANGULAR_TO_POLAR                        0x00000001L
#define M_POLAR_TO_RECTANGULAR                        0x00000002L

/************************************************************************/
/* Buffer attributes                                                    */
/************************************************************************/
#define M_NO_ATTR                                     0x00000000L
#define M_IN                                          0x00000001L
#define M_OUT                                         0x00000002L
#define M_SRC                                         M_IN
#define M_DEST                                        M_OUT

#define M_IMAGE                                       0x00000004L
#define M_GRAB                                        0x00000008L
#define M_PROC                                        0x00000010L
#define M_DISP                                        0x00000020L
#define M_GRAPH                                       0x00000040L
#define M_EVENT                                       0x00000080L
#define M_SYSTEM_THREAD                               0x00000100L
#define M_APPLICATION                                 0x00000200L
#define M_STRUCT_ELEMENT                              0x00000400L
#define M_ARRAY                                       0x00000800L
#define M_STAT_LIST                                   0x00001000L
#define M_HIST_LIST                                   0x00002000L
#define M_PROJ_LIST                                   0x00004000L
#define M_EVENT_LIST                                  0x00008000L
#define M_EXTREME_LIST                                0x00010000L
#define M_COUNT_LIST                                  0x00020000L
#ifdef M_MIL_75_COMPATIBLE
#define M_LUT                                         0x00000100L
#else
#define M_LUT                                         0x00040000L
#endif //M_MIL_75_COMPATIBLE
#define M_LUT_OLD                                     0x00000100L
#define M_WARP_COEFFICIENT                            0x00080000L
#define M_DIGITIZER                                   0x00100000L
#define M_DISPLAY                                     0x00200000L
#ifdef M_MIL_75_COMPATIBLE
#define M_KERNEL                                      0x00000200L
#else
#define M_KERNEL                                      0x00400000L
#endif //M_MIL_75_COMPATIBLE
#define M_KERNEL_OLD                                  0x00000200L
#define M_SYSTEM                                      0x00800000L
#define M_GRAPHIC_CONTEXT                             0x01000000L
#define M_CALL_CONTEXT                                0x02000000L
#define M_ERROR_CONTEXT                               0x04000000L
#define M_THREAD_CONTEXT                              0x08000000L
#define M_EVENT_CONTEXT                               0x10000000L
#define M_USER_ATTRIBUTE                              0x20000000L
#define M_HOOK_CONTEXT                                0x40000000L
#define M_INTERNAL_ALLOCATION                         ((BUFATTRTYPE)MAKE_INT64(0x0000000080000000))
#define M_ALL_BUFFER_TYPES                           (M_IMAGE          | \
                                                      M_LUT            | \
                                                      M_KERNEL         | \
                                                      M_STRUCT_ELEMENT | \
                                                      M_ARRAY          | \
                                                      M_HIST_LIST      | \
                                                      M_PROJ_LIST      | \
                                                      M_EVENT_LIST     | \
                                                      M_EXTREME_LIST   | \
                                                      M_COUNT_LIST)

/* Use this macro to verify safely if a Mil object is a buffer. */
#define M_IS_BUFFER_TYPE( ParIs )      ( (0                != (ParIs & M_ALL_BUFFER_TYPES)) && \
                                         (M_USER_ATTRIBUTE != (ParIs & M_USER_ATTRIBUTE  )) )

/* Use this macro to verify safely if a Mil object is a display. */
#define M_IS_DISPLAY_TYPE( ParIs )     ( (M_DISPLAY == (ParIs & M_DISPLAY        )) && \
                                         (0         == (ParIs & M_USER_ATTRIBUTE )) && \
                                         (0         == (ParIs & M_ALL_BUFFER_TYPES)) )

/* Use this macro to verify safely if a Mil object is a digitizer. */
#define M_IS_DIGITIZER_TYPE( ParIs )   ( (M_DIGITIZER == (ParIs & M_DIGITIZER      )) && \
                                         (0           == (ParIs & M_USER_ATTRIBUTE )) && \
                                         (0           == (ParIs & M_ALL_BUFFER_TYPES)) )


#define M_USER_OBJECT_1                               (M_USER_ATTRIBUTE | 0x00010000L)
#define M_USER_OBJECT_2                               (M_USER_ATTRIBUTE | 0x00020000L)
#define M_BLOB_OBJECT                                 (M_USER_ATTRIBUTE | 0x00040000L)
#define M_BLOB_FEATURE_LIST                           (M_BLOB_OBJECT    | 0x00000001L)
#define M_BLOB_RESULT                                 (M_BLOB_OBJECT    | 0x00000002L)
#define M_PAT_OBJECT                                  (M_USER_ATTRIBUTE | 0x00080000L)
#define M_PAT_MODEL                                   (M_PAT_OBJECT     | 0x00000001L)
#define M_PAT_RESULT                                  (M_PAT_OBJECT     | 0x00000002L)
#define M_OCR_OBJECT                                  (M_USER_ATTRIBUTE | 0x00100000L)
#define M_OCR_FONT                                    (M_OCR_OBJECT     | 0x00000001L)
#define M_OCR_RESULT                                  (M_OCR_OBJECT     | 0x00000002L)
#define M_OCR_MULTILINES_RESULT                       (M_OCR_OBJECT     | 0x00000003L)
#define M_MEAS_OBJECT                                 (M_USER_ATTRIBUTE | 0x00200000L)
#define M_MEAS_MARKER                                 (M_MEAS_OBJECT    | 0x00000001L)
#define M_MEAS_RESULT                                 (M_MEAS_OBJECT    | 0x00000002L)
#define M_MEAS_CONTEXT                                (M_MEAS_OBJECT    | 0x00000004L)
#define M_CAL_OBJECT                                  (M_USER_ATTRIBUTE | 0x00400000L)
#define M_CAL_PARENT                                  (M_CAL_OBJECT     | 0x00000001L)
#define M_CAL_CHILD                                   (M_CAL_OBJECT     | 0x00000002L)
#define M_CODE_OBJECT                                 (M_USER_ATTRIBUTE | 0x00800000L)
#define M_CODE_CONTEXT                                (M_CODE_OBJECT    | 0x00000001L)
#define M_BGA_OBJECT                                  (M_USER_ATTRIBUTE | 0x01000000L)
#define M_BGA_CONTEXT                                 (M_BGA_OBJECT     | 0x00000001L)
#define M_FREE_OBJECT_1                               (M_USER_ATTRIBUTE | 0x02000000L)
#define M_MOD_OBJECT                                  (M_USER_ATTRIBUTE | 0x04000000L)
#define M_MOD_GEOMETRIC                               (M_MOD_OBJECT     | 0x00000001L)
#define M_MOD_GEOMETRIC_PREDEFINED                    (M_MOD_OBJECT     | 0x00000002L)
#define M_MOD_RESULT                                  (M_MOD_OBJECT     | 0x00000004L)
#define M_MOD_GEOMETRIC_CONTROLLED                    (M_MOD_OBJECT     | 0x00000008L)
#define M_EDGE_OBJECT                                 (M_USER_ATTRIBUTE | 0x08000000L)
#define M_EDGE_CONTOUR                                (M_EDGE_OBJECT    | 0x00000001L)
#define M_EDGE_CREST                                  (M_EDGE_OBJECT    | 0x00000002L)
#define M_EDGE_RESULT                                 (M_EDGE_OBJECT    | 0x00000004L)

#define M_USER_DEFINE_LOW_ATTRIBUTE                   0x0000FFFFL

#define M_SYSTEM_ALLOCATED                            0x00000001L
#define M_USER_ALLOCATED                              0xFFFFFFFEL

#define M_SIGNED                                      0x80000000L
#define M_FLOAT                                      (0x40000000L | M_SIGNED)
#define M_DOUBLE                                     (0x20000000L | M_SIGNED)

// For M_IMAGE buffers only:
#define M_OVR                                         0x00000080L
#define M_COMPRESS                                    0x00004000L
#define M_READ_PREFETCH_ENABLED                       0x00008000L

//Keep only for backward compatibility.
#define M_MMX_ENABLED                                 M_READ_PREFETCH_ENABLED
#define M_SSE_ENABLED                                 M_READ_PREFETCH_ENABLED

#define M_FLIP                                        0x00010000L
#define M_PACKED                                      0x00020000L
#define M_PLANAR                                      0x00040000L
#define M_ON_BOARD                                    0x00080000L
#define M_OFF_BOARD                                   0x00100000L
#define M_NON_PAGED                                   0x00200000L
#define M_DIB                                         0x10000000L
#define M_DDRAW                                       0x40000000L
#define M_DIRECTX                                     M_DDRAW
#define M_SINGLE                                      0x01000000L
#define M_VIA                                         M_SINGLE
#define M_PAGED                                       0x02000000L
#define M_NO_FLIP                                     0x04000000L
#define M_OVERSCAN_BUFFER                             0x08000000L
#define M_COMPLEX_OLD                                 0x00400000L  //TBC put this bit in the higher 32-bits of attribute
#define M_OFFSCREEN                                   M_OVR
#define M_USER_ATTRIBUTE                              0x20000000L
#define M_UND                                         0x00800000L

#define M_INTERNAL_ALLOCATION   ((BUFATTRTYPE)MAKE_INT64(0x0000000080000000))

   // New define place in the 32 higher bits of the attribute
#define M_GDI                   ((BUFATTRTYPE)MAKE_INT64(0x0000000100000000))
#define M_HOST_MEMORY           ((BUFATTRTYPE)MAKE_INT64(0x0000000200000000))
#define M_VIDEO_MEMORY          ((BUFATTRTYPE)MAKE_INT64(0x0000000400000000))
#define M_NON_LOCAL_MEMORY      ((BUFATTRTYPE)MAKE_INT64(0x0000000800000000))
#define M_FAST_MEMORY           ((BUFATTRTYPE)MAKE_INT64(0x0000001000000000))
#define M_SHARED                ((BUFATTRTYPE)MAKE_INT64(0x0000002000000000))
#define M_PRIMARY_SURFACE       ((BUFATTRTYPE)MAKE_INT64(0x0000004000000000))
#define M_BITMAP_HANDLE         ((BUFATTRTYPE)MAKE_INT64(0x0000008000000000))
// BITS RESERVED FOR COMPLEX BUFFERS          0x0000070000000000
#define M_LIVE_VIDEO            ((BUFATTRTYPE)MAKE_INT64(0x0000080000000000))
#define M_MTX0                  ((BUFATTRTYPE)MAKE_INT64(0x0000100000000000))
#define M_NON_CACHABLE          ((BUFATTRTYPE)MAKE_INT64(0x0000200000000000))
#define M_FAST_OVERSCAN         ((BUFATTRTYPE)MAKE_INT64(0x0000400000000000))
#define M_UNUSED_BITS           ((BUFATTRTYPE)MAKE_INT64(0xFFFF800000000000)) //Update when reusing bits for new defines

#define M_COMPLEX_OFFSET                                     (40)
#define M_COMPLEX_MASK                                (((BUFATTRTYPE)0x7)<<M_COMPLEX_OFFSET)
#define M_IS_COMPLEX(attrib)                          (((attrib&M_COMPLEX_MASK) != 0) && ((attrib&M_COMPRESS) == 0))
#define M_COMPLEX(_nb)                                ((((BUFATTRTYPE)_nb)<<M_COMPLEX_OFFSET)&M_COMPLEX_MASK)
#define M_COMPLEX_NB_BUFFER(attrib)                   (((attrib&M_COMPRESS) != 0)?0:((long)(((BUFATTRTYPE)attrib&M_COMPLEX_MASK)>>M_COMPLEX_OFFSET)))

#define M_COMPRESSION_FORMAT_MASK                     ((BUFATTRTYPE)MAKE_INT64(0x0000070008c00000))

#define M_JPEG_LOSSLESS                               0x00400000L
#define M_JPEG_LOSSY                                  0x00800000L
#define M_JPEG_LOSSLESS_INTERLACED                    0x00C00000L
#define M_JPEG_LOSSY_INTERLACED                       0x08000000L
#define M_JPEG2000_LOSSY                              0x08400000L
#define M_JPEG2000_LOSSLESS                           0x08800000L
#define M_JPEG2000_LOSSY_INTERLACED                   ((BUFATTRTYPE)MAKE_INT64(0x0000010008c00000))
#define M_JPEG2000_LOSSLESS_INTERLACED                ((BUFATTRTYPE)MAKE_INT64(0x0000020008c00000))

#define M_RESERVED_COMPRESSION_TYPE                   0x08C00000L
#define M_IS_LOSSY(a)                                 ((((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSY) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSY_INTERLACED) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSY) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSY_INTERLACED))
#define M_IS_LOSSLESS(a)                              ((((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSLESS) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSLESS_INTERLACED) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSLESS) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSLESS_INTERLACED))
#define M_IS_JPEG(a)                                  ((((a) & M_COMPRESSION_FORMAT_MASK) >= M_JPEG_LOSSLESS) && \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) <= M_JPEG_LOSSY_INTERLACED))
#define M_IS_JPEG2000(a)                              ((((a) & M_COMPRESSION_FORMAT_MASK) >= M_JPEG2000_LOSSY) && \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) <= M_JPEG2000_LOSSLESS_INTERLACED))
#define M_IS_COMPRESSED(a)                            (((a) & M_COMPRESS) != 0)
#define M_IS_INTERLACED(a)                            ((((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSY_INTERLACED) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG_LOSSLESS_INTERLACED) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSY_INTERLACED) || \
                                                       (((a) & M_COMPRESSION_FORMAT_MASK) == M_JPEG2000_LOSSLESS_INTERLACED))

/* 6 bits reserved for buffer internal format and format definitions */
#define M_INTERNAL_FORMAT             0x00003F00L
#define M_INTERNAL_FORMAT_SHIFT       8L
#define M_INTERNAL_COMPLETE_FORMAT    0x7FFFFF80L
#define M_ANY_INTERNAL_FORMAT         (0   << M_INTERNAL_FORMAT_SHIFT)

// RGB formats
#define M_MONO1                       (1   << M_INTERNAL_FORMAT_SHIFT)   // Force  1 bit  pixels in monochrome format
#define M_MONO8                       (2   << M_INTERNAL_FORMAT_SHIFT)   // Force  8 bits pixels in monochrome format
#define M_MONO16                      (3   << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in monochrome format
#define M_MONO32                      (4   << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in monochrome format
#define M_MONO10                      (5   << M_INTERNAL_FORMAT_SHIFT)   // Force 10 bits pixels in monochrome format => use with displays to describe 10-bits grayscale monitor (medical)
#define M_RGB_BGR_START               (6   << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in color XRGB  1.5.5.5  format
#define M_RGB15                       (6   << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in color XRGB  1.5.5.5  format
#define M_RGB16                       (7   << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in color RGB   5.6.5    format
#define M_RGB24                       (8   << M_INTERNAL_FORMAT_SHIFT)   // Force 24 bits pixels in color RGB   8.8.8    format
#define M_RGBX32                      (9   << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color RGBX  8.8.8.8  format
#define M_RGB32                       M_RGBX32
#define M_XRGB32                      (10  << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color XRGB  8.8.8.8  format
#define M_RGB48                       (11  << M_INTERNAL_FORMAT_SHIFT)   // Force 48 bits pixels in color RGB  16.16.16  format
#define M_RGB96                       (12  << M_INTERNAL_FORMAT_SHIFT)   // Force 96 bits pixels in color RGB  32.32.32  format
#define M_RGB3                        (13  << M_INTERNAL_FORMAT_SHIFT)   // Force  3 bits pixels in color RGB   1.1.1    format
#define M_BGR15                       (14  << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in color BGRX  5.5.5.1  format
#define M_BGR16                       (15  << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits pixels in color BGR   5.6.5    format
#define M_BGR24                       (16  << M_INTERNAL_FORMAT_SHIFT)   // Force 24 bits pixels in color BGR   8.8.8    format
#define M_BGRX32                      (17  << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color BGRX  8.8.8.8  format
#define M_BGR32                       M_BGRX32
#define M_XBGR32                      (18  << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color XBGR  8.8.8.8  format
#define M_BGR30                       (19  << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color XBGR  2.10.10.10  format
#define M_RGB_BGR_END                 (19  << M_INTERNAL_FORMAT_SHIFT)   // Force 32 bits pixels in color XBGR  8.8.8.8  format

// YUV formats
#define M_YUV_FORMAT_START            (21  << M_INTERNAL_FORMAT_SHIFT)
#define M_YUV9                        (21  << M_INTERNAL_FORMAT_SHIFT)   // Force  9 bits YUV pixels in color YUV 16:1:1 format
#define M_YUV12                       (22  << M_INTERNAL_FORMAT_SHIFT)   // Force 12 bits YUV pixels in color YUV  4:1:1 format
#define M_YUV16                       (23  << M_INTERNAL_FORMAT_SHIFT)   // Force 16 bits YUV pixels in color YUV  4:2:2 format
#define M_YUV24                       (27  << M_INTERNAL_FORMAT_SHIFT)   // Force 24 bits YUV pixels in color YUV  4:4:4 format
#define M_YUV16_YUYV                  (28  << M_INTERNAL_FORMAT_SHIFT)   // Force YUV16 packed buffers into a YUYV memory layout ( meteor, corona, meteor2 ) (FOURCC: YUY2).
#define M_YUV16_UYVY                  (29  << M_INTERNAL_FORMAT_SHIFT)   // Force YUV16 packed buffers into a UYVY memory layout ( meteor ) (FOURCC: UYVY).
#define M_YUV16_1394                  M_YUV16_UYVY
#define M_YUV12_1394                  (31  << M_INTERNAL_FORMAT_SHIFT)   // Force YUV12 packed buffers with the special memory layout of the 1394.
#define M_YUV_FORMAT_END              (39  << M_INTERNAL_FORMAT_SHIFT)
#define M_IS_FORMAT_YUV(f)            (((f&M_INTERNAL_FORMAT)>=M_YUV_FORMAT_START)&&((f&M_INTERNAL_FORMAT)<=M_YUV_FORMAT_END))
#define M_IS_FORMAT_RGB_BGR(f)        (((f&M_INTERNAL_FORMAT)>=M_RGB_BGR_START)&&((f&M_INTERNAL_FORMAT)<=M_RGB_BGR_END))
#define M_BIT_FIELD_FORMAT(f)         (1<<(((f&M_INTERNAL_FORMAT)>>M_INTERNAL_FORMAT_SHIFT)-1))
#define M_SINGLE_BAND                 (255 << M_INTERNAL_FORMAT_SHIFT)   // PutColor and GetColor specification for a single band

#define M_YUV1611                     M_YUV9
#define M_YUV411                      M_YUV12
#define M_YUV422                      M_YUV16
#define M_YUV422_16BIT                (24 << M_INTERNAL_FORMAT_SHIFT)
#define M_YUV444                      M_YUV24
#define M_YUV422_YUYV                 M_YUV16_YUYV
#define M_YUV422_UYVY                 M_YUV16_UYVY
#define M_YUV422_1394                 M_YUV16_1394
#define M_YUV411_1394                 M_YUV12_1394

// The following MACRO keep only attribute used by the host.
#define M_KEEP_HO_ATTRIBUTES(a)                                                   \
   ((a&M_IMAGE) ?                                                                \
      (a&(M_INTERNAL_FORMAT|M_IMAGE|M_STRUCT_ELEMENT|M_LUT|M_KERNEL|             \
          M_ARRAY|M_HIST_LIST|M_PROJ_LIST|M_EXTREME_LIST|M_EVENT_LIST|           \
          M_COUNT_LIST|M_PACKED|M_PLANAR|M_COMPRESS|M_COMPRESSION_FORMAT_MASK|   \
          M_FLIP|M_NO_FLIP|M_NON_PAGED|M_PAGED|M_READ_PREFETCH_ENABLED|          \
          M_HOST_MEMORY))                                                        \
   :  (a))


// The following MACRO takes a data format as input and returns the number
// of bits per pixel that it implies as an output.
// For an unknown data format, it returns -1.
#define NB_OF_BITS_PER_PIXEL(DataFormat,NbOfBits)            \
   switch( (DataFormat & M_INTERNAL_FORMAT) )                \
      {                                                      \
      case M_MONO1:        NbOfBits = 1;        break;       \
      case M_MONO8:        NbOfBits = 8;        break;       \
      case M_MONO16:       NbOfBits = 16;       break;       \
      case M_MONO32:       NbOfBits = 32;       break;       \
      case M_RGB15:        NbOfBits = 16;       break;       \
      case M_RGB16:        NbOfBits = 16;       break;       \
      case M_RGB24:        NbOfBits = 24;       break;       \
      case M_RGBX32:       NbOfBits = 32;       break;       \
      case M_XRGB32:       NbOfBits = 32;       break;       \
      case M_RGB48:        NbOfBits = 48;       break;       \
      case M_RGB96:        NbOfBits = 96;       break;       \
      case M_RGB3:         NbOfBits = 3;        break;       \
      case M_BGR15:        NbOfBits = 16;       break;       \
      case M_BGR16:        NbOfBits = 16;       break;       \
      case M_BGR24:        NbOfBits = 24;       break;       \
      case M_BGRX32:       NbOfBits = 32;       break;       \
      case M_XBGR32:       NbOfBits = 32;       break;       \
      case M_BGR30:        NbOfBits = 32;       break;       \
                                                             \
      case M_YUV9:         NbOfBits = 9;        break;       \
      case M_YUV12:        NbOfBits = 12;       break;       \
      case M_YUV16:        NbOfBits = 16;       break;       \
      case M_YUV422_16BIT: NbOfBits = 32;       break;       \
      case M_YUV24:        NbOfBits = 24;       break;       \
      case M_YUV16_YUYV:   NbOfBits = 16;       break;       \
      case M_YUV16_UYVY:   NbOfBits = 16;       break;       \
      case M_YUV12_1394:   NbOfBits = 12;       break;       \
      default:             NbOfBits = -1;       break;       \
      }

// The following MACRO takes a data format as input and returns the number
// of bits per pixel per plane that it implies as an output.
// For an unknown data format, it returns -1.
#define M_NB_BITS_PER_PIXEL_PER_PLANE(DataFormat,NbOfBits)      \
   if (DataFormat&M_PLANAR)                                     \
      {                                                         \
      switch( (DataFormat & M_INTERNAL_FORMAT) )                \
         {                                                      \
         case M_MONO1:        NbOfBits = 1;        break;       \
         case M_MONO8:        NbOfBits = 8;        break;       \
         case M_MONO16:       NbOfBits = 16;       break;       \
         case M_MONO32:       NbOfBits = 32;       break;       \
         case M_RGB3:         NbOfBits = 1;        break;       \
         case M_RGB24:        NbOfBits = 8;        break;       \
         case M_RGB48:        NbOfBits = 16;       break;       \
         case M_RGB96:        NbOfBits = 32;       break;       \
                                                                \
         case M_YUV9:         NbOfBits = 8;        break;       \
         case M_YUV12:        NbOfBits = 8;        break;       \
         case M_YUV16:        NbOfBits = 8;        break;       \
         case M_YUV422_16BIT: NbOfBits = 16;       break;       \
         case M_YUV24:        NbOfBits = 8;        break;       \
         case M_YUV12_1394:   NbOfBits = 8;        break;       \
         default:             NbOfBits = -1;       break;       \
         }                                                      \
      }                                                         \
   else                                                         \
      {                                                         \
      switch( (DataFormat & M_INTERNAL_FORMAT) )                \
         {                                                      \
         case M_MONO1:        NbOfBits = 1;        break;       \
         case M_MONO8:        NbOfBits = 8;        break;       \
         case M_MONO16:       NbOfBits = 16;       break;       \
         case M_MONO32:       NbOfBits = 32;       break;       \
         case M_RGB15:        NbOfBits = 16;       break;       \
         case M_RGB16:        NbOfBits = 16;       break;       \
         case M_RGB24:        NbOfBits = 24;       break;       \
         case M_RGBX32:       NbOfBits = 32;       break;       \
         case M_XRGB32:       NbOfBits = 32;       break;       \
         case M_BGR15:        NbOfBits = 16;       break;       \
         case M_BGR16:        NbOfBits = 16;       break;       \
         case M_BGR24:        NbOfBits = 24;       break;       \
         case M_BGRX32:       NbOfBits = 32;       break;       \
         case M_XBGR32:       NbOfBits = 32;       break;       \
         case M_BGR30:        NbOfBits = 32;       break;       \
                                                                \
         case M_YUV9:         NbOfBits = 9;        break;       \
         case M_YUV12:        NbOfBits = 12;       break;       \
         case M_YUV16:        NbOfBits = 16;       break;       \
         case M_YUV422_16BIT: NbOfBits = 32;       break;       \
         case M_YUV24:        NbOfBits = 24;       break;       \
         case M_YUV16_YUYV:   NbOfBits = 16;       break;       \
         case M_YUV16_UYVY:   NbOfBits = 16;       break;       \
         case M_YUV12_1394:   NbOfBits = 12;       break;       \
         default:             NbOfBits = -1;       break;       \
         }                                                      \
      }

// The following MACRO compare 2 attribute and return (boolean)
//  if the internal format are equivalent. Two internal format
//  are equivalent if the internal format is equal. The YUV16
//  is a special case since M_YUV16 is used to define one
//  of the 2 YUV16 type: YUYV and UYVY
#define M_EQUIVALENT_INTERNAL_FORMAT(a,b)                                              \
   (  (a&M_INTERNAL_FORMAT) == (b&M_INTERNAL_FORMAT)                             ||    \
      ((a&M_INTERNAL_FORMAT)==M_YUV16  &&  (b&M_INTERNAL_FORMAT)==M_YUV16_YUYV)  ||    \
      ((a&M_INTERNAL_FORMAT)==M_YUV16  &&  (b&M_INTERNAL_FORMAT)==M_YUV16_UYVY)  ||    \
      ((b&M_INTERNAL_FORMAT)==M_YUV16  &&  (a&M_INTERNAL_FORMAT)==M_YUV16_YUYV)  ||    \
      ((b&M_INTERNAL_FORMAT)==M_YUV16  &&  (a&M_INTERNAL_FORMAT)==M_YUV16_UYVY)    )   \

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_CHAR                        (M_MONO8     | M_SIGNED)
#define M_UCHAR                       (M_MONO8)
#define M_SHORT                       (M_MONO16    | M_SIGNED)
#define M_USHORT                      (M_MONO16)
#define M_LONG                        (M_MONO32    | M_SIGNED)
#define M_ULONG                       (M_MONO32)
#define M_RGB555                      (M_RGB15     | M_SIGNED)
#define M_URGB555                     (M_RGB15)
#define M_URGB888                     (M_RGB24)
#define M_BGR888                      (M_BGR24     | M_SIGNED)
#define M_RGB161616                   (M_RGB48     | M_SIGNED)
#define M_URGB161616                  (M_RGB48)
#define M_RGB323232                   (M_RGB96     | M_SIGNED)
#define M_URGB323232                  (M_RGB96)
#define M_YUV9_PLANAR                 (M_YUV9      | M_PLANAR)
#define M_YUV12_PLANAR                (M_YUV12     | M_PLANAR)
#define M_YUV16_PLANAR                (M_YUV16     | M_PLANAR)
#define M_YUV16_PACKED                (M_YUV16     | M_PACKED)
#define M_BGR15_PACKED                (M_BGR15     | M_PACKED)
#define M_RGB24_PACKED                (M_RGB24     | M_PACKED)
#define M_RGB32_PACKED                (M_RGB32     | M_PACKED)
#define M_BGR24_PACKED                (M_BGR24     | M_PACKED)
#define M_NODIBFLIP                   (M_FLIP)
#define M_DIB_BGR24_PACKED            (M_BGR24     | M_PACKED | M_FLIP | M_DIB)
#define M_BGR32_PACKED                (M_BGR32     | M_PACKED)
#endif

/************************************************************************/
/* MbufCreateColor() Values                                             */
/************************************************************************/
#define M_HOST_ADDRESS                                0x80000000L
#define M_PHYSICAL_ADDRESS                            0x40000000L
#define M_PITCH                                       0x20000000L
#define M_DEFAULT                                     0x10000000L
#define M_PITCH_BYTE                                  0x08000000L
#define M_BUF_ID                                      0x04000000L
#define M_BUF_ID_MODIFY                               0x02000000L
#define M_BUF_MD_ID                                   0x01000000L
#define M_DIRECTX_CREATION                            0x00800000L
#define M_PHYSICAL_ADDRESS_REMOTE                     0x00400000L
#define M_NO_MAP                                      0x00200000L
#define M_ALLOCATION                                  0x00100000L    // only used by MDbufCreateColor and not by MbufCreateColor (for now)
#define M_HOST_ADDRESS_REMOTE                         0x00080000L
#define M_FIRST_MIL_BUFFER                            0x00040000L
#define M_MIL_ID                                      0x00020000L
#define M_LL_ID                                       0x00010000L
#define M_ALIGNMENT_RESERVED_BITS                     0x000000FFL
#define M_ADDRESS_ALIGNMENT(n)                                (n)

/************************************************************************/
/* MRECT: structure used to define a rectangle coordinates              */
/************************************************************************/
// The MRECT structure defines the coordinates of the upper-left and lower-right corners of a rectangle.
typedef struct _MRECT {
   long left;
   long top;
   long right;
   long bottom;
   } MRECT;

typedef MRECT *LPMRECT;


/************************************************************************/
/* MbufBlit() Values                                                    */
/************************************************************************/

// A combination of flags that determine the valid members of the MBLTDESC structure.
#define M_BLT_REGION       0x00000001L  // Uses the lRegion member of the MBLTDESC structure to determine which region to copy.
#define M_BLT_MODE         0x00000002L  // Uses the lMode member of the MBLTDESC structure to determine if the blit is synchronous or not.
#define M_BLT_TYPE         0x00000004L  // Uses the lType member of the MBLTDESC structure to determine the type of the copy.
#define M_BLT_DESTRECT     0x00000008L  // Uses the lDestRect member of the MBLTDESC structure to determine the valid destination rectangle.
#define M_BLT_SRCRECT      0x00000010L  // Uses the lSrcRect member of the MBLTDESC structure to determine the valid source rectangle.

// Indicate which region (M_BLT_REGION) to copy from the source buffer to the destination buffer (the default is M_BLT_FRAME).
// The valid values for this member are:
#define M_BLT_FRAME        1  // Blit all lines of the source buffer in the destination buffer. ***DEFAULT***
#define M_BLT_ODD          2  // Blit all lines from the sources buffer to ODD lines of the destination buffer.
#define M_BLT_EVEN         3  // Blit all lines from the sources buffer to EVEN lines of the destination buffer.
#define M_BLT_ODD_ODD      4  // Blit ODD lines from the sources buffer to ODD lines of the destination buffer.
#define M_BLT_EVEN_EVEN    5  // Blit EVEN lines from the sources buffer to EVEN lines of the destination buffer.

// Indicate if the blit (M_BLT_MODE) is done asynchronously or synchronously (the default is synchronously). The valid value are:
#define M_BLT_ASYNC        10  // Performs this blit asynchronously.
#define M_BLT_SYNC         11  // Performs this blit synchronously. ***DEFAULT***

// Indicate which type (M_BLT_TYPE) of copy is performed (the default is a normal blit, M_SCRCOPY). The valid values are:
#define M_BLT_SRCCOPY               20  // Performs a normal blit. (only supported by M_BLT_FRAME) ***DEFAULT***
#define M_BLT_SRCKEYONBLACK         21  // Performs a blit with black keying. (only supported by M_BLT_FRAME)
#define M_BLT_SRCMIRRORLEFTRIGHT    22 // This blit mirrors the surface from left to right (y-axis).
#define M_BLT_SRCMIRRORUPDOWN       23 // This blit mirrors the surface from top to bottom (x-axis).
#define M_BLT_SRCMIRRORLRUPDN       24 // This blit mirrors the surface from top to bottom and from left to right.
#define M_BLT_SCALELINEAR           25
#define M_BLT_SCALENEAR             26
#define M_BLT_SCALECONSTANT         27
#define M_BLT_SCALEYUV2BGR          28
#define M_BLT_SCALEYUV2Y            29
#define M_BLT_SCALEYUV              30

#ifdef M_USE_MBUFBLIT
#if ! (M_MIL_USE_PPC || M_MIL_USE_LINUX)
#include <memory.h>  // for memset()
#include <mddraw.h>   // for lpdirectdrawsurface
#endif

// ****************************************************************************
//
// Description of the MBLTDESC_VER1 structure :
//
//    lSize :        Size of the structure, in bytes. This member must be
//                   initialized before the structure is used.
//    lValidMembers: A combination of flags that determine the valid members of
//                   the MBLTDESC structure.
//    lRegion:       Indicate which region to copy from the source buffer to
//                   the destiantion buffer (the default is M_BLT_FRAME).
//    lMode:         Indicate if the blit is done asynchronously or
//                   synchronously (the default is synchronously).
//    lType:         Indicate which type of copy is performed (the default
//                   is a normal blit, M_SCR_COPY).
//    DestRect:      RECT structure that defines the upper-left and lower-right
//                   points of the rectangle to blit to on the destination
//                   surface. If this parameter is set to ZERO, the entire
//                   destination surface is used.
//    SrcRect:       RECT structure that defines the upper-left and lower-right
//                   points of the rectangle to blit from on the source surface.
//                   If this parameter is set to ZERO, the entire source surface
//                   is used.
//    lpDDDestSurface: Address of the destination direct draw surface used for
//                   the copy. MIL will fill automaticaly this member is the blit
//                   is valid. Use this member if you want to know if an
//                   asynchronously blit is done (GetBltStatus(DDGBS_ISBLTDONE)).
//    lSrcType:      Indicate which type of protocole to use for the source (FB, PCI or AGP)
//    lDestType:     Indicate which type of protocole to use for the destination (FB, PCI or AGP)
//
// ****************************************************************************

#if !(M_MIL_USE_PPC || M_MIL_USE_LINUX)
typedef struct _MBLTDESC_VER1 {
   long        lSize;
   long	 	   lValidMembers;
   long  	  	lRegion;
   long  	  	lMode;
   long  	  	lType;
   MRECT     	DestRect;
   MRECT     	SrcRect;
   LPDIRECTDRAWSURFACE lpDDDestSurface;
   long		   lSrcType;
   long		   lDestType;
   } MBLTDESC_VER1;
typedef MBLTDESC_VER1 *LPMBLTDESC_VER1;

typedef MBLTDESC_VER1 MBLTDESC;

typedef MBLTDESC *LPMBLTDESC;
#endif // M_MIL_USE_PPC

#define M_INIT_STRUCT(x) (memset(&x,0,sizeof(x)), x.lSize=sizeof(x))
#endif // M_USE_MBUFBLIT


/************************************************************************/
/* MbufGet(), MbufPut(), MbufChild(), ...                               */
/************************************************************************/
#define M_RED                                         0x1000L
#define M_GREEN                                       0x2000L
#define M_BLUE                                        0x4000L
#define M_ALL_BAND                                    -1L
#define M_ALL_BANDS                                   M_ALL_BAND
#define M_HUE                                         0x00008000L
#define M_SATURATION                                  M_GREEN
#define M_LUMINANCE                                   0x00020000L   // M_GRAB_INPUT_GAIN
#define M_CHROMINANCE                                 0x00040000L   // M_GRAB_INPUT_GAIN
#define M_Y                                           M_RED
#define M_U                                           M_GREEN
#define M_V                                           M_BLUE
#define M_DITHERING                                   0x2000L
#define M_CBCR                                        0x4000L

#define M_ALL_BITS                                    -1L
#define M_DONT_CARE                                   0x00008000L

/************************************************************************/
/* MbufTransfer()                                                       */
/************************************************************************/
/*Transfer function value*/
#define M_DEFAULT                                     0x10000000L
#define M_CLEAR                                       0x00000001L
#define M_AUTO_SCALE                            2L  //0x00000002L
#define M_BIT_SHIFT                             3L  //0x00000003L
#define M_WINDOW_RANGE                       3072L  //0x00000C00L
#define M_COMPOSITION                                 0x00001000L
#define M_COPY_EVEN                                   0x00002000L
#define M_COPY_ODD                                    0x00004000L
#define M_COPY_TO_HWND                                0x00010000L
#define M_COPY                                        0x00020000L
#define M_REPLICATE                                   0x00040000L
#define M_SCALE                                       0x00008010L    // Can be add to some function like M_COPY

/*Transfer type*/
#define M_DEFAULT                                     0x10000000L
#define M_MIL_METHOD                                  0x01000000L
#define M_DIB_METHOD                                  0x02000000L
#define M_DIRECTX_METHOD                              0x04000000L
#define M_MTX0_METHOD                                 0x08000000L
#define M_SYNCHRONOUS                           1L  //0x00000001L
#define M_ASYNCHRONOUS                          2L  //0x00000002L
#define M_CAN_GROW_RECT                               0x00010000L
#define M_BILINEAR                                    0x00000008L
#define M_BICUBIC                                     0x00000010L
#define M_NEAREST_NEIGHBOR                            0x00000040L

/*Mask used to retreive field in the transfer type*/
#define M_TRANSFER_METHOD                             0xFF000000L
#define M_SCALING_METHOD                              0x00000078L

/*Structure used with autoscale in internal mode (passed to the function*/
/* via the extended void pointer*/
typedef struct _MBTAUTOSCALE {
   double      Minimum;
   double      Maximum;
   MIL_ID      LutId;
   } MBTAUTOSCALE;
typedef MBTAUTOSCALE *LPMBTAUTOSCALE;


/************************************************************************/
/* MbufImport(), MbufExport()                                           */
/************************************************************************/
#define M_RESTORE                                     0L
#define M_LOAD                                        1L

#define M_MIL                                         0L
#define M_RAW                                         1L
#define M_TIFF                                        2L
#define M_GIF                                         3L
#define M_BMP                                         4L
#define M_JPEG                                        5L // Internal use only
#define M_RIFF                                        6L // Internal use only
#define M_JPEG2K_RAW                                  7L // Internal use only
#define M_JPEG2K_JP2                                  8L // Internal use only
#define M_JPEG2K_ADV202                               9L // Internal use only

#define M_JPEG_LOSSLESS                               0x00400000L
#define M_JPEG_LOSSY                                  0x00800000L
#define M_JPEG_LOSSLESS_INTERLACED                    0x00C00000L
#define M_JPEG_LOSSY_INTERLACED                       0x08000000L
#define M_JPEG2000_LOSSY                              0x08400000L
#define M_JPEG2000_LOSSLESS                           0x08800000L

// Special file formats for import and export.
#define M_JPEG_LOSSY_RGB                              0x01000000L
#define M_JPEG2000_LOSSY_JP2                          M_JPEG2000_LOSSY+1
#define M_JPEG2000_LOSSLESS_JP2                       M_JPEG2000_LOSSLESS+1
#define M_JPEG2K_LOSSY_INTERLACED                     M_JPEG2000_LOSSY+2
#define M_JPEG2K_LOSSLESS_INTERLACED                  M_JPEG2000_LOSSLESS+2
#define M_ADV202_LOSSY                                M_JPEG2000_LOSSY+3
#define M_ADV202_LOSSY_INTERLACED                     M_JPEG2000_LOSSY+4

#define M_PLANAR                                      0x00040000L

#define M_INTERACTIVE                                 M_NULL


/************************************************************************/
/* MbufImportSequence(), MbufExportSequence()                           */
/************************************************************************/
#define M_RESTORE                                     0L
#define M_LOAD                                        1L

#define M_AVI_MIL                                     100L
#define M_AVI_DIB                                     101L // Standard Windows AVI files.
#define M_AVI_MJPG                                    102L
#define M_AVI_MJPEG                                   M_AVI_MJPG
#define M_DEFAULT                                     0x10000000L

// Import/Export Control Flags.
#define M_DEFAULT                                     0x10000000L         /* Create a file-Write/Read-Close           */
#define M_APPEND                                      0x20000000L         /* Open-Append-Close                        */
#define M_READ                                        1                   /* Read from an opened file.                */
#define M_WRITE                                       2                   /* Write into an opened file.               */
#define M_OPEN                                        0x00000010L         /* Open a file to Read/Write( with create ) */
#define M_CLOSE                                       0x00000020L         /* Close an open file.                      */
#define M_OPEN_APPEND                                 (M_OPEN + M_APPEND) /* Open an existing file to append into.    */

#define M_INTERACTIVE                                 M_NULL


/************************************************************************/
/* MbufLoad()                                                           */
/************************************************************************/
#define M_INTERACTIVE                                 M_NULL


/************************************************************************/
/* MbufSave()                                                           */
/************************************************************************/
#define M_INTERACTIVE                                 M_NULL


/************************************************************************/
/* MbufRestore()                                                        */
/************************************************************************/
#define M_INTERACTIVE                                 M_NULL


/************************************************************************/
/* MbufControlNeighborhood()                                            */
/************************************************************************/
#define M_ABSOLUTE_VALUE                              50L
#define M_NORMALIZATION_FACTOR                        52L
#define M_OVERSCAN                                    53L
#define M_OVERSCAN_REPLACE_VALUE                      54L
#define M_OFFSET_CENTER_X                             55L
#define M_OFFSET_CENTER_Y                             56L
#define M_ENABLE                                      -9997L
#define M_DISABLE                                     -9999L
#define M_TRANSPARENT                                 0x01000059L
#define M_REPLACE                                     0x01000060L
#define M_MIRROR                                      0x01000061L
#define M_REPLACE_MAX                                 0x01000063L
#define M_REPLACE_MIN                                 0x01000064L
#define M_SATURATION                                  M_GREEN


/************************************************************************/
/* MbufInquire() / MbufControl() Types                                  */
/************************************************************************/
#define M_OWNER_APPLICATION                           1000L
#define M_OWNER_SYSTEM                                1001L
#define M_SIZE_X                                      1002L
#define M_SIZE_Y                                      1003L
#define M_SIZE_Z                                      1004L
#define M_SIZE_BAND                                   1005L
#define M_SIZE_BAND_LUT                               1006L
#define M_SIZE_BIT                                    1007L
#define M_TYPE                                        1008L
#define M_NUMBER                                      1009L
#define M_FORMAT                                      1010L
#define M_FORMAT_SIZE                                 1011L
#define M_INIT_FLAG                                   1012L
#define M_ATTRIBUTE                                   1013L
#define M_SIGN                                        1014L
#define M_LUT_ID                                      1015L
#define M_NATIVE_ID                                   1016L
#define M_NATIVE_CONTROL_ID                           1017L
#define M_SURFACE                                     1021L
//1022L was used by M_DDRAW_SURFACE, that is now in the Buffer Display reserved area.
#define M_OWNER_SYSTEM_TYPE                           1023L
#define M_REAL_FORMAT                                 1024L
#define M_MIL_ID_INTERNAL                             1025L
#define M_EXTENDED_FORMAT                             1026L
#define M_EXTENDED_ATTRIBUTE                          1027L
#define M_DRIVER_ID                                   1028L
#define M_INTER_SYSTEM_COPY                           1029L
#define M_BUF_MODIFIED_HOOKED                         1030L
#define M_MAP_HOST_ADDRESS                            1031L
#define M_REFRESH_REMOTE_GRAB_BUFFERS                 1032L
#define M_REMOTE_GRAB_BUFFERS                         1033L
#define M_CALL_HOOKS                                  1034L
#define M_CALL_STATE_MACHINE_PRE_BUF_MODIFIED_HOOKS   1035L
#define M_CALL_STATE_MACHINE_POST_BUF_MODIFIED_HOOKS  1036L
#define M_REFRESH_REMOTE_GRAB_BUFFERS_AFTER_SET_SCHEME 1037L
#define M_DRIVER_ASYNC_CALL                           1038L
#define M_EFFECTIVE_SIZE_BIT                          1039L

#define M_GNL_ID                                      1130L
#define M_ONL_ID                                      M_GNL_ID

#define M_ALLOCATION_OVERSCAN_SIZE                    2311L
#define M_FAST_OVERSCAN_BUFFER                        2312L

#define M_INTER_SYSTEM_ID                             5000L
#define M_PARENT_ID                                   5001L
#define M_ANCESTOR_ID                                 5002L
#define M_PARENT_OFFSET_X                             5003L
#define M_PARENT_OFFSET_Y                             5004L
#define M_ANCESTOR_OFFSET_X                           5005L
#define M_ANCESTOR_OFFSET_Y                           5006L
#define M_PARENT_OFFSET_BAND                          5007L
#define M_ANCESTOR_OFFSET_BAND                        5008L
#define M_NB_CHILD                                    5009L
#define M_MODIFICATION_COUNT                          5010L
#define M_ANCESTOR_SIZE_X                             5012L
#define M_HOST_ADDRESS_FAR                            5013L
#define M_ASSOCIATED_LUT                              5014L
#define M_CURRENT_BUF_ID                              5015L
#define M_ASSOCIATED_BUFFER_ID                        5016L
#define M_MAP_BUFFER_TO_HOST                          5017L
#define M_HOST_ID                                     5020L
#define M_DMA_BUFFER                                  5021L
#define M_DMA_BUFFER_PTR                              5022L
#define M_DMA_BUFFER_PHYSICAL_PTR                     5023L
#define M_VALID_GRAB_BUFFER                           5025L
#define M_VALID_GRAB_BUFFER_OFFSET                    5026L
#define M_LOW_LEVEL_BUFFER_ID                         5027L
#define M_BUFFER_INFO                                 5028L
#define M_MEMBANK                                     5029L // Pulsar internal use only
#define M_LOCPOS_X                                    5030L // Pulsar internal use only
#define M_LOCPOS_Y                                    5031L // Pulsar internal use only
#define M_LOCPOS_BIT                                  5032L // Pulsar internal use only
#define M_ON_BOARD_DISP_BUFFER_NATIVE_ID              5033L // Pulsar internal use only
#define M_ON_BOARD_DISP_BUFFER_MIL_ID                 5034L // Pulsar internal use only
#define M_VGA_DISP_BUFFER_ID                          5035L // Pulsar internal use only
#define M_OVR_DISP_BUFFER_ID                          5036L // Pulsar internal use only
#define M_MEMORG                                      5037L // Pulsar internal use only
#define M_ANCESTOR_OFFSET_BIT                         5038L
#define M_DIB_MODE                                    5039L
#define M_FLIP_MODE                                   5040L
#define M_MODIFIED                                    5044L
#define M_COMPRESSION_TYPE                            5045L
#define M_HUFFMAN_DC_LUMINANCE                        5046L // Buffer M_COMPRESS Jpeg
#define M_HUFFMAN_AC_LUMINANCE                        5047L // Buffer M_COMPRESS Jpeg
#define M_HUFFMAN_DC_CHROMINANCE                      5048L // Buffer M_COMPRESS Jpeg
#define M_HUFFMAN_AC_CHROMINANCE                      5049L // Buffer M_COMPRESS Jpeg
#define M_HUFFMAN_DC                                  5050L // Buffer M_COMPRESS Jpeg
#define M_HUFFMAN_AC                                  5051L // Buffer M_COMPRESS Jpeg
#define M_PREDICTOR                                   5052L // Buffer M_COMPRESS Jpeg
#define M_QUANTIZATION_LUMINANCE                      5053L // Buffer M_COMPRESS Jpeg
#define M_QUANTIZATION_CHROMINANCE                    5054L // Buffer M_COMPRESS Jpeg
#define M_QUANTIZATION_OLD                            5055L // Buffer M_COMPRESS Jpeg
#define M_Q_FACTOR_LUMINANCE                          5056L // Buffer M_COMPRESS Jpeg
#define M_Q_FACTOR_CHROMINANCE                        5057L // Buffer M_COMPRESS Jpeg
#define M_Q_FACTOR_OLD                                5058L // Buffer M_COMPRESS Jpeg
#define M_RESTART_INTERVAL                            5059L // Buffer M_COMPRESS Jpeg
#define M_JPEG_HW_PROG                                5060L // Buffer M_COMPRESS Jpeg
#define M_SIZE_BYTE                                   5061L
#define M_PROC_HOST_ID                                5062L
#define M_INTERNAL_COMPRESSION_BUFFER_ID              5063L
#define M_INTERNAL_BUFFER_HOST_ADDRESS                5064L
#define M_INTERNAL_JPEG_FRAME_SIZEBYTE                5065L
#define M_INTERNAL_COMPRESSION_BUFFER_SIZE_DIVISION_FACTOR  5066L
#define M_SIZE_BYTE_FIRST_FIELD                       5067L
#define M_SIZE_BYTE_SECOND_FIELD                      5068L
#define M_JPEG_COMPRESS                               5069L // Buffer M_COMPRESS Jpeg
#define M_JPEG_DECOMPRESS                             5070L // Buffer M_COMPRESS Jpeg
#define M_REGENERATE_HEADER                           5071L // Internal Use Only.
#define M_PARSE_JPEG_DATA                             5072L // Internal Use Only.
#define M_HEADER_MODIFIED                             5073L // Internal Use Only.
#define M_TARGET_SIZE                                 5074L // Set internal compression buffer size in byte
#define M_SIZE_BYTE_PER_PIXEL                         5075L
#define M_ATTACHED_BUFFER_0_ID                        5076L
#define M_ATTACHED_BUFFER_1_ID                        5077L
#define M_ASSOCIATED_VGA_BUFFER_ID                    5078L
#define M_FIELD_UPDATE_MODE                           5079L
#define M_YCRCB_RANGE                                 5080L
#define M_EXTRA_BUFFER_INFO                           5081L // Used by MILDCOM, MIL doesn't anwser to this inquire
#define M_MODIFICATION_HOOK                           5082L
#define M_FLOAT_DECOMPRESSION                         5083L
#define M_DONE_IN_FLOAT                               5084L
#define M_REVERSE_X                                   5087L
#define M_REVERSE_Y                                   5088L
#define M_SUBSAMPLE_X                                 5089L
#define M_SUBSAMPLE_Y                                 5090L
#define M_SUBSAMPLE                                   5091L
#define M_BUFFER_INFO_OBJECT                          5092L // TBC Will be named M_BUFFER_INFO and changed also M_BUFFER_INFO_OBJECT_VIA_COPY
#define M_BUFFER_DISPLAY_INFO_PTR                     5093L
#define M_NUMBER_OF_BUFFERS                           5094L // Used with complex surface
#define M_GRAB_BUFFERS                                5095L // Internal Use Only.
#define M_USED_IN_CONTINUOUS_GRAB                     5096L
#define M_GRAB_TYPE                                   5097L
// Reserved    5098   M_SIZE_X_RED                    5098L
// Reserved    5099   M_SIZE_Y_RED                    5099L
// MTXDMA reserved area
#define M_BUF_MTXDMA_START                            5100L
#define M_BUFFER_CACHE                                5100L
#define M_BUFFER_CACHE_FLUSH                          5101L
#define M_BUFFER_AGP_CAPABLE                          5102L
#define M_WRITE_COMBINING                             5103L
#define M_BUF_MTXDMA_END                              5150L

// Display reserved area
#define M_BUF_DISPLAY_START                           5151L
#define M_IS_IN_DISPLAY_BUF_RANGE(X)     ((X>=M_BUF_DISPLAY_START) && (X<=M_BUF_DISPLAY_END))
#define M_DC_ALLOC                                    M_BUF_DISPLAY_START+0L
#define M_DC_FREE                                     M_BUF_DISPLAY_START+1L
#define M_DC_HANDLE                                   M_BUF_DISPLAY_START+2L
#define M_ENHANCE_FORMAT                              M_BUF_DISPLAY_START+3L
#define M_DDRAW_SURFACE                               M_BUF_DISPLAY_START+4L
#define M_DIRECTX_OWNER_COMPATIBLE                    M_BUF_DISPLAY_START+5L
#define M_MTX0_CAPABLE_DIRECTX_OWNER                  M_BUF_DISPLAY_START+6L
#define M_DIRECTX_OWNER                               M_BUF_DISPLAY_START+7L
#define M_ENHANCE_FROM_ID                             M_BUF_DISPLAY_START+8L
#define M_ENHANCE_MAX_DISPLAY_FORMAT                  M_BUF_DISPLAY_START+9L
#define M_VIDEO_SYSTEM_ID                             M_BUF_DISPLAY_START+10L
#define M_NEXT_FREE_BUFFER                            M_BUF_DISPLAY_START+11L
#define M_OLDEST_READY_BUFFER                         M_BUF_DISPLAY_START+12L
#define M_WAIT_ON_BUFFER_FREED                        M_BUF_DISPLAY_START+13L
#define M_ENHANCE_SYSTEM_CREATE                       M_BUF_DISPLAY_START+14L
#define M_ENHANCE_GRAB_CREATE                         M_BUF_DISPLAY_START+15L
#define M_FREE_ENHANCED_BUFFER                        M_BUF_DISPLAY_START+16L
#define M_ENHANCED_BUFFER                             M_BUF_DISPLAY_START+17L
#define M_INVALID_RECT                                M_BUF_DISPLAY_START+18L
#define M_VALID_RECT                                  M_BUF_DISPLAY_START+19L
#define M_STATE_MACHINE                               M_BUF_DISPLAY_START+20L
#define M_INTERNAL_BUFFERS                            M_BUF_DISPLAY_START+21L
#define M_NB_INTERNAL_BUFFERS                         M_BUF_DISPLAY_START+22L
#define M_STATE_MACHINE_MODE                          M_BUF_DISPLAY_START+23L
#define M_REGISTER_SELECT_FIELD                       M_BUF_DISPLAY_START+24L
#define M_UNREGISTER_SELECT_FIELD                     M_BUF_DISPLAY_START+25L
#define M_BITMAPINFO                                  M_BUF_DISPLAY_START+26L
#define M_LAST_MODIFIED_BUFFER                        M_BUF_DISPLAY_START+27L
#define M_DIRECTX_SURFACE                             M_BUF_DISPLAY_START+28L
//////////////MAXIMAL VALUE//////////////////////////////////////////////+49L
#define M_BUF_DISPLAY_END                             5200L

#define M_HOOK_TO_BUFFER_MODIFICATIONS                5201L
#define M_BUFFER_INFO_OBJECT_COPY                     5202L
#define M_CACHE_CONTROL                               5204L
#define M_HOST_ADDRESS_BAND                           5205L
/* Reserve next 16 values               from          5206L*/
/*                                      to            5221L*/
#define M_PHYS_ADDRESS_BAND                           5222L
/* Reserve next 16 values               from          5223L*/
/*                                      to            5237L*/
#define M_GRAB_BUFFERS_AFTER_SET_SCHEME               5238L
#define M_IS_PHYSICALLY_IN_CACHE                      5239L
#define M_NB_CHILD_SELECTED_ON_DISPLAY                5240L
#define M_UNSUPPORTED_FORMAT                          5241L
#define M_PREFERRED_DIGITIZER_FORMAT                  5242L

#define M_CACHE_FLUSH                                 8192L

// Used in JPEG2000.
#define M_FORWARD_WAVELET_HOOK                        8193L
#define M_FORWARD_WAVELET_HOOK_DATA                   8194L
#define M_REVERSE_WAVELET_HOOK                        8195L
#define M_REVERSE_WAVELET_HOOK_DATA                   8196L
#define M_COMPRESSION_START_HOOK                      8197L
#define M_COMPRESSION_START_HOOK_DATA                 8198L
#define M_COMPRESSION_END_HOOK                        8199L
#define M_COMPRESSION_END_HOOK_DATA                   8200L
#define M_DECOMPRESSION_START_HOOK                    8201L
#define M_DECOMPRESSION_START_HOOK_DATA               8202L
#define M_DECOMPRESSION_END_HOOK                      8203L
#define M_DECOMPRESSION_END_HOOK_DATA                 8204L
#define M_USER_DATA_SIZE                              8205L
#define M_USE_ADV202_FORMAT                           8206L

// Used both in JPEG and JPEG2000.
#define M_ALLOCATE_MEMORY                             8207L
#define M_SHRINK_MEMORY                               8208L
#define M_ALLOCATED_SIZE_BYTE                         8209L
#define M_COMPRESSED_DATA_SIZE_BYTE                   8210L

// The difference between the following defines must be >= 2. The bits
// 14, 13 and 12 (zero-based) of the flags must also always be cleared.
#define M_NUMBER_SUBBAND                              33000L
#define M_DECOMPOSITION_LEVEL                         33002L
#define M_Q_FACTOR                                    33004L
#define M_QUANTIZATION                                33006L

#define M_HOST_ADDRESS                                0x80000000L
#define M_HOST_ADDRESS_RED                            M_HOST_ADDRESS + M_RED    // 0x80001000L
#define M_HOST_ADDRESS_GREEN                          M_HOST_ADDRESS + M_GREEN  // 0x80002000L
#define M_HOST_ADDRESS_BLUE                           M_HOST_ADDRESS + M_BLUE   // 0x80004000L
#define M_HOST_ADDRESS_REMOTE                         0x00080000L
#define M_HOST_ADDRESS_REMOTE_RED                     M_HOST_ADDRESS_REMOTE + M_RED    // 0x00081000L
#define M_HOST_ADDRESS_REMOTE_GREEN                   M_HOST_ADDRESS_REMOTE + M_GREEN  // 0x00082000L
#define M_HOST_ADDRESS_REMOTE_BLUE                    M_HOST_ADDRESS_REMOTE + M_BLUE   // 0x00084000L


#define M_SIZE_X                                      1002L
#define M_SIZE_X_RED                                  M_SIZE_X + M_RED     // 0x000013eaL = 5098
#define M_SIZE_X_GREEN                                M_SIZE_X + M_GREEN   // 0x000023eaL = 9194
#define M_SIZE_X_BLUE                                 M_SIZE_X + M_BLUE    // 0x000043eaL = 17386

#define M_SIZE_Y                                      1003L
#define M_SIZE_Y_RED                                  M_SIZE_Y + M_RED     // 0x000013ebL = 5099
#define M_SIZE_Y_GREEN                                M_SIZE_Y + M_GREEN   // 0x000023ebL = 9195
#define M_SIZE_Y_BLUE                                 M_SIZE_Y + M_BLUE    // 0x000043ebL = 17387

#define M_PHYSICAL_ADDRESS                            0x40000000L
#define M_PITCH                                       0x20000000L
#define M_DEFAULT                                     0x10000000L
#define M_PITCH_BYTE                                  0x08000000L
#define M_BUF_ID                                      0x04000000L

// !!! MAP FOR OLD DEFINES
#if OldDefinesSupport
#define M_DMA_READ_HOST_ID                            M_HOST_ID
#define M_BUF_ASSOCIATED_BUFFER_ID                    M_ASSOCIATED_BUFFER_ID
#define M_WINDOW_DDRAW_SURFACE                        M_DDRAW_SURFACE
#define M_WINDOW_DC_ALLOC                             M_DC_ALLOC
#define M_WINDOW_DC_FREE                              M_DC_FREE
#define M_WINDOW_DC                                   M_DC_HANDLE
#endif

/************************************************************************/
/* MbufControl() MbufInquire() Values                                   */
/************************************************************************/
#define M_MAX_INTERNAL_BUFFERS                        8L

#define M_ODD_FIELD                                   1L
#define M_EVEN_FIELD                                  2L
#define M_BOTH_FIELDS                                 3L
#define M_NO_HOOK                            0x00000004L
#define M_GRAB_TYPE_PSEUDO                            5L
#define M_GRAB_TYPE_LIVE                              6L
#define M_GRAB_TYPE_PSEUDO_MULTIPLE_BUFFERS           7L
#define M_GRAB_TYPE_NO_TEARING                        8L
#define M_UNCONDITIONAL                               0L
#define M_CONDITIONAL                                 1L
#define M_NOT_IN_CACHE                                0L
#define M_IN_CACHE                                    1L
#define M_NOT_CACHABLE                                2L
#define M_CACHABLE                                    3L
// State machine type
#define M_INVALID                                    -1L
#define M_DEFAULT                            0x10000000L
#define M_WITH_VSYNC                                  1L
#define M_WITHOUT_VSYNC                               2L
#define M_BASIC                                       3L
// State machine mode
#define M_FRAME                                       30L
#define M_FIELD                                       46L

/************************************************************************/
/* MbufControlRegion() ControlTypes                                     */
/************************************************************************/
#define M_CACHE_CONTROL                            5204L
#define M_MODIFIED                                 5044L
#define M_GRAB_TIME_STAMP                    0x00000040L
#define M_DRIVER_ASYNC_CALL_END              0x80000000L
#define M_ALWAYS_SYNCHRONOUS                 0x40000000L
#define M_MODIFIED_WITH_ERROR                0x20000000L    
#define M_DRIVER_ASYNC_CALL_CANCELLED        0x10000000L    

/************************************************************************/
/* MbufControlRegion() ControlValues                                    */
/************************************************************************/
#define M_NOT_IN_CACHE                                0L
#define M_IN_CACHE                                    1L

#define M_NO_HOOK                            0x00000004L

#define M_ODD_FIELD                                   1L
#define M_EVEN_FIELD                                  2L
#define M_BOTH_FIELDS                                 3L


/************************************************************************/
/* MbufDiskInquire()                                                    */
/************************************************************************/
#define M_LUT_PRESENT                                 6000L
#define M_ASPECT_RATIO                                6001L
#define M_FRAME_RATE                                  6002L
#define M_NUMBER_OF_IMAGES                            6003L
#define M_FORMAT_SUPPORTED                            5180L


/* Common with other Inquire/Control */
#define M_ABSOLUTE_VALUE                              50L
#define M_NORMALIZATION_FACTOR                        52L
#define M_OVERSCAN                                    53L
#define M_OVERSCAN_REPLACE_VALUE                      54L
#define M_OFFSET_CENTER_X                             55L
#define M_OFFSET_CENTER_Y                             56L
#define M_SIZE_X                                      1002L
#define M_SIZE_Y                                      1003L
#define M_SIZE_BAND                                   1005L
#define M_SIZE_BIT                                    1007L
#define M_TYPE                                        1008L
#define M_ATTRIBUTE                                   1013L
#define M_SIGN                                        1014L
#define M_GREEN                                       0x2000L
#define M_SATURATION                                  M_GREEN
#define M_FILE_FORMAT                                 1015L
#ifdef M_MIL_75_COMPATIBLE
#define M_LUT                                         0x00000100L
#else
#define M_LUT                                         0x00040000L
#endif //M_MIL_75_COMPATIBLE
#define M_LUT_OLD                                     0x00000100L

/************************************************************************/
/* Lattice values                                                       */
/************************************************************************/
#define M_4_CONNECTED                                 0x00000010L
#define M_8_CONNECTED                                 0x00000020L


/************************************************************************/
/* Data types for results                                               */
/************************************************************************/
#define M_TYPE_CHAR                                   0x00010000L
#define M_TYPE_SHORT                                  0x00020000L
#define M_TYPE_LONG                                   0x00040000L
#define M_TYPE_FLOAT                                  0x00080000L
#define M_TYPE_DOUBLE                                 0x00100000L
#define M_TYPE_PTR                                    0x00200000L
#define M_TYPE_MIL_ID                                 0x00400000L
#define M_TYPE_STRING                                 0x00800000L
#define M_TYPE_STRING_PTR                             M_TYPE_STRING
#define M_TYPE_ASCII                                  M_TYPE_STRING
#define M_TYPE_BINARY                                 0x01000000L
#define M_TYPE_HEX                                    0x02000000L
#define M_TYPE_INT64                                  0x04000000L
#define M_TYPE_FILENAME                               0x08000000L
#define M_TYPE_ARRAY_ID_PTR                           0x10000000L


/* Bit encoded image types */
#define M_GREYSCALE                                   0x0200L
#define M_GRAYSCALE                                   M_GREYSCALE

/************************************************************************/
/* MgraAlloc()                                                           */
/************************************************************************/
#define M_DEFAULT_GRAPHIC_CONTEXT                     0x00FF0002L        //TBM MIL 8.0

/************************************************************************/
/* MgraFont()                                                           */
/************************************************************************/
#define M_FONT_DEFAULT_SMALL                          0L
#define M_FONT_DEFAULT_MEDIUM                         1L
#define M_FONT_DEFAULT_LARGE                          2L
#define M_FONT_DEFAULT                                M_FONT_DEFAULT_SMALL

/************************************************************************/
/* Used by MgraControl() / MgraInquire() Types                          */
/************************************************************************/
#define M_GRAPHIC_POSITION_X                          3L
#define M_GRAPHIC_POSITION_Y                          4L
#define M_COLOR                                       5L
#define M_BACKCOLOR                                   6L
#define M_RGB_COLOR                                   0x40000000L
#define M_RGB888(r,g,b)                               (M_RGB_COLOR|((unsigned long)(((unsigned char)(r)|((unsigned long)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16))))
#define M_RGB888_r(color)                             ((unsigned long)color&0x000000FF)
#define M_RGB888_g(color)                             (((unsigned long)color&0x0000FF00)>>8)
#define M_RGB888_b(color)                             (((unsigned long)color&0x00FF0000)>>16)
#define M_FONT                                        7L
#define M_FONT_X_SCALE                                8L
#define M_FONT_Y_SCALE                                9L
#define M_THICKNESS                                   10L
#define M_BACKGROUND_MODE                             12L
#define M_OPAQUE                                      0x01000058L
#define M_TRANSPARENT                                 0x01000059L
#define M_RGB_COLOR_MODE                              13L
#define M_RED_8_BIT                                   14L
#define M_GREEN_8_BIT                                 15L
#define M_BLUE_8_BIT                                  16L
#define M_BACK_RED_8_BIT                              17L
#define M_BACK_GREEN_8_BIT                            18L
#define M_BACK_BLUE_8_BIT                             19L

/********************************************************************/
/* Predefined colors                                                */
/********************************************************************/
#define M_COLOR_BLACK                                 M_RGB888(0,0,0)
#define M_COLOR_DARK_RED                              M_RGB888(128,0,0)
#define M_COLOR_DARK_GREEN                            M_RGB888(0,128,0)
#define M_COLOR_DARK_YELLOW                           M_RGB888(128,128,0)
#define M_COLOR_DARK_BLUE                             M_RGB888(0,0,128)
#define M_COLOR_DARK_MAGENTA                          M_RGB888(128,0,128)
#define M_COLOR_DARK_CYAN                             M_RGB888(0,128,128)
#define M_COLOR_BRIGHT_GREY                           M_RGB888(192,192,192)
#define M_COLOR_LIGHT_GREEN                           M_RGB888(192,220,192)
#define M_COLOR_LIGHT_BLUE                            M_RGB888(166,202,240)
#define M_COLOR_LIGHT_WHITE                           M_RGB888(255,251,240)
#define M_COLOR_LIGHT_GREY                            M_RGB888(160,160,164)
#define M_COLOR_DARK_GREY                             M_RGB888(128,128,128)
#define M_COLOR_RED                                   M_RGB888(255,0,0)
#define M_COLOR_GREEN                                 M_RGB888(0,255,0)
#define M_COLOR_YELLOW                                M_RGB888(255,255,0)
#define M_COLOR_BLUE                                  M_RGB888(0,0,255)
#define M_COLOR_MAGENTA                               M_RGB888(255,0,255)
#define M_COLOR_CYAN                                  M_RGB888(0,255,255)
#define M_COLOR_WHITE                                 M_RGB888(255,255,255)

/************************************************************************/
/* MappGetError()                                                       */
/************************************************************************/
#define M_NULL_ERROR                                  0L
#define M_NO_ERROR                                    0L
#define M_CURRENT                                     1L
#define M_CURRENT_FCT                                 2L
#define M_CURRENT_SUB_NB                              3L
#define M_CURRENT_SUB                                 4L
#define M_CURRENT_SUB_1                               4L
#define M_CURRENT_SUB_2                               5L
#define M_CURRENT_SUB_3                               6L
#define M_GLOBAL                                      7L
#define M_GLOBAL_FCT                                  8L
#define M_GLOBAL_SUB_NB                               9L
#define M_GLOBAL_SUB                                  10L
#define M_GLOBAL_SUB_1                                10L
#define M_GLOBAL_SUB_2                                11L
#define M_GLOBAL_SUB_3                                12L
#define M_INTERNAL                                    13L
#define M_INTERNAL_FCT                                14L
#define M_INTERNAL_SUB_NB                             15L
#define M_INTERNAL_SUB                                16L
#define M_INTERNAL_SUB_1                              16L
#define M_INTERNAL_SUB_2                              17L
#define M_INTERNAL_SUB_3                              18L
#define M_PARAM_NB                                    19L
#define M_FATAL                                       40L


#define M_UNHOOK                                      0x04000000L
#define M_FROM_HOOK                                   0x10000000L
#define M_MESSAGE                                     0x20000000L
#define M_ERROR                                       0x40000000L
#define M_NATIVE_ERROR                                0x80000000L
#define M_THREAD_RECURSIVE                            0x00800000L /* Bit field exclusive to M_TRACE  to M_PROCESSING      (3L to 6L) */
#define M_THREAD_CURRENT                              0x01000000L /*                        M_TRACE_START                       (1L) */
                                                                  /*                        M_TRACE_END                         (2L) */
                                                                  /*                        M_ERROR                     (0x40000000) */
                                                                  /*                        M_MESSAGE                   (0x20000000) */
                                                                  /*                        M_CURRENT to M_REGION_SIZE_Y (1L to 44L) */
                                                                  /*                        M_UNHOOK                    (0x04000000) */
#define M_ERROR_CURRENT                               (M_ERROR|M_CURRENT)
#define M_ERROR_GLOBAL                                (M_ERROR|M_GLOBAL)
#define M_ERROR_FATAL                                 (M_ERROR|M_FATAL)


/************************************************************************/
/* MbufGetHookInfo()  //MbufHookFunction()                              */
/************************************************************************/
#define M_BUFFER_ID                                           41L
#define M_REGION_OFFSET_X                                     42L
#define M_REGION_OFFSET_Y                                     43L
#define M_REGION_SIZE_X                                       44L
#define M_REGION_SIZE_Y                                       45L
#define M_FIELD                                               46L
#define M_BAND                                                47L
#define M_DONE_BY_DIRECTX                                     48L
#define M_GRAB_TIME_STAMP                             0x00000040L //64L
#define M_EVENT_TYPE                            M_REGION_OFFSET_X
#define M_PARAM1                                M_REGION_OFFSET_Y
#define M_PARAM2                                  M_REGION_SIZE_X
#define M_PARAM3                                  M_REGION_SIZE_Y

#define M_PRE_GRAB_BUFFER_CHANGE                              10L
#define M_POST_GRAB_BUFFER_CHANGE                             11L
#define M_MODIFIED_BUFFER_INTERNAL                            12L
#define M_REMOTE_GRAB_BUFFERS_REQUEST                         13L //TBR
#define M_REMOTE_GRAB_BUFFERS_SET                             14L //TBR
#define M_STATE_MACHINE_PRE_BUFFER_MODIFIED                   15L
#define M_STATE_MACHINE_POST_BUFFER_MODIFIED                  16L
#define M_REMOTE_GRAB_BUFFERS_REQUEST_AFTER_SET_SCHEME        17L //TBR
#define M_REMOTE_BUFFER_EVENT                                 18L //TBR
#define M_MODIFIED_BUFFER                             0x02000000L
#define M_UNHOOK                                      0x04000000L

// Hook redirected to the displays (use the M_DISP_BUF_HOOK flag)
#define M_DISP_BUF_HOOK                               0x00100000L
#define M_PRE_FLIP_COMPLEX_BUFFER                    (M_DISP_BUF_HOOK + 01L)
#define M_POST_FLIP_COMPLEX_BUFFER                   (M_DISP_BUF_HOOK + 02L)

//Remote Event types
#define M_EVENT_REMOTE_GRAB_BUFFERS_REQUEST                         1L
#define M_EVENT_REMOTE_GRAB_BUFFERS_SET                             2L
#define M_EVENT_REMOTE_GRAB_BUFFERS_REQUEST_AFTER_SET_SCHEME        3L
#define M_EVENT_REMOTE_GRAB_BUFFERS_UNSUPPORTED_FORMAT              4L

/************************************************************************/
/* MappAlloc                                                             */
/************************************************************************/
#define M_USER_OBJECT                                 0x00000001L
#define M_SYSTEM_OBJECT                               0x00000002L
#define M_INHERITED                                   0x00000004L
#define M_NOT_INHERITED                               0x00000008L
#define M_BROADCASTED                                 0x00000010L
#define M_NOT_BROADCASTED                             0x00000020L
#define M_OLD_DISPLAYS                                0x00000200L
#define M_NEW_DISPLAYS                                0x00000400L

#define M_FUNCTION_NAME_SIZE                          32L
#define M_ERROR_FUNCTION_NAME_SIZE                    M_FUNCTION_NAME_SIZE
#define M_ERROR_MESSAGE_SIZE                          128L

#define M_NBFCTNAMEMAX                                445 /* max number of function codes     */
#define M_NBERRMSGMAX                                 100 /* max number of error messages     */
#define M_NBSUBERRMSGMAX                              10  /* max number of sub error messages */

#define M_FUNC_ERROR                                  (M_NBERRMSGMAX+1L) /* M_MFUNC error numbers   */


/************************************************************************/
/* MappHookFunction()                                                   */
/************************************************************************/
#define M_ERROR_CURRENT                               (M_ERROR|M_CURRENT)
#define M_ERROR_GLOBAL                                (M_ERROR|M_GLOBAL)
#define M_TRACE_START                                 1L
#define M_TRACE_END                                   2L
#define M_ERROR_FATAL                                 (M_ERROR|M_FATAL)
#define M_UNHOOK                                      0x04000000L
#define M_THREAD_RECURSIVE                            0x00800000L /* Bit field exclusive to M_TRACE  to M_PROCESSING      (3L to 6L) */
#define M_THREAD_CURRENT                              0x01000000L /*                        M_TRACE_START                       (1L) */
#define M_INTERNAL_ALLOC                              0x00200000L /*                        M_TRACE_END                         (2L) */
#define M_INTERNAL_FREE                               0x00400000L /*                        M_ERROR                     (0x40000000) */
#define M_OBJECT_FREE                                 0x00010000L /*                        M_MESSAGE                   (0x20000000) */
                                                                  /*                        M_CURRENT to M_REGION_SIZE_Y (1L to 44L) */
                                                                  /*                        M_UNHOOK                    (0x04000000) */
#define M_VSYNC_SIGNAL                                0x00000100L /* For hook when a vsync occured on a host                         */
                                                                  /*  Range use: 0x00000100 to 0x0000010F                            */
                                                                  /*  The host system to use (device number) must be added to        */
                                                                  /*  M_VSYNC.  Exemple: to hook on the vsync of the 3rd host        */
                                                                  /*                system, use the hook type M_VSYNC_SIGNAL+M_DEV2  */
#define M_MAX_VGA_DEVICE                              16L
#define M_VSYNC_HOOK_SYSTEM_NUM(hooktype)             (hooktype&0x0000000F)   /* IMPORTANT these macros are base on the value of M_MAX_VGA_DEVICE */
#define M_IS_VSYNC_HOOK(hooktype)                     ((hooktype&(~(M_UNHOOK|0x0000000F)))==M_VSYNC_SIGNAL)
#define M_MIL_DISPLAY_DLL_LOAD                        0x000000200L

/************************************************************************/
/* MappGetHookInfo()                                                    */
/************************************************************************/
#define M_NULL_ERROR                                  0L
#define M_NO_ERROR                                    0L
#define M_CURRENT                                     1L
#define M_CURRENT_FCT                                 2L
#define M_CURRENT_SUB_NB                              3L
#define M_CURRENT_SUB                                 4L
#define M_CURRENT_SUB_1                               4L
#define M_CURRENT_SUB_2                               5L
#define M_CURRENT_SUB_3                               6L
#define M_GLOBAL                                      7L
#define M_GLOBAL_FCT                                  8L
#define M_GLOBAL_SUB_NB                               9L
#define M_GLOBAL_SUB                                  10L
#define M_GLOBAL_SUB_1                                10L
#define M_GLOBAL_SUB_2                                11L
#define M_GLOBAL_SUB_3                                12L
#define M_PARAM_NB                                    19L
#define M_BUFFER_ID                                   41L
#define M_REGION_OFFSET_X                             42L
#define M_REGION_OFFSET_Y                             43L
#define M_REGION_SIZE_X                               44L
#define M_REGION_SIZE_Y                               45L
#define M_OBJECT_ID                                   46L

#define M_PARAM_VALUE                                 0x08000000L
#define M_PARAM_TYPE                                  0x10000000L
#define M_PARAM_ATTRIBUTES                            0x00400000L
#define M_MESSAGE                                     0x20000000L



/************************************************************************/
/* MappInquire() / MappControl() Types                                  */
/* NB: All values in this range must be included between
                       M_APP_INQUIRE_SYS_START and M_APP_INQUIRE_SYS_END*/
/************************************************************************/

#ifndef M_MIL_75_COMPATIBLE
#define M_VERSION                                     (M_APP_INQUIRE_SYS_START+1L)
#define M_TRACE_HOOKS                                 (M_APP_INQUIRE_SYS_START+2L)
#define M_TRACE                                       (M_APP_INQUIRE_SYS_START+3L)
#define M_PARAMETER                                   (M_APP_INQUIRE_SYS_START+4L)
#define M_PROCESSING                                  (M_APP_INQUIRE_SYS_START+6L)
#define M_LAST_PLATFORM_USE                           (M_APP_INQUIRE_SYS_START+7L)
#define M_CURRENT_ERROR_HANDLER_PTR                   (M_APP_INQUIRE_SYS_START+8L)
#define M_CURRENT_ERROR_HANDLER_USER_PTR              (M_APP_INQUIRE_SYS_START+9L)
#define M_GLOBAL_ERROR_HANDLER_PTR                    (M_APP_INQUIRE_SYS_START+10L)
#define M_GLOBAL_ERROR_HANDLER_USER_PTR               (M_APP_INQUIRE_SYS_START+11L)
#define M_FATAL_ERROR_HANDLER_PTR                     (M_APP_INQUIRE_SYS_START+12L)
#define M_FATAL_ERROR_HANDLER_USER_PTR                (M_APP_INQUIRE_SYS_START+13L)
#define M_TRACE_START_HANDLER_PTR                     (M_APP_INQUIRE_SYS_START+14L)
#define M_TRACE_START_HANDLER_USER_PTR                (M_APP_INQUIRE_SYS_START+15L)
#define M_TRACE_END_HANDLER_PTR                       (M_APP_INQUIRE_SYS_START+16L)
#define M_TRACE_END_HANDLER_USER_PTR                  (M_APP_INQUIRE_SYS_START+17L)
#define M_IRQ_CONTROL                                 (M_APP_INQUIRE_SYS_START+18L)
#define M_ERROR_HANDLER_PTR                           (M_APP_INQUIRE_SYS_START+19L)
#define M_ERROR_HANDLER_USER_PTR                      (M_APP_INQUIRE_SYS_START+20L)
#define M_CURRENT_APPLICATION                         (M_APP_INQUIRE_SYS_START+21L)
#define M_MIL_UNIQUE_NUMBER                           (M_APP_INQUIRE_SYS_START+23L)
#define M_MIL_TRACE_FILENAME                          (M_APP_INQUIRE_SYS_START+24L)
#define M_FREE_ALL_DEFAULT                            (M_APP_INQUIRE_SYS_START+25L)
#define M_LICENSE_VALID_MODULES                       (M_APP_INQUIRE_SYS_START+26L)
#define M_LICENSE_UNLOCKED_MODULES                    M_LICENSE_VALID_MODULES
#define M_DEFAULT_VGA_SYSTEM                          (M_APP_INQUIRE_SYS_START+27L)
#define M_LCR_NUM                                     (M_APP_INQUIRE_SYS_START+28L)
#define M_LICENSE_TEMP_UNLOCKED_MODULES               (M_APP_INQUIRE_SYS_START+29L)
#define M_FORCE_DO_JOB                                (M_APP_INQUIRE_SYS_START+30L)
#define M_LST                                         (M_APP_INQUIRE_SYS_START+31L)
#define M_LSSTR                                       (M_APP_INQUIRE_SYS_START+32L)
#define M_IN_VERSION                                  (M_APP_INQUIRE_SYS_START+34L)
#define M_LST_STR                                     (M_APP_INQUIRE_SYS_START+35L)
#define M_LIC_ADD_RES                                 (M_APP_INQUIRE_SYS_START+36L)
#define M_LICENSE_MODULES                             (M_APP_INQUIRE_SYS_START+37L)
#define M_ERROR_CODE_PTR                              (M_APP_INQUIRE_SYS_START+38L)
#define M_INTERNAL_CALL_MODE                          (M_APP_INQUIRE_SYS_START+39L)
#define M_MILCE_ALLOC_IN_DMA_SIZE                     (M_APP_INQUIRE_SYS_START+40L)
#define M_MILCE_ALLOC_IN_MMF_SIZE                     (M_APP_INQUIRE_SYS_START+41L)
#define M_MILCE_ALLOC_IN_MMF_AUTO_COMMIT              (M_APP_INQUIRE_SYS_START+42L)
#define M_X86_FPU_FLAGS                               (M_APP_INQUIRE_SYS_START+43L)
#define M_REAL_TRACE_LEVEL                            (M_APP_INQUIRE_SYS_START+44L)
#define M_NON_PAGED_COMMUNICATION_BUFFER              (M_APP_INQUIRE_SYS_START+45L)
#define M_FORCE_THREAD_DETACH                         (M_APP_INQUIRE_SYS_START+46L)
#define M_BUFTRANSFER_METHOD                          (M_APP_INQUIRE_SYS_START+47L)
#define M_DISABLE_BUFTRANSFER_METHOD                  (M_APP_INQUIRE_SYS_START+48L)
#define M_RESET_DEFAULTS                              (M_APP_INQUIRE_SYS_START+49L)
#define M_USE_WINDOWED_CLASS                          (M_APP_INQUIRE_SYS_START+50L)
#define M_CACHE_AUTO_FLUSH                            (M_APP_INQUIRE_SYS_START+51L)
#define M_IN_HIGHLEVEL_MODULE                         (M_APP_INQUIRE_SYS_START+52L)
#define M_USING_OLD_DISP                              (M_APP_INQUIRE_SYS_START+53L)
#define M_MIL_DISPLAY_DLL_LOADED                      (M_APP_INQUIRE_SYS_START+54L)
#define M_LICENSE_VALID_SOFT_MODULES                  (M_APP_INQUIRE_SYS_START+55L)
#define M_ASSERT_NOT_OWNING_APP                       (M_APP_INQUIRE_SYS_START+56L)
#define M_TEMP_LICENSE_DAYS_LEFT                      (M_APP_INQUIRE_SYS_START+57L)
#else
#define M_VERSION                                     1L
#define M_TRACE                                       3L
#define M_PARAMETER                                   4L
#define M_MEMORY                                      5L
#define M_PROCESSING                                  6L
#define M_LAST_PLATFORM_USE                           7L
#define M_CURRENT_ERROR_HANDLER_PTR                   8L
#define M_CURRENT_ERROR_HANDLER_USER_PTR              9L
#define M_GLOBAL_ERROR_HANDLER_PTR                    10L
#define M_GLOBAL_ERROR_HANDLER_USER_PTR               11L
#define M_FATAL_ERROR_HANDLER_PTR                     12L
#define M_FATAL_ERROR_HANDLER_USER_PTR                13L
#define M_TRACE_START_HANDLER_PTR                     14L
#define M_TRACE_START_HANDLER_USER_PTR                15L
#define M_TRACE_END_HANDLER_PTR                       16L
#define M_TRACE_END_HANDLER_USER_PTR                  17L
#define M_IRQ_CONTROL                                 18L
#define M_ERROR_HANDLER_PTR                           19L
#define M_ERROR_HANDLER_USER_PTR                      20L
#define M_CURRENT_APPLICATION                         21L
#define M_MIL_UNIQUE_NUMBER                           23L
#define M_MIL_TRACE_FILENAME                          24L
#define M_FREE_ALL_DEFAULT                            25L
#define M_LICENSE_VALID_MODULES                       26L
#define M_LICENSE_UNLOCKED_MODULES                    M_LICENSE_VALID_MODULES
#define M_DEFAULT_VGA_SYSTEM                          27L
#define M_LCR_NUM                                     28L
#define M_LICENSE_TEMP_UNLOCKED_MODULES               29L
#define M_FORCE_DO_JOB                                30L
#define M_LST                                         31L
#define M_LSSTR                                       32L
#define M_IN_VERSION                                  34L
#define M_LST_STR                                     35L
#define M_LIC_ADD_RES                                 36L
#define M_LICENSE_MODULES                             37L
#define M_ERROR_CODE_PTR                              38L
#define M_INTERNAL_CALL_MODE                          39L
#define M_MILCE_ALLOC_IN_DMA_SIZE                     40L
#define M_MILCE_ALLOC_IN_MMF_SIZE                     41L
#define M_MILCE_ALLOC_IN_MMF_AUTO_COMMIT              42L
#define M_X86_FPU_FLAGS                               43L
#define M_REAL_TRACE_LEVEL                            44L
#define M_NON_PAGED_COMMUNICATION_BUFFER              45L
#define M_FORCE_THREAD_DETACH                         46L
#define M_BUFTRANSFER_METHOD                          47L
#define M_DISABLE_BUFTRANSFER_METHOD                  48L
#define M_RESET_DEFAULTS                              49L
#define M_USE_WINDOWED_CLASS                          50L
#define M_CACHE_AUTO_FLUSH                            51L
#define M_IN_HIGHLEVEL_MODULE                         52L
#define M_USING_OLD_DISP                              -1L
#define M_MIL_DISPLAY_DLL_LOADED                      -1L
#endif //M_MIL_75_COMPATIBLE

#define M_NUMBER_OF_BOARD_TYPES                       (M_APP_INQUIRE_SYS_START+63L)
#define M_LCRM                                        (M_APP_INQUIRE_SYS_START+64L)
/* Reserve next 31 values               from          (M_APP_INQUIRE_SYS_START+65L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+95)*/

#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY                                      (M_APP_INQUIRE_SYS_START+120L) /*5120*/ //Only 2 bits set. Used in bitwise operations
#else
#define M_MEMORY                         5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE

#define M_LCRID                                       (M_APP_INQUIRE_SYS_START+128L)
/* Reserve next 31 values               from          (M_APP_INQUIRE_SYS_START+129L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+159L)*/
#define M_LCRM_NOT                                    (M_APP_INQUIRE_SYS_START+160L)
/* Reserve next 31 values               from          (M_APP_INQUIRE_SYS_START+161L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+191L)*/
#define M_LCRT                                        (M_APP_INQUIRE_SYS_START+192L)
/* Reserve next 31 values               from          (M_APP_INQUIRE_SYS_START+193L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+223L)*/
#define M_LICENSE_FINGERPRINT                         (M_APP_INQUIRE_SYS_START+224L)
/* Reserve next 31 values               from          (M_APP_INQUIRE_SYS_START+225L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+255L)*/
#define M_NAME_OF_BOARD_TYPE                          (M_APP_INQUIRE_SYS_START+256L)
/* Reserve next 15 values               from          (M_APP_INQUIRE_SYS_START+257L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+271L)*/
#define M_DISPLAY_NAME_OF_BOARD_TYPE                  (M_APP_INQUIRE_SYS_START+272L)
/* Reserve next 15 values               from          (M_APP_INQUIRE_SYS_START+273L)*/
/*                                      to            (M_APP_INQUIRE_SYS_START+287L)*/
#define M_RESERVE_WORKSPACE                           (M_APP_INQUIRE_SYS_START+288L)
#define M_FREE_WORKSPACE                              (M_APP_INQUIRE_SYS_START+289L)
#define M_DEFAULT_HOST_FROM_ID                        (M_APP_INQUIRE_SYS_START+290L)
#define M_DEFAULT_WORKSPACE                           (M_APP_INQUIRE_SYS_START+291L)

// Display reserved area
/*Reserve values between M_APP_DISPLAY_START and M_APP_DISPLAY_END*/
#define M_APP_DISPLAY_START                          (M_APP_INQUIRE_SYS_START+300L)
// Value reserved for AppInquire/AppControl to the hardware manager (300 to 349)
#define M_APP_HM_DISPLAY_START                       M_APP_DISPLAY_START
#define M_MTX0_CAPABLE_DIRECTX_OBJECT                (M_APP_HM_DISPLAY_START+0L)      // 300
#define M_MTX0_CAPABLE_DIRECTX_OBJECT_COUNT          (M_APP_HM_DISPLAY_START+1L)      // 301
#define M_SYSTEM_ASSOCIATED_TO_DX_OBJECT             (M_APP_HM_DISPLAY_START+2L)      // 302
#define M_CLIENT_TO_MONITOR_RECT                     (M_APP_HM_DISPLAY_START+3L)      // 303
#define M_VGA_DEVICE_SYSTEM                          (M_APP_HM_DISPLAY_START+4L)      // 304 to 319
                                                                           //5L to 19L reserved for M_VGA_DEVICE_SYSTEM
#define M_DUAL_HEAD                                  (M_APP_HM_DISPLAY_START+20L)     // 320
#define M_MULTI_MONITOR                              (M_APP_HM_DISPLAY_START+21L)     // 321
#define M_NB_OF_VGA_DEVICE                           (M_APP_HM_DISPLAY_START+22L)     // 322
#define M_VGA_SYSTEM_FROM_PHYSICAL_ADDRESS           (M_APP_HM_DISPLAY_START+23L)     // 323
#define M_USE_DDRAW_UNDERLAY                         (M_APP_HM_DISPLAY_START+24L)     // 324
#define M_APP_HM_DISPLAY_END                         (M_APP_HM_DISPLAY_START+49L)
// Value reserved for AppInquire/AppControl to the DirectX display wrapper (350 to 374)
#define M_APP_DX_DISPLAY_START                       M_APP_HM_DISPLAY_END+1L
#define M_DIRECTX_VERSION                            M_APP_DX_DISPLAY_START+0L      // 350
#define M_DIRECTX_CAN_CREATE_OVER_HOST_MEMORY        M_APP_DX_DISPLAY_START+1L      // 351
#define M_APP_DX_DISPLAY_END                         M_APP_DX_DISPLAY_START+24L
// General purpose AppInquire/AppControl for MilDisplay (375 to 399)
#define M_APP_GENERAL_DISPLAY_START                  (M_APP_DX_DISPLAY_END+1L)         // 375
#define M_CAN_ALLOC_MTX0                             (M_APP_GENERAL_DISPLAY_START+0L)  // 375
#define M_DISPLAY_CONTROL_PANEL                      (M_APP_GENERAL_DISPLAY_START+1L)  // 376
// End of reserved value for display
#define M_APP_DISPLAY_END                            (M_APP_INQUIRE_SYS_START+399L)
#define M_IS_IN_DISPLAY_APP_HM_RANGE(X)  ((X>=M_APP_HM_DISPLAY_START) && (X<=M_APP_HM_DISPLAY_END))
#define M_IS_IN_DISPLAY_APP_DX_RANGE(X)  ((X>=M_APP_DX_DISPLAY_START) && (X<=M_APP_DX_DISPLAY_END))
#define M_IS_IN_DISPLAY_APP_RANGE(X)     ((X>=M_APP_DISPLAY_START) && (X<=M_APP_DISPLAY_END))


#define M_DIRECT_ACCESS_START                        (M_APP_INQUIRE_SYS_START+400L)
#define M_REG_DEF_START                              (M_APP_INQUIRE_SYS_START+400L)
/*Reserve values between M_REG_DEF_START and M_REG_DEF_END*/
#define M_REG_DEF_BOARD_NAME                         (M_APP_INQUIRE_SYS_START+401L)
#define M_REG_DEF_SYSTEM_NAME                        (M_APP_INQUIRE_SYS_START+402L)
#define M_REG_DEF_SYSTEM_DISPLAYABLE_NAME            (M_APP_INQUIRE_SYS_START+403L )
#define M_REG_DEF_SYSTEM_DEV                         (M_APP_INQUIRE_SYS_START+404L)
#define M_REG_DEF_BUFFER_SIZEBAND                    (M_APP_INQUIRE_SYS_START+405L)
#define M_REG_DEF_BUFFER_SIZEX                       (M_APP_INQUIRE_SYS_START+406L)
#define M_REG_DEF_BUFFER_SIZEY                       (M_APP_INQUIRE_SYS_START+407L)
#define M_REG_DEF_DISPLAY_DEV                        (M_APP_INQUIRE_SYS_START+408L)
#define M_REG_DEF_DISPLAY_TYPE                       (M_APP_INQUIRE_SYS_START+409L)
#define M_REG_DEF_DISPLAY_FORMAT                     (M_APP_INQUIRE_SYS_START+410L)
#define M_REG_DEF_DIG_DEV                            (M_APP_INQUIRE_SYS_START+411L)
#define M_REG_DEF_DIG_FORMAT                         (M_APP_INQUIRE_SYS_START+412L)
#define M_REG_DEF_USE_CURRENT_USER                   (M_APP_INQUIRE_SYS_START+413L)
#define M_REG_DEF_SYSTEM_CAN_GRAB                    (M_APP_INQUIRE_SYS_START+414L)
#define M_REG_DEF_END                                (M_APP_INQUIRE_SYS_START+430L)

#define M_REG_DRIVERS_PATH                           (M_APP_INQUIRE_SYS_START+431L)
#define M_REG_INSTALL_DIR                            (M_APP_INQUIRE_SYS_START+432L)
#define M_START_LICENSE_MANAGER                      (M_APP_INQUIRE_SYS_START+433L)
#define M_DUMP_ID_TABLE                              (M_APP_INQUIRE_SYS_START+434L)
#define M_DEFAULT_SYSTEM_ID                          (M_APP_INQUIRE_SYS_START+435L)
#define M_REG_DEF_RESET                              (M_APP_INQUIRE_SYS_START+436L)
#define M_DIRECT_ACCESS_END                          (M_APP_INQUIRE_SYS_START+499L)

#define M_NON_PAGED_MEMORY_FREE                      (M_APP_INQUIRE_SYS_START+500L)
#define M_NON_PAGED_MEMORY_USED                      (M_APP_INQUIRE_SYS_START+501L)
#define M_NON_PAGED_MEMORY_SIZE                      (M_APP_INQUIRE_SYS_START+502L)

#define M_USE_NEW_DISPLAYS                           (M_APP_INQUIRE_SYS_START+0x00000200L) /*512L*/
#define M_MEMORY_ALLOC_PRE_PADDING                   (M_APP_INQUIRE_SYS_START+513L)
#define M_MEMORY_ALLOC_POST_PADDING                  (M_APP_INQUIRE_SYS_START+514L)
#define M_MEMORY_ALLOC_PITCH_BYTE                    (M_APP_INQUIRE_SYS_START+515L)
#define M_MEMORY_ALLOC_ALIGNMENT                     (M_APP_INQUIRE_SYS_START+516L)

#define M_IMAGING_BOARD_TYPE                         (M_APP_INQUIRE_SYS_START+528L)
/* Reserve next 15 values               from         (M_APP_INQUIRE_SYS_START+529L)*/
/*                                      to           (M_APP_INQUIRE_SYS_START+543L)*/
#define M_LICENSER_LONG_NAME                         (M_APP_INQUIRE_SYS_START+544L)
/* Reserve next 31 values               from         (M_APP_INQUIRE_SYS_START+545L)*/
/*                                      to           (M_APP_INQUIRE_SYS_START+575L)*/
#define M_LICENSER_SHORT_NAME                        (M_APP_INQUIRE_SYS_START+576L)
/* Reserve next 31 values               from         (M_APP_INQUIRE_SYS_START+577L)*/
/*                                      to           (M_APP_INQUIRE_SYS_START+607L)*/
#define M_LICENSER_SCORE                             (M_APP_INQUIRE_SYS_START+608L)
/* Reserve next 31 values               from         (M_APP_INQUIRE_SYS_START+609L)*/
/*                                      to           (M_APP_INQUIRE_SYS_START+639L)*/
#define M_LCRM_SOFT                                  (M_APP_INQUIRE_SYS_START+640L)
/* Reserve next 15 values               from         (M_APP_INQUIRE_SYS_START+641L)*/
/*                                      to           (M_APP_INQUIRE_SYS_START+671L)*/



/************************************************************************/
/* Licensing: Module information                                        */
/************************************************************************/
#define M_LICENSE_LITE               0x00000001L //0L
#define M_LICENSE_DEBUG              0x00000002L //1L
#define M_LICENSE_IM                 0x00000004L //2L
#define M_LICENSE_CODE               0x00000008L //3L
#define M_LICENSE_MEAS               0x00000010L //4L
#define M_LICENSE_PAT                0x00000020L //5L
#define M_LICENSE_MOD                0x00000040L //6L
#define M_LICENSE_JPEG2000           0x00000080L //7L
#define M_LICENSE_BGA                0x00000100L //8L
#define M_LICENSE_BLOB               0x00000200L //9L
#define M_LICENSE_CAL                0x00000400L //10L
#define M_LICENSE_OCR                0x00000800L //11L
#define M_LICENSE_JPEGSTD            0x00001000L //12L
#define M_LICENSE_INSP3X             0x00200000L //21L
#define M_LICENSE_INSP40             0x00400000L //22L
#define M_LICENSE_METEOR_II_1394     0x00800000L //23L
#define M_LICENSE_EDGE               0x01000000L //24L

#define M_MATROX_VGA_FINGERPRINT                      (M_LICENSE_FINGERPRINT+2)
#define M_ETHERNET_FINGERPRINT                        (M_LICENSE_FINGERPRINT+7)
#define M_HARD_ID_KEY_FINGERPRINT                     (M_LICENSE_FINGERPRINT+8)
#define M_METEOR_II_1394_FINGERPRINT                  (M_LICENSE_FINGERPRINT+21)
#define M_CORONA_II_FINGERPRINT                       (M_LICENSE_FINGERPRINT+23)
#define M_ORION_FINGERPRINT                           (M_LICENSE_FINGERPRINT+24)
#define M_CRONOS_FINGERPRINT                          (M_LICENSE_FINGERPRINT+25)
#define M_ODYSSEY_FINGERPRINT                         (M_LICENSE_FINGERPRINT+26)
#define M_CRONOSPLUS_FINGERPRINT                      (M_LICENSE_FINGERPRINT+27)
#define M_HELIOS_FINGERPRINT                          (M_LICENSE_FINGERPRINT+28)
#define M_MORPHIS_FINGERPRINT                         (M_LICENSE_FINGERPRINT+29)
#define M_IRIS_FINGERPRINT                            (M_LICENSE_FINGERPRINT+30)
#define M_MATROX_BOARD_FINGERPRINT                    (M_LICENSE_FINGERPRINT+31)


#define M_MODIFIED_BUFFER_HANDLER_PTR                 0x10000000L   // Must not interfere with M_ERROR
#define M_MODIFIED_BUFFER_HANDLER_USER_PTR            0x20000000L   // Must not interfere with M_ERROR
#define M_OBJECT_TYPE                                 0x80000000L


/************************************************************************/
/* MWrite() / MpatRead() Types                                          */
/************************************************************************/
#define M_FILE                                        25L


/************************************************************************/
/* MappInquire() / MappControl() Values                                 */
/************************************************************************/
#define M_PARAMETER_CHECK                    0x00000001L
#define M_PRINT_DISABLE                               0L
#define M_PRINT_ENABLE                                1L
#define M_CHECK_DISABLE                               2L
#define M_CHECK_ENABLE                                3L
#define M_COMPENSATION_DISABLE                        4L
#define M_COMPENSATION_ENABLE                         5L
#define M_PRINT_INTERNAL                              6L
#define M_HOOK_CONTEXT_SIZE                           7L
#define M_ENABLE_FOR_ALL_FUNCTIONS                    8L
#define M_DISABLE_FOR_SOME_FUNCTIONS                  9L

#define M_THREAD_RECURSIVE                            0x00800000L /* Bit field exclusive to M_TRACE  to M_PROCESSING      (3L to 6L) */
#define M_THREAD_CURRENT                              0x01000000L /*                        M_TRACE_START                       (1L) */
                                                                  /*                        M_TRACE_END                         (2L) */
                                                                  /*                        M_ERROR                     (0x40000000) */
                                                                  /*                        M_MESSAGE                   (0x20000000) */
                                                                  /*                        M_CURRENT to M_REGION_SIZE_Y (1L to 44L) */
                                                                  /*                        M_UNHOOK                    (0x04000000) */

#define M_TIMER_ALLOC                                 1L
#define M_TIMER_FREE                                  2L
#define M_TIMER_RESOLUTION                            3L
#define M_TIMER_RESET                                 4L
#define M_TIMER_READ                                  5L
#define M_TIMER_WAIT                                  6L
#define M_TIMER_COMPENSATION                          7L
#define M_TIMER_WAIT_EXACT                            8L
#define M_TIMER_LOCK_TO_CPU                           9L
#define M_TIMER_UNLOCK_FROM_CPU                       10L
#define M_TIMER_SINGLE_CPU                            11L
#define M_TIMER_MULTI_CPU                             12L
#define M_TIMER_MIL_NOP                               0x8000L


/************************************************************************/
/* MappModify()                                                         */
/************************************************************************/
#define M_SWAP_ID                1L

/************************************************************************/
/* Binary functions in BLOB module.                                     */
/************************************************************************/
#define M_LENGTH                                      0x00002000L

/************************************************************************/
/* MmeasCalculate(), MmeasGetResult(), MpatGetResult() */
/************************************************************************/
#define M_ANGLE                                       0x00000800L
#define M_ORIENTATION                                 0x00002400L

/************************************************************************/
/* MblobControl() and/or MblobInquire() values and MmeasControl()       */
/************************************************************************/
#define M_PIXEL_ASPECT_RATIO                          5L

/***********************************************************************/
/* MfuncParam... defines                                               */
/***********************************************************************/
#define M_READ                                        1
#define M_WRITE                                       2
#define M_PA_FUNC_MAX_NB_PARAM                        M_NB_MAX_PARAMETER

/************************************************************************/
/* MfuncPrintMessage() defines                                          */
/************************************************************************/
#define M_RESP_YES                                    1L
#define M_RESP_NO                                     2L
#define M_RESP_CANCEL                                 4L
#define M_RESP_YES_NO                                 (M_RESP_YES|M_RESP_NO)
#define M_RESP_YES_NO_CANCEL                          (M_RESP_YES|M_RESP_NO|M_RESP_CANCEL)

/************************************************************************/
/* MfuncCalibrationInfo() defines                                       */
/************************************************************************/
#define M_COPY_CALIBRATION_INFO              3L
#define M_COMPARE_CALIBRATION_INFO           4L

#define M_GET_CALIBRATION_ID                 5L
#define M_GET_CORRECTION_STATE               7L
#define M_GET_PARENT_OFFSET_X                8L
#define M_GET_PARENT_OFFSET_Y                9L

#define M_CLEAR_CALIBRATION_ID              10L
#define M_DELETE_OBJECT_ID                  11L
#define M_CLEAR_OBJECT_ID                   12L

#define M_ADD_CALIBRATION_USE               20L
#define M_REMOVE_CALIBRATION_USE            21L
#define M_USE_CALIBRATION                   22L

//#ifdef M_MIL_75_COMPATIBLE
#define M_DISABLE_OBJECT                    40L   //TBR MIL 8.0
#define M_ENABLE_OBJECT                     41L
#define M_FORCE_DISABLING_LEVEL             43L
//#endif // M_MIL_75_COMPATIBLE

/************************************************************************/
/* MfuncAlloc defines                                                   */
/************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_USER_FUNCTION                     0x8000E000
#define M_USER_MODULE_1                     0x8000E200
#define M_USER_MODULE_2                     0x8000E400
#define M_USER_MODULE_3                     0x8000E600
#define M_USER_MODULE_4                     0x8000E800
#define M_USER_MODULE_5                     0x8000EA00
#define M_USER_MODULE_6                     0x8000EC00
#define M_USER_MODULE_7                     0x8000EE00
#else
#define M_USER_MODULE_1                     0x8000E000
#define M_USER_MODULE_2                     0x8000E200
#define M_USER_MODULE_3                     0x8000E400
#define M_USER_MODULE_4                     0x8000E600
#define M_USER_MODULE_5                     0x8000E800
#define M_USER_MODULE_6                     0x8000EA00
#define M_USER_MODULE_7                     0x8000EC00
//#define M_USER_MODULE_8                     0x8000EE00
#define M_USER_FUNCTION                     0x8000F200
#endif //M_MIL_75_COMPATIBLE
#define M_USER_MODULE_PA                    0x8000F000
#define M_USER_PA_FUNCTION                  M_USER_MODULE_PA
#define M_USER_MODULE_MASK                  0x8000FF00

#define M_ALLOC                             0x00010000
#define M_FREE                              0x00020000
#define M_SYNCHRONOUS_FUNCTION              0x00080000
#define M_ASYNCHRONOUS_FUNCTION             0x00000000
#define M_LOCAL                             0x01000000
#define M_REMOTE                            0x00000000
#define M_NB_MAX_PARAMETER                  16
#define M_NUMBER_OF_USER_MODULES            10L

#define M_CACHE_FLUSH                       8192L
#define M_NON_PAGED                         0x00200000L
#define M_PAGED                             0x02000000L
#define M_DEFAULT                           0x10000000L

/************************************************************************/
/* MfuncControl/MfuncInquire() defines                                  */
/************************************************************************/
#define M_USER_DATA_PTR                     1
#define M_DRIVER_INFO_PTR                   2
#define M_INTERNAL_CALL                     3
#define M_MIL_TIMER_INIT                    4
#define M_MIL_TIMER_DUMP                    5
/*Reserve                                   6
   and                                      7 */
#define M_ASSOCIATED_NAME                   8
#define M_RESULT_Q_PTR                      9
#define M_OBJECT_PTR                       10
#define M_FUNCTION_NAME_FROM_OPCODE        11
#define M_FUNCTION_NAME                    12
#define M_SLAVE_FUNCTION_PTR               13
#define M_SLAVE_DLL_NAME                   14
#define M_SLAVE_DLL_NAME_SIZE              15
#define M_SLAVE_FUNCTION_NAME              16
#define M_SLAVE_FUNCTION_NAME_SIZE         17
#define M_SLAVE_FUNCTION_OPCODE            18
#define M_INITFLAG                         19
#define M_PARAM_NUMBER                     20
#define M_USING_MODULES                    21
#define M_OBJECT_LOCK                      22
#define M_PUSH_TRACE_LEVEL                 23
#define M_POP_TRACE_LEVEL                  24


#define M_FUNCTION_NAME_SIZE               32L

#define M_PARAM_TYPE               0x10000000L
/* Reserve next 15 values   from   0x10000001L*/
/*                          to     0x1000000FL*/

#define M_OBJECT_TYPE              0x80000000L

/************************************************************************/
/* Mfile() defines                                                      */
/************************************************************************/
#define M_NO_MEMORY                                   1L


/************************************************************************/
/* MilGPU                                                               */
/************************************************************************/
  /* Vertex processing type */
#define M_SOFTWARE                                    20L
#define M_HARDWARE                                    21L
#define M_MIXED                                       22L


/************************************************************************/
/* MobjControl() defines                                                */
/************************************************************************/
#define M_MAP_HOOK_CONTEXT                             1L
#define M_INIT_HOOK_CONTEXT                            2L
#define M_ASSOCIATED_CALIBRATION                     125L   // already defined in milcal.h


/************************************************************************/
/* Special defines required to pass a pointer to a function that needs  */
/* a double. (use in McalControl to set a user string in the context)   */
/************************************************************************/
#define M_PTR_TO_DOUBLE(ptr)            ((double) MPtrToInt((void*)(ptr)))
#define M_STRING_TO_DOUBLE(ptr)         M_PTR_TO_DOUBLE((ptr))


/************************************************************************/
/* MIL OPTIONAL COMPILE DEFINES prefixes */
/************************************************************************/

#if M_MIL_USE_WINDOWS
   #define MFTYPE        _stdcall         // Exported Function type (32 bits)
   #define MFHOOKTYPE    _stdcall         // Same as MFTYPE, for internal use only.
   #define MFTYPE32                       // Exported Function type (32 bits)
   #define MFCTYPE       _stdcall         // Exported C Function type (32 bits)
   #define MFCTYPE32                      // Exported C Function type (32 bits)
   #define MFTYPEHT      _stdcall         // Non Exported Function type
   #define MPTYPE                         // Pointer and Reference type
   #define MFINLINE      inline           // Inline function type
   #define MHUGETYPE                      // To force HUGE pointers
   #define MFARTYPE                       // To force FAR type pointers
   #define MINTFTYPE     _stdcall         // Interrupt Function type (16 bits)
   #define MREGTYPE      register         // Register type variables
#elif M_MIL_USE_PPC || M_MIL_USE_LINUX
// Much more to define for MIL generation on PPC
   #define MFTYPE                         // Exported Function type (16 bits)
   #define MFHOOKTYPE                     // Same as MFTYPE, for internal use only.
   #define MFTYPE32                       // Exported Function type (32 bits)
   #define MFCTYPE                        // Exported C Function type (16 bits)
   #define MFCTYPE32                      // Exported C Function type (32 bits)
   #define MFTYPEHT                       // Non Exported Function type
   #define MPTYPE                         // Pointer and Reference type
   #define MFINLINE      inline           // Inline function type
   #define MHUGETYPE                      // To force HUGE pointers
   #define MFARTYPE                       // To force FAR type pointers
   #define MINTFTYPE     MFTYPE           // Interrupt Function type (16 bits)
   #define MREGTYPE      register         // Register type variables
#else
   #error Operating system not supported in Mil.h!
#endif



/************************************************************************/
/* MIL OS support                                                       */
/************************************************************************/
#include <milos.h>

/************************************************************************/
/* Temporary define                                                     */
/************************************************************************/
//Not activated for the moment
#if M_MIL_USE_64_BIT_ATTRIBUTES
typedef MIL_INT64 BUFATTRTYPE;
#else
typedef long BUFATTRTYPE;
#endif

/************************************************************************/
/* MIL command decoder functions */
#include <mildec.h>

#ifndef __midl /* IDL compiler used by ActiveMIL */

/************************************************************************/
/* MIL library prototypes */
#include <milproto.h>

/************************************************************************/
/* MIL custom-made header file*/
#include <mildyn.h>

#endif

/************************************************************************/
/* Make sure that M_MIL_USE_NT_WINDOWS is not defined (obsolete)        */
#ifdef M_MIL_USE_NT_WINDOWS
   #error M_MIL_USE_NT_WINDOWS is obsolete use M_MIL_USE_WINDOWS instead
#endif

#endif
