#pragma once

#include "stdafx.h"
#include "Objet.h"
#include "Plan.h"
#include "Sphere.h"
#include "Tetraedre.h"
#include "tinyxml.h"
#include <tinyxml.h>
class Scene
{
private :
	int nb_obj;
	Objet** tab_obj;
	int nb_lum;
	Lumiere** tab_lum;
	Camera* camera;

public:
	Scene(void);
	~Scene(void);
	void chargerScene(const char* pFilename);
	void afficherScene();
	Objet** getObjets(){return tab_obj;};
	int getNbObjets(){return nb_obj;};
	Lumiere** getLumieres(){return tab_lum;};
	int getNbLumieres(){return nb_lum;};
	bool intersect(Rayon& r, Intersection& I);

};

