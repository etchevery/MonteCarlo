#include "StdAfx.h"
#include "Materiau.h"


Materiau::Materiau(void)
{
}



void Materiau::initFromXML(TiXmlHandle hObj){
	TiXmlElement* pElem=hObj.FirstChildElement("Materiau").Element();
	double phong,refr;
	cout << "Init materiau " <<pElem->Value()<<endl;
	
	const char* brdf = pElem->Attribute("BRDF");
	if (strcmp(brdf,"Emissive")==0){
		this->myReflectance.type=Emissive;
	}else if (strcmp(brdf,"Diffuse")==0){
		this->myReflectance.type=Diffuse;
	}else if (strcmp(brdf,"Glossy")==0){
		this->myReflectance.type=Glossy;
	}else if (strcmp(brdf,"Mirror")==0){
		this->myReflectance.type=Mirror;
	}else if (strcmp(brdf,"Dielectric")==0){
		this->myReflectance.type=Dielectric;
	}

	
	pElem->QueryDoubleAttribute("Phong", &phong);
	this->myReflectance.pExp=phong;

	pElem->QueryDoubleAttribute("Refr", &refr);
	this->myReflectance.indiceRefraction=refr;
	
	
	TiXmlHandle hCouleur = TiXmlHandle(pElem);
	pElem = hCouleur.FirstChildElement("kE").Element();
	if(pElem){
		this->myReflectance.kE.initFromXML(TiXmlHandle(pElem));
	}
	
	pElem = hCouleur.FirstChildElement("kD").Element();
	if(pElem){
		this->myReflectance.kD.initFromXML(TiXmlHandle(pElem));
	}
	pElem = hCouleur.FirstChildElement("kS").Element();
	if(pElem){
		this->myReflectance.kS.initFromXML(TiXmlHandle(pElem));
	}
	pElem = hCouleur.FirstChildElement("kR").Element();
	if(pElem){
		this->myReflectance.kR.initFromXML(TiXmlHandle(pElem));
	}
	pElem = hCouleur.FirstChildElement("kT").Element();
	if(pElem){
		this->myReflectance.kT.initFromXML(TiXmlHandle(pElem));
	}
}

void Materiau::afficher(){
	cout << "De couleur emissive: ";
	myReflectance.kE.afficher();
	cout << "De couleur diffuse: ";
	myReflectance.kD.afficher();
	cout << "De couleur speculaire: ";
	myReflectance.kS.afficher();
	cout << "De couleur mirroir: ";
	myReflectance.kR.afficher();
	cout << "De couleur refraction: ";
	myReflectance.kT.afficher();
	cout << "Indice de réfraction : "<<myReflectance.indiceRefraction<<" et de puissance de Phong : "<<myReflectance.pExp<<endl;
	cout<<"Type de BRDF :"<<myReflectance.type<<endl<<endl;
}