#include "pch/pch.h"

#include "game/game.h"
#include "game/playership.h"
#include "game/shiprocket.h"
#include "DiceInvaders.h"

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

	const float currentTime = game.getInterface().getElapsedTime();
	if (keys.fire && ((currentTime - m_lastShot) > def().firePeriod))
	{
		game.getGameObjectManager().createGameObject<cShipRocket>(s_defaultShipRocket, cCoord2D(m_coord.x, m_coord.y - IDiceInvaders::SPRITE_SIZE));
		m_lastShot = currentTime;
	}
}
