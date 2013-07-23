#include "MessageReader.h"

bool MessageReader::Load(uint8 * buf_, uint32 size_){
	m_keysMap.clear();
	std::vector<uint8> xmlstr;
	m_xmlDoc.Clear();
	m_messNode = 0;
	bool res = ReadMessage(buf_, size_, xmlstr);
	if(!res){
		printf("Received message in incorrect format.\n");
		return res;
	};
	if(xmlstr[xmlstr.size()-1] != 0){
		xmlstr.resize(xmlstr.size()+1);
		xmlstr[xmlstr.size()-1] = 0;
	};

	/*debug substitution*/
	//const char * debugxml = "<PAVControl><Message id=\"0\"/><StopR/><SetModuleParamsR module=\"camera\"><Error code=\"1\">A</Error></SetModuleParamsR><GetModuleParamsR module=\"camera\"><Error code=\"2\"/><Clip val=\"false\"/><ClipSize val=\"0 0 1280 1024\"/><DCFFileName val=\"..\\mc1310_8channels.dcf\"/><Fps val=\"110\"/><FpsMax val=\"360\"/><FpsMin val=\"11\"/><ImageSpec val=\"None\"/></GetModuleParamsR><GetModuleParamsR module=\"stage\"><Error code=\"3\"/><AccCoef val=\"90000 90000 200\"/><CommandRate val=\"50\"/><Eps val=\"1.5 1.5 0.05\"/><MoveToTries val=\"5\"/><PU val=\"0.25 0.25 0.004\"/><QueryPosTries val=\"3\"/><VelCoef val=\"295.5 295.5 4.73\"/><VelMax val=\"37233 37233 40\"/><VelMin val=\"295.5 295.5 4.73\"/><Velocity val=\"23640 23640 14.190001\"/><ZeroTries val=\"5\"/></GetModuleParamsR><GetModuleParamsR module=\"proc\"><Error code=\"4\"/><CoordFile val=\"coord.txt\"/><CoordTemplate val=\"&lt;id&gt;:    |&lt;rn&gt;|       &lt;x:.3&gt;       &lt;y:.3&gt;&lt;z:.3&gt;\"/><EventDirectory val=\"C:/Andrey/svn-cur/tmp/\"/><ImageQuality val=\"99\"/><ImageTemplate val=\"&lt;ox:03&gt;_&lt;oy:03&gt;\img&lt;oz:03&gt;.jpg\"/><MaxProcThreads val=\"1\"/></GetModuleParamsR><GetModuleParamsR module=\"guide\"><Error code=\"5\"/><PathsDir val=\"..\\..\\..\\bin\"/></GetModuleParamsR><SetPathParamsR lib=\"PAVPathLib-dbg.pth\" path=\"NS_SPIRAL1S\"><Error code=\"6\"/></SetPathParamsR><GetPathParamsR lib=\"PAVPathLib-dbg.pth\" path=\"NS_SPIRAL1S\"><Error code=\"0\">server sais hello</Error><count val=\"3 3 10\"/><delta val=\"100 100 1\"/><offset val=\"0.000000 0.000000 0.000000\"/><order val=\"ZXY\"/></GetPathParamsR><StartR error=\"4\"/></PAVControl>";
	m_xmlDoc.Parse((char*)&xmlstr[0]);/*(debugxml);*/


	if(m_xmlDoc.Error()){
		printf("Message Parsing failed. Col %d, row %d, in:\n%s\n Description: %s\n",
			m_xmlDoc.ErrorCol(), m_xmlDoc.ErrorRow(), &xmlstr[0], m_xmlDoc.ErrorDesc());
		return false;
	};
	TiXmlElement *rootnode = m_xmlDoc.FirstChildElement();
	if(!rootnode){
		printf("Root node not found.\n");
		return false;
	};
	if(rootnode->ValueStr() != PAVPROT_NODE_ROOT){
		printf("Wrong root node, got \"%s\".\n", rootnode->ValueStr().c_str());
		return false;
	};
	m_messNode = rootnode->FirstChildElement();
	if(!m_messNode){
		printf("Message node not found.\n");
		return false;
	};
	m_messNode->ValueStr();
	if(m_messNode->ValueStr() != PAVPROT_NODE_MESS){
		printf("Wrong message node, got \"%s\".\n", m_messNode->ValueStr().c_str());
		return false;
	};

	int errid;
	if(m_messNode->QueryIntAttribute("error", &errid) == TIXML_SUCCESS){
		printf("Server response of an error, code=%d (%s).\n", errid, GetErrorStr(errid).c_str());
		const char *errstr = m_messNode->GetText();
		if(errstr)
			printf("\tStatus:\"%s\"\n", errstr);
	}else{
		const char *msg = m_messNode->GetText();
		if(msg)
			printf(">>Got message from server: \"%s\"\n", msg);
	};
	if(m_messNode->QueryIntAttribute("id", &m_messId) != TIXML_SUCCESS)
		m_messId = -1;
	//we are not taking server errors or id-absence as errors, and will try to process xml further
	return res;
};

