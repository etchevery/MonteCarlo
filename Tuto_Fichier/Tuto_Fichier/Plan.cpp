#include "stdafx.h"
#include "Plan.h"

Plan::Plan(){
	
	this->x=0;
	this->y=0;
	this->z=0;
}


Plan::Plan(double param_x,double param_y,double param_z,double param_norm){
	
	this->x=param_x;
	this->y=param_y;
	this->z=param_z;
	this->norm=param_norm;
}

Plan::~Plan(){
}

void Plan::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	Objet::initFromXML(hObj);
	TiXmlElement* pElem=hObj.Element();

	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	this->setX(x);
	this->setY(y);
	this->setZ(z);

	//this->setNorm(norm);
}
void Plan::afficher(){

	cout<<"Plan :"<<endl;
	cout<<"Coord : "<<x<<", "<<y<<", "<<z<<endl;
	Objet::afficher();
}


Intersection Plan::intersection (Rayon* r)
  {



	Intersection inter;
	double t;

	 inter.objet=this;
  
	/* Pour un plan, x=0
	 * Donc  dx.t+px=0 => t= -px/dx si dx!=0 
	 */
	 if(r->getDirection().x!=0){
		 t=-r->getPosition().x/r->getDirection().x; 
		if(t>EPSILON){
		    inter.point=vector3(0.0,
								t * r->getDirection().y + r->getPosition().y,
								t * r->getDirection().z + r->getPosition().z);

		    inter.distance = (inter.point-r->getPosition()).Length();
			  //il y a intersection on calcul la normal à ce point d'intersection
			vector3 normal=this->normale();
			  //on normalise la normale
			  normal.Normalize();
			  inter.surfaceNormal=normal;

		}else{
			inter.distance = DBL_MAX;
		}
	 }else{
		 inter.distance = DBL_MAX;
	 }

	 return(inter);
  }

	vector3 Plan::normale()
	{
		return vector3(this->x,this->y,this->z);
	}
