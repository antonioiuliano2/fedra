#ifndef _VERTIGOSCAN_DATA_STRUCTURES_
#define _VERTIGOSCAN_DATA_STRUCTURES_

#include "Config.h"
#include "Connection.h"

#pragma pack(push)
#pragma pack(1)

#include "id.h"
#include "TVectors.h"
#include "VSRawData.h"
#include "Config.h"

#define VS_CATALOG_SECTION				1
#define VS_VIEW_SECTION					2
#define VS_TRACK_SECTION				3
#define VS_GRAIN_SECTION 				4
#define VS_CONFIG_SECTION				5
#define VS_FRAGMENT_SECTION				6

#define VS_DATA_BYTES					0x30
#define VS_HEADER_BYTES					0x60

#define VS_OLD_HEADER_TYPE				0x701
#define VS_HEADER_TYPE					0x702

#define VS_COMPRESSION_NULL				0
#define VS_COMPRESSION_METHOD			0x100

#define VS_COMPRESSION_GRAIN_SUPPRESSION	(VS_COMPRESSION_METHOD + 2)

typedef struct
{
	char ClassName[SYSAL_MAX_CONN_NAME_LEN];
	SySalConfig Config;
	} VS_Config;

typedef struct
{
	int Index;
	int StartView;
	int CountOfViews;
	int FitCorrectionDataSize;
	int CodingMode;
	VS_View *pViews;
	} VS_Fragment;

typedef struct
{
	int Index;
	int StartView;
	int CountOfViews;
	int FitCorrectionDataSize;
	int CodingMode;
	VS_View2 *pViews;
	} VS_Fragment2;

typedef struct
{
	struct 
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
		   t_Hdr
#endif
	{
		IO_Header Type;
		Identifier ID;
		} Hdr;
	struct 
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
		   t_Area
#endif
	{
		float XMin, XMax, YMin, YMax;
		float XStep, YStep;
		int XViews, YViews;
		int Fragments;
		} Area;
	struct 
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
		   t_Config
#endif
	{
		int CountOfConfigs;
		VS_Config *pConfigs;
		} Config;
	int *pFragmentIndices;
	unsigned char Reserved[256];
	} IO_VS_Catalog;

typedef struct
{
	struct 
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
		   t_Hdr
#endif
	{
		IO_Header Type;
		Identifier ID;
		} Hdr;
	VS_Fragment Fragment;
	unsigned char Reserved[256];
	} IO_VS_Fragment;

typedef struct
{
	struct 
#ifdef USE_MIDL_INCOMPATIBLE_STRUCTS
		   t_Hdr
#endif
	{
		IO_Header Type;
		Identifier ID;
		} Hdr;
	VS_Fragment2 Fragment;
	unsigned char Reserved[256];
	} IO_VS_Fragment2;

#pragma pack(pop)

#endif