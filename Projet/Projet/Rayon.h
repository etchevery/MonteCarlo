#pragma once
#include "stdafx.h"



class Rayon
{
private :
	double x_position;
	double y_position;
	double z_position;
	double x_direction;
	double y_direction;
	double z_direction;
public:
	Rayon(void);
	~Rayon(void);
	Rayon(double px, double py, double pz, double dx, double dy, double dz);
	Rayon(vector3 point,vector3 dir);
	void setPosition(double x,double y,double z);
	void setDirection(double x,double y, double z);
	vector3 getPosition();
	vector3 getDirection();

	void initFromXML(TiXmlHandle hObj);
	void afficher();
};

typedef Rayon Camera;