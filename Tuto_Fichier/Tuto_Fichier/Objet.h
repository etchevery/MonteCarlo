#ifndef OBJET_H
#define OBJET_H

#include "stdafx.h"
#include "common.h"
#include "Materiau.h"
#include "Rayon.h"


class Intersection;

class Objet{
public :
	virtual ~Objet(){};
	
	virtual void initFromXML(TiXmlHandle hObj);
	virtual void afficher(){m_mat.afficher();};
	virtual Intersection intersect(Rayon* r);
	Reflectance GetReflectance(){return m_mat.getReflectance();};

private :
	Materiau m_mat;
};

#endif //OBJET_H