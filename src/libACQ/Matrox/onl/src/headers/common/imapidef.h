/*******************************************************************
 *
 * COPYRIGHT (c) 2000 Matrox Electronic Systems Ltd.
 * All Rights Reserved
 *
 *******************************************************************/

#ifndef __IMAPIDEF_H__
#define __IMAPIDEF_H__

#if !defined(_IM_LOCAL_CODE) && defined(SHOW_INCLUDE_FILE)
#pragma message ("#include "__FILE__)
#endif


/*******************************************************************
 *
 * Odyssey Native Library #defines.
 *
 *******************************************************************/

/* Initialization */
#define IM_PARTIAL_RESET                  1
#define IM_DOWNLOAD                       2
#define IM_DOWNLOAD_ONLY                  4

/* Messaging macro */
#define IM_PRINT_NO_WAIT(NodeId)          (((NodeId) << 16) |    0)
#define IM_PRINT_WAIT(NodeId)             (((NodeId) << 16) |    1)
#define IM_PRINT_WAIT_NO_ECHO(NodeId)     (((NodeId) << 16) |    2)
#define IM_PRINT_WAIT_ECHO(NodeId)        (((NodeId) << 16) |    4)
#define IM_PRINT_RETRY(NodeId)            (((NodeId) << 16) |    8)
#define IM_PRINT_NO_ECHO(NodeId)          (((NodeId) << 16) | 0x10)

/* Data formats */
#define IM_UNSIGNED                       0x00000000
#define IM_SIGNED                         0x10000000
#define IM_IEEE754                        0x20000000
#define IM_PACKED                         0x40000000
#define IM_ALIGN_ON_SIZE                  0x80000000       /* Internal use only */

/* Buffer types */
#define IM_BINARY                         (1L  | IM_PACKED)
#define IM_RGB                            (24L | IM_PACKED)
#define IM_UBYTE                          (8L  | IM_UNSIGNED)
#define IM_USHORT                         (16L | IM_UNSIGNED)
#define IM_ULONG                          (32L | IM_UNSIGNED)
#define IM_BYTE                           (8L  | IM_SIGNED)
#define IM_SHORT                          (16L | IM_SIGNED)
#define IM_LONG                           (32L | IM_SIGNED)
#define IM_FLOAT                          (32L | IM_IEEE754)
#define IM_DOUBLE                         (64L | IM_IEEE754)

/* Conditional operators */
#define IM_IN_RANGE                       1
#define IM_OUT_RANGE                      2
#define IM_EQUAL                          3
#define IM_NOT_EQUAL                      4
#define IM_GREATER                        5
#define IM_LESS                           6
#define IM_GREATER_OR_EQUAL               7
#define IM_LESS_OR_EQUAL                  8

/* Morphological operators */
#define IM_ERODE                          1
#define IM_DILATE                         2
#define IM_THIN                           3
#define IM_THICK                          4
#define IM_HIT_OR_MISS                    5
#define IM_MATCH                          6
#define IM_CLOSE                          10
#define IM_OPEN                           11

/* Arithmetic operators */
#define IM_ADD                            0x0000
#define IM_SUB                            0x0001
#define IM_SUB_NEG                        0x000A
#define IM_SUB_ABS                        0x0011
#define IM_MIN                            0x0012
#define IM_MAX                            0x0013
#define IM_OR                             0x0016
#define IM_AND                            0x0017
#define IM_XOR                            0x0018
#define IM_NOR                            0x0019
#define IM_NAND                           0x001A
#define IM_XNOR                           0x001B
#define IM_NOT                            0x0014
#define IM_NEG                            0x0023
#define IM_ABS                            0x000C
#define IM_PASS                           0x0002
#define IM_MULT                           0x0100
#define IM_DIV                            0x0101
#define IM_DIV_INTO                       0x0102

/* Additional arithmetic operators */
#define IM_LOG                            0x0000
#define IM_EXP                            0x0001
#define IM_SIN                            0x0002
#define IM_COS                            0x0003
#define IM_TAN                            0x0004
#define IM_QUAD                           0x0005

/* Yet more arithmetic operators */
#define IM_SUB_CLIP                       0x0081
#define IM_SHIFT                          0x0200
#define IM_ADD_SAT                        0x0201
#define IM_SUB_SAT                        0x0202
#define IM_MULT_SAT                       0x0203
#define IM_SQUARE                         0x0300
#define IM_SQRT                           0x0301
#define IM_ATAN                           0x0302
#define IM_CUBE                           0x0303
#define IM_CBRT                           0x0304
#define IM_SQUARE_ADD                     0x0305
#define IM_MULT_MSB                       0x0306
#define IM_DIV_FRAC                       0x0307
#define IM_ATAN2                          0x0308
#define IM_MAGNITUDE                      0x0309
#define IM_POW                            0x030A
#define IM_POW_YX                         0x030B


/* imGen1d() */
#define IM_POLYNOMIAL                     0x0400

/* Buffer inquiries */
#define IM_BUF_SIZE_X                     1
#define IM_BUF_SIZE_Y                     2
#define IM_BUF_NUM_BANDS                  3
#define IM_BUF_TYPE                       4
#define IM_BUF_SIZE_BIT                   5
#define IM_BUF_FORMAT                     6
#define IM_BUF_PARENT_ID                  7
#define IM_BUF_OFFSET_X                   8
#define IM_BUF_OFFSET_Y                   9
#define IM_BUF_OFFSET_BAND                10
#define IM_BUF_NUM_FIELDS                 11
#define IM_BUF_OWNER_ID                   12

/* Additional buffer inquiries */
#define IM_BUF_LOCATION                   13
#define IM_BUF_PHYSICAL_ADDRESS           14
#define IM_BUF_PITCH                      15
#define IM_BUF_PITCH_GRANULARITY          16
#define IM_BUF_BUF_POINTER                17
#define IM_BUF_BUF_JPEG_POINTER           18
#define IM_BUF_BAND_PTR                   19
#define IM_BUF_LOCAL_ADDRESS              20
#define IM_BUF_PARENT_POINTER             21
#define IM_BUF_PHYSICAL_ADDRESS64         22

/* System inquiries */
#define IM_SYS_NUM_NODES                  1
#define IM_SYS_NUM_DIGITIZERS             2
#define IM_SYS_NUM_DISPLAYS               3
#define IM_SYS_NUM_SYSTEMS                4
#define IM_SYS_PRODUCT_LIST               5
#define IM_SYS_PRODUCT_LIST_FREE          6
#define IM_SYS_GET_DEV_NUMBER             7

/* Device inquiries */
#define IM_DEV_CPU                        1
#define IM_DEV_CPU_SPEED                  2
#define IM_DEV_VIA_PRIMARY                3
#define IM_DEV_VIA_DISPLAY                4
#define IM_DEV_ACCELERATOR                5
#define IM_DEV_MEM_PROC                   6
#define IM_DEV_MEM_DISP                   7
#define IM_DEV_MEM_OVERLAY                8
#define IM_DEV_MEM_HOST                   9
#define IM_DEV_OWNER_SYSTEM               10
#define IM_DEV_OWNER_NODE                 11
#define IM_DEV_MSG_TOTAL                  12
#define IM_DEV_MSG_FREE                   13
#define IM_DEV_FREE_MEM_PROC              14
#define IM_DEV_FREE_MEM_DISP              15
#define IM_DEV_FREE_MEM_HOST              16
#define IM_DEV_FREE_MEM_PROC_FAST         17
#define IM_DEV_FREE_MEM_PROC_SHARED       18
#define IM_DEV_FREE_MEM_PROC_EXTENDED     19
#define IM_DEV_FREE_HEAP                  20
#define IM_DEV_PRODUCT_ID                 21
#define IM_DEV_PRODUCT_CLASS              22
#define IM_DEV_CUSTOMER_PRODUCT_ID        23
#define IM_DEV_PRODUCT_SERIAL_NUMBER      24
#define IM_DEV_CPU_TYPE                   25
#define IM_DEV_PITCH_GRANULARITY          26
#define IM_DEV_HEAP_SIZE                  27
#define IM_DEV_NUM_OF_STREAMS             28    /* Internal use */
#define IM_DEV_PERCENT_UTILIZATION        29

