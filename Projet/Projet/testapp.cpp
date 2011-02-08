#include "stdafx.h"
#include "Materiau.h"
#include "raytracer.h"
#include "surface.h"
#include "Config.h"
#include <Windows.h>
#include <time.h>
#include "timing.h"

//definition des clocks 
TIMING::CpuTime clock_intersect_plan("intersect_plan", -1, 5);
TIMING::CpuTime clock_intersect_sphere("intersect_sphere", -1, 5);
TIMING::CpuTime clock_intersect_cube("intersect_cube", -1, 5);
TIMING::CpuTime clock_intersect_triangle("intersect_triangle", -1, 5);
TIMING::CpuTime clock_intersect_cylindre("intersect_cylindre", -1, 5);
TIMING::CpuTime clock_intersect_tetraedre("intersect_tetraedre", -1, 5);
TIMING::CpuTime clock_intersect_program("intersect_program", -1, 5);
TIMING::CpuTime clock_scene("scene", -1, 5);
TIMING::CpuTime clock_render("render", -1, 5);



#define SCRWIDTH	400
#define SCRHEIGHT	300

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
	config.indirectLighting=true;
	config.echantillonType=IMPORTANCE;
	config.russianRoulette=false;
	config.profondeur=1;
	config.nbLancerParPixel=5;
	config.filename="Salle.xml";
}

void SaveImage(const char* NomFichier,Couleur* img){
	FILE *out=fopen(NomFichier,"w" ); 
	int i; 
	fprintf(out,"P3\n");
	fprintf(out,"%d",SCRWIDTH);
	fprintf(out," ");
	fprintf(out,"%d",SCRHEIGHT);
	fprintf(out,"\n255\n" ); 
	
	for(i=0;i<SCRHEIGHT*SCRWIDTH;i++) {
		fprintf(out,"%d %d %d ",int(img[i].r),int(img[i].g),int(img[i].b)); 	
	}
	fclose(out); 
}

void SaveLogFile(const char* NomFichier,Configuration config){
	FILE *out =fopen(NomFichier,"w" ); 
	
	fprintf(out,"Stockage des resultats\n\n");
	fprintf(out,"-Type d'échantillon : ");
	if(config.echantillonType==0){
		 fprintf(out,"UNIFORME\n");
	}else{
		fprintf(out,"IMPORTANCE\n");
	}
	
	fprintf(out,"-Activation de lumière directe : %d\n",config.directLighting);
	fprintf(out,"-Activation de lumière indirecte : %d\n",config.indirectLighting);
	fprintf(out,"-Activation de la méthode de la roulette russe : %d\n",config.russianRoulette);
	fprintf(out,"-Fichier testé : %s\n",config.filename);
	fprintf(out,"-Nombre de lancers de rayons pour chaque pixel : %d\n",config.nbLancerParPixel);
	fprintf(out,"-Profondeur : %d\n",config.profondeur);
	fprintf(out,"-Temps d'exécution: \n");
	//on joute les times dans le fichiers
	clock_intersect_plan.printToFile(out);
	clock_intersect_sphere.printToFile(out);
	clock_intersect_cube.printToFile(out);
	clock_intersect_triangle.printToFile(out);
	clock_intersect_cylindre.printToFile(out);
	clock_intersect_tetraedre.printToFile(out);
	clock_intersect_program.printToFile(out);
	clock_scene.printToFile(out);
	clock_render.printToFile(out);
	fclose(out); 
}

void afficherClock(){
	clock_intersect_plan.print();
	clock_intersect_sphere.print();
	clock_intersect_cube.print();
	clock_intersect_triangle.print();
	clock_intersect_cylindre.print();
	clock_intersect_tetraedre.print();
	clock_intersect_program.print();
	clock_scene.print();
	clock_render.print();
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


	clock_scene.begin(); // ---> start clock_scene

	// prepare renderer
	Scene* maScene = new Scene();
	maScene->chargerScene(config.filename);
	maScene->afficherScene();

	tracer = new Engine(config);
	tracer->SetScene(maScene);
	tracer->SetTarget(surface->GetBuffer(),SCRWIDTH, SCRHEIGHT );
	int tpos = 60;

	clock_scene.end(); // ---> end clock_scene		

	system("Pause");

	
		tracer->InitRender();
	clock_render.begin(); // ---> start clock_render
		tracer->Render(); //calcul de l'image
	clock_render.end(); // ---> end clock_render

	SaveImage("test.ppm",tracer->GetImage());
	
	SaveLogFile("Resultat.log",config);
	afficherClock();

	while (1)
	{
		DrawWindow();
	}

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
