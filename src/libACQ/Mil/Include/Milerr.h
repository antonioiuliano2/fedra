/***************************************************************************/
/*

    Filename:  MILERR.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines that are used by the
               mil functions to generate error codes and messages that
               will be used by the MIL Application Error Logging and
               other error related functions.

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef  __MILERR_H
#define  __MILERR_H

/*
First line is the error code.
Second line is the error message for this code.
Other 9 lines are optional sub-messages with more detailed information.

------------------------------------------------------------------------
-                                                                      -
-  WARNING: Strings (including the NULL character at the end) should   -
-           never be longer than M_ERROR_MESSAGE_SIZE (128L).          -
-           No checking is done anywhere in the code to ensure this.   -
-                                                                      -
------------------------------------------------------------------------
*/


#define NO_MSG             MIL_TEXT("")  /* empty error message          */
#define NO_SUBMSG          MIL_TEXT("")  /* empty sub-error message      */
#define NO_FCTNAME         MIL_TEXT("")  /* empty function name          */

// ActiveMIL
// ychenard - 15/08/2000
// Please use the MILOCX_COMPILATION to remove any function definition from this file
// when not compiling ActiveMIL.
// This is a hack from us to include the low-level error messages from MIL.
// If MIL cleans up the error handling some day, please contact the ActiveMIL team.
// 
//   NOTE: if you add anything else than defines in this file, please put them 
//         within a "#ifndef MILOCX_COMPILATION" block.
#ifndef MILERR
   #define MILERR(Fake1, Fake2, Fake3, Fake4, Fake5)
#endif

/* LIST OF ERROR CODES AND MESSAGES */

#define M_NULL_ERROR             0L
#define M_ERR_0_MSG              MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_1         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_2         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_3         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_4         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_5         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_6         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_7         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_8         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_9         MIL_TEXT("Null.")

#define M_SIMD_ERROR_1           1L
#define M_ERR_1_MSG              MIL_TEXT("SIMD Error.")
#define M_ERR_1_SUBMSG_1         MIL_TEXT("Cannot force MMX usage on a non-MMX CPU.")
#define M_ERR_1_SUBMSG_2         MIL_TEXT("Cannot force SSE usage on a non-SSE CPU or OS doesn't support SSE.")
#define M_ERR_1_SUBMSG_3         MIL_TEXT("Cannot force SSE2 usage on a non-SSE2 CPU or OS doesn't support SSE2.")
#define M_ERR_1_SUBMSG_4         MIL_TEXT("Cannot force 3DNow usage on a non-3DNow CPU.")
#define M_ERR_1_SUBMSG_5         MIL_TEXT("Cannot force MMX Extension usage on a non-MMX Extenion CPU.")
#define M_ERR_1_SUBMSG_6         MIL_TEXT("Cannot force 3DNow Extension usage on a non-3DNow Extenion CPU.")
#define M_ERR_1_SUBMSG_7         NO_SUBMSG
#define M_ERR_1_SUBMSG_8         NO_SUBMSG
#define M_ERR_1_SUBMSG_9         NO_SUBMSG

#define M_SIMD_ERROR_2           2L
#define M_ERR_2_MSG              MIL_TEXT("SIMD Error.")
#define M_ERR_2_SUBMSG_1         MIL_TEXT("Cannot force Altivec usage on a non-Altivec CPU.")
#define M_ERR_2_SUBMSG_2         NO_SUBMSG
#define M_ERR_2_SUBMSG_3         NO_SUBMSG
#define M_ERR_2_SUBMSG_4         NO_SUBMSG
#define M_ERR_2_SUBMSG_5         NO_SUBMSG
#define M_ERR_2_SUBMSG_6         NO_SUBMSG
#define M_ERR_2_SUBMSG_7         NO_SUBMSG
#define M_ERR_2_SUBMSG_8         NO_SUBMSG
#define M_ERR_2_SUBMSG_9         NO_SUBMSG

#define M_FUNC_FCT_ERROR            3L
#define M_ERR_3_MSG              MIL_TEXT("Function call error")
#define M_ERR_3_SUBMSG_1         MIL_TEXT("MappGetError cannot be called from inside a MIL function. Use MfuncGetError instead.")
#define M_ERR_3_SUBMSG_2         NO_SUBMSG
#define M_ERR_3_SUBMSG_3         NO_SUBMSG
#define M_ERR_3_SUBMSG_4         NO_SUBMSG
#define M_ERR_3_SUBMSG_5         NO_SUBMSG
#define M_ERR_3_SUBMSG_6         NO_SUBMSG
#define M_ERR_3_SUBMSG_7         NO_SUBMSG
#define M_ERR_3_SUBMSG_8         NO_SUBMSG
#define M_ERR_3_SUBMSG_9         NO_SUBMSG

#define M_XX4XX_ERROR            4L
#define M_ERR_4_MSG              NO_SUBMSG
#define M_ERR_4_SUBMSG_1         NO_SUBMSG
#define M_ERR_4_SUBMSG_2         NO_SUBMSG
#define M_ERR_4_SUBMSG_3         NO_SUBMSG
#define M_ERR_4_SUBMSG_4         NO_SUBMSG
#define M_ERR_4_SUBMSG_5         NO_SUBMSG
#define M_ERR_4_SUBMSG_6         NO_SUBMSG
#define M_ERR_4_SUBMSG_7         NO_SUBMSG
#define M_ERR_4_SUBMSG_8         NO_SUBMSG
#define M_ERR_4_SUBMSG_9         NO_SUBMSG

#define M_XX5XX_ERROR            5L
#define M_ERR_5_MSG              NO_SUBMSG
#define M_ERR_5_SUBMSG_1         NO_SUBMSG
#define M_ERR_5_SUBMSG_2         NO_SUBMSG
#define M_ERR_5_SUBMSG_3         NO_SUBMSG
#define M_ERR_5_SUBMSG_4         NO_SUBMSG
#define M_ERR_5_SUBMSG_5         NO_SUBMSG
#define M_ERR_5_SUBMSG_6         NO_SUBMSG
#define M_ERR_5_SUBMSG_7         NO_SUBMSG
#define M_ERR_5_SUBMSG_8         NO_SUBMSG
#define M_ERR_5_SUBMSG_9         NO_SUBMSG

#define M_INVALID_PARAM_ERROR    6L
#define M_ERR_6_MSG              MIL_TEXT("Invalid parameter.")
#define M_ERR_6_SUBMSG_1         MIL_TEXT("Bad parameter value.")
#define M_ERR_6_SUBMSG_2         MIL_TEXT("One parameter does not reside within the buffer's limits.")
#define M_ERR_6_SUBMSG_3         MIL_TEXT("The pointer should not be null.")
/* WARNING: Obsolete sub errors (4,5); if you want to use these, notify ActiveMIL team */
#define M_ERR_6_SUBMSG_4         MIL_TEXT("Parameter 1 not in supported list.")
#define M_ERR_6_SUBMSG_5         MIL_TEXT("Parameter 2 not in supported list.")
#define M_ERR_6_SUBMSG_6         MIL_TEXT("No graphic text font selected.")
#define M_ERR_6_SUBMSG_7         MIL_TEXT("The member StructSize from the MILBUFFERINFO structure given as an argument is invalid.")
#define M_ERR_6_SUBMSG_8         MIL_TEXT("Result buffer too small to hold result.")
#define M_ERR_6_SUBMSG_9         MIL_TEXT("Scale factors out of supported range.")

#define M_OVERSCAN_ERROR         7L
#define M_ERR_7_MSG              MIL_TEXT("Overscan processing error.")
#define M_ERR_7_SUBMSG_1         MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_7_SUBMSG_2         MIL_TEXT("Buffer too small to perform the selected overscan")
#define M_ERR_7_SUBMSG_3         NO_SUBMSG
#define M_ERR_7_SUBMSG_4         NO_SUBMSG
#define M_ERR_7_SUBMSG_5         NO_SUBMSG
#define M_ERR_7_SUBMSG_6         NO_SUBMSG
#define M_ERR_7_SUBMSG_7         NO_SUBMSG
#define M_ERR_7_SUBMSG_8         NO_SUBMSG
#define M_ERR_7_SUBMSG_9         NO_SUBMSG

#define M_ALLOC_ERROR            8L
#define M_ERR_8_MSG              MIL_TEXT("Allocation error.")
#define M_ERR_8_SUBMSG_1         MIL_TEXT("Not enough memory to allocate application.")
#define M_ERR_8_SUBMSG_2         MIL_TEXT("Only one application can be allocated by host thread.")
#define M_ERR_8_SUBMSG_3         MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_8_SUBMSG_4         MIL_TEXT("Not enough memory to allocate buffer.")
#define M_ERR_8_SUBMSG_5         MIL_TEXT("Cannot allocate system.")
#define M_ERR_8_SUBMSG_6         MIL_TEXT("Cannot allocate digitizer.")
#define M_ERR_8_SUBMSG_7         MIL_TEXT("Cannot allocate display.")
#define M_ERR_8_SUBMSG_8         MIL_TEXT("Not enough host memory to allocate buffer.")
#define M_ERR_8_SUBMSG_9         MIL_TEXT("Buffer type not supported.")

#define M_CHILD_ERROR            9L
#define M_ERR_9_MSG              MIL_TEXT("Child allocation error.")
#define M_ERR_9_SUBMSG_1         MIL_TEXT("Only one application can be allocated by host thread.")
#define M_ERR_9_SUBMSG_2         MIL_TEXT("Not enough memory to allocate child application.")
#define M_ERR_9_SUBMSG_3         MIL_TEXT("Not enough memory to allocate child buffer.")
#define M_ERR_9_SUBMSG_4         MIL_TEXT("Cannot allocate temporary child buffer in memory.")
#define M_ERR_9_SUBMSG_5         MIL_TEXT("Impossible to make a band child of a compressed buffer.")
#define M_ERR_9_SUBMSG_6         NO_SUBMSG
#define M_ERR_9_SUBMSG_7         NO_SUBMSG
#define M_ERR_9_SUBMSG_8         NO_SUBMSG
#define M_ERR_9_SUBMSG_9         NO_SUBMSG

#define M_ACCESS_ERROR           10L
#define M_ERR_10_MSG             MIL_TEXT("Buffer access error.")
#define M_ERR_10_SUBMSG_1        MIL_TEXT("Cannot M_RESTORE a M_RAW file format buffer.")
MILERR( M_ERR,10,SUBMSG,1,       MIL_TEXT("Cannot load a raw file format buffer with the AdjustImage parameter set to True."))
#define M_ERR_10_SUBMSG_2        MIL_TEXT("Cannot export buffer.")
#define M_ERR_10_SUBMSG_3        MIL_TEXT("Source buffer must be a M_IMAGE buffer.")
MILERR( M_ERR,10,SUBMSG,3,       MIL_TEXT("Source image invalid."))
#define M_ERR_10_SUBMSG_4        MIL_TEXT("Cannot import buffer.")
MILERR( M_ERR,10,SUBMSG,4,       MIL_TEXT("Cannot load image."))
#define M_ERR_10_SUBMSG_5        MIL_TEXT("Cannot restore buffer.")
MILERR( M_ERR,10,SUBMSG,5,       MIL_TEXT("Cannot load image."))
#define M_ERR_10_SUBMSG_6        MIL_TEXT("Cannot export child buffer in M_PLANAR format")
MILERR( M_ERR,10,SUBMSG,6,       MIL_TEXT("Cannot save child image in planar format"))
#define M_ERR_10_SUBMSG_7        NO_SUBMSG
#define M_ERR_10_SUBMSG_8        NO_SUBMSG
#define M_ERR_10_SUBMSG_9        NO_SUBMSG

