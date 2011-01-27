#pragma once
#include "Objet.h"
#include "Rayon.h"
#include "Sphere.h"
#include "Plan.h"
#include "common.h"

class Scene
{
private :
	int nb_obj;
	Objet** tab_obj;
	//Lumiere* tab_lum;
	Camera* camera;

public:
	Scene(void);
	~Scene(void);
	void chargerScene(const char* pFilename);
	void afficherScene();
};

