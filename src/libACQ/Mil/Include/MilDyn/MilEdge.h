/***************************************************************************/
/*

    Filename:  MilEdge.h
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines and the prototypes for the
               MIL edge and line crest extractor. (Medge...).

    COPYRIGHT (c) 2002 Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILEDGEEXTR_H__
#define __MILEDGEEXTR_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      MilEdge CAPI defines                               */
/***************************************************************************/


/***************************************************************************/
/* MedgeAlloc()                                                            */
/***************************************************************************/
#define M_CONTOUR                            0x800L      // Already defined in    milblob.h
#define M_CREST                              0x801L

/***************************************************************************/
/* MedgeControl()                                                          */
/***************************************************************************/

/* Global to context behaviour */
#define M_CHAIN_ALL_NEIGHBORS                3L
#define M_MAGNITUDE_TYPE                     10L
#define M_NORM                               4000L
#define M_SQR_NORM                           4001L
#define M_FILTER                             1046L       // Already defined in    mil.h
#define M_FILTER_WIDTH                       109L        // Already defined in    milmod.h
#define M_KERNEL_SIZE                        301L        // Already defined in    milmod.h
#define M_MAX_POWER_LOSS                     0x1200L     // Already defined in    milmod.h
#define M_FILTER_MODE                        122L        // Already defined in    milmod.h
#define M_FILTER_MAX_DEPTH                   302L
#ifdef M_MIL_75_COMPATIBLE
#define M_KERNEL                             0x00000200L // Already defined in    milmod.h, mil.h
#else
#define M_KERNEL                             0x00400000L // Already defined in    milmod.h, mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_RECURSIVE                          0x00040000  // Already defined in    mildev.h
#define M_SHEN                               0x802L
#define M_EXPONENTIAL                        2           // Already defined in    mil.h
#define M_GAUSSIAN                           0x805L
#define M_SOBEL                              M_EDGE_DETECT  // Already defined in mil.h
#define M_PREWITT                            M_EDGE_DETECT2 // Already defined in mil.hL
#define M_FREI_CHEN                          M_ID_OFFSET_OF_DEFAULT_KERNEL + 9L
#define M_DERICHE                            M_ID_OFFSET_OF_DEFAULT_KERNEL + 10L
#define M_FILTER_SMOOTHNESS                  1047L
#define M_ACCURACY                           106L        // Already defined in    milmod.h
#define M_THRESHOLD_TYPE                     3000L
#define M_THRESHOLD_MODE                     33L
#define M_THRESHOLD_VALUE_LOW                3001L
#define M_THRESHOLD_VALUE_HIGH               3002L
#define M_THRESHOLD_LOW                      M_THRESHOLD_VALUE_LOW
#define M_THRESHOLD_HIGH                     M_THRESHOLD_VALUE_HIGH
#define M_DETAIL_LEVEL                       111L        // Already defined in    milmod.h
#define M_NO_HYSTERESIS                      20L
#define M_FULL_HYSTERESIS                    25L
#define M_HYSTERESIS                         31L
#define M_SAVE_DERIVATIVES                   11L
#define M_SAVE_ANGLE                         12L
#define M_SAVE_MAGNITUDE                     13L
#define M_SAVE_MASK                          39L
#define M_SAVE_CHAIN_MAGNITUDE               59L
#define M_SAVE_CHAIN_ANGLE                   60L
#define M_SAVE_IMAGE                         61L
#define M_MASK_SIZE_X                        0x0000001DL
#define M_MASK_SIZE_Y                        0x0000001EL
#define M_FILL_GAP_DISTANCE                  44L
#define M_FILL_GAP_ANGLE                     45L
#define M_FILL_GAP_POLARITY                  46L
#define M_FILL_GAP_CONTINUITY                38L
#define M_EXTRACTION_ANGLE                   47L
#define M_EXTRACTION_TOLERANCE               48L
#define M_EXTRACTION_USE_POLARITY            49L

