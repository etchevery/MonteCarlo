#pragma once

#include "stdafx.h"
#include "timing.h"

class Objet;

class Intersection{
private:
	vector3 point;	    // Point d'intersection
	vector3 normal;	    // Vecteur normal
	double distance;    //distance entre le rayon et l'objet
	Objet* objet;		// objet intersecté
public:
	Intersection();
	~Intersection();
	Intersection(Objet* obj);
	Intersection(Objet* obj, Rayon* r, vector3 A,vector3 B,vector3 C);
	vector3 getPoint();
	vector3 getNormal();
	double getDistance();
	Objet* getObjet();

	void setPoint(vector3 pt);
	void setNormal(vector3 norm);
	void setDistance(double dist);
	void setObjet(Objet* obj);

	void afficher();

};
