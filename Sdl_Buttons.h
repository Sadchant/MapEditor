#ifndef SDL_BUTTONS_H
#define SDL_BUTTONS_H

#include "BrushButton.h"
#include "Brushmanager.h"
#include "Rectangle.h"
#include "ButtonPage_BODEN.h"
#include "ButtonPage_MECHANISMEN.h"
#include "ButtonPage_MONSTER.h"
#include "ButtonPage_OBJEKTE.h"
#include "KeyMouseManager.h"
#include <vector>
#include <fstream>

#define TABSHEIGHT 50

class CSDL_Buttons
{
public:
	CSDL_Buttons(CBrushmanager* brushManager);	
	~CSDL_Buttons();
	void Render();
	void HandleKlickEvent(ME_MOUSE mouse, int mouseX, int mouseY);
	

private:
	CSDL_Buttons(const CSDL_Buttons& other);
	CSDL_Buttons& operator = (const CSDL_Buttons& other);
	void SelectKategorie(int mouseY);	
	
	CSprite button_background;
	CSprite tabs;
	Kategorie aktKategorie;

	int aktID;
	
	CBrushmanager* brushManager;

	static const int NUMBUTTONPAGES = 4;
	
	CButtonPage* buttonPages[NUMBUTTONPAGES];
	CButtonPage* aktButtonPage;
};

#endif