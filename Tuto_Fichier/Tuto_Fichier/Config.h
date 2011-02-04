#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using namespace std;

enum Echantillon{UNIFORME,IMPORTANCE};

/* Configuration pour le rendu de la scène */
typedef struct config_render_struct {
	Echantillon echantillonType;
	bool directLighting;
	bool indirectLighting;
	bool russianRoulette;
	char* filename;
	 int nbLancerParPixel;
	 int profondeur;
} Configuration;


#endif