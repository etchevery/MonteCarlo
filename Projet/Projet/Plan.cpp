#include "stdafx.h"
#include "Plan.h"


Plan::Plan(){	
	this->u=vector3();
	this->v=vector3();
	this->w=vector3();
	this->x=vector3();
}


Plan::Plan(vector3 a,vector3 b,vector3 c,vector3 d){
	
	this->u=a;
	this->v=b;
	this->w=c;
	this->x=d;
}

Plan::~Plan(){
}

bool Plan::operator ==( Plan& P){
return(this->u==P.u && this->v==P.v && this->w==P.w);
}

void Plan::initFromXML(TiXmlHandle hObj){
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double x4, y4, z4;

	Objet::initFromXML(hObj);
	TiXmlElement* pElem=hObj.Element();

	pElem->QueryDoubleAttribute("x1", &x1);
	pElem->QueryDoubleAttribute("y1", &y1);
	pElem->QueryDoubleAttribute("z1", &z1);
	pElem->QueryDoubleAttribute("x2", &x2);
	pElem->QueryDoubleAttribute("y2", &y2);
	pElem->QueryDoubleAttribute("z2", &z2);
	pElem->QueryDoubleAttribute("x3", &x3);
	pElem->QueryDoubleAttribute("y3", &y3);
	pElem->QueryDoubleAttribute("z3", &z3);
	pElem->QueryDoubleAttribute("x4", &x4);
	pElem->QueryDoubleAttribute("y4", &y4);
	pElem->QueryDoubleAttribute("z4", &z4);

	this->setU(vector3(x1,y1,z1));
	this->setV(vector3(x2,y2,z2));
	this->setW(vector3(x3,y3,z3));
	this->setX(vector3(x4,y4,z4));
}


void Plan::afficher(){

	cout<<"Plan :"<<endl;
	this->u.afficher();
	this->v.afficher();
	this->w.afficher();
	this->x.afficher();
	Objet::afficher();
}


Intersection Plan::intersect (Rayon* r)
  {

    Intersection inter, inter_tmp;

	clock_intersect_plan.begin(); // ---> start clock_intersect_plan 

	inter_tmp=Intersection(this,r, this->u,this->v,this->w);
		if(inter_tmp.getDistance()<DBL_MAX){
			inter=inter_tmp;
		}else{
			inter=Intersection(this,r, this->u,this->w,this->x);
		}

	inter.setObjet(this);
	
	clock_intersect_plan.end(); // ---> end clock_intersect_plan

	return(inter);
}


	vector3 Plan::normale()
	{
		return (this->w-this->u).vector_product(this->v-this->u);
	}
