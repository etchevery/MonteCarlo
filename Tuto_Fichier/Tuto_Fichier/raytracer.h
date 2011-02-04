#pragma once

#include "stdafx.h"
#include "common.h"
#include "Config.h"
#include "Intersection.h"
#include "Materiau.h"
#include "Rayon.h"
#include "Scene.h"
#include "Couleur.h"


using namespace std;
// -----------------------------------------------------------
// Engine class definition
// Raytracer core
// -----------------------------------------------------------
class Scene;
class Objet;
class Engine
{
public:
	Engine(Configuration config);
	~Engine();
	void SetTarget( Pixel* a_Dest, int a_Width, int a_Height );
	Scene* GetScene() { return maScene; }
	void SetScene(Scene* s){maScene=s;}
	void InitRender();
	bool Render();
	void PourcentageAvancement(int avancement,int totalPixels,int& pourcentage);

//algo principal
Couleur Raytracer( Rayon& ,int );

//MonteCarlo Fonctions: Echantillonage demi sphère Brdf
	vector3 MonteCarlo(vector3&, double); //choix entre uniforme ou avec phong
	vector3 MonteCarloUniforme(vector3& alignWithZ);//uniforme
	vector3 MonteCarloImportanceSampling(vector3&,double);// avec proba
	bool rouletteRusse(const Couleur&,double&);//roulette russe normale
    bool glossyRoulette(const Couleur& kS, const Couleur& kD, double& puissanceRestant);//choix surface glossy: spec ou diffus?
//intersection avec les lumières
	bool intersectShadowRay(Rayon& ray);

// Illumination directe
	Couleur directeIllumination( Rayon& ,Intersection& ,Reflectance);
//selon type de matériaux (diffus ou spéc)calcul couleure directe
	Couleur diffuse( Intersection& intersection, const vector3& incidence, const Couleur& color,Reflectance refl);
	Couleur speculaire( Intersection& intersection, vector3& incidence, const Couleur& color, Rayon& viewRay,Reflectance refl);

	// Illumination Indirecte, propagation du rayon
	Couleur diffusePropagation(Rayon& ray, Intersection& intersection, int depth);
    Couleur speculairePropagation(Rayon& ray, Intersection& intersection, int depth);
	Couleur reflexionPropagation(Rayon& ray, Intersection& intersection, int depth);
	Couleur refractionPropagation(Rayon& ray, Intersection& intersect, int depth);

	// Geometrie reflexion, refraction
	vector3 rayonReflexion(vector3& dir, const vector3& normal);//calcul direction rayon réfléchi
	bool Engine::isOut(vector3& direction,vector3& normal);//test rayon entrant ou sortant objet à refraction
    bool calculRefraction(Rayon& ray, Intersection& intersection, Rayon& refrRay, double& schlick);//calcul rayon refracté
	double calculSchlick(double n, double nt, vector3& rayDir, vector3& refrDir, vector3& surfNorm);//approximation schilck
	bool isRefracte(vector3 direction, vector3 normal, Intersection& intersect, double oldIndex, double newIndex, vector3& refractDirection);//savoir si il y a refraction

protected:
	// renderer data
	float m_WX1, m_WY1, m_WX2, m_WY2, m_DX, m_DY, m_SX, m_SY;
	int m_Width, m_Height;
	Configuration config;
	Scene* maScene;
	//Stokage résultat
	Pixel* m_Dest;
};