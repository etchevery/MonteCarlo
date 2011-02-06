#include "StdAfx.h"
#include "PointLumiere.h"


void PointLumiere::afficher(){
	cout << "Lumiere de type ponctuelle : ";
	Lumiere::afficher();
	position.afficher();
}
void PointLumiere::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	TiXmlElement* pElem=hObj.Element();
	Lumiere::initFromXML(hObj);
	pElem->QueryDoubleAttribute("x", &x);
	pElem->QueryDoubleAttribute("y", &y);
	pElem->QueryDoubleAttribute("z", &z);

	this->position.Set(x,y,z);
	this->falloff[0]=1.0;
	this->falloff[1]=0.0;
	this->falloff[2]=0.0;

}


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
	return Rayon(startPoint,this->getIncidence(startPoint));
}

vector3 PointLumiere::getIncidence(const vector3 &point) {
	return position-point;
}