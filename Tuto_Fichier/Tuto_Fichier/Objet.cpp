#include "stdafx.h"
#include "Objet.h"

using namespace std;

void Objet::initFromXML(TiXmlHandle hObj){
	m_mat.initFromXML(hObj);
}


