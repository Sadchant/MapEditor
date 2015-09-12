#include "BrushButton.h"

CBrushButton::CBrushButton(int id, Kategorie kategorie, SDL_Rect buttonRect, CTexture* texture, CTexture* buttonBorder) :
textur(texture),
border(buttonBorder)
{
	draw_Brush.id = id;
	draw_Brush.kategorie = kategorie;
	this->buttonRect = buttonRect;
	SDL_Rect tempRect = {buttonRect.x-2, buttonRect.y-2, buttonRect.w+4, buttonRect.h+4};
	background = new CRectangle(tempRect);
	textur.SetPos(buttonRect.x, buttonRect.y);
	border.SetPos(tempRect.x, tempRect.y);
	is_selected = false;
}

CBrushButton::CBrushButton(const CBrushButton& other) :
textur(other.textur),
border(other.border)
{
	draw_Brush = other.draw_Brush;
	buttonRect = other.buttonRect;
	background = new CRectangle(*other.background);
	is_selected = other.is_selected;

}

CBrushButton& CBrushButton::operator =(const CBrushButton& other)
{
	draw_Brush = other.draw_Brush;
	buttonRect = other.buttonRect;
	textur = other.textur;
	border = CSprite(g_pLoader->getTexture("T_BUTTONBORDER"));
	background = new CRectangle(*other.background);
	is_selected = other.is_selected;
	return *this;
}

CBrushButton::~CBrushButton()
{
	SAFE_DELETE(background);
}
// schaut, ob die übergebene Mausposition überhaupt auf dem Button war, wenn ja
// gibt er seine Instanz zurück (damit später die Farbe deselektiert werden kann)
// wenn der Button nicht getroffen wurde gibt die Funktion NULL zurück
CBrushButton* CBrushButton::SelectTry(int mouseX, int mouseY, SDL_Color farbe)
{
	if (CheckCollision(mouseX, mouseY))
	{
		background->ChangeColor(farbe);
		is_selected = true;
		return this;
	}
	return NULL;
}

CBrushButton* CBrushButton::SelectBackground(int mouseX, int mouseY)
{
	if (CheckCollision(mouseX, mouseY))
	{
		is_extra_selected = true;
		return this;
	}
	return NULL;
}

void CBrushButton::SetColor(SDL_Color color)
{
	background->ChangeColor(color);
}

//rendert den gesamten Button
void CBrushButton::Render()
{	
	switch (draw_Brush.kategorie)
	{
	case BODEN:
	{
		textur.Render(draw_Brush.id);

	}
	}
	
	border.Render();

	// Background nur rendern, wenn er auch aktiv ist
	if (is_selected)
		background->Render();

	// an der Stelle noch extra-Viereck für Button als Hintergrund ausgewählt einbauen
	/*
	if (is_extra_selected)
		extra_background->Render(); // geht nicht weil gibts nicht
	*/
}

// schaut ob der übergebene Punkt im Button lag
bool CBrushButton::CheckCollision(int X, int Y)
{
	return ((X >= buttonRect.x) &&
		(X <= buttonRect.x + buttonRect.w) &&
		(Y >= buttonRect.y) &&
		(Y <= buttonRect.y + buttonRect.h));
}