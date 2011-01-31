#ifndef PLAN_H
#define PLAN_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"



using namespace std;
class Plan : public Objet{
private:
			double x;
			double y;
			double z;

			double norm;
public :
	Plan();
    Plan(double param_x,double param_y,double param_z,double param_norm);
	~Plan();
	
	double getX(){return x;};
	double getY(){return y;};
	double getZ(){return z;};
	double getNorm(){return norm;};

	void setX(double param_x){this->x=param_x;};
	void setY(double param_y){this->y=param_y;};
	void setZ(double param_z){this->z=param_z;};
	void setNorm(double param_norm){this->norm=param_norm;};

	void initFromXML(TiXmlHandle hObj);
	void afficher();
	Intersection intersection (Rayon* r);
    vector3 normale();
};

#endif // PLAN_H