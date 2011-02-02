#pragma once
#include "common.h"
class Materiau{
public:
	Materiau();
	void SetColor( Color& a_Color ) { m_Color = a_Color; }
	Color GetColor() { return m_Color; }
	void SetDiffuse( double a_Diff ) { m_Diff = a_Diff; }
	void SetReflection( double a_Refl ) { m_Refl = a_Refl; }
	double GetSpecular() { return 1.0f - m_Diff; }
	double GetDiffuse() { return m_Diff; }
	double GetReflection() { return m_Refl; }
	
	void afficher();
private:
	Color m_Color;
	double m_Refl;
	double m_Diff;
};