// Typical control values
#define M_LOW                                1L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_MEDIUM                             2L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h
#define M_HIGH                               3L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h, mil.h
#define M_VERY_HIGH                          4L          // Already defined in    milcode.h, milmod.h, milocr, milpat.h

// Utilities
#define M_FLOAT_MODE                         70L
#define M_ALLIGNEMENT_MODE                   51L
#define M_TIMEOUT                            2077L       // Already defined in    milmod.h, mil.h

/* Global to context features */
#define M_BOX_X_MIN                          6L          // Already defined in    milblob.h
#define M_BOX_Y_MIN                          7L          // Already defined in    milblob.h
#define M_BOX_X_MAX                          8L          // Already defined in    milblob.h
#define M_BOX_Y_MAX                          9L          // Already defined in    milblob.h
#define M_X_MIN_AT_Y_MIN                     21L         // Already defined in    milblob.h
#define M_X_MAX_AT_Y_MAX                     22L         // Already defined in    milblob.h
#define M_Y_MIN_AT_X_MAX                     23L         // Already defined in    milblob.h
#define M_Y_MAX_AT_X_MIN                     24L         // Already defined in    milblob.h
#define M_CHAIN_APPROXIMATION                0x00010200L 
#define M_CHAINS                             0x105L      // Already defined in    milblob.h
#define M_APPROXIMATION_TOLERANCE            26L
#define M_LINE                               0x00000080L // Already defined in    milmod.h
#define M_ARC                                0x00000081L
#define M_CENTER_OF_GRAVITY_X                34L         // Already defined in    milblob.h
#define M_CENTER_OF_GRAVITY_Y                35L         // Already defined in    milblob.h
#define M_POSITION                           0x00000402L // Already defined in    milmeas.h
#define M_POSITION_X                         0x00003400L // Already defined in    milmeas.h, milcode.h, milmod.h, milpat.h, mil.h
#define M_POSITION_Y                         0x00004400L // Already defined in    milblob.h, milcode.h, milmod.h, milpat.h, mil.h
#define M_MOMENT_ELONGATION                  50L
#define M_SIZE                               0xC00L      // Already defined in    milmod.h
#define M_FAST_LENGTH                        52L
#define M_TORTUOSITY                         76L
#define M_CLOSURE                            77L
#define M_STRENGTH                           55L
#define M_AVERAGE_STRENGTH                   57L
#define M_BOX                                0x101L      // Already defined in    milblob.h
#define M_CONTACT_POINTS                     0x102L      // Already defined in    milblob.h
#define M_CENTER_OF_GRAVITY                  0x103L      // Already defined in    milblob.h
#define M_LENGTH                             0x00002000L // Already defined in    milblob.h, milcal.h, milmeas.h, milmod.h, mil.h
#define M_FERET_X                            72L         // Already defined in    milblob.h
#define M_FERET_Y                            5L          // Already defined in    milblob.h
#define M_FERET                              69L
#define M_FERET_BOX                          M_FERET
#define M_FERET_MIN_ANGLE                    15L         // Already defined in    milblob.h
#define M_FERET_MAX_ANGLE                    17L         // Already defined in    milblob.h
#define M_FERET_MIN_DIAMETER                 14L         // Already defined in    milblob.h
#define M_FERET_MAX_DIAMETER                 16L         // Already defined in    milblob.h
#define M_FERET_MEAN_DIAMETER                18L         // Already defined in    milblob.h
#define M_FERET_ELONGATION                   27L         // Already defined in    milblob.h
#define M_NUMBER_OF_FERETS                   63L         // Already defined in    milblob.h
#define M_CONVEX_PERIMETER                   20L         // Already defined in    milblob.h
#define M_GENERAL_FERET                      0x400L      // Already defined in    milblob.h

