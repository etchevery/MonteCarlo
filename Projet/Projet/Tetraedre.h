#ifndef TETRAEDRE_H
#define TETRAEDRE_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"
#include "timing.h"


using namespace std;
class Tetraedre : public Objet{
private:
			vector3 a;
			vector3 b;
			vector3 c;
			vector3 d;
public :
	Tetraedre();
    Tetraedre(vector3 a,vector3 b,vector3 c,vector3 d);
	~Tetraedre();
	
	vector3 getA(){return a;};
	vector3 getB(){return b;};
	vector3 getC(){return c;};
	vector3 getD(){return d;};

	void setA(vector3 a){this->a=a;};
	void setB(vector3 b){this->b=b;};
	void setC(vector3 c){this->c=c;};
	void setD(vector3 d){this->d=d;};

	void initFromXML(TiXmlHandle hObj);

	bool operator ==( Tetraedre& T);

	void afficher();
	Intersection intersect (Rayon* r);

};

#endif // Tetraedre_H