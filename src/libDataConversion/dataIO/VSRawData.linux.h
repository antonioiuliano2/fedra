#ifndef _VERTIGOSCAN_RAW_DATA_
#define _VERTIGOSCAN_RAW_DATA_

#include "Track.linux.h"
#include "Track2.linux.h"
#include "TVectors.linux.h"

//#pragma pack(push)
//#pragma pack(1)

typedef struct
{
	int TileX, TileY;
	float X[2], Y[2];
	float MapX[2], MapY[2];
	float ImageMat[2][2][2];
	struct t_RelevantZs
	{
		float TopExt;
		float TopInt;
		float BottomInt;
		float BottomExt;
		} RelevantZs;
	struct t_Layers
	{
		int Count;
		float *pZs;
		} Layers[2];
	unsigned char Status[2];
	int TCount[2];
	Track *pTracks[2];
	} VS_View;

typedef struct
{
	int TileX, TileY;
	float X[2], Y[2];
	float MapX[2], MapY[2];
	float ImageMat[2][2][2];
	struct t_RelevantZs2
	{
		float TopExt;
		float TopInt;
		float BottomInt;
		float BottomExt;
		} RelevantZs;
	struct t_LayerInfo
	{
		int Clusters;
		float Z;
		};
	struct t_Layers2
	{
		int Count;
		struct t_LayerInfo *pLayerInfo;
		} Layers[2];
	unsigned char Status[2];
	int TCount[2];
	Track2 *pTracks[2];
	} VS_View2;

/*#define VSSCAN_OK						0x00
#define VSSCAN_NOTOPFOCUS				0x01
#define VSSCAN_NOBOTTOMFOCUS			0x02
#define VSSCAN_ZLIMITER					0x04
#define VSSCAN_XLIMITER					0x08
#define VSSCAN_YLIMITER					0x10
#define VSSCAN_TERMINATED				0x80
#define VSSCAN_NOTSCANNED				0xFF
*/
const int VSSCAN_OK = 0x00;
const int VSSCAN_NOTOPFOCUS = 0x01;
const int VSSCAN_NOBOTTOMFOCUS = 0x02;
const int VSSCAN_ZLIMITER = 0x04;
const int VSSCAN_XLIMITER = 0x08;
const int VSSCAN_YLIMITER = 0x10;
const int VSSCAN_TERMINATED = 0x80;
const int VSSCAN_NOTSCANNED = 0xFF;

//#pragma pack(pop)

#endif
