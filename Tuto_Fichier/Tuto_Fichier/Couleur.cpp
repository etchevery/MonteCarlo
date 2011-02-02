#include "StdAfx.h"
#include "Couleur.h"

void Couleur::initFromXML(TiXmlHandle hObj){
	double r,g,b;
	TiXmlElement* pElem=hObj.Element();
	pElem->QueryDoubleAttribute("red", &r);
	pElem->QueryDoubleAttribute("green", &g);
	pElem->QueryDoubleAttribute("blue", &b);

	this->Set(r,g,b);
}
Couleur& Couleur::clamp() {

	r = MIN(1.0, r);
	g = MIN(1.0, g);
	b = MIN(1.0, b);

	r = MAX(0.0, r);
	g = MAX(0.0, g);
	b = MAX(0.0, b);

	return *this;
}

Couleur& Couleur::normalize()
{
	double maxcomp = MAX(r, MAX(g,b));
	double n[3];
	n[0] /= maxcomp;
	n[1] /= maxcomp;
	n[2] /= maxcomp;

	return *this;
}

const Couleur Couleur::black = Couleur(0,0,0);