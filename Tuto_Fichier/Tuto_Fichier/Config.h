#ifndef RENDERSETTINGSH
#define CONFIGH

#include <string>

using namespace std;

enum Echantillon
{
	UNIFORME,
	IMPORTANCE1
};

/* Configuration pour le rendu de la scène */
typedef struct config_render_struct {
	Echantillon echantillonType;
	bool directLighting;
	bool indirectLighting;
	bool russianRoulette;
	string filename;
	unsigned int nbLancerParPixel;
	unsigned int profondeur;
} Configuration;


#endif