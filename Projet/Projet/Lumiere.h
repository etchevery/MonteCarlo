#pragma once

#include "stdafx.h"
/*****************************************************************************/
/*                      Abstract Lumiere Class								 */
/******************* **********************************************************/
/* General abstract Lumiere class used to dervie all other Lumieres for the ray
 * tracer.
 */

class Lumiere {
    public:
        Lumiere();     
        virtual ~Lumiere();
        Lumiere(const Couleur color, double cfall, double lfall, double qfall);
        
        //retourne l'apport de lumière sur le point
        virtual Couleur getIntensity(const vector3& refPoint)=0;
		//rayon entre la lumiere et l'intersection
		virtual Rayon getShadowRay(const vector3& startPoint)=0;

		virtual void initFromXML(TiXmlHandle hObj);
		virtual void afficher();
    protected:
        Couleur illumination;
		double falloff[3];// const, linear quadratic
};
