#pragma once

#include <string>
#include "Framework.h"

using namespace std;

class AMapManager
{
public:
	virtual ~AMapManager(){};
	virtual void Neue_Map(int width, int height) = 0;
	virtual Point Map_oeffnen(string path) = 0;
	virtual void Map_speichern(string path) = 0;
	virtual void Groesse_aendern(int width, int height) = 0;
	virtual void Rueckgaengig() = 0;
	virtual void Wiederholen() = 0;
};