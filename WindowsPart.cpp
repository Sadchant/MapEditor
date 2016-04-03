#include "WindowsPart.h"

CWindowsPart::CWindowsPart(HINSTANCE hInst)
{
	// Hauptfenster erstellen
	hauptfenster = erstelleHauptfenster(hInst);

	// Prüfen, ob alles glatt ging
	if (hauptfenster == NULL)
		cout << "KRACHBUMM boeser Fehler beim Fenster-Erzeugen!" << endl;

	// Alle Steuerelemente erstellen
	erstelleSteuerelemente(hauptfenster, hInst);

	//string exepath = ExePath();
	zeiger_auf_Instanz = this;

	gui_data = { MAP_INIT_WIDTH, MAP_INIT_HEIGHT, "\0", "\0", false, false};
	groesse_open = false;
}

bool CWindowsPart::HandleMessages()
{
	if (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
		
	}
	return on;
}

/*string CWindowsPart::ExePath() 
{
	LPWSTR buffer = (LPWSTR)calloc(sizeof(LPWSTR), 500);
	string result;
	GetModuleFileName(NULL, buffer, MAX_PATH);

	wstring wString = buffer;
	result = ws2s(wString);

	string::size_type pos = string(result).find_last_of("\\/");
	return string(result).substr(0, pos);
}*/

HWND CWindowsPart::erstelleHauptfenster(HINSTANCE hInst)
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
	windowclass.lpfnWndProc = windowProcExtern;
	popupclass.lpfnWndProc = childProcExtern;

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
		FENSTERNAME,
		//WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 50,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		//550, 120,
		NULL,
		NULL,
		hInst,
		NULL);
	//CreateWindowEx()
	sdl_area = CreateWindow(L"STATIC",
		L"sdl_area",
		WS_CHILD | WS_VISIBLE,
		0, WINDOWSGUI_HEIGHT,
		1904, 720,
		hWnd,
		NULL,
		hInst,
		NULL);

	// Fensterhandle zurückgeben
	return (hWnd);

}

void CWindowsPart::erstelleSteuerelemente(HWND hWnd, HINSTANCE hInst)
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

	
	hSeitenverhaeltnis = CreateWindow(L"BUTTON",
		L"Seitenverhältnis beibehalten",
		BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD,
		320, 15,
		250, 15,
		hWnd,
		(HMENU)ID_SEITENVERHAELTNIS,
		hInst,
		NULL);

	
	hAnzeigen = CreateWindow(L"BUTTON",
		L"anzeigen",
		BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD,
		320, 35,
		200, 15,
		hWnd,
		(HMENU)ID_ANZEIGEN,
		hInst,
		NULL);

}

HWND CWindowsPart::erstelleGroesseWindow(HWND hWnd, HINSTANCE hInst, bool neu, int welches)
{
	groesse_open = true;
	wstring tempWstring = to_wstring(gui_data.width);
	LPCWSTR xGroesse = tempWstring.c_str();

	wstring tempWstring2 = to_wstring(gui_data.height);
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
		(HMENU)welches,
		hInst,
		NULL);

	return phWnd;
}

string CWindowsPart::ws2s(const wstring& wstr)
{
	typedef codecvt_utf8<wchar_t> convert_typeX;
	wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

string CWindowsPart::mapFileDialog(HWND hWnd, bool open)
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
			result = ws2s(filePath);
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

string CWindowsPart::bildFileDialog(HWND hWnd)
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
	ofn.lpstrDefExt = L"png";

	if (GetOpenFileName(&ofn))
	{
		result = ws2s(filePath);
	}
	free(filePath);
	return result;
}

// Callback-Funktion zur Nachrichtenverarbeitung
//
LRESULT CALLBACK CWindowsPart::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		on = false;
		PostQuitMessage(0);
		return (0);

	}break;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_MENU_NEUE_MAP:
		{
			if (!groesse_open)
				popup = erstelleGroesseWindow(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), false, ID_POPUP_OK_NEW);
			gui_data.save_path = "\0";
			return 0;
		}
		case ID_MENU_OEFFNEN:
		{
			gui_data.save_path = mapFileDialog(hWnd, true);
			if (gui_data.save_path.compare("\0"))
			{
				Point temp_point = g_pEventManager->Map_oeffnen(gui_data.save_path);
				gui_data.width = temp_point.x;
				gui_data.height = temp_point.y;
			}
				
			return 0;
		}
		case ID_MENU_SPEICHERN:
		{
			if (!gui_data.save_path.compare("\0"))
				gui_data.save_path = mapFileDialog(hWnd, false);
			// hier kein else, weil er das auch aufrufen soll, nachdem der mapfiledialog geöffnet wurde
			if (gui_data.save_path.compare("\0"))
				g_pEventManager->Map_speichern(gui_data.save_path);
			return 0;
		}
		case ID_MENU_SPEICHERN_UNTER:
		{
			gui_data.save_path = mapFileDialog(hWnd, false);
			// hier kein else, weil er das auch aufrufen soll, nachdem der mapfiledialog geöffnet wurde
			if (gui_data.save_path.compare("\0"))
				g_pEventManager->Map_speichern(gui_data.save_path);
			return 0;
		}
		case ID_GROESSEAENDERN:
		{
			if (!groesse_open)
				popup = erstelleGroesseWindow(hWnd, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), false, ID_POPUP_OK_SIZE);
			return 0;
		}
		case ID_BILDOEFFNEN:
		{
			gui_data.image_path = bildFileDialog(hWnd);
			if (gui_data.image_path.compare("\0"))
				g_pEventManager->Bild_oeffnen(gui_data.image_path, gui_data.width, gui_data.height);
			// sonst ignoriert er ab hier Tastatureingaben
			SetFocus(hauptfenster);
			return 0;	
		}
		case ID_SEITENVERHAELTNIS:
		{
			// hier müsste an eig den State aus der Checkbox auslesen aber meeh ;)
			gui_data.seitenverhaeltnis = !gui_data.seitenverhaeltnis;
			g_pEventManager->Seitenverhaeltnis(gui_data.seitenverhaeltnis, gui_data.width, gui_data.height);
			// sonst ignoriert er ab hier Tastatureingaben
			SetFocus(hauptfenster);
			return 0;
		}
		case ID_ANZEIGEN:
		{
			gui_data.anzeigen = !gui_data.anzeigen;
			g_pEventManager->Anzeigen(gui_data.anzeigen);
			// sonst ignoriert er ab hier Tastatureingaben
			SetFocus(hauptfenster);
			return 0;
		}
		} break;
	} break;
	// linke Maustatse gedrückt
	case WM_LBUTTONDOWN:
	{
		g_pKeyMouseManager->PressMouseKey(ME_MOUSE_LEFT);
		g_pEventManager->HandleKlickEvent(ME_MOUSE_LEFT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) - WINDOWSGUI_HEIGHT);
		// damit das Programm mitbekommt, wenn die Maus außerhalb des Fensters losgelassen wird
		SetCapture(hauptfenster);
	}break;

	// linke Maustatse losgelassen
	case WM_LBUTTONUP:
	{
		g_pKeyMouseManager->ReleaseMouseKey(ME_MOUSE_LEFT);
		// stellt den normalen Maus-Modus wieder her
		ReleaseCapture();
	}break;

	// mittlere Maustatse gedrückt
	case WM_MBUTTONDOWN:
	{
		g_pKeyMouseManager->PressMouseKey(ME_MOUSE_MIDDLE);
		g_pEventManager->HandleKlickEvent(ME_MOUSE_MIDDLE, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) - WINDOWSGUI_HEIGHT);
		SetCapture(hauptfenster);
	}break;

	// mittlere Maustaste losgelassen
	case WM_MBUTTONUP:
	{
		g_pKeyMouseManager->ReleaseMouseKey(ME_MOUSE_MIDDLE);
		ReleaseCapture();
	}break;

	// rechte Maustaste gedrückt
	case WM_RBUTTONDOWN:
	{
		g_pKeyMouseManager->PressMouseKey(ME_MOUSE_RIGHT);
		g_pEventManager->HandleKlickEvent(ME_MOUSE_RIGHT, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) - WINDOWSGUI_HEIGHT);
		SetCapture(hauptfenster);
	}break;

	// rechte Maustaste losgelassen
	case WM_RBUTTONUP:
	{
		g_pKeyMouseManager->ReleaseMouseKey(ME_MOUSE_RIGHT);
		ReleaseCapture();
	}break;

	// Mausrad gedreht
	case WM_MOUSEWHEEL:
	{
		short dings = HIWORD(wParam);
		g_pEventManager->HandleWheelEvent((short)HIWORD(wParam) / 120, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) - WINDOWSGUI_HEIGHT);
	}break;
	case WM_MOUSEMOVE:
	{
		int mouse_x = GET_X_LPARAM(lParam);
		int mouse_y = GET_Y_LPARAM(lParam) - WINDOWSGUI_HEIGHT;

		// sollte sich die Maus rechts aus dem Fenster bewegen
		if (mouse_x > WINDOW_WIDTH) return (DefWindowProc(hWnd, message, wParam, lParam));

		g_pKeyMouseManager->SetMouse_x(mouse_x);
		g_pKeyMouseManager->SetMouse_y(mouse_y);
	}break;
	case WM_KEYDOWN:
	{
		ManageKey(wParam, true, hWnd);
		return 0;
	}break;
	case WM_KEYUP:
	{
		ManageKey(wParam, false, hWnd);
		return 0;
	}
	}

	// Die Nachricht wurde nicht von uns verarbeitet, also
	// von Windows verarbeiten lassen
	return (DefWindowProc(hWnd, message, wParam, lParam));

} // WindowProc

void CWindowsPart::ManageKey(int key_code, bool pressed, HWND hWnd)
{
	switch (key_code)
	{
	case 0x57:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_W, pressed);
	}break;
	case 0x41:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_A, pressed);
	}break;
	case 0x44:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_D, pressed);
	}break;
	case VK_UP:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_UP, pressed);
	}break;
	case VK_DOWN:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_DOWN, pressed);
	}break;
	case VK_LEFT:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_LEFT, pressed);
	}break;
	case VK_RIGHT:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_RIGHT, pressed);
	}break;
	case VK_CONTROL:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_STRG, pressed);
	}break;
	case 0x5A:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_Z, pressed);
		if (IS_DOWN(VK_CONTROL))
			g_pEventManager->Rueckgaengig();			
	}break;
	case 0x59:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_Y, pressed);
		if (IS_DOWN(VK_CONTROL))
			g_pEventManager->Wiederholen();
	}break;
	case 0x53:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_S, pressed);
		if (IS_DOWN(VK_CONTROL))
		{
			if (!gui_data.save_path.compare("\0"))
				gui_data.save_path = mapFileDialog(hWnd, false);
			// hier kein else, weil er das auch aufrufen soll, nachdem der mapfiledialog geöffnet wurde
			if (gui_data.save_path.compare("\0"))
				g_pEventManager->Map_speichern(gui_data.save_path);
		}
			
	}break;
	case 0x4F:
	{
		g_pKeyMouseManager->Change_Key_Status(ME_KEY_O, pressed);

		if (IS_DOWN(VK_CONTROL))
		{
			gui_data.save_path = mapFileDialog(hWnd, true);
			if (gui_data.save_path.compare("\0"))
				g_pEventManager->Map_oeffnen(gui_data.save_path);
		}
	}break;
	}
}

LRESULT CALLBACK CWindowsPart::childProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		groesse_open = false;
		return 0;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_POPUP_OK_NEW:
		{
			LPWSTR inhalt = (LPWSTR)calloc(sizeof(LPWSTR), 500);
			GetWindowText(hXGroesse, inhalt, 256);
			string result = ws2s(inhalt);
			gui_data.width = stoi(result);

			if (gui_data.width < MAP_MIN_WIDTH)
				gui_data.width = MAP_MIN_WIDTH;
			if (gui_data.width > MAP_MAX_WIDTH)
				gui_data.width = MAP_MAX_WIDTH;

			GetWindowText(hYGroesse, inhalt, 256);
			result = ws2s(inhalt);
			gui_data.height = stoi(result);

			if (gui_data.height < MAP_MIN_HEIGHT)
				gui_data.height = MAP_MIN_HEIGHT;
			if (gui_data.height > MAP_MAX_HEIGHT)
				gui_data.height = MAP_MAX_HEIGHT;

			g_pEventManager->Neue_Map(gui_data.width, gui_data.height);
			free(inhalt);
			DestroyWindow(hWnd);
			groesse_open = false;
			return 0;
		}
		case ID_POPUP_OK_SIZE:
		{
			LPWSTR inhalt = (LPWSTR)calloc(sizeof(LPWSTR), 500);
			GetWindowText(hXGroesse, inhalt, 256);
			string result = ws2s(inhalt);
			gui_data.width = stoi(result);

			if (gui_data.width < MAP_MIN_WIDTH)
				gui_data.width = MAP_MIN_WIDTH;
			if (gui_data.width > MAP_MAX_WIDTH)
				gui_data.width = MAP_MAX_WIDTH;
			
			GetWindowText(hYGroesse, inhalt, 256);
			result = ws2s(inhalt);
			gui_data.height = stoi(result);

			if (gui_data.height < MAP_MIN_HEIGHT)
				gui_data.height = MAP_MIN_HEIGHT;
			if (gui_data.height > MAP_MAX_HEIGHT)
				gui_data.height = MAP_MAX_HEIGHT;

			g_pEventManager->Groesse_aendern(gui_data.width, gui_data.height);
			free(inhalt);
			DestroyWindow(hWnd);
			groesse_open = false;
			return 0;
		}
		}break;
	}break;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

LRESULT CALLBACK windowProcExtern(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return zeiger_auf_Instanz->windowProc(hwnd, umessage, wparam, lparam);
}

LRESULT CALLBACK childProcExtern(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return zeiger_auf_Instanz->childProc(hwnd, umessage, wparam, lparam);
}