#define M_DISPLAY_ERROR          11L
#define M_ERR_11_MSG             MIL_TEXT("Display error.")
#define M_ERR_11_SUBMSG_1        NO_SUBMSG
#define M_ERR_11_SUBMSG_2        MIL_TEXT("Display Lut dimensions are not compatible with the user Lut.")
#define M_ERR_11_SUBMSG_3        MIL_TEXT("Cannot associate a M_PSEUDO Lut with a monochrome display.")
MILERR( M_ERR,11,SUBMSG,3,       MIL_TEXT("Cannot associate a pseudo LUT with a monochrome display."))
#define M_ERR_11_SUBMSG_4        MIL_TEXT("Zoom factors out of zoom limits or equal to zero.")
#define M_ERR_11_SUBMSG_5        MIL_TEXT("Buffer not currently selected on display.")
MILERR( M_ERR,11,SUBMSG,5,       MIL_TEXT("Buffer not currently associated with a display."))
#define M_ERR_11_SUBMSG_6        MIL_TEXT("Incompatible display type.")
#define M_ERR_11_SUBMSG_7        MIL_TEXT("Display must be allocated with the M_WINDOWED init flag.")
MILERR( M_ERR,11,SUBMSG,7,       MIL_TEXT("The DisplayType must be set to dispDefaultWindow, dispUserWindow, or dispExternalWindow."))
#define M_ERR_11_SUBMSG_8        MIL_TEXT("Invalid window handle.")
#define M_ERR_11_SUBMSG_9        MIL_TEXT("Cannot allocate compensation buffer. Display and buffer should belong to the same system")

#define M_OPERATION_ERROR        12L
#define M_ERR_12_MSG             MIL_TEXT("Operation error.")
#define M_ERR_12_SUBMSG_1        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_12_SUBMSG_2        MIL_TEXT("Not enough host memory to allocate buffer.")
#define M_ERR_12_SUBMSG_3        MIL_TEXT("Application still has MIL objects associated with it.")
#define M_ERR_12_SUBMSG_4        MIL_TEXT("Only logical addresses are supported on host system.")
#define M_ERR_12_SUBMSG_5        MIL_TEXT("Pitch must be a multiple of 4 bytes for binary buffers.")
#define M_ERR_12_SUBMSG_6        MIL_TEXT("Requested operation not supported.") 
#define M_ERR_12_SUBMSG_7        MIL_TEXT("Pitch must be a multiple of 2 pixels.")
#define M_ERR_12_SUBMSG_8        MIL_TEXT("Creation with physical address must be done on non paged buffers.")
#define M_ERR_12_SUBMSG_9        MIL_TEXT("The polar band to put must fit in the destination.")

#define M_DIGITIZER_ERROR        13L
#define M_ERR_13_MSG             MIL_TEXT("Digitizer error.")
#define M_ERR_13_SUBMSG_1        MIL_TEXT("Digitizer and buffer must belong to same system.")
#define M_ERR_13_SUBMSG_2        MIL_TEXT("Can not free digitizer. Continuous grab in progress.")
#define M_ERR_13_SUBMSG_3        MIL_TEXT("Can not grab. Digitizer already used for continuous grab")
#define M_ERR_13_SUBMSG_4        NO_SUBMSG
#define M_ERR_13_SUBMSG_5        NO_SUBMSG
#define M_ERR_13_SUBMSG_6        NO_SUBMSG
#define M_ERR_13_SUBMSG_7        NO_SUBMSG
#define M_ERR_13_SUBMSG_8        NO_SUBMSG
#define M_ERR_13_SUBMSG_9        NO_SUBMSG

#define M_HOOK_ERROR             14L
#define M_ERR_14_MSG             MIL_TEXT("Hook function error.")
#define M_ERR_14_SUBMSG_1        MIL_TEXT("Function and/or user data not found.")
#define M_ERR_14_SUBMSG_2        MIL_TEXT("A hook function must be provided. If you are trying to unhook, add M_UNHOOK to the hook type.")
#define M_ERR_14_SUBMSG_3        MIL_TEXT("Object not hooked to event.")
#define M_ERR_14_SUBMSG_4        MIL_TEXT("Invalid hook type.")
#define M_ERR_14_SUBMSG_5        MIL_TEXT("Required hardware not present.")
#define M_ERR_14_SUBMSG_6        NO_SUBMSG
#define M_ERR_14_SUBMSG_7        NO_SUBMSG
#define M_ERR_14_SUBMSG_8        MIL_TEXT("A C++ exception occured in user hook function.")
#define M_ERR_14_SUBMSG_9        NO_SUBMSG

#define M_JPEG_COMPRESS_ERROR    15L
#define M_ERR_15_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_15_SUBMSG_1        MIL_TEXT("Unable to allocate memory.")
#define M_ERR_15_SUBMSG_2        MIL_TEXT("Invalid image depth for JPEG compression.")
#define M_ERR_15_SUBMSG_3        MIL_TEXT("Invalid compression parameter.")
#define M_ERR_15_SUBMSG_4        MIL_TEXT("Invalid Huffman table")
#define M_ERR_15_SUBMSG_5        MIL_TEXT("Invalid predictor.")
#define M_ERR_15_SUBMSG_6        MIL_TEXT("Invalid Q Factor.")
#define M_ERR_15_SUBMSG_7        MIL_TEXT("Invalid quantization table.")
#define M_ERR_15_SUBMSG_8        MIL_TEXT("Invalid restart interval.")
#define M_ERR_15_SUBMSG_9        MIL_TEXT("Invalid source or destination format.")

#define M_COMPRESS_ERROR         16L
#define M_ERR_16_MSG             MIL_TEXT("Compression error")
#define M_ERR_16_SUBMSG_1        MIL_TEXT("Invalid compression type.")
#define M_ERR_16_SUBMSG_2        MIL_TEXT("Corrupted data.")
#define M_ERR_16_SUBMSG_3        MIL_TEXT("Invalid image depth for required compression type.")
#define M_ERR_16_SUBMSG_4        MIL_TEXT("Buffer allocated with MbufCreate cannot be the destination of a compression operation.")
MILERR( M_ERR,16,SUBMSG,4,       MIL_TEXT("Image allocated with AssignMemory cannot be the destination of a compression operation."))
#define M_ERR_16_SUBMSG_5        MIL_TEXT("Buffer allocated with MbufCreate cannot be the destination of a MbufPut operation.")
MILERR( M_ERR,16,SUBMSG,5,       MIL_TEXT("Image allocated with AssignMemory cannot be the destination of a Put method."))
#define M_ERR_16_SUBMSG_6        MIL_TEXT("Buffer too small to contain the compressed data.")
#define M_ERR_16_SUBMSG_7        MIL_TEXT("Cannot modify the pointer of a buffer allocated with MbufCreate.")
#define M_ERR_16_SUBMSG_8        NO_SUBMSG
#define M_ERR_16_SUBMSG_9        NO_SUBMSG

#define M_JPEG_COMPRESS_ERROR_2  17L
#define M_ERR_17_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_17_SUBMSG_1        MIL_TEXT("Chrominance tables are allowed only with 3 band YUV buffers.")
#define M_ERR_17_SUBMSG_2        MIL_TEXT("Luminance tables are allowed only with 3 band YUV buffers.")
#define M_ERR_17_SUBMSG_3        MIL_TEXT("The predictor value is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_4        MIL_TEXT("The Q factor is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_5        MIL_TEXT("The restart interval is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_6        MIL_TEXT("The source image must be a multiple of 16 in X and a multiple of 8 in Y.")
#define M_ERR_17_SUBMSG_7        MIL_TEXT("The destination image must be a multiple of 16 in X and a multiple of 8 in Y.")
#define M_ERR_17_SUBMSG_8        MIL_TEXT("The source image must be a multiple of 16 in X and a multiple of 16 in Y.")
#define M_ERR_17_SUBMSG_9        MIL_TEXT("The destination image must be a multiple of 16 in X and a multiple of 16 in Y.")


#define M_BMP_ERROR              18L
#define M_ERR_18_MSG             MIL_TEXT("BMP handler file access error.")
#define M_ERR_18_SUBMSG_1        MIL_TEXT("Not a bitmap file.")
#define M_ERR_18_SUBMSG_2        MIL_TEXT("Error closing bitmap file.")
#define M_ERR_18_SUBMSG_3        MIL_TEXT("Cannot open file in read mode.")
#define M_ERR_18_SUBMSG_4        MIL_TEXT("Error reading file.")
#define M_ERR_18_SUBMSG_5        MIL_TEXT("Unable to position file pointer.")
#define M_ERR_18_SUBMSG_6        MIL_TEXT("Cannot create or open file in write mode.")
#define M_ERR_18_SUBMSG_7        MIL_TEXT("No bitmap file opened in read mode.")
#define M_ERR_18_SUBMSG_8        MIL_TEXT("No bitmap file opened in write mode.")
#define M_ERR_18_SUBMSG_9        MIL_TEXT("Error writing file.")

#define M_BMP_ERROR_2            19L
#define M_ERR_19_MSG             MIL_TEXT("BMP handler general error.")
#define M_ERR_19_SUBMSG_1        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_19_SUBMSG_2        MIL_TEXT("Not supported color format.")
#define M_ERR_19_SUBMSG_3        MIL_TEXT("Invalid write color format specified.")
#define M_ERR_19_SUBMSG_4        MIL_TEXT("Invalid write compression type specified.")
#define M_ERR_19_SUBMSG_5        MIL_TEXT("Not supported type of conversion.")
#define M_ERR_19_SUBMSG_6        MIL_TEXT("Invalid array format specified.")
#define M_ERR_19_SUBMSG_7        MIL_TEXT("Invalid length of palette arrays specified.")
#define M_ERR_19_SUBMSG_8        MIL_TEXT("No palette to read.")
#define M_ERR_19_SUBMSG_9        MIL_TEXT("Palette not needed for current write format.")


#define M_TIFF_ERROR             20L
#define M_ERR_20_MSG             MIL_TEXT("TIFF file access error.")
#define M_ERR_20_SUBMSG_1        MIL_TEXT("Cannot open file.")
#define M_ERR_20_SUBMSG_2        MIL_TEXT("Cannot close file.")
#define M_ERR_20_SUBMSG_3        MIL_TEXT("Cannot read file.")
#define M_ERR_20_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_20_SUBMSG_5        MIL_TEXT("Cannot write to file.")
#define M_ERR_20_SUBMSG_6        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_20_SUBMSG_7        MIL_TEXT("The image file does not conform to the TIFF 6.0 specification.")
#define M_ERR_20_SUBMSG_8        MIL_TEXT("Wrong Byte Order, Only INTEL Byte Ordering supported.")
#define M_ERR_20_SUBMSG_9        MIL_TEXT("Not a TIFF file.")

#define M_MIL_FILE_ERROR         21L
#define M_ERR_21_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_21_SUBMSG_1        MIL_TEXT("Cannot open file.")
#define M_ERR_21_SUBMSG_2        MIL_TEXT("Cannot close file.")
#define M_ERR_21_SUBMSG_3        MIL_TEXT("Cannot read file.")
#define M_ERR_21_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_21_SUBMSG_5        MIL_TEXT("Cannot write to file.")
#define M_ERR_21_SUBMSG_6        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_21_SUBMSG_7        MIL_TEXT("The image file does not conform to the TIFF 6.0 specification.")
#define M_ERR_21_SUBMSG_8        MIL_TEXT("Wrong Byte Order, Only INTEL Byte Ordering supported.")
#define M_ERR_21_SUBMSG_9        MIL_TEXT("Not a MIL file.")

#define M_MIL_FILE_ERROR_2       22L
#define M_ERR_22_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_22_SUBMSG_1        MIL_TEXT("Only 8, 16 or 32 BitsPerSample supported.")
#define M_ERR_22_SUBMSG_2        MIL_TEXT("Only the Lempel-Ziv & Welch (LZW) compression supported.")
#define M_ERR_22_SUBMSG_3        MIL_TEXT("PhotometricInterp incompatible with SamplePerPixel.")
#define M_ERR_22_SUBMSG_4        MIL_TEXT("Only PlanarConfiguration 2 supported for multi-band images.")
#define M_ERR_22_SUBMSG_5        MIL_TEXT("Up to 8 Samples Per Pixel supported.")
#define M_ERR_22_SUBMSG_6        MIL_TEXT("Only identical BitsPerSample for every sample supported.")
#define M_ERR_22_SUBMSG_7        MIL_TEXT("Cannot seek in file.")
#define M_ERR_22_SUBMSG_8        MIL_TEXT("Bad file format detected.")
#define M_ERR_22_SUBMSG_9        MIL_TEXT("Invalid info requested.")

