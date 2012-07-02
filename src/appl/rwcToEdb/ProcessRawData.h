#ifndef _PROCESS_RAW_DATA
#define _PROCESS_RAW_DATA

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "Struct.h"
#include "Catalog.h"
#include "Fragment.h"

#ifndef __CINT__
#include "EdbRun.h"
#include "EdbSegment.h"
#include "EdbRunAccess.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#endif

#define SIZE_OF_CHAR 1
#define SIZE_OF_INT 4
#define SIZE_OF_UNSIGNED_SHORT_INT 2
#define SIZE_OF_UNSIGNED_INT  4
#define SIZE_OF_DOUBLE  8
#define SIZE_OF_FLOAT  4

class ProcessRawData
{
private:
  EdbRun   *_run;
	Catalog  *_catalog; 
	Fragment *_fragment;
	std::string _catFileName;
	std::string _fragFileName;
	std::string _outputRootFileName;
	bool _clusters;
	bool _filterTracks;

public:
	ProcessRawData();
	~ProcessRawData();
	void initCatalog();
	void initFragment();

	void setCatalogName(std::string fileName)  {_catFileName = fileName;}
	void setFragmentName(std::string fileName) {_fragFileName = fileName;}
	void setRootName(std::string fileName) {_outputRootFileName = fileName;}
	void setClOption(bool noCl) {_clusters = noCl;}
	void setFilterTrackOption(bool filterTracks) {_filterTracks = filterTracks;}
	void filterTracks(bool filterTracks) {_filterTracks = filterTracks;}
	const char* getCatalogName() {return _catFileName.c_str();}
	const char* getFragmentName(){return _fragFileName.c_str();}
	const char* getRootName(){return _outputRootFileName.c_str();}
	bool  getClOption() {return _clusters;}
	bool  getFilterTracksOption() {return _filterTracks;}
	unsigned int getFragmentsNumberFromCatalog();
	bool readCatalog();
	bool readFragment();
	bool dumpCatalogInEdbStructure();
	bool dumpFragmentInEdbStructure();
	
	int makeTracksPlot(const char *plotFileName);
};


#endif
