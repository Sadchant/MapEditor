#ifndef WINDOWS_PART_H
#define WINDOWS_PART_H

#include <windows.h>
#include <Windowsx.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <locale>
#include <codecvt>
#include <string>

#include "KeyMouseManager.h"
#include "EventManager.h"



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

#define ID_POPUP_OK_NEW			4013
#define ID_POPUP_OK_SIZE		4014

#define WINDOWSGUI_HEIGHT 60

#define MAP_INIT_WIDTH 29
#define MAP_INIT_HEIGHT 16
#define MAP_MIN_WIDTH 29
#define MAP_MIN_HEIGHT 16
#define MAP_MAX_WIDTH 1800
#define MAP_MAX_HEIGHT 1800

#define FENSTERNAME L"Map Editor 1.11"

#define IS_DOWN(x) (GetKeyState(x) & 0x8000)


using namespace std;

struct SGui_Data
{
	int width;
	int height;
	string save_path;
	string image_path;
	bool anzeigen;
	bool seitenverhaeltnis;
};

class CWindowsPart
{
public:
	LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK childProc(HWND, UINT, WPARAM, LPARAM);

	CWindowsPart(HINSTANCE hInst);
	bool HandleMessages();

	HWND Get_SDL_area(){ return sdl_area; }
	

private:
	//string ExePath();
	HWND erstelleHauptfenster(HINSTANCE hInst);
	void erstelleSteuerelemente(HWND hWnd, HINSTANCE hInst);
	HWND erstelleGroesseWindow(HWND hWnd, HINSTANCE hInst, bool neu, int welches);
	string mapFileDialog(HWND hWnd, bool open);
	string bildFileDialog(HWND hWnd);

	string ws2s(const wstring& wstr);
	void ManageKey(int key_code, bool pressed, HWND hWnd);

	HWND hauptfenster;
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

	MSG message;

	bool on = true;
	bool groesse_open;

	SGui_Data gui_data;
};

static LRESULT CALLBACK windowProcExtern(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK childProcExtern(HWND, UINT, WPARAM, LPARAM);

static CWindowsPart* zeiger_auf_Instanz = NULL;


#endif