#define M_MIL_FILE_ERROR_3       23L
#define M_ERR_23_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_23_SUBMSG_1        MIL_TEXT("Invalid parameter detected.")
#define M_ERR_23_SUBMSG_2        MIL_TEXT("Missing information in file.")
#define M_ERR_23_SUBMSG_3        MIL_TEXT("Bad size information detected in file.")
#define M_ERR_23_SUBMSG_4        MIL_TEXT("Bad identification information detected in file.")
#define M_ERR_23_SUBMSG_5        MIL_TEXT("Invalid data information detected in file.")
#define M_ERR_23_SUBMSG_6        MIL_TEXT("Unexpected internal error.")
#define M_ERR_23_SUBMSG_7        MIL_TEXT("Cannot open file: Invalid file name.")
#define M_ERR_23_SUBMSG_8        NO_SUBMSG
#define M_ERR_23_SUBMSG_9        NO_SUBMSG

#define M_MULTI_THREAD_ERROR     24L
#define M_ERR_24_MSG             MIL_TEXT("Multi thread error.")
#define M_ERR_24_SUBMSG_1        MIL_TEXT("Cannot find synchronization event to delete.")
#define M_ERR_24_SUBMSG_2        MIL_TEXT("Invalid MIL_ID, MIL thread or event was not allocated in current HOST thread.")
MILERR( M_ERR,24,SUBMSG,2,       MIL_TEXT("Invalid thread or event ID; it was not allocated in current host thread."))
#define M_ERR_24_SUBMSG_3        MIL_TEXT("Application must be freed before exiting a thread.")
#define M_ERR_24_SUBMSG_4        MIL_TEXT("Operation not supported on this system")
#define M_ERR_24_SUBMSG_5        NO_SUBMSG
#define M_ERR_24_SUBMSG_6        NO_SUBMSG
#define M_ERR_24_SUBMSG_7        NO_SUBMSG
#define M_ERR_24_SUBMSG_8        NO_SUBMSG
#define M_ERR_24_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR             25L
#define M_ERR_25_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_25_SUBMSG_1        MIL_TEXT("Not supported color format.")
#define M_ERR_25_SUBMSG_2        MIL_TEXT("Error closing JPEG file.")
#define M_ERR_25_SUBMSG_3        MIL_TEXT("Error opening JPEG file.")
#define M_ERR_25_SUBMSG_4        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_25_SUBMSG_5        MIL_TEXT("Image too large to save.")
#define M_ERR_25_SUBMSG_6        MIL_TEXT("Invalid JPEG Marker.")
#define M_ERR_25_SUBMSG_7        MIL_TEXT("Bad identification information detected in file.")
#define M_ERR_25_SUBMSG_8        MIL_TEXT("Buffer to receive data does not fit with data.")
#define M_ERR_25_SUBMSG_9        MIL_TEXT("Error reading JPEG file.")

#define M_BUFFER_FREE_ERROR      26L
#define M_ERR_26_MSG             MIL_TEXT("Buffer free operation error.")
#define M_ERR_26_SUBMSG_1        MIL_TEXT("Buffer still has child(ren) associated with it.")
#define M_ERR_26_SUBMSG_2        MIL_TEXT("Use MnatBufDestroy() on this kind of buffer.")
#define M_ERR_26_SUBMSG_3        MIL_TEXT("User attempting to free a system allocated buffer.")
#define M_ERR_26_SUBMSG_4        MIL_TEXT("Internal error, internal function attempting to free a user allocated buffer.")
#define M_ERR_26_SUBMSG_5        MIL_TEXT("This buffer is currently used by a digitizer for a continuous grab.")
#define M_ERR_26_SUBMSG_6        NO_SUBMSG
#define M_ERR_26_SUBMSG_7        NO_SUBMSG
#define M_ERR_26_SUBMSG_8        NO_SUBMSG
#define M_ERR_26_SUBMSG_9        NO_SUBMSG

#define M_SYSTEM_FREE_ERROR      27L
#define M_ERR_27_MSG             MIL_TEXT("System free operation error.")
#define M_ERR_27_SUBMSG_1        MIL_TEXT("System still has buffer(s) associated with it.")
#define M_ERR_27_SUBMSG_2        MIL_TEXT("System still has display(s) associated with it.")
#define M_ERR_27_SUBMSG_3        MIL_TEXT("System still has digitizer(s) associated with it.")
#define M_ERR_27_SUBMSG_4        NO_SUBMSG
#define M_ERR_27_SUBMSG_5        NO_SUBMSG
#define M_ERR_27_SUBMSG_6        NO_SUBMSG
#define M_ERR_27_SUBMSG_7        NO_SUBMSG
#define M_ERR_27_SUBMSG_8        NO_SUBMSG
#define M_ERR_27_SUBMSG_9        NO_SUBMSG

#define M_FUNCTION_START_ERROR   28L
#define M_ERR_28_MSG             MIL_TEXT("Function start error.")
#define M_ERR_28_SUBMSG_1        MIL_TEXT("No application allocated.")
#define M_ERR_28_SUBMSG_2        NO_SUBMSG
#define M_ERR_28_SUBMSG_3        NO_SUBMSG
#define M_ERR_28_SUBMSG_4        NO_SUBMSG
#define M_ERR_28_SUBMSG_5        NO_SUBMSG
#define M_ERR_28_SUBMSG_6        NO_SUBMSG
#define M_ERR_28_SUBMSG_7        NO_SUBMSG
#define M_ERR_28_SUBMSG_8        NO_SUBMSG
#define M_ERR_28_SUBMSG_9        NO_SUBMSG

#define M_COMMAND_DECODER_ERROR  29L
#define M_ERR_29_MSG             MIL_TEXT("System command error.")
#define M_ERR_29_SUBMSG_1        MIL_TEXT("Requested operation not supported.")
#define M_ERR_29_SUBMSG_2        MIL_TEXT("Operation execution failed.")
#define M_ERR_29_SUBMSG_3        NO_SUBMSG
#define M_ERR_29_SUBMSG_4        NO_SUBMSG 
#define M_ERR_29_SUBMSG_5        NO_SUBMSG
#define M_ERR_29_SUBMSG_6        NO_SUBMSG
#define M_ERR_29_SUBMSG_7        NO_SUBMSG
#define M_ERR_29_SUBMSG_8        NO_SUBMSG
#define M_ERR_29_SUBMSG_9        NO_SUBMSG

#define M_LABELLING_ERROR        30L
#define M_ERR_30_MSG             MIL_TEXT("Labelling error.")
#define M_ERR_30_SUBMSG_1        MIL_TEXT("Maximum number of labels reached.")
#define M_ERR_30_SUBMSG_2        MIL_TEXT("Should use a buffer of greater bit depth.")
#define M_ERR_30_SUBMSG_3        NO_SUBMSG
#define M_ERR_30_SUBMSG_4        NO_SUBMSG
#define M_ERR_30_SUBMSG_5        NO_SUBMSG
#define M_ERR_30_SUBMSG_6        NO_SUBMSG
#define M_ERR_30_SUBMSG_7        NO_SUBMSG
#define M_ERR_30_SUBMSG_8        NO_SUBMSG
#define M_ERR_30_SUBMSG_9        NO_SUBMSG

#define M_FILE_ERROR             31L
#define M_ERR_31_MSG             MIL_TEXT("File access error.")
#define M_ERR_31_SUBMSG_1        MIL_TEXT("Cannot open output file.")
#define M_ERR_31_SUBMSG_2        MIL_TEXT("Cannot write to file.")
#define M_ERR_31_SUBMSG_3        MIL_TEXT("Cannot open input file.")
#define M_ERR_31_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_31_SUBMSG_5        MIL_TEXT("Cannot close output file.")
#define M_ERR_31_SUBMSG_6        MIL_TEXT("Cannot close input file.")
#define M_ERR_31_SUBMSG_7        MIL_TEXT("The FileFormatBufId does not represent the actual file format.")
#define M_ERR_31_SUBMSG_8        MIL_TEXT("This OS doesn't support file access.")
#define M_ERR_31_SUBMSG_9        MIL_TEXT("Not a MIL file.")

#define M_APP_FREE_ERROR         32L
#define M_ERR_32_MSG             MIL_TEXT("Application free operation error.")
#define M_ERR_32_SUBMSG_1        MIL_TEXT("Application still has system(s) associated with it.")
#define M_ERR_32_SUBMSG_2        MIL_TEXT("Default host system still has buffer(s) associated with it.")
#define M_ERR_32_SUBMSG_3        MIL_TEXT("Application still has child(ren) associated with it.")
#define M_ERR_32_SUBMSG_4        MIL_TEXT("Application was not freed.")
#define M_ERR_32_SUBMSG_5        MIL_TEXT("Application still has object(s) associated with it.")
#define M_ERR_32_SUBMSG_6        MIL_TEXT("Application must be freed in the thread in which it was allocated.")
#define M_ERR_32_SUBMSG_7        NO_SUBMSG
#define M_ERR_32_SUBMSG_8        NO_SUBMSG
#define M_ERR_32_SUBMSG_9        NO_SUBMSG

#define M_TIFF_ERROR_2           33L
#define M_ERR_33_MSG             MIL_TEXT("TIFF File access error.")
#define M_ERR_33_SUBMSG_1        MIL_TEXT("Only 1, 8, 16 or 32 BitsPerSample supported.")
#define M_ERR_33_SUBMSG_2        MIL_TEXT("Only the Lempel-Ziv & Welch (LZW) compression supported.")
#define M_ERR_33_SUBMSG_3        MIL_TEXT("PhotometricInterp incompatible with SamplePerPixel.")
#define M_ERR_33_SUBMSG_4        MIL_TEXT("Only PlanarConfiguration 2 supported for multi-band images.")
#define M_ERR_33_SUBMSG_5        MIL_TEXT("Up to 8 Samples Per Pixel supported.")
#define M_ERR_33_SUBMSG_6        MIL_TEXT("Only identical BitsPerSample for every sample supported.")
#define M_ERR_33_SUBMSG_7        MIL_TEXT("Cannot seek in file.")
#define M_ERR_33_SUBMSG_8        MIL_TEXT("Bad file format detected.")
#define M_ERR_33_SUBMSG_9        MIL_TEXT("Invalid info requested.")

#define M_PROCESSING_ERROR       34L
#define M_ERR_34_MSG             MIL_TEXT("Processing error.")
#define M_ERR_34_SUBMSG_1        MIL_TEXT("All buffers do not have the same working system.")
#define M_ERR_34_SUBMSG_2        MIL_TEXT("Cannot find any working system between buffers.")
#define M_ERR_34_SUBMSG_3        MIL_TEXT("Cannot process a HOST buffer as a whole and a temporary buffer.")
#define M_ERR_34_SUBMSG_4        MIL_TEXT("Source buffers cannot overlap destination buffer.")
#define M_ERR_34_SUBMSG_5        MIL_TEXT("No processor on target processing system.")
#define M_ERR_34_SUBMSG_6        MIL_TEXT("Pixel values out of supported range.")
#define M_ERR_34_SUBMSG_7        NO_SUBMSG
#define M_ERR_34_SUBMSG_8        NO_SUBMSG
#define M_ERR_34_SUBMSG_9        MIL_TEXT("Not enough memory or system limitation, cannot process buffer.")

