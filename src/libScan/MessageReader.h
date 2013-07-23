#ifndef _MESSAGEREADER_H_
#define _MESSAGEREADER_H_

#include "PAVProtocol.h"
#include "tinyxml.h"
#include <map>

class MessageReader: PAVProtocol{
public:
	MessageReader():m_stoped(false)
	{};

	bool Load(uint8 * buf_, uint32 size_);

	void Process();

	void ProcessStopNode(TiXmlElement *el_);

	void ProcessStartNode(TiXmlElement *el_);

	void ProcessMarksNode(TiXmlElement *el_);

	void ProcessSMPNode(TiXmlElement *el_);

	void ProcessSPPNode(TiXmlElement *el_);

	void ProcessGMPNode(TiXmlElement *el_);

	void ProcessGPPNode(TiXmlElement *el_);

	void ProcessErrorNode(TiXmlElement *err_);

	std::string GetErrorStr(int err_);

	bool GetJobFinished(){return m_stoped;};

	std::string GetValueForKey(const char* key_){return m_keysMap[key_];};
	std::string GetValueForKey(std::string& key_){return m_keysMap[key_];};

private:
	TiXmlDocument m_xmlDoc;
	TiXmlElement *m_messNode;
	bool m_stoped;
	int m_messId;
	std::map<std::string, std::string> m_keysMap;
};


#endif
