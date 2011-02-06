#include "stdafx.h"
#include "Materiau.h"
#include "raytracer.h"
#include "surface.h"
#include "Config.h"

#define SCRWIDTH	400
#define SCRHEIGHT	200

static WNDCLASS wc;
static HWND wnd;
static char bitmapbuffer[sizeof( BITMAPINFO ) + 16];
static BITMAPINFO* bh;

HDC window_hdc;
Pixel* buffer = 0;
Engine* tracer = 0;
Configuration config;
Surface* surface=0;

//mettre tous les paramèters
void initConf(Configuration& config){
	config.directLighting=true;
	config.echantillonType=IMPORTANCE;
	config.indirectLighting=true;
	config.russianRoulette=true;
	config.profondeur=1;
	config.nbLancerParPixel=4;
	config.filename="Objet.xml";
}

void DrawWindow();

static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int result = 0, keycode = 0;
	switch (message)
	{
	case WM_PAINT:
		if (!buffer) break;
		StretchDIBits( window_hdc, 0, 0, SCRWIDTH, SCRHEIGHT, 0, 0, SCRWIDTH, SCRHEIGHT, buffer, bh, DIB_RGB_COLORS, SRCCOPY );
		ValidateRect( wnd, NULL );
		break;
	case WM_KEYDOWN:
		if ((wParam & 0xFF) != 27) break;
	case WM_CLOSE:
		ReleaseDC( wnd, window_hdc );
		DestroyWindow( wnd );
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, 1, 0, 0 );
		ExitProcess( 0 );
		break;
	default:
		result = DefWindowProc(hWnd,message,wParam,lParam);
	}
	return result;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	RECT rect;
	int cc;
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "raytracer";
	if (!RegisterClass(&wc)) return FALSE;
	rect.left = rect.top = 0;
	rect.right = SCRWIDTH, rect.bottom = SCRHEIGHT;
	AdjustWindowRect( &rect, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0 );
	rect.right -= rect.left, rect.bottom -= rect.top;
	wnd = CreateWindowEx( 0, "raytracer", "raytracer", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, 0, 0, 0, 0 );
	ShowWindow(wnd,SW_NORMAL);
	for ( cc = 0; cc < sizeof( BITMAPINFOHEADER ) + 16; cc++ ) bitmapbuffer[cc] = 0;
	bh = (BITMAPINFO *)&bitmapbuffer;
	bh->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bh->bmiHeader.biPlanes = 1;
	bh->bmiHeader.biBitCount = 32;
	bh->bmiHeader.biCompression = BI_BITFIELDS;
	bh->bmiHeader.biWidth = SCRWIDTH, bh->bmiHeader.biHeight = -SCRHEIGHT;
	((unsigned long*)bh->bmiColors)[0] = 255 << 16;
	((unsigned long*)bh->bmiColors)[1] = 255 << 8;
	((unsigned long*)bh->bmiColors)[2] = 255;
	window_hdc = GetDC(wnd);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);
	initConf(config);
	// prepare output canvas
	surface = new Surface(SCRWIDTH,SCRHEIGHT);
	buffer = surface->GetBuffer();
	surface->Clear(0);
	AllocConsole();   //Sortie Console
	freopen("CONOUT$","wb",stdout);


	// prepare renderer
	Scene* maScene = new Scene();
	maScene->chargerScene(config.filename);
	maScene->afficherScene();

	tracer = new Engine(config);
	tracer->SetScene(maScene);
	tracer->SetTarget(surface->GetBuffer(),SCRWIDTH, SCRHEIGHT );
	int tpos = 60;
	maScene->afficherScene();
	system("Pause");
	// go
	tracer->InitRender();
	tracer->Render();

	while (1)
	{
		DrawWindow();
	}
	
	system("PAUSE");
FreeConsole();  // Close the console window
	return 1;
}

void DrawWindow()
{
	MSG message;
	HACCEL haccel = 0;
	InvalidateRect( wnd,NULL,TRUE );
	SendMessage( wnd, WM_PAINT, 0, 0 );
	while (PeekMessage( &message, wnd, 0, 0, PM_REMOVE ))
	{
		if (TranslateAccelerator( wnd, haccel, &message ) == 0)
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
	Sleep( 0 );
}
