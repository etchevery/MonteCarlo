// -----------------------------------------------------------
// raytracer.cpp
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------
#include "stdafx.h"
#include "raytracer.h"
#include "Scene.h"
#include "common.h"
#include "windows.h"
#include "winbase.h"
#include "Couleur.h"
#include "Rayon.h"
#include "Intersection.h"
#include <iostream>

using namespace std;
Engine::Engine(Configuration config)
{
	maScene = new Scene();
	this->config=config;
}

Engine::~Engine()
{
	delete maScene;
}

// -----------------------------------------------------------
// Engine::SetTarget
// Sets the render target canvas
// -----------------------------------------------------------
void Engine::SetTarget( Pixel* a_Dest, int a_Width, int a_Height ){
	m_Dest = a_Dest;//stockage image
	m_Width = a_Width;//taille �cran
	m_Height = a_Height;
}

bool Engine::rouletteRusse(const Couleur& couleur, double& puissanceRestant)
{
	double p = MAX(couleur.r, MAX(couleur.g, couleur.b));
	puissanceRestant = 1.0/p;
	return (rand() > p);
}

bool Engine::glossyRoulette(const Couleur& kS, const Couleur& kD, double& puissanceRestant){
	double spec = MAX(kS.r, MAX(kS.g, kS.b));//composante max sp�culaire
	double diffuse = MAX(kD.r, MAX(kD.g, kD.b));//composante max diffus
    double p = spec/(spec + diffuse);//calcul proba
	puissanceRestant = 1.0/p;
    
    return (rand() > p);
}

vector3 Engine::MonteCarlo(vector3& normale, double nPhong){

	if (config.echantillonType == UNIFORME)
		return MonteCarloUniforme(normale);
	else //sinon model de phong
		return MonteCarloImportanceSampling(normale,nPhong);
}

vector3 Engine::MonteCarloUniforme(vector3& normale){
	double r1, r2, r3;
	double x, y, z;
	r1 = rand();
	r2 = rand();
	r3 = rand();

	x = 1 - 2*r1;
	y = 1 - 2*r2;
	z = 1 - 2*r3;
	vector3 rayon(x, y, z);

	if (rayon.Dot(normale) < 0)
		rayon = -rayon;

	rayon.Normalize();
	return rayon;
}


vector3 Engine::MonteCarloImportanceSampling(vector3& normale, double nPhong){

	double phi = rand() * 2 * PI;//composante phi al�atoire
	double theta = acos(pow(rand(), 1/(nPhong+1)));//composante theta al�atoire avec prise en compte indice de phong

	double sintheta = sin(theta);
	//cr�ation du rayon
	vector3 rayon(sintheta*cos(phi), sintheta*sin(phi), cos(theta));

	//Alignement avec la normale
	vector3 t(rand(),rand(),rand());
	vector3 u = t*normale;
	u.Normalize();
	vector3 v = normale*u;
	mat3 rot(u,v,normale);
	rot = rot.transpose();
	return rot*rayon;
}
// -----------------------------------------------------------
// Engine::Raytrace
// -----------------------------------------------------------
Couleur Engine::Raytracer( Rayon& a_Ray, int a_Depth){
	Intersection intersection;//stocke les informations sur l'intersection

	// si on ne touche rien on quitte
	if (!maScene->intersect(a_Ray,&intersection))
	 return Couleur::black;

	//on r�cup�re les informations de mat�riaux (couleurs)
	Reflectance refl = intersection.getObjet()->GetReflectance();

	// Initialise couleur de base de l'objet
	Couleur couleur = refl.kE;
	//puissance du rayons (tend vers l'annulation apr�s r�flexion)
	double restant = 1.0;
	//mode de fin du rayon
	if (a_Depth > config.profondeur){
		if(config.russianRoulette){//roulette russe
		Couleur poids;
		switch(refl.type){
		case Diffuse:	 poids = refl.kD; break;//diffus
		case Glossy: poids = refl.kD + refl.kS; break;//glossy
		case Mirror: poids = refl.kR; break;//reflexion
		default:  poids = refl.kT; break;//refraction
		}
		if (rouletteRusse(poids, restant))
			return couleur;
		}else //mode de terminaison sur la profondeur
		return couleur;
	}

	//selon le type de materiaux
	switch(refl.type){
	case Diffuse:
		couleur += directeIllumination(a_Ray,intersection,refl);
		couleur += restant*diffusePropagation(a_Ray,intersection,a_Depth);
		break;
	case Glossy:
		couleur += directeIllumination(a_Ray,intersection,refl);
			double rrMult;
			if (glossyRoulette(refl.kS, refl.kD, rrMult))
				couleur += restant*(1.0/(1.0-1.0/rrMult)) * diffusePropagation(a_Ray, intersection, a_Depth);
			else
				couleur += restant*rrMult*speculairePropagation(a_Ray,intersection,a_Depth);
		break;
	case Mirror:
		couleur += restant*reflexionPropagation(a_Ray, intersection, a_Depth);
		break;
	default://refraction
		Rayon refrRay;
		double schlick;
		bool refracte = calculRefraction(a_Ray, intersection, refrRay, schlick);
		couleur += restant*schlick*reflexionPropagation(a_Ray, intersection, a_Depth);

		if (refracte)
		couleur += restant*(1-schlick)*refractionPropagation(refrRay,intersection,a_Depth);
		break;
	}
	return couleur;
}

