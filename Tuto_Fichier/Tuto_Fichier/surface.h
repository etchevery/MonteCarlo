// -----------------------------------------------------------
// surface.h
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#ifndef SURFACE_H
#define SURFACE_H

#include "string.h"
#include "common.h"


class Surface
{
	enum
	{
		OWNER = 1
	};

public:
	// constructor / destructors
	Surface( int a_Width, int a_Height );
	Surface( char* a_File );
	~Surface();

	// member data access
    Pixel* GetBuffer() { return m_Buffer; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }

	// Special operations
	void Clear( Pixel a_Color );

private:
	// Attributes
	Pixel* m_Buffer;	
	int m_Width, m_Height;		
};


#endif