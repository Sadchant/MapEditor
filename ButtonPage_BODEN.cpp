#include "ButtonPage_BODEN.h"

CButtonPage_BODEN::CButtonPage_BODEN(CBrushmanager* brushmanager)
{
	this->brushManager = brushmanager;
	GenerateButtons();
	temp_Button = NULL;
	//Calculate_tile_count();
}


void CButtonPage_BODEN::Handle_Klick(ME_MOUSE mouse, int mouseX, int mouseY)
{
	vector<CBrushButton>::iterator it_boden_standard;
	// wenn STRG gedrückt wurde, wähle den Hintergrund aus
	if (mouse == ME_MOUSE_LEFT && KEY_DOWN(ME_KEY_STRG))
	{
		// boden auswählen
		for (it_boden_standard = boden_standard.begin(); it_boden_standard != boden_standard.end(); it_boden_standard++)
		{
			temp_Button = it_boden_standard->SelectBackground(mouseX, mouseY);
			if (temp_Button)
			{
				brushManager->SetGroundId(temp_Button->Get_DrawBrush()->id);
				return;
			}
		}
		return;
	}
	// je nach Button eine andere Farbe auswählen
	SDL_Color temp_color;
	switch (mouse)
	{
	case ME_MOUSE_LEFT:
		temp_color = SDL_RED
		break;
	case ME_MOUSE_MIDDLE:
		temp_color = SDL_YELLOW;
		break;
	case ME_MOUSE_RIGHT:
		temp_color = SDL_CYAN;
		break;
	}

	// versuchen, einen Button auszuwählen, wenn ja im Brush-Manager den Brush mappen
	for (it_boden_standard = boden_standard.begin(); it_boden_standard != boden_standard.end(); it_boden_standard++)
	{
		temp_Button = it_boden_standard->SelectTry(mouseX, mouseY, temp_color);
		if (temp_Button)
		{
			brushManager->Map_Mouse_Key(mouse, temp_Button);
			return;
		}
	}
}


void CButtonPage_BODEN::Render()
{
	for (it_boden_standard = boden_standard.begin(); it_boden_standard != boden_standard.end(); it_boden_standard++)
	{
		it_boden_standard->Render();
	}
}

// geht durch alle Texturfiles und liest die Anzahl der Tilevariationen aus
void CButtonPage_BODEN::Calculate_tile_count()
{
	CSprite tempSprite = CSprite(g_pLoader->getTexture("T_TEXTURSET_1"));
	tempSprite = CSprite(g_pLoader->getTexture("T_TEXTURSET_2"));
	
}

void CButtonPage_BODEN::GenerateButtons()
{
	SNext_row_data temp_data = Generate_boden_standard(y_start);
	temp_data = Generate_boden_wall(temp_data.akt_y + abstand, temp_data.last_id + 1);
}


// Erzeugt alle Buttons, mit denen man Boden malen kann
// gibt die y-Höhe zurück
SNext_row_data CButtonPage_BODEN::Generate_boden_standard(int start_y)
{
	akt_y = start_y;
	akt_x = x_start;

	// i repräsentiert die id
	int i;
	for (i = 0; i < NUMBODEN; i++)
	{
		// sollte die Buttonreihe aus dem linken Teil herauslaufen wird alles gerichtet
		if (akt_y + bor_ButtonWidth > MOVECAMX)
		{
			akt_y += abstand;
			akt_x = x_start;
		}
		buttonRect = { akt_x, akt_y, buttonWidth, buttonHeight };
		CBrushButton akt_Button = CBrushButton(i * 64, BODEN, buttonRect, g_pLoader->getTexture("T_TEXTURSET_1_MENU"), g_pLoader->getTexture("T_BUTTONBORDER"));
		boden_standard.push_back(akt_Button);
		akt_x += abstand;
	}
	return{ akt_y, i };
}

// Erzeugt alle Buttons, mit denen man Berge malen kann
// gibt die y-Höhe zurück
SNext_row_data CButtonPage_BODEN::Generate_boden_wall(int start_y, int index)
{
	akt_y = start_y;
	akt_x = x_start;

	int i;
	for (i = index;  i < NUMBODENWALL; i++)
	{
		// sollte die Buttonreihe aus dem linken Teil herauslaufen wird alles gerichtet
		if (akt_y + bor_ButtonWidth > MOVECAMX)
		{
			akt_y += abstand;
			akt_x = x_start;
		}
		buttonRect = { akt_x, akt_y, buttonWidth, buttonHeight };
		CBrushButton akt_Button = CBrushButton(i * 64, BODEN, buttonRect, g_pLoader->getTexture("T_TEXTURSET_WALL_1_MENU"), g_pLoader->getTexture("T_BUTTONBORDER"));
		boden_standard.push_back(akt_Button);
		akt_x += abstand;
	}
	return{ akt_y, i };
}