// Tuto_Fichier.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Scene.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	Scene maScene = Scene();
	maScene.chargerScene("SpherePlan.xml");
	maScene.afficherScene();
	return 0;
}

