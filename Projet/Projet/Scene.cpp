#include "StdAfx.h"
#include "Sphere.h"


Scene::Scene(void)
{
}


Scene::~Scene(void)
{
}

// load the named file and dump its structure to STDOUT
void Scene::chargerScene(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	Objet* obj_tmp;
	Lumiere* lum_tmp;

	int cpt_obj=0;
	int cpt_lum=0;
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hObj(0);

		//Comptage du nombre d'objet
		pElem=hDoc.FirstChildElement("Sphere").Element();
		while (pElem){
			cpt_obj++;
			pElem=pElem->NextSiblingElement("Sphere");
		}
		pElem=hDoc.FirstChildElement("Plan").Element();
		while (pElem){
			cpt_obj++;
			pElem=pElem->NextSiblingElement("Plan");
		}
		pElem=hDoc.FirstChildElement("Tetraedre").Element();
		while (pElem){
			cpt_obj++;
			pElem=pElem->NextSiblingElement("Tetraedre");
		}
		cout << "Nombre d'objets dans le fichier : "<<cpt_obj<<endl;
		tab_obj = new Objet*[cpt_obj];
		nb_obj=cpt_obj;
		cpt_obj=0;

		//Comptage du nombre de lumière
		pElem=hDoc.FirstChildElement("Lumiere").Element();
		while (pElem){
			cpt_lum++;
			pElem=pElem->NextSiblingElement("Lumiere");
		}
		tab_lum = new Lumiere*[cpt_lum];
		nb_lum=cpt_lum;
		cpt_lum=0;


		//Sphere
		pElem=hDoc.FirstChildElement("Sphere").Element();
		while (pElem){			
			obj_tmp= new Sphere();
			hObj=TiXmlHandle(pElem);
			obj_tmp->initFromXML(hObj);
			tab_obj[cpt_obj]=obj_tmp;
			pElem=pElem->NextSiblingElement("Sphere");
			cpt_obj++;			
		}

		//Plan
		pElem=hDoc.FirstChildElement("Plan").Element();
		while (pElem){
			obj_tmp= new Plan();
			hObj=TiXmlHandle(pElem);
			obj_tmp->initFromXML(hObj);
			tab_obj[cpt_obj]=obj_tmp;
			pElem=pElem->NextSiblingElement("Plan");
			cpt_obj++;
			
		}
		//Tetraedre
		pElem=hDoc.FirstChildElement("Tetraedre").Element();
		while (pElem){
			obj_tmp= new Tetraedre();
			hObj=TiXmlHandle(pElem);
			obj_tmp->initFromXML(hObj);
			tab_obj[cpt_obj]=obj_tmp;
			pElem=pElem->NextSiblingElement("Tetraedre");
			cpt_obj++;
		}

		if(nb_obj=cpt_obj){
			cout<<"Tous les objets ont été chargés"<<endl;
		}
		//Camera
		pElem=hDoc.FirstChildElement("Camera").Element();
		if (pElem) {
			camera = new Camera();
			hObj=TiXmlHandle(pElem);
			camera->initFromXML(hObj);
		}else{
			cerr << "Camera non définie" << endl;
		}
		//Lumieres
		pElem=hDoc.FirstChildElement("Lumiere").Element();
		while (pElem){
			if (strcmp(pElem->Attribute("type"),"point")==0){
				lum_tmp= new PointLumiere();
			}else{
				//lum_tmp= new DirectionelleLumiere();
			}
			hObj=TiXmlHandle(pElem);
			lum_tmp->initFromXML(hObj);
			tab_lum[cpt_lum]=lum_tmp;
			pElem=pElem->NextSiblingElement("Lumiere");
			cpt_lum++;
			
		}

	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}

void Scene::afficherScene(){
	int i=0;
	for (i=0;i<nb_obj;i++){
		tab_obj[i]->afficher();
	}
	camera->afficher();

	for (i=0;i<nb_lum;i++){
		tab_lum[i]->afficher();
	}
	cout<<endl<<endl;
}

	bool Scene::intersect(Rayon& r, Intersection& I)
	{
		int nbObjet = this->getNbObjets();
		Objet** tab = this->getObjets();
		Intersection tmp;
		bool isIntersect = false;  //par défaut pas d'intersection
		double d_min=DBL_MAX; 

		//on parcourt tous les objets
		for(int i=0;i<nbObjet;i++){
			//si le rayon intersecte l'objet
			//et si la distance de l'objet en cours est inférieure à celle de l'objet le plus proche
			tmp=tab[i]->intersect(&r);
			if(tmp.getDistance()<d_min){
				isIntersect = true;
				I=tmp;
				d_min=I.getDistance();
			}
		}	
		return isIntersect;
	}