Couleur Engine::directeIllumination(Rayon& ray, Intersection& intersection,Reflectance refl)
{
	Couleur couleur;
	Lumiere** lumiere = maScene->getLumieres();

	//parcourt des lumieres
	for (int i = 0; i < maScene->getNbLumieres(); i++) {
		//le rayon entre l'intersection et la lumiere
		/*Rayon shadowRay = lumiere[i]->getShadowRay(intersection.getPoint(),ray);
		//si on a pas d'intersection on calcul l'apport de cette lumiere
		if (!intersectShadowRay(shadowRay)) {
			vector3 lightIncidence = shadowRay.getDirection();
			lightIncidence.Normalize();
			if (refl.kD != Couleur::black)//si la composante est diffuse
				couleur += diffuse(intersection, lightIncidence,lumiere[i]->getIntensity(),refl);
			if (refl.kS != Couleur::black)//si la composante est speculaire
				couleur += speculaire(intersection, lightIncidence,lumiere[i]->getIntensity(),ray,refl);
		}*/
	}
	return couleur;
}

//intersection entre un rayon d'ombre et un objet
bool Engine::intersectShadowRay(Rayon& ray) {
	Intersection rec;
	return true;//(maScene->intersect(ray,&rec));
}

//calcul de la composante diffuse �clairage direct
Couleur Engine::diffuse( Intersection& intersection, const vector3& incidence, const Couleur& color,Reflectance refl) {
	vector3 normale(intersection.getNormal());
	vector3 incidant(incidence);
	return refl.kD*color* MAX(normale.Dot(incidant),0.0);
}

//calcul de la composante speculaire �clairage direct
Couleur Engine::speculaire( Intersection& intersection,vector3& incidence, const Couleur& color, Rayon& viewRay,Reflectance refl) {
	vector3 reflechi =-rayonReflexion(incidence,intersection.getNormal());
	vector3 vuRay = -viewRay.getDirection();
	vuRay.Normalize();
	double scalarTerm = MAX(reflechi.Dot(vuRay),0);

	return refl.kS*color*pow(scalarTerm,refl.pExp);
}


Couleur Engine::diffusePropagation(Rayon& ray, Intersection& intersection, int depth){
	//apport de la composante diffuse
	Couleur diffusComp = intersection.getObjet()->GetReflectance().kD;
	//nouvelle direction
	vector3 rayDir = MonteCarlo(intersection.getNormal(),1);
	//creation nouveau rayon 
	Rayon newRay(intersection.getPoint(),rayDir);
	//on relance le calcul avec la nouvelle direction
	Couleur diffColor = Raytracer(newRay, depth + 1);

	//brdf = kD/PI
	if (config.echantillonType == UNIFORME)// Probablity: 1/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return 2 * (intersection.getNormal().Dot(rayDir))*diffusComp*diffColor;
	else if (config.echantillonType == IMPORTANCE)// Probability: cos(theta)/PI -- (1/probability)*cos(theta)*brdf*radiancealongray	
		return diffusComp*diffColor;
	else return Couleur::black;
}

