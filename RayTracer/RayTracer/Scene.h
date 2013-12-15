#ifndef __Scene
#define __Scene

#include <vector>
#include "GenVector.h"
#include "objLoader.h"
#include "Material.h"
#include "Shape.h"
#include "Sphere.h"
#include "Triangle.h"

class Scene
{
public:
	Scene( objLoader obj ){
		shapes = std::vector<Shape*>();
		mats = std::vector<Material>();
		if( obj.sphereList != NULL ){
			for( int sInd = 0; sInd < obj.sphereCount; sInd++ ){

				obj_sphere *spheres = obj.sphereList[sInd];
				Vector3 spherePos = toVector3( *obj.vertexList[ spheres->pos_index ] ); 
				
				float radius;
				Vector3 rad = toVector3( *obj.normalList[spheres->up_normal_index] );
				radius = rad.length();

				Sphere* s = new Sphere(spherePos, radius);
				shapes.push_back(s);
			}
		}

		for( int tInd = 0; tInd < obj.faceCount; tInd++ ){
				obj_face *triangles = obj.faceList[tInd];

				Vector3 triPos1 = toVector3( *obj.vertexList[ triangles->vertex_index[0] ] ).normalize();
				Vector3 triPos2 = toVector3( *obj.vertexList[ triangles->vertex_index[1] ] ).normalize(); 
				Vector3 triPos3 = toVector3( *obj.vertexList[ triangles->vertex_index[2] ] ).normalize();
							
				Triangle* t = new Triangle( triPos1, triPos2, triPos3 );
				shapes.push_back(t);
		}

		for(int i = 0; i < obj.materialCount; i++){
			obj_material * matLoad = obj.materialList[i];
			Material mtl = Material();
			float kaRed = matLoad->amb[0];
			float kaGreen = matLoad->amb[1];
			float kaBlue = matLoad->amb[2];

			mtl.ka = Color( kaRed, kaGreen, kaBlue );

			float kdRed = matLoad->diff[0];
			float kdGreen = matLoad->diff[1];
			float kdBlue = matLoad->diff[2];

			mtl.kd = Color( kdRed, kdGreen, kdBlue );

			float ksRed = matLoad->spec[0];
			float ksGreen = matLoad->spec[1];
			float ksBlue = matLoad->spec[2];

			mtl.ks = Color( ksRed, ksGreen, ksBlue );

			mtl.glossy = matLoad->glossy;

			mtl.trans = matLoad->trans;

			mtl.reflect = matLoad->reflect;

			mtl.refract = matLoad->refract;

			mtl.shiny = matLoad->shiny;

			mtl.texture[500] = *matLoad->texture_filename;
			
			mats.push_back( mtl );

		}
	}
	Scene(){}
	std::vector<Shape*> shapes;
	std::vector<Material> mats;
};
#endif
