#include "GroundHandler.h"

CGroundHandler::CGroundHandler(SDL_Rect* camera)
{
	this->camera = camera;
	map = NULL;
	lowerSpriteTile = NULL;
	for (int i = 0; i < NUMUPST; i++)
	{
		upperSpriteTiles[i] = NULL;
	}
}

CGroundHandler::~CGroundHandler()
{
	Delete_Map();
}

void CGroundHandler::Neue_map(int width, int height)
{
	// sollte die Map blockiert sein, warte, bis sie wieder freigegeben wird

	Delete_Map();
	numTilesX = width;
	numTilesY = height;
	numTiles = numTilesX*numTilesY;
	map = new Tiles[numTiles];
	for (int i = 0; i < numTiles; i++)
	{
		map[i].lowerTileType = 0 + GetRandom();
		for (int j = 0; j < NUMUPST; j++)
		{
			map[i].upperTileTypes[j] = -1;
		}
	}
	lowerSpriteTile = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	for (int i = 0; i < NUMUPST; i++)
	{
		upperSpriteTiles[i] = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	}
	groundRadiusMan.Init(camera, tile_width, tile_height, numTilesX);
}

// gibt die Gr��e der geladenen Map als Struct zur�ck
Point CGroundHandler::Load_map(string filePath)
{
	Point dimensions = {};
	Delete_Map();
	ifstream input(filePath);
	if (!input.is_open())
	{
		cout << filePath << "konnte nicht geoeffnet werden!";
		return dimensions;
	}
	input >> numTilesX;
	input >> numTilesY;
	input >> tile_width;
	input >> tile_height;

	dimensions.x = numTilesX;
	dimensions.y = numTilesY;

	numTiles = numTilesX*numTilesY;
	map = new Tiles[numTiles];

	for (int i = 0; i < numTiles; i++)
	{
		input >> map[i].lowerTileType;
		for (int j = 0; j < NUMUPST; j++)
		{
			input >> map[i].upperTileTypes[j];
		}
	}
	input.close();
	lowerSpriteTile = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	for (int i = 0; i < NUMUPST; i++)
	{
		upperSpriteTiles[i] = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	}
	groundRadiusMan.Init(camera, tile_width, tile_height, numTilesX);
	return dimensions;
}

void CGroundHandler::Save_map(string filePath)
{
	ofstream output(filePath);
	if (!output.is_open())
	{
		cout << filePath << "konnte nicht geoeffnet werden!";
		return;
	}
	output << numTilesX << " ";
	output << numTilesY << " ";
	output << tile_width << " ";
	output << tile_height << " ";
	for (int i = 0; i < numTiles; i++)
	{
		output << map[i].lowerTileType << " ";
		for (int j = 0; j < NUMUPST; j++)
		{
			output << map[i].upperTileTypes[j] << " ";
		}
	}
	output.close();
}

void CGroundHandler::Change_groesse(int width, int height, int aktBackgroundId)
{
	Tiles* oldMap;
	int oldnumTilesX = numTilesX;
	int oldnumTiles = numTiles;
	oldMap = new Tiles[numTiles];
	// alte Map zwischenspeichern
	for (int i = 0; i < numTiles; i++)
	{
		oldMap[i] = map[i];
	}
	Delete_Map();
	// n�uen Map-Kram anlegen
	numTilesX = width;
	numTilesY = height;
	numTiles = numTilesX*numTilesY;
	map = new Tiles[numTiles];
	lowerSpriteTile = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	for (int i = 0; i < NUMUPST; i++)
	{
		upperSpriteTiles[i] = new CSpriteObject(g_pLoader->getTexture("T_TEXTURSET1"), numTiles);
	}
	int oldindex;

	for (int i = 0; i < numTilesY; i++)
	{
		for (int j = 0; j < numTilesX; j++)
		{
			oldindex = i*oldnumTilesX + j;
			// sollte die neue Map gr��er sein, m�ssen die neuen Felder mit dem Hintegrund neu belegt werden
			if ((oldindex >= oldnumTiles) || (j >= oldnumTilesX))
				map[i*numTilesX + j] = Tiles{ aktBackgroundId + GetRandom(), { -1, -1, -1, -1 } };
			else
				// ansonsten wir der alte wert kopiert
				map[i*numTilesX + j] = oldMap[oldindex];
		}
	}

	// die zwischenmap wieder freigeben
	delete[] oldMap;
	// da sich vermutlich die Map-Breite ge�ndert hat, m�ssen die Radien neu berechnet werden
	groundRadiusMan.Init(camera, tile_width, tile_height, numTilesX);
}

