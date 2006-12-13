/////////////////////////////////////////////////////////////////
//
// Filename          :  MILCODE.H
// Content           :  function capi of McodeAlloc
//
// Copyright © 1999 Matrox Electronic Systems Ltd.
// All Rights Reserved
//
/////////////////////////////////////////////////////////////////

#ifndef __MILCODE_H__
#define __MILCODE_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************/
/* CAPI defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* McodeAlloc()  Code types                                               */
/**************************************************************************/

#define M_CODE39                 0x00000001
#define M_DATAMATRIX             0x00000002
#define M_EAN13                  0x00000004
#define M_MAXICODE               0x00000008
#define M_INTERLEAVED25          0x00000010
#define M_CODE128                0x00000020
#define M_BC412                  0x00000040
#define M_CODABAR                0x00000080
#define M_PDF417                 0x00000100
#define M_POSTNET                0x00000200
#define M_PLANET                 0x00000400
#define M_UPC_A                  0x00000800
#define M_UPC_E                  0x00001000
#define M_PHARMACODE             0x00002000
#define M_RSSCODE                0x00004000
// RSS Sub type section...
#define M_RSS14                  0x00008000
#define M_RSS14_TRUNCATED        0x00010000
#define M_RSS_LIMITED            0x00020000
#define M_RSS_EXPANDED           0x00040000
#define M_RSS14_STACKED          0x00080000
#define M_RSS14_STACKED_OMNI     0x00100000
#define M_RSS_EXPANDED_STACKED   0x00200000
// ...End of RSS Sub type section.
#define M_EAN8                   0x00400000
#define M_MICROPDF417            0x00800000
#define M_COMPOSITECODE          0x01000000
#define M_UCCEAN128              0x02000000 //Part of a composite, this is a subtype only...

// Allocation control flags
#define M_CONTROL_FLAG           0x00019400L // Already define as 0x00019400L
#define M_IGNORE_START_STOP      0x00000001

/**************************************************************************/
/* McodeReadMultiple()  Code types                                        */
/**************************************************************************/
#define M_AUTO_DETECT_ANGLE      0x00000001

/**************************************************************************/
/* McodeInquire(), McodeGetResult(), McodeControl() parameters            */
/**************************************************************************/

#define M_CODE_TYPE               0x00000001L
#define M_WRITE_SIZE_X            0x00000002L
#define M_STRING                           3L    // Already define as 3L
#define M_FOREGROUND_VALUE                 4L     // Already define as 4L
#define M_WRITE_SIZE_Y            0x00000005L
#define M_THRESHOLD                        82      // Already define as 82
#define M_OLD_THRESHOLD                     6      // Already define as 6
#define M_STATUS                  0x00008002L
#define M_FOREGROUND_WHITE              0x80L      // Already define as 0x80L
#define M_FOREGROUND_BLACK             0x100L      // Already define as 0x100L
#define M_SEARCH_ANGLE            0x00000100L      // Already define as 0x00000100L
#define M_SEARCH_ANGLE_DELTA_NEG  0x00000200L      // Already define as 0x00000200L
#define M_SEARCH_ANGLE_DELTA_POS  0x00000400L      // Already define as 0x00000400L
#define M_ANGLE                   0x00000800L      // Already define as 0x00000800L
#define M_CELL_SIZE               0x00001000L
#define M_CELL_SIZE_MIN           0x00002000L
#define M_CELL_SIZE_MAX           0x00004000L
#define M_ASPECT_RATIO                  6001L      // Already define as 6001L
#define M_CELL_NUMBER_X           0x00008000L
#define M_CELL_NUMBER_Y           0x00008001L
#define M_SPEED                   M_SPEED_FACTOR   // Already define as   8L
#define M_STRING_SIZE             0x00008003L
#define M_ERROR_CORRECTION        0x00008004L
#define M_ENCODING                0x00008005L
#define M_DOT_SPACING             0x00008007L
#define M_SEGMENT_NUMBER_X        0x0000800AL
#define M_SEGMENT_NUMBER_Y        0x0000800BL

#define M_BIT_STREAM              0x00008008L
#define M_POSITION_X              0x00003400L      // Already define as 0x00003400L
#define M_POSITION_Y              0x00004400L      // Already define as 0x00004400L
#define M_MATRIX_BASE_VECTOR_1_X  0x00004500L
#define M_MATRIX_BASE_VECTOR_1_Y  0x00004600L
#define M_MATRIX_BASE_VECTOR_2_X  0x00004700L
#define M_MATRIX_BASE_VECTOR_2_Y  0x00004800L

#define M_DISTANCE_MAX            0x00004900L
#define M_SCAN_REVERSE            0x00004B00L
#define M_USE_SHARPEN             0x00004C00L
#define M_USE_PRESEARCH           0x00004D00L

