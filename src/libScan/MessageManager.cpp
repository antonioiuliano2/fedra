#include "MessageManager.h"
#include <iostream>

void MessageManager::Clean(){
	m_messId++;
	m_xml = "";
	m_xmlDoc.Clear();
};

void MessageManager::CreateMessage(){
	Clean();
	TiXmlElement main(PAVPROT_NODE_ROOT);
	TiXmlElement mess(PAVPROT_NODE_MESS);
	main.SetAttribute("version", PAVCLIENTVERSION);
	mess.SetAttribute("id", m_messId);
	m_xmlDocAddPtr = m_xmlDoc.InsertEndChild(main)->ToElement();
	m_xmlDocAddPtr->InsertEndChild(mess);
};

void MessageManager::AddStopNode(){
	TiXmlElement stop(PAVPROT_NODE_STOP);
	m_xmlDocAddPtr->InsertEndChild(stop);
};

void MessageManager::AddStartNode(const char *lib_, const char *path_, const char *comment_){
	TiXmlElement start(PAVPROT_NODE_START);
	start.SetAttribute("lib", lib_);
	start.SetAttribute("path", path_);
	TiXmlText comment(comment_);
	start.InsertEndChild(comment);
	m_xmlDocAddPtr->InsertEndChild(start);
};

void MessageManager::AddMarksNode(int brick_, int plate_, const char *mapext_){
	TiXmlElement marks(PAVPROT_NODE_MARKS);
	marks.SetAttribute("brick", brick_);
	marks.SetAttribute("plate", plate_);
	marks.SetAttribute("mapext", mapext_);
	m_xmlDocAddPtr->InsertEndChild(marks);
};

void MessageManager::AddGetModuleParamsNode(const char *module_, ...){//ToDo: unix-compatible?
	TiXmlElement gmp(PAVPROT_NODE_GMP);
	gmp.SetAttribute("module", module_);

	char* var;
	va_list args;
	va_start(args, module_);
	while((var = va_arg(args, char *)) != NULL){
		TiXmlElement vare(var);
		gmp.InsertEndChild(vare);
	}
	va_end(args);

	m_xmlDocAddPtr->InsertEndChild(gmp);
};

void MessageManager::AddSetModuleParamsNode(const char *module_, ...){//ToDo: unix-compatible?
	TiXmlElement smp(PAVPROT_NODE_SMP);
	smp.SetAttribute("module", module_);

	char* var, *val;
	va_list args;
	va_start(args, module_);
	while((var = va_arg(args, char *)) != NULL){
		val = va_arg(args, char *);
		if(val != NULL){
			TiXmlElement vare(var);
			vare.SetAttribute("val", val);
			smp.InsertEndChild(vare);
		}else{
			printf("Value is not provided for parameter <%s> in module <%s>\n", var, module_);
			break;
		};
	}
	va_end(args);

	m_xmlDocAddPtr->InsertEndChild(smp);
};

void MessageManager::AddGetPathParamsNode(const char *lib_, const char *path_, ...){//ToDo: unix-compatible?
	TiXmlElement gpp(PAVPROT_NODE_GPP);
	gpp.SetAttribute("lib", lib_);
	gpp.SetAttribute("path", path_);

	char* var;
	va_list args;
	va_start(args, path_);
	var = va_arg(args, char *);
	while(var){
		TiXmlElement vare(var);
		gpp.InsertEndChild(vare);
		var = va_arg(args, char *);
	}
	va_end(args);

	m_xmlDocAddPtr->InsertEndChild(gpp);
};

void MessageManager::AddSetPathParamsNode(const char *lib_, const char *path_, ...){//ToDo: unix-compatible?
	TiXmlElement spp(PAVPROT_NODE_SPP);
	spp.SetAttribute("lib", lib_);
	spp.SetAttribute("path", path_);

	char* var, *val;
	va_list args;
	va_start(args, path_);
	while((var = va_arg(args, char *)) != NULL){
		val = va_arg(args, char *);
		if(val != NULL){
			TiXmlElement vare(var);
			vare.SetAttribute("val", val);
			spp.InsertEndChild(vare);
		}else{
			printf("Value is not provided for parameter <%s> in lib <%s>, path <%s>\n", var, lib_, path_);
			break;
		};
	}
	va_end(args);

	m_xmlDocAddPtr->InsertEndChild(spp);
};

void MessageManager::FillBuff(){
	TiXmlPrinter printer;
	m_xmlDoc.Accept( &printer );
	printf("%s\n", printer.CStr());//ToDo: verbisity
	std::string str;
	str << m_xmlDoc;
	WriteMessage(m_messBuf, str.c_str(), str.length());
};