#define M_INVALID_ID             35L
#define M_ERR_35_MSG             MIL_TEXT("Invalid MIL ID.")
#define M_ERR_35_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_35_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_35_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_35_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_35_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_35_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_35_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_35_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_35_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_INVALID_NATURE         36L
#define M_ERR_36_MSG             MIL_TEXT("Inappropriate MIL ID.")
#define M_ERR_36_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_36_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_36_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_36_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_36_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_36_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_36_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_36_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_36_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_INVALID_PARAM_ERROR_2  37L
#define M_ERR_37_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_37_SUBMSG_1        MIL_TEXT("For this operation, you should supply a LUT buffer with at least 512 entries.")
#define M_ERR_37_SUBMSG_2        MIL_TEXT("For this operation the grab mode must be asynchronous.")
#define M_ERR_37_SUBMSG_3        MIL_TEXT("This type of conversion requires two 3 band buffers.")
#define M_ERR_37_SUBMSG_4        MIL_TEXT("This type of conversion requires a 3 band source buffer.")
#define M_ERR_37_SUBMSG_5        MIL_TEXT("This type of conversion requires a 3 band destination buffer.")
#define M_ERR_37_SUBMSG_6        MIL_TEXT("Invalid interpolation type specified.")
#define M_ERR_37_SUBMSG_7        MIL_TEXT("Specified center is outside buffer.")
#define M_ERR_37_SUBMSG_8        MIL_TEXT("Image buffer must be 8 bit monochrome for this operation.")
MILERR( M_ERR,37,SUBMSG,8,       MIL_TEXT("Image must be 8 bit monochrome for this operation."))
#define M_ERR_37_SUBMSG_9        MIL_TEXT("Look up table must be 1 or 3 bands, 8 bits deep and have an X size of 256 or more.")

#define M_INVALID_ATTRIBUTE      38L
#define M_ERR_38_MSG             MIL_TEXT("Invalid attributes.")
#define M_ERR_38_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_38_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_38_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_38_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_38_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_38_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_38_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_38_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_38_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_CALL_CONTEXT_ERROR     39L
#define M_ERR_39_MSG             MIL_TEXT("Call context error")
#define M_ERR_39_SUBMSG_1        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_39_SUBMSG_2        NO_SUBMSG
#define M_ERR_39_SUBMSG_3        NO_SUBMSG
#define M_ERR_39_SUBMSG_4        NO_SUBMSG
#define M_ERR_39_SUBMSG_5        NO_SUBMSG
#define M_ERR_39_SUBMSG_6        NO_SUBMSG
#define M_ERR_39_SUBMSG_7        NO_SUBMSG
#define M_ERR_39_SUBMSG_8        NO_SUBMSG
#define M_ERR_39_SUBMSG_9        NO_SUBMSG

#define M_DRIVER_OBSOLETE        40L
#define M_ERR_40_MSG             MIL_TEXT("MIL driver obsolete.")
#define M_ERR_40_SUBMSG_1        MIL_TEXT("Version must be 8.0 or higher.")
#define M_ERR_40_SUBMSG_2        NO_SUBMSG
#define M_ERR_40_SUBMSG_3        NO_SUBMSG
#define M_ERR_40_SUBMSG_4        NO_SUBMSG
#define M_ERR_40_SUBMSG_5        NO_SUBMSG
#define M_ERR_40_SUBMSG_6        NO_SUBMSG
#define M_ERR_40_SUBMSG_7        NO_SUBMSG
#define M_ERR_40_SUBMSG_8        NO_SUBMSG
#define M_ERR_40_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_3  41L
#define M_ERR_41_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_41_SUBMSG_1        MIL_TEXT("Parameter 1 not in supported list.")
#define M_ERR_41_SUBMSG_2        MIL_TEXT("Parameter 2 not in supported list.")
#define M_ERR_41_SUBMSG_3        MIL_TEXT("Parameter 3 not in supported list.")
#define M_ERR_41_SUBMSG_4        MIL_TEXT("Parameter 4 not in supported list.")
#define M_ERR_41_SUBMSG_5        MIL_TEXT("Parameter 5 not in supported list.")
#define M_ERR_41_SUBMSG_6        MIL_TEXT("Parameter 6 not in supported list.")
#define M_ERR_41_SUBMSG_7        MIL_TEXT("Parameter 7 not in supported list.")
#define M_ERR_41_SUBMSG_8        MIL_TEXT("Parameter 8 not in supported list.")
#define M_ERR_41_SUBMSG_9        MIL_TEXT("Parameter 9 not in supported list.")

#define M_ALLOC_ERROR_2          42L
#define M_ERR_42_MSG             MIL_TEXT("Allocation error.")
#define M_ERR_42_SUBMSG_1        MIL_TEXT("Not enough host memory to do operation.")
#define M_ERR_42_SUBMSG_2        MIL_TEXT("Invalid attribute, M_GRAB not supported on host.")
MILERR( M_ERR,42,SUBMSG,2,       MIL_TEXT("The CanGrab property is not supported on host."))
#define M_ERR_42_SUBMSG_3        MIL_TEXT("Incompatible buffer dimensions, SizeBand, SizeY and (SizeX x SizeBit) must be identical.")
MILERR( M_ERR,42,SUBMSG,3,       MIL_TEXT("Incompatible buffer dimensions, NumberOfBands, SizeY and (SizeX x DataDepth) must be identical."))
#define M_ERR_42_SUBMSG_4        MIL_TEXT("Unable to communicate with MIL MtxDMA service.")
#define M_ERR_42_SUBMSG_5        MIL_TEXT("The size of the memory allocated is zero.")
#define M_ERR_42_SUBMSG_6        MIL_TEXT("Impossible to make a band child of a compressed buffer.")
#define M_ERR_42_SUBMSG_7        MIL_TEXT("You require a fast overscan buffer, but the M_ALLOCATION_OVERSCAN_SIZE is 0.")
#define M_ERR_42_SUBMSG_8        MIL_TEXT("Memory already allocated.")
#define M_ERR_42_SUBMSG_9        MIL_TEXT("The MIL_ID table is full. Make sure you don't have a resource leak.")

#define M_TIMER_ERROR            43L
#define M_ERR_43_MSG             MIL_TEXT("Timer error.")
#define M_ERR_43_SUBMSG_1        MIL_TEXT("Invalid timer mode specified.")
#define M_ERR_43_SUBMSG_2        MIL_TEXT("TimePtr parameter cannot be null.")
#define M_ERR_43_SUBMSG_3        MIL_TEXT("Installed hardware does not supports a high-resolution performance counter.")
#define M_ERR_43_SUBMSG_4        MIL_TEXT("Timer must be resetted prior to reading it.")
#define M_ERR_43_SUBMSG_5        NO_SUBMSG
#define M_ERR_43_SUBMSG_6        NO_SUBMSG
#define M_ERR_43_SUBMSG_7        NO_SUBMSG
#define M_ERR_43_SUBMSG_8        NO_SUBMSG
#define M_ERR_43_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_4  44L
#define M_ERR_44_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_44_SUBMSG_1        MIL_TEXT("Valid data formats for binary buffers are M_SINGLE_BAND and M_PLANAR.")
MILERR( M_ERR,44,SUBMSG,1,       MIL_TEXT("Valid formats for binary images are single band and Planar."))
#define M_ERR_44_SUBMSG_2        MIL_TEXT("Operation not supported on float buffer.")
#define M_ERR_44_SUBMSG_3        MIL_TEXT("Destination buffer is binary, MaskValue must be equal to 0 or 1.")
#define M_ERR_44_SUBMSG_4        MIL_TEXT("Image buffer must be monochrome for this operation.")
MILERR( M_ERR,44,SUBMSG,4,       MIL_TEXT("Image must be monochrome for this operation."))
#define M_ERR_44_SUBMSG_5        MIL_TEXT("Source buffers must be of the same type")
#define M_ERR_44_SUBMSG_6        MIL_TEXT("Integer source buffers must be unsigned")
#define M_ERR_44_SUBMSG_7        MIL_TEXT("Operation not supported on binary buffer.")
#define M_ERR_44_SUBMSG_8        MIL_TEXT("Source buffers must be of the same pixel depth.")
#define M_ERR_44_SUBMSG_9        MIL_TEXT("The width and height of the source and destination buffers must be a power of 2.")

#define M_INVALID_PARAM_ERROR_5  45L
#define M_ERR_45_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_45_SUBMSG_1        MIL_TEXT("The number of bands is not valid.")
#define M_ERR_45_SUBMSG_2        MIL_TEXT("The X dimension is not valid.")
#define M_ERR_45_SUBMSG_3        MIL_TEXT("The Y dimension is not valid.")
#define M_ERR_45_SUBMSG_4        MIL_TEXT("The pixel depth is not valid.")
#define M_ERR_45_SUBMSG_5        MIL_TEXT("The buffer attributes are not valid.")
#define M_ERR_45_SUBMSG_6        MIL_TEXT("The data type is not valid.")
#define M_ERR_45_SUBMSG_7        MIL_TEXT("Invalid FFT type specified.")
#define M_ERR_45_SUBMSG_8        MIL_TEXT("Invalid operation mode specified.")
#define M_ERR_45_SUBMSG_9        MIL_TEXT("Invalid transform specified.")

#define M_INVALID_PARAM_ERROR_6  46L
#define M_ERR_46_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_46_SUBMSG_1        MIL_TEXT("Operation not supported on 32 bits buffer.")
#define M_ERR_46_SUBMSG_2        MIL_TEXT("Invalid minimal variation specified.")
#define M_ERR_46_SUBMSG_3        MIL_TEXT("Invalid combination of control flag.")
#define M_ERR_46_SUBMSG_4        MIL_TEXT("A marker image or a minimal variation must be supplied.")
#define M_ERR_46_SUBMSG_5        MIL_TEXT("Value out of range.")
#define M_ERR_46_SUBMSG_6        MIL_TEXT("Invalid mode specified.")
#define M_ERR_46_SUBMSG_7        MIL_TEXT("The pointer should not be null.")
#define M_ERR_46_SUBMSG_8        MIL_TEXT("The buffer must be a M_IMAGE + M_COMPRESS buffer.")
MILERR( M_ERR,46,SUBMSG,8,       MIL_TEXT("The image must be a compress image. CompressionType property no equal to imUncompressed."))
#define M_ERR_46_SUBMSG_9        MIL_TEXT("Invalid compression type.")

#define M_INVALID_PARAM_ERROR_7  47L
#define M_ERR_47_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_47_SUBMSG_1        MIL_TEXT("Cannot change compression type of a single band.")
#define M_ERR_47_SUBMSG_2        MIL_TEXT("Invalid ControlFlag value.")
MILERR( M_ERR,47,SUBMSG,2,       MIL_TEXT("Invalid constant specified."))
#define M_ERR_47_SUBMSG_3        MIL_TEXT("The MIL_INT64 pointer can not be null.")
#define M_ERR_47_SUBMSG_4        MIL_TEXT("Invalid source buffers.")
#define M_ERR_47_SUBMSG_5        MIL_TEXT("Invalid Q factor value.")
#define M_ERR_47_SUBMSG_6        MIL_TEXT("Invalid destination buffers.")
#define M_ERR_47_SUBMSG_7        MIL_TEXT("The second LUT must be M_NULL for M_WARP_POLYNOMIAL operation mode.")
MILERR( M_ERR,47,SUBMSG,7,       MIL_TEXT("When WarpParameters.Coefficients it used the WarpParameters.WarpLUT data are ignored."))
#define M_ERR_47_SUBMSG_8        MIL_TEXT("The angle range must be lower than 360 degrees.")
#define M_ERR_47_SUBMSG_9        MIL_TEXT("Invalid First Warp parameter. Size or Type invalid.")

#define M_INVALID_PARAM_ERROR_8  48L
#define M_ERR_48_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_48_SUBMSG_1        MIL_TEXT("Source buffers must be of the same type and pixel depth.")
#define M_ERR_48_SUBMSG_2        MIL_TEXT("Destination buffers must be of the same type and pixel depth.")
#define M_ERR_48_SUBMSG_3        MIL_TEXT("Angle value must be between -360 and 360.")
#define M_ERR_48_SUBMSG_4        MIL_TEXT("Polar conversion only supported on float buffer.")
#define M_ERR_48_SUBMSG_5        MIL_TEXT("This type of conversion requires a 1 band destination buffer.")
#define M_ERR_48_SUBMSG_6        MIL_TEXT("Invalid transform type")
#define M_ERR_48_SUBMSG_7        MIL_TEXT("The M_NORMALIZE flag must be used with this transform.")
MILERR( M_ERR,48,SUBMSG,7,       MIL_TEXT("The Normalize parameter must set to True with this transform."))
#define M_ERR_48_SUBMSG_8        MIL_TEXT("This type of conversion requires a 1 band source buffer.")
#define M_ERR_48_SUBMSG_9        MIL_TEXT("Invalid number of bytes. Use zero or a positive value.")