#define IM_DEV_PPC                        1

/* Device control */
#define IM_DEV_RAMDISK                    1
#define IM_DEV_TRACING                    2
#define IM_DEV_SET_MEMORY_CALIBRATION     3

/* Device inquiries product id */
#define IM_DEV_ODYSSEY_CLASS              0x10000
#define IM_DEV_ODYSSEY_PHASE_II           (IM_DEV_ODYSSEY_CLASS | 1)
#define IM_DEV_ODYSSEY_PROTO              (IM_DEV_ODYSSEY_CLASS | 2)
#define IM_DEV_ODYSSEY                    (IM_DEV_ODYSSEY_CLASS | 3)
#define IM_DEV_ODYSSEY_XCL                (IM_DEV_ODYSSEY_CLASS | 4)
#define IM_DEV_ODYSSEY_XA                 (IM_DEV_ODYSSEY_CLASS | 5)
#define IM_DEV_ODYSSEY_XD                 (IM_DEV_ODYSSEY_CLASS | 6)
#define IM_DEV_ODYSSEY_XPRO               (IM_DEV_ODYSSEY_CLASS | 7)
#define IM_DEV_ODYSSEY_XPRO_PROC          (IM_DEV_ODYSSEY_CLASS | 8)
#define IM_DEV_ODYSSEY_XPRO_FLEX          (IM_DEV_ODYSSEY_CLASS | 9)
#define IM_DEV_ODYSSEY_DISPLAY            (IM_DEV_ODYSSEY_CLASS | 10)
#define IM_DEV_ODYSSEY_CARRIER            (IM_DEV_ODYSSEY_CLASS | 11)
#define IM_DEV_ODYSSEY_UNDEF              (IM_DEV_ODYSSEY_CLASS | 99)

/* Externel Device inquiries board id */
#define IM_EXTDEV_NONE                    0
#define IM_EXTDEV_USER                    1
#define IM_EXTDEV_UNKNOWN                 2
#define IM_EXTDEV_CL_FULL                 3
#define IM_EXTDEV_CL_DUAL                 4
#define IM_EXTDEV_ANALOG                  5
#define IM_EXTDEV_DIGITAL                 6

#define IM_EXTREV_UNKNOWN                 0
#define IM_EXTREV_CL_FULL                 1
#define IM_EXTREV_XCL_FULL                2
#define IM_EXTREV_CL_DUAL                 3
#define IM_EXTREV_XCL_DUAL                4
#define IM_EXTREV_ANALOG_PMC              5
#define IM_EXTREV_ANALOG_PMC_REV2         6
#define IM_EXTREV_ANALOG_PMC_REV2_100     7
#define IM_EXTREV_ANALOG_XA               8
#define IM_EXTREV_ANALOG_XA_LC            9
#define IM_EXTREV_DIGITAL_PMC             10
#define IM_EXTREV_DIGITAL_XD              11


/* Link port speed */

#define IM_LINK_SPEED_PCI_PTHRU_16_66_MHZ    0
#define IM_LINK_SPEED_PCI_16_66_MHZ          1
#define IM_LINK_SPEED_PCI_33_33_MHZ          2
#define IM_LINK_SPEED_PCI_50_00_MHZ          3
#define IM_LINK_SPEED_PCI_66_66_MHZ          4
#define IM_LINK_SPEED_NOT_USED1              5
#define IM_LINK_SPEED_NOT_USED2              6
#define IM_LINK_SPEED_NOT_USED3              7
#define IM_LINK_SPEED_PCIX_PTHRU_16_66_MHZ   8
#define IM_LINK_SPEED_PCIX_16_66_MHZ         9
#define IM_LINK_SPEED_PCIX_33_33_MHZ         10
#define IM_LINK_SPEED_PCIX_50_00_MHZ         11
#define IM_LINK_SPEED_PCIX_66_66_MHZ         12
#define IM_LINK_SPEED_PCIX_83_33_MHZ         13
#define IM_LINK_SPEED_PCIX_100_00_MHZ        14
#define IM_LINK_SPEED_PCIX_133_33_MHZ        15
#define IM_LINK_SPEED_NOT_SET                16

/* Application inquires */
#define IM_APP_TIMEOUT                    1
#define IM_APP_STDOUT                     2

/* Miscellaneous defines */
#define IM_DEFAULT                        0x10000000
#define IM_NONE                           0x20000000
#define IM_DONT_CARE_32                   0x00ffffff
#define IM_NO_CHANGE                      -9999
#define IM_ALL                            0
#define IM_ENABLE                         -9997
#define IM_DISABLE                        -9999
#define IM_UNKNOWN                        -9999
#define IM_CONTINUOUS                     -1
#define IM_FORWARD                        1
#define IM_REVERSE                        2
#define IM_REVERSE_0                      0x10
#define IM_REVERSE_1                      0x20
#define IM_REVERSE_2                      0x40
#define IM_REVERSE_3                      0x80
#define IM_REVERSE_4                      0x100
#define IM_REVERSE_5                      0x200
#define IM_REVERSE_6                      0x400
#define IM_REVERSE_7                      0x800
#define IM_VERY_LOW                       0
#define IM_LOW                            1
#define IM_MEDIUM                         2
#define IM_HIGH                           3
#define IM_CLEAR                          1
#define IM_NO_CLEAR                       2
#define IM_IDEMPOTENCE                    -1
#define IM_FAST                           0x40000
#define IM_VERY_FAST                      0x80000
#define IM_BEST                           0x100000
#define IM_MULTIPLE                       0x400000
#define IM_ABSOLUTE                       1
#define IM_OFFSET                         2
#define IM_CLEAR_BACKGROUND               0x2000
#define IM_SYNCHRONOUS                    1
#define IM_ASYNCHRONOUS                   2
#define IM_FILL                           -1
#define IM_VIA_ONLY                       0
#define IM_ADDRESS_ONLY                   1
#define IM_SAVE                           2
#define IM_FASTEST                        3
#define IM_SAME_KERNEL                    4
#define IM_CACHE_BUF_SIZE                 0x1000
#define IM_PARENT                         0x40000000
#define IM_CALCULATE                      -9998

/* More miscellaneous stuff */
#define IM_NO_HALT                        0
#define IM_NOW                            1
#define IM_FRAME                          2
#define IM_FIELD                          3
#define IM_LINE                           4
#define IM_PATCH                          5
#define IM_PACK_0                         1
#define IM_UNPACK_0                       2
#define IM_PACK_1                         17
#define IM_UNPACK_1                       18
#define IM_DONT_CARE                      0x8000
#define IM_EXACT                          0x20000
#define IM_URGENT                         5
#define IM_SYNCHRONIZED                   1
#define IM_CLIP_SQUARE                    10
#define IM_FIRST_FIELD                    -1
#define IM_FIRST                          0
#define IM_SECOND                         1
#define IM_BOTH                           2
#define IM_MODE_2                         0x8000000
#define IM_THRESH_CONSTANT                0x100
#define IM_THRESH_PIXEL                   0x200
#define IM_PASS_CONSTANT                  0x400
#define IM_PASS_PIXEL                     0x800
#define IM_PIXEL                          1
#define IM_LINE_INTERLACED                0x10000000
#define IM_LINE_PSEUDO                    0x20000000

/* VIA-related values not defined elsewhere */
#define IM_24_TO_32                       1
#define IM_32_TO_24                       2
#define IM_PROGRESSIVE                    0
#define IM_INTERLACED                     1
#define IM_ODD                            0
#define IM_EVEN                           1
#define IM_NEXT                           2

/* Display control */
#define IM_KEY_OFF                        0
#define IM_KEY_ALWAYS                     1
#define IM_KEY_IN_RANGE                   2
#define IM_KEY_OUT_RANGE                  3

/* VGA Mode */
#define IM_SINGLE_SCREEN                  0
#define IM_DUAL_SCREEN                    1
#define IM_DUAL_HEAD                      2
#define IM_WINDOWED                       3 
#define IM_NO_DISPLAY                     4

