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
			map[i].upperTileTypes[j] = 0;
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
				map[i*numTilesX + j] = Tiles{ aktBackgroundId + GetRandom() * 16, { aktBackgroundId, aktBackgroundId, aktBackgroundId, aktBackgroundId } };
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
			map[i].lowerTileType = id + GetRandom() * 16;
			for (int j = 0; j < 4; j++) {
				map[i].upperTileTypes[j] = id;
			}
		}
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
	//id -= id%64;
	if (map[index].lowerTileType / 64 != id / 64) {
		map[index].lowerTileType = id + (rand() % 4) * 16;
		for (int i = 0; i < 4; i++)
			map[index].upperTileTypes[i] = id;
	}
	updateTileSeam(index);
	updateTileSeam(min(index + numTilesX, numTiles-1));
	updateTileSeam(min(index + 1, numTiles-1));
	updateTileSeam(max(index - numTilesX, 0));
	updateTileSeam(max(index - 1, 0));
}

void CGroundHandler::updateTileSeam(int index) {
	if (index < 0 || index >= numTiles)
		return;

	Tiles& current = map[index];
	// Terrain-Basis-Id's der angrenzenden Felder (Variationen vereint)
	int adjacent_TerrainId[4] = {	map[min(index + numTilesX, numTiles-1)].lowerTileType / 64,
									map[min(index + 1, numTiles-1)].lowerTileType / 64,
									map[max(index - numTilesX, 0)].lowerTileType / 64,
									map[max(index - 1, 0)].lowerTileType / 64 };
	// Terrain-Basis-ID's der angrenzenden Felder, von den aktuellen Rahmensprites ausgehend
	int seamTerrainID[4] = {	current.lowerTileType/64,
								current.lowerTileType/64,
								current.lowerTileType/64,
								current.lowerTileType/64 };

	// For-Schleife: Bestimme Nachbarfarben, wenns nach den aktuellen Sprites geht
	for (int i = 0; i < 4; i++) {
		int shape = current.upperTileTypes[i] % 16;
		int color = current.upperTileTypes[i] / 64;
		switch (shape) {
		case 1:										// Alle Kanten gleiche Farbe
			seamTerrainID[(i + 3) % 4] = color;			// Kante links der aktuellen Kante
		case 12: case 13: case 14: case 15:			// U-Form mit den nächsten beiden Kanten gegen Uhrzeigersinn
			seamTerrainID[(i + 2) % 4] = color;			// Kante gegenüber der aktuellen Kante
		case 8: case 9: case 10: case 11:			// L-Form mit der nächsten Kante gegen Uhrzeigersinn
			seamTerrainID[(i + 1) % 4] = color;			// Kante rechts der aktuellen Kante
		case 4: case 5: case 6: case 7:				// nur aktuelle Kante
			seamTerrainID[i] = color;					// aktuelle Kante
		break;

		case 2: case 3:								// Aktuelle und gegenüberliegende Kante
			seamTerrainID[i] = color;					// aktuelle Kante
			seamTerrainID[(i + 2) % 4] = color;			// gegenüberliegende Kante
		// sonst shape 0 -> Kein Rand
		default:
			;
		}
	}

	// Brich ab wenn Sprites ok sind
	// TODO
	bool correct_seam = true;
	for (int i = 0; i < 4; i++)
		if (adjacent_TerrainId[i] == seamTerrainID[i] ||
			(adjacent_TerrainId[i] > current.lowerTileType / 64 && current.upperTileTypes[i]%16 == 0))
			;// Rand stimmt mit Nachbarn überein ODER (Kein Übergang benötigt UND Sprite ist deaktiviert)
		else
			correct_seam = false;
	if (correct_seam) {
		;// return;
	}

	// sonst generiere neue Sprites

	// Sonderfall: Alle angrenzenden Felder sind gleich, aber aktuelles Feld ist anders
	bool alle_gleich = true;
	for (int i = 1; i < 4; i++)
		if (adjacent_TerrainId[i] != adjacent_TerrainId[0])
			alle_gleich = false;
	if (alle_gleich && current.lowerTileType/64 != adjacent_TerrainId[0]) {
		current.upperTileTypes[0] = adjacent_TerrainId[0] * 64 + 1;
		for (int i = 1; i < 4; i++)
			current.upperTileTypes[i] = adjacent_TerrainId[0]*64;
		return;
	}

	// Ansonsten generiere Ecksprites da, wo die Ecke beginnt (Position am weitesten im Uhrzeigersinn)
	for (int i = 0; i < 4; i++) {
		// Farbe ist entweder gleich dem Nachbarn oder es muss keine Kante erzeugt werden
		if (adjacent_TerrainId[i] >= current.lowerTileType / 64) {
			current.upperTileTypes[i] = adjacent_TerrainId[i] * 64;
			continue;
		}

		if (adjacent_TerrainId[i] != adjacent_TerrainId[(i - 1) % 4]) {		// An aktueller Position beginnt eine Ecke
			if (adjacent_TerrainId[i] == adjacent_TerrainId[(i + 1) % 4]) { // Die mindestens zwei lang ist
				if (adjacent_TerrainId[i] == adjacent_TerrainId[(i + 2) % 4]) // die drei lang ist
					current.upperTileTypes[i] = adjacent_TerrainId[i] * 64 + 12 + i; // U-Form
				else
					current.upperTileTypes[i] = adjacent_TerrainId[i] * 64 + 8 + i;  // L-Form
			}
			else if (adjacent_TerrainId[i] == adjacent_TerrainId[(i + 2) % 4]) { // Nachbarn sind anders, was ist gegenüber?
				if (i < (i + 2) % 4)											// Nur auf einer Seite einen Sprite erzeugen!
					current.upperTileTypes[i] = adjacent_TerrainId[i] * 64 + 2 + i % 2;	// ||-Form
				else
					current.upperTileTypes[i] = adjacent_TerrainId[i] * 64;				// gegenüber bekommt ||-Form
			}
			else
				current.upperTileTypes[i] = adjacent_TerrainId[i] * 64 + 4 + i;	// Kein anderer Sprite gleich, einfache Kante
		}
		else
			current.upperTileTypes[i] = adjacent_TerrainId[i] * 64;		// linker Nachbar gleich, also deckt seine U bzw L Form die aktuelle Position ab
	}

	/*for (int i = 0; i < 4; i++) {
		if (adjacent_TerrainId[i] >= current.lowerTileType / 64)
			current.upperTileTypes[i] = adjacent_TerrainId[i] * 64;
		else if (adjacent_TerrainId[i] < current.lowerTileType / 64)
			current.upperTileTypes[i] = adjacent_TerrainId[i] * 64 + 4 + i;
	}*/
}

