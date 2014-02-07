#ifndef __AABB
#define __AABB

#include <vector>
#include <algorithm>
#include "GenVector.h"
#include "Ray.h"
#include "Shape.h"
#include "Hitpoint.h"

class AABB : public Shape{

public:

	AABB( Vector3 corner1, Vector3 corner2, int mid, std::vector<Shape*> l )
	{
		list = l;
		matid = mid;
		if( corner1[0] < corner2[0] ){
			bbMin[0] = corner1[0];
			bbMax[0] = corner2[0];
		}
		else{
			bbMin[0] = corner2[0];
			bbMax[0] = corner1[0];
		}
		if( corner1[1] < corner2[1] ){
			bbMin[1] = corner1[1];
			bbMax[1] = corner2[1];
		}
		else{
			bbMin[1] = corner2[1];
			bbMax[1] = corner1[1];
		}
		if( corner1[2] < corner2[2] ){
			bbMin[2] = corner1[2];
			bbMax[2] = corner2[2];
		}
		else{
			bbMin[2] = corner2[2];
			bbMax[2] = corner1[2];
		}
		max = -1;
	}
	AABB(){

	}
	~AABB(){}
	int max;

	virtual float intersect( Ray r )
	{

		//we want to find the farthest entrace and closest exit to the box
		//if the exit is closer than the entrance, there is no hit
		const size_t vecDim = 3;
		float entrance = 0.0f;
		float exit =  9999;
		Vector3 normal = Vector3(0,0,0);
	
		for(int i=0; i<vecDim; i++)
		{
			float slabA = bbMin[i];
			float slabB = bbMax[i];
			float invDir = 1.0f / r.getDir()[i];
			float origin = r.getOrig()[i];
		
			float closestHit = (slabA - origin) * invDir;
			float farthestHit = (slabB - origin) * invDir;
		
			if(farthestHit < closestHit)
				std::swap(closestHit, farthestHit);
		
			bool tooClose = farthestHit < entrance;
			bool tooFar = closestHit > exit;
		
			if(tooClose || tooFar)
				return -1;
		
			bool foundNewEntrance = closestHit > entrance;
			entrance = foundNewEntrance ? closestHit : entrance;
		
			bool foundNewExit = farthestHit < exit;
			exit = foundNewExit ? farthestHit : exit;
		
			if(foundNewEntrance)
			{
				normal = Vector3(0,0,0);
				normal[i] = r.getDir()[i] * -1;
				normal.normalize();
			}
		}
		norm = normal;
	
		return entrance;

	}

	virtual Vector3 getNormal( Vector3 p ){ return Vector3(0,0,0) ; }

	virtual int getMatid(){ return matid; }

	virtual Shape* getBB()
	{ 
		std::vector<Shape*> list;
		AABB bb = AABB(Vector3(0,0,0),Vector3(1,1,1),1, list);
		return &bb;
	}

	int matid;

	float bbMin[3];
	float bbMax[3];
	std::vector<Shape*> list;
private:


	Vector3 norm;
	
};

#endif
