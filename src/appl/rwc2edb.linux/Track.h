#ifndef _SYSAL_TRACK_TYPE_0_
#define _SYSAL_TRACK_TYPE_0_

#include "TVectors.h"

//#pragma pack(push)
//#pragma pack(4)

typedef struct
{
	int Field;
	unsigned PointsN;
	TVector *pPoints;
	char *pCorrection;
	TVector Intercept;
	TVector Slope;
	float Sigma;
	float FirstZ;
	float LastZ;
	TVector InterceptErrors;
	TVector SlopeErrors;
	bool Valid;
	} Track;

//#pragma pack(pop)

#endif