Couleur Engine::speculairePropagation(Rayon& ray, Intersection& intersection, int depth){
	//apport de la composante sp�caulaire
	Reflectance refl = intersection.getObjet()->GetReflectance();
	//direction r�fl�chie
	vector3 reflexionDir = rayonReflexion(ray.getDirection(), intersection.getNormal());
	reflexionDir.Normalize();
	//nouvelle direction
	vector3 rayDir = MonteCarlo(reflexionDir, refl.pExp);
    //creation nouveau rayon 
	Rayon newRay(intersection.getPoint(),rayDir);
	//on relance le calcul avec la nouvelle direction
    Couleur specColor = Raytracer(newRay,depth+1);
    
	// brdf = kS(pExp+1)/(2PI)
	if (config.echantillonType == UNIFORME)
		// Probablity: 1/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return pow(rayDir.Dot(reflexionDir),refl.pExp) *(rayDir.Dot(intersection.getNormal())) * refl.kS * (refl.pExp+1) * specColor;
	else if (config.echantillonType == IMPORTANCE)
		// Probability: cos^pExp(theta)*(pExp+1)/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return refl.kS * specColor*(rayDir.Dot(intersection.getNormal()));
	else return Couleur::black;
}


Couleur Engine::reflexionPropagation(Rayon& ray, Intersection& intersection, int depth){
	Couleur reflColor;
	Reflectance refl = intersection.getObjet()->GetReflectance();
	vector3 rayDirection = ray.getDirection();
	//direction r�fl�chie
	vector3 reflectDirection = rayonReflexion(rayDirection,intersection.getNormal());
	//nouveau rayon apr�s r�flexion
	Rayon newRayon(intersection.getPoint(),reflectDirection);
	//on relance
	reflColor = Raytracer(newRayon,depth+1);

	//Si Reflexion
	if (refl.kT == Couleur::black)
		return refl.kR*reflColor;
	else
		return refl.kT*reflColor;
}

vector3 Engine::rayonReflexion(vector3& dir, const vector3& normal){
	return dir - (2 * (dir.Dot(normal)) * normal);
}

bool Engine::isOut(vector3& direction,vector3& normale){
	// Rayon va dedans ou dehors: negatif= va dedans
	normale.Normalize();
	double cosAngle = direction.Dot(normale);
	return (cosAngle  < 0);
	}


bool Engine::calculRefraction(Rayon& ray, Intersection& intersection, Rayon& refrRay, double& schlick){
	double n, nt;
	vector3 refractDirection;
	schlick=1.0;
	vector3 direction = ray.getDirection();
	direction.Normalize();
	vector3 normal = intersection.getNormal();

	Reflectance refl = intersection.getObjet()->GetReflectance();
	double index = refl.indiceRefraction;

	if (isOut(direction,normal)){//si va dedans
		n = 1.0;
		nt = index;
	}else{//sinon il ressort vers l'air
		normal = -normal;
		n = index;
		nt = 1.0;
	}
	//savoir si le rayon est enti�rement r�fl�chie ou r�fract�
	bool refracted = isRefracte(direction,normal, intersection, n, nt, refractDirection);
	// Calcul de l'approximation de Schlick  des equations de Fresnel 
	if (refracted){
	vector3 d  = ray.getDirection();
	d.Normalize();
	vector3 rd = refractDirection;
	rd.Normalize();
	refrRay = Rayon(intersection.getPoint(),refractDirection);
	schlick = calculSchlick(n, nt, d, rd,intersection.getNormal());
	}
	return refracted;
}

