#ifndef BUTTONCOLORMANAGER_H
#define BUTTONCOLORMANAGER_H

#include <fstream>
#include <iostream>
#include <vector>
#include "SDL.h"

using namespace std;

class CButtonColorManager
{
public:
	CButtonColorManager();
	SDL_Color SelectColor();
	void ReadColors();

private:
	static const int numColors = 26;
	vector<SDL_Color> openColors;
	int color_Index; // an der Position der aktuellen Farbe
};

#endif