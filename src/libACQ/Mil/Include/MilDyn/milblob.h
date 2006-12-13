/***************************************************************************/
/*

    Filename:  MILBLOB.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines for the MIL blob
               analysis module. (Mblob...).

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILBLOB_H__
#define __MILBLOB_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/* Binary only */

#define M_LABEL_VALUE              1L
#define M_AREA                     2L
#define M_PERIMETER                3L
#define M_FERET_X                 72L
#define M_FERET_Y                  5L
#define M_BOX_X_MIN                6L
#define M_BOX_Y_MIN                7L
#define M_BOX_X_MAX                8L
#define M_BOX_Y_MAX                9L
#define M_FIRST_POINT_X           75L
#define M_FIRST_POINT_Y           132L
#define M_AXIS_PRINCIPAL_LENGTH   12L
#define M_AXIS_SECONDARY_LENGTH   13L
#define M_FERET_MIN_DIAMETER      14L
#define M_FERET_MIN_ANGLE         15L
#define M_FERET_MAX_DIAMETER      16L
#define M_FERET_MAX_ANGLE         17L
#define M_FERET_MEAN_DIAMETER     18L
#define M_CONVEX_AREA             19L
#define M_CONVEX_PERIMETER        20L
#define M_X_MIN_AT_Y_MIN          21L
#define M_X_MAX_AT_Y_MAX          22L
#define M_Y_MIN_AT_X_MAX          23L
#define M_Y_MAX_AT_X_MIN          24L
#define M_COMPACTNESS             25L
#define M_NUMBER_OF_HOLES         26L
#define M_FERET_ELONGATION        27L
#define M_ROUGHNESS               28L
#define M_EULER_NUMBER            47L
#define M_LENGTH          0x00002000L
#define M_BREADTH                 49L
#define M_ELONGATION              50L
#define M_INTERCEPT_0             51L
#define M_INTERCEPT_45            52L
#define M_INTERCEPT_90            53L
#define M_INTERCEPT_135           54L
#define M_NUMBER_OF_RUNS          55L
#define M_GENERAL_FERET        0x400L

/* Greyscale only (ie, trivial for binary) */

#define M_SUM_PIXEL               29L
#define M_MIN_PIXEL               30L
#define M_MAX_PIXEL               31L
#define M_MEAN_PIXEL              32L
#define M_SIGMA_PIXEL             33L
#define M_SUM_PIXEL_SQUARED       46L

/* Binary or greyscale (might want both for a greyscale image) */

#define M_CENTER_OF_GRAVITY_X     34L
#define M_CENTER_OF_GRAVITY_Y     35L
#define M_MOMENT_X0_Y1            36L
#define M_MOMENT_X1_Y0            37L
#define M_MOMENT_X1_Y1            38L
#define M_MOMENT_X0_Y2            39L
#define M_MOMENT_X2_Y0            40L
#define M_MOMENT_CENTRAL_X1_Y1    41L
#define M_MOMENT_CENTRAL_X0_Y2    42L
#define M_MOMENT_CENTRAL_X2_Y0    43L
#define M_AXIS_PRINCIPAL_ANGLE    44L
#define M_AXIS_SECONDARY_ANGLE    45L
#define M_GENERAL_MOMENT       0x800L

/* General moment type */

#define M_ORDINARY             0x400L
#define M_CENTRAL              0x800L

/* the defines are used by the active mil BlobAnalysis.Results.Get method YV */

#define M_RUN_LENGTHS            100L
#define M_RUN_X_COORDINATES      101L
#define M_RUN_Y_COORDINATES      102L

/* Sorting options. */
#define M_NO_SORT         0L
#define M_SORT1_UP        0x02000000L
#define M_SORT2_UP        0x04000000L
#define M_SORT3_UP        0x06000000L
#define M_SORT1_DOWN      0x0A000000L
#define M_SORT2_DOWN      0x0C000000L
#define M_SORT3_DOWN      0x0E000000L

/* Chain code specific features */
#define M_NUMBER_OF_CHAINED_PIXELS 56L
#define M_CHAIN_X                  65L
#define M_CHAIN_Y                 144L
#define M_CHAIN_INDEX              67L

