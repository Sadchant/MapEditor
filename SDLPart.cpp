#include "SDLPart.h"

CSDLPart::CSDLPart():
camera({0,0,1280,720}),
mapManager(&camera, &brushmanager, &map_mutex),
brushmanager(),
sdl_buttons(&brushmanager),
backgroundImage(&camera)
{
	g_pEventManager->InjectEventUser(this);
}

void CSDLPart::HandleSDL()
{
	Render();
	FpsCounter();
	HandleEvents();
}
void CSDLPart::HandleWheelEvent(int value, int mouse_x, int mouse_y)
{
	if (mouse_x > MOVECAMX && mouse_y > 0)
		mapManager.ChangeBrushBorder(value);
}


void CSDLPart::HandleKlickEvent(ME_MOUSE mouse_button, int mouse_x, int mouse_y)
{
	if (mouse_x < MOVECAMX && mouse_y > 0)
		sdl_buttons.HandleKlickEvent(mouse_button, mouse_x, mouse_y);
}


void CSDLPart::Render()
{	
	sdl_buttons.Render();
	backgroundImage.Render();

	map_mutex.lock();
	mapManager.Render();
	g_pRenderlayer->Render();
	map_mutex.unlock();

	g_pFramework->Update();
	g_pDebugscreen->Render();
	
}

void CSDLPart::FpsCounter()
{
	framecounter++;
	timecounter += g_pTimer->GetElapsed();

	if (timecounter >= 1.0f)
	{
		g_pDebugscreen->FPS(framecounter);
		framecounter = 0;
		timecounter--;
	}
}

void CSDLPart::HandleEvents()
{
	backgroundImage.Update();
	mapManager.Handle_Events();
}
