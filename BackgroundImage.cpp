#include "BackgroundImage.h"

CBackgroundImage::CBackgroundImage(SDL_Rect* camera)
{
	this->camera = camera;
	sdl_renderer = g_pFramework->GetRenderer();
	sdl_texture = NULL;
	target_Rect = { 0, 0, 0, 0 };
	seitenverhaeltnis = false;
	visible = false;
	renderlayer = INGAMETEXT_LAYER;
	source_Rect = {};	
	bool loadImage = false;
	string filepath = "";
	int map_width = 0;
	int map_height = 0;

	// damit vom EventManager aus ein backgroundBild angestoßen werden kann
	g_pEventManager->InjectBackgroundImage(this);
}

CBackgroundImage::~CBackgroundImage()
{
	SDL_DestroyTexture(sdl_texture);
}


void CBackgroundImage::LoadImage(string filePath)
{
	if (sdl_texture != NULL)
		SDL_DestroyTexture(sdl_texture);
	
	// Bild laden
	sdl_texture = IMG_LoadTexture(sdl_renderer, filePath.c_str());
	if (sdl_texture == NULL)																// Prüfen, ob alles glatt ging
	{
		cout << "Fehler beim Laden von: " << filePath.c_str();
		cout << endl;
		cout << "Fehlermeldung: " << SDL_GetError() << endl;																		// Gesamtes Spiel beenden
	}

	SDL_QueryTexture(sdl_texture, NULL, NULL, &source_Rect.w, &source_Rect.h);
	// wird benötigt damit die Textur überhaupt transparent gerendert werden kann
	SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
	// Transparenz setzen
	SDL_SetTextureAlphaMod(sdl_texture, 100);
}


void CBackgroundImage::SetGroesse(int map_width, int map_height)
{
	float width_ratio = (float)map_width*45 / source_Rect.w;
	float height_ratio = (float)map_height*45 / source_Rect.h;
	if (seitenverhaeltnis)
	{
		if (width_ratio <= height_ratio)
		{
			target_Rect.w = static_cast<int>(width_ratio*source_Rect.w);
			target_Rect.h = static_cast<int>(width_ratio*source_Rect.h);
		}
		else
		{
			target_Rect.w = static_cast<int>(height_ratio*source_Rect.w);
			target_Rect.h = static_cast<int>(height_ratio*source_Rect.h);
		}
	}
	else
	{
		target_Rect.w = static_cast<int>(width_ratio*source_Rect.w);
		target_Rect.h = static_cast<int>(height_ratio*source_Rect.h);
	}
}

void CBackgroundImage::Update()
{
	// workaround, damit der WindowsThread nicht mit dem SDL_Renderer rummanscht und es abstürzt
	if (loadImage)
	{
		LoadImage(filepath);
		SetGroesse(map_width, map_height);
		loadImage = false;
	}
		
}

void CBackgroundImage::Render()
{
	if (visible && sdl_texture != NULL)
		g_pRenderlayer->add_Renderjob(this, renderlayer);
}


void CBackgroundImage::RenderYourself()
{
	target_Rect.x = MOVECAMX;
	target_Rect.y = 0;
	target_Rect.x -= camera->x;
	target_Rect.y -= camera->y;
	if ((SDL_RenderCopy(sdl_renderer, sdl_texture, &source_Rect, &target_Rect)) < 0)				// Textur wird in der Renderer kopiert
	{
		cout << "CBackgroundImage::RenderYourself: Fehler beim Kopieren der Textur: " << SDL_GetError() << endl;
	}
}


void CBackgroundImage::Bild_oeffnen(string path, int map_width, int map_height)
{
	loadImage = true;
	filepath = path;
	this->map_width = map_width;
	this->map_height = map_height;
}



void CBackgroundImage::Seitenverhaeltnis(bool seitenverhaeltnis, int map_width, int map_height)
{
	this->seitenverhaeltnis = seitenverhaeltnis;
	SetGroesse(map_width, map_height);
}

