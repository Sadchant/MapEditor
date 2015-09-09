#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <iostream>
#include <windows.h>
#include "Singleton.h"
#include "Timer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using namespace std;

#define g_pFramework CFramework::Get()

struct Point 
{
	int x; int y;
};

class CFramework : public TSingleton<CFramework>
{
public:
	bool  Init(HWND area);
	bool  Init_Video(HWND area);
	void  Quit();
	void  Update();
	void  Render();
	bool  KeyDown(Uint8 Key_ID);
	Point getMousePosition();
	SDL_Renderer*	GetRenderer() { return sdl_Renderer; }
	SDL_Window*	GetWindow() { return sdl_Window; }

private:
	SDL_Window*		sdl_Window;						// Fenster, in dem alles stattfindet		
	SDL_Renderer*	sdl_Renderer;					// Renderer, der alles rendert
	SDL_Event		event;
	const Uint8*	pKeystate;						// Array für aktuellen Tastaturstatus
	Point mousePosition;
};

#endif