#define M_JPEG_ERROR_2           49L
#define M_ERR_49_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_49_SUBMSG_1        MIL_TEXT("Vertical sampling factor of more than 4 not supported.")
#define M_ERR_49_SUBMSG_2        MIL_TEXT("MIL supports only 1-band and 3-band buffers.")
MILERR( M_ERR,49,SUBMSG,2,       MIL_TEXT("Supports only 1-band and 3-band images."))
#define M_ERR_49_SUBMSG_3        MIL_TEXT("MIL only supports sequential baseline DCT JPEG and lossless JPEG files.")
MILERR( M_ERR,49,SUBMSG,3,       MIL_TEXT("Supports only sequential baseline DCT JPEG and lossless JPEG files."))
#define M_ERR_49_SUBMSG_4        MIL_TEXT("Point transform not supported.")
#define M_ERR_49_SUBMSG_5        MIL_TEXT("Reading beyond the available data.")
#define M_ERR_49_SUBMSG_6        MIL_TEXT("End Of Image marker before the end of the file.")
#define M_ERR_49_SUBMSG_7        MIL_TEXT("16-bit quantization table not supported.")
#define M_ERR_49_SUBMSG_8        MIL_TEXT("Horizontal sampling factor of more than 4 not supported.")
#define M_ERR_49_SUBMSG_9        MIL_TEXT("Sampling factors do not correspond to any supported format.")

#define M_INVALID_PARAM_ERROR_9  50L
#define M_ERR_50_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_50_SUBMSG_1        MIL_TEXT("Inquire type not supported.")
#define M_ERR_50_SUBMSG_2        MIL_TEXT("The look up table must have a Y size of 1.")
#define M_ERR_50_SUBMSG_3        MIL_TEXT("The size of the buffer is too large.")
#define M_ERR_50_SUBMSG_4        MIL_TEXT("The destination buffer cannot be 1-bit.")
#define M_ERR_50_SUBMSG_5        MIL_TEXT("SaturationSizeBit must be 1, 8, 16 or 32.")
#define M_ERR_50_SUBMSG_6        MIL_TEXT("SaturationType must be M_SIGNED, M_UNSIGNED or M_FLOAT.")
MILERR( M_ERR,50,SUBMSG,6,       MIL_TEXT("SaturationType must be imSigned, imUnsigned or imFloat."))
#define M_ERR_50_SUBMSG_7        MIL_TEXT("Invalid combinaison of SaturationType and SizeBit.")
#define M_ERR_50_SUBMSG_8        MIL_TEXT("YUV buffers must be M_UNSIGNED.")
MILERR( M_ERR,50,SUBMSG,8,       MIL_TEXT("YUV images DataType should must be imUnsigned."))
#define M_ERR_50_SUBMSG_9        MIL_TEXT("Invalid filename: Filename length must be at least 1.")

#define M_AVI_FILE_ERROR         51L
#define M_ERR_51_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_51_SUBMSG_1        MIL_TEXT("Error creating AVI file.")
#define M_ERR_51_SUBMSG_2        MIL_TEXT("Error writing into AVI file.")
#define M_ERR_51_SUBMSG_3        MIL_TEXT("File format not supported.")
#define M_ERR_51_SUBMSG_4        MIL_TEXT("Incompatible format between file and images.")
#define M_ERR_51_SUBMSG_5        MIL_TEXT("Error opening AVI file.")
#define M_ERR_51_SUBMSG_6        MIL_TEXT("One of the buffers to export is empty.")
#define M_ERR_51_SUBMSG_7        MIL_TEXT("Incompatible JPEG format between the buffers and the file type requested.")
#define M_ERR_51_SUBMSG_8        MIL_TEXT("The number of bands should be the same for all the buffers in the sequence.")
#define M_ERR_51_SUBMSG_9        MIL_TEXT("The depth should be the same for all the buffers in the sequence.")

#define M_DMA_ERROR              52L
#define M_ERR_52_MSG             MIL_TEXT("Error using Matrox Dma service")
#define M_ERR_52_SUBMSG_1        MIL_TEXT("Cannot open the Matrox Dma manager")
#define M_ERR_52_SUBMSG_2        MIL_TEXT("Cannot acces PCI data")
#define M_ERR_52_SUBMSG_3        NO_SUBMSG
#define M_ERR_52_SUBMSG_4        MIL_TEXT("Cannot communicate with MtxDMA driver. Check if it is installed and started")
#define M_ERR_52_SUBMSG_5        MIL_TEXT("Cannot acces hook information")
#define M_ERR_52_SUBMSG_6        MIL_TEXT("Can perform this operation only on Default Host")
#define M_ERR_52_SUBMSG_7        MIL_TEXT("Unable to recover info on Dma pool")
#define M_ERR_52_SUBMSG_8        NO_SUBMSG
#define M_ERR_52_SUBMSG_9        MIL_TEXT("Could not enable or disable the Write-combining feature")

#define M_BMP_ERROR_3            53L
#define M_ERR_53_MSG             MIL_TEXT("BMP handler general error.")
#define M_ERR_53_SUBMSG_1        MIL_TEXT("MIL does not support a bit count of zero in BMP files.")
#define M_ERR_53_SUBMSG_2        MIL_TEXT("MIL does not support 1-bit BMP files.")
#define M_ERR_53_SUBMSG_3        MIL_TEXT("MIL does not support 4-bit BMP files.")
#define M_ERR_53_SUBMSG_4        MIL_TEXT("Run Length Encoding ( RLE ) compression not supported.")
#define M_ERR_53_SUBMSG_5        NO_SUBMSG
#define M_ERR_53_SUBMSG_6        NO_SUBMSG
#define M_ERR_53_SUBMSG_7        NO_SUBMSG
#define M_ERR_53_SUBMSG_8        NO_SUBMSG
#define M_ERR_53_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR_3           54L
#define M_ERR_54_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_54_SUBMSG_1        MIL_TEXT("Invalid JPEG data.")
#define M_ERR_54_SUBMSG_2        MIL_TEXT("Invalid data for a MTRX specific marker.")
#define M_ERR_54_SUBMSG_3        MIL_TEXT("Extended Sequential DCT not supported in MIL.")
#define M_ERR_54_SUBMSG_4        MIL_TEXT("Progressive DCT not supported in MIL.")
#define M_ERR_54_SUBMSG_5        MIL_TEXT("YUV9 Packed images not supported.")
#define M_ERR_54_SUBMSG_6        MIL_TEXT("YUV12 Packed images not supported.")
#define M_ERR_54_SUBMSG_7        MIL_TEXT("YUV24 Packed images not supported.")
#define M_ERR_54_SUBMSG_8        MIL_TEXT("Invalid data. EOI marker not found.")
#define M_ERR_54_SUBMSG_9        MIL_TEXT("Cannot create or open file in write mode.")
  
#define M_ALLOC_ERROR_3          55L
#define M_ERR_55_MSG             MIL_TEXT("Allocation error.")
#define M_ERR_55_SUBMSG_1        MIL_TEXT("No pitch control flag used in create")
#define M_ERR_55_SUBMSG_2        MIL_TEXT("Cannot create a buffer with physical address.")
#define M_ERR_55_SUBMSG_3        MIL_TEXT("Cannot allocate a M_COMPRESS buffer with the M_DISP attribute.")
MILERR( M_ERR,55,SUBMSG,3,       MIL_TEXT("Cannot allocate an image with a compression type and the CanDisplay property set to True."))
#define M_ERR_55_SUBMSG_4        MIL_TEXT("Cannot allocate a M_COMPRESS buffer with the M_SIGNED type.")
MILERR( M_ERR,55,SUBMSG,4,       MIL_TEXT("Cannot allocate an image with a compression type and the DataType property set to imSigned."))
#define M_ERR_55_SUBMSG_5        MIL_TEXT("Interlaced buffers should have a height of at least 2.")
#define M_ERR_55_SUBMSG_6        MIL_TEXT("Requested system DLL cannot be loaded.")
#define M_ERR_55_SUBMSG_7        MIL_TEXT("MIL DLLs not found.")
#define M_ERR_55_SUBMSG_8        MIL_TEXT("Not enough non-paged memory to allocate buffer.")
#define M_ERR_55_SUBMSG_9        MIL_TEXT("Not enough non-paged memory for MIL CE allocation.\nContinuing the application may cause abnormal behavior!")

#define M_LIMITATION_ERROR       56L
#define M_ERR_56_MSG             MIL_TEXT("Limitation error.")
#define M_ERR_56_SUBMSG_1        MIL_TEXT("This operation cannot be performed on a section of a compressed buffer.")
#define M_ERR_56_SUBMSG_2        MIL_TEXT("This operation cannot be performed on a child of a compressed buffer.")
#define M_ERR_56_SUBMSG_3        MIL_TEXT("This operation cannot be performed on a section of a YUV buffer.")
#define M_ERR_56_SUBMSG_4        MIL_TEXT("This operation cannot be performed on a child of a YUV buffer.")
#define M_ERR_56_SUBMSG_5        MIL_TEXT("This operation cannot be performed on a section of a M_PACKED buffer.")
MILERR( M_ERR,56,SUBMSG,5,       MIL_TEXT("This operation cannot be performed on a section of an imPacked format image."))
#define M_ERR_56_SUBMSG_6        MIL_TEXT("This operation cannot be performed on a child of a M_PACKED buffer.")
MILERR( M_ERR,56,SUBMSG,6,       MIL_TEXT("This operation cannot be performed on a child of an imPacked format image."))
#define M_ERR_56_SUBMSG_7        MIL_TEXT("This operation cannot be performed on a section of a M_PLANAR buffer.")
MILERR( M_ERR,56,SUBMSG,7,       MIL_TEXT("This operation cannot be performed on a section of an imPlanar format image."))
#define M_ERR_56_SUBMSG_8        MIL_TEXT("This operation cannot be performed on a child of a M_PLANAR buffer.")
MILERR( M_ERR,56,SUBMSG,8,       MIL_TEXT("This operation cannot be performed on a child of an imPlanar format image."))
#define M_ERR_56_SUBMSG_9        NO_SUBMSG

#define M_OPERATION_ERROR_2      57L
#define M_ERR_57_MSG             MIL_TEXT("Operation error.")
#define M_ERR_57_SUBMSG_1        MIL_TEXT("Pitch must be a multiple of 4 pixels.")
#define M_ERR_57_SUBMSG_2        MIL_TEXT("Default PitchByte value is incompatible with this type of buffer")
#define M_ERR_57_SUBMSG_3        MIL_TEXT("Operation not supported on remote system.")
#define M_ERR_57_SUBMSG_4        MIL_TEXT("Not enough non-paged memory to perform the operation on remote system.")
#define M_ERR_57_SUBMSG_5        MIL_TEXT("Not enough remote memory to perform the operation on the remote system.")
#define M_ERR_57_SUBMSG_6        MIL_TEXT("Operation not supported on this system")
#define M_ERR_57_SUBMSG_7        MIL_TEXT("Cannot allocate event.")
#define M_ERR_57_SUBMSG_8        MIL_TEXT("Cannot create event.")
#define M_ERR_57_SUBMSG_9        MIL_TEXT("There is no low level id for this MIL_ID.")

#define M_LICENSING_ERROR        58L
#define M_ERR_58_MSG             MIL_TEXT("Licensing error")
#define M_ERR_58_SUBMSG_1        NO_SUBMSG
#define M_ERR_58_SUBMSG_2        MIL_TEXT("License watchdog timed out")
#define M_ERR_58_SUBMSG_3        MIL_TEXT("A module was used without a valid license")
#define M_ERR_58_SUBMSG_4        MIL_TEXT("Debugging is not allowed with a run-time license. You must use a development license.")
#define M_ERR_58_SUBMSG_5        MIL_TEXT("Your license does not allow the allocation of JPEG buffers")
#define M_ERR_58_SUBMSG_6        MIL_TEXT("Your license does not allow the allocation of JPEG2000 buffers")
#define M_ERR_58_SUBMSG_7        NO_SUBMSG
#define M_ERR_58_SUBMSG_8        NO_SUBMSG
#define M_ERR_58_SUBMSG_9        NO_SUBMSG

