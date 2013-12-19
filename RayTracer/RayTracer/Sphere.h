#ifndef __Sphere
#define __Sphere

#include "GenVector.h"
#include "Ray.h"
#include "Shape.h"

class Sphere : public Shape{

public:
	Sphere(Vector3 p, float rad, int mid){
		pos = p;
		radius = rad;
		matid = mid;
	}
	~Sphere(){}

	virtual float intersect( Ray r ){
	float t1;
	float t2;
	float t = 100000000;
	Vector3 dir = r.getDir().normalize();
	Vector3 orig = r.getOrig();
	float a = dir.dot( dir );
	float b = ( dir * 2 ).dot( orig - pos );
	float c = ( orig - pos ).dot( orig - pos ) - ( radius * radius );
	t1 = ( -b + sqrt( ( b * b ) - 4 * ( a * c ) ) ) / 2 * a;
	t2 = ( -b - sqrt( ( b * b ) - 4 * ( a * c ) ) ) / 2 * a;
	if( ( b * b ) - 4 * ( a * c ) > 0 ){
		if( t1 < t2 ){
			t = t1;
		}else if( t2 >= 0 ){
			t = t2;
		}
	}

	return t;
	}
	virtual Vector3 getNormal(Vector3 p){return (p-pos).normalize();}
	virtual int getMatid(){return matid;}
private:
	Vector3 pos;
	float radius;
	int matid;
	
};
#endif
