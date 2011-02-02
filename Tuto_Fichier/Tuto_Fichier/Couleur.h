#ifndef COULEURH
#define COULEURH

#include "stdafx.h"
#include "common.h"


class Couleur : public vector3 { 
public:
	Couleur();
	Couleur(const double r, const double g, const double b);
	Couleur(const Couleur& c);

	void initFromXML(TiXmlHandle hObj);
	void afficher(){cout << "(" << x << "," << y << "," << z << ")" << endl;};
	Couleur& operator	= ( const Couleur& c );


	friend Couleur operator * (const Couleur& a, const Couleur& b);	
	friend Couleur operator - (const Couleur& c);
	friend Couleur operator + (const Couleur& a, const Couleur& b);
	friend Couleur operator - (const Couleur& a, const Couleur& b);
	friend Couleur operator * (const Couleur& a, const double d);
	friend Couleur operator * (const double d, const Couleur& a);
	friend Couleur operator / (const Couleur& a, const double d);
	friend Couleur operator / (const Couleur& a, const Couleur& b);	
	friend int operator == (const Couleur& a, const Couleur& b);		
	friend int operator != (const Couleur& a, const Couleur& b);			

	Couleur& clamp();//Chaque composante<1
	Couleur& normalize();
	void ToRgb() { r=int(255.0*r), g=int(255.0*g); b=int(255.0*b); };
	static const Couleur black;
};

/* Constructors */

inline Couleur::Couleur() {
	r = g = b = 0.0;
}

inline Couleur::Couleur(const double x, const double y, const double z)
: vector3(x, y, z) {}

inline Couleur::Couleur(const Couleur& v)
: vector3(v) {}


/* Operators */

inline Couleur& Couleur::operator = (const Couleur& v)
{ r = v.r; g = v.g; b = v.b; return *this; }


/* Friends */

inline Couleur operator * (const Couleur& a, const Couleur& b) {
	return Couleur(a.r*b.r, a.g*b.g, a.b*b.b);
}

inline Couleur operator - (const Couleur& a)
{  return Couleur(-a.r,-a.g,-a.b); }

inline Couleur operator + (const Couleur& a, const Couleur& b)
{ return Couleur(a.r+ b.r, a.g + b.g, a.b + b.b); }

inline Couleur operator - (const Couleur& a, const Couleur& b)
{ return Couleur(a.r-b.r, a.g-b.g, a.b-b.b); }

inline Couleur operator * (const Couleur& a, const double d)
{ return Couleur(d*a.r, d*a.g, d*a.b); }

inline Couleur operator * (const double d, const Couleur& a)
{ return a*d; }

inline Couleur operator / (const Couleur& a, const double d)
{ double d_inv = 1.0/d; return Couleur(a.r*d_inv, a.g*d_inv,
  a.b*d_inv); }

inline Couleur operator / (const Couleur& a, const Couleur& b)
{
	return Couleur(a.r / b.r, a.g / b.g, a.b / b.b);
}

inline int operator == (const Couleur& a, const Couleur& b)
{ return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

inline int operator != (const Couleur& a, const Couleur& b)
{ return !(a == b); }

#endif