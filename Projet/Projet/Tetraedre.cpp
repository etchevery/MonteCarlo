#include "stdafx.h"
#include "Tetraedre.h"

Tetraedre::Tetraedre(){	
	this->a=vector3();
	this->b=vector3();
	this->c=vector3();
	this->d=vector3();
}


Tetraedre::Tetraedre(vector3 a,vector3 b,vector3 c,vector3 d){
	this->a=a;
	this->b=b;
	this->c=c;
	this->d=d;
}

Tetraedre::~Tetraedre(){
}

bool Tetraedre::operator ==( Tetraedre& T){
 bool test=false;
 if(this->a==T.a && this->b==T.b && this->c==T.c && this->d==T.d)
	test=true;

 return test;
}

void Tetraedre::initFromXML(TiXmlHandle hObj){
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

	this->setA(vector3(x1,y1,z1));
	this->setB(vector3(x2,y2,z2));
	this->setC(vector3(x3,y3,z3));
	this->setD(vector3(x4,y4,z4));
}


void Tetraedre::afficher(){

	cout<<"Tetraedre :"<<endl;
	this->a.afficher();
	this->b.afficher();
	this->c.afficher();
	this->d.afficher();
	Objet::afficher();
}


Intersection Tetraedre::intersect (Rayon* r)
  {

    Intersection inter, inter_tmp, I[4];

		clock_intersect_tetraedre.begin(); // ---> start clock_intersect_triangle

	I[0]=Intersection(this,r, this->a,this->b,this->c);
	I[1]=Intersection(this,r, this->c,this->d,this->a);
	I[2]=Intersection(this,r, this->a,this->b,this->d);
	I[3]=Intersection(this,r, this->b,this->c,this->d);

	double d_min=I[0].getDistance(); 
	inter=I[0];

	for(int i=1;i<3;i++){
		if(I[i].getDistance()<DBL_MAX 
			&& I[i].getDistance()<d_min)
				{
					inter=I[i];
					d_min=I[i].getDistance();
				}
	}

	inter.setObjet(this);

	clock_intersect_tetraedre.end(); // ---> end clock_intersect_tetraedre

	return(inter);

}	
