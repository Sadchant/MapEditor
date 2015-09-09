#pragma once

#include "AProgress.h"

class CDrawProgress : public AProgress
{
public:
	CDrawProgress();
	~CDrawProgress();
	void Do();
	void Undo();
};

