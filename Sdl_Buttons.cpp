#include "Sdl_Buttons.h"


CSDL_Buttons::CSDL_Buttons(CBrushmanager* brushManager) :
button_background(g_pLoader->getTexture("T_MAPEDITOR_HINTERGRUND")),
tabs(g_pLoader->getTexture("T_TABS"))
{
	this->brushManager = brushManager;

	buttonPages[0] = new CButtonPage_BODEN(brushManager);
	buttonPages[1] = new CButtonPage_OBJEKTE(brushManager);
	buttonPages[2] = new CButtonPage_MONSTER(brushManager);
	buttonPages[3] = new CButtonPage_MECHANISMEN(brushManager);
	aktButtonPage = buttonPages[0];
}

CSDL_Buttons::~CSDL_Buttons()
{
	for (CButtonPage* i : buttonPages)
	{
		SAFE_DELETE(i);
	}
}

void CSDL_Buttons::HandleKlickEvent(ME_MOUSE mouse, int mouseX, int mouseY)
{
	if (mouseX > MOVECAMX)
		return;
		
	if (mouseY < TABSHEIGHT)
	{
		if (mouse == ME_MOUSE_LEFT)
			SelectKategorie(mouseX);
		return;
	}
	aktButtonPage->Handle_Klick(mouse, mouseX, mouseY);
}

// rendert den Hintergrund und alle Buttons, je nach Kategorie
void CSDL_Buttons::Render()
{
	button_background.Render();
	tabs.Render(0, aktKategorie);
	aktButtonPage->Render();
}


// wenn sich die Maus-x-Position über dem Button befindet wird er ausgewählt
void CSDL_Buttons::SelectKategorie(int mouseX)
{
	if ((mouseX >= 0) && (mouseX <= 156))
	{
		aktButtonPage = buttonPages[0];
		aktKategorie = BODEN;
		return;
	}
	if ((mouseX > 156) && (mouseX <= 312))
	{
		aktButtonPage = buttonPages[1];
		aktKategorie = OBJEKTE;
		return;
	}
	if ((mouseX > 312) && (mouseX <= 468))
	{
		aktButtonPage = buttonPages[2];
		aktKategorie = MONSTER;
		return;
	}
	if ((mouseX > 468) && (mouseX <= MOVECAMX))
	{
		aktButtonPage = buttonPages[3];
		aktKategorie = MECHANISMEN;
		return;
	}
}




