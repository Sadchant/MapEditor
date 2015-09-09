// C++ für Spieleprogrammierer
// Listing 10.3
// Eine kleine Anwendung
//

#include <windows.h>
#include "sdl.h"
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <locale>
#include <codecvt>
#include <string>

#include "ButtonEvents.h"

using namespace std;

// Prototypen
//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

HWND ErstelleHauptfenster(HINSTANCE hInst);
void ErstelleSteuerelemente(HWND hWnd, HINSTANCE hInst);
HWND erstelleGroesseWindow(HWND hWnd, HINSTANCE hInst);

string ws2s(const wstring& wstr);

// IDs der Child-Fenster
//
#define ID_GROESSEAENDERN		4002
#define ID_BILDOEFFNEN			4003
#define ID_SEITENVERHAELTNIS	4004
#define ID_ANZEIGEN				4005
#define ID_POPUPWND				4006
#define ID_XGROESSE				4007
#define	ID_YGROESSE				4008

#define ID_MENU_NEUE_MAP		4009
#define ID_MENU_OEFFNEN			4010
#define ID_MENU_SPEICHERN		4011
#define ID_MENU_SPEICHERN_UNTER	4012

#define POPUP_OK				4013


// Globale Variablen
// Don't do this at home!
//
HWND popup;
HWND hGroesse_aendern;
HWND hBild_oeffnen;
HWND hSeitenverhaeltnis;
HWND hAnzeigen;

HWND hXGroesse;
HWND hYGroesse;
HWND hXGroesseText;
HWND yGroesseText;

HWND hOk;

HWND sdl_area;

bool firstSave = true;

// Hauptprogramm
//
int WINAPI alteWinMain(HINSTANCE hInst)
{
	HWND hWnd;   // Fenster-Handle
	MSG message; // Nachricht

	// Hauptfenster erstellen
	hWnd = ErstelleHauptfenster(hInst);

	// Prüfen, ob alles glatt ging
	if (hWnd == NULL)
		return (0);

	// Alle Steuerelemente erstellen
	ErstelleSteuerelemente(hWnd, hInst);

	// Der "Herzschlag" des Programms
	// Hier werden alle Nachrichten abgeholt,
	// übersetzt und weitergeleitet
	//

	// SDL-Kram
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

	//SDL_Window* sdl_Window = SDL_CreateWindow("Test", 5, 312, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Window* sdl_Window = SDL_CreateWindowFrom(sdl_area);
	SDL_Renderer* sdl_Renderer = SDL_CreateRenderer(sdl_Window, -1, SDL_RENDERER_ACCELERATED);

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
		SDL_RenderPresent(sdl_Renderer);
		SDL_RenderClear(sdl_Renderer);
	}

	// Programm beenden
	return (int)(message.wParam);

} // WinMain