#define M_DATAMATRIX_TYPE         0x00004E00L //Undocumented flag usefull to increase
                                              //speed if the user know his Datamatrix
                                              //are always obsolete or never obsolete...
                                              //Possible values are : M_OBSOLETE, M_REGULAR or
                                              //M_ANY. M_ANY is the default value since it allow
                                              //to read both type of Datamatrix.
#define M_SCANLINE_HEIGHT         0x00004F00L //Usefull to control the height of the scanline (in pixel).
#define M_SCANLINE_FREQUENCE      0x00005000L          //Obsolete : replaced by M_SCANLINE_STEP
                                                       //keep here for backward compatibility
#define M_SCANLINE_STEP           M_SCANLINE_FREQUENCE //Usefull to control the distance between scanline.
#define M_SUB_TYPE                0x00005100L //To enable code sub type for a read operation.
                                              //Example : M_RSS14, M_RSS_LIMITED,...

#define M_CHECK_DIGIT             0x0000000EL
#define M_SIZE_X                        1002L //Already define as 1002L
#define M_SIZE_Y                        1003L //Already define as 1003L
#define M_TOP_LEFT_X                    1004L
#define M_TOP_LEFT_Y                    1005L

//Grading result flags
#define M_OVERALL_SYMBOL_GRADE            0x00009000L
#define M_SYMBOL_CONTRAST_GRADE           0x00009001L
#define M_PRINT_GROWTH_GRADE              0x00009002L
#define M_AXIAL_NONUNIFORMITY_GRADE       0x00009003L
#define M_UNUSED_ERROR_CORRECTION_GRADE   0x00009004L
#define M_DECODE_GRADE                    0x00009005L
#define M_EDGE_DETERMINATION_GRADE        0x00009006L
#define M_MINIMUM_REFLECTANCE_GRADE       0x00009007L
#define M_MINIMUM_EDGE_CONTRAST_GRADE     0x00009008L
#define M_MODULATION_GRADE                0x00009009L
#define M_DEFECTS_GRADE                   0x0000900AL
#define M_DECODABILITY_GRADE              0x0000900BL
#define M_RAP_GRADE                       0x0000900CL
#define M_SYMBOL_BASED_GRADE              0x0000900DL
#define M_START_STOP_PATTERN_GRADE        0x0000900EL

#define M_TYPE_LONG               0x00040000L      // Already define as 0x00040000L
#define M_TYPE_DOUBLE             0x00100000L      // Already define as 0x00010000L
#define M_ESCAPE_SEQUENCE         0x00000010L

#define M_SIZE_BYTE                     5061L        // already defined as 5061L

/**************************************************************************/
/* McodeSave(), McodeRestore()                                            */
/**************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY                         (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#else
#define M_MEMORY                         5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE

#define M_INTERACTIVE                    M_NULL // Already defined in mil.h, milcal.h, miledge.h, milmeas.h, milocr.h, milpat.h, milmod.h



/**************************************************************************/
/* McodeInquire(), McodeGetResult(), McodeControl() parameter values      */
/**************************************************************************/

#define M_ECC_050                 0            // These numbers are importants!!
#define M_ECC_080                 1
#define M_ECC_100                 2
#define M_ECC_140                 3
#define M_ECC_NONE                4
#define M_ECC_CHECK_DIGIT         5
#define M_ECC_UNKNOWN             6
#define M_ECC_200                 7
#define M_ECC_REED_SOLOMON_0      10
#define M_ECC_REED_SOLOMON_1      (M_ECC_REED_SOLOMON_0 + 1)
#define M_ECC_REED_SOLOMON_2      (M_ECC_REED_SOLOMON_0 + 2)
#define M_ECC_REED_SOLOMON_3      (M_ECC_REED_SOLOMON_0 + 3)
#define M_ECC_REED_SOLOMON_4      (M_ECC_REED_SOLOMON_0 + 4)
#define M_ECC_REED_SOLOMON_5      (M_ECC_REED_SOLOMON_0 + 5)
#define M_ECC_REED_SOLOMON_6      (M_ECC_REED_SOLOMON_0 + 6)
#define M_ECC_REED_SOLOMON_7      (M_ECC_REED_SOLOMON_0 + 7)
#define M_ECC_REED_SOLOMON_8      (M_ECC_REED_SOLOMON_0 + 8)
#define M_ECC_REED_SOLOMON        19
#define M_ECC_010                 20
#define M_ECC_040                 21
#define M_ECC_060                 22
#define M_ECC_070                 23
#define M_ECC_090                 24
#define M_ECC_110                 25
#define M_ECC_120                 26
#define M_ECC_130                 27
#define M_ECC_COMPOSITE           28

