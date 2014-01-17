#ifndef __Sphere
#define __Sphere

#include "GenVector.h"
#include "Ray.h"
#include "Shape.h"

class Sphere : public Shape{

public:

	Sphere( Vector3 p, float rad, int mid )
	{

		pos = p;
		radius = rad;
		matid = mid;

	}

	~Sphere(){}

	virtual float intersect( Ray r )
	{

	float t1;
	float t2;
	float t = 100000000;

	Vector3 dir = r.getDir();
	Vector3 orig = r.getOrig();
	
	float a = dir.dot( dir );

	float b = ( dir * 2 ).dot( orig - pos );

	float c = ( orig - pos ).dot( orig - pos ) - ( radius * radius );
	bool inter = ( b * b ) - 4 * ( a * c ) > 0;
	if( inter )
	{

		t1 = ( -b + sqrt( ( b * b ) - 4 * ( a * c ) ) ) / 2 * a;

		t2 = ( -b - sqrt( ( b * b ) - 4 * ( a * c ) ) ) / 2 * a;

		bool t1Closer = t1 < t2;
		bool t2Intersects = t2 >= 0;
		if( t1Closer )
		{

			t = t1;

		}
		else if( t2Intersects )
		{

			t = t2;

		}

	}

	return t;

	}

	virtual Vector3 getNormal( Vector3 p ){ return ( p - pos ) / radius ; }

	virtual int getMatid(){ return matid; }

private:

	Vector3 pos;
	float radius;
	int matid;
	
};

#endif
