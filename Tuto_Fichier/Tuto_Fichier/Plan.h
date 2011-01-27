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
public :
	Plan();
	Plan(double param_x,double param_y,double param_z);
	~Plan();
	
	double getX(){return x;};
	double getY(){return y;};
	double getZ(){return z;};
	
	void setX(double param_x){x=param_x;};
	void setY(double param_y){y=param_y;};
	void setZ(double param_z){z=param_z;};

	void initFromXML(TiXmlHandle hObj);
	void afficher();
	Intersection intersection (Rayon* r);
};

#endif // PLAN_H