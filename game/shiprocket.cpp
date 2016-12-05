#include "pch/pch.h"

#include "game/shiprocket.h"
#include "game/aliensquadron.h"
#include "game/game.h"

const cShipRocketDef s_defaultShipRocket;

//----------------------------------------------------------------------------
cShipRocket::cShipRocket(const cCoord2D& initialPos)
	: cSprite(initialPos)
{
}

//----------------------------------------------------------------------------
void cShipRocket::update(float elapsed)
{
	cSprite::update(elapsed);

	m_coord.y -= def().speed * elapsed;

	// Destroy if going off the screen or colliding with the aliens
	if ((m_coord.y < 0) || cGame::get().getAlienSquadron()->collideRocket(*this))
	{
		setPendingDestroy();
	}
}
