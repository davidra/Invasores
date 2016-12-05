#include "pch/pch.h"

#include "game/powerup.h"
#include "game/game.h"
#include "game/playership.h"

const cPowerUpDef s_lifeUpPowerUp;
const cPowerUpDef s_rapidFirePowerUp(ePowerUpType::RAPID_FIRE, "|R|");
const cPowerUpDef s_invulnerabilityPowerUp(ePowerUpType::INVULNERABILITY, "|¤|");

//----------------------------------------------------------------------------
void cPowerUp::update(float elapsed)
{
	m_coord.y += def().speed * elapsed;

	// Collide with player
	const cPlayerShip* const playerShip = cGame::get().getPlayerShip();
	const cCoord2D& playerPos = playerShip->getPos();

	const cAABB2D damageBox(m_coord + def().hitBox);
	const cAABB2D hitBox(playerPos + playerShip->def().hitBox);
	if (overlap(damageBox, hitBox))
	{
		cGame::get().onPowerUp(def().type);
		setPendingDestroy();
	}

	if (m_coord.y > cGame::get().getScreenHeight())
	{
		setPendingDestroy();
	}
}

//----------------------------------------------------------------------------
void cPowerUp::draw(float elapsed)
{
	cGame::get().getTextPrinter().AddText(m_coord + def().hitBox.m_min, def().visual);
}
