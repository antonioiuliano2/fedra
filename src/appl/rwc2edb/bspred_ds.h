#ifndef _BULK_CSSS_PREDICTION_BATCH_STRUCTURES_
#define _BULK_CSSS_PREDICTION_BATCH_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "id.h"

#define BS_PRED_SECTION 				12
#define BS_TRACK_SECTION 				1
#define BS_IMAGEPRED_SECTION 			7
#define BS_CLUSTERPRED_SECTION 			8


#define BS_DATA_BYTES					0x20
#define BS_HEADER_BYTES					0x40

#define BS_HEADER_TYPE					0X01

typedef struct 
{
	Identifier Id;
	Identifier MapId;
	struct
	{
		float X, Y;
		} Pos, Slope;
	float PosTol;
	float SideSlopeTol, GlobalSlopeTol, GoodSlopeTol;
	} BS_Track;


typedef struct
{
	Identifier Id;
	Identifier MapId;
	struct
	{
		float X, Y;
		} Pos;
	float StartZ, EndZ;
	int Frames;
	int MaxClusters;
	int Threshold;
	} BS_ClusterDump;


typedef struct 
{
	Identifier Id;
	Identifier MapId;
	struct
	{
		float X, Y;
		} Pos;
	float StartZ, EndZ;
	int Frames;
	int XSize, YSize;
	} BS_PhotoSequence;


typedef struct
{
	IO_Header Hdr;
	int CountOfTracks;
	int CountOfClusterDumps;
	int CountOfPhotoSequences;
	int Reserved[8];
	BS_Track *pTracks;
	BS_ClusterDump *pClusterDumps;
	BS_PhotoSequence *pPhotoSequences;
	void *pReserved[8];
	} IO_BS_Batch;

#pragma pack(pop)

#endif
