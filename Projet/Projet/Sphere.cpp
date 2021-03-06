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
 return(this->rayon==S.rayon && this->getCentre()==S.getCentre());
}


Sphere::~Sphere(){
}

void Sphere::initFromXML(TiXmlHandle hObj){
	double rayon,x,y,z;
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

	/* rep�re globale
	 * equation du sphere (x-c_x)�+(y-c_y)�+(z-z_y)�=rayon�   {centre (c_x,c_y,c_z)}
	 * (dx.t+px-c_x)�+(dy.t+py-c_y)�+(dz.t+pz-c_z)�=rayon�
	 * (dx�+dy�+dz�)t�+2*(dx.px+dy.py+dz.pz)t+((px-c_x)�+(py-c_y)�+(pz-c_z)�-rayon�)=0
	 * at�+2*bt+c=0
	 * delta=4b�-4*a*c=4(b�-ac); on pose delta=1/4(delta) car est de signe de b�-ac
	 * x1=(-2b-sqrt(4(b�-ac)))/2a=(-b-sqrt(b�-ac))/a
	 * x2=(-b+sqrt(b�-ac))/a
	 */
Intersection Sphere::intersect(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;

	clock_intersect_sphere.begin(); // ---> start clock_intersect_sphere

	//param�tre du rayon lanc�
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
		  double inv_a=1/a;
		  t1 = inv_a*(-b + sqrt (delta));
		  //optimisation du calcul de t2: t2 = inv_a*(-b - sqrt (delta));
		  t2=inv_a*(-2*b-a*t1);
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
			  inter.setPoint(vector3(t * dx + px,t * dy + py,t * dz + pz));
			  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
			  //il y a intersection on calcul la normal � ce point d'intersection
			  vector3 normal=this->normale(inter.getPoint());
			  //on normalise la normale
			  normal.Normalize();
			  inter.setNormal(normal);
			}
	  }

	  	clock_intersect_sphere.end(); // ---> end clock_intersect_sphere

	  return (inter);
    }



	vector3 Sphere::normale(vector3 p)
	{
		return p-(*this).getCentre();
	}

	
Sphere::Sphere(vector3 Sommets[], int n)
{
	Sphere S;
	int i;
	double xMin, xMax;
	double yMin, yMax;
	double zMin, zMax;
	int ixMin, jyMin, kzMin;
	int ixMax, jyMax, kzMax;
	vector3 dSx, dSy, dSz, dS; //vecteur diam�tre
	double dx, dy, dz; //la norme au carr�e des vecteurs diam�tres
	double rayon, rayonCarre; //rayon
	vector3 C; //centre du cercle
	vector3 vtmp; //vecteur tmp
	double d;

	//initialisation des valeurs et indices minimales et maximales.
	ixMin=jyMin=kzMin=0;
	ixMax=jyMax=kzMax=0;
	xMin=xMax=Sommets[0].x;
	yMin=yMax=Sommets[0].y;
	zMin=zMax=Sommets[0].z;

	//recherche  des valeurs et indices minimales et maximales.
	for(i=1;i<n;i++){
		//selon x
		if(Sommets[i].x<xMin){
			xMin=Sommets[i].x;
			ixMin=i;
		}else if(Sommets[i].x>xMax){
			xMax=Sommets[i].x;
			ixMax=i;
		}
		//selon y
		if(Sommets[i].y<yMin){
			yMin=Sommets[i].y;
			jyMin=i;
		}else if(Sommets[i].y>yMax){
			yMax=Sommets[i].y;
			jyMax=i;
		}
		//selon z
		if(Sommets[i].z<zMin){
			zMin=Sommets[i].z;
			kzMin=i;
		}else if(Sommets[i].z>zMax){
			zMax=Sommets[i].z;
			kzMax=i;
		}
	}
	//initialisation diam�tre du sph�re selon les 3 directions
	dSx=Sommets[ixMax]-Sommets[ixMin];
	dSy=Sommets[jyMax]-Sommets[jyMin];
	dSz=Sommets[kzMax]-Sommets[kzMin];
	//calcul
	dx=dSx.Dot(dSx); 
	dy=dSy.Dot(dSy);
	dz=dSy.Dot(dSz);

	if(dx>=dy && dx>=dz){  //le plus grand rayon est selon x
		C=Sommets[ixMin]+(0.5*dSx); //le centre le point milieu des deux extr�mes 
		vtmp=Sommets[ixMax]-C;
		rayonCarre=vtmp.Dot(vtmp);
	}else if(dy>=dx && dy>=dz){  //le plus grand rayon est selon y
		C=Sommets[jyMin]+(0.5*dSy); //le centre le point milieu des deux extr�mes 
		vtmp=Sommets[jyMax]-C;
		rayonCarre=vtmp.Dot(vtmp);
	}else{ //sinon c'est selon z
		C=Sommets[kzMin]+(0.5*dSz); //le centre le point milieu des deux extr�mes 
		vtmp=Sommets[kzMax]-C;
		rayonCarre=vtmp.Dot(vtmp);
	}

	rayon=sqrt(rayonCarre);

	//On v�rifie si tous les points appartiennent au cercle
	//si non on prend un sphere un peu plus grand qui les contiennent.
	for(i=0;i<n;i++){
		dS=Sommets[i]-C;
		d=dS.Dot(dS);
		//si le point n'est pas dans le sphere
		if(d>rayonCarre){
			//on prend un sph�re plus grand qui l'inclu
			d=sqrt(d);
			rayon=0.5*(rayon+d); 
			rayonCarre=pow(rayon,2);
			C=((d-rayon)/d)*dS; //on d�place le centre
		}
	}

	this->setCenter(C);
	this->setRayon(rayon);

}