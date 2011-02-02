#include "StdAfx.h"
#include "Scene.h"

using namespace std;

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
	
	int cpt_obj=0;
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
		cout << "Nombre d'objets dans le fichier : "<<cpt_obj<<endl;
		tab_obj = new Objet*[cpt_obj];
		nb_obj=cpt_obj;
		cpt_obj=0;


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
		if(nb_obj=cpt_obj){
			cout<<"Tous les objets ont �t� charg�s"<<endl;
		}
		//Camera
		pElem=hDoc.FirstChildElement("Camera").Element();
		if (pElem) {
			camera = new Camera();
			hObj=TiXmlHandle(pElem);
			camera->initFromXML(hObj);
		}else{
			cerr << "Camera non d�finie" << endl;
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
}