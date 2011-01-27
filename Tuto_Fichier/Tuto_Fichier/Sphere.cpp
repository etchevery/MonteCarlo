#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(){
	this->rayon=0;
	this->centre_x=0;
	this->centre_y=0;
	this->centre_z=0;
}

Sphere::Sphere(double r,double x,double y,double z){
	this->rayon=r;
	this->centre_x=x;
	this->centre_y=y;
	this->centre_z=z;
}

Sphere::~Sphere(){
}

void Sphere::initFromXML(TiXmlHandle hObj){
	double rayon,x,y,z;
	cout<<"Init sphere from xml"<<endl;
	Objet::initFromXML(hObj);
	TiXmlElement* pElem=hObj.Element();

	pElem->QueryDoubleAttribute("rayon", &rayon);
	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	cout<<"Rayon = "<<rayon<<", x = "<<x<<" , y = "<<y<<" , z ="<<z<<endl;
	this->setRayon(rayon);
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}

void Sphere::afficher(){
	cout<<"Sphere :"<<endl;
	cout <<"Rayon : "<<rayon<<endl;
	cout<<"Centre : "<<centre_x<<", "<<centre_y<<", "<<centre_z<<endl;
	Objet::afficher();
}

Intersection Sphere::intersection(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;

	/* equation du sphere x²+y²+z²=rayon²
	 * (dx.t+px)²+(dy.t+py)²+(dz.t+pz)²=rayon²
	 * (dx²+dy²+dz²)t²+(dx.px+dy.py+dz.pz)t+(px²+py²+pz²-rayon²)=0
	 * at²+bt+c=0
	 */
	 
	 inter.objet=this;
  
	 a=pow(r->getDirection().x,2)+pow(r->getDirection().y,2)+pow(r->getDirection().z,2);
	 b=(r->getDirection().x*r->getPosition().x)+(r->getDirection().y*r->getPosition().y)+(r->getDirection().z*r->getPosition().z);
	 c=pow(r->getPosition().x,2)+pow(r->getPosition().y,2)+pow(r->getPosition().z,2)-pow(this->rayon,2);
	 //calcul du discriminant
	 delta = pow(b,2)-4*a*c;
	 
	  if (delta <= 0) {
		  inter.distance = DBL_MAX; //distance infinie (pas d'intersection)
	  } else { //deux solutions

		  t1 = (-b + sqrt (delta)) / 2*a;
		  t2 = (-b - sqrt (delta)) / 2*a;

		  if (t1 <= EPSILON && t2 <= EPSILON){
			inter.distance = DBL_MAX;
		  }else{

			  if ((t1 <= t2 && t1 > EPSILON) || (t2 < t1 && t2 < EPSILON)){
				t = t1;
			  }else if ((t2 < t1 && t2 > EPSILON) || (t1 < t2 && t1 < EPSILON)){
				t = t2;
			  }

			  inter.point=vector3(t * r->getDirection().x + r->getPosition().x,
								  t * r->getDirection().y + r->getPosition().y,
								  t * r->getDirection().z + r->getPosition().z);

			  inter.distance = (inter.point-r->getPosition()).Length();

			}
	  }
	  	 //TODO: la normale 
	  return (inter);
    }