#ifndef _BSCAN_DATA_STRUCTURES_
#define _BSCAN_DATA_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "id.h"
#include "Cluster.h"
#include "Track.h"
#include "FlatTrack.h"
#include "TVectors.h"

#define BULK_TRACK_SECTION 				1
#define BULK_FIELD_SECTION				3
#define BULK_CLUSTER_SECTION 			4
#define BULK_IMAGE_SECTION 				5
#define BULK_GRAIN_SECTION				6
#define BULK_IMAGEPRED_SECTION 			7
#define BULK_CLUSTERPRED_SECTION 		8
#define BULK_BLACKTRACK_SECTION 		10	
#define BULK_BLACKSTRIP_SECTION 		11	

#define BULK_DATA_BYTES					0x20
#define BULK_HEADER_BYTES				0x40

#define BULK_OLD_HEADER_TYPE			0X10
#define BULK_OLD2_HEADER_TYPE			0X20
#define BULK_HEADER_TYPE				0X40

#define BULK_COMPRESSION_NULL			0
#define BULK_COMPRESSION_METHOD		0x100

#define BULK_COMPRESSION_GRAINS_DIFF	(BULK_COMPRESSION_METHOD + 1)

typedef struct 
{
	IO_Header Type;
	Identifier ID;
	int Frames;
	float YPos, ZPos;
	struct
	{
		float TopExt;
		float TopInt;
   		float BottomInt;
		float BottomExt;
		} RelevantZs;
	float ImageMat[2][2];
	int MaxClusters;
	float YCenter, ZCenter;
	} BULK_ClustersDumpHeader;

typedef struct 
{
	IO_Header Type;
	Identifier ID;
  	int Width, Height;
	int Frames;
	float YPos, ZPos;
	struct
	{
		float TopExt;
		float TopInt;
   		float BottomInt;
		float BottomExt;
		} RelevantZs;
	float ImageMat[2][2];
	float YCenter, ZCenter;
	} BULK_PhotoSeqHeader;

typedef struct 
{
	IO_Header Type;
	Identifier ID;
	float YPos, ZPos;
	float YSlope, ZSlope;
	float SideSlopeTolerance;
	float GlobalSlopeTolerance;
	float GoodSlopeTolerance;
	int FitCorrectionDataSize;
	int TCount[2];
	int BTCount[2];	
	int CodingMode;
	int FCount;
	struct
	{
		float TopExt;
		float TopInt;
   		float BottomInt;
		float BottomExt;
		} RelevantZs;
	} BULK_TracksHeader;

typedef struct
{
	float Z;
	BYTE *pImage;
	} BULK_PhotoInfo;

typedef struct
{
	float Z;
	int Count;
	Cluster *pClusters;
	} BULK_ClustersPlaneInfo;

typedef struct
{
	BULK_PhotoSeqHeader Hdr;
	BULK_PhotoInfo *pImages;
	} IO_BULK_PhotoSeq;

typedef struct
{
	BULK_ClustersDumpHeader Hdr;
	BULK_ClustersPlaneInfo *pPlanes;
	} IO_BULK_ClustersDump;

typedef struct
{
	BULK_TracksHeader Hdr;
	BYTE *FieldHistory;
	Track *pTracks[2];
	FlatTrack *pFlatTracks[2];
	} IO_BULK_Tracks;

typedef struct
{
	unsigned Size;
	} IO_BULK_Compressed_GrainSection_Header;

typedef struct
{
	IO_BULK_Compressed_GrainSection_Header Hdr;
	float TransversePrecision;
	float DepthPrecision;
	} IO_BULK_DiffCompression_Header;

typedef struct
{
	struct t_Coding
	{
		float Base;
		int Polarization;
		unsigned char Bits;
		} XCoding, YCoding, ZCoding;
	} IO_BULK_Compressed_GrainSection_TrackHeader;

#pragma pack(pop)

#endif