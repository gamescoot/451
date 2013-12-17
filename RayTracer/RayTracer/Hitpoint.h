#ifndef __Hitpoint
#define __Hitpoint

#include "GenVector.h"
#include "Material.h"

class Hitpoint
{
public:
	Hitpoint(){}
	Hitpoint(Vector3 loc, int mid)
	{
		location = loc;
		matid = mid;
	}
	Vector3 getHit(){
		return location;
	}
	int getMatid(){
		return matid;
	}
private:
	Vector3 location;
	int matid;
};


#endif