#define M_GENERAL_FERET_ANGLE                62L
#define M_ALL_FEATURES                       0x100L      // Already defined in    milblob.h
#define M_FIRST_FERET_INDEX                  640L
#define M_SECOND_FERET_INDEX                 257L
#define M_FERET_INDEX                        (M_FIRST_FERET_INDEX|M_SECOND_FERET_INDEX)
#define M_NEAREST_EDGEL_X                    37L
#define M_NEAREST_EDGEL_Y                    264L
#define M_NEAREST_EDGEL                      (M_NEAREST_EDGEL_X|M_NEAREST_EDGEL_Y)

/* Specific to M_CREST context behaviour */
#define M_FOREGROUND_VALUE                   4L          // Already defined in    milblob.h, milmod.h,  milcal.h,  milcode.h, milocr.h
#define M_FOREGROUND_BLACK                   0x100L      // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h
#define M_FOREGROUND_WHITE                   0x80L       // Already defined in    milblob.h, milmod.h,  milcode.h, milocr.h
#define M_ANY                                0x11000000L // Already defined in    milmod.h,  milcode.h, milocr.h,  milmeas.h, mil.h
#define M_SAVE_CHAIN_WIDTH                   28L
#define M_TOPOLOGY                           28L
#define M_AVERAGE_WIDTH                      104L

/* Sorting keys */
#define M_SORT1_UP                           0x02000000L // Already defined in    milblob.h
#define M_SORT1_DOWN                         0x0A000000L // Already defined in    milblob.h
#define M_SORT2_UP                           0x04000000L // Already defined in    milblob.h
#define M_SORT2_DOWN                         0x0C000000L // Already defined in    milblob.h
#define M_SORT3_UP                           0x06000000L // Already defined in    milblob.h
#define M_SORT3_DOWN                         0x0E000000L // Already defined in    milblob.h
#define M_NO_SORT                            0L          // Already defined in    milblob.h

/* Statistic flags */
#define M_MIN                                0x02000000L // Already defined in    milmeas.h, mil.h
#define M_MAX                                0x04000000L // Already defined in    milmeas.h, mil.h
#define M_MEAN                               0x08000000L // Already defined in    milmeas.h, mil.h, milocr.h
#define M_MIN_ABS                            0x00200000L // Already defined in    mil.h
#define M_MAX_ABS                            0x00800000L // Already defined in    mil.h
#define M_STANDARD_DEVIATION                 0x10000000L // Already defined in    milmeas.h, mil.h

/* Global to result behaviour */
#define M_DRAW_RELATIVE_ORIGIN_X             319L        // Already defined in    milmod.h
#define M_DRAW_RELATIVE_ORIGIN_Y             320L        // Already defined in    milmod.h
#define M_DRAW_SCALE_X                       3203L       // Already defined in    milmod.h
#define M_DRAW_SCALE_Y                       3204L       // Already defined in    milmod.h
#define M_DRAW_CROSS_SIZE                    115L
#define M_MODEL_FINDER_COMPATIBLE            19L

/***************************************************************************/
/* MedgeInquire()                                                          */
/***************************************************************************/

/* To Context */
#define M_CONTEXT_TYPE                       2L
#define M_FILTER_POWER                       1048L
#define M_OVERSCAN                           53L         // Already defined in    mil.h, milmod.h
#define M_OVERSCAN_REPLACE_VALUE             54L         // Already defined in    mil.h, milmod.h
#define M_MIRROR                             0x01000061L // Already defined in    mil.h
#define M_REPLACE                            0x01000060L // Already defined in    mil.h, milocr.h

/***************************************************************************/
/* MedgeSelect()                                                           */
/***************************************************************************/
#define M_INCLUDE                            1L          // Already defined in    milblob.h
#define M_INCLUDE_ONLY                       0x101L      // Already defined in    milblob.h
#define M_EXCLUDE                            2L          // Already defined in    milblob.h
#define M_EXCLUDE_ONLY                       0x102L      // Already defined in    milblob.h
#define M_DELETE                             3L          // Already defined in    milblob.h, milmod.h
#define M_GROUP                              4L
#define M_UNGROUP                            5L
#define M_ALL_EDGES                          0x00200000L
#define M_INCLUDED_EDGES                     0x00400000L
#define M_EXCLUDED_EDGES                     0x00800000L