// Erstelle Hauptfenster
//
// Hauptfenster erstellen und Handle zurückliefern
//
HWND ErstelleHauptfenster(HINSTANCE hInst)
{
	HWND        hWnd;        // Fenster-Handle
	WNDCLASSEX  windowclass; // Nachricht
	WNDCLASSEX  popupclass; // Nachricht

	// Der Klassenname des Fensters ist frei wählbar
	LPCWSTR szClassName = L"Fenster";

	HMENU hMenubar = CreateMenu();

	// Struktur mit gewünschten Eigenschaften füllen
	//

	// Größe der Struktur zwischenspeichern
	windowclass.cbSize = sizeof (WNDCLASSEX);
	popupclass.cbSize = sizeof (WNDCLASSEX);

	// Fenster soll beim Verschieben neu gezeichnet werden
	windowclass.style = CS_HREDRAW | CS_VREDRAW;
	popupclass.style = CS_HREDRAW | CS_VREDRAW;

	// Zeiger auf Callback-Funktion
	windowclass.lpfnWndProc = WindowProc;
	popupclass.lpfnWndProc = ChildProc;

	// Keine erweiterten Einstellungen
	windowclass.cbClsExtra = 0;
	windowclass.cbWndExtra = 0;
	popupclass.cbClsExtra = 0;
	popupclass.cbWndExtra = 0;

	// Instanz speichern
	windowclass.hInstance = hInst;
	popupclass.hInstance = hInst;

	// Icons und Cursor festlegen
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	popupclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	popupclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	popupclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	// Hintergrundfarbe festlegen
	windowclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;
	popupclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;

	// Ein Menü brauchen wir nicht
	windowclass.lpszMenuName = NULL;
	popupclass.lpszMenuName = NULL;

	// Klassenname angeben
	windowclass.lpszClassName = szClassName;
	popupclass.lpszClassName = L"Popup";

	// Fensterklasse registrieren
	if (!RegisterClassEx(&windowclass))
		return (NULL);
	if (!RegisterClassEx(&popupclass))
		return (NULL);

	// Das Fenster erzeugen
	hWnd = CreateWindowEx(NULL,
		szClassName,
		L"Map Editor",
		//WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 100,
		1920, 839,
		NULL,
		NULL,
		hInst,
		NULL);
	//CreateWindowEx()
	sdl_area = CreateWindow(L"STATIC",
		L"sdl_area",
		WS_CHILD | WS_VISIBLE,
		0, 60,
		1904, 720,
		hWnd,
		NULL,
		hInst,
		NULL);

	// Fensterhandle zurückgeben
	return (hWnd);

} // ErstelleHauptfenster


// ErstelleSteuerelemente
//
// Alle Steuerelemente erstellen
//
void ErstelleSteuerelemente(HWND hWnd, HINSTANCE hInst)
{
	HMENU hMenubar = CreateMenu();
	HMENU menu = CreateMenu();

	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)menu, L"Menü");
	AppendMenu(menu, MF_STRING, ID_MENU_NEUE_MAP, L"neue Map");
	AppendMenu(menu, MF_STRING, ID_MENU_OEFFNEN, L"Map öffnen");
	AppendMenu(menu, MF_STRING, ID_MENU_SPEICHERN, L"Map speichern");
	AppendMenu(menu, MF_STRING, ID_MENU_SPEICHERN_UNTER, L"Map speichern unter");

	SetMenu(hWnd, hMenubar);

	hGroesse_aendern = CreateWindow(L"BUTTON",
		L"Größe ändern",
		BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		10, 10,
		110, 40,
		hWnd,
		(HMENU)ID_GROESSEAENDERN,
		hInst,
		NULL);

	hBild_oeffnen = CreateWindow(L"BUTTON",
		L"Hintergrundbild öffnen",
		BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		130, 10,
		170, 40,
		hWnd,
		(HMENU)ID_BILDOEFFNEN,
		hInst,
		NULL);

	g_pButtonEvents->SetBildData(seitenverhaeltnis, false);
	hSeitenverhaeltnis = CreateWindow(L"BUTTON",
		L"Seitenverhältnis beibehalten",
		BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD,
		320, 15,
		250, 15,
		hWnd,
		(HMENU)ID_SEITENVERHAELTNIS,
		hInst,
		NULL);

	g_pButtonEvents->SetBildData(anzeigen, false);
	hAnzeigen = CreateWindow(L"BUTTON",
		L"anzeigen",
		BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD,
		320, 35,
		200, 15,
		hWnd,
		(HMENU)ID_ANZEIGEN,
		hInst,
		NULL);

} // ErstelleSteuerelemente