/* File formats */
#define IM_MIL                            0
#define IM_RAW                            1
#define IM_TIFF                           2

/* Additional file formats */
#define IM_NATIVE                         10

/* Conversions */
#define IM_TRUNCATE                       1
#define IM_ROUND                          2
#define IM_ZERO_EXTEND                    4
#define IM_SIGN_EXTEND                    5
#define IM_CLIP                           0x80
#define IM_ABS_CLIP                       (IM_ABS | IM_CLIP)

/* imBufStatistics() */
#define IM_MAX_PIXEL                      0x00000001
#define IM_MIN_PIXEL                      0x00000002
#define IM_SUM_PIXEL                      0x00000004
#define IM_SUM_PIXEL_SQUARED              0x00000008
#define IM_MEAN_PIXEL                     0x00000010
#define IM_SIGMA_PIXEL                    0x00000020
#define IM_MIN_ABS_PIXEL                  0x00000040
#define IM_MAX_ABS_PIXEL                  0x00000080
#define IM_SUM_ABS_PIXEL                  0x00000100
#define IM_NUM_PIXEL                      0x00000200
#define IM_FIELDS_ONLY                    0x40000000
#define IM_NO_FIELDS                      0x80000000

/* imIntFlip() */
#define IM_FLIP_H                         0x0001
#define IM_FLIP_V                         0x0002
#define IM_ROTATE_90                      0x0010
#define IM_ROTATE_180                     0x0020
#define IM_ROTATE_270                     0x0030

/* imIntHistogramEqualize() */
#define IM_UNIFORM                        1
#define IM_EXPONENTIAL                    2
#define IM_RAYLEIGH                       3
#define IM_HYPER_CUBE_ROOT                4
#define IM_HYPER_LOG                      5
#define IM_IMAGE_TO_LUT                   1
#define IM_HIST_TO_LUT                    3

/* imIntRank() */
#define IM_MEDIAN                         0x10000

/* Geometric transforms */
#define IM_ROTATE                         0x401
#define IM_SCALE                          2
#define IM_TRANSLATE                      4
#define IM_SHEAR_X                        5
#define IM_SHEAR_Y                        6
#define IM_RECT_TO_POLAR                  7
#define IM_POLAR_TO_RECT                  8

/* Interpolation types */
#define IM_NO_INTERPOLATE                 0x00000002
#define IM_INTERPOLATE                    0x00000004
#define IM_BILINEAR                       0x00000008
#define IM_BICUBIC                        0x00000010
#define IM_AVERAGE                        0x00000020
#define IM_NEAREST_NEIGHBOR               0x00000040

/* Overscan or graphic modes */
#define IM_OPAQUE                         0x01000058
#define IM_TRANSPARENT                    0x01000059
#define IM_REPLACE                        0x01000060
#define IM_MIRROR                         0x01000061
#define IM_REPLACE_MAX                    0x01000063
#define IM_REPLACE_MIN                    0x01000064

/* Synchronization */
#define IM_WAITING                        0x1
#define IM_EXECUTING                      0x2
#define IM_READY                          0x4
#define IM_STARTED                        0x8
#define IM_COMPLETED                      0x10
#define IM_OSB_RESET                      0x20
#define IM_OSB_CANCEL                     0x40

#define IM_INQUIRE                        0x0800000
#define IM_LINE_INT                       0x1000000
#define IM_INFINITE                       -1
#define IM_NON_SIGNALLED                  IM_WAITING
#define IM_SIGNALLED                      IM_COMPLETED
#define IM_AUTO_RESET                     1
#define IM_MANUAL_RESET                   2

#define IM_SINGLE_NODE                    1
#define IM_MULTI_NODE                     2

/* Buffer location */
#define IM_PROC                           1
#define IM_DISP                           2
#define IM_HOST                           3
#define IM_NON_PAGED                      4
#define IM_NON_PAGED_VIRTUAL              5
#define IM_PROC_EXTENDED                  6
#define IM_PROC_FAST                      7
#define IM_PROC_SHARED                    8
#define IM_LOCAL                          9  // This case is for imBufCreate when we pass local address

#define IM_NO_FLUSH                       0x10000
#define IM_NO_READ_WRITE                  0x20000
#define IM_NO_WRITE                       0x40000

/* Buffer control */
#define IM_BUF_FLUSH                      1
#define IM_BUF_PROTECT                    2
#define IM_BUF_PAD                        3
// Also IM_BUF_PITCH                      15

// Protect values
#define IM_START                          0x01000000
#define IM_END                            0x02000000
#define IM_READ_ONLY                      0x04000000
#define IM_NO_ACCESS                      0x08000000


/* PP ALU opcodes for imBinTriadic() and imIntTriadic() */
#define IM_PP_ZERO                        0x00000000
#define IM_PP_ONE                         0x07f80000
#define IM_PP_PASS_A                      0x05500000
#define IM_PP_PASS                        IM_PP_PASS_A
#define IM_PP_NOT                         0x02a80000
#define IM_PP_AND                         0x04400000
#define IM_PP_OR                          0x07700000
#define IM_PP_XOR                         0x03300000
#define IM_PP_NAND                        0x03b80000
#define IM_PP_NOR                         0x00880000
#define IM_PP_XNOR                        0x04c80000
#define IM_PP_MERGE                       0x05600000
#define IM_PP_XOR_XOR                     0x04b00000

/* PP ALU opcodes for imIntTriadic() */
#define IM_PP_PASS_B                      0x06600000
#define IM_PP_PASS_C                      0x07800000
#define IM_PP_SRA_ADD                     0x0b510000
#define IM_PP_SRA_SUB                     0x0cad0000
#define IM_PP_ADD_AND                     0x0b500000
#define IM_PP_SUB_AND                     0x0cac0000
#define IM_PP_ADD_OR                      0x0ab00000
#define IM_PP_SUB_OR                      0x0d4c0000
#define IM_PP_ADD_ABS                     0x0cb30000
#define IM_PP_SUB_ABS                     0x0b4f0000
#define IM_PP_EXT_FIELD                   0x06000000
#define IM_PP_INS_FIELD                   0x05600000
#define IM_PP_ADD_FIELD                   0x0b000000

/* Pseudo-opcodes for imIntTriadic() */
#define IM_PP_ADD_ADD                     0xf0000f00
#define IM_PP_ADD_SUB                     0xf0000f01
#define IM_PP_SUB_SUB                     0xf0000f02

/* Colour conversion */
#define IM_RGB_TO_HSL                     1
#define IM_RGB_TO_L                       2
#define IM_HSL_TO_RGB                     3
#define IM_L_TO_RGB                       4
#define IM_RGB_TO_I                       5
#define IM_RGB_TO_H                       6
#define IM_RGB_TO_RGB                     7
#define IM_RGB_TO_BGR                     8
#define IM_BGR_TO_RGB                     IM_RGB_TO_BGR
#define IM_BAYER_GB                       10
#define IM_BAYER_BG                       11
#define IM_BAYER_GR                       12
#define IM_BAYER_RG                       13
#define IM_RGB_TO_Y                       20
#define IM_RGB_TO_YUV                     0x20
#define IM_RGB_TO_YUV16                   0x21
#define IM_RGB_TO_YUV12                   0x22
#define IM_RGB_TO_YUV9                    0x23
#define IM_Y_TO_RGB                       40
#define IM_YUV_TO_RGB                     0x40
#define IM_YUV16_TO_RGB                   0x41
#define IM_YUV12_TO_RGB                   0x42
#define IM_YUV9_TO_RGB                    0x43
#define IM_RGB_TO_RGB555                  0x50
#define IM_RGB_TO_RGB565                  0x51
#define IM_MATRIX                         0x100
#define _IM_RGB_TO_YCRCB                  0x200
#define _IM_RGB_TO_YCLIP                  0x201
#define _IM_YCRCB_TO_RGB                  0x202

/* Distance transforms */
#define IM_CHAMFER_3_4                    1
#define IM_CITY_BLOCK                     2
#define IM_CHESSBOARD                     3

/* Lattice values */
#define IM_4_CONNECTED                    0x10
#define IM_8_CONNECTED                    0x20

