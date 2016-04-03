#include "WindowsPart.h"
#include "SDLPart.h"
#include "EventManager.h"
#include <thread>


void SDLThread(CSDLPart* sdlPart, boolean* window_alive);

using namespace std;



int main(int argc, char *argv[])
{	
	HINSTANCE hinstance = GetModuleHandle(0);
	CWindowsPart windowsPart(hinstance);
	g_pFramework->Init(windowsPart.Get_SDL_area());
	//CSDLPart sdlPart();
	//g_pEventManager->InjectEventUser(&sdlPart);
	CSDLPart* sdlPart = new CSDLPart();
	g_pEventManager->InjectEventUser(sdlPart);

	boolean window_alive = true;
	thread sdl_Thread (SDLThread, sdlPart, &window_alive);

	while (window_alive)
	{
		window_alive = windowsPart.HandleMessages();
	}
	// Ende der Schleife: Fenster wurde geschlossen. Stoppe SDL-Thread:
	window_alive = false;
	
	//g_pFramework->Quit();	
	sdl_Thread.join();
	delete sdlPart;
	return 0;
}

void SDLThread(CSDLPart* sdlPart, boolean* window_alive)
{	
	while (*window_alive)
	{
		sdlPart->HandleSDL();
	}
}