void MessageReader::Process(){
	m_stoped = false;
	if(m_messNode){
		printf("\n_____________________________\n PAVICOM responce id=%d\n_____________________________\n", m_messId);
		TiXmlElement *el;
		std::string stopr = PAVPROT_NODE_STOP; stopr+="R";
		std::string startr = PAVPROT_NODE_START; startr+="R";
		std::string marksr = PAVPROT_NODE_MARKS; marksr+="R";
		std::string smpr = PAVPROT_NODE_SMP; smpr+="R";
		std::string gmpr = PAVPROT_NODE_GMP; gmpr+="R";
		std::string sppr = PAVPROT_NODE_SPP; sppr+="R";
		std::string gppr = PAVPROT_NODE_GPP; gppr+="R";
		el = m_messNode;
		while((el = el->NextSiblingElement())){
			if(el->ValueStr() == stopr){
				ProcessStopNode(el);
			}else if(el->ValueStr() == startr){
				ProcessStartNode(el);
			}else if(el->ValueStr() == smpr){
				ProcessSMPNode(el);
			}else if(el->ValueStr() == gmpr){
				ProcessGMPNode(el);
			}else if(el->ValueStr() == sppr){
				ProcessSPPNode(el);
			}else if(el->ValueStr() == gppr){
				ProcessGPPNode(el);
			}else if(el->ValueStr() == marksr){
				ProcessMarksNode(el);
			};
		};
		printf("_____________________________\n\n");
	};
};

void MessageReader::ProcessStopNode(TiXmlElement *el_){
	int errid;
	if(el_->QueryIntAttribute("error", &errid) == TIXML_SUCCESS){
		printf("STOP response code=%d (%s).\n", errid, GetErrorStr(errid).c_str());
		const char *errstr = el_->GetText();
		if(errstr)
			printf("\tStatus:\"%s\"\n", errstr);
	}else
		printf("STOP succeded.\n");
};

void MessageReader::ProcessStartNode(TiXmlElement *el_){
	int errid=PAVPROT_OK;
	std::string key = PAVPROT_NODE_START;
	key += "R";
	if(el_->QueryIntAttribute("error", &errid) == TIXML_SUCCESS){
		if(errid == PAVPROT_CANCELED || errid == PAVPROT_FINISHED)
			m_stoped = true;
		printf("START response code=%d (%s).\n", errid, GetErrorStr(errid).c_str());
		const char *errstr = el_->GetText();
		if(errstr)
			printf("\tStatus:\"%s\"\n", errstr);
	}else
		printf("START succeded.\n");
	char val[10];
	sprintf(val, "%d", errid);
	m_keysMap[key] = val;
};

void MessageReader::ProcessMarksNode(TiXmlElement *el_){
	int errid=PAVPROT_OK;
	std::string key = PAVPROT_NODE_MARKS;
	key += "R";
	if(el_->QueryIntAttribute("error", &errid) == TIXML_SUCCESS){
		if(errid == PAVPROT_BUSY)
			printf("MARKS response code=%d (%s).\n", errid, GetErrorStr(errid).c_str());
		const char *errstr = el_->GetText();
		if(errstr)
			printf("\tStatus:\"%s\"\n", errstr);
	}else
		printf("MARKS succeded.\n");
	char val[10];
	sprintf(val, "%d", errid);
	m_keysMap[key] = val;
};

