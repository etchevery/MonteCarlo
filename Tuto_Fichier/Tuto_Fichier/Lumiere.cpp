#include "StdAfx.h"
#include "Lumiere.h"


Lumiere::Lumiere(void)
{
	this->illumination=Couleur(0,0,0);
}


Lumiere::~Lumiere(void)
{
}

void Lumiere::afficher(){
	illumination.afficher();
}

void PointLumiere::afficher(){
	cout << "Lumiere de type ponctuelle : ";
	Lumiere::afficher();
	position.afficher();
}

void DirectionelleLumiere::afficher(){
	cout << "Lumiere de type directionnelle : ";
	Lumiere::afficher();
	direction.afficher();
}

void Lumiere::initFromXML(TiXmlHandle hObj){
	illumination.initFromXML(hObj);
}

void PointLumiere::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	TiXmlElement* pElem=hObj.Element();
	Lumiere::initFromXML(hObj);
	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	this->position.Set(x,y,z);
}

void DirectionelleLumiere::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	TiXmlElement* pElem=hObj.Element();
	Lumiere::initFromXML(hObj);
	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	this->direction.Set(x,y,z);
}
