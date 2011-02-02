#ifndef LUMIERE_H
#define LUMIERE_H

#include "stdafx.h"
#include <vector>
#include "common.h"
#include "raytracer.h"
#include "Couleur.h"

using namespace std;
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
        
        // Returns the color of the Lumiere.
        //virtual Couleur getIntensity(const vector3& refPoint);

		// Gets the shadow ray from STARTPOINT to this Lumiere object given a
		// certain shadow BIAS.
		//virtual Rayon getShadowRay(const vector3& startPoint, Rayon& viewRay);

		// For pathtracing. Implemented only for area Lumieres.
		//virtual void sample(const vector3& fromPoint, const vector3& fromNormal, Couleur& intensity, vector3& incidence);
		
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
    // Constructs a point Lumiere given the position and the color.
    // Parameters: 
    //      x, y, z - Position of the Lumiere.
    //      color - The Couleur values regarding the color of the Lumiere
    public:
		PointLumiere(){position=vector3(0,0,0);};
		PointLumiere(double x, double y, double z, const Couleur color, double cfall=1.0, double lfall=0.0, double qfall=0.0);
		//Rayon getShadowRay(const vector3& startPoint, double bias, Rayon& viewRay);
		//Couleur getIntensity(const vector3& refPoint);
		void initFromXML(TiXmlHandle hObj);
		void afficher();
	private:
		vector3 position;
		vector3 getIncidence(const vector3 &point);
};

/*****************************************************************************/
/*                     Directional Lumiere Class                               */
/*****************************************************************************/
class DirectionelleLumiere: public Lumiere {
    public:
		DirectionelleLumiere(){direction=vector3(0,0,0);};
		DirectionelleLumiere(double x, double y, double z, const Couleur color);
		//Rayon getShadowRay(const vector3& startPoint, double bias, Rayon& viewRay);
		void initFromXML(TiXmlHandle hObj);
		void afficher();
	private:
		vector3 direction;
		vector3 getIncidence(const vector3 &point);
};

#endif