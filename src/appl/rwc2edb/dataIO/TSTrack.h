#ifndef _SYSAL_TOTALSCAN_TRACKS_
#define _SYSAL_TOTALSCAN_TRACKS_

#include "TVectors.h"
#include "Segment.h"

#pragma pack(push)
#pragma pack(4)

#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
struct t_TS_Vertex;
#endif

#define TSTRACK_FLAG_NULL						0
#define TSTRACK_FLAG_PASSING_THROUGH			1
#define TSTRACK_FLAG_RECOVERED_BY_TOPOLOGY		2

typedef struct 
{
	int Count;
	TS_Segment *pTopEnd, *pBottomEnd;
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
	t_TS_Vertex *pTopVertex, *pBottomVertex;	
#else
	void *pTopVertex, *pBottomVertex;
#endif
	unsigned Flags;
	TVector Intercept;
	TVector Slope;
	float Sigma;
	float FirstZ, LastZ;
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
	union
	{
		int Reserved[8];
		struct
		{
			struct
			{
				float X, Y;
				} TopIntercept, TopSlope, BottomIntercept, BottomSlope;
			};
		};
#else
	int Reserved[8];
#endif
	} TS_Track;

typedef struct t_TS_Vertex
{
	TVector Pos;
	float AvgDistance;
	int Count;
	struct t_VertexTSTrack
	{				
		TS_Track *pTSTrack;
		boolean IsDownstream;
		int Reserved[4];
		} *pTSTracks;
	int Reserved[4];
	} TS_Vertex;

#pragma pack(pop)

#endif