/* Short cuts for enabling multiple features */

#define M_ALL_FEATURES         0x100L   /* All except general Feret */
#define M_BOX                  0x101L
#define M_CONTACT_POINTS       0x102L
#define M_CENTER_OF_GRAVITY    0x103L
#define M_NO_FEATURES          0x104L   /* Still do label and area */
#define M_CHAINS               0x105L

/* MblobControl() and/or MblobInquire() values */

#define M_IMAGE_TYPE               1L
#define M_BLOB_IDENTIFICATION      2L
#define M_LATTICE                  3L
#define M_FOREGROUND_VALUE         4L
#define M_PIXEL_ASPECT_RATIO       5L
#define M_NUMBER_OF_FERETS        63L
#define M_IMAGE_START_X            7L
#define M_IMAGE_START_Y            8L
#define M_RESET                    9L
#define M_IDENTIFIER_IMAGE        10L
#define M_GREY_IMAGE              11L
#define M_IDENTIFIER_START_BIT    12L
#define M_GREY_START_BIT          13L
#define M_SAVE_RUNS               14L
#define M_IDENTIFIER_TYPE         15L
#define M_MAX_LABEL               16L
#define M_SELECT_FEATURE          17L
#define M_AUTO_RESET      0x00002000L  // Already defined in mil.h


/* Blob identification values */

#define M_WHOLE_IMAGE              1L
#define M_INDIVIDUAL               2L
#define M_LABELLED                 4L
#define M_LABELED          M_LABELLED  // One "L" is american english while
                                       // two "LL" is British (ref. Thalia tech writ.).

/* Enabling value. */

#define M_ENABLE               -9997L
#define M_DISABLE              -9999L

/* Lattice values */

#define M_4_CONNECTED     0x00000010L
#define M_8_CONNECTED     0x00000020L


/* Foreground values */

#define M_NONZERO               0x80L
#define M_ZERO                 0x100L
#define M_NON_ZERO          M_NONZERO
#define M_FOREGROUND_WHITE      0x80L
#define M_FOREGROUND_BLACK     0x100L


/* Data types for results */

#define M_TYPE_CHAR       0x00010000L
#define M_TYPE_SHORT      0x00020000L
#define M_TYPE_LONG       0x00040000L
#define M_TYPE_FLOAT      0x00080000L
#define M_TYPE_DOUBLE     0x00100000L


/* Conditional test */

#define M_ALWAYS              0L
#define M_IN_RANGE            1L
#define M_OUT_RANGE           2L
#define M_EQUAL               3L
#define M_NOT_EQUAL           4L
#define M_GREATER             5L
#define M_LESS                6L
#define M_GREATER_OR_EQUAL    7L
#define M_LESS_OR_EQUAL       8L


/* Image type values (and flags for blob_enable_feature()) */

#define M_BINARY          0x00001000L
#define M_GREYSCALE           0x0200L
#define M_GRAYSCALE       M_GREYSCALE


/* MblobReconstruct() defines */

#define M_RECONSTRUCT_FROM_SEED     1L
#define M_ERASE_BORDER_BLOBS        2L
#define M_FILL_HOLES                3L
#define M_EXTRACT_HOLES             4L
#define M_SEED_PIXELS_ALL_IN_BLOBS  1L
#define M_FOREGROUND_ZERO           2L

/* MblobDraw() defines */
#define M_DRAW_BLOBS             0x400  
#define M_DRAW_BLOBS_CONTOUR     0x100  
#define M_DRAW_HOLES             0x2000  
#define M_DRAW_HOLES_CONTOUR     0x4000  
#define M_DRAW_BOX               32L     // also used in m*draw
#define M_DRAW_POSITION          64L     // also used in m*draw
#define M_DRAW_CENTER_OF_GRAVITY 0x080L
#define M_DRAW_FERET_MIN         0x1000L
#define M_DRAW_FERET_MAX         0x008L
#define M_DRAW_FERET_BOX         16L
#define M_DRAW_AXIS              0x200L  // also used in mmoddraw
#define M_DRAW_BOX_CENTER        1L      // also define in milmeas