/* imIntTriadic() */
#define IM_CONSTANT_A                     0x100
#define IM_CONSTANT_B                     0x200
#define IM_CONSTANT_C                     0x400
#define IM_CONSTANT_AC                    (IM_CONSTANT_A | IM_CONSTANT_C)

/* Graphics control */
#define IM_PLOT_POLY                      1
#define IM_PLOT_LINES                     2
#define IM_PLOT_DOTS                      3
#define IM_FONT_DEFAULT                   1
#define IM_FONT_SMALL                     2
#define IM_FONT_MEDIUM                    3
#define IM_FONT_LARGE                     4

/* JPEG module */
#define IM_BASELINE                       0
#define IM_LOSSLESS                       3
#define IM_HUFFMAN                        0
#define IM_ARITHMETIC                     8
#define IM_RGB24                          0
#define IM_YUV24                          1
#define IM_YUV16                          2
#define IM_YUV12                          3
#define IM_YUV9                           4

/*************************************/
/* Pattern matching module           */
/*************************************/
#define IM_TEMPLATE                       0x0001
#define IM_NORMALIZED                     0x0002
#define IM_ROTATION                       0x0004

#define IM_PAT_FOUND_FLAG                 1
#define IM_PAT_SCORE                      2
#define IM_PAT_POSITION_X                 3
#define IM_PAT_POSITION_Y                 4
#define IM_PAT_ANGLE                      5
#define IM_PAT_SCALE                      6
#define IM_PAT_ORIENTATION                7

#define IM_PAT_TYPE                       1
#define IM_PAT_SIZE_X                     2
#define IM_PAT_SIZE_Y                     3
#define IM_PAT_CENTRE_X                   4
#define IM_PAT_CENTRE_Y                   5
#define IM_PAT_NOMINAL_X                  6
#define IM_PAT_NOMINAL_Y                  7
#define IM_PAT_SPEED                      8
#define IM_PAT_POSITION_START_X           9
#define IM_PAT_POSITION_START_Y           10
#define IM_PAT_POSITION_SIZE_X            11
#define IM_PAT_POSITION_SIZE_Y            12
#define IM_PAT_POSITION_ACCURACY          13
#define IM_PAT_PREPROCESSED               14
#define IM_PAT_OFFSET_X                   15
#define IM_PAT_OFFSET_Y                   16
#define IM_PAT_ACCEPTANCE                 17
#define IM_PAT_NUMBER                     18
#define IM_PAT_WORKBUF_SIZE_X             19
#define IM_PAT_WORKBUF_SIZE_Y             20
#define IM_PAT_WORKBUF_FAST               21
#define IM_PAT_WORKBUF_ROTATION           22
#define IM_PAT_WORKBUF                    23
#define IM_PAT_RESULT_SIZE                24
#define IM_PAT_CERTAINTY                  25

#define IM_PAT_FIRST_LEVEL                31
#define IM_PAT_LAST_LEVEL                 32
#define IM_PAT_MODEL_STEP                 33
#define IM_PAT_FAST_FIND                  34
#define IM_PAT_MIN_SPACING_X              35
#define IM_PAT_MIN_SPACING_Y              36
#define IM_PAT_SCORE_TYPE                 37
#define IM_PAT_FILE_TYPE                  38

#define IM_PAT_MODEL_POINTER              39
#define IM_PAT_RESULT_POINTER             40
#define IM_PAT_REJECTION                  41
#define IM_PAT_BEST_REJECT_SCORE          42
#define IM_PAT_BEST_REJECT_LEVEL          43

#define IM_PAT_CENTER_X                   IM_PAT_CENTRE_X
#define IM_PAT_CENTER_Y                   IM_PAT_CENTRE_Y
#define IM_PAT_ORIGINAL_X                 IM_PAT_NOMINAL_X
#define IM_PAT_ORIGINAL_Y                 IM_PAT_NOMINAL_Y

/**********************************/
/* Blob Analysis module           */
/**********************************/
/* Binary features */
#define IM_BLOB_LABEL_VALUE               1
#define IM_BLOB_AREA                      2
#define IM_BLOB_PERIMETER                 3
#define IM_BLOB_FERET_X                   4
#define IM_BLOB_FERET_Y                   5
#define IM_BLOB_BOX_X_MIN                 6
#define IM_BLOB_BOX_Y_MIN                 7
#define IM_BLOB_BOX_X_MAX                 8
#define IM_BLOB_BOX_Y_MAX                 9
#define IM_BLOB_FIRST_POINT_X             10
#define IM_BLOB_FIRST_POINT_Y             11
#define IM_BLOB_AXIS_PRINCIPAL_LENGTH     12
#define IM_BLOB_AXIS_SECONDARY_LENGTH     13
#define IM_BLOB_FERET_MIN_DIAMETER        14
#define IM_BLOB_FERET_MIN_ANGLE           15
#define IM_BLOB_FERET_MAX_DIAMETER        16
#define IM_BLOB_FERET_MAX_ANGLE           17
#define IM_BLOB_FERET_MEAN_DIAMETER       18
#define IM_BLOB_CONVEX_AREA               19
#define IM_BLOB_CONVEX_PERIMETER          20
#define IM_BLOB_X_MIN_AT_Y_MIN            21
#define IM_BLOB_X_MAX_AT_Y_MAX            22
#define IM_BLOB_Y_MIN_AT_X_MAX            23
#define IM_BLOB_Y_MAX_AT_X_MIN            24
#define IM_BLOB_COMPACTNESS               25
#define IM_BLOB_NUMBER_OF_HOLES           26
#define IM_BLOB_FERET_ELONGATION          27
#define IM_BLOB_ROUGHNESS                 28
#define IM_BLOB_EULER_NUMBER              47
#define IM_BLOB_LENGTH                    48
#define IM_BLOB_BREADTH                   49
#define IM_BLOB_ELONGATION                50
#define IM_BLOB_INTERCEPT_0               51
#define IM_BLOB_INTERCEPT_45              52
#define IM_BLOB_INTERCEPT_90              53
#define IM_BLOB_INTERCEPT_135             54
#define IM_BLOB_NUMBER_OF_RUNS            55
#define IM_BLOB_ASPECT_RATIO              IM_BLOB_FERET_ELONGATION
#define IM_BLOB_GENERAL_FERET             0x400

/* Greyscale features */
#define IM_BLOB_SUM_PIXEL                 29
#define IM_BLOB_MIN_PIXEL                 30
#define IM_BLOB_MAX_PIXEL                 31
#define IM_BLOB_MEAN_PIXEL                32
#define IM_BLOB_SIGMA_PIXEL               33
#define IM_BLOB_SUM_PIXEL_SQUARED         46

/* Binary or greyscale features */
#define IM_BLOB_CENTER_OF_GRAVITY_X       34
#define IM_BLOB_CENTER_OF_GRAVITY_Y       35
#define IM_BLOB_MOMENT_X0_Y1              36
#define IM_BLOB_MOMENT_X1_Y0              37
#define IM_BLOB_MOMENT_X1_Y1              38
#define IM_BLOB_MOMENT_X0_Y2              39
#define IM_BLOB_MOMENT_X2_Y0              40
#define IM_BLOB_MOMENT_CENTRAL_X1_Y1      41
#define IM_BLOB_MOMENT_CENTRAL_X0_Y2      42
#define IM_BLOB_MOMENT_CENTRAL_X2_Y0      43
#define IM_BLOB_AXIS_PRINCIPAL_ANGLE      44
#define IM_BLOB_AXIS_SECONDARY_ANGLE      45
#define IM_BLOB_GENERAL_MOMENT            0x800

/* Short cuts for enabling multiple features */
#define IM_BLOB_ALL_FEATURES              0x100
#define IM_BLOB_BOX                       0x101
#define IM_BLOB_CONTACT_POINTS            0x102
#define IM_BLOB_CENTER_OF_GRAVITY         0x103
#define IM_BLOB_NO_FEATURES               0x104

