#ifndef _DB_PREDICTIONS_DATA_STRUCTURES_
#define _DB_PREDICTIONS_DATA_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "id.h"

#define DBP_TRACK_SECTION 				1
#define DBP_EVENT_SECTION 				2

#define DBP_DATA_BYTES					0x30
#define DBP_HEADER_BYTES				0x50

#define DBP_HEADER_TYPE					0x1
#define DBP_MAX_ADDITIONAL_DATA_SIZE	32
#define DBP_ADDITIONAL_DATA_NAME_LEN	16

#define DBP_TYPE_INT					0
#define DBP_TYPE_SHORT					1
#define DBP_TYPE_CHAR					2
#define DBP_TYPE_FLOAT					3
#define DBP_TYPE_DOUBLE					4

typedef struct 
{
	IO_Header Type;
	Identifier ID;
	int Count;
	int AdditionalDataSize;
	struct 
	{
		int Type;
		char Name[DBP_ADDITIONAL_DATA_NAME_LEN];
		} AdditionalData[DBP_MAX_ADDITIONAL_DATA_SIZE];
	} DBPred_TracksHeader;

typedef struct 
{
	Identifier ID;
	union
	{		
		int Int;
		short Short;
		char Char;
		float Float;
		double Double;
		boolean Bool;
		} AdditionalData[DBP_MAX_ADDITIONAL_DATA_SIZE];
	} DBPred_Track;

typedef struct
{
	DBPred_TracksHeader Hdr;	
	DBPred_Track *pTracks;
	} IO_DBPred_Tracks;

#pragma pack(pop)

#endif