#include "EventManager.h"


void CEventManager::HandleWheelEvent(int value, int mouse_x, int mouse_y)
{
	event_User->HandleWheelEvent(value, mouse_x, mouse_y);
}


void CEventManager::HandleKlickEvent(ME_MOUSE mouse_button, int mouse_x, int mouse_y)
{
	event_User->HandleKlickEvent(mouse_button, mouse_x, mouse_y);
}


void CEventManager::Neue_Map(int width, int height)
{
	mapManager->Neue_Map(width, height);
	backgroundImage->SetGroesse(width, height);
}


Point CEventManager::Map_oeffnen(string path)
{
	Point temp_point = mapManager->Map_oeffnen(path);
	backgroundImage->SetGroesse(temp_point.x, temp_point.y);
	return temp_point;
}


void CEventManager::Map_speichern(string path)
{
	mapManager->Map_speichern(path);
}


void CEventManager::Groesse_aendern(int width, int height)
{
	mapManager->Groesse_aendern(width, height);
	backgroundImage->SetGroesse(width, height);
}


void CEventManager::Bild_oeffnen(string path, int map_width, int map_height)
{
	backgroundImage->Bild_oeffnen(path, map_width, map_height);
}


void CEventManager::Anzeigen(bool anzeigen)
{
	backgroundImage->Anzeigen(anzeigen);
}


void CEventManager::Seitenverhaeltnis(bool seitenverhaeltnis, int map_width, int map_height)
{
	backgroundImage->Seitenverhaeltnis(seitenverhaeltnis, map_width, map_height);
}


void CEventManager::Rueckgaengig()
{

}


void CEventManager::Wiederholen()
{

}


