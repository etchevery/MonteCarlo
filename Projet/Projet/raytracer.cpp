#include "stdafx.h"
#include "raytracer.h"
#include <omp.h>


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
	m_Width = a_Width;//taille écran
	m_Height = a_Height;
}

bool Engine::rouletteRusse(const Couleur& couleur, double& puissanceRestant)
{
	double p = MAX(couleur.r, MAX(couleur.g, couleur.b));
	puissanceRestant = 1.0/p;
	return (Rand(1.0) > p);
}

bool Engine::glossyRoulette(const Couleur& kS, const Couleur& kD, double& puissanceRestant){
	double spec = MAX(kS.r, MAX(kS.g, kS.b));//composante max spéculaire
	double diffuse = MAX(kD.r, MAX(kD.g, kD.b));//composante max diffus
    double p = spec/(spec + diffuse);//calcul proba
	puissanceRestant = 1.0/p;
    return (Rand(1.0) > p);
}

vector3 Engine::MonteCarlo(vector3& normale, double nPhong){

	if (config.echantillonType == UNIFORME)
		return MonteCarloUniforme(normale);
	else //sinon model de phong
		return MonteCarloImportanceSampling(normale,nPhong);
}

vector3 Engine::MonteCarloUniforme(vector3& normale){
	double r1, r2, r3, x, y, z;
	r1 = Rand(1.0);
	r2 = Rand(1.0);
	r3 = Rand(1.0);

	x = 1 - 2*r1;
	y = 1 - 2*r2;
	z = 1 - 2*r3;
	vector3 rayon(x, y, z);

	if (rayon.Dot(normale)<0.0)
		rayon = -rayon;

	rayon.Normalize();

	return rayon;
}


vector3 Engine::MonteCarloImportanceSampling(vector3& normale, double nPhong){
	double z =Rand(1.0);
	double phi = Rand(1.0) * 2.0 * PI;//composante phi aléatoire
	double theta = acos(pow(z, 1/(nPhong+1)));//composante theta aléatoire avec prise en compte indice de phong

	double sintheta = sin(theta);
	//création du rayon
	vector3 rayon(sintheta*cos(phi), sintheta*sin(phi), z);

	//Alignement avec la normale
	vector3 t(Rand(1.0),Rand(1.0),Rand(1.0));
	vector3 u = t.Cross(normale);
	u.Normalize();
	vector3 v = normale.Cross(u);
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
	if (!maScene->intersect(a_Ray,intersection)){
		if(a_Depth==0)
		return Couleur(1,1,1);
		else
			return Couleur::black;
	}

	//on récupère les informations de matériaux (couleurs)
	Reflectance refl = intersection.getObjet()->GetReflectance();

	// Initialise couleur de base de l'objet
	Couleur couleur=refl.kE;
	//puissance du rayons (tend vers l'annulation après réflexion)
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
				couleur += restant*(1.0/(1.0-1.0/rrMult))*diffusePropagation(a_Ray, intersection, a_Depth);
			else
				couleur += restant*rrMult*speculairePropagation(a_Ray,intersection,a_Depth);
			
		break;
	case Mirror:
		couleur += restant*reflexionPropagation(a_Ray, intersection, a_Depth);
		break;
	default://refraction
		Rayon refrRay;
		double schlick;
		if(calculRefraction(a_Ray, intersection, refrRay, schlick))
			couleur += restant*(1-schlick)*refractionPropagation(refrRay,intersection,a_Depth);

		couleur += restant*schlick*reflexionPropagation(a_Ray, intersection, a_Depth);
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
	//intersection.afficher();
		//le rayon entre l'intersection et la lumiere
		Rayon shadowRay = lumiere[i]->getShadowRay(intersection.getPoint());			
		//si on a pas d'intersection on calcul l'apport de cette lumiere
		if (!intersectShadowRay(shadowRay)) {
			vector3 lightIncidence = shadowRay.getDirection();
			lightIncidence.Normalize();
			if (refl.kD != Couleur::black)//si la composante est diffuse
				couleur += diffuse(intersection, lightIncidence,lumiere[i]->getIntensity(intersection.getPoint()),refl);
				
			if (refl.kS != Couleur::black)//si la composante est speculaire
				couleur += speculaire(intersection, lightIncidence,lumiere[i]->getIntensity(intersection.getPoint()),ray,refl);
		}
	}
	return couleur;
}