#define M_IN_RANGE                           1L          // Already defined in    milblob.h, mil.h
#define M_OUT_RANGE                          2L          // Already defined in    milblob.h, mil.h
#define M_EQUAL                              3L          // Already defined in    milblob.h, mil.h
#define M_NOT_EQUAL                          4L          // Already defined in    milblob.h, mil.h
#define M_GREATER                            5L          // Already defined in    milblob.h, mil.h
#define M_LESS                               6L          // Already defined in    milblob.h, mil.h
#define M_GREATER_OR_EQUAL                   7L          // Already defined in    milblob.h, mil.h
#define M_LESS_OR_EQUAL                      8L          // Already defined in    milblob.h, mil.h

#define M_INSIDE_BOX                         300L
#define M_INSIDE_CHAIN                       301L
#define M_OUTSIDE_BOX                        302L
#define M_OUTSIDE_CHAIN                      303L
#define M_EQUAL_CHAIN                        0x40
#define M_INSIDE_OR_EQUAL_BOX                (M_INSIDE_BOX|M_EQUAL_CHAIN)
#define M_INSIDE_OR_EQUAL_CHAIN              (M_INSIDE_CHAIN|M_EQUAL_CHAIN)
#define M_OUTSIDE_OR_EQUAL_BOX               (M_OUTSIDE_BOX|M_EQUAL_CHAIN)
#define M_OUTSIDE_OR_EQUAL_CHAIN             (M_OUTSIDE_CHAIN|M_EQUAL_CHAIN)
#define M_CROP_CHAIN                         304L
#define M_NEAREST_NEIGHBOR                   0x00000040L // Already defined in    milmeas.h, mil.h
#define M_ALL_NEAREST_NEIGHBORS              0x00000080L 

/***************************************************************************/
/* MedgeGetResult()                                                        */
/***************************************************************************/
#define M_LABEL_VALUE                        1L          // Already defined in    milblob.h
#define M_THRESHOLD_VALUES                   3003L
#define M_THRESHOLDS                         M_THRESHOLD_VALUES
#define M_NUMBER_OF_CHAINS                   0x769L
#define M_CHAIN_MAGNITUDE                    36L
#define M_CHAIN_ANGLE                        258L
#define M_IMAGE_ID                           M_BUF_ID    // Already defined in    milmod.h (0x04000000L)
#define M_MASK_ID                            (M_BUF_ID | 0x80000L)
#define M_ANGLE_ID                           0x20000000L
#define M_MAGNITUDE_ID                       0x20000L
#define M_FIRST_DERIVATIVE_X_ID              0x10000L
#define M_FIRST_DERIVATIVE_Y_ID              0x8000L
#define M_SECOND_DERIVATIVE_X_ID             0x4000L
#define M_SECOND_DERIVATIVE_Y_ID             0x1000L
#define M_CROSS_DERIVATIVE_ID                0x800L
#define M_FIRST_DERIVATIVES_ID               (M_FIRST_DERIVATIVE_X_ID  | M_FIRST_DERIVATIVE_Y_ID)
#define M_SECOND_DERIVATIVES_ID              (M_SECOND_DERIVATIVE_X_ID | M_SECOND_DERIVATIVE_Y_ID)
#define M_NUMBER_OF_CHAINED_PIXELS           56L         // Already defined in    milblob.h, milmod.h
#define M_NUMBER_OF_CHAINED_EDGELS           M_NUMBER_OF_CHAINED_PIXELS
#define M_CHAIN_CODE                         58L         // Already defined in    milblob.h
#define M_FIRST_POINT_X                      75L         // Already defined in    milblob.h
#define M_FIRST_POINT_Y                      132L
#define M_FIRST_POINT                        (M_FIRST_POINT_X|M_FIRST_POINT_Y)
#define M_CHAIN_INDEX                        67L         // Already defined in    milblob.h, milmod.h
#define M_CHAIN_X                            65L         // Already defined in    milblob.h, milmod.h
#define M_CHAIN_Y                            144L        // Already defined in    milblob.h, milmod.h
#define M_CHAIN                              (M_CHAIN_X|M_CHAIN_Y)
#define M_VERTICES_CHAIN_INDEX               71L
#define M_VERTICES_INDEX                     79L
#define M_VERTEX_X                           68L                                
#define M_VERTEX_Y                           40L
#define M_VERTEX                             (M_VERTEX_X|M_VERTEX_Y)
#define M_VERTICES_X                         M_VERTEX_X
#define M_VERTICES_Y                         M_VERTEX_Y
#define M_VERTICES                           M_VERTEX
#define M_NUMBER_OF_VERTICES                 73L
#define M_NUMBER_OF_VERTEXES                 M_NUMBER_OF_VERTICES
#define M_NUMBER_OF_VERTEX                   M_NUMBER_OF_VERTICES
#define M_BULGE                              145L
#define M_BULGES                             M_BULGE


