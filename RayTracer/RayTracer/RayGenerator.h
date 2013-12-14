#ifndef __RayGenerator
#define __RayGenerator

#include "Ray.h"
#include "Camera.h"
#include "GenVector.h"
struct rarray{
	Ray rayArray[10000];
	unsigned w;
};
class RayGenerator
{
public:
	RayGenerator()
	{}
	RayGenerator( Camera cam, unsigned width, unsigned height )
	{
		r1 = rarray();
		r1.w=width;
		int index = 0;
		for( unsigned x = 0; x < width; x++ ){
			for( unsigned y = 0; y < height; y++ ){
				float u = -(int)width / 2 + ( width * ( x + 0.5 ) ) / width;
				float v = -(int)height / 2 + ( height * ( y + 0.5 ) ) / height;
				Vector3 dir = Vector3( u,-v,-50 )-cam.e;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				dir.normalize();
				Ray pixelRay = Ray(dir, cam.e);
				r1.rayArray[index]=pixelRay;
				index++;
			}
		}
	}

		struct rarray r1;
	Ray getRay(unsigned x, unsigned y){
		int w = r1.w;
		return r1.rayArray[w*x+y];
	}

};
#endif