bool Engine::isRefracte(vector3 direction, vector3 normal,  Intersection& intersect, double oldIndex, double newIndex, vector3& refractDirection) {
    double n = oldIndex/newIndex;
    double c = direction.Dot(normal);
    double cosPhi2 = (1 - ((n * n) * (1 - (c * c))));
    
    if (cosPhi2 > 0){
        double cosPhi = sqrt(cosPhi2);
        vector3 term1 = n * (direction - normal * (c));
        refractDirection = term1 - normal * cosPhi2;
        return true;
    }
//si n�gatif tout est r�fl�chi
return false;
}

double Engine::calculSchlick(double n, double nt, vector3& rayDir, vector3& refrDir, vector3& surfNorm){
	double n1, n2,sens=1.0;
	n1 = MAX(n,nt);
	n2 = MIN(n,nt);
	double F0 = pow(((n1-n2)/(n1+2)),2);

	if (isOut(rayDir,surfNorm))
	sens=-1.0;
	double c = 1.0 - (sens*(refrDir.Dot(surfNorm)));
	return F0 + (1-F0)*pow(c,5);
}

Couleur Engine::refractionPropagation(Rayon& ray,Intersection& intersection, int depth) {
	Reflectance refl = intersection.getObjet()->GetReflectance();
	Couleur coef = refl.kT;
	return coef * Raytracer(ray, depth+1);
}
// -----------------------------------------------------------
// Engine::InitRender
// Initializes the renderer, by resetting the line / tile
// counters and precalculating some values
// -----------------------------------------------------------
void Engine::InitRender()
{
	// screen plane in world space coordinates
	m_WX1 = -4, m_WX2 = 4, m_WY1 = m_SY = 3, m_WY2 = -3;
	// calculate deltas for interpolation
	m_DX = (m_WX2 - m_WX1) / m_Width;
	m_DY = (m_WY2 - m_WY1) / m_Height;
	m_SY += m_DY;
}

// -----------------------------------------------------------
// Engine::Render
// Fires rays in the scene one scanline at a time, from left
// to right
// -----------------------------------------------------------
bool Engine::Render()
{
	// render scene
	//ici notre camera
	//camera.h stoke origine et taille �cran normalement
	vector3 o( 0, 0, -5 );

	// reset last found primitive pointer
	int pos = 0;
	int totalPixels = m_Height*m_Width;
	int avancement,pourcentage =0;
	float dist;
	Couleur acc;
	float pasPixel=1.0f/(float)config.nbLancerParPixel;

	// Boucle sur les pixel de l'�cran
	for ( int y = 0; y < m_Height; y++ )
	{
		m_SX = m_WX1;
		for ( int x = 0; x < m_Width; x++ )
		{
			// Initialisation de la couleur
			acc=Couleur::black;
			//boucle � l'interieur du pixel 
			for (float k=0; k<1; k+=pasPixel)
			  for (float l=0; l<1; l+=pasPixel){

			//Initialisation rayon
			vector3 dir = vector3( m_SX+k*pasPixel, m_SY+l*pasPixel,0) - o;
			dir.Normalize();
			Rayon r(o, dir);
			  
			acc+=Raytracer( r, 1);
			}
			//moyenne sur tous les rayons
			acc = acc / (float)(config.nbLancerParPixel*config.nbLancerParPixel);
			//clamper la couleur
			acc.clamp();
			//transformation en RGB standard
			acc.ToRgb();

			m_Dest[pos++] = (int(acc.r) << 16) + (int(acc.g) << 8) + int(acc.b);
			

			m_SX += m_DX;
		}
		m_SY += m_DY;
		avancement++;
		PourcentageAvancement(avancement,totalPixels,pourcentage);
	}
	// all done
	return true;
}


void Engine::PourcentageAvancement(int avancement,int totalPixels,int& pourcentage){
		int newPourcentage = (int)((float)avancement/totalPixels*100);
		if (newPourcentage > pourcentage)
		{
			pourcentage = newPourcentage;
			cout << "Percent Complete: " << pourcentage << endl;
		}
}

