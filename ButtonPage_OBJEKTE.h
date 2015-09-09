#ifndef BUTTONPAGE_OBJEKTE_H
#define BUTTONPAGE_OBJEKTE_H

#include "ButtonPage.h"

class CButtonPage_OBJEKTE : public CButtonPage
{
public:
	CButtonPage_OBJEKTE(CBrushmanager* brushManager);
	void Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY);
	void Render();

private:
	void GenerateButtons();
	vector<CBrushButton> objekte;
};


#endif