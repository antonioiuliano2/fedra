///-- Author :  Valeri Tioukov   6.03.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbMath                                                              //
//                                                                      //
// collection of general-purpose mathematical algorithms                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbMath.h"
#include <math.h>

ClassImp(EdbMath)

//-------------------------------------------------------------------------------------
float EdbMath::Magnitude3( float Point1[3], float Point2[3] )
{
    float dx = Point2[0] - Point1[0];
    float dy = Point2[1] - Point1[1];
    float dz = Point2[2] - Point1[2];
    return (float)sqrt( dx*dx+dy*dy+dz*dz );
}

//-------------------------------------------------------------------------------------
float EdbMath::DistancePointLine3( float Point[3], float LineStart[3], float LineEnd[3], bool inside )
{
  // Disclamer: This code is partially stalled from site of Paul Bourke

    float LineMag;
    float U;
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
