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

#include "EmathVer.h"

ClassImp(EdbMath)
ClassImp(TIndex2)

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
  // Disclamer: This is a modified code from site of Paul Bourke

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
bool EdbMath::LineLineIntersect( float p1[3], float p2[3], float p3[3], float p4[3],
				 float pa[3], float pb[3],
				 double &mua, double &mub)
{
// Disclamer: This is a modified code from site of Paul Bourke
//
//     Calculate the line segment PaPb that is the shortest route between
//     two lines P1P2 and P3P4. Calculate also the values of mua and mub where
//        Pa = P1 + mua (P2 - P1)
//        Pb = P3 + mub (P4 - P3)
//     Return FALSE if no solution exists (the lines are parallel)

   float p13[3],p43[3],p21[3];
   double d1343,d4321,d1321,d4343,d2121;
   double numer,denom;
   const float EPS=1.E-10;

   p13[0] = p1[0] - p3[0];
   p13[1] = p1[1] - p3[1];
   p13[2] = p1[2] - p3[2];
   p43[0] = p4[0] - p3[0];
   p43[1] = p4[1] - p3[1];
   p43[2] = p4[2] - p3[2];
   if (TMath::Abs(p43[0])  < EPS && 
       TMath::Abs(p43[1])  < EPS && 
       TMath::Abs(p43[2])  < EPS)      return false;
   p21[0] = p2[0] - p1[0];
   p21[1] = p2[1] - p1[1];
   p21[2] = p2[2] - p1[2];
   if (TMath::Abs(p21[0])  < EPS && 
       TMath::Abs(p21[1])  < EPS && 
       TMath::Abs(p21[2])  < EPS)      return false;

   d1343 = p13[0] * p43[0] + p13[1] * p43[1] + p13[2] * p43[2];
   d4321 = p43[0] * p21[0] + p43[1] * p21[1] + p43[2] * p21[2];
   d1321 = p13[0] * p21[0] + p13[1] * p21[1] + p13[2] * p21[2];
   d4343 = p43[0] * p43[0] + p43[1] * p43[1] + p43[2] * p43[2];
   d2121 = p21[0] * p21[0] + p21[1] * p21[1] + p21[2] * p21[2];

   denom = d2121 * d4343 - d4321 * d4321;
   if (TMath::Abs(denom) < EPS)
      return false;
   numer = d1343 * d4321 - d1321 * d4343;

   mua = numer / denom;
   mub = (d1343 + d4321 * (mua)) / d4343;

   pa[0] = p1[0] + mua * p21[0];
   pa[1] = p1[1] + mua * p21[1];
   pa[2] = p1[2] + mua * p21[2];
   pb[0] = p3[0] + mub * p43[0];
   pb[1] = p3[1] + mub * p43[1];
   pb[2] = p3[2] + mub * p43[2];

   return true;
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
bool EdbMath::LinFitCOV( int n, float *x, float *y, double *c, float *p, float *d, float *chi2)
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
	*(p+i) = (float)P(i);
	for (j=0; j<2; j++)
	{
	    *(d+i*2+j) = (float)DP(i,j);
	}
    }
    
    for (i=0; i<n; i++)
    {
	dy = (double)(y[i] - *p - (*(p+1))*x[i]);
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

//-------------------------------------------------------------------------------------
int EdbMath::LFIT3( float *X, float *Y, float *Z, float *W, int L, 
		     float &X0, float &Y0, float &Z0, float &TX, float &TY, float &EX, float &EY )
{
  // Linar fit in 3-d case (microtrack-like)
  // Input: X,Y,Z - coords, W -weight - arrays of the lengh >=L
  //        Note that X,Y,Z modified by function
  // Output: X0,Y0,Z0 - center of gravity of segment
  //         TX,TY : tangents in respect to Z axis

  if(L<2) return 0;

  // first find C.O.G.:
  double wx=0, wy=0, wz=0, w=0;
  for(int i=0; i<L; i++) {
    wx += X[i]*W[i];
    wy += Y[i]*W[i];
    wz += Z[i]*W[i];
    w  += W[i];
  }
  X0 = wx/w;
  Y0 = wy/w;
  Z0 = wz/w;
  for(int i=0; i<L; i++) {
    X[i] -= X0;
    Y[i] -= Y0;
    Z[i] -= Z0;
  }

  if(L==2) {
    TX = (X[1]-X[0])/(Z[1]-Z[0]);
    TY = (Y[1]-Y[0])/(Z[1]-Z[0]);
    EX=EY=0;
    return L;
  }

  float x0=0,y0=0;
  EdbMath::LFITW( Z,X,W, L, 1, TX,x0,EX );
  EdbMath::LFITW( Z,Y,W, L, 1, TY,y0,EY );

  X0 += x0;
  Y0 += y0;
  
  return L;
}

//-------------------------------------------------------------------------------------
void TIndex2::BuildIndex( int n, double *w )
{
  Int_t  *ind = new Int_t[n];
  TMath::Sort(n,w,ind,0);
  Set(n);
  for (Int_t i=0;i<n;i++) {
    (*this)[i] = w[ind[i]];
  }
}

//-------------------------------------------------------------------------------------
Int_t TIndex2::Find(Int_t major, Int_t minor)
{
  if (GetSize()<1) return -1;
  Double_t value = BuildValue(major,minor);
  Int_t i = TMath::BinarySearch( fN, GetArray(), value);
  if (i < 0) return -1;
  if (TMath::Abs((*this)[i] - value) > 1.e-10) return -1;
  return i;
}

//-------------------------------------------------------------------------------------
Int_t TIndex2::FindIndex(Int_t major)
{
  // return the value of minor (this make sence in the case when major is unique)
  if (fN<1) return -1;
  Double_t value = (Double_t)major;
  Int_t i = TMath::BinarySearch( fN, GetArray(), value);
  if (i < -1) return -1;
  if (i == -1) i++;
  if(i>fN-1) return -1;
  if( (*this)[i]<major ) i++; 
  if(i>fN-1) return -1;
  if (TMath::Abs((*this)[i] - value) > .5) return -1;
  return (Int_t)( ((*this)[i]-value+1e-10)*1e+9 );
}

//-------------------------------------------------------------------------------------
Int_t TIndex2::FindIndexArr(Int_t major, TArrayI &mina )
{
  // return the number of found events with given major
  // Output: mina - array of minors - should be big enough

  if (fN<1) return -1;
  Double_t value = (Double_t)major;
  Int_t i = TMath::BinarySearch( fN, GetArray(), value);
  if (i < -1) return -1;
  if (i == -1) i++;
  if(i>fN-1) return -1;
  if( (*this)[i]<major ) i++; 
  if(i>fN-1) return -1;
  if (TMath::Abs((*this)[i] - value) > .5) return -1;

  int na=mina.fN;
  int ia=0;
  for(ia=0; ia<na; ia++) {
    if( (*this)[i+ia] > value+1-1e-10 ) break;
    mina[ia] = (Int_t)( ((*this)[i+ia]-value+1e-10)*1e+9 );
  }
  return ia;
}

//---------------------------------------------------------------------------
void TIndex2::Print()
{
  for(int i=0; i<fN; i++)
    printf("%d \t:  %d  %d\n",i,Major(i),Minor(i));
}
