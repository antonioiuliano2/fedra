#ifndef _MARK_MAP_DATA_STRUCTURES_
#define _MARK_MAP_DATA_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "id.h"
#include "MarkMap.h"

#define MAP_FULL_SECTION				0xA

#define MAP_DATA_BYTES					0x60
#define MAP_HEADER_BYTES				0x60

#define MAP_MARK_HEADER_TYPE			0x0001

typedef struct 
{
	IO_HeaderId Hdr;
	MarkMap Map;
	} IO_MarkMap;

#pragma pack(pop)

#endif