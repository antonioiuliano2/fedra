#include <iostream>
#include <cmath>
#include <vector>

#ifndef FITTER_H
#define FITTER_H

class Fitter
{
private:
	double _m;
	double _q;
	double _r;
	double _chi2;
	std::vector<double> _x;
	std::vector<double> _y;
	double _errx, _erry;
	unsigned short _nPoints;

public:
	Fitter();
	~Fitter();
	double getSlope() {return _m;};
	double getIntercept() {return _q;};
	double getPearsonCoefficent() {return _r;};
	double getChi2(){return _chi2;};
	void addPoint(double x, double y);
	bool evaluateLinearFit();
	void printStatistics();
	void setErrXY(double errx, double erry);
};

#endif
