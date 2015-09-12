#include "GroundHandler.h"

CGroundHandler::CGroundHandler(SDL_Rect* camera):
terrainRenderer(&camera, &map, numTilesX)
{
	this->camera = camera;
	map = NULL;
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
		map[i].lowerTileType = 0 + GetRandom()*16;
		for (int j = 0; j < NUMUPST; j++)
		{
			map[i].upperTileTypes[j] = -1;
		}
	}

	groundRadiusMan.Init(camera, tile_width, tile_height, numTilesX);
}

// gibt die Größe der geladenen Map als Struct zurück
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
	// neuen Map-Kram anlegen
	numTilesX = width;
	numTilesY = height;
	numTiles = numTilesX*numTilesY;
	map = new Tiles[numTiles];

	int oldindex;

	for (int i = 0; i < numTilesY; i++)
	{
		for (int j = 0; j < numTilesX; j++)
		{
			oldindex = i*oldnumTilesX + j;
			// sollte die neue Map größer sein, müssen die neuen Felder mit dem Hintegrund neu belegt werden
			if ((oldindex >= oldnumTiles) || (j >= oldnumTilesX))
				map[i*numTilesX + j] = Tiles{ aktBackgroundId + GetRandom()*16, { -1, -1, -1, -1 } };
			else
				// ansonsten wir der alte wert kopiert
				map[i*numTilesX + j] = oldMap[oldindex];
		}
	}

	// die zwischenmap wieder freigeben
	delete[] oldMap;
	// da sich vermutlich die Map-Breite geändert hat, müssen die Radien neu berechnet werden
	groundRadiusMan.Init(camera, tile_width, tile_height, numTilesX);
}

void CGroundHandler::SetNewBackGround(int id)
{
	for (int i = 0; i < numTiles; i++)
	{
		if ((map[i].lowerTileType >= aktBackgroundId) && (map[i].lowerTileType < aktBackgroundId + 64))
			map[i].lowerTileType = id + GetRandom()*16;
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
	terrainRenderer.Render();
}

void CGroundHandler::RenderBorder(int mouseX, int mouseY, int radius)
{
	groundRadiusMan.SelectAktBorder(radius);
	groundRadiusMan.RenderBrushBorder(mouseX, mouseY);
}

void CGroundHandler::Delete_Map()
{
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
	id -= id%64;
	if (map[index].lowerTileType / 64 == id)
		return;
		map[index].lowerTileType = id + (rand() % 4) * 16;
}

void CGroundHandler::updateTileSeam(int index) {
	// Terrain-Basis-Id's der angrenzenden Felder (Variationen vereint)
	int adjacent_TerrainId[4] = {	map[index + numTilesX].lowerTileType % 64,
									map[index + 1].lowerTileType % 64,
									map[index - numTilesX].lowerTileType % 64,
									map[index - 1].lowerTileType % 64 };
	// Terrain-Basis-ID's der angrenzenden Felder, von den aktuellen Rahmensprites ausgehend
	int current_Seam[4] = { 0 };
	Tiles& current = map[index];

	// Berechne aus den aktuellen Sprite-ID's welche Tex-BaseID die benachbarten Felder haben sollten, iteriere dazu über alle aktuellen Rahmen-Sprites
	for (int i = 0; i < 4; i++) {
		// 'Form' des aktuellen Rahmensprites
		// 0 kein Rahmen/inaktiv;			1 überall gleicher Rahmen
		// 2 oben + unten gleicher Rand		3 links + rechts gleicher Rand
		// 4-7 einfacher Rand unten/rechts/oben/links
		// 8-11 gleicher Rand unten+rechts/rechts+oben/oben+links/links+unten
		// 12-15 gleicher Rand unten+rechts+oben/rechts+oben+links/oben+links+unten/links+unten+rechts
		int tmp_shape = current.upperTileTypes[i] % 16;

		// Ränder bei Shape > 3
		if (tmp_shape > 3)
			current_Seam[tmp_shape % 4] = current.upperTileTypes[tmp_shape % 4] % 64;
		if (tmp_shape > 7)
			current_Seam[(tmp_shape + 1) % 4] = current.upperTileTypes[tmp_shape % 4] % 64;
		if (tmp_shape > 11)
			current_Seam[(tmp_shape + 2) % 4] = current.upperTileTypes[tmp_shape % 4] % 64;
		if (tmp_shape == 1)
			for (int j = 0; j < 4; j++) current_Seam[j] = current.upperTileTypes[0] % 64;
		if (tmp_shape == 2) {
			current_Seam[0] = current.upperTileTypes[0] % 64;
			current_Seam[2] = current.upperTileTypes[0] % 64;
		}
		if (tmp_shape == 3) {
			current_Seam[1] = current.upperTileTypes[1] % 64;
			current_Seam[3] = current.upperTileTypes[1] % 64;
		}
	}
	// Nun können die Rahmentexturen mit den tatsächlichen benachbarten Texturen abgeglichen werden

	bool equal = true;
	for (int i = 0; i < 4; i++)
		if (current_Seam[i] != adjacent_TerrainId[i])
			equal = false;

	if (equal)
		return;
	else {
		// Generiere neue ID's für die Rahmensprites
		// TODO
	}
}

