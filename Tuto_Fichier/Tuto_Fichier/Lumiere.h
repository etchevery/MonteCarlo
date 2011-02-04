#pragma once

#include "Stdafx.h"

/*****************************************************************************/
/*                      Abstract Lumiere Class								 */
/******************* **********************************************************/
/* General abstract Lumiere class used to dervie all other Lumieres for the ray
 * tracer.
 */
class Lumiere {
    public:
        // Default Constructor
        Lumiere();
        
        // Default Destructor
        virtual ~Lumiere();
        
        // Constructs a Lumiere given the position and the color.
        // Parameters: 
        //      x, y, z - Position of the Lumiere.
		//		*fall - const, linear, quadratic falloff terms
        Lumiere(const Couleur color, double cfall=1.0, double lfall=0.0, double qfall=0.0);
        
        //retourne l'apport de lumière sur le point
        virtual Couleur getIntensity(const vector3& refPoint)=0;
		//rayon entre la lumiere et l'intersection
		virtual Rayon getShadowRay(const vector3& startPoint)=0;

		virtual void initFromXML(TiXmlHandle hObj);
		virtual void afficher();
    protected:
        Couleur illumination;
		double falloff[3];		// const, linear quadratic
};

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
	private:
		vector3 position;
		vector3 getIncidence(const vector3 &point);
};

/*****************************************************************************/
/*                     Directional Lumiere Class                               */
/*****************************************************************************/
/*class DirectionelleLumiere: public Lumiere {
    public:
		DirectionelleLumiere(){direction=vector3(0,0,0);};
		DirectionelleLumiere(double x, double y, double z, const Couleur color);
		//Rayon getShadowRay(const vector3& startPoint, double bias, Rayon& viewRay);
			Rayon getShadowRay(const vector3& startPoint);
		Couleur getIntensity(const vector3& refPoint);
		void initFromXML(TiXmlHandle hObj);
		void afficher();
	private:
		vector3 direction;
		vector3 getIncidence(const vector3 &point);
};
*/