#ifndef _SYSAL_CLUSTER_TYPE_0_
#define _SYSAL_CLUSTER_TYPE_0_

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	float X;
	float Y;
	int Area;
	struct
	{
   		double IXX;
		double IXY;
		double IYY;
		} Inertia;
	float GrayLevel;
	} Cluster;

#pragma pack(pop)

#endif