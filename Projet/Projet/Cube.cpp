/*******************************************************************
 * \file    Cube.cpp                                                *
 * \author  Arnaud ETCHEVERRY <arnaud.etcheverry@eisti.eu>         * 
 *          Abal-Kassim CHEIK AHAMED <abal.kassim.cheik@eisti.eu>  *
 * \version 1.0												       *
 * \date 11 janvier 2010										   *	
 * \brief														   *
 * \remarks														   *
 * \description													   *
 *******************************************************************/

#include "stdafx.h"
#include "Cube.h"



Cube::Cube(void)
{
	this->centre = vector3();
	this->longueur = 1.0;
}


    Cube::Cube(vector3 c,double l)
	{
		this->centre = c;
		this->longueur = l;
	}

	Cube::~Cube()
	{
	}
	
	void Cube::afficher(){
		cout<<"Cube :"<<endl;
		cout <<"Centre ---> ";
		centre.afficher();
		cout <<"Longueur : "<< longueur <<endl;
		Objet::afficher();
	}




	Intersection Cube::intersect(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;


	 //inter.setObjet(this);

	 
	  	 //TODO: la normale 
	    return (inter);
  }

		vector3 Cube::normale(vector3 p)
	{
		return vector3();
	}