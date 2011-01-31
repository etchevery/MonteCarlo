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

#if !defined(__CYLINDRE__)
#define      __CYLINDRE__

#include "Objet.h"
#include <tinyxml.h>
#include <iostream>
#include "Intersection.h"
#include "Rayon.h"

using namespace std;

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
  Intersection intersection (Rayon* r);
  vector3 normale(vector3 p);
  void afficher();
 };

#endif // __Cylindre__