/* Groups of results that can be read back together */
#define IM_BLOB_GROUP1                    0x201
#define IM_BLOB_GROUP2                    0x202
#define IM_BLOB_GROUP3                    0x203
#define IM_BLOB_GROUP4                    0x204
#define IM_BLOB_GROUP5                    0x205
#define IM_BLOB_GROUP6                    0x206
#define IM_BLOB_GROUP7                    0x207
#define IM_BLOB_GROUP8                    0x208
#define IM_BLOB_GROUP9                    0x209

/* imBlobControl() and/or imBlobInquire() */
#define IM_BLOB_IMAGE_TYPE                1
#define IM_BLOB_IDENTIFICATION            2
#define IM_BLOB_LATTICE                   3
#define IM_BLOB_FOREGROUND_VALUE          4
#define IM_BLOB_PIXEL_ASPECT_RATIO        5
#define IM_BLOB_NUMBER_OF_FERETS          6
#define IM_BLOB_RESET                     9
#define IM_BLOB_SAVE_RUNS                 14
#define IM_BLOB_MAX_LABEL                 16
#define IM_BLOB_MAX_TIME                  17
#define IM_BLOB_TIMEOUT                   18
#define IM_BLOB_TIME_SLICE                19

/* General moment type */
#define IM_ORDINARY                       0x400
#define IM_CENTRAL                        0x800

/* Blob identification */
#define IM_WHOLE_IMAGE                    1
#define IM_INDIVIDUAL                     2
#define IM_LABELLED                       4

/* Foreground values */
#define IM_NONZERO                        0x80
#define IM_ZERO                           0x100
#define IM_NON_ZERO                       IM_NONZERO

/* Image type values (and modifiers for certain features) */
#define IM_GREYSCALE                      0x0200
#define IM_GRAYSCALE                      IM_GREYSCALE

/* Data types for results */
#define IM_TYPE_CHAR                      0x10000
#define IM_TYPE_SHORT                     0x20000
#define IM_TYPE_LONG                      0x40000
#define IM_TYPE_FLOAT                     0x80000
#define IM_TYPE_DOUBLE                    0x100000

/* Miscellaneous */
#define IM_ALWAYS                         0
#define IM_ALL_BLOBS                      0x105
#define IM_INCLUDED_BLOBS                 0x106
#define IM_EXCLUDED_BLOBS                 0x107
#define IM_INCLUDE                        1
#define IM_EXCLUDE                        2
#define IM_DELETE                         3
#define IM_COMPRESS                       8
#define IM_MIN_FERETS                     2
#define IM_MAX_FERETS                     64
#define IM_INCLUDE_ONLY                   0x101
#define IM_EXCLUDE_ONLY                   0x102
/*********************************/
/* End of Blob Analysis #defines */
/*********************************/

/* Error handling */
#define IM_ERR_RESET                      0x1
#define IM_ERR_CODE                       0x2
#define IM_ERR_MSG                        0x4
#define IM_ERR_FUNC                       0x8
#define IM_ERR_MSG_FUNC                   0xc
#define IM_ERR_MSG_SIZE                   80                      /* Multiple of 8 for endian portability */
#define IM_ERR_FUNC_SIZE                  32                      /* Multiple of 8 for endian portability */
#define IM_ERR_SIZE                       (2 * IM_ERR_MSG_SIZE)   /* to hold both message and function plus some extra */

/* Error codes */
#define IM_SUCCESS                         0
#define IM_ERR_BUFFER                     -1
#define IM_ERR_DEVICE                     -2
#define IM_ERR_FILE                       -3
#define IM_ERR_MEMORY                     -4
#define IM_ERR_NOT_PRESENT                -5
#define IM_ERR_OPCODE                     -6
#define IM_ERR_OSB                        -7
#define IM_ERR_PARAMETER                  -8
#define IM_ERR_RESTRICTION                -9
#define IM_ERR_THREAD                     -10
#define IM_ERR_TIMEOUT                    -11
#define IM_ERR_SYSTEM                     -12
#define IM_ERR_HALTED                     -13
#define IM_ERR_BUF_ATTRIBUTE              -14
#define IM_ERR_MISC                       -15
#define IM_ERR_INTERNODE                  -16
#define IM_ERR_BUF_PARAM                  IM_ERR_BUF_ATTRIBUTE

/*******************************************************************
 * Pre-defined pseudo-buffers.
 * These can be passed in place of a buffer only where documented.
 *******************************************************************/

/* Flag bit to identify pre-defined buffers */
#define IM_BUF_PREDEFINED                 0x80000000

/* Miscellaneous */
/* IM_LINK_MEMORY is to say to the router to put the stream in memory */
#define IM_VM_CHANNEL                     (IM_BUF_PREDEFINED | 0x00)
#define IM_PCI_TRANSFER_WINDOW            (IM_BUF_PREDEFINED | 0x01)
#define IM_DISP_TEXT                      (IM_BUF_PREDEFINED | 0x02)
#define IM_LINK_0                         (IM_BUF_PREDEFINED | 0x03)
#define IM_LINK_1                         (IM_BUF_PREDEFINED | 0x04)
#define IM_LINK_2                         (IM_BUF_PREDEFINED | 0x05)
#define IM_LINK_3                         (IM_BUF_PREDEFINED | 0x06)
#define IM_LINK_MEMORY                    (IM_BUF_PREDEFINED | 0x07)

/* the internal predefined buffers go here */
/* do not use any define from 0x10 to 0x1F */
/* they are reserved to pass the priority of the default thread */
/* The priority will be in the range of the user thread (1 to 15) */
#define _IM_DEFAULT_THREAD                (IM_BUF_PREDEFINED | 0x10)
/* imBufChild() */
#define IM_DISP_MONO                      (IM_BUF_PREDEFINED | 0x20)
#define IM_DISP_COLOR                     (IM_BUF_PREDEFINED | 0x30)
#define IM_DISP_RED                       (IM_BUF_PREDEFINED | 0x40)
#define IM_DISP_GREEN                     (IM_BUF_PREDEFINED | 0x50)
#define IM_DISP_BLUE                      (IM_BUF_PREDEFINED | 0x60)
#define IM_DISP_OVERLAY                   (IM_BUF_PREDEFINED | 0x70)

/* imIntConvolve() */
#define IM_SMOOTH                         (IM_BUF_PREDEFINED | 0x80)
#define IM_LAPLACIAN_EDGE                 (IM_BUF_PREDEFINED | 0x81)
#define IM_LAPLACIAN_EDGE2                (IM_BUF_PREDEFINED | 0x82)
#define IM_SHARPEN                        (IM_BUF_PREDEFINED | 0x83)
#define IM_SHARPEN2                       (IM_BUF_PREDEFINED | 0x84)
#define IM_HORIZ_EDGE                     (IM_BUF_PREDEFINED | 0x85)
#define IM_VERT_EDGE                      (IM_BUF_PREDEFINED | 0x86)
#define IM_SOBEL_EDGE                     (IM_BUF_PREDEFINED | 0x87)
#define IM_PREWITT_EDGE                   (IM_BUF_PREDEFINED | 0x88)
#define IM_ROBERTS_EDGE                   (IM_BUF_PREDEFINED | 0x89)

/* imIntRank() or morphology */
#define IM_3X3_RECT_0                     (IM_BUF_PREDEFINED | 0x8a)
#define IM_3X3_CROSS_0                    (IM_BUF_PREDEFINED | 0x8b)

/* Morphology */
#define IM_3X3_RECT_1                     (IM_BUF_PREDEFINED | 0x8c)
#define IM_3X3_CROSS_1                    (IM_BUF_PREDEFINED | 0x8d)

/* imIntRank() */
#define IM_3X1                            (IM_BUF_PREDEFINED | 0x8e)
#define IM_5X1                            (IM_BUF_PREDEFINED | 0x8f)
#define IM_7X1                            (IM_BUF_PREDEFINED | 0x90)
#define IM_9X1                            (IM_BUF_PREDEFINED | 0x91)
#define IM_1X3                            (IM_BUF_PREDEFINED | 0x92)
#define IM_1X5                            (IM_BUF_PREDEFINED | 0x93)
#define IM_1X7                            (IM_BUF_PREDEFINED | 0x94)
#define IM_1X9                            (IM_BUF_PREDEFINED | 0x95)

