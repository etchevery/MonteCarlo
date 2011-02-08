#pragma once

#include "stdafx.h"
#include "Lumiere.h"
#include "tinyxml.h"
#include <tinyxml.h>
/*****************************************************************************/
/*                          Point Lumiere Class                                */
/*****************************************************************************/
class PointLumiere: public Lumiere {
    public:
		PointLumiere(){position=vector3(0,0,0);};
		PointLumiere(double x, double y, double z, const Couleur color, double cfall=1.0, double lfall=0.0, double qfall=0.0);
		Rayon getShadowRay(const vector3& startPoint);
		Couleur getIntensity(const vector3& refPoint);
		void initFromXML(TiXmlHandle hObj);
		void afficher();
		vector3 getPosition(){return position;};
	private:
		vector3 position;
		vector3 getIncidence(const vector3 &point);
};