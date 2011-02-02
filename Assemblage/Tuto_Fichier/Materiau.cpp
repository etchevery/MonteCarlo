#include "StdAfx.h"
#include "Materiau.h"


Materiau::Materiau() :
	m_Color( Color( 0.2f, 0.2f, 0.2f ) ),
	m_Refl( 0 ), m_Diff( 0.2f )
{
}

void Materiau::initFromXML(TiXmlHandle hObj){
	TiXmlElement* pElem=hObj.FirstChildElement("Materiau").Element();
	double r,g,b,diff,refl;
	cout << "Init materiau " <<pElem->Value()<<endl;
	pElem->QueryDoubleAttribute("red", &r);
	pElem->QueryDoubleAttribute("green", &g);
	pElem->QueryDoubleAttribute("blue", &b);
	
	
	this->SetColor(vector3(r,g,b));

	pElem->QueryDoubleAttribute("Diff", &diff);
	this->SetDiffuse(diff);
	pElem->QueryDoubleAttribute("Refl", &refl);
	this->SetReflection(refl);

}

void Materiau::afficher(){
	cout << "De couleur : "<< m_Color.x<<", "<<m_Color.y<<", "<<m_Color.z<<endl;
	cout << "De diffusion : "<<m_Diff<<" et de réflection : "<<m_Refl<<endl<<endl;
}