/***************************************************************************/
/*

    Filename:  MilMod.h
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines and the prototypes for the
               MIL geometric pattern matching module. (Mmod...).

    COPYRIGHT (c) 2000 Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILMOD_H__
#define __MILMOD_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif


/***************************************************************************/
/*                      MilGeo CAPI defines                                */
/***************************************************************************/

/***************************************************************************/
/* MmodAlloc()                                                             */
/***************************************************************************/

/* Object types */
#define M_RESULT                0x00001000L

#define M_GEOMETRIC             0x00002000L
#define M_GEOMETRIC_PREDEFINED  0x00004000L
#define M_GEOMETRIC_CONTROLLED  0x00008000L

/* Control flags */
#define M_MOD_DEFINE_READY      1L

/***************************************************************************/
/* MmodGetResult()                                                         */
/***************************************************************************/
/* For ResultIndex */
#define M_ALL              0x40000000L  // already defined in Mil.h

/* For Result Type */
#define M_NUMBER                 1009L  // already defined in Mil.h
#define M_GENERAL          0x20000000L
#define M_CONTEXT_ID       0x00001010L
#define M_SCORE            0x00001400L  // already defined in MilPat.h
#define M_TARGET_COVERAGE  0x00001500L
#define M_MODEL_COVERAGE   0x00001600L
#define M_FIT_ERROR        0x00001700L
#define M_SCORE_TARGET     0x00001800L
#define M_POSITION_X       0x00003400L  // already defined in MilPat.h
#define M_POSITION_Y       0x00004400L  // already defined in MilPat.h
#define M_ANGLE            0x00000800L  // already defined in Mil.h
#define M_SCALE            0x00008010L  // already defined in Milpat.h
#define M_A_FORWARD        0x00008001L
#define M_B_FORWARD        0x00008002L
#define M_C_FORWARD        0x00008003L
#define M_D_FORWARD        0x00008004L
#define M_E_FORWARD        0x00008005L
#define M_F_FORWARD        0x00008006L
#define M_A_REVERSE        0x00008011L
#define M_B_REVERSE        0x00008012L
#define M_C_REVERSE        0x00008013L
#define M_D_REVERSE        0x00008014L
#define M_E_REVERSE        0x00008015L
#define M_F_REVERSE        0x00008016L
#define M_RADIUS          M_MAJOR_AXIS 
#define M_MAJOR_AXIS              215L 
#define M_MINOR_AXIS       0x0000000AL 
#define M_ARC_PORTION             225L
#define M_ELASTICITY              109L
#define M_TARGET_EDGE_SIZE        222L
#define M_TARGET_EDGES_X          223L
#define M_TARGET_EDGES_Y          224L

#define M_AVAILABLE        0x40000000L  // already defined in MilEdge.h

#define M_TYPE_LONG        0x00040000L  // already defined in Mil.h
#define M_TYPE_DOUBLE      0x00100000L  // already defined in Mil.h

/***************************************************************************/
/* MmodDefine()                                                            */
/***************************************************************************/

#define M_DELETE                    3L   // 3   Already defined in Milblob.h

#define M_IMAGE            0x00000004L   // 4   Already defined in  Mil.h       
#define M_CIRCLE           0x00000008L   // 8   Already defined in  MilBga.h        
#define M_ELLIPSE          0x00000010L   // 16
#define M_SQUARE           0x00000020L   // 32
#define M_RECTANGLE        0x00000040L   // 64
#define M_LINE             0x00000080L   // 128
#define M_RING             0x00000100L   // 256
#define M_DXF_FILE         0x00000200L   // 512
#define M_CROSS            0x00002000L   // 8192 Already defined in  MilBga.h
#define M_DIAMOND          0x00008000L   //      32768
#define M_TRIANGLE         0x00010000L   //      65536
#define M_MERGE_MODEL      0x00020000L

#define M_CLONE            0x10000000L   

