#ifndef __Shape
#define __Shape

#include "GenVector.h"
#include "Ray.h"

class Shape
{
public:
	Shape(){}
	~Shape(){}

	virtual Vector3 intersect(Ray ray)=0;
	virtual Vector3 getNormal(Vector3 pt)=0;
	virtual int getMatid()=0;
private:
};
#endif
