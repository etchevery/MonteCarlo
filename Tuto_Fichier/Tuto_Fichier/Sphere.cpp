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

Intersection Sphere::intersect(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;

	/* rep�re globale
	 * equation du sphere (x-c_x)�+(y-c_y)�+(z-z_y)�=rayon�   {centre (c_x,c_y,c_z)}
	 * (dx.t+px-c_x)�+(dy.t+py-c_y)�+(dz.t+pz-c_z)�=rayon�
	 * (dx�+dy�+dz�)t�+2*(dx.px+dy.py+dz.pz)t+((px-c_x)�+(py-c_y)�+(pz-c_z)�-rayon�)=0
	 * at�+bt+c=0
	 */
	 
	 inter.setObjet(this);
  
	 a=pow(r->getDirection().x,2)+pow(r->getDirection().y,2)+pow(r->getDirection().z,2);
	 b=2*((r->getDirection().x*(r->getPosition().x-this->centre_x))
		 +(r->getDirection().y*(r->getPosition().y-this->centre_y))
		 +(r->getDirection().z*(r->getPosition().z-this->centre_z)));
	 
	 c=pow(r->getPosition().x-this->centre_x,2)
		 +pow(r->getPosition().y-this->centre_y,2)
		 +pow(r->getPosition().z-this->centre_z,2)
		 -pow(this->rayon,2);


	 //calcul du discriminant
	 delta = pow(b,2)-4*a*c;

	  if (delta <= 0) {
		  inter.setDistance(DBL_MAX); //distance infinie (pas d'intersection)
	  } else { //deux solutions

		  t1 = (-b + sqrt (delta)) / 2*a;
		  t2 = (-b - sqrt (delta)) / 2*a;
		  //les deux solutions sont tr�s tr�s proche de 0 (assimilable � deux nombres n�gatifs en info)
		  if (t1 <= EPSILON && t2 <= EPSILON){
			inter.setDistance(DBL_MAX);
		  }else{

			  //choix entre les deux solutions
			  if ((t1 <= t2 && t1 > EPSILON) || (t2 < t1 && t2 < EPSILON)){
				t = t1;  //la solution sera t1
			  }else if ((t2 <= t1 && t2 > EPSILON) || (t1 < t2 && t1 < EPSILON)){
				t = t2;  //la solution sera t2
			  }


			  //on recup�re la solution avec le t calcul�e
			  inter.setPoint(
				  vector3(t * r->getDirection().x + r->getPosition().x - this->centre_x,
				  t * r->getDirection().y + r->getPosition().y - this->centre_y,
				  t * r->getDirection().z + r->getPosition().z - this->centre_z)
							);
			  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
			  //il y a intersection on calcul la normal � ce point d'intersection
			  vector3 normal=this->normale(inter.getPoint());
			  //on normalise la normale
			  normal.Normalize();
			  inter.setNormal(normal);
			}
	  }

	  	 //TODO: la normale 
	  return (inter);
    }

	vector3 Sphere::normale(vector3 p)
	{
		return p-(*this).getCentre();
	}