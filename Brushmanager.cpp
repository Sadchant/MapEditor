#include "Brushmanager.h"

CBrushmanager::CBrushmanager()
{
	// setzt die aktuellen Brushs aufs Ende, damit sie einen nicht verwendbaren Wert verwenden
	groundID = 0;
	oldColorDel = false;
	radius = 1;
	brushButtons[0] = NULL;
	brushButtons[1] = NULL;
	brushButtons[2] = NULL;
}

void CBrushmanager::SetGroundId(int id)
{
	groundChanged = true;
	groundID = id; 
}

int CBrushmanager::GetGroundId()
{
	groundChanged = false;
	return groundID; 
}

void CBrushmanager::Map_Mouse_Key(ME_MOUSE mouse, ABrushButton* brushButton)
{
	if (brushButtons[mouse])
		brushButtons[mouse]->DeSelect();
	brushButtons[mouse] = brushButton;
}

DrawBrush* CBrushmanager::Get_DrawBrush(ME_MOUSE mouse)
{
	if (brushButtons[mouse])
	{
		akt_DrawBrush = brushButtons[mouse]->Get_DrawBrush();
		return brushButtons[mouse]->Get_DrawBrush();
	}
		
	return NULL;
}