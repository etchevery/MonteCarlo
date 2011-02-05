/*******************************************************************
 * \file   Intersection.cpp                                        *
 * \author  Arnaud ETCHEVERRY <arnaud.etcheverry@eisti.eu>         * 
 *          Abal-Kassim CHEIK AHAMED <abal.kassim.cheik@eisti.eu>  *
 * \version 1.0												       *
 * \date 11 janvier 2010										   *	
 * \brief														   *
 * \remarks														   *
 * \description													   *
 *******************************************************************/

#include "stdafx.h"
#include "Intersection.h"

	Intersection::Intersection()
	{
		this->point=vector3();	  
		this->normal=vector3();	    
		this->distance=DBL_MAX;  
		this->objet=NULL;
	}

	Intersection::Intersection(Objet* obj)
	{
		this->point=vector3();	  
		this->normal=vector3();	    
		this->distance=DBL_MAX;  
		this->objet=NULL;
	}

	 Intersection::Intersection(Objet* obj, Rayon* r, vector3 A,vector3 B,vector3 C){

	 this->objet=obj;

	double dx=r->getDirection().x; double px=r->getPosition().x;
	double dy=r->getDirection().y; double py=r->getPosition().y;
	double dz=r->getDirection().z; double pz=r->getPosition().z;

	vector3 AB=B-A; //vecteur AB
	vector3 AC=C-A; //vecteur AC
	vector3 AP=r->getPosition()-A; //vecteur AP (P:position rayon)

	cout << endl <<  "AB=";AB.afficher();
	cout << "AC=";AC.afficher(); cout << endl;
	cout << "Posi=";r->getPosition().afficher(); 
    cout << "Diri=";r->getDirection().afficher(); cout << endl;

	double D= AB.z * dy * AC.x - AB.y * dz * AC.x 
		     - dx * AB.x * AC.y - dz * AB.x * AC.z 
			 + dz * AB.x * AC.y + dx * AB.y * AC.z;

    double u =(-dy * AP.x * AC.z + dy * AC.x * AP.z - AC.x * dz * AP.y + AP.x * dz * AC.y -
     dx * AC.y * AP.z + dx * AP.y * AC.z) / D;
	double v =(dx * AB.y * AP.z + dz * AB.x * AP.y - AB.y * dz * AP.x - dx * AB.z * AP.y -
				dy * AB.x * AP.z + AB.z * dy * AP.x) / D;
    double t =(AP.x * AB.z * AC.y - AP.x * AB.y * AC.z - AB.x * AC.y * AP.z + AB.x * AP.y * AC.z +
			 AC.x * AB.y * AP.z - AC.x * AB.z * AP.y) / D;

	  if (fabs (D) > EPSILON && u > EPSILON && v > EPSILON && (u + v) <= 1 + EPSILON)
    {
		this->point=vector3(t * r->getDirection().x + r->getPosition().z,
							   t * r->getDirection().y + r->getPosition().y,
							   t * r->getDirection().z + r->getPosition().z);
		this->distance= (this->point-r->getPosition()).Length() ;

			vector3 N=AB.vector_product(AC);
			N.Normalize();
			this->normal=N;

    }else{
		 this->distance=DBL_MAX;
	}

}

	Intersection::~Intersection()
	{
	}


	vector3 Intersection::getPoint()
	{
		return this->point;
	}

	vector3 Intersection::getNormal()
	{
		return this->normal;
	}

	double Intersection::getDistance()
	{
		return this->distance;
	}

	Objet* Intersection::getObjet()
	{
		return this->objet;
	}


	void Intersection::setPoint(vector3 pt)
	{
		this->point=pt;
	}

	void Intersection::setNormal(vector3 norm)
	{
		this->normal=norm;
	}

	void Intersection::setDistance(double dist)
	{
		this->distance=dist;
	}

	void Intersection::setObjet(Objet* obj)
	{
		this->objet=obj;
	}


  

	void Intersection::afficher()
	{
		if(this->distance>=DBL_MAX){
			cout << "pas d'intersection" <<endl;
		}else{
			cout << endl << "********************************************" << endl << endl;
				cout<<"Intersection ---> ";
				this->objet->afficher();
				cout<<"Point d'intersection ---> ";
				this->point.afficher();
				cout<<"Distance: " << this->distance << endl; 
				cout<<"Normal ---> " ;
				this->normal.afficher();
		    cout << endl << "********************************************" << endl << endl;
		}
	}
