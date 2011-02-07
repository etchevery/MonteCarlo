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
#include "Polygone.h"



	Polygone::Polygone(){
		this->nbFaces=4;
		for(int i=0;i<this->nbFaces;i++){
			for (int j=0;j<3;j++){
				this->Faces[i][j]=vector3();
			}
		}
	}

	Polygone::Polygone(int n){
		this->nbFaces=n;
		for(int i=0;i<this->nbFaces;i++){
			for (int j=0;j<3;j++){
				this->Faces[i][j]=vector3();
			}
		}
	}

    Polygone::Polygone(vector3* S[3], int n){
		this->nbFaces=n;
		for(int i=0;i<this->nbFaces;i++){
			for (int j=0;j<3;j++){
				this->Faces[i][j]=S[i][j];
			}
		}
	}

	Polygone::~Polygone(){
	}
	
	vector3** Polygone::getFaces(){
		return this->Faces;
	}

	vector3* Polygone::getFaces(int i){
		vector3 tmp[3];
		//if(i<0 || i>=this->nbFaces);

		for(int k=0;k<3;k++){
			tmp[k]=Faces[i][k];
		}

		return tmp;
	}

	int Polygone::getNbFaces(){
		return this->nbFaces;
	}

	void Polygone::setFaces(int i, vector3 c[3]){
		//if(i<0 || i>=this->nbFaces) return;
		for(int k=0;k<3;k++){
			this->Faces[i][k]=c[k];
		}
	}

		void Polygone::afficher(){
		cout<<"Polygone :"<<endl;
		for(int i=0;i<this->nbFaces;i++){
			cout << "Face n°" << i; 
			for(int k=0;k<3;k++){
				cout <<" : " <<  "(" << this->Faces[i][k].x << ";" << this->Faces[i][k].y << ";" << this->Faces[i][k].y <<  ")" <<endl; ;
			}
		}
		Objet::afficher();
	}


	Intersection Polygone::intersect (Rayon* r){
		Intersection inter;

		return inter;
	}