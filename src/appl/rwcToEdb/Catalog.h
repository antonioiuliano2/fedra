#ifndef CATALOG_H
#define CATALOG_H

#include "Struct.h"

class Catalog
{
private:
	Area _area;
	unsigned int _nConfigs, _xViews, _yViews;
	unsigned int *_pFragmentIndexes;
	unsigned char Reserved[256];
	HeaderInfo _headerInfo;
	SetupStringRepresentation _setup;


public:
	SetupStringRepresentation setup;
	Catalog(){_pFragmentIndexes = 0;}
	~Catalog(){delete [] _pFragmentIndexes;}
	void setHeaderInfo (HeaderInfo headerInfo);
	HeaderInfo getHeaderInfo() {return _headerInfo;}
	void initFragmentIndexes(unsigned int yviews, unsigned int xviews){_pFragmentIndexes = new unsigned int[yviews*xviews];}
	bool setFragmentIndexes(unsigned int y, unsigned int x, unsigned int value);
	void setXviews(unsigned int x){_xViews = x;}
	void setYviews(unsigned int y){_yViews = y;}
	void setXmin(double xMin){_area.xMin = xMin;}
	void setXmax(double xMax){_area.xMax = xMax;}
	void setYmin(double yMin){_area.yMin = yMin;}
	void setYmax(double yMax){_area.yMax = yMax;}
	void setXstep(double xStep){_area.xStep = xStep;}
	void setYstep(double yStep){_area.yStep = yStep;}
	void setNfragments(unsigned int n){_area.nFragments = n;}
	int findConfig(std::string configName);
	std::string getConfigValue(int index,std::string configName);

	
	unsigned int getFragmentIndexes(unsigned int y, unsigned int x) {return _pFragmentIndexes[y*_xViews+x];};
	unsigned int getXviews(){return _xViews;}
	unsigned int getYviews(){return _yViews;}
	double getXmin(){return _area.xMin;}
	double getXmax(){return _area.xMax;}
	double getYmin(){return _area.yMin;}
	double getYmax(){return _area.yMax;}
	double getXstep(){return _area.xStep;}
	double getYstep(){return _area.yStep;}
	unsigned int getNfragments(){return _area.nFragments;}
	SetupStringRepresentation* getSetup() {return &_setup;}


	

};

#endif