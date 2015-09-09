#include "ButtonColorManager.h"

CButtonColorManager::CButtonColorManager()
{
	ReadColors();
	color_Index = rand() % openColors.size();
}

// befüllt Liste mit allColors mit brauchbaren Farben aus einer Datei 
void CButtonColorManager::ReadColors()
{
	int temp; // buffer, da direktes schreiben in color.r, ... falsche zahlen geschrieben hat
	SDL_Color tempColor = { 0 };
	string fileName = "Editor_Data/Colors.color";
	ifstream colors(fileName);
	if (!colors.is_open())
	{
		printf("%s konnte nicht geoeffnet werden!", fileName);
		//cout << fileName << " konnte nicht geoeffnet werden!" << endl;
		return;
	}
	while (!colors.eof())
		//for (int i = 0; i < 26; i++)
	{
		colors >> temp;
		tempColor.r = temp;
		colors >> temp;
		tempColor.g = temp;
		colors >> temp;
		tempColor.b = temp;
		openColors.push_back(tempColor);
	}
	colors.close();
}

// wählt eine Farbe aus und zählt den Index hoch, sodass der immer rund geht
SDL_Color CButtonColorManager::SelectColor()
{
	SDL_Color tempColor = openColors[color_Index];
	color_Index++;
	color_Index %= openColors.size();
	return tempColor;
}