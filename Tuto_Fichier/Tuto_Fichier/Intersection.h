#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "common.h"

class Intersection{
private:
	vector3 point;	    // Point d'intersection
	vector3 normal;	    // Vecteur normal
	double distance;    //distance entre le rayon et l'objet
	//Objet* objet;		// objet intersecté

public:
	Intersection();
	~Intersection();
	//Intersection(Objet* obj);
	vector3 getPoint();
	vector3 getNormal();
	double getDistance();
	//Objet* getObjet();

	void setPoint(vector3 pt);
	void setNormal(vector3 norm);
	void setDistance(double dist);
	//void setObjet(Objet* obj);

	void afficher();

};


#endif