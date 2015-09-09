#pragma once
#include "AProgress.h"

class CGroundProgress :
	public AProgress
{
public:
	CGroundProgress();
	~CGroundProgress();
	void Do();
	void Undo();
};