/***************************************************************************/
/* MmodDefineFromFile()                                                    */
/***************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY           (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#else
#define M_MEMORY                             5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_INTERACTIVE           M_NULL   // Already defined in mil.h, milcal.h, milcode.h, miledge.h, milmeas.h, milocr.h, milpat.h
#define M_DXF_FILE         0x00000200L   // 512


/***************************************************************************/
/* MmodControl() / MmodInquire()                                           */
/***************************************************************************/
  
// For Index 
#define M_DEFAULT                      0x10000000L  // already defined in Mil.h                     
#define M_CONTEXT                      0x08000000L
#define M_ALL                          0x40000000L  // already defined in Mil.h

// Modifier 
#define M_SUPPORTED                    0x20000000L

// Context controls/inquires 
#define M_IMAGE_ID                     M_BUF_ID
#define M_EDGE_RESULT_ID               (M_BUF_ID|0x1L)
#define M_NUMBER                       1009L  // already defined in Mil.h
#define M_REFERENCE_X                  100L
#define M_REFERENCE_Y                  101L
#define M_REFERENCE_ANGLE              110L
#define M_SEARCH_OFFSET_X              102L
#define M_SEARCH_OFFSET_Y              103L
#define M_SEARCH_SIZE_X                104L
#define M_SEARCH_SIZE_Y                105L
#define M_ACCURACY                     106L
#define M_SPEED                        M_SPEED_FACTOR  // already defined in MilPat.h (8L)
#define M_SMOOTHNESS                   108L   // TBM
#define M_FILTER_WIDTH                 109L
#define M_FILTER_MODE                  122L  // allready defined in miledge.h
#define M_DETAIL_LEVEL                 111L
#define M_SEARCH_ANGLE_RANGE           113L
#define M_SEARCH_SCALE_RANGE           114L
#define M_SEARCH_POSITION_RANGE        119L
#define M_SEARCH_ASPECT_RATIO_RANGE    117L
#define M_ASPECT_RATIO_MODE            118L
#define M_TIMEOUT                     2077L    // already define in mil.h
#define M_STOP_FIND                    115L
#define M_LINE_EXTRACTOR               116L
#define M_SHARED_EDGES                 220L
#define M_TARGET_CACHING                39L    // Already define in milpat.h
#define M_SAVE_TARGET_EDGES            121L    // Already define in milpat.h
#define M_OVERSCAN                      53L    // Already define in mil.h
#define M_OVERSCAN_REPLACE_VALUE        54L    // Already define in mil.h



// Context controls/inquires for M_GEOMETRIC_CONTROLLED
#define M_FIRST_LEVEL                   31L //Already defined in milpat.h
#define M_LAST_LEVEL                    32L //Already defined in milpat.h
#define M_AUTO                         444L //Already defined in milocr.h

// Context controls/inquires for circle finder
#define M_ARC_PORTION_MIN              107L
#define M_ELASTICITY                   109L
#define M_MODEL_BASED                  112L


// Model controls/inquires
#define M_ACCEPTANCE                         200L
#define M_ACCEPTANCE_TARGET                  201L
#define M_CERTAINTY                          202L           
#define M_CERTAINTY_TARGET                   203L
#define M_SCORE_UNCERTAINTY_FACTOR           204L
#define M_SCORE_TARGET_UNCERTAINTY_FACTOR    205L
#define M_POLARITY                           0x00004000L  // already defined in MilMeas.h 
#define M_POLARITY_CHECK                     M_POLARITY  
#define M_MIN_SEPARATION_X                   M_MIN_SPACING_X  // already defined in MilPat.h 
#define M_MIN_SEPARATION_Y                   M_MIN_SPACING_Y  // already defined in MilPat.h 
#define M_SCALE                              0x00008010L      // already defined in MilPat.h 
#define M_SCALE_MIN_FACTOR                   210L
#define M_SCALE_MAX_FACTOR                   211L
#define M_ASPECT_RATIO                       6001L // (Already defined in milcal.h) 
#define M_ASPECT_RATIO_MIN_FACTOR            230L 
#define M_ASPECT_RATIO_MAX_FACTOR            231L 

