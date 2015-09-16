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
		map[i].lowerTileType = 0 + (rand()%4) * 16;
		for (int j = 0; j < NUMUPST; j++)
		{
			map[i].upperTileTypes[j] = 0 + (rand()%4) * 16;
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
				map[i*numTilesX + j] = Tiles{ aktBackgroundId + (rand() % 4) * 16,
					  { aktBackgroundId + (rand() % 4) * 16,
						aktBackgroundId + (rand() % 4) * 16,
						aktBackgroundId + (rand() % 4) * 16,
						aktBackgroundId + (rand() % 4) * 16 } };
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
		if ((map[i].lowerTileType >= aktBackgroundId) && (map[i].lowerTileType < aktBackgroundId + 64)) {
			map[i].lowerTileType = id + (rand()%4) * 16;
			for (int j = 0; j < 4; j++) {
				map[i].upperTileTypes[j] = id + (rand()%4) * 16;
			}
		}
	}
	for (int i = 0; i < numTiles; i++)
		updateTileSeam(i);
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
	id /= 64;
	if (map[index].lowerTileType / 64 != id) {
		map[index].lowerTileType = id * 64 + (rand() % 4) * 16;
		for (int i = 0; i < 4; i++)
			map[index].upperTileTypes[i] = id * 64 + (rand()%4) * 16;
		updateTileSeam(index);
		updateTileSeam(min(index + numTilesX, numTiles - 1));
		updateTileSeam(min(index + 1, numTiles - 1));
		updateTileSeam(max(index - numTilesX, 0));
		updateTileSeam(max(index - 1, 0));
	}
}

void CGroundHandler::updateTileSeam(int index) {
	if (index < 0 || index >= numTiles)
		return;

	Tiles& current = map[index];

	// aktuelle Varianten
	int variant[4];
	for (int i = 0; i < 4; i++)
		variant[i] = (current.upperTileTypes[i] / 16) % 4;

	// Terrainfarben der Nachbarfelder
	int adj_color[4] = {	map[min(index + numTilesX, numTiles-1)].lowerTileType / 64,
							map[min(index + 1, numTiles-1)].lowerTileType / 64,
							map[max(index - numTilesX, 0)].lowerTileType / 64,
							map[max(index - 1, 0)].lowerTileType / 64 };

	// Benötigte Spriteformen
	int want_shape[4];
	for (int i = 0; i < 4; i++) {
		if (adj_color[i] == adj_color[(i + 3) % 4]) {							// Linker Nachbar hat gleiche Farbe?
			want_shape[i] = 0;
			if (adj_color[i] == adj_color[(i + 2) % 4] && adj_color[i] == adj_color[(i + 1) % 4])
				want_shape[i] = 1;
		} // Linker Nachbar hat andere Farbe
		else if (adj_color[i] == adj_color[(i + 2) % 4]) {							// Gegenüber gleiche Farbe?
			if (adj_color[i] == adj_color[(i + 1) % 4]) {							// Rechts gleiche Farbe?
				want_shape[i] = 12 + i;
			} else { // nur i und Gegenüber gleich
				want_shape[i] = (i < 2) ? 2 + i : 0;
			}
		} // Gegenüber hat auch andere Farbe
		else {
			if (adj_color[i] == adj_color[(i + 1) % 4]) {
				want_shape[i] = 8 + i;
			} else {
				want_shape[i] = 4 + i;
			}
		}
	}

	// Sprite deaktivieren wenn Hierarchie es verlangt
	for (int i = 0; i < 4; i++) {
		if (current.lowerTileType / 64 <= adj_color[i])
			want_shape[i] = 0;
	}
	
	// Shape und Color der aktuellen Tiles updaten, ohne Variante zu verändern
	for (int i = 0; i < 4; i++) {
		current.upperTileTypes[i] =
			adj_color[i] * 64 +
			variant[i] * 16 +
			want_shape[i];
	}
}

