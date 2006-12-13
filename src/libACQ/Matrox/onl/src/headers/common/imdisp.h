/*******************************************************************
 *
 * COPYRIGHT (c) 2002 Matrox Electronic Systems Ltd.
 * All Rights Reserved
 *
 *******************************************************************/

#ifndef __IMDISP_H__
#define __IMDISP_H__

/********************************************************************
 *
 * File to be included by applications wishing to use a display
 * on Odyssey (or GenesisPlus).
 *
 ********************************************************************/

/* THIS FILE IS OBSOLETE, YOU DON'T NEED IT ANYMORE */
#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imapi.h"

#define DISPFILE "c:\\display.ini"
#define DISP_MODE_VGA     1
#define DISP_MODE_GENESIS 2

#if defined (IM_DEV_ODYSSEY)
#define DISP_FLAG 0x3000000
#else
#define DISP_FLAG 0x5000000
#endif


typedef struct
{
    long Mode;
    long SizeX;
    long SizeY;
    long Bits;
    long Bands;
    long Address;
    long Pitch;
    long KeyVal;
} _IM_DISP_PARAMS_ST;

static _IM_DISP_PARAMS_ST DispParams;
static long GetDisplayParams(_IM_DISP_PARAMS_ST *DispParams);

void myDevAlloc(long System, long Node, char *ShellFile, long Mode, long *DevPtr);
void myBufChild(long Thread, long Buf, long StartX, long StartY, long SizeX, long SizeY, long *ChildPtr);
void myBufCopy(long Thread, long SrcBuf, long DstBuf, long Control, long OSB);
void myBufCopyVM(long Thread, long SrcBuf, long DstBuf, long SrcControl, long DstControl, long OSB);
void myBufClear(long Thread, long Buf, double Value, long OSB);
long myDispInquire(long Thread, long Disp, long Item, void *ValuePtr);
void myDispControl(long Thread, long Disp, long Control, long Mode);
void myDispAlloc(long Thread, long System, long Display, char *DispFile, long Mode, long *DispPtr);
void myDispFree(long Thread, long Display);


void myDevAlloc(long System, long Node, char *ShellFile, long Mode, long *DevPtr)
{
    GetDisplayParams(&DispParams);

    imDevAlloc(System, Node, ShellFile, Mode, DevPtr);
}


long myDispInquire(long Thread, long Disp, long Item, void *ValuePtr)
{
    long Value = 0;

    Disp = Disp;

/* The function is defined as synchronous */
    imSyncHost(Thread, 0, IM_COMPLETED);

    switch (Item)
    {
    case IM_DISP_MODE:
        if (DispParams.Bands == 1)
            Value = IM_DISP_MONO;
        else if (DispParams.Bands == 3)
            Value = IM_DISP_COLOR;
        else
            Value = IM_NONE;
        break;

    case IM_DISP_RESOLUTION_X:
        Value = DispParams.SizeX;
        break;

    case IM_DISP_RESOLUTION_Y:
        Value = DispParams.SizeY;
        break;

    case IM_DISP_KEY_MODE:
        Value = (DispParams.KeyVal >= 0) ? IM_KEY_IN_RANGE : IM_KEY_ALWAYS;  // No keying currently means dual screen with no overlay
        break;

    case IM_DISP_KEY_LOW:
    case IM_DISP_KEY_HIGH:
        Value = DispParams.KeyVal;
        break;

    case IM_DISP_VGA_MODE:
        Value = (DispParams.KeyVal >= 0) ? IM_SINGLE_SCREEN : IM_DUAL_SCREEN;
        break;

    case IM_DISP_PAN_X:
    case IM_DISP_PAN_Y:
        Value = 0;
        break;

    case IM_DISP_ZOOM:
        Value = 1;
        break;

    case IM_DISP_WRTMSK:
        Value = 0x00ffffff;
        break;
    }

    if (ValuePtr)
        *(long *)ValuePtr = Value;

    return(Value);
}


void myDispControl(long Thread, long Disp, long Control, long Mode)
{
    Thread = Thread;
    Disp = Disp;
    Control = Control;
    Mode = Mode;
}


void myDispAlloc(long Thread, long System, long Display, char *DispFile, long Mode, long *DispPtr)
{
    System = System;
    Display = Display;
    DispFile = DispFile;
    Mode = Mode;

/* The function is defined as synchronous */
    imSyncHost(Thread, 0, IM_COMPLETED);

    if (DispPtr)
        *DispPtr = 1;  // 0 would be an error
}


void myDispFree(long Thread, long Display)
{
    Thread = Thread;
    Display = Display;
}


