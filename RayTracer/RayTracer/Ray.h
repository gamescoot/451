#ifndef __Ray
#define __Ray

#include "GenVector.h"
#include "GenVector.h"

class Ray
{

public:

	Ray(){}
	
	Ray( Vector3 dir, Vector3 orig )
	{

		direction = dir;
		origin = orig;
	
	}

	Vector3 getDir(){
	
		return direction;
	
	}

	Vector3 getOrig(){
	
		return origin;
	
	}
	int mid;
	Vector3 norm;
private:

	Vector3 direction;
	Vector3 origin;

};

#endif
