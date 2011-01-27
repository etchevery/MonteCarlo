#include "stdafx.h"
#include "Plan.h"


Plan::Plan(){
	
	this->x=0;
	this->y=0;
	this->z=0;
}

Plan::Plan(double param_x,double param_y,double param_z){
	
	this->x=param_x;
	this->y=param_y;
	this->z=param_z;
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
}
void Plan::afficher(){

	cout<<"Plan :"<<endl;
	cout<<"Coord : "<<x<<", "<<y<<", "<<z<<endl;
	Objet::afficher();
}

Intersection Plan::intersection (Rayon* r)
  {

	/* Pour un plan, x=0
	 * Donc  dx.t+px=0 => t= px/dx si dx!=0 
	 */

	Intersection inter;
	double t;

	 inter.objet=this;
  

	 if(r->getDirection().x!=0){
		 t=r->getPosition().x/r->getDirection().x; 
		if(t>EPSILON){
		    inter.point=vector3(0.0,
						  t * r->getDirection().y + r->getPosition().y,
						  t * r->getDirection().z + r->getPosition().z);

		    inter.distance = (inter.point-r->getPosition()).Length();
		}else{
			inter.distance = DBL_MAX;
		}
	 }else{
		 inter.distance = DBL_MAX;
	 }

	 //TODO: la normale 

	 return(inter);
  }