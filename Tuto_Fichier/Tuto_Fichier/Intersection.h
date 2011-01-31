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

static void afficher(Intersection inter)
{
	if(inter.distance>=DBL_MAX){
		cout << "pas d'intersection" <<endl;
	}else{
		cout<<"Intersection ---> ";
		inter.objet->afficher();
		cout<<"Point d'intersection ---> ";
		inter.point.afficher();
		cout<<"Distance: " << inter.distance << endl; 
		cout<<"Surface Normal ---> " ;
		inter.surfaceNormal.afficher();
	}
}
#endif