#define M_ENC_NUM                 0            // These numbers are importants!!
#define M_ENC_ALPHA               1
#define M_ENC_ALPHANUM_PUNC       2
#define M_ENC_ALPHANUM            3
#define M_ENC_ASCII               4
#define M_ENC_ISO8                5
#define M_ENC_MODE2               6
#define M_ENC_MODE3               (M_ENC_MODE2 + 1)
#define M_ENC_MODE4               (M_ENC_MODE2 + 2)
#define M_ENC_MODE5               (M_ENC_MODE2 + 3)
#define M_ENC_MODE6               (M_ENC_MODE2 + 4)
#define M_ENC_STANDARD           11
#define M_ENC_UNKNOWN            13
#define M_ENC_EAN13                  0x00000020 //32
#define M_ENC_EAN8                   0x00000040 //64
#define M_ENC_UPCA                   0x00000080 //128
#define M_ENC_UPCE                   0x00000100 //256
#define M_ENC_UCCEAN128_PDF417       0x00000200 //512
#define M_ENC_RSS14                  0x00000400 //1024
#define M_ENC_RSS14_TRUNCATED        0x00000800 //2048
#define M_ENC_RSS_LIMITED            0x00001000 //4096
#define M_ENC_RSS_EXPANDED           0x00002000 //8192
#define M_ENC_RSS14_STACKED          0x00004000 //16384
#define M_ENC_RSS14_STACKED_OMNI     0x00008000 //32768
#define M_ENC_RSS_EXPANDED_STACKED   0x00010000 //65536
#define M_ENC_UCCEAN128_MICROPDF417  0x00020000 //131072
#define M_ENC_UCCEAN128              0x00040000 //262144
#define M_ENC_PDF417                 0x00080000 //524288
#define M_ENC_MICROPDF417            0x00040000 //1048576

#define M_STATUS_OK                    0  //Obsolete flag
#define M_STATUS_READ_OK               M_STATUS_OK
#define M_STATUS_CRC_FAILED            1
#define M_STATUS_ECC_UNKNOWN           2
#define M_STATUS_ENC_UNKNOWN           3
#define M_STATUS_WRITE_OK              4
#define M_STATUS_WRITE_FAILED          5
#define M_STATUS_NOT_FOUND             6
#define M_STATUS_VERIFY_OK             7
#define M_STATUS_VERIFY_FAILED         8
#define M_STATUS_NO_RESULT_AVAILABLE   9

#define M_VERY_LOW               0L   // Already define as 0L
#define M_LOW                    1L   // Already define as 1L
#define M_MEDIUM                 2L   // Already define as 2L
#define M_HIGH                   3L   // Already define as 3L
#define M_VERY_HIGH              4L   // Already define as 4L

#define M_ANY                     0x11000000L      // Already define as 0x11000000L
#define M_DEFAULT                 0x10000000L      // Already define as 0x10000000L

#define M_PRESENT                          2L// Already define as 2L
#define M_ABSENT                           3L

#define M_OBSOLETE                      25L
#define M_REGULAR                       0x00020000L  // Already define as 0x00020000L

//Grading result possible values
#define M_CODE_GRADE_A              4L //These numbers should not be changed
#define M_CODE_GRADE_B              3L
#define M_CODE_GRADE_C              2L
#define M_CODE_GRADE_D              1L
#define M_CODE_GRADE_F              0L
#define M_CODE_GRADE_NOT_AVAILABLE  5L

/**************************************************************************/
/* Function prototypes                                                    */
/**************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE McodeAlloc     (MIL_ID SystemId, long CodeType, long ControlFlag, MIL_ID *CodeIdPtr);
MFTYPE32 void   MFTYPE McodeControl   (MIL_ID CodeId, long ControlType, double ControlValue);
MFTYPE32 long   MFTYPE McodeInquire   (MIL_ID CodeId, long ParamToInquire, void *ValuePtr);
MFTYPE32 void   MFTYPE McodeRead      (MIL_ID CodeId, MIL_ID ImageBufId, long ControlFlag);
MFTYPE32 void   MFTYPE McodeWrite     (MIL_ID CodeId, MIL_ID ImageBufId, MIL_TEXT_PTR String, long ControlFlag);
MFTYPE32 void   MFTYPE McodeGetResult (MIL_ID CodeId, long ResultType, void *ResultPtr);
MFTYPE32 void   MFTYPE McodeSave      (MIL_TEXT_PTR FileName, MIL_ID CodeId, long ControlFlag);
MFTYPE32 MIL_ID MFTYPE McodeRestore   (MIL_TEXT_PTR FileName, MIL_ID SystemId, long ControlFlag, MIL_ID* CodeIdPtr);
MFTYPE32 void   MFTYPE McodeFree      (MIL_ID CodeId );
MFTYPE32 void   MFTYPE McodeVerify    (MIL_ID CodeId, MIL_ID ImageBufId, MIL_TEXT_PTR String, long ControlFlag);
 
#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILCODE_H__ */
