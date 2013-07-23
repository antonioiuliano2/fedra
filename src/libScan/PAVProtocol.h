#ifndef _PAVPROTOCOL_H_
#define _PAVPROTOCOL_H_

#include "PAVProtocolDef.h"
#include <vector>

#define PAVCLIENTVERSION "1.0"
class PAVProtocol{
public:

	//enum ParseRes {ParseRet, ParseNoRet, ParseErr};

	PAVProtocol();
	virtual ~PAVProtocol(){};
	
	uint32 Crc32(uint32 len_,const uint8* buf_);


	virtual bool WriteMessage(std::vector<uint8> &buf_, const void* srcbuf_, uint32 srcsize_);
	virtual bool WriteMessageRaw(std::vector<uint8> &buf_, const void* srcbuf_, uint32 srcsize_);
	virtual bool ReadMessage(uint8* buf_, uint32 size_, std::vector<uint8> &dstbuf_);
	virtual bool ReadMessageRaw(uint8* buf_, uint32 size_, std::vector<uint8> &dstbuf_);

protected:
	uint32 m_headerSize;
};


#endif



