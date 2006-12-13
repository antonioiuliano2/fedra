/***************************************************************************/
/*

    Filename:  MILCAL.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines and the prototypes for the
               MIL calibration module. (Mcal...).

    COPYRIGHT (c) 1993-1997 Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILCAL_H__
#define __MILCAL_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      Calibration CAPI defines                           */
/***************************************************************************/

/***************************************************************************/
/* McalAlloc()                                                             */
/***************************************************************************/

/***************************************************************************/
/* McalGrid(), McalList()                                                  */
/***************************************************************************/

/* Calibration type */
#define M_LINEAR_INTERPOLATION                 0x01L
#define M_PERSPECTIVE_TRANSFORMATION           0x02L
/* Internal */
#define M_TSAI                                 0x08L
/* Calibration grid type */
#define M_CIRCLE_GRID                         0x001L
/* Optional ModeFlag that can be
   added to the grid type */
#define M_Y_AXIS_UP                           0x010L
#define M_Y_AXIS_DOWN                         0x020L


/***************************************************************************/
/* McalTransformCoordinate() and McalTransformData()                       */
/***************************************************************************/

/* Transform type */
#define M_PIXEL_TO_WORLD                          1L
#define M_WORLD_TO_PIXEL                          2L

/* Aspect ratio pre-correction flags */
#define M_PRE_CORRECTED                       0x100L

/* Data type */
#define M_LENGTH_X                              0x3L
#define M_LENGTH_Y                              0x4L
#define M_LENGTH                         0x00002000L  /* (Already defined in milmeas.h) */
#define M_AREA                                    2L  /* (Already defined in milblob.h) */
#define M_ANGLE						        0x00000800L  /* (Already defined in milmeas.h) */


/***************************************************************************/
/* McalControl() and/or McalInquire()                                      */
/***************************************************************************/

/* Contol parameters. */
#define M_FOREGROUND_VALUE	                     4L
#define M_CAMERA_POSITION_X                     100L
#define M_CAMERA_POSITION_Y                     101L
#define M_CAMERA_POSITION_Z                     102L
#define M_TSAI_CENTER_X                         103L
#define M_TSAI_CENTER_Y                         104L
#define M_TSAI_CELL_SIZE_X                      105L
#define M_TSAI_CELL_SIZE_Y                      106L
#define M_TSAI_NUMBER_CELL_X                    107L
#define M_TSAI_NUMBER_PIXEL_X                   108L
#define M_GRID_ORIGIN_X                         109L
#define M_GRID_ORIGIN_Y                         110L
#define M_GRID_ORIGIN_Z                         111L
#define M_RELATIVE_ORIGIN_X                     112L
#define M_RELATIVE_ORIGIN_Y                     113L
#define M_RELATIVE_ORIGIN_Z                     114L
#define M_RELATIVE_ORIGIN_ANGLE                 115L
#define M_ROW_NUMBER                            116L
#define M_COLUMN_NUMBER                         117L
#define M_ROW_SPACING                           118L
#define M_COLUMN_SPACING                        119L
#define M_INPUT_COORDINATE_SYSTEM               121L
#define M_OUTPUT_COORDINATE_SYSTEM              122L
#define M_ASPECT_RATIO                         6001L /* (Already defined in mil.h) */
#define M_ASPECT_RATIO_AUTO_SETTING             123L
#define M_ENHANCED_RESULTS_ACCURACY             124L
#define M_ASSOCIATED_CALIBRATION                125L
#define M_CORRECTION_STATE                      126L
#define M_PERSPECTIVE_EFFECT                    127L
#define M_PARTIALLY_ALIGNED                     128L
#define M_COMPLETELY_ALIGNED                    129L
#define M_CALIBRATION_SUCCESSFUL                130L
#define M_CALIBRATION_RESET                     245L
    
#define M_TRANSFORM_FILL_MODE                   131L
#define M_TRANSFORM_CACHE                       132L
#define M_TRANSFORM_AXIS_ALIGNMENT              133L
#define M_TRANSFORM_ACCURACY                    134L

#define M_USER_STRING                           136L
#define M_USER_STRING_LENGTH                    137L

#define M_DEBUG_SAVING                          138L

#define M_PIXEL_SIZE_X                          139L
#define M_PIXEL_SIZE_Y                          140L

#define M_CORRECTED_OFFSET_X                    141L
#define M_CORRECTED_OFFSET_Y                    142L

#define M_CHILD_CALIBRATION_OFFSET_X            145L
#define M_CHILD_CALIBRATION_OFFSET_Y            146L

#define M_NUMBER_OF_CALIBRATION_POINTS          200L
#define M_CALIBRATION_IMAGE_POINTS_X            201L
#define M_CALIBRATION_IMAGE_POINTS_Y            202L
#define M_CALIBRATION_WORLD_POINTS_X            203L
#define M_CALIBRATION_WORLD_POINTS_Y            204L
#define M_CALIBRATION_MODE                      205L
#define M_GRID_TYPE                             206L


#define M_AVERAGE_PIXEL_ERROR                0x1000L
#define M_MAXIMUM_PIXEL_ERROR                0x2000L
#define M_AVERAGE_WORLD_ERROR                0x4000L
#define M_MAXIMUM_WORLD_ERROR                0x8000L
#define M_ALL_PIXEL_ERRORS                   0x3000L
#define M_ALL_WORLD_ERRORS                   0xC000L
#define M_ALL_ERRORS                         0xF000L


/* Flag that can be added to the contol parameters. */
#define M_TYPE_CHAR                      0x00010000L /* (Already defined in mil.h) */
#define M_TYPE_SHORT                     0x00020000L /* (Already defined in mil.h) */
#define M_TYPE_LONG                      0x00040000L /* (Already defined in mil.h) */
#define M_TYPE_FLOAT                     0x00080000L /* (Already defined in mil.h) */
#define M_TYPE_DOUBLE                    0x00100000L /* (Already defined in mil.h) */


