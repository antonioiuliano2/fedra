/***************************************************************************/
/*

    Filename:  MILMEAS.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines for the MIL measurement
               module. (Mmeas...).

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMEAS_H__
#define __MILMEAS_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// If we get any warning with any of these defines, then there are
// important changes to do with the Save and Restore functions.
//
// When it happens, you will have to declare the current define as "old"
// and redeclare the define with the new value. Then you will have to
// modify both the Save and the Restore functions so that they use the good
// define depending on the version.
// This is important because it will affect both backward and forward
// compatibility with other versions of MIL.
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
////////////////////////////////////////////////////////////////////////////

/**************************************************************************/
/* CAPI defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* MmeasAllocMarker                                                       */
/**************************************************************************/

#define M_POINT                           1L
#define M_EDGE                            2L
#define M_STRIPE                          3L
#define M_CIRCLE                 0x00000008L   // 8   Already defined in milmod.h
#define M_EDGE_LIST                   M_EDGE
#define M_STRIPE_LIST               M_STRIPE


/**************************************************************************/
/* MmeasAllocResult                                                       */
/**************************************************************************/
#define M_CALCULATE                       1L


/**************************************************************************/
/* Bitwise values that the followings cannot take                         */
/**************************************************************************/

#define M_TYPE_DOUBLE                     0x00100000L
#define M_TYPE_LONG                       0x00040000L
#define M_MULTI_MARKER_MASK               0x000003FFL
#define M_EDGE_FIRST                      0x20000000L//0x00100000L 
#define M_EDGE_SECOND                     0x40000000L//0x00200000L  
#define M_WEIGHT_FACTOR                   0x01000000L
#define M_MIN                             0x02000000L
#define M_MAX                             0x04000000L
#define M_MEAN                            0x08000000L
#define M_STANDARD_DEVIATION              0x10000000L 
#define M_MEAS_FUTURE_USE_1               0x20000000L 
#define M_MEAS_FUTURE_USE_2               0x40000000L 
#define M_MEAS_FUTURE_USE_3               0x80000000L 

/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() parameters :                                          */
/**************************************************************************/

#define M_POSITION_VARIATION              0x00008000L
#define M_ANGLE                           0x00000800L
#define M_WIDTH                           0x00010000L
#define M_WIDTH_VARIATION                 0x00020000L
#define M_POLARITY                        0x00004000L
#define M_CONTRAST                        0x00001000L
#define M_LENGTH                          0x00002000L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L
#define M_EDGE_INSIDE                     0x00000080L
#define M_POSITION                        0x00000402L
#define M_SPACING                         0x00000200L      
#define M_NUMBER                                1009L
#define M_NUMBER_MIN                            1010L
#define M_FIT_ERROR                       0x00001700L  // already defined in MilMod.h

#define M_SPACING_VARIATION               0x00000100L      
#define M_SCORE                           0x00001400L
#define M_ORIENTATION                     0x00002400L
#define M_POSITION_X                      0x00003400L
#define M_POSITION_Y                      0x00004400L
#define M_CONTRAST_VARIATION              0x00005400L
#define M_EDGE_STRENGTH                   0x00006400L
#define M_EDGE_STRENGTH_VARIATION         0x00007400L
#define M_EDGE_INSIDE_VARIATION           0x00008400L
#define M_BOX_ORIGIN                      0x00009400L
#define M_BOX_SIZE                        0x0000A400L
#define M_BOX_CENTER                      0x0000B400L
#define M_BOX_FIRST_CENTER                0x0000C400L
#define M_BOX_SECOND_CENTER               0x0000D400L
#define M_BOX_ANGLE_MODE                  0x0000E400L
#define M_BOX_ANGLE                       0x0000F400L
#define M_BOX_ANGLE_DELTA_NEG             0x00010400L
#define M_BOX_ANGLE_DELTA_POS             0x00011400L
#define M_BOX_ANGLE_TOLERANCE             0x00012400L
#define M_BOX_ANGLE_ACCURACY              0x00013400L
#define M_BOX_ANGLE_INTERPOLATION_MODE    0x00014400L
#define M_EDGE_THRESHOLD                  0x00015400L
#define M_MARKER_REFERENCE                0x00016400L
#define M_BOX_ANGLE_SIZE                  0x00017400L
#define M_MARKER_TYPE                     0x00018400L
#define M_CONTROL_FLAG                    0x00019400L
#define M_POSITION_MIN                    0x0001A400L
#define M_POSITION_MAX                    0x0001B400L
#define M_BOX_EDGE_VALUES                 0x0001C400L
#define M_ANY_ANGLE                       0x0001D400L
#define M_VALID_FLAG                      0x0001E400L
#define M_BOX_CORNER_TOP_LEFT             0x0001F400L
#define M_BOX_CORNER_TOP_RIGHT            0x00020400L
#define M_BOX_CORNER_BOTTOM_LEFT          0x00021400L
#define M_BOX_CORNER_BOTTOM_RIGHT         0x00022400L
#define M_BOX_EDGE_VALUES_NUMBER          0x00023400L
#define M_POSITION_INSIDE_STRIPE          0x00024400L
#define M_BOX_ANGLE_REFERENCE             0x00025400L
#define M_TOTAL_SCORE                     0x00029400L
#define M_BOX_RADIUS                      0x0002A400L
#define M_BOX_START_ANGLE                 0x0002B400L
#define M_BOX_END_ANGLE                   0x0002C400L
#define M_ALL                             0x40000000L // Already defined in Mil.h
#define M_ALL_OLD                                  0L

