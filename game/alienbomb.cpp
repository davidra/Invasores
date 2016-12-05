#include "pch/pch.h"

#include "game/game.h"
#include "game/alienbomb.h"
#include "game/playership.h"
#include "DiceInvaders.h"

const cAlienBombDef s_defaultAlienBomb;

//----------------------------------------------------------------------------
void cAlienBomb::update(float elapsed)
{
	cSprite::update(elapsed);

	m_coord.y += def().speed * elapsed;

	const cPlayerShip* const playerShip = cGame::get().getPlayerShip();
	if (!playerShip->isInvulnerable())
	{
		const cCoord2D& playerPos = playerShip->getPos();

		const cAABB2D damageBox(getPos() + def().hitBox);
		const cAABB2D hitBox(playerPos + playerShip->def().hitBox);
		if (overlap(damageBox, hitBox))
		{
			cGame::get().onPlayerHit();
			setPendingDestroy();
		}
	}
}