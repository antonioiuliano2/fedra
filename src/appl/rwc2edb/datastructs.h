#ifndef _SYSAL_IO_DATA_STRUCTURES_
#define _SYSAL_IO_DATA_STRUCTURES_

#pragma pack(push)
#pragma pack(1)

#include "bscan_ds.h"
#include "scscan_ds.h"
#include "bspred_ds.h"
#include "tscan_ds.h"
#include "dbpred_ds.h"
#include "mmap_ds.h"
#include "vscan_ds.h"

typedef union
{	
	IO_Header Hdr;
	IO_HeaderId HdrId;
	IO_BULK_Tracks BTks;
	IO_BULK_PhotoSeq BPhSeq;
	IO_BULK_ClustersDump BCDmp;
	IO_CS_SS_Tracks STks;
	IO_BS_Batch BSBatch;
	IO_TS_Reconstruction TSRec;
	IO_DBPred_Tracks DBPTks;
	IO_MarkMap MkMap;
	IO_VS_Fragment VSFrag;
	IO_VS_Catalog VSCat;
	} IO_Data;

#define IODATA_EXT_ERROR_OK				0x00000000L
#define IODATA_EXT_ERROR_OPEN			0x01000000L
#define IODATA_EXT_ERROR_WRITE			0x02000000L
#define IODATA_EXT_ERROR_READ			0x03000000L
#define IODATA_EXT_ERROR_CLOSE			0x04000000L
#define IODATA_EXT_ERROR_CANTRUN		0x05000000L
#define IODATA_EXT_ERROR_UNKFORMAT		0x06000000L
#define IODATA_EXT_ERROR_DISABLED		0x07000000L
#define IODATA_EXT_ERROR_ABORTED		0x08000000L
#define IODATA_EXT_ERROR_UNKCOMPRESS	0x09000000L
#define IODATA_EXT_ERROR_COMPRESS		0x0A000000L
#define IODATA_EXT_ERROR_DECOMPRESS		0x0B000000L
#define IODATA_EXT_ERROR_UNKNOWN		0xFF000000L

#define MAKE_IO_EXT_ERROR(x, h) (x | ((unsigned)h.InfoType << 16) | ((unsigned)h.HeaderFormat))

#pragma pack(pop)

#endif
