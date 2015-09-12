#ifndef BACKGROUNDHANDLER_H
#define BACKGROUNDHANDLER_H

#include <mutex>
#include "SpriteObject.h"
#include "GroundRadiusManager.h"
#include "TerrainRenderer.h"

class CGroundHandler
{
public:
	CGroundHandler(SDL_Rect* camera);
	~CGroundHandler();
	void Init_map(int width, int height); //erzeugt ein map-array von der richtigen gr��e
	void Neue_map(int width, int height);
	Point Load_map(string filePath);
	void Save_map(string filePath);
	void Change_groesse(int width, int height, int aktBackgroundId);

	void SetNewBackGround(int id);
	void Draw(int mouseX, int mouseY, int id, int radius);

	int Get_map_width(){ return numTilesX*tile_width; }
	int Get_map_height(){ return numTilesY*tile_height; }
	
	void Render();
	void RenderBorder(int mouseX, int mouseY, int radius);

private:
	void Delete_Map();
	int GetRandom();	// gibt Zahl zwischen 0 und 3 zur�ck
	int CalculateModus(int numTilesX, int radius, int index);
	void DrawModus1(int index, int radius, int id);
	void DrawModus2(int index, int radius, int id);
	void DrawModus3(int index, int radius, int id);
	void DrawField(int index, int id);
	void updateTileSeam(int index);

	Tiles* map;
	int numTiles;
	int tile_width = TILEWIDTH, tile_height = TILEHEIGHT;
	int numTilesX, numTilesY;
	int index_1, // erste Position im zweidimensionalen Tile-Array, repr�sentiert y
		index_2, // zweite Position im zweidimensionalen Tile-Array, repr�sentiert x
		index, // Position im eindimensionalen Array
		temp_x, // tats�chliche x-Position
		temp_y; // tats�chliche y-Position

	TerrainRenderer terrainRenderer;
	

	SDL_Rect* camera;
	CGroundRadiusManager groundRadiusMan;

	int aktBackgroundId;
	int aktModus;
	int aktElem;


};

#endif