#ifndef _TOTALSCAN_DATA_STRUCTURES_
#define _TOTALSCAN_DATA_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "id.h"
#include "Segment.h"
#include "TSTrack.h"
#include "TVectors.h"

#define TS_LAYER_SECTION 				1
#define TS_RAW_SECTION 					2
#define TS_SEGMENT_SECTION				3
#define TS_TRACK_SECTION 				4
#define TS_VERTEX_SECTION 				5
#define TS_FULL_SECTION 				9

#define TS_DATA_BYTES					0x20
#define TS_HEADER_BYTES					0x40

#define TS_HEADER_TYPE					0x200
#define TS_OLD_HEADER_TYPE				0x100

typedef struct 
{
	IO_Header Type;
	Identifier ID;
	TVector Pos;
	int Reserved[32];
	int TopSheet;
	int SheetsCount;
	int TSTracksCount;
	int VerticesCount;
	int MaxTracksInSegment;
	TVector RefCenter;
	} TS_TracksHeader;

typedef struct 
{
	int SheetId;
	struct
	{
		float TopExt;
		float TopInt;
		float BottomInt;
		float BottomExt;
		} RelevantZs;
	float RefDepth;
	struct t_GlobalSideAlign
	{
		float DSlope[2];
		float DShrink[2];
		} GlobalSideAlign[2];
		
	struct t_LocalSideAlign
	{
		struct t_PosAlign
		{
			double M[2][2];
			double D[3];
			double Reserved[4];
			} PosAlign;
		struct t_SlopeAlign
		{
			double DSlope[2];
			double DShrink[2];
			double Reserved[4];
			} SlopeAlign;
		} LocalSideAlign[4];

	int RTCount[2];
	int SCount[2];
	int Reserved[4];
	TS_RawTrack *pRawTracks[2];
	TS_Segment *pSegments[2];
	void *pReserved[4];
	} TS_LayerInfo;

typedef struct
{
	TS_TracksHeader Hdr;
	TS_LayerInfo *pLayers;
	TS_Track *pTracks;
	TS_Vertex *pVertices;
	} IO_TS_Reconstruction;

#pragma pack(pop)

#endif