#ifndef TRACK_H
#define TRACK_H

#include <iostream>
#include <vector>
#include <cmath>
#include "Struct.h"

class Track
{
private:
	unsigned int _areaSum;
	unsigned short _count;
	double _x;
	double _y;
	double _z;
	double _errx;  
	double _erry;
	double _errz;
	double _sx;
	double _sy;
	double _sz;
	double _sigma;
	double _topZ;
	double _bottomZ;
	std::vector<Grain> _grains;

	double pointTo3DlineDistance(double x0, double y0, double z0);



public:
	Track();
	~Track(){};

	// track estimators
	double meanDeltaTheta3D;
	double sigmaDeltaTheta3D;
	double meanDeltaThetaXZ;
	double sigmaDeltaThetaXZ;
	double meanDeltaThetaYZ;
	double sigmaDeltaThetaYZ;
	double meanDistanceClustersTo3DLine, meanGapClusterToCluster;
	double mx, mxSigma, qx, rx, xChi2;
	double my, mySigma, qy, ry, yChi2;
	double dz;
	

	void setAreaSum(unsigned int areaSum) {_areaSum = areaSum;}
	void setCount(unsigned short count) {_count = count;}
	void setX(double x) {_x = x;}
	void setY(double y) {_y = y;}
	void setZ(double z) {_z = z;}

	void setErrX(double errx) {_errx = errx;}
	void setErrY(double erry) {_erry = erry;}
	void setErrZ(double errz) {_errz = errz;}

	void setSX(double sx) {_sx = sx;}
	void setSY(double sy) {_sy = sy;}
	void setSZ(double sz) {_sz = sz;}
	void setSigma(double sigma) {_sigma = sigma;}
	void setTopZ(double z) {_topZ = z;}
	void setBottomZ(double z) {_bottomZ = z;}
	void addGrain(Grain gr) {_grains.push_back(gr);}

	unsigned int getAreaSum() const {return _areaSum;}
	unsigned short getCount() const {return _count;}
	double getX() const {return _x;}
	double getY() const {return _y;}
	double getZ() const {return _z;}
	double getErrX() const {return _errx;}
	double getErrY() const {return _erry;}
	double getErrZ() const {return _errz;}
	double getSX() const {return _sx;}
	double getSY() const {return _sy;}
	double getSZ() const {return _sz;}
	double getSigma() const {return _sigma;}
	double getTopZ() const {return _topZ;}
	double getBottomZ() const {return _bottomZ;}
	std::vector<Grain> getGrains() const {return _grains;}

	bool evaluateEstimators();


    

};

#endif
