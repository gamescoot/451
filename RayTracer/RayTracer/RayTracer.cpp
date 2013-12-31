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

#define RES 100
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
Hitpoint getHitPoint( Ray r, Scene* s ){
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
Color getColor( Ray r, Scene s ){
	Vector3 pixelColor = Vector3( 0, 0, 0 );
	int amountShapes = s.shapes.size();

	if( amountShapes > 0 ){
		Hitpoint hit = getHitPoint( r, &s );
		int hitMaterial = hit.getMatid();

		//if hit
		if(hitMaterial >= -1 ){
			Material m = s.mats[ hitMaterial ];
			//getRayColor()
			//write Color to buffer
			for( int i = 0; i < s.lightMats.size(); i++ ){
				Material lmat = s.lightMats[i];
				Vector3 lightPos = s.lights[i];
				Vector3 lightVec = ( lightPos - ( r.getOrig() + hit.getHit() * ( r.getDir() ) ) ).normalize();
		
				float cosine = lightVec.dot( hit.getNorm() );
				Vector3 look = ( ( s.cam.position + hit.getHit() * ( r.getDir() ) ) - s.cam.position ).normalize();
				Vector3 reflect = ( ( 2 * lightVec.dot( hit.getNorm() ) ) * hit.getNorm() - lightVec );
				float halfVector = pow( ( look ).dot( reflect ), 2 );

				Vector3 ambientColor = lmat.getIa() * m.getIa();
				Vector3 diffuseColor = m.getId() * cosine * lmat.getId();
				Vector3 specularColor = m.getIs() * halfVector * lmat.getIs();
				pixelColor = pixelColor + ambientColor;
		
				
				Ray r = Ray( lightVec, s.cam.position + hit.getHit() * ( r.getDir() ) );
				Hitpoint shadow = getHitPoint( r, &s );
				if( !(shadow.getHit() >= 0 && shadow.getHit() < 10000) ){
					pixelColor = pixelColor + specularColor + diffuseColor;
				}
				Vector3 toBounce = ( s.cam.position + hit.getHit() * ( r.getDir() ) ) - s.cam.position;
				Vector3 negNorm = -hit.getNorm();
				Vector3 bounce = -2 * ( toBounce.dot( negNorm ) ) * negNorm + toBounce;

				Hitpoint reflection = getHitPoint( Ray( bounce, ( s.cam.position + hit.getHit() * ( r.getDir() ) ) ), &s );
				if( reflection.getHit() >= 0 && reflection.getHit() < 10000 ){
					Material mmm = s.mats[reflection.getMatid()];
					//pixelColor =  pixelColor * ( 1 - m.reflect ) + mmm.getIa() * m.reflect;
					}
			}
			float sceneSpecificScale = 10;
			pixelColor = pixelColor * sceneSpecificScale;
			
			//pixelColor = Vector3( abs( hit.getNorm()[0] ), abs( hit.getNorm()[1] ), abs( hit.getNorm()[2] ) );
			
			return Color( pixelColor[0], pixelColor[1], pixelColor[2] );
			return Color( 0, 0, 0 );
		}
	}
	//else is miss
	//return background color
	Vector3 d = r.getDir() * 255.0;
	Color charColor = Color( abs( d[0] ), abs( d[1] ), abs( d[2] ) );
	return charColor;
}