//intersection entre un rayon d'ombre et un objet
bool Engine::intersectShadowRay(Rayon& ray) {
	Intersection rec;
	bool test=maScene->intersect(ray,rec);
	return(test);
}

//calcul de la composante diffuse éclairage direct
Couleur Engine::diffuse( Intersection& intersection, const vector3& incidence, const Couleur& color,Reflectance refl) {
	vector3 normale(intersection.getNormal());
	vector3 incidant(incidence);
	return refl.kD*color*MAX(normale.Dot(incidant),0.0);
}

//calcul de la composante speculaire éclairage direct
Couleur Engine::speculaire( Intersection& intersection,vector3& incidence, const Couleur& color, Rayon& viewRay,Reflectance refl) {
	vector3 reflechi =-rayonReflexion(incidence,intersection.getNormal());
	vector3 vuRay = -viewRay.getDirection();
	vuRay.Normalize();
	return refl.kS*color*pow(MAX(reflechi.Dot(vuRay),0),refl.pExp);
}


Couleur Engine::diffusePropagation(Rayon& ray, Intersection& intersection, int depth){
	//nouvelle direction
	vector3 rayDir = MonteCarlo(intersection.getNormal(),1.0);
	//creation nouveau rayon 
	Rayon newRay(intersection.getPoint(),rayDir);
	//on relance le calcul avec la nouvelle direction
	Couleur diffColor = Raytracer(newRay,depth+1);
	//apport de la composante diffuse
	Couleur diffusComp = intersection.getObjet()->GetReflectance().kD;
	//brdf = kD/PI
	if (config.echantillonType == UNIFORME)// Probablity: 1/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return 2 * (intersection.getNormal().Dot(rayDir))*diffusComp*diffColor;
	else// Probability: cos(theta)/PI -- (1/probability)*cos(theta)*brdf*radiancealongray	
		return diffusComp*diffColor;

}

Couleur Engine::speculairePropagation(Rayon& ray, Intersection& intersection, int depth){
	//reflectance matériau
	Reflectance refl = intersection.getObjet()->GetReflectance();
	//nouvelle direction
	vector3 rayDir = MonteCarlo(intersection.getNormal(), refl.pExp);
    //creation nouveau rayon 
	Rayon newRay(intersection.getPoint(),rayDir);
	//on relance le calcul avec la nouvelle direction
    Couleur specColor = Raytracer(newRay,depth+1);

	//direction réfléchie
	vector3 reflexionDir = rayonReflexion(ray.getDirection(),intersection.getNormal());
	reflexionDir.Normalize();

	// brdf = kS(pExp+1)/(2PI)
	if (config.echantillonType == UNIFORME)// Probablity: 1/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return refl.kS*specColor*pow(rayDir.Dot(reflexionDir),refl.pExp)*(rayDir.Dot(intersection.getNormal()))*(refl.pExp+1) ;
	else// Probability: cos^pExp(theta)*(pExp+1)/(2PI) -- (1/probability)*cos(theta)*brdf*radiancealongray
		return refl.kS*specColor*(rayDir.Dot(intersection.getNormal()));
}


