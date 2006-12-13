/*******************************************************************
 *
 * COPYRIGHT (c) 2000 Matrox Electronic Systems Ltd.
 * All Rights Reserved
 *
 *******************************************************************/

#ifndef __IMAPIPRO_H__
#define __IMAPIPRO_H__

#if !defined(_IM_LOCAL_CODE) && defined(SHOW_INCLUDE_FILE)
#pragma message ("#include "__FILE__)
#endif

#if !defined(_IM_HOST_OS_LINUX_DRIVER)
#include <stdio.h>
#endif

/*********************************/
/* Applications functions        */
/*********************************/

IM_EXTC void IM_FTYPE imAppCatchError(long Mode, void (*handler)(void *), void *HandlerParam) IM_LFTYPE;
IM_EXTC void IM_FTYPE imAppControl(long Item, double Value) IM_LFTYPE;
IM_EXTC long IM_FTYPE imAppGetError(long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imAppInquire(long Item, void *ValuePtr) IM_LFTYPE;

/*********************************/
/* Device allocation functions   */
/*********************************/

IM_EXTC void IM_FTYPE imDevAlloc(long System, long ProcessingNode, char *ShellFile, long Mode, long *DevPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDevControl(long Dev, long Item, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDevFree(long Dev) IM_LFTYPE;
IM_EXTC long IM_FTYPE imDevInquire(long Dev, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDevTrace(long Dev, char *LogStr, ...) IM_LFTYPE;

/*********************************/
/* Thread control functions      */
/*********************************/

IM_EXTC void IM_FTYPE imThrAlloc(long Dev, long Control, long *ThreadPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imThrCancel(long Thread) IM_LFTYPE;
IM_EXTC void IM_FTYPE imThrControl(long Thread, long Item, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imThrFree(long Thread) IM_LFTYPE;
IM_EXTC long IM_FTYPE imThrGetError(long Thread, long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imThrHalt(long Thread, long Mode) IM_LFTYPE;
IM_EXTC long IM_FTYPE imThrInquire(long Thread, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imThrNop(long Thread, long OSB) IM_LFTYPE;

/*********************************/
/*  Synchronization functions    */
/*********************************/

IM_EXTC void IM_FTYPE imSyncAlloc(long Thread, long *OSBPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imSyncFree(long Thread, long OSB) IM_LFTYPE;
IM_EXTC long IM_FTYPE imSyncGetError(long Thread, long OSB, long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imSyncHost(long Thread, long OSB, long State) IM_LFTYPE;
IM_EXTC long IM_FTYPE imSyncHostMultiple(long Thread, long NumOSB, long *OSBs, long State, double Timeout) IM_LFTYPE;
IM_EXTC void IM_FTYPE imSyncThread(long Thread, long OSB, long State) IM_LFTYPE;
IM_EXTC void IM_FTYPE imSyncControl(long Thread, long OSB, long Item, double Value) IM_LFTYPE;
IM_EXTC long IM_FTYPE imSyncInquire(long Thread, long OSB, long Item, void * ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imSyncGrabSequence(long Thread, long OSB, long Frame, long Mode) IM_LFTYPE;

/*********************************/
/* Buffer Allocation functions   */
/*********************************/

IM_EXTC void IM_FTYPE imBufAlloc(long Thread, long Xsize, long Ysize, long Nbands, long Type, long Location, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufAlloc2d(long Thread, long Xsize, long Ysize, long Type, long Location, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufAlloc1d(long Thread, long Xsize, long Type, long Location, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufAllocControl(long Thread, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufChild(long Thread, long Buf, long Xstart, long Ystart, long Xsize, long Ysize, long *ChildPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufChildBand(long Thread, long Buf, long BandNum, long *BandPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufChildMove(long Thread, long Child, long Xoff, long Yoff, long Xsize, long Ysize) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufClear(long Thread, long Buf, double Value, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufClone(long Thread, long Buf, long NewLocation, long *NewBufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCopy(long Thread, long SrcBuf, long DstBuf, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCopyField(long Thread, long SrcBuf, long SrcTag, long DstBuf, long DstTag) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCopyPCI(long Thread, long SrcBuf, long DstBuf, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCopyVM(long Thread, long SrcBuf, long DstBuf, long SrcControl, long DstControl, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufFree(long Thread, long Buf) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufGet(long Thread, long Buf, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufGet1d(long Thread, long Buf, long Xstart, long Xsize, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufGet2d(long Thread, long Buf, long Xstart, long Ystart, long Xsize, long Ysize, void *Ptr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBufGetField(long Thread, long Buf, long Tag, long *ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBufGetFieldDouble(long Thread, long Buf, long Tag, double *ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBufGetNextField(long Thread, long Buf, long *ContextPtr, long *TagPtr, double *ValuePtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBufInquire(long Thread, long Buf, long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufLoad(long Thread, char *FileName, long Format, long Buf) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBufMap(long Thread, long Buf,long Band,long Ystart, void **AddrPtr, long *PitchPtr, long *NlinesPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufModify(long Thread,long Buf,long Xsize,long Ysize,long Type) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPack(long Thread,long SrcBuf,long TagBuf,long DstBuf,long Mode,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPut(long Thread, long Buf, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPut1d(long Thread, long Buf, long Xstart, long Xsize, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPut2d(long Thread, long Buf, long Xstart, long Ystart, long Xsize, long Ysize, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPutField(long Thread, long Buf, long Tag, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufPutFormatted(long Thread, long Buf, long Control, void *Ptr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufRemoveField(long Thread, long Buf, long Tag) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufSave(long Thread, char *FileName, long Format, long Buf) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufRestore(long Thread, char *FileName, long Format, long Location, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCreate(long Thread, long Xsize, long Ysize,long Nbands, long Type, long Location, void **AddrPtr,long Pitch, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCreate64(long Thread, long Xsize, long Ysize,long Nbands, long Type, long Location, ULONGLONG *AddrPtr,long Pitch, long *BufPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufControl(long Thread, long Buf, long Item, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufCopyROI(long Thread, long Src, long Dst, long Mode, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufStatistics(long Thread, long Src, long Result, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBufStatCond(long Thread, long Src, long Result, long Mode, long CondBuf, long Cond, double Low, double High, long OSB) IM_LFTYPE;

/**********************************/
/* Camera functions               */
/**********************************/

IM_EXTC void IM_FTYPE imCamAlloc(long Thread, char *CamFile,long Mode, long *CameraPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamClone(long Thread,long Camera,long Mode,long *NewCamPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamControl(long Thread, long Camera, long Item, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamFree(long Thread, long Camera) IM_LFTYPE;
IM_EXTC long IM_FTYPE imCamInquire(long Thread, long Camera, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamSave(long Thread, char *CamFile, long Camera) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamControlReg(long Thread, long Camera, double RegValue, char *Register) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCamInquireReg(long Thread, long Camera, void *CurrentValuePtr, void *DefaultValuePtr, char *Register) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigAlloc(long Thread, long System,long Digitizer,long Mode,long *DigPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigCapture(long Thread, long Dig, long Cam, long Mode) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigControl(long Thread,long Dig,long Item, double Value ) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigGrab(long Thread, long Dig, long Cam, long Buf, long Nframes, long Control, long OSB) IM_LFTYPE;
IM_EXTC long IM_FTYPE imDigInquire(long Thread,long Dig, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigFree(long Thread,long Dig) IM_LFTYPE;
IM_EXTC long IM_FTYPE imDigLutGet(long Thread, long Dig, long Cam, long LutBuf, long LutComp, long LutAddr, long LutSize) IM_LFTYPE;
IM_EXTC long IM_FTYPE imDigLutSet(long Thread, long Dig, long Cam, long GrabBuf, long LutComp, long LutAddr, long LutSize, long Mode) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDigGrabSequence(long Thread, long Dig, long Cam, long Buf, long Count, long Control, long OSB) IM_LFTYPE;

/*********************************/
/* Display functions             */
/*********************************/

IM_EXTC void IM_FTYPE imDispAlloc(long Thread, long System, long Display, char *DispFile, long Mode,long *DispPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDispControl(long Thread, long Disp,long Control,long Mode) IM_LFTYPE;
IM_EXTC long IM_FTYPE imDispInquire(long Thread,long Disp,long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imDispFree(long Thread,long Disp) IM_LFTYPE;

/********************************************************************
 *
 * Preliminary prototypes for cursor functions.
 *
 ********************************************************************/

IM_EXTC void IM_FTYPE imCurAlloc(long Dev, long Mode, long *Cursor) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurFree(long Dev, long Cursor) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurDefine(long Dev, long Cursor, long SizeX, long SizeY, long HotX, long HotY, unsigned char *Data) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurSetColor(long Dev, long Cursor, long Index, long Red, long Green, long Blue) IM_LFTYPE;

IM_EXTC void IM_FTYPE imCurSelect(long Dev, long Cursor) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurEnable(long Dev, long Flag) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurSetPosition(long Dev, long PosX, long PosY) IM_LFTYPE;
IM_EXTC void IM_FTYPE imCurGetPosition(long Dev, long *PosX, long *PosY) IM_LFTYPE;


/*********************************/
/* Point functions               */
/*********************************/

IM_EXTC void IM_FTYPE imIntConvert(long Thread, long Src, long Dst, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntDyadic(long Thread, long Src1, long Src2, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntGainOffset(long Thread, long Src, long Dst, long Offset, long Gain, long Shift, long ClipVal, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntLutMap(long Thread, long Src, long Dst, long Lut, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntLutSubtract(long Thread, long Src1, long Src2, long Dst, long Lut1, long Lut2, long SrcBits, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntMac1(long Thread, long Src, long Dst, long Fac, long Const, long Shift, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntMac2(long Thread, long Src1, long Src2, long Dst, long Fac1, long Fac2, long Shift, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntMonadic(long Thread, long Src, double Const, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntTriadic(long Thread, long SrcA, long SrcB, long SrcC, long Dst,long Rotate,long Op,long Mode,long OSB) IM_LFTYPE;

/*********************************/
/* Neighbourhood functions       */
/*********************************/

IM_EXTC void IM_FTYPE imIntConnectMap(long Thread, long Src, long Dst, long Lut, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntConvolve(long Thread, long Src, long Dst, long Kernel, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntCorrelate(long Thread,long Src,long Dst,long Model,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntErodeDilate(long Thread, long Src, long Dst, long Kernel,long Op,long Niter,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntEdgeDetect(long Thread,long Src,long DstIntensity, long DstAngle, long KerX, long KerY, double Threshold,long ModeIntensity,long ModeAngle,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntRank(long Thread, long Src, long Dst, long Kernel, long Rank, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntThickThin(long Thread, long Src, long Dst, long Struct, long Op, long Niter,long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntLabel(long Thread,long Src,long Dst,long Mode,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntDistance(long Thread,long Src,long Dst,long Transform,long Control,long OSB) IM_LFTYPE;

/*********************************/
/* Statistical functions         */
/*********************************/

IM_EXTC void IM_FTYPE imIntCountDifference(long Thread, long Src1, long Src2, long Result, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntFindExtreme(long Thread, long Src, long Result, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntHistogram(long Thread, long Src, long Result, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntHistogramEqualize(long Thr, long Src, long Dst, long HistSize, long Func, double Alpha, long Min, long Max, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntLocateEvent(long Thread, long Src, long X, long Y, long Pix, long Num, long Cond, long Low, long High, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntProject(long Thread, long Src, long Result, double ProjectAngle, long Mode, long OSB) IM_LFTYPE;

/********************************/
/* Binary functions             */
/********************************/

IM_EXTC void IM_FTYPE imBinConvert(long Thread, long Src, long Dst, long Cond, long Val1, long Val2, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBinMorphic(long Thread, long Src, long Dst,long Kernel,long Op,long Niter,long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBinTriadic(long Thread, long SrcA, long SrcB, long SrcC, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBinCountDifference(long Thread, long SrcBuf1, long SrcBuf2, long Result, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBinThin(long Thread, long SrcBuf, long DstBuf, long NIter, long Control, long OSB) IM_LFTYPE;

/********************************/
/* Floating point functions     */
/********************************/

IM_EXTC void IM_FTYPE imFloatConvert(long Thread, long Src, long Dst, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatDyadic(long Thread, long Src1, long Src2, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatMac1(long Thread, long Src, long Dst, double Fac, double Const, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatMac2(long Thread, long Src1, long Src2, long Dst, double Fac1, double Fac2, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatMonadic(long Thread, long Src, double Const, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatUnary(long Thread, long Src, long Dst, long Op, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imFloatClip(long Thread, long Src, long Dst, long Cond, double Thresh, double Thresh_h, double Val1, double Val2, long Mode, long OSB) IM_LFTYPE;

/********************************/
/* Miscellaneous functions      */
/********************************/
IM_EXTC void IM_FTYPE imIntConvertColor(long Thread,long Src,long Dst,long Type,long Mode,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntFFT(long Thread,long SrcR,long SrcI,long DstR,long DstI,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntBinarize(long Thread, long Src, long Dst,long Cond,long Thresh_l, long Thresh_h, long Val1, long Val2, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntRecFilter(long Thread, long Src, long Src2, long Dst, long Dst2, long Lut, long SrcBits, long DstBits, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntClip(long Thread, long Src, long Dst, long Cond, long Thresh, long Thresh_h, long Val1, long Val2, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntAverage(long Thread, long In, long Acc, long Sum, long Ave, long NumFrame, long Normalize, long Control, long Osb) IM_LFTYPE;

/********************************/
/* Geometric transform functions*/
/********************************/

IM_EXTC void IM_FTYPE imIntFlip(long Thread, long Src, long Dst, long Func, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntSubsample(long Thread, long Src, long Dst, long Xfac, long Yfac, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntZoom(long Thread, long Src, long Dst, long Xfac, long Yfac, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntScale(long Thread, long Src, long Dst, double XFac, double YFac, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntWarpLut(long Thread,long Src,long Dst,long Xlut,long Ylut,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntWarpPolynomial(long Thread,long Src,long Dst,long Coef,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imIntWarpMatrix(long Thread,long Src,long Dst,long Coef,long Control,long OSB) IM_LFTYPE;

/********************************/
/* Data generations functions   */
/********************************/
IM_EXTC void IM_FTYPE imGen1d(long Thread, long Buf, long Func, long Start, long End, long NumCoeffs, double *Coeffs, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGenWarp1stOrder(long Thread,long Coef,long Transform,double Val1,double Val2,long Mode,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGenWarp4Corner(long Thread,long Coef, double X1,double Y1,double X2, double Y2, double X3,double Y3,double X4, double Y4, long Xstart,long Ystart,long Xend,long Yend, long Mode,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGenWarpLutMatrix(long Thread,long Xlut,long Ylut,long Coef,long Control,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGenWarpLutPolar(long Thread, long Xlut, long Ylut, double Xcen, double Ycen, double Rstart, double Rend, double Astart, double Aend, long Mode, long Control, long OSB) IM_LFTYPE;

/********************************/
/* Graphics functions           */
/********************************/

IM_EXTC void IM_FTYPE imGraArc(long Thread,long Context,long Buf,long Xcen,long Ycen,long Xrad,long Yrad, double StartAng,double EndAng) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraArcFill(long Thread,long Context,long Buf,long Xcen,long Ycen,long Xrad,long Yrad, double StartAng,double EndAng) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraFill(long Thread,long Context,long Buf,long Xstart,long Ystart) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraLine(long Thread,long Context,long Buf, long Xstart,long Ystart,long Xend,long Yend) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraPlot(long Thread,long Context,long buf, long Xbuf,long Ybuf,long NumPoints) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraRect(long Thread,long Context,long Buf,long Xstart, long Ystart,long Xend,long Yend) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraRectFill(long Thread,long Context,long Buf,long Xstart, long Ystart,long Xend,long Yend) IM_LFTYPE;
IM_EXTC void IM_FTYPE imGraText(long Thread,long Context,long Buf, long Xstart,long Ystart,char *String) IM_LFTYPE;

/*******************************/
/* JPEG functions              */
/*******************************/
IM_EXTC void IM_FTYPE imJpegAlloc(long Thread,long Control,long *JpegPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegControl(long Thread,long Jpeg,long Item,long Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegControlBand(long Thread,long Jpeg,long Band,long Item,long Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegEncode(long Thread,long Buf,long Jpeg,long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegFree(long Thread,long Jpeg) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegPutTable(long Thread,long Jpeg,long TableType,long TableNum,long TableSize,void *TablePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegWrite(long Thread,FILE *FileHandle,long Jpeg) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegSave(long Thread,char *FileName,long Jpeg) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegDecode(long Thread,long Buf,long Jpeg,long OSB) IM_LFTYPE;
IM_EXTC long IM_FTYPE imJpegInquire(long Thread,long Jpeg,long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegRead(long Thread,FILE *FileHandle,long Jpeg) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegRestore(long Thread,char *FileName,long *JpegPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegGetTable(long Thread,long Jpeg,long TableType,long TableNum,long *TableSize,void *TablePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegWriteBuf(long Thread, long DstBuf, long JpegBuf, long Start, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imJpegReadBuf(long Thread, long DstBuf, long JpegBuf, long Start, long OSB) IM_LFTYPE;

/*******************************/
/* Pattern Matching Module     */
/*******************************/
IM_EXTC void IM_FTYPE imPatAllocAutoModel(long Thread, long SrcBuf, long XSize, long YSize, long XUncert, long YUncert, long Type, long Mode, long *ModelPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatAllocModel(long Thread, long SrcBuf, long XOff, long YOff, long XSize, long YSize, long Type, long *ModelPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatAllocResult(long Thread, long NumEntries, long *ResultPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatAllocRotatedModel(long Thread, long SrcModel, double RotAngle, long Mode, long *NewModelPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatCopy(long Thread, long Model, long DstBuf, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatCopyResult(long Thread, long Result, long Type, long DstBuf, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatFindModel(long Thread, long SrcBuf, long Model, long Result, long OSB) IM_LFTYPE;
/* IM_EXTC void IM_FTYPE imPatFindMultipleModel(long Thread, long SrcBuf, long *Model, long *Result, long NumModels, long Mode, long OSB) IM_LFTYPE; */
IM_EXTC void IM_FTYPE imPatFree(long Thread, long ModelOrResult) IM_LFTYPE;
IM_EXTC long IM_FTYPE imPatGetNumber(long Thread, long Result, long *NumPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatGetResult(long Thread, long Result, long Type, void *Ptr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imPatInquire(long Thread, long ModelOrResult, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatPreprocModel(long Thread, long Buf, long Model, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatRead(long Thread, FILE *FileHandle, long *ModelPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatRestore(long Thread, char *FileName, long *ModelPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSave(long Thread, char *FileName, long Model) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetAcceptance(long Thread, long Model, double Acceptance) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetAccuracy(long Thread, long Model, long Accuracy) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetCenter(long Thread, long Model, double XCen, double YCen) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetCertainty(long Thread, long Model, double Certainty) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetDontCare(long Thread, long Model, long Image, long XOff, long YOff, long Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetNumber(long Thread, long Model, long Number) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetPosition(long Thread, long Model, long XOff, long YOff, long XSize, long YSize) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetSearchParameter(long Thread, long Model, long Param, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatSetSpeed(long Thread, long Model, long Speed) IM_LFTYPE;
IM_EXTC void IM_FTYPE imPatWrite(long Thread, FILE *FileHandle, long Model) IM_LFTYPE;

/****************************/
/* Blob Analysis Module     */
/****************************/
IM_EXTC void IM_FTYPE imBlobAllocFeatureList(long Thread, long *FeatListPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobAllocResult(long Thread, long *ResultPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobCalculate(long Thread, long IdentBuf, long GreyBuf, long FeatList, long Result, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobControl(long Thread, long Result, long Item, double Value) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobCopyResult(long Thread, long Result, long Feature, long Mode, long DstBuf, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobCopyRuns(long Thread, long Result, long Label, long XBuf, long YBuf, long LenBuf, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobFill(long Thread, long Result, long DstBuf, long Mode, long Value, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobSelect(long Thread, long Result, long Operation, long Feature, long Mode, long Cond, double Val1, double Val2) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobFree(long Thread, long Result) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBlobGetLabel(long Thread, long Result, long XPos, long YPos, long *LabelPtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBlobGetNumber(long Thread, long Result, long *NumPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobGetResult(long Thread, long Result, long Feature, long Mode, void *ArrayPtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobGetResultSingle(long Thread, long Result, long Label, long Feature, long Mode, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobGetRuns(long Thread, long Result, long Label, long Type, void *XPtr, void *YPtr, void *LenPtr) IM_LFTYPE;
IM_EXTC long IM_FTYPE imBlobInquire(long Thread, long Result, long Item, void *ValuePtr) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobLabel(long Thread, long Result, long DstBuf, long Mode, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobSelectFeature(long Thread, long FeatList, long Feature, long Mode) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobSelectFeret(long Thread, long FeatList, double AtAngle) IM_LFTYPE;
IM_EXTC void IM_FTYPE imBlobSelectMoment(long Thread, long FeatList, long Mode, long Type, long XOrder, long YOrder) IM_LFTYPE;

/*******************************/
/* System functions            */
/*******************************/

IM_EXTC long   IM_FTYPE imSysInquire(long System,long Item,void *ValuePtr) IM_LFTYPE;
IM_EXTC long   IM_FTYPE imSysGetMemoryMap(long Thread, long item, long *result1, long *result2, long *result3) IM_LFTYPE;
IM_EXTC void   IM_FTYPE imSysDiag(long System, long Node, long item, void* ptr) IM_LFTYPE;
IM_EXTC double IM_FTYPE imSysClock(long Thread,double Offset) IM_LFTYPE;
IM_EXTC void   IM_FTYPE imSysSleep(long Thread,double Sleep) IM_LFTYPE;
IM_EXTC void   IM_FTYPE imSysTimeStamp(long Thread, long Buf, long Tag, double Offset) IM_LFTYPE;

IM_EXTC int    IM_FTYPE im_open_pipe(long mode) IM_LFTYPE;
IM_EXTC void   IM_FTYPE im_close_pipe(void) IM_LFTYPE;
IM_EXTC int    IM_FTYPE im_print_msg(int wait_on, ...) IM_LFTYPE;

/***********************/
/* RLE functions       */
/***********************/
IM_EXTC void IM_FTYPE imRleDecode(long Thread, long Buf, long CompBuf, long Control, long OSB) IM_LFTYPE;
IM_EXTC void IM_FTYPE imRleEncode(long Thread, long Buf, long CompBuf, long Control, long OSB) IM_LFTYPE;


/*******************************/
/* Mil PPC Special function    */
/*******************************/
IM_EXTC void IM_FTYPE imMilPpcComm(long Thread, long DatagramSize, void *DatagramPtr, long IsAsynchrone) IM_LFTYPE;


/*******************************/
/* Internal ONL test function  */
/*******************************/
IM_EXTC void IM_FTYPE imPrivateDo1dIO(long Thread, long Src, long Dst, long Type, long MaxPix, long Control, long OSB);
IM_EXTC void IM_FTYPE imPrivateDo2dIO(long Thread, long Src, long Dst, long Type, long BlockX, long BlockY, long KerSize, long OverMode, long Control, long OSB);
IM_EXTC void IM_FTYPE imPrivateMonadic1(long Thread, long Src, double Val, long Dst, long Op, long OSB);
IM_EXTC void IM_FTYPE imPrivateMonadic2(long Thread, long Src, double Val, long Dst, long Dst2, long Op, long OSB);
IM_EXTC void IM_FTYPE imPrivateConvertColorMatrix(long Thread, long Src, long Dst, long Type, long CoefId, long OSB);
IM_EXTC void IM_FTYPE imPrivateBufClear(long Thread, long Dst, long Value, long OSB);

#endif /* __IMAPIPRO_H__ */

