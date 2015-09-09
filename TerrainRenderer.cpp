#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer(SDL_Rect** camera, Tiles** map, const int& mapwidth):
map(*map), mapwidth(mapwidth), camera(*camera) {
	numspritex = this->camera->w / TILEWIDTH + 2;
	numspritey = this->camera->h / TILEHEIGHT + 1;

	// Erzeuge neue Terrainsprites
	spriteGrid = vector<CSprite>(numspritex*numspritey, CSprite(g_pLoader->getTexture("T_TEXTURSET1")));
	for (int i = 0; i < NUMUPST; i++)
		upperSpriteGrid[i] = vector<CSprite>(numspritex*numspritey, CSprite(g_pLoader->getTexture("T_TEXTURSET1")));
}

void TerrainRenderer::Render() {
	// Für alle Tilepositionen die die Kamera sieht
	for (int i = 0; i < numspritex; i++) {
		for (int j = 0; j < numspritey; j++) {
			// Berechne die Tileposition auf der Map
			int mapPosition = CAM_TILEPOS_X + i + (CAM_TILEPOS_Y + j)*mapwidth;
			// Positioniere den Sprite inklusive dem offset der Kamera auf dem Tileraster
			spriteGrid[j*numspritex + i].SetPos(i*TILEWIDTH-CAM_OFFSET_X, j*TILEHEIGHT-CAM_OFFSET_Y);
			// Render-Aufruf, magic by Sadchant
			spriteGrid[j*numspritex + i].Render(map[mapPosition].lowerTileType % 4, map[mapPosition].lowerTileType / 50);
			
			// Dito für die Rand-Sprites
			for (int k = 0; k < NUMUPST; k++) {
				if (map[mapPosition].upperTileTypes[k] > -1) {
					upperSpriteGrid[k][j*numspritex + i].SetPos(i*TILEWIDTH - CAM_OFFSET_X, j*TILEHEIGHT - CAM_OFFSET_Y);
					upperSpriteGrid[k][j*numspritex + i].Render((map[mapPosition].lowerTileType) % 4, (map[mapPosition].upperTileTypes[k]) / 4);
				}
			}

		}
	}
}