/* Morphology */
#define IM_5X5_RECT_1                     (IM_BUF_PREDEFINED | 0x96)
#define IM_7X7_RECT_1                     (IM_BUF_PREDEFINED | 0x97)

/* imIntRank() */
#define IM_3X3_X                          (IM_BUF_PREDEFINED | 0x98)   /* 0x98 */

/* imIntConvolve() */
#define IM_SMOOTH_FAST                    (IM_BUF_PREDEFINED | 0x99)
#define IM_SOBEL_X                        (IM_BUF_PREDEFINED | 0x9a)
#define IM_SOBEL_Y                        (IM_BUF_PREDEFINED | 0x9b)

#define __PUT_NEXT_ONE_HERE__             (IM_BUF_PREDEFINED | 0xa0)   /* I mean on the next lines update _IM_BUF_LAST_PREDEF accordingly */

/* ---------------- */
#define _IM_BUF_LAST_PREDEF               (IM_BUF_PREDEFINED | 0xd1)   /* This one must always stay the last one */

#define _IM_BUF_NUM_PREDEF                (_IM_BUF_LAST_PREDEF & ~IM_BUF_PREDEFINED)

/*******************************************************************
 * Default thread macro to pass the priority
 *******************************************************************/
#define _IM_DEFAULT_THREAD_PRIO(p)        (_IM_DEFAULT_THREAD | p)
#define _IM_DEFAULT_THREAD_GET_PRIO(p)    (p & 0xF)
#define _IM_IS_A_DEFAULT_THREAD(p)        ((p & ~0xf) == _IM_DEFAULT_THREAD)

/*******************************************************************
 * Buffer fields (all tag values must be unique).
 * Tags below 10000 are reserved for user-defined fields.
 *******************************************************************/

/* VIA control options */
#define IM_CTL_BYTE_SWAP                  10000
#define IM_CTL_DIR_X                      10001
#define IM_CTL_DIR_Y                      10002
#define IM_CTL_PACK                       10003
#define IM_CTL_PRIORITY                   10004
#define IM_CTL_SUBSAMP_X                  10005
#define IM_CTL_SUBSAMP_Y                  10006
#define IM_CTL_TAG_BUF                    10007
#define IM_CTL_VIA                        10008
#define IM_CTL_FMTCVR                     10009
#define IM_CTL_WRTMSK                     10010
#define IM_CTL_ZOOM_X                     10011
#define IM_CTL_ZOOM_Y                     10012
#define IM_CTL_BYTE_EXT                   10013
#define IM_CTL_HEADER_EOF                 10014
#define IM_CTL_HEADER_SOF                 10015
#define IM_CTL_COUNT                      10016
#define IM_CTL_SCAN_MODE                  10017
#define IM_CTL_STREAM_ID                  10018
#define IM_CTL_START_X                    10019
#define IM_CTL_START_Y                    10020
#define IM_CTL_STOP_X                     10021
#define IM_CTL_STOP_Y                     10022
#define IM_CTL_START_FIELD                10023
#define IM_CTL_ADDR_MODE                  10024
#define IM_CTL_COUNT_MODE                 10025
#define IM_CTL_LINE_INT                   10026
#define IM_CTL_GRAB_MODE                  10027
#define IM_CTL_CHANNEL                    10028
#define IM_CTL_BYPASS                     10029
#define IM_CTL_CAPTURE_MODE               10030
#define IM_CTL_LINE_INT_FIELD             10031
#define IM_CTL_LINE_INT_STEP              10032
#define IM_CTL_SETUP                      10033
#define IM_CTL_DISPLAY_SYNC               10034
#define IM_CTL_SETUP_ADDRESS              10035
#define IM_CTL_REPLICATE                  10037
#define IM_CTL_CHANNEL_DELAY              10038
#define IM_CTL_LINK                       10039

/* Processing results */
#define IM_RES_MIN_PIXEL                  11000L
#define IM_RES_MAX_PIXEL                  11001L
#define IM_RES_NUM_EVENTS                 11002L
#define IM_RES_NUM_DIFFERENCES            11003L
#define IM_RES_NUM_PIXELS                 11004L
#define IM_RES_IDEMPOTENCE                11005L
#define IM_RES_SUM_PIXEL                  11006L
#define IM_RES_SUM_PIXEL_SQUARED          11007L
#define IM_RES_MEAN_PIXEL                 11008L
#define IM_RES_SIGMA_PIXEL                11009L
#define IM_RES_MIN_ABS_PIXEL              11010L
#define IM_RES_MAX_ABS_PIXEL              11011L
#define IM_RES_SUM_ABS_PIXEL              11012L

/* Run length encoding */
#define IM_RLE_SIZE                       12000
#define IM_RLE_START                      12001
#define IM_RLE_MODE                       12002
#define IM_RLE_COLOR                      12003
#define IM_RLE_BACK_COLOR                 12004

/* Display control and inquiry */
#define IM_DISP_BUF                       13000
#define IM_DISP_KEY_MODE                  13001
#define IM_DISP_KEY_LOW                   13002
#define IM_DISP_KEY_HIGH                  13003
#define IM_DISP_LUT_BUF                   13004
#define IM_DISP_MODE                      13005
#define IM_DISP_PAN_X                     13006
#define IM_DISP_PAN_Y                     13007
#define IM_DISP_WRTMSK                    13008
#define IM_DISP_ZOOM                      13010
#define IM_DISP_RESOLUTION_X              13011
#define IM_DISP_RESOLUTION_Y              13012
#define IM_DISP_VGA_MODE                  13013
#define IM_DISP_ZOOM_Y                    13014
#define IM_DISP_OVERLAY_MODE              13015

/* Neighbourhood processing options */
#define IM_KER_ABSOLUTE                   14050
#define IM_KER_FACTOR                     14052
#define IM_CTL_OVERSCAN                   14053
#define IM_CTL_OVERSCAN_VAL               14054
#define IM_KER_CENTER_X                   14055
#define IM_KER_CENTER_Y                   14056

/* Additional kernel attributes */
#define IM_KER_CLIP                       14200
#define IM_KER_SHIFT                      14201
#define IM_KER_OFFSET                     14202

/* Thread attributes */
#define IM_THR_MAX_PPS                    15001
#define IM_THR_PRIORITY                   15002
#define IM_THR_OWNER_ID                   15003
#define IM_THR_STACK_SIZE                 15004
#define IM_THR_VERIFY                     15005
#define IM_THR_EXECUTE                    15006
#define IM_THR_ACCELERATOR                15007
#define IM_THR_TMP_BUFFER                 15008
#define IM_THR_TIME_SLICE                 15009

/* Additional control options */
#define IM_CTL_DIRECTION                  16000
#define IM_CTL_BLOCK_X                    16001
#define IM_CTL_BLOCK_Y                    16002
#define IM_CTL_WORK_BUF                   16003
#define IM_CTL_WORK_BUF2                  16004
#define IM_CTL_WORK_BUF3                  16005
#define IM_CTL_NORMALIZE                  16006
#define IM_CTL_THRESHOLD                  16007
#define IM_CTL_RESAMPLE                   16008
#define IM_CTL_SAMPLE_X                   16009
#define IM_CTL_SAMPLE_Y                   16010
#define IM_CTL_PRECISION                  16011
#define IM_CTL_CENTER_X                   16012
#define IM_CTL_CENTER_Y                   16013
#define IM_CTL_INPUT_BITS                 16014
#define IM_CTL_OUTPUT_BITS                16015
#define IM_CTL_COMPUTATION                16016
#define IM_CTL_OVERSCAN_X                 16017
#define IM_CTL_OVERSCAN_Y                 16018
#define IM_CTL_SRC_SIZE_X                 16019
#define IM_CTL_SRC_SIZE_Y                 16020
#define IM_CTL_ZOOM                       16021
#define IM_CTL_MASK_BUF                   16022
#define IM_CTL_MAX_SCORE                  16023
#define IM_CTL_SCORE_TYPE                 16024
#define IM_CTL_SHIFT                      16025
#define IM_CTL_STEP                       16026
#define IM_CTL_STEP_X                     16027
#define IM_CTL_STEP_Y                     16028
#define IM_CTL_CLIP                       16029
#define IM_CTL_COEF_ORDER                 16030
#define IM_CTL_SKIP_ZERO                  16031
#define IM_CTL_INPUT_FORMAT               16032
#define IM_CTL_OUTPUT_FORMAT              16033
#define IM_CTL_WEIGHT                     16034
#define IM_CTL_SRC_BUF3                   16035
#define IM_CTL_RASTER_OUT                 16036
#define IM_CTL_COEF_TYPE                  16037
#define IM_CTL_KER_SIZE                   16038
#define IM_CTL_MASK_OP                    16039
#define IM_CTL_CACHE_BUF                  16040
#define IM_CTL_LUT_BUF                    16041
#define IM_CTL_THRESHOLD_FRAC             16042
#define IM_CTL_WEIGHT_2                   16043
#define IM_CTL_BIT_ORDER                  16044
#define IM_CTL_MODE                       16045
#define IM_CTL_WHITE_BALANCE              16046
#define IM_CTL_CENTER_DC                  16047


