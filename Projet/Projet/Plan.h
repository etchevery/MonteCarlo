#pragma once

#include "stdafx.h"
#include "Objet.h"
#include "tinyxml.h"
#include <tinyxml.h>
class Plan : public Objet{
private:
			vector3 u;
			vector3 v;
			vector3 w;
public :
	Plan();
    Plan(vector3 a,vector3 b,vector3 c);
	~Plan();
	
	vector3 getU(){return u;};
	vector3 getV(){return v;};
	vector3 getW(){return w;};

	void setU(vector3 u){this->u=u;};
	void setV(vector3 v){this->v=v;};
	void setW(vector3 w){this->w=w;};

	void initFromXML(TiXmlHandle hObj);
	void afficher();
	Intersection intersect (Rayon* r);
    vector3 normale();
};
