#ifndef PLAN_H
#define PLAN_H
#include "Objet.h"
#include "Intersection.h"
#include "Rayon.h"
#include "common.h"



using namespace std;
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

	bool operator ==( Plan& P);

	void afficher();
	Intersection intersect (Rayon* r);
    vector3 normale();
};

#endif // PLAN_H