#include "stdafx.h"
#include "Plan.h"

Plan::Plan(){	
	this->u=vector3();
	this->v=vector3();
	this->w=vector3();
}


Plan::Plan(vector3 a,vector3 b,vector3 c,vector3 d){
	
	this->u=a;
	this->v=b;
	this->w=c;
	this->x=d;
}

Plan::~Plan(){
}

bool Plan::operator ==( Plan& P){
return(this->u==P.u && this->v==P.v && this->w==P.w);
}

void Plan::initFromXML(TiXmlHandle hObj){
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double x4, y4, z4;

	Objet::initFromXML(hObj);
	TiXmlElement* pElem=hObj.Element();

	pElem->QueryDoubleAttribute("x1", &x1);
	pElem->QueryDoubleAttribute("y1", &y1);
	pElem->QueryDoubleAttribute("z1", &z1);
	pElem->QueryDoubleAttribute("x2", &x2);
	pElem->QueryDoubleAttribute("y2", &y2);
	pElem->QueryDoubleAttribute("z2", &z2);
	pElem->QueryDoubleAttribute("x3", &x3);
	pElem->QueryDoubleAttribute("y3", &y3);
	pElem->QueryDoubleAttribute("z3", &z3);
	pElem->QueryDoubleAttribute("x4", &x4);
	pElem->QueryDoubleAttribute("y4", &y4);
	pElem->QueryDoubleAttribute("z4", &z4);

	this->setU(vector3(x1,y1,z1));
	this->setV(vector3(x2,y2,z2));
	this->setW(vector3(x3,y3,z3));
	this->setX(vector3(x4,y4,z4));
}


void Plan::afficher(){

	cout<<"Plan :"<<endl;
	this->u.afficher();
	this->v.afficher();
	this->w.afficher();
	this->x.afficher();
	Objet::afficher();
}


Intersection Plan::intersect2 (Rayon* r)
  {

    Intersection inter;
	double t, p, q;
	inter.setObjet(this);
  
	/* Pour un plan, ax+by+cz+d=0  où  (a,b,c) vecteur normal
	 * Donc  ....
	 */
	double dx=r->getDirection().x; double px=r->getPosition().x;
	double dy=r->getDirection().y; double py=r->getPosition().y;
	double dz=r->getDirection().z; double pz=r->getPosition().z;

	vector3 D(dx,dy,dz); //vecteur directionnel du rayon (normalisé)
	vector3 N=this->normale(); //vecteur normal au plan  
	N.Normalize(); //normalisation du vecteur
	/*(P): ax+by+cz+d=0 :  vecteur u appartient à (P) 
	 * d=-(a*u.x+b*u.y+c*u.z) (par exemple)
	 */ 
	double d=-(N.x*this->u.x+N.y*this->u.y+N.z*this->u.z);
	

	/*si le produit saclaire D.N = 0
	 * alors  Rayon parallèle au plan P
	 * >>> Si O(px,py,pz) appartient au plan alors le rayon est dans le plan
	 * >>> sinon le rayon est parallèle à P sans intersection */

	if(D.Dot(N)==0) 
	{
		//si O appartient à (P)
		if(N.x*px+N.y*py+N.z*pz+d==0)
		{
			inter.setDistance(DBL_MAX); //pas d'intersection  //rayon est dans le plan
		}else{
			inter.setDistance(DBL_MAX); //pas d'intersection
		}
	}else{
		/*il y a une unique intersection I d'équation paramétrique: 
		 *unique t tel que  I_x=px+t*dx, I_=py+t*dy, I_z=pz+t*dz 
		 * t vérifie alors  t(a*dx+b*dy+c*dz)=-d-(a*px+b*py+c*pz)
		 */
			p=N.x*dx+N.y*dy+N.z*dz;
			q=-d-(N.x*px+N.y*py+N.z*pz);
			t=q/p;   //TODO: vérifier si p peut-être null

			if(t>EPSILON){
				inter.setPoint(vector3(t * r->getDirection().x + r->getPosition().z,
									t * r->getDirection().y + r->getPosition().y,
									t * r->getDirection().z + r->getPosition().z));
				inter.setDistance( (inter.getPoint()-r->getPosition()).Length() );
				inter.setNormal(N);
			}else{
			inter.setDistance(DBL_MAX);
			}
	}


	return(inter);
}

Intersection Plan::intersect (Rayon* r)
  {

    Intersection inter, inter_tmp, inter_Sphere;

	vector3 Sommets[4]={this->u,this->v,this->w,this->x};
	Sphere S(Sommets,4); //sphere englobant du plan
     
	inter_Sphere=S.intersect(r);
	if(inter_Sphere.getDistance()<DBL_MAX){
		inter_tmp=Intersection(this,r, this->u,this->v,this->w);
		if(inter_tmp.getDistance()<DBL_MAX){
			inter=inter_tmp;
		}else{
			inter=Intersection(this,r, this->u,this->w,this->x);
		}
	}
	inter.setObjet(this);
	return(inter);
}


	vector3 Plan::normale()
	{
		return (this->v-this->u).vector_product(this->u-this->w);
	}
