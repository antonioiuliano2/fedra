#ifndef _PAVPROTOCOLDEF_H_
#define _PAVPROTOCOLDEF_H_

/****************************
* messge stucture:
* [msg size][data crc32][protocol version][data]
* msg size = size of (data & crc), 4 bytes
* data crc32 is obtained using algorithm described below, 4 bytes
* protocol version=PAVPROT_VERSION, 2b
*
* data structure depends on messge type:
* [msg type=PAVPROT_MSGXML, 1b][xml string size, incl 0 char if present, 4b][string]
* [msg type=PAVPROT_MSGRAW, 1b][raw data size,4b][raw data]
*
*****************************/
/*
	CRC-32 description
	Name : "CRC-32"
	Width : 32
	Poly : 04C11DB7
	Init : FFFFFFFF
	RefIn : True
	RefOut : True
	XorOut : FFFFFFFF
	Check : CBF43926
*/

#ifndef _MSC_VER
#include "inttypes.h"
#endif


#define PAVPROT_VERSION 1

#define PAVPROT_MSGXML 1
#define PAVPROT_MSGRAW 2

#define PAVPROT_OK 0
#define PAVPROT_ERROR 1
#define PAVPROT_BUSY 2
#define PAVPROT_CANCELED 3
#define PAVPROT_FINISHED 4
#define PAVPROT_SEQVIOLATION 5
#define PAVPROT_XMLERROR 6
#define PAVPROT_MSGERROR 7

#define PAVPROT_CLIENTVERSION 0.2
#define PAVPROT_MSGRAW 2

#define PAVPROT_NODE_ROOT "PAVProtocol"
#define PAVPROT_NODE_SMP "SetModuleParams"
#define PAVPROT_NODE_GMP "GetModuleParams"
#define PAVPROT_NODE_SPP "SetPathParams"
#define PAVPROT_NODE_GPP "GetPathParams"
#define PAVPROT_NODE_MARKS "FindMarks"
#define PAVPROT_NODE_START "Start"
#define PAVPROT_NODE_STOP "Stop"
#define PAVPROT_NODE_MESS "Message"

//ToDo: Remove all the folowing(defined in last PAVDef.h): 
#ifdef __CINT__
typedef Char_t			int8;
typedef Short_t			int16;
typedef Int_t				int32;
typedef Long64_t		int64;
typedef UChar_t			uint8;
typedef UShort_t		uint16;
typedef UInt_t			uint32;
typedef ULong64_t		uint64;
#else
#ifdef _MSC_VER
typedef		__int8		int8;
typedef		__int16		int16;
typedef		__int32		int32;
typedef		__int64		int64;
//typedef __int128	int128;
typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;
//typedef unsigned int128 uint128;
#else
typedef		int8_t		int8;
typedef		int16_t		int16;
typedef		int32_t		int32;
typedef		int64_t		int64;
//typedef __int128	int128;
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef uint64_t	uint64;
//typedef unsigned int128 uint128;
#endif
#endif

typedef int64 Int64;

typedef float  float32;
typedef double float64;

#endif

