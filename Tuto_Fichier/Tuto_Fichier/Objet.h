#ifndef OBJET_H
#define OBJET_H

#include "common.h"
#include "Materiau.h"
class Objet{
public :
	virtual ~Objet(){};
	
	virtual void initFromXML(TiXmlHandle hObj);

	virtual void afficher(){m_mat.afficher();};
private :
	Materiau m_mat;
};

#endif //OBJET_H