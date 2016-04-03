#ifndef BUTTONPAGE_BODEN_H
#define BUTTONPAGE_BODEN_H

#include "ButtonPage.h"
#include "BrushButton.h"
#include "Brushmanager.h"
#include <vector>

#define SDL_RED { 255, 0, 0 };
#define SDL_YELLOW { 255, 255, 0 };
#define SDL_CYAN { 0, 255, 255 };

class CButtonPage_BODEN : public CButtonPage
{
public:
	CButtonPage_BODEN(CBrushmanager* brushManager);
	void Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY);
	void Render();

private:
	void GenerateButtons();
	void Calculate_tile_count();
	SNext_row_data Generate_boden_standard(int start_y);
	SNext_row_data Generate_boden_wall(int start_y, int index);
	SNext_row_data Generate_boden_fluid(int start_y, int index);

	static const int NUMBODEN = 8;
	static const int NUMBODENWALL = 8;

	vector<CBrushButton> boden_standard;
	vector<CBrushButton> boden_wall;
	vector<CBrushButton> boden_fluid;

	vector<CBrushButton>::iterator it_boden_standard;

	CBrushmanager* brushManager;
};


#endif