HWND erstelleGroesseWindow(HWND hWnd, HINSTANCE hInst, bool neu)
{
	g_pButtonEvents->SetGroesse(xGroesse, 20);
	g_pButtonEvents->SetGroesse(yGroesse, 30);

	wstring tempWstring = to_wstring(g_pButtonEvents->GetGroesse(xGroesse));
	LPCWSTR xGroesse = tempWstring.c_str();

	wstring tempWstring2 = to_wstring(g_pButtonEvents->GetGroesse(yGroesse));
	LPCWSTR yGroesse = tempWstring2.c_str();
	
	if (neu)
	{
		xGroesse = L"";
		yGroesse = L"";
	}
	

	HWND phWnd = CreateWindow(L"Popup",
		NULL,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		200, 180,
		hWnd,
		NULL,
		hInst,
		NULL);

	hXGroesseText = CreateWindow(L"STATIC",
		L"Breite:",
		WS_VISIBLE | WS_CHILD,
		15, 22,
		300, 20,
		phWnd,
		NULL,
		hInst,
		NULL);

	hXGroesseText = CreateWindow(L"STATIC",
		L"Höhe:",
		WS_VISIBLE | WS_CHILD,
		20, 52,
		300, 20,
		phWnd,
		NULL,
		hInst,
		NULL);

	hXGroesse = CreateWindow(L"EDIT",
		xGroesse,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		70, 20,
		40, 20,
		phWnd,
		(HMENU)ID_XGROESSE,
		hInst,
		NULL);

	hYGroesse = CreateWindow(L"EDIT",
		yGroesse,
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		70, 50,
		40, 20,
		phWnd,
		(HMENU)ID_YGROESSE,
		hInst,
		NULL);

	hXGroesseText = CreateWindow(L"STATIC",
		L"Tiles",
		WS_VISIBLE | WS_CHILD,
		120, 22,
		40, 20,
		phWnd,
		NULL,
		hInst,
		NULL);

	hXGroesseText = CreateWindow(L"STATIC",
		L"Tiles",
		WS_VISIBLE | WS_CHILD,
		120, 52,
		40, 20,
		phWnd,
		NULL,
		hInst,
		NULL);

	hOk = CreateWindow(L"BUTTON",
		L"Bestätigen",
		BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
		47, 85,
		90, 40,
		phWnd,
		(HMENU)POPUP_OK,
		hInst,
		NULL);

	return phWnd;
}

