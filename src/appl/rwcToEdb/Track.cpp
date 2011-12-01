#include <fstream>
#include "Track.h"
#include "fitter.h"

Track::Track()
{
	meanDeltaTheta3D = sigmaDeltaTheta3D = 	meanDeltaThetaXZ = sigmaDeltaThetaXZ = meanDeltaThetaYZ = sigmaDeltaThetaYZ = 0;
	meanDistanceClustersTo3DLine = meanGapClusterToCluster = 0;
	mx =  qx = rx = xChi2 = 0;
	my =  qy = ry = yChi2 = 0;
	dz = 0;
	// to be set as input...
	_errx = _erry = 0.3;
	_errz = 3.;
}

double Track::pointTo3DlineDistance(double x0, double y0, double z0)
{
	double dist = 0;
	double qp_x = _x-x0;
	double qp_y = _y-y0;
	double qp_z = _z-z0;
	double vx = _sx;
	double vy = _sy;
	double vz = 1;
	double qr = (qp_x*vx+qp_y*vy+qp_z*vz)/sqrt(vx*vx+vy*vy+vz*vz);
	double qp =sqrt(qp_x*qp_x+qp_y*qp_y+qp_z*qp_z);
	dist = sqrt(qp*qp-qr*qr);
	//std::cout << qr << "\t" << qp << "\t" << dist << std::endl;
	return dist;
}

bool Track::evaluateEstimators()
{
	//std::ofstream out("test.txt",std::ios::app); // used now for debugging --> later to be removed
	
	if (_grains.size() == 0)  
		return false;
	
	unsigned short nGrains = _count;
	double s = 0, sx = 0, sy = 0, sz = 0, sxx = 0, syy = 0, szz = 0, szx = 0, szy = 0;
	double d = _errz/_errx;

	Fitter fitXZ; fitXZ.setErrXY(3.,0.33);  //error on z coordinate is ~3 um, in x or y is 0.33 (pixel dimension)
	Fitter fitYZ; fitYZ.setErrXY(3.,0.33);
	for (unsigned short i = 0; i < nGrains; i++)
	{
		double x = _grains.at(i).x;
		double y = _grains.at(i).y;
		double z = _grains.at(i).z;

		fitXZ.addPoint(z,x);
		fitYZ.addPoint(z,y);
		
		meanDistanceClustersTo3DLine+= pointTo3DlineDistance(x,y,z);

		if (i < nGrains -1)
		{
			double dx = _grains.at(i+1).x-x;
			double dy = _grains.at(i+1).y-y;
			double dz = _grains.at(i+1).z-z;
			meanGapClusterToCluster += sqrt(dx*dx+dy*dy+dz*dz);
		}

		if (i < nGrains-2)
		{
			double dx1 = _grains.at(i+1).x-x;
			double dy1 = _grains.at(i+1).y-y;
			double dz1 = _grains.at(i+1).z-z;
			double dx2 = _grains.at(i+2).x-_grains.at(i+1).x;
			double dy2 = _grains.at(i+2).y-_grains.at(i+1).y;
			double dz2 = _grains.at(i+2).z-_grains.at(i+1).z;

			double sx1  = dx1/dz1; // tan_x
			double sy1  = dy1/dz1; // tan_y

			double sx2  = dx2/dz2; // tan_x
			double sy2  = dy2/dz2; // tan_y
			
			double valueXZ = acos((dx1*dx2+dz1*dz2)/(sqrt(dx1*dx1+dz1*dz1)*sqrt(dx2*dx2+dz2*dz2)));
			double valueYZ = acos((dy1*dy2+dz1*dz2)/(sqrt(dy1*dy1+dz1*dz1)*sqrt(dy2*dy2+dz2*dz2)));
			double value3D = acos((dx1*dx2+dy1*dy2+dz1*dz2)/(sqrt(dx1*dx1+dy1*dy1+dz1*dz1)*sqrt(dx2*dx2+dy2*dy2+dz2*dz2)));
			//double value3D = acos((sx1*sx2+sy1*sy2+1)/(sqrt(sx1*sx1+sy1*sy1+1)*sqrt(sx2*sx2+sy2*sy2+1)));

			double signXZ = 1.;
			double signYZ = 1.;

			if (sx2<sx1)
				signXZ*=-1;
			if (sy2<sy1)
				signYZ*=-1;

			meanDeltaThetaXZ+= valueXZ * signXZ;
			meanDeltaThetaYZ+= valueYZ * signYZ;
			meanDeltaTheta3D+= value3D;

			sigmaDeltaThetaXZ+=valueXZ*valueXZ;
			sigmaDeltaThetaYZ+=valueYZ*valueYZ;
			sigmaDeltaTheta3D+=value3D*value3D;
			
		}
	}
	meanDeltaThetaXZ/=(nGrains-2.);
	meanDeltaThetaYZ/=(nGrains-2.);
	meanDeltaTheta3D/=(nGrains-2.);

	meanGapClusterToCluster/=(nGrains-1.);
	
	sigmaDeltaThetaXZ/=(nGrains-2.);
	sigmaDeltaThetaYZ/=(nGrains-2.);
	sigmaDeltaTheta3D/=(nGrains-2.);

	sigmaDeltaThetaXZ-=pow(meanDeltaThetaXZ,2);
	sigmaDeltaThetaYZ-=pow(meanDeltaThetaYZ,2);
	sigmaDeltaTheta3D-=pow(meanDeltaTheta3D,2);

	sigmaDeltaThetaXZ = sqrt(sigmaDeltaThetaXZ);
	sigmaDeltaThetaYZ = sqrt(sigmaDeltaThetaYZ);
	sigmaDeltaTheta3D = sqrt(sigmaDeltaTheta3D);

	
	
	// fit line grains on plane x-z
	fitXZ.evaluateLinearFit();
	mx = fitXZ.getSlope();
	qx = fitXZ.getIntercept();
	rx = fitXZ.getPearsonCoefficent();
	
	
	// fit line grains on plane y-z
	fitYZ.evaluateLinearFit();
	my = fitYZ.getSlope();
	qy = fitYZ.getIntercept();
	ry = fitYZ.getPearsonCoefficent();
	
	xChi2 = fitXZ.getChi2();
	yChi2 = fitYZ.getChi2();

	// lenght of the segment (in z).
	dz = _grains.at(0).z-_grains.at(nGrains-1).z;
	meanDistanceClustersTo3DLine/=nGrains;

	return true;
}