void myBufChild(long Thread, long Buf, long StartX, long StartY, long SizeX, long SizeY, long *ChildPtr)
{
    long Address[3], Pitch, BytesPerPixel, Type, NumBands;

    if (Buf != IM_DISP && Buf != IM_DISP_COLOR)
    {
        imBufChild(Thread, Buf, StartX, StartY, SizeX, SizeY, ChildPtr);
        return;
    }

    if (SizeX == IM_ALL || (StartX + SizeX ) > DispParams.SizeX)
        SizeX = DispParams.SizeX - StartX;
    if (SizeY == IM_ALL || (StartY + SizeY ) > DispParams.SizeY)
        SizeY = DispParams.SizeY - StartY;

    if (DispParams.Address != 0)
    {
        Address[0] = DispParams.Address;
        Pitch = DispParams.Pitch;
        NumBands = DispParams.Bands;

        if (DispParams.Mode == DISP_MODE_VGA)
        {
            if (NumBands == 1)
            {
                BytesPerPixel = 1;
                Type = IM_UBYTE;
            }
            else
            {
                BytesPerPixel = 4;
                Type = IM_LONG;
            }
            Address[0] += (StartX * BytesPerPixel) + (StartY * Pitch);

            imBufCreate(Thread, SizeX, SizeY, 1, Type, IM_NON_PAGED, (void **)Address, Pitch, ChildPtr);
        }
        else  // Genesis
        {
            BytesPerPixel = 1;
            Address[0] += (StartX * BytesPerPixel) + (StartY * Pitch);
            if (NumBands == 3)
            {
                Address[1] = Address[0] + 0x200000;
                Address[2] = Address[1] + 0x200000;
            }

            imBufCreate(Thread, SizeX, SizeY, NumBands, IM_UBYTE, IM_NON_PAGED, (void **)Address, Pitch, ChildPtr);
        }
    }
    else
        *ChildPtr = 0;
}


void myBufCopy(long Thread, long SrcBuf, long DstBuf, long Control, long OSB)
{
    if ((DstBuf & DISP_FLAG) != DISP_FLAG)    // i.e. Not a special display buffer
    {
        imBufCopy(Thread, SrcBuf, DstBuf, Control, OSB);
        return;
    }

    if (DispParams.Mode == DISP_MODE_VGA)
    {
#if defined(IM_DEV_ODYSSEY)
        if (DispParams.Bands == 3)
            imBufPutField(Thread, SrcBuf, IM_CTL_BYTE_SWAP, IM_ENABLE);

        imBufCopyPCI(Thread, SrcBuf, DstBuf, SrcBuf, OSB);
#else

        if (DispParams.Bands == 1)
        {
            imBufCopyPCI(Thread, SrcBuf, DstBuf, 0, OSB);
        }
        else  // color mode
        {
            long NumBands;

            imBufInquire(Thread, SrcBuf, IM_BUF_NUM_BANDS, &NumBands);

        /* The buffer must have 1 or 3 bands of 8 bits */
            if (NumBands == 1)
            {
                long Bands[4], Buf4Band;

                Bands[0] = Bands[1] = Bands[2] = Bands[3] = SrcBuf;
                imBufCreate(Thread, 0, 0, 4, IM_UBYTE, IM_DEFAULT, (void **)Bands, 0, &Buf4Band);
                imBufCopy(Thread, Buf4Band, DstBuf, 0, OSB);
                imBufFree(Thread, Buf4Band);
            }
            else if (NumBands == 3)
            {
                imBufPutField(Thread, SrcBuf, IM_CTL_PACK, IM_24_TO_32);
                imBufPutField(Thread, SrcBuf, IM_CTL_BYTE_SWAP, IM_ENABLE);
                imBufCopyPCI(Thread, SrcBuf, DstBuf, SrcBuf, OSB);
            }
        }
#endif
    }

    else  // Display is on a Genesis
    {
#if defined(IM_DEV_ODYSSEY)
        imBufCopyPCI(Thread, SrcBuf, DstBuf, 0, OSB);

#else   // Bug on Genesis prevents 1 band buffer being copied to all bands of a 3-band buffer
        if (DispParams.Bands == 1)
        {
            imBufCopyPCI(Thread, SrcBuf, DstBuf, 0, OSB);
        }
        else  // color mode
        {
            long NumBands;

            imBufInquire(Thread, SrcBuf, IM_BUF_NUM_BANDS, &NumBands);

            if (NumBands == 1)
            {
                long Bands[3], Buf3Band;

                Bands[0] = Bands[1] = Bands[2] = SrcBuf;
                imBufCreate(Thread, 0, 0, 3, IM_UBYTE, IM_DEFAULT, (void **)Bands, 0, &Buf3Band);
                imBufCopy(Thread, Buf3Band, DstBuf, 0, OSB);
                imBufFree(Thread, Buf3Band);
            }
            else if (NumBands == 3)
            {
                imBufCopyPCI(Thread, SrcBuf, DstBuf, 0, OSB);
            }
        }
#endif
    }
}


