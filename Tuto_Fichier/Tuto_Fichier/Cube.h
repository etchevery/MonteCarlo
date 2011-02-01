#ifndef CUBE_H
#define CUBE_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"



using namespace std;
class Cube : public Objet{
private:
			vector3 centre; //centre
			double longueur; //hauteur 
public :
	Cube();
    Cube(vector3 c,double l);
	~Cube();
	
	vector3 getCentre(){return centre;};
	double getLongueur(){return longueur;};

	void setCentre(vector3 c){this->centre=c;};
	void setLongueur(double l){this->longueur=l;};
	void afficher();
	Intersection intersect (Rayon* r);
    vector3 normale(vector3 p);
};

#endif // CUBE_H