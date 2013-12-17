#ifndef __Material
#define __Material

#include "GenVector.h"
#include "objLoader.h"

class Material
{
public:
	Material(){}
	~Material()
	{
	}
	Color getKa(){
		return ka;
	}
	Color getKs(){
		return ks;
	}
	Color getKd(){
		return kd;
	}
	double getReflect(){
		return reflect;
	}
	double getTrans(){
		return trans;
	}
	double getGlossy(){
		return glossy;
	}
	double getShiny(){
		return shiny;
	}
	double getRefract(){
		return refract;
	}
	char * getTexture(){
		return texture;
	}
	int getId(){
		return id;
	}
	Color ka;
	Color kd;
	Color ks;
	double reflect;
	double trans;
	double glossy;
	double shiny;
	double refract;
	char texture[500];
	int id;
};


#endif
