#ifndef _SYSAL_TRACK_TYPE_1_
#define _SYSAL_TRACK_TYPE_1_

#include "tvectors.h"

#pragma pack(push)
#pragma pack(4)

typedef struct
{
	float X;
	float Y;
	float Z;
	struct
	{
  		float X;
		float Y;		
		} Dir;
	float Width;
	float Length;
	} FlatStrip;

typedef struct
{
	int Field;
	int StripsN;
	FlatStrip *pStrips;
	BYTE *pCorrection;
	TVector Intercept;
	TVector Slope;
	float HorizSigma;
	float VertSigma;
	float ZBase;
	float LastL;
	TVector InterceptErrors;
	TVector SlopeErrors;
	boolean Valid;
	} FlatTrack;

#pragma pack(pop)

#endif