/***************************************************************************/
/* MedgeGetNeighbors()                                                     */
/***************************************************************************/
#define M_NEAREST_NEIGHBOR_RADIUS            116L
#define M_NEIGHBOR_MINIMUM_SPACING           117L
#define M_NEIGHBOR_MAXIMUM_NUMBER            118L
#define M_NEIGHBOR_SEARCH_RADIUS_MAX         120L
#define M_NEIGHBOR_SEARCH_RADIUS_MIN         121L
#define M_NEIGHBOR_POLARITY                  122L
#define M_NEIGHBOR_POLARITY_TOLERANCE        123L
#define M_NEIGHBOR_DIRECTION                 124L
#define M_NEIGHBOR_DIRECTION_TOLERANCE       125L
#define M_NEIGHBOR_DIRECTION_SIGN            126L
#define M_GET_EDGELS                         1L
#define M_GET_SUBEDGELS                      2L


/***************************************************************************/
/* MedgeDraw()                                                             */
/***************************************************************************/
#define M_DRAW_LABEL                         1L
#define M_DRAW_IMAGE                         2L          // Already defines in    milmod.h, milpat.h
#define M_DRAW_EDGES                         4L          // Already defines in    milmod.h, milmeas.h 
#define M_DRAW_EDGE                          M_DRAW_EDGES
#define M_DRAW_FERET_MAX                     0x008L      // Already defined in    milblob.h
#define M_DRAW_FERET_BOX                     16L         // Already defined in    milblob.h
#define M_DRAW_BOX                           32L         // Already defined in    milblob.h, milmod.h, milmeas.h, milpat.h
#define M_DRAW_POSITION                      64L         // Already defined in    milblob.h, milmod.h, milmeas.h, milpat.h
#define M_DRAW_CENTER_OF_GRAVITY             0x080L      // Already defined in    milblob.h
#define M_DRAW_INDEX                         0x100L
#define M_DRAW_MASK                          0x200L      
#define M_DRAW_VERTICES                      0x400L
#define M_DRAW_VERTEXES                      M_DRAW_VERTICES
#define M_DRAW_VERTEX                        M_DRAW_VERTICES
#define M_DRAW_SEGMENTS                      0x800L
#define M_DRAW_FERET_MIN                     0x1000L     // Already defined in    milblob.h
#define M_DRAW_FIRST_DERIVATIVE_X            0x2000L
#define M_DRAW_FIRST_DERIVATIVE_Y            0x4000L
#define M_DRAW_MAGNITUDE                     0x8000L
#define M_DRAW_SECOND_DERIVATIVE_X           0x10000L
#define M_DRAW_SECOND_DERIVATIVE_Y           0x20000L
#define M_DRAW_ANGLE                         0x40000L
#define M_DRAW_CROSS_DERIVATIVE              0x80000L
#define M_DRAW_GENERAL_FERET                 0x100000L
#define M_DRAW_EDGELS                        0x200000L
#define M_DRAW_ACTIVEMIL_INDEX               0x400000L
#define M_DRAW_VALUE                         0x800000L
#define M_DRAW_NEAREST_EDGELS                0x1000000L


