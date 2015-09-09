#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Renderlayer.h"
#include "Util.h"

class CRectangle : public CRenderable
{
public:
	CRectangle(SDL_Rect groesse);
	/*CRectangle(const CRectangle& other);
	CRectangle& operator = (const CRectangle& other);
	~CRectangle();*/
	void ChangeColor(SDL_Color newColor);
	void Render();
	void RenderYourself();

private:
	SDL_Renderer* sdl_renderer;
	SDL_Rect groesse;
	SDL_Color farbe;
	Renderlayers renderlayer = MENU_BUTTONBACKGROUND;
};

#endif