#define M_AVI_FILE_ERROR_2       59L
#define M_ERR_59_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_59_SUBMSG_1        MIL_TEXT("Cannot export a sequence of compressed child buffers.")
#define M_ERR_59_SUBMSG_2        MIL_TEXT("The compression type should be the same for all the buffers in the sequence.")
#define M_ERR_59_SUBMSG_3        MIL_TEXT("The internal format should be the same for all the buffers in the sequence.")
#define M_ERR_59_SUBMSG_4        MIL_TEXT("The file format requested for the M_APPEND is not the same as the file format of the file to append into.")
MILERR( M_ERR,59,SUBMSG,4,       MIL_TEXT("The file format requested for the aviAppendToFile parameter is not the same as the file format of the file to append into.")) 
#define M_ERR_59_SUBMSG_5        MIL_TEXT("The size of the images should all be the same." )
#define M_ERR_59_SUBMSG_6        MIL_TEXT("Impossible to write into a closed file.")
#define M_ERR_59_SUBMSG_7        MIL_TEXT("Error creating AVI stream.")
#define M_ERR_59_SUBMSG_8        MIL_TEXT("The file is already opened.")
#define M_ERR_59_SUBMSG_9        MIL_TEXT("Impossible to read from a closed file.")

#define M_JPEG_COMPRESS_ERROR_3  60L
#define M_ERR_60_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_60_SUBMSG_1        MIL_TEXT("Cannot allocate 3-band M_JPEG_LOSSLESS_INTERLACED buffer.")
MILERR( M_ERR,60,SUBMSG,1,       MIL_TEXT("Cannot allocate 3-band image with the Format property set to imJPEGLosslessInterlaced.")) 
#define M_ERR_60_SUBMSG_2        MIL_TEXT("In lossless, the RestartInterval multiplied by the number of \ncolumns in the buffer must fit in 16 bits (smaller than 65536).")
#define M_ERR_60_SUBMSG_3        MIL_TEXT("The restart interval must fit in 16 bits (smaller than 65536).")
#define M_ERR_60_SUBMSG_4        MIL_TEXT("Only JPEG lossless buffers can have predictors.")
#define M_ERR_60_SUBMSG_5        MIL_TEXT("Luminance parameters are only allowed for YUV buffers.")
#define M_ERR_60_SUBMSG_6        MIL_TEXT("Chrominance parameters are only allowed for YUV buffers.")
#define M_ERR_60_SUBMSG_7        MIL_TEXT("AC tables and quantization parameters are only allowed \nfor JPEG lossy buffers.")
#define M_ERR_60_SUBMSG_8        MIL_TEXT("For YUV buffers, specific luminance or chrominance \nflags must be used.")
#define M_ERR_60_SUBMSG_9        MIL_TEXT("Field size is only applicable to interlaced buffers.")

#define M_AVI_FILE_ERROR_3       61L
#define M_ERR_61_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_61_SUBMSG_1        MIL_TEXT("Error reading from the AVI file.")
#define M_ERR_61_SUBMSG_2        MIL_TEXT("Cannot read after the last frame.")
#define M_ERR_61_SUBMSG_3        MIL_TEXT("Cannot initialize the COM library.")
#define M_ERR_61_SUBMSG_4        MIL_TEXT("Images must have a width greater than or equal to 16.")
#define M_ERR_61_SUBMSG_5        MIL_TEXT("Images must have a height greater than or equal to 8.")
#define M_ERR_61_SUBMSG_6        NO_SUBMSG
#define M_ERR_61_SUBMSG_7        NO_SUBMSG
#define M_ERR_61_SUBMSG_8        NO_SUBMSG
#define M_ERR_61_SUBMSG_9        NO_SUBMSG

#define M_USER_BIT_CONFIG_ERROR  62L
#define M_ERR_62_MSG             MIL_TEXT("Auxiliary I/O Error:")
#define M_ERR_62_SUBMSG_1        MIL_TEXT("4Sight not detected, or pins already allocated on an other system.")
#define M_ERR_62_SUBMSG_2        MIL_TEXT("Cannot write register, register is read only.")
#define M_ERR_62_SUBMSG_3        MIL_TEXT("Cannot read register, register is write only.")
#define M_ERR_62_SUBMSG_4        MIL_TEXT("Invalid data or register.")
#define M_ERR_62_SUBMSG_5        MIL_TEXT("Unkown Error.")
#define M_ERR_62_SUBMSG_6        MIL_TEXT("Interrupt Overrun.")
#define M_ERR_62_SUBMSG_7        MIL_TEXT("Interrupts have been dropped.")
#define M_ERR_62_SUBMSG_8        NO_SUBMSG
#define M_ERR_62_SUBMSG_9        NO_SUBMSG

#define M_USER_BIT_ERROR         63L
#define M_ERR_63_MSG             MIL_TEXT("Invalid parameter error:")
#define M_ERR_63_SUBMSG_1        MIL_TEXT("Cannot write selected pin, pin is in input mode.")
#define M_ERR_63_SUBMSG_2        MIL_TEXT("Cannot enable interrupt, pin is in output mode.")
#define M_ERR_63_SUBMSG_3        MIL_TEXT("Cannot hook to selected pin, pin is not configured as an interrupt.")
#define M_ERR_63_SUBMSG_4        MIL_TEXT("Control type not supported.")
#define M_ERR_63_SUBMSG_5        MIL_TEXT("Control value not supported.")
#define M_ERR_63_SUBMSG_6        MIL_TEXT("Inquire type not supported.")
#define M_ERR_63_SUBMSG_7        MIL_TEXT("Invalid bit mask.")
#define M_ERR_63_SUBMSG_8        MIL_TEXT("Cannot hook or set interrupt.")
#define M_ERR_63_SUBMSG_9        MIL_TEXT("No interrupt line has been assigned to auxiliary IO.")

#define M_TIFF_ERROR_3           64L
#define M_ERR_64_MSG             MIL_TEXT("TIFF File access error.")
#define M_ERR_64_SUBMSG_1        MIL_TEXT("Error recording tag")
#define M_ERR_64_SUBMSG_2        MIL_TEXT("LZW compression not supported in multi strips TIFF file")
#define M_ERR_64_SUBMSG_3        NO_SUBMSG
#define M_ERR_64_SUBMSG_4        NO_SUBMSG
#define M_ERR_64_SUBMSG_5        NO_SUBMSG
#define M_ERR_64_SUBMSG_6        NO_SUBMSG
#define M_ERR_64_SUBMSG_7        NO_SUBMSG
#define M_ERR_64_SUBMSG_8        NO_SUBMSG
#define M_ERR_64_SUBMSG_9        NO_SUBMSG

#define M_MIL_FILE_ERROR_4       65L
#define M_ERR_65_MSG             MIL_TEXT("MIL File access error.")
#define M_ERR_65_SUBMSG_1        MIL_TEXT("Error recording tag")
#define M_ERR_65_SUBMSG_2        MIL_TEXT("LZW compression not supported in multi strips MIL file")
#define M_ERR_65_SUBMSG_3        NO_SUBMSG
#define M_ERR_65_SUBMSG_4        NO_SUBMSG
#define M_ERR_65_SUBMSG_5        NO_SUBMSG
#define M_ERR_65_SUBMSG_6        NO_SUBMSG
#define M_ERR_65_SUBMSG_7        NO_SUBMSG
#define M_ERR_65_SUBMSG_8        NO_SUBMSG
#define M_ERR_65_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR_4           66L
#define M_ERR_66_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_66_SUBMSG_1        MIL_TEXT("Second field of interlaced image not found.")
#define M_ERR_66_SUBMSG_2        NO_SUBMSG
#define M_ERR_66_SUBMSG_3        NO_SUBMSG
#define M_ERR_66_SUBMSG_4        NO_SUBMSG
#define M_ERR_66_SUBMSG_5        NO_SUBMSG
#define M_ERR_66_SUBMSG_6        NO_SUBMSG
#define M_ERR_66_SUBMSG_7        NO_SUBMSG
#define M_ERR_66_SUBMSG_8        NO_SUBMSG
#define M_ERR_66_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_10 67L
#define M_ERR_67_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_67_SUBMSG_1        MIL_TEXT("The width and height of the source and destination buffers must be a multiple of 8.")
#define M_ERR_67_SUBMSG_2        MIL_TEXT("Operation not supported on 16 bits buffer.")
#define M_ERR_67_SUBMSG_3        MIL_TEXT("Size of image buffers must be greater than or equal to the size of the kernel.")
MILERR( M_ERR,67,SUBMSG,3,       MIL_TEXT("Size of images must be greater than or equal to the size of the kernel."))
#define M_ERR_67_SUBMSG_4        MIL_TEXT("Invalid data formats")
#define M_ERR_67_SUBMSG_5        MIL_TEXT("Operation not supported on 3 bands planar buffer.")
#define M_ERR_67_SUBMSG_6        MIL_TEXT("Float address must be a multiple of 4")
#define M_ERR_67_SUBMSG_7        MIL_TEXT("This operation is only supported on the host")
#define M_ERR_67_SUBMSG_8        MIL_TEXT("Cannot create a M_NO_MAP buffer with null pointers")
#define M_ERR_67_SUBMSG_9        MIL_TEXT("Can only create a M_NO_MAP buffer with a physical address")

#define M_J2K_ERROR_1            68L
#define M_ERR_68_MSG             MIL_TEXT("JPEG2000 handler general error.")
#define M_ERR_68_SUBMSG_1        MIL_TEXT("Error closing JPEG2000 file.")
#define M_ERR_68_SUBMSG_2        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_68_SUBMSG_3        MIL_TEXT("Cannot create or open file in write mode.")
#define M_ERR_68_SUBMSG_4        MIL_TEXT("Error opening JPEG2000 file.")
#define M_ERR_68_SUBMSG_5        NO_SUBMSG
#define M_ERR_68_SUBMSG_6        NO_SUBMSG
#define M_ERR_68_SUBMSG_7        NO_SUBMSG
#define M_ERR_68_SUBMSG_8        NO_SUBMSG
#define M_ERR_68_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_2            69L
#define M_ERR_69_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_69_SUBMSG_1        MIL_TEXT("Invalid JPEG2000 data.")
#define M_ERR_69_SUBMSG_2        MIL_TEXT("Unexpected marker in the JPEG2000 image.")
#define M_ERR_69_SUBMSG_3        MIL_TEXT("Component index out of range.")
#define M_ERR_69_SUBMSG_4        MIL_TEXT("MIL supports only 1-band and 3-band buffers.")
#define M_ERR_69_SUBMSG_5        MIL_TEXT("All components must have the same bit depth.")
#define M_ERR_69_SUBMSG_6        MIL_TEXT("Only 8, 10, 12, 14 or 16-bit images are supported.")
#define M_ERR_69_SUBMSG_7        MIL_TEXT("Invalid subsampling factors.")
#define M_ERR_69_SUBMSG_8        MIL_TEXT("Scalar implicit quantization not supported.")
#define M_ERR_69_SUBMSG_9        MIL_TEXT("Only the entropy coding style is supported.")

#define M_LICENSE_ERROR_2        70L
#define M_ERR_70_MSG             MIL_TEXT("License error.")
#define M_ERR_70_SUBMSG_1        NO_SUBMSG
#define M_ERR_70_SUBMSG_2        MIL_TEXT("No valid license found (2)")
#define M_ERR_70_SUBMSG_3        MIL_TEXT("No valid license found (3)")
#define M_ERR_70_SUBMSG_4        NO_SUBMSG
#define M_ERR_70_SUBMSG_5        NO_SUBMSG
#define M_ERR_70_SUBMSG_6        MIL_TEXT("No valid license found (6)")
#define M_ERR_70_SUBMSG_7        NO_SUBMSG
#define M_ERR_70_SUBMSG_8        NO_SUBMSG
#define M_ERR_70_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_3            71L
#define M_ERR_71_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_71_SUBMSG_1        MIL_TEXT("Unsupported progression order.")
#define M_ERR_71_SUBMSG_2        MIL_TEXT("Multi-layer images are not supported.")
#define M_ERR_71_SUBMSG_3        MIL_TEXT("Unsupported compression setting.")
#define M_ERR_71_SUBMSG_4        MIL_TEXT("This marker is not supported by MIL yet.")
#define M_ERR_71_SUBMSG_5        MIL_TEXT("Wavelet type cannot be changed.")
#define M_ERR_71_SUBMSG_6        MIL_TEXT("Mismatched field settings.")
#define M_ERR_71_SUBMSG_7        MIL_TEXT("Not enough user memory to do the operation.")
#define M_ERR_71_SUBMSG_8        MIL_TEXT("Invalid number of decomposition levels.")
#define M_ERR_71_SUBMSG_9        MIL_TEXT("Invalid number of wavelet sub-bands.")

