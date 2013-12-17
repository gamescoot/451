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
		matid=mid;
	}
	~Sphere(){}

	virtual Vector3 intersect( Ray r ){
	Vector3 t1;
	Vector3 t2;
	Vector3 a = r.getDir().dot( r.getDir() );
	Vector3 b = r.getDir().dot( r.getOrig() - pos );
	Vector3 c = ( r.getOrig() - pos ).dot( r.getOrig() - pos ) - ( radius * radius );
	t1 = ( -b + vecSqrt( ( b * b ) - a * c ) ) / a;
	t2 = ( -b - vecSqrt( ( b * b ) - a * c ) ) / a;
		if( ( ( b * b ) - a * c ).c[2] >= 0 ){
			if(t1.length()>t2.length()){
				this->norm = (r.getOrig()+r.getDir()*t1)/radius-pos;
				return t1;
			}
			this->norm = ((r.getOrig()+r.getDir()*t2)/radius-pos);
			return t2;
		}
	return Vector3(0,0,0);
	}
	virtual Vector3 getNormal( Vector3 pt ){return norm;}
	virtual int getMatid(){return matid;}
private:
	Vector3 pos;
	float radius;
	int matid;
	Vector3 norm;
	
};
#endif
