#pragma once
/***************************************************************************************************
alienbomb.h

Alien bomb game object class
 
by David Ramos
***************************************************************************************************/
#include "game/sprite.h"

//----------------------------------------------------------------------------
class cAlienBombDef : public cSpriteDef
{
public:
	cAlienBombDef(float speed = 160.0f, const char* spriteName = "data/bomb.bmp")
		: cSpriteDef(spriteName)
		, speed(speed)
	{}

private:
	float speed;
};

//----------------------------------------------------------------------------
class cAlienBomb : public cSprite
{
public:
	void update(float elapsed) override;
private:
};