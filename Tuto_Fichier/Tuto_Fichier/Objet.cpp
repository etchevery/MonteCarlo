#include "stdafx.h"
#include "Objet.h"
#include "Intersection.h"
using namespace std;

void Objet::initFromXML(TiXmlHandle hObj){
	m_mat.initFromXML(hObj);
}


Intersection Objet::intersect(Rayon* r){
	Intersection I=Intersection();
	return I;
}