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

Lumiere::~Lumiere(void){}

void Lumiere::afficher(){
	illumination.afficher();
}



void Lumiere::initFromXML(TiXmlHandle hObj){
	illumination.initFromXML(hObj);
}

