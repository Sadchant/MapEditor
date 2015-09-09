#ifndef BUTTONPAGE_MECHANISMEN_H
#define BUTTONPAGE_MECHANISMEN_H

#include "ButtonPage.h"

class CButtonPage_MECHANISMEN : public CButtonPage
{
public:
	CButtonPage_MECHANISMEN(CBrushmanager* brushManager);
	void Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY);
	void Render();

private:
	void GenerateButtons();
	vector<CBrushButton> mechanismen;
};


#endif