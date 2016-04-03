#ifndef SPRITE_H
#define SPRITE_H



#include "Renderlayer.h"
#include "Loader.h"


class CSprite : public CRenderable
{
public:
	CSprite(CTexture* texture);

	void		Render();
	void		Render(int frameNumber);
	void		Render(int frameNumber, int direction);
	void		RenderYourself();
	Point		GetTextureDimensions();

protected:
	SDL_Renderer* renderer;
	SDL_Rect source_Rect;			// Ausschnitt aus der Textur
	CTexture* texture;
	

};

#endif

