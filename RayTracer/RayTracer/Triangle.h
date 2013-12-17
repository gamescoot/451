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

	Triangle( Vector3 pos1, Vector3 pos2, Vector3 pos3, int mid){
	vertex1 = pos1;
	vertex2 = pos2;
	vertex3 = pos3;
	norm = (vertex1-vertex3).cross(vertex2-vertex1).normalize();
	matid=mid;
	}	
	~Triangle(){}

	virtual Vector3 intersect( Ray r ){
		Vector3 top = norm.dot(vertex1-r.getOrig());
		Vector3 bottom = r.getDir().dot(norm);
		Vector3 intersectionParam = top/bottom;

		if(intersectionParam[0]<0){
			return -1.0;
		}

		Vector3 rayPoint = r.getOrig()+intersectionParam*r.getDir();

		Vector3 test1 = (vertex2-vertex1).cross(rayPoint-vertex1).dot(norm);
		Vector3 test2 = (vertex3-vertex2).cross(rayPoint-vertex2).dot(norm);
		Vector3 test3 = (vertex1-vertex3).cross(rayPoint-vertex3).dot(norm);
		if( test1.c[0]>0){
			if(test2.c[0]>0){
				if(test3.c[0]>0){
					return intersectionParam;
				}
			}
		}
		return Vector3(0,0,0);
	}
	virtual Vector3 getNormal(Vector3 pt){ return norm; }
	virtual int getMatid(){return matid;}
	
private:
	Vector3 vertex1;
	Vector3 vertex2;
	Vector3 vertex3;
	Vector3 norm;
	int matid;
};
#endif