string ws2s(const wstring& wstr)
{
	typedef codecvt_utf8<wchar_t> convert_typeX;
	wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

string mapFileDialog(HWND hWnd, bool open)
{
	string result = "\0";
	LPWSTR filePath = (LPWSTR)calloc(sizeof(LPWSTR), 500);
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Map Dateien (*.map)\0*.map\0Alle Dateien (*.*)\0*.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"map";

	if (open)
	{
		if (GetOpenFileName(&ofn))
		{
			wstring wString = filePath;
			result = ws2s(wString);		
			cout << result << endl;
		}
	}
	else
	{
		if (GetSaveFileName(&ofn))
		{
			result = ws2s(filePath);
		}
	}
	
	free(filePath);
	return result;
}

string bildFileDialog(HWND hWnd)
{
	string result = "\0";
	LPWSTR filePath = (LPWSTR)calloc(sizeof(LPWSTR), 500);
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"png Dateien (*.png)\0*.png\0Alle Dateien (*.*)\0*.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"map";

	if (GetOpenFileName(&ofn))
	{
		result = ws2s(filePath);
	}
	free(filePath);
	return result;
}

// Callback-Funktion zur Nachrichtenverarbeitung
//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{

	// Messages auswerten
	switch (message)
	{
	case WM_CREATE:
	{
	} break;
		// Fenster schließen? (Auch Alt-F4)
	case WM_DESTROY:
	{
					   // Nachricht zum Beenden schicken
					   PostQuitMessage(0);
					   return (0);

	}

	case WM_COMMAND:
	{
					   switch (wParam)
					   { 
					   case ID_MENU_NEUE_MAP:
					   {
												cout << "neue Map" << endl;
												popup = erstelleGroesseWindow(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), false);
												g_pButtonEvents->PressButton(neue_Map);
												firstSave = true;
												return 0;
					   }
					   case ID_MENU_OEFFNEN:
					   {
											   cout << "Map öffnen" << endl;
											   string path = mapFileDialog(hWnd, true);
											   if (path.compare("\0"))
											   {
												   g_pButtonEvents->SetSavePath(path);
												   g_pButtonEvents->PressButton(map_oeffnen);
											   }
											   firstSave = false;
											   return 0;
					   }
					   case ID_MENU_SPEICHERN:
					   {
												 if (firstSave)
												 {
													 g_pButtonEvents->SetSavePath(mapFileDialog(hWnd, false));
													 firstSave = false;
												 }
												 cout << "speichern" << endl;
												 g_pButtonEvents->PressButton(map_speichern);
												 return 0;
					   }
					   case ID_MENU_SPEICHERN_UNTER:
					   {
													   cout << "speichern unter" << endl;
													   g_pButtonEvents->SetSavePath(mapFileDialog(hWnd, false));
													   g_pButtonEvents->PressButton(map_speichern);
													   firstSave = false;
													   return 0;
					   }
					   case ID_GROESSEAENDERN:
					   {
												 cout << "Größe ändern" << endl;
												 popup = erstelleGroesseWindow(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), false);
												 g_pButtonEvents->PressButton(groesse_edit);
												 return 0;											  

					   }
					   case ID_BILDOEFFNEN:
					   {
											  cout << "Bild öffnen" << endl;
											  g_pButtonEvents->SetBildPath(bildFileDialog(hWnd));
											  //int Resultat; // Rückgabewert der Messagebox

											  // Messagebox für Sicherheitsabfrage
											  /*Resultat = MessageBox(hWnd, L"Wirklich beenden?",
												  L"Programm beenden",
												  MB_YESNO | MB_ICONQUESTION);

											  // Wurde "Ja" angeklickt?
											  if (Resultat == IDYES)
											  {
												  // Ja, also Programm beenden
												  PostQuitMessage(0);
												  return (0);

											  }

											  // Nein, also ganz normal weiter
											  return (0);*/

					   }
					   case ID_SEITENVERHAELTNIS:
					   {
													cout << "Seitenverhältnis" << endl;
													if (g_pButtonEvents->GetBildData(seitenverhaeltnis))
													{
														g_pButtonEvents->SetBildData(seitenverhaeltnis, false);
													}
													else
														g_pButtonEvents->SetBildData(seitenverhaeltnis, true);
													
													
													return 0;
					   }
					   case ID_ANZEIGEN:
					   {
										   cout << "anzeigen" << endl;
										   if (g_pButtonEvents->GetBildData(anzeigen))
										   {
											   g_pButtonEvents->SetBildData(anzeigen, false);
										   }
										   else
											   g_pButtonEvents->SetBildData(anzeigen, true);
										   return 0;
					   }
					   } break;
	} break;
	}

	// Die Nachricht wurde nicht von uns verarbeitet, also
	// von Windows verarbeiten lassen
	return (DefWindowProc(hWnd, message, wParam, lParam));

} // WindowProc

LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
					   return 0;
	}
	case WM_COMMAND:
	{
					   switch (wParam)
					   {
					   case POPUP_OK:
					   {
										cout << "bestätigen" << endl;
										LPWSTR inhalt = (LPWSTR)calloc(sizeof(LPWSTR), 500);
										GetWindowText(hXGroesse, inhalt, 256);
										string result = ws2s(inhalt);
										int ergebnis = stoi(result);
										g_pButtonEvents->SetGroesse(xGroesse, ergebnis);

										GetWindowText(hYGroesse, inhalt, 256);
										result = ws2s(inhalt);
										ergebnis = stoi(result);
										g_pButtonEvents->SetGroesse(yGroesse, ergebnis);
										free(inhalt);
										DestroyWindow(hWnd);
										return 0;
					   }
					   }break;
	}break;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));	
}

/*int main(int argc, char *argv[])
{
	HWND hwndC = GetConsoleWindow(); /* Great!!  This function
									 cleverly "retrieves the window Handle
									 used by the console associated with the calling process",
									 as msdn says */

	// Then we could just get the HINSTANCE:
	/*HINSTANCE hinstance = GetModuleHandle(0); // HMODULE=HINSTANCE
	alteWinMain(hinstance);
	return 0;
}*/