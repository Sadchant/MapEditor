#include "Framework.h"

// initialisiert die benötigten SDL-Teile und ruft Init_Video auf
bool CFramework::Init(HWND area)
{
	sdl_Window = NULL;
	sdl_Renderer = NULL;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) == -1)		// Alle benötigten Systeme der SDL initialisieren
	{
		cout << "SDL konnte nicht initialisiert werden!" << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;
		Quit();
		return false;
	}

	if (Init_Video(area) == false)
	{
		cout << "SDL_Video konnte nicht initialisiert werden!" << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;
		Quit();
		return false;
	}
	pKeystate = SDL_GetKeyboardState(NULL);					// Zeiger auf internes Array für Tastaturstatus ermitteln
	IMG_Init(IMG_INIT_PNG);
	if (TTF_Init() == -1)
	{
		cout << "Fehler beim Initialsieren von TTF" << TTF_GetError() << endl;
		Quit();
		return false;
	}
	
	return true;
}

// erzeugt ein SDL_Window mit den übergebenen Werten und einen SDL_Renderer
bool CFramework::Init_Video(HWND area)
{
	string name = "Map-Editor";
	const char* pName = name.c_str();

	if (sdl_Renderer != NULL)					// sollte bereits ein Renderer vorhanden sein, zerstöre ihn (ein Renderer besitzt intern ein Window, also erst den Renderer destroyen)
	{
		SDL_DestroyRenderer(sdl_Renderer);
	}
	if (sdl_Window != NULL)						// sollte bereits ein Fenster vorhanden sein, zerstöre es
	{
		SDL_DestroyWindow(sdl_Window);
	}
	sdl_Window = SDL_CreateWindowFrom(area);
	//sdl_Window = SDL_CreateWindow(pName, 4, 200, 1904, 720, SDL_WINDOW_SHOWN);
	sdl_Renderer = SDL_CreateRenderer(sdl_Window, -1, SDL_RENDERER_ACCELERATED);

	if ((sdl_Window == NULL) || (sdl_Renderer == NULL))								// Prüfen, ob alles funktioniert hat
	{
		cout << "Fehler beim Erzeugen des Fensters!" << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;
		Quit();
		return (false);
	}
	return (true);
}


// beendet das Framework und fährt die SDL herunter
void CFramework::Quit()
{
	SDL_DestroyWindow(sdl_Window);
	SDL_DestroyRenderer(sdl_Renderer);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

// updatet Timer und Tastaturstatus
void CFramework::Update()
{
	g_pTimer->Update();			// Timer updaten
}


// schaut, ob die übergebene Tasten-ID in pKeystate drin ist, wenn ja, wird true zurückgegeben
bool CFramework::KeyDown(Uint8 Key_ID)
{
	return (pKeystate[Key_ID] ? true : false);
}

Point CFramework::getMousePosition()
{
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	return mousePosition;
}

// rendert alles aus dem Renderer und reinigt ihn danach
void CFramework::Render()
{
	SDL_RenderPresent(sdl_Renderer);
	if ((SDL_RenderClear(sdl_Renderer)) < 0)
	{
		cout << "Fehler beim Clearen des Renderers:" << SDL_GetError() << endl;
	}
}


