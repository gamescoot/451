// RayTracer.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <stdio.h>
#include "objLoader.h"
#include "RayGenerator.h"
#include "Buffer.h"
#include "simplePPM.cpp"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Shape.h"
#include "Hitpoint.h"

//#include "obj_parser.cpp"
#define RES 100
Vector3 vecSqrt(Vector3 v);
int intersect( Vector3 pos, double radius, Ray r );
int triIntersect( Vector3 a, Vector3 b, Vector3 c, Ray r , Vector3 n);
Hitpoint getHitPoint(Ray r, Scene* s);
void printVector(obj_vector *v)
{
	printf("%.2f,", v->e[0] );
	printf("%.2f,", v->e[1] );
	printf("%.2f  ", v->e[2] );
}

int main(int argc, char **argv)
{
	RayGenerator generator;
	Camera cam;
	Ray r;
	Buffer b = Buffer(RES,RES);

	//load test.obj
	objLoader *objData = new objLoader();
	objData->load("spheres.obj");
	Scene s = Scene(*objData);

	cam = s.cam;
	generator = RayGenerator(cam, RES, RES);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			r = generator.getRay(x, y);
			//Vector3 d = r.getDir()*255.0f;
			//Color c = Color( abs(d[0]), abs(d[1]), abs(d[2]) );
			b.at(x,y) = Color(0,0,0);

			Hitpoint hit = getHitPoint(r,&s);
				//printf("%i\n", intersect(spherePos, radius, r));
			Vector3 pos = hit.getHit();
			if(!(pos.c[0]==0&&pos.c[1]==0&&pos.c[2]==0)){
				Material m = s.mats[hit.getMatid()];
				Color ka = m.getKa();
				Color ks = m.getKs();
				Vector3 norm = hit.getNorm()*255.0f;
				Color n = Color(abs(norm[0]), abs(norm[1]), abs(norm[2]));
				b.at(x,y)=ka;
			}
		}

	}

	simplePPM_write_ppm("rainboj.ppm", b.getWidth(), b.getHeight(), (unsigned char*)&b.at(0,0));
}
Hitpoint getHitPoint(Ray r, Scene* s){
	std::vector<Shape *> shapes = (*s).shapes;
	Hitpoint p = Hitpoint(shapes[0]->intersect(r), shapes[0]->getMatid(),shapes[0]->getNormal(shapes[0]->intersect(r)).normalize());
	for( int sInd=0; sInd < shapes.size(); sInd++ ){
		Shape * currentShape = shapes[sInd];
		Hitpoint hit = Hitpoint(currentShape->intersect(r), currentShape->getMatid(), (currentShape->getNormal(Vector3(0,0,0))));
		if(p.getHit().c[0]>hit.getHit().c[0]&&Vector3(0,0,0).length()< hit.getHit().length()){
			p = hit;
		}
	}
	Vector3 pos = p.getHit();
	if(!(pos.c[0]==0&&pos.c[1]==0&&pos.c[2]==0)){
			return p;
	}
	return Hitpoint(Vector3(0,0,0), -1, Vector3(1,0,0));
}