Couleur Engine::reflexionPropagation(Rayon& ray, Intersection& intersection, int depth){
	Couleur reflColor;
	Reflectance refl = intersection.getObjet()->GetReflectance();
	vector3 rayDirection = ray.getDirection();
	//direction réfléchie
	vector3 reflectDirection = rayonReflexion(rayDirection,intersection.getNormal());
	//nouveau rayon aprés réflexion
	Rayon newRayon(intersection.getPoint(),reflectDirection);
	//on relance
	reflColor = Raytracer(newRayon,depth+1);

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
	vector3 normale = intersection.getNormal();

	double index = intersection.getObjet()->GetReflectance().indiceRefraction;

	if (isOut(direction,normale)){//si va dedans
		n = 1.0;
		nt = index;
	}else{//sinon il ressort vers l'air
		normale = -normale;
		n = index;
		nt = 1.0;
	}
	//savoir si le rayon est entièrement réfléchie ou réfracté
	bool refracted = isRefracte(direction,normale, n, nt, refractDirection);

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

bool Engine::isRefracte(vector3 direction, vector3 normal, double oldIndex, double newIndex, vector3& refractDirection) {
    double n = oldIndex/newIndex;
    double c = direction.Dot(normal);
    double cosPhi2 = (1 - ((n * n) * (1 - (c * c))));
    
    if (cosPhi2 > 0){
        double cosPhi = sqrt(cosPhi2);
        vector3 term1 = n * (direction - normal * (c));
        refractDirection = term1 - normal * cosPhi2;
        return true;
    }
//si négatif tout est réfléchi
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
	//formule schlick
	return F0 + (1-F0)*pow(c,5);
}

Couleur Engine::refractionPropagation(Rayon& ray,Intersection& intersection, int depth) {
	Couleur coef = intersection.getObjet()->GetReflectance().kT;
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
	m_DX = (m_WX2 - m_WX1) / m_Width;//pas pixel axe OX
	m_DY = (m_WY2 - m_WY1) / m_Height;//pas pixel axe OY
	m_SY += m_DY;
}

// -----------------------------------------------------------
// Engine::Render
// Fires rays in the scene one scanline at a time, from left
// to right
// -----------------------------------------------------------
bool Engine::Render(){

	//ici notre camera
	vector3 origineCam(this->maScene->getCamera()->getPosition());

	// reset last found primitive pointer
	int pos = 0, pos1 = 0,totalPixels = m_Height*m_Width,avancement=0,pourcentage =0;
	Couleur acc;
	float pasPixelX=abs(m_DX)/(float)config.nbLancerParPixel;
	float pasPixelY=abs(m_DY)/(float)config.nbLancerParPixel;
	img_color = new Couleur[totalPixels];
	// Boucle sur les pixel de l'écran
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	int x=0;
	//#pragma omp parallel shared(m_Height) private(x){
	for ( int y = 0; y < m_Height; y++){
		m_SX = m_WX1;
		for ( x = 0; x < m_Width; x++ ){
			// Initialisation de la couleur
			acc=Couleur::black;		
			//boucle à l'interieur du pixel	
			for (float k=0.0; k<abs(m_DY); k+=pasPixelY)
			  for (float l=0.0; l<abs(m_DX); l+=pasPixelX){
			//Initialisation rayon
			vector3 dir = vector3(m_SX+l,m_SY-k,0.0) - origineCam;
			dir.Normalize();
			Rayon r(origineCam, dir);
			 //on procède au calcul 
			acc+=Raytracer(r,0);
			}
			//moyenne sur tous les rayons
			acc = acc /(pow((float)config.nbLancerParPixel,2));
			//clamper la couleur
			acc.clamp();
			//transformation en RGB standard
			acc.ToRgb();

			m_Dest[pos++] = (int(acc.r) << 16) + (int(acc.g) << 8) + int(acc.b);
			img_color[pos1] = acc;
			
			pos1++;
			//pixel suivant axe 0X
			m_SX += m_DX;
			PourcentageAvancement(avancement++,totalPixels,pourcentage);
		}
		//Pixel suivant axe 0Y
		m_SY += m_DY;
	}

	PourcentageAvancement(totalPixels,totalPixels,pourcentage);
	return true;
}


void Engine::PourcentageAvancement(int avancement,int totalPixels,int& pourcentage){
		int newPourcentage = (int)((float)avancement/totalPixels*100);
		if (newPourcentage > pourcentage){
			pourcentage = newPourcentage;
			cout << "Effectue : " << pourcentage << "%"<<endl;
		}
}

