#include "MapManager.h"

CMapManager::CMapManager(SDL_Rect* camera, CBrushmanager* brushManager, mutex* map_mutex) :
groundHandler(camera)
{
	this->camera = camera;
	camera_x = camera->x;
	camera_y = camera->y;
	this->map_mutex = map_mutex;
	this->brushManager = brushManager;
	groundHandler.Neue_map(29, 16);
	CalculateMapDimensions();
	g_pEventManager->InjectMapManager(this);
	aktKategorie = BODEN;
}


// Achtung, mouseX und mouseY sind relativ zum Map-Teil gesehen!
void CMapManager::Handle_Events()
{
	// wenn der Hintergrund geändert wurde, ändere ihn auch
	if (brushManager->hasGroundchanged())
		groundHandler.SetNewBackGround(brushManager->GetGroundId());

	// gedrückte Maustasten bearbeiten
	tempDrawBrush = NULL;
	if (g_pKeyMouseManager->MouseKeyDown(ME_MOUSE_LEFT))
		tempDrawBrush = brushManager->Get_DrawBrush(ME_MOUSE_LEFT);
	if (g_pKeyMouseManager->MouseKeyDown(ME_MOUSE_MIDDLE))
		tempDrawBrush = brushManager->Get_DrawBrush(ME_MOUSE_MIDDLE);
	if (g_pKeyMouseManager->MouseKeyDown(ME_MOUSE_RIGHT))
		tempDrawBrush = brushManager->Get_DrawBrush(ME_MOUSE_RIGHT);

	if (tempDrawBrush)
		Draw(g_pKeyMouseManager->GetMouse_x() - MOVECAMX, g_pKeyMouseManager->GetMouse_y());
}

void CMapManager::Render()
{
	groundHandler.Render();
	UpdateCamera();
	RenderBrushBorder();
}

// jeden Tick aufrufen!
void CMapManager::UpdateCamera()
{
	if ((KEY_DOWN(ssc_up) && KEY_DOWN(ssc_down)) ||
		(KEY_DOWN(ssc_left) && (KEY_DOWN(ssc_right)) ||
		(KEY_DOWN(ssc_up_fast) && KEY_DOWN(ssc_down_fast)) ||
		(KEY_DOWN(ssc_left_fast) && (KEY_DOWN(ssc_right_fast)))))
	{
		return;
	}

	if (KEY_DOWN(ssc_up) || KEY_DOWN(ssc_up_fast))
	{
		if (KEY_DOWN(ssc_left) || KEY_DOWN(ssc_left_fast))
			fDirection = 0.625f;
		else if (KEY_DOWN(ssc_right) || KEY_DOWN(ssc_right_fast))
			fDirection = 0.375f;
		else fDirection = 0.5f;
	}	
	else if ((KEY_DOWN(ssc_down)) || (KEY_DOWN(ssc_down_fast)))
	{
		if (KEY_DOWN(ssc_left) || KEY_DOWN(ssc_left_fast))
			fDirection = 0.875f;
		else if (KEY_DOWN(ssc_right) || KEY_DOWN(ssc_right_fast))
			fDirection = 0.125f;
		else fDirection = 0.0f;
	}	
	else if (KEY_DOWN(ssc_left) || KEY_DOWN(ssc_left_fast))
		fDirection = 0.75f;
	else if (KEY_DOWN(ssc_right) || KEY_DOWN(ssc_right_fast))
		fDirection = 0.25f;

	if (KEY_DOWN(ssc_up) || KEY_DOWN(ssc_down) || KEY_DOWN(ssc_left) || KEY_DOWN(ssc_right))
	{
		camera_x += speed_normal * sin(fDirection * 2 * M_PI) * g_pTimer->GetElapsed();
		camera_y += speed_normal * cos(fDirection * 2 * M_PI) * g_pTimer->GetElapsed();
	}
	if (KEY_DOWN(ssc_up_fast) || KEY_DOWN(ssc_down_fast) || KEY_DOWN(ssc_left_fast) || KEY_DOWN(ssc_right_fast))
	{
		camera_x += speed_fast * sin(fDirection * 2 * M_PI) * g_pTimer->GetElapsed();
		camera_y += speed_fast * cos(fDirection * 2 * M_PI) * g_pTimer->GetElapsed();
	}

	// Camera korrigieren

	if (camera_x < 0)
		camera_x = 0;
	if (camera_y < 0)
		camera_y = 0;

	if (camera_x > map_width - camera->w)
		camera_x = map_width - camera->w;
	if (camera_y > map_height - camera->h)
		camera_y = map_height - camera->h;

	camera->x = static_cast<int>(camera_x);
	camera->y = static_cast<int>(camera_y);
}

void CMapManager::CalculateMapDimensions()
{
	map_width = groundHandler.Get_map_width();
	map_height = groundHandler.Get_map_height();
}

// jeden Tick aufrufen!
void CMapManager::RenderBrushBorder()
{
	aktmousePos.x = g_pKeyMouseManager->GetMouse_x() - MOVECAMX;
	aktmousePos.y = g_pKeyMouseManager->GetMouse_y();
	
	if (aktmousePos.x < 0)
		return;
	switch (aktKategorie)
	{
	case BODEN:
	{
		groundHandler.RenderBorder(aktmousePos.x, aktmousePos.y, brushManager->Get_Radius());
	}
	}
}

void CMapManager::ChangeBrushBorder(int value)
{
	tempRadius = brushManager->Get_Radius();

	tempRadius += value * 2;

	if (tempRadius < 1)
		tempRadius = 1;
	if (tempRadius > 15)
		tempRadius = 15;
	brushManager->Set_Radius(tempRadius);
}

// bekommt mouse_x und mouse_y relativ zum Feld, wo gemalt werden kann
void CMapManager::Draw(int mouseX, int mouseY)
{
	if (mouseX < 0 || mouseY < 0 || mouseX > 1280 || mouseY >720)
		return;
	switch (tempDrawBrush->kategorie)
	{
	case BODEN:
	{
				  groundHandler.Draw(mouseX, mouseY, tempDrawBrush->id, brushManager->Get_Radius());

	}break;
	case OBJEKTE:
	{

	}break;
	case MONSTER:
	{

	}break;
	case MECHANISMEN:
	{

	}break;
}
}


void CMapManager::Neue_Map(int width, int height)
{
	map_mutex->lock();
	groundHandler.Neue_map(width, height);
	map_mutex->unlock();
	CalculateMapDimensions();
}


Point CMapManager::Map_oeffnen(string path)
{
	map_mutex->lock();
	Point tempDimensions = groundHandler.Load_map(path);
	map_mutex->unlock();
	CalculateMapDimensions();
	return tempDimensions;
}


void CMapManager::Map_speichern(string path)
{
	groundHandler.Save_map(path);
}


void CMapManager::Groesse_aendern(int width, int height)
{
	map_mutex->lock();
	groundHandler.Change_groesse(width, height, brushManager->GetGroundId());
	map_mutex->unlock();
	CalculateMapDimensions();
}

void CMapManager::Rueckgaengig()
{

}


void CMapManager::Wiederholen()
{

}
