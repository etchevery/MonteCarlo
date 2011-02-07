#ifndef CUBE_H
#define CUBE_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"



using namespace std;
class Cube : public Objet{
private:
	/*   1 ************ 2
	 *     *          *
	 *     *          *
	 *     *          *
	 *   4 ************ 3
	 */
			vector3 Sommets[8]; //sommet du cube
public :
	Cube();
    Cube(vector3 S[8]);
	~Cube();
	
	vector3* getSommets();
	int getNbSommets(){return 8;};
	int getNBFacesTriangles(){return 6*2;};
	void setSommets(vector3 c[8]);
	void afficher();
	Intersection intersect (Rayon* r);

};

#endif // CUBE_H