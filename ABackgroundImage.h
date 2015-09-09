#pragma once

#include <string>

using namespace std;

class ABackgroundImage
{
public:
	virtual ~ABackgroundImage(){};
	virtual void Bild_oeffnen(string path, int map_width, int map_height) = 0;
	virtual void Anzeigen(bool anzeigen) = 0;
	virtual void Seitenverhaeltnis(bool seitenverhaeltnis, int map_width, int map_height) = 0;
	virtual void SetGroesse(int map_width, int map_height) = 0;
};