#define M_MAJOR_AXIS_MIN                         214L       // Already defined in MilMod.h
#define M_MAJOR_AXIS                             215L       // Already defined in MilMod.h 
#define M_MAJOR_AXIS_MAX                         216L       // Already defined in MilMod.h                         
#define M_RADIUS                          M_MAJOR_AXIS      // Already defined in MilMod.h
#define M_RADIUS_MIN                      M_MAJOR_AXIS_MIN  // Already defined in MilMod.h
#define M_RADIUS_MAX                      M_MAJOR_AXIS_MAX  // Already defined in MilMod.h
#define M_RADIUS_VARIATION                       217L                    

#define M_CONTROL_FLAG                    0x00019400L
#define M_ZERO_OFFSET_X                            1L
#define M_ZERO_OFFSET_Y                            2L
#define M_PIXEL_ASPECT_RATIO                       5L
#define M_PIXEL_ASPECT_RATIO_INPUT                 6L
#define M_PIXEL_ASPECT_RATIO_OUTPUT                7L

#define M_RESULT_TYPE                              1L
#define M_DISTANCE                        0x00080000L
#define M_DISTANCE_X                      0x00081000L
#define M_DISTANCE_Y                      0x00082000L
#define M_ANGLE                           0x00000800L
#define M_LINE_EQUATION                   0x00800000L
#define M_LINE_EQUATION_SLOPE             0x00801000L
#define M_LINE_EQUATION_INTERCEPT         0x00802000L
/* Internal */
#define M_REF_POSITION_MARKER_FIRST       0x00803000L
#define M_REF_POSITION_MARKER_SECOND      0x00804000L
#define M_ASPECT_RATIO_MARKER_FIRST       0x00805000L
#define M_ASPECT_RATIO_MARKER_SECOND      0x00806000L


/**************************************************************************/
/* MmeasInquire(), MmeasSetMarker(), MmeasGetResult(), MmeasFindMarker(), */
/* MmeasCalculate() values :                                              */
/**************************************************************************/

#define M_VERTICAL                      1L
#define M_HORIZONTAL                    2L
#define M_ANY                  0x11000000L
#define M_POSITIVE                      2L
#define M_NEGATIVE                     -2L /*Must be the additive inverse of M_POSITIVE*/
#define M_OPPOSITE                      3L
#define M_SAME                         -1L /* For spacing */
#ifndef M_MIL_75_COMPATIBLE
#define M_NORMAL                        8L
#else
#define M_NORMAL                        1L
#endif //M_MIL_75_COMPATIBLE
#define M_CORRECTED                     2L
#define M_DISABLE                   -9999L
#define M_ENABLE                    -9997L
#define M_NEAREST_NEIGHBOR     0x00000040L
#define M_BILINEAR             0x00000008L
#define M_BICUBIC              0x00000010L
#define M_YES                           1L
#define M_NO                            0L
#define M_TRUE                          1L
#define M_FALSE                         0L
#define M_INVALID                      -1L
#define M_SMOOTHNESS                  108L      //  already define in milmod.h



/**************************************************************************/
/* MmeasDraw()                                                            */
/**************************************************************************/

