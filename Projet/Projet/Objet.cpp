#include "stdafx.h"
#include "Objet.h"


void Objet::initFromXML(TiXmlHandle hObj){
	m_mat.initFromXML(hObj);
}


Intersection Objet::intersect(Rayon* r){
	Intersection I=Intersection();
	return I;
}