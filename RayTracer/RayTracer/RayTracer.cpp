// Obj_loader.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "objLoader.h"
#include "RayGenerator.h"
#include "Buffer.h"
#include "simplePPM.cpp"
#include "Camera.h"
//#include "obj_parser.cpp"
#define RES 100
Vector3 vecSqrt(Vector3 v);
int intersect( Vector3 pos, double radius, Ray r );
int triIntersect( Vector3 a, Vector3 b, Vector3 c, Ray r , Vector3 n);
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
	objData->load("day4.obj");

	printf("Number of vertices: %i\n", objData->vertexCount);
	printf("Number of vertex normals: %i\n", objData->normalCount);
	printf("Number of texture coordinates: %i\n", objData->textureCount);
	printf("\n");
	printf("Number of spheres: %i\n", objData->sphereCount);
	for(int i=0; i<objData->sphereCount; i++)
	{
		obj_sphere *o = objData->sphereList[i];
		printf(" sphere ");
		printVector(objData->vertexList[ o->pos_index ]);
		//printVector(objData->normalList[ o->up_normal_index ]);
		//printVector(objData->normalList[ o->equator_normal_index ]);
		printf("\n");
	}

	printf("\n");
	//create camera
	cam = Camera(Vector3(0,0,0),Vector3(0,0,1),Vector3(0,1,0));
	

	//glm::vec3 e,c,u;
	//e = glm::vec3(objData->vertexList[camera->camera_pos_index][0],objData->vertexList[camera->camera_pos_index][1],objData->vertexList[camera->camera_pos_index][2]);
	//c = glm::vec3(objData->vertexList[camera->camera_look_point_index][0],objData->vertexList[camera->camera_look_point_index][1],objData->vertexList[camera->camera_look_point_index][2]);
	//u = glm::vec3(objData->vertexList[camera->camera_up_norm_index][0],objData->vertexList[camera->camera_up_norm_index][1],objData->vertexList[camera->camera_up_norm_index][2]);
	//glm::mat4 cam = glm::lookAt(e, c, u);]
	generator = RayGenerator(cam, RES, RES);
	for(int y=0; y<RES; y++)
	{
		for(int x=0; x<RES; x++)
		{
			r = generator.getRay(x, y);
			Vector3 d = r.getDir()*255.0f;
			Color c = Color( abs(d[0]), abs(d[1]), abs(d[2]) );
			b.at(x,y) = c;
			for( int sInd=0; sInd < objData->sphereCount; sInd++ ){
				obj_sphere *spheres = objData->sphereList[sInd];
				Vector3 spherePos=Vector3(objData->vertexList[ spheres->pos_index ]->e[0],objData->vertexList[ spheres->pos_index ]->e[1],objData->vertexList[ spheres->pos_index ]->e[2]); 
				double radius;
				obj_vector rad = *objData->normalList[spheres->up_normal_index];
				radius = rad.e[1];
		
				//printf("%i\n", intersect(spherePos, radius, r));
				if(intersect(spherePos, radius, r)){
					b.at(x,y)=Color(0,0,0);
				}
			}
			for( int tInd = 0; tInd < objData->faceCount; tInd++ ){
				obj_face *triangles = objData->faceList[tInd];
				Vector3 triPos1 = Vector3(objData->vertexList[ triangles->vertex_index[0] ]->e[0],objData->vertexList[ triangles->vertex_index[0] ]->e[1], objData->vertexList[ triangles->vertex_index[0] ]->e[2] ).normalize();
				Vector3 triPos2 = Vector3(objData->vertexList[ triangles->vertex_index[1] ]->e[0],objData->vertexList[ triangles->vertex_index[1] ]->e[1], objData->vertexList[ triangles->vertex_index[1] ]->e[2] ).normalize();
				Vector3 triPos3= Vector3(objData->vertexList[ triangles->vertex_index[2] ]->e[0],objData->vertexList[ triangles->vertex_index[2] ]->e[1], objData->vertexList[ triangles->vertex_index[2] ]->e[2] ).normalize();
					Vector3 norm = (triPos1-triPos2).cross(triPos2-triPos3).normalize();
					if(triIntersect(triPos1, triPos2, triPos3, r,norm)){
						b.at(x,y)=Color(0,0,0);
					}
			}
	
		}
	}

	simplePPM_write_ppm("rainboj.ppm", b.getWidth(), b.getHeight(), (unsigned char*)&b.at(0,0));
}
int intersect( Vector3 pos, double radius, Ray r ){
	Vector3 t1;
	Vector3 t2;
	Vector3 a=r.getDir().dot(r.getDir());
	Vector3 b=r.getDir().dot(r.getOrig()-pos);
	Vector3 c=(r.getOrig()-pos).dot(r.getOrig()-pos)-(radius*radius);
	t1=(-b+vecSqrt((b*b)-a*c))/a;
	t2=(-b-vecSqrt((b*b)-a*c))/a;
	//printf("t1:%d %d %d\n", t1.c[0],t1.c[1],t1.c[2]);
	//printf("t2:%d %d %d\n", t2.c[0],t2.c[1],t2.c[2]);
	if(((b*b)-a*c).c[2]>=0){
		//printf("blah:%d %d %d\n", ((b*b)-a*c).c[0],((b*b)-a*c).c[1],((b*b)-a*c).c[1]);
		return 1;
	}
	return 0;
}
int triIntersect( Vector3 a, Vector3 b, Vector3 c, Ray r , Vector3 n){
	Vector3 x = r.getDir();
	Vector3 test1 = (b-a).cross(x-a).dot(n);
	Vector3 test2 = (c-b).cross(x-b).dot(n);
	Vector3 test3 = (a-c).cross(x-c).dot(n);
	if( test1.c[1]>0){
		if(test2.c[1]>0){
			if(test3.c[1]>0){
				return 1;
			}
		}
	}
	return 0;
}
Vector3 vecSqrt(Vector3 v){
	Vector3 newV;
	newV.c[0]=sqrt(v.c[0]);
	newV.c[1]=sqrt(v.c[1]);
	newV.c[2]=sqrt(v.c[2]);
	return newV;

}



