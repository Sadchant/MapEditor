#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <list>
#include "GroundHandler.h"

#include "Brushmanager.h"
#include "EventManager.h"
#include "KeyMouseManager.h"

#include "AMapManager.h"



class CMapManager : public AMapManager
{
public:
	CMapManager(SDL_Rect* camera, CBrushmanager* brushManager, mutex* map_mutex);
	void Handle_Events();
	void ChangeBrushBorder(int value);
	void Render();
	void UpdateCamera();

	void Neue_Map(int width, int height);
	Point Map_oeffnen(string path);
	void Map_speichern(string path);
	void Groesse_aendern(int width, int height);
	void Rueckgaengig();
	void Wiederholen();

private:
	void CalculateMapDimensions();
	void RenderBrushBorder();
	void Draw(int mouseX, int mouseY);

	list<Uint8> mouseButtons;
	list<SDL_Scancode> keys;
	list<Uint8>::iterator it_mouseButtons;
	list<SDL_Scancode>::iterator it_keys;


	SDL_Rect* camera;
	float camera_x;
	float camera_y;
	CGroundHandler groundHandler;
	CBrushmanager* brushManager;
	float fDirection;

	ME_KEY ssc_up = ME_KEY_W;
	ME_KEY ssc_left = ME_KEY_A;
	ME_KEY ssc_down = ME_KEY_S;
	ME_KEY ssc_right = ME_KEY_D;
	ME_KEY ssc_up_fast = ME_KEY_UP;
	ME_KEY ssc_left_fast = ME_KEY_LEFT;
	ME_KEY ssc_down_fast = ME_KEY_DOWN;
	ME_KEY ssc_right_fast = ME_KEY_RIGHT;
	float speed_normal = 400.0f;
	float speed_fast = 800.0f;

	int map_width, map_height;
	Point aktmousePos;

	Kategorie aktKategorie;

	int tempRadius;

	DrawBrush* tempDrawBrush;

	mutex* map_mutex;

};









#endif