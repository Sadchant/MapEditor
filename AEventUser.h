#pragma once

#include "KeyMouseManager.h"

// Schnittstelle für Objekte, die auf Events reagieren sollen
class AEventUser
{
public:
	virtual ~AEventUser(){};
	virtual void HandleWheelEvent(int value, int mouse_x, int mouse_y) = 0;
	virtual void HandleKlickEvent(ME_MOUSE mouse_button, int mouse_x, int mouse_y) = 0;
};