/* Contol values. */
#define M_PIXEL                              0x1000L
#define M_WORLD                              0x2000L
#define M_CLIP                               0x0010L
#define M_FIT                                0x0020L
#define M_COMPLETE                       0x00000000L /* (Already defined in mil.h) */
#define M_PARTIAL                        0x00000001L /* (Already defined in mil.h) */
#define M_ENABLE                              -9997L /* (Already defined in mil.h) */
#define M_DISABLE                             -9999L /* (Already defined in mil.h) */
#define M_FULL_CORRECTION                    0x0001L


/***************************************************************************/
/* McalRestore(), McalSave(), McalStream()                                 */
/***************************************************************************/
#define M_INTERACTIVE                         M_NULL // Already defined in mil.h, milcode.h, miledge.h, milmeas.h, milocr.h, milpat.h, milmod.h


/***************************************************************************/
/* McalTransformImage()                                                      */
/***************************************************************************/

/***************************************************************************/
/*                 Calibration CAPI function prototype                     */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

#ifndef M_MIL_75_COMPATIBLE
MFTYPE32 MIL_ID MFTYPE McalAlloc(MIL_ID SystemId,
                                 long Mode,
                                 long ModeFlag,
                                 MIL_ID *ContextIdPtr);
#else
MFTYPE32 MIL_ID MFTYPE McalAlloc(long Mode,
                                 long ModeFlag,
                                 MIL_ID *ContextIdPtr);
#endif

MFTYPE32 MIL_ID MFTYPE McalChild(MIL_ID CalParentId,
                                 long lOffX,
                                 long lOffY,
                                 MIL_ID *ContextIdPtr);

MFTYPE32 void MFTYPE McalFree(MIL_ID CalibrationId);

MFTYPE32 void MFTYPE McalAssociate(MIL_ID CalibrationId,
                                   MIL_ID AppOrMilObjectId,
                                   long ControlFlag);

MFTYPE32 void MFTYPE McalGrid(MIL_ID CalibrationId,
                              MIL_ID SrcImageId,
                              double GridOffsetX,
                              double GridOffsetY,
                              double GridOffsetZ,
                              long   RowNumber,
                              long   ColumnNumber,
                              double RowSpacing,
                              double ColumnSpacing,
                              long   Mode,
                              long   ModeFlag);

MFTYPE32 void MFTYPE McalList(MIL_ID CalibrationId,
                              double * XPixArray,
                              double * YPixArray,
                              double * XWorldArray,
                              double * YWorldArray,
                              double * ZWorld,
                              long   NumData,
                              long   Mode,
                              long   ModeFlag);

MFTYPE32 void MFTYPE McalRelativeOrigin(MIL_ID CalibrationID,
                                        double XOffset,
                                        double YOffset,
                                        double ZOffset,
                                        double AngularOffset,
                                        long ControlFlag);

MFTYPE32 void MFTYPE McalTransformCoordinate(MIL_ID CalibrationOrImageId,
                                             long TransformType,
                                             double X,
                                             double Y,
                                             double * ResXPtr,
                                             double * ResYPtr);

MFTYPE32 void MFTYPE McalTransformCoordinateList(MIL_ID CalibrationOrImageId,
                                                 long TransformType,
                                                 long NumPoints,
                                                 double *SrcXPtr,
                                                 double *SrcYPtr,
                                                 double *ResXPtr,
                                                 double *ResYPtr);

MFTYPE32 void MFTYPE McalTransformResult(MIL_ID CalibrationOrImageId,
                                         long TransformType,
                                         long DataType,
                                         double Data,
                                         double * ResData);

MFTYPE32 void MFTYPE McalTransformResultInRegion(MIL_ID CalibrationOrImageId,
                                                 long TransformType,
                                                 long DataType,
                                                 long lBoxMinX,
                                                 long lBoxMinY,
                                                 long lBoxMaxX,
                                                 long lBoxMaxY,
                                                 double Data,
                                                 double * ResData);

MFTYPE32 void MFTYPE McalTransformImage(MIL_ID SrcImageId,
                                        MIL_ID DestImageId,
                                        MIL_ID CalibrationId,
                                        long InterpolationType,
                                        long OperationType,
                                        long ControlFlag);

MFTYPE32 void MFTYPE McalControl(MIL_ID CalibrationId,
                                 long   Parameter,
                                 double Value);

MFTYPE32 long MFTYPE McalInquire(MIL_ID CalibrationId,
                                 long   Parameter,
                                 void   * ValuePtr);

MFTYPE32 void MFTYPE McalSave(MIL_TEXT_PTR FileName,
                              MIL_ID CalibrationId,
                              long ControlFlag);

#ifndef M_MIL_75_COMPATIBLE
MFTYPE32 MIL_ID MFTYPE McalRestore(MIL_TEXT_PTR FileName,
                                   MIL_ID SystemId,
                                   long ControlFlag,
                                   MIL_ID * CalibrationIdPtr);
#else
MFTYPE32 MIL_ID MFTYPE McalRestore(MIL_TEXT_PTR FileName,
                                   long ControlFlag,
                                   MIL_ID * CalibrationIdPtr);
#endif


MFTYPE32 void MFTYPE McalStream( MIL_TEXT_PTR MemPtrOrFileName,
                                 MIL_ID SystemId,
                                 long   Operation, long StreamType,
                                 double Version,   long ControlFlag,
                                 MIL_ID *ObjectIdPtr                 );

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILCAL_H__ */
