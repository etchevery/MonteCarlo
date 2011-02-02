#include "StdAfx.h"
#include "Rayon.h"


Rayon::Rayon(void)
{
	x_position=0;
	y_position=0;
	z_position=0;
	x_direction=0;
	y_direction=0;
	z_direction=0;
}

Rayon::Rayon(double px, double py, double pz, double dx, double dy, double dz)
{
	x_position=px;
	y_position=py;
	z_position=pz;
	x_direction=dx;
	y_direction=dy;
	z_direction=dz;
}

Rayon::Rayon(vector3 point,vector3 dir){
	x_position=point.x;
	y_position=point.y;
	z_position=point.z;
	x_direction=dir.x;
	y_direction=dir.y;
	z_direction=dir.z;
}
Rayon::~Rayon(void)
{
}

void Rayon::setPosition(double x,double y,double z){
	x_position=x;
	y_position=y;
	z_position=z;
}
void Rayon::setDirection(double x,double y, double z){
	x_direction=x;
	y_direction=y;
	z_direction=z;
}

vector3 Rayon::getPosition(){ 
	return vector3(x_position,y_position,z_position);
}

vector3 Rayon::getDirection(){ 
	return vector3(x_direction,y_direction,z_direction);
}

void Rayon::initFromXML(TiXmlHandle hObj){
	double x,y,z;
	TiXmlElement* pElem=hObj.Element();
	
	pElem->QueryDoubleAttribute("x_pos", &x);
	pElem->QueryDoubleAttribute("y_pos", &y);
	pElem->QueryDoubleAttribute("z_pos", &z);
	
	if (z<0){
		this->setPosition(x,y,z);
	}else{
		cerr << "Mauvais positionnement de la caméra"<<endl;
	}
	
	pElem->QueryDoubleAttribute("x_look", &x);
	pElem->QueryDoubleAttribute("y_look", &y);
	pElem->QueryDoubleAttribute("z_look", &z);

	this->setDirection(x,y,z);

}

void Rayon::afficher(){
	cout<<"Rayon"<<endl;
	cout << "Position : "<<x_position<<", "<<y_position<<", "<<z_position<<endl;
	cout << "Direction : "<<x_direction<<", "<<y_direction<<", "<<z_direction<<endl<<endl;
}