void MessageReader::ProcessSMPNode(TiXmlElement *el_){
	const char * mod = el_->Attribute("module");
	if(!mod){
		std::string xmlstr;
		xmlstr << *el_;
		printf("SMP format error, got \"%s\"\n", xmlstr.c_str());
		return;
	};
	printf("SMP for module \"%s\": ", mod);
	TiXmlElement *err = el_->FirstChildElement("Error");
	ProcessErrorNode(err);
};

void MessageReader::ProcessSPPNode(TiXmlElement *el_){
	const char * lib = el_->Attribute("lib");
	const char * path = el_->Attribute("path");
	if(!lib || !path){
		std::string xmlstr;
		xmlstr << *el_;
		printf("SPP format error, got \"%s\"\n", xmlstr.c_str());
		return;
	};
	printf("SPP for path \"%s@%s\": ", path, lib);
	TiXmlElement *err = el_->FirstChildElement("Error");
	ProcessErrorNode(err);
};

void MessageReader::ProcessGMPNode(TiXmlElement *el_){
	const char * mod = el_->Attribute("module");
	if(!mod){
		std::string xmlstr;
		xmlstr << *el_;
		printf("GMP format error, got \"%s\"\n", xmlstr.c_str());
		return;
	};

	printf("GMP for module \"%s\": ", mod);
	TiXmlElement *err = el_->FirstChildElement("Error");
	ProcessErrorNode(err);

	TiXmlElement * param;
	if(err)
		param = err->NextSiblingElement();
	else
		param = el_->FirstChildElement();

	while(param){
		printf("\t%s=%s\n", param->Value(), param->Attribute("val"));
		param = param->NextSiblingElement();
	};
};

void MessageReader::ProcessGPPNode(TiXmlElement *el_){
	const char * lib = el_->Attribute("lib");
	const char * path = el_->Attribute("path");
	if(!lib || !path){
		std::string xmlstr;
		xmlstr << *el_;
		printf("GPP format error, got \"%s\"\n", xmlstr.c_str());
		return;
	};
	printf("GPP for path \"%s@%s\": ", path, lib);
	TiXmlElement *err = el_->FirstChildElement("Error");
	ProcessErrorNode(err);

	TiXmlElement * param;
	if(err)
		param = err->NextSiblingElement();
	else
		param = el_->FirstChildElement();

	std::string rootkey = PAVPROT_NODE_GPP;
	rootkey += "R.";

	while(param){
		std::string key = rootkey;
		key += param->Value();
		m_keysMap[key] = param->Attribute("val");
		printf("\t%s=%s\n", param->Value(), param->Attribute("val"));
		param = param->NextSiblingElement();
	};
};

void MessageReader::ProcessErrorNode(TiXmlElement *err_){
	if(err_){
		int errid;
		if(err_->QueryIntAttribute("code", &errid) == TIXML_SUCCESS){
			if(errid != PAVPROT_OK)
				printf("failed. code=%d (%s).\n", errid, GetErrorStr(errid).c_str());
			else
				printf("succeeded.\n");
		}else 
			printf("unknown state\n");
		const char *errstr = err_->GetText();
		if(errstr)
			printf("\t>>Status:\"%s\"\n", errstr);
	}else
		printf("succeeded.\n");

};


std::string MessageReader::GetErrorStr(int err_){
	switch(err_){
	case PAVPROT_OK:
		return "Succsess";
	case PAVPROT_ERROR:
		return "Error";
	case PAVPROT_BUSY:
		return "Busy";
	case PAVPROT_CANCELED:
		return "Canceled";
	case PAVPROT_FINISHED:
		return "Finished";
	case PAVPROT_SEQVIOLATION:
		return "Message sequence violation";
	case PAVPROT_XMLERROR:
		return "XML Error";
	case PAVPROT_MSGERROR:
		return "Message error";
	};
	return "Unknown error";
};


