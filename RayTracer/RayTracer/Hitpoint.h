#ifndef __Hitpoint
#define __Hitpoint

#include "GenVector.h"
#include "Material.h"

class Hitpoint
{
public:
	Hitpoint(){}
	Hitpoint(Vector3 loc, int mid, Vector3 n)
	{
		location = loc;
		matid = mid;
		norm = n;
	}
	Vector3 getHit(){
		return location;
	}
	int getMatid(){
		return matid;
	}
	Vector3 getNorm(){
		return norm;
	}
private:
	Vector3 location;
	int matid;
	Vector3 norm;
};


#endif
