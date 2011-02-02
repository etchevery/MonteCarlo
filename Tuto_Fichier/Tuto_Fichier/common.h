// -----------------------------------------------------------
// common.h
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#ifndef I_COMMON_H
#define I_COMMON_H

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include <tinyxml.h>
#include <iostream>

using namespace std;

typedef unsigned int Pixel;

inline double Rand( double a_Range ) { return ((double)rand() / RAND_MAX) * a_Range; }

#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE(A)	{double l=1/sqrtf(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define LENGTH(A)		(sqrtf(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))

#define EPSILON			0.0001f
#define TRACEDEPTH		6

#define PI				3.141592653589793238462f
#define UNIFORM			1
#define IMPORTANCE		2
class vector3
{
public:
	vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {};
	vector3( double a_X, double a_Y, double a_Z ) : x( a_X ), y( a_Y ), z( a_Z ) {};
	void Set( double a_X, double a_Y, double a_Z ) { x = a_X; y = a_Y; z = a_Z; }
	void Normalize() { double l = 1.0f / Length(); x *= l; y *= l; z *= l; }
	double Length() { return (double)sqrt( x * x + y * y + z * z ); }
	double SqrLength() { return x * x + y * y + z * z; }
	double Dot( vector3 a_V ) { return x * a_V.x + y * a_V.y + z * a_V.z; }
	vector3 vector_product(vector3 v){ return vector3(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x); };
	vector3 Cross( vector3 b ) { return vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); }
	void operator += ( vector3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( vector3* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( vector3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( vector3* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( double f ) { x *= f; y *= f; z *= f; }
	void operator *= ( vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
	void operator *= ( vector3* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
	vector3 operator- () const { return vector3( -x, -y, -z ); }
	friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	friend vector3 operator + ( const vector3& v1, vector3* v2 ) { return vector3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
	friend vector3 operator - ( const vector3& v1, vector3* v2 ) { return vector3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
	friend vector3 operator * ( const vector3& v, double f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	friend vector3 operator * ( const vector3& v1, vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend vector3 operator * ( double f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	union
	{
		struct { double x, y, z; };
		struct { double r, g, b; };
		struct { double cell[3]; };
	};
	void afficher(){cout << "Point: " << "(" << x << "," << y << "," << z << ")" << endl;}
};

class plane
{
public:
	plane() : N( 0, 0, 0 ), D( 0 ) {};
	plane( vector3 a_Normal, double a_D ) : N( a_Normal ), D( a_D ) {};
	union
	{
		struct
		{
			vector3 N;
			double D;
		};
		double cell[4];
	};
};

typedef vector3 Color;


#endif