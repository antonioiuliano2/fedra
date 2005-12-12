/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Dec 09 16:31:13 2002
 */
/* Compiler settings for D:\SySal2\SySalDataIO\SySalDataIO.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ISySalObject = {0xC022EEAD,0x748A,0x11D3,{0xA4,0x7B,0xE8,0x9C,0x0B,0xCE,0x97,0x20}};


const IID IID_ISySalDataIO = {0x4B47E8CD,0x894C,0x11D3,{0xA4,0x7C,0x9F,0x37,0x35,0x22,0x60,0x36}};


const IID IID_ISySalDataIO2 = {0x5892a1f5,0x5dd9,0x4fe3,{0xa0,0x24,0xd8,0xb6,0x24,0x91,0x7c,0x1d}};


const IID LIBID_SYSALDATAIOLib = {0x4B47E8C0,0x894C,0x11D3,{0xA4,0x7C,0x9F,0x37,0x35,0x22,0x60,0x36}};


const CLSID CLSID_SySalDataIO = {0x4B47E8CE,0x894C,0x11D3,{0xA4,0x7C,0x9F,0x37,0x35,0x22,0x60,0x36}};


#ifdef __cplusplus
}
#endif

