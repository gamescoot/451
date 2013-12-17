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
