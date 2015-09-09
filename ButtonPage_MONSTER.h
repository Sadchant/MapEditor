#ifndef BUTTONPAGE_MONSTER_H
#define BUTTONPAGE_MONSTER_H

#include "ButtonPage.h"

class CButtonPage_MONSTER : public CButtonPage
{
public:
	CButtonPage_MONSTER(CBrushmanager* brushManager);
	void Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY);
	void Render();

private:
	void GenerateButtons();
	vector<CBrushButton> monster;
};


#endif