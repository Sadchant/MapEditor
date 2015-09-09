#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include "Sprite.h"
#include <vector>

class CSpriteObject
{
public:

	CSpriteObject(CTexture* texture, int groesse);
	~CSpriteObject();
	void		SetPos(int stelle, int x, int y); // setzt Position des SpriteObjekts an stelle
	void		SetPos(int stelle, float x, float y);
	void		Render(int stelle);
	void		Render(int stelle, int frameNumber); //muss nach SetPos aufgerufen werden
	void		Render(int stelle, int frameNumber, int direction);
	Point		GetTextureDimensions(){ return textureDimensions; }

private:
	vector<CSprite*> sprites;
	int groesse;
	Point textureDimensions;
};












#endif
