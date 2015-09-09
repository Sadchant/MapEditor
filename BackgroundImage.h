#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H

#include "Renderlayer.h"

#include "Debugscreen.h" // zu Testzwecken
#include "EventManager.h"
#include "ABackgroundImage.h"

#include <mutex>

class CBackgroundImage : public CRenderable,
						 public ABackgroundImage
{
public:
	CBackgroundImage(SDL_Rect* camera);
	~CBackgroundImage();
	void LoadImage(string filePath);
	void SetGroesse(int map_width, int map_height);
	
	void Update();
	void Render();
	void RenderYourself();

	void Bild_oeffnen(string path, int map_width, int map_height);
	void Anzeigen(bool visible){ this->visible = visible; }
	void Seitenverhaeltnis(bool seitenverhaeltnis, int map_width, int map_height);

private:
	SDL_Rect* camera;
	SDL_Renderer* sdl_renderer;
	SDL_Texture* sdl_texture;
	SDL_Rect source_Rect;
	bool seitenverhaeltnis;
	bool visible;

	bool loadImage;
	string filepath;
	int map_width;
	int map_height;

	mutex image_mutex;

};
















#endif