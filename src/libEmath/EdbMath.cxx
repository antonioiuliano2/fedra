///-- Author :  Valeri Tioukov   6.03.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbMath                                                              //
//                                                                      //
// collection of general-purpose mathematical algorithms                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "EdbMath.h"
//#include <math.h>

ClassImp(EdbMath)

//-------------------------------------------------------------------------------------
double EdbMath::Magnitude3( float Point1[3], float Point2[3] )
{
    double dx = Point2[0] - Point1[0];
    double dy = Point2[1] - Point1[1];
    double dz = Point2[2] - Point1[2];
    return TMath::Sqrt( dx*dx+dy*dy+dz*dz );
}

//-------------------------------------------------------------------------------------
double EdbMath::DistancePointLine3( float Point[3], float LineStart[3], float LineEnd[3], bool inside )
{
  // Disclamer: This code is partially stalled from site of Paul Bourke

    double LineMag;
    double U;
    float Intersection[3];
 
    LineMag = Magnitude3( LineEnd, LineStart );
 
    U = ( ( ( Point[0] - LineStart[0] ) * ( LineEnd[0] - LineStart[0] ) ) +
        ( ( Point[1] - LineStart[1] ) * ( LineEnd[1] - LineStart[1] ) ) +
        ( ( Point[2] - LineStart[2] ) * ( LineEnd[2] - LineStart[2] ) ) ) /
        ( LineMag * LineMag );
 
    if( U < 0.0f || U > 1.0f )
      inside=false;   // closest point does not fall within the line segment
    else inside=true;

    Intersection[0] = LineStart[0] + U * ( LineEnd[0] - LineStart[0] );
    Intersection[1] = LineStart[1] + U * ( LineEnd[1] - LineStart[1] );
    Intersection[2] = LineStart[2] + U * ( LineEnd[2] - LineStart[2] );
 
    return Magnitude3( Point, Intersection );
}

//-------------------------------------------------------------------------------------
bool EdbMath::LinFitDiag( int n, float *x, float *y, float *e, float p[2], float d[2][2], float *chi2)
{
  // linear fit by YP:  y = p[0] + p[1]*x , d[2][2] is error matrix for p[2]
  // Input : n     - number of points
  //         x,y,e - data arrays of the length n
    
    if (n < 2) return false;
    int i = 0;
    double w = 0., ed = 0.;
    double sw = 0., swx = 0., swx2 = 0., swy = 0., swxy = 0.;
    float dy = 0.;

    for (i=0; i<n; i++)
    {
	ed = (double)e[i];
	if (ed > 0.)
	{
	    w = 1./(ed*ed);
	    sw   += w;
	    swx  += w*x[i];
	    swx2 += w*x[i]*x[i];
	    swy  += w*y[i];
	    swxy += w*x[i]*y[i];
	}
    }
    double det = sw*swx2 - swx*swx;
    if (det == 0.) return false;
    d[0][0] = (float)( swx2/det);
    d[0][1] = (float)(-swx/det);
    d[1][0] = d[0][1];
    d[1][1] = (float)(sw/det);
    p[0] = (float)((swy*swx2 - swxy*swx)/det);
    p[1] = (float)((swxy*sw  - swy*swx)/det);
    *chi2 = 0.;
    for (i=0; i<n; i++)
    {
	if ( e[i] > 0.)
	{
	    dy = y[i] - p[0] - p[1]*x[i];
	    *chi2 += dy*dy/(e[i]*e[i]);
	}
    }
    return true;
}