#define M_ANGLE                              0x00000800L  // already defined in Mil.h
#define M_ANGLE_DELTA_NEG                    0x00000200L  // Already define in MilPat.h 
#define M_ANGLE_DELTA_POS                    0x00000400L  // Already define in MilPat.h 
#define M_INDEX                              217L
#define M_DEFORMATION_FACTOR                 218L
#define M_FIT_ERROR_WEIGHTING_FACTOR         221L
#define M_CLONE_CONTROLS                     226L 
#define M_USER_LABEL                         228L
#define M_INDEX_FROM_LABEL                   229L
#define M_NUMBER_OF_CHAINED_PIXELS           56L    // Already define in MilBlob.h
#define M_NUMBER_OF_CHAINED_EDGELS           M_NUMBER_OF_CHAINED_PIXELS
#define M_CHAIN_X                            65L    // Already define in MilBlob.h
#define M_CHAIN_Y                            144L   // Already define in MilBlob.h
#define M_CHAIN_ANGLE                        258L   // Already define in MilEdge.h
#define M_CHAIN_INDEX                        67L    // Already define in MilBlob.h

#define M_MIN_SEPARATION_SCALE               68L
#define M_MIN_SEPARATION_ANGLE               69L
#define M_ASSOCIATED_CALIBRATION             125L   // Already define in MilCal.h
#define M_CALIBRATED                         0x0000300L

//Controlls/inquires for Synthetic models

#define M_BOX_MARGIN_LEFT                    308L
#define M_BOX_MARGIN_RIGHT                   309L
#define M_BOX_MARGIN_TOP                     310L
#define M_BOX_MARGIN_BOTTOM                  311L
#define M_PIXEL_SCALE                        316L
#define M_CAD_Y_AXIS                         318L

// CAD Y Axis control values
#define M_FLIP                        0x00010000L   // Already defined in Mil.h
#define M_NO_FLIP                     0x04000000L   // Already defined in Mil.h

             
// Types of polarity check
#define M_SAME                               -1L   // Already defined in MilMeas.h
#define M_REVERSE                             4L   // Already defined in Mil.h
#define M_SAME_OR_REVERSE                     5L
#define M_ANY                        0x11000000L   // Already defined in MilMeas.h
#define M_MIX                              M_ANY
#define M_UNDEFINED                           0L

// To remove user label
#define M_NO_LABEL                   0x20000000L

// Control values
#define M_VERY_LOW                             0L  // Already define in milpat.h
#define M_LOW                                  1L  // Already define in milpat.h
#define M_MEDIUM                               2L  // Already define in milpat.h
#define M_HIGH                                 3L  // Already define in milpat.h
#define M_VERY_HIGH                            4L  // Already define in milpat.h

#define M_GRADIENT_TYPE                        1L
#define M_BLOB_TYPE                            2L

#define M_MAX_POWER_LOSS                  0x1200L
#define M_SIZE                             0xC00L    // already define in medge.h
#define M_RECURSIVE                    0x00040000    // Already defined in mildev.h
#ifdef M_MIL_75_COMPATIBLE
#define M_KERNEL                      0x00000200L    // Already defined in    milmod.h
#else
#define M_KERNEL                      0x00400000L    // Already defined in mil.h
#endif //M_MIL_75_COMPATIBLE



/***************************************************************************/
/* MmodInquire()                                                           */
/***************************************************************************/
#define M_VALID                        0x00000001L  // Already define in Mil.h
#define M_OBJECT_TYPE                  0x80000000L  // Already define in Mil.h
#define M_PREPROCESSED                 14L          // Already define in MilPat.h
#define M_KERNEL_SIZE                  301L
#define M_NUMBER_MODELS                302L
#define M_MODIFICATION_COUNT           5010L        // Already define in Mil.h
#define M_OWNER_SYSTEM                 1001L        // already define in mil.h
#define M_SIZE_BYTE                    5061L        // already defined in Mil.h
#define M_ALLOC_OFFSET_X               15L          // Already define in MilPat.h
#define M_ALLOC_OFFSET_Y               16L          // Already define in MilPat.h
#define M_ALLOC_SIZE_X                 2L           // Already define in MilPat.h
#define M_ALLOC_SIZE_Y                 3L           // Already define in MilPat.h
#define M_ORIGINAL_X                   6L           // Already define in MilPat.h
#define M_ORIGINAL_Y                   7L           // Already define in MilPat.h
#define M_NUMBER_OF_WARNING            305L
#define M_WARNINGS                     306L
#define M_STRING_OF_WARNING            307L
#define M_ALLOC_SIZE_BAND              321L

