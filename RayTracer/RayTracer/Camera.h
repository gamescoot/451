#ifndef __CAMERA
#define __CAMERA

#include "GenVector.h"

class Camera
{

public:

	Camera(){}

	~Camera(){}

	Camera( Vector3 pos, Vector3 lookPt, Vector3 up )
	{

		position = pos;
		Vector3 look = ( lookPt - position );

		Vector3 w = ( -look ).normalize();
		Vector3 u = up.cross( w ).normalize();
		Vector3 v = w.cross( -u ).normalize();

		this->u = u;
		this->v = v;
		this->w = w;

	}

	Vector3 u;
	Vector3 v;
	Vector3 w;
	Vector3 position;

	private:

};
	
#endif
