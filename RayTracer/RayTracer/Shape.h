#ifndef __Shape
#define __Shape

#include "GenVector.h"
#include "Ray.h"


class Shape
{

public:

	Shape(){}

	~Shape(){}

	virtual float intersect( Ray ray ) = 0;

	virtual Vector3 getNormal( Vector3 p ) = 0;

	virtual int getMatid() = 0;

	virtual Shape* getBB() = 0;

private:

};

#endif
