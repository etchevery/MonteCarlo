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
	Rayon Rp=Rayon(1.0,1.0,1.0,-1.0,1.0,1.0);
	Intersection I;
    Rayon R=Rayon(1.0,0.5,1.0,0.0,0.0,-1.0);


	cout << endl << ">>>>>>>>> intersection global: " << maScene.intersect(Rp, I) << endl << endl;
    I.afficher();

}

int _tmain(int argc, _TCHAR* argv[])
{
	Scene maScene = Scene();
	maScene.chargerScene("SpherePlan.xml");
	maScene.afficherScene(); 
	cout << endl << "***************************************************************************" << endl << endl;
	test_intersecteur(maScene);

	return 0;
}

