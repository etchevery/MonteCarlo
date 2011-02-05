#pragma once

#include "stdafx.h"


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
