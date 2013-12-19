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
#include <math.h>

//#include "obj_parser.cpp"
#define RES 100
Hitpoint getHitPoint(Ray r, Scene* s);
Color getColor(Ray r, Scene s);
void printVector(obj_vector *v)
{
	printf("%.2f,", v->e[0] );
	printf("%.2f,", v->e[1] );
	printf("%.2f  ", v->e[2] );
}

int main(int argc, char **argv)
{
	//Generate Framebuffer
	Buffer b = Buffer(RES,RES);
	RayGenerator generator;
	Camera cam;
	Ray r;
	

	//compute camera Data
	objLoader *objData = new objLoader();
	objData->load("spheres.obj");
	Scene s = Scene(*objData);
	cam = s.cam;

	//for each pixel
	//generate rays for pixel
	generator = RayGenerator(cam, RES, RES);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			//for each ray
			//getColor()
			r = generator.getRay(x, y);
			Color charColor = getColor(r,s);

			//write color to buffer
			b.at(x,y) = charColor;
		}

	}
	//save image
	simplePPM_write_ppm("rainboj.ppm", b.getWidth(), b.getHeight(), (unsigned char*)&b.at(0,0));
}
Hitpoint getHitPoint(Ray r, Scene* s){
	std::vector<Shape *> shapes = (*s).shapes;
	Hitpoint hit = Hitpoint(shapes[0]->intersect(r), shapes[0]->getMatid(), shapes[0]->getNormal(r.getOrig()+r.getDir()*shapes[0]->intersect(r)));
	float t = hit.getHit();

	//for each object in scenelist
	for( int sInd=1; sInd < shapes.size(); sInd++ ){
		Shape * currentShape = shapes[sInd];
		float newt = currentShape->intersect(r);
		int newMatid = currentShape->getMatid();
		Vector3 newNorm = currentShape->getNormal((r.getOrig()+r.getDir()*newt));
		
		Hitpoint newHit = Hitpoint(newt, newMatid, newNorm);
		//test intersect closer
		if( newt < t && newt > 0){
			hit = newHit;
			t = hit.getHit();
		}
	}
	//return hit data
	if( t >= 0 && t<10000){
			return hit;
	}
	
	return Hitpoint(-1, -2, Vector3(1,0,0));
}
Color getColor(Ray r, Scene s){
	int amountShapes = s.shapes.size();

	if(amountShapes>0){
		Hitpoint hit = getHitPoint(r,&s);
		int hitMaterial = hit.getMatid();


		//if hit
		if(hitMaterial>=-1){
			Material m = s.mats[hit.getMatid()];
			//getRayColor()
			//write Color to buffer
			Vector3 pixelColor = Vector3(0,0,0);
			for(int i = 0; i< s.lightMats.size(); i++){
				Material lmat = s.lightMats[i];
				Vector3 lightVec = s.lights[i];
				lightVec = (lightVec-(s.cam.position +hit.getHit()*(-s.cam.w))).normalize();
		
				float cosine = hit.getNorm().dot(lightVec);
				Vector3 look = s.cam.position-(s.cam.position+hit.getHit()*(-s.cam.w)).normalize();
				Vector3 reflect = 2*(lightVec.dot(hit.getNorm()))*hit.getNorm()-lightVec;
				reflect=Vector3(pow(reflect[0], m.getShiny()), pow(reflect[1],m.getShiny()), pow(reflect[2],m.getShiny()));
				float halfVector = (look).dot(reflect);
				Vector3 ambientColor = lmat.getIa()*m.getIa();
				Vector3 diffuseColor = lmat.getId()*m.getId()*cosine;
				Vector3 specularColor = lmat.getIs()*m.getIs()*halfVector;
				pixelColor = pixelColor +ambientColor+diffuseColor ;
		
		
				Ray r = Ray(lightVec, s.cam.position+hit.getHit()*(-s.cam.w));
				Hitpoint shadow = getHitPoint(r, &s);
				if(shadow.getHit()>=0&&shadow.getHit()<10000){
					pixelColor=pixelColor+specularColor;
				}
				Vector3 toBounce = (s.cam.position+hit.getHit()*(-s.cam.w))-s.cam.position;
				Vector3 negNorm = -hit.getNorm();
				Vector3 bounce = -2*(toBounce.dot(negNorm))*negNorm + toBounce;

				Hitpoint reflection = getHitPoint(Ray(bounce,(s.cam.position+hit.getHit()*(-s.cam.w))), &s);
				if(reflection.getHit()>=0&&reflection.getHit()<10000){
					Material mmm = s.mats[hit.getMatid()];
					pixelColor=(Vector3(1,1,1)-m.reflect)*pixelColor+mmm.getIa()*m.reflect;
					}
			}
			float sceneSpecificScale = 10.0f;
			//pixelColor=m.ia+(hit.getNorm().dot(s.lights[1]-hit.getHit())*m.id*s.lightMats[1].getId()+m.is*s.lightMats[1].getIs()*hit.getNorm().dot(s.cam.position-(s.cam.position+hit.getHit()*(-s.cam.w))));
			pixelColor=pixelColor*sceneSpecificScale;
			return Color(pixelColor[0],pixelColor[1],pixelColor[2]);
		}
	}
	Vector3 d = r.getDir().normalize()*255.0;
	Color charColor = Color( abs(d[0]), abs(d[1]), abs(d[2]) );
	return charColor;
}