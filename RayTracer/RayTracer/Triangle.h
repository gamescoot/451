#ifndef __Triangle
#define __Triangle

#include "GenVector.h"
#include "objLoader.h"
#include "Material.h"
#include "Ray.h"
#include "Shape.h"
#include <vector>

class Triangle : public Shape{

public:

	Triangle( Vector3 pos1, Vector3 pos2, Vector3 pos3 ){
	vertex1 = pos1;
	vertex2 = pos2;
	vertex3 = pos3;
	norm = (vertex1-vertex2).cross(vertex2-vertex3).normalize();
	}	
	~Triangle(){}

	virtual Vector3 intersect( Ray r ){
		Vector3 x = r.getDir();
		Vector3 test1 = (vertex2-vertex1).cross(x-vertex1).dot(norm);
		Vector3 test2 = (vertex3-vertex2).cross(x-vertex2).dot(norm);
		Vector3 test3 = (vertex1-vertex3).cross(x-vertex3).dot(norm);
		if( test1.c[1]>0){
			if(test2.c[1]>0){
				if(test3.c[1]>0){
					Vector3 numerator = (vertex1-r.getOrig()).dot(norm);
					Vector3 denominator = r.getDir().dot(norm);
					Vector3 t = numerator/denominator;
					return t;
				}
			}
		}
		return Vector3(0,0,0);
	}
	virtual Vector3 getNormal(Vector3 pt){ return norm; }

private:
	Vector3 vertex1;
	Vector3 vertex2;
	Vector3 vertex3;
	Vector3 norm;
};
#endif