/***************************************************************************/
/* Complementary definitions                                               */
/***************************************************************************/
#define M_ENABLE                             -9997L      // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h
#define M_DISABLE                            -9999L      // Already defined in    milblob.h, milcal.h, milmeas.h, mil.h
#define M_PACKED                             0x00020000L // Already defined in    mil.h
#define M_SUPPORTED                          0x20000000L // Already defined in    milmod.h
#define M_AVAILABLE                          0x40000000L // Already defined in    milmod.h                                             
#define M_TYPE_LABEL                         0x01000000L
#define M_TYPE_INDEX                         0x02000000L
#define M_ALL                                0x40000000L // Already defined in    milmod.h, milmeas.h, milocr.h,  milpat.h,  mil.h
#define M_DEFAULT                            0x10000000L // Already defined in    milmod.h, milocr.h,  milcode.h, mil.h
#define M_TYPE_LONG                          0x00040000L // Already defined in    milmod.h, milmeas.h, milcode.h, milblob.h, milcal.h, mil.h
#define M_TYPE_DOUBLE                        0x00100000L // Already defined in    milmod.h, milmeas.h, milcode.h, milblob.h, milcal.h, mil.h
#define M_TYPE_MIL_ID                        0x00400000L // Already defined in    mil.h
#define M_NUMBER_OF_RESULTS                  0x80000000L 
#define M_TRUE                               1L          // Already defined in    mil.h, milmeas.h
#define M_FALSE                              0L          // Already defined in    mil.h, milmeas.h
#define M_NO_CHECK                           0x80000000L
#define M_SAME                               -1L         // Already defined in    milmod.h, milmeas.h, milocr.h
#define M_SAME_OR_REVERSE                    5L          // Already defined in    milmod.h
#define M_SAME_EDGE                          67L
#define M_REVERSE                            4L          // Already defined in    milmod.h, mil.h
#define M_ANY                                0x11000000L // Already defined in    milmod.h, milmeas.h, milcode.h, milocr.h, mil.h
#define M_SIZE_X                             1002L       // Already defined in    milcode.h, mil.h
#define M_SIZE_Y                             1003L       // Already defined in    milcode.h, mil.h
#define M_SIZE_BAND                          1005L       // Already defined in    mil.h
#define M_TYPE                               1008L       // Already defined in    mil.h
#define M_SIGN                               1014L       // Already defined in    mil.h
#define M_SIZE_BIT                           1007L       // Already defined in    mil.h
#define M_WORLD                              0x2000L     // Already defined in    milcal.h
#define M_FULL_ANGLE                         78L
#define M_SIZE_BYTE                          5061L       // Already defined in    milmod.h, milcode.h, mil.h
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY                             (M_APP_INQUIRE_SYS_START+120L) /*5120*/    // Already defined in    milmod.h, milcode.h, mil.h
#else
#define M_MEMORY                             5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_EDGE_CONTOUR                       (M_EDGE_OBJECT | 0x00000001L) // Already defined in    mil.h
#define M_EDGE_CONTEXT                       (M_EDGE_OBJECT | 0x00000002L) // Already defined in    mil.h
#define M_EDGE_RESULT                        (M_EDGE_OBJECT | 0x00000004L) // Already defined in    mil.h
#define M_EDGE_RESULT_BUFFER                 M_EDGE_RESULT
#define M_INTERACTIVE                        M_NULL // Already defined in mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h


/***************************************************************************/
/*                 MilEdge CAPI function prototypes                        */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

