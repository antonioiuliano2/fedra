#include "Side.h"

Vector Side::mapPoint(Vector p)

{
	Vector v; 
	v.x = _mappedFovPosX + _mXX*p.x + _mXY * p.y;
	v.y = _mappedFovPosY + _mYX*p.x + _mYY * p.y;
	v.z = p.z;
	return v;
}

Vector2 Side::mapPoint(Vector2 p)

{
	Vector2 v; 
	v.x = _mappedFovPosX + _mXX*p.x + _mXY * p.y;
	v.y = _mappedFovPosY + _mYX*p.x + _mYY * p.y;
	return v;
}

Vector Side::iMapPoint(Vector p)

{
	Vector v; 
	p.x -= _mappedFovPosX;
	p.y -= _mappedFovPosY;
	v.x = _mXX*p.x + _mXY * p.y;
	v.y = _mYX*p.x + _mYY * p.y;
	v.z = p.z;
	return v;
}

Vector2 Side::iMapPoint(Vector2 p)

{
	Vector2 v; 
	p.x -= _mappedFovPosX;
	p.y -= _mappedFovPosY;
	v.x = _mXX*p.x + _mXY * p.y;
	v.y = _mYX*p.x + _mYY * p.y;
	return v;
}

void Side::setM(double mxx, double mxy, double myx, double myy)
{
	_mXX = mxx;
	_mXY = mxy;
	_mYX = myx;
	_mYY = myy;
	double det = 1 / (mxx * myy - mxy * myx);
	_iXX = _mYY * det;
	_iXY = -_mXY * det;
	_iYX = -_mYX * det;
	_iYY = _mXX * det;
}


Side Side::operator=(const Side &rhs)
{
	if (this == &rhs)
		return *this;
	_topZ = rhs.getTopZ();
	_bottomZ = rhs.getBottomZ();
	_nGrains = rhs.getGrains();
	_nTracks = rhs.getTracks();
	_z = rhs.getZ();
	_stageFovPosX = rhs.getStageFovPosX();
	_stageFovPosY = rhs.getStageFovPosY();
	_mappedFovPosX = rhs.getMappedFovPosX();
	_mappedFovPosY = rhs.getMappedFovPosY();
	_mXX = rhs.getMXX(); _mXY = rhs.getMXY(); _mYX = rhs.getMYX(); _mYY = rhs.getMYY();
	_iXX = rhs.getIXX(); _iXY = rhs.getIXY(); _iYX = rhs.getIYX(); _iYY = rhs.getIYY();
	_flag = rhs.getFlag();
	_nLayers = rhs._nLayers;
	for (int i = 0; i < static_cast<int> (rhs._tArr.size()); i++)
	{
		_tArr.push_back(rhs._tArr.at(i));
	}
	for (int i = 0; i < static_cast<int> (rhs._layer.size()); i++)
	{
		_layer.push_back(rhs.getLayersArr().at(i));
	}
	
	return *this;
}

void Side::addLayer(unsigned int grains, double z)
{
	Layer lay;
	lay.grains = grains;
	lay.zLayer = z;
	_layer.push_back(lay);
}
