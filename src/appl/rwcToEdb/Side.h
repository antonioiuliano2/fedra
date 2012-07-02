
#ifndef SIDE_H
#define SIDE_H

#include <vector>
#include "Struct.h"
#include "Track.h"


class Side
{
private:
	double _topZ;
	double _bottomZ;
	int _nGrains;
	int _nTracks;
	int _nLayers;
	double _z;
	double _stageFovPosX;
	double _stageFovPosY;
	double _mappedFovPosX;
	double _mappedFovPosY;
	double _mXX, _mXY, _mYX, _mYY; //from view coord to experiment coord
	double _iXX, _iXY, _iYX, _iYY; //from experiment coord to view coord
	int _flag;
	std::vector<Track> _tArr;
	std::vector<Layer> _layer;
	

public:
	Side(){};
	~Side(){};

	Side operator=(const Side &);

	inline void setTopZ(double topZ)           {_topZ = topZ;}
	inline void setBottomZ(double bottomZ)     {_bottomZ = bottomZ;}
	inline void setZ(double z)                {_z = z;}
	inline void setStageFovPosX(double posx)  {_stageFovPosX = posx;}
	inline void setStageFovPosY(double posy)  {_stageFovPosY = posy;}
	inline void setMappedFovPosX(double posx) {_mappedFovPosX = posx;}
	inline void setMappedFovPosY(double posy) {_mappedFovPosY = posy;}
	inline void setGrains(int nGrains)        {_nGrains = nGrains;}
	inline void setTracks(int nTracks)        {_nTracks = nTracks;}
	inline void setFlag(int flag)             {_flag = flag;}
	inline void setLayers(int nLayers)             {_nLayers = nLayers;}
	void addLayer(unsigned int grains, double z);
	void addTrack(Track tr) {_tArr.push_back(tr);}
	void setM(double mxx,double mxy,double myx,double myy);
	void setMXX(double mxx) {_mXX = mxx;}
	void setMXY(double mxy) {_mXY = mxy;}
	void setMYX(double myx) {_mYX = myx;}
	void setMYY(double myy) {_mYY = myy;}
	void setIXX(double mxx) {_iXX = mxx;}
	void setIXY(double mxy) {_iXY = mxy;}
	void setIYX(double myx) {_iYX = myx;}
	void setIYY(double myy) {_iYY = myy;}
	
	double getTopZ()     const       {return _topZ;}
	double getBottomZ()  const        {return _bottomZ;}
	double getZ()        const        {return _z;}
	double getStageFovPosX()  const   {return _stageFovPosX;}
	double getStageFovPosY()  const   {return _stageFovPosY;}
	double getMappedFovPosX() const   {return _mappedFovPosX;}
	double getMappedFovPosY() const   {return _mappedFovPosY;}
	int getLayers() const {return _nLayers;}
	double getMXX() const {return _mXX;}
	double getMXY() const {return _mXY;}
	double getMYX() const {return _mYX;}
	double getMYY() const {return _mYY;}
	double getIXX() const {return _iXX;}
	double getIXY() const {return _iXY;}
	double getIYX() const {return _iYX;}
	double getIYY() const {return _iYY;}
	Layer* getLayerAt(int id)  {return &(_layer.at(id));} //mettere check boundary
	Track* getTrackAt(int id) {return &(_tArr.at(id));}//mettere check boundary
	std::vector<Layer> getLayersArr()  const {return _layer;}

	int getGrains() const {return _nGrains;}
	int getTracks() const {return _nTracks;}
	int getFlag()   const {return _flag;}
	
	Vector mapPoint(Vector p);
	Vector2 mapPoint(Vector2 p);
	Vector iMapPoint(Vector p);
	Vector2 iMapPoint(Vector2 p);

	
};
#endif
