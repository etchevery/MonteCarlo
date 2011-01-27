#ifndef INTERSECTIONH
#define INTERSECTIONH

// Forward declarations
class Primitive;

#include "common.h"
#include "Objet.h"


typedef struct intersect_record_struct {

	vector3 point;			// Point d'intersection
	vector3 surfaceNormal;	// Vecteur normal
	Objet* objet;		// objet intersecté
	double distance;    //distance entre le rayon et l'objet

} Intersection;

#endif