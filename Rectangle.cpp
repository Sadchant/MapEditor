#include "Rectangle.h"

CRectangle::CRectangle(SDL_Rect groesse)
{
	this->groesse = groesse;
	sdl_renderer = g_pFramework->GetRenderer();
}

/*CRectangle::CRectangle(const CRectangle& other)
{
	sdl_renderer = g_pFramework->GetRenderer();
	groesse = SDL_Rect(other.groesse);
	farbe = other.farbe;
	renderlayer = other.renderlayer;
}

CRectangle& CRectangle::operator = (const CRectangle& other)
{
	sdl_renderer = g_pFramework->GetRenderer();
	groesse = SDL_Rect(other.groesse);
	farbe = other.farbe;
	renderlayer = other.renderlayer;
	return *this;
}*/



void CRectangle::ChangeColor(SDL_Color newColor)
{
	farbe = newColor;
}

void CRectangle::Render()
{
	g_pRenderlayer->add_Renderjob(this, renderlayer);
}

void CRectangle::RenderYourself()
{
	if ((SDL_SetRenderDrawColor(sdl_renderer, farbe.r, farbe.g, farbe.b, farbe.a)) < 0)
	{
		cout << "CRectangle::RenderYourself: Fehler beim Setzend er Farbe" << endl;
	}
	if ((SDL_RenderFillRect(sdl_renderer, &groesse)) < 0)
	{
		cout << "CRectangle::RenderYourself: Fehler beim Setzen der Farbe" << endl;
	}

}