#pragma once
/***************************************************************************************************
playership.h

Player's ship class
 
by David Ramos
***************************************************************************************************/
#include "game/gameobject.h"
#include "game/sprite.h"

//----------------------------------------------------------------------------
class cPlayerShipDef : public cSpriteDef
{
public:
	cPlayerShipDef(float vertPos = 480 - 32, float speed = 160.0f, float firePeriod = 0.5f, const char* spriteName = "data/player.bmp")
		: cSpriteDef(spriteName)
		, vertPos(vertPos)
		, speed(speed)
		, firePeriod(firePeriod)
	{}

	float vertPos;
	float speed;
	float firePeriod; // period of the rate of fire, in seconds
};

//----------------------------------------------------------------------------
class cPlayerShip : public cSprite
{
	REGISTER_GAMEOBJECT(cPlayerShip);

public:
	cPlayerShip(int initialXPos);

	void init(const IGameObjectDef& gameObjectDef) override;
	void update(float elapsed) override;

private:
	float		m_lastShot	= 0.0f;
};

extern const cPlayerShipDef s_defaultPlayerShipDef;