//-- Author :  Valeri Tioukov   6.03.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbMath                                                              //
//                                                                      //
// collection of general-purpose mathematical algorithms                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "EdbMath.h"
#include "TMatrixD.h"
#include "TVectorD.h"

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

//-------------------------------------------------------------------------------------
bool EdbMath::LinFitCOV( int n, float *x, float *y, float *c, float p[2], float d[2][2], float *chi2)
{
  // linear fit by YP with using full covariance matrix of measurements :  
  //                  y = p[0] + p[1]*x , d[2][2] is error matrix for p[2]
  // Input : n     - number of points
  //         x,y - data arrays of the length n
  //         c   - full covariance matrix c[n][n]
    
    if (n < 2) return false;
    int i = 0, j = 0;
    double dy = 0;

    TMatrixD D(n,n);   // error matrix of measurements
    TMatrixD A(n,2);   // construction matrix
    TMatrixD AT(2,n);  // A transposed
    TVectorD Y(n);     // measurements vector
    TVectorD Y1(n);    // temporary vector
    TVectorD DY(n);    // residuals
    TMatrixD DI(n,n);  // Inverse of D-matrix
    TMatrixD DP(2,2);  // error matrix of parameters
    TMatrixD B(2,2);   // inverse error matrix of parameters
    TVectorD P(2);     // parameters vector

    for (i=0; i<n; i++)
    {
	A(i,0) = (double)1.;
	A(i,1) = (double)x[i];
	AT(0,i) = A(i,0);
	AT(1,i) = A(i,1);
	Y(i) = (double)y[i];
	for (j=0; j<n; j++)
	{
	    D(i,j) = (double)(*(c+i*n+j));
	}	
    }

    Y1 = Y;
    DI = D.Invert();
    B  = AT*(DI*A);
    DP = B.Invert();
    Y1 *= DI;
    AT.ResizeTo(n,n);
    Y1 *= AT; 
    Y1.ResizeTo(2);
    P  = Y1;
    P  *= DP;
    
    for (i=0; i<2; i++)
    {
	p[i] = (float)P(i);
	for (j=0; j<2; j++)
	{
	    d[i][j] = (float)DP(i,j);
	}
    }
    
    for (i=0; i<n; i++)
    {
	dy = (double)(y[i] - p[0] - p[1]*x[i]);
	DY(i) = dy;
    }
    TVectorD DY1 = DY;
    DY *= DI;
    *chi2 = (float)(DY1*DY);

    return true;
}

//-------------------------------------------------------------------------------------
void EdbMath::LFITW( float *X, float *Y, float *W, int L, int KEY, float &A, float &B, float &E )
{
  // E250:
  //     TO PERFORM A WEIGHTED STRAIGHT LINE FIT
  //
  //     FOR FORMULAE USED SEE MENZEL, FORMULAS OF PHYSICS P.116
  //
  //     FIT IS OF Y=AX+B , WITH S**2 ESTIMATOR E. WEIGHTS ARE IN W.
  //     IF KEY=0, POINTS WITH Y=0 ARE IGNORED
  //     L IS NO. OF POINTS
  //

  A=0; B=0; E=0;

  //     CALCULTE SUMS
  if(L<2)  return;

  double WW=0.;
  double WWF=0.;
  double WWFI=0.;
  double W2=0.;
  double W2X=0.;
  double W2Y=0.;
  double W2XY=0.;
  double W2X2=0.;
  double W2Y2=0.;
  int    ICNT=0;

  for( int j=0; j<L; j++ ){
    if( Y[j]==0. && KEY==0 )  continue;
    WW=W[j]*W[j];
    W2=WW+W2;
    WWF=WW*X[j];
    W2X=WWF+W2X;
    W2X2=WWF*X[j]+W2X2;
    W2XY=WWF*Y[j]+W2XY;
    WWFI=WW*Y[j];
    W2Y=WWFI+W2Y;
    W2Y2=WWFI*Y[j]+W2Y2;
    ICNT++;
  }

  //     FIT PARAMETERS
  A=(W2XY-W2X*W2Y/W2)/(W2X2-W2X*W2X/W2);
  B=(W2Y-A*W2X)/W2;
  if(ICNT<3)     return;
  E = (W2Y2-W2Y*W2Y/W2 -
       (W2XY-W2X*W2Y/W2)*(W2XY-W2X*W2Y/W2)/(W2X2-W2X*W2X/W2))/(ICNT-2);
}
