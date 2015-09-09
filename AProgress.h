#pragma once

class AProgress
{
public:
	virtual ~AProgress(){};
	virtual void Do() = 0;
	virtual void Undo() = 0;
};