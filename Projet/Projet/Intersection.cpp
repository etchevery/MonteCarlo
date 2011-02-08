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
    double t,a,f,u,v;
	double dx=r->getDirection().x; double px=r->getPosition().x;
	double dy=r->getDirection().y; double py=r->getPosition().y;
	double dz=r->getDirection().z; double pz=r->getPosition().z;

		clock_intersect_triangle.begin(); // ---> start clock_intersect_triangle

	vector3 E;
	vector3 AB=B-A; //vecteur AB
	vector3 AC=C-A; //vecteur AC
	vector3 AP=r->getPosition()-A; //vecteur AP (P:position rayon)
	vector3 APD=r->getDirection().vector_product(AC);
	a = AB.Dot(APD);

	if (a > -EPSILON && a < EPSILON){
		this->distance=DBL_MAX; //pas d'intersection
	}else{
		f=1/a;
		u=f*(AP.Dot(APD));
		if(u<0.0 || u>1.0){
			this->distance=DBL_MAX; //pas d'intersection
		}else{
			E=AP.vector_product(AB);
			v=f*(r->getDirection().Dot(E));

		 	if (v < 0.0 || u + v > 1.0){
				this->distance=DBL_MAX; //pas d'intersection
			}else{
				t=f*AC.Dot(E);
				if(t>EPSILON){  //intersection 
					this->point= vector3(t * dx + px,t * dy + py,t * dz + pz);
					this->distance=(this->point-r->getPosition()).Length() ;
					vector3 N=AB.vector_product(AC);
					N.Normalize();
					this->normal=N;
				}else{ //intersection = segment (rayon dans le triangle)
					this->distance=DBL_MAX; //pas d'intersection
				}
			}
		}
	}
	clock_intersect_triangle.end(); // ---> end clock_intersect_triangle

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
