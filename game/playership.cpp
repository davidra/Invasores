#include "pch/pch.h"

#include "game/game.h"
#include "game/playership.h"
#include "game/shiprocket.h"
#include "DiceInvaders.h"

namespace
{
	const float g_rapidFirePeriod = 0.1f;
}

const cPlayerShipDef s_defaultPlayerShipDef;

//----------------------------------------------------------------------------
cPlayerShip::cPlayerShip(float initialXPos) 
	: cSprite(cCoord2D(initialXPos, 0.0f))
{}

//----------------------------------------------------------------------------
void cPlayerShip::init(const IGameObjectDef& gameObjectDef)
{
	cSprite::init(gameObjectDef);

	m_coord.y = def().vertPos;
}

//----------------------------------------------------------------------------
void cPlayerShip::update(float elapsed)
{
	cSprite::update(elapsed);

	const float moveDelta = elapsed * def().speed;

	cGame& game = cGame::get();

	// Update position
	IDiceInvaders::KeyStatus keys;
	game.getInterface().getKeyStatus(keys);
	if (keys.right)
	{
		m_coord.x += moveDelta;
	}
	else if (keys.left)
	{
		m_coord.x -= moveDelta;
	}

	m_coord.x = clamp(0.0f, m_coord.x, static_cast<float>(game.getScreenWidth() - IDiceInvaders::SPRITE_SIZE));

	// Fire rockets if needed
	const bool rapidFireActive = m_rapidFireTimer > 0.0f;
	const float currentTime = game.getInterface().getElapsedTime();
	const float firePeriod = rapidFireActive ? g_rapidFirePeriod : def().firePeriod;
	if (keys.fire && ((currentTime - m_lastShot) > firePeriod))
	{
		game.getGameObjectManager().createGameObject<cShipRocket>(s_defaultShipRocket, cCoord2D(m_coord.x, m_coord.y - IDiceInvaders::SPRITE_SIZE));
		m_lastShot = currentTime;
	}

	// Update rapid fire
	if (rapidFireActive)
	{
		m_rapidFireTimer -= elapsed;
	}

	// Update invulnerability
	if (isInvulnerable())
	{
		m_invulnerableTimer -= elapsed;
		if (m_invulnerableTimer <= 0.0f)
		{
			blink(0);
		}
	}
}

//----------------------------------------------------------------------------
void cPlayerShip::onHit()
{
	onInvulnerable(def().invulnerableTimeAfterHit);
}

//----------------------------------------------------------------------------
void cPlayerShip::onInvulnerable(float timer) 
{ 
	constexpr unsigned blinkChangesPerSecond = 10;
	blink(blinkChangesPerSecond);

	m_invulnerableTimer = timer; 
}