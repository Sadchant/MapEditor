#ifndef BUTTONEVENTS_H
#define BUTTONEVENTS_H

#include "Singleton.h"
#include <string>

#define g_pButtonEvents CButtonEvents::Get ()

enum wndButtons
{
	neue_Map,
	map_oeffnen,
	map_speichern,
	groesse_edit,
	bild_oeffnen
};

enum mapGroesse
{
	xGroesse,
	yGroesse
};

enum bildDaten
{
	seitenverhaeltnis,
	anzeigen
};

class CButtonEvents : public TSingleton<CButtonEvents>
{
public:
	void PressButton(wndButtons button){ buttons[button] = true; }
	bool ButtonPressed(wndButtons button) { return buttons[button];  }

	void SetSavePath(string new_path){ savePath = new_path; }
	string GetSavePath() { return savePath; }

	void SetBildPath(string new_path){ bildPath = new_path; }
	string GetBildPath() { return bildPath; }

	void SetRootPath(string rootpath){ rootPath = rootpath; }
	string GetRootPath() { return rootPath; }

	void SetSaved(bool new_status){ saved = new_status; }
	bool IsSaved(){ return saved; }

	int GetGroesse(mapGroesse index) { return groesse[index]; }
	void SetGroesse(mapGroesse index, int wert) { groesse[index] = wert; }

	void SetBildData(bildDaten index, bool status){ bild[index] = status; }
	bool GetBildData(bildDaten index){ return bild[index]; }

	void ClearButtonStates()
	{
		for (int i = 0; i < NUMBUTTONS; i++)
		{
			buttons[i] = false;
		}
	}

private:
	static const int NUMBUTTONS = 5;
	// repräsentieren die Menu-Buttons und "Hintergrundbild öffnen"
	bool buttons[NUMBUTTONS];
	string savePath;
	string bildPath;
	string rootPath;
	bool saved = true;
	int groesse[2];	
	bool bild[2];

};

#endif