void CGroundHandler::SetNewBackGround(int id)
{
	for (int i = 0; i < numTiles; i++)
	{
		if ((map[i].lowerTileType >= aktBackgroundId) && (map[i].lowerTileType <= aktBackgroundId + 4))
			map[i].lowerTileType = id+ GetRandom();
	}
	aktBackgroundId = id ;
}

void CGroundHandler::Draw(int mouseX, int mouseY, int id, int radius)
{
	index_1 = (camera->y + mouseY) / tile_height;
	index_2 = (camera->x + mouseX) / tile_width;
	index = (index_1 * numTilesX) + index_2;
	aktModus = CalculateModus(numTilesX, radius, index);
	g_pDebugscreen->Set("Modus:", aktModus);
	switch (aktModus)
	{
		case 1:
		{
				  DrawModus1(index, radius, id);
		}break;
		case 2:
		{
				  DrawModus2(index, radius, id);
		}break;
		case 3:
		{
				  DrawModus3(index, radius, id);
		}break;
	}
}




void CGroundHandler::Render()
{
	for (int i = 0; i < (camera->h / tile_height) + 1; i++) // geht �ber die Anzahl der Tiles in y-Richtung
	{
		for (int j = 0; j < (camera->w / tile_width) + 2; j++) // geht �ber die Anzahl der Tiles in x-Richtung, hier + 2, da rundungsfehler aufgefangen werden m�ssen
		{
			index_1 = (int)(i + camera->y / tile_height);
			index_2 = (int)(j + camera->x / tile_width);
			index = (index_1 * numTilesX) + index_2;
			if (index >= numTiles)
				break;
			temp_x = index_2 * tile_width;
			temp_y = index_1 * tile_height;
			lowerSpriteTile->SetPos(index, temp_x - camera->x, temp_y - camera->y);
			lowerSpriteTile->Render(index, (map[index].lowerTileType) % 4, map[index].lowerTileType/50);

			// R�nder rendern
			for (int k = 0; k < NUMUPST; k++)
			{
				if (map[index].upperTileTypes[k] > -1)
				{
					upperSpriteTiles[k]->SetPos(index, temp_x - camera->x, temp_y - camera->y);
					upperSpriteTiles[k]->Render(index, (map[index].lowerTileType) % 4, (map[index].upperTileTypes[k]) / 4);
				}
			}
			
			
			index_1 = -1;
			index_2 = -1;
			index = -1;
			temp_x = -1;
			temp_y = -1;
		}
	}
}

void CGroundHandler::RenderBorder(int mouseX, int mouseY, int radius)
{
	groundRadiusMan.SelectAktBorder(radius);
	groundRadiusMan.RenderBrushBorder(mouseX, mouseY);
}

void CGroundHandler::Delete_Map()
{
	SAFE_DELETE(lowerSpriteTile);
	for (int i = 0; i < NUMUPST; i++)
	{
		SAFE_DELETE(upperSpriteTiles[i]);
	}
	SAFE_DELETE_ARRAY(map);
	numTiles = -1;
}

int CGroundHandler::GetRandom()
{
	return rand() % 3;
}

int CGroundHandler::CalculateModus(int numTilesX, int radius, int index)
{
	int column = index % numTilesX;
	if ((column >= numTilesX - (radius / 2)) && (column < numTilesX))
		return 1;
	if ((column >= 0) && (column < radius / 2))
		return 2;
	return 3;
}

void CGroundHandler::DrawModus1(int index, int radius, int id)
{
	while (groundRadiusMan.HasNextElement())
	{
		aktElem = index + groundRadiusMan.GetNextElement(); // da der Wert relativ zum angeklickten Feld ist
		if (aktElem % numTilesX < numTilesX - radius)
			continue;
		DrawField(aktElem, id);
	}
}

void CGroundHandler::DrawModus2(int index, int radius, int id)
{
	while (groundRadiusMan.HasNextElement())
	{
		aktElem = index + groundRadiusMan.GetNextElement(); // da der Wert relativ zum angeklickten Feld ist
		if (aktElem % numTilesX > radius)
			continue;
		DrawField(aktElem, id);
	}
}

void CGroundHandler::DrawModus3(int index, int radius, int id)
{
	while (groundRadiusMan.HasNextElement())
	{
		aktElem = index + groundRadiusMan.GetNextElement(); // da der Wert relativ zum angeklickten Feld ist
		DrawField(aktElem, id);
	}
}

void CGroundHandler::DrawField(int index, int id)
{
	if ((index < 0) || (index >= numTiles))
		return;
	if ((map[index].lowerTileType >= id) && (map[index].lowerTileType < id + 4))
		return;
	map[index].lowerTileType = id + rand() % 4;
}

