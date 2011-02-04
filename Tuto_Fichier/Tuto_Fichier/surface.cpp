// -----------------------------------------------------------
// surface.cpp
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#include "common.h"
#include "surface.h"
#include "stdio.h"
#include "string.h"


// -----------------------------------------------------------
// Hicolor surface class implementation
// -----------------------------------------------------------

Surface::Surface( int a_Width, int a_Height ) :
	m_Width( a_Width ),
	m_Height( a_Height )
{
	m_Buffer = new Pixel[a_Width * a_Height];
}

Surface::~Surface()
{
	delete [] m_Buffer;
}

void Surface::Clear( Pixel a_Color )
{
	int s = m_Width * m_Height;
	for ( int i = 0; i < s; i++ ) m_Buffer[i] = a_Color;
}