#define M_J2K_ERROR_4            72L
#define M_ERR_72_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_72_SUBMSG_1        MIL_TEXT("The target size must be a positive number of bytes.")
#define M_ERR_72_SUBMSG_2        MIL_TEXT("Target size is only allowed in lossy mode.")
#define M_ERR_72_SUBMSG_3        MIL_TEXT("Quantization tables are applicable to lossy buffers only.")
#define M_ERR_72_SUBMSG_4        MIL_TEXT("The Q factor is only valid with lossy buffers.")
#define M_ERR_72_SUBMSG_5        MIL_TEXT("Wrong number of entries in the quantization table.")
#define M_ERR_72_SUBMSG_6        MIL_TEXT("Invalid MIL_ID for the quantization table.")
#define M_ERR_72_SUBMSG_7        MIL_TEXT("The quantization table must be a one-band, 32 bits floating-point array.")
#define M_ERR_72_SUBMSG_8        MIL_TEXT("Valid Q factor values are between 1 and 99 (both inclusive).")
#define M_ERR_72_SUBMSG_9        MIL_TEXT("Too many tile-parts.")

#define M_J2K_ERROR_5            73L
#define M_ERR_73_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_73_SUBMSG_1        MIL_TEXT("Corrupted JPEG2000 data.")
#define M_ERR_73_SUBMSG_2        MIL_TEXT("Quantization coefficient out of range.")
#define M_ERR_73_SUBMSG_3        MIL_TEXT("Luminance and chrominance parameters are only allowed for YUV buffers.")
#define M_ERR_73_SUBMSG_4        MIL_TEXT("The number of reserved bytes must be greater than 0.")
#define M_ERR_73_SUBMSG_5        MIL_TEXT("Tile offsets are out of range.")
#define M_ERR_73_SUBMSG_6        MIL_TEXT("Image offsets are out of range.")
#define M_ERR_73_SUBMSG_7        MIL_TEXT("Only one COC marker by band is allowed.")
#define M_ERR_73_SUBMSG_8        MIL_TEXT("Only one QCC marker by band is allowed.")
#define M_ERR_73_SUBMSG_9        MIL_TEXT("Invalid size bit.")

#define M_FUNC_CALL_ERROR        74L
#define M_ERR_74_MSG             MIL_TEXT("Function call error.")
#define M_ERR_74_SUBMSG_1        MIL_TEXT("Calibration not supported on remote system")
#define M_ERR_74_SUBMSG_2        MIL_TEXT("Unable to load the DLL")
#define M_ERR_74_SUBMSG_3        MIL_TEXT("Work function not found in the DLL")
#define M_ERR_74_SUBMSG_4        MIL_TEXT("Cannot execute because two MIL objects resides in two different workspaces/platform and compensation cannot be done.") //TBM MIL 8
#define M_ERR_74_SUBMSG_5        MIL_TEXT("Slave module is not in the user modules table")
#define M_ERR_74_SUBMSG_6        MIL_TEXT("Slave function is not in the user functions table")
#define M_ERR_74_SUBMSG_7        NO_SUBMSG
#define M_ERR_74_SUBMSG_8        NO_SUBMSG
#define M_ERR_74_SUBMSG_9        NO_SUBMSG

#define M_FREE_ERROR             75L
#define M_ERR_75_MSG             MIL_TEXT("Cannot free internally allocated objects")
#define M_ERR_75_SUBMSG_1        NO_SUBMSG
#define M_ERR_75_SUBMSG_2        NO_SUBMSG
#define M_ERR_75_SUBMSG_3        NO_SUBMSG
#define M_ERR_75_SUBMSG_4        NO_SUBMSG
#define M_ERR_75_SUBMSG_5        NO_SUBMSG
#define M_ERR_75_SUBMSG_6        NO_SUBMSG
#define M_ERR_75_SUBMSG_7        NO_SUBMSG
#define M_ERR_75_SUBMSG_8        NO_SUBMSG
#define M_ERR_75_SUBMSG_9        NO_SUBMSG

#define M_BUF_TRANSFER_ERROR     76L
#define M_ERR_76_MSG             NO_MSG
#define M_ERR_76_SUBMSG_1        MIL_TEXT("Unknown transfer function")
#define M_ERR_76_SUBMSG_2        MIL_TEXT("Transfer method not supported for this transfer function")
#define M_ERR_76_SUBMSG_3        MIL_TEXT("Transfer can't be done asynchronously")
#define M_ERR_76_SUBMSG_4        MIL_TEXT("Scaling not supported by this transfer function")
#define M_ERR_76_SUBMSG_5        MIL_TEXT("No transfer made")
#define M_ERR_76_SUBMSG_6        MIL_TEXT("MTX0 transfer cannot be done on paged memory")
#define M_ERR_76_SUBMSG_7        MIL_TEXT("MTX0 not supported in this environment")
#define M_ERR_76_SUBMSG_8        MIL_TEXT("Illegal null MIL_ID passed to MbufTransfer")
#define M_ERR_76_SUBMSG_9        MIL_TEXT("Invalid Transfer type")

#define M_DISP_SERVICE_ERROR     77L
#define M_ERR_77_MSG             MIL_TEXT("Display service error")
#define M_ERR_77_SUBMSG_1        MIL_TEXT("Cannot load MilDisplay DLL")
#define M_ERR_77_SUBMSG_2        MIL_TEXT("Cannot find address of requested function")
#define M_ERR_77_SUBMSG_3        MIL_TEXT("Cannot find AppAlloc function in MilDisplay")
#define M_ERR_77_SUBMSG_4        MIL_TEXT("Cannot find AppFree function in MilDisplay")
#define M_ERR_77_SUBMSG_5        NO_SUBMSG
#define M_ERR_77_SUBMSG_6        NO_SUBMSG
#define M_ERR_77_SUBMSG_7        NO_SUBMSG
#define M_ERR_77_SUBMSG_8        NO_SUBMSG
#define M_ERR_77_SUBMSG_9        NO_SUBMSG

#define M_ALLOC_ERROR_4          78L
#define M_ERR_78_MSG             MIL_TEXT("Allocation error.")
#define M_ERR_78_SUBMSG_1        MIL_TEXT("M_BGR15 and M_BGR16 buffers are not supported")
#define M_ERR_78_SUBMSG_2        MIL_TEXT("This requested system is not (correctly) intalled")
#define M_ERR_78_SUBMSG_3        NO_SUBMSG
#define M_ERR_78_SUBMSG_4        MIL_TEXT("Cannot open the DCF.")
#define M_ERR_78_SUBMSG_5        MIL_TEXT("Cannot create a buffer at address 0")
#define M_ERR_78_SUBMSG_6        MIL_TEXT("Cannot allocate buffer with the M_INTERNAL_ALLOCATION attribute")
#define M_ERR_78_SUBMSG_7        MIL_TEXT("The driver DLL has an invalid format and can't be loaded")
#define M_ERR_78_SUBMSG_8        MIL_TEXT("Invalid bits set in the attribute")
#define M_ERR_78_SUBMSG_9        MIL_TEXT("Cannot create a buffer on this MIL_ID, the source buffer doesn't have a valid host address.")

#define M_SYS_CTRL_INQ_ERROR     79L
#define M_ERR_79_MSG             MIL_TEXT("System Control/Inquire error.")
#define M_ERR_79_SUBMSG_1        MIL_TEXT("Control/Inquire type supported only on the Host system")
#define M_ERR_79_SUBMSG_2        MIL_TEXT("Default PitchByte must be a power of 2.")
#define M_ERR_79_SUBMSG_3        MIL_TEXT("This Control/Inquire is not supported any more")
#define M_ERR_79_SUBMSG_4        MIL_TEXT("This Control/Inquire is now a Digitizer Control/Inquire")
#define M_ERR_79_SUBMSG_5        MIL_TEXT("This Control/Inquire is now a Display Control/Inquire")
#define M_ERR_79_SUBMSG_6        NO_SUBMSG
#define M_ERR_79_SUBMSG_7        NO_SUBMSG
#define M_ERR_79_SUBMSG_8        NO_SUBMSG
#define M_ERR_79_SUBMSG_9        NO_SUBMSG

#define M_APP_MODIF_ERROR        80L
#define M_ERR_80_MSG             MIL_TEXT("Swap ID error.")
#define M_ERR_80_SUBMSG_1        MIL_TEXT("This function does not support remote objects")
#define M_ERR_80_SUBMSG_2        NO_SUBMSG
#define M_ERR_80_SUBMSG_3        NO_SUBMSG
#define M_ERR_80_SUBMSG_4        NO_SUBMSG
#define M_ERR_80_SUBMSG_5        NO_SUBMSG
#define M_ERR_80_SUBMSG_6        NO_SUBMSG
#define M_ERR_80_SUBMSG_7        NO_SUBMSG
#define M_ERR_80_SUBMSG_8        NO_SUBMSG
#define M_ERR_80_SUBMSG_9        NO_SUBMSG

#define M_INSTALLATION_ERROR     81L
#define M_ERR_81_MSG             MIL_TEXT("Installation error")
#define M_ERR_81_SUBMSG_1        MIL_TEXT("Could not find the installation path in the registry")
#define M_ERR_81_SUBMSG_2        MIL_TEXT("The list of installed boards is corrupted in the registry. Please re-install MIL")
#define M_ERR_81_SUBMSG_3        NO_SUBMSG
#define M_ERR_81_SUBMSG_4        NO_SUBMSG
#define M_ERR_81_SUBMSG_5        NO_SUBMSG
#define M_ERR_81_SUBMSG_6        NO_SUBMSG
#define M_ERR_81_SUBMSG_7        NO_SUBMSG
#define M_ERR_81_SUBMSG_8        NO_SUBMSG
#define M_ERR_81_SUBMSG_9        NO_SUBMSG

#define M_BUF_CTRL_INQ_ERROR     82L
#define M_ERR_82_MSG             MIL_TEXT("Buffer Control/Inquire error.")
#define M_ERR_82_SUBMSG_1        MIL_TEXT("Invalid index of bands.")
#define M_ERR_82_SUBMSG_2        NO_SUBMSG
#define M_ERR_82_SUBMSG_3        NO_SUBMSG
#define M_ERR_82_SUBMSG_4        NO_SUBMSG
#define M_ERR_82_SUBMSG_5        NO_SUBMSG
#define M_ERR_82_SUBMSG_6        NO_SUBMSG
#define M_ERR_82_SUBMSG_7        NO_SUBMSG
#define M_ERR_82_SUBMSG_8        NO_SUBMSG
#define M_ERR_82_SUBMSG_9        NO_SUBMSG

#define M_CALL_PA_PARAM_ERROR    83L
#define M_ERR_83_MSG             MIL_TEXT("Invalid parameter in function call to PA")
#define M_ERR_83_SUBMSG_1        MIL_TEXT("All the MIL IDs need to be allocated on the same system")
#define M_ERR_83_SUBMSG_2        MIL_TEXT("Only buffer IDs are supported")
#define M_ERR_83_SUBMSG_3        MIL_TEXT("At least one parameter must be an ID")
#define M_ERR_83_SUBMSG_4        MIL_TEXT("Cannot execute MfuncCallPA on the Host system")
#define M_ERR_83_SUBMSG_5        MIL_TEXT("Too many parameters")
#define M_ERR_83_SUBMSG_6        NO_SUBMSG
#define M_ERR_83_SUBMSG_7        NO_SUBMSG
#define M_ERR_83_SUBMSG_8        NO_SUBMSG
#define M_ERR_83_SUBMSG_9        NO_SUBMSG

