#ifndef _SYSAL_VECTORS_
#define _SYSAL_VECTORS_

#include <math.h>
#include "TVectors.h"

#pragma pack(push)
#pragma pack(4)

struct Vector
{
	float X;
	float Y;
	float Z;

	Vector() {};
	Vector(float x,float y,float z) : X(x), Y(y), Z(z) {};
	Vector(TVector &T) : X(T.X), Y(T.Y), Z(T.Z) {};
	};

#pragma pack(pop)

inline float operator!(Vector A)
{	return sqrt(A.X*A.X+A.Y*A.Y+A.Z*A.Z);	};

inline Vector operator-(Vector A)
{	return Vector(-A.X,-A.Y,-A.Z);	};

inline Vector operator+(Vector A,Vector B)
{  return Vector(A.X+B.X,A.Y+B.Y,A.Z+B.Z);	};

inline Vector operator-(Vector A,Vector B)
{  return Vector(A.X-B.X,A.Y-B.Y,A.Z-B.Z);	};

inline Vector operator^(Vector A,Vector B)
{  return Vector(A.Y*B.Z-A.Z*B.Y,A.Z*B.X-A.X*B.Z,A.X*B.Y-A.Y*B.X);	};

inline float operator*(Vector A,Vector B)
{  return A.X*B.X+A.Y*B.Y+A.Z*B.Z;	};

inline Vector operator*(float A,Vector B)
{  return Vector(A*B.X,A*B.Y,A*B.Z);	};

inline Vector operator*(Vector A,float B)
{  return Vector(B*A.X,B*A.Y,B*A.Z);	};

inline Vector operator/(Vector A,float B)
{  return Vector(A.X/B,A.Y/B,A.Z/B);	};

#endif


