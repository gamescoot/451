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

	Triangle( Vector3 pos1, Vector3 pos2, Vector3 pos3, int mid ){

	vertex1 = pos1;
	vertex2 = pos2;
	vertex3 = pos3;
	matid = mid;

	}	

	~Triangle(){}

	virtual float intersect( Ray r ){

		Vector3 dir = r.getDir().normalize();
		Vector3 orig = r.getOrig();
		Vector3 norm = getNormal( Vector3( 0, 0, 0 ) );

		float top = ( vertex1 - orig ).dot( norm );
		float bottom = dir.dot( norm );
		float intersectionParam = top / bottom;

		if( intersectionParam < 0 )
		{

			return -2.0;

		}

		Vector3 rayPoint = orig + intersectionParam * dir;

		float test1 = ( vertex2 - vertex1 ).cross( rayPoint - vertex1 ).dot( norm );
		float test2 = ( vertex3 - vertex2 ).cross( rayPoint - vertex2 ).dot( norm );
		float test3 = ( vertex1 - vertex3 ).cross( rayPoint - vertex3 ).dot( norm );

		if( test1 > 0 )
		{

			if( test2 > 0 )
			{

				if( test3 > 0 )
				{

					return intersectionParam;

				}

			}
		
		}

		return -1;
	
	}
	
	virtual Vector3 getNormal( Vector3 p ){return ( vertex1 - vertex3 ).cross( vertex2 - vertex1 ).normalize(); }
	
	virtual int getMatid(){return matid;}
	
private:

	Vector3 vertex1;
	Vector3 vertex2;
	Vector3 vertex3;
	
	int matid;

};

#endif