/***************************************************************************/
/* MedgeAlloc()                                                            */
/***************************************************************************/
MFTYPE32 MIL_ID MFTYPE  MedgeAlloc         (MIL_ID       SystemId, 
                                            long         EdgeFinderType, 
                                            long         ControlFlag,
                                            MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeAllocResult()                                                      */
/***************************************************************************/
MFTYPE32 MIL_ID MFTYPE  MedgeAllocResult   (MIL_ID       SystemId,
                                            long         ControlFlag, 
                                            MIL_ID*      EdgeResultIdPtr);

/***************************************************************************/
/* MedgeControl()                                                          */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeControl       (MIL_ID       ContextOrResultId, 
                                            long         ControlType, 
                                            double       ControlValue);

/***************************************************************************/
/* MedgeInquire()                                                          */
/***************************************************************************/
MFTYPE32 long MFTYPE    MedgeInquire       (MIL_ID       ContextOrResultId, 
                                            long         InquireType,
                                            void*        UserVarPtr);

/***************************************************************************/
/* MedgeSelect()                                                           */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeSelect        (MIL_ID       EdgeResultId, 
                                            long         Operation,
                                            long         Feature,
                                            long         Condition,
                                            double       Param1,
                                            double       Param2);

/***************************************************************************/
/* MedgeGetResult()                                                        */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeGetResult     (MIL_ID       ResultId, 
                                            long         ResultIndex,
                                            long         ResultType,
                                            void*        FirstResultArrayPtr,
                                            void*        SecondResultArrayPtr);

/***************************************************************************/
/* MedgeDraw()                                                             */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeDraw          (MIL_ID       GraphContId,
                                            MIL_ID       EdgeResultId,
                                            MIL_ID       DestImageId,
                                            long         Operation,
                                            long         IndexOrLabel,
                                            long         ControlFlag);

/***************************************************************************/
/* MedgeCalculate()                                                        */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeCalculate     (MIL_ID       ContextId, 
                                            MIL_ID       SourceImageId,
                                            MIL_ID       SourceDeriv1Id,
                                            MIL_ID       SourceDeriv2Id,
                                            MIL_ID       SourceDeriv3Id,
                                            MIL_ID       EdgeResultId,
                                            long         ControlFlag);

/***************************************************************************/
/* MedgeSave()                                                             */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeSave          (MIL_TEXT_PTR FileName,
                                            MIL_ID       ContextOrResultId,
                                            long         ControlFlag);

/***************************************************************************/
/* MedgeRestore()                                                          */
/***************************************************************************/
MFTYPE32 MIL_ID  MFTYPE MedgeRestore       (MIL_TEXT_PTR FileName,
                                            MIL_ID       SystemId, 
                                            long         ControlFlag,
                                            MIL_ID*      ContextIdPtr);

/***************************************************************************/
/* MedgeMask()                                                             */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeMask          (MIL_ID       ContextId,
                                            MIL_ID       MaskImageId,
                                            long         ControlFlag);

/***************************************************************************/
/* MedgeGetNeighbors()                                                     */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeGetNeighbors  (MIL_ID       ResultId,
                                            long         SizeOfArray,
                                            double*      SrcArrayX,
                                            double*      SrcArrayY,
                                            double*      SrcArrayAngle,
                                            double*      DstArrayX,
                                            double*      DstArrayY,
                                            long*        Index,
                                            long*        Label,
                                            long         ControlFlag);

/***************************************************************************/
/* MedgePut()                                                              */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgePut          (MIL_ID  ResultId,
                                           long    NbEdgels,
                                           long*   pArrayLabel,
                                           double* pArrayX,
                                           double* pArrayY,
                                           double* pArrayAngle,
                                           double* pArrayMagnitude,
                                           long    ControlFlag );


/***************************************************************************/
/* MedgeFree()                                                             */
/***************************************************************************/
MFTYPE32 void MFTYPE    MedgeFree          (MIL_ID       ObjectId);



#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILEDGEEXTR_H__ */
