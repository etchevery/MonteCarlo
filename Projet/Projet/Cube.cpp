/*******************************************************************
 * \file    Cube.cpp                                                *
 * \author  Arnaud ETCHEVERRY <arnaud.etcheverry@eisti.eu>         * 
 *          Abal-Kassim CHEIK AHAMED <abal.kassim.cheik@eisti.eu>  *
 * \version 1.0												       *
 * \date 11 janvier 2010										   *	
 * \brief														   *
 * \remarks														   *
 * \description													   *
 *******************************************************************/

#include "stdafx.h"
#include "Cube.h"



	

Cube::Cube(void)
{
	for(int i=0;i<8;i++){
		this->Sommets[i]=vector3();
	}
}


    Cube::Cube(vector3 S[6])
	{
		for(int i=0;i<8;i++){
			this->Sommets[i]=S[i];
		}
	}

	Cube::~Cube()
	{
	}
	
	vector3* Cube::getSommets()
	{
		return this->Sommets;
	}

	void afficher();
	Intersection intersect (Rayon* r);
    vector3 normale(vector3 p);

	void Cube::afficher(){
		cout<<"Cube :"<<endl;
		for(int i=0;i<8;i++){
			this->Sommets[i].afficher();
		}
		Objet::afficher();
	}


	//cube est une composition de 12 triangles
	Intersection Cube::intersect(Rayon* r)
{
    Intersection inter, inter_tmp, I[12];

    //Liste des faces: ligne=numero face, colonne=numero sommet
	int FacesTriangle[12][3];

	//Face Carré 1
	FacesTriangle[0][0]=1;FacesTriangle[0][1]=2;FacesTriangle[0][2]=3;
	FacesTriangle[1][0]=1;FacesTriangle[1][1]=4;FacesTriangle[1][2]=3;
	//Face Carré 2
	FacesTriangle[2][0]=6;FacesTriangle[2][1]=5;FacesTriangle[2][2]=8;
	FacesTriangle[3][0]=6;FacesTriangle[3][1]=7;FacesTriangle[3][2]=8;
	//Face Carré 3
	FacesTriangle[4][0]=3;FacesTriangle[4][1]=2;FacesTriangle[4][2]=6;
	FacesTriangle[5][0]=3;FacesTriangle[5][1]=7;FacesTriangle[5][2]=6;
	//Face Carré 4
	FacesTriangle[6][0]=5;FacesTriangle[6][1]=1;FacesTriangle[6][2]=4;
	FacesTriangle[7][0]=5;FacesTriangle[7][1]=8;FacesTriangle[7][2]=4;
	//Face Carré 5
	FacesTriangle[8][0]=4;FacesTriangle[8][1]=3;FacesTriangle[8][2]=7;
	FacesTriangle[9][0]=4;FacesTriangle[9][1]=8;FacesTriangle[9][2]=7;
	//Face Carré 6
	FacesTriangle[10][0]=1;FacesTriangle[10][1]=2;FacesTriangle[10][2]=6;
	FacesTriangle[11][0]=1;FacesTriangle[11][1]=5;FacesTriangle[11][2]=6;

	vector3 A=this->Sommets[FacesTriangle[0][0]];
	vector3 B=this->Sommets[FacesTriangle[0][1]];
	vector3 C=this->Sommets[FacesTriangle[0][2]];

	I[0]=Intersection(this,r,A,B,C);
	double d_min=I[0].getDistance(); 
	inter=I[0];

	for(int i=1;i<12;i++){
		A=this->Sommets[FacesTriangle[i][0]];
		B=this->Sommets[FacesTriangle[i][1]];
		C=this->Sommets[FacesTriangle[i][2]];
		I[i]=Intersection(this,r,A,B,C);
		if(I[i].getDistance()<DBL_MAX 
			&& I[i].getDistance()<d_min)
				{
					inter=I[i];
					d_min=I[i].getDistance();
				}
	}

	inter.setObjet(this);
	return(inter);
  }
