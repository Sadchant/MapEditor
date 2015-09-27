#ifndef BUTTONPAGE_H
#define BUTTONPAGE_H

//#include "Sdl_Buttons.h"
#include "SDL.h"
#include "ButtonColorManager.h"
#include "Brushmanager.h"

struct SNext_row_data
{
	int akt_y;
	int last_id;
};

class CButtonPage
{
public:
	virtual ~CButtonPage(){};
	virtual void Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY) = 0;
	virtual void Render() = 0;

protected:
	virtual void GenerateButtons() = 0;
	CBrushButton* temp_Button;

	int y_start = 80;
	int x_start = 30;
	SDL_Rect buttonRect;
	int abstand = 65;

	int buttonWidth = 45; // mit Rand 49
	int buttonHeight = 45; // mit Rand 49
	int bor_ButtonWidth = buttonWidth + 4;
	int bor_ButtonHeight = buttonHeight + 4;
	int akt_y;
	int akt_x;
};

#endif