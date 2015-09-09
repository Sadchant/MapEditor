#ifndef SDL_PART_H
#define SDL_PART_H


#include "Util.h"
#include "Framework.h"
#include "Debugscreen.h"
#include "MapManager.h"
#include "Sdl_Buttons.h"
#include "BackgroundImage.h"
#include "AEventUser.h"
#include "EventManager.h"
#include <mutex>


class CSDLPart : public AEventUser
{
public:
	CSDLPart();
	void HandleSDL();
	void HandleWheelEvent(int value, int mouse_x, int mouse_y);
	void HandleKlickEvent(ME_MOUSE mouse_button, int mouse_x, int mouse_y);

private:
	void Render();
	void HandleEvents();
	void FpsCounter();

	SDL_Rect		camera;

	CMapManager mapManager;
	CBrushmanager brushmanager;
	CSDL_Buttons sdl_buttons;
	CBackgroundImage backgroundImage;

	bool temp;

	int				framecounter;
	float			timecounter;

	SDL_Event aktEvent;
	Uint32 aktMousebutton;
	int aktMouse_X;
	int aktMouse_Y;

	mutex map_mutex;
};

#endif