// Model inquires for Misc Synthetic Model 
#define M_AXIS_RATIO_MIN                     212L
#define M_AXIS_RATIO_MAX                     213L
#define M_MAJOR_AXIS_MIN                     214L
#define M_MAJOR_AXIS                         215L         // Alredy defided for MmodGetResult
#define M_MAJOR_AXIS_MAX                     216L
#define M_RADIUS                             M_MAJOR_AXIS
#define M_RADIUS_MIN                         M_MAJOR_AXIS_MIN
#define M_RADIUS_MAX                         M_MAJOR_AXIS_MAX
#define M_FOREGROUND_VALUE                   4L       // Already defined in MilBlob.h         
#define M_FOREGROUND_WHITE                   0x80L    // Already define in MilBlob.h 
#define M_FOREGROUND_BLACK                   0x100L   // Already define in MilBlob.h 
#define M_FOREGROUND_ANY                     M_FOREGROUND_WHITE+M_FOREGROUND_BLACK

#define M_LENGTH                             0x00002000L // already defined in Mil.h
#define M_WIDTH                              0x00010000L // Already defined in Milmeas.h
#define M_HEIGHT                             317L
#define M_THICKNESS                          10L  // already defined in Mil.h
#define M_HORIZONTAL_THICKNESS               233L
#define M_VERTICAL_THICKNESS                 232L
#define M_INNER_RADIUS                       234L
#define M_OUTER_RADIUS                       235L
                                             
#define M_BOX_OFFSET_X                       312L
#define M_BOX_OFFSET_Y                       313L
#define M_BOX_SIZE_X                         314L
#define M_BOX_SIZE_Y                         315L


/***************************************************************************/
/* MmodDraw()                                                              */
/***************************************************************************/

#define M_DRAW_IMAGE                    2L   // Already define in MilPat.h
#define M_DRAW_EDGES                    4L
#define M_DRAW_DONT_CARES               8L   // Already define in MilPat.h
#define M_DRAW_FLAT_REGIONS            16L
#define M_DRAW_BOX                     32L   // Already define in MilPat.h
#define M_DRAW_POSITION                64L   // Already define in MilPat.h
#define M_DRAW_GRADIENT               128L
#define M_DRAW_WEIGHT_REGIONS         256L
#define M_DRAW_AXIS                 0x200L   // Already define in MilBlob.h

#define M_ORIGINAL                    409L   // Already define in MilPat.h
#define M_MODEL                   0X10000L
#define M_TARGET                  0X80000L

#define M_DRAW_RELATIVE_ORIGIN_X             319L     // Already define in MilEdge.h
#define M_DRAW_RELATIVE_ORIGIN_Y             320L     // Already define in MilEdge.h
#define M_DRAW_SCALE_X                       3203L    // Already define in MilEdge.h
#define M_DRAW_SCALE_Y                       3204L    // Already define in MilEdge.h

/***************************************************************************/
/* MmodMask()                                                              */
/***************************************************************************/

#define M_DONT_CARES            M_DONT_CARE // Already defined in mil.h
#define M_FLAT_REGIONS                 501L
#define M_WEIGHT_REGIONS               502L


/***************************************************************************/
/* MmodPreprocess()                                                        */
/***************************************************************************/
#define M_RESET                          9L
#define M_COMPLETE              0x00000000L // Already defined in Mil.h
#define M_PARTIAL               0x00000001L // Already defined in Mil.h



/***************************************************************************/
/* MmodSave()/MmodRestore()                                                */
/***************************************************************************/
#ifndef M_MIL_75_COMPATIBLE
#define M_MEMORY                         (M_APP_INQUIRE_SYS_START+120L) /*5120*/  // Already defined in Mil.h
#else
#define M_MEMORY                             5L  // Already defined in Mil.h
#endif //M_MIL_75_COMPATIBLE
#define M_INTERACTIVE                    M_NULL // Already defined in    mil.h, milcal.h, milcode.h, milmeas.h, milocr.h, milpat.h, milmod.h


