#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H


#include <stdarg.h>
#include <string>
#include "PAVProtocol.h"
#include "tinyxml.h"
#include "tinystr.h"

#define NULL_TRM ((void*)NULL)
//for x64 compatible endings

class MessageManager: PAVProtocol{
public:
	MessageManager():m_messId(0), m_xmlDoc("mess")
	{};

	void Clean();

	void CreateMessage();

	void AddStopNode();

	void AddStartNode(const char *lib_, const char *path_, const char *comment_);

	void AddMarksNode(int brick_, int plate_, const char *mapext_);

	void AddGetModuleParamsNode(const char *module_, ...);

	void AddSetModuleParamsNode(const char *module_, ...);

	void AddGetPathParamsNode(const char *lib_, const char *path_, ...);

	void AddSetPathParamsNode(const char *lib_, const char *path_, ...);

	void FillBuff();

	void FillBuffOuter(uint8* buf_, uint32 size_){
		printf("%s\n", buf_);//ToDo: verbisity
		WriteMessage(m_messBuf, buf_, size_);
	};

	uint8 * GetBuf(){return &m_messBuf[0];};
	uint32 GetBufSize(){return m_messBuf.size();};

private:
	std::string m_xml;
	std::vector<uint8> m_messBuf;
	uint32 m_messId;
	TiXmlDocument m_xmlDoc;
	TiXmlElement *m_xmlDocAddPtr;
};

#endif
