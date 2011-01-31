// Tuto_Fichier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Scene.h"

using namespace std;

static void test_intersecteur()
{
Sphere S=Sphere(1.0,10.0,10.0,10.0);
S.afficher();

Rayon Rs=Rayon(0.0,0.0,0.0,1.0,1.0,1.0);
Rs.afficher();

Intersection Is;
Is=S.intersection(&Rs);
afficher(Is);

Rayon Rp=Rayon(1.0,1.0,1.0,-1.0,1.0,0.0);
Rp.afficher();

Plan P=Plan(1.0,0.0,0.0,5.0);
P.afficher();
Intersection Ip;
Ip=P.intersection(&Rp);
afficher(Ip);

}

int _tmain(int argc, _TCHAR* argv[])
{
	/*Scene maScene = Scene();
	maScene.chargerScene("SpherePlan.xml");
	maScene.afficherScene(); */

	test_intersecteur();

	return 0;
}

