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
    Intersection inter, inter_tmp, inter_face, inter_Sphere;
	vector3 A,B,C;
    //Liste des faces: ligne=numero face, colonne=numero sommet
	int FacesTriangle[12][3];
	Sphere S(this->Sommets,4); //sphere englobant le cube 
	inter_Sphere=S.intersect(r);

	if(inter_Sphere.getDistance()<DBL_MAX){
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
		

			double d_min=DBL_MAX; 

			//on parcourt les 6 faces du cube 
			for(int j=0;j<12;j+2){
				A=this->Sommets[FacesTriangle[j][0]];
				B=this->Sommets[FacesTriangle[j][1]];
				C=this->Sommets[FacesTriangle[j][2]];
				inter_face=Intersection(this,r,A,B,C);
				//s'il n y a pas d'intersection sur un triangle de la meme face, on regarde l'autre face
				if(!(inter_face.getDistance()<DBL_MAX)){
					A=this->Sommets[FacesTriangle[j+1][0]];
					B=this->Sommets[FacesTriangle[j+1][1]];
					C=this->Sommets[FacesTriangle[j+1][2]];
					inter_face=Intersection(this,r,A,B,C);
				}
				inter_tmp=inter_face;
				if(inter_tmp.getDistance()<d_min){
					inter=inter_tmp;
					d_min=inter.getDistance();
				}
			}
	}

	inter.setObjet(this);
	return(inter);
  }
