#ifndef __Hitpoint
#define __Hitpoint

#include "GenVector.h"
#include "Material.h"

class Hitpoint
{
public:

	Hitpoint(){}

	Hitpoint( float loc, int mid, Vector3 n )
	{

		location = loc;
		matid = mid;
		norm = n;
	
	}

	float getHit(){
	
		return location;
	
	}

	int getMatid(){
	
		return matid;
	
	}

	Vector3 getNorm(){
	
		return norm;
	
	}

	void setHit( float loc ){
	
		location = loc;
	
	}

	void setMatid( int mid ){
	
		matid = mid;
	
	}

	void setNorm( Vector3 n ){
	
		norm = n;
	
	}

private:

	float location;
	int matid;
	Vector3 norm;

};

#endif
