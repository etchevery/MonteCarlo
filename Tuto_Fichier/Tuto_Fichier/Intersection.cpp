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
			cout<<"Intersection ---> ";
			this->objet->afficher();
			cout<<"Point d'intersection ---> ";
			this->point.afficher();
			cout<<"Distance: " << this->distance << endl; 
			cout<<"Normal ---> " ;
			this->normal.afficher();
		}
	}
