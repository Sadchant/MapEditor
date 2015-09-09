#ifndef BRUSHMANAGER_H
#define BRUSHMANAGER_H

#include "SDL.h"
#include "BrushButton.h"
#include "KeyMouseManager.h"

#define NOBRUSHSELECTEDEXCEPTION 555

struct Brush
{
	ABrushButton* brushButton;
	int radius;	
};

/*struct ButtonModified
{
	SDL_Scancode button;
	Uint16 modifier;
	bool operator<(const ButtonModified& o)
		const
	{
				  return (button<o.button || (button == o.button && modifier<o.modifier));
	}
	bool operator==(const ButtonModified& o)
		const
	{
		return (button == o.button && modifier==o.modifier);
	}
};*/



class CBrushmanager
{
public:	
	CBrushmanager();
	void Map_Mouse_Key(ME_MOUSE mouse, ABrushButton* brushButton);
	DrawBrush* Get_DrawBrush(ME_MOUSE mouse);
	void SetGroundId(int id);
	int GetGroundId();
	bool OldColorDeleted(){ return oldColorDel; }
	bool hasGroundchanged(){ return groundChanged; }

	DrawBrush* Get_akt_DrawBrush(){ return akt_DrawBrush; }
	void Set_Radius(int radius){ this->radius = radius; }
	int Get_Radius(){ return radius; }

private:
	void Delete_Brush(CBrushButton* brushButton); //findet bisherigen Eintrag und löscht diesen

	int groundID; // der aktuelle Grundton des Fußbodens
	bool groundChanged;

	bool oldColorDel; // wurde ein anderer Button entfärbt?

	ABrushButton* brushButtons[3];
	DrawBrush* akt_DrawBrush;
	int radius; // aktueller Radius
};

#endif