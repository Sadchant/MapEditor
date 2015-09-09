#pragma once

#include "Singleton.h"
#include <iostream>

#define KEY_DOWN(x) g_pKeyMouseManager->KeyDown(x)

enum ME_KEY
{
	ME_KEY_W,
	ME_KEY_A,
	ME_KEY_D,
	ME_KEY_UP,
	ME_KEY_DOWN,
	ME_KEY_RIGHT,
	ME_KEY_LEFT,
	ME_KEY_STRG,
	ME_KEY_Z,
	ME_KEY_Y,
	ME_KEY_S,
	ME_KEY_O
};

enum ME_MOUSE
{
	ME_MOUSE_LEFT,
	ME_MOUSE_MIDDLE,
	ME_MOUSE_RIGHT
};


#define g_pKeyMouseManager CKeyMouseManager::Get()

class CKeyMouseManager : public TSingleton<CKeyMouseManager>
{
public:
	CKeyMouseManager()
	{ 
		for (auto &key : keys) key = false; 
		for (auto &mouse : mouse_keys) mouse = false;
	}
	void Change_Key_Status(ME_KEY key_ID, bool status){ keys[key_ID] = status; }
	void  PressKey(ME_KEY key_ID){ keys[key_ID] = true; }
	void  ReleaseKey(ME_KEY key_ID){ keys[key_ID] = false; }
	bool  KeyDown(ME_KEY key_ID){ return keys[key_ID]; cout << "dong"; }

	void  PressMouseKey(ME_MOUSE key_ID){ mouse_keys[key_ID] = true; }
	void  ReleaseMouseKey(ME_MOUSE key_ID){ mouse_keys[key_ID] = false; }
	bool  MouseKeyDown(ME_MOUSE key_ID){ return mouse_keys[key_ID]; cout << "ding"; }

	void SetMouse_x(int mouse_x){ this->mouse_x = mouse_x; }
	void SetMouse_y(int mouse_y){ this->mouse_y = mouse_y; }
	int GetMouse_x(){ return mouse_x; }
	int GetMouse_y(){ return mouse_y; }


private:
	bool keys[12];
	bool mouse_keys[3];
	int mouse_x;
	int mouse_y;

};