#define M_DRAW_BOX_CENTER          1L
#define M_DRAW_LINE                2L
#define M_DRAW_EDGES               4L
#define M_DRAW_WIDTH               8L
#define M_DRAW_POSITION_VARIATION  16L
#define M_DRAW_BOX                 32L    // also used by Mblobdraw and MpatDraw and Mmoddraw 
#define M_DRAW_POSITION            64L    // also used by Mblobdraw and MpatDraw and Mmoddraw 
#define M_DRAW_EDGES_PROFILE       128L
#define M_DRAW_SPACING             256L
#define M_DRAW_WIDTH_VARIATION     512L
#define M_DRAW_IN_BOX              0x100000L

#define M_RESULT                   0x00001000L // Also used by MmodDraw 
#define M_MARKER                   504L

#define M_ALL                      0x40000000L // Also used by MmodDraw


/**************************************************************************/
/* MmeasRestoreMarker(), MmeasSaveMarker                                  */
/**************************************************************************/
#define M_INTERACTIVE                      M_NULL // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milocr.h, milpat.h, milmod.h


/**************************************************************************/
/* Utility defines                                                        */
/**************************************************************************/

#define M_INFINITE_SLOPE    (1.0E+300)


// !!! MAP FOR OLD DEFINES                                
#if OldDefinesSupport                                     
#define M_BOX_EDGES_STRENGTH              M_BOX_EDGE_VALUES
#define M_BOX_EDGES_STRENGTH_NUMBER       M_BOX_EDGE_VALUES_NUMBER
#endif
// !!! MAP FOR OLD DEFINES                                

/**************************************************************************/
/* Function prototypes                                                    */
/**************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MmeasAllocMarker  (MIL_ID SystemId,
                                          long   MarkerType,
                                          long   ControlFlag,
                                          MIL_ID *MarkerIdPtr);

MFTYPE32 MIL_ID MFTYPE MmeasAllocResult  (MIL_ID SystemId,
                                          long   ResultBufferType,
                                          MIL_ID MPTYPE *ResultIdPtr);

MFTYPE32 MIL_ID MFTYPE MmeasAllocContext (MIL_ID SystemId,
                                          long   ControlFlag,
                                          MIL_ID *ContextId);

MFTYPE32 void   MFTYPE MmeasFree         (MIL_ID MarkerOrResultIdOrContextId);

MFTYPE32 void   MFTYPE MmeasSaveMarker   (MIL_TEXT_PTR FileName,
                                          MIL_ID MarkerId,
                                          long   ControlFlag);

MFTYPE32 MIL_ID MFTYPE MmeasRestoreMarker(MIL_TEXT_PTR FileName,
                                          MIL_ID SystemId,
                                          long   ControlFlag,
                                          MIL_ID *MarkerIdPtr);

MFTYPE32 void   MFTYPE MmeasSetMarker    (MIL_ID MarkerId,
                                          long   Parameter,
                                          double FirstValue,
                                          double SecondValue);

MFTYPE32 void   MFTYPE MmeasFindMarker   (MIL_ID ContextId,
                                          MIL_ID ImageId,
                                          MIL_ID MarkerId,
                                          long   MeasurementList);

MFTYPE32 void   MFTYPE MmeasCalculate    (MIL_ID ContextID,
                                          MIL_ID Marker1Id,
                                          MIL_ID Marker2Id,
                                          MIL_ID ResultId,
                                          long   MeasurementList);

MFTYPE32 void   MFTYPE MmeasGetResult    (MIL_ID MarkerOrResultId,
                                          long   ResultType,
                                          void   *FirstResultPtr,
                                          void   *SecondResultPtr);

MFTYPE32 void MFTYPE MmeasGetResultSingle(MIL_ID MarkerOrResultId,
                                          long   ResultType,
                                          void  *FirstResultPtr,
                                          void  *SecondResultPtr,
                                          long Index);

MFTYPE32 long   MFTYPE MmeasInquire      (MIL_ID MarkerIdOrResultIdOrContextId,
                                          long   ParamToInquire,
                                          void   *FirstValuePtr,
                                          void   *SecondValuePtr);

MFTYPE32 void   MFTYPE MmeasControl      (MIL_ID ContextId,
                                          long   ControlType,
                                          double Value);

MFTYPE32 void MFTYPE MmeasDraw           (MIL_ID GraphContId,
                                          MIL_ID MarkerOrResultId,
                                          MIL_ID DestImageId,
                                          long Operation,
                                          long Index,
                                          long ControlFlag);


#endif /* #ifdef __midl */



/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILMEAS_H__ */
