#pragma once
/***************************************************************************************************
shiprocket.h

Rocket class. Fired by the ship's ship. Moves up until exiting the screen or hitting an alien
 
by David Ramos
***************************************************************************************************/
#include "game/sprite.h"

//----------------------------------------------------------------------------
class cShipRocketDef : public cSpriteDef
{
public:
	cShipRocketDef(float speed = 320.0f, const char* spriteName = "data/rocket.bmp")
		: cSpriteDef(spriteName)
		, speed(speed)
	{}

	float speed;
};

//----------------------------------------------------------------------------
class cShipRocket : public cSprite
{
	REGISTER_GAMEOBJECT(cShipRocket);

public:
	cShipRocket(float hrzPos, float vrtPos);

	void update(float elapsed) override;
};

extern const cShipRocketDef s_defaultShipRocket;