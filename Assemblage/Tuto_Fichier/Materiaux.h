#ifndef MATERIAUXH
#define MATERIAUXH

#include "common.h"
#include "Couleur.h"
#include <string>

using namespace std;


/* Types de BRDF */
enum BRDFType{
	Emissive,
	Diffuse,
	Glossy,
	Mirror,
	Dielectric
};

/* Structure pour la reflectance du materiaux */
typedef struct reflectance_struct {

	BRDFType type;		// Type de BRDF 
	Couleur kE;			// Emissive
	Couleur kD;			// Diffuse
	Couleur kS;			// Speculaire reflection
	Couleur kR;			// Mirroir reflection
    Couleur kT;        //refraction
    double indiceRefraction; // Indice de refraction
	double pExp;		// Phong puissance
} Reflectance;



/* Material objects hold information one might need
   about the material of an object in a Scene. */
class Materiaux {

protected:
	Reflectance myReflectance;

public:

	/* Constructeurs */
	Materiaux();
	Materiaux(Reflectance reflec);
	virtual Reflectance getReflectance();
};
#endif