void myBufCopyVM(long Thread, long SrcBuf, long DstBuf, long SrcControl, long DstControl, long OSB)
{
    myBufCopy(Thread, SrcBuf, DstBuf, SrcControl, OSB);
}


void myBufClear(long Thread, long Buf, double Value, long OSB)
{
    long RGBValue = (long) Value;
    long BGRValue = RGBValue;

    if (DispParams.Mode == DISP_MODE_VGA && DispParams.Bands == 3)  // Convert RGB to BGR
        BGRValue = (RGBValue & 0x00ff00) | ((RGBValue >> 16) & 0xff) | ((RGBValue & 0xff) << 16);

#if defined(IM_DEV_ODYSSEY)
    if ((Buf & DISP_FLAG) != DISP_FLAG)    // i.e. Not a special display buffer
    {
        imBufClear(Thread, Buf, Value, OSB);
        return;
    }
    else   // Clear not yet working on Odyssey for created buffer
    {
        long ProcBuf;

        imBufClone(Thread, Buf, IM_PROC, &ProcBuf);
        imBufClear(Thread, ProcBuf, BGRValue, 0);
        imBufCopy(Thread, ProcBuf, Buf, 0, OSB);
        imBufFree(Thread, ProcBuf);
    }
#else
        imBufClear(Thread, Buf, BGRValue, OSB);
#endif
}

#define MAX_LINE 120
static long GetDisplayParams(_IM_DISP_PARAMS_ST *DispParams)
{
    char Line[120];
    FILE *fp;
    long status = 0;

    memset(DispParams, sizeof(_IM_DISP_PARAMS_ST), 0);
    DispParams->KeyVal = -1; // < 0 means keying disabled

    fp = fopen(DISPFILE, "r");
    if (fp != NULL)
    {
        while (fgets(Line, MAX_LINE-1, fp))
        {
            if (strstr(Line, "Mode ="))
            {
                if (strstr(Line, "VGA"))
                    DispParams->Mode = DISP_MODE_VGA;
                else
                    DispParams->Mode = DISP_MODE_GENESIS;
            }
            else if (strstr(Line, "SizeX ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->SizeX);
            else if (strstr(Line, "SizeY ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->SizeY);
            else if (strstr(Line, "Bits ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->Bits);
            else if (strstr(Line, "Bands ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->Bands);
            else if (strstr(Line, "Address ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->Address);
            else if (strstr(Line, "Pitch ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->Pitch);
            else if (strstr(Line, "KeyVal ="))
                sscanf(strstr(Line, "=")+1, "%i", &DispParams->KeyVal);
        }

        fclose(fp);
    }
//    else
//        printf("Failed to open file %s\n", DISPFILE);

    if (DispParams->Address == 0 || DispParams->SizeX == 0 || DispParams->SizeY == 0 || DispParams->Pitch == 0)
    {
#if defined(IM_DEV_ODYSSEY)
        printf("*** The Odyssey display is not properly configured\n");
#else
        printf("*** The display is not properly configured\n");
#endif
        printf("*** Please configure it before running this application\n");
    }
    else
    {
        status = 1;
    }

    return status;
}

#define imDevAlloc(System, Node, ShellFile, Mode, DevPtr) myDevAlloc(System, Node, ShellFile, Mode, DevPtr)
#define imBufChild(Thread, Buf, StartX, StartY, SizeX, SizeY, ChildPtr) myBufChild(Thread, Buf, StartX, StartY, SizeX, SizeY, ChildPtr)
#define imBufCopy(Thread, SrcBuf, DstBuf, Control, OSB) myBufCopy(Thread, SrcBuf, DstBuf, Control, OSB)
#define imBufCopyVM(Thread, SrcBuf, DstBuf, SrcControl, DstControl, OSB) myBufCopyVM(Thread, SrcBuf, DstBuf, SrcControl, DstControl, OSB)
#define imBufClear(Thread, Buf, Value, OSB) myBufClear(Thread, Buf, Value, OSB)
#define imDispInquire(Thread, Disp, Item, ValuePtr) myDispInquire(Thread, Disp, Item, ValuePtr)
#define imDispControl(Thread, Disp, Control, Mode) myDispControl(Thread, Disp, Control, Mode)
#define imDispAlloc(Thread, System, Display, DispFile, Mode, DispPtr) myDispAlloc(Thread, System, Display, DispFile, Mode, DispPtr)
#define imDispFree(Thread, Display) myDispFree(Thread, Display)

#endif /* #if 0 */

#endif /* __IMDISP_H__ */
