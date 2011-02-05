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

bool Sphere::operator ==( Sphere& S){
 bool test=false;
 if(this->rayon==S.rayon && this->getCentre()==S.getCentre())
	test=true;

 return test;
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

	/* repère globale
	 * equation du sphere (x-c_x)²+(y-c_y)²+(z-z_y)²=rayon²   {centre (c_x,c_y,c_z)}
	 * (dx.t+px-c_x)²+(dy.t+py-c_y)²+(dz.t+pz-c_z)²=rayon²
	 * (dx²+dy²+dz²)t²+2*(dx.px+dy.py+dz.pz)t+((px-c_x)²+(py-c_y)²+(pz-c_z)²-rayon²)=0
	 * at²+bt+c=0
	 */
Intersection Sphere::intersect2(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;

	//paramètre du rayon lancé
    double dx=r->getDirection().x; double px=r->getPosition().x;
	double dy=r->getDirection().y; double py=r->getPosition().y;
	double dz=r->getDirection().z; double pz=r->getPosition().z;
 
	 inter.setObjet(this);
  
	 a=pow(dx,2)+pow(dy,2)+pow(dz,2);
	 b=((dx*(px-this->centre_x))+(dy*(py-this->centre_y))+(dz*(pz-this->centre_z)));
	 c=pow(px-this->centre_x,2)+pow(py-this->centre_y,2)+pow(pz-this->centre_z,2)-pow(this->rayon,2);

	 //calcul du discriminant
	 delta = pow(b,2)-a*c;

	  if (delta <= 0) {
		  inter.setDistance(DBL_MAX); //distance infinie (pas d'intersection)
	  } else { //deux solutions

		  t1 = (-b + sqrt (delta)) / a;
		  t2 = (-b - sqrt (delta)) / a;
		  //les deux solutions sont très très proche de 0 (assimilable à deux nombres négatifs en info)
		  if (t1 <= EPSILON && t2 <= EPSILON){
			inter.setDistance(DBL_MAX);
		  }else{

			  //choix entre les deux solutions
			  if ((t1 <= t2 && t1 > EPSILON) || (t2 < t1 && t2 < EPSILON)){
				t = t1;  //la solution sera t1
			  }else if ((t2 <= t1 && t2 > EPSILON) || (t1 < t2 && t1 < EPSILON)){
				t = t2;  //la solution sera t2
			  }
			  //on recupère la solution avec le t calculée
			  inter.setPoint(vector3(t * dx + px,t * dy + py,t * dz + pz));
			  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
			  //il y a intersection on calcul la normal à ce point d'intersection
			  vector3 normal=this->normale(inter.getPoint());
			  //on normalise la normale
			  normal.Normalize();
			  inter.setNormal(normal);
			}
	  }
	  	cout << "rayon=" << rayon << " , size=" << (this->getCentre()-inter.getPoint()).Length() << endl;
	cout << endl << "vérification: " << "eq " 
		<< pow(inter.getPoint().x-this->centre_x,2)
		+pow(inter.getPoint().y-this->centre_y,2)
		+pow(inter.getPoint().z-this->centre_z,2)-pow((this->getCentre()-inter.getPoint()).Length(),2) << endl;
	  return (inter);
    }

/*Soient D une droite de l'espace et S une sphère de centre C et de rayon r , 
 *  H le projeté orthogonal du point C sur la droite D. 
 *  Notons d = CH (x_h-x_c,y_h-y_x,z_h-z_c) :
 * OH.(dx,dy,dz)=0 => (x_h-x_c)dx+(y_h-y_c)dy+(z_h-z_c)dz=0
 * Il existe unique t tel que x_h=t.dx+px, y_h=t.dy+py, z_h=t.dz+pz
 * t vérifie : t(dx²+dy²+dz²)+dx(px-x_c)+dy(py-y_c)+dz(pz-z_c)=0
 *** Si d > r alors la droite D et la sphère S n'ont pas de points en commun, 
 *  l'intersection est vide.
 *** Si d = r alors la droite D et la sphère S ont un unique point en commun et
 * dans ce cas on dit que la droite D est tangente en H à S
 *** Si d < r alors la droite D et la sphère S ont deux points en commun A et B 
 * symétriques par rapport au point H, 
 * dans ce cas on dit que la droite D est sécante à S. ( OA = OB = r )
 *
 *
 */

//Lorsque (d) ne passe pas par le centre de la sphère, 
//il n'existe qu'un seul grand cercle passant par A et B (représenté en bleu sur la figure). 
//Comme (OH) est un diamètre perpendiculaire à la corde [AB] alors (OH) est médiatrice de [AB].

 Intersection Sphere::intersect(Rayon* r)
{
	Intersection inter;
	double ah,bh;
	double d;
	double t,t1,t2;

	//paramètre du rayon lancé
    double dx=r->getDirection().x; double px=r->getPosition().x;
	double dy=r->getDirection().y; double py=r->getPosition().y;
	double dz=r->getDirection().z; double pz=r->getPosition().z;
	double c_x=this->centre_x;
	double c_y=this->centre_y;
	double c_z=this->centre_z;
	/* calcul des coordonnées du point H:  CH.D=0
	 * t(dx²+dy²+dz²)+dx(px-x_c)+dy(py-y_c)+dz(pz-z_c)=0
	 * a*t+b=0 t=-b/a  si a/=0  (a toujours non null)
	 */

	ah=pow(dx,2)+pow(dy,2)+pow(dz,2);
	bh=dx*(px-c_x)+dy*(py-c_y)+dz*(pz-c_z);
	t=-bh/ah;
	vector3 H(t * dx + px,t * dy + py,t * dz + pz);

		 inter.setObjet(this);

	d=(H-this->getCentre()).Length();

	//pas d'intersection
	if(d>this->rayon){
		inter.setDistance(DBL_MAX);
	}else if(d==this->rayon){ //unique intersection
		inter.setPoint(H);
		inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
	    vector3 normal=this->normale(inter.getPoint());
		normal.Normalize();
	    inter.setNormal(normal);
	}else{ //deux intersections possibles: A, B  (CA=CB et AH=HB) 
		double a=ah;
		double b=((dx*(px-this->centre_x))+(dy*(py-this->centre_y))+(dz*(pz-this->centre_z)));
		double c=pow(px-this->centre_x,2)+pow(py-this->centre_y,2)+pow(pz-this->centre_z,2)-pow(this->rayon,2);
		double delta = pow(b,2)-a*c;
		 t1 = (-b + sqrt (delta)) / a;
		 t2 = (-b - sqrt (delta)) / a;
		  //les deux solutions sont très très proche de 0 (assimilable à deux nombres négatifs en info)
		 if (t1 <= EPSILON && t2 <= EPSILON){
			inter.setDistance(DBL_MAX);
		 }else{
			  //choix entre les deux solutions
			  if ((t1 <= t2 && t1 > EPSILON) || (t2 < t1 && t2 < EPSILON)){
				t = t1;  //la solution sera t1
			  }else if ((t2 <= t1 && t2 > EPSILON) || (t1 < t2 && t1 < EPSILON)){
				t = t2;  //la solution sera t2
			  }

			  //on recupère la solution avec le t calculée
			  inter.setPoint(vector3(t * dx + px,t * dy + py,t * dz + pz));
			  inter.setDistance ( (inter.getPoint()-(*r).getPosition()).Length() );
			  //il y a intersection on calcul la normal à ce point d'intersection
			  vector3 normal=this->normale(inter.getPoint());
			  //on normalise la normale
			  normal.Normalize();
			  inter.setNormal(normal);
			}

	}

	cout<< endl << "sphere::debut verification : ************************************************" << endl;
	cout << "rayon=" << rayon << " , =  size=" << (this->getCentre()-inter.getPoint()).Length() << endl;
	cout << endl << "remplace eq: : " << " = " 
		<< pow(inter.getPoint().x-this->centre_x,2)
		+pow(inter.getPoint().y-this->centre_y,2)
		+pow(inter.getPoint().z-this->centre_z,2)-pow((this->getCentre()-inter.getPoint()).Length(),2) << endl;
	cout<< endl << "sphere:: fin verification : **************************************************" << endl;
	return(inter);
 }



	vector3 Sphere::normale(vector3 p)
	{
		return p-(*this).getCentre();
	}

