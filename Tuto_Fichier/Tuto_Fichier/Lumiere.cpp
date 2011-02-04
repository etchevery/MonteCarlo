#include "StdAfx.h"
#include "Lumiere.h"

Lumiere::Lumiere(Couleur color, double cfall, double lfall, double qfall) {
    illumination = color;
	falloff[0] = cfall;
	falloff[1] = lfall;
	falloff[2] = qfall;
}
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

/*void DirectionelleLumiere::afficher(){
	cout << "Lumiere de type directionnelle : ";
	Lumiere::afficher();
	direction.afficher();
}*/

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

/*void DirectionelleLumiere::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	TiXmlElement* pElem=hObj.Element();
	Lumiere::initFromXML(hObj);
	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	this->direction.Set(x,y,z);
}*/
Couleur PointLumiere::getIntensity(const vector3& refPoint)
{
	double c, l, q;
	c = falloff[0];
	l = 0;
	q = 0;
	if (falloff[1] != 0 || falloff[2] != 0){
		double r = (position - refPoint).Length();
		l = falloff[1]*r;
		q = falloff[2]*r*r;
	}
	return illumination / (c+l+q);
}

Rayon PointLumiere::getShadowRay(const vector3& startPoint){

	return Rayon(startPoint, this->getIncidence(startPoint));
}

vector3 PointLumiere::getIncidence(const vector3 &point) {
	vector3 incidence = position - point;
    return incidence;
}