/***************************************************************************/
/*                 WARNINGS definitions and string table                   */
/***************************************************************************/
#define M_MAX_WARNING_STRING_LENGTH     64L
#define M_LAST_WARNING_VALUE            12L

/* Warning definitions */
#define M_POOR_GEOMETRIC_MODEL           1L
#define M_HIGH_GEOMETRIC_COMPLEXITY      2L
#define M_UNSTABLE_ANGLE                 3L
#define M_UNSTABLE_SCALE                 4L
#define M_UNSTABLE_POSITION              5L
#define M_BETTER_OUT_OF_ANGLE_RANGE      6L
#define M_BETTER_OUT_OF_SCALE_RANGE      7L
#define M_BETTER_OUT_OF_SEARCH_REGION    8L
#define M_POOR_GEOMETRIC_TARGET          9L
#define M_TIMEOUT_END                   10L
#define M_SEARCH_OUTSIDE_TARGET         11L
#define M_NO_INITIAL_CALIBRATION        12L


/***************************************************************************/
/*                 MilGeo CAPI function prototypes                         */
/***************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE MmodAlloc         (MIL_ID   SystemId, 
                                          long     MethodType, 
                                          long     ControlFlag,
                                          MIL_ID*  ObjectIdPtr);

MFTYPE32 MIL_ID MFTYPE MmodAllocResult   (MIL_ID   SystemId,
                                          long     ControlFlag, 
                                          MIL_ID*  ResultIdPtr);

MFTYPE32 void MFTYPE MmodDefine          (MIL_ID   ContextId,
                                          long     ModelType, 
                                          double   Param1,
                                          double   Param2,
                                          double   Param3,
                                          double   Param4,
                                          double   Param5);

MFTYPE32 void MFTYPE MmodDefineFromFile  (MIL_ID   ContextId,
                                          long     FileType,
                                          LPMILSTR FileName,
                                          long     ControlFlag );

MFTYPE32 void MFTYPE MmodControl         (MIL_ID   ContextId, 
                                          long     Index,
                                          long     ControlType, 
                                          double   ControlValue);

MFTYPE32 long MFTYPE MmodInquire         (MIL_ID   ContextId, 
                                          long     Index,
                                          long     InquireType,
                                          void*    UserVarPtr);

MFTYPE32 void MFTYPE MmodMask            (MIL_ID   ContextId,
                                          long     Index,
                                          MIL_ID   MaskBufferId,
                                          long     MaskType,
                                          long     ControlFlag);

MFTYPE32 void MFTYPE MmodPreprocess      (MIL_ID   ContextId, 
                                          long     ControlFlag);

MFTYPE32 void MFTYPE MmodFind            (MIL_ID   ContextId, 
                                          MIL_ID   TargetImageId,
                                          MIL_ID   ResultId);

MFTYPE32 void MFTYPE MmodGetResult       ( MIL_ID   ResultId, 
                                           long     ResultIndex,
                                           long     ResultType,
                                           void    *ResultArrayPtr );

MFTYPE32 void MFTYPE MmodDraw            (MIL_ID GraphContId,
                                          MIL_ID ContextOrResultId,
                                          MIL_ID DestImageBufId, 
                                          long   Operation,
                                          long   Index,
                                          long   ControlFlag);

MFTYPE32 void MFTYPE MmodSave            (MIL_TEXT_PTR FileName,
                                          MIL_ID   ContextId,
                                          long     ControlFlag);

MFTYPE32 MIL_ID  MFTYPE MmodRestore      (MIL_TEXT_PTR FileName,
                                          MIL_ID   SystemId, 
                                          long     ControlFlag,
                                          MIL_ID*  ContextIdPtr);

MFTYPE32 void MFTYPE MmodFree            (MIL_ID ObjectId);

#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILMOD_H__ */
