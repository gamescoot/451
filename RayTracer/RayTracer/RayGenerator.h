#ifndef __RayGenerator
#define __RayGenerator

#include "Ray.h"
#include "Camera.h"
#include "GenVector.h"
#include <vector>
struct rarray{
	std::vector<Ray>  rayArray;
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
				float u = - (int) width / 2 + ( x + 0.5 );
				float v = - (int) height / 2 + ( y + 0.5 );
				Vector3 dir = ( -0.6f * width * cam.w + u * cam.u + v * cam.v ).normalize();
				Ray pixelRay = Ray( dir, cam.position );
				r1.rayArray.push_back(pixelRay);
				index++;
			}
		}
	}

		struct rarray r1;
	Ray getRay( unsigned x, unsigned y ){
		int w = r1.w;
		return r1.rayArray[ w * x + y ];
	}

};
#endif
