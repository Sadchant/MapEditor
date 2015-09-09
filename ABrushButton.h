#pragma once

enum Kategorie
{
	BODEN,
	OBJEKTE,
	MONSTER,
	MECHANISMEN
};

struct DrawBrush
{
	Kategorie kategorie;
	int id;
};

class ABrushButton
{
public:
	virtual ~ABrushButton(){};
	virtual void DeSelect() = 0;
	virtual DrawBrush* Get_DrawBrush() = 0;
};