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

#define RES 200
Hitpoint getHitPoint( Ray r, Scene* s );
Color getColor( Ray r, Scene s );

void printVector( obj_vector *v )
{

	printf( "%.2f,", v->e[0] );
	printf( "%.2f,", v->e[1] );
	printf( "%.2f  ", v->e[2] );

}

int main( int argc, char **argv )
{

	//Generate Framebuffer
	Buffer b = Buffer( RES, RES );
	RayGenerator generator;
	Camera cam;
	Ray r;
	

	//compute camera Data
	objLoader *objData = new objLoader();
	objData->load( "spheres.obj" );
	Scene s = Scene( *objData );
	cam = s.cam ;

	//for each pixel
	//generate rays for pixel
	generator = RayGenerator( cam, RES, RES );

	for( int y = 0; y < RES ; y++ )
	{

		for( int x = 0; x < RES; x++ )
		{

			//for each ray
			//getColor()
			r = generator.getRay( x, y );
			
			Color charColor = getColor( r, s );

			//write color to buffer
			b.at( x, y ) = charColor;

		}

	}

	//save image
	simplePPM_write_ppm( "rainboj.ppm", b.getWidth(), b.getHeight(), ( unsigned char* ) &b.at( 0, 0 ) );

}

Hitpoint getHitPoint( Ray r, Scene* s )
{

	std::vector<Shape *> shapes = ( *s ).shapes;
	Hitpoint hit;
	float t;
	Vector3 origin = r.getOrig();
	Vector3 dir = r.getDir();

	//for each object in scenelist
	for( int sInd=0; sInd < shapes.size(); sInd++ ){

		Shape * currentShape = shapes[ sInd ];
		float newt = currentShape->intersect( r );
		int newMatid = currentShape->getMatid();
		Vector3 newNorm = currentShape->getNormal( origin + newt * dir );
		
		Hitpoint newHit = Hitpoint( newt, newMatid, newNorm );
		//test intersect closer
		if( sInd == 0 || newt < t && newt > 0 ){

			hit = newHit;
			t = newt;

		}

	}

	//return hit data
	if( t >= 0 && t < 10000 ){

			return hit;

	}
	
	return Hitpoint( -1, -2, Vector3( 1, 0, 0 ) );
}

Color getColor( Ray r, Scene s )
{

	Vector3 pixelColor = Vector3( 0, 0, 0 );
	int amountShapes = s.shapes.size();

	if( amountShapes > 0 ){

		Hitpoint hit = getHitPoint( r, &s );
		int hitMaterial = hit.getMatid();

		//if hit
		if(hitMaterial >= -1 ){

			Material m = s.mats[ hitMaterial ];
			float sceneSpecificScale = 10;

			Vector3 orig = r.getOrig();
			Vector3 dir = r.getDir();

			float hitDist = hit.getHit();
			Vector3 hitPoint = orig + hit.getHit() * dir;
			Vector3 hitNorm = hit.getNorm();

			float max = 10000;

			//getRayColor()
			//write Color to buffer
			for( int i = 0; i < s.lightMats.size(); i++ ){

				Material lmat = s.lightMats[i];
				Vector3 lightPos = s.lights[i];
				Vector3 lightVec = ( lightPos - hitPoint ).normalize();
		
				float cosine = lightVec.dot( hitNorm );
				Vector3 look = ( hitPoint  - orig ).normalize();
				Vector3 reflect = ( 2 * lightVec.dot( hitNorm ) ) * hitNorm - lightVec;
				float halfVector = pow( ( look ).dot( reflect ), 2 );//m.getShiny() );

				Vector3 ambientColor = lmat.getIa() * m.getIa();
				Vector3 diffuseColor = m.getId() * cosine * lmat.getId();
				Vector3 specularColor = m.getIs() * halfVector * lmat.getIs();

				pixelColor = pixelColor + ambientColor;
		
				Ray r = Ray( lightVec, hitPoint );
				Hitpoint shadow = getHitPoint( r, &s );
				float shadowDist = shadow.getHit();

				if( !( shadowDist >= 0 && shadowDist < max ) ){

					pixelColor = pixelColor + specularColor + diffuseColor;

				}

			}

			//Vector that will bounce
			Vector3 toBounce = hitPoint - orig;
			//Reflected Vector
			Vector3 bounce = ( toBounce.dot( hitNorm ) * hitNorm * ( -2 ) + toBounce );
			//Hitpoint of what the reflected vector hits
			Hitpoint reflection = getHitPoint( Ray( bounce, hitPoint ), &s );

			//if the vector hits anything
			if( reflection.getHit() >= 0 && reflection.getHit() < max ){

				//get the material info of hit object
				Material reflectMat = s.mats[ reflection.getMatid() ];
				Vector3 reflectIa = reflectMat.getIa();
				double refAmount = m.reflect;

				//change the pixel color according to the reflection
				pixelColor =  pixelColor * ( 1 - refAmount ) + reflectIa * refAmount;

			}

			pixelColor = pixelColor * sceneSpecificScale;

		}
	}

	//else is miss
	//return background color
	Color charColor = Color( pixelColor[0], pixelColor[1], pixelColor[2] );
	return charColor;

}