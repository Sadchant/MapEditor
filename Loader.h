#ifndef LOADER_H
#define LOADER_H

#include "Framework.h"
#include "Singleton.h"
#include "Texture.h"
#include "Util.h"
#include <map>
#include <fstream>
#include <string>

#define g_pLoader CLoader::Get()

class CLoader : public TSingleton<CLoader>
{
public:
	CLoader();
	~CLoader();
	CTexture* getTexture(string id) { return textures[id].texture; }

	void load(string id);											// erzeugt f�r id eine CTexture-Instanz

	void loadAll();													// erzeugt f�r alle id's eine CTexture-Instanz
	void reloadTextures();											// L�d Texturen neu (bei �nderungen im Renderer)

private:
	struct TextureWrapper
	{
		string filename;
		Renderlayers renderlayer;
		int frameWidth;
		int frameHeight;
		CTexture* texture;
	};
	map<string, TextureWrapper> textures;
	static void splitstr(string line, string* result);				// Zerteilt Line bei jedem ' ' und schreibt die St�cke in result
	static Renderlayers decodeRenderlayer(string rl);				// Gibt zu einem String das passende Renderlayer-Enum zur�ck (Textlayer bei keinem match)
};

#endif