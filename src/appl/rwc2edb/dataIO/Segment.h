#ifndef _SYSAL_SEGMENT_TYPE_0_
#define _SYSAL_SEGMENT_TYPE_0_

#include "TVectors.h"

#pragma pack(push)
#pragma pack(4)

typedef struct 
{
	int Field;
	int PointsN;	
	TVector Intercept;
	TVector Slope;
	float Sigma;
	float FirstZ, LastZ;
	} TS_RawTrack;

typedef struct
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
t_TS_Segment
#endif
{
	int PointsN;
	TS_RawTrack **pTracks;
	TVector Intercept;
	TVector Slope;
	float Sigma;
	float FirstZ, LastZ;
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
	t_TS_Segment *pTopLink, *pBottomLink;
#else
	void *pTopLink, *pBottomLink;
#endif
	} TS_Segment;

#pragma pack(pop)

#endif