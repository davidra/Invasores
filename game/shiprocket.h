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
	cShipRocketDef(float speed = 400.0f, const cAABB2D& damageBox = { {12.0f, 7.0f}, {19.0f, 25.0f} }, const char* spriteName = "data/rocket.bmp")
		: cSpriteDef(spriteName)
		, speed(speed)
		, damageBox(damageBox)
	{}

	float	speed;
	cAABB2D	damageBox;	// bounding box with sprite-local coordinates that defines the region that can collide with aliens
};

//----------------------------------------------------------------------------
class cShipRocket : public cSprite
{
	REGISTER_GAMEOBJECT(cShipRocket);

public:
	cShipRocket(const cCoord2D& initialPos);

	void update(float elapsed) override;
};

extern const cShipRocketDef s_defaultShipRocket;