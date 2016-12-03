#include "pch/pch.h"

#include "game/shiprocket.h"
#include "game/game.h"

const cShipRocketDef s_defaultShipRocket;

//----------------------------------------------------------------------------
cShipRocket::cShipRocket(float hrzPos, float vrtPos)
	: cSprite(hrzPos, vrtPos)
{
}

//----------------------------------------------------------------------------
void cShipRocket::update(float elapsed)
{
	cSprite::update(elapsed);

	m_vrtPos -= def().speed * elapsed;

	if (m_vrtPos < 0)
	{
		setPendingDestroy();
	}

	// Collide with the aliens
}
