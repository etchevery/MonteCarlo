#ifndef SPHERE_H
#define SPHERE_H

#include "Objet.h"
#include <tinyxml.h>
#include <iostream>
#include "Intersection.h"
#include "Rayon.h"

using namespace std;

class Sphere : public Objet{
private:
	double rayon;
	double centre_x;
	double centre_y;
	double centre_z;
public :
	Sphere();
	Sphere(double r,double x,double y,double z);
	~Sphere();
	double getRayon(){ return rayon;};
	double getX(){return centre_x;};
	double getY(){return centre_y;};
	double getZ(){return centre_z;};
	vector3 getCentre(){ return vector3(centre_x,centre_y,centre_z); }
	void setRayon(double r){rayon=r;};
	void setX(double x){centre_x=x;};
	void setY(double y){centre_y=y;};
	void setZ(double z){centre_z=z;};

	void initFromXML(TiXmlHandle hObj);
	void afficher();

	Intersection intersection(Rayon* r);
	vector3 normale(vector3 p);
};

#endif // SPHERE_H