/* Miscellaneous */
#define M_ALL_BLOBS           0x40000000L
#define M_INCLUDED_BLOBS      0x80000000L
#define M_EXCLUDED_BLOBS      0x20000000L
#define M_ALL_BLOBS_OLD       0x105L
#define M_INCLUDED_BLOBS_OLD  0x106L
#define M_EXCLUDED_BLOBS_OLD  0x107L

#define M_INCLUDE                  1L
#define M_EXCLUDE                  2L
#define M_DELETE                   3L
#define M_MIN_FERETS               2L
#define M_MAX_FERETS              64L
#define M_INCLUDE_ONLY         0x101L
#define M_EXCLUDE_ONLY         0x102L
#define M_CONTOUR              0x800L
#define M_DUMMY               0x4000L
#define M_CLEAR           0x00000001L
#define M_NO_CLEAR        0x00000002L

/* Other defines are in MIL.H */

/********************************************************************
 * Function prototypes
 ********************************************************************/
#ifndef __midl

MFTYPE32 MIL_ID MFTYPE MblobAllocFeatureList(MIL_ID SystemId,MIL_ID MPTYPE *FeatureListPtr);
MFTYPE32 MIL_ID MFTYPE MblobAllocResult(MIL_ID SystemId, MIL_ID MPTYPE *BlobResIdPtr);
MFTYPE32 void MFTYPE MblobCalculate(MIL_ID BlobIdentImageId, MIL_ID GreyImageId,
              MIL_ID FeatureListId, MIL_ID BlobResId);
MFTYPE32 void MFTYPE MblobControl(MIL_ID BlobResId, long ProcMode, double Value);
MFTYPE32 void MFTYPE MblobFill(MIL_ID BlobResId, MIL_ID TargetImageId, long Mode,
              long Value);
MFTYPE32 void MFTYPE MblobFree(MIL_ID BlobResId);
MFTYPE32 long MFTYPE MblobGetLabel(MIL_ID BlobResId, long XPos, long YPos,
              long MPTYPE *BlobLabelPtr);
MFTYPE32 long MFTYPE MblobGetNumber(MIL_ID BlobResId, long MPTYPE *CountPtr);
MFTYPE32 void MFTYPE MblobGetResult(MIL_ID BlobResId, long Feature,
              void MPTYPE *TargetArrayPtr);
MFTYPE32 void MFTYPE MblobGetResultSingle(MIL_ID BlobResId, long BlobLabel,
              long Feature, void MPTYPE *ValuePtr);
MFTYPE32 void MFTYPE MblobGetRuns(MIL_ID BlobResId, long BlobLabel, long ArrayType,
              void MPTYPE *RunXPtr, void MPTYPE *RunYPtr, void MPTYPE *RunLengthPtr);
MFTYPE32 long MFTYPE MblobInquire(MIL_ID BlobResId, long ProcMode, void MPTYPE *ValuePtr);
MFTYPE32 void MFTYPE MblobLabel(MIL_ID BlobResId, MIL_ID TargetImageId, long Mode);
MFTYPE32 void MFTYPE MblobSelect(MIL_ID BlobResId, long Operation, long Feature,
              long Condition, double CondLow, double CondHigh);
MFTYPE32 void MFTYPE MblobSelectFeature(MIL_ID FeatureListId, long Feature);
MFTYPE32 void MFTYPE MblobSelectFeret(MIL_ID FeatureListId, double Angle);
MFTYPE32 void MFTYPE MblobSelectMoment(MIL_ID FeatureListId, long MomType,
              long XMomOrder, long YMomOrder);
MFTYPE32 void MFTYPE MblobReconstruct(MIL_ID srce_image_id, MIL_ID seed_image_id,
              MIL_ID dest_image_id, long operation, long mode);

MFTYPE32 void MFTYPE MblobDraw(MIL_ID GraphContId, MIL_ID ResultId, MIL_ID DestImageId,
                     long Operation, long Label, long ControlFlag);

#endif


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILBLOB_H__ */