#define M_DISP_CTRL_INQ_ERROR    84L
#define M_ERR_84_MSG             MIL_TEXT("Display Control/Inquire error.")
#define M_ERR_84_SUBMSG_1        MIL_TEXT("This Control/Inquire is not supported any more")
#define M_ERR_84_SUBMSG_2        NO_SUBMSG
#define M_ERR_84_SUBMSG_3        NO_SUBMSG
#define M_ERR_84_SUBMSG_4        NO_SUBMSG
#define M_ERR_84_SUBMSG_5        NO_SUBMSG
#define M_ERR_84_SUBMSG_6        NO_SUBMSG
#define M_ERR_84_SUBMSG_7        NO_SUBMSG
#define M_ERR_84_SUBMSG_8        NO_SUBMSG
#define M_ERR_84_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_6            85L
#define M_ERR_85_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_85_SUBMSG_1        MIL_TEXT("Too many tiles.")
#define M_ERR_85_SUBMSG_2        MIL_TEXT("Invalid code block size.")
#define M_ERR_85_SUBMSG_3        MIL_TEXT("Unexpected field delimiter.")
#define M_ERR_85_SUBMSG_4        MIL_TEXT("Video standard not supported.")
#define M_ERR_85_SUBMSG_5        MIL_TEXT("Unknown wavelet and quantization settings.")
#define M_ERR_85_SUBMSG_6        MIL_TEXT("Unknown entropy coder parameters.")
#define M_ERR_85_SUBMSG_7        MIL_TEXT("Code block attributes are truncated.")
#define M_ERR_85_SUBMSG_8        MIL_TEXT("The field is truncated.")
#define M_ERR_85_SUBMSG_9        MIL_TEXT("Invalid JP2 format.")

#define M_XX86XX_ERROR           86L
#define M_ERR_86_MSG             NO_MSG
#define M_ERR_86_SUBMSG_1        NO_SUBMSG
#define M_ERR_86_SUBMSG_2        NO_SUBMSG
#define M_ERR_86_SUBMSG_3        NO_SUBMSG
#define M_ERR_86_SUBMSG_4        NO_SUBMSG
#define M_ERR_86_SUBMSG_5        NO_SUBMSG
#define M_ERR_86_SUBMSG_6        NO_SUBMSG
#define M_ERR_86_SUBMSG_7        NO_SUBMSG
#define M_ERR_86_SUBMSG_8        NO_SUBMSG
#define M_ERR_86_SUBMSG_9        NO_SUBMSG

#define M_XX87XX_ERROR           87L
#define M_ERR_87_MSG             NO_MSG
#define M_ERR_87_SUBMSG_1        NO_SUBMSG
#define M_ERR_87_SUBMSG_2        NO_SUBMSG
#define M_ERR_87_SUBMSG_3        NO_SUBMSG
#define M_ERR_87_SUBMSG_4        NO_SUBMSG
#define M_ERR_87_SUBMSG_5        NO_SUBMSG
#define M_ERR_87_SUBMSG_6        NO_SUBMSG
#define M_ERR_87_SUBMSG_7        NO_SUBMSG
#define M_ERR_87_SUBMSG_8        NO_SUBMSG
#define M_ERR_87_SUBMSG_9        NO_SUBMSG

#define M_XX88XX_ERROR           88L
#define M_ERR_88_MSG             NO_MSG
#define M_ERR_88_SUBMSG_1        NO_SUBMSG
#define M_ERR_88_SUBMSG_2        NO_SUBMSG
#define M_ERR_88_SUBMSG_3        NO_SUBMSG
#define M_ERR_88_SUBMSG_4        NO_SUBMSG
#define M_ERR_88_SUBMSG_5        NO_SUBMSG
#define M_ERR_88_SUBMSG_6        NO_SUBMSG
#define M_ERR_88_SUBMSG_7        NO_SUBMSG
#define M_ERR_88_SUBMSG_8        NO_SUBMSG
#define M_ERR_88_SUBMSG_9        NO_SUBMSG

#define M_XX89XX_ERROR           89L
#define M_ERR_89_MSG             NO_MSG
#define M_ERR_89_SUBMSG_1        NO_SUBMSG
#define M_ERR_89_SUBMSG_2        NO_SUBMSG
#define M_ERR_89_SUBMSG_3        NO_SUBMSG
#define M_ERR_89_SUBMSG_4        NO_SUBMSG
#define M_ERR_89_SUBMSG_5        NO_SUBMSG
#define M_ERR_89_SUBMSG_6        NO_SUBMSG
#define M_ERR_89_SUBMSG_7        NO_SUBMSG
#define M_ERR_89_SUBMSG_8        NO_SUBMSG
#define M_ERR_89_SUBMSG_9        NO_SUBMSG

#define M_XX90XX_ERROR           90L
#define M_ERR_90_MSG             NO_MSG
#define M_ERR_90_SUBMSG_1        NO_SUBMSG
#define M_ERR_90_SUBMSG_2        NO_SUBMSG
#define M_ERR_90_SUBMSG_3        NO_SUBMSG
#define M_ERR_90_SUBMSG_4        NO_SUBMSG
#define M_ERR_90_SUBMSG_5        NO_SUBMSG
#define M_ERR_90_SUBMSG_6        NO_SUBMSG
#define M_ERR_90_SUBMSG_7        NO_SUBMSG
#define M_ERR_90_SUBMSG_8        NO_SUBMSG
#define M_ERR_90_SUBMSG_9        NO_SUBMSG

#define M_XX91XX_ERROR           91L
#define M_ERR_91_MSG             NO_MSG
#define M_ERR_91_SUBMSG_1        NO_SUBMSG
#define M_ERR_91_SUBMSG_2        NO_SUBMSG
#define M_ERR_91_SUBMSG_3        NO_SUBMSG
#define M_ERR_91_SUBMSG_4        NO_SUBMSG
#define M_ERR_91_SUBMSG_5        NO_SUBMSG
#define M_ERR_91_SUBMSG_6        NO_SUBMSG
#define M_ERR_91_SUBMSG_7        NO_SUBMSG
#define M_ERR_91_SUBMSG_8        NO_SUBMSG
#define M_ERR_91_SUBMSG_9        NO_SUBMSG

#define M_XX92XX_ERROR           92L
#define M_ERR_92_MSG             NO_MSG
#define M_ERR_92_SUBMSG_1        NO_SUBMSG
#define M_ERR_92_SUBMSG_2        NO_SUBMSG
#define M_ERR_92_SUBMSG_3        NO_SUBMSG
#define M_ERR_92_SUBMSG_4        NO_SUBMSG
#define M_ERR_92_SUBMSG_5        NO_SUBMSG
#define M_ERR_92_SUBMSG_6        NO_SUBMSG
#define M_ERR_92_SUBMSG_7        NO_SUBMSG
#define M_ERR_92_SUBMSG_8        NO_SUBMSG
#define M_ERR_92_SUBMSG_9        NO_SUBMSG

#define M_XX93XX_ERROR           93L
#define M_ERR_93_MSG             NO_MSG
#define M_ERR_93_SUBMSG_1        NO_SUBMSG
#define M_ERR_93_SUBMSG_2        NO_SUBMSG
#define M_ERR_93_SUBMSG_3        NO_SUBMSG
#define M_ERR_93_SUBMSG_4        NO_SUBMSG
#define M_ERR_93_SUBMSG_5        NO_SUBMSG
#define M_ERR_93_SUBMSG_6        NO_SUBMSG
#define M_ERR_93_SUBMSG_7        NO_SUBMSG
#define M_ERR_93_SUBMSG_8        NO_SUBMSG
#define M_ERR_93_SUBMSG_9        NO_SUBMSG

#define M_XX94XX_ERROR           94L
#define M_ERR_94_MSG             NO_MSG
#define M_ERR_94_SUBMSG_1        NO_SUBMSG
#define M_ERR_94_SUBMSG_2        NO_SUBMSG
#define M_ERR_94_SUBMSG_3        NO_SUBMSG
#define M_ERR_94_SUBMSG_4        NO_SUBMSG
#define M_ERR_94_SUBMSG_5        NO_SUBMSG
#define M_ERR_94_SUBMSG_6        NO_SUBMSG
#define M_ERR_94_SUBMSG_7        NO_SUBMSG
#define M_ERR_94_SUBMSG_8        NO_SUBMSG
#define M_ERR_94_SUBMSG_9        NO_SUBMSG

#define M_XX95XX_ERROR           95L
#define M_ERR_95_MSG             NO_MSG
#define M_ERR_95_SUBMSG_1        NO_SUBMSG
#define M_ERR_95_SUBMSG_2        NO_SUBMSG
#define M_ERR_95_SUBMSG_3        NO_SUBMSG
#define M_ERR_95_SUBMSG_4        NO_SUBMSG
#define M_ERR_95_SUBMSG_5        NO_SUBMSG
#define M_ERR_95_SUBMSG_6        NO_SUBMSG
#define M_ERR_95_SUBMSG_7        NO_SUBMSG
#define M_ERR_95_SUBMSG_8        NO_SUBMSG
#define M_ERR_95_SUBMSG_9        NO_SUBMSG

#define M_XX96XX_ERROR           96L
#define M_ERR_96_MSG             NO_MSG
#define M_ERR_96_SUBMSG_1        NO_SUBMSG
#define M_ERR_96_SUBMSG_2        NO_SUBMSG
#define M_ERR_96_SUBMSG_3        NO_SUBMSG
#define M_ERR_96_SUBMSG_4        NO_SUBMSG
#define M_ERR_96_SUBMSG_5        NO_SUBMSG
#define M_ERR_96_SUBMSG_6        NO_SUBMSG
#define M_ERR_96_SUBMSG_7        NO_SUBMSG
#define M_ERR_96_SUBMSG_8        NO_SUBMSG
#define M_ERR_96_SUBMSG_9        NO_SUBMSG

#define M_XX97XX_ERROR           97L
#define M_ERR_97_MSG             NO_MSG
#define M_ERR_97_SUBMSG_1        NO_SUBMSG
#define M_ERR_97_SUBMSG_2        NO_SUBMSG
#define M_ERR_97_SUBMSG_3        NO_SUBMSG
#define M_ERR_97_SUBMSG_4        NO_SUBMSG
#define M_ERR_97_SUBMSG_5        NO_SUBMSG
#define M_ERR_97_SUBMSG_6        NO_SUBMSG
#define M_ERR_97_SUBMSG_7        NO_SUBMSG
#define M_ERR_97_SUBMSG_8        NO_SUBMSG
#define M_ERR_97_SUBMSG_9        NO_SUBMSG

#define M_XX98XX_ERROR           98L
#define M_ERR_98_MSG             NO_MSG
#define M_ERR_98_SUBMSG_1        NO_SUBMSG
#define M_ERR_98_SUBMSG_2        NO_SUBMSG
#define M_ERR_98_SUBMSG_3        NO_SUBMSG
#define M_ERR_98_SUBMSG_4        NO_SUBMSG
#define M_ERR_98_SUBMSG_5        NO_SUBMSG
#define M_ERR_98_SUBMSG_6        NO_SUBMSG
#define M_ERR_98_SUBMSG_7        NO_SUBMSG
#define M_ERR_98_SUBMSG_8        NO_SUBMSG
#define M_ERR_98_SUBMSG_9        NO_SUBMSG

#define M_XX99XX_ERROR           99L
#define M_ERR_99_MSG             NO_MSG
#define M_ERR_99_SUBMSG_1        NO_SUBMSG
#define M_ERR_99_SUBMSG_2        NO_SUBMSG
#define M_ERR_99_SUBMSG_3        NO_SUBMSG
#define M_ERR_99_SUBMSG_4        NO_SUBMSG
#define M_ERR_99_SUBMSG_5        NO_SUBMSG
#define M_ERR_99_SUBMSG_6        NO_SUBMSG
#define M_ERR_99_SUBMSG_7        NO_SUBMSG
#define M_ERR_99_SUBMSG_8        NO_SUBMSG
#define M_ERR_99_SUBMSG_9        NO_SUBMSG


#endif
