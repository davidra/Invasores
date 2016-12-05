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
	cAlienBombDef(float speed = 160.0f, const cAABB2D& hitBox = { {12.0f, 8.0f}, {20.0f, 24.0f} }, const char* spriteName = "data/bomb.bmp")
		: cSpriteDef(spriteName)
		, speed(speed)
		, hitBox(hitBox)
	{}

	float	speed;
	cAABB2D hitBox;
};

//----------------------------------------------------------------------------
class cAlienBomb : public cSprite
{
	REGISTER_GAMEOBJECT(cAlienBomb);

	using cSprite::cSprite;

public:
	void update(float elapsed) override;
};

extern const cAlienBombDef s_defaultAlienBomb;