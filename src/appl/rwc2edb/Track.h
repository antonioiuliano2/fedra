#ifndef _SYSAL_TRACK_TYPE_0_
#define _SYSAL_TRACK_TYPE_0_

#include "tvectors.h"

#pragma pack(push)
#pragma pack(4)

typedef struct
{
	int Field;
	int PointsN;
	TVector *pPoints;
	BYTE *pCorrection;
	TVector Intercept;
	TVector Slope;
	float Sigma;
	float FirstZ;
	float LastZ;
	TVector InterceptErrors;
	TVector SlopeErrors;
	boolean Valid;
	} Track;

#pragma pack(pop)

#endif