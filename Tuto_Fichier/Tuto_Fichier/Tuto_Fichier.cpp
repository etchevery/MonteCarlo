// Tuto_Fichier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Scene.h"

using namespace std;

static void test_intersecteur(Scene maScene)
{
	int taille = maScene.getNbObjets();
	Objet** tab = maScene.getObjets();

Rayon Rs=Rayon(0.0,0.0,0.0,1.0,1.0,1.0);
//Rs.afficher();

Plan P=Plan(1.0,1.0,1.0,2.0);
Intersection Is;
Is=P.intersect(&Rs);
Is.afficher();
/*
Rayon Rp=Rayon(1.0,1.0,1.0,-1.0,1.0,0.0);
Rp.afficher();


Intersection Ip;
Ip=tab[1]->intersect(&Rp);
Ip.afficher();
*/

}

int _tmain(int argc, _TCHAR* argv[])
{
	Scene maScene = Scene();
	maScene.chargerScene("SpherePlan.xml");
	maScene.afficherScene(); 

	test_intersecteur(maScene);

	return 0;
}

