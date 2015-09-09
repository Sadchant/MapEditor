#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H
/*  TerrainRenderer: Wird mit diversen Membervariablen vom Groundhandler injiziert, sodass er auf diese
	lesend zugreifen kann (die geladene Map und die Kamera). Aufgabe: Rendere die Tiles der Map, die von
	der Kamera gesehen werden.
*/



#include "Sprite.h"
#include <vector>

#define NUMUPST 4
#define TILEWIDTH 45
#define TILEHEIGHT 45

// geben an, um wieviele Pixel die Camera im Tileraster verschoben ist
#define CAM_OFFSET_X (camera->x%TILEWIDTH)
#define CAM_OFFSET_Y (camera->y%TILEHEIGHT)

// geben an, auf welchem Tile die Camera im Tileraster liegt
#define CAM_TILEPOS_X (camera->x/TILEWIDTH)
#define CAM_TILEPOS_Y (camera->y/TILEHEIGHT)

struct Tiles
{
	int lowerTileType;
	int upperTileTypes[NUMUPST];
};

class TerrainRenderer {
public:
	TerrainRenderer(SDL_Rect** camera, Tiles** map, const int& mapwidth);
	void Render();
private:
	int numspritex, numspritey;					// Dimensionen der Kamera in Tiles

	vector<CSprite> spriteGrid;					// Beinhaltet genug Sprites, um die Kamera abzudecken
	vector<CSprite> upperSpriteGrid[NUMUPST];
	SDL_Rect* camera;
	Tiles*& map;								// ref auf die membervariable von Groundmanager welche immer die aktuelle Map hat
	const int& mapwidth;
};

#endif