/* Graphics context */
#define IM_GRA_COLOR                      17000
#define IM_GRA_BACK_COLOR                 17001
#define IM_GRA_DRAW_MODE                  17002
#define IM_GRA_PLOT_MODE                  17003
#define IM_GRA_SCALE_X                    17004
#define IM_GRA_SCALE_Y                    17005
#define IM_GRA_OFFSET_X                   17006
#define IM_GRA_OFFSET_Y                   17007
#define IM_GRA_FONT                       17008
#define IM_GRA_FONT_SCALE_X               17009
#define IM_GRA_FONT_SCALE_Y               17010
#define IM_GRA_BACK_MODE                  17011
#define IM_GRA_COLOR_MODE                 17012
#define IM_GRA_BOX_X_MIN                  17013
#define IM_GRA_BOX_X_MAX                  17014
#define IM_GRA_BOX_Y_MIN                  17015
#define IM_GRA_BOX_Y_MAX                  17016
#define IM_GRA_START_X                    17017
#define IM_GRA_START_Y                    17018
#define IM_GRA_NUMBER                     17019
#define IM_GRA_THRESHOLD                  17020
#define IM_GRA_MAX_SIZE                   17021
#define IM_GRA_FLUSH                      17022

/* OSB attributes */
#define IM_OSB_STATE                      18000
#define IM_OSB_MODE                       18001
#define IM_OSB_INTERRUPT_DRIVEN           18002      /* Internal use only  */
#define IM_OSB_TIMEOUT                    18003
#define IM_OSB_SCOPE                      18004

/* JPEG control */
#define IM_JPEG_MODE                      19000
#define IM_JPEG_NUM_BANDS                 19001
#define IM_JPEG_SIZE_X                    19002
#define IM_JPEG_SIZE_Y                    19003
#define IM_JPEG_SIZE_BIT                  19004
#define IM_JPEG_SIZE                      19005
#define IM_JPEG_TYPE                      19006
#define IM_JPEG_TABLE_QUANT               19007
#define IM_JPEG_TABLE_AC                  19008
#define IM_JPEG_TABLE_DC                  19009
#define IM_JPEG_SAVE_TABLES               19010
#define IM_JPEG_SAVE_IMAGE                19011
#define IM_JPEG_SAVE_HEADER               19012
#define IM_JPEG_PREDICTOR                 19013
#define IM_JPEG_Q_FACTOR                  19014
#define IM_JPEG_NUM_BLOCKS                19015
#define IM_JPEG_SUBSAMP_X                 19016
#define IM_JPEG_SUBSAMP_Y                 19017
#define IM_JPEG_RESTART_ROWS              19018
#define IM_JPEG_COMPUTE_TABLES            19019
#define IM_JPEG_ENTROPY_CODING            19020
#define IM_JPEG_RESET                     19021
#define IM_JPEG_MEM_SEG_LENGTH            19022
#define IM_JPEG_START_INTERVAL            19023
#define IM_JPEG_SAVE_RSTINFO              19024
#define IM_JPEG_FORMAT                    19025
#define IM_JPEG_CONVERT_COLOR             19026

#define IM_GMOD_TYPE                      20000

/* imBufCopyROI */
#define IM_ROI_SIZE_X                     21001
#define IM_ROI_SIZE_Y                     21002
#define IM_ROI_SRC_START_X                21003
#define IM_ROI_SRC_START_Y                21004
#define IM_ROI_DST_START_X                21005
#define IM_ROI_DST_START_Y                21006
#define IM_ROI_NUMBER                     21007

/***************************************
* Grab control and inquire defines     *
****************************************/

/* Limit of the grab control generic defines */
#define IM_DIG_CHANNEL                    30000
#define IM_DIG_SYNC_CHANNEL               30001
#define IM_DIG_GAIN                       30002
#define IM_DIG_REF_WHITE                  30003
#define IM_DIG_REF_BLACK                  30004
#define IM_DIG_LUT_BUF                    30005
#define IM_DIG_USER_IN                    30006
#define IM_DIG_USER_OUT                   30007
#define IM_DIG_EXP_TIME                   30008
#define IM_DIG_EXP_DELAY                  30009
#define IM_DIG_EXP_SOURCE                 30010
#define IM_DIG_EXP_MODE                   30011
#define IM_DIG_EXPOSURE                   30012
#define IM_DIG_TRIG_SOURCE                30013
#define IM_DIG_TRIG_MODE                  30014
#define IM_DIG_TRIGGER                    30015
#define IM_DIG_SIZE_X                     30016
#define IM_DIG_SIZE_Y                     30017
#define IM_DIG_SIZE_BIT                   30018
#define IM_DIG_TYPE                       30019
#define IM_DIG_NUM_BANDS                  30020
#define IM_DIG_SCAN_MODE                  30021
#define IM_DIG_MAX_CHANNELS               30022
#define IM_DIG_INPUT_MODE                 30023
#define IM_DIG_USER_IN_FORMAT             30024
#define IM_DIG_USER_OUT_FORMAT            30025
#define IM_DIG_GRAB_SOURCE                30026
#define IM_DIG_BRIGHTNESS                 30027
#define IM_DIG_CONTRAST                   30028
#define IM_DIG_HUE                        30029
#define IM_DIG_SATURATION                 30030
#define IM_DIG_YC_SEPARATION              30031
#define IM_DIG_SIGNAL_AMPLITUDE           30032
#define IM_DIG_SIGNAL_BLACK               30033
#define IM_DIG_SIGNAL_WHITE               30034
#define IM_DIG_PIXEL_CLOCK                30035
#define IM_DIG_AUTOMATIC_INPUT_GAIN       30036
#define IM_DIG_INPUT_GAIN                 30037

/* Additional control for UART  */
#define IM_UART_BAUD_RATE                 30038
#define IM_UART_PARITY                    30039
#define IM_UART_STOP_BITS                 30040
#define IM_UART_DATA_BITS                 30041
#define IM_UART_TIMEOUT                   30042
#define IM_UART_WRITE_CHAR                30043
#define IM_UART_READ_CHAR                 30044
#define IM_UART_READ_LENGTH               30045
#define IM_UART_READ_MAX_LENGTH           30046
#define IM_UART_READ_STRING               30047
#define IM_UART_WRITE_LENGTH              30048
#define IM_UART_WRITE_STRING              30049
#define IM_UART_DATA_PRESENT              30050
#define IM_UART_ERROR                     30051
/* IM_UART_ERROR+IM_ERR_RESET is reserved 30052 */
#define IM_UART_WRITE_TERMINATOR          30053
#define IM_UART_READ_TERMINATOR           30054
#define _IM_UART_READ_CONFIG              30055       /* Internal use only */

/* do NOT modify the following values */
#define IM_UART_WRITE_TERMINATED_STRING   0x00002000
#define IM_UART_WRITE_FIXED_STRING        0x00004000
#define IM_UART_READ_TERMINATED_STRING    0x00006000
#define IM_UART_READ_FIXED_STRING         0x00008000

