#ifndef POLYGONE_H
#define POLYGONE_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"



using namespace std;
class Polygone : public Objet{
private:
		vector3* Faces[3]; //faces (Triangle) du Polygone
		int nbFaces; //nombre de Polygone
public :
	Polygone();
	Polygone(int n);
    Polygone(vector3* S[3], int n);
	~Polygone();
	
	vector3** getFaces();
	vector3* getFaces(int i);
	int getNbFaces();
	void setFaces(int i, vector3 c[3]);
	void afficher();
	Intersection intersect (Rayon* r);

};

#endif // Polygone_H