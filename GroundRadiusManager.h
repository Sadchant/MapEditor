#ifndef GROUNDRADIUSMANAGER_H
#define GROUNDRADIUSMANAGER_H

#include <vector>
//#include <fstream>
#include <iostream>
#include "Sprite.h"
#include "Debugscreen.h"

using namespace std;

class CGroundRadiusManager
{
public:
	CGroundRadiusManager();
	void GenerateRadiusData(); // öffnet auch die Datei!
	void Init(SDL_Rect* camera, int tile_width, int tile_height, int numTilesX);
	int HasNextElement();
	int GetNextElement(); // gibt den aktuellen Radius zurück und schaltet den iterator eins weiter
	vector<int> getTargetArea();	// Gibt alle Positionen die vom Pinsel getroffen werden in Indexform zurück.

	void RenderBrushBorder(int mouseX, int mouseY);
	void SelectAktBorder(int radius);

private:
	struct Brush {
		vector<int> pos;
		CSprite sprite;
	};
	void fillVector();
	void AddRowtoVector(int row, int column, int number);
	void ClearAllVectors();
	void loadBrushes();

	SDL_Rect* camera;
	vector<int>  radius_1;
	vector<int>  radius_3;
	vector<int>  radius_5;
	vector<int>  radius_7;
	vector<int>  radius_9;
	vector<int>  radius_11;
	vector<int>  radius_13;
	vector<int>  radius_15;
	vector<int>::iterator it_aktRadius;
	vector<int>*  aktVector;
	int aktRadius;
	int numTilesX;
	int tempZahl;

	CSprite border_1;
	CSprite border_3;
	CSprite border_5;
	CSprite border_7;
	CSprite border_9;
	CSprite border_11;
	CSprite border_13;
	CSprite border_15;

	CSprite* aktBorder;
	int borderXPos;
	int borderYPos;
	int tile_width;
	int tile_height;

	vector<int> vecRadiusData;

	vector<Brush> brushes;
	
};

#endif