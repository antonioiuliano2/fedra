#include "fitter.h"

Fitter::Fitter()
{
	_x.clear();
	_y.clear();
	_errx = _erry = _chi2 = _m = _q = _r = 0;
	_nPoints = 0;
}

Fitter::~Fitter()
{
}

void Fitter::addPoint(double x, double y)
{
	_x.push_back(x);
	_y.push_back(y);
	_nPoints++;
}

void Fitter::setErrXY(double errx, double erry)
{
	_errx = errx;
	_erry = erry;
}

bool Fitter::evaluateLinearFit()
{
	if (_nPoints == 0 || _erry == 0)
		return false;
	double s = 0, sx = 0, sy = 0, sxx = 0, syy = 0, sxy = 0;
	double d = _errx / _erry;
	//unsigned int nPoints = static_cast<unsigned int>(_x.size());
	for (unsigned short i = 0; i < _nPoints; i++)
	{
		double x = _x.at(i);
		double y = _y.at(i);
		s   += 1;		
		sx  += x;
		sy  += y;
		sxx += x*x;
		syy += y*y;
		sxy += x*y;
	}

	_r  = (s*sxy-sx*sy)/(sqrt(s*sxx-sx*sx)*sqrt(s*syy-sy*sy));
	s/=_errx;
	sx/=_errx;
	sy/=_errx;
	sxx/=_errx;
	syy/=_errx;
	sxy/=_errx;
	
	double den  = 1./(s*sxx - sx*sx);
	_m = (s*sxy - sx*sy)*den;
	//_r  = (s*sxy-sx*sy)/(sqrt(s*sxx-sx*sx)*sqrt(s*syy-sy*sy));
	double r2 = _r*_r;
	_m = 1./(2*d)*(-1./(d*_m)+d*_m/r2+1./(d*_m)*sqrt((1-d*d*_m*_m/r2)*(1-d*d*_m*_m/r2)+4*d*d*_m*_m));
	_q = (sy-sx*_m)/s;
	
	for (unsigned short i = 0; i < _nPoints; i++)
	{
		double x = _x.at(i);
		double y = _y.at(i);
		_chi2 += ((y-_q-_m*x)*(y-_q-_m*x))/((_erry*_erry)+_m*(_errx*_errx));
	}

	//_chi2/=(1+d*d*_m*_m);
				
	return true;
}

void Fitter::printStatistics()
{
	std::cout << "nPoints: " << _nPoints
		<< "\n y = " << _m << " * x + " << _q 
		<< "\n r = " << _r << "\nchi2 = " << _chi2 << std::endl;
}
