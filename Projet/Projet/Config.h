#pragma once

enum Echantillon{UNIFORME,IMPORTANCE};

/* Configuration pour le rendu de la sc�ne */
typedef struct config_render_struct {
	Echantillon echantillonType;
	bool directLighting;
	bool indirectLighting;
	bool russianRoulette;
	char* filename;
	 int nbLancerParPixel;
	 int profondeur;
} Configuration;