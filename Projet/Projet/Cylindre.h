/*******************************************************************
 * \file    Cylindre.h                                                *
 * \author  Arnaud ETCHEVERRY <arnaud.etcheverry@eisti.eu>         * 
 *          Abal-Kassim CHEIK AHAMED <abal.kassim.cheik@eisti.eu>  *
 * \version 1.0												       *
 * \date 11 janvier 2010										   *	
 * \brief														   *
 * \remarks														   *
 * \description													   *
 *******************************************************************/
#pragma once

#include "stdafx.h"

class Cylindre:public Objet
{
private:
	double r;  //rayon 
	double height; //longueur
public:
	Cylindre(void);
    Cylindre(double r, double height);
	virtual ~Cylindre(void);

	void setR(double r);
	void setheight(double height);

	double getR();
	double getheight();

	  /* calcule l'intersection entre le rayon r et l'objet*/
  Intersection intersect (Rayon* r);
  vector3 normale(vector3 p);
  void afficher();
 };
