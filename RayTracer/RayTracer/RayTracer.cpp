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

#define RES 300
#define MAXI 99999

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
	simplePPM_write_ppm( "output.ppm", b.getWidth(), b.getHeight(), ( unsigned char* ) &b.at( 0, 0 ) );

}

Hitpoint getHitPoint( Ray r, Scene* s )
{
	int newMatid;
	std::vector<Shape *> shapes = ( *s ).shapes;
	TreeNode* root = s->root;
	Hitpoint hit;
	float t = traverseSceneTree(root, r);
	hit.setHit(t);
	hit.setMatid(r.mid);
	hit.setNorm(r.norm);
			//printf("%d\n", hit.getMatid());
	return hit;
}

Color getColor( Ray r, Scene s ){

	Vector3 pixelColor = Vector3( abs(r.getDir()[0]*255.0f), abs(r.getDir()[1]*255.0f), abs(r.getDir()[2]*255.0f) );
	int amountShapes = s.shapes.size();
	bool shapesExist = amountShapes > 0;
	if( shapesExist ){

		Hitpoint hit = getHitPoint( r, &s );
		int hitMaterial = hit.getMatid();

		//if hit
		bool didItHit = hit.getHit()!=-1;

		if( didItHit ){

			pixelColor = Vector3(0,0,0);
			
			Material m = s.mats[ 1 ];//hitMaterial ];
			float sceneSpecificScale = 50;

			Vector3 orig = r.getOrig();
			Vector3 dir = r.getDir();

			float hitDist = hit.getHit();
			Vector3 hitPoint = orig + hitDist * dir;
			Vector3 hitNorm = hit.getNorm();

			//getRayColor()
			//write Color to buffer
			for( int i = 0; i < s.lightMats.size(); i++ ){

				Material lmat = s.lightMats[i];
				Vector3 lightPos = s.lights[i];

				Vector3 lightVec = ( lightPos - hitPoint + hitNorm / 1000 );
				float lightLen = lightVec.length();
				lightVec = lightVec.normalize();
		
				float cosine = hitNorm.dot( lightVec );
				Vector3 look = ( hitPoint  - orig ).normalize();
				Vector3 reflect = ( 2 * lightVec.dot( hitNorm ) ) * hitNorm - lightVec;
				float halfVector = pow( ( look ).dot( reflect ), 2 );//m.getShiny() );

				Vector3 ambientColor = lmat.getIa() * m.getIa();
				Vector3 diffuseColor = m.getId() * cosine * lmat.getId();
				Vector3 specularColor = m.getIs() * halfVector * lmat.getIs();

				pixelColor = pixelColor + ambientColor;
		
				Ray r = Ray( lightVec, hitPoint + hitNorm / 1000 );
				Hitpoint shadow = getHitPoint( r, &s );
				float shadowDist = shadow.getHit();

				bool notInShadow = !( shadowDist > 0 && shadowDist < lightLen);

				if( notInShadow ){

					pixelColor = pixelColor  + diffuseColor  + specularColor;

				}

			}

			//Vector that will bounce
			Vector3 toBounce = hitPoint - orig;
			//Reflected Vector
			Vector3 bounce = ( toBounce.dot( hitNorm ) * hitNorm * ( -2 ) + toBounce );
			//Hitpoint of what the reflected vector hits
			Hitpoint reflection = getHitPoint( Ray( bounce, hitPoint ), &s );

			//if the vector hits anything
			bool didReflHit = reflection.getHit() > 0 && reflection.getHit() < MAXI;
			if( didReflHit ){

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