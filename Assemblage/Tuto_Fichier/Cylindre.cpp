/*******************************************************************
 * \file    Cylindre.cpp                                                *
 * \author  Arnaud ETCHEVERRY <arnaud.etcheverry@eisti.eu>         * 
 *          Abal-Kassim CHEIK AHAMED <abal.kassim.cheik@eisti.eu>  *
 * \version 1.0												       *
 * \date 11 janvier 2010										   *	
 * \brief														   *
 * \remarks														   *
 * \description													   *
 *******************************************************************/

#include "stdafx.h"
#include "Cylindre.h"


Cylindre::Cylindre(void)
{
	this->r = 0.0f;
	this->height = 0.0f;
}


 Cylindre::~Cylindre(void)
{
}

    Cylindre::Cylindre(double r, double height)
	{
		this->r = r;
		this->height = height;
	}


	void  Cylindre::setR(double r)
	{
		this->r = r;
	}

	void  Cylindre::setheight(double height)
	{
		this->height = height;
	}


	double  Cylindre::getR()
	{
		return this->r;
	}

	double Cylindre::getheight()
	{
		return this->height;
	}

	void Cylindre::afficher(){
		cout<<"Cylindre :"<<endl;
		cout <<"Rayon : "<<r<<endl;
		cout <<"Hauteur : "<<height<<endl;
		Objet::afficher();
	}

	Intersection Cylindre::intersect(Rayon* r)
{
	Intersection inter;
	double a;
	double b;
	double c;
	double delta;
	double t, t1, t2;

	/* equation du Cylindre y²+z²=rayon² avec 0<x<longueur=height
	 * (dy.t+py)²+(dz.t+pz)²=rayon²
	 * (dy²+dz²)t²+(dy.py+dz.pz)t+(py²+pz²-rayon²)=0
	 * at²+bt+c=0
	 */
	 
	// inter.setObjet(this);
  
	 a=pow(r->getDirection().y,2)+pow(r->getDirection().z,2);
	 b=(r->getDirection().y*r->getPosition().y)+(r->getDirection().z*r->getPosition().z);
	 c=pow(r->getPosition().y,2)+pow(r->getPosition().z,2)-pow(this->r,2);
	 //calcul du discriminant
	 delta = pow(b,2)-4*a*c;
	 
  if (delta <= 0) {
	  inter.setDistance(DBL_MAX); //distance infinie (pas d'intersection)
	//si py²+pz²-rayon²<=0 ou c<0
	//on se situe sur l'alignement du cylindre
	  if(c<0){
		  if( (this->height!=0) 
			  && (r->getPosition().x>this->height 
			  || (r->getPosition().x>=0.0 && r->getPosition().x<this->height))){

				  if(r->getDirection().x!=0.0){
					  t = (this->height-r->getPosition().x)/r->getDirection().x;
					  inter.setPoint( vector3(this->height,
						  t * r->getDirection().y + r->getPosition().y,
						  t * r->getDirection().z + r->getPosition().z) );
					  if(t>EPSILON && c<=0){
						  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
					  }else{
						  inter.setDistance(DBL_MAX);
					  }
				  }else{
					  inter.setDistance ( DBL_MAX );
				  }
		  }
			
		  if( (this->height!=0) 
			  && (r->getPosition().x<this->height 
			  || (r->getPosition().x>0.0 && r->getPosition().x<=this->height))){
				  if(r->getDirection().x!=0.0){
					  t = (-r->getPosition().x)/r->getDirection().x;
					  inter.setPoint(vector3(0.0,
						  t * r->getDirection().y + r->getPosition().y,
						  t * r->getDirection().z + r->getPosition().z) );
					  if(t>EPSILON && c<=0){
						  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
					  }else{
						  inter.setDistance(DBL_MAX);
					  }
				  }else{
					  inter.setDistance(DBL_MAX);
				  }
		  }
	  }

     }else{ //deux solutions

      t1 = (-b + sqrt (delta)) / 2*a;
      t2 = (-b - sqrt (delta)) / 2*a;

      if (t1 <= EPSILON && t2 <= EPSILON){
	    inter.setDistance(DBL_MAX);
	  }else{

	  if ((t1 <= t2 && t1 > EPSILON) || (t2 < t1 && t2 < EPSILON)){
	    t = t1;
	  }else if ((t2 < t1 && t2 > EPSILON) || (t1 < t2 && t1 < EPSILON)){
	    t = t2;
	  }

	  inter.setPoint(vector3(t * r->getDirection().x + r->getPosition().x,
							 t * r->getDirection().x + r->getPosition().x,
						     t * r->getDirection().z + r->getPosition().z));

	  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );

	  if( (this->height!=0) && (inter.getPoint().x>this->height)
			  || (r->getPosition().x>this->height && c<=0)){
				  if(r->getDirection().x!=0.0){
					  t = (this->height-r->getPosition().x)/r->getDirection().x;
					  inter.setPoint(vector3(this->height,
						  t * r->getDirection().y + r->getPosition().y,
						  t * r->getDirection().z + r->getPosition().z) );
					  if(t>EPSILON && c<=0){
						  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
					  }else{
						  inter.setDistance(DBL_MAX);
					  }
				  }else{
					 inter.setDistance(DBL_MAX);
				  }
		  }
			
		  if( (this->height!=0) 
			  && (inter.getPoint().x<0.0) 
			  || (r->getPosition().x<0.0 && c<=0)){
				  if(r->getDirection().x!=0.0){
					  t = (-r->getPosition().x)/r->getDirection().x;
					  inter.setPoint(vector3(0.0,
						  t * r->getDirection().y + r->getPosition().y,
						  t * r->getDirection().z + r->getPosition().z)  );
					  if(t>EPSILON){
						  inter.setDistance ( (inter.getPoint()-r->getPosition()).Length() );
					  }else{
						  inter.setDistance(DBL_MAX);
					  }
				  }else{
					  inter.setDistance(DBL_MAX);
				  }
		  }
	  }
  }
		
	  	 //TODO: la normale 
	    return (inter);
  }