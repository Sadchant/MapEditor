#include "WindowsPart.h"
#include "SDLPart.h"
#include "EventManager.h"
#include <thread>

void initButtons();
void SDLThread(CSDLPart* windowsPart);

using namespace std;



int main(int argc, char *argv[])
{
	
	HINSTANCE hinstance = GetModuleHandle(0);
	CWindowsPart* windowsPart = new CWindowsPart(hinstance);
	g_pFramework->Init(windowsPart->Get_SDL_area());
	CSDLPart sdlPart = CSDLPart();
	g_pEventManager->InjectEventUser(&sdlPart);
	thread sdl_Thread (SDLThread, &sdlPart);

	while (true)
	{
		windowsPart->HandleMessages();
	}
	
	//g_pFramework->Quit();	
	sdl_Thread.join();
	delete windowsPart;
	return 0;
}

void SDLThread(CSDLPart* sdlPart)
{
	
	while (true)
	{
		sdlPart->HandleSDL();
	}
}