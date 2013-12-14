#ifndef __CAMERA
#define __CAMERA

#include "GenVector.h"

class Camera
{
public:
	Camera(){
	}
	Camera(Vector3 e1, Vector3 c1, Vector3 u1)
	{
		e=e1;
		c=c1;
		u=u1;
	}
	Vector3 e;
	Vector3 c;
	Vector3 u;
};
	

#endif
