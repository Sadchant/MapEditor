#ifndef BRUSHBUTTON_H
#define BRUSHBUTTON_H

#include "Sprite.h"
#include "Rectangle.h"
#include "Debugscreen.h"
#include "ABrushButton.h"



class CBrushButton : public ABrushButton
{
public:
	CBrushButton(int id, Kategorie kategorie, SDL_Rect buttonRect, CTexture* texture, CTexture* buttonBorder);
	CBrushButton(const CBrushButton& other);
	CBrushButton& operator =(const CBrushButton& other);
	~CBrushButton();
	CBrushButton* SelectTry(int mouseX, int mouseY, SDL_Color farbe);
	CBrushButton* SelectBackground(int mouseX, int mouseY); // schaut, ob ein Button getroffen wurde, wenn ja, setze den Hintergrund
	void SetColor(SDL_Color color);
	void DeSelect(){ is_selected = false; }
	void Render();
	DrawBrush* Get_DrawBrush(){ return &draw_Brush; }

private:
	bool CheckCollision(int X, int Y);
	DrawBrush draw_Brush;
	SDL_Rect buttonRect;
	CSprite textur;
	CSprite border;
	CRectangle* background;
	bool is_selected;		// ist der Button normal angeklickt worden?
	bool is_extra_selected; // ist der Inhalt des Buttons aktueller Hintergrund?
};



#endif