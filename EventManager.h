#pragma once

#include "Singleton.h"
#include "AMapManager.h"
#include "ABackgroundImage.h"
#include "AEventUser.h"

#define g_pEventManager CEventManager::Get()

class CEventManager : public TSingleton<CEventManager>
{
public:
	void HandleWheelEvent(int value, int mouse_x, int mouse_y);
	void HandleKlickEvent(ME_MOUSE mouse_button, int mouse_x, int mouse_y);

	void Neue_Map(int width, int height);
	Point Map_oeffnen(string path);
	void Map_speichern(string path);

	void Groesse_aendern(int width, int height);
	void Bild_oeffnen(string path, int map_width, int map_height);
	void Anzeigen(bool anzeigen);
	void Seitenverhaeltnis(bool seitenverhaeltnis, int map_width, int map_height);

	void Rueckgaengig();
	void Wiederholen();

	void InjectEventUser(AEventUser* event_User){ this->event_User = event_User; }
	void InjectMapManager(AMapManager* mapManager){ this->mapManager = mapManager; }
	void InjectBackgroundImage(ABackgroundImage* backgroundImage){ this->backgroundImage = backgroundImage; }

private:
	AEventUser* event_User;
	AMapManager* mapManager;
	ABackgroundImage* backgroundImage;

};

