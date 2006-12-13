/***************************************************************************/
/*

    Filename:  MILFUNC.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) Mfunc user's functions.

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILFUNC_H
#define __MILFUNC_H

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif													 

/***************************************************************************/
/* TOOLKIT FOR PSEUDO MIL FUNCTIONS DEVELOPMENT                            */
/***************************************************************************/
typedef MFTYPE32 void (MFTYPE MPTYPE *MFUNCFCTPTR)(MIL_ID FunctionId);
typedef MFTYPE32 void (MFTYPE MPTYPE *MFUNCSLAVEFCTPTR)(MIL_ID FunctionId);
typedef long (*MFUNCSLAVEPAFCTPTR)(MIL_ID FunctionId);
MFTYPE32 MIL_ID MFTYPE MfuncAlloc(MIL_TEXT_PTR FunctionName,
                                  long ParameterNumber,
                                  MFUNCFCTPTR WorkFunctionPtr,
                                  MIL_TEXT_PTR WorkDllName,
                                  MIL_TEXT_PTR WorkFunctionName,
                                  long WorkFunctionOpcode,
                                  long InitFlag,
                                  MIL_ID *FuncIdVarPtr);
MFTYPE32 MIL_ID MFTYPE MfuncAllocId(MIL_ID FunctionId, long ObjectType, void *UserPtr);
MFTYPE32 long MFTYPE MfuncParamCheck(MIL_ID FuncId);
MFTYPE32 void MFTYPE MfuncParamId(MIL_ID FuncId,long ParamIndex,MIL_ID ParamVal,
                                  long ParamIs, long ParamHasAttr);
MFTYPE32 void MFTYPE MfuncParamLong(MIL_ID FuncId,long ParamIndex,long ParamVal);
MFTYPE32 void MFTYPE MfuncParamDouble(MIL_ID FuncId,long ParamIndex,double ParamVal);
MFTYPE32 void MFTYPE MfuncParamPointer(MIL_ID FuncId, long ParamIndex, void * ParamVal, long SizeOfData, long ParamAttribute);
#ifndef M_MIL_75_COMPATIBLE
MFTYPE32 void MFTYPE MfuncParamIdPointer(MIL_ID FuncId, long ParamIndex, MIL_ID *ParamVal, long ParamIs, long ParamHasAttr);
MFTYPE32 void MFTYPE MfuncParamString(MIL_ID FuncId,long ParamIndex,MIL_TEXT_PTR ParamVal, long SizeOfData, long ParamAttribute);
#else
MFTYPE32 void MFTYPE MfuncParamIdPointer(MIL_ID FuncId, long ParamIndex, MIL_ID *ParamVal, long ParamHasAttr);
MFTYPE32 void MFTYPE MfuncParamString(MIL_ID FuncId,long ParamIndex,void MPTYPE * ParamVal, long SizeOfData, long ParamAttribute);
#endif //M_MIL_75_COMPATIBLE
MFTYPE32 void MFTYPE MfuncParamFilename(MIL_ID FuncId, long ParamIndex, MIL_TEXT_PTR Filename, long SizeOfFilename, long ParamRole);
MFTYPE32 void MFTYPE MfuncParamValue(MIL_ID FuncId, long ParamIndex, void *ParamValuePtr);
MFTYPE32 long MFTYPE MfuncErrorReport(MIL_ID FuncId,
                                      long ErrorCode,
                                      MIL_TEXT_PTR ErrorString,
                                      MIL_TEXT_PTR ErrorSubString1,
                                      MIL_TEXT_PTR ErrorSubString2,
                                      MIL_TEXT_PTR ErrorSubString3);
MFTYPE32 long MFTYPE MfuncGetError(MIL_ID FunctId,long ErrorType,void MPTYPE *ErrorVarPtr);
MFTYPE32 void MFTYPE MfuncFreeId(MIL_ID FunctionId, MIL_ID ObjectId);
MFTYPE32 void MFTYPE MfuncFree(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncCall(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncInquire(MIL_ID ObjectId,long InquireType, void* UserV020arPtr);

MFTYPE32 long   MFTYPE MfuncPrintMessage(long Mode, MIL_TEXT_PTR Str1,
                                        MIL_TEXT_PTR Str2 ,
                                        MIL_TEXT_PTR Str3 ,
                                        MIL_TEXT_PTR Str4 ,
                                        MIL_TEXT_PTR Str5);
MFTYPE32 long   MFTYPE MfuncExit();
MFTYPE32 void*  MFTYPE MfuncMemory(long Operation, long OperationValue, long OperationFlag, void *DataPointer);
MFTYPE32 MIL_ID MFTYPE MfuncGetNextId(MIL_ID CurrentId);
MFTYPE32 void   MFTYPE MfuncControl(MIL_ID ObjectId,long ControlType, double ControlFlag);

/*The following Mfunc functions are obsolete*/
MFTYPE32 long MFTYPE MfuncStart(MIL_ID FuncId);          /*Replaced by MfuncCall*/
MFTYPE32 void MFTYPE MfuncFreeAndEnd(MIL_ID FuncId);     /*Replaced by MfuncCall*/
MFTYPE32 long MFTYPE MfuncModified(MIL_ID ImageId);      /*Replaced by MbufControl(M_MODIFIED)*/
MFTYPE32 long MFTYPE MfuncParamRegister(MIL_ID FuncId);  /*Parameters must always be registered*/
MFTYPE32 void MFTYPE MfuncIdSetObjectType(MIL_ID FunctionId, MIL_ID ObjectId, long ObjectType); /*Replaced by MfuncInquire and MfuncControl*/
MFTYPE32 long MFTYPE MfuncIdGetObjectType(MIL_ID FunctionId, MIL_ID ObjectId);                  /*Replaced by MfuncInquire*/
MFTYPE32 void MFTYPE MfuncIdSetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId, void *UserPtr);      /*Replaced by MfuncControl*/
MFTYPE32 void* MFTYPE MfuncIdGetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId);                    /*Replaced by MfuncInquire*/


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif
#endif