// Additional control for Odyssey grab modules extra timer pulse
#define IM_DIG_EXP_TIME2                  30056
#define IM_DIG_EXP_DELAY2                 30057

/* Additional control for Odyssey PMC module */
#define IM_DIG_EXP_ARM_SOURCE             30058
#define IM_DIG_EXP_ARM_MODE               30059
#define IM_DIG_HRST_MODE                  30060
#define IM_DIG_VRST_MODE                  30061

/* Additional control for Camera Link module */
#define IM_DIG_CC1_SOURCE                 30062
#define IM_DIG_CC2_SOURCE                 30063
#define IM_DIG_CC3_SOURCE                 30064
#define IM_DIG_CC4_SOURCE                 30065

/* Additional control for Analog module */
#define IM_DIG_INPUT_SOURCE               30066

/* Additionnal control for UART */
#define IM_UART_CHANNEL_LOCK              30067
#define IM_UART_CHANNEL_UNLOCK            30068

/* inquire to get the Camera data from remote nodes */
#define IM_DIG_CAM_POINTER                30069

/* inquire to get the Digitizer data from remote nodes */
#define IM_DIG_DATA_POINTER               30070

// Users events
#define IM_DIG_EVENT_HUNLCK               30071
#define IM_DIG_EVENT_HLCK                 30072
#define IM_DIG_EVENT_PLLUNLCK             30073
#define IM_DIG_EVENT_PLLLCK               30074
#define IM_DIG_EVENT_SOONG                30075
#define IM_DIG_EVENT_SOENG                30076
#define IM_DIG_EVENT_EOOG                 30077
#define IM_DIG_EVENT_SOOG                 30078
#define IM_DIG_EVENT_EOEG                 30079
#define IM_DIG_EVENT_SOEG                 30080
#define IM_DIG_EVENT_EOFG                 30081
#define IM_DIG_EVENT_SOFG                 30082
#define IM_DIG_EVENT_EOG                  30083
#define IM_DIG_EVENT_SOG                  30084
#define IM_DIG_EVENT_EOT0                 30085
#define IM_DIG_EVENT_SOT0                 30086
#define IM_DIG_EVENT_EOT1                 30087
#define IM_DIG_EVENT_SOT1                 30088
#define IM_DIG_EVENT_BUFOVR               30089
#define IM_DIG_EVENT_UART                 30090
#define IM_DIG_EVENT_SONDF                30091
#define IM_DIG_EVENT_SENDF                30092
#define IM_DIG_EVENT_USRIN                30093

#define IM_DIG_ENCODER                    30094
#define IM_DIG_ENCODER_EOG                30095
#define IM_DIG_ENC_TCOUNT                 30096

// Number of channels present on a digitizer
#define IM_DIG_NUM_CHANNELS               30097

// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
// Insert new IM_DIG_... control above here
// Renumber IM_DIG_LAST_CONTROL according to previous addition
// It must be the same value than the last IM_DIG_... defined
#define IM_DIG_LAST_CONTROL               30097

// WARNING
// do not put any control for imDigControl/imDigInquire in the following 4 ranges:
// 8192-12287     space for IM_UART_WRITE_TERMINATED_STRING
// 16384-20479    space for IM_UART_WRITE_FIXED_STRING
// 24576-28671    space for IM_UART_READ_TERMINATED_STRING
// 32768-36863    space for IM_UART_READ_FIXED_STRING


#define IM_UART_FRAMING_ERROR             0x0001
#define IM_UART_PARITY_ERROR              0x0002
#define IM_UART_OVERRUN_ERROR             0x0004
#define IM_UART_TIMEOUT_ERROR             0x0008

#define IM_SEQ_TIME                       50100
#define IM_SEQ_FRAMES                     50101
#define IM_SEQ_MAX_TIME                   50102
#define IM_SEQ_TIME_BUF                   50103
#define IM_SEQ_TIME_BUF2                  50104
#define IM_SEQ_PRIORITY                   50105

#define IM_SEQ_WAIT                       1
#define IM_SEQ_HALT                       2
#define IM_SEQ_ABORT                      3
#define IM_SEQ_WAIT_THREAD                4

/* Digitizer Low Level control */
#define IM_DIGLL_REF_WHITE                4
#define IM_DIGLL_REF_BLACK                5
#define IM_DIGLL_LEVEL_COARSE             6
#define IM_DIGLL_LEVEL_FINE               7


/***************************************/
/* Control and inquire values          */
/***************************************/

#define IM_CHANNEL_0                      0x00010000
#define IM_CHANNEL_1                      0x00020000
#define IM_CHANNEL_2                      0x00040000
#define IM_CHANNEL_3                      0x00080000
#define IM_CHANNEL_GRAB                   0x01000000
#define IM_ALL_CHANNELS                   (IM_CHANNEL_0|IM_CHANNEL_1|IM_CHANNEL_2|IM_CHANNEL_3)

#define IM_CHANNEL(index)                 (IM_CHANNEL_0 << (index))

#define IM_BIT0                           0x01000000
#define IM_BIT1                           0x02000000
#define IM_BIT2                           0x04000000
#define IM_BIT3                           0x08000000
#define IM_BIT4                           0x10000000
#define IM_BIT5                           0x20000000
#define IM_BIT6                           0x40000000
#define IM_ALL_BITS                       (IM_BIT0|IM_BIT1|IM_BIT2|IM_BIT3|IM_BIT4|IM_BIT5|IM_BIT6)

#define IM_BIT(index)                     (IM_BIT0 << (index))

/* Flags that could be ored with channels */
#define IM_AUTO                           0x0L
#define IM_MANUAL                         0x1000000L

#define IM_SOFTWARE                       0L
#define IM_HARDWARE                       1L
#define IM_VSYNC                          2L
#define IM_HSYNC                          3L
#define IM_EXPOSURE                       4L
#define IM_USER_BIT                       5L          /* equal IM_VSYNC+IM_FIELD */
#define IM_USER_OUT                       6L          /* Camera Link only */
#define IM_ENCODER                        7L
#define IM_TIMER1                         0x01000000
#define IM_TIMER2                         0x02000000
#define IM_TIMER3                         0x04000000
#define IM_TIMER4                         0x08000000
#define IM_ALL_TIMERS                     (IM_TIMER1|IM_TIMER2|IM_TIMER3|IM_TIMER4)
#define IM_TRIGGER1                       0x00100000
#define IM_TRIGGER2                       0x00200000
#define IM_TRIGGER3                       0x00400000
#define IM_TRIGGER4                       0x00800000
#define IM_ALL_TRIGGERS                   (IM_TRIGGER1|IM_TRIGGER2|IM_TRIGGER3|IM_TRIGGER4)

#define IM_FALLING_EDGE                   0
#define IM_RISING_EDGE                    1

#define IM_ACTIVE_LOW                     2
#define IM_ACTIVE_HIGH                    3

#define IM_ANALOG                         0
#define IM_DIGITAL                        1

#define IM_TTL                            0x00010000
#define IM_RS422                          0x00020000
#define IM_LVDS                           IM_RS422

#define IM_USE_MEMORY_DCF                 0
#define IM_USE_EMBEDDED_DCF               1

#define IM_PORT1                          1
#define IM_PORT2                          2

#define IM_INPUT_A                        0x00000001
#define IM_INPUT_B                        0x00000002
#define IM_ALL_INPUTS                     (IM_INPUT_A|IM_INPUT_B)
#define IM_AC_COUPLED                     0x00010000
#define IM_DC_COUPLED                     0x00020000
#define IM_INPUT_A_AC                     (IM_INPUT_A|IM_AC_COUPLED)
#define IM_INPUT_A_DC                     (IM_INPUT_A|IM_DC_COUPLED)
#define IM_INPUT_B_AC                     (IM_INPUT_B|IM_AC_COUPLED)
#define IM_INPUT_B_DC                     (IM_INPUT_B|IM_DC_COUPLED)

#define IM_COM_PORT                       0x10000
#define IM_ODYSSEY                        0x20000

#endif